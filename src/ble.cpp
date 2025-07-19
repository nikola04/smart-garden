#include "main.h"
#include "ble.h"
#include "config.h"
#include "storage.h"
#include "BLEDevice.h"
#include "BLE2902.h"
#include "callbacks.h"
#include "network.h"
#include "json.h"
#include "wifi_scan.h"

BLECharacteristic *sensorCharacteristic;
BLECharacteristic *wifiCharacteristic;

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

    pService->start();

    // Advertising setup
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);

    pAdvertising->start();

    Serial.println("BLE advertising started");
}

wifi_status_t last_wifi_status;
void ble_loop() {
    // wifi scan
    int wifiStatus;
    if((wifiStatus = getWiFiScanStatus()) != -3){
        if(wifiStatus == -2){
            wifiCharacteristic->setValue("fail");
            wifiCharacteristic->notify();
            endWiFiScan();
        }else if(wifiStatus == 0){
            int n = getWiFiResultsCount();
            for(int i = 0; i < n; i++){
                String ssid = getWiFiResultSSID(i);
                int rssi = getWiFiResultRSSI(i);
                String json = stringifyWiFiNetwork(ssid, rssi);

                wifiCharacteristic->setValue(json.c_str());
                wifiCharacteristic->notify();
            }
            endWiFiScan();
            wifiCharacteristic->setValue("done");
            wifiCharacteristic->notify();
        }
    }

    // wifi status
    wifi_status_t status = get_wifi_status();
    if(status != last_wifi_status){
        String json = stringifyWiFiStatus(status);
        sensorCharacteristic->setValue(json.c_str());
        sensorCharacteristic->notify();
    }

    last_wifi_status = status;
}

void start_ble_server() {

}

void stop_ble_server() {
    BLEDevice::deinit(true);
    log("BLE server stopped");
}
