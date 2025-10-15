#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <Arduino.h>
#include "Audio.h"

// Forward declaration
class FMTransmitter;

class AudioPlayer {
public:
    AudioPlayer();
    ~AudioPlayer();
    
    bool begin();
    
    // Link to FM transmitter for RDS updates
    void setFMTransmitter(FMTransmitter* fm);
    
    // Playback control
    bool play(const String& url);
    void stop();
    void pause();
    void resume();
    
    // Volume control (0-21)
    void setVolume(uint8_t volume);
    uint8_t getVolume();
    
    // Status
    bool isPlaying();
    String getCurrentTitle();
    String getCurrentArtist();
    
    // Metadata updates (called by audio callbacks)
    void updateMetadata(const String& title, const String& artist);
    void updateStationName(const String& station);
    
    // Update loop
    void loop();
    
private:
    Audio audio;
    bool playing;
    uint8_t currentVolume;
    String currentTitle;
    String currentArtist;
    String currentStation;
    FMTransmitter* fmTransmitter;  // Pointer to FM transmitter for RDS
};

// Global instance for callbacks
extern AudioPlayer* g_audioPlayer;

#endif // AUDIO_PLAYER_H
