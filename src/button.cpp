#include "button.h"

ulong ButtonManager::lastButtonsPress = 0;

ButtonManager::ButtonManager(int pin, int longPressDuration, int holdDelay, bool wakeupState){
    this->pin = pin;
    this->wakeupState = wakeupState;
    this->longPressDuration = longPressDuration;
    this->holdDelay = holdDelay;

    this->pressStart = 0;
    this->lastPress = 0;
    this->isPressed = false;

    this->shortPressHandler = nullptr;
    this->longPressHandler = nullptr;
}

void ButtonManager::begin() {
    pinMode(this->pin, INPUT_PULLUP);
}

void ButtonManager::loop() {
    int pinValue = digitalRead(pin);

    if(wakeupState) {
        if(pinValue == LOW) return; // still not released
        
        wakeupState = false;
    }

    if(!isPressed && pinValue == LOW) { // first press
        isPressed = true;
        pressStart = millis();
        return;
    }

    if(!isHold && pinValue == LOW){ 
        if(millis() - pressStart > holdDelay) {
            isHold = true;
            holdHandler();
        }
    }

    if(isHold && pinValue == HIGH){
        isHold = false;
        isPressed = false;
        releaseHandler();
        return;
    }
    
    if(isPressed && pinValue == HIGH){ // release
        lastButtonsPress = lastPress = millis();
        isPressed = false;
        ulong pressDuration = lastPress - pressStart;

        if(pressDuration < 30) return; // debounce

        if(pressDuration >= longPressDuration){ // long press
            if(longPressHandler) longPressHandler();
            return;
        }
        if(shortPressHandler) shortPressHandler();
    }
}

void ButtonManager::setShortPressHandler(void (*handler)()) {
    shortPressHandler = handler;
}

void ButtonManager::setLongPressHandler(void (*handler)()) {
    longPressHandler = handler;
}

void ButtonManager::setHoldHandler(void (*handler)()){
    holdHandler = handler;
}

void ButtonManager::setReleaseHandler(void (*handler)()){
    releaseHandler = handler;
}
