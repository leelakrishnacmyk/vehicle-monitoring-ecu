#include <stdio.h>
#include "adc.h"
#include "config.h"
#include "sensor.h"

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
    SensorData data;
    int adc_16v;
    float reconstructed_16v;

    adc_16v = battery_voltage_to_adc(16.0f);
    reconstructed_16v = adc_to_voltage(adc_16v) * BATTERY_DIVIDER_RATIO;

    ASSERT_TRUE(adc_16v < (int)ADC_MAX_VALUE,
                "16 V battery input stays below ADC saturation");
    ASSERT_TRUE(reconstructed_16v > 15.8f && reconstructed_16v < 16.2f,
                "6:1 divider preserves 16 V measurement");

    ASSERT_TRUE(temperature_to_adc(130.0f) > 0,
                "Temperature ADC conversion produces valid code");

    sensor_set_scenario(1U);
    sensor_read_all(&data);
    ASSERT_TRUE(data.temperature_valid != 0U &&
                data.battery_voltage_valid != 0U &&
                data.rpm_valid != 0U,
                "Normal sensor readings are valid");

    sensor_set_scenario(6U);
    sensor_read_all(&data);
    ASSERT_TRUE(data.temperature_valid == 0U,
                "Temperature ADC endpoint is detected as invalid");

    sensor_set_scenario(7U);
    sensor_read_all(&data);
    ASSERT_TRUE(data.battery_voltage_valid == 0U,
                "Battery ADC endpoint is detected as invalid");

    printf("\n====================================\n");
    printf("ADC/Sensor Tests Passed: %d/%d\n", passed, total);
    printf("====================================\n");

    return (passed == total) ? 0 : 1;
}
