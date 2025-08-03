#include "draw.h"

void initDrawUtils(Adafruit_SSD1306 *display){
    display->clearDisplay();
    display->display();
    display->setTextColor(WHITE);
    display->setFont();
    display->setCursor(0, 0);
}

void drawText(Adafruit_SSD1306 *display, const char *string){
    display->setTextSize(1);
    display->setCursor(24, 10);
    display->println(string);
    display->display();
}

void drawTemplate(Adafruit_SSD1306 *display, float temp){
    display->clearDisplay();
    // wifi sunny temp       charging battery
    drawWiFiStatus(display, wifiGetStatus());
    drawTemperature(display, temp);
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
void drawWiFiStatus(Adafruit_SSD1306 *display, wifi_status_t status){
    int x = 8;
    int y = 10;

    switch(status){
        case WIFI_CONNECTED:
            drawArc(display, x, y, 10, 210, 330);
            drawArc(display, x, y, 6, 210, 330);
            display->fillCircle(x, y, 2, WHITE);
            break;

        case WIFI_CONNECTING:
            if(connecting_icon_state == 1){
                drawArc(display, x, y, 6, 210, 330);
            }
            display->fillCircle(x, y, 2, WHITE);
            connecting_icon_state = (connecting_icon_state + 1) % 2;
            break;

        case WIFI_DISCONNECTED:
        default:
            drawArc(display, x, y, 10, 210, 330);
            drawArc(display, x, y, 6, 210, 330);
            display->fillCircle(x, y, 2, WHITE);
            int x_offset = x + 4;
            int y_offset = y - 1;
            display->drawLine(x_offset, y_offset, x_offset + 5, y_offset + 5, WHITE);
            display->drawLine(x_offset + 5, y_offset, x_offset, y_offset + 5, WHITE);
            break;
    }
}

void drawBatteryStatus(Adafruit_SSD1306 *display, int level, bool charging){
}

void drawSunIcon(Adafruit_SSD1306 *display, bool sunny){
    int x = 32;
    int y = 10;
    if(sunny){
        display->fillCircle(x, y, 4, WHITE);
    } else{
        drawArc(display, x, y, 4, 120, 220);
    }
}

void drawTemperature(Adafruit_SSD1306 *display, float temp){
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);

    String tempStr = String(temp, 1) + " " + (char)248 + "C"; // e.g. "27.3 °C"

    int16_t x1, y1;
    uint16_t w, h;
    display->getTextBounds(tempStr, 0, 0, &x1, &y1, &w, &h);

    int16_t x = 128 - w - 2;
    int16_t y = 0;

    display->setCursor(x, y);
    display->print(tempStr);
}
