#include "sensor_sim.h"

static uint32_t current_scenario = 1U;

void sensor_sim_set_scenario(uint32_t scenario)
{
    current_scenario = scenario;
}

float sensor_sim_temperature_c(void)
{
    switch (current_scenario)
    {
        case 2U:
            return 120.0f;
        case 5U:
            return 140.0f;
        case 6U:
            return -37.5f; /* Converts to an ADC endpoint for sensor-fault testing. */
        default:
            return 90.0f;
    }
}

float sensor_sim_battery_voltage_v(void)
{
    switch (current_scenario)
    {
        case 3U:
            return 10.5f;
        case 5U:
            return 16.0f;
        case 7U:
            return 0.0f; /* ADC 0: simulated open/short sensor condition. */
        default:
            return 13.8f;
    }
}

uint32_t sensor_sim_engine_rpm(void)
{
    switch (current_scenario)
    {
        case 4U:
            return 6500U;
        case 5U:
            return 7000U;
        default:
            return 2500U;
    }
}
