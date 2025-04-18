#ifndef MAIN_H
#define MAIN_H

#include "Arduino.h"
#include "esp_sleep.h"

void handle_wakeup(esp_sleep_wakeup_cause_t wakeup_reason);
void handle_sleep();
void handle_button_press();
void handle_wifi_connected();
void sleep_timeout_loop();
void button_press_loop();

#endif // MAIN_H
