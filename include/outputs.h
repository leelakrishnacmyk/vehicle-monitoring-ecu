#ifndef OUTPUTS_H
#define OUTPUTS_H

#include <stdint.h>

void outputs_init(void);
void outputs_enable(void);
void outputs_disable(void);
uint8_t outputs_are_enabled(void);

#endif
