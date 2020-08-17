#include "arch/aarch64/mem/mem.h"
#include "arch/aarch64/mem/map.h"
#include "sys/mem/phys.h"
#include "sys/mem/pt.h"

extern char _kernel_end;

#define MMIO_BASE   0x3F000000

void aarch64_mem_init(void) {
    // Initialize paging structure pool
    pt_alloc_init((uintptr_t) &_kernel_end, 0x1000 * 2048);

    // Initialize physical memory allocator
    uintptr_t end_aligned = (((uintptr_t) &_kernel_end) + 0xFFF) & ~0xFFF;
    size_t page_count = (1024 * 1024 * 1024) >> 12;
    mm_phys_init(end_aligned, page_count);
}
