/**
 * @file    startup_LPC11Uxx.S
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2019, ARM Limited, All Rights Reserved
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
    .long   0
    .long   0
    .long   0
    .long   0                                               /* Reserved*/
    .long   DAPLINK_BUILD_KEY                               /* Build type - BL/IF*/
    .long   DAPLINK_HIC_ID                                  /* Compatibility*/
    .long   DAPLINK_VERSION                                 /* Version*/
    .long   SVC_Handler                                     /* SVCall Handler*/
    .long   0
    .long   g_board_info                                    /* Ptr to Board info, family info other target details*/
    .long   PendSV_Handler                                  /* PendSV Handler*/
    .long   SysTick_Handler                                 /* SysTick Handler*/

    /* 32 External Interrupts*/
    .long   FLEX_INT0_IRQHandler      /* All GPIO pin can be routed to FLEX_INTx */
    .long   FLEX_INT1_IRQHandler
    .long   FLEX_INT2_IRQHandler
    .long   FLEX_INT3_IRQHandler
    .long   FLEX_INT4_IRQHandler
    .long   FLEX_INT5_IRQHandler
    .long   FLEX_INT6_IRQHandler
    .long   FLEX_INT7_IRQHandler
    .long   GINT0_IRQHandler
    .long   GINT1_IRQHandler          /* PIO0 (0:7) */
    .long   Reserved1_IRQHandler		   /* Reserved */
    .long   Reserved2_IRQHandler
    .long   Reserved3_IRQHandler
    .long   Reserved4_IRQHandler
    .long   SSP1_IRQHandler           /* SSP1 */
    .long   I2C_IRQHandler            /* I2C */
    .long   TIMER16_0_IRQHandler      /* 16-bit Timer0 */
    .long   TIMER16_1_IRQHandler      /* 16-bit Timer1 */
    .long   TIMER32_0_IRQHandler      /* 32-bit Timer0 */
    .long   TIMER32_1_IRQHandler      /* 32-bit Timer1 */
    .long   SSP0_IRQHandler           /* SSP0 */
    .long   UART_IRQHandler           /* UART */
    .long   USB_IRQHandler            /* USB IRQ */
    .long   USB_FIQHandler            /* USB FIQ */
    .long   ADC_IRQHandler            /* A/D Converter */
    .long   WDT_IRQHandler            /* Watchdog timer */
    .long   BOD_IRQHandler            /* Brown Out Detect */
    .long   FMC_IRQHandler            /* IP2111 Flash Memory Controller */
    .long   Reserved5_IRQHandler	       /* Reserved */
    .long   Reserved6_IRQHandler       /* Reserved */
    .long   USBWakeup_IRQHandler      /* USB wake up */
    .long   Reserved7_IRQHandler       /* Reserved */

#if defined(MBED_BOOTLOADER)

// Set the CRP (Code Read Protection) configuration word at address 0x2FC to ensure that
// CRP is disabled.
#define CRP_KEY_ADDR (0x000002FC)
#define CRP_DISABLED (0xFFFFFFFF)

1:  // Fill up to CRP config word address.
	.dcb.l  ((CRP_KEY_ADDR - (1b - __isr_vector)) / 4)

CRP_Key:
    .long   CRP_DISABLED

#endif // MBED_BOOTLOADER

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
    .weak SVC_Handler
    .type SVC_Handler, %function
SVC_Handler:
    ldr   r0,=SVC_Handler
    bx    r0
    .size SVC_Handler, . - SVC_Handler

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
    def_irq_handler    FLEX_INT0_IRQHandler      /* All GPIO pin can be routed to FLEX_INTx */
    def_irq_handler    FLEX_INT1_IRQHandler
    def_irq_handler    FLEX_INT2_IRQHandler
    def_irq_handler    FLEX_INT3_IRQHandler
    def_irq_handler    FLEX_INT4_IRQHandler
    def_irq_handler    FLEX_INT5_IRQHandler
    def_irq_handler    FLEX_INT6_IRQHandler
    def_irq_handler    FLEX_INT7_IRQHandler
    def_irq_handler    GINT0_IRQHandler
    def_irq_handler    GINT1_IRQHandler          /* PIO0 (0:7) */
    def_irq_handler    Reserved1_IRQHandler		   /* Reserved */
    def_irq_handler    Reserved2_IRQHandler
    def_irq_handler    Reserved3_IRQHandler
    def_irq_handler    Reserved4_IRQHandler
    def_irq_handler    SSP1_IRQHandler           /* SSP1 */
    def_irq_handler    I2C_IRQHandler            /* I2C */
    def_irq_handler    TIMER16_0_IRQHandler      /* 16-bit Timer0 */
    def_irq_handler    TIMER16_1_IRQHandler      /* 16-bit Timer1 */
    def_irq_handler    TIMER32_0_IRQHandler      /* 32-bit Timer0 */
    def_irq_handler    TIMER32_1_IRQHandler      /* 32-bit Timer1 */
    def_irq_handler    SSP0_IRQHandler           /* SSP0 */
    def_irq_handler    UART_IRQHandler           /* UART */
    def_irq_handler    USB_IRQHandler            /* USB IRQ */
    def_irq_handler    USB_FIQHandler            /* USB FIQ */
    def_irq_handler    ADC_IRQHandler            /* A/D Converter */
    def_irq_handler    WDT_IRQHandler            /* Watchdog timer */
    def_irq_handler    BOD_IRQHandler            /* Brown Out Detect */
    def_irq_handler    FMC_IRQHandler            /* IP2111 Flash Memory Controller */
    def_irq_handler    Reserved5_IRQHandler	       /* Reserved */
    def_irq_handler    Reserved6_IRQHandler       /* Reserved */
    def_irq_handler    USBWakeup_IRQHandler      /* USB wake up */
    def_irq_handler    Reserved7_IRQHandler       /* Reserved */

    .end
