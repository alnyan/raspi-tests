#pragma once
#include <stdint.h>
#include <stddef.h>

extern volatile uint32_t bcm283x_mbox[36];

int mbox_call(uint8_t channel);

// Common calls
int mbox_get_arm_memory(size_t *mem);
