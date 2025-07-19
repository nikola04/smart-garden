#include "wifi_scan.h"

bool scanning = false;
int lastIndex = 0;

void startWiFiScanAsync(){
    if(scanning) 
        return;

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.scanNetworks(true);
    lastIndex = 0;
    scanning = true;
}

// -3 not scanning, -2 fail, -1 scanning, 0 scanned
int getWiFiScanStatus(){
    if(!scanning)
        return -3;

    int n = WiFi.scanComplete();
    if(n >= 0) 
        return 0;
    
    return n;
}

int getWiFiResultsCount(){
    int n = WiFi.scanComplete();
    if(n >= 0)
        return n;

    return -1;
}

int getWiFiNextResultIndex(){
    if(!scanning || lastIndex >= getWiFiResultsCount())
        return -1;
    return lastIndex++;
}

String getWiFiResultSSID(int index){
    return WiFi.SSID(index);
}

int getWiFiResultRSSI(int index){
    return WiFi.RSSI(index);
}

void endWiFiScan(){
    if(!scanning)
        return;
    
    WiFi.scanDelete();
    scanning = false;
}
