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
    .long   DMA0_IRQHandler                                 /* DMA channel 0 transfer complete*/
    .long   DMA1_IRQHandler                                 /* DMA channel 1 transfer complete*/
    .long   DMA2_IRQHandler                                 /* DMA channel 2 transfer complete*/
    .long   DMA3_IRQHandler                                 /* DMA channel 3 transfer complete*/
    .long   DMA_Error_IRQHandler                            /* DMA channel 0 - 15 error*/
    .long   Reserved21_IRQHandler  /* Reserved interrupt 21 */
    .long   FTFL_IRQHandler  /* FTFL interrupt */
    .long   Read_Collision_IRQHandler  /* Read collision interrupt */
    .long   LVD_LVW_IRQHandler  /* Low Voltage Detect, Low Voltage Warning */
    .long   LLW_IRQHandler  /* Low Leakage Wakeup */
    .long   Watchdog_IRQHandler  /* WDOG interrupt */
    .long   I2C0_IRQHandler  /* I2C0 interrupt */
    .long   SPI0_IRQHandler  /* SPI0 interrupt */
    .long   I2S0_Tx_IRQHandler  /* I2S0 transmit interrupt */
    .long   I2S0_Rx_IRQHandler  /* I2S0 receive interrupt */
    .long   UART0_LON_IRQHandler  /* UART0 LON interrupt */
    .long   UART0_RX_TX_IRQHandler  /* UART0 receive/transmit interrupt */
    .long   UART0_ERR_IRQHandler  /* UART0 error interrupt */
    .long   UART1_RX_TX_IRQHandler  /* UART1 receive/transmit interrupt */
    .long   UART1_ERR_IRQHandler  /* UART1 error interrupt */
    .long   UART2_RX_TX_IRQHandler  /* UART2 receive/transmit interrupt */
    .long   UART2_ERR_IRQHandler  /* UART2 error interrupt */
    .long   ADC0_IRQHandler  /* ADC0 interrupt */
    .long   CMP0_IRQHandler  /* CMP0 interrupt */
    .long   CMP1_IRQHandler  /* CMP1 interrupt */
    .long   FTM0_IRQHandler  /* FTM0 fault, overflow and channels interrupt */
    .long   FTM1_IRQHandler  /* FTM1 fault, overflow and channels interrupt */
    .long   CMT_IRQHandler  /* CMT interrupt */
    .long   RTC_IRQHandler  /* RTC interrupt */
    .long   RTC_Seconds_IRQHandler  /* RTC seconds interrupt */
    .long   PIT0_IRQHandler  /* PIT timer channel 0 interrupt */
    .long   PIT1_IRQHandler  /* PIT timer channel 1 interrupt */
    .long   PIT2_IRQHandler  /* PIT timer channel 2 interrupt */
    .long   PIT3_IRQHandler  /* PIT timer channel 3 interrupt */
    .long   PDB0_IRQHandler  /* PDB0 interrupt */
    .long   USB0_IRQHandler  /* USB0 interrupt */
    .long   USBDCD_IRQHandler  /* USBDCD interrupt */
    .long   TSI0_IRQHandler  /* TSI0 interrupt */
    .long   MCG_IRQHandler  /* MCG interrupt */
    .long   LPTimer_IRQHandler  /* LPTimer interrupt */
    .long   PORTA_IRQHandler  /* Port A interrupt */
    .long   PORTB_IRQHandler  /* Port B interrupt */
    .long   PORTC_IRQHandler  /* Port C interrupt */
    .long   PORTD_IRQHandler  /* Port D interrupt */
    .long   PORTE_IRQHandler  /* Port E interrupt */
    .long   SWI_IRQHandler  /* Software interrupt */

    .size    __isr_vector, . - __isr_vector

/* Flash Configuration */
    .section .FlashConfig, "a"
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFF
    .long 0xFFFFFFFE

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
    def_irq_handler    DMA0_IRQHandler
    def_irq_handler    DMA1_IRQHandler
    def_irq_handler    DMA2_IRQHandler
    def_irq_handler    DMA3_IRQHandler
    def_irq_handler    DMA_Error_IRQHandler
    def_irq_handler    Reserved21_IRQHandler
    def_irq_handler    FTFL_IRQHandler
    def_irq_handler    Read_Collision_IRQHandler
    def_irq_handler    LVD_LVW_IRQHandler
    def_irq_handler    LLW_IRQHandler
    def_irq_handler    Watchdog_IRQHandler
    def_irq_handler    I2C0_IRQHandler
    def_irq_handler    SPI0_IRQHandler
    def_irq_handler    I2S0_Tx_IRQHandler
    def_irq_handler    I2S0_Rx_IRQHandler
    def_irq_handler    UART0_LON_IRQHandler
    def_irq_handler    UART0_RX_TX_IRQHandler
    def_irq_handler    UART0_ERR_IRQHandler
    def_irq_handler    UART1_RX_TX_IRQHandler
    def_irq_handler    UART1_ERR_IRQHandler
    def_irq_handler    UART2_RX_TX_IRQHandler
    def_irq_handler    UART2_ERR_IRQHandler
    def_irq_handler    ADC0_IRQHandler
    def_irq_handler    CMP0_IRQHandler
    def_irq_handler    CMP1_IRQHandler
    def_irq_handler    FTM0_IRQHandler
    def_irq_handler    FTM1_IRQHandler
    def_irq_handler    CMT_IRQHandler
    def_irq_handler    RTC_IRQHandler
    def_irq_handler    RTC_Seconds_IRQHandler
    def_irq_handler    PIT0_IRQHandler
    def_irq_handler    PIT1_IRQHandler
    def_irq_handler    PIT2_IRQHandler
    def_irq_handler    PIT3_IRQHandler
    def_irq_handler    PDB0_IRQHandler
    def_irq_handler    USB0_IRQHandler
    def_irq_handler    USBDCD_IRQHandler
    def_irq_handler    TSI0_IRQHandler
    def_irq_handler    MCG_IRQHandler
    def_irq_handler    LPTimer_IRQHandler
    def_irq_handler    PORTA_IRQHandler
    def_irq_handler    PORTB_IRQHandler
    def_irq_handler    PORTC_IRQHandler
    def_irq_handler    PORTD_IRQHandler
    def_irq_handler    PORTE_IRQHandler
    def_irq_handler    SWI_IRQHandler

    .end
