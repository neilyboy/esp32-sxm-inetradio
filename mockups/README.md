# UI Mockups - ESP32 SiriusXM IntraRadio

This folder contains **HTML mockups** of the ESP32 touchscreen interface.

## 📱 What Are These?

These HTML files replicate the actual 320x240 ILI9341 TFT display interface shown on the ESP32 device. They're created by reading the drawing code from `src/ui_manager.cpp` and `include/config.h`.

## ✅ Accuracy

- **Dimensions:** Exact 320x240 pixels
- **Colors:** RGB565 codes converted from actual config
- **Layout:** Positions match TFT drawing code
- **Fonts:** Approximate (web fonts differ from TFT fonts)

## 🖼️ Available Screens

1. **01_splash_screen.html** - Boot/initialization screen
2. **02_wifi_scan.html** - WiFi network selection
3. **03_sxm_login.html** - SiriusXM credentials entry
4. **04_fm_config.html** - FM frequency configuration
5. **05_main_screen.html** - Main playback screen (with RDS indicator)
6. **06_channel_list.html** - Channel browser
7. **07_settings.html** - Settings menu

## 🎯 How to Use

### View Locally
```bash
cd mockups
open index.html  # Mac
xdg-open index.html  # Linux
start index.html  # Windows
```

Or open in browser: `file:///path/to/esp32-sxm-intradio/mockups/index.html`

### Take Screenshots

1. Open any mockup HTML file
2. Press **F11** for fullscreen (or use browser screenshot tool)
3. Take screenshot of just the 320x240 device area
4. Use for GitHub README, documentation, etc.

**Browser Screenshot Tools:**
- **Chrome/Edge:** Ctrl+Shift+P → "Capture screenshot"
- **Firefox:** Ctrl+Shift+S
- **Safari:** Develop → Take Screenshot

## 📸 Screenshot Tips

**Best results:**
- Use 100% browser zoom (not zoomed in/out)
- Crop to just the device (320x240 black border)
- Save as PNG for best quality
- Consider adding drop shadow in image editor

**Recommended dimensions for GitHub:**
- Individual screens: 320x240px (native)
- Overview grid: 640x480px (2x scaled)
- Banner image: 1200x400px with multiple screens

## 🎨 Color Reference

From `include/config.h`:

```cpp
#define COLOR_BG          0x18E3  // Dark blue background: rgb(24, 56, 99)
#define COLOR_PRIMARY     0x0438  // SXM blue: rgb(8, 28, 56)
#define COLOR_DARKGRAY    0x4208  // Dark gray: rgb(66, 68, 136)
#define COLOR_SECONDARY   0x8410  // Gray: rgb(132, 16, 32)
```

## ⚠️ Limitations

**These mockups are NOT:**
- ❌ Interactive (no touch response)
- ❌ Connected to real data (static content)
- ❌ Perfect font matches (TFT uses different fonts)
- ❌ Animated (static screenshots only)

**They ARE:**
- ✅ Accurate layout representations
- ✅ Correct dimensions and colors
- ✅ Easy to screenshot for documentation
- ✅ Based on actual code (not guesses)

## 🔄 Updating Mockups

If you modify `src/ui_manager.cpp` or `include/config.h`, you may want to update these mockups to match.

**What to update:**
- Colors: Check `COLOR_*` defines in config.h
- Layouts: Check drawing coordinates in ui_manager.cpp
- Text: Update content to match actual display

## 📦 Using in GitHub

Add screenshots to your README:

```markdown
## User Interface

![Main Screen](mockups/screenshots/main_screen.png)
![Channel List](mockups/screenshots/channel_list.png)

Or create a gallery:

<p align="center">
  <img src="mockups/screenshots/splash.png" width="200"/>
  <img src="mockups/screenshots/wifi.png" width="200"/>
  <img src="mockups/screenshots/main.png" width="200"/>
</p>
```

## 🛠️ Technical Details

**How mockups were created:**

1. Read `src/ui_manager.cpp` to understand:
   - Screen layouts
   - Text positions
   - Button sizes
   - Color usage

2. Read `include/config.h` for:
   - RGB565 color codes
   - Screen dimensions
   - Constants

3. Convert RGB565 to RGB888:
   ```
   R = (rgb565 >> 11) & 0x1F  → * 255 / 31
   G = (rgb565 >> 5) & 0x3F   → * 255 / 63
   B = (rgb565) & 0x1F        → * 255 / 31
   ```

4. Recreate layout in HTML/CSS with exact pixel positions

## 🆘 Support

These mockups are **approximations** of the actual display. If you find inaccuracies:

1. Check the actual code in `src/ui_manager.cpp`
2. Test on real hardware if possible
3. Update HTML mockups to match
4. Submit a PR if you improve accuracy!

---

**Generated from:** `src/ui_manager.cpp` + `include/config.h`  
**Display:** 320x240 ILI9341 TFT Touchscreen  
**Purpose:** Documentation, screenshots, previews
