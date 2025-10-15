#include "ui_manager.h"

UIManager::UIManager(TFT_eSPI* tft) : tft(tft), currentScreen(SCREEN_NONE) {}

void UIManager::begin() {
    tft->begin();
    tft->setRotation(1); // Landscape
    tft->fillScreen(COLOR_BG);
}

void UIManager::setScreen(Screen screen) {
    currentScreen = screen;
    clearScreen();
}

Screen UIManager::getCurrentScreen() {
    return currentScreen;
}

bool UIManager::checkTouch(uint16_t& x, uint16_t& y) {
    uint16_t touchX, touchY;
    bool touched = tft->getTouch(&touchX, &touchY);
    
    if (touched) {
        x = touchX;
        y = touchY;
        return true;
    }
    
    return false;
}

void UIManager::clearScreen() {
    tft->fillScreen(COLOR_BG);
}

void UIManager::drawHeader(const String& title) {
    tft->fillRect(0, 0, SCREEN_WIDTH, 30, COLOR_PRIMARY);
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(2);
    tft->setTextDatum(MC_DATUM);
    tft->drawString(title, SCREEN_WIDTH / 2, 15);
}

void UIManager::drawSplash() {
    clearScreen();
    
    // Draw SiriusXM logo text
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(3);
    tft->setTextDatum(MC_DATUM);
    tft->drawString("SiriusXM", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 20);
    
    tft->setTextSize(2);
    tft->drawString("IntraRadio", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 20);
    
    tft->setTextSize(1);
    tft->drawString("Initializing...", SCREEN_WIDTH / 2, SCREEN_HEIGHT - 30);
}

void UIManager::drawWiFiScan(const std::vector<String>& networks, int selected) {
    drawHeader("WiFi Networks");
    
    int y = 40;
    int itemHeight = 35;
    int maxVisible = 5;
    
    for (int i = 0; i < networks.size() && i < maxVisible; i++) {
        uint16_t bgColor = (i == selected) ? COLOR_PRIMARY : COLOR_DARKGRAY;
        uint16_t textColor = COLOR_WHITE;
        
        tft->fillRoundRect(10, y, SCREEN_WIDTH - 20, itemHeight - 5, 5, bgColor);
        tft->setTextColor(textColor);
        tft->setTextSize(2);
        tft->setTextDatum(ML_DATUM);
        tft->drawString(networks[i], 20, y + itemHeight / 2 - 2);
        
        y += itemHeight;
    }
    
    // Draw scroll indicator if needed
    if (networks.size() > maxVisible) {
        drawScrollbar(SCREEN_WIDTH - 8, 40, 175, networks.size(), selected, maxVisible);
    }
}

void UIManager::drawPasswordInput(const String& ssid, const String& password) {
    drawHeader("Enter Password");
    
    // Draw SSID
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(1);
    tft->setTextDatum(TL_DATUM);
    tft->drawString("Network: " + ssid, 10, 40);
    
    // Draw password field
    tft->fillRoundRect(10, 60, SCREEN_WIDTH - 20, 30, 5, COLOR_DARKGRAY);
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(2);
    tft->setTextDatum(ML_DATUM);
    
    String displayPass = "";
    for (int i = 0; i < password.length(); i++) {
        displayPass += "*";
    }
    tft->drawString(displayPass, 20, 75);
    
    // Draw keyboard
    drawKeyboard(false);
    
    // Draw connect button
    drawButton(SCREEN_WIDTH - 90, 95, 80, 30, "Connect", COLOR_GREEN);
}

void UIManager::drawSXMLogin(const String& email, const String& password, bool emailField) {
    drawHeader("SiriusXM Login");
    
    // Email field
    uint16_t emailColor = emailField ? COLOR_PRIMARY : COLOR_DARKGRAY;
    tft->fillRoundRect(10, 50, SCREEN_WIDTH - 20, 30, 5, emailColor);
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(1);
    tft->setTextDatum(TL_DATUM);
    tft->drawString("Email:", 15, 42);
    tft->setTextSize(2);
    tft->setTextDatum(ML_DATUM);
    tft->drawString(email, 20, 65);
    
    // Password field
    uint16_t passColor = !emailField ? COLOR_PRIMARY : COLOR_DARKGRAY;
    tft->fillRoundRect(10, 95, SCREEN_WIDTH - 20, 30, 5, passColor);
    tft->setTextSize(1);
    tft->setTextDatum(TL_DATUM);
    tft->drawString("Password:", 15, 87);
    
    String displayPass = "";
    for (int i = 0; i < password.length(); i++) {
        displayPass += "*";
    }
    tft->setTextSize(2);
    tft->setTextDatum(ML_DATUM);
    tft->drawString(displayPass, 20, 110);
    
    // Draw keyboard
    drawKeyboard(false);
    
    // Draw login button
    drawButton(SCREEN_WIDTH / 2 - 40, 135, 80, 30, "Login", COLOR_GREEN);
}

void UIManager::drawFMConfig(float frequency) {
    drawHeader("FM Frequency");
    
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(3);
    tft->setTextDatum(MC_DATUM);
    
    char freqStr[10];
    sprintf(freqStr, "%.1f MHz", frequency);
    tft->drawString(freqStr, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 20);
    
    // Draw - and + buttons
    drawButton(60, SCREEN_HEIGHT / 2 + 20, 60, 40, "-", COLOR_SECONDARY);
    drawButton(SCREEN_WIDTH - 120, SCREEN_HEIGHT / 2 + 20, 60, 40, "+", COLOR_SECONDARY);
    
    // Draw save button
    drawButton(SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT - 50, 80, 35, "Save", COLOR_GREEN);
}

void UIManager::drawMainScreen(const String& channelName, const String& artist) {
    clearScreen();
    
    // Draw SXM logo area (touchable)
    tft->fillRoundRect(10, 10, 140, 80, 10, COLOR_PRIMARY);
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(2);
    tft->setTextDatum(MC_DATUM);
    tft->drawString("SiriusXM", 80, 50);
    
    // Draw current channel info
    tft->fillRoundRect(160, 10, SCREEN_WIDTH - 170, 80, 10, COLOR_DARKGRAY);
    tft->setTextSize(2);
    tft->setTextDatum(MC_DATUM);
    tft->drawString(channelName, 240, 35);
    
    if (artist.length() > 0) {
        tft->setTextSize(1);
        tft->drawString(artist, 240, 60);
    }
    
    // Draw internet radio button
    drawButton(10, 100, 140, 60, "Internet\nRadio", COLOR_SECONDARY);
    
    // Draw settings button
    drawButton(160, 100, 140, 60, "Settings", COLOR_SECONDARY);
    
    // Draw status bar at bottom
    tft->fillRect(0, SCREEN_HEIGHT - 20, SCREEN_WIDTH, 20, COLOR_DARKGRAY);
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(1);
    tft->setTextDatum(MC_DATUM);
    tft->drawString("Touch SiriusXM logo to change channels", SCREEN_WIDTH / 2, SCREEN_HEIGHT - 10);
}

void UIManager::drawChannelList(const std::vector<String>& channels, int selected, int offset) {
    drawHeader("Select Channel");
    
    int y = 40;
    int itemHeight = 40;
    int maxVisible = 4;
    
    for (int i = offset; i < channels.size() && i < offset + maxVisible; i++) {
        uint16_t bgColor = (i == selected) ? COLOR_PRIMARY : COLOR_DARKGRAY;
        uint16_t textColor = COLOR_WHITE;
        
        tft->fillRoundRect(10, y, SCREEN_WIDTH - 20, itemHeight - 5, 5, bgColor);
        tft->setTextColor(textColor);
        tft->setTextSize(2);
        tft->setTextDatum(ML_DATUM);
        tft->drawString(channels[i], 20, y + itemHeight / 2);
        
        y += itemHeight;
    }
    
    // Draw scrollbar
    if (channels.size() > maxVisible) {
        drawScrollbar(SCREEN_WIDTH - 8, 40, 160, channels.size(), selected, maxVisible);
    }
    
    // Draw back button
    drawButton(10, SCREEN_HEIGHT - 40, 80, 35, "Back", COLOR_SECONDARY);
}

void UIManager::drawSettings() {
    drawHeader("Settings");
    
    int y = 50;
    int itemHeight = 45;
    
    const char* menuItems[] = {
        "WiFi Settings",
        "SXM Credentials",
        "FM Frequency",
        "About"
    };
    
    for (int i = 0; i < 4; i++) {
        tft->fillRoundRect(10, y, SCREEN_WIDTH - 20, itemHeight - 5, 5, COLOR_DARKGRAY);
        tft->setTextColor(COLOR_WHITE);
        tft->setTextSize(2);
        tft->setTextDatum(ML_DATUM);
        tft->drawString(menuItems[i], 20, y + itemHeight / 2);
        
        // Draw arrow
        tft->drawString(">", SCREEN_WIDTH - 30, y + itemHeight / 2);
        
        y += itemHeight;
    }
    
    // Draw back button
    drawButton(10, SCREEN_HEIGHT - 40, 80, 35, "Back", COLOR_SECONDARY);
}

void UIManager::drawLoading(const String& message) {
    clearScreen();
    
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(2);
    tft->setTextDatum(MC_DATUM);
    tft->drawString(message, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    
    // Draw simple spinner
    static uint8_t angle = 0;
    for (int i = 0; i < 8; i++) {
        float a = (angle + i * 45) * 0.0174533;
        int x1 = SCREEN_WIDTH / 2 + cos(a) * 20;
        int y1 = SCREEN_HEIGHT / 2 + 30 + sin(a) * 20;
        int x2 = SCREEN_WIDTH / 2 + cos(a) * 30;
        int y2 = SCREEN_HEIGHT / 2 + 30 + sin(a) * 30;
        uint16_t color = tft->color565(255 - i * 30, 255 - i * 30, 255 - i * 30);
        tft->drawLine(x1, y1, x2, y2, color);
    }
    angle += 10;
}

void UIManager::drawButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const String& text, uint16_t color, bool pressed) {
    uint16_t bgColor = pressed ? tft->color565(color >> 1, (color >> 1) & 0x3F, color & 0x1F) : color;
    
    tft->fillRoundRect(x, y, w, h, 5, bgColor);
    tft->drawRoundRect(x, y, w, h, 5, COLOR_WHITE);
    
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(2);
    tft->setTextDatum(MC_DATUM);
    tft->drawString(text, x + w / 2, y + h / 2);
}

void UIManager::drawKeyboard(bool uppercase) {
    int startY = 170;
    int keyW = 26;
    int keyH = 22;
    int gap = 2;
    
    const char* rows[] = {
        "1234567890",
        "QWERTYUIOP",
        "ASDFGHJKL",
        "ZXCVBNM"
    };
    
    if (!uppercase) {
        rows[1] = "qwertyuiop";
        rows[2] = "asdfghjkl";
        rows[3] = "zxcvbnm";
    }
    
    for (int row = 0; row < 4; row++) {
        int len = strlen(rows[row]);
        int startX = (SCREEN_WIDTH - (len * (keyW + gap))) / 2;
        
        for (int col = 0; col < len; col++) {
            int x = startX + col * (keyW + gap);
            int y = startY + row * (keyH + gap);
            
            tft->fillRoundRect(x, y, keyW, keyH, 3, COLOR_DARKGRAY);
            tft->setTextColor(COLOR_WHITE);
            tft->setTextSize(1);
            tft->setTextDatum(MC_DATUM);
            
            char c[2] = {rows[row][col], '\0'};
            tft->drawString(c, x + keyW / 2, y + keyH / 2);
        }
    }
    
    // Space bar
    int spaceX = 80;
    int spaceY = startY + 4 * (keyH + gap);
    tft->fillRoundRect(spaceX, spaceY, 160, keyH, 3, COLOR_DARKGRAY);
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(1);
    tft->setTextDatum(MC_DATUM);
    tft->drawString("SPACE", spaceX + 80, spaceY + keyH / 2);
    
    // Backspace
    int bkspX = 10;
    tft->fillRoundRect(bkspX, spaceY, 60, keyH, 3, COLOR_RED);
    tft->drawString("<-", bkspX + 30, spaceY + keyH / 2);
}

char UIManager::getKeyboardPress(uint16_t x, uint16_t y, bool uppercase) {
    int startY = 170;
    int keyW = 26;
    int keyH = 22;
    int gap = 2;
    
    const char* rows[] = {
        "1234567890",
        "QWERTYUIOP",
        "ASDFGHJKL",
        "ZXCVBNM"
    };
    
    if (!uppercase) {
        rows[1] = "qwertyuiop";
        rows[2] = "asdfghjkl";
        rows[3] = "zxcvbnm";
    }
    
    // Check main keys
    for (int row = 0; row < 4; row++) {
        int len = strlen(rows[row]);
        int startX = (SCREEN_WIDTH - (len * (keyW + gap))) / 2;
        
        for (int col = 0; col < len; col++) {
            int kx = startX + col * (keyW + gap);
            int ky = startY + row * (keyH + gap);
            
            if (x >= kx && x <= kx + keyW && y >= ky && y <= ky + keyH) {
                return rows[row][col];
            }
        }
    }
    
    // Check space bar
    int spaceY = startY + 4 * (keyH + gap);
    if (x >= 80 && x <= 240 && y >= spaceY && y <= spaceY + keyH) {
        return ' ';
    }
    
    // Check backspace
    if (x >= 10 && x <= 70 && y >= spaceY && y <= spaceY + keyH) {
        return '\b'; // Backspace
    }
    
    return '\0';
}

void UIManager::showMessage(const String& title, const String& message, uint16_t duration) {
    // Draw message box
    int boxW = 260;
    int boxH = 100;
    int boxX = (SCREEN_WIDTH - boxW) / 2;
    int boxY = (SCREEN_HEIGHT - boxH) / 2;
    
    tft->fillRoundRect(boxX, boxY, boxW, boxH, 10, COLOR_DARKGRAY);
    tft->drawRoundRect(boxX, boxY, boxW, boxH, 10, COLOR_WHITE);
    
    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(2);
    tft->setTextDatum(MC_DATUM);
    tft->drawString(title, SCREEN_WIDTH / 2, boxY + 25);
    
    tft->setTextSize(1);
    tft->drawString(message, SCREEN_WIDTH / 2, boxY + 55);
    
    delay(duration);
}

void UIManager::drawProgress(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t percent) {
    tft->drawRoundRect(x, y, w, h, 3, COLOR_WHITE);
    
    uint16_t fillW = (w - 4) * percent / 100;
    tft->fillRoundRect(x + 2, y + 2, fillW, h - 4, 2, COLOR_GREEN);
}

void UIManager::drawScrollbar(uint16_t x, uint16_t y, uint16_t h, int total, int current, int visible) {
    tft->fillRect(x, y, 4, h, COLOR_DARKGRAY);
    
    int thumbH = (h * visible) / total;
    int thumbY = y + (h * current) / total;
    
    tft->fillRect(x, thumbY, 4, thumbH, COLOR_WHITE);
}
