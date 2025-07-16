#ifndef NETWORK_H
#define NETWORK_H

#include "sensors.h"
#include "power.h"
#include "Arduino.h"

enum wifi_status_t {
    WIFI_CONNECTING = 1,
    WIFI_CONNECTED = 2,
    WIFI_DISCONNECTED = 0,
};

wifi_status_t get_wifi_status();
int connect_to_wifi(String ssid, String password);
int wifi_off();
void wifi_connect_loop();
int send_data(sensors_data_t sdata, power_data_t pdata);
String json_stringify(sensors_data_t sdata, power_data_t pdata);

#endif // NETWORK_H
