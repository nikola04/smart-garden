#ifndef SENSORS_H
#define SENSORS_H

#include "types.h"

typedef struct air_data {
    float temperature; // Celsius
    float humidity;    // %
} air_data_t;

typedef struct soil_data {
    float moisture; // %
    ushort sensors_used;
} soil_data_t;

typedef struct light_data {
    ushort value;
    bool night;
} light_data_t;

typedef struct sensors {
    air_data_t air;
    soil_data_t soil;
    light_data_t light;
} sensors_data_t;

air_data_t read_air_sensor();
soil_data_t read_soil_sensors();
light_data_t read_light_sensor();
sensors_data_t read_sensors();

#endif // SENSORS_H
