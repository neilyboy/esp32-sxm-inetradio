#include "fm_transmitter.h"
#include "config.h"

FMTransmitter::FMTransmitter() : currentFrequency(FM_DEFAULT_FREQ), initialized(false) {}

bool FMTransmitter::begin() {
    Wire.begin(I2C_SDA, I2C_SCL);
    
    // Reset QN8066
    uint8_t initData[] = {
        0x00, 0x81,  // System1: SWRST
    };
    
    if (!writeRegisters(0x00, initData, 2)) {
        Serial.println("FM: Failed to reset");
        return false;
    }
    
    delay(100);
    
    // Initialize QN8066 registers
    uint8_t configData[] = {
        0x00, 0x01,  // System1: Enable
        0x01, 0x34,  // System2: 
        0x02, 0x10,  // System3:
        0x03, 0x00,  // System4
        0x04, 0x00,  // System5
        0x05, 0x58,  // CCA
        0x06, 0x00,  // SNR
        0x07, 0x00,  // RSSISIG
        0x08, 0x00,  // CID1
        0x09, 0x00,  // CID2
        0x0A, 0x00,  // STATUS1
        0x0B, 0x00,  // STATUS2
        0x0C, 0x00,  // STATUS3
        0x0D, 0x00,  // STATUS4
        0x0E, 0x00,  // RDS_DATA
        0x0F, 0x00   // RDS_DATA
    };
    
    // Set TX mode
    uint8_t txMode = 0x10; // TX mode enable
    if (!writeRegister(0x00, txMode)) {
        Serial.println("FM: Failed to set TX mode");
        return false;
    }
    
    delay(10);
    
    // Set frequency
    if (!setFrequency(currentFrequency)) {
        Serial.println("FM: Failed to set frequency");
        return false;
    }
    
    // Set transmit power (adjust as needed, max 127)
    setPower(80);  // Medium power
    
    initialized = true;
    Serial.printf("FM Transmitter initialized at %.1f MHz\n", currentFrequency);
    
    return true;
}

bool FMTransmitter::setFrequency(float frequency) {
    if (frequency < FM_MIN_FREQ || frequency > FM_MAX_FREQ) {
        Serial.println("FM: Frequency out of range");
        return false;
    }
    
    uint16_t freqReg = frequencyToRegister(frequency);
    
    // Write frequency to registers (QN8066 specific)
    uint8_t freqHigh = (freqReg >> 8) & 0xFF;
    uint8_t freqLow = freqReg & 0xFF;
    
    if (!writeRegister(0x01, freqLow)) {
        return false;
    }
    
    if (!writeRegister(0x02, freqHigh)) {
        return false;
    }
    
    currentFrequency = frequency;
    Serial.printf("FM: Frequency set to %.1f MHz\n", frequency);
    
    return true;
}

float FMTransmitter::getFrequency() {
    return currentFrequency;
}

bool FMTransmitter::setPower(uint8_t power) {
    if (power > 127) {
        power = 127;
    }
    
    // Write power to register (QN8066 specific - register 0x04)
    return writeRegister(0x04, power);
}

bool FMTransmitter::setMute(bool mute) {
    uint8_t value = mute ? 0x00 : 0x01;
    return writeRegister(0x00, value);
}

bool FMTransmitter::isTransmitting() {
    return initialized;
}

bool FMTransmitter::writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(QN8066_ADDR);
    Wire.write(reg);
    Wire.write(value);
    uint8_t error = Wire.endTransmission();
    
    return (error == 0);
}

bool FMTransmitter::readRegister(uint8_t reg, uint8_t& value) {
    Wire.beginTransmission(QN8066_ADDR);
    Wire.write(reg);
    uint8_t error = Wire.endTransmission(false);
    
    if (error != 0) {
        return false;
    }
    
    Wire.requestFrom(QN8066_ADDR, (uint8_t)1);
    if (Wire.available()) {
        value = Wire.read();
        return true;
    }
    
    return false;
}

bool FMTransmitter::writeRegisters(uint8_t startReg, uint8_t* data, uint8_t len) {
    Wire.beginTransmission(QN8066_ADDR);
    Wire.write(startReg);
    
    for (uint8_t i = 0; i < len; i++) {
        Wire.write(data[i]);
    }
    
    uint8_t error = Wire.endTransmission();
    return (error == 0);
}

uint16_t FMTransmitter::frequencyToRegister(float frequency) {
    // QN8066 frequency calculation
    // Formula: Channel = (Freq_MHz - 60) / 0.05
    uint16_t channel = (uint16_t)((frequency - 60.0) / 0.05);
    return channel;
}
