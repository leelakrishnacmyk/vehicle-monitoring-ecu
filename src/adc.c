#include "adc.h"
#include "config.h"

static int clamp_adc(int adc_value)
{
    if (adc_value < 0)
    {
        return 0;
    }

    if (adc_value > (int)ADC_MAX_VALUE)
    {
        return (int)ADC_MAX_VALUE;
    }

    return adc_value;
}

static int volts_to_adc(float sensor_voltage)
{
    if (sensor_voltage <= 0.0f)
    {
        return 0;
    }

    if (sensor_voltage >= ADC_REFERENCE_VOLTAGE)
    {
        return (int)ADC_MAX_VALUE;
    }

    return clamp_adc((int)((sensor_voltage / ADC_REFERENCE_VOLTAGE) *
                           ADC_MAX_VALUE + 0.5f));
}

void adc_init(void)
{
    /* Simulated ADC initialization. */
}

float adc_to_voltage(int adc_value)
{
    int clamped = clamp_adc(adc_value);

    return ((float)clamped * ADC_REFERENCE_VOLTAGE) / ADC_MAX_VALUE;
}

int temperature_to_adc(float temperature)
{
    float sensor_voltage = TEMP_SENSOR_V_OFFSET +
                           (temperature / TEMP_SENSOR_MAX_TEMP) *
                           TEMP_SENSOR_V_SPAN;

    return volts_to_adc(sensor_voltage);
}

int battery_voltage_to_adc(float voltage)
{
    float sensor_voltage = voltage / BATTERY_DIVIDER_RATIO;

    return volts_to_adc(sensor_voltage);
}
