#include "json.h"

String stringifyDeviceConfig(String deviceName, String powerMode, String wifiSSID, String wifiPassword) {
    String json = "{";
    json += "\"device_name\":\"" + deviceName + "\",";
    json += "\"power_mode\":\"" + powerMode + "\",";
    json += "\"wifi_ssid\":\"" + wifiSSID + "\",";
    json += "\"wifi_password\":\"" + wifiPassword + "\",";
    json += "\"api_key\":\"null\""; // one-time visible
    json += "}";
    
    return json;
}

String wifiStatusToString(WiFiStatus status) {
    if (status == WiFiStatus::CONNECTED)
        return "connected";
    if (status == WiFiStatus::CONNECTING)
        return "connecting";

    return "disconnected";
}

String stringifyWiFiStatus(WiFiStatus wifiStatus){

    String json = "{";
    json += "\"wifi\":\"" + wifiStatusToString(wifiStatus) + "\"";
    json += "}";

    return json.c_str();
}

String stringifyWiFiNetwork(String ssid, int rssi){
    String json = "{";
    json += "\"ssid\":\"" + ssid + "\",";
    json += "\"rssi\":\"" + String(rssi) + "\"";
    json += "}";

    return json.c_str();
}

String stringifyBLEData(WiFiStatus wifiStatus, SensorsData sensorsData, power_data powerData){
    String json = "{";
    json += "\"wifi\":\"" + wifiStatusToString(wifiStatus) + "\",";
    json += "\"battery\":\"" + String(powerData.battery.level) + "\",";
    json += "\"charger\":\"" + String(powerData.solar_panel.current) + "\",";
    json += "\"air_temp\":\"" + String(sensorsData.air.temperature) + "\",";
    json += "\"air_hum\":\"" + String(sensorsData.air.humidity) + "\",";
    json += "\"air_press\":\"" + String(sensorsData.air.pressure) + "\",";
    json += "\"soil\":\"" + String(sensorsData.soil.moisture) + "\",";
    json += "\"light\":\"" + String(sensorsData.light.night) + "\"";
    json += "}";

    return json.c_str();
}

String stringifyAPIData(SensorsData sdata, power_data_t pdata){
    String json = "{";

    json += "\"air\":{";
    json += "\"temperature\":" + String(sdata.air.temperature) + ",";
    json += "\"humidity\":" + String(sdata.air.humidity) + ",";
    json += "\"pressure\":" + String(sdata.air.pressure) + "},";
    
    json += "\"soil\":{";
    json += "\"moisture\":" + String(sdata.soil.moisture) + ",";
    json += "\"sensors_used\":" + String(sdata.soil.sensors_used) + "},";
    
    json += "\"light\":{";
    json += "\"value\":" + String(sdata.light.value) + ",";
    json += "\"night\":" + String(sdata.light.night ? "true" : "false") + "},";
    
    json += "\"battery\":{";
    json += "\"level\":" + String(pdata.battery.level) + ",";
    json += "\"voltage\":" + String(pdata.battery.voltage) + "},";
    
    json += "\"charger\":{";
    json += "\"voltage\":" + String(pdata.solar_panel.voltage) + ",";
    json += "\"current\":" + String(pdata.solar_panel.current) + ",";
    json += "\"charging\":" + String(pdata.solar_panel.charging ? "true" : "false") + "}";

    json += "}";
    return json;
}
