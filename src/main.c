#include <stdint.h>
#include <stdio.h>

#include "adc.h"
#include "config.h"
#include "ecu_state.h"
#include "fault_manager.h"
#include "hal.h"
#include "sensor.h"
#include "system.h"
#include "timer.h"
#include "uart.h"
#include "watchdog.h"

static FaultMask sensor_faults_from_data(const SensorData *data)
{
    FaultMask faults = FAULT_NONE;

    if (data->temperature_valid == 0U)
    {
        faults |= FAULT_SENSOR_TEMP;
    }
    if (data->battery_voltage_valid == 0U)
    {
        faults |= FAULT_SENSOR_VOLTAGE;
    }
    if (data->rpm_valid == 0U)
    {
        faults |= FAULT_SENSOR_RPM;
    }

    return faults;
}

int main(void)
{
    static const uint32_t scenario_sequence[] =
    {
        1U, 1U, 2U, 2U, 3U, 3U,
        4U, 4U, 5U, 5U, 1U, 1U
    };
    const uint32_t cycle_count =
        (uint32_t)(sizeof(scenario_sequence) / sizeof(scenario_sequence[0]));
    uint32_t next_wake;
    uint32_t cycle;

    printf("====================================\n");
    printf("     VEHICLE MONITORING ECU\n");
    printf("====================================\n\n");

    uart_init();
    adc_init();
    timer_init();
    ecu_state_init();
    fault_manager_init();
    watchdog_init(WATCHDOG_TIMEOUT_MS);

    uart_send("ECU initialization complete.");

    next_wake = hal_tick_ms();

    for (cycle = 0U; cycle < cycle_count; ++cycle)
    {
        SensorData sensors;
        FaultMask sensor_faults;
        FaultMask active_faults;
        SystemState state;

        /* Check before starting work; the previous cycle must have kicked the watchdog. */
        watchdog_check();
        if (watchdog_reset_required() != 0U)
        {
            system_reset();
            break;
        }

        sensor_set_scenario(scenario_sequence[cycle]);
        sensor_read_all(&sensors);
        sensor_faults = sensor_faults_from_data(&sensors);

        active_faults = fault_manager_update(
            sensors.temperature_c,
            sensors.battery_voltage_v,
            sensors.engine_rpm,
            sensor_faults);

        state = check_system(
            sensors.temperature_c,
            sensors.battery_voltage_v,
            sensors.engine_rpm,
            active_faults);

        update_ecu_state(state);

        printf("\n--- ECU CYCLE %u ---\n", (unsigned int)(cycle + 1U));
        uart_send_float("Temperature", sensors.temperature_c);
        uart_send_float("Battery Voltage", sensors.battery_voltage_v);
        uart_send_uint("Engine RPM", (unsigned int)sensors.engine_rpm);
        uart_send("Sensor validity checked.");
        uart_send("ECU State:");
        uart_send(get_state_name(state));
        uart_send_faults(active_faults);
        handle_ecu_state();

        /* Kick only after the complete cycle finished successfully. */
        watchdog_kick();

        next_wake += ECU_CYCLE_MS;
        {
            uint32_t now = hal_tick_ms();
            int32_t wait_ms = (int32_t)(next_wake - now);

            if (wait_ms > 0)
            {
                delay_ms((uint32_t)wait_ms);
            }
        }
    }

    uart_send("ECU monitoring stopped.");

    return system_reset_requested() != 0U ? 1 : 0;
}
