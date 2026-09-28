#ifndef UART_H
#define UART_H

#include "ecu_types.h"

void uart_init(void);
void uart_send(const char *message);
void uart_send_float(const char *label, float value);
void uart_send_uint(const char *label, unsigned int value);
void uart_send_faults(FaultMask faults);

#endif
