#include <stdio.h>

#include "sensor.h"
#include "fault_manager.h"
#include "ecu_state.h"
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "watchdog.h"

int main(void)
{
    printf("====================================\n");
    printf("     VEHICLE MONITORING ECU\n");
    printf("====================================\n\n");

    // Initialize ECU modules
    uart_init();
    adc_init();
    timer_init();
    watchdog_init(1000);

    uart_send("ECU initialization complete.");

    for (int cycle = 1; cycle <= 10; cycle++)
    {
        int scenario = ((cycle - 1) % 5) + 1;

        set_scenario(scenario);

        float temperature = read_temperature();
        float voltage = read_battery_voltage();
        int rpm = read_rpm();

        SystemState detected_state =
            check_system(temperature, voltage, rpm);

        int faults =
            detect_faults(temperature, voltage, rpm);

        update_ecu_state(detected_state);

        printf("\n--- ECU CYCLE %d ---\n", cycle);

        uart_send_float("Temperature", temperature);
        uart_send_float("Battery Voltage", voltage);
        uart_send_int("Engine RPM", rpm);

        printf("[UART] ECU State: %s\n",
               get_state_name(get_ecu_state()));

        print_faults(faults);

        handle_ecu_state();

        watchdog_kick();
        watchdog_check();

        delay_ms(100);
    }

    uart_send("ECU monitoring stopped.");

    return 0;
}
