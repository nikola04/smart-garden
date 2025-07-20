#ifndef POWER_H
#define POWER_H

#include "types.h"

typedef struct battery {
    float level; // Battery level in percentage
    ushort voltage;  // milliVolts
} battery_t;

typedef struct solar_panel {
    int voltage; // milliVolts
    int current; // milliAmps
    bool charging;
} solar_panel_t;

typedef struct power_data {
    battery_t battery;
    solar_panel_t solar_panel;
} power_data_t;

battery_t readBatteryData();
solar_panel_t readSolarPanelData();
power_data_t readPowerData();

#endif // POWER_H
