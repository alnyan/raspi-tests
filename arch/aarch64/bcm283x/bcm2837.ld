ENTRY(_start);

SECTIONS {
    . = 0x80000;

    _kernel_start = .;
    .text : ALIGN(4K) {
        *(.text.boot)
        *(.text)
        *(.rodata)
    }

    .data : ALIGN(4K) {
        *(.data)
    }

    .bss : ALIGN(4K) {
        *(.bss)
        *(COMMON)
    }

    . = ALIGN(4K);
    _kernel_end = .;
}
