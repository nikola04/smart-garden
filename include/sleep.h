#ifndef SLEEP_H
#define SLEEP_H

#include "Arduino.h"
#include "esp_sleep.h"

enum class WakeupReason {
    EXT,
    TIMER,
    UNDEFINED
};

class SleepManager {
public: 
    SleepManager(int sleepTimer);

    void loop();
    void enableSleepTimer();
    void disableSleepTimer();
    void resetSleepTimer();

    void setSleepHandler(void (*handler)());
    void enableExtWakeup(int pin);
    void enableTimerWakeup(int timeSec);

    void handleWakeup(void (*handler)(WakeupReason));
    void startSleep();

private:
    bool timerEnabled;
    ulong timerStart;
    ulong timerDuration;

    void (*sleepHandler)();
};

#endif // SLEEP_H
