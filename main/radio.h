#ifndef RADIO_H
#define RADIO_H

#define ECHO_UART_BAUD_RATE 115200
#define UART_PORT_NUM 2
#define UART_2_TXD 17
#define UART_2_RXD 16
#define BUF_LEN 1024
#define RADIO_MSG_MAX_LEN 256
#define RADIO_QUEUE_DEPTH 5

typedef struct {
    char *rx_buffer;
} uart_task_params_t;

extern uart_task_params_t uartTaskParams;

extern QueueHandle_t radio_msg_queue;

int InitRadio();


int SendRadioMsg(char * buf);

//int ReceiveRadioMsg();

int ProcessIncomingMessage();

void uart_radio_rx_task(void *arg);





#endif