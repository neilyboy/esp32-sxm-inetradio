#include "sxm_client.h"
#include "config.h"

SXMClient::SXMClient() {}

SXMClient::~SXMClient() {
    logout();
}

bool SXMClient::login(const String& email, const String& password) {
    Serial.println("SXM: Attempting login...");
    
    // Note: This is a simplified implementation
    // The actual SiriusXM API requires proper OAuth/token handling
    // For now, we'll store credentials for future use
    
    HTTPClient http;
    http.begin(SXM_LOGIN_URL);
    http.addHeader("Content-Type", "application/json");
    
    // Build JSON payload
    StaticJsonDocument<512> doc;
    doc["email"] = email;
    doc["password"] = password;
    
    String payload;
    serializeJson(doc, payload);
    
    int httpCode = http.POST(payload);
    
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        
        // Parse response
        DynamicJsonDocument responseDoc(2048);
        DeserializationError error = deserializeJson(responseDoc, response);
        
        if (!error) {
            // Extract token (adjust based on actual API response)
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
    
    // This is a placeholder - actual implementation would fetch from SXM API
    // For now, we'll populate with common SXM channels
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
    SXMChannel* ch = getChannelById(channelId);
    if (ch) {
        return ch->streamUrl;
    }
    return "";
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
