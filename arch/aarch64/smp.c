#include <stdint.h>
#include "sys/debug.h"
#include "arch/aarch64/smp.h"
#include "arch/aarch64/cpu.h"
#include "sys/mem/phys.h"
#include "sys/mem/mm.h"
#include "sys/spin.h"

spin_t aarch64_ap_spin = 1;
struct aarch64_smp_ops *aarch64_smp_ops = NULL;
// Indirection so that AP can properly find its struct cpu
// Physical addresses
uintptr_t aarch64_ap_ptrs[15];
int cpu_count = 1;

void aarch64_smp_init_next(void) {
    spin_release(&aarch64_ap_spin);
}

int aarch64_smp_init(void) {
    // Setup BSP cpu struct
    struct cpu *bsp = &cpus[0];
    int ap_count;
    asm volatile ("msr tpidr_el1, %0"::"r"(bsp));
    bsp = get_cpu();
    kdebug("cpu0: %p", bsp);
    bsp->cpu_state = CPU_STATE_READY;

    if (aarch64_smp_ops && aarch64_smp_ops->smp_init) {
        ap_count = aarch64_smp_ops->smp_init();
        if (ap_count < 0) {
            kdebug("Failed to get CPU count");
            while (1);
        }

        kdebug("Found %d APs", ap_count);
        cpu_count += ap_count;
    } else {
        ap_count = 0;
    }

    // Setup AP stacks
    for (int i = 0; i < ap_count; ++i) {
        uintptr_t page = mm_phys_page_alloc();
        if (page == MM_NADDR || page >= 0x100000000) {
            kdebug("Failed to alloc a page for AP stack");
            break;
        }

        cpus[i + 1].ap_stack_phys = page & 0xFFFFFFFF;
        aarch64_ap_ptrs[i] = MM_PHYS(&cpus[i + 1]);
        kdebug("cpu%d stack phys %p", i + 1, page);
    }

    // Release the AP spinlock once, so that the first AP can init
    // The lock will then be released by the AP itself allowing the
    // next one to init and so on
    aarch64_smp_init_next();

    return 0;
}
