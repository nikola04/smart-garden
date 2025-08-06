#include "system.h"
#include "esp_system.h"
#include "sleep.h"
#include "logger.h"

void triggerRestart(void* arg){
    Logger::getInstance().log("System", "restarting..");
    esp_restart();
}

void triggerDeepSleep(void* arg){
    SleepManager::getInstance().startSleep();
}
