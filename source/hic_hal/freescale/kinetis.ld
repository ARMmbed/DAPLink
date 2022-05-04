/**
 * @file    kinetis.ld
 * @brief   Kinetis linker script
 *
 * DAPLink Interface Firmware
 * Copyright 2016 Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * Copyright (c) 2021, Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "daplink_addr.h"
#include "daplink_defaults.h"

/* Entry Point */
ENTRY(Reset_Handler)

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : DAPLINK_HEAP_SIZE;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : DAPLINK_STACK_SIZE;

/* Specify the memory areas */
MEMORY
{
#if defined(DAPLINK_BL)
  m_interrupts          (RX)  : ORIGIN = DAPLINK_ROM_APP_START, LENGTH = 0x3C0
  m_bootloader_config   (RX)  : ORIGIN = DAPLINK_ROM_APP_START + 0x3C0, LENGTH = 0x40
  m_flash_config        (RX)  : ORIGIN = DAPLINK_ROM_APP_START + 0x400, LENGTH = 0x10
  m_text                (RX)  : ORIGIN = DAPLINK_ROM_APP_START + 0x410, LENGTH = DAPLINK_ROM_APP_SIZE - 0x410
#else
  m_interrupts          (RX)  : ORIGIN = DAPLINK_ROM_APP_START, LENGTH = 0x400
  m_text                (RX)  : ORIGIN = DAPLINK_ROM_APP_START + 0x400, LENGTH = DAPLINK_ROM_APP_SIZE - 0x400
#endif
  m_cfgrom              (RW)  : ORIGIN = DAPLINK_ROM_CONFIG_USER_START, LENGTH = DAPLINK_ROM_CONFIG_USER_SIZE
  m_data                (RW)  : ORIGIN = DAPLINK_RAM_APP_START, LENGTH = DAPLINK_RAM_APP_SIZE
  m_cfgram              (RW)  : ORIGIN = DAPLINK_RAM_SHARED_START, LENGTH = DAPLINK_RAM_SHARED_SIZE
}

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into internal flash */
  .interrupts :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    FILL(0xffffffff)
    . = ALIGN(4);
    . += LENGTH(m_interrupts) - (. - ORIGIN(m_interrupts)); /* pad out to end of m_interrupts */
  } > m_interrupts

#if defined(DAPLINK_BL)
  .bootloader_config :
  {
    . = ALIGN(4);
    KEEP(*(.BootloaderConfig)) /* Bootloader Configuration Area (BCA) */
    . = ALIGN(4);
  } > m_bootloader_config

  .flash_config :
  {
    . = ALIGN(4);
    KEEP(*(.FlashConfig))    /* Flash Configuration Field (FCF) */
    . = ALIGN(4);
  } > m_flash_config
#endif

  /* The program code and other data goes into internal flash */
  .text :
  {
    . = ALIGN(4);
    *(.text)                 /* .text sections (code) */
    *(.text*)                /* .text* sections (code) */
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
  } > m_text

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_text

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_text

 .ctors :
  {
    __CTOR_LIST__ = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_text

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_text

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_text

  __etext = .;    /* define a global symbol at end of code */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization */

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* create a global symbol at data start */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    *(.ram_func)             /* RAM functions */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* define a global symbol at data end */
  } > m_data

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);

  /* fill .text out to the end of the interface */
  .fill __DATA_END :
  {
    FILL(0xffffffff)
    . = ALIGN(4);
    . += DAPLINK_ROM_APP_START + DAPLINK_ROM_APP_SIZE - __DATA_END - 4;
    /* Need some contents in this section or it won't be copied to bin or hex. The CRC will
     * be placed here by post_build_script.py. */
    LONG(0x55555555)
  } > m_text

  text_end = ORIGIN(m_text) + LENGTH(m_text);
  ASSERT(__DATA_END <= text_end, "region m_text overflowed with text and data")

  .cfgrom (NOLOAD) :
  {
    *(cfgrom)
  } > m_cfgrom

  /* Uninitialized data section */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section */
    . = ALIGN(4);
    __START_BSS = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    __END_BSS = .;
  } > m_data

  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __heap_limit = .; /* Add for _sbrk */
  } > m_data

  .stack :
  {
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_data

  .cfgram (NOLOAD) :
  {
    *(cfgram)
  } > m_cfgram

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_data) + LENGTH(m_data);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }

  ASSERT(__StackLimit >= __HeapLimit, "region overflowed with stack and heap")
}

