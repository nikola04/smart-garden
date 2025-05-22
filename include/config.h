#ifndef CONFIG_H
#define CONFIG_H

#define SERIAL_BAUD             115200
#define I2C_SDA                 21
#define I2C_SCL                 22
#define GPIO_BUTTON_PIN         15
#define LONG_PRESS_DURATION_MS  1000
#define DEEPSLEEP_TIME_S        60 * 10 // 10 minutes
#define INACTIVE_TIME_MS        20000
#define WIFI_SSID               "HAARP ANTENA"
#define WIFI_PASSWORD           "83599455"
#define API_URL                 "http://79.72.65.226/api/report"

#endif // CONFIG_H
