# Deployment Guide

Complete step-by-step instructions for building and deploying the ESP32 SiriusXM IntraRadio.

## Prerequisites

### Software Requirements
- **Operating System:** Windows, macOS, or Linux
- **Python:** 3.7 or higher
- **Git:** For cloning repositories
- **USB Driver:** CP210x or CH340 driver for ESP32

### Hardware Requirements
- All components listed in HARDWARE_GUIDE.md
- USB cable for ESP32 programming
- Computer with USB port

## Step 1: Install Development Tools

### Install PlatformIO

**Option A: PlatformIO CLI (Recommended)**
```bash
# Install PlatformIO
pip install platformio

# Verify installation
pio --version
```

**Option B: Visual Studio Code + PlatformIO Extension**
1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Open Extensions (Ctrl+Shift+X)
3. Search for "PlatformIO IDE"
4. Click Install

### Install USB Drivers

**For CP2102 (most common):**
- Download from [Silicon Labs](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)

**For CH340:**
- Download from [CH340 driver page](http://www.wch-ic.com/downloads/CH341SER_EXE.html)

**Linux users:**
```bash
# Add user to dialout group
sudo usermod -a -G dialout $USER
# Log out and back in for changes to take effect
```

## Step 2: Clone and Setup Project

```bash
# Clone the repository
git clone <your-repository-url>
cd esp32-sxm-intradio

# Or if starting from these files
cd /path/to/esp32-sxm-intradio
```

## Step 3: Configure TFT_eSPI Library

**CRITICAL:** The display library requires manual configuration.

See `TFT_SETUP_GUIDE.md` for detailed instructions.

**Quick setup:**
1. Build project once to download libraries:
   ```bash
   pio run
   ```

2. Navigate to TFT_eSPI library:
   ```bash
   cd .pio/libdeps/esp32dev/TFT_eSPI/
   ```

3. Edit `User_Setup.h` with the configuration provided in TFT_SETUP_GUIDE.md

4. Return to project directory:
   ```bash
   cd ../../../../
   ```

## Step 4: Build the Project

```bash
# Clean previous builds
pio run --target clean

# Build the project
pio run
```

**Expected output:**
```
Processing esp32dev (platform: espressif32; board: esp32dev; framework: arduino)
...
Linking .pio/build/esp32dev/firmware.elf
Building .pio/build/esp32dev/firmware.bin
========================= [SUCCESS] Took X.XX seconds =========================
```

**Common build errors:**

| Error | Solution |
|-------|----------|
| `TFT_eSPI.h: No such file` | Run `pio lib install` |
| `User_Setup.h` conflicts | Edit TFT_eSPI User_Setup.h |
| Out of memory | Use `huge_app.csv` partition |
| Multiple definitions | Clean build: `pio run -t clean` |

## Step 5: Hardware Assembly

Follow the wiring instructions in `HARDWARE_GUIDE.md`.

**Testing checklist:**
- [ ] Display shows colors when powered
- [ ] Touch responds to finger
- [ ] ESP32 has stable 3.3V power
- [ ] No components are hot to touch
- [ ] Serial monitor shows boot messages

## Step 6: Upload Firmware

### Connect ESP32

1. Connect ESP32 to computer via USB
2. Identify the serial port:

**Windows:**
```bash
pio device list
# Look for COM port (e.g., COM3)
```

**macOS/Linux:**
```bash
ls /dev/tty.* # macOS
ls /dev/ttyUSB* # Linux
```

### Upload

```bash
# Auto-detect port and upload
pio run --target upload

# Or specify port manually
pio run --target upload --upload-port /dev/ttyUSB0
```

**If upload fails:**

1. **Press and hold BOOT button** on ESP32
2. Click **Upload**
3. Release BOOT when "Connecting..." appears

### Monitor Serial Output

```bash
# Open serial monitor
pio device monitor

# Or specify baud rate
pio device monitor -b 115200
```

Expected boot messages:
```
=== ESP32 SiriusXM IntraRadio ===
Initializing display...
Initializing settings...
Initializing FM transmitter...
FM Transmitter initialized at 88.1 MHz
Initializing audio player...
Audio player initialized
First run - entering setup mode
Scanning WiFi networks...
```

## Step 7: First-Time Configuration

### 7.1 WiFi Setup

1. Device displays WiFi scan results
2. **Enable WiFi hotspot** on your phone
3. Touch your network SSID on display
4. Use on-screen keyboard to enter password
5. Touch **Connect**

**Troubleshooting:**
- If connection fails, check password
- Ensure phone hotspot is 2.4GHz (ESP32 doesn't support 5GHz)
- Move closer to phone for stronger signal

### 7.2 SiriusXM Login

1. Touch **Email** field
2. Enter SiriusXM account email
3. Touch **Password** field
4. Enter SiriusXM password
5. Touch **Login**

**Note:** Credentials are stored securely in ESP32 NVS.

**Troubleshooting:**
- Verify SiriusXM subscription is active
- Check internet connectivity
- Ensure credentials are correct

### 7.3 FM Frequency Setup

1. Use **-** and **+** buttons to adjust frequency
2. Avoid frequencies used by local stations
3. Recommended: 87.9, 88.1, 88.3 FM
4. Touch **Save**

**Finding clear frequencies:**
- Tune car radio manually and listen
- Use FM frequency finder app
- Try frequencies at low end (87.5-88.5)

## Step 8: Testing

### Test Checklist

- [ ] **Display:** Touch interface responsive
- [ ] **WiFi:** Connected to phone hotspot
- [ ] **SiriusXM:** Channel list appears
- [ ] **Audio:** Stream plays when channel selected
- [ ] **FM:** Car radio receives signal on selected frequency
- [ ] **Settings:** Can change configuration

### Audio Test

1. Touch **SiriusXM logo** on main screen
2. Select a channel (e.g., "Hits 1")
3. Tune car radio to configured FM frequency
4. Listen for audio

**Troubleshooting:**
- **No audio on radio:** Check FM antenna connection
- **Choppy audio:** Move closer to phone/improve WiFi
- **No channel list:** Verify SXM login
- **Static:** Adjust FM frequency away from interference

## Step 9: Final Assembly

Once everything works:

1. **Disconnect power**
2. **Secure all wiring:**
   - Use hot glue or zip ties
   - Ensure no shorts
   - Keep antenna clear
3. **Install in enclosure:**
   - Mount display in cutout
   - Route antenna outside
   - Secure ESP32 and modules
   - Provide ventilation
4. **Test again before closing**

## Step 10: Car Installation

### Mounting

**Options:**
1. **Dashboard mount:** 3M adhesive strips
2. **Vent clip:** Universal phone mount
3. **Cup holder:** Adjustable mount

**Requirements:**
- Easy access to touchscreen
- USB power nearby
- Not blocking airbags
- Antenna has clear path

### Power Connection

**Recommended:**
- Car USB-C charger (2A minimum)
- Quality USB cable
- Cigarette lighter socket

**Advanced:**
- Hardwire to fuse box
- Use add-a-fuse tap
- Connect to ignition-switched circuit
- Add inline 1A fuse

### Antenna Positioning

1. **Extend antenna** fully (10-15cm)
2. **Position away from:**
   - Metal surfaces
   - Other electronics
   - WiFi devices
3. **Test range** before permanent installation

## Usage Guide

### Daily Use

1. **Power on device** (automatically when car starts if using powered USB)
2. **Enable phone hotspot**
3. **Device auto-connects** to WiFi
4. **Auto-loads last channel**
5. **Tune car radio** to FM frequency
6. **Touch SXM logo** to change channels

### Changing Channels

1. Touch **SiriusXM logo**
2. Scroll through channel list
3. Touch channel to select
4. Device streams and transmits on FM

### Settings Menu

Access settings by touching **Settings button** on main screen:

- **WiFi Settings:** Change network/password
- **SXM Credentials:** Update login
- **FM Frequency:** Adjust transmission frequency
- **About:** Version information

## Maintenance

### Regular Maintenance

- **Check connections** monthly
- **Clean display** with microfiber cloth
- **Verify antenna** is secure
- **Update firmware** when available

### Troubleshooting Guide

**Device won't power on:**
- Check USB power supply (must be 5V 1A+)
- Verify USB cable is data+power capable
- Check for shorts in wiring

**WiFi won't connect:**
- Verify phone hotspot is active
- Check 2.4GHz band (not 5GHz)
- Forget network and re-enter password
- Move closer to phone

**No FM signal:**
- Check antenna connection
- Verify FM frequency is clear
- Increase transmit power (code modification)
- Test in different location

**Audio cutting out:**
- Improve WiFi signal strength
- Reduce distance to phone
- Check for interference
- Try different FM frequency

**Device resets randomly:**
- Check power supply capacity (2A recommended)
- Add larger capacitor (470µF) to power input
- Check for loose connections
- Verify USB cable quality

## Advanced Configuration

### Adjusting FM Transmit Power

Edit `src/fm_transmitter.cpp`:
```cpp
// Line ~45
setPower(80);  // Change 80 to 50-127 (higher = more range)
```

**Legal note:** Higher power may violate FCC Part 15 regulations!

### Custom Channel List

If SiriusXM API changes, edit `src/sxm_client.cpp` to add channels manually.

### Audio Quality

Adjust buffer size in `include/config.h`:
```cpp
#define AUDIO_BUFFER_SIZE 1024  // Increase for smoother playback
```

### Display Brightness

The ILI9341 backlight can be controlled via PWM:
```cpp
// Add to config.h
#define TFT_BL 32  // Backlight pin

// In setup()
ledcSetup(0, 5000, 8);  // Channel 0, 5kHz, 8-bit
ledcAttachPin(TFT_BL, 0);
ledcWrite(0, 128);  // 0-255 brightness
```

## Firmware Updates

### OTA Updates (Future)

The project can be extended to support Over-The-Air updates:

1. Add OTA library to platformio.ini
2. Implement OTA handler in main.cpp
3. Upload firmware via WiFi

### USB Updates

```bash
# Connect device via USB
# Build and upload
pio run --target upload
```

## Backup and Restore

### Backup Settings

Settings are stored in NVS. To backup:
```bash
# Read partition table
esptool.py --port /dev/ttyUSB0 read_flash 0x9000 0x5000 nvs_backup.bin
```

### Restore Settings

```bash
# Write partition table
esptool.py --port /dev/ttyUSB0 write_flash 0x9000 nvs_backup.bin
```

## Support and Community

### Getting Help

1. Check HARDWARE_GUIDE.md for wiring issues
2. Check TFT_SETUP_GUIDE.md for display issues
3. Review serial monitor output for error messages
4. Check GitHub issues

### Contributing

Improvements welcome! Areas for contribution:
- SiriusXM API integration improvements
- Internet radio station database
- UI/UX enhancements
- Power optimization
- Additional FM transmitter support

## Legal Disclaimer

- **SiriusXM subscription required**
- **FM transmission must comply with local regulations**
- **FCC Part 15 compliance required in USA**
- **Use at your own risk**
- **Not for commercial use**

---

**Congratulations!** You now have a working ESP32 SiriusXM IntraRadio device!

Enjoy your in-car satellite radio experience! 📻🚗
