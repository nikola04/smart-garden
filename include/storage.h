#ifndef SSTORAGE_H
#define STORAGE_H

#include "Arduino.h"

String getDeviceName();
void setDeviceName(const String& ssid);

String getPowerMode();
void setPowerMode(const String& mode);

String getWifiSSID();
void setWifiSSID(const String& ssid);

String getWifiPassword();
void setWifiPassword(const String& password);

String getAPIKey();
void setAPIKey(const String& apiKey);

#endif // SSTORAGE_H
