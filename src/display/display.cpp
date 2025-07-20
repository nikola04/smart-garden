#include "display.h"
#include "draw.h"
#include "Wire.h"

DisplayManager::DisplayManager(): display(128, 32, &Wire, -1),
    displayFunctions{ &DisplayManager::drawPower, &DisplayManager::drawSoil, &DisplayManager::drawAir, &DisplayManager::drawBattery }
{}

void DisplayManager::powerOn() {
    if (displayOn) return;

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) return;

    displayOn = true;
    initDrawUtils(&display);
    drawTemplate(&display);
}

void DisplayManager::powerOff() {
    if (!displayOn) return;

    display.clearDisplay();
    display.display();
    display.ssd1306_command(SSD1306_DISPLAYOFF);
    displayOn = false;
}

bool DisplayManager::isOn() const {
    return displayOn;
}

void DisplayManager::refresh() {
    drawTemplate(&display);
    (this->*displayFunctions[currentMode])();
    lastRefresh = millis();
}

void DisplayManager::checkWiFiStatus() {
    if (!displayOn) return;

    wifi_status_t currentStatus = wifiGetStatus();
    if (currentStatus != previousWiFiStatus || 
        (currentStatus == WIFI_CONNECTING && millis() - lastRefresh > 650)) {
        previousWiFiStatus = currentStatus;
        refresh();
    }
}

void DisplayManager::cycle() {
    currentMode = (currentMode + 1) % displayCount;
}

void DisplayManager::showNotification(const char* message) {
    drawText(&display, message);
}

void DisplayManager::setData(const power_data_t& p, const sensors_data_t& s) {
    powerData = p;
    sensorData = s;
}

void DisplayManager::drawPower() {}
void DisplayManager::drawSoil() {}
void DisplayManager::drawAir() {}
void DisplayManager::drawBattery() {}
