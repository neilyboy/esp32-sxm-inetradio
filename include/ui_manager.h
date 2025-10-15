#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "config.h"

enum Screen {
    SCREEN_NONE,
    SCREEN_SPLASH,
    SCREEN_WIFI_SCAN,
    SCREEN_WIFI_PASSWORD,
    SCREEN_SXM_LOGIN,
    SCREEN_FM_CONFIG,
    SCREEN_MAIN,
    SCREEN_CHANNEL_LIST,
    SCREEN_SETTINGS,
    SCREEN_LOADING
};

class UIManager {
public:
    UIManager(TFT_eSPI* tft);
    
    void begin();
    void setScreen(Screen screen);
    Screen getCurrentScreen();
    
    // Touch handling
    bool checkTouch(uint16_t& x, uint16_t& y);
    
    // Screen drawing
    void drawSplash();
    void drawWiFiScan(const std::vector<String>& networks, int selected);
    void drawPasswordInput(const String& ssid, const String& password);
    void drawSXMLogin(const String& email, const String& password, bool emailField);
    void drawFMConfig(float frequency);
    void drawMainScreen(const String& channelName, const String& artist);
    void drawChannelList(const std::vector<String>& channels, int selected, int offset);
    void drawSettings();
    void drawLoading(const String& message);
    
    // UI Elements
    void drawButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const String& text, uint16_t color, bool pressed = false);
    void drawKeyboard(bool uppercase = false);
    char getKeyboardPress(uint16_t x, uint16_t y, bool uppercase = false);
    
    // Utility
    void showMessage(const String& title, const String& message, uint16_t duration = 2000);
    void drawProgress(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t percent);
    
private:
    TFT_eSPI* tft;
    Screen currentScreen;
    
    void clearScreen();
    void drawHeader(const String& title);
    void drawScrollbar(uint16_t x, uint16_t y, uint16_t h, int total, int current, int visible);
};

#endif // UI_MANAGER_H
