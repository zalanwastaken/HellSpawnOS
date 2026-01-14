ENTRY(main)

SECTIONS {
    . = 0x9000;

    .text : {
        *(.start)
        *(.text*)
    }

    .rodata ALIGN(2K) : {
        *(.rodata*)
    }

    .data ALIGN(2K) : {
        *(.data*)
    }

    .bss ALIGN(2K) (NOLOAD) : {
        *(COMMON)
        *(.bss*)
    }
}
