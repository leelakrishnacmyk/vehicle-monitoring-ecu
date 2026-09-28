#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stdint.h>

void watchdog_init(uint32_t timeout_ms);
void watchdog_kick(void);
void watchdog_check(void);
void watchdog_check_at(uint32_t now_ms);
uint8_t watchdog_reset_required(void);
void watchdog_clear_reset_request(void);

#endif
