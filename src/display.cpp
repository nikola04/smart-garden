#include "Arduino.h"
#include "config.h"
#include "network.h"
#include "display.h"
#include "draw.h"

Adafruit_SSD1306 display(128, 32, &Wire, -1);
static bool display_on = false;
static ushort current_mode = 0;
static wifi_status_t wifi_status = WIFI_DISCONNECTED;
static power_data_t power_data;
static sensors_data_t sensors_data;

void display_power_data();
void display_soil_data();
void display_air_data();
void display_battery_data();

void (*displays[])() = { display_power_data, display_soil_data, display_air_data, display_battery_data };
short displays_count = sizeof(displays) / sizeof(displays[0]);

static ulong last_refresh = 0;
void refresh_display(){
    draw_template(&display);
    displays[current_mode]();
    last_refresh = millis();
}

bool is_display_on(){
    return display_on;
}

void display_power_on() {
    if(display_on)
        return;

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
        return;

    display_on = true;
    init_draw_utils(&display);
    draw_template(&display);
}

void display_power_off() {
    if(!display_on)
        return;
    display.clearDisplay();
    display.display();
    display.ssd1306_command(SSD1306_DISPLAYOFF);
    display_on = false;
}

void display_check_wifi_status_loop(){
    if(!display_on)
        return;
    wifi_status_t status = get_wifi_status();
    if(status != wifi_status || (status == WIFI_CONNECTING && millis() - last_refresh > 650)) {
        wifi_status = status;
        refresh_display();
    }
}

void cycle_display() {
    current_mode++;
    if(current_mode >= displays_count)
        current_mode = 0;
}

void display_notification(const char *string){
    draw_text(&display, string);
}

void display_power_data() {

}

void display_soil_data() {

}

void display_air_data() {

}

void display_battery_data() {

}

void set_display_data(power_data_t power_d, sensors_data_t sensor_d) {
    power_data = power_d;
    sensors_data = sensor_d;
}
