#ifndef NETWORK_H
#define NETWORK_H

#include "sensors.h"
#include "power.h"

int connect_to_wifi(const char* ssid, const char* password);
int disconnect_from_wifi();
int send_data(sensors_data_t sdata, power_data_t pdata);

#endif // NETWORK_H
