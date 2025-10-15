# System Architecture

## Overview

The ESP32 SiriusXM IntraRadio is a standalone device that streams internet radio and SiriusXM to your car's FM radio. This document explains the system architecture and data flow.

---

## High-Level Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                         Car Environment                          │
│                                                                   │
│  ┌──────────────┐         ┌─────────────┐        ┌────────────┐ │
│  │ Phone WiFi   │◄────────┤   ESP32     ├────────► Car Radio  │ │
│  │ Hotspot      │ WiFi    │   Device    │   FM   │ (Receiver) │ │
│  └──────────────┘         └─────────────┘        └────────────┘ │
│                                 │                                 │
│                                 │ USB-C                           │
│                           ┌─────▼──────┐                         │
│                           │ Car USB    │                         │
│                           │ Charger    │                         │
│                           └────────────┘                         │
└─────────────────────────────────────────────────────────────────┘
```

---

## Hardware Architecture

```
                        ESP32 Development Board
                    ┌───────────────────────────┐
                    │                           │
                    │   ESP32-WROOM-32         │
                    │   Dual Core 240MHz       │
                    │   4MB Flash              │
                    │                           │
                    └───────────┬───────────────┘
                                │
        ┌───────────────────────┼───────────────────────┐
        │                       │                       │
   ┌────▼─────┐          ┌─────▼──────┐         ┌─────▼──────┐
   │  ILI9341 │          │  PCM5102   │         │  QN8066    │
   │ Display  │          │  I2S DAC   │         │ FM TX      │
   │          │          │            │         │            │
   │ Touch    │          │ Audio Out  │         │ Antenna    │
   └──────────┘          └────┬───────┘         └────────────┘
                              │                       ▲
                              └───────────────────────┘
                              Analog Audio Connection
```

---

## Software Architecture

### Component Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                         Main Application                         │
│                            (main.cpp)                            │
│                                                                   │
│  ┌────────────┐  ┌────────────┐  ┌────────────┐  ┌───────────┐ │
│  │   WiFi     │  │  SiriusXM  │  │   Audio    │  │    FM     │ │
│  │  Manager   │  │   Client   │  │   Player   │  │Transmitter│ │
│  └─────┬──────┘  └─────┬──────┘  └─────┬──────┘  └─────┬─────┘ │
│        │               │               │               │         │
│        │               │               │               │         │
│  ┌─────▼───────────────▼───────────────▼───────────────▼─────┐  │
│  │                    UI Manager                              │  │
│  │                (Touch & Display)                           │  │
│  └────────────────────────────────────────────────────────────┘  │
│                                │                                  │
│                    ┌───────────▼────────────┐                    │
│                    │   Settings Manager     │                    │
│                    │   (NVS Storage)        │                    │
│                    └────────────────────────┘                    │
└─────────────────────────────────────────────────────────────────┘
```

### Module Dependencies

```
main.cpp
├── ui_manager.h
│   ├── TFT_eSPI (library)
│   └── config.h
├── wifi_manager.h
│   └── WiFi (Arduino)
├── sxm_client.h
│   ├── HTTPClient (Arduino)
│   └── ArduinoJson (library)
├── audio_player.h
│   └── Audio (ESP32-audioI2S library)
├── fm_transmitter.h
│   └── Wire (I2C)
├── settings.h
│   └── Preferences (Arduino)
└── radio_stations.h
```

---

## Data Flow Architecture

### Startup Sequence

```
1. Power On
   │
   ├──> Initialize Display
   │    └──> Show Splash Screen
   │
   ├──> Load Settings (NVS)
   │    ├──> WiFi Credentials
   │    ├──> SXM Credentials
   │    ├──> FM Frequency
   │    └──> Last Channel
   │
   ├──> Initialize FM Transmitter
   │    └──> Set Frequency
   │
   ├──> Initialize Audio Player
   │    └──> Configure I2S
   │
   ├──> Check First Run
   │    │
   │    ├──> [First Run] ──> Setup Wizard
   │    │                    ├──> WiFi Setup
   │    │                    ├──> SXM Login
   │    │                    └──> FM Config
   │    │
   │    └──> [Not First] ──> Auto Connect
   │                         ├──> WiFi
   │                         ├──> SXM Login
   │                         ├──> Load Channels
   │                         └──> Resume Last Channel
   │
   └──> Main Loop
```

### Audio Streaming Flow

```
User Selects Channel
        │
        ├──> SXM Client: Get Stream URL
        │    │
        │    ├──> API Request (HTTPS)
        │    ├──> Parse JSON Response
        │    └──> Return HLS URL
        │
        ├──> Audio Player: Start Stream
        │    │
        │    ├──> Download m3u8 Playlist
        │    ├──> Select Best Quality
        │    ├──> Download Audio Segments
        │    ├──> Decode (AAC/MP3)
        │    └──> Output via I2S
        │
        ├──> I2S DAC: Convert to Analog
        │    │
        │    └──> Analog Audio Signal
        │
        └──> FM Transmitter: Broadcast
             │
             ├──> Modulate on FM Frequency
             └──> Transmit via Antenna
                  │
                  └──> Car Radio Receives
```

### UI Interaction Flow

```
Touch Event
     │
     ├──> Read Touch Coordinates (x, y)
     │
     ├──> Determine Current Screen
     │    │
     │    ├──> [Main Screen]
     │    │    ├──> SXM Logo Touched ──> Go to Channel List
     │    │    ├──> Settings Touched ──> Go to Settings
     │    │    └──> Internet Radio ────> Go to Radio List
     │    │
     │    ├──> [Channel List]
     │    │    ├──> Channel Touched ───> Play Channel
     │    │    └──> Back Touched ──────> Main Screen
     │    │
     │    ├──> [Settings]
     │    │    ├──> WiFi Touched ──────> WiFi Setup
     │    │    ├──> SXM Touched ───────> SXM Login
     │    │    ├──> FM Touched ────────> FM Config
     │    │    └──> Back Touched ──────> Main Screen
     │    │
     │    └──> [Keyboard]
     │         ├──> Key Pressed ───────> Add to Buffer
     │         ├──> Backspace ─────────> Remove Last Char
     │         └──> Submit ────────────> Process Input
     │
     └──> Update Display
```

---

## State Machine

### Application States

```
┌─────────────┐
│   INIT      │ (Startup)
└──────┬──────┘
       │
       ├──> First Run?
       │    │
       │    ├──[Yes]──> ┌──────────────┐
       │    │           │ WIFI_SETUP   │
       │    │           └──────┬───────┘
       │    │                  │
       │    │           ┌──────▼───────┐
       │    │           │  SXM_SETUP   │
       │    │           └──────┬───────┘
       │    │                  │
       │    │           ┌──────▼───────┐
       │    │           │  FM_SETUP    │
       │    │           └──────┬───────┘
       │    │                  │
       │    └──[No]────────────┘
       │                       │
       │               ┌───────▼───────┐
       └───────────────►     MAIN      │◄──────────┐
                       └───┬───────┬───┘           │
                           │       │               │
                   ┌───────┘       └────────┐      │
                   │                        │      │
           ┌───────▼────────┐      ┌────────▼──────┴──┐
           │ CHANNEL_SELECT │      │    SETTINGS      │
           └───────┬────────┘      └──────────────────┘
                   │
                   └────> Play Channel
                          │
                          └────> Return to MAIN
```

### State Transitions

| From State | Event | To State |
|------------|-------|----------|
| INIT | First run detected | WIFI_SETUP |
| INIT | Has credentials | MAIN |
| WIFI_SETUP | WiFi connected | SXM_SETUP |
| SXM_SETUP | Login success | FM_SETUP |
| FM_SETUP | Frequency set | MAIN |
| MAIN | Touch SXM logo | CHANNEL_SELECT |
| MAIN | Touch Settings | SETTINGS |
| CHANNEL_SELECT | Touch channel | Play & return to MAIN |
| CHANNEL_SELECT | Touch Back | MAIN |
| SETTINGS | Touch WiFi | WIFI_SETUP |
| SETTINGS | Touch SXM | SXM_SETUP |
| SETTINGS | Touch FM | FM_SETUP |
| SETTINGS | Touch Back | MAIN |

---

## Memory Architecture

### ESP32 Memory Layout

```
┌─────────────────────────────────────────┐
│         Flash Memory (4MB)              │
├─────────────────────────────────────────┤
│  App Partition (~2MB)                   │
│    - main.cpp compiled code             │
│    - Libraries                          │
│    - Static data                        │
├─────────────────────────────────────────┤
│  OTA Partition (~2MB)                   │
│    - Reserved for updates               │
├─────────────────────────────────────────┤
│  NVS (Non-Volatile Storage) (20KB)      │
│    - WiFi credentials                   │
│    - SiriusXM credentials               │
│    - Settings                           │
│    - Last channel                       │
├─────────────────────────────────────────┤
│  SPIFFS/LittleFS (optional)             │
│    - Logos, images (if needed)          │
└─────────────────────────────────────────┘

┌─────────────────────────────────────────┐
│         SRAM (520KB)                    │
├─────────────────────────────────────────┤
│  Stack (16KB per task)                  │
│    - Main task                          │
│    - Audio task                         │
│    - WiFi task                          │
├─────────────────────────────────────────┤
│  Heap (remaining ~350KB)                │
│    - Audio buffers (largest)            │
│    - Network buffers                    │
│    - UI framebuffer (partial)           │
│    - Dynamic allocations                │
└─────────────────────────────────────────┘
```

### Memory Usage Estimates

| Component | RAM Usage | Flash Usage |
|-----------|-----------|-------------|
| ESP32 Core | 50KB | 200KB |
| WiFi Stack | 70KB | 150KB |
| TFT_eSPI | 20KB | 50KB |
| Audio Library | 150KB | 300KB |
| HTTP Client | 30KB | 80KB |
| ArduinoJson | 10KB | 20KB |
| Application Code | 50KB | 200KB |
| **Total** | **~380KB** | **~1MB** |

---

## Communication Protocols

### SPI Bus (Display)

```
ESP32 (Master)  ──SPI──►  ILI9341 Display (Slave)
                          XPT2046 Touch (Slave)

Signals:
├── MOSI: ESP32 → Display (Data)
├── MISO: Display → ESP32 (Touch data)
├── SCK:  ESP32 → Display (Clock, ~27MHz)
├── CS:   ESP32 → Display (Chip Select)
├── T_CS: ESP32 → Touch (Touch Chip Select)
└── DC:   ESP32 → Display (Data/Command)
```

### I2C Bus (FM Transmitter)

```
ESP32 (Master)  ──I2C──►  QN8066 (Slave, 0x21)

Signals:
├── SDA: Bidirectional data
└── SCL: Clock (~100kHz)

Operations:
├── Write Register: Set frequency, power, mode
└── Read Register:  Get status, signal level
```

### I2S Bus (Audio)

```
ESP32 (Master)  ──I2S──►  PCM5102 DAC (Slave)

Signals:
├── BCK:  Bit Clock (~2.8MHz for 44.1kHz audio)
├── LRC:  Left/Right Clock (Word Select, 44.1kHz)
└── DIN:  Serial Audio Data

Format: 16-bit stereo PCM
```

### WiFi (Radio)

```
ESP32 ◄──WiFi──► Phone Hotspot ◄──4G/5G──► Internet
                                              │
                  ┌───────────────────────────┤
                  │                           │
          ┌───────▼────────┐         ┌────────▼────────┐
          │  SiriusXM API  │         │ Radio Stations  │
          │  (HTTPS)       │         │ (HTTP/HTTPS)    │
          └────────────────┘         └─────────────────┘
```

---

## Thread/Task Architecture

### FreeRTOS Tasks

```
┌─────────────────────────────────────────────┐
│              Task Schedule                   │
├─────────────────────────────────────────────┤
│                                              │
│  Main Task (Priority: 1)                    │
│  ├── UI rendering                           │
│  ├── Touch handling                         │
│  └── State management                       │
│                                              │
│  Audio Task (Priority: 10) [High Priority]  │
│  ├── Stream downloading                     │
│  ├── Audio decoding                         │
│  └── I2S output                             │
│                                              │
│  WiFi Task (Priority: 5)                    │
│  ├── Connection management                  │
│  ├── HTTP requests                          │
│  └── HTTPS handling                         │
│                                              │
│  Idle Task (Priority: 0)                    │
│  └── Sleep/power management                 │
│                                              │
└─────────────────────────────────────────────┘
```

### Task Communication

```
Main Task
    │
    ├──[Queue]──► Audio Task
    │             └──► Play/Stop/Pause commands
    │
    ├──[Event]──► WiFi Task
    │             └──► Connect/Disconnect requests
    │
    └──[Shared Memory]──► Settings
                           └──► Read/Write settings (mutex protected)
```

---

## Power Architecture

### Power Flow

```
Car USB Port (12V DC)
        │
        ▼
┌───────────────┐
│ Car Charger   │
│ DC-DC Buck    │
│ 12V → 5V      │
└───────┬───────┘
        │ 5V @ 2A
        ▼
┌───────────────┐
│ ESP32 VIN     │
│ LDO Regulator │
│ 5V → 3.3V     │
└───────┬───────┘
        │
        ├─────────────────────┬──────────────────┐
        │ 3.3V                │ 3.3V             │ 5V
        ▼                     ▼                  ▼
┌───────────┐         ┌──────────────┐    ┌─────────┐
│ ILI9341   │         │   QN8066     │    │PCM5102  │
│ ~150mA    │         │   ~80mA      │    │ ~20mA   │
└───────────┘         └──────────────┘    └─────────┘
```

### Power Consumption

| State | Power Draw | Battery Life (10,000mAh) |
|-------|------------|--------------------------|
| Idle (WiFi on) | ~200mA @ 5V | ~10 hours |
| Streaming | ~700mA @ 5V | ~2.8 hours |
| Peak (startup) | ~1A @ 5V | ~2 hours |

---

## Network Architecture

### Data Flow

```
Internet
    │
    ▼
┌─────────────────┐
│ SiriusXM CDN    │ (HTTPS)
│ or Radio Server │
└────────┬────────┘
         │ HLS Stream
         │ (m3u8 + AAC segments)
         ▼
┌─────────────────┐
│ Phone 4G/5G     │
└────────┬────────┘
         │ WiFi Hotspot (2.4GHz)
         ▼
┌─────────────────┐
│ ESP32 WiFi      │
└────────┬────────┘
         │ Internal
         ▼
┌─────────────────┐
│ Audio Library   │ Decode
└────────┬────────┘
         │ PCM Audio
         ▼
┌─────────────────┐
│ I2S Output      │
└────────┬────────┘
         │ Digital Audio
         ▼
┌─────────────────┐
│ PCM5102 DAC     │
└────────┬────────┘
         │ Analog Audio
         ▼
┌─────────────────┐
│ QN8066 FM TX    │
└────────┬────────┘
         │ FM Radio
         ▼
     Car Stereo
```

---

## File System Architecture

Currently no filesystem is used. All configuration stored in NVS (key-value pairs).

**Future Enhancement:** Use LittleFS for:
- Channel logos
- Album art cache
- Station presets
- Configuration backup

---

## Error Handling Architecture

### Error Flow

```
Error Occurs
    │
    ├──► Log to Serial (debug)
    │
    ├──► Store error code
    │
    ├──► Display error message
    │    └──► UI: Show error popup
    │
    ├──► Attempt recovery
    │    ├──► WiFi error → Reconnect
    │    ├──► Stream error → Retry
    │    └──► Hardware error → Reset
    │
    └──► Critical error → Restart device
```

### Recovery Strategies

| Error Type | Recovery Action | Fallback |
|------------|----------------|----------|
| WiFi disconnect | Auto-reconnect (3 attempts) | Show setup screen |
| Stream timeout | Retry URL (2 attempts) | Skip to next segment |
| SXM auth fail | Re-login with stored creds | Show login screen |
| FM TX fail | Reinitialize I2C | Display warning |
| Display glitch | Reinit display | Soft reset |

---

## Security Architecture

### Credential Storage

```
ESP32 NVS (Encrypted)
├── WiFi SSID (plain text)
├── WiFi Password (encrypted)
├── SXM Email (plain text)
└── SXM Password (encrypted)

NVS Namespace: "sxm_radio"
Encryption: ESP32 Flash Encryption (optional)
```

### Network Security

- All SiriusXM API calls use HTTPS
- WiFi password never logged
- No credentials in source code
- Token refresh before expiry

---

## Scalability Considerations

**Current Limitations:**
- Single device, single user
- No cloud backend
- Fixed station list

**Possible Enhancements:**
- Multi-device sync via MQTT
- Cloud-based favorites
- Shared family presets
- Remote control via app

---

This architecture provides a robust, maintainable foundation for the ESP32 SiriusXM IntraRadio project.
