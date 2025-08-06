#include "network.h"
#include "config.h"
#include "storage.h"
#include "json.h"
#include "logger.h"

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

    Logger::getInstance().log("API", "sending data..");
    try{
        http.begin(apiUrl);
        http.addHeader("Content-Type", "application/json");
        http.addHeader("X-api-key", apiKey);

        int responseCode = http.POST(payload);
        if (responseCode < 200 || responseCode >= 300){
            http.end();
            Logger::getInstance().error("API", ("data not sent: " + String(responseCode)).c_str());
            return APIClientResult::HTTP_ERROR;
        }
        http.end();
        Logger::getInstance().log("API", "data sent.");
        return APIClientResult::SUCCESS;
    }catch(...){
        http.end();
        Logger::getInstance().error("API", "data not sent: Exception catched");
        return APIClientResult::EXCEPTION;
    }
}
