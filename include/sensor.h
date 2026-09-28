#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include "ecu_types.h"

typedef struct
{
    float temperature_c;
    float battery_voltage_v;
    uint32_t engine_rpm;
    uint16_t temperature_adc;
    uint16_t battery_voltage_adc;
    uint8_t temperature_valid;
    uint8_t battery_voltage_valid;
    uint8_t rpm_valid;
} SensorData;

void sensor_set_scenario(uint32_t scenario);
void sensor_read_all(SensorData *data);

#endif
