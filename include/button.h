#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

class ButtonManager {
public:
    ButtonManager(int pin, int longPressDuration, int holdDelay, bool wakeupState);

    static ulong lastButtonsPress;

    void begin();
    void loop();

    void setShortPressHandler(void (*handler)());
    void setLongPressHandler(void (*handler)());
    void setHoldHandler(void (*handler)());
    void setReleaseHandler(void (*handler)());

private:
    int pin;
    bool wakeupState;
    int longPressDuration;
    int holdDelay;

    bool isHold;
    bool isPressed;
    ulong lastPress;
    ulong pressStart;

    void (*shortPressHandler)();
    void (*longPressHandler)();
    void (*holdHandler)();
    void (*releaseHandler)();
};

#endif // BUTTON_H
