#ifndef SENSOR_SIM_H
#define SENSOR_SIM_H

#include <stdint.h>

void sensor_sim_set_scenario(uint32_t scenario);
float sensor_sim_temperature_c(void);
float sensor_sim_battery_voltage_v(void);
uint32_t sensor_sim_engine_rpm(void);

#endif
