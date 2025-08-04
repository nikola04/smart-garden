#ifndef MAIN_H
#define MAIN_H

#include "Arduino.h"
#include "sleep.h"
#include "network.h"

void handleWakeup(WakeupReason);
void handleSleep();
void handleButtonPress();
void handleButtonLongPress();
void handleButtonHold();
void handleButtonRelease();
void handleBLEDisconnect();
void handleBLEConnect();
void handleWiFiStatusChange(WiFiStatus status);

#endif // MAIN_H
