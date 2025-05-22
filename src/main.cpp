#include "main.h"
#include "config.h"
#include "types.h"
#include "sensors.h"
#include "power.h"
#include "network.h"
#include "display.h"
#include "Wire.h"
#include "Adafruit_SSD1306.h"


static int wakeup_function = -1;
static bool waiting_wifi = false;
static ulong last_press = 0;

void setup() {
    Serial.begin(SERIAL_BAUD);

    // Pins configuration
    Wire.begin(I2C_SDA, I2C_SCL);
    pinMode(GPIO_BUTTON_PIN, INPUT_PULLUP);

    waiting_wifi = true;
    set_display_data(read_power_data(), read_sensors());
    connect_to_wifi(WIFI_SSID, WIFI_PASSWORD);
    display_power_on();

    // Deep sleep configuration
    // constexpr uint64_t WAKEUP_TIMER_US = DEEPSLEEP_TIME_S * 1000000ULL;
    // esp_sleep_enable_ext1_wakeup(1ULL << GPIO_BUTTON_PIN, ESP_EXT1_WAKEUP_ANY_HIGH);
    // esp_sleep_enable_timer_wakeup(WAKEUP_TIMER_US);

    // esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    // handle_wakeup(wakeup_reason);
}

void log(const char *message) {
    Serial.println(message);
}

void loop(){
    wifi_connect_loop();
    display_check_wifi_status_loop();
    if(waiting_wifi && get_wifi_status() == WIFI_CONNECTED){
        waiting_wifi = false;
        handle_wifi_connected();
    }
    // button_press_loop();
    // sleep_timeout_loop();
}

// check if inactive for a while to go to sleep
void sleep_timeout_loop(){
    if(millis() - last_press < INACTIVE_TIME_MS) return;
    handle_sleep();
}

// handle button states
void button_press_loop(){
    static ulong press_start = 0;
    static bool pressed = false;
    if(!pressed && digitalRead(GPIO_BUTTON_PIN) == HIGH){
        pressed = true;
        press_start = millis();
        return;
    }
    if(pressed && digitalRead(GPIO_BUTTON_PIN) == LOW){
        last_press = millis();
        pressed = false;
        ulong press_duration = last_press - press_start;
        if(press_duration < 30) return; // debounce
        if(press_duration >= LONG_PRESS_DURATION_MS){ // long press
            handle_sleep();
            return;
        }
        handle_button_press();
    }
}

// on short button press
void handle_button_press(){
    cycle_display();
}

// when wifi connected send data and go to sleep if needed
void handle_wifi_connected(){
    log("WiFi connected");

    if(send_data(read_sensors(), read_power_data()) != 0){
        log("Failed to send data");
        display_notification("Fail!");
        return;
    }
    log("Data sent successfully");
    display_notification("Sent!");

    // if(wakeup_function == 0){
    //     handle_sleep();
    //     return;
    // }
}

void handle_wakeup(esp_sleep_wakeup_cause_t wakeup_reason){
    connect_to_wifi(WIFI_SSID, WIFI_PASSWORD);
    log("Connecting to WiFi");
    waiting_wifi = true;

    if(wakeup_reason == ESP_SLEEP_WAKEUP_TIMER){
        log("Woke up from timer");
        wakeup_function = 0;
        return;
    }
    if(wakeup_reason == ESP_SLEEP_WAKEUP_EXT1){
        delay(LONG_PRESS_DURATION_MS / 2); // debounce - wait for half long press
        if(digitalRead(GPIO_BUTTON_PIN) == LOW) {
            handle_sleep();
            return;
        }
        log("Woke up from external interrupt");
        wakeup_function = 1;

        sensors_data_t sensors = read_sensors();
        power_data_t power = read_power_data();

        set_display_data(power, sensors);
        display_power_on();
        return;
    }
    if(wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED){
        log("Woke up from undefined reason");
        handle_sleep();
        return;
    }
}

void handle_sleep(){
    log("Going back to sleep");
    wakeup_function = -1;
    display_power_off();
    wifi_off();
    Serial.flush();
    esp_deep_sleep_start();
}
