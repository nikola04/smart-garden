#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H

#include "Arduino.h"

class SoilSensor {
public:
    SoilSensor(int analogPin, int dryValue = 3700, int wetValue = 1500);

    void begin();

    int readMoisture();

private:
    int pin;
    int dry;
    int wet;

    int readRaw();
};

#endif
