#include "debug.h"
#include "network.h"
#include "ble.h"
#include "sleep.h"
#include "system.h"
// #include "buffer.h"

// Buffer buffer(64);

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
    if (c == '\n' || c == '\r') 
        return;

    serial.println(c);
    processCommand(c);
    serial.println();
    if(isEnabled) serial.print("> ");
}

void UARTDebug::log(const char* message){
    if(isEnabled) serial.print("\b\b \b"); // delete "> "
    serial.println(message);
    if(isEnabled) serial.print("> ");
}

void UARTDebug::enable(){
    isEnabled = true;
    printMenu(serial);
    serial.println();
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
    if(screen == DebugScreen::WIFI_MODE){
        processWiFiCommand(cmd);
        return;
    }
    serial.println("Unknown mode");
}

void UARTDebug::processMainCommand(char cmd){
    if(cmd == '?'){
        printMenu(serial);
    }else if(cmd == 'w'){
        screen = DebugScreen::WIFI_MODE;
        printWiFiMenu(serial);
    }else if(cmd == 'b'){
        printBLEStatus(serial);
    }else if(cmd == 'm'){
        screen = DebugScreen::LOG_MODE;
        printLogModeMenu(serial);
    }else if(cmd == 'x'){
        triggerRestart(nullptr);
    }else if(cmd == 's'){
        triggerDeepSleep(nullptr);
    }else if(cmd == 'q'){
        serial.println("Quiting debug mode..");
        isEnabled = false;
        serial.println("Restart device to enter debug again.");
    }else serial.println("Unknown command");
}

void UARTDebug::processWiFiCommand(char cmd){
    if(cmd == '?'){
        printWiFiMenu(serial);
        return;
    }
    if(cmd == 'b'){
        screen = DebugScreen::MAIN;
        printMenu(serial);
        return;
    }

    if(cmd == 's'){
        serial.println("Not supported yet.");
    }else if(cmd == 'm'){
        serial.println("Not supported yet.");
    }else {
        serial.println("Unknown command");
    }
}

void UARTDebug::processLogCommand(char cmd){
    if(cmd == '?'){
        printLogModeMenu(serial);
        return;
    }
    if(cmd == 'b'){
        screen = DebugScreen::MAIN;
        printMenu(serial);
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
    serial.println("Mode is set to: " + Logger::getInstance().getModeString());
    screen = DebugScreen::MAIN;
}
