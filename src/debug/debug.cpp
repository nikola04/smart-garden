#include "debug.h"
#include "network.h"
#include "ble.h"
#include "sleep.h"
#include "system.h"

UARTDebug::UARTDebug(HardwareSerial &serialPort, uint32_t baudRate): serial(serialPort), baud(baudRate) {
    isEnabled = false;
}

void UARTDebug::begin(){
    serial.begin(baud);
}

void UARTDebug::loop(){
    if (!isEnabled || !serial.available()) return;

    char c = serial.read();
    if (c == '\n' || c == '\r') return;
    serial.println(c);
    processCommand(c);
    serial.println("");
    if(isEnabled) serial.print("> ");
}

void UARTDebug::log(const char* message){
    if(isEnabled) serial.print("\b\b \b"); // delete "> "
    serial.println(message);
    if(isEnabled) serial.print("> ");
}

void UARTDebug::enable(){
    isEnabled = true;
    printMenu();
    serial.print("> ");
}

void UARTDebug::disable(){
    if(!isEnabled) return;
    serial.print("\b\b \b"); // delete "> "
    isEnabled = false;
}

void UARTDebug::processCommand(char cmd) {
    if(cmd == '?'){
        printMenu();
        return;
    }
    if(cmd == 'w'){
        printWiFiStatus();
        return;
    }
    if(cmd == 'b'){
        printBLEStatus();
        return;
    }
    if(cmd == 'x'){
        triggerRestart(nullptr);
        return;
    }
    if(cmd == 's'){
        triggerDeepSleep(nullptr);
        return;
    }
    if(cmd == 'q'){
        serial.println("Quiting debug mode..");
        isEnabled = false;
        serial.println("Restart device to enter debug again.");
        return;
    }
    
    serial.println("Unknown command");
}

void UARTDebug::printMenu(){
    serial.println(F("\n--- SmartGarden Command-Line Interface ---"));
    serial.println("");
    serial.println("General:");
    serial.println(F("?     : Display this help menu"));
    serial.println(F("w     : Show WiFi status"));
    serial.println(F("b     : Show BLE status"));
    serial.println(F("x     : Restart device"));
    serial.println(F("s     : Sleep now"));
    serial.println(F("q     : Quit debug mode"));
    serial.println("");
}

void UARTDebug::printWiFiStatus(){
    serial.print(F("WiFi status: "));
    WiFiStatus wifiStatus = WiFiConnectManager::getInstance().getStatus();
    if(wifiStatus == WiFiStatus::CONNECTED)
        serial.println("Connected");
    else if(wifiStatus == WiFiStatus::CONNECTED)
        serial.println("Connecting");
    else
        serial.println("Disconnected");
}

void UARTDebug::printBLEStatus(){
    serial.print(F("BLE status: "));
    BLEStatus bleStatus = BLEManager::getInstance().getStatus();
    if(bleStatus == BLEStatus::CONNECTED)
        serial.println("Connected");
    else if(bleStatus == BLEStatus::DISCONNECTED)
        serial.println("Disconnected");
    else
        serial.println("Off");
}
