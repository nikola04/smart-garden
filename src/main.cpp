#include "main.h"
#include "config.h"
#include "ble.h"
#include "network.h"
#include "display.h"
#include "storage.h"
#include "wireless.h"
#include "logger.h"
#include <driver/rtc_io.h>

static int wakeupFunction = -1;
static bool waitingWifi = false;
static ulong lastPress = 0;

DisplayManager displayManager;

void setup() {
    Serial.begin(SERIAL_BAUD);

    // Pins configuration
    Wire.begin(I2C_SDA, I2C_SCL);
    pinMode(GPIO_BUTTON_PIN, INPUT_PULLUP);

    // Init sensors
    initSensors();

    // Setup wireless
    initWireless();

    // WiFi connection
    initWifi(getWifiSSID(), getWifiPassword());

    // Initialize display
    displayManager.setData(readPowerData(), readSensors());

    // Initialize BLE
    setupBLE();

    // Deep sleep configuration
    constexpr uint64_t WAKEUP_TIMER_US = DEEPSLEEP_TIME_S * 1000000ULL;
    esp_sleep_enable_ext0_wakeup((gpio_num_t) GPIO_BUTTON_PIN, 0);
    esp_sleep_enable_timer_wakeup(WAKEUP_TIMER_US);

    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    handleWakeup(wakeup_reason);
}

void loop(){
    static ulong lastUpdate = 0;
    if(millis() - lastUpdate > 3000){
        sensors_data_t sensors = readSensors();
        power_data_t power = readPowerData();

        displayManager.setData(power, sensors);
        lastUpdate = millis();
    }
    
    wifiConnectionLoop();
    displayManager.loop();
    if(waitingWifi && wifiGetStatus() == WIFI_CONNECTED){
        handleWifiConnected();
        waitingWifi = false;
    }
    loopBLE();
    sleepTimeoutLoop();
    buttonPressLoop();
}

// check if inactive for a while to go to sleep
void sleepTimeoutLoop(){
    if(millis() - lastPress < INACTIVE_TIME_MS) return;
    if(waitingWifi) return;
    handleSleep();
}

// handle button states
void buttonPressLoop(){
    static bool wakeup_button_state = true; // assume that button is not released after wakeup
    static ulong press_start = 0;
    static bool pressed = false;
    int pinValue = digitalRead(GPIO_BUTTON_PIN);

    if(wakeup_button_state && pinValue == LOW) return; // still not released
    if(wakeup_button_state) wakeup_button_state = false;

    if(!pressed && pinValue == LOW){ // first press
        pressed = true;
        press_start = millis();
        return;
    }
    if(pressed && pinValue == HIGH){ // release
        lastPress = millis();
        pressed = false;
        ulong pressDuration = lastPress - press_start;
        if(pressDuration < 30) return; // debounce
        if(pressDuration >= LONG_PRESS_DURATION_MS){ // long press
            handleButtonLongPress();
            return;
        }
        handleButtonPress();
    }
}

// on short button press
void handleButtonPress(){
    log("press");
    displayManager.cycle();
}

// on short button press
void handleButtonLongPress(){
    log("long press");
    handleSleep();
}

// when wifi connected send data and go to sleep if needed
void handleWifiConnected(){
    log("WiFi connected");

    displayManager.showNotification("Sending..");
    if(sendData(readSensors(), readPowerData()) != 0){
        log("Failed to send data");
        displayManager.showNotification("Fail!");
    }else {
        log("Data sent successfully");
        displayManager.showNotification("Sent!");
    }

    if(wakeupFunction == 0){
        handleSleep();
    }
}

void handleWakeup(esp_sleep_wakeup_cause_t wakeup_reason){
    if(wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED){
        handleSleep();
        return;
    }
    if(wakeup_reason == ESP_SLEEP_WAKEUP_EXT0){
        delay(LONG_PRESS_DURATION_MS / 2); // debounce - wait for a half of long press
        if(digitalRead(GPIO_BUTTON_PIN) == HIGH) {
            handleSleep();
            return;
        }
    }

    wifiConnect();
    log("Connecting to WiFi");
    waitingWifi = true;

    if(wakeup_reason == ESP_SLEEP_WAKEUP_TIMER){
        log("Woke up from timer");
        wakeupFunction = 0;
        return;
    }
    if(wakeup_reason == ESP_SLEEP_WAKEUP_EXT0){
        log("Woke up from external interrupt");
        wakeupFunction = 1;

        displayManager.powerOn();
        return;
    }
}

void handleSleep(){
    log("Going back to sleep");
    stopBLE();
    wakeupFunction = -1;
    displayManager.powerOff();
    wifiDisable();
    Serial.flush();
    esp_deep_sleep_start();
}
