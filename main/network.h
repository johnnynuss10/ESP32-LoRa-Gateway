#ifndef NETWORK_H
#define NETWORK_H

#define AIO_BROKER   "mqtt://io.adafruit.com:1883"
#define BUF_LEN 1024
#define FEED_NAME "johnnyFeed"


void mqtt_start(void);

//void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

void adafruit_io_publish(const char *feed, const char *value);

int AdafruitTX(char * buf);

void network_task(void *arg);



#endif