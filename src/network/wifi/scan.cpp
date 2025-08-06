#include "network.h"
#include "logger.h"

bool scanning = false;
int lastIndex = 0;

void wifiStartScan(){
    if(scanning) 
        return;

    Logger::getInstance().debug("WiFiScan", "starting..");
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.scanNetworks(true, true);
    lastIndex = 0;
    scanning = true;
}

// -3 not scanning, -2 fail, -1 scanning, 0 scanned
int wifiGetScanStatus(){
    if(!scanning)
        return -3;

    int n = WiFi.scanComplete();
    if(n >= 0) 
        return 0;
    
    return n;
}

int wifiGetResultsCount(){
    int n = WiFi.scanComplete();
    if(n >= 0){
        Logger::getInstance().debug("WiFiScan", "scanned.");
        return n;
    }

    return -1;
}

String wifiGetResultSSID(int index){
    return WiFi.SSID(index);
}

int wifiGetResultRSSI(int index){
    return WiFi.RSSI(index);
}

void wifiEndScan(){
    if(!scanning)
        return;
    
    WiFi.scanDelete();
    scanning = false;
}
