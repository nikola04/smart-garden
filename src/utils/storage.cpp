#include "storage.h"
#include "config.h"
#include "logger.h"
#include "Preferences.h"

String getPreference(const String &key, const String &fallback) {
    Preferences prefs;
    prefs.begin("config", false);
    char buffer[128] = {0};
    int len = prefs.getString(key.c_str(), buffer, sizeof(buffer));
    if(len == 0){
        prefs.putString(key.c_str(), fallback.c_str());
        prefs.end();
        return fallback;
    }

    prefs.end();
    return String(buffer);
}

void setPreference(const String &key, const String &value) {
    Preferences prefs;
    prefs.begin("config", false);
    prefs.putString(key.c_str(), value.c_str());
    prefs.end();
}

String getDeviceName() {
    return getPreference("device_name", DEFAULT_DEVICE_NAME);
}

void setDeviceName(const String &name) {
    setPreference("device_name", name);
}

String getPowerMode() {
    return getPreference("power_mode", DEFAULT_POWER_MODE);
}

void setPowerMode(const String &mode) {
    setPreference("power_mode", mode);
}

String getWifiSSID() {
    return getPreference("wifi_ssid", DEFAULT_WIFI_SSID);
}

void setWifiSSID(const String &ssid) {
    setPreference("wifi_ssid", ssid);
}

String getWifiPassword() {
    return getPreference("wifi_password", DEFAULT_WIFI_PASSWORD);
}

void setWifiPassword(const String &password) {
    setPreference("wifi_password", password);
}

String getAPIKey() {
    return getPreference("api_key", DEFAULT_API_KEY);
}

void setAPIKey(const String &apiKey) {
    setPreference("api_key", apiKey);
}

String getLoggerMode(){
    return getPreference("logger_mode", DEFAULT_LOGGER_MODE);
}

void setLoggerMode(const String &loggerMode){
    setPreference("logger_mode", loggerMode);
}
