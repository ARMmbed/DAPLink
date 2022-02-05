/**
 * @file    startup_LPC11Uxx.s
 * @brief   CMSIS Cortex-M0 Core Device Startup File for
 *          NXP LPC11Uxx Device Series
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2020, ARM Limited, All Rights Reserved
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
    .syntax unified
    .arch armv6-m

    .section .isr_vector, "a"
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
    .long    DAPLINK_BUILD_KEY     /* DAPLINK: Build type (BL/IF) */
    .long    DAPLINK_HIC_ID        /* DAPLINK: Compatibility */
    .long    DAPLINK_VERSION       /* DAPLINK: Version */
    .long    SVC_Handler           /* SVCall Handler */
    .long    0                     /* Reserved */
    .long    g_board_info          /* DAPLINK: Pointer to board/family/target info */
    .long    PendSV_Handler        /* PendSV Handler */
    .long    SysTick_Handler       /* SysTick Handler */

/* LPC11xx interrupts */
    .long   FLEX_INT0_IRQHandler      /* 16+ 0 All GPIO pin can be routed to FLEX_INTx */
    .long   FLEX_INT1_IRQHandler      /* 16+ 1 Pin interrupt                           */
    .long   FLEX_INT2_IRQHandler      /* 16+ 2 Pin interrupt                           */
    .long   FLEX_INT3_IRQHandler      /* 16+ 3 Pin interrupt                           */
    .long   FLEX_INT4_IRQHandler      /* 16+ 4 Pin interrupt                           */
    .long   FLEX_INT5_IRQHandler      /* 16+ 5 Pin interrupt                           */
    .long   FLEX_INT6_IRQHandler      /* 16+ 6 Pin interrupt                           */
    .long   FLEX_INT7_IRQHandler      /* 16+ 7 Pin interrupt                           */
    .long   GINT0_IRQHandler          /* 16+ 8 Port interrupt                          */
    .long   GINT1_IRQHandler          /* 16+ 9 Port interrupt                          */
    .long   Reserved_IRQHandler       /* 16+10 Reserved                                */
    .long   Reserved_IRQHandler       /* 16+11 Reserved                                */
    .long   Reserved_IRQHandler       /* 16+12 Reserved                                */
    .long   Reserved_IRQHandler       /* 16+13 Reserved                                */
    .long   SSP1_IRQHandler           /* 16+14 SSP1                                    */
    .long   I2C_IRQHandler            /* 16+15 I2C                                     */
    .long   TIMER16_0_IRQHandler      /* 16+16 16-bit Timer0                           */
    .long   TIMER16_1_IRQHandler      /* 16+17 16-bit Timer1                           */
    .long   TIMER32_0_IRQHandler      /* 16+18 32-bit Timer0                           */
    .long   TIMER32_1_IRQHandler      /* 16+19 32-bit Timer1                           */
    .long   SSP0_IRQHandler           /* 16+20 SSP0                                    */
    .long   UART_IRQHandler           /* 16+21 UART                                    */
    .long   USB_IRQHandler            /* 16+22 USB IRQ                                 */
    .long   USB_FIQHandler            /* 16+23 USB IRQ                                 */
    .long   ADC_IRQHandler            /* 16+24 A/D Converter                           */
    .long   WDT_IRQHandler            /* 16+25 Watchdog Timer                          */
    .long   BOD_IRQHandler            /* 16+26 Brown Out Detect                        */
    .long   FMC_IRQHandler            /* 16+27 IP2111 Flash Memory Controller          */
    .long   Reserved_IRQHandler       /* 16+28 Reserved                                */
    .long   Reserved_IRQHandler       /* 16+29 Reserved                                */
    .long   USBWakeup_IRQHandler      /* 16+30 USB wake up                             */
    .long   IOH_IRQHandler            /* 16+31 I/O Handler                             */

/* Set the CRP (Code Read Protection) configuration word
 * at address 0x2FC to ensure that CRP is disabled.
 * - 0xFFFFFFFF => CRP Disabled
 * - 0x12345678 => CRP Level 1
 * - 0x87654321 => CRP Level 2
 * - 0x43218765 => CRP Level 3 (ARE YOU SURE?)
 * - 0x4E697370 => NO ISP (ARE YOU SURE?)
 */
#define CRP_KEY_ADDR (0x000002FC)
#define CRP_DISABLED (0xFFFFFFFF)
1:  /* Fill up to CRP config word address. */
	.dcb.l  ((CRP_KEY_ADDR - (1b - __isr_vector)) / 4)
CRP_Key:
    .long   CRP_DISABLED

    .size    __isr_vector, . - __isr_vector

    .text
    .thumb

    .thumb_func
    .align 2
    .globl   Reset_Handler
    .weak    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
    cpsid   i                    /* Mask interrupts */
    .equ    VTOR, 0xE000ED08
    ldr     r0, =VTOR
    ldr     r1, =__isr_vector
    str     r1, [r0]
    ldr     r2, [r1]
    msr     msp, r2

    /* Power on RAM1 and USBRAM area */
    ldr     r0, =0x40048080 /* System clock control */
    ldr     r1, =0x0C00485F /* boot default + RAM1, USBRAM */
    str     r1, [r0]

    ldr    r0, =SystemInit
    blx    r0

/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in 
 *      linker script.
 *      __etext: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */

    ldr    r1, =__etext
    ldr    r2, =__data_start__
    ldr    r3, =__data_end__

    subs   r3, r2
    ble    .LC1

    movs   r4, 0
.LC0:
    ldr    r0, [r1,r4]
    str    r0, [r2,r4]
    adds   r4, 4
    cmp    r4, r3
    blt    .LC0
.LC1:

    cpsie  i               /* Unmask interrupts */
    ldr    r0, =_start
    bx     r0
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

    .macro    def_irq_handler    handler_name
    .weak     \handler_name
    .set      \handler_name, Default_Handler
    .endm

    def_irq_handler    FLEX_INT0_IRQHandler
    def_irq_handler    FLEX_INT1_IRQHandler
    def_irq_handler    FLEX_INT2_IRQHandler
    def_irq_handler    FLEX_INT3_IRQHandler
    def_irq_handler    FLEX_INT4_IRQHandler
    def_irq_handler    FLEX_INT5_IRQHandler
    def_irq_handler    FLEX_INT6_IRQHandler
    def_irq_handler    FLEX_INT7_IRQHandler
    def_irq_handler    GINT0_IRQHandler
    def_irq_handler    GINT1_IRQHandler
    def_irq_handler    SSP1_IRQHandler
    def_irq_handler    I2C_IRQHandler
    def_irq_handler    TIMER16_0_IRQHandler
    def_irq_handler    TIMER16_1_IRQHandler
    def_irq_handler    TIMER32_0_IRQHandler
    def_irq_handler    TIMER32_1_IRQHandler
    def_irq_handler    SSP0_IRQHandler
    def_irq_handler    UART_IRQHandler
    def_irq_handler    USB_IRQHandler
    def_irq_handler    USB_FIQHandler
    def_irq_handler    ADC_IRQHandler
    def_irq_handler    WDT_IRQHandler
    def_irq_handler    BOD_IRQHandler
    def_irq_handler    FMC_IRQHandler
    def_irq_handler    USBWakeup_IRQHandler
    def_irq_handler    IOH_IRQHandler

    .end
