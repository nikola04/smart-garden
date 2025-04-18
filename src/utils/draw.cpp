#include "draw.h"

Adafruit_SSD1306 *display = NULL;

void init_draw_utils(Adafruit_SSD1306 *display){
    display->setTextSize(2);
    display->setTextColor(WHITE);
    display->setCursor(0, 0);
}

void draw_template(){
    // wifi sunny temp       charging battery
}

void draw_wifi_status(wifi_status_t status){
}

void draw_battery_status(int level, bool charging){
}

void draw_sun_icon(bool sunny){
}

void draw_temperature(float temp){
}
