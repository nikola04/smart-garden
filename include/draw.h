#ifndef DRAW_H
#define DRAW_H

#include "Adafruit_SSD1306.h"
#include "network.h"

void initDrawUtils(Adafruit_SSD1306 *display);
void drawText(Adafruit_SSD1306 *display, const char *string);
void drawTemplate(Adafruit_SSD1306 *display, float temp);
void drawWiFiStatus(Adafruit_SSD1306 *display, wifi_status_t status);
void drawBatteryStatus(Adafruit_SSD1306 *display, int level, bool charging);
void drawSunIcon(Adafruit_SSD1306 *display, bool sunny);
void drawTemperature(Adafruit_SSD1306 *display, float temp);

#endif // DRAW_H
