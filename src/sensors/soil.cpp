#include "soil_sensor.h"
#include "logger.h"

SoilSensor::SoilSensor(int analogPin, int dryValue, int wetValue)
    : pin(analogPin), dry(dryValue), wet(wetValue) {}

void SoilSensor::begin(){
    pinMode(pin, INPUT);
}

int SoilSensor::readRaw() {
    long sum = 0;
    int totalReads = 10;
    for (int i = 0; i < totalReads; i++) {
        sum += analogRead(pin);
        delay(5);
    }
    return sum / totalReads;
}

int SoilSensor::readMoisture() {
    int raw = readRaw();
    int percentage = map(raw, dry, wet, 0, 100);
    return constrain(percentage, 0, 100);
}
