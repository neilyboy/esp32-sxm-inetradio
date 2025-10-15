# Hardware Assembly Guide

## Bill of Materials (BOM)

### Required Components

| Item | Quantity | Estimated Cost | Notes |
|------|----------|---------------|-------|
| ESP32 Development Board | 1 | $8-15 | ESP32-WROOM-32 or DevKit V1 |
| 2.8" ILI9341 TFT Touchscreen | 1 | $15-20 | 320x240 SPI display |
| QN8066 FM Transmitter Module | 1 | $5-10 | I2C interface, stereo |
| PCM5102 I2S DAC Module | 1 | $3-5 | For audio output |
| USB-C Power Module | 1 | $5-8 | 5V input, 3.3V/5V output |
| Breadboard or PCB | 1 | $3-10 | For prototyping/assembly |
| Jumper Wires | 20+ | $3-5 | Male-to-male and male-to-female |
| Case/Enclosure | 1 | $5-15 | Optional but recommended |

**Total Estimated Cost:** $50-90 USD

### Optional Components
- PAM8403 Audio Amplifier ($2-3) - If you need louder audio
- Small speaker (3W) ($2-5) - For testing audio output
- External antenna wire (10-15cm) - For better FM range
- Capacitors (100µF, 10µF) - For power filtering
- 3D printed case - Custom enclosure

## Detailed Wiring Instructions

### ESP32 Pinout Reference
```
                    ESP32 DevKit V1
                   +--------------+
              3V3  |[ ]      [ ]| GND
              EN   |[ ]      [ ]| GPIO 23 (MOSI)
         GPIO 36   |[ ]      [ ]| GPIO 22 (SCL)
         GPIO 39   |[ ]      [ ]| GPIO 1 (TX)
         GPIO 34   |[ ]      [ ]| GPIO 3 (RX)
         GPIO 35   |[ ]      [ ]| GPIO 21 (SDA/Touch CS)
         GPIO 32   |[ ]      [ ]| GND
         GPIO 33   |[ ]      [ ]| GPIO 19 (MISO)
         GPIO 25   |[ ]      [ ]| GPIO 18 (SCK)
         GPIO 26   |[ ]      [ ]| GPIO 5
         GPIO 27   |[ ]      [ ]| GPIO 17
         GPIO 14   |[ ]      [ ]| GPIO 16
         GPIO 12   |[ ]      [ ]| GPIO 4 (RST)
         GND       |[ ]      [ ]| GPIO 0
         GPIO 13   |[ ]      [ ]| GPIO 2 (DC)
         GPIO 9    |[ ]      [ ]| GPIO 15 (CS)
         GPIO 10   |[ ]      [ ]| GPIO 8
         GPIO 11   |[ ]      [ ]| GPIO 7
         5V        |[ ]      [ ]| GPIO 6
                   +--------------+
```

### 1. ILI9341 Display Connection

**Display Pinout:**
```
ILI9341 Pin → ESP32 Pin → Function
───────────────────────────────────
VCC         → 3.3V      → Power
GND         → GND       → Ground
CS          → GPIO 15   → Chip Select
RESET       → GPIO 4    → Reset
DC/RS       → GPIO 2    → Data/Command
MOSI/SDI    → GPIO 23   → SPI MOSI
SCK         → GPIO 18   → SPI Clock
LED         → 3.3V      → Backlight (or via resistor)
MISO/SDO    → GPIO 19   → SPI MISO
T_CLK       → GPIO 18   → Touch Clock (shared with SCK)
T_CS        → GPIO 21   → Touch Chip Select
T_DIN       → GPIO 23   → Touch Data In (shared with MOSI)
T_DO        → GPIO 19   → Touch Data Out (shared with MISO)
T_IRQ       → (not used)→ Touch Interrupt (optional)
```

**Important Notes:**
- The display uses **3.3V logic** - do NOT connect to 5V!
- Touch controller shares SPI bus with display
- Backlight can draw up to 100mA - use resistor if needed (100Ω)
- Some displays have built-in voltage regulators for 5V input

### 2. QN8066 FM Transmitter Connection

**QN8066 Pinout:**
```
QN8066 Pin → ESP32 Pin → Function
─────────────────────────────────
VCC        → 3.3V      → Power (3.3V ONLY!)
GND        → GND       → Ground
SCL        → GPIO 22   → I2C Clock
SDA        → GPIO 21   → I2C Data
LINL       → PCM5102 OUTL → Left Audio Input
LINR       → PCM5102 OUTR → Right Audio Input
ANT        → Antenna Wire  → FM Antenna (10-15cm wire)
```

**Important Notes:**
- QN8066 is **3.3V ONLY** - 5V will damage it!
- Antenna wire: Use 10-15cm solid wire for optimal range
- Add 10µF capacitor between VCC and GND (close to module)
- Expected range: 10-30 feet (3-10 meters) depending on power settings

### 3. PCM5102 I2S DAC Connection

**PCM5102 Pinout:**
```
PCM5102 Pin → ESP32 Pin → Function
──────────────────────────────────
VIN         → 5V        → Power (can use 3.3V-5V)
GND         → GND       → Ground
BCK         → GPIO 25   → Bit Clock
LCK/LRCK    → GPIO 26   → Left/Right Clock (Word Select)
DIN         → GPIO 27   → Data Input
OUTL        → QN8066 LINL → Left Audio Output
OUTR        → QN8066 LINR → Right Audio Output
SCK         → GND       → System Clock (tie to GND for I2S mode)
FLT         → GND       → Filter (normal mode)
DEMP        → GND       → De-emphasis off
XSMT        → 3.3V      → Soft mute disable
FMT         → GND       → I2S format
```

**Important Notes:**
- PCM5102 outputs analog audio directly
- No need for external amplifier for FM transmitter input
- Very low noise, high quality audio
- Jumpers on breakout board: check manufacturer settings

### 4. Power Distribution

**Power Requirements:**
```
Component          Voltage    Current    Notes
─────────────────────────────────────────────
ESP32              3.3V       500mA      Active WiFi
ILI9341 Display    3.3V       150mA      Backlight on
QN8066 FM TX       3.3V       50-100mA   Transmitting
PCM5102 DAC        3.3-5V     20mA       Low power
──────────────────────────────────────────────
Total              -          ~750mA     Peak usage
```

**Recommended Power Setup:**
1. **USB-C Input** → 5V @ 1A (car USB charger)
2. **ESP32 Built-in Regulator** → 3.3V for ESP32
3. **Buck Converter** (optional) → Dedicated 3.3V rail for peripherals

**Power Wiring:**
```
USB-C 5V → ESP32 VIN (5V)
         ↓
      [ESP32 3.3V Regulator]
         ↓
      3.3V Out → Display, QN8066, etc.
```

**Important:** If you experience brownouts or resets:
- Add 470µF capacitor across USB-C input
- Use separate 3.3V buck converter (AMS1117-3.3)
- Ensure USB cable can handle 1A+

## Assembly Steps

### Step 1: Test Components Individually

**Test Display:**
```cpp
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

void setup() {
    tft.init();
    tft.fillScreen(TFT_RED);
    delay(1000);
    tft.fillScreen(TFT_GREEN);
    delay(1000);
    tft.fillScreen(TFT_BLUE);
}
```

**Test Touch:**
```cpp
uint16_t x, y;
if (tft.getTouch(&x, &y)) {
    tft.fillCircle(x, y, 3, TFT_WHITE);
}
```

**Test I2C (QN8066):**
```cpp
#include <Wire.h>

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22); // SDA, SCL
    
    Wire.beginTransmission(0x21); // QN8066 address
    byte error = Wire.endTransmission();
    
    if (error == 0) {
        Serial.println("QN8066 found!");
    } else {
        Serial.println("QN8066 not found");
    }
}
```

### Step 2: Breadboard Assembly

1. **Place ESP32** in center of breadboard
2. **Connect Power Rails:**
   - ESP32 3.3V → Positive rail
   - ESP32 GND → Negative rail
3. **Connect Display** following pinout above
4. **Connect QN8066** following pinout above
5. **Connect PCM5102** following pinout above
6. **Add Power Capacitors:**
   - 100µF across USB input
   - 10µF near each module

### Step 3: Wire Management

**Color Coding (recommended):**
- Red: Power (3.3V, 5V)
- Black: Ground
- Yellow: I2C (SDA/SCL)
- Blue: SPI (MOSI, MISO, SCK)
- Green: Other signals (CS, DC, RST)
- Orange: I2S Audio (BCK, LCK, DIN)

### Step 4: Antenna Installation

1. Cut **15cm** of solid wire (20-22 AWG)
2. Strip 1cm on one end
3. Solder/connect to QN8066 ANT pad
4. Route antenna away from ESP32 to reduce interference
5. Keep straight for best radiation pattern

### Step 5: Testing

1. **Power on** - Check for smoke/hot components
2. **Serial Monitor** - Check for boot messages
3. **Display** - Should show splash screen
4. **Touch** - Test calibration
5. **WiFi** - Scan for networks
6. **FM** - Tune car radio to selected frequency
7. **Audio** - Play test stream

## Troubleshooting

### Display Issues

**No display / white screen:**
- Check 3.3V power connection
- Verify SPI wiring (MOSI, SCK, CS, DC, RST)
- Try different TFT_eSPI User_Setup configuration
- Test with simple TFT_eSPI example

**Display works but touch doesn't:**
- Check T_CS connection (GPIO 21)
- Touch shares SPI bus - verify MISO connection
- Some displays need touch calibration

**Flickering display:**
- Add decoupling capacitor (100µF) near display
- Check power supply stability
- Reduce SPI clock speed in User_Setup.h

### FM Transmitter Issues

**No FM signal:**
- Verify QN8066 I2C address (0x21)
- Check antenna connection
- Increase transmit power in code
- Verify 3.3V power (NOT 5V!)

**Weak signal:**
- Extend antenna to 15cm
- Adjust transmit power (setPower function)
- Move antenna away from ESP32
- Check for interference

**Distorted audio:**
- Check audio input levels
- Verify PCM5102 connections
- Add decoupling caps on audio lines
- Check ground connections

### Audio Issues

**No audio:**
- Verify I2S wiring (BCK, LCK, DIN)
- Check PCM5102 power
- Verify stream URL is valid
- Check WiFi connection

**Choppy audio:**
- Increase ESP32 CPU frequency (240MHz)
- Check WiFi signal strength
- Increase audio buffer size
- Use lower bitrate stream

### Power Issues

**ESP32 resets randomly:**
- Add larger capacitor (470µF+) to power input
- Use dedicated power supply (not computer USB)
- Check current draw of all components
- Consider separate 3.3V regulator

**Components not working:**
- Check for voltage drops under load
- Verify breadboard connections
- Test with multimeter
- Check for short circuits

## PCB Design (Advanced)

For a permanent installation, consider designing a custom PCB:

**Design Considerations:**
1. **Separate ground planes** for digital and analog
2. **Decoupling capacitors** on every power pin
3. **Short traces** for I2S and SPI signals
4. **Antenna routing** - keep away from digital signals
5. **Mounting holes** for display and enclosure
6. **ESD protection** on USB and touch inputs

**Recommended PCB Layout:**
```
[Display Connector]
       |
   [ESP32]
       |
[I2C Bus] ── [QN8066] ── [Antenna]
       |
[I2S Bus] ── [PCM5102]
       |
  [USB-C Connector]
```

## Enclosure Design

**Dimensions:**
- Internal space: 80mm x 65mm x 25mm (minimum)
- Display cutout: 71mm x 52mm (adjust for your display)
- Antenna exit: 3mm hole on side
- USB-C cutout: 9mm x 3.5mm

**3D Printing Tips:**
- Use PETG for heat resistance (car interior)
- Design snap-fit mounting for display
- Add ventilation holes for ESP32
- Include cable management clips
- Design press-fit buttons for settings reset

## Car Installation

**Mounting Options:**
1. **Dashboard mount** - Use 3M adhesive strips
2. **Vent clip** - Universal phone mount adapter
3. **Cup holder mount** - Adjustable phone holder
4. **Custom bracket** - 3D printed for specific vehicle

**Power Connection:**
1. Use car USB-C charger (12V → 5V, 2A+)
2. Optional: Hardwire to ignition-switched 12V
3. Add fuse (1A) for safety

**Best Practices:**
- Keep away from airbag deployment zones
- Secure all wiring to prevent interference
- Route antenna for optimal reception
- Test FM frequencies before permanent install
- Ensure good phone-to-ESP32 WiFi hotspot connection

## Legal Considerations

**FCC Regulations (USA):**
- Part 15 compliant FM transmitters: < 250µV/m @ 3m
- Maximum range: ~100 feet
- Must not cause interference
- Use only clear FM frequencies

**Similar regulations apply in other countries.**

Always check local laws regarding FM transmission!
