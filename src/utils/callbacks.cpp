#include "callbacks.h"
#include "sensors.h"
#include "power.h"
#include "network.h"

bool deviceConnected = false;

void ServerCallbacks::onConnect(BLEServer* pServer) {
    deviceConnected = true;
}

void ServerCallbacks::onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->start();
}

void DeviceCallbacks::onWrite(BLECharacteristic *pChar) {
    std::string value = pChar->getValue();

    JsonDocument doc; // You might want to specify StaticJsonDocument<N> or DynamicJsonDocument
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

void DeviceCallbacks::onRead(BLECharacteristic *pChar) {
    String json = "{";
    json += "\"device_name\":\"" + getDeviceName() + "\",";
    json += "\"wifi_ssid\":\"" + getWifiSSID() + "\",";
    json += "\"wifi_password\":\"" + getWifiPassword() + "\",";
    json += "\"api_key\":\"\""; // one-time visible
    json += "}";

    pChar->setValue(json.c_str());
}

void SensorCallbacks::onRead(BLECharacteristic *pChar) {
    sensors_data_t sensors = read_sensors();
    power_data_t power = read_power_data();
    wifi_status_t wifi = get_wifi_status();
    String wifi_status;

    if (wifi == WIFI_CONNECTED)
        wifi_status = "connected";
    else if (wifi == WIFI_CONNECTING)
        wifi_status = "connecting";
    else
        wifi_status = "disconnected";

    String json = "{";
    json += "\"wifi\":\"" + wifi_status + "\",";
    json += "\"battery\":\"" + String(power.battery.level) + "\",";
    json += "\"charger\":\"" + String(power.solar_panel.current) + "\",";
    json += "\"air_temp\":\"" + String(sensors.air.temperature) + "\",";
    json += "\"air_hum\":\"" + String(sensors.air.humidity) + "\",";
    json += "\"soil\":\"" + String(sensors.soil.moisture) + "\",";
    json += "\"light\":\"" + String(sensors.light.night) + "\"";
    json += "}";

    pChar->setValue(json.c_str());
}
