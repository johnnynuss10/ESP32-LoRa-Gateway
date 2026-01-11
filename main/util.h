#ifndef UTIL_H
#define UTIL_H

#define MIN(x,y)    ((x)<(y))?(x):(y)

int uart_getline(char * dest, size_t max_len);
int uart_console_start();




#endif