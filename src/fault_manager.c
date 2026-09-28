#include <stdio.h>
#include "fault_manager.h"

int detect_faults(float temperature, float voltage, int rpm)
{
    int faults = FAULT_NONE;

    if (temperature > 130.0f)
    {
        faults |= FAULT_OVER_TEMPERATURE;
    }

    if (voltage < 11.0f)
    {
        faults |= FAULT_UNDER_VOLTAGE;
    }

    if (voltage > 15.0f)
    {
        faults |= FAULT_OVER_VOLTAGE;
    }

    if (rpm > 6000)
    {
        faults |= FAULT_HIGH_RPM;
    }

    return faults;
}

SystemState check_system(float temperature, float voltage, int rpm)
{
    int faults = detect_faults(temperature, voltage, rpm);

    if (faults != FAULT_NONE)
    {
        return SYSTEM_FAULT;
    }

    if (temperature > 110.0f || rpm > 5000)
    {
        return SYSTEM_WARNING;
    }

    return SYSTEM_NORMAL;
}

const char* get_state_name(SystemState state)
{
    switch (state)
    {
        case SYSTEM_NORMAL:
            return "NORMAL";

        case SYSTEM_WARNING:
            return "WARNING";

        case SYSTEM_FAULT:
            return "FAULT";

        default:
            return "UNKNOWN";
    }
}

void print_faults(int faults)
{
    if (faults == FAULT_NONE)
    {
        printf("Fault       : No Fault\n");
        return;
    }

    printf("Faults:\n");

    if (faults & FAULT_OVER_TEMPERATURE)
    {
        printf("  - Engine Over Temperature\n");
    }

    if (faults & FAULT_UNDER_VOLTAGE)
    {
        printf("  - Battery Under Voltage\n");
    }

    if (faults & FAULT_OVER_VOLTAGE)
    {
        printf("  - Battery Over Voltage\n");
    }

    if (faults & FAULT_HIGH_RPM)
    {
        printf("  - Engine Over Speed\n");
    }
}
