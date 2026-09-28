#include "outputs.h"

static uint8_t outputs_enabled = 1U;

void outputs_init(void)
{
    outputs_enabled = 1U;
}

void outputs_enable(void)
{
    outputs_enabled = 1U;
}

void outputs_disable(void)
{
    outputs_enabled = 0U;
}

uint8_t outputs_are_enabled(void)
{
    return outputs_enabled;
}
