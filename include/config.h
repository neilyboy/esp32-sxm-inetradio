#ifndef CONFIG_H
#define CONFIG_H

// Pin Definitions for ILI9341 Display
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  4
#define TOUCH_CS 21

// Pin Definitions for I2C (FM Transmitter)
#define I2C_SDA 21
#define I2C_SCL 22

// Pin Definitions for I2S Audio
#define I2S_BCLK 25
#define I2S_LRC  26
#define I2S_DOUT 27

// Display Settings
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

// FM Transmitter Settings
#define FM_MIN_FREQ 87.5
#define FM_MAX_FREQ 108.0
#define FM_DEFAULT_FREQ 88.1

// Audio Settings
#define AUDIO_BUFFER_SIZE 1024
#define AUDIO_SAMPLE_RATE 44100

// Network Settings
#define WIFI_TIMEOUT_MS 20000
#define HTTP_TIMEOUT_MS 10000

// Storage Keys
#define PREF_NAMESPACE "sxm_radio"
#define KEY_WIFI_SSID "wifi_ssid"
#define KEY_WIFI_PASS "wifi_pass"
#define KEY_SXM_EMAIL "sxm_email"
#define KEY_SXM_PASS "sxm_pass"
#define KEY_FM_FREQ "fm_freq"
#define KEY_LAST_CHANNEL "last_ch"

// UI Colors (RGB565)
#define COLOR_BLACK       0x0000
#define COLOR_WHITE       0xFFFF
#define COLOR_RED         0xF800
#define COLOR_GREEN       0x07E0
#define COLOR_BLUE        0x001F
#define COLOR_CYAN        0x07FF
#define COLOR_MAGENTA     0xF81F
#define COLOR_YELLOW      0xFFE0
#define COLOR_ORANGE      0xFD20
#define COLOR_GRAY        0x8410
#define COLOR_DARKGRAY    0x4208
#define COLOR_LIGHTGRAY   0xC618
#define COLOR_BG          0x18E3  // Dark blue background
#define COLOR_PRIMARY     0x0438  // SXM blue
#define COLOR_SECONDARY   0x8410  // Gray

// API Endpoints
#define SXM_LOGIN_URL "https://player.siriusxm.com/rest-api/2/experience/modules/get/login"
#define SXM_CHANNEL_URL "https://player.siriusxm.com/rest-api/2/experience/modules/get/channel-player"

#endif // CONFIG_H
