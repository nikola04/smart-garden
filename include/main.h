#ifndef MAIN_H
#define MAIN_H

#include "Arduino.h"
#include "esp_sleep.h"

void handleWakeup(esp_sleep_wakeup_cause_t wakeup_reason);
void handleSleep();
void handleButtonPress();
void handleButtonLongPress();
void handleWifiConnected();
void sleepTimeoutLoop();
void buttonPressLoop();

#endif // MAIN_H
