#include <stdio.h>
#include <stdint.h>
#include "hal.h"
#include "watchdog.h"

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
    uint32_t start;

    watchdog_init(1000U);
    start = hal_tick_ms();

    watchdog_check_at(start + 500U);
    ASSERT_TRUE(watchdog_reset_required() == 0U,
                "Watchdog stays healthy before timeout");

    watchdog_check_at(start + 1001U);
    ASSERT_TRUE(watchdog_reset_required() != 0U,
                "Watchdog detects simulated hang");

    watchdog_clear_reset_request();
    ASSERT_TRUE(watchdog_reset_required() == 0U,
                "Watchdog reset request can be cleared");

    watchdog_kick();
    watchdog_check();
    ASSERT_TRUE(watchdog_reset_required() == 0U,
                "Watchdog kick prevents immediate timeout");

    printf("\n====================================\n");
    printf("Watchdog Tests Passed: %d/%d\n", passed, total);
    printf("====================================\n");

    return (passed == total) ? 0 : 1;
}
