/*
******************************************************************************
**
**  File        : LinkerScript.ld
**
**  Author      : STM32CubeIDE
**
**  Abstract    : Linker script for STM32WB55xG Device
**                     1024Kbytes FLASH
**                      256Kbytes RAM
**
**                Set heap size, stack size and stack location according
**                to application requirements.
**
**                Set memory bank area and size if external memory is used.
**
**  Target      : STMicroelectronics STM32
**
**  Distribution: The file is distributed as is without any warranty
**                of any kind.
**
*****************************************************************************
** @attention
**
** <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
** All rights reserved.</center></h2>
**
** This software component is licensed by ST under BSD 3-Clause license,
** the "License"; You may not use this file except in compliance with the
** License. You may obtain a copy of the License at:
**                        opensource.org/licenses/BSD-3-Clause
**
*****************************************************************************
*/
/* Header files where daplink specific memory region start and end addresses are defined */ 
#include "daplink_addr.h" 
#include "daplink_defaults.h"

/* Reset Entry Point - first executable instruction */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack or the end address of device RAM + 1
*  Stack PUSH instruction always decrements by 1 and then PUSH data 	
*/
_estack = DAPLINK_RAM_START + DAPLINK_RAM_SIZE;	

__heap_size__ = 0x200 ;	/* required amount of heap  */
__stack_size__ = 0x400 ;	/* required amount of stack */
_Min_Heap_Size = DEFINED(__heap_size__)  ? __heap_size__  : DAPLINK_HEAP_SIZE; ;	/* required amount of heap  */
_Min_Stack_Size = DEFINED(__stack_size__) ? __stack_size__ : DAPLINK_STACK_SIZE;	/* required amount of stack */

/* Memories definition */
/* Attributes rx - Read & Execute section, rw - Read / Write section */
MEMORY
{
  RAM_SHARED            (xrw) : ORIGIN = 0x20030000, LENGTH = 10K
  m_interrupts          (rx)  : ORIGIN = DAPLINK_ROM_CONFIG_ADMIN_START, LENGTH = 0x400
  m_text                (rx)  : ORIGIN = DAPLINK_ROM_APP_START, LENGTH = DAPLINK_ROM_APP_SIZE
  m_cfgrom              (rw)  : ORIGIN = DAPLINK_ROM_CONFIG_USER_START, LENGTH = DAPLINK_ROM_CONFIG_USER_SIZE
  m_data                (rw)  : ORIGIN = DAPLINK_RAM_APP_START, LENGTH = DAPLINK_RAM_APP_SIZE
  m_cfgram              (rw)  : ORIGIN = DAPLINK_RAM_SHARED_START, LENGTH = DAPLINK_RAM_SHARED_SIZE
}

/* Sections */
SECTIONS
{
  /* The startup code into "FLASH" Rom type memory */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >m_interrupts

  /* The program code and other data into "FLASH" Rom type memory */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >m_text

  /* Constant data into "FLASH" Rom type memory */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >m_text

  .ARM.extab   : {
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } >m_text

  .ARM : {
    . = ALIGN(4);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  } >m_text

  .preinit_array     :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  } >m_text

  .init_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  } >m_text

  .fini_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  } >m_text

  /* Used by the startup to initialize data */
  _sidata = LOADADDR(.data);

    __DATA_ROM = .;
  /* Initialized data sections into "RAM" Ram type memory */
  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    *(.RamFunc)        /* .RamFunc sections */
    *(.RamFunc*)       /* .RamFunc* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */	

  } >m_data
  
  __DATA_END = __DATA_ROM + (_edata - _sdata);
 
  .fill __DATA_END :
  {
    FILL(0xffffffff)
    . = ALIGN(4);
    . += 0x0800C000 + 0x00013C00 - __DATA_END - 4;
    LONG(0x55555555)
  } > m_text
 
  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")

  .cfgrom (NOLOAD) :
  {
    *(cfgrom)
  } >m_cfgrom
  
  /* Uninitialized data section into "RAM" Ram type memory */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    _sbss = .;         /* define a global symbol at bss start */
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */    
  } >m_data

  /* User_heap_stack section, used to check that there is enough "RAM" Ram  type memory left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } > m_data 

  .cfgram (NOLOAD) :
  {
    *(cfgram)
  } >m_cfgram

  /* Remove information from the compiler libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }
   .ARM.attributes 0       : { *(.ARM.attributes) }
   MAPPING_TABLE (NOLOAD) : { *(MAPPING_TABLE) } >RAM_SHARED
   MB_MEM1 (NOLOAD)       : { *(MB_MEM1) } >RAM_SHARED
   MB_MEM2 (NOLOAD)       : { _sMB_MEM2 = . ; *(MB_MEM2) ; _eMB_MEM2 = . ; } >RAM_SHARED

}
