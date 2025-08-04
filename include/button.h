#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

class ButtonManager {
public:
    ButtonManager(int pin, int longPressDuration, bool wakeupState);

    static ulong lastButtonsPress;

    void begin();
    void loop();

    void setShortPressHandler(void (*handler)());
    void setLongPressHandler(void (*handler)());

private:
    int pin;
    bool wakeupState;
    int longPressDuration;

    bool isPressed;
    ulong lastPress;
    ulong pressStart;

    void (*shortPressHandler)();
    void (*longPressHandler)();
};

#endif // BUTTON_H
