#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <Preferences.h>

class Settings {
public:
    Settings();
    ~Settings();
    
    bool begin();
    void reset();
    
    // WiFi Settings
    bool hasWiFiCredentials();
    String getWiFiSSID();
    String getWiFiPassword();
    void setWiFiCredentials(const String& ssid, const String& password);
    
    // SiriusXM Settings
    bool hasSXMCredentials();
    String getSXMEmail();
    String getSXMPassword();
    void setSXMCredentials(const String& email, const String& password);
    
    // SiriusXM Server (for m3u8XM server mode)
    String getSXMServer();
    void setSXMServer(const String& server);
    bool hasSXMServer();
    
    // FM Frequency
    float getFMFrequency();
    void setFMFrequency(float frequency);
    
    // Last Channel
    int getLastChannel();
    void setLastChannel(int channel);
    
    // First run flag
    bool isFirstRun();
    void setFirstRunComplete();
    
private:
    Preferences preferences;
};

#endif // SETTINGS_H
