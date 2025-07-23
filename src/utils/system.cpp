#include "system.h"
#include "esp_timer.h"
#include "esp_system.h"
#include "esp_sleep.h"

void triggerRestart(void* arg){
    esp_restart();
}

void triggerDeepSleep(void* arg){
    esp_deep_sleep_start();
}
