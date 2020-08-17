#pragma once
#include <stdint.h>
#include <stddef.h>

enum page_usage {
    PU_UNSPEC = 0,
    PU_RESERVED,
    PU_PRIVATE,
    PU_SHARED,
    PU_DEVICE
};

struct phys_stat {
    size_t pages_commit;
    size_t pages_avail;
    size_t pages_alloc;
};

struct page;
extern struct page *phys_pages;

#define GET_PAGE(phys)      (&phys_pages[(phys) >> 12])

void mm_phys_init(uintptr_t pages_begin, size_t max_count);
void mm_phys_commit(uintptr_t page, enum page_usage pu);
uintptr_t mm_phys_page_alloc(void);
void mm_phys_page_free(uintptr_t page);
void mm_phys_stat(struct phys_stat *st);
