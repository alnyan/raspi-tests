.set KERNEL_OFFSET, 0xFFFFFF8000000000
// Code for early setup of higher-half virtual memory
.section .text
.global aarch64_setup_identity_table
.global aarch64_early_higher_half
// Args:
//  x9 - L1 page table (aligned block of 512 64-bit entries)
//  x8 - Entry point, with KERNEL_OFFSET applied
aarch64_early_higher_half:
    // Setup no-cache attributes in MAIR[0]
    mov x1, #(4 << 4) | (4 << 0)
    msr mair_el1, x1

    // Check for 4K granule, x1 = IPS
    mrs x1, id_aa64mmfr0_el1
    mov x2, #(0xFUL << 28)
    and x3, x1, x2
    cmp x3, x2
    beq _no_4k_gran
    and x1, x1, #0x7

    // MMU control register setup
    lsl x1, x1, #32
    ldr x2, =0xb5193519
    orr x2, x2, x1
    msr tcr_el1, x2
    isb

    // Write TTBRn
    msr ttbr0_el1, x9
    msr ttbr1_el1, x9

    // Write system control register
    dsb ish
    isb
    mrs x1, sctlr_el1
    ldr x2, =0xC00800UL
    orr x1, x1, x2

    bic x1, x1, #(1 << 25)
    bic x1, x1, #(1 << 24)
    bic x1, x1, #(1 << 19)
    bic x1, x1, #(1 << 12)
    bic x1, x1, #(1 << 4)
    bic x1, x1, #(1 << 3)
    bic x1, x1, #(1 << 2)
    bic x1, x1, #(1 << 1)

    orr x1, x1, #(1 << 0)       /* Enable MMU */
    msr sctlr_el1, x1
    isb

    ldr x1, =_now_high
    br x1
_now_high:
    // Remove low mapping for now: full high half
    dsb ish
    isb
    msr ttbr0_el1, xzr
    isb

    br x8
_no_4k_gran:
    b .

// Args:
//  x9 - page table
aarch64_setup_identity_table:
    // Setup entries for identity mapping
    mov x1, xzr
1:  // Point entry to 1G block: x1 << 30
    lsl x2, x1, #30
    orr x2, x2, #(1 << 0)   /* Present */
    orr x2, x2, #(1 << 10)  /* Accessed */
    orr x2, x2, #(3 << 8)   /* Inner shareable */

    str x2, [x9, x1, lsl #3]

    add x1, x1, #1
    cmp x1, #512
    bne 1b

    ret
