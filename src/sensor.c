#include "sensor.h"
#include "adc.h"

static int current_scenario = 0;

void set_scenario(int scenario)
{
    current_scenario = scenario;
}

float read_temperature(void)
{
    float temperature;

    switch (current_scenario)
    {
        case 1:
            temperature = 90.0f;
            break;

        case 2:
            temperature = 120.0f;
            break;

        case 3:
            temperature = 90.0f;
            break;

        case 4:
            temperature = 90.0f;
            break;

        case 5:
            temperature = 140.0f;
            break;

        default:
            temperature = 90.0f;
            break;
    }

    int adc_value = temperature_to_adc(temperature);

    /*
     * Convert the ADC reading back to temperature.
     * This simulates the firmware processing
     * the sensor's raw ADC value.
     */

    float sensor_voltage = adc_to_voltage(adc_value);

    return ((sensor_voltage - 0.5f) / 2.0f) * 150.0f;
}

float read_battery_voltage(void)
{
    float voltage;

    switch (current_scenario)
    {
        case 3:
            voltage = 10.5f;
            break;

        case 5:
            voltage = 16.0f;
            break;

        default:
            voltage = 13.8f;
            break;
    }

    int adc_value = battery_voltage_to_adc(voltage);

    float sensor_voltage = adc_to_voltage(adc_value);

    return sensor_voltage * 4.0f;
}

int read_rpm(void)
{
    switch (current_scenario)
    {
        case 4:
            return 6500;

        case 5:
            return 7000;

        default:
            return 2500;
    }
}
