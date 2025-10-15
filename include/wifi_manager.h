#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <vector>

struct WiFiNetwork {
    String ssid;
    int rssi;
    bool encrypted;
};

class WiFiMgr {
public:
    WiFiMgr();
    
    // Scan for networks
    std::vector<WiFiNetwork> scanNetworks();
    
    // Connect to network
    bool connect(const String& ssid, const String& password, uint32_t timeout = 20000);
    
    // Check connection status
    bool isConnected();
    
    // Get connection info
    String getIP();
    int getRSSI();
    
    // Disconnect
    void disconnect();
    
private:
    bool waitForConnection(uint32_t timeout);
};

#endif // WIFI_MANAGER_H
