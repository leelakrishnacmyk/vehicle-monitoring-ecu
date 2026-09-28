#ifndef FAULT_MANAGER_H
#define FAULT_MANAGER_H

typedef enum
{
    SYSTEM_NORMAL,
    SYSTEM_WARNING,
    SYSTEM_FAULT
} SystemState;

/*
 * Each fault gets its own bit.
 * This allows multiple faults to exist at the same time.
 */
typedef enum
{
    FAULT_NONE             = 0,
    FAULT_OVER_TEMPERATURE = 1 << 0,
    FAULT_UNDER_VOLTAGE    = 1 << 1,
    FAULT_OVER_VOLTAGE     = 1 << 2,
    FAULT_HIGH_RPM         = 1 << 3
} FaultCode;

int detect_faults(float temperature, float voltage, int rpm);

SystemState check_system(float temperature, float voltage, int rpm);

const char* get_state_name(SystemState state);

void print_faults(int faults);

#endif
