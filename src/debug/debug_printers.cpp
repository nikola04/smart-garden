#include "debug.h"
#include "logger.h"
#include "network.h"
#include "ble.h"

void printMenu(HardwareSerial &serial){
    serial.println(F("\n\\\\\\ SmartGarden Debug Interface ///"));
    serial.println("");
    serial.println("Commands:");
    serial.println(F("?     : Display this help menu"));
    serial.println(F("w     : Configure WiFi network"));
    serial.println(F("b     : Show BLE status"));
    serial.println(F("m     : Edit Logger mode"));
    serial.println(F("x     : Restart device"));
    serial.println(F("s     : Sleep now"));
    serial.println(F("q     : Quit debug mode"));
}

void printLogModeMenu(HardwareSerial &serial){
    String currentMode = Logger::getInstance().getModeString();

    serial.println("\n\\\\\\ Logger mode selector ///");
    serial.println("");
    serial.println("Current: " + currentMode);
    serial.println("");
    serial.println("Commands:");
    serial.println(F("?     : Display this logger menu"));
    serial.println(F("0     : Turn off"));
    serial.println(F("1     : Enable INFO mode"));
    serial.println(F("2     : Enable WARN mode"));
    serial.println(F("3     : Enable ERROR mode (recommended)"));
    serial.println(F("4     : Enable DEBUG mode"));
    serial.println(F("b     : Go to Main Menu"));
}

void printWiFiMenu(HardwareSerial &serial){
    String currentMode = Logger::getInstance().getModeString();

    serial.println("\n\\\\\\ WiFi Configuration ///");
    serial.println("");
    printWiFiStatus(serial);
    serial.println("");
    serial.println("Commands:");
    serial.println(F("?     : Display this wifi configuration menu"));
    serial.println(F("s     : Scan WiFi networks"));
    serial.println(F("m     : Enter configuration manually"));
    serial.println(F("b     : Go to Main Menu"));
}

void printWiFiStatus(HardwareSerial &serial){
    serial.print(F("Status: "));
    WiFiStatus wifiStatus = WiFiConnectManager::getInstance().getStatus();
    if(wifiStatus == WiFiStatus::CONNECTED)
        serial.println("Connected");
    else if(wifiStatus == WiFiStatus::CONNECTED)
        serial.println("Connecting");
    else
        serial.println("Disconnected");
}

void printBLEStatus(HardwareSerial &serial){
    serial.print(F("BLE status: "));
    BLEStatus bleStatus = BLEManager::getInstance().getStatus();
    if(bleStatus == BLEStatus::CONNECTED)
        serial.println("Connected");
    else if(bleStatus == BLEStatus::DISCONNECTED)
        serial.println("Disconnected");
    else
        serial.println("Off");
}
