#ifndef NETWORK_H
#define NETWORK_H

#include "WiFi.h"
#include "sensors.h"
#include "power.h"

enum wifi_status_t {
    WIFI_CONNECTING = 1,
    WIFI_CONNECTED = 2,
    WIFI_DISCONNECTED = 0,
};

// wifi/connect.cpp
wifi_status_t wifiGetStatus();
void initWifi(String ssid, String password);
int wifiConnect();
int wifiDisable();
void wifiConnectionLoop();

// wifi/scan.cpp
void wifiStartScan();
int wifiGetScanStatus();
int wifiGetResultsCount();
String wifiGetResultSSID(int index);
int wifiGetResultRSSI(int index);
void wifiEndScan();

// api/uploader.cpp
int sendData(sensors_data_t sdata, power_data_t pdata);

#endif // NETWORK_H
