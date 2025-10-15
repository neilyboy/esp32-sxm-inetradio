# ESP32 SiriusXM IntraRadio

A standalone ESP32-based internet radio that streams SiriusXM and internet radio stations, broadcasting over FM to your car stereo.

## Hardware Requirements

### Core Components
- **ESP32 Development Board** (ESP32-WROOM-32 or similar)
  - Must have WiFi capability
  - Recommend 4MB+ flash
- **2.8" ILI9341 TFT Touchscreen Display** (320x240, SPI)
  - [ELEGOO 2.8" TFT Display](https://www.amazon.com/ELEGOO-Display-Acrylic-Protector-Projects/dp/B0FJQJZYXG)
- **FM Transmitter Module**:
  - **QN8066** (**REQUIRED for RDS support** - stereo, RDS, I2C)
  - ~~KT0803L~~ (no RDS support, not recommended)
- **I2S DAC Module** (PCM5102 or similar) - for audio output
- **USB-C Power Module** (5V to 3.3V/5V for ESP32)

### Optional Components
- Small amplifier board (PAM8403) if needed for audio boost
- Enclosure/case for clean installation

## Features

- ✅ WiFi configuration via touchscreen
- ✅ SiriusXM channel streaming (requires SiriusXM subscription)
- ✅ Internet radio station support (32 pre-loaded stations)
- ✅ FM transmission on user-selectable frequency (87.5-108 MHz)
- ✅ **RDS (Radio Data System) support** - displays song info on car radio! 📻
  - Shows station name (8 chars)
  - Shows artist and song title (64 chars scrolling text)
  - Automatic metadata extraction from streams
  - Uses pu2clr/QN8066 library
- ✅ Touch-based channel selection
- ✅ Settings management (WiFi, credentials, FM frequency)
- ✅ Persistent storage of settings
- ✅ Auto-connect on power-up
- ✅ Resume last played channel

## SiriusXM Configuration

**IMPORTANT:** The ESP32 cannot run Python, so it cannot run the m3u8XM script directly. You have two options:

### Option 1: Internet Radio Only (Works Immediately)
- Skip SiriusXM setup entirely
- Use the 32 pre-loaded internet radio stations
- No additional configuration needed
- **Best for testing the hardware first!**

### Option 2: SiriusXM via m3u8XM Server (Recommended for Full SXM)
- Run the [m3u8XM Python script](https://github.com/myselfondiscord/m3u8XM) on a **separate device**:
  - Raspberry Pi (recommended)
  - Home server
  - Always-on PC/Mac
- ESP32 connects to this server to get SiriusXM stream URLs
- **Full setup guide:** See `M3U8XM_SERVER_SETUP.md`

**Quick Server Setup:**
1. Install m3u8XM on Raspberry Pi
2. Run the API wrapper server (provided in guide)
3. Configure ESP32 with server IP in `include/config.h`:
   ```cpp
   #define USE_SXM_SERVER true
   #define DEFAULT_SXM_SERVER "192.168.1.100:5000"  // Your server IP
   ```
4. Rebuild and flash ESP32

**Or configure via settings menu on device** (feature can be added - see TODO in M3U8XM_SERVER_SETUP.md)

### Option 3: Direct SiriusXM API (Advanced - Not Fully Implemented)
- Implement native SiriusXM API calls in C++
- See `SIRIUSXM_API.md` for implementation guide
- Requires reverse engineering SiriusXM's authentication
- **Status:** Placeholder code only

**For most users:** Start with Option 1 (internet radio) to test hardware, then add Option 2 (server mode) for SiriusXM.

## Wiring Diagram

### ESP32 to ILI9341 Display
```
ESP32          ILI9341
-----          -------
GPIO 23   -->  MOSI
GPIO 18   -->  SCK
GPIO 15   -->  CS
GPIO 2    -->  DC
GPIO 4    -->  RST
GPIO 19   -->  MISO
GPIO 21   -->  T_CS (Touch CS)
3.3V      -->  VCC
GND       -->  GND
3.3V      -->  LED (backlight)
```

### ESP32 to QN8066 FM Transmitter
```
ESP32          QN8066
-----          ------
GPIO 22   -->  SCL (I2C Clock)
GPIO 21   -->  SDA (I2C Data)
3.3V      -->  VCC
GND       -->  GND
```

### ESP32 to PCM5102 I2S DAC
```
ESP32          PCM5102
-----          -------
GPIO 25   -->  BCK (Bit Clock)
GPIO 26   -->  LCK (Word Select)
GPIO 27   -->  DIN (Data)
GND       -->  GND
5V        -->  VIN
           
PCM5102 --> QN8066
OUTL    --> LINL (Left Input)
OUTR    --> LINR (Right Input)
GND     --> GND
```

### Power
```
USB-C Power (5V) --> Buck Converter --> 3.3V for ESP32
                                    --> 5V for peripherals
```

## Software Setup

### Prerequisites
- **PlatformIO** (recommended) or Arduino IDE
- USB cable for ESP32 programming

### Installation

1. **Clone this repository:**
   ```bash
   git clone <your-repo>
   cd esp32-sxm-intradio
   ```

2. **Install PlatformIO:**
   ```bash
   pip install platformio
   ```

3. **Build the project:**
   ```bash
   pio run
   ```

4. **Upload to ESP32:**
   ```bash
   pio run --target upload
   ```

5. **Monitor serial output (optional):**
   ```bash
   pio device monitor
   ```

## First-Time Setup

1. **Power on the device** - Connect USB-C power
2. **WiFi Configuration:**
   - Device will scan for WiFi networks
   - Touch to select your network (phone's hotspot)
   - Enter password using on-screen keyboard
   - Device will connect and save credentials

3. **SiriusXM Credentials:**
   - Enter your SiriusXM email
   - Enter your SiriusXM password
   - Credentials are stored securely

4. **FM Frequency Selection:**
   - Choose an empty FM frequency (87.5-108 MHz)
   - Avoid frequencies used by local stations
   - Recommended: 87.9, 88.1, or 88.3 FM

5. **Ready to Use:**
   - Main screen shows SiriusXM logo
   - Touch logo to browse channels
   - Use arrow buttons or direct input to change channels

## Usage

### Main Screen
- **SiriusXM Logo** - Touch to open channel list
- **Settings Icon** - Configure WiFi, credentials, FM frequency
- **Internet Radio Icon** - Browse internet radio stations

### Channel Selection
- **Browse Mode** - Scroll through channel list
- **Direct Input** - Type channel number
- **Arrow Buttons** - Navigate up/down
- **Back Button** - Return to main screen

### Settings Menu
- **WiFi Settings** - Change network/password
- **SiriusXM Credentials** - Update login info
- **FM Frequency** - Adjust transmission frequency
- **About** - Version and info

## Configuration

All settings are stored in ESP32 NVS (Non-Volatile Storage) and persist across reboots.

## Troubleshooting

### No Audio
- Check FM transmitter wiring
- Verify FM frequency is clear
- Ensure DAC is properly connected
- Check audio output level

### WiFi Won't Connect
- Verify password is correct
- Check signal strength
- Ensure phone hotspot is active
- Try forgetting and re-entering credentials

### Display Not Working
- Check SPI wiring
- Verify power connections
- Test with example sketch first

### SiriusXM Not Streaming
- Verify credentials are correct
- Check internet connectivity
- Ensure SiriusXM subscription is active

## Development

### Project Structure
```
esp32-sxm-intradio/
├── src/
│   ├── main.cpp           # Main application entry
│   ├── ui/                # UI components
│   │   ├── screen_manager.cpp
│   │   ├── main_screen.cpp
│   │   ├── channel_screen.cpp
│   │   ├── settings_screen.cpp
│   │   └── keyboard.cpp
│   ├── network/           # Network & streaming
│   │   ├── wifi_manager.cpp
│   │   ├── sxm_client.cpp
│   │   └── radio_client.cpp
│   ├── audio/             # Audio handling
│   │   ├── audio_player.cpp
│   │   └── audio_decoder.cpp
│   ├── hardware/          # Hardware drivers
│   │   ├── fm_transmitter.cpp
│   │   ├── display.cpp
│   │   └── touch.cpp
│   └── storage/           # Settings storage
│       └── settings.cpp
├── include/               # Header files
├── lib/                   # External libraries
├── platformio.ini         # PlatformIO config
└── README.md
```

### Adding Internet Radio Stations
Edit `include/radio_stations.h` to add more stations.

## Documentation

- **[README.md](README.md)** - This file, project overview
- **[QUICKSTART.md](QUICKSTART.md)** - 30-minute quick start guide
- **[DEPLOYMENT.md](DEPLOYMENT.md)** - Complete deployment instructions
- **[HARDWARE_GUIDE.md](HARDWARE_GUIDE.md)** - Detailed wiring and assembly
- **[TFT_SETUP_GUIDE.md](TFT_SETUP_GUIDE.md)** - Critical display configuration
- **[PARTS_LIST.md](PARTS_LIST.md)** - Shopping list with links
- **[M3U8XM_SERVER_SETUP.md](M3U8XM_SERVER_SETUP.md)** - Raspberry Pi server setup
- **[RDS_SUPPORT.md](RDS_SUPPORT.md)** - RDS configuration and troubleshooting ⭐ NEW!
- **[SIRIUSXM_API.md](SIRIUSXM_API.md)** - API integration guide
- **[ARCHITECTURE.md](ARCHITECTURE.md)** - System design documentation
- **[PROJECT_STATUS.md](PROJECT_STATUS.md)** - Current status and roadmap

## License

MIT License - See LICENSE file for details

## Credits

- **SiriusXM API integration** based on [m3u8XM](https://github.com/myselfondiscord/m3u8XM)
- **RDS support** powered by [pu2clr/QN8066](https://github.com/pu2clr/QN8066) library
- **Audio streaming** via [ESP32-audioI2S](https://github.com/schreibfaul1/ESP32-audioI2S)
- **Display library** [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
- Built for the maker community

## Disclaimer

This project is for educational purposes. Ensure you have an active SiriusXM subscription. 
FM transmission power must comply with local regulations (typically < 100mW without license).
