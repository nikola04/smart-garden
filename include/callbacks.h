#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "main.h"
#include "storage.h"
#include "BLEDevice.h"
#include "ArduinoJson.h"

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


#endif // CALLBACKS_H
