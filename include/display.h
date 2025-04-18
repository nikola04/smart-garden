#ifndef DISPLAY_H
#define DISPLAY_H

#include "sensors.h"
#include "power.h"

void set_display_data(power_data_t power_data, sensors_data_t sensors_data);
void check_wifi_status_loop();
void display_power_on();
void display_power_off();
void cycle_display();

#endif // DISPLAY_H
