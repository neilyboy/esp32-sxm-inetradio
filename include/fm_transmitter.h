#ifndef FM_TRANSMITTER_H
#define FM_TRANSMITTER_H

#include <Arduino.h>
#include <QN8066.h>  // pu2clr QN8066 library with RDS support

class FMTransmitter {
public:
    FMTransmitter();
    
    bool begin();
    bool setFrequency(float frequency);
    float getFrequency();
    bool setPower(uint8_t power);
    bool setMute(bool mute);
    bool isTransmitting();
    
    // RDS (Radio Data System) support - displays on car radio!
    void setStationName(const String& name);  // 8 chars max (e.g. "SXM HITS")
    void setSongInfo(const String& artist, const String& title);  // Shows on display
    void setRadioText(const String& text);  // Scrolling text (64 chars max)
    void updateRDS();  // Call periodically to send RDS data
    
private:
    QN8066 tx;  // pu2clr QN8066 library object
    float currentFrequency;
    bool initialized;
    
    String currentStationName;
    String currentRadioText;
};

#endif // FM_TRANSMITTER_H
