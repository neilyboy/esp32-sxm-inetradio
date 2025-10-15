#ifndef FM_TRANSMITTER_H
#define FM_TRANSMITTER_H

#include <Arduino.h>
#include <Wire.h>

// QN8066 I2C Address
#define QN8066_ADDR 0x21

class FMTransmitter {
public:
    FMTransmitter();
    
    bool begin();
    bool setFrequency(float frequency); // in MHz (87.5 - 108.0)
    float getFrequency();
    
    bool setPower(uint8_t power); // 0-127
    bool setMute(bool mute);
    
    bool isTransmitting();
    
private:
    float currentFrequency;
    bool initialized;
    
    // QN8066 register operations
    bool writeRegister(uint8_t reg, uint8_t value);
    bool readRegister(uint8_t reg, uint8_t& value);
    bool writeRegisters(uint8_t startReg, uint8_t* data, uint8_t len);
    
    // Frequency calculation
    uint16_t frequencyToRegister(float frequency);
};

#endif // FM_TRANSMITTER_H
