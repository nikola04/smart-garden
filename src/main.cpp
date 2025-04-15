#include "Arduino.h"
#include "esp_sleep.h"
#include "config.h"
#include "types.h"
#include "sensors.h"
#include "power.h"
#include "network.h"

void handle_wakeup(esp_sleep_wakeup_cause_t wakeup_reason);

void setup() {
    Serial.begin(SERIAL_BAUD);

    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    handle_wakeup(wakeup_reason);

    esp_sleep_enable_ext1_wakeup(1ULL << GPIO_WAKEUP_PIN, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_sleep_enable_timer_wakeup(DEEPSLEEP_TIME * 1000000ULL);

    esp_deep_sleep_start();
}

void loop(){}

void log(const char* message){
    Serial.println(message);
}

void handle_wakeup(esp_sleep_wakeup_cause_t wakeup_reason){
    if(wakeup_reason == ESP_SLEEP_WAKEUP_TIMER){
        int connected = connect_to_wifi(WIFI_SSID, WIFI_PASSWORD);
        if(connected != 0){
            log("Failed to connect to WiFi");
            return;
        }
        log("Connected to WiFi");

        int sent = send_data(read_sensors(), read_power_data());
        if(sent != 0){
            log("Failed to send data");
            return;
        }
        log("Data sent successfully");

        disconnect_from_wifi();
    }else if(wakeup_reason == ESP_SLEEP_WAKEUP_EXT1){
    }
}
