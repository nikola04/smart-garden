#ifndef NETWORK_H
#define NETWORK_H

#include "WiFi.h"
#include "sensors.h"
#include "power.h"

enum class WiFiStatus {
    DISCONNECTED = 0,
    CONNECTING = 1,
    CONNECTED = 2,
};

// wifi/connect.cpp
class WiFiConnectManager {
public:
    static WiFiConnectManager& getInstance() {
        static WiFiConnectManager instance;
        return instance;
    }

    void init();
    void begin();

    void setStatusHandler(void (*handler)(WiFiStatus));

    void loop();
    void disable();

    WiFiStatus getStatus();
private:
    WiFiConnectManager();

    WiFiStatus wifiStatus;
    String ssid;
    String password;

    ulong connectionStart;

    void (*statusHandler)(WiFiStatus);
};

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
