#ifndef CONFIG_H
#define CONFIG_H

#define SERIAL_BAUD             115200
#define I2C_SDA                 21
#define I2C_SCL                 22
#define GPIO_BUTTON_PIN         15
#define LONG_PRESS_DURATION_MS  1000
#define HOLD_DELAY_MS           3000
#define DEEPSLEEP_TIME_S        60 * 10 // 10 minutes
#define INACTIVE_TIME_MS        15000
#define ALTITUDE_M              145

#define SERVICE_UUID                    "5C885D90-2726-4368-A12D-342D26C1C99A"
#define SYSTEM_CHARACTERISTIC_UUID      "1EA6DA5B-3DC4-4495-8E8F-05A7B727679D"
#define DEVICE_CHARACTERISTIC_UUID      "57C1288C-B1A8-44BC-AABD-1D94F987BE97"
#define SENSORS_CHARACTERISTIC_UUID     "78B32B3B-DA18-43BB-B56D-4773BA5DDBD1"
#define WIFI_CHARACTERISTIC_UUID        "45DC0F38-9C98-4B8A-A49A-5C8542E63E44"

#define DEFAULT_DEVICE_NAME             "DevBoard"
#define DEFAULT_POWER_MODE              "balanced" // eco, balanced, power 
#define DEFAULT_API_URL                 "https://garden.nnikola.xyz/api/report"
#define DEFAULT_API_KEY                 "49e6c23080ef188e45edbaaa1f1bd21c7e8c79bfe88455e40636266d98182807"
#define DEFAULT_WIFI_SSID               ""
#define DEFAULT_WIFI_PASSWORD           ""

#endif // CONFIG_H
