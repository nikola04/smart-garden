#include "logger.h"
#include "config.h"
#include "storage.h"

void Logger::init(UARTDebug *uartDebugger){
    this->uartDebugger = uartDebugger;
    
    String modeString = getLoggerMode();
    if(modeString.equals("DEBUG")) mode = LoggerMode::DEBUG;
    else if(modeString.equals("INFO")) mode = LoggerMode::INFO;
    else if(modeString.equals("WARN")) mode = LoggerMode::WARN;
    else if(modeString.equals("OFF")) mode = LoggerMode::OFF;
    else mode = LoggerMode::ERROR;
}

void Logger::setMode(LoggerMode mode){
    this->mode = mode;
    setLoggerMode(getModeString().c_str());
}

LoggerMode Logger::getMode(){
    return this->mode;
}

String Logger::getModeString(){
    switch (this->mode) {
        case LoggerMode::INFO:  return "INFO";
        case LoggerMode::WARN:  return "WARN";
        case LoggerMode::ERROR: return "ERROR";
        case LoggerMode::DEBUG: return "DEBUG";
        case LoggerMode::OFF:
        default:                return "OFF";
    }
}

String formatMessage(const char* type, const char* tag, const char* message) {
    String msg;
    msg.reserve(64);
    msg += "[";
    msg += type;
    msg += "]: ";
    msg += tag;
    msg += " > ";
    msg += message;
    return msg;
}
void Logger::log(const char *tag, const char *message){
    if(mode < LoggerMode::INFO) return;
    uartDebugger->log(formatMessage("LOG", tag, message).c_str());
}

void Logger::warn(const char *tag, const char *message){
    if(mode < LoggerMode::WARN) return;
    uartDebugger->log(formatMessage("WARN", tag, message).c_str());
}

void Logger::error(const char *tag, const char *message){
    if(mode < LoggerMode::ERROR) return;
    uartDebugger->log(formatMessage("ERROR", tag, message).c_str());
}

void Logger::debug(const char *tag, const char *message){
    if(mode < LoggerMode::DEBUG) return;
    uartDebugger->log(formatMessage("DEBUG", tag, message).c_str());
}
