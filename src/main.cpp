#include "main.h"
#include "config.h"
#include "ble.h"
#include "display.h"
#include "storage.h"
#include "wireless.h"
#include "logger.h"
#include "button.h"
#include "json.h"
#include "debug.h"

ButtonManager wakeupButtonManager(GPIO_BUTTON_PIN, LONG_PRESS_DURATION_MS, HOLD_DELAY_MS, true);
DisplayManager displayManager;
APIClient apiClient;
UARTDebug uartDebugger;

void setup() {
    // Initialize Logger & Debug
    uartDebugger.begin();
    Logger::getInstance().init(&uartDebugger);

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
    SleepManager::getInstance().enableSleepTimer();

    handleWakeup();
}

void loop(){
    updateDataLoop();
    WiFiConnectManager::getInstance().loop();
    displayManager.loop();
    BLEManager::getInstance().loop();
    SleepManager::getInstance().loop();
    wakeupButtonManager.loop();
    uartDebugger.loop();
}

void enterInteractiveMode(){
    Logger::getInstance().debug("Main", "entering interactive mode..");
    uartDebugger.enable();
    displayManager.powerOn();
}

void updateDataLoop(){
    static ulong lastUpdate = 0;
    if(millis() - lastUpdate <= 3000) return;

    WiFiStatus wifiStatus = WiFiConnectManager::getInstance().getStatus();
    SensorsData sensorsData = SensorsManager::readSensors();
    power_data_t powerData = readPowerData();

    displayManager.setData(powerData, sensorsData);

    BLEManager::getInstance().handleDataUpdate(wifiStatus, sensorsData, powerData);

    lastUpdate = millis();
}

// on short button press
void handleButtonPress(){
    SleepManager::getInstance().resetSleepTimer();
    displayManager.cycle();
}

// on short button press
void handleButtonLongPress(){
    if(SleepManager::getInstance().getWakeupReason() == WakeupReason::TIMER) // if woken up on timer and then long press turn on peripherals instead
        enterInteractiveMode();
    else 
        SleepManager::getInstance().startSleep();
}

bool isBLEConnected = false;
bool isWiFiConnecting = false;
bool isBtnHold = false;
void enableSleepIfIdle() {
    if (!isBLEConnected && !isWiFiConnecting && !isBtnHold) {
        SleepManager::getInstance().enableSleepTimer();
    }
}

void handleButtonHold(){
    isBtnHold = true;
    SleepManager::getInstance().disableSleepTimer();
    BLEManager::getInstance().start();
    displayManager.showNotification("BLE Advertising..");
}

void handleButtonRelease(){
    isBtnHold = false;
    enableSleepIfIdle();
    BLEManager::getInstance().stop();
    displayManager.screenMode();
}

void handleBLEConnect(){
    isBLEConnected = true;
    SleepManager::getInstance().disableSleepTimer();
    displayManager.showNotification("BLE Connected"); // will automatically refresh display for BLE status icon
}
void handleBLEDisconnect(){
    isBLEConnected = false;
    enableSleepIfIdle();
    displayManager.showNotification("BLE Disconnected"); // will automatically refresh display for BLE status icon
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
            displayManager.showNotification("Send failed");
        }else {
            displayManager.showNotification("Data sent");
        }

        if(SleepManager::getInstance().getWakeupReason() == WakeupReason::TIMER && !displayManager.isOn()){ // if woken up by timer
            SleepManager::getInstance().startSleep();
            return;
        }
    }
    // on connected or disconnected:
    displayManager.refresh();
    isWiFiConnecting = false;
    enableSleepIfIdle();
}

void handleWakeup(){
    WakeupReason wakeupReason = SleepManager::getInstance().getWakeupReason();
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
        Logger::getInstance().debug("Main", "woke up from timer.");
        return;
    }
    if(wakeupReason == WakeupReason::EXT){
        Logger::getInstance().debug("Main", "woke up from external interrupt.");
        enterInteractiveMode();
        return;
    }
}

void handleSleep(){
    displayManager.powerOff();
    BLEManager::getInstance().disable();
    WiFiConnectManager::getInstance().disable();
    uartDebugger.disable();
    Serial.flush();
}
