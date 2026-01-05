#ifndef INC_PROJECT_H_
#define INC_PROJECT_H_

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "driver/uart.h"
#include <ctype.h>
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "mqtt_client.h"

#include "radio.h"
#include "network.h"
#include "wifi.h"
#include "usernvs.h"
#include "util.h"


extern const char dataFeed[];
extern esp_mqtt_client_config_t mqtt_cfg;


// All Task priorities defined here
#define TASK_PRIORITY_UART_RX 5





#endif