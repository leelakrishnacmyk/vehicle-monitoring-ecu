#include <stdio.h>
#include "ecu_state.h"
#include "fault_manager.h"
#include "outputs.h"

#define ASSERT_TRUE(condition, name) \
    do { \
        ++total; \
        if (condition) { \
            ++passed; \
            printf("[PASS] %s\n", name); \
        } else { \
            printf("[FAIL] %s\n", name); \
        } \
    } while (0)

int main(void)
{
    int passed = 0;
    int total = 0;
    FaultMask faults;
    SystemState state;

    ecu_state_init();
    fault_manager_init();

    faults = fault_manager_update(90.0f, 13.8f, 2500U, FAULT_NONE);
    state = check_system(90.0f, 13.8f, 2500U, faults);
    update_ecu_state(state);
    ASSERT_TRUE(get_ecu_state() == SYSTEM_NORMAL,
                "Normal conditions drive NORMAL state");
    ASSERT_TRUE(outputs_are_enabled() != 0U,
                "Outputs enabled in NORMAL state");

    faults = fault_manager_update(120.0f, 13.8f, 2500U, FAULT_NONE);
    state = check_system(120.0f, 13.8f, 2500U, faults);
    update_ecu_state(state);
    ASSERT_TRUE(get_ecu_state() == SYSTEM_WARNING,
                "Warning threshold drives WARNING state");
    ASSERT_TRUE(ecu_safe_state_active() == 0U && outputs_are_enabled() != 0U,
                "WARNING keeps outputs enabled");

    faults = fault_manager_update(140.0f, 13.8f, 2500U, FAULT_NONE);
    state = check_system(140.0f, 13.8f, 2500U, faults);
    update_ecu_state(state);
    faults = fault_manager_update(140.0f, 13.8f, 2500U, FAULT_NONE);
    state = check_system(140.0f, 13.8f, 2500U, faults);
    update_ecu_state(state);

    ASSERT_TRUE(get_ecu_state() == SYSTEM_FAULT,
                "Consecutive fault samples drive FAULT state");
    ASSERT_TRUE(ecu_safe_state_active() != 0U && outputs_are_enabled() == 0U,
                "FAULT state disables outputs");

    /* One clean sample must not clear the latched fault. */
    faults = fault_manager_update(90.0f, 13.8f, 2500U, FAULT_NONE);
    state = check_system(90.0f, 13.8f, 2500U, faults);
    update_ecu_state(state);
    ASSERT_TRUE(get_ecu_state() == SYSTEM_FAULT,
                "First clean sample does not clear latched fault");

    /* Second clean sample clears the fault and restores outputs. */
    faults = fault_manager_update(90.0f, 13.8f, 2500U, FAULT_NONE);
    state = check_system(90.0f, 13.8f, 2500U, faults);
    update_ecu_state(state);

    ASSERT_TRUE(get_ecu_state() == SYSTEM_NORMAL &&
                ecu_safe_state_active() == 0U &&
                outputs_are_enabled() != 0U,
                "Recovered conditions restore NORMAL state and outputs");

    /* Directly verify FAULT -> WARNING also re-enables outputs. */
    update_ecu_state(SYSTEM_FAULT);
    update_ecu_state(SYSTEM_WARNING);
    ASSERT_TRUE(ecu_safe_state_active() == 0U && outputs_are_enabled() != 0U,
                "FAULT to WARNING recovery re-enables outputs");

    printf("\n====================================\n");
    printf("State Tests Passed: %d/%d\n", passed, total);
    printf("====================================\n");

    return (passed == total) ? 0 : 1;
}
