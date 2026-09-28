#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

void system_reset(void);
uint8_t system_reset_requested(void);
void system_clear_reset_request(void);

#endif
