#include "network.h"
#include "storage.h"
#include "WiFi.h"

static wifi_status_t wifiStatus = WIFI_DISCONNECTED;
static ulong connectionStartTime = 0;

String wifiSSID = "";
String wifiPassword = "";

wifi_status_t wifiGetStatus(){
    return wifiStatus;
}

int wifiConnect(String ssid, String password){
    connectionStartTime = millis();
    wifiSSID = ssid;
    wifiPassword = password;
    WiFi.begin(ssid.c_str(), password.c_str());
    return 0;
}

void wifiReconnect(void){
    wifiConnect(getWifiSSID(), getWifiPassword());
}

void wifiConnectionLoop(){
    static ushort retryCount = 0;
    static ulong lastRetryTime = 0;

    wl_status_t status = WiFi.status();
    if(status == WL_CONNECTED){
        wifiStatus = WIFI_CONNECTED;
        retryCount = 0;
        return;
    }

    if(status == WL_NO_SHIELD){
        wifiStatus = WIFI_DISCONNECTED;
        return;
    }

    if(status == WL_CONNECT_FAILED || (millis() - connectionStartTime > 15000)){
        if(retryCount < 4){
            ulong retry_delay = 1000 * (1 << retryCount);

            if(millis() - lastRetryTime > retry_delay){
                lastRetryTime = millis();
                retryCount++;
                wifiConnect(wifiSSID, wifiPassword);
            }
        }else wifiStatus = WIFI_DISCONNECTED;
    }else wifiStatus = WIFI_CONNECTING;
}

int wifiDisable(){
    WiFi.mode(WIFI_OFF);
    return 0;
}
