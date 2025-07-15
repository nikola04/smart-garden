#include "main.h"
#include "ble.h"
#include "config.h"
#include "storage.h"
#include "BLEDevice.h"
#include "ArduinoJson.h"

BLEServer *pServer;

bool deviceConnected = false;

class ServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override {
        deviceConnected = true;
    }

    void onDisconnect(BLEServer* pServer) override {
        deviceConnected = false;
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->start();
    }
};

class ConfigCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pChar) override {
        std::string value = pChar->getValue();

        JsonDocument doc; // Use appropriate size for your JSON
        DeserializationError error = deserializeJson(doc, value);
        if (error) {
            pChar->setValue("ERR: Invalid JSON");
            return;
        }

        if (!doc["device_name"].isNull()) setDeviceName(doc["device_name"].as<const char*>());
        if (!doc["wifi_ssid"].isNull()) setWifiSSID(doc["wifi_ssid"].as<const char*>());
        if (!doc["wifi_pswd"].isNull()) setWifiPassword(doc["wifi_pswd"].as<const char*>());
        if (!doc["api_key"].isNull()) setAPIKey(doc["api_key"].as<const char*>());

        pChar->setValue("OK");
        log("Configuration updated");
    }

    void onRead(BLECharacteristic *pChar) override {
        String json = "{";
        json += "\"device_name\":\"" + getDeviceName() + "\",";
        json += "\"wifi_ssid\":\"" + getWifiSSID() + "\",";
        json += "\"wifi_password\":\"" + getWifiPassword() + "\",";
        json += "\"api_key\":\"" + getAPIKey() + "\"";
        json += "}";

        pChar->setValue(json.c_str());
    }
};

void setup_ble() {
    BLEDevice::init(("SmartGarden - " + String(getDeviceName())).c_str());
    pServer = BLEDevice::createServer();

    pServer->setCallbacks(new ServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic *configCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
    );

    configCharacteristic->setCallbacks(new ConfigCallbacks());

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
