#include <stdio.h>
#include "uart.h"

void uart_init(void)
{
    printf("[UART] Communication initialized.\n");
}

void uart_send(const char* message)
{
    printf("[UART] %s\n", message);
}

void uart_send_float(const char* label, float value)
{
    printf("[UART] %s: %.1f\n", label, value);
}

void uart_send_int(const char* label, int value)
{
    printf("[UART] %s: %d\n", label, value);
}
