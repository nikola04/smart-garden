#include "sensors.h"
#include "helpers.h"

air_data_t readAirSensor() {
    air_data_t air_sensor;

    // Read the air sensor data
    int temp = 21;
    int humidity = 50;

    air_sensor.temperature = temp;
    air_sensor.humidity = humidity;

    return air_sensor;
}

soil_data_t readSoilSensors() {
    soil_data_t soil_sensor;

    // Read the calibrated and calculated soil sensor data
    float moisture_sensors[] = { 66.4, 62.5, 55.6, 69.1, 1.0 };

    int n;
    float *filtered_values = filter_outliers(moisture_sensors, 5, &n);
    float moisture = (float) fcalculate_mean(filtered_values, n);

    soil_sensor.moisture = moisture;
    soil_sensor.sensors_used = n;

    return soil_sensor;
}

light_data_t readLightSensor() {
    light_data_t light_sensor;

    // Read the light sensor data
    ushort value = 1000;
    bool night = false;

    light_sensor.value = value;
    light_sensor.night = night;

    return light_sensor;
}

sensors_data_t readSensors()
{
    sensors_data_t sensors;

    sensors.air = readAirSensor();
    sensors.soil = readSoilSensors();
    sensors.light = readLightSensor();

    return sensors;
}
