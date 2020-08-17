#pragma once
#include <stdint.h>

enum page_usage {
    PU_UNSPEC = 0,
    PU_PRIVATE,
    PU_SHARED,
    PU_DEVICE
};

struct page;

uintptr_t mm_phys_page_alloc(void);
void mm_phys_page_free(uintptr_t page);
