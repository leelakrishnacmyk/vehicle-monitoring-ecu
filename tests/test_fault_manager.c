#include <stdio.h>
#include "fault_manager.h"

int main(void)
{
    int passed = 0;
    int total = 0;

    /* Test 1: Normal conditions */
    total++;

    int faults = detect_faults(90.0f, 13.8f, 2500);

    if (faults == FAULT_NONE)
    {
        printf("[PASS] Normal conditions\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Normal conditions\n");
    }

    /* Test 2: Over temperature */
    total++;

    faults = detect_faults(140.0f, 13.8f, 2500);

    if (faults & FAULT_OVER_TEMPERATURE)
    {
        printf("[PASS] Over temperature detection\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Over temperature detection\n");
    }

    /* Test 3: Under voltage */
    total++;

    faults = detect_faults(90.0f, 10.5f, 2500);

    if (faults & FAULT_UNDER_VOLTAGE)
    {
        printf("[PASS] Under voltage detection\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Under voltage detection\n");
    }

    /* Test 4: High RPM */
    total++;

    faults = detect_faults(90.0f, 13.8f, 6500);

    if (faults & FAULT_HIGH_RPM)
    {
        printf("[PASS] High RPM detection\n");
        passed++;
    }
    else
    {
        printf("[FAIL] High RPM detection\n");
    }

    /* Test 5: Multiple faults */
    total++;

    faults = detect_faults(140.0f, 16.0f, 7000);

    if ((faults & FAULT_OVER_TEMPERATURE) &&
        (faults & FAULT_OVER_VOLTAGE) &&
        (faults & FAULT_HIGH_RPM))
    {
        printf("[PASS] Multiple fault detection\n");
        passed++;
    }
    else
    {
        printf("[FAIL] Multiple fault detection\n");
    }

    printf("\n====================================\n");
    printf("Tests Passed: %d/%d\n", passed, total);
    printf("====================================\n");

    return (passed == total) ? 0 : 1;
}
