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

String stringifyWiFiStatus(WiFiStatus wifi){
    String wifi_status = "disconnected";;
    if (wifi == WiFiStatus::CONNECTED)
        wifi_status = "connected";
    else if (wifi == WiFiStatus::CONNECTING)
        wifi_status = "connecting";
        
    String json = "{";
    json += "\"wifi\":\"" + wifi_status + "\"";
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

String stringifyAPIData(SensorsData sdata, power_data_t pdata){
    String json = "{";

    json += "\"air\":{";
    json += "\"temperature\":" + String(sdata.air.temperature) + ",";
    json += "\"humidity\":" + String(sdata.air.humidity) + "},";
    
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
