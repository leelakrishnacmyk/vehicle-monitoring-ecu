#ifndef WATCHDOG_H
#define WATCHDOG_H

void watchdog_init(unsigned int timeout_ms);
void watchdog_kick(void);
void watchdog_check(void);

#endif
