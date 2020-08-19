#include <stdint.h>

#include "arch/aarch64/bcm283x/uart.h"
#include "arch/aarch64/bcm283x/mbox.h"
#include "arch/aarch64/mem/mem.h"
#include "arch/aarch64/smp.h"
#include "arch/aarch64/cpu.h"
#include "sys/mem/phys.h"
#include "sys/mem/mm.h"
#include "sys/kernel.h"
#include "sys/debug.h"

static int bcm283x_smp_init(void);
static struct aarch64_smp_ops bcm283x_smp_ops = {
    .smp_init = bcm283x_smp_init
};

static int bcm283x_smp_init(void) {
    // TODO: detect CPU count?
    return 3;
}

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

    // Wake up the AP cores
    aarch64_smp_ops = &bcm283x_smp_ops;
    aarch64_smp_init();

    kernel_main();
}

_Noreturn void bcm283x_ap_entry(void) {
    uint64_t mpidr_el1;
    asm volatile ("mrs %0, mpidr_el1":"=r"(mpidr_el1));
    struct cpu *cpu = &cpus[mpidr_el1 & 0xF];
    asm volatile ("msr tpidr_el1, %0"::"r"(cpu));

    cpu = get_cpu();
    kdebug("cpu%d: %p", mpidr_el1 & 0xF, cpu);
    cpu->cpu_state = CPU_STATE_READY;

    aarch64_smp_init_next();

    while (1);
}
