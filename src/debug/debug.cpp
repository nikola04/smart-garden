#include "debug.h"
#include "network.h"
#include "ble.h"
#include "sleep.h"
#include "system.h"

UARTDebug::UARTDebug(HardwareSerial &serialPort, uint32_t baudRate): serial(serialPort), baud(baudRate) {
    isEnabled = false;
    screen = DebugScreen::MAIN;
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
    if(screen == DebugScreen::MAIN){
        processMainCommand(cmd);
        return;
    }
    if(screen == DebugScreen::LOG_MODE){
        processLogCommand(cmd);
        return;
    }
    serial.println("Unknown mode");
}

void UARTDebug::processMainCommand(char cmd){
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
    if(cmd == 'm'){
        screen = DebugScreen::LOG_MODE;
        printLogModeMenu();
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

String loggerModeToString(LoggerMode mode){
    switch (mode) {
        case LoggerMode::OFF:   return "OFF";
        case LoggerMode::INFO:  return "INFO";
        case LoggerMode::WARN:  return "WARN";
        case LoggerMode::ERROR: return "ERROR";
        case LoggerMode::DEBUG: return "DEBUG";
        default:                return "UNKNOWN";
    }
}

void UARTDebug::processLogCommand(char cmd){
    if(cmd == '?'){
        printLogModeMenu();
        return;
    }
    if(cmd == 'b'){
        screen = DebugScreen::MAIN;
        printMenu();
        return;
    }

    if(cmd == '0'){
        Logger::getInstance().setMode(LoggerMode::OFF);
    }else if(cmd == '1'){
        Logger::getInstance().setMode(LoggerMode::INFO);
    }else if(cmd == '2'){
        Logger::getInstance().setMode(LoggerMode::WARN);
    }else if(cmd == '3'){
        Logger::getInstance().setMode(LoggerMode::ERROR);
    }else if(cmd == '4'){
        Logger::getInstance().setMode(LoggerMode::DEBUG);
    }else {
        serial.println("Unknown command");
        return;
    }
    serial.println("Mode is set to: " + loggerModeToString(Logger::getInstance().getMode()));
    screen = DebugScreen::MAIN;
}

void UARTDebug::printMenu(){
    serial.println(F("\n\\\\\\ SmartGarden Debug Interface ///"));
    serial.println("");
    serial.println("Commands:");
    serial.println(F("?     : Display this help menu"));
    serial.println(F("w     : Show WiFi status"));
    serial.println(F("b     : Show BLE status"));
    serial.println(F("m     : Edit Logger mode"));
    serial.println(F("x     : Restart device"));
    serial.println(F("s     : Sleep now"));
    serial.println(F("q     : Quit debug mode"));
    serial.println("");
}

void UARTDebug::printLogModeMenu(){
    String currentMode = loggerModeToString(Logger::getInstance().getMode());

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
