#include <stdio.h>
#include <windows.h>
#include "watchdog.h"

static unsigned int watchdog_timeout = 1000;
static unsigned long last_kick_time = 0;

void watchdog_init(unsigned int timeout_ms)
{
    watchdog_timeout = timeout_ms;
    last_kick_time = GetTickCount();

    printf("[WDT] Watchdog initialized: %u ms\n",
           watchdog_timeout);
}

void watchdog_kick(void)
{
    last_kick_time = GetTickCount();
}

void watchdog_check(void)
{
    unsigned long current_time = GetTickCount();

    if ((current_time - last_kick_time) > watchdog_timeout)
    {
        printf("[WDT] WATCHDOG TIMEOUT!\n");
        printf("[WDT] ECU RESET REQUIRED.\n");

        last_kick_time = current_time;
    }
}
