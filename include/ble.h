#ifndef BLE_H
#define BLE_H

#include "BLEDevice.h"
#include "network.h"
#include "logger.h"
#include "storage.h"

class BLEManager {
public:
    BLEManager();

    void init();
    void start();

    void loop();
    void handleWiFiStatusChange(WiFiStatus status);

    void stop();
private:
    BLEService *pService;
    BLECharacteristic *sensorCharacteristic;
    BLECharacteristic *wifiCharacteristic;
};

class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override;
    void onDisconnect(BLEServer* pServer) override;
};

class SystemCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pChar) override;
};

class DeviceCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pChar) override;
    void onRead(BLECharacteristic* pChar) override;
};

class SensorCallbacks : public BLECharacteristicCallbacks {
    void onRead(BLECharacteristic* pChar) override;
};

class WifiCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pChar) override;
};

#endif // BLE_H
