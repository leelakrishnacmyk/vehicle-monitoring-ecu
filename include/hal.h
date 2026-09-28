#ifndef HAL_H
#define HAL_H

#include <stdint.h>

uint32_t hal_tick_ms(void);
void hal_delay_ms(uint32_t milliseconds);

#endif
