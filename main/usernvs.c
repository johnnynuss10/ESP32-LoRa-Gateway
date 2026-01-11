#include "project.h"

static char aio_user[32];
static char aio_key[64];
char ssid[32], pass[64];

int InitNVS()
{

    esp_err_t ret;

    // 1. NVS (REQUIRED)
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    return 0;
}

bool load_credentials(char *ssid, char *pass, char *aio_user, char *aio_key)
{
    nvs_handle_t nvs;
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &nvs);
    if (err != ESP_OK) return false;

    size_t len;

    len = 32;
    if (nvs_get_str(nvs, KEY_WIFI_SSID, ssid, &len) != ESP_OK) goto fail;

    len = 64;
    if (nvs_get_str(nvs, KEY_WIFI_PASS, pass, &len) != ESP_OK) goto fail;

    len = 32;
    if (nvs_get_str(nvs, KEY_AIO_USER, aio_user, &len) != ESP_OK) goto fail;

    len = 64;
    if (nvs_get_str(nvs, KEY_AIO_KEY, aio_key, &len) != ESP_OK) goto fail;

    nvs_close(nvs);
    return true;

fail:
    nvs_close(nvs);
    return false;
}

void save_credentials(const char *ssid, const char *pass, const char *aio_user, const char *aio_key)
{
    nvs_handle_t nvs;
    ESP_ERROR_CHECK(nvs_open(NVS_NAMESPACE, NVS_READWRITE, &nvs));

    ESP_ERROR_CHECK(nvs_set_str(nvs, KEY_WIFI_SSID, ssid));
    ESP_ERROR_CHECK(nvs_set_str(nvs, KEY_WIFI_PASS, pass));
    ESP_ERROR_CHECK(nvs_set_str(nvs, KEY_AIO_USER, aio_user));
    ESP_ERROR_CHECK(nvs_set_str(nvs, KEY_AIO_KEY, aio_key));

    ESP_ERROR_CHECK(nvs_commit(nvs));
    nvs_close(nvs);

    ESP_LOGI("NVS", "Credentials saved");
}

void provision_via_uart(char *ssid, char *pass, char *aio_user, char *aio_key)
{
    char tBuf[32];
    printf("\n=== ESP32 Provisioning ===\n");
    while(1)
    {
        printf("\nEnter WiFi SSID: ");
        fflush(stdout);
        //ESP_LOGI("TAG", "Enter WiFi SSID: ");
        uart_getline(ssid, 32);

        printf("\nEnter WiFi Password: ");
        fflush(stdout);
        //printf("Enter WiFi Password: ");
        uart_getline(pass, 64);

        printf("\nEnter Adafruit IO Username: ");
        fflush(stdout);
        //printf("Enter Adafruit IO Username: ");
        uart_getline(aio_user, 32);

        printf("\nEnter Adafruit IO Key: ");
        fflush(stdout);
        //printf("Enter Adafruit IO Key: ");
        uart_getline(aio_key, 64);
        printf("\nYou entered: \n");
        printf("SSID: %s\n", ssid);
        printf("Wifi Password: %s\n", pass);
        printf("Adafruit IO username: %s\n", aio_user);
        printf("Adafruit IO Key: %s\n", aio_key);
        printf("Is that correct? (Y/N) ");
        fflush(stdout);
        uart_getline(tBuf, sizeof(tBuf));
        if (tolower(tBuf[0]) == 'y')
        {
            break;
        }
        
    }

    save_credentials(ssid, pass, aio_user, aio_key);
}

void InitCredentials(wifi_config_t * wificfg, esp_mqtt_client_config_t * mqttcfg)
{
    

    if (!load_credentials(ssid, pass, aio_user, aio_key)) {
        ESP_LOGW("BOOT", "No credentials found, entering provisioning");
        provision_via_uart(ssid, pass, aio_user, aio_key);
    }
    //ESP_LOGW("BOOT", "No credentials found, entering provisioning");
    //provision_via_uart(ssid, pass, aio_user, aio_key);


    // Use loaded credentials
    strncpy((char *)wificfg->sta.ssid, ssid, 32);
    strncpy((char *)wificfg->sta.password, pass, 64);
    mqttcfg->credentials.username = aio_user;
    mqttcfg->credentials.authentication.password = aio_key;
    
}
