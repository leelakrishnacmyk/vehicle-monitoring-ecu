#include "system.h"
#include "uart.h"

static uint8_t reset_requested = 0U;

void system_reset(void)
{
    reset_requested = 1U;
    uart_send("SYSTEM RESET REQUESTED.");
}

uint8_t system_reset_requested(void)
{
    return reset_requested;
}

void system_clear_reset_request(void)
{
    reset_requested = 0U;
}
