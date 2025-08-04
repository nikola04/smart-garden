#ifndef DISPLAY_H
#define DISPLAY_H

#include "Adafruit_SSD1306.h"
#include "types.h"
#include "sensors.h"
#include "power.h"
#include "network.h"

class DisplayManager {
public:
    DisplayManager();
    void loop();
    void powerOn();
    void powerOff();
    bool isOn() const;

    void refresh();
    void cycle();
    void showNotification(const char* message);
    void setData(const power_data_t& powerData, const SensorsData& sensorData);

private:
    void drawPower();
    void drawSoil();
    void drawAir();
    void drawBattery();

    Adafruit_SSD1306 display;
    bool displayOn = false;
    uint16_t currentMode = 0;
    uint16_t displayCount = 4;

    uint32_t lastRefresh = 0;

    power_data_t powerData;
    SensorsData sensorData;

    using DrawFunction = void (DisplayManager::*)();
    DrawFunction displayFunctions[4];
};

#endif // DISPLAY_H
