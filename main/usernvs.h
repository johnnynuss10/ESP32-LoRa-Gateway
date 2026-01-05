#ifndef USERNVS_H
#define USERNVS_H

#define NVS_NAMESPACE     "config"
#define KEY_WIFI_SSID    "wifi_ssid"
#define KEY_WIFI_PASS    "wifi_pass"
#define KEY_AIO_USER     "aio_user"
#define KEY_AIO_KEY      "aio_key"

int InitNVS();
bool load_credentials(char *ssid, char *pass, char *aio_user, char *aio_key);
void provision_via_uart(char *ssid, char *pass, char *aio_user, char *aio_key);
void save_credentials(const char *ssid, const char *pass, const char *aio_user, const char *aio_key);
void InitCredentials(wifi_config_t * wificfg, esp_mqtt_client_config_t * mqttcfg);





#endif