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
    refresh();
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
    WiFiStatus wifiStatus = WiFiConnectManager::getInstance().getStatus();
    drawTemplate(&display, wifiStatus, sensorData.air.temperature);
    if(currentMode >= 0) {
        (this->*displayFunctions[currentMode])();
    }
    display.display();
    lastRefresh = millis();
}

void DisplayManager::loop() {
    if (!displayOn) return;

    WiFiStatus wifiStatus = WiFiConnectManager::getInstance().getStatus();
    if(wifiStatus == WiFiStatus::CONNECTING && millis() - lastRefresh > 1000){
        refresh();
    }
}

void DisplayManager::cycle() {
    currentMode = (currentMode + 1) % displayCount;
    this->refresh();
}

void DisplayManager::showNotification(const char* message) {
    currentMode = -1;
    drawText(&display, message);
}

void DisplayManager::setData(const power_data_t& p, const SensorsData& s) {
    powerData = p;
    sensorData = s;
}

void DisplayManager::drawPower() {
    drawText(&display, "display: 1");
}
void DisplayManager::drawSoil() {
    drawText(&display, "display: 2");
}
void DisplayManager::drawAir() {
    drawText(&display, (String(sensorData.air.pressure) + " mbar").c_str());
}
void DisplayManager::drawBattery() {
    drawText(&display, "display: 4");
}
