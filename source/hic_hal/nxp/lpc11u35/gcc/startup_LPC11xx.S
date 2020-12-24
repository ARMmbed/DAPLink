/* File: startup_ARMCM0.S
 * Purpose: startup file for Cortex-M0 devices. Should use with 
 *   GCC for ARM Embedded Processors
 * Version: V1.2
 * Date: 15 Nov 2011
 * 
 * Copyright (c) 2011, ARM Limited
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the ARM Limited nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
    .syntax unified
    .arch armv6-m

/* Memory Model
   The HEAP starts at the end of the DATA section and grows upward.
   
   The STACK starts at the end of the RAM and grows downward.
   
   The HEAP and stack STACK are only checked at compile time:
   (DATA_SIZE + HEAP_SIZE + STACK_SIZE) < RAM_SIZE
   
   This is just a check for the bare minimum for the Heap+Stack area before
   aborting compilation, it is not the run time limit:
   Heap_Size + Stack_Size = 0x80 + 0x80 = 0x100
 */

    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop            /* Top of Stack */
    .long    Reset_Handler         /* Reset Handler */
    .long    NMI_Handler           /* NMI Handler */
    .long    HardFault_Handler     /* Hard Fault Handler */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    DAPLINK_BUILD_KEY     /* Build type - BL/IF */
    .long    DAPLINK_HIC_ID        /* Compatibility */
    .long    DAPLINK_VERSION       /* Version */
    .long    SVC_Handler           /* SVCall Handler */
    .long    0                     /* Reserved */
    .long    g_board_info          /* Ptr to Board info, family info other target details */
    .long    PendSV_Handler        /* PendSV Handler */
    .long    SysTick_Handler       /* SysTick Handler */

/* LPC11xx interrupts */
    .long   FLEX_INT0_IRQHandler      /* 16  0 Wake-up on  pin  PIO0_0             */
    .long   FLEX_INT1_IRQHandler      /* 17  1 Wake-up on  pin  PIO0_1             */
    .long   FLEX_INT2_IRQHandler      /* 18  2 Wake-up on  pin  PIO0_2             */
    .long   FLEX_INT3_IRQHandler      /* 19  3 Wake-up on  pin  PIO0_3             */
    .long   FLEX_INT4_IRQHandler      /* 20  4 Wake-up on  pin  PIO0_4             */
    .long   FLEX_INT5_IRQHandler      /* 21  5 Wake-up on  pin  PIO0_5             */
    .long   FLEX_INT6_IRQHandler      /* 22  6 Wake-up on  pin  PIO0_6             */
    .long   FLEX_INT7_IRQHandler      /* 23  7 Wake-up on  pin  PIO0_7             */
    .long   GINT0_IRQHandler          /* 24  8 Wake-up on  pin  PIO0_8             */
    .long   GINT1_IRQHandler          /* 25  9 Wake-up on  pin  PIO0_9             */
    .long   Reserved_IRQHandler       /* 26 10 Reserved                            */
    .long   Reserved_IRQHandler       /* 27 11 Reserved                            */
    .long   Reserved_IRQHandler       /* 28 12 Reserved                            */
    .long   Reserved_IRQHandler       /* 29 13 Reserved                            */
    .long   SSP1_IRQHandler           /* 30 14 SSP1                                */
    .long   I2C_IRQHandler            /* 31 15 I2C0 SI (state change)              */
    .long   TIMER16_0_IRQHandler      /* 32 16 CT16B0 16 bit timer 0               */
    .long   TIMER16_1_IRQHandler      /* 33 17 CT16B1 16 bit timer 1               */
    .long   TIMER32_0_IRQHandler      /* 34 18 CT32B0 32 bit timer 0               */
    .long   TIMER32_1_IRQHandler      /* 35 19 CT32B1 32 bit timer 1               */
    .long   SSP0_IRQHandler           /* 36 20 SSP                                 */
    .long   UART_IRQHandler           /* 37 21 UART                                */
    .long   USB_IRQHandler            /* 38 22 USB IRQ                             */
    .long   USB_FIQHandler            /* 39 23 USB IRQ                             */
    .long   ADC_IRQHandler            /* 40 24 ADC end of conversion               */
    .long   WDT_IRQHandler            /* 41 25 Watchdog interrupt (WDINT)          */
    .long   BOD_IRQHandler            /* 42 26 BOD Brown-out detect                */
    .long   FMC_IRQHandler            /* 43 27 IP2111 Flash Memory Controller      */
    .long   Reserved_IRQHandler       /* 44 28 Reserved                            */
    .long   Reserved_IRQHandler       /* 45 29 Reserved                            */
    .long   USBWakeup_IRQHandler      /* 46 30 USB wake up                         */
    .long   Reserved_IRQHandler       /* 47 31 Reserved                            */

    .size    __isr_vector, . - __isr_vector

    .section .text.Reset_Handler
    .thumb
    .thumb_func
    .align 2
    .globl    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in 
 *      linker script.
 *      __etext: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */

    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs    r3, r2
    ble    .Lflash_to_ram_loop_end

    movs    r4, 0
.Lflash_to_ram_loop:
    ldr    r0, [r1,r4]
    str    r0, [r2,r4]
    adds    r4, 4
    cmp    r4, r3
    blt    .Lflash_to_ram_loop
.Lflash_to_ram_loop_end:

	/* Power on RAM1 and USBRAM area */
	ldr     r0, =0x40048080 /* System clock control */
	ldr     r1, =0x0C00485F /* boot default + RAM1, USBRAM */
	str     r1, [r0]

    ldr    r0, =SystemInit
    blx    r0
    ldr    r0, =_start
    bx    r0
    .pool
    .size Reset_Handler, . - Reset_Handler
    
    .text
/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro    def_default_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
    b    .
    .size    \handler_name, . - \handler_name
    .endm

    def_default_handler     NMI_Handler
    def_default_handler     HardFault_Handler
    def_default_handler     SVC_Handler
    def_default_handler     PendSV_Handler
    def_default_handler     SysTick_Handler
    def_default_handler     Default_Handler
    def_default_handler     Reserved_IRQHandler

    .macro    def_irq_default_handler    handler_name
    .weak     \handler_name
    .set      \handler_name, Default_Handler
    .endm

    def_irq_default_handler    FLEX_INT0_IRQHandler
    def_irq_default_handler    FLEX_INT1_IRQHandler
    def_irq_default_handler    FLEX_INT2_IRQHandler
    def_irq_default_handler    FLEX_INT3_IRQHandler
    def_irq_default_handler    FLEX_INT4_IRQHandler
    def_irq_default_handler    FLEX_INT5_IRQHandler
    def_irq_default_handler    FLEX_INT6_IRQHandler
    def_irq_default_handler    FLEX_INT7_IRQHandler
    def_irq_default_handler    GINT0_IRQHandler
    def_irq_default_handler    GINT1_IRQHandler
    def_irq_default_handler    SSP1_IRQHandler
    def_irq_default_handler    I2C_IRQHandler
    def_irq_default_handler    TIMER16_0_IRQHandler
    def_irq_default_handler    TIMER16_1_IRQHandler
    def_irq_default_handler    TIMER32_0_IRQHandler
    def_irq_default_handler    TIMER32_1_IRQHandler
    def_irq_default_handler    SSP0_IRQHandler
    def_irq_default_handler    UART_IRQHandler
    def_irq_default_handler    USB_IRQHandler
    def_irq_default_handler    USB_FIQHandler
    def_irq_default_handler    ADC_IRQHandler
    def_irq_default_handler    WDT_IRQHandler
    def_irq_default_handler    BOD_IRQHandler
    def_irq_default_handler    FMC_IRQHandler
    def_irq_default_handler    USBWakeup_IRQHandler

    .end
