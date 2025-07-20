#include "main.h"
#include "config.h"
#include "ble.h"
#include "network.h"
#include "display.h"
#include "storage.h"
#include "wireless.h"
#include "logger.h"

static int wakeupFunction = -1;
static bool waitingWifi = false;
static ulong lastPress = 0;

DisplayManager displayManager;

void setup() {
    Serial.begin(SERIAL_BAUD);

    // Pins configuration
    Wire.begin(I2C_SDA, I2C_SCL);
    pinMode(GPIO_BUTTON_PIN, INPUT_PULLUP);

    // Setup wireless
    initWireless();

    // WiFi connection
    waitingWifi = true;
    wifiConnect(getWifiSSID(), getWifiPassword());

    // Initialize display
    displayManager.setData(readPowerData(), readSensors());
    displayManager.powerOn();

    // Initialize BLE
    setupBLE();

    // Deep sleep configuration
    // constexpr uint64_t WAKEUP_TIMER_US = DEEPSLEEP_TIME_S * 1000000ULL;
    // esp_sleep_enable_ext1_wakeup(1ULL << GPIO_BUTTON_PIN, ESP_EXT1_WAKEUP_ANY_HIGH);
    // esp_sleep_enable_timer_wakeup(WAKEUP_TIMER_US);

    // esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    // handle_wakeup(wakeup_reason);
}

void loop(){
    wifiConnectionLoop();
    displayManager.checkWiFiStatus();
    if(waitingWifi && wifiGetStatus() == WIFI_CONNECTED){
        waitingWifi = false;
        handleWifiConnected();
    }
    buttonPressLoop();
    loopBLE();
    // sleepTimeoutLoop();
}

// check if inactive for a while to go to sleep
void sleepTimeoutLoop(){
    if(millis() - lastPress < INACTIVE_TIME_MS) return;
    handleSleep();
}

// handle button states
void buttonPressLoop(){
    static ulong press_start = 0;
    static bool pressed = false;
    if(!pressed && digitalRead(GPIO_BUTTON_PIN) == HIGH){
        pressed = true;
        press_start = millis();
        return;
    }
    if(pressed && digitalRead(GPIO_BUTTON_PIN) == LOW){
        lastPress = millis();
        pressed = false;
        ulong pressDuration = lastPress - press_start;
        if(pressDuration < 30) return; // debounce
        if(pressDuration >= LONG_PRESS_DURATION_MS){ // long press
            handleSleep();
            return;
        }
        handleButtonPress();
    }
}

// on short button press
void handleButtonPress(){
    displayManager.cycle();
}

// when wifi connected send data and go to sleep if needed
void handleWifiConnected(){
    log("WiFi connected");

    if(sendData(readSensors(), readPowerData()) != 0){
        log("Failed to send data");
        displayManager.showNotification("Fail!");
        return;
    }
    log("Data sent successfully");
    displayManager.showNotification("Sent!");

    if(wakeupFunction == 0){
        // handle_sleep();
        return;
    }
}

void handleWakeup(esp_sleep_wakeup_cause_t wakeup_reason){
    const char *wifiSsid = getWifiSSID().c_str();
    const char *wifiPswd = getWifiPassword().c_str();
    wifiConnect(wifiSsid, wifiPswd);
    log("Connecting to WiFi");
    waitingWifi = true;

    if(wakeup_reason == ESP_SLEEP_WAKEUP_TIMER){
        log("Woke up from timer");
        wakeupFunction = 0;
        return;
    }
    if(wakeup_reason == ESP_SLEEP_WAKEUP_EXT1){
        delay(LONG_PRESS_DURATION_MS / 2); // debounce - wait for half long press
        if(digitalRead(GPIO_BUTTON_PIN) == LOW) {
            handleSleep();
            return;
        }
        log("Woke up from external interrupt");
        wakeupFunction = 1;

        sensors_data_t sensors = readSensors();
        power_data_t power = readPowerData();

        displayManager.setData(power, sensors);
        displayManager.powerOn();
        return;
    }
    if(wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED){
        log("Woke up from undefined reason");
        handleSleep();
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
