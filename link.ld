ENTRY(kernel_main)

SECTIONS {
    . = 0x1000;

    .text : {
        *(.start)
        *(.text*)
    }

    .rodata ALIGN(4K) : {
        *(.rodata*)
    }

    .data ALIGN(4K) : {
        *(.data*)
    }

    .bss ALIGN(4K) (NOLOAD) : {
        *(COMMON)
        *(.bss*)
    }

    kernel_end = .;
}
