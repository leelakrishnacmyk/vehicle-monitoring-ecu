#include "hal.h"

#ifdef _WIN32
#include <windows.h>

uint32_t hal_tick_ms(void)
{
    return (uint32_t)GetTickCount();
}

void hal_delay_ms(uint32_t milliseconds)
{
    Sleep(milliseconds);
}
#else
#include <time.h>

uint32_t hal_tick_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (uint32_t)((uint64_t)ts.tv_sec * 1000ULL +
                      (uint64_t)ts.tv_nsec / 1000000ULL);
}

void hal_delay_ms(uint32_t milliseconds)
{
    struct timespec request;
    request.tv_sec = (time_t)(milliseconds / 1000U);
    request.tv_nsec = (long)((milliseconds % 1000U) * 1000000UL);
    nanosleep(&request, 0);
}
#endif
