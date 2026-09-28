#include <stdio.h>
#include "ecu_state.h"

static SystemState current_state = SYSTEM_NORMAL;

void update_ecu_state(SystemState new_state)
{
    current_state = new_state;
}

SystemState get_ecu_state(void)
{
    return current_state;
}

void handle_ecu_state(void)
{
    switch (current_state)
    {
        case SYSTEM_NORMAL:
            printf("[ECU] System operating normally.\n");
            break;

        case SYSTEM_WARNING:
            printf("[ECU] WARNING: Check vehicle conditions.\n");
            break;

        case SYSTEM_FAULT:
            printf("[ECU] FAULT DETECTED!\n");
            printf("[ECU] Entering SAFE STATE.\n");
            break;

        default:
            printf("[ECU] Unknown state.\n");
            break;
    }
}
