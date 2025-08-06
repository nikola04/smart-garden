#include "network.h"
#include "storage.h"
#include "WiFi.h"
#include "logger.h"

WiFiConnectManager::WiFiConnectManager(){
    wifiStatus = WiFiStatus::DISCONNECTED;
    ssid = "";
    password = "";
}

void WiFiConnectManager::init(){
    this->ssid = getWifiSSID();
    this->password = getWifiPassword();
}

void WiFiConnectManager::begin(){
    Logger::getInstance().debug("WiFi", "connecting..");
    WiFi.disconnect();
    connectionStart = millis();
    if(ssid.length() == 0) return;
    WiFi.begin(ssid.c_str(), password.c_str());
}

void WiFiConnectManager::loop(){
    static ushort retryCount = 0;
    static ulong lastRetryTime = 0;

    WiFiStatus oldStatus = wifiStatus;
    wl_status_t status = WiFi.status();
    if(status == WL_CONNECTED){ // if connected
        wifiStatus = WiFiStatus::CONNECTED;
        retryCount = 0;
    }else if(status == WL_DISCONNECTED){ // disconnect
        wifiStatus = WiFiStatus::DISCONNECTED;
    }else if(status == WL_CONNECT_FAILED || status == WL_CONNECTION_LOST || (millis() - connectionStart > 10000)){ // retry on fail or timeout
        if(retryCount < 3){
            ulong retry_delay = 1000 * (1 << retryCount);

            if(millis() - lastRetryTime > retry_delay){
                lastRetryTime = millis();
                retryCount++;
                Logger::getInstance().debug("WiFi", "retrying to connect..");
                this->begin(); // retry
                return;
            }
        }else wifiStatus = WiFiStatus::DISCONNECTED;
    }else wifiStatus = WiFiStatus::CONNECTING;

    if(oldStatus != wifiStatus)
        statusHandler(wifiStatus);
}

WiFiStatus WiFiConnectManager::getStatus(){
    return wifiStatus;
}

void WiFiConnectManager::disable(){
    WiFi.mode(WIFI_OFF);
}

void WiFiConnectManager::setStatusHandler(void (*handler)(WiFiStatus)){
    statusHandler = handler;
}
