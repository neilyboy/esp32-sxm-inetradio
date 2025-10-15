#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include <Arduino.h>
#include "Audio.h"

class AudioPlayer {
public:
    AudioPlayer();
    ~AudioPlayer();
    
    bool begin();
    
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
    
    // Update loop
    void loop();
    
private:
    Audio audio;
    bool playing;
    uint8_t currentVolume;
    String currentTitle;
    String currentArtist;
};

#endif // AUDIO_PLAYER_H
