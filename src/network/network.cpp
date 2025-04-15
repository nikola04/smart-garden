#include "network.h"
#include "WiFi.h"
#include "HTTPClient.h"

#define API_URL "http://example.com/api/data"

int connect_to_wifi(char *ssid, char *password)
{
    unsigned long start_time = millis();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - start_time > 15000)
            return -1;

        delay(500);
    }
    return 0;
}

int disconnect_from_wifi()
{
    WiFi.disconnect();
    return 0;
}

String json_stringify(sensors_data_t sdata, power_data_t pdata);

int send_data(sensors_data_t sdata, power_data_t pdata)
{
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String payload = json_stringify(sdata, pdata);
        
        http.begin(API_URL);
        http.addHeader("Content-Type", "application/json");
        int responseCode = http.POST(payload);
        http.end();

        if (responseCode < 200 || responseCode >= 300)
            return -1;
        return 0;
    }
    else {
        Serial.println("WiFi not connected");
    }
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
    
    json += "\"solar_panel\":{";
    json += "\"voltage\":" + String(pdata.solar_panel.voltage) + ",";
    json += "\"current\":" + String(pdata.solar_panel.current) + ",";
    json += "\"charging\":" + String(pdata.solar_panel.charging ? "true" : "false") + "}";

    json += "}";
    return json;
}
