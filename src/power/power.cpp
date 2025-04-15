#include "power.h"

battery_t read_battery_data(){
    battery_t battery;

    battery.level = 0.0f;
    battery.voltage = 0;

    return battery;
}

solar_panel_t read_solar_panel_data(){
    solar_panel_t solar_panel;

    solar_panel.voltage = 0;
    solar_panel.current = 0;
    solar_panel.charging = false;

    return solar_panel;
}

power_data_t read_power_data(){
    power_data_t power;

    power.battery = read_battery_data();
    power.solar_panel = read_solar_panel_data();

    return power;
}
