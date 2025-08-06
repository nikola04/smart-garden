#include "power.h"
#include "logger.h"

battery_t readBatteryData(){
    battery_t battery;

    battery.level = 0.0f;
    battery.voltage = 0;

    return battery;
}

solar_panel_t readSolarPanelData(){
    solar_panel_t solar_panel;

    solar_panel.voltage = 0;
    solar_panel.current = 0;
    solar_panel.charging = false;

    return solar_panel;
}

power_data_t readPowerData(){
    power_data_t power;

    power.battery = readBatteryData();
    power.solar_panel = readSolarPanelData();

    return power;
}
