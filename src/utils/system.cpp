#include "system.h"
#include "esp_system.h"
#include "sleep.h"

void triggerRestart(void* arg){
    esp_restart();
}

void triggerDeepSleep(void* arg){
    SleepManager::getInstance().startSleep();
}
