#include "project.h"

char rxBuf[BUF_LEN];
static int totalrxChars = 0;
int temprxChars = 0;
static int messageProcessed = 0;

static const char *TAG = "RYLR998";

static QueueHandle_t uart_queue;



uart_task_params_t uartTaskParams = {rxBuf};

static const char * radio_vars[] =
{
		"ADDRESS",
		"NETWORKID",
		"MODE",
		"IPR",
		"BAND",
		"PARAMETER",
		"CPIN",
		"CRFOP",
		"UID",
		"VER",
};


//Create a function that will initialize UART to communicate with RYLR998

int InitRadio()
{
    radio_msg_queue = xQueueCreate(RADIO_QUEUE_DEPTH, RADIO_MSG_MAX_LEN);

    //Configure UART2
    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    int intr_alloc_flags = 0;

    uart_driver_install(UART_PORT_NUM, BUF_LEN * 2, 0, 10, &uart_queue, intr_alloc_flags);
    uart_param_config(UART_PORT_NUM, &uart_config);
    uart_set_pin(UART_PORT_NUM, UART_2_TXD, UART_2_RXD, -1, -1);   

    xTaskCreate(uart_radio_rx_task, "uart_radio_rx_task", 4096, &uartTaskParams, TASK_PRIORITY_UART_RX, NULL);

    radio_msg_queue = xQueueCreate(RADIO_QUEUE_DEPTH, RADIO_MSG_MAX_LEN);

    xTaskCreate(network_task, "network_task", 4096, NULL, 5, NULL);

    // Loop through each radio parameter and print each value set on radio
    
    for (int i = 0; i < sizeof(radio_vars)/sizeof(char*); i++)
    {
        char sendString[20];
        sprintf(sendString, "AT+%s?", radio_vars[i]);
        SendRadioMsg(sendString);
    }
    
    return 0;
}


//Create a function that will send buf contents to UART

int SendRadioMsg(char * buf)
{

    const char * lineterm = "\r\n";

    // Need to capitalize all letters in buf
    for (int i = 0; i < strlen(buf); i++)
        buf[i] = toupper(buf[i]);
    
    //transmit buf
    uart_write_bytes(UART_PORT_NUM, (const char *)buf, strlen(buf));
    uart_write_bytes(UART_PORT_NUM, lineterm, strlen(lineterm));

    // Get response from radio
    // Set timeout
    while (1)
    {
        // Check timeout

        // Check for returned message
        if (messageProcessed)
        {
            messageProcessed = 0;
            return 0;
        }

    }

    return 0;
}


/*
int ReceiveRadioMsg() // Replaced by uart_rx_task()
{

    int len = uart_read_bytes(UART_PORT_NUM, rxBuf + totalrxChars, (sizeof(rxBuf) - 1), 10);
    totalrxChars = totalrxChars + len;
    if (rxBuf[totalrxChars - 1] == '\n')
    {
        rxBuf[totalrxChars] = '\0';
        messageProcessFlag = 1;
    }
    //ESP_LOGI(TAG, "Recv str: %s", (char *) rxBuf);

    // At this point rxBuf should contain a complete radio transmission which is ready to be processed
    

    return 0;
}
*/



int ProcessIncomingMessage()
{

    // Print Received string and reset rxBuf
    ESP_LOGI(TAG, "Recv str: %s", (char *) rxBuf);

    totalrxChars = 0;
    memset(rxBuf, 0, sizeof(rxBuf));

    messageProcessed = 1;

    return 0;
}

void uart_radio_rx_task(void *arg)
{

    uart_event_t event;

    while (1)
    {

        if (xQueueReceive(uart_queue, &event, portMAX_DELAY))
        {
            if (event.type == UART_DATA)
            {
                int len = uart_read_bytes(UART_PORT_NUM, rxBuf + totalrxChars, (sizeof(rxBuf) - 1), 10);
                totalrxChars = totalrxChars + len;

                if (rxBuf[totalrxChars - 1] == '\n')
                {
                    rxBuf[totalrxChars] = '\0';

                    // Send rxBuf to networking task
                    if (strchr(rxBuf, '$') != NULL)
                    {
                        xQueueSend(radio_msg_queue, rxBuf, 0);
                    }
                    
                    ProcessIncomingMessage();
                }
            }
        }
    }
    // Give lower priority tasks a chance to run
    vTaskDelay(2);
}


