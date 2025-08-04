#include "ble.h"
#include "BLE2902.h"
#include "config.h"
#include "json.h"

BLEManager::BLEManager(){
    started = false;
}

void BLEManager::init() {
    BLEDevice::init(("SmartGarden - " + String(getDeviceName())).c_str());

    BLEServer* pServer = BLEDevice::createServer();

    pServer->setCallbacks(new ServerCallbacks());

    pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic *systemCharacteristic = pService->createCharacteristic(
        SYSTEM_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    systemCharacteristic->setCallbacks(new SystemCallbacks());

    BLECharacteristic *deviceCharacteristic = pService->createCharacteristic(
        DEVICE_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
    );
    deviceCharacteristic->setCallbacks(new DeviceCallbacks());

    sensorCharacteristic = pService->createCharacteristic(
        SENSORS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY
    );
    sensorCharacteristic->setCallbacks(new SensorCallbacks());
    sensorCharacteristic->addDescriptor(new BLE2902());

    wifiCharacteristic = pService->createCharacteristic(
        WIFI_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY
    );
    wifiCharacteristic->setCallbacks(new WifiCallbacks());
    wifiCharacteristic->addDescriptor(new BLE2902());
}

void BLEManager::start() {
    if(started) return;
    started = true;

    pService->start();

    // Advertising setup
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);

    pAdvertising->start();

    Serial.println("BLE advertising started");
}

void BLEManager::loop() {
    if(!started) return;
    // wifi scan
    int wifiStatus;
    if((wifiStatus = wifiGetScanStatus()) != -3){
        if(wifiStatus == -2){
            wifiCharacteristic->setValue("fail");
            wifiCharacteristic->notify();
            wifiEndScan();
        }else if(wifiStatus == 0){
            int n = wifiGetResultsCount();
            for(int i = 0; i < n; i++){
                String ssid = wifiGetResultSSID(i);
                int rssi = wifiGetResultRSSI(i);
                String json = stringifyWiFiNetwork(ssid, rssi);

                wifiCharacteristic->setValue(json.c_str());
                wifiCharacteristic->notify();
            }
            wifiEndScan();
            wifiCharacteristic->setValue("done");
            wifiCharacteristic->notify();
        }
    }
}

void BLEManager::handleWiFiStatusChange(WiFiStatus status){
    if(!started) return;
    
    String json = stringifyWiFiStatus(status);
    sensorCharacteristic->setValue(json.c_str());
    sensorCharacteristic->notify();
}

void BLEManager::stop(){
    started = false;
    BLEDevice::deinit(true);
    log("BLE server stopped");
}
