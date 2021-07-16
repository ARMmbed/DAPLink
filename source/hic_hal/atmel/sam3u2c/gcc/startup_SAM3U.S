/**
 * @file    startup_MK20D5.s
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 1997 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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
/*****************************************************************************/
/* Version: GCC for ARM Embedded Processors                                  */
/*****************************************************************************/
    .syntax unified
    .arch armv7-m

    .section .isr_vector, "a"
    .align 2
    .globl __isr_vector
__isr_vector:
    .long   __StackTop                                      /* Top of Stack */
    .long   Reset_Handler                                   /* Reset Handler */
    .long   NMI_Handler                                     /* NMI Handler*/
    .long   HardFault_Handler                               /* Hard Fault Handler*/
    .long   MemManage_Handler                               /* MPU Fault Handler*/
    .long   BusFault_Handler                                /* Bus Fault Handler*/
    .long   UsageFault_Handler                              /* Usage Fault Handler*/
    .long   0                                               /* Reserved*/
    .long   DAPLINK_BUILD_KEY                               /* DAPLINK: Build type (BL/IF)*/
    .long   DAPLINK_HIC_ID                                  /* DAPLINK: Compatibility*/
    .long   DAPLINK_VERSION                                 /* DAPLINK: Version*/
    .long   SVC_Handler                                     /* SVCall Handler*/
    .long   DebugMon_Handler                                /* Debug Monitor Handler*/
    .long   g_board_info                                    /* DAPLINK: Pointer to board/family/target info*/
    .long   PendSV_Handler                                  /* PendSV Handler*/
    .long   SysTick_Handler                                 /* SysTick Handler*/

    /* External Interrupts*/
    .long   SUPC_IRQHandler           /*  0: Supply Controller */
    .long   RSTC_IRQHandler           /*  1: Reset Controller */
    .long   RTC_IRQHandler            /*  2: Real Time Clock */
    .long   RTT_IRQHandler            /*  3: Real Time Timer */
    .long   WDT_IRQHandler            /*  4: Watchdog Timer */
    .long   PMC_IRQHandler            /*  5: Power Management Controller */
    .long   EEFC0_IRQHandler          /*  6: Enhanced Embedded Flash Controller 0 */
    .long   EEFC1_IRQHandler          /*  7: Enhanced Embedded Flash Controller 1 */
    .long   UART_IRQHandler           /*  8: UART */
    .long   SMC_IRQHandler            /*  9: Static Memory Controller */
    .long   PIOA_IRQHandler           /* 10: Parallel I/O Controller A */
    .long   PIOB_IRQHandler           /* 11: Parallel I/O Controller B */
    .long   PIOC_IRQHandler           /* 12: Parallel I/O Controller C */
    .long   USART0_IRQHandler         /* 13: USART 0 */
    .long   USART1_IRQHandler         /* 14: USART 1 */
    .long   USART2_IRQHandler         /* 15: USART 2 */
    .long   USART3_IRQHandler         /* 16: USART 3 */
    .long   HSMCI_IRQHandler          /* 17: High Speed Multimedia Card Interface */
    .long   TWI0_IRQHandler           /* 18: Two-wire Interface 0 */
    .long   TWI1_IRQHandler           /* 19: Two-wire Interface 1 */
    .long   SPI_IRQHandler            /* 20: Synchronous Peripheral Interface */
    .long   SSC_IRQHandler            /* 21: Synchronous Serial Controller */
    .long   TC0_IRQHandler            /* 22: Timer Counter 0 */
    .long   TC1_IRQHandler            /* 23: Timer Counter 1 */
    .long   TC2_IRQHandler            /* 24: Timer Counter 2 */
    .long   PWM_IRQHandler            /* 25: Pulse Width Modulation Controller */
    .long   ADC12B_IRQHandler         /* 26: 12-bit ADC Controller */
    .long   ADC_IRQHandler            /* 27: 10-bit ADC Controller */
    .long   DMAC_IRQHandler           /* 28: DMA Controller */
    .long   UDPHS_IRQHandler          /* 29: USB Device High Speed */

    .size    __isr_vector, . - __isr_vector

    .text
    .thumb

/* Reset Handler */

    .thumb_func
    .align 2
    .globl   Reset_Handler
    .weak    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    cpsid   i               /* Mask interrupts */
    .equ    VTOR, 0xE000ED08
    ldr     r0, =VTOR
    ldr     r1, =__isr_vector
    str     r1, [r0]
    ldr     r2, [r1]
    msr     msp, r2
#ifndef __NO_SYSTEM_INIT
    ldr   r0,=SystemInit
    blx   r0
#endif
/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in
 *      linker script.
 *      __etext: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */

    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

#if 1
/* Here are two copies of loop implemenations. First one favors code size
 * and the second one favors performance. Default uses the first one.
 * Change to "#if 0" to use the second one */
.LC0:
    cmp     r2, r3
    ittt    lt
    ldrlt   r0, [r1], #4
    strlt   r0, [r2], #4
    blt    .LC0
#else
    subs    r3, r2
    ble    .LC1
.LC0:
    subs    r3, #4
    ldr    r0, [r1, r3]
    str    r0, [r2, r3]
    bgt    .LC0
.LC1:
#endif

#ifdef __STARTUP_CLEAR_BSS
/*     This part of work usually is done in C library startup code. Otherwise,
 *     define this macro to enable it in this startup.
 *
 *     Loop to zero out BSS section, which uses following symbols
 *     in linker script:
 *      __bss_start__: start of BSS section. Must align to 4
 *      __bss_end__: end of BSS section. Must align to 4
 */
    ldr r1, =__bss_start__
    ldr r2, =__bss_end__

    movs    r0, 0
.LC2:
    cmp     r1, r2
    itt    lt
    strlt   r0, [r1], #4
    blt    .LC2
#endif /* __STARTUP_CLEAR_BSS */

    cpsie   i               /* Unmask interrupts */
#ifndef __START
#define __START _start
#endif
#ifndef __ATOLLIC__
    ldr   r0,=__START
    blx   r0
#else
    ldr   r0,=__libc_init_array
    blx   r0
    ldr   r0,=main
    bx    r0
#endif
    .pool
    .size Reset_Handler, . - Reset_Handler

    .align  1
    .thumb_func
    .weak DefaultISR
    .type DefaultISR, %function
DefaultISR:
    b DefaultISR
    .size DefaultISR, . - DefaultISR

    .align 1
    .thumb_func
    .weak NMI_Handler
    .type NMI_Handler, %function
NMI_Handler:
    ldr   r0,=NMI_Handler
    bx    r0
    .size NMI_Handler, . - NMI_Handler

    .align 1
    .thumb_func
    .weak HardFault_Handler
    .type HardFault_Handler, %function
HardFault_Handler:
    ldr   r0,=HardFault_Handler
    bx    r0
    .size HardFault_Handler, . - HardFault_Handler

    .align 1
    .thumb_func
    .weak MemManage_Handler
    .type MemManage_Handler, %function
MemManage_Handler:
    ldr   r0,=MemManage_Handler
    bx    r0
    .size MemManage_Handler, . - MemManage_Handler

    .align 1
    .thumb_func
    .weak BusFault_Handler
    .type BusFault_Handler, %function
BusFault_Handler:
    ldr   r0,=BusFault_Handler
    bx    r0
    .size BusFault_Handler, . - BusFault_Handler

    .align 1
    .thumb_func
    .weak UsageFault_Handler
    .type UsageFault_Handler, %function
UsageFault_Handler:
    ldr   r0,=UsageFault_Handler
    bx    r0
    .size UsageFault_Handler, . - UsageFault_Handler

    .align 1
    .thumb_func
    .weak SVC_Handler
    .type SVC_Handler, %function
SVC_Handler:
    ldr   r0,=SVC_Handler
    bx    r0
    .size SVC_Handler, . - SVC_Handler

    .align 1
    .thumb_func
    .weak DebugMon_Handler
    .type DebugMon_Handler, %function
DebugMon_Handler:
    ldr   r0,=DebugMon_Handler
    bx    r0
    .size DebugMon_Handler, . - DebugMon_Handler

    .align 1
    .thumb_func
    .weak PendSV_Handler
    .type PendSV_Handler, %function
PendSV_Handler:
    ldr   r0,=PendSV_Handler
    bx    r0
    .size PendSV_Handler, . - PendSV_Handler

    .align 1
    .thumb_func
    .weak SysTick_Handler
    .type SysTick_Handler, %function
SysTick_Handler:
    ldr   r0,=SysTick_Handler
    bx    r0
    .size SysTick_Handler, . - SysTick_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro def_irq_handler  handler_name
    .weak \handler_name
    .set  \handler_name, DefaultISR
    .endm

/* Exception Handlers */
    def_irq_handler SUPC_IRQHandler           /*  0: Supply Controller */
    def_irq_handler RSTC_IRQHandler           /*  1: Reset Controller */
    def_irq_handler RTC_IRQHandler            /*  2: Real Time Clock */
    def_irq_handler RTT_IRQHandler            /*  3: Real Time Timer */
    def_irq_handler WDT_IRQHandler            /*  4: Watchdog Timer */
    def_irq_handler PMC_IRQHandler            /*  5: Power Management Controller */
    def_irq_handler EEFC0_IRQHandler          /*  6: Enhanced Embedded Flash Controller 0 */
    def_irq_handler EEFC1_IRQHandler          /*  7: Enhanced Embedded Flash Controller 1 */
    def_irq_handler UART_IRQHandler           /*  8: UART */
    def_irq_handler SMC_IRQHandler            /*  9: Static Memory Controller */
    def_irq_handler PIOA_IRQHandler           /* 10: Parallel I/O Controller A */
    def_irq_handler PIOB_IRQHandler           /* 11: Parallel I/O Controller B */
    def_irq_handler PIOC_IRQHandler           /* 12: Parallel I/O Controller C */
    def_irq_handler USART0_IRQHandler         /* 13: USART 0 */
    def_irq_handler USART1_IRQHandler         /* 14: USART 1 */
    def_irq_handler USART2_IRQHandler         /* 15: USART 2 */
    def_irq_handler USART3_IRQHandler         /* 16: USART 3 */
    def_irq_handler HSMCI_IRQHandler          /* 17: High Speed Multimedia Card Interface */
    def_irq_handler TWI0_IRQHandler           /* 18: Two-wire Interface 0 */
    def_irq_handler TWI1_IRQHandler           /* 19: Two-wire Interface 1 */
    def_irq_handler SPI_IRQHandler            /* 20: Synchronous Peripheral Interface */
    def_irq_handler SSC_IRQHandler            /* 21: Synchronous Serial Controller */
    def_irq_handler TC0_IRQHandler            /* 22: Timer Counter 0 */
    def_irq_handler TC1_IRQHandler            /* 23: Timer Counter 1 */
    def_irq_handler TC2_IRQHandler            /* 24: Timer Counter 2 */
    def_irq_handler PWM_IRQHandler            /* 25: Pulse Width Modulation Controller */
    def_irq_handler ADC12B_IRQHandler         /* 26: 12-bit ADC Controller */
    def_irq_handler ADC_IRQHandler            /* 27: 10-bit ADC Controller */
    def_irq_handler DMAC_IRQHandler           /* 28: DMA Controller */
    def_irq_handler UDPHS_IRQHandler          /* 29: USB Device High Speed */

    .end
