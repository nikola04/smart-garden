#include "config.h"
#include "network.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include "storage.h"
#include "main.h"
#include "Arduino.h"

static wifi_status_t wifi_status = WIFI_DISCONNECTED;

wifi_status_t get_wifi_status(){
    return wifi_status;
}

String wifiSSID = "";
String wifiPassword = "";

static ulong connection_start_time = 0;
void wifi_connect_loop(){
    static ushort wifi_retry_count = 0;
    static ulong last_retry_time = 0;

    wl_status_t status = WiFi.status();
    if(status == WL_CONNECTED){
        wifi_status = WIFI_CONNECTED;
        wifi_retry_count = 0;
        return;
    }

    if(status == WL_NO_SHIELD){
        wifi_status = WIFI_DISCONNECTED;
        return;
    }

    if(status == WL_CONNECT_FAILED || (millis() - connection_start_time > 15000)){
        if(wifi_retry_count < 5){
            ulong retry_delay = 1000 * (1 << wifi_retry_count);
            
            if(millis() - last_retry_time > retry_delay){
                last_retry_time = millis();
                wifi_retry_count++;
                connect_to_wifi(wifiSSID, wifiPassword);
            }
        }else wifi_status = WIFI_DISCONNECTED;
    }else wifi_status = WIFI_CONNECTING;
}

int connect_to_wifi(String ssid, String password){
    connection_start_time = millis();
    wifiSSID = ssid;
    wifiPassword = password;
    WiFi.begin(ssid.c_str(), password.c_str());
    return 0;
}

int wifi_off(){
    WiFi.mode(WIFI_OFF);
    return 0;
}

int send_data(sensors_data_t sdata, power_data_t pdata){
    if (WiFi.status() != WL_CONNECTED) return -2;

    String api_key = getAPIKey();

    HTTPClient http;
    http.setTimeout(5000);
    String payload = json_stringify(sdata, pdata);
    
    try{
        http.begin(API_URL);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("X-api-key", api_key);
        int responseCode = http.POST(payload);
        if (responseCode < 200 || responseCode >= 300)
            return -1;
    }catch(...){
        return -2;
    }

    http.end();
    return 0;
}

String json_stringify(sensors_data_t sdata, power_data_t pdata){
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
