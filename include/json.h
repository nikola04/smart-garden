#include "network.h"
#ifndef JSON_H
#define JSON_H

String stringifyDeviceConfig(String deviceName, String powerMode, String wifiSSID, String wifiPassword);
String stringifyWiFiStatus(wifi_status_t);
String stringifyWiFiNetwork(String, int);
String stringifyAPIData(sensors_data_t, power_data_t);

#endif // JSON_H
