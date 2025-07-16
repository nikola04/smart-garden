#include "main.h"
#include "ble.h"
#include "config.h"
#include "storage.h"
#include "BLEDevice.h"
#include "callbacks.h"

void setup_ble() {
    BLEDevice::init(("SmartGarden - " + String(getDeviceName())).c_str());
    BLEServer* pServer = BLEDevice::createServer();

    pServer->setCallbacks(new ServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic *deviceCharacteristic = pService->createCharacteristic(
        DEVICE_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
    );
    deviceCharacteristic->setCallbacks(new DeviceCallbacks());

    BLECharacteristic *sensorCharacteristic = pService->createCharacteristic(
        SENSORS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ
    );
    sensorCharacteristic->setCallbacks(new SensorCallbacks());

    pService->start();

    // Advertising setup
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);

    pAdvertising->start();

    Serial.println("BLE advertising started");
}


void start_ble_server() {

}

void stop_ble_server() {
    BLEDevice::deinit(true);
    log("BLE server stopped");
}

void ble_loop() {
    // Handle BLE events
}
