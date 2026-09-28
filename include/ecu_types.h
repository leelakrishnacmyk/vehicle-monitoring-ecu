#ifndef ECU_TYPES_H
#define ECU_TYPES_H

#include <stdint.h>

typedef enum
{
    SYSTEM_NORMAL,
    SYSTEM_WARNING,
    SYSTEM_FAULT
} SystemState;

typedef enum
{
    FAULT_NONE             = 0U,
    FAULT_OVER_TEMPERATURE = 1U << 0,
    FAULT_UNDER_VOLTAGE    = 1U << 1,
    FAULT_OVER_VOLTAGE     = 1U << 2,
    FAULT_HIGH_RPM         = 1U << 3,
    FAULT_SENSOR_TEMP      = 1U << 4,
    FAULT_SENSOR_VOLTAGE   = 1U << 5,
    FAULT_SENSOR_RPM       = 1U << 6
} FaultCode;

typedef uint32_t FaultMask;

#endif
