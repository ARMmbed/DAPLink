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
    .long   0x5A5A5A5A                                      /* Reserved*/
    .long   DAPLINK_BUILD_KEY                               /* Build type - BL/IF*/
    .long   DAPLINK_HIC_ID                                  /* Compatibility*/
    .long   DAPLINK_VERSION                                 /* Version*/
    .long   SVC_Handler                                     /* SVCall Handler*/
    .long   DebugMon_Handler                                /* Debug Monitor Handler*/
    .long   g_board_info                                    /* Ptr to Board info, family info other target details*/
    .long   PendSV_Handler                                  /* PendSV Handler*/
    .long   SysTick_Handler                                 /* SysTick Handler*/

    /* External Interrupts*/
    .long   DAC_IRQHandler              /* 16 D/A Converter */
    .long   M0CORE_IRQHandler           /* 17 M0 Core */
    .long   DMA_IRQHandler              /* 18 General Purpose DMA */
    .long   EZH_IRQHandler              /* 19 EZH/EDM */
    .long   FLASH_EEPROM_IRQHandler     /* 20 Reserved for Typhoon */
    .long   ETH_IRQHandler              /* 21 Ethernet */
    .long   SDIO_IRQHandler             /* 22 SD/MMC */
    .long   LCD_IRQHandler              /* 23 LCD */
    .long   USB0_IRQHandler             /* 24 USB0 */
    .long   USB1_IRQHandler             /* 25 USB1 */
    .long   SCT_IRQHandler              /* 26 State Configurable Timer */
    .long   RIT_IRQHandler              /* 27 Repetitive Interrupt Timer */
    .long   TIMER0_IRQHandler           /* 28 Timer0 */
    .long   TIMER1_IRQHandler           /* 29 Timer1 */
    .long   TIMER2_IRQHandler           /* 30 Timer2 */
    .long   TIMER3_IRQHandler           /* 31 Timer3 */
    .long   MCPWM_IRQHandler            /* 32 Motor Control PWM */
    .long   ADC0_IRQHandler             /* 33 A/D Converter 0 */
    .long   I2C0_IRQHandler             /* 34 I2C0 */
    .long   I2C1_IRQHandler             /* 35 I2C1 */
    .long   SPI_IRQHandler              /* 36 SPI */
    .long   ADC1_IRQHandler             /* 37 A/D Converter 1 */
    .long   SSP0_IRQHandler             /* 38 SSP0 */
    .long   SSP1_IRQHandler             /* 39 SSP1 */
    .long   UART0_IRQHandler            /* 40 UART0 */
    .long   UART1_IRQHandler            /* 41 UART1 */
    .long   UART2_IRQHandler            /* 42 UART2 */
    .long   UART3_IRQHandler            /* 43 UART3 */
    .long   I2S0_IRQHandler             /* 44 I2S0 */
    .long   I2S1_IRQHandler             /* 45 I2S1 */
    .long   SPIFI_IRQHandler            /* 46 SPI Flash Interface */
    .long   SGPIO_IRQHandler            /* 47 SGPIO */
    .long   GPIO0_IRQHandler            /* 48 GPIO0 */
    .long   GPIO1_IRQHandler            /* 49 GPIO1 */
    .long   GPIO2_IRQHandler            /* 50 GPIO2 */
    .long   GPIO3_IRQHandler            /* 51 GPIO3 */
    .long   GPIO4_IRQHandler            /* 52 GPIO4 */
    .long   GPIO5_IRQHandler            /* 53 GPIO5 */
    .long   GPIO6_IRQHandler            /* 54 GPIO6 */
    .long   GPIO7_IRQHandler            /* 55 GPIO7 */
    .long   GINT0_IRQHandler            /* 56 GINT0 */
    .long   GINT1_IRQHandler            /* 57 GINT1 */
    .long   EVRT_IRQHandler             /* 58 Event Router */
    .long   CAN1_IRQHandler             /* 59 C_CAN1 */
    .long   0                           /* 60 Reserved */
    .long   VADC_IRQHandler             /* 61 VADC */
    .long   ATIMER_IRQHandler           /* 62 ATIMER */
    .long   RTC_IRQHandler              /* 63 RTC */
    .long   0                           /* 64 Reserved */
    .long   WDT_IRQHandler              /* 65 WDT */
    .long   M0s_IRQHandler              /* 66 M0s */
    .long   CAN0_IRQHandler             /* 67 C_CAN0 */
    .long   QEI_IRQHandler              /* 68 QEI */

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
    def_irq_handler DAC_IRQHandler              /* 16 D/A Converter */
    def_irq_handler M0CORE_IRQHandler           /* 17 M0 Core */
    def_irq_handler DMA_IRQHandler              /* 18 General Purpose DMA */
    def_irq_handler EZH_IRQHandler              /* 19 EZH/EDM */
    def_irq_handler FLASH_EEPROM_IRQHandler     /* 20 Reserved for Typhoon */
    def_irq_handler ETH_IRQHandler              /* 21 Ethernet */
    def_irq_handler SDIO_IRQHandler             /* 22 SD/MMC */
    def_irq_handler LCD_IRQHandler              /* 23 LCD */
    def_irq_handler USB0_IRQHandler             /* 24 USB0 */
    def_irq_handler USB1_IRQHandler             /* 25 USB1 */
    def_irq_handler SCT_IRQHandler              /* 26 State Configurable Timer */
    def_irq_handler RIT_IRQHandler              /* 27 Repetitive Interrupt Timer */
    def_irq_handler TIMER0_IRQHandler           /* 28 Timer0 */
    def_irq_handler TIMER1_IRQHandler           /* 29 Timer1 */
    def_irq_handler TIMER2_IRQHandler           /* 30 Timer2 */
    def_irq_handler TIMER3_IRQHandler           /* 31 Timer3 */
    def_irq_handler MCPWM_IRQHandler            /* 32 Motor Control PWM */
    def_irq_handler ADC0_IRQHandler             /* 33 A/D Converter 0 */
    def_irq_handler I2C0_IRQHandler             /* 34 I2C0 */
    def_irq_handler I2C1_IRQHandler             /* 35 I2C1 */
    def_irq_handler SPI_IRQHandler              /* 36 SPI */
    def_irq_handler ADC1_IRQHandler             /* 37 A/D Converter 1 */
    def_irq_handler SSP0_IRQHandler             /* 38 SSP0 */
    def_irq_handler SSP1_IRQHandler             /* 39 SSP1 */
    def_irq_handler UART0_IRQHandler            /* 40 UART0 */
    def_irq_handler UART1_IRQHandler            /* 41 UART1 */
    def_irq_handler UART2_IRQHandler            /* 42 UART2 */
    def_irq_handler UART3_IRQHandler            /* 43 UART3 */
    def_irq_handler I2S0_IRQHandler             /* 44 I2S0 */
    def_irq_handler I2S1_IRQHandler             /* 45 I2S1 */
    def_irq_handler SPIFI_IRQHandler            /* 46 SPI Flash Interface */
    def_irq_handler SGPIO_IRQHandler            /* 47 SGPIO */
    def_irq_handler GPIO0_IRQHandler            /* 48 GPIO0 */
    def_irq_handler GPIO1_IRQHandler            /* 49 GPIO1 */
    def_irq_handler GPIO2_IRQHandler            /* 50 GPIO2 */
    def_irq_handler GPIO3_IRQHandler            /* 51 GPIO3 */
    def_irq_handler GPIO4_IRQHandler            /* 52 GPIO4 */
    def_irq_handler GPIO5_IRQHandler            /* 53 GPIO5 */
    def_irq_handler GPIO6_IRQHandler            /* 54 GPIO6 */
    def_irq_handler GPIO7_IRQHandler            /* 55 GPIO7 */
    def_irq_handler GINT0_IRQHandler            /* 56 GINT0 */
    def_irq_handler GINT1_IRQHandler            /* 57 GINT1 */
    def_irq_handler EVRT_IRQHandler             /* 58 Event Router */
    def_irq_handler CAN1_IRQHandler             /* 59 C_CAN1 */
    def_irq_handler VADC_IRQHandler             /* 61 VADC */
    def_irq_handler ATIMER_IRQHandler           /* 62 ATIMER */
    def_irq_handler RTC_IRQHandler              /* 63 RTC */
    def_irq_handler WDT_IRQHandler              /* 65 WDT */
    def_irq_handler M0s_IRQHandler              /* 66 M0s */
    def_irq_handler CAN0_IRQHandler             /* 67 C_CAN0 */
    def_irq_handler QEI_IRQHandler              /* 68 QEI */

    .end
