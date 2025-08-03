#include "bmp180.h"
#include "logger.h"
#include "config.h"

Adafruit_BMP085 BMP180::bmp;

bool BMP180::begin(){
    if (!bmp.begin()) {
        log("BMP180 init failed!");
        return false;
    }
    log("BMP180 initialized.");
    return true;
}

float BMP180::getTemperature(){
    return bmp.readTemperature();
}

float BMP180::getPressure(){
    return bmp.readSealevelPressure(ALTITUDE_M) / 100.0;
}
