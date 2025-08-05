#include "ble.h"
#include "BLE2902.h"
#include "config.h"
#include "json.h"

BLEManager::BLEManager(){
    advertisingStarted = false;
    serviceStarted = false;
    deviceConnected = false;
}

void BLEManager::init() {
    advertisingStarted = false;
    serviceStarted = false;
    deviceConnected = false;

    BLEDevice::init(("SmartGarden - " + String(getDeviceName())).c_str());

    pServer = BLEDevice::createServer();

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

    // Advertising setup
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);

    pService->start();
    serviceStarted = true;
    log("services inited and started");
}

void BLEManager::start() {
    if(!serviceStarted && pService != nullptr) {
        log("service not started. starting...");
        pService->start();
        serviceStarted = true;
    }

    if(advertisingStarted) return;
    advertisingStarted = true;

    pServer->startAdvertising();
    log("BLE advertising started");
}

void BLEManager::stop() {
    if(!advertisingStarted) return;
    advertisingStarted = false;

    pServer->getAdvertising()->stop();
    log("BLE advertising stopped");
}

void BLEManager::loop() {
    if(!serviceStarted) return;
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

void BLEManager::handleDataUpdate(WiFiStatus wifiStatus, SensorsData sensorsData, power_data powerData){
    String json = stringifyBLEData(wifiStatus, sensorsData, powerData);

    sensorCharacteristic->setValue(json.c_str());
    sensorCharacteristic->notify();
}

void BLEManager::handleWiFiStatusChange(WiFiStatus status){
    if(!serviceStarted) return;
    
    String json = stringifyWiFiStatus(status);
    sensorCharacteristic->setValue(json.c_str());
    sensorCharacteristic->notify();
}

void BLEManager::onConnect(){
    advertisingStarted = false;
    deviceConnected = true;
    connectHandler();
}

void BLEManager::onDisconnect(){
    deviceConnected = false;
    disconnectHandler();
}

void BLEManager::setConnectHandler(void (*handler)()){
    this->connectHandler = handler;
}

void BLEManager::setDisconnectHandler(void (*handler)()){
    this->disconnectHandler = handler;
}

void BLEManager::disable()
{
    advertisingStarted = false;

    if (serviceStarted && pService != nullptr) {
        pService->stop();
        pService = nullptr;
    }

    // BLEDevice::deinit(true);

    sensorCharacteristic = nullptr;
    wifiCharacteristic = nullptr;
    serviceStarted = false;
}
