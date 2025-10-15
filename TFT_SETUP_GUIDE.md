# TFT_eSPI Configuration Guide

## Important: Library Configuration

The `TFT_eSPI` library requires manual configuration to match your hardware setup. Follow these steps carefully.

## Step 1: Locate TFT_eSPI Library

After installing the library via PlatformIO, find it at:
```
~/.platformio/packages/framework-arduinoespressif32/libraries/TFT_eSPI/
```

Or in your project:
```
.pio/libdeps/esp32dev/TFT_eSPI/
```

## Step 2: Edit User_Setup.h

Navigate to the TFT_eSPI library folder and edit `User_Setup.h`:

### Option A: Use User_Setup.h (Recommended)

Replace the contents of `User_Setup.h` with:

```cpp
// ESP32 SiriusXM IntraRadio Display Configuration
// For 2.8" ILI9341 Display (320x240)

#define USER_SETUP_INFO "ESP32_SXM_Radio"

// Define to use specific driver
#define ILI9341_DRIVER

// ESP32 Pin Configuration
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  4

// Touch controller (XPT2046)
#define TOUCH_CS 21

// Font settings
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts

#define SMOOTH_FONT

// SPI Frequency
#define SPI_FREQUENCY  27000000  // 27MHz for ESP32
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000

// Color depth
// #define TFT_RGB_ORDER TFT_RGB  // Colour order Red-Green-Blue
// #define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

// Uncomment if colors are inverted
// #define TFT_INVERSION_ON
// #define TFT_INVERSION_OFF
```

### Option B: Use User_Setup_Select.h (Alternative)

If you prefer to use a predefined setup:

1. Edit `User_Setup_Select.h`
2. Comment out the default setup line:
   ```cpp
   // #include <User_Setup.h>
   ```
3. Uncomment a setup that matches your display, or create a custom one

## Step 3: Touch Calibration

The touch controller may need calibration. Add this code to test:

```cpp
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_WHITE);
    tft.println("Touch corners to calibrate:");
    tft.println("Top-left, Top-right");
    tft.println("Bottom-right, Bottom-left");
    
    // Calibrate
    uint16_t calData[5];
    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
    
    Serial.println("Calibration complete!");
    Serial.println("Add these values to your code:");
    Serial.print("uint16_t calData[5] = {");
    for (int i = 0; i < 5; i++) {
        Serial.print(calData[i]);
        if (i < 4) Serial.print(", ");
    }
    Serial.println("};");
}

void loop() {
    uint16_t x, y;
    if (tft.getTouch(&x, &y)) {
        tft.fillCircle(x, y, 3, TFT_WHITE);
        Serial.printf("Touch at: %d, %d\n", x, y);
    }
}
```

After calibration, add the calibration data to your main code:

```cpp
void setup() {
    tft.init();
    tft.setRotation(1);
    
    // Set calibration data
    uint16_t calData[5] = {275, 3620, 264, 3532, 1}; // Your values here
    tft.setTouch(calData);
}
```

## Step 4: Display Orientation

The display can be rotated using `setRotation()`:

```cpp
tft.setRotation(0);  // Portrait (USB on bottom)
tft.setRotation(1);  // Landscape (USB on right)
tft.setRotation(2);  // Portrait inverted (USB on top)
tft.setRotation(3);  // Landscape inverted (USB on left)
```

For this project, we use **rotation 1** (landscape).

## Troubleshooting

### Display shows but colors are wrong

Try toggling color order:
```cpp
#define TFT_RGB_ORDER TFT_BGR  // or TFT_RGB
```

### Display is inverted

Enable/disable inversion:
```cpp
#define TFT_INVERSION_ON  // or TFT_INVERSION_OFF
```

### Display is white/blank

1. Check power connections (3.3V)
2. Verify SPI pins match your wiring
3. Try lower SPI frequency (20MHz)
4. Test with TFT_eSPI examples first

### Touch not working

1. Verify TOUCH_CS pin (GPIO 21)
2. Check that MISO is connected
3. Run touch calibration
4. Try different TOUCH_CS pin if needed

### Compilation errors

Make sure you're using TFT_eSPI version 2.5.0 or higher:
```ini
lib_deps = 
    bodmer/TFT_eSPI@^2.5.43
```

## Alternative: Custom User_Setup file

You can create a custom setup file in your project:

1. Create `lib/TFT_eSPI_Config/User_Setup.h` in your project
2. Add to `platformio.ini`:
   ```ini
   build_flags = 
       -D USER_SETUP_LOADED=1
       -I lib/TFT_eSPI_Config
   ```

This prevents modifications to the library itself.

## Verification Test

Use this simple test to verify your setup:

```cpp
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
    Serial.begin(115200);
    Serial.println("TFT Test");
    
    tft.init();
    tft.setRotation(1);
    
    // Test colors
    tft.fillScreen(TFT_RED);
    delay(1000);
    tft.fillScreen(TFT_GREEN);
    delay(1000);
    tft.fillScreen(TFT_BLUE);
    delay(1000);
    
    // Test text
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.println("Display OK!");
    tft.println("Resolution:");
    tft.print(tft.width());
    tft.print(" x ");
    tft.println(tft.height());
    
    Serial.println("Display test complete");
}

void loop() {
    // Touch test
    uint16_t x, y;
    if (tft.getTouch(&x, &y)) {
        tft.fillCircle(x, y, 5, TFT_YELLOW);
        Serial.printf("Touch: %d, %d\n", x, y);
    }
    delay(10);
}
```

Expected output:
- Display should show colors correctly
- Resolution should be 320 x 240
- Touch should draw yellow circles where you touch

If all tests pass, your display is correctly configured!
