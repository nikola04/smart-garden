#ifndef DRAW_H
#define DRAW_H

#include "Adafruit_SSD1306.h"
#include "network.h"

void init_draw_utils(Adafruit_SSD1306 *display);
void draw_template();
void draw_wifi_status(wifi_status_t status);
void draw_battery_status(int level, bool charging);
void draw_sun_icon(bool sunny);
void draw_temperature(float temp);

#endif // DRAW_H
