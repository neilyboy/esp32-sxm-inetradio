#include "sxm_client.h"
#include "config.h"

SXMClient::SXMClient() : sxmServer(DEFAULT_SXM_SERVER) {}

SXMClient::~SXMClient() {
    logout();
}

void SXMClient::setSXMServer(const String& serverUrl) {
    sxmServer = serverUrl;
    Serial.printf("SXM: Server set to %s\n", sxmServer.c_str());
}

String SXMClient::getSXMServer() {
    return sxmServer;
}

bool SXMClient::login(const String& email, const String& password) {
    Serial.println("SXM: Attempting login...");
    
#if USE_SXM_SERVER
    // Use m3u8XM server mode
    Serial.println("SXM: Using server mode");
    return loginToServer(email, password);
#else
    // Direct API mode (not fully implemented)
    Serial.println("SXM: Using direct API mode (placeholder)");
    
    HTTPClient http;
    http.begin(SXM_LOGIN_URL);
    http.addHeader("Content-Type", "application/json");
    
    StaticJsonDocument<512> doc;
    doc["email"] = email;
    doc["password"] = password;
    
    String payload;
    serializeJson(doc, payload);
    
    int httpCode = http.POST(payload);
    
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        DynamicJsonDocument responseDoc(2048);
        DeserializationError error = deserializeJson(responseDoc, response);
        
        if (!error) {
            if (responseDoc.containsKey("token")) {
                authToken = responseDoc["token"].as<String>();
                Serial.println("SXM: Login successful");
                http.end();
                return true;
            }
        }
    }
    
    lastError = "Login failed: HTTP " + String(httpCode);
    Serial.println(lastError);
    http.end();
    return false;
#endif
}

bool SXMClient::isAuthenticated() {
    return authToken.length() > 0;
}

void SXMClient::logout() {
    authToken = "";
    sessionId = "";
    channels.clear();
}

bool SXMClient::fetchChannelList() {
    if (!isAuthenticated()) {
        lastError = "Not authenticated";
        return false;
    }
    
    Serial.println("SXM: Fetching channel list...");
    
#if USE_SXM_SERVER
    return fetchChannelListFromServer();
#else
    // Placeholder implementation with mock channels
    channels.clear();
    
    // Example channels (these would come from API)
    const char* channelData[][4] = {
        {"1", "Hits 1", "Pop", "https://example.com/ch1"},
        {"2", "The Highway", "Country", "https://example.com/ch2"},
        {"3", "Octane", "Rock", "https://example.com/ch3"},
        {"4", "BPM", "Dance", "https://example.com/ch4"},
        {"5", "Hip-Hop Nation", "Hip-Hop", "https://example.com/ch5"},
        {"6", "The Pulse", "Pop", "https://example.com/ch6"},
        {"7", "Classic Vinyl", "Classic Rock", "https://example.com/ch7"},
        {"8", "Soul Town", "Soul", "https://example.com/ch8"},
        {"9", "The Coffee House", "Singer-Songwriter", "https://example.com/ch9"},
        {"10", "Watercolors", "Jazz", "https://example.com/ch10"}
    };
    
    for (int i = 0; i < 10; i++) {
        SXMChannel ch;
        ch.number = channelData[i][0];
        ch.name = channelData[i][1];
        ch.genre = channelData[i][2];
        ch.streamUrl = channelData[i][3];
        ch.id = String(i + 1);
        channels.push_back(ch);
    }
    
    Serial.printf("SXM: Loaded %d channels\n", channels.size());
    return true;
#endif
}

std::vector<SXMChannel> SXMClient::getChannels() {
    return channels;
}

SXMChannel* SXMClient::getChannelById(const String& id) {
    for (auto& ch : channels) {
        if (ch.id == id) {
            return &ch;
        }
    }
    return nullptr;
}

SXMChannel* SXMClient::getChannelByNumber(const String& number) {
    for (auto& ch : channels) {
        if (ch.number == number) {
            return &ch;
        }
    }
    return nullptr;
}

String SXMClient::getStreamUrl(const String& channelId) {
#if USE_SXM_SERVER
    // Get stream URL from server
    return getStreamUrlFromServer(channelId);
#else
    // Use cached stream URL from channel
    SXMChannel* ch = getChannelById(channelId);
    if (ch) {
        return ch->streamUrl;
    }
    return "";
#endif
}

String SXMClient::getLastError() {
    return lastError;
}

bool SXMClient::makeRequest(const String& url, const String& method, const String& payload, String& response) {
    HTTPClient http;
    http.begin(url);
    
    if (authToken.length() > 0) {
        http.addHeader("Authorization", "Bearer " + authToken);
    }
    
    int httpCode = -1;
    
    if (method == "GET") {
        httpCode = http.GET();
    } else if (method == "POST") {
        http.addHeader("Content-Type", "application/json");
        httpCode = http.POST(payload);
    }
    
    if (httpCode == HTTP_CODE_OK) {
        response = http.getString();
        http.end();
        return true;
    }
    
    lastError = "Request failed: HTTP " + String(httpCode);
    http.end();
    return false;
}

bool SXMClient::parseChannelList(const String& jsonResponse) {
    DynamicJsonDocument doc(8192);
    DeserializationError error = deserializeJson(doc, jsonResponse);
    
    if (error) {
        lastError = "JSON parse error";
        return false;
    }
    
    // Parse channel list from JSON
    // This depends on actual API response format
    
    return true;
}

// Server mode implementations
bool SXMClient::loginToServer(const String& email, const String& password) {
    HTTPClient http;
    String url = "http://" + sxmServer + SXM_SERVER_LOGIN;
    
    Serial.printf("SXM: Connecting to server at %s\n", url.c_str());
    
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    
    StaticJsonDocument<256> doc;
    doc["username"] = email;
    doc["password"] = password;
    
    String payload;
    serializeJson(doc, payload);
    
    int httpCode = http.POST(payload);
    
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        
        DynamicJsonDocument responseDoc(1024);
        DeserializationError error = deserializeJson(responseDoc, response);
        
        if (!error && responseDoc.containsKey("success")) {
            if (responseDoc["success"].as<bool>()) {
                authToken = "server_authenticated";  // Token from server if provided
                Serial.println("SXM: Server login successful");
                http.end();
                return true;
            }
        }
    }
    
    lastError = "Server login failed: HTTP " + String(httpCode);
    Serial.println(lastError);
    http.end();
    return false;
}

bool SXMClient::fetchChannelListFromServer() {
    HTTPClient http;
    String url = "http://" + sxmServer + SXM_SERVER_CHANNELS;
    
    Serial.printf("SXM: Fetching channels from %s\n", url.c_str());
    
    http.begin(url);
    http.addHeader("Authorization", "Bearer " + authToken);
    
    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        
        DynamicJsonDocument doc(16384);
        DeserializationError error = deserializeJson(doc, response);
        
        if (!error) {
            channels.clear();
            
            JsonArray channelArray = doc["channels"].as<JsonArray>();
            
            for (JsonObject channelObj : channelArray) {
                SXMChannel channel;
                channel.id = channelObj["id"].as<String>();
                channel.name = channelObj["name"].as<String>();
                channel.number = channelObj["number"] | channelObj["channelNumber"].as<String>();
                channel.genre = channelObj["genre"].as<String>();
                channel.logoUrl = channelObj["logoUrl"] | channelObj["logo"].as<String>();
                // Stream URL will be fetched on demand
                
                channels.push_back(channel);
            }
            
            Serial.printf("SXM: Loaded %d channels from server\n", channels.size());
            http.end();
            return true;
        }
        
        lastError = "Failed to parse channel list from server";
        Serial.println(lastError);
    } else {
        lastError = "Failed to fetch channels: HTTP " + String(httpCode);
        Serial.println(lastError);
    }
    
    http.end();
    return false;
}

String SXMClient::getStreamUrlFromServer(const String& channelId) {
    HTTPClient http;
    String url = "http://" + sxmServer + SXM_SERVER_STREAM + "/" + channelId;
    
    Serial.printf("SXM: Getting stream URL from %s\n", url.c_str());
    
    http.begin(url);
    http.addHeader("Authorization", "Bearer " + authToken);
    
    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, response);
        
        if (!error && doc.containsKey("streamUrl")) {
            String streamUrl = doc["streamUrl"].as<String>();
            http.end();
            return streamUrl;
        }
    }
    
    lastError = "Failed to get stream URL: HTTP " + String(httpCode);
    Serial.println(lastError);
    http.end();
    return "";
}
