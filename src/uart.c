#include <stdio.h>
#include "uart.h"

void uart_init(void)
{
    printf("[UART] Communication initialized.\n");
}

void uart_send(const char *message)
{
    printf("[UART] %s\n", message);
}

void uart_send_float(const char *label, float value)
{
    printf("[UART] %s: %.1f\n", label, value);
}

void uart_send_uint(const char *label, unsigned int value)
{
    printf("[UART] %s: %u\n", label, value);
}

void uart_send_faults(FaultMask faults)
{
    if (faults == FAULT_NONE)
    {
        printf("[UART] Faults: No Fault\n");
        return;
    }

    printf("[UART] Faults:\n");

    if ((faults & FAULT_OVER_TEMPERATURE) != 0U)
    {
        printf("  - Engine Over Temperature\n");
    }
    if ((faults & FAULT_UNDER_VOLTAGE) != 0U)
    {
        printf("  - Battery Under Voltage\n");
    }
    if ((faults & FAULT_OVER_VOLTAGE) != 0U)
    {
        printf("  - Battery Over Voltage\n");
    }
    if ((faults & FAULT_HIGH_RPM) != 0U)
    {
        printf("  - Engine Over Speed\n");
    }
    if ((faults & FAULT_SENSOR_TEMP) != 0U)
    {
        printf("  - Temperature Sensor Invalid\n");
    }
    if ((faults & FAULT_SENSOR_VOLTAGE) != 0U)
    {
        printf("  - Battery Voltage Sensor Invalid\n");
    }
    if ((faults & FAULT_SENSOR_RPM) != 0U)
    {
        printf("  - RPM Sensor Invalid\n");
    }
}
