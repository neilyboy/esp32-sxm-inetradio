#include "settings.h"
#include "config.h"

Settings::Settings() {}

Settings::~Settings() {
    preferences.end();
}

bool Settings::begin() {
    return preferences.begin(PREF_NAMESPACE, false);
}

void Settings::reset() {
    preferences.clear();
}

// WiFi Settings
bool Settings::hasWiFiCredentials() {
    return preferences.isKey(KEY_WIFI_SSID) && preferences.isKey(KEY_WIFI_PASS);
}

String Settings::getWiFiSSID() {
    return preferences.getString(KEY_WIFI_SSID, "");
}

String Settings::getWiFiPassword() {
    return preferences.getString(KEY_WIFI_PASS, "");
}

void Settings::setWiFiCredentials(const String& ssid, const String& password) {
    preferences.putString(KEY_WIFI_SSID, ssid);
    preferences.putString(KEY_WIFI_PASS, password);
}

// SiriusXM Settings
bool Settings::hasSXMCredentials() {
    return preferences.isKey(KEY_SXM_EMAIL) && preferences.isKey(KEY_SXM_PASS);
}

String Settings::getSXMEmail() {
    return preferences.getString(KEY_SXM_EMAIL, "");
}

String Settings::getSXMPassword() {
    return preferences.getString(KEY_SXM_PASS, "");
}

void Settings::setSXMCredentials(const String& email, const String& password) {
    preferences.putString(KEY_SXM_EMAIL, email);
    preferences.putString(KEY_SXM_PASS, password);
}

// FM Frequency
float Settings::getFMFrequency() {
    return preferences.getFloat(KEY_FM_FREQ, FM_DEFAULT_FREQ);
}

void Settings::setFMFrequency(float frequency) {
    preferences.putFloat(KEY_FM_FREQ, frequency);
}

// Last Channel
int Settings::getLastChannel() {
    return preferences.getInt(KEY_LAST_CHANNEL, 1);
}

void Settings::setLastChannel(int channel) {
    preferences.putInt(KEY_LAST_CHANNEL, channel);
}

// First run flag
bool Settings::isFirstRun() {
    return !preferences.getBool("setup_done", false);
}

void Settings::setFirstRunComplete() {
    preferences.putBool("setup_done", true);
}
