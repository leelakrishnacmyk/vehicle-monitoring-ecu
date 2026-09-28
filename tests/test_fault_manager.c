#include <stdio.h>
#include "config.h"
#include "fault_manager.h"

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

    faults = detect_faults(90.0f, 13.8f, 2500U, FAULT_NONE);
    ASSERT_TRUE(faults == FAULT_NONE, "Normal conditions");

    faults = detect_faults(TEMP_FAULT_C, 13.8f, 2500U, FAULT_NONE);
    ASSERT_TRUE((faults & FAULT_OVER_TEMPERATURE) != 0U,
                "Temperature fault boundary");

    faults = detect_faults(90.0f, VOLTAGE_UNDER_FAULT, 2500U, FAULT_NONE);
    ASSERT_TRUE((faults & FAULT_UNDER_VOLTAGE) != 0U,
                "Under-voltage fault boundary");

    faults = detect_faults(90.0f, VOLTAGE_OVER_FAULT, 2500U, FAULT_NONE);
    ASSERT_TRUE((faults & FAULT_OVER_VOLTAGE) != 0U,
                "Over-voltage fault boundary");

    faults = detect_faults(90.0f, 13.8f, RPM_FAULT, FAULT_NONE);
    ASSERT_TRUE((faults & FAULT_HIGH_RPM) != 0U,
                "RPM fault boundary");

    ASSERT_TRUE(check_system(TEMP_WARN_C, 13.8f, 2500U, FAULT_NONE) == SYSTEM_WARNING,
                "Temperature warning boundary");
    ASSERT_TRUE(check_system(90.0f, 13.8f, RPM_WARN, FAULT_NONE) == SYSTEM_WARNING,
                "RPM warning boundary");
    ASSERT_TRUE(check_system(90.0f, VOLTAGE_WARN_LOW, 2500U, FAULT_NONE) == SYSTEM_WARNING,
                "Low-voltage warning boundary");
    ASSERT_TRUE(check_system(90.0f, VOLTAGE_WARN_HIGH, 2500U, FAULT_NONE) == SYSTEM_WARNING,
                "High-voltage warning boundary");

    fault_manager_init();
    faults = fault_manager_update(140.0f, 13.8f, 2500U, FAULT_NONE);
    ASSERT_TRUE((faults & FAULT_OVER_TEMPERATURE) == 0U,
                "Fault is debounced after first bad sample");

    faults = fault_manager_update(140.0f, 13.8f, 2500U, FAULT_NONE);
    ASSERT_TRUE((faults & FAULT_OVER_TEMPERATURE) != 0U,
                "Fault latches after consecutive bad samples");

    ASSERT_TRUE(check_system(140.0f, 13.8f, 2500U, faults) == SYSTEM_FAULT,
                "Active fault drives FAULT state");

    faults = fault_manager_update(127.0f, 13.8f, 2500U, FAULT_NONE);
    ASSERT_TRUE((faults & FAULT_OVER_TEMPERATURE) != 0U,
                "Hysteresis keeps temperature fault active");

    faults = fault_manager_update(TEMP_CLEAR_C, 13.8f, 2500U, FAULT_NONE);
    ASSERT_TRUE((faults & FAULT_OVER_TEMPERATURE) == 0U,
                "Temperature fault clears below hysteresis threshold");

    faults = detect_faults(140.0f, 16.0f, 7000U, FAULT_NONE);
    ASSERT_TRUE((faults & FAULT_OVER_TEMPERATURE) != 0U &&
                (faults & FAULT_OVER_VOLTAGE) != 0U &&
                (faults & FAULT_HIGH_RPM) != 0U,
                "Multiple simultaneous faults");

    printf("\n====================================\n");
    printf("Fault Tests Passed: %d/%d\n", passed, total);
    printf("====================================\n");

    return (passed == total) ? 0 : 1;
}
