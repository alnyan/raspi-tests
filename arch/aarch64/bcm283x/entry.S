.set KERNEL_OFFSET, 0xFFFFFF8000000000
.section .text.boot
.global _start
.type _start, %function
_start:
    mrs x0, mpidr_el1
    ands x0, x0, #0xFF
    beq 1f
_ap_loop:
    b _ap_loop
1:
    // Get current EL
    mrs x0, currentel
    asr x0, x0, #2
    and x0, x0, #3
    ldr x1, =(_el_entry_tab - KERNEL_OFFSET)
    ldr x1, [x1, x0, lsl #3]
    br x1

// TODO: learn what this code does really
_bsp_entry_el3:
    // Return to EL2 from EL3
    mov x2, #0x5B1
    msr scr_el3, x2
    mov x2, #0x3C9
    msr spsr_el3, x2
    adr x2, _bsp_entry_el2
    msr elr_el3, x2
    eret
_bsp_entry_el2:
    // Return to EL1 from EL2
    mov x2, #(1 << 31)
    orr x2, x2, #(1 << 1)
    msr hcr_el2, x2
    mov x2, #0x3C4
    msr spsr_el2, x2
    adr x2, _bsp_entry_el1
    msr elr_el2, x2
    eret
_bsp_entry_el1:
    // Prepare for higher-half code entry
    ldr x8, =_bsp_entry_high
    b aarch64_early_higher_half

_bsp_entry_high:
    ldr x8, =_bsp_stack_top
    mov sp, x8

    bl bcm283x_bsp_entry
    b .

.size _start, . - _start

.align 8
// Jump table
_el_entry_tab:
    .dword 0
    .dword _bsp_entry_el1 - KERNEL_OFFSET
    .dword _bsp_entry_el2 - KERNEL_OFFSET
    .dword _bsp_entry_el3 - KERNEL_OFFSET
.size _el_entry_tab, . - _el_entry_tab

.section .bss
.align 8
_bsp_stack_bottom:
    .skip 8192
_bsp_stack_top:
