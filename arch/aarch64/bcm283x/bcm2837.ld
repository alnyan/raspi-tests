ENTRY(_start);

/* bits [63:42] select TTBR1 */
KERNEL_OFFSET = 0xFFFFFF8000000000;

SECTIONS {
    . = KERNEL_OFFSET + 0x80000;

    _kernel_start = .;
    .text : AT(. - KERNEL_OFFSET) ALIGN(4K) {
        *(.text.boot)
        *(.text)
        *(.rodata*)
    }

    .data : AT(. - KERNEL_OFFSET) ALIGN(4K) {
        *(.data)
    }

    .bss : AT(. - KERNEL_OFFSET) ALIGN(4K) {
        *(.bss)
        *(COMMON)
    }

    . = ALIGN(4K);
    _kernel_end = .;
}
