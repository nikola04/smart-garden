#include "network.h"
#include "config.h"
#include "storage.h"
#include "json.h"

APIClient::APIClient(){
    String apiUrl = "";
    String apiKey = "";
}

void APIClient::init(String apiUrl, String apiKey){
    this->apiUrl = apiUrl;
    this->apiKey = apiKey;
}

APIClientResult APIClient::send(String payload){
    WiFiStatus wifiStatus = WiFiConnectManager::getInstance().getStatus();
    if(wifiStatus != WiFiStatus::CONNECTED) return APIClientResult::WIFI_NOT_CONNECTED;

    HTTPClient http;
    http.setTimeout(5000);

    try{
        http.begin(apiUrl);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("X-api-key", apiKey);

        int responseCode = http.POST(payload);
        if (responseCode < 200 || responseCode >= 300){
            http.end();
            return APIClientResult::HTTP_ERROR;
        }
        http.end();
        return APIClientResult::SUCCESS;
    }catch(...){
        http.end();
        return APIClientResult::EXCEPTION;
    }
}
