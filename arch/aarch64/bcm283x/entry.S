.set KERNEL_OFFSET, 0xFFFFFF8000000000
.section .text.boot
.global _start
.type _start, %function
_start:
    mrs x0, mpidr_el1
    ands x0, x0, #0xFF
    beq 1f
    b bcm283x_ap_loop
1:
    ldr x9, =aarch64_l1_kernel - KERNEL_OFFSET

    // Initialize kernel page tables
    bl aarch64_setup_identity_table

    // BSP Entry
    ldr x7, =_bsp_stack_top
    ldr x8, =bcm283x_bsp_entry
    b _drop_to_el1

.size _start, . - _start

// x9 - TTBR1 page table
// x8 - Entry point
// x7 - SP
.global _drop_to_el1
_drop_to_el1:
    // Get current EL
    mrs x0, currentel
    asr x0, x0, #2
    and x0, x0, #0x3
    cmp x0, #3
    beq _drop_el3
    cmp x0, #2
    beq _drop_el2
    b _drop_el1

// TODO: learn what this code does really
_drop_el3:
    // Return to EL2 from EL3
    mov x2, #0x5B1
    msr scr_el3, x2
    mov x2, #0x3C9
    msr spsr_el3, x2
    adr x2, _drop_el2
    msr elr_el3, x2
    eret
_drop_el2:
    // Return to EL1 from EL2
    mov x2, #(1 << 31)
    orr x2, x2, #(1 << 1)
    msr hcr_el2, x2
    mov x2, #0x3C4
    msr spsr_el2, x2
    adr x2, _drop_el1
    msr elr_el2, x2

    msr sp_el1, x7
    ldr x0, =vectors
    msr vbar_el1, x0
    eret
_drop_el1:
    // Prepare for higher-half code entry
    mov sp, x7
    b aarch64_early_higher_half

.align 11
vectors:
    .align 7
    mov x0, #0
    mrs x1, esr_el1
    mrs x2, elr_el1
    mrs x3, spsr_el1
    mrs x4, far_el1
    b aarch64_exc_handler
    .align 7
    mov x0, #1
    mrs x1, esr_el1
    mrs x2, elr_el1
    mrs x3, spsr_el1
    mrs x4, far_el1
    b aarch64_exc_handler
    .align 7
    mov x0, #2
    mrs x1, esr_el1
    mrs x2, elr_el1
    mrs x3, spsr_el1
    mrs x4, far_el1
    b aarch64_exc_handler
    .align 7
    mov x0, #3
    mrs x1, esr_el1
    mrs x2, elr_el1
    mrs x3, spsr_el1
    mrs x4, far_el1
    b aarch64_exc_handler

.section .bss
.align 8
_bsp_stack_bottom:
    .skip 8192
_bsp_stack_top:
