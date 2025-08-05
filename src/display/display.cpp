#include "display.h"
#include "draw.h"
#include "logger.h"
#include "ble.h"
#include "Wire.h"

DisplayManager::DisplayManager(): display(128, 32, &Wire, -1){

}

void DisplayManager::init() {
    displayFunctions[0] = &DisplayManager::drawPower;
    displayFunctions[1] = &DisplayManager::drawSoil;
    displayFunctions[2] = &DisplayManager::drawAir;
    displayFunctions[3] = &DisplayManager::drawBattery;
    mode = DisplayMode::NOTIFICATION;
    notificationText = "Hello!";
    displayFn = 0;
}

void DisplayManager::powerOn() {
    if (displayOn) return;

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) return;

    displayOn = true;
    initDrawUtils(&display);
    refresh();
}

void DisplayManager::powerOff() {
    if (!displayOn && !display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) return;

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
    BLEStatus bleStatus = BLEManager::getInstance().getStatus();

    drawTemplate(&display, wifiStatus, bleStatus, sensorData.air.temperature);
    if(mode == DisplayMode::SCREEN) {
        if (displayFn < displayFnCount && displayFunctions[displayFn]) 
            (this->*displayFunctions[displayFn])();
    }else {
        drawText(&display, notificationText.c_str());
    }
    display.display();
    lastRefresh = millis();
}

void DisplayManager::loop() {
    if (!displayOn) return;

    WiFiStatus wifiStatus = WiFiConnectManager::getInstance().getStatus();
    if((wifiStatus == WiFiStatus::CONNECTING && millis() - lastRefresh > 750) 
        || millis() - lastRefresh > 2500){
        refresh();
    }
}

void DisplayManager::showNotification(const char* message) {
    this->notificationText = String(message);
    this->mode = DisplayMode::NOTIFICATION;
    refresh();
}

void DisplayManager::cycle() {
    if(this->mode == DisplayMode::SCREEN)
        this->displayFn = (displayFn + 1) % displayFnCount;
    else {
        this->mode = DisplayMode::SCREEN;
        this->displayFn = 0;
    } 
    this->refresh();
}

void DisplayManager::screenMode(){
    this->displayFn = 0;
    this->mode = DisplayMode::SCREEN;
    this->refresh();
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

void DisplayManager::setData(const power_data_t& p, const SensorsData& s) {
    powerData = p;
    sensorData = s;
}
