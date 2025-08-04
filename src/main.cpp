#include "main.h"
#include "config.h"
#include "ble.h"
#include "display.h"
#include "storage.h"
#include "wireless.h"
#include "logger.h"
#include "button.h"

static int wakeupFunction = -1;
static bool waitingWifi = false;

ButtonManager wakeupButtonManager(GPIO_BUTTON_PIN, LONG_PRESS_DURATION_MS, true);
WiFiConnectManager& wifiManager = WiFiConnectManager::getInstance();
DisplayManager displayManager;
BLEManager bleManager;
SleepManager sleepManager(INACTIVE_TIME_MS);

void setup() {
    Serial.begin(SERIAL_BAUD);

    // Pins configuration
    Wire.begin(I2C_SDA, I2C_SCL);

    // Buttons configuration
    wakeupButtonManager.begin();
    wakeupButtonManager.setShortPressHandler(handleButtonPress);
    wakeupButtonManager.setLongPressHandler(handleButtonLongPress);

    // Init sensors
    SensorsManager::init();

    // Setup wireless
    initWireless();

    // WiFi connection
    wifiManager.init();
    wifiManager.setStatusHandler(handleWiFiStatusChange);

    // Initialize display
    displayManager.setData(readPowerData(), SensorsManager::readSensors());

    // Initialize BLE
    bleManager.init();

    // Deep sleep configuration
    sleepManager.enableTimerWakeup(DEEPSLEEP_TIME_S);
    sleepManager.enableExtWakeup(GPIO_BUTTON_PIN);
    sleepManager.setSleepHandler(handleSleep);
    sleepManager.handleWakeup(handleWakeup);
    sleepManager.enableSleepTimer();
}

void loop(){
    static ulong lastUpdate = 0;
    if(millis() - lastUpdate > 3000){
        SensorsData sensors = SensorsManager::readSensors();
        power_data_t power = readPowerData();

        displayManager.setData(power, sensors);
        lastUpdate = millis();
    }
    
    wifiManager.loop();
    displayManager.loop();
    bleManager.loop();
    sleepManager.loop();
    wakeupButtonManager.loop();
}

// on short button press
void handleButtonPress(){
    log("press");
    sleepManager.resetSleepTimer();
    displayManager.cycle();
}

// on short button press
void handleButtonLongPress(){
    log("long press");
    if(wakeupFunction == 0){ // if woken up on timer and then long press turn on peripherals instead
        displayManager.powerOn();
        bleManager.start();
        wakeupFunction = 1;
    } else sleepManager.startSleep();
}

void handleWiFiStatusChange(WiFiStatus status){
    bleManager.handleWiFiStatusChange(status);
    
    if(status == WiFiStatus::CONNECTING){
        sleepManager.disableSleepTimer();
        return;
    }
    if(status == WiFiStatus::CONNECTED){
        displayManager.showNotification("Sending..");
        if(sendData(SensorsManager::readSensors(), readPowerData()) != 0){
            displayManager.showNotification("Fail!");
        }else {
            displayManager.showNotification("Sent!");
        }

        if(wakeupFunction == 0){
            sleepManager.startSleep();
        }
    }
    // on connected or disconnected:
    displayManager.refresh();
    sleepManager.enableSleepTimer(); 
}

void handleWakeup(WakeupReason wakeupReason){
    if(wakeupReason == WakeupReason::UNDEFINED){
        displayManager.powerOff();
        sleepManager.startSleep();
        return;
    }
    if(wakeupReason == WakeupReason::EXT){
        delay(LONG_PRESS_DURATION_MS / 2); // debounce - wait for a half of long press
        if(digitalRead(GPIO_BUTTON_PIN) == HIGH) {
            sleepManager.startSleep();
            return;
        }
    }

    wifiManager.begin(); // try connecting

    if(wakeupReason == WakeupReason::TIMER){
        log("Woke up from timer");
        wakeupFunction = 0;
        return;
    }
    if(wakeupReason == WakeupReason::EXT){
        log("Woke up from external interrupt");
        wakeupFunction = 1;

        displayManager.powerOn();
        bleManager.start();
        return;
    }
}

void handleSleep(){
    log("Going back to sleep");
    wakeupFunction = -1;
    displayManager.powerOff();
    bleManager.stop();
    wifiManager.disable();
    Serial.flush();
}
