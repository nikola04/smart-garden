#include "sleep.h"
#include "logger.h"

SleepManager::SleepManager(){
    timerEnabled = false;
    timerStart = 0;
}

void SleepManager::init(int sleepTimer){
    timerDuration = sleepTimer;
}

void SleepManager::enableExtWakeup(int pin){
    esp_sleep_enable_ext0_wakeup((gpio_num_t) pin, 0);
}

void SleepManager::enableTimerWakeup(int timeSec){
    uint64_t wakeupTimerUS = static_cast<uint64_t>(timeSec) * 1000000ULL; // time in microSec
    esp_sleep_enable_timer_wakeup(wakeupTimerUS);
}

void SleepManager::handleWakeup(void (*handler)(WakeupReason)){
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    switch(wakeup_reason) {
        case ESP_SLEEP_WAKEUP_EXT0:
            handler(WakeupReason::EXT);
            return;
        case ESP_SLEEP_WAKEUP_TIMER:
            handler(WakeupReason::TIMER);
            return;
        default:
            handler(WakeupReason::UNDEFINED);
    }
}

void SleepManager::setSleepHandler(void (*handler)()){
    sleepHandler = handler;
}

void SleepManager::startSleep(){
    sleepHandler();
    log("sleeping now...");
    esp_deep_sleep_start();
}

void SleepManager::loop()
{
    if(!timerEnabled) return;
    if(millis() - timerStart < timerDuration) return;

    this->startSleep();
}

void SleepManager::enableSleepTimer(){
    if(timerEnabled) return;

    timerStart = millis();
    timerEnabled = true;
}

void SleepManager::disableSleepTimer(){
    timerEnabled = false;
}

void SleepManager::resetSleepTimer(){
    timerStart = millis();
}
