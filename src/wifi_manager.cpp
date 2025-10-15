#include "wifi_manager.h"

WiFiMgr::WiFiMgr() {}

std::vector<WiFiNetwork> WiFiMgr::scanNetworks() {
    std::vector<WiFiNetwork> networks;
    
    Serial.println("Scanning WiFi networks...");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    
    int n = WiFi.scanNetworks();
    Serial.printf("Found %d networks\n", n);
    
    for (int i = 0; i < n; i++) {
        WiFiNetwork network;
        network.ssid = WiFi.SSID(i);
        network.rssi = WiFi.RSSI(i);
        network.encrypted = (WiFi.encryptionType(i) != WIFI_AUTH_OPEN);
        
        // Skip duplicate SSIDs
        bool duplicate = false;
        for (const auto& existing : networks) {
            if (existing.ssid == network.ssid) {
                duplicate = true;
                break;
            }
        }
        
        if (!duplicate && network.ssid.length() > 0) {
            networks.push_back(network);
        }
    }
    
    // Sort by signal strength
    std::sort(networks.begin(), networks.end(), [](const WiFiNetwork& a, const WiFiNetwork& b) {
        return a.rssi > b.rssi;
    });
    
    return networks;
}

bool WiFiMgr::connect(const String& ssid, const String& password, uint32_t timeout) {
    Serial.printf("Connecting to WiFi: %s\n", ssid.c_str());
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    
    return waitForConnection(timeout);
}

bool WiFiMgr::waitForConnection(uint32_t timeout) {
    unsigned long startTime = millis();
    
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - startTime > timeout) {
            Serial.println("WiFi connection timeout");
            return false;
        }
        delay(100);
    }
    
    Serial.println("WiFi connected!");
    Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
    
    return true;
}

bool WiFiMgr::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiMgr::getIP() {
    return WiFi.localIP().toString();
}

int WiFiMgr::getRSSI() {
    return WiFi.RSSI();
}

void WiFiMgr::disconnect() {
    WiFi.disconnect();
}
