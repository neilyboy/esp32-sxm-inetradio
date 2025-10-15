# Quick Start Guide

Get your ESP32 SiriusXM IntraRadio up and running in 30 minutes!

## What You Need

### Hardware (Buy Now!)
- ✅ ESP32 Dev Board (~$10)
- ✅ 2.8" ILI9341 Touchscreen (~$15) - [Amazon Link](https://www.amazon.com/ELEGOO-Display-Acrylic-Protector-Projects/dp/B0FJQJZYXG)
- ✅ QN8066 FM Transmitter Module (~$7)
- ✅ PCM5102 I2S DAC (~$4)
- ✅ Jumper wires (~$5)
- ✅ USB-C car charger

### Software
- ✅ PlatformIO (free)
- ✅ USB driver for ESP32

### Accounts
- ✅ Active SiriusXM subscription
- ✅ Phone with WiFi hotspot

## Installation in 5 Steps

### 1️⃣ Install PlatformIO (5 min)

```bash
pip install platformio
```

Or use VS Code + PlatformIO extension

### 2️⃣ Clone & Build (5 min)

```bash
git clone <your-repo>
cd esp32-sxm-intradio
pio run
```

**Important:** Edit TFT_eSPI library config (see TFT_SETUP_GUIDE.md)

### 3️⃣ Wire It Up (10 min)

**Display → ESP32:**
- MOSI → GPIO 23
- SCK → GPIO 18
- CS → GPIO 15
- DC → GPIO 2
- RST → GPIO 4
- VCC → 3.3V
- GND → GND

**FM Transmitter → ESP32:**
- SDA → GPIO 21
- SCL → GPIO 22
- VCC → 3.3V
- GND → GND

**DAC → ESP32:**
- BCK → GPIO 25
- LCK → GPIO 26
- DIN → GPIO 27
- VCC → 5V
- GND → GND

See HARDWARE_GUIDE.md for complete wiring diagram.

### 4️⃣ Flash Firmware (5 min)

```bash
# Connect ESP32 via USB
pio run --target upload

# Open serial monitor
pio device monitor
```

### 5️⃣ Configure & Enjoy (5 min)

1. **Power on** → See splash screen
2. **Select WiFi** → Choose your phone's hotspot
3. **Enter password** → Use on-screen keyboard
4. **Login to SXM** → Enter email & password
5. **Set FM frequency** → Choose 88.1 FM (or clear frequency)
6. **Start streaming!** → Touch channel to play

## Usage

### Main Screen
```
┌─────────────────────────────┐
│ [SiriusXM]   [Channel Info] │
│                              │
│ [Internet Radio]  [Settings] │
│                              │
│  Touch SXM logo for channels │
└─────────────────────────────┘
```

### Daily Use
1. Turn on car → Device powers up
2. Enable phone hotspot
3. Device auto-connects
4. Tune car radio to FM frequency
5. Touch channel to change

## Troubleshooting

| Problem | Quick Fix |
|---------|-----------|
| Display blank | Check 3.3V power & SPI wiring |
| No touch | Verify TOUCH_CS on GPIO 21 |
| No WiFi | Use 2.4GHz hotspot (not 5GHz) |
| No FM signal | Check antenna wire (15cm long) |
| No audio | Verify I2S wiring & WiFi connection |
| Device resets | Use 2A+ power supply |

**Full troubleshooting:** See DEPLOYMENT.md

## Tips

### Best FM Frequencies
- Try: 87.9, 88.1, 88.3, 88.5 FM
- Avoid local stations
- Use FM scanner app to find clear frequencies

### WiFi Hotspot Setup

**iPhone:**
Settings → Personal Hotspot → Turn On

**Android:**
Settings → Network → Hotspot → Turn On

**Important:** Must be 2.4GHz band!

### Power Options

**Quick:** USB car charger (2A minimum)

**Advanced:** Hardwire to fuse box with ignition-switched circuit

### Antenna

- Use 15cm solid wire
- Keep straight
- Route away from ESP32
- Don't coil or fold

## Next Steps

### Enhance Your Build

- 🎨 **3D print enclosure** → Custom case for dashboard
- 📡 **Better antenna** → External antenna connector
- 🔊 **Volume control** → Add rotary encoder
- 🌈 **Custom UI** → Modify colors and layout
- 📻 **Preset buttons** → Quick-access favorite channels

### Advanced Features

- Add Bluetooth audio input
- Implement voice control
- Add LCD album art display
- Create car integration (steering wheel controls)
- Add backup power (battery)

## Support

### Documentation
- **README.md** → Project overview
- **HARDWARE_GUIDE.md** → Detailed wiring & assembly
- **TFT_SETUP_GUIDE.md** → Display configuration
- **DEPLOYMENT.md** → Complete deployment guide

### Getting Help
1. Check documentation
2. Review serial monitor output
3. Test with example sketches
4. Open GitHub issue

## Safety & Legal

⚠️ **Important:**
- SiriusXM subscription required
- FM transmission must comply with local laws (FCC Part 15 in USA)
- Keep transmit power low (<100mW)
- Don't use while driving (setup only)
- Secure all wiring in vehicle

## Cost Breakdown

| Item | Price |
|------|-------|
| ESP32 | $10 |
| Display | $15 |
| FM Module | $7 |
| DAC | $4 |
| Wires & misc | $10 |
| **Total** | **~$50** |

Plus: USB charger, enclosure (optional)

## Timeline

- **Shopping:** 3-7 days (online orders)
- **Assembly:** 30-60 minutes
- **First run:** 5 minutes
- **Refinement:** Ongoing

**Total to working device:** ~1 hour (after parts arrive)

## Success Checklist

After setup, you should have:
- ✅ Device displays touchscreen UI
- ✅ WiFi connects to phone automatically
- ✅ Channel list loads from SiriusXM
- ✅ Audio streams without buffering
- ✅ FM transmits to car radio
- ✅ Settings persist after power cycle
- ✅ Device fits in enclosure
- ✅ Mounted safely in vehicle

## Enjoy Your Build! 🎉

You now have a professional-grade internet radio with SiriusXM streaming in your car!

**Share your build:**
- Post photos on social media
- Contribute improvements on GitHub
- Help others in the community

---

Need detailed help? See **DEPLOYMENT.md** for comprehensive instructions.

Have fun! 🚗📻🎵
