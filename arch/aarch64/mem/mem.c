#include "arch/aarch64/mem/mem.h"
#include "arch/aarch64/mem/map.h"
#include "sys/mem/pt.h"

extern char _kernel_end;

#define MMIO_BASE   0x3F000000

void aarch64_mem_init(void) {
    pt_alloc_init((uintptr_t) &_kernel_end, 0x1000 * 2048);
}
