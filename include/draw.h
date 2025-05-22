#ifndef DRAW_H
#define DRAW_H

#include "Adafruit_SSD1306.h"
#include "network.h"

void init_draw_utils(Adafruit_SSD1306 *display);
void draw_text(Adafruit_SSD1306 *display, const char *string);
void draw_template(Adafruit_SSD1306 *display);
void draw_wifi_status(Adafruit_SSD1306 *display, wifi_status_t status);
void draw_battery_status(Adafruit_SSD1306 *display, int level, bool charging);
void draw_sun_icon(Adafruit_SSD1306 *display, bool sunny);
void draw_temperature(Adafruit_SSD1306 *display, float temp);

#endif // DRAW_H
