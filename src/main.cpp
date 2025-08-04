#include "main.h"
#include "config.h"
#include "ble.h"
#include "display.h"
#include "storage.h"
#include "wireless.h"
#include "logger.h"
#include "button.h"
#include "json.h"

static int wakeupFunction = -1;

ButtonManager wakeupButtonManager(GPIO_BUTTON_PIN, LONG_PRESS_DURATION_MS, HOLD_DELAY_MS, true);
DisplayManager displayManager;
APIClient apiClient;

void setup() {
    Serial.begin(SERIAL_BAUD);

    // Pins configuration
    Wire.begin(I2C_SDA, I2C_SCL);

    // Buttons configuration
    wakeupButtonManager.begin();
    wakeupButtonManager.setShortPressHandler(handleButtonPress);
    wakeupButtonManager.setLongPressHandler(handleButtonLongPress);
    wakeupButtonManager.setHoldHandler(handleButtonHold);
    wakeupButtonManager.setReleaseHandler(handleButtonRelease);

    // Initialize sensors
    SensorsManager::init();

    // Setup wireless
    initWireless();

    // WiFi connection
    WiFiConnectManager::getInstance().init();
    WiFiConnectManager::getInstance().setStatusHandler(handleWiFiStatusChange);

    // Initialize display
    displayManager.init();
    displayManager.setData(readPowerData(), SensorsManager::readSensors());

    // Initialize API Client
    apiClient.init(DEFAULT_API_URL, getAPIKey());

    // Initialize BLE
    BLEManager::getInstance().init();
    BLEManager::getInstance().setConnectHandler(handleBLEConnect);
    BLEManager::getInstance().setDisconnectHandler(handleBLEDisconnect);

    // Deep sleep configuration
    SleepManager::getInstance().init(INACTIVE_TIME_MS);
    SleepManager::getInstance().enableTimerWakeup(DEEPSLEEP_TIME_S);
    SleepManager::getInstance().enableExtWakeup(GPIO_BUTTON_PIN);
    SleepManager::getInstance().setSleepHandler(handleSleep);
    SleepManager::getInstance().handleWakeup(handleWakeup);
    SleepManager::getInstance().enableSleepTimer();
}

void loop(){
    static ulong lastUpdate = 0;
    if(millis() - lastUpdate > 3000){
        SensorsData sensors = SensorsManager::readSensors();
        power_data_t power = readPowerData();

        displayManager.setData(power, sensors);
        lastUpdate = millis();
    }
    
    WiFiConnectManager::getInstance().loop();
    displayManager.loop();
    BLEManager::getInstance().loop();
    SleepManager::getInstance().loop();
    wakeupButtonManager.loop();
}

// on short button press
void handleButtonPress(){
    log("press");
    SleepManager::getInstance().resetSleepTimer();
    displayManager.cycle();
}

// on short button press
void handleButtonLongPress(){
    log("long press");
    if(wakeupFunction == 0){ // if woken up on timer and then long press turn on peripherals instead
        log("turning on peripherals");
        displayManager.powerOn();
        BLEManager::getInstance().start();
        wakeupFunction = 1;
    } else SleepManager::getInstance().startSleep();
}

bool isBLEConnected = false;
bool isWiFiConnecting = false;
bool isBtnHold = false;
void handleButtonHold(){
    log("hold");
    isBtnHold = true;
    SleepManager::getInstance().disableSleepTimer();
    BLEManager::getInstance().start();
}

void handleButtonRelease(){
    log("release");
    isBtnHold = false;
    if(!isBLEConnected && !isWiFiConnecting) SleepManager::getInstance().enableSleepTimer();
    BLEManager::getInstance().stop();
}

void handleBLEConnect(){
    isBLEConnected = true;
    SleepManager::getInstance().disableSleepTimer();
}
void handleBLEDisconnect(){
    isBLEConnected = false;
    if(!isWiFiConnecting && !isBtnHold) SleepManager::getInstance().enableSleepTimer();
}

void handleWiFiStatusChange(WiFiStatus status){
    BLEManager::getInstance().handleWiFiStatusChange(status);
    
    if(status == WiFiStatus::CONNECTING){
        isWiFiConnecting = true;
        SleepManager::getInstance().disableSleepTimer();
        return;
    }
    if(status == WiFiStatus::CONNECTED){
        displayManager.showNotification("Sending..");

        String apiPayload = stringifyAPIData(SensorsManager::readSensors(), readPowerData());
        if(apiClient.send(apiPayload) != APIClientResult::SUCCESS){
            displayManager.showNotification("Fail!");
        }else {
            displayManager.showNotification("Sent!");
        }

        if(wakeupFunction == 0){
            SleepManager::getInstance().startSleep();
            return;
        }
    }
    // on connected or disconnected:
    displayManager.refresh();
    isWiFiConnecting = false;
    if(!isBLEConnected && !isBtnHold) SleepManager::getInstance().enableSleepTimer();
}

void handleWakeup(WakeupReason wakeupReason){
    if(wakeupReason == WakeupReason::UNDEFINED){
        SleepManager::getInstance().startSleep();
        return;
    }
    if(wakeupReason == WakeupReason::EXT){
        delay(LONG_PRESS_DURATION_MS / 2); // debounce - wait for a half of long press
        if(digitalRead(GPIO_BUTTON_PIN) == HIGH) {
            SleepManager::getInstance().startSleep();
            return;
        }
    }

    WiFiConnectManager::getInstance().begin(); // try connecting

    if(wakeupReason == WakeupReason::TIMER){
        log("Woke up from timer");
        wakeupFunction = 0;
        return;
    }
    if(wakeupReason == WakeupReason::EXT){
        log("Woke up from external interrupt");
        wakeupFunction = 1;

        displayManager.powerOn();
        return;
    }
}

void handleSleep(){
    wakeupFunction = -1;
    displayManager.powerOff();
    BLEManager::getInstance().disable();
    WiFiConnectManager::getInstance().disable();
    Serial.flush();
}
