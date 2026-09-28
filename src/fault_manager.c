#include "config.h"
#include "fault_manager.h"

static FaultMask active_faults = FAULT_NONE;
static uint8_t pending_set_count[32];
static uint8_t pending_clear_count[32];
static uint8_t warning_active = 0U;

static int fault_clear_condition(FaultCode fault,
                                 float temperature_c,
                                 float battery_voltage_v,
                                 uint32_t engine_rpm,
                                 FaultMask sensor_faults)
{
    switch (fault)
    {
        case FAULT_OVER_TEMPERATURE:
            return temperature_c <= TEMP_CLEAR_C;
        case FAULT_UNDER_VOLTAGE:
            return battery_voltage_v >= VOLTAGE_UNDER_CLEAR;
        case FAULT_OVER_VOLTAGE:
            return battery_voltage_v <= VOLTAGE_OVER_CLEAR;
        case FAULT_HIGH_RPM:
            return engine_rpm <= RPM_CLEAR;
        case FAULT_SENSOR_TEMP:
            return (sensor_faults & FAULT_SENSOR_TEMP) == 0U;
        case FAULT_SENSOR_VOLTAGE:
            return (sensor_faults & FAULT_SENSOR_VOLTAGE) == 0U;
        case FAULT_SENSOR_RPM:
            return (sensor_faults & FAULT_SENSOR_RPM) == 0U;
        default:
            return 1;
    }
}

static int warning_clear_condition(float temperature_c,
                                   float battery_voltage_v,
                                   uint32_t engine_rpm)
{
    return temperature_c < TEMP_WARN_CLEAR_C &&
           engine_rpm < RPM_WARN_CLEAR &&
           battery_voltage_v > VOLTAGE_WARN_LOW_CLEAR &&
           battery_voltage_v < VOLTAGE_WARN_HIGH_CLEAR;
}

static int warning_condition(float temperature_c,
                             float battery_voltage_v,
                             uint32_t engine_rpm)
{
    return temperature_c >= TEMP_WARN_C ||
           engine_rpm >= RPM_WARN ||
           battery_voltage_v <= VOLTAGE_WARN_LOW ||
           battery_voltage_v >= VOLTAGE_WARN_HIGH;
}

void fault_manager_init(void)
{
    uint32_t i;

    active_faults = FAULT_NONE;
    warning_active = 0U;

    for (i = 0U; i < 32U; ++i)
    {
        pending_set_count[i] = 0U;
        pending_clear_count[i] = 0U;
    }
}

FaultMask detect_faults(float temperature_c,
                        float battery_voltage_v,
                        uint32_t engine_rpm,
                        FaultMask sensor_faults)
{
    FaultMask faults = sensor_faults;

    if ((sensor_faults & FAULT_SENSOR_TEMP) == 0U &&
        temperature_c >= TEMP_FAULT_C)
    {
        faults |= FAULT_OVER_TEMPERATURE;
    }

    if ((sensor_faults & FAULT_SENSOR_VOLTAGE) == 0U &&
        battery_voltage_v <= VOLTAGE_UNDER_FAULT)
    {
        faults |= FAULT_UNDER_VOLTAGE;
    }

    if ((sensor_faults & FAULT_SENSOR_VOLTAGE) == 0U &&
        battery_voltage_v >= VOLTAGE_OVER_FAULT)
    {
        faults |= FAULT_OVER_VOLTAGE;
    }

    if ((sensor_faults & FAULT_SENSOR_RPM) == 0U &&
        engine_rpm >= RPM_FAULT)
    {
        faults |= FAULT_HIGH_RPM;
    }

    return faults;
}

FaultMask fault_manager_update(float temperature_c,
                               float battery_voltage_v,
                               uint32_t engine_rpm,
                               FaultMask sensor_faults)
{
    FaultMask raw_faults = detect_faults(temperature_c,
                                         battery_voltage_v,
                                         engine_rpm,
                                         sensor_faults);
    uint32_t bit;

    for (bit = 0U; bit < 32U; ++bit)
    {
        FaultMask mask = (FaultMask)1U << bit;

        if ((raw_faults & mask) != 0U)
        {
            pending_clear_count[bit] = 0U;

            if ((active_faults & mask) == 0U)
            {
                if (pending_set_count[bit] < FAULT_SET_DEBOUNCE_SAMPLES)
                {
                    ++pending_set_count[bit];
                }

                if (pending_set_count[bit] >= FAULT_SET_DEBOUNCE_SAMPLES)
                {
                    active_faults |= mask;
                }
            }
        }
        else
        {
            pending_set_count[bit] = 0U;

            if ((active_faults & mask) != 0U)
            {
                if (fault_clear_condition((FaultCode)mask,
                                          temperature_c,
                                          battery_voltage_v,
                                          engine_rpm,
                                          sensor_faults) != 0)
                {
                    if (pending_clear_count[bit] < FAULT_CLEAR_DEBOUNCE_SAMPLES)
                    {
                        ++pending_clear_count[bit];
                    }

                    if (pending_clear_count[bit] >= FAULT_CLEAR_DEBOUNCE_SAMPLES)
                    {
                        active_faults &= ~mask;
                        pending_clear_count[bit] = 0U;
                    }
                }
                else
                {
                    pending_clear_count[bit] = 0U;
                }
            }
        }
    }

    return active_faults;
}

FaultMask get_active_faults(void)
{
    return active_faults;
}

SystemState check_system(float temperature_c,
                         float battery_voltage_v,
                         uint32_t engine_rpm,
                         FaultMask active_faults_now)
{
    if (active_faults_now != FAULT_NONE)
    {
        warning_active = 0U;
        return SYSTEM_FAULT;
    }

    if (warning_condition(temperature_c, battery_voltage_v, engine_rpm))
    {
        warning_active = 1U;
        return SYSTEM_WARNING;
    }

    if (warning_active != 0U)
    {
        if (warning_clear_condition(temperature_c,
                                    battery_voltage_v,
                                    engine_rpm) == 0)
        {
            return SYSTEM_WARNING;
        }

        warning_active = 0U;
    }

    return SYSTEM_NORMAL;
}

const char *get_state_name(SystemState state)
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
