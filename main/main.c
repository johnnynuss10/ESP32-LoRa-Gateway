/*
 * ESP32_LORA_GATEWAY V1
 *
 * Author: Johnny Nuss
 */
/* 

   ESP32 LoRa Gateway for use with STM32 Remote Sensor Nodes
*/

#include "project.h"

//static const char *TAG = "example";

void app_main(void)
{

    InitNVS();
    InitWifi();

    //UART 
    InitRadio();

    // MQTT
    //mqtt_start();


}
