/*
 * LPC11u35 linker script file.
 * This linker is intended to use with libc.
 */

MEMORY
{
  irom  (rx)      : ORIGIN = 0x0,         LENGTH = 0x10000 /* 64k */
  isram (rwx)     : ORIGIN = 0x10000000,  LENGTH = 0x2000  /* 8k   */
  isramUsb (rwx)  : ORIGIN = 0x20004000,  LENGTH = 0x800   /* 2k   */
}

  /* Define a symbol for the top of stack */
  /* It won't be used if user define FIXED_STACKHEAP_SIZE */
  __StackTop = ORIGIN(isram) + LENGTH(isram);

ENTRY(Reset_Handler)

SECTIONS
{
  /* MAIN TEXT SECTION */
  .text : ALIGN(4)
  {
    __text_start = .;
    FILL(0xff)
    /* Always keep this vector table */
    KEEP(*(.isr_vector_table))
    *(.after_vectors*)
    *(.text*)
    *(.rodata .rodata.*)
    . = ALIGN(4);
    __text_end = .;
  } > irom

  /*
   * For exception handling/unwind - some Newlib functions (in common
   * with C++ and STDC++) use this.
   * Use KEEP so it's not discarded with --gc-sections
   */
  .ARM.extab : ALIGN(4)
  {
    KEEP(*(.ARM.extab* .gnu.linkonce.armextab.*))
  } > irom
  __exidx_start = .;

  .ARM.exidx : ALIGN(4)
  {
    KEEP(*(.ARM.exidx* .gnu.linkonce.armexidx.*))
  } > irom
  __exidx_end = .;

  /* This is used for USB RAM section */
  .usb_ram (NOLOAD): ALIGN(4)
  {
    FILL(0xff)
    *(USB_RAM)
  } > isramUsb

  /* MAIN BSS SECTION */
  .bss 0x10000100: ALIGN(4)
  {
    __bss_start = .;
    *(.bss*)
    rt_*.o(COMMON)
    *(COMMON)
    *(.heap)
    __fixed_heap_end = .;
    *(.stack)
    . = ALIGN(4) ;
    __bss_end = .;
    PROVIDE(end = .);
  } > isram

  __data_load_addr = LOADADDR (.data);
  
  .data : ALIGN(4)
  {
    FILL(0xff)
    __data_start = .;
    *(.data*)
    . = ALIGN(4) ;
    __data_end = .;
  } > isram AT>irom
  
  
  /* Define a symbol for the start of heap */
  /* It won't be used if user defined FIXED_STACKHEAP_SIZE */
  PROVIDE(_pvHeapStart = .);
}
