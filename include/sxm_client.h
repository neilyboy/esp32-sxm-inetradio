#ifndef SXM_CLIENT_H
#define SXM_CLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <vector>

struct SXMChannel {
    String id;
    String name;
    String number;
    String genre;
    String logoUrl;
    String streamUrl;
};

class SXMClient {
public:
    SXMClient();
    ~SXMClient();
    
    // Server configuration (for m3u8XM server mode)
    void setSXMServer(const String& serverUrl);
    String getSXMServer();
    
    // Authentication
    bool login(const String& email, const String& password);
    bool isAuthenticated();
    void logout();
    
    // Channel management
    bool fetchChannelList();
    std::vector<SXMChannel> getChannels();
    SXMChannel* getChannelById(const String& id);
    SXMChannel* getChannelByNumber(const String& number);
    
    // Streaming
    String getStreamUrl(const String& channelId);
    
    // Error handling
    String getLastError();
    
private:
    HTTPClient http;
    String authToken;
    String sessionId;
    String sxmServer;  // Server URL for m3u8XM mode
    std::vector<SXMChannel> channels;
    String lastError;
    
    bool makeRequest(const String& url, const String& method, const String& payload, String& response);
    bool parseChannelList(const String& jsonResponse);
    
    // Server mode methods
    bool loginToServer(const String& email, const String& password);
    bool fetchChannelListFromServer();
    String getStreamUrlFromServer(const String& channelId);
};

#endif // SXM_CLIENT_H
