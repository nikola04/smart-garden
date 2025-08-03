#ifndef BMP180_H
#define BMP180_H

#include "Adafruit_BMP085.h"

class BMP180 {
private:
    static Adafruit_BMP085 bmp;
public:
    bool begin();
    float getTemperature();
    float getPressure();
};

#endif // BMP180_H
