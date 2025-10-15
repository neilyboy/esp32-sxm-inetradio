# Parts Shopping List

Complete shopping list with links and recommendations for building the ESP32 SiriusXM IntraRadio.

## Essential Components

### 1. ESP32 Development Board

**What to buy:** ESP32-WROOM-32 Development Board

**Specifications:**
- Dual-core 240MHz processor
- WiFi 802.11 b/g/n
- Bluetooth (not used in this project)
- 4MB Flash minimum
- Built-in USB-to-Serial

**Recommended:**
- 🔗 [Amazon - ESP32 DevKit V1](https://www.amazon.com/s?k=ESP32+development+board)
- 🔗 [AliExpress - ESP32 WROOM](https://www.aliexpress.com/wholesale?SearchText=esp32+wroom+development+board)

**Price:** $8-15

**Notes:** 
- Avoid generic ESP8266 (not compatible)
- Get one with pre-soldered headers
- Ensure it has CP2102 or CH340 USB chip

---

### 2. 2.8" ILI9341 TFT Touchscreen Display

**What to buy:** 2.8" ILI9341 SPI TFT LCD Display with Touch (320x240)

**Specifications:**
- Size: 2.8 inches
- Controller: ILI9341
- Resolution: 320x240 pixels
- Interface: SPI
- Touch Controller: XPT2046
- Voltage: 3.3V/5V
- Colors: 262K

**Recommended:**
- 🔗 [Amazon - ELEGOO 2.8" Display](https://www.amazon.com/ELEGOO-Display-Acrylic-Protector-Projects/dp/B0FJQJZYXG) ⭐ **Project tested with this**
- 🔗 [Amazon - Generic ILI9341 2.8"](https://www.amazon.com/s?k=ili9341+2.8+inch+tft+display)

**Price:** $15-20

**Notes:**
- Must be ILI9341 controller (not ILI9488)
- Ensure it includes touch functionality
- Some come with SD card slot (not needed but okay)
- Yellow PCB is most common version

---

### 3. QN8066 FM Transmitter Module

**What to buy:** QN8066 FM Transmitter Breakout Board

**Specifications:**
- Chip: QN8066
- Frequency Range: 76-108 MHz
- Interface: I2C
- Voltage: 3.3V (5V will damage it!)
- Stereo support: Yes
- Output Power: Adjustable

**Where to buy:**
- 🔗 [AliExpress - QN8066 Module](https://www.aliexpress.com/wholesale?SearchText=qn8066+fm+transmitter)
- 🔗 [eBay - QN8066](https://www.ebay.com/sch/i.html?_nkw=qn8066+fm+transmitter)

**Price:** $5-10

**Alternative:** KT0803L module ($3-5, lower quality)

**Notes:**
- ⚠️ Must be 3.3V only!
- Usually comes as bare module
- May need to solder header pins
- Include antenna wire (10-15cm)

---

### 4. PCM5102 I2S DAC Module

**What to buy:** PCM5102A I2S Digital to Analog Converter Module

**Specifications:**
- Chip: PCM5102A (Texas Instruments)
- Interface: I2S
- Voltage: 3.3V-5V
- SNR: 112dB
- Output: Line level (2Vrms)

**Recommended:**
- 🔗 [Amazon - PCM5102 DAC](https://www.amazon.com/s?k=pcm5102+dac+module)
- 🔗 [AliExpress - PCM5102](https://www.aliexpress.com/wholesale?SearchText=pcm5102+i2s+module)

**Price:** $3-5

**Notes:**
- Often sold as "Purple PCM5102 module"
- Some have jumpers for configuration
- Very low noise, high quality audio

---

### 5. Jumper Wires

**What to buy:** Dupont Jumper Wire Kit

**Specifications:**
- Male-to-Male: 20 pieces
- Male-to-Female: 20 pieces
- Length: 20cm
- AWG: 24-26

**Recommended:**
- 🔗 [Amazon - Jumper Wire Kit](https://www.amazon.com/s?k=dupont+jumper+wires)

**Price:** $5-8

**Notes:**
- Get variety pack (M-M, M-F, F-F)
- Different colors helpful for organization
- 20cm length is ideal

---

### 6. USB-C Car Charger

**What to buy:** 2-Port USB Car Charger (2A+ per port)

**Specifications:**
- Output: 5V 2A minimum per port
- Ports: USB-A or USB-C
- Safety: Overcurrent protection
- LED indicator: Optional

**Recommended:**
- 🔗 [Amazon - Anker Car Charger](https://www.amazon.com/s?k=anker+car+charger)
- Any quality brand with 2A+ output

**Price:** $10-15

**Notes:**
- Don't use cheap no-name chargers
- 2A minimum to prevent brownouts
- Dual port useful for phone + device

---

## Optional but Recommended

### 7. Breadboard

**For prototyping before permanent assembly**

**Recommended:**
- 830 tie-point breadboard
- 🔗 [Amazon - Breadboard](https://www.amazon.com/s?k=830+tie+point+breadboard)

**Price:** $5-8

---

### 8. Enclosure/Case

**For clean finished look**

**Options:**
- **3D Printed:** Custom design (STL files can be created)
- **Project Box:** Universal plastic enclosure
- 🔗 [Amazon - Project Box 100x60x25mm](https://www.amazon.com/s?k=project+box+electronics)

**Price:** $5-15

**Recommended internal dimensions:** 80mm x 65mm x 25mm minimum

---

### 9. Antenna Wire

**For optimal FM range**

**What to buy:** 20-22 AWG solid core wire

**Recommended:**
- 22 AWG solid copper wire
- Length: 15cm (cut from longer spool)
- 🔗 [Amazon - Solid Core Wire](https://www.amazon.com/s?k=22+awg+solid+wire)

**Price:** $5 (for spool)

**Notes:**
- Must be solid core (not stranded)
- Keep straight for best performance
- Copper or brass

---

### 10. Power Capacitors

**For stable power supply**

**What to buy:**
- 1x 470µF 16V Electrolytic Capacitor
- 2x 100µF 16V Electrolytic Capacitor
- 5x 10µF 16V Ceramic Capacitor

**Recommended:**
- 🔗 [Amazon - Capacitor Kit](https://www.amazon.com/s?k=electrolytic+capacitor+kit)

**Price:** $8-10 for kit

---

### 11. PCB or Perfboard

**For permanent assembly (alternative to breadboard)**

**Options:**
- Custom PCB (design and order from JLCPCB/PCBWay)
- Universal perfboard/protoboard

**Recommended:**
- 🔗 [Amazon - Perfboard](https://www.amazon.com/s?k=perfboard)

**Price:** $5-10

---

## Tools Needed

### Basic Tools

- ✅ Soldering iron (if using perfboard)
- ✅ Solder and flux
- ✅ Wire strippers
- ✅ Small screwdriver set
- ✅ Multimeter (for testing)
- ✅ USB cable (for programming ESP32)

### Optional Tools

- Hot glue gun (for securing components)
- Heat shrink tubing
- Label maker
- 3D printer (for custom enclosure)

---

## Cost Summary

| Component | Price |
|-----------|-------|
| ESP32 Dev Board | $10 |
| 2.8" ILI9341 Display | $17 |
| QN8066 FM Transmitter | $7 |
| PCM5102 DAC | $4 |
| Jumper Wires | $6 |
| USB Car Charger | $12 |
| **Subtotal (Essential)** | **$56** |
| | |
| Breadboard | $6 |
| Enclosure | $10 |
| Antenna Wire | $5 |
| Capacitors | $9 |
| Perfboard | $7 |
| **Total (with Optional)** | **$93** |

**Note:** Prices are estimates and may vary by region and retailer.

---

## Shopping Checklist

Print this checklist when shopping:

**Essential:**
- [ ] ESP32 Development Board (ESP32-WROOM-32)
- [ ] 2.8" ILI9341 TFT Display with Touch
- [ ] QN8066 FM Transmitter Module (3.3V!)
- [ ] PCM5102 I2S DAC Module
- [ ] Jumper Wire Kit (M-M, M-F)
- [ ] USB-C Car Charger (2A+)

**Optional:**
- [ ] Breadboard (830 tie-point)
- [ ] Enclosure/Project Box
- [ ] Antenna Wire (22 AWG solid)
- [ ] Capacitor Kit
- [ ] Perfboard/Protoboard

**Tools:**
- [ ] USB Cable for ESP32
- [ ] Soldering Iron & Solder
- [ ] Wire Strippers
- [ ] Multimeter

---

## Bulk Purchase Recommendations

If building multiple units or want spare parts:

**Budget Bundle (~$45/unit):**
- Buy ESP32 in 5-pack
- Buy displays in 3-pack
- Bulk jumper wires
- Shared tool costs

**Where to buy in bulk:**
- 🔗 AliExpress (cheapest, 2-4 week shipping)
- 🔗 Banggood
- 🔗 LCSC (components)

---

## Country-Specific Shopping

### USA 🇺🇸
- **Primary:** Amazon Prime (fast shipping)
- **Budget:** AliExpress
- **Components:** Adafruit, SparkFun, Digi-Key

### UK 🇬🇧
- **Primary:** Amazon UK
- **Electronics:** Pimoroni, The Pi Hut
- **Components:** RS Components, Farnell

### Canada 🇨🇦
- **Primary:** Amazon.ca
- **Electronics:** RobotShop, Digi-Key Canada

### Europe 🇪🇺
- **Primary:** Amazon (local)
- **Electronics:** Reichelt, Conrad
- **Bulk:** AliExpress

### Australia 🇦🇺
- **Primary:** AliExpress (shipping)
- **Local:** Core Electronics, Jaycar

---

## Verification Before Purchase

Before buying, verify:

- ✅ ESP32 has WiFi capability
- ✅ Display is ILI9341 controller (not ILI9488)
- ✅ Display includes touch (XPT2046)
- ✅ FM module is 3.3V compatible
- ✅ All modules have or include header pins
- ✅ USB car charger provides 2A minimum

---

## Compatibility Notes

**⚠️ Not Compatible:**
- ESP8266 (different chip, use ESP32)
- 5V FM transmitters (will damage ESP32)
- ILI9488 displays (different driver)
- 5GHz WiFi (ESP32 only supports 2.4GHz)

**✅ Compatible Alternatives:**
- ESP32-S3 (newer, compatible)
- ESP32-C3 (cheaper, may need code changes)
- KT0803L FM module (lower quality alternative)
- Different size ILI9341 displays (3.2", etc.)

---

## Quick Links Summary

**Fast Shopping (Amazon):**
1. ESP32: https://www.amazon.com/s?k=ESP32+development+board
2. Display: https://www.amazon.com/ELEGOO-Display-Acrylic-Protector-Projects/dp/B0FJQJZYXG
3. DAC: https://www.amazon.com/s?k=pcm5102+dac+module
4. Wires: https://www.amazon.com/s?k=dupont+jumper+wires

**Budget Shopping (AliExpress):**
1. ESP32: Search "ESP32 WROOM development board"
2. QN8066: Search "QN8066 FM transmitter"
3. All components available for ~40% less (slower shipping)

---

**Happy Shopping! 🛒**

Next step after receiving parts: See [HARDWARE_GUIDE.md](HARDWARE_GUIDE.md) for assembly instructions.
