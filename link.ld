ENTRY(kernel_main)

SECTIONS {
  . = 0x1000;

  .text : {
    *(.start)
    *(.text*)
    *(.rodata*)
    *(.data*)
    *(.bss*)
    *(COMMON)
  }

  kernel_end = .;
}
