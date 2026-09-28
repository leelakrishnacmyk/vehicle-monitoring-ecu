#include "hal.h"
#include "uart.h"
#include "watchdog.h"

static uint32_t watchdog_timeout = 1000U;
static uint32_t last_kick_time = 0U;
static uint8_t reset_required = 0U;

void watchdog_init(uint32_t timeout_ms)
{
    watchdog_timeout = timeout_ms;
    last_kick_time = hal_tick_ms();
    reset_required = 0U;

    uart_send("Watchdog initialized.");
}

void watchdog_kick(void)
{
    last_kick_time = hal_tick_ms();
}

void watchdog_check_at(uint32_t now_ms)
{
    if ((uint32_t)(now_ms - last_kick_time) > watchdog_timeout)
    {
        reset_required = 1U;
        uart_send("WATCHDOG TIMEOUT: reset required.");
        last_kick_time = now_ms;
    }
}

void watchdog_check(void)
{
    watchdog_check_at(hal_tick_ms());
}

uint8_t watchdog_reset_required(void)
{
    return reset_required;
}

void watchdog_clear_reset_request(void)
{
    reset_required = 0U;
}
