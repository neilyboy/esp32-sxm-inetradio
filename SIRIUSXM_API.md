# SiriusXM API Integration Guide

## Overview

This document explains how to integrate with SiriusXM's streaming API. The current implementation in this project uses a simplified approach and placeholder data. For full functionality, you'll need to implement proper SiriusXM authentication and streaming.

## Important Notes

⚠️ **Legal Disclaimer:**
- SiriusXM API is not officially public
- You MUST have an active SiriusXM subscription
- This is for personal use only
- Reverse engineering may violate ToS
- Use at your own risk

## Understanding the m3u8XM Project

The [m3u8XM Python script](https://github.com/myselfondiscord/m3u8XM) serves as a reference for SiriusXM API interaction. Our ESP32 implementation needs to replicate this functionality.

### Key Components

1. **Authentication** - Login with SiriusXM credentials
2. **Session Management** - Maintain authenticated session
3. **Channel Discovery** - Fetch available channels
4. **Stream URL Generation** - Get HLS/m3u8 stream URLs
5. **Metadata** - Retrieve now-playing information

## Implementation Approaches

### Approach 1: Direct API Integration (Recommended)

Implement SiriusXM API calls directly on ESP32.

**Pros:**
- Standalone device
- No dependencies
- Lower latency

**Cons:**
- Complex authentication
- API may change
- Limited documentation

### Approach 2: Proxy Server (Easier)

Run Python script on another device (Raspberry Pi, home server) and have ESP32 connect to it.

**Pros:**
- Simpler ESP32 code
- Leverage existing Python implementation
- Easier to debug

**Cons:**
- Requires additional hardware
- Network dependency
- More complex setup

### Approach 3: Cached Credentials (Current Implementation)

Use manual channel list with hardcoded URLs.

**Pros:**
- Simple
- Fast startup
- Reliable

**Cons:**
- Manual updates needed
- No real-time metadata
- Limited channel selection

## Authentication Flow

### Step 1: Login Request

**Endpoint:** `https://player.siriusxm.com/rest-api/2/experience/modules/get/login`

**Method:** POST

**Headers:**
```http
Content-Type: application/json
User-Agent: Mozilla/5.0 (compatible; ESP32)
```

**Payload:**
```json
{
  "moduleRequest": {
    "resultTemplate": "web",
    "deviceInfo": {
      "osVersion": "ESP32",
      "platform": "Web",
      "clientDeviceId": "your-device-id",
      "deviceModel": "ESP32"
    }
  },
  "standardAuth": {
    "username": "user@example.com",
    "password": "password"
  }
}
```

**Response:**
```json
{
  "ModuleListResponse": {
    "status": 1,
    "messages": [...],
    "moduleList": {
      "modules": [
        {
          "moduleDetails": {
            "token": "auth-token-here",
            "sessionId": "session-id-here"
          }
        }
      ]
    }
  }
}
```

### Step 2: Extract Tokens

Parse response to get:
- `authToken` - For authenticated requests
- `sessionId` - Session identifier

### Step 3: Store Credentials

Save tokens to NVS (Non-Volatile Storage) for future use.

## Channel List Request

**Endpoint:** `https://player.siriusxm.com/rest-api/2/experience/modules/get/channel-lineup`

**Method:** POST

**Headers:**
```http
Content-Type: application/json
Authorization: Bearer {authToken}
```

**Payload:**
```json
{
  "moduleRequest": {
    "resultTemplate": "web"
  },
  "consumeRequests": []
}
```

**Response:**
```json
{
  "ModuleListResponse": {
    "moduleList": {
      "modules": [
        {
          "moduleArea": "Discovery",
          "moduleDetails": {
            "liveChannelData": [
              {
                "channelId": "siriusxmhits1",
                "name": "Hits 1",
                "channelNumber": "2",
                "genre": "Pop",
                "description": "Today's Hits",
                "logoUrl": "https://...",
                "isFavorite": false
              }
            ]
          }
        }
      ]
    }
  }
}
```

## Stream URL Generation

**Endpoint:** `https://player.siriusxm.com/rest-api/2/experience/modules/get/channel-player`

**Method:** POST

**Headers:**
```http
Content-Type: application/json
Authorization: Bearer {authToken}
```

**Payload:**
```json
{
  "moduleRequest": {
    "resultTemplate": "web"
  },
  "channelId": "siriusxmhits1"
}
```

**Response:**
```json
{
  "ModuleListResponse": {
    "moduleList": {
      "modules": [
        {
          "moduleDetails": {
            "liveChannelData": {
              "hlsAudioUrl": "https://sxm-hls-url.com/channel.m3u8?token=xxx",
              "markerLists": [
                {
                  "markers": [
                    {
                      "cut": {
                        "title": "Song Title",
                        "artists": [{"name": "Artist Name"}]
                      }
                    }
                  ]
                }
              ]
            }
          }
        }
      ]
    }
  }
}
```

## ESP32 Implementation

### Update `src/sxm_client.cpp`

Replace placeholder implementation with real API calls:

```cpp
bool SXMClient::login(const String& email, const String& password) {
    HTTPClient http;
    
    // Build login request
    StaticJsonDocument<1024> doc;
    doc["moduleRequest"]["resultTemplate"] = "web";
    doc["moduleRequest"]["deviceInfo"]["osVersion"] = "ESP32";
    doc["moduleRequest"]["deviceInfo"]["platform"] = "Web";
    doc["moduleRequest"]["deviceInfo"]["deviceModel"] = "ESP32";
    doc["standardAuth"]["username"] = email;
    doc["standardAuth"]["password"] = password;
    
    String payload;
    serializeJson(doc, payload);
    
    // Make request
    http.begin("https://player.siriusxm.com/rest-api/2/experience/modules/get/login");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("User-Agent", "ESP32-SXM/1.0");
    
    int httpCode = http.POST(payload);
    
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        
        // Parse response
        DynamicJsonDocument responseDoc(8192);
        deserializeJson(responseDoc, response);
        
        // Extract tokens
        JsonObject modules = responseDoc["ModuleListResponse"]["moduleList"]["modules"][0];
        authToken = modules["moduleDetails"]["token"].as<String>();
        sessionId = modules["moduleDetails"]["sessionId"].as<String>();
        
        http.end();
        return true;
    }
    
    http.end();
    return false;
}

bool SXMClient::fetchChannelList() {
    if (!isAuthenticated()) return false;
    
    HTTPClient http;
    
    // Build request
    StaticJsonDocument<512> doc;
    doc["moduleRequest"]["resultTemplate"] = "web";
    
    String payload;
    serializeJson(doc, payload);
    
    // Make request
    http.begin("https://player.siriusxm.com/rest-api/2/experience/modules/get/channel-lineup");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + authToken);
    
    int httpCode = http.POST(payload);
    
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        parseChannelList(response);
        http.end();
        return true;
    }
    
    http.end();
    return false;
}

String SXMClient::getStreamUrl(const String& channelId) {
    HTTPClient http;
    
    // Build request
    StaticJsonDocument<512> doc;
    doc["moduleRequest"]["resultTemplate"] = "web";
    doc["channelId"] = channelId;
    
    String payload;
    serializeJson(doc, payload);
    
    // Make request
    http.begin("https://player.siriusxm.com/rest-api/2/experience/modules/get/channel-player");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + authToken);
    
    int httpCode = http.POST(payload);
    
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        
        // Parse stream URL
        DynamicJsonDocument responseDoc(4096);
        deserializeJson(responseDoc, response);
        
        String streamUrl = responseDoc["ModuleListResponse"]["moduleList"]["modules"][0]
                                     ["moduleDetails"]["liveChannelData"]["hlsAudioUrl"].as<String>();
        
        http.end();
        return streamUrl;
    }
    
    http.end();
    return "";
}
```

### Parse Channel List

```cpp
bool SXMClient::parseChannelList(const String& jsonResponse) {
    DynamicJsonDocument doc(16384); // Large buffer for channel list
    DeserializationError error = deserializeJson(doc, jsonResponse);
    
    if (error) {
        lastError = "JSON parse error: " + String(error.c_str());
        return false;
    }
    
    channels.clear();
    
    JsonArray channelArray = doc["ModuleListResponse"]["moduleList"]["modules"][0]
                               ["moduleDetails"]["liveChannelData"];
    
    for (JsonObject channelObj : channelArray) {
        SXMChannel channel;
        channel.id = channelObj["channelId"].as<String>();
        channel.name = channelObj["name"].as<String>();
        channel.number = channelObj["channelNumber"].as<String>();
        channel.genre = channelObj["genre"].as<String>();
        channel.logoUrl = channelObj["logoUrl"].as<String>();
        
        channels.push_back(channel);
    }
    
    return true;
}
```

## Handling HLS Streams

SiriusXM uses HLS (HTTP Live Streaming) with .m3u8 playlists.

### HLS Stream Structure

```
Master Playlist (channel.m3u8)
├── Variant 1 (128kbps)
│   └── Segments (.aac files)
├── Variant 2 (256kbps)
│   └── Segments (.aac files)
```

### ESP32-audioI2S Library

The `Audio` library we're using supports HLS:

```cpp
audio.connecttohost("https://sxm-server.com/channel.m3u8?token=xxx");
```

It automatically:
- Downloads m3u8 playlist
- Selects appropriate bitrate
- Fetches and decodes segments
- Buffers for smooth playback

## Token Refresh

SiriusXM tokens expire. Implement refresh logic:

```cpp
void SXMClient::checkTokenExpiry() {
    unsigned long currentTime = millis();
    
    if (currentTime - lastTokenRefresh > TOKEN_LIFETIME) {
        refreshToken();
    }
}

bool SXMClient::refreshToken() {
    // Re-authenticate with stored credentials
    String email = getSavedEmail();
    String password = getSavedPassword();
    
    return login(email, password);
}
```

## Metadata Updates

Get now-playing information:

```cpp
struct NowPlaying {
    String songTitle;
    String artistName;
    String albumName;
    String albumArt;
    int duration;
    int position;
};

NowPlaying SXMClient::getNowPlaying(const String& channelId) {
    // Parse from stream URL response
    // "markerLists" contains current track info
}
```

## Error Handling

```cpp
enum SXMError {
    SXM_SUCCESS,
    SXM_AUTH_FAILED,
    SXM_NETWORK_ERROR,
    SXM_INVALID_TOKEN,
    SXM_CHANNEL_UNAVAILABLE,
    SXM_SUBSCRIPTION_EXPIRED
};

SXMError handleResponse(int httpCode, const String& response) {
    if (httpCode == 401) return SXM_INVALID_TOKEN;
    if (httpCode == 403) return SXM_SUBSCRIPTION_EXPIRED;
    // ... handle other codes
}
```

## Testing

### Test Authentication

```cpp
void testAuth() {
    SXMClient client;
    
    if (client.login("test@example.com", "password")) {
        Serial.println("✓ Auth successful");
        Serial.println("Token: " + client.getToken());
    } else {
        Serial.println("✗ Auth failed: " + client.getLastError());
    }
}
```

### Test Channel Fetch

```cpp
void testChannels() {
    if (client.fetchChannelList()) {
        Serial.printf("✓ Found %d channels\n", client.getChannels().size());
        
        for (auto& ch : client.getChannels()) {
            Serial.printf("  %s - %s (%s)\n", 
                ch.number.c_str(), 
                ch.name.c_str(), 
                ch.genre.c_str());
        }
    }
}
```

### Test Streaming

```cpp
void testStream() {
    String url = client.getStreamUrl("siriusxmhits1");
    
    if (url.length() > 0) {
        Serial.println("✓ Stream URL: " + url);
        
        if (audioPlayer.play(url)) {
            Serial.println("✓ Playback started");
        }
    }
}
```

## Alternative: Use Python Bridge

If ESP32 implementation is too complex, run Python script on separate device:

### Setup Python Server

```python
# sxm_server.py
from flask import Flask, jsonify
from sxm_client import SXMClient  # From m3u8XM

app = Flask(__name__)
client = SXMClient()

@app.route('/login', methods=['POST'])
def login():
    # Handle login
    pass

@app.route('/channels')
def channels():
    return jsonify(client.get_channels())

@app.route('/stream/<channel_id>')
def stream(channel_id):
    return jsonify({'url': client.get_stream_url(channel_id)})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
```

### ESP32 Client

```cpp
// Connect to Python server instead of SXM directly
String SXMClient::getStreamUrl(const String& channelId) {
    HTTPClient http;
    http.begin("http://your-server-ip:5000/stream/" + channelId);
    
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        
        StaticJsonDocument<512> doc;
        deserializeJson(doc, response);
        
        return doc["url"].as<String>();
    }
    
    return "";
}
```

## Security Considerations

1. **Never hardcode credentials** in source code
2. **Use HTTPS** for all API calls
3. **Store tokens securely** in NVS
4. **Implement token refresh** before expiry
5. **Handle failed auth** gracefully
6. **Rate limit** API requests

## Debugging

### Enable Verbose Logging

```cpp
#define SXM_DEBUG 1

#ifdef SXM_DEBUG
    #define SXM_LOG(msg) Serial.println(String("[SXM] ") + msg)
#else
    #define SXM_LOG(msg)
#endif
```

### Monitor HTTP Traffic

Use Wireshark or Charles Proxy to inspect:
- Request headers
- Request payload
- Response structure
- Token format

### Test with cURL

```bash
# Test login
curl -X POST https://player.siriusxm.com/rest-api/2/experience/modules/get/login \
  -H "Content-Type: application/json" \
  -d '{"standardAuth":{"username":"user@example.com","password":"pass"}}'
```

## Resources

- **m3u8XM GitHub:** https://github.com/myselfondiscord/m3u8XM
- **ESP32-audioI2S:** https://github.com/schreibfaul1/ESP32-audioI2S
- **ArduinoJson:** https://arduinojson.org/
- **HLS Specification:** https://datatracker.ietf.org/doc/html/rfc8216

## Conclusion

Full SiriusXM integration requires:
1. Proper authentication flow
2. Channel list parsing
3. Stream URL generation
4. HLS playback handling
5. Token management

The current implementation uses placeholders. For production use, implement real API calls following this guide.

**Recommendation:** Start with Python bridge approach for easier development and debugging.
