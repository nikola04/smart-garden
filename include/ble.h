#ifndef BLE_H
#define BLE_H

#include "BLEDevice.h"
#include "network.h"
#include "logger.h"
#include "storage.h"

void setupBLE();
void startBLE();
void stopBLE();
void loopBLE();

class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override;
    void onDisconnect(BLEServer* pServer) override;
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
