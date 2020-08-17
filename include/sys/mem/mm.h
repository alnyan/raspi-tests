#pragma once

// Should define:
// * Type: mm_space_t
// * Constant macros: MM_PAGE_RO,
//                    MM_PAGE_RW,
//                    MM_PAGE_USER,
//                    MM_PAGE_KERNEL,
//                    MM_PAGE_NX,
//                    MM_PAGE_HUGE
// * Func macros:     MM_VIRTUALIZE(addr),
//                    MM_PHYS(vaddr)
#if defined(ARCH_AARCH64)
#include "arch/aarch64/mem/map.h"
#endif
#include "sys/mem/phys.h"

int mm_map_single(mm_space_t space,
                  uintptr_t virt,
                  uintptr_t phys,
                  uintptr_t flags,
                  enum page_usage u);
