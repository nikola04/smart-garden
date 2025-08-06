#include "logger.h"
#include "sensors.h"
#include "helpers.h"

BMP180 SensorsManager::bmp180;

void SensorsManager::init() {
    bmp180.begin();
    Logger::getInstance().debug("Sensors", "initialized.");
}

AirData SensorsManager::readAirSensor(){
    float temp = bmp180.getTemperature();
    float pressure = bmp180.getPressure();
    int humidity = 50;

    AirData airData;
    airData.temperature = temp;
    airData.humidity = humidity;
    airData.pressure = pressure;

    return airData;
}

SoilData SensorsManager::readSoilSensor(){
    SoilData soilData;

    // Read the calibrated and calculated soil sensor data
    float moisture_sensors[] = { 66.4, 62.5, 55.6, 69.1, 1.0 };

    int n;
    float *filtered_values = filter_outliers(moisture_sensors, 5, &n);
    float moisture = (float) fcalculate_mean(filtered_values, n);

    soilData.moisture = moisture;
    soilData.sensors_used = n;

    return soilData;
}

LightData SensorsManager::readLightSensor(){
    LightData lightData;

    ushort value = 1000;
    bool night = false;

    lightData.value = value;
    lightData.night = night;

    return lightData;
}

SensorsData SensorsManager::readSensors(){
    SensorsData sensorsData;
    
    sensorsData.air = readAirSensor();
    sensorsData.soil = readSoilSensor();
    sensorsData.light = readLightSensor();

    return sensorsData;
}
