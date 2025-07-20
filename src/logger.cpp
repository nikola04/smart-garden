#include "logger.h"
#include "config.h"

void log(const char *message) {
    Serial.println(message);
}


void loggerInit() {
    Serial.begin(SERIAL_BAUD);
}
