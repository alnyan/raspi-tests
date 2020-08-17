#include <stdint.h>

#include "arch/aarch64/bcm283x/uart.h"
#include "arch/aarch64/bcm283x/mbox.h"
#include "arch/aarch64/mem/mem.h"
#include "sys/mem/phys.h"
#include "sys/kernel.h"
#include "sys/debug.h"

static void bcm283x_add_ram(void) {
    // Add bcm283x memory regions to alloc
    // Determine VC/ARM SDRAM split
    size_t mem;
    if (mbox_get_arm_memory(&mem) != 0) {
        kdebug("Failed to determine ARM memory\n");
        while (1);
    }
    // Skip 64K at start
    for (size_t i = 16; i < (mem >> 12); ++i) {
        uintptr_t page = i << 12;
        mm_phys_commit(page, PU_UNSPEC);
    }
    // TODO: Commit the rest as "device memory"
}

_Noreturn void bcm283x_bsp_entry(void) {
    bcm283x_uart_init();
    aarch64_mem_init();
    bcm283x_add_ram();

    struct phys_stat st;
    mm_phys_stat(&st);
    kdebug("Physical memory:");
    kdebug("Total: %zuM, available: %zuM, used: %zuM",
           st.pages_commit / 256,
           st.pages_avail / 256,
           st.pages_alloc / 256);

    kernel_main();
}
