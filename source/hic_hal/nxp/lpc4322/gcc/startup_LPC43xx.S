/**
 * @file    startup_MK20D5.s
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 1997 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 - 2017 NXP
 * Copyright (c) 2009-2021, Arm Limited, All Rights Reserved
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
    .long   __StackTop                  /* Top of Stack */
    .long   Reset_Handler               /* Reset Handler */
    .long   NMI_Handler                 /* NMI Handler */
    .long   HardFault_Handler           /* Hard Fault Handler */
    .long   MemManage_Handler           /* MPU Fault Handler */
    .long   BusFault_Handler            /* Bus Fault Handler */
    .long   UsageFault_Handler          /* Usage Fault Handler */
    .long   0x5A5A5A5A                  /* Reserved */
    .long   DAPLINK_BUILD_KEY           /* DAPLINK: Build type (BL/IF) */
    .long   DAPLINK_HIC_ID              /* DAPLINK: Compatibility */
    .long   DAPLINK_VERSION             /* DAPLINK: Version */
    .long   SVC_Handler                 /* SVCall Handler */
    .long   DebugMon_Handler            /* Debug Monitor Handler */
    .long   g_board_info                /* DAPLINK: Pointer to board/family/target info */
    .long   PendSV_Handler              /* PendSV Handler */
    .long   SysTick_Handler             /* SysTick Handler */

    /* External LPC43xx/M4 Interrupts */
    .long   DAC_IRQHandler              /*  0  DAC interrupt */
    .long   M0APP_IRQHandler            /*  1  Cortex-M0APP; Latched TXEV; for M4-M0APP communication */
    .long   DMA_IRQHandler              /*  2  DMA interrupt */
    .long   0                           /*  3  Reserved */
    .long   FLASHEEPROM_IRQHandler      /*  4  flash bank A, flash bank B, EEPROM ORed interrupt */
    .long   ETHERNET_IRQHandler         /*  5  Ethernet interrupt */
    .long   SDIO_IRQHandler             /*  6  SD/MMC interrupt */
    .long   LCD_IRQHandler              /*  7  LCD interrupt */
    .long   USB0_IRQHandler             /*  8  OTG interrupt */
    .long   USB1_IRQHandler             /*  9  USB1 interrupt */
    .long   SCT_IRQHandler              /* 10  SCT combined interrupt */
    .long   RITIMER_IRQHandler          /* 11  RI Timer interrupt */
    .long   TIMER0_IRQHandler           /* 12  Timer 0 interrupt */
    .long   TIMER1_IRQHandler           /* 13  Timer 1 interrupt */
    .long   TIMER2_IRQHandler           /* 14  Timer 2 interrupt */
    .long   TIMER3_IRQHandler           /* 15  Timer 3 interrupt */
    .long   MCPWM_IRQHandler            /* 16  Motor control PWM interrupt */
    .long   ADC0_IRQHandler             /* 17  ADC0 interrupt */
    .long   I2C0_IRQHandler             /* 18  I2C0 interrupt */
    .long   I2C1_IRQHandler             /* 19  I2C1 interrupt */
    .long   SPI_IRQHandler              /* 20  SPI interrupt */
    .long   ADC1_IRQHandler             /* 21  ADC1 interrupt */
    .long   SSP0_IRQHandler             /* 22  SSP0 interrupt */
    .long   SSP1_IRQHandler             /* 23  SSP1 interrupt */
    .long   USART0_IRQHandler           /* 24  USART0 interrupt */
    .long   UART1_IRQHandler            /* 25  Combined UART1, Modem interrupt */
    .long   USART2_IRQHandler           /* 26  USART2 interrupt */
    .long   USART3_IRQHandler           /* 27  Combined USART3, IrDA interrupt */
    .long   I2S0_IRQHandler             /* 28  I2S0 interrupt */
    .long   I2S1_IRQHandler             /* 29  I2S1 interrupt */
    .long   SPIFI_IRQHandler            /* 30  SPISI interrupt */
    .long   SGPIO_IRQHandler            /* 31  SGPIO interrupt */
    .long   PIN_INT0_IRQHandler         /* 32  GPIO pin interrupt 0 */
    .long   PIN_INT1_IRQHandler         /* 33  GPIO pin interrupt 1 */
    .long   PIN_INT2_IRQHandler         /* 34  GPIO pin interrupt 2 */
    .long   PIN_INT3_IRQHandler         /* 35  GPIO pin interrupt 3 */
    .long   PIN_INT4_IRQHandler         /* 36  GPIO pin interrupt 4 */
    .long   PIN_INT5_IRQHandler         /* 37  GPIO pin interrupt 5 */
    .long   PIN_INT6_IRQHandler         /* 38  GPIO pin interrupt 6 */
    .long   PIN_INT7_IRQHandler         /* 39  GPIO pin interrupt 7 */
    .long   GINT0_IRQHandler            /* 40  GPIO global interrupt 0 */
    .long   GINT1_IRQHandler            /* 41  GPIO global interrupt 1 */
    .long   EVENTROUTER_IRQHandler      /* 42  Event router interrupt */
    .long   C_CAN1_IRQHandler           /* 43  C_CAN1 interrupt */
    .long   0                           /* 44  Reserved */
    .long   ADCHS_IRQHandler            /* 45  ADCHS combined interrupt */
    .long   ATIMER_IRQHandler           /* 46  Alarm timer interrupt */
    .long   RTC_IRQHandler              /* 47  RTC interrupt */
    .long   0                           /* 48  Reserved */
    .long   WWDT_IRQHandler             /* 49  WWDT interrupt */
    .long   M0SUB_IRQHandler            /* 50  TXEV instruction from the M0 subsystem core interrupt */
    .long   C_CAN0_IRQHandler           /* 51  C_CAN0 interrupt */
    .long   QEI_IRQHandler              /* 52  QEI interrupt */

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
    def_irq_handler  DAC_IRQHandler             /* D/A Converter */
    def_irq_handler  M0APP_IRQHandler           /* M0 Core */
    def_irq_handler  DMA_IRQHandler             /* General Purpose DMA */
    def_irq_handler  FLASHEEPROM_IRQHandler     /* EZH/EDM */
    def_irq_handler  ETHERNET_IRQHandler        /* Ethernet */
    def_irq_handler  SDIO_IRQHandler            /* SD/MMC */
    def_irq_handler  LCD_IRQHandler             /* LCD */
    def_irq_handler  USB0_IRQHandler            /* USB0 */
    def_irq_handler  USB1_IRQHandler            /* USB1 */
    def_irq_handler  SCT_IRQHandler             /* State Configurable Timer */
    def_irq_handler  RITIMER_IRQHandler         /* Repetitive Interrupt Timer */
    def_irq_handler  TIMER0_IRQHandler          /* Timer0 */
    def_irq_handler  TIMER1_IRQHandler          /* Timer1 */
    def_irq_handler  TIMER2_IRQHandler          /* Timer2 */
    def_irq_handler  TIMER3_IRQHandler          /* Timer3 */
    def_irq_handler  MCPWM_IRQHandler           /* Motor Control PWM */
    def_irq_handler  ADC0_IRQHandler            /* A/D Converter 0 */
    def_irq_handler  I2C0_IRQHandler            /* I2C0 */
    def_irq_handler  I2C1_IRQHandler            /* I2C1 */
    def_irq_handler  SPI_IRQHandler             /* SPI */
    def_irq_handler  ADC1_IRQHandler            /* A/D Converter 1 */
    def_irq_handler  SSP0_IRQHandler            /* SSP0 */
    def_irq_handler  SSP1_IRQHandler            /* SSP1 */
    def_irq_handler  USART0_IRQHandler          /* UART0 */
    def_irq_handler  UART1_IRQHandler           /* UART1 */
    def_irq_handler  USART2_IRQHandler          /* UART2 */
    def_irq_handler  USART3_IRQHandler          /* UART3 */
    def_irq_handler  I2S0_IRQHandler            /* I2S0 */
    def_irq_handler  I2S1_IRQHandler            /* I2S1 */
    def_irq_handler  SPIFI_IRQHandler           /* SPI Flash Interface */
    def_irq_handler  SGPIO_IRQHandler           /* SGPIO */
    def_irq_handler  PIN_INT0_IRQHandler        /* GPIO0 */
    def_irq_handler  PIN_INT1_IRQHandler        /* GPIO1 */
    def_irq_handler  PIN_INT2_IRQHandler        /* GPIO2 */
    def_irq_handler  PIN_INT3_IRQHandler        /* GPIO3 */
    def_irq_handler  PIN_INT4_IRQHandler        /* GPIO4 */
    def_irq_handler  PIN_INT5_IRQHandler        /* GPIO5 */
    def_irq_handler  PIN_INT6_IRQHandler        /* GPIO6 */
    def_irq_handler  PIN_INT7_IRQHandler        /* GPIO7 */
    def_irq_handler  GINT0_IRQHandler           /* GINT0 */
    def_irq_handler  GINT1_IRQHandler           /* GINT1 */
    def_irq_handler  EVENTROUTER_IRQHandler     /* Event Router */
    def_irq_handler  C_CAN1_IRQHandler          /* C_CAN1 */
    def_irq_handler  ADCHS_IRQHandler           /* VADC */
    def_irq_handler  ATIMER_IRQHandler          /* ATIMER */
    def_irq_handler  RTC_IRQHandler             /* RTC */
    def_irq_handler  WWDT_IRQHandler            /* WDT */
    def_irq_handler  M0SUB_IRQHandler           /* M0s */
    def_irq_handler  C_CAN0_IRQHandler          /* C_CAN0 */
    def_irq_handler  QEI_IRQHandler             /* QEI */

    .end
