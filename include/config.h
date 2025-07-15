#ifndef CONFIG_H
#define CONFIG_H

#define SERIAL_BAUD             115200
#define I2C_SDA                 21
#define I2C_SCL                 22
#define GPIO_BUTTON_PIN         15
#define LONG_PRESS_DURATION_MS  1000
#define DEEPSLEEP_TIME_S        60 * 10 // 10 minutes
#define INACTIVE_TIME_MS        20000

#define SERVICE_UUID            "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID     "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define DEVICE_NAME             "DevBoard"
#define API_URL                 "https://garden.nnikola.xyz/api/report"

#endif // CONFIG_H
