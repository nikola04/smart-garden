#include "logger.h"
#include "config.h"

void Logger::init(UARTDebug *uartDebugger){
    this->uartDebugger = uartDebugger;
}

void Logger::setMode(LoggerMode mode){
    this->mode = mode;
}

LoggerMode Logger::getMode(){
    return this->mode;
}

String Logger::getModeString(){
    switch (this->mode) {
        case LoggerMode::OFF:   return "OFF";
        case LoggerMode::INFO:  return "INFO";
        case LoggerMode::WARN:  return "WARN";
        case LoggerMode::ERROR: return "ERROR";
        case LoggerMode::DEBUG: return "DEBUG";
        default:                return "UNKNOWN";
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
