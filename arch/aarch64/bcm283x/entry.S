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
    ldr x0, =_bsp_stack_top
    mov sp, x0

    bl bcm283x_bsp_entry
    b .

.size _start, . - _start

.section .bss
.align 8
_bsp_stack_bottom:
    .skip 8192
_bsp_stack_top:
