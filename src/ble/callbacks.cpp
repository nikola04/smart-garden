#include "ble.h"
#include "json.h"
#include "sensor.h"
#include "power.h"
#include "system.h"
#include "ArduinoJson.h"
#include "esp_timer.h"
#include "esp_system.h"

void ServerCallbacks::onConnect(BLEServer* pServer) {
    BLEManager::getInstance().onConnect();
}

void ServerCallbacks::onDisconnect(BLEServer* pServer) {
    BLEManager::getInstance().onDisconnect();
}

void SystemCallbacks::onWrite(BLECharacteristic *pChar){
    std::string value = pChar->getValue();

    if(value == "sleep"){
        esp_timer_handle_t timer;
        esp_timer_create_args_t args = {
            .callback = &triggerDeepSleep,
            .arg = nullptr,
            .name = "sleep_delay"
        };
        esp_timer_create(&args, &timer);
        esp_timer_start_once(timer, 200 * 1000);
    } else if(value == "restart"){
        esp_timer_handle_t timer;
        esp_timer_create_args_t args = {
            .callback = &triggerRestart,
            .arg = nullptr,
            .name = "restart_delay"
        };
        esp_timer_create(&args, &timer);
        esp_timer_start_once(timer, 200 * 1000);
    }
}

void DeviceCallbacks::onWrite(BLECharacteristic *pChar) {
    std::string value = pChar->getValue();

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, value);
    if (error) {
        pChar->setValue("ERR: Invalid JSON");
        return;
    }

    if (!doc["device_name"].isNull()) setDeviceName(doc["device_name"].as<const char*>());
    if (!doc["power_mode"].isNull()) setPowerMode(doc["power_mode"].as<const char*>());
    if (!doc["wifi_ssid"].isNull()) setWifiSSID(doc["wifi_ssid"].as<const char*>());
    if (!doc["wifi_password"].isNull()) setWifiPassword(doc["wifi_password"].as<const char*>());
    if (!doc["api_key"].isNull()) setAPIKey(doc["api_key"].as<const char*>());

    pChar->setValue("OK");

    if (!doc["wifi_ssid"].isNull() || !doc["wifi_password"].isNull()){
        WiFiConnectManager::getInstance().init();
        WiFiConnectManager::getInstance().begin();
    }

    log("Configuration updated");
}

void DeviceCallbacks::onRead(BLECharacteristic *pChar) {
    String json = stringifyDeviceConfig(getDeviceName(), getPowerMode(), getWifiSSID(), getWifiPassword());
    
    pChar->setValue(json.c_str());
}

void SensorCallbacks::onRead(BLECharacteristic *pChar) {
    SensorsData sensors = SensorsManager::readSensors();
    power_data_t power = readPowerData();
    WiFiStatus wifi = WiFiConnectManager::getInstance().getStatus();
    String wifiStatus;

    if (wifi == WiFiStatus::CONNECTED)
        wifiStatus = "connected";
    else if (wifi == WiFiStatus::CONNECTING)
        wifiStatus = "connecting";
    else
        wifiStatus = "disconnected";

    String json = "{";
    json += "\"wifi\":\"" + wifiStatus + "\",";
    json += "\"battery\":\"" + String(power.battery.level) + "\",";
    json += "\"charger\":\"" + String(power.solar_panel.current) + "\",";
    json += "\"air_temp\":\"" + String(sensors.air.temperature) + "\",";
    json += "\"air_hum\":\"" + String(sensors.air.humidity) + "\",";
    json += "\"soil\":\"" + String(sensors.soil.moisture) + "\",";
    json += "\"light\":\"" + String(sensors.light.night) + "\"";
    json += "}";

    pChar->setValue(json.c_str());
}

void WifiCallbacks::onWrite(BLECharacteristic *pChar){
    std::string value = pChar->getValue();

    if(value == "scan"){
        wifiStartScan();
        return;
    }
}
