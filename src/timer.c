#include "hal.h"
#include "timer.h"

void timer_init(void)
{
    /* Timer services are provided by the HAL. */
}

void delay_ms(uint32_t milliseconds)
{
    hal_delay_ms(milliseconds);
}
