#include <windows.h>
#include "timer.h"

void timer_init(void)
{
    /* Simulated timer initialization */
}

void delay_ms(unsigned int milliseconds)
{
    Sleep(milliseconds);
}
