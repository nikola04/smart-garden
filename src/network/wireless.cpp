#include "wireless.h"
#include "esp_wifi.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "network.h"
#include "main.h"

void applyEcoMode(){
    esp_wifi_set_max_tx_power(11);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_N0);
}

void applyBalancedMode(){
    esp_wifi_set_max_tx_power(17);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P3);
}

void applyPowerMode(){
    esp_wifi_set_max_tx_power(20);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P9);
}

void initWireless(void){
    String powerMode = getPowerMode();
    if(powerMode == "eco") applyEcoMode();
    else if(powerMode == "power") applyPowerMode();
    else applyBalancedMode();
}
