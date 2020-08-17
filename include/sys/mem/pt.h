#pragma once
#include <stdint.h>
#include <stddef.h>

void pt_alloc_init(uintptr_t start, size_t end);
void *pt_alloc(void);
void pt_free(void *pt);
