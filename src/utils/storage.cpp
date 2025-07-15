#include "storage.h"
#include "config.h"
#include "Preferences.h"

String getPreference(const String &key) {
    Preferences prefs;
    prefs.begin("config", false);
    char buffer[128] = {0};
    prefs.getString(key.c_str(), buffer, sizeof(buffer));
    prefs.end();
    return String(buffer);
}

void setPreference(const String &key, const String &value) {
    Preferences prefs;
    prefs.begin("config", false);
    prefs.putString(key.c_str(), value.c_str());
    prefs.end();
}

String getWifiSSID() {
    return getPreference("wifi_ssid");
}

void setWifiSSID(const String &ssid) {
    setPreference("wifi_ssid", ssid);
}

String getDeviceName() {
    String name = getPreference("device_name");
    if(name && name.length() > 2) 
        return name;
        
    setDeviceName(String(DEVICE_NAME));
    return String(DEVICE_NAME);
}

void setDeviceName(const String &name) {
    setPreference("device_name", name);
}

String getWifiPassword() {
    return getPreference("wifi_password");
}

void setWifiPassword(const String &password) {
    setPreference("wifi_password", password);
}

String getAPIKey() {
    return getPreference("api_key");
}

void setAPIKey(const String &apiKey) {
    setPreference("api_key", apiKey);
}
