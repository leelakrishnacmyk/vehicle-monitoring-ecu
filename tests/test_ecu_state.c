#include <stdio.h>
#include "ecu_state.h"

int main(void)
{
    int passed = 0;
    int total = 0;

    /* Test NORMAL */
    total++;

    update_ecu_state(SYSTEM_NORMAL);

    if (get_ecu_state() == SYSTEM_NORMAL)
    {
        printf("[PASS] NORMAL state\n");
        passed++;
    }
    else
    {
        printf("[FAIL] NORMAL state\n");
    }

    /* Test WARNING */
    total++;

    update_ecu_state(SYSTEM_WARNING);

    if (get_ecu_state() == SYSTEM_WARNING)
    {
        printf("[PASS] WARNING state\n");
        passed++;
    }
    else
    {
        printf("[FAIL] WARNING state\n");
    }

    /* Test FAULT */
    total++;

    update_ecu_state(SYSTEM_FAULT);

    if (get_ecu_state() == SYSTEM_FAULT)
    {
        printf("[PASS] FAULT state\n");
        passed++;
    }
    else
    {
        printf("[FAIL] FAULT state\n");
    }

    /* Test recovery */
    total++;

    update_ecu_state(SYSTEM_FAULT);
    update_ecu_state(SYSTEM_NORMAL);

    if (get_ecu_state() == SYSTEM_NORMAL)
    {
        printf("[PASS] FAULT -> NORMAL recovery\n");
        passed++;
    }
    else
    {
        printf("[FAIL] FAULT -> NORMAL recovery\n");
    }

    printf("\n====================================\n");
    printf("State Tests Passed: %d/%d\n", passed, total);
    printf("====================================\n");

    return (passed == total) ? 0 : 1;
}
