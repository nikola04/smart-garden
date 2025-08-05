#ifndef DEBUG_H
#define DEBUG_H

#include "HardwareSerial.h"

class UARTDebug {
public:
    UARTDebug(HardwareSerial &serialPort = Serial, uint32_t baudRate = 115200);

    void begin();
    void loop();

private:
    HardwareSerial &serial;
    uint32_t baud;
    bool debugLogsEnabled;

    void printMenu();
    void processCommand(char cmd);
};

#endif // DEBUG_H
