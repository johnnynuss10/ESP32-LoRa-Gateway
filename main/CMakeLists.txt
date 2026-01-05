idf_component_register(SRCS "util.c" "usernvs.c" "wifi.c" "radio.c" "network.c" "main.c"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES esp_driver_uart
                       PRIV_REQUIRES esp_driver_i2c
                       PRIV_REQUIRES esp_wifi
                       REQUIRES esp_wifi esp_event mqtt
                       PRIV_REQUIRES nvs_flash)
