#include "project.h"

int uart_console_start()
{
    
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    
    uart_param_config(UART_NUM_0, &uart_config);

    uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0);

    return 0;
}


int uart_getline(char * dest, size_t max_len)
{
    int totalrxChars = 0;
    char rxBuf[BUF_LEN];
    memset(rxBuf, 0, sizeof(rxBuf));

    // Read UART in while loop and exit if '\n' is detected

    while(1)
    {
        int len = uart_read_bytes(UART_NUM_0, rxBuf + totalrxChars, (sizeof(rxBuf) - 1), 10);
        //int len = uart_read_bytes(UART_NUM_0, rxBuf, (BUF_LEN), 10);
        if (len < 0)
        {
            ESP_LOGI("TEST", "ERROR: uart_read_bytes() Returned %d", len);
            vTaskDelay(1000);
            continue;
        }
        totalrxChars = totalrxChars + len;

        if (rxBuf[0] == '\b')
        {
            rxBuf[0] = '\0';
            totalrxChars = 0;
        }
        
        if (len > 0 && totalrxChars >= 1)
        {
            putchar(rxBuf[totalrxChars-1]);
            //printf("%c", rxBuf[totalrxChars-1]);
            fflush(stdout);
            //ESP_LOG_BUFFER_HEX("Received", rxBuf, len);
            //ESP_LOGI("", "%c", rxBuf[len - 1]);
            //vTaskDelay(10);
        }

        if (rxBuf[totalrxChars - 1] == '\b' && totalrxChars > 1)
        {
            rxBuf[totalrxChars - 1] = '\0';
            rxBuf[totalrxChars - 2] = '\0';
            printf(" \b");
            fflush(stdout);
            totalrxChars -= 2;
        }
        
        
        if (len > 0 && totalrxChars >= 1 && rxBuf[totalrxChars - 1] == '\r') //Enter sends '\r'
        {
            rxBuf[totalrxChars - 1] = '\0';
            printf("\n");
            fflush(stdout);
            printf("Saved value: %s\n", (char *) rxBuf);
            fflush(stdout);
            strncpy(dest, rxBuf, max_len);
            return 0;
        }
        //ESP_LOGI("TEST", "Loop");
        //vTaskDelay(pdMS_TO_TICKS(1));
    }

    return 0;
}


