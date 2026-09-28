#ifndef UART_H
#define UART_H

void uart_init(void);

void uart_send(const char* message);

void uart_send_float(const char* label, float value);

void uart_send_int(const char* label, int value);

#endif
