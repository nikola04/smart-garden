#include "debug.h"
#include "network.h"
#include "ble.h"
#include "sleep.h"

UARTDebug::UARTDebug(HardwareSerial &serialPort, uint32_t baudRate) 
    : serial(serialPort), baud(baudRate), debugLogsEnabled(true) {}

void UARTDebug::begin(){
    serial.begin(baud);
    printMenu();
}

void UARTDebug::loop(){
    if (!serial.available()) return;

    char c = serial.read();
    if (c == '\n' || c == '\r') return;

    processCommand(c);
    // printMenu();
}

void UARTDebug::printMenu(){
    serial.println(F("\n--- DEBUG MENU ---"));
    serial.println(F("1. Show WiFi status"));
    serial.println(F("2. Show BLE status"));
    serial.println(F("3. Restart device"));
    serial.println(F("4. Toggle debug logs"));
    serial.println(F("5. Sleep now"));
    serial.println(F("x. Exit menu"));
    serial.print(F("Enter choice: "));
}

void UARTDebug::processCommand(char cmd) {
    serial.println();
    switch (cmd) {
        case '?':
            printMenu();
            break;
        case '1': {
            serial.print(F("WiFi status: "));
            WiFiStatus wifiStatus = WiFiConnectManager::getInstance().getStatus();
            if(wifiStatus == WiFiStatus::CONNECTED)
                serial.println("Connected");
            else if(wifiStatus == WiFiStatus::CONNECTED)
                serial.println("Connecting");
            else
                serial.println("Disconnected");
            break;
        } case '2': {
            serial.print(F("BLE status: "));
            BLEStatus bleStatus = BLEManager::getInstance().getStatus();
            if(bleStatus == BLEStatus::CONNECTED)
                serial.println("Connected");
            else if(bleStatus == BLEStatus::DISCONNECTED)
                serial.println("Disconnected");
            else
                serial.println("OFF");
            break;
        } case '3':
            serial.println(F("Restarting..."));
            ESP.restart();
            break;
        case '4':
            debugLogsEnabled = !debugLogsEnabled;
            serial.print(F("Debug logs "));
            serial.println(debugLogsEnabled ? "enabled" : "disabled");
            break;
        case '5':
            serial.println(F("Going to sleep now..."));
            SleepManager::getInstance().startSleep();
            break;
        case 'x':
        case 'X':
            serial.println(F("Exiting debug menu"));
            break;
        default:
            serial.println(F("Unknown command"));
            break;
    }
}
