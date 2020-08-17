#pragma once
#include <stdint.h>

#define MM_VIRTUALIZE(addr)             ((uintptr_t) (addr) | 0xFFFFFF8000000000UL)

#define MM_PAGE_KERNEL                  (0 << 6)
#define MM_PAGE_USER                    (1 << 6)
#define MM_PAGE_RO                      (1 << 7)
#define MM_PAGE_RW                      (0 << 7)
#define MM_PAGE_NX                      (1UL << 54)

typedef uint64_t *mm_space_t;
