#ifndef BLE_H
#define BLE_H

#include "BLEDevice.h"
#include "network.h"
#include "logger.h"
#include "storage.h"

enum class BLEStatus {
    OFF,
    CONNECTED,
    DISCONNECTED
};

class BLEManager {
public:
    static BLEManager& getInstance() {
        static BLEManager instance;
        return instance;
    }

    void init();
    void start();
    void stop();

    BLEStatus getStatus();

    void loop();
    void handleDataUpdate(WiFiStatus, SensorsData, power_data);
    void handleWiFiStatusChange(WiFiStatus status);

    void onConnect();
    void onDisconnect();

    void setConnectHandler(void (*handler)());
    void setDisconnectHandler(void (*handler)());

    void disable();
private:
    BLEManager();

    bool advertisingStarted = false;
    bool serviceStarted = false;
    BLEService *pService;
    BLEServer *pServer;
    BLECharacteristic *sensorCharacteristic;
    BLECharacteristic *wifiCharacteristic;

    void (*connectHandler)();
    void (*disconnectHandler)();

    bool deviceConnected;
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
