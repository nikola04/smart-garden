#ifndef SENSORS_H
#define SENSORS_H

#include "types.h"
#include "bmp180.h"
#include "soil_sensor.h"

struct AirData {
    float temperature;      // Celsius
    float humidity;         // %
    float pressure;         // hPa
};

struct SoilData {
    float moisture;         // %
    ushort sensors_used;
};

struct LightData {
    ushort value;
    bool night;
};

struct SensorsData {
    AirData air;
    SoilData soil;
    LightData light;
};

class SensorsManager {
public:
    static void init();

    static AirData readAirSensor();
    static SoilData readSoilSensor();
    static LightData readLightSensor();
    static SensorsData readSensors();
private:
    static BMP180 bmp180;
    static SoilSensor soilSensor;
};

#endif // SENSORS_H
