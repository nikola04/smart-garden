#ifndef LOGGER_H
#define LOGGER_H

#include "debug.h"

enum class LoggerMode{
    OFF = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
    DEBUG = 4
};

class Logger{
public:
    static Logger& getInstance(){
        static Logger instance;
        return instance;
    }
    void init(UARTDebug* uartDebugger);

    void setMode(LoggerMode mode);
    LoggerMode getMode();
    String getModeString();

    void log(const char *tag, const char *message);
    void warn(const char *tag, const char *message);
    void error(const char *tag, const char *message);
    void debug(const char *tag, const char *message);
private:
    UARTDebug *uartDebugger = nullptr;
    LoggerMode mode = LoggerMode::ERROR;
};

#endif // LOGGER_H
