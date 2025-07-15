#include "draw.h"
#include "display.h"

void init_draw_utils(Adafruit_SSD1306 *display){
    if(!is_display_on) return;

    display->clearDisplay();
    display->display();
    display->setTextSize(2);
    display->setTextColor(WHITE);
    display->setCursor(0, 0);
}

void draw_text(Adafruit_SSD1306 *display, const char *string){
    display->setCursor(32, 10);
    display->println(string);
    display->display();
}

void draw_template(Adafruit_SSD1306 *display){
    if(!is_display_on) return;

    display->clearDisplay();
    // wifi sunny temp       charging battery
    draw_wifi_status(display, get_wifi_status());
    display->display();
}

void drawArc(Adafruit_SSD1306 *display, int cx, int cy, int radius, int start_angle, int end_angle){
    int step = 5; 
    for(int angle = start_angle; angle < end_angle; angle += step){
        float rad1 = angle * M_PI / 180.0;
        float rad2 = (angle + step) * M_PI / 180.0;

        int x1 = cx + (int)(radius * cos(rad1));
        int y1 = cy + (int)(radius * sin(rad1));
        int x2 = cx + (int)(radius * cos(rad2));
        int y2 = cy + (int)(radius * sin(rad2));

        display->drawLine(x1, y1, x2, y2, WHITE);
    }
}

static int connecting_icon_state = 0;
void draw_wifi_status(Adafruit_SSD1306 *display, wifi_status_t status){
    int x = 12;
    int y = 12;

    switch(status){
        case WIFI_CONNECTED:
            drawArc(display, x, y, 12, 220, 320);
            drawArc(display, x, y, 8, 210, 330);
            display->fillCircle(x, y, 2, WHITE);
            break;

        case WIFI_CONNECTING:
            if(connecting_icon_state == 1){
                drawArc(display, x, y, 8, 210, 330);
            }
            display->fillCircle(x, y, 2, WHITE);
            connecting_icon_state = (connecting_icon_state + 1) % 2;
            break;

        case WIFI_DISCONNECTED:
        default:
            drawArc(display, x, y, 12, 210, 330);
            drawArc(display, x, y, 8, 210, 330);
            display->fillCircle(x, y, 2, WHITE);
            int x_offset = x + 5;
            int y_offset = y - 2;
            display->drawLine(x_offset, y_offset, x_offset + 6, y_offset + 6, WHITE);
            display->drawLine(x_offset + 6, y_offset, x_offset, y_offset + 6, WHITE);
            break;
    }
}

void draw_battery_status(Adafruit_SSD1306 *display, int level, bool charging){
}

void draw_sun_icon(Adafruit_SSD1306 *display, bool sunny){
    int x = 32;
    int y = 10;
    switch (sunny){
        case true:
            display->fillCircle(x, y, 4, WHITE);
            break;
        case false:
        default:
            drawArc(display, x, y, 4, 120, 220);
            break;
    }
}

void draw_temperature(Adafruit_SSD1306 *display, float temp){
}
