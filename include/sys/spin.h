#pragma once
#include <stdint.h>

typedef uint32_t spin_t;

void spin_lock(spin_t *spin);
void spin_release(spin_t *spin);
