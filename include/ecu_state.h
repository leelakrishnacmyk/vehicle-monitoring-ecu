#ifndef ECU_STATE_H
#define ECU_STATE_H

#include "ecu_types.h"

void ecu_state_init(void);
void update_ecu_state(SystemState new_state);
SystemState get_ecu_state(void);
uint8_t ecu_safe_state_active(void);
void handle_ecu_state(void);

#endif
