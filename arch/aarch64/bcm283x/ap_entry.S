#include "arch/aarch64/asm_cpu.h"
.set KERNEL_OFFSET, 0xFFFFFF8000000000
.section .text
.global bcm283x_ap_loop
.type bcm283x_ap_loop, %function
bcm283x_ap_loop:
    // spin_* are leaf functions,
    // so no stack is used
    ldr x0, =aarch64_ap_spin - KERNEL_OFFSET
    bl spin_lock
    b _ap_entry
.size bcm283x_ap_loop, . - bcm283x_ap_loop

_ap_entry:
    // Recall our CPU number
    mrs x0, mpidr_el1
    and x0, x0, #0xFF

    // Grab physical stack address provided to us by BSP
    sub x1, x0, #1
    ldr x0, =aarch64_ap_ptrs - KERNEL_OFFSET
    ldr x0, [x0, x1, lsl #3]
    ldr w0, [x0, #4]

    // Turn it into a virtual one
    ldr x1, =KERNEL_OFFSET
    orr x7, x1, x0

    // Use shared kernel L1 table for identity-mapping
    // KERNEL_OFFSET addresses
    ldr x9, =aarch64_l1_kernel - KERNEL_OFFSET
    // x8 - entry point
    ldr x8, =bcm283x_ap_entry

    b _drop_to_el1
