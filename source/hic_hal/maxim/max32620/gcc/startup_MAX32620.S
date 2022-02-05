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
    .long   CLKMAN_IRQHandler         /* 16:01 CLKMAN */
    .long   PWRMAN_IRQHandler         /* 17:02 PWRMAN */
    .long   FLC_IRQHandler            /* 18:03 Flash Controller */
    .long   RTC0_IRQHandler           /* 19:04 RTC INT0 */
    .long   RTC1_IRQHandler           /* 20:05 RTC INT1 */
    .long   RTC2_IRQHandler           /* 21:06 RTC INT2 */
    .long   RTC3_IRQHandler           /* 22:07 RTC INT3 */
    .long   PMU_IRQHandler            /* 23:08 PMU */
    .long   USB_IRQHandler            /* 24:09 USB */
    .long   AES_IRQHandler            /* 25:10 AES */
    .long   MAA_IRQHandler            /* 26:11 MAA */
    .long   WDT0_IRQHandler           /* 27:12 WATCHDOG0 */
    .long   WDT0_P_IRQHandler         /* 28:13 WATCHDOG0 PRE-WINDOW */
    .long   WDT1_IRQHandler           /* 29:14 WATCHDOG1 */
    .long   WDT1_P_IRQHandler         /* 30:15 WATCHDOG1 PRE-WINDOW */
    .long   GPIO_P0_IRQHandler        /* 31:16 GPIO Port 0  */
    .long   GPIO_P1_IRQHandler        /* 32:17 GPIO Port 1  */
    .long   GPIO_P2_IRQHandler        /* 33:18 GPIO Port 2  */
    .long   GPIO_P3_IRQHandler        /* 34:19 GPIO Port 3  */
    .long   GPIO_P4_IRQHandler        /* 35:20 GPIO Port 4  */
    .long   GPIO_P5_IRQHandler        /* 36:21 GPIO Port 5  */
    .long   GPIO_P6_IRQHandler        /* 37:22 GPIO Port 6  */
    .long   TMR0_IRQHandler           /* 38:23 Timer32-0 */
    .long   TMR16_0_IRQHandler        /* 39:24 Timer16-s0 */
    .long   TMR1_IRQHandler           /* 40:25 Timer32-1 */
    .long   TMR16_1_IRQHandler        /* 41:26 Timer16-s1 */
    .long   TMR2_IRQHandler           /* 42:27 Timer32-2 */
    .long   TMR16_2_IRQHandler        /* 43:28 Timer16-s2 */
    .long   TMR3_IRQHandler           /* 44:29 Timer32-3 */
    .long   TMR16_3_IRQHandler        /* 45:30 Timer16-s3 */
    .long   TMR4_IRQHandler           /* 46:31 Timer32-4 */
    .long   TMR16_4_IRQHandler        /* 47:32 Timer16-s4 */
    .long   TMR5_IRQHandler           /* 48:33 Timer32-5 */
    .long   TMR16_5_IRQHandler        /* 49:34 Timer16-s5 */
    .long   UART0_IRQHandler          /* 50:35 UART0 */
    .long   UART1_IRQHandler          /* 51:36 UART1 */
    .long   UART2_IRQHandler          /* 52:37 UART0 */
    .long   UART3_IRQHandler          /* 53:38 UART1 */
    .long   PT_IRQHandler             /* 54:39 PT */
    .long   I2CM0_IRQHandler          /* 55:40 I2C Master 0 */
    .long   I2CM1_IRQHandler          /* 56:41 I2C Master 1 */
    .long   I2CM2_IRQHandler          /* 57:42 I2C Master 2 */
    .long   I2CS_IRQHandler           /* 58:43 I2C Slave */
    .long   SPI0_IRQHandler           /* 59:44 SPI0 */
    .long   SPI1_IRQHandler           /* 60:45 SPI1 */
    .long   SPI2_IRQHandler           /* 61:46 SPI2 */
    .long   SPIB_IRQHandler           /* 62:47 SPI Bridge */
    .long   OWM_IRQHandler            /* 63:48 1-Wire Master */
    .long   AFE_IRQHandler            /* 64:49 AFE */

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
    def_irq_handler CLKMAN_IRQHandler         /* 16:01 CLKMAN */
    def_irq_handler PWRMAN_IRQHandler         /* 17:02 PWRMAN */
    def_irq_handler FLC_IRQHandler            /* 18:03 Flash Controller */
    def_irq_handler RTC0_IRQHandler           /* 19:04 RTC INT0 */
    def_irq_handler RTC1_IRQHandler           /* 20:05 RTC INT1 */
    def_irq_handler RTC2_IRQHandler           /* 21:06 RTC INT2 */
    def_irq_handler RTC3_IRQHandler           /* 22:07 RTC INT3 */
    def_irq_handler PMU_IRQHandler            /* 23:08 PMU */
    def_irq_handler USB_IRQHandler            /* 24:09 USB */
    def_irq_handler AES_IRQHandler            /* 25:10 AES */
    def_irq_handler MAA_IRQHandler            /* 26:11 MAA */
    def_irq_handler WDT0_IRQHandler           /* 27:12 WATCHDOG0 */
    def_irq_handler WDT0_P_IRQHandler         /* 28:13 WATCHDOG0 PRE-WINDOW */
    def_irq_handler WDT1_IRQHandler           /* 29:14 WATCHDOG1 */
    def_irq_handler WDT1_P_IRQHandler         /* 30:15 WATCHDOG1 PRE-WINDOW */
    def_irq_handler GPIO_P0_IRQHandler        /* 31:16 GPIO Port 0  */
    def_irq_handler GPIO_P1_IRQHandler        /* 32:17 GPIO Port 1  */
    def_irq_handler GPIO_P2_IRQHandler        /* 33:18 GPIO Port 2  */
    def_irq_handler GPIO_P3_IRQHandler        /* 34:19 GPIO Port 3  */
    def_irq_handler GPIO_P4_IRQHandler        /* 35:20 GPIO Port 4  */
    def_irq_handler GPIO_P5_IRQHandler        /* 36:21 GPIO Port 5  */
    def_irq_handler GPIO_P6_IRQHandler        /* 37:22 GPIO Port 6  */
    def_irq_handler TMR0_IRQHandler           /* 38:23 Timer32-0 */
    def_irq_handler TMR16_0_IRQHandler        /* 39:24 Timer16-s0 */
    def_irq_handler TMR1_IRQHandler           /* 40:25 Timer32-1 */
    def_irq_handler TMR16_1_IRQHandler        /* 41:26 Timer16-s1 */
    def_irq_handler TMR2_IRQHandler           /* 42:27 Timer32-2 */
    def_irq_handler TMR16_2_IRQHandler        /* 43:28 Timer16-s2 */
    def_irq_handler TMR3_IRQHandler           /* 44:29 Timer32-3 */
    def_irq_handler TMR16_3_IRQHandler        /* 45:30 Timer16-s3 */
    def_irq_handler TMR4_IRQHandler           /* 46:31 Timer32-4 */
    def_irq_handler TMR16_4_IRQHandler        /* 47:32 Timer16-s4 */
    def_irq_handler TMR5_IRQHandler           /* 48:33 Timer32-5 */
    def_irq_handler TMR16_5_IRQHandler        /* 49:34 Timer16-s5 */
    def_irq_handler UART0_IRQHandler          /* 50:35 UART0 */
    def_irq_handler UART1_IRQHandler          /* 51:36 UART1 */
    def_irq_handler UART2_IRQHandler          /* 52:37 UART0 */
    def_irq_handler UART3_IRQHandler          /* 53:38 UART1 */
    def_irq_handler PT_IRQHandler             /* 54:39 PT */
    def_irq_handler I2CM0_IRQHandler          /* 55:40 I2C Master 0 */
    def_irq_handler I2CM1_IRQHandler          /* 56:41 I2C Master 1 */
    def_irq_handler I2CM2_IRQHandler          /* 57:42 I2C Master 2 */
    def_irq_handler I2CS_IRQHandler           /* 58:43 I2C Slave */
    def_irq_handler SPI0_IRQHandler           /* 59:44 SPI0 */
    def_irq_handler SPI1_IRQHandler           /* 60:45 SPI1 */
    def_irq_handler SPI2_IRQHandler           /* 61:46 SPI2 */
    def_irq_handler SPIB_IRQHandler           /* 62:47 SPI Bridge */
    def_irq_handler OWM_IRQHandler            /* 63:48 1-Wire Master */
    def_irq_handler AFE_IRQHandler            /* 64:49 AFE */

    .end
