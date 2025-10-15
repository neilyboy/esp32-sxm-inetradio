#include <Arduino.h>
#include <TFT_eSPI.h>
#include "config.h"
#include "settings.h"
#include "wifi_manager.h"
#include "sxm_client.h"
#include "fm_transmitter.h"
#include "audio_player.h"
#include "ui_manager.h"

// Global objects
TFT_eSPI tft = TFT_eSPI();
Settings settings;
WiFiMgr wifiManager;
SXMClient sxmClient;
FMTransmitter fmTransmitter;
AudioPlayer audioPlayer;
UIManager* uiManager;

// State variables
enum AppState {
    STATE_INIT,
    STATE_WIFI_SETUP,
    STATE_SXM_SETUP,
    STATE_FM_SETUP,
    STATE_MAIN,
    STATE_CHANNEL_SELECT,
    STATE_SETTINGS
};

AppState currentState = STATE_INIT;
int selectedNetwork = 0;
int selectedChannel = 0;
int channelOffset = 0;
String inputBuffer = "";
bool inputEmailField = true;
float currentFMFreq = FM_DEFAULT_FREQ;

std::vector<WiFiNetwork> wifiNetworks;
std::vector<SXMChannel> sxmChannels;

// Forward declarations
void handleWiFiSetup();
void handleSXMSetup();
void handleFMSetup();
void handleMainScreen();
void handleChannelSelect();
void handleSettings();
void setupComplete();

void setup() {
    Serial.begin(115200);
    Serial.println("\n\n=== ESP32 SiriusXM IntraRadio ===");
    
    // Initialize display
    tft.init();
    tft.setRotation(1);
    uiManager = new UIManager(&tft);
    uiManager->begin();
    uiManager->drawSplash();
    
    delay(2000);
    
    // Initialize settings
    if (!settings.begin()) {
        Serial.println("Failed to initialize settings");
        uiManager->showMessage("Error", "Settings init failed", 3000);
        return;
    }
    
    // Initialize FM transmitter
    uiManager->drawLoading("Init FM...");
    if (!fmTransmitter.begin()) {
        Serial.println("Warning: FM transmitter not found");
        uiManager->showMessage("Warning", "FM TX not found", 2000);
    } else {
        currentFMFreq = settings.getFMFrequency();
        fmTransmitter.setFrequency(currentFMFreq);
    }
    
    // Initialize audio player
    uiManager->drawLoading("Init Audio...");
    if (!audioPlayer.begin()) {
        Serial.println("Failed to initialize audio");
        uiManager->showMessage("Error", "Audio init failed", 3000);
        return;
    }
    
    // Link audio player to FM transmitter for RDS metadata
    audioPlayer.setFMTransmitter(&fmTransmitter);
    
    // Check if first run
    if (settings.isFirstRun()) {
        Serial.println("First run - entering setup mode");
        currentState = STATE_WIFI_SETUP;
    } else {
        // Try to connect to saved WiFi
        uiManager->drawLoading("Connecting WiFi...");
        String ssid = settings.getWiFiSSID();
        String password = settings.getWiFiPassword();
        
        if (wifiManager.connect(ssid, password)) {
            Serial.println("Connected to WiFi");
            
            // Login to SXM
            uiManager->drawLoading("Login to SXM...");
            
            // Set SXM server if configured
            if (settings.hasSXMServer()) {
                sxmClient.setSXMServer(settings.getSXMServer());
            }
            
            String email = settings.getSXMEmail();
            String sxmPass = settings.getSXMPassword();
            
            if (sxmClient.login(email, sxmPass)) {
                sxmClient.fetchChannelList();
                sxmChannels = sxmClient.getChannels();
                
                // Load last channel
                int lastChannel = settings.getLastChannel();
                if (lastChannel > 0 && lastChannel <= sxmChannels.size()) {
                    selectedChannel = lastChannel - 1;
                }
                
                currentState = STATE_MAIN;
            } else {
                uiManager->showMessage("Error", "SXM login failed", 3000);
                currentState = STATE_SXM_SETUP;
            }
        } else {
            uiManager->showMessage("Error", "WiFi failed", 3000);
            currentState = STATE_WIFI_SETUP;
        }
    }
}

void loop() {
    // Update audio player
    audioPlayer.loop();
    
    // Update RDS data transmission (call periodically to keep RDS active)
    static unsigned long lastRDSUpdate = 0;
    if (millis() - lastRDSUpdate > 100) {  // Update every 100ms
        fmTransmitter.updateRDS();
        lastRDSUpdate = millis();
    }
    
    // Handle touch input
    uint16_t touchX, touchY;
    bool touched = uiManager->checkTouch(touchX, touchY);
    
    // State machine
    switch (currentState) {
        case STATE_WIFI_SETUP:
            handleWiFiSetup();
            break;
            
        case STATE_SXM_SETUP:
            handleSXMSetup();
            break;
            
        case STATE_FM_SETUP:
            handleFMSetup();
            break;
            
        case STATE_MAIN:
            handleMainScreen();
            break;
            
        case STATE_CHANNEL_SELECT:
            handleChannelSelect();
            break;
            
        case STATE_SETTINGS:
            handleSettings();
            break;
    }
    
    delay(50);
}

void handleWiFiSetup() {
    static bool networksScanned = false;
    static unsigned long lastTouch = 0;
    
    if (!networksScanned) {
        uiManager->drawLoading("Scanning WiFi...");
        wifiNetworks = wifiManager.scanNetworks();
        networksScanned = true;
        
        if (wifiNetworks.size() == 0) {
            uiManager->showMessage("Error", "No networks found", 3000);
            delay(2000);
            networksScanned = false;
            return;
        }
        
        // Draw network list
        std::vector<String> networkNames;
        for (const auto& net : wifiNetworks) {
            networkNames.push_back(net.ssid);
        }
        uiManager->setScreen(SCREEN_WIFI_SCAN);
        uiManager->drawWiFiScan(networkNames, selectedNetwork);
    }
    
    uint16_t x, y;
    if (uiManager->checkTouch(x, y)) {
        if (millis() - lastTouch < 300) return; // Debounce
        lastTouch = millis();
        
        // Check if a network was touched
        int itemHeight = 35;
        int touchedItem = (y - 40) / itemHeight;
        
        if (touchedItem >= 0 && touchedItem < wifiNetworks.size()) {
            selectedNetwork = touchedItem;
            
            // Enter password
            String ssid = wifiNetworks[selectedNetwork].ssid;
            inputBuffer = "";
            
            while (true) {
                uiManager->setScreen(SCREEN_WIFI_PASSWORD);
                uiManager->drawPasswordInput(ssid, inputBuffer);
                
                if (uiManager->checkTouch(x, y)) {
                    delay(200); // Debounce
                    
                    // Check keyboard
                    char key = uiManager->getKeyboardPress(x, y, false);
                    if (key != '\0') {
                        if (key == '\b') {
                            if (inputBuffer.length() > 0) {
                                inputBuffer.remove(inputBuffer.length() - 1);
                            }
                        } else {
                            inputBuffer += key;
                        }
                        continue;
                    }
                    
                    // Check connect button
                    if (x >= SCREEN_WIDTH - 90 && x <= SCREEN_WIDTH - 10 && y >= 95 && y <= 125) {
                        // Connect
                        uiManager->drawLoading("Connecting...");
                        
                        if (wifiManager.connect(ssid, inputBuffer)) {
                            settings.setWiFiCredentials(ssid, inputBuffer);
                            uiManager->showMessage("Success", "WiFi connected!", 2000);
                            currentState = STATE_SXM_SETUP;
                            return;
                        } else {
                            uiManager->showMessage("Error", "Connection failed", 3000);
                            break;
                        }
                    }
                }
                delay(50);
            }
        }
    }
}

void handleSXMSetup() {
    static String email = "";
    static String password = "";
    
    uiManager->setScreen(SCREEN_SXM_LOGIN);
    uiManager->drawSXMLogin(email, password, inputEmailField);
    
    uint16_t x, y;
    if (uiManager->checkTouch(x, y)) {
        delay(200); // Debounce
        
        // Check which field was touched
        if (y >= 50 && y <= 80) {
            inputEmailField = true;
        } else if (y >= 95 && y <= 125) {
            inputEmailField = false;
        }
        
        // Check keyboard
        char key = uiManager->getKeyboardPress(x, y, false);
        if (key != '\0') {
            if (key == '\b') {
                if (inputEmailField && email.length() > 0) {
                    email.remove(email.length() - 1);
                } else if (!inputEmailField && password.length() > 0) {
                    password.remove(password.length() - 1);
                }
            } else {
                if (inputEmailField) {
                    email += key;
                } else {
                    password += key;
                }
            }
        }
        
        // Check login button
        if (x >= SCREEN_WIDTH / 2 - 40 && x <= SCREEN_WIDTH / 2 + 40 && y >= 135 && y <= 165) {
            uiManager->drawLoading("Logging in...");
            
            // Set SXM server if configured
            if (settings.hasSXMServer()) {
                sxmClient.setSXMServer(settings.getSXMServer());
            }
            
            if (sxmClient.login(email, password)) {
                settings.setSXMCredentials(email, password);
                sxmClient.fetchChannelList();
                sxmChannels = sxmClient.getChannels();
                uiManager->showMessage("Success", "Logged in!", 2000);
                currentState = STATE_FM_SETUP;
            } else {
                uiManager->showMessage("Error", sxmClient.getLastError(), 3000);
            }
        }
    }
}

void handleFMSetup() {
    uiManager->setScreen(SCREEN_FM_CONFIG);
    uiManager->drawFMConfig(currentFMFreq);
    
    uint16_t x, y;
    if (uiManager->checkTouch(x, y)) {
        delay(200); // Debounce
        
        // Check - button
        if (x >= 60 && x <= 120 && y >= SCREEN_HEIGHT / 2 + 20 && y <= SCREEN_HEIGHT / 2 + 60) {
            currentFMFreq -= 0.2;
            if (currentFMFreq < FM_MIN_FREQ) {
                currentFMFreq = FM_MIN_FREQ;
            }
            fmTransmitter.setFrequency(currentFMFreq);
        }
        
        // Check + button
        if (x >= SCREEN_WIDTH - 120 && x <= SCREEN_WIDTH - 60 && y >= SCREEN_HEIGHT / 2 + 20 && y <= SCREEN_HEIGHT / 2 + 60) {
            currentFMFreq += 0.2;
            if (currentFMFreq > FM_MAX_FREQ) {
                currentFMFreq = FM_MAX_FREQ;
            }
            fmTransmitter.setFrequency(currentFMFreq);
        }
        
        // Check save button
        if (x >= SCREEN_WIDTH / 2 - 40 && x <= SCREEN_WIDTH / 2 + 40 && y >= SCREEN_HEIGHT - 50 && y <= SCREEN_HEIGHT - 15) {
            settings.setFMFrequency(currentFMFreq);
            settings.setFirstRunComplete();
            uiManager->showMessage("Success", "Setup complete!", 2000);
            currentState = STATE_MAIN;
        }
    }
}

void handleMainScreen() {
    static bool screenDrawn = false;
    
    if (!screenDrawn) {
        String channelName = sxmChannels.size() > 0 ? sxmChannels[selectedChannel].name : "No channels";
        uiManager->setScreen(SCREEN_MAIN);
        uiManager->drawMainScreen(channelName, "");
        screenDrawn = true;
    }
    
    uint16_t x, y;
    if (uiManager->checkTouch(x, y)) {
        delay(200); // Debounce
        
        // Check SXM logo (channel select)
        if (x >= 10 && x <= 150 && y >= 10 && y <= 90) {
            currentState = STATE_CHANNEL_SELECT;
            screenDrawn = false;
        }
        
        // Check settings button
        if (x >= 160 && x <= 300 && y >= 100 && y <= 160) {
            currentState = STATE_SETTINGS;
            screenDrawn = false;
        }
    }
}

void handleChannelSelect() {
    static bool screenDrawn = false;
    
    if (!screenDrawn) {
        std::vector<String> channelNames;
        for (const auto& ch : sxmChannels) {
            channelNames.push_back(ch.number + " - " + ch.name);
        }
        
        uiManager->setScreen(SCREEN_CHANNEL_LIST);
        uiManager->drawChannelList(channelNames, selectedChannel, channelOffset);
        screenDrawn = true;
    }
    
    uint16_t x, y;
    if (uiManager->checkTouch(x, y)) {
        delay(200); // Debounce
        
        // Check back button
        if (x >= 10 && x <= 90 && y >= SCREEN_HEIGHT - 40 && y <= SCREEN_HEIGHT - 5) {
            currentState = STATE_MAIN;
            screenDrawn = false;
            return;
        }
        
        // Check channel selection
        int itemHeight = 40;
        int touchedItem = (y - 40) / itemHeight;
        
        if (touchedItem >= 0 && touchedItem < 4) {
            int channelIndex = channelOffset + touchedItem;
            if (channelIndex < sxmChannels.size()) {
                selectedChannel = channelIndex;
                settings.setLastChannel(selectedChannel + 1);
                
                // Play channel
                uiManager->drawLoading("Loading channel...");
                String streamUrl = sxmChannels[selectedChannel].streamUrl;
                
                if (audioPlayer.play(streamUrl)) {
                    uiManager->showMessage("Playing", sxmChannels[selectedChannel].name, 2000);
                } else {
                    uiManager->showMessage("Error", "Failed to play", 3000);
                }
                
                currentState = STATE_MAIN;
                screenDrawn = false;
            }
        }
    }
}

void handleSettings() {
    static bool screenDrawn = false;
    
    if (!screenDrawn) {
        uiManager->setScreen(SCREEN_SETTINGS);
        uiManager->drawSettings();
        screenDrawn = true;
    }
    
    uint16_t x, y;
    if (uiManager->checkTouch(x, y)) {
        delay(200); // Debounce
        
        // Check back button
        if (x >= 10 && x <= 90 && y >= SCREEN_HEIGHT - 40 && y <= SCREEN_HEIGHT - 5) {
            currentState = STATE_MAIN;
            screenDrawn = false;
            return;
        }
        
        // Check menu items
        int itemHeight = 45;
        int touchedItem = (y - 50) / itemHeight;
        
        switch (touchedItem) {
            case 0: // WiFi Settings
                currentState = STATE_WIFI_SETUP;
                screenDrawn = false;
                break;
                
            case 1: // SXM Credentials
                currentState = STATE_SXM_SETUP;
                screenDrawn = false;
                break;
                
            case 2: // FM Frequency
                currentState = STATE_FM_SETUP;
                screenDrawn = false;
                break;
                
            case 3: // About
                uiManager->showMessage("About", "ESP32 SXM Radio v1.0", 3000);
                break;
        }
    }
}
