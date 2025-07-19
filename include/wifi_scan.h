#ifndef WIFI_SCAN_H
#define WIFI_SCAN_H

#include "WiFi.h"

void startWiFiScanAsync();
int getWiFiScanStatus();
int getWiFiResultsCount();
int getWiFiNextResultIndex();

String getWiFiResultSSID(int index);
int getWiFiResultRSSI(int index);

void endWiFiScan();

#endif // WIFI_SCAN_H
