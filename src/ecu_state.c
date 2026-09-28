#include "ecu_state.h"
#include "outputs.h"
#include "uart.h"

static SystemState current_state = SYSTEM_NORMAL;
static uint8_t safe_state_active = 0U;

void ecu_state_init(void)
{
    current_state = SYSTEM_NORMAL;
    safe_state_active = 0U;
    outputs_init();
}

void update_ecu_state(SystemState new_state)
{
    current_state = new_state;

    switch (new_state)
    {
        case SYSTEM_NORMAL:
        case SYSTEM_WARNING:
            /* WARNING keeps outputs operational while alerting the driver. */
            safe_state_active = 0U;
            outputs_enable();
            break;

        case SYSTEM_FAULT:
        default:
            /* FAULT enforces simulated failsafe output shutdown. */
            safe_state_active = 1U;
            outputs_disable();
            break;
    }
}

SystemState get_ecu_state(void)
{
    return current_state;
}

uint8_t ecu_safe_state_active(void)
{
    return safe_state_active;
}

void handle_ecu_state(void)
{
    switch (current_state)
    {
        case SYSTEM_NORMAL:
            uart_send("System operating normally.");
            break;

        case SYSTEM_WARNING:
            uart_send("WARNING: Check vehicle conditions.");
            break;

        case SYSTEM_FAULT:
            uart_send("FAULT DETECTED!");
            uart_send("SAFE STATE ACTIVE: outputs disabled.");
            break;

        default:
            uart_send("Unknown ECU state.");
            break;
    }
}
