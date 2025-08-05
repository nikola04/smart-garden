#include "network.h"
#ifndef JSON_H
#define JSON_H

String stringifyDeviceConfig(String deviceName, String powerMode, String wifiSSID, String wifiPassword);
String stringifyWiFiStatus(WiFiStatus);
String stringifyWiFiNetwork(String, int);
String stringifyBLEData(WiFiStatus wifiStatus, SensorsData sensorsData, power_data powerData);
String stringifyAPIData(SensorsData, power_data_t);

#endif // JSON_H
