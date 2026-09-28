#ifndef ECU_STATE_H
#define ECU_STATE_H

#include "fault_manager.h"

void update_ecu_state(SystemState new_state);
SystemState get_ecu_state(void);
void handle_ecu_state(void);

#endif
