#include "adc.h"

void adc_init(void)
{
    /* Simulated ADC initialization */
}

float adc_to_voltage(int adc_value)
{
    return (adc_value * ADC_REFERENCE_VOLTAGE)
           / ADC_MAX_VALUE;
}

int temperature_to_adc(float temperature)
{
    /*
     * Simulate a temperature sensor where:
     * 0 C   -> 0.5 V
     * 150 C -> 2.5 V
     */

    float sensor_voltage =
        0.5f + (temperature / 150.0f) * 2.0f;

    return (int)((sensor_voltage / ADC_REFERENCE_VOLTAGE)
                 * ADC_MAX_VALUE);
}

int battery_voltage_to_adc(float voltage)
{
    /*
     * Simulate a voltage divider.
     * Battery voltage is scaled down by 4.
     */

    float sensor_voltage = voltage / 4.0f;

    return (int)((sensor_voltage / ADC_REFERENCE_VOLTAGE)
                 * ADC_MAX_VALUE);
}
