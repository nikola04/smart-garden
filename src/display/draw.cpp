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
    display->setCursor(0, 22);
    display->println(string);
}

void drawTemplate(Adafruit_SSD1306 *display, WiFiStatus wifiStatus, BLEStatus bleStatus, float temp){
    display->clearDisplay();
    // wifi sunny temp       charging battery
    drawWiFiStatus(display, wifiStatus);
    drawBLE(display, bleStatus);
    drawTemperature(display, temp);
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
void drawWiFiStatus(Adafruit_SSD1306 *display, WiFiStatus status){
    const int x = 0;
    const int y = 0;

    const int cx = x + 8;
    const int cy = y + 8;

    switch(status){
        case WiFiStatus::CONNECTED:
            drawArc(display, cx, cy, 8, 210, 330);
            drawArc(display, cx, cy, 5, 210, 330);
            display->fillCircle(cx, cy, 1, WHITE);
            break;

        case WiFiStatus::CONNECTING:
            if(connecting_icon_state == 1){
                drawArc(display, cx, cy, 5, 210, 330);
            }
            display->fillCircle(cx, cy, 1, WHITE);
            connecting_icon_state = (connecting_icon_state + 1) % 2;
            break;

        case WiFiStatus::DISCONNECTED:
        default:
            drawArc(display, cx, cy, 8, 210, 330);
            drawArc(display, cx, cy, 5, 210, 330);
            display->fillCircle(cx, cy, 1, WHITE);

            const int x_offset = cx + 3;
            const int y_offset = cy - 2;
            display->drawLine(x_offset, y_offset, x_offset + 4, y_offset + 4, WHITE);
            display->drawLine(x_offset + 4, y_offset, x_offset, y_offset + 4, WHITE);
            break;
    }
}

void drawBLE(Adafruit_SSD1306 *display, BLEStatus status){
    if(status == BLEStatus::CONNECTED){
        display->setTextSize(1);
        display->setCursor(22, 4);
        display->println("BLE");
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
    int16_t y = 4;

    display->setCursor(x, y);
    display->print(tempStr);
}
