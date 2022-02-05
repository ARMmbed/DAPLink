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
    .long   WWDG_IRQHandler            /* Window Watchdog */
    .long   PVD_IRQHandler             /* PVD through EXTI Line detect */
    .long   TAMPER_IRQHandler          /* Tamper */
    .long   RTC_IRQHandler             /* RTC */
    .long   FLASH_IRQHandler           /* Flash */
    .long   RCC_IRQHandler             /* RCC */
    .long   EXTI0_IRQHandler           /* EXTI Line 0 */
    .long   EXTI1_IRQHandler           /* EXTI Line 1 */
    .long   EXTI2_IRQHandler           /* EXTI Line 2 */
    .long   EXTI3_IRQHandler           /* EXTI Line 3 */
    .long   EXTI4_IRQHandler           /* EXTI Line 4 */
    .long   DMA1_Channel1_IRQHandler   /* DMA1 Channel 1 */
    .long   DMA1_Channel2_IRQHandler   /* DMA1 Channel 2 */
    .long   DMA1_Channel3_IRQHandler   /* DMA1 Channel 3 */
    .long   DMA1_Channel4_IRQHandler   /* DMA1 Channel 4 */
    .long   DMA1_Channel5_IRQHandler   /* DMA1 Channel 5 */
    .long   DMA1_Channel6_IRQHandler   /* DMA1 Channel 6 */
    .long   DMA1_Channel7_IRQHandler   /* DMA1 Channel 7 */
    .long   ADC1_2_IRQHandler          /* ADC1_2 */
    .long   USB_HP_CAN1_TX_IRQHandler  /* USB High Priority or CAN1 TX */
    .long   USB_LP_CAN1_RX0_IRQHandler /* USB Low  Priority or CAN1 RX0 */
    .long   CAN1_RX1_IRQHandler        /* CAN1 RX1 */
    .long   CAN1_SCE_IRQHandler        /* CAN1 SCE */
    .long   EXTI9_5_IRQHandler         /* EXTI Line 9..5 */
    .long   TIM1_BRK_IRQHandler        /* TIM1 Break */
    .long   TIM1_UP_IRQHandler         /* TIM1 Update */
    .long   TIM1_TRG_COM_IRQHandler    /* TIM1 Trigger and Commutation */
    .long   TIM1_CC_IRQHandler         /* TIM1 Capture Compare */
    .long   TIM2_IRQHandler            /* TIM2 */
    .long   TIM3_IRQHandler            /* TIM3 */
    .long   TIM4_IRQHandler            /* TIM4 */
    .long   I2C1_EV_IRQHandler         /* I2C1 Event */
    .long   I2C1_ER_IRQHandler         /* I2C1 Error */
    .long   I2C2_EV_IRQHandler         /* I2C2 Event */
    .long   I2C2_ER_IRQHandler         /* I2C2 Error */
    .long   SPI1_IRQHandler            /* SPI1 */
    .long   SPI2_IRQHandler            /* SPI2 */
    .long   USART1_IRQHandler          /* USART1 */
    .long   USART2_IRQHandler          /* USART2 */
    .long   USART3_IRQHandler          /* USART3 */
    .long   EXTI15_10_IRQHandler       /* EXTI Line 15..10 */
    .long   RTC_Alarm_IRQHandler        /* RTC Alarm through EXTI Line */
    .long   USBWakeUp_IRQHandler       /* USB Wakeup from suspend */

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
    def_irq_handler WWDG_IRQHandler            /* Window Watchdog */
    def_irq_handler PVD_IRQHandler             /* PVD through EXTI Line detect */
    def_irq_handler TAMPER_IRQHandler          /* Tamper */
    def_irq_handler RTC_IRQHandler             /* RTC */
    def_irq_handler FLASH_IRQHandler           /* Flash */
    def_irq_handler RCC_IRQHandler             /* RCC */
    def_irq_handler EXTI0_IRQHandler           /* EXTI Line 0 */
    def_irq_handler EXTI1_IRQHandler           /* EXTI Line 1 */
    def_irq_handler EXTI2_IRQHandler           /* EXTI Line 2 */
    def_irq_handler EXTI3_IRQHandler           /* EXTI Line 3 */
    def_irq_handler EXTI4_IRQHandler           /* EXTI Line 4 */
    def_irq_handler DMA1_Channel1_IRQHandler   /* DMA1 Channel 1 */
    def_irq_handler DMA1_Channel2_IRQHandler   /* DMA1 Channel 2 */
    def_irq_handler DMA1_Channel3_IRQHandler   /* DMA1 Channel 3 */
    def_irq_handler DMA1_Channel4_IRQHandler   /* DMA1 Channel 4 */
    def_irq_handler DMA1_Channel5_IRQHandler   /* DMA1 Channel 5 */
    def_irq_handler DMA1_Channel6_IRQHandler   /* DMA1 Channel 6 */
    def_irq_handler DMA1_Channel7_IRQHandler   /* DMA1 Channel 7 */
    def_irq_handler ADC1_2_IRQHandler          /* ADC1_2 */
    def_irq_handler USB_HP_CAN1_TX_IRQHandler  /* USB High Priority or CAN1 TX */
    def_irq_handler USB_LP_CAN1_RX0_IRQHandler /* USB Low  Priority or CAN1 RX0 */
    def_irq_handler CAN1_RX1_IRQHandler        /* CAN1 RX1 */
    def_irq_handler CAN1_SCE_IRQHandler        /* CAN1 SCE */
    def_irq_handler EXTI9_5_IRQHandler         /* EXTI Line 9..5 */
    def_irq_handler TIM1_BRK_IRQHandler        /* TIM1 Break */
    def_irq_handler TIM1_UP_IRQHandler         /* TIM1 Update */
    def_irq_handler TIM1_TRG_COM_IRQHandler    /* TIM1 Trigger and Commutation */
    def_irq_handler TIM1_CC_IRQHandler         /* TIM1 Capture Compare */
    def_irq_handler TIM2_IRQHandler            /* TIM2 */
    def_irq_handler TIM3_IRQHandler            /* TIM3 */
    def_irq_handler TIM4_IRQHandler            /* TIM4 */
    def_irq_handler I2C1_EV_IRQHandler         /* I2C1 Event */
    def_irq_handler I2C1_ER_IRQHandler         /* I2C1 Error */
    def_irq_handler I2C2_EV_IRQHandler         /* I2C2 Event */
    def_irq_handler I2C2_ER_IRQHandler         /* I2C2 Error */
    def_irq_handler SPI1_IRQHandler            /* SPI1 */
    def_irq_handler SPI2_IRQHandler            /* SPI2 */
    def_irq_handler USART1_IRQHandler          /* USART1 */
    def_irq_handler USART2_IRQHandler          /* USART2 */
    def_irq_handler USART3_IRQHandler          /* USART3 */
    def_irq_handler EXTI15_10_IRQHandler       /* EXTI Line 15..10 */
    def_irq_handler RTC_Alarm_IRQHandler        /* RTC Alarm through EXTI Line */
    def_irq_handler USBWakeUp_IRQHandler       /* USB Wakeup from suspend */

    .end
