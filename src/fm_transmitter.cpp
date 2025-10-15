#include "fm_transmitter.h"
#include "config.h"

FMTransmitter::FMTransmitter() : currentFrequency(FM_DEFAULT_FREQ), initialized(false) {}

bool FMTransmitter::begin() {
    // Initialize I2C
    Wire.begin(I2C_SDA, I2C_SCL);
    
    // Setup QN8066 using pu2clr library
    tx.setup();
    
    delay(100);
    
    // Configure for TX mode
    tx.setTX(currentFrequency);
    
    // Set transmit power (range 70-120, higher = more power but check FCC limits!)
    tx.setTxInputImpedance(1);  // 20K impedance
    tx.setTxPilot(1);  // Enable stereo pilot
    tx.setTxInputBufferGain(1);  // Input buffer gain
    tx.setTxPower(85);  // Medium power for legal compliance
    
    // Enable RDS
    tx.rdsSetMode(1);  // Enable RDS
    tx.rdsSetStationName("SXM ESP32");  // Default station name
    tx.rdsSetRadioText("Internet Radio");  // Default text
    
    initialized = true;
    Serial.printf("FM Transmitter initialized at %.1f MHz with RDS support\n", currentFrequency);
    
    return true;
}

bool FMTransmitter::setFrequency(float frequency) {
    if (frequency < FM_MIN_FREQ || frequency > FM_MAX_FREQ) {
        Serial.println("FM: Frequency out of range");
        return false;
    }
    
    tx.setTX(frequency);
    currentFrequency = frequency;
    Serial.printf("FM: Frequency set to %.1f MHz\n", frequency);
    
    return true;
}

float FMTransmitter::getFrequency() {
    return currentFrequency;
}

bool FMTransmitter::setPower(uint8_t power) {
    // Constrain power to valid range (70-120)
    if (power < 70) power = 70;
    if (power > 120) power = 120;
    
    tx.setTxPower(power);
    Serial.printf("FM: Power set to %d\n", power);
    
    return true;
}

bool FMTransmitter::setMute(bool mute) {
    tx.setTxMute(mute ? 1 : 0);
    return true;
}

bool FMTransmitter::isTransmitting() {
    return initialized;
}

// RDS (Radio Data System) Methods

void FMTransmitter::setStationName(const String& name) {
    // Station name max 8 characters
    String truncated = name.substring(0, 8);
    currentStationName = truncated;
    
    // Convert String to char array for library
    char stationName[9];
    truncated.toCharArray(stationName, 9);
    tx.rdsSetStationName(stationName);
    
    Serial.printf("FM RDS: Station name set to '%s'\n", stationName);
}

void FMTransmitter::setSongInfo(const String& artist, const String& title) {
    // Combine artist and title for radio text
    String combined = artist + " - " + title;
    setRadioText(combined);
}

void FMTransmitter::setRadioText(const String& text) {
    // Radio text max 64 characters
    String truncated = text.substring(0, 64);
    currentRadioText = truncated;
    
    // Convert String to char array for library
    char radioText[65];
    truncated.toCharArray(radioText, 65);
    tx.rdsSetRadioText(radioText);
    
    Serial.printf("FM RDS: Radio text set to '%s'\n", radioText);
}

void FMTransmitter::updateRDS() {
    // Send RDS data
    // Call this periodically (e.g., in main loop) to keep RDS active
    tx.rdsSendGroup();
}
