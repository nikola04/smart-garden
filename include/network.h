#ifndef NETWORK_H
#define NETWORK_H

#include "HTTPClient.h"
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

// api/client.cpp
enum class APIClientResult {
    WIFI_NOT_CONNECTED,
    HTTP_ERROR,
    EXCEPTION,
    SUCCESS
};

class APIClient {
public:
    APIClient();

    void init(String apiUrl, String apiKey);
    APIClientResult send(String payload);
private:
    String apiUrl;
    String apiKey;
};

// wifi/scan.cpp
void wifiStartScan();
int wifiGetScanStatus();
int wifiGetResultsCount();
String wifiGetResultSSID(int index);
int wifiGetResultRSSI(int index);
void wifiEndScan();


#endif // NETWORK_H
