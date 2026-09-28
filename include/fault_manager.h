#ifndef FAULT_MANAGER_H
#define FAULT_MANAGER_H

#include <stdint.h>
#include "ecu_types.h"

void fault_manager_init(void);

FaultMask detect_faults(float temperature_c,
                        float battery_voltage_v,
                        uint32_t engine_rpm,
                        FaultMask sensor_faults);

FaultMask fault_manager_update(float temperature_c,
                               float battery_voltage_v,
                               uint32_t engine_rpm,
                               FaultMask sensor_faults);

SystemState check_system(float temperature_c,
                         float battery_voltage_v,
                         uint32_t engine_rpm,
                         FaultMask active_faults);

FaultMask get_active_faults(void);
const char *get_state_name(SystemState state);

#endif
