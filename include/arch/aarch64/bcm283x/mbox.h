#pragma once
#include <stdint.h>

extern volatile uint32_t bcm283x_mbox[36];

int mbox_call(uint8_t channel);
