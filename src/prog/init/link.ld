ENTRY(main)

SECTIONS {
  . = 0x9000;

  .text : {
    *(.start)
    *(.text*)
    *(.rodata*)
    *(.data*)
    *(.bss*)
    *(COMMON)
  }
}
