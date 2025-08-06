#ifndef DEBUG_H
#define DEBUG_H

#include "HardwareSerial.h"

enum class DebugScreen{
    MAIN,
    LOG_MODE
};

class UARTDebug {
public:
    UARTDebug(HardwareSerial &serialPort = Serial, uint32_t baudRate = 115200);

    void begin();
    void loop();

    void log(const char* message);

    void enable();
    void disable();

private:
    HardwareSerial &serial;
    uint32_t baud;

    bool isEnabled;
    DebugScreen screen;

    void processCommand(char cmd);
    void processMainCommand(char cmd);
    void processLogCommand(char cmd);
    void printMenu();
    void printLogModeMenu();
    void printWiFiStatus();
    void printBLEStatus();
};

#endif // DEBUG_H
