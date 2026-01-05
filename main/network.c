#include "project.h"

const char dataFeed[] = FEED_NAME;

char adafruitDataBuf[BUF_LEN];

QueueHandle_t radio_msg_queue = NULL;

static esp_mqtt_client_handle_t mqtt_client;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

esp_mqtt_client_config_t mqtt_cfg = 
{
    .broker.address.uri = AIO_BROKER,
    .credentials.username = "",
    .credentials.authentication.password = "",
};

void mqtt_start(void)
{
    
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt_client);
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{

    esp_mqtt_event_handle_t event = event_data;

    if (event->event_id == MQTT_EVENT_CONNECTED) 
    {
        ESP_LOGI("MQTT", "Connected to Adafruit IO");
    }

}

void adafruit_io_publish(const char *feed, const char *value)
{
    char topic[64];
    snprintf(topic, sizeof(topic), "%s/feeds/%s", (mqtt_cfg.credentials.username), feed);

    esp_mqtt_client_publish(mqtt_client, topic, value, 0, 1, 0);
}

int AdafruitTX(char * buf)
{

    static const char *TAG = "Adafruit IO";

    char stringTemp[64];
    char stringNode[64];
    char adafruitSendValue[256];

    // Extract device ID from rxBuf
    char * ptr = strchr(buf, '=') + 1;
    int count = 0;

    while (isdigit((int) *(ptr)))
    {
        stringNode[count++] = (*(ptr++));
    }
    stringNode[count] = '\0';

    // Extract Temp from rxBuf
    ptr = strchr(ptr, ':') + 1;
    count = 0;

    while (isdigit((int) *(ptr)) || (*ptr == '.'))
    {
        stringTemp[count++] = (*(ptr++));
    }
    stringTemp[count] = '\0';

    // Format adafruitSendValue 
    snprintf(adafruitSendValue, sizeof(adafruitSendValue), "{\"value\": \"%s,%s\"}", stringTemp, stringNode);
    ESP_LOGI(TAG, "Sending to Adafruit IO: %s", (char *) adafruitSendValue);
    adafruit_io_publish(dataFeed, adafruitSendValue);

    return 0;
}

void network_task(void *arg)
{
    char msg[RADIO_MSG_MAX_LEN];

    while (1)
    {
        if (xQueueReceive(radio_msg_queue, msg, portMAX_DELAY))
        {
            ESP_LOGI("NET", "Processing radio msg: %s", msg);

            AdafruitTX(msg);
        }
    }
}