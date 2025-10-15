#include "audio_player.h"
#include "config.h"

AudioPlayer::AudioPlayer() : playing(false), currentVolume(12) {}

AudioPlayer::~AudioPlayer() {
    stop();
}

bool AudioPlayer::begin() {
    // Initialize I2S audio
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(currentVolume); // 0...21
    
    Serial.println("Audio player initialized");
    return true;
}

bool AudioPlayer::play(const String& url) {
    Serial.printf("Playing: %s\n", url.c_str());
    
    stop(); // Stop any current playback
    
    bool success = audio.connecttohost(url.c_str());
    
    if (success) {
        playing = true;
        Serial.println("Playback started");
    } else {
        Serial.println("Failed to start playback");
        playing = false;
    }
    
    return success;
}

void AudioPlayer::stop() {
    if (playing) {
        audio.stopSong();
        playing = false;
        Serial.println("Playback stopped");
    }
}

void AudioPlayer::pause() {
    if (playing) {
        audio.pauseResume();
        Serial.println("Playback paused");
    }
}

void AudioPlayer::resume() {
    if (!playing) {
        audio.pauseResume();
        Serial.println("Playback resumed");
    }
}

void AudioPlayer::setVolume(uint8_t volume) {
    if (volume > 21) {
        volume = 21;
    }
    currentVolume = volume;
    audio.setVolume(volume);
}

uint8_t AudioPlayer::getVolume() {
    return currentVolume;
}

bool AudioPlayer::isPlaying() {
    return playing && audio.isRunning();
}

String AudioPlayer::getCurrentTitle() {
    return currentTitle;
}

String AudioPlayer::getCurrentArtist() {
    return currentArtist;
}

void AudioPlayer::loop() {
    audio.loop();
}

// Optional: Audio event callbacks
void audio_info(const char *info){
    Serial.print("audio_info: "); Serial.println(info);
}

void audio_id3data(const char *info){
    Serial.print("id3data: "); Serial.println(info);
}

void audio_eof_mp3(const char *info){
    Serial.print("eof_mp3: "); Serial.println(info);
}

void audio_showstation(const char *info){
    Serial.print("station: "); Serial.println(info);
}

void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle: "); Serial.println(info);
}

void audio_bitrate(const char *info){
    Serial.print("bitrate: "); Serial.println(info);
}

void audio_commercial(const char *info){
    Serial.print("commercial: "); Serial.println(info);
}

void audio_icyurl(const char *info){
    Serial.print("icyurl: "); Serial.println(info);
}

void audio_lasthost(const char *info){
    Serial.print("lasthost: "); Serial.println(info);
}
