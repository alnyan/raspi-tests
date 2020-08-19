#pragma once
#include <stdint.h>
#include "arch/aarch64/asm_cpu.h"

struct cpu {
    uint32_t cpu_state;         // @ 0x0
    uint32_t ap_stack_phys;     // @ 0x4
};

extern struct cpu cpus[];

static inline struct cpu *get_cpu(void) {
    uintptr_t ptr;
    asm volatile ("mrs %0, tpidr_el1":"=r"(ptr));
    return (struct cpu *) ptr;
}
