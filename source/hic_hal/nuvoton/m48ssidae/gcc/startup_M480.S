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
    .long   DAPLINK_BUILD_KEY                               /* Build type - BL/IF*/
    .long   DAPLINK_HIC_ID                                  /* Compatibility*/
    .long   DAPLINK_VERSION                                 /* Version*/
    .long   SVC_Handler                                     /* SVCall Handler*/
    .long   DebugMon_Handler                                /* Debug Monitor Handler*/
    .long   g_board_info                                    /* Ptr to Board info, family info other target details*/
    .long   PendSV_Handler                                  /* PendSV Handler*/
    .long   SysTick_Handler                                 /* SysTick Handler*/

    /* External Interrupts*/
    .long   BOD_IRQHandler            /* 0: Brown Out detection */
    .long   IRC_IRQHandler            /* 1: Internal RC */
    .long   PWRWU_IRQHandler          /* 2: Power down wake up */
    .long   RAMPE_IRQHandler          /* 3: RAM parity error */
    .long   CKFAIL_IRQHandler         /* 4: Clock detection fail */
    .long   Reserved0_Handler           /* 5: Reserved */
    .long   RTC_IRQHandler            /* 6: Real Time Clock */
    .long   TAMPER_IRQHandler         /* 7: Tamper detection */
    .long   WDT_IRQHandler            /* 8: Watchdog timer */
    .long   WWDT_IRQHandler           /* 9: Window watchdog timer */
    .long   EINT0_IRQHandler          /* 10: External Input 0 */
    .long   EINT1_IRQHandler          /* 11: External Input 1 */
    .long   EINT2_IRQHandler          /* 12: External Input 2 */
    .long   EINT3_IRQHandler          /* 13: External Input 3 */
    .long   EINT4_IRQHandler          /* 14: External Input 4 */
    .long   EINT5_IRQHandler          /* 15: External Input 5 */
    .long   GPA_IRQHandler            /* 16: GPIO Port A */
    .long   GPB_IRQHandler            /* 17: GPIO Port B */
    .long   GPC_IRQHandler            /* 18: GPIO Port C */
    .long   GPD_IRQHandler            /* 19: GPIO Port D */
    .long   GPE_IRQHandler            /* 20: GPIO Port E */
    .long   GPF_IRQHandler            /* 21: GPIO Port F */
    .long   QSPI0_IRQHandler          /* 22: QSPI0 */
    .long   SPI0_IRQHandler           /* 23: SPI0 */
    .long   BRAKE0_IRQHandler         /* 24: */
    .long   EPWM0P0_IRQHandler        /* 25: */
    .long   EPWM0P1_IRQHandler        /* 26: */
    .long   EPWM0P2_IRQHandler        /* 27: */
    .long   BRAKE1_IRQHandler         /* 28: */
    .long   EPWM1P0_IRQHandler        /* 29: */
    .long   EPWM1P1_IRQHandler        /* 30: */
    .long   EPWM1P2_IRQHandler        /* 31: */
    .long   TMR0_IRQHandler           /* 32: Timer 0 */
    .long   TMR1_IRQHandler           /* 33: Timer 1 */
    .long   TMR2_IRQHandler           /* 34: Timer 2 */
    .long   TMR3_IRQHandler           /* 35: Timer 3 */
    .long   UART0_IRQHandler          /* 36: UART0 */
    .long   UART1_IRQHandler          /* 37: UART1 */
    .long   I2C0_IRQHandler           /* 38: I2C0 */
    .long   I2C1_IRQHandler           /* 39: I2C1 */
    .long   PDMA_IRQHandler           /* 40: Peripheral DMA */
    .long   DAC_IRQHandler            /* 41: DAC */
    .long   ADC00_IRQHandler          /* 42: ADC0 interrupt source 0 */
    .long   ADC01_IRQHandler          /* 43: ADC0 interrupt source 1 */
    .long   ACMP01_IRQHandler         /* 44: ACMP0 and ACMP1 */
    .long   Reserved1_Handler           /* 45: Reserved */
    .long   ADC02_IRQHandler          /* 46: ADC0 interrupt source 2 */
    .long   ADC03_IRQHandler          /* 47: ADC0 interrupt source 3 */
    .long   UART2_IRQHandler          /* 48: UART2 */
    .long   UART3_IRQHandler          /* 49: UART3 */
    .long   Reserved2_Handler           /* 50: Reserved */
    .long   SPI1_IRQHandler           /* 51: SPI1 */
    .long   SPI2_IRQHandler           /* 52: SPI2 */
    .long   USBD_IRQHandler           /* 53: USB device */
    .long   OHCI_IRQHandler           /* 54: OHCI */
    .long   USBOTG_IRQHandler         /* 55: USB OTG */
    .long   CAN0_IRQHandler           /* 56: CAN0 */
    .long   CAN1_IRQHandler           /* 57: CAN1 */
    .long   SC0_IRQHandler            /* 58: */
    .long   SC1_IRQHandler            /* 59: */
    .long   SC2_IRQHandler            /* 60: */
    .long   Reserved3_Handler           /* 61: */
    .long   SPI3_IRQHandler           /* 62: SPI3 */
    .long   Reserved4_Handler           /* 63: */
    .long   SDH0_IRQHandler           /* 64: SDH0 */
    .long   USBD20_IRQHandler         /* 65: USBD20 */
    .long   EMAC_TX_IRQHandler        /* 66: EMAC_TX */
    .long   EMAC_RX_IRQHandler        /* 67: EMAX_RX */
    .long   I2S0_IRQHandler           /* 68: I2S0 */
    .long   Reserved5_Handler           /* 69: ToDo: Add description to this Interrupt */
    .long   OPA0_IRQHandler           /* 70: OPA0 */
    .long   CRYPTO_IRQHandler         /* 71: CRYPTO */
    .long   GPG_IRQHandler            /* 72: */
    .long   EINT6_IRQHandler          /* 73: */
    .long   UART4_IRQHandler          /* 74: UART4 */
    .long   UART5_IRQHandler          /* 75: UART5 */
    .long   USCI0_IRQHandler          /* 76: USCI0 */
    .long   USCI1_IRQHandler          /* 77: USCI1 */
    .long   BPWM0_IRQHandler          /* 78: BPWM0 */
    .long   BPWM1_IRQHandler          /* 79: BPWM1 */
    .long   SPIM_IRQHandler           /* 80: SPIM */
    .long   Reserved6_Handler           /* 81: ToDo: Add description to this Interrupt */
    .long   I2C2_IRQHandler           /* 82: I2C2 */
    .long   Reserved7_Handler           /* 83: */
    .long   QEI0_IRQHandler           /* 84: QEI0 */
    .long   QEI1_IRQHandler           /* 85: QEI1 */
    .long   ECAP0_IRQHandler          /* 86: ECAP0 */
    .long   ECAP1_IRQHandler          /* 87: ECAP1 */
    .long   GPH_IRQHandler            /* 88: */
    .long   EINT7_IRQHandler          /* 89: */
    .long   SDH1_IRQHandler           /* 90: SDH1 */
    .long   Reserved8_Handler           /* 91: */
    .long   EHCI_IRQHandler           /* 92: EHCI */
    .long   USBOTG20_IRQHandler       /* 93: */

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

    /* Unlock Register */
    ldr     r0, =0x40000100
    ldr     r1, =0x59
    str     r1, [r0]
    ldr     r1, =0x16
    str     r1, [r0]
    ldr     r1, =0x88
    str     r1, [r0]

#if !defined(ENABLE_SPIM_CACHE)
    ldr     r0, =0x40000200            /* R0 = Clock Controller Register Base Address */
    ldr     r1, [r0,#0x4]              /* R1 = 0x40000204  (AHBCLK) */
    orr     r1, r1, #0x4000
    str     r1, [r0,#0x4]              /* CLK->AHBCLK |= CLK_AHBCLK_SPIMCKEN_Msk; */

    ldr     r0, =0x40007000            /* R0 = SPIM Register Base Address */
    ldr     r1, [r0,#4]                /* R1 = SPIM->CTL1 */
    orr     r1, r1,#2                  /* R1 |= SPIM_CTL1_CACHEOFF_Msk */
    str     r1, [r0,#4]                /* _SPIM_DISABLE_CACHE() */
    ldr     r1, [r0,#4]                /* R1 = SPIM->CTL1 */
    orr     r1, r1, #4                 /* R1 |= SPIM_CTL1_CCMEN_Msk */
    str     r1, [r0,#4]                /* _SPIM_ENABLE_CCM() */
#endif

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

    /* Lock */
    ldr     r0, =0x40000100
    ldr     r1, =0
    str     r1, [r0]

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
    def_irq_handler BOD_IRQHandler            /* 0: Brown Out detection */
    def_irq_handler IRC_IRQHandler            /* 1: Internal RC */
    def_irq_handler PWRWU_IRQHandler          /* 2: Power down wake up */
    def_irq_handler RAMPE_IRQHandler          /* 3: RAM parity error */
    def_irq_handler CKFAIL_IRQHandler         /* 4: Clock detection fail */
    def_irq_handler Reserved0_Handler           /* 5: Reserved */
    def_irq_handler RTC_IRQHandler            /* 6: Real Time Clock */
    def_irq_handler TAMPER_IRQHandler         /* 7: Tamper detection */
    def_irq_handler WDT_IRQHandler            /* 8: Watchdog timer */
    def_irq_handler WWDT_IRQHandler           /* 9: Window watchdog timer */
    def_irq_handler EINT0_IRQHandler          /* 10: External Input 0 */
    def_irq_handler EINT1_IRQHandler          /* 11: External Input 1 */
    def_irq_handler EINT2_IRQHandler          /* 12: External Input 2 */
    def_irq_handler EINT3_IRQHandler          /* 13: External Input 3 */
    def_irq_handler EINT4_IRQHandler          /* 14: External Input 4 */
    def_irq_handler EINT5_IRQHandler          /* 15: External Input 5 */
    def_irq_handler GPA_IRQHandler            /* 16: GPIO Port A */
    def_irq_handler GPB_IRQHandler            /* 17: GPIO Port B */
    def_irq_handler GPC_IRQHandler            /* 18: GPIO Port C */
    def_irq_handler GPD_IRQHandler            /* 19: GPIO Port D */
    def_irq_handler GPE_IRQHandler            /* 20: GPIO Port E */
    def_irq_handler GPF_IRQHandler            /* 21: GPIO Port F */
    def_irq_handler QSPI0_IRQHandler          /* 22: QSPI0 */
    def_irq_handler SPI0_IRQHandler           /* 23: SPI0 */
    def_irq_handler BRAKE0_IRQHandler         /* 24: */
    def_irq_handler EPWM0P0_IRQHandler        /* 25: */
    def_irq_handler EPWM0P1_IRQHandler        /* 26: */
    def_irq_handler EPWM0P2_IRQHandler        /* 27: */
    def_irq_handler BRAKE1_IRQHandler         /* 28: */
    def_irq_handler EPWM1P0_IRQHandler        /* 29: */
    def_irq_handler EPWM1P1_IRQHandler        /* 30: */
    def_irq_handler EPWM1P2_IRQHandler        /* 31: */
    def_irq_handler TMR0_IRQHandler           /* 32: Timer 0 */
    def_irq_handler TMR1_IRQHandler           /* 33: Timer 1 */
    def_irq_handler TMR2_IRQHandler           /* 34: Timer 2 */
    def_irq_handler TMR3_IRQHandler           /* 35: Timer 3 */
    def_irq_handler UART0_IRQHandler          /* 36: UART0 */
    def_irq_handler UART1_IRQHandler          /* 37: UART1 */
    def_irq_handler I2C0_IRQHandler           /* 38: I2C0 */
    def_irq_handler I2C1_IRQHandler           /* 39: I2C1 */
    def_irq_handler PDMA_IRQHandler           /* 40: Peripheral DMA */
    def_irq_handler DAC_IRQHandler            /* 41: DAC */
    def_irq_handler ADC00_IRQHandler          /* 42: ADC0 interrupt source 0 */
    def_irq_handler ADC01_IRQHandler          /* 43: ADC0 interrupt source 1 */
    def_irq_handler ACMP01_IRQHandler         /* 44: ACMP0 and ACMP1 */
    def_irq_handler Reserved1_Handler           /* 45: Reserved */
    def_irq_handler ADC02_IRQHandler          /* 46: ADC0 interrupt source 2 */
    def_irq_handler ADC03_IRQHandler          /* 47: ADC0 interrupt source 3 */
    def_irq_handler UART2_IRQHandler          /* 48: UART2 */
    def_irq_handler UART3_IRQHandler          /* 49: UART3 */
    def_irq_handler Reserved2_Handler           /* 50: Reserved */
    def_irq_handler SPI1_IRQHandler           /* 51: SPI1 */
    def_irq_handler SPI2_IRQHandler           /* 52: SPI2 */
    def_irq_handler USBD_IRQHandler           /* 53: USB device */
    def_irq_handler OHCI_IRQHandler           /* 54: OHCI */
    def_irq_handler USBOTG_IRQHandler         /* 55: USB OTG */
    def_irq_handler CAN0_IRQHandler           /* 56: CAN0 */
    def_irq_handler CAN1_IRQHandler           /* 57: CAN1 */
    def_irq_handler SC0_IRQHandler            /* 58: */
    def_irq_handler SC1_IRQHandler            /* 59: */
    def_irq_handler SC2_IRQHandler            /* 60: */
    def_irq_handler Reserved3_Handler           /* 61: */
    def_irq_handler SPI3_IRQHandler           /* 62: SPI3 */
    def_irq_handler Reserved4_Handler           /* 63: */
    def_irq_handler SDH0_IRQHandler           /* 64: SDH0 */
    def_irq_handler USBD20_IRQHandler         /* 65: USBD20 */
    def_irq_handler EMAC_TX_IRQHandler        /* 66: EMAC_TX */
    def_irq_handler EMAC_RX_IRQHandler        /* 67: EMAX_RX */
    def_irq_handler I2S0_IRQHandler           /* 68: I2S0 */
    def_irq_handler Reserved5_Handler           /* 69: ToDo: Add description to this Interrupt */
    def_irq_handler OPA0_IRQHandler           /* 70: OPA0 */
    def_irq_handler CRYPTO_IRQHandler         /* 71: CRYPTO */
    def_irq_handler GPG_IRQHandler            /* 72: */
    def_irq_handler EINT6_IRQHandler          /* 73: */
    def_irq_handler UART4_IRQHandler          /* 74: UART4 */
    def_irq_handler UART5_IRQHandler          /* 75: UART5 */
    def_irq_handler USCI0_IRQHandler          /* 76: USCI0 */
    def_irq_handler USCI1_IRQHandler          /* 77: USCI1 */
    def_irq_handler BPWM0_IRQHandler          /* 78: BPWM0 */
    def_irq_handler BPWM1_IRQHandler          /* 79: BPWM1 */
    def_irq_handler SPIM_IRQHandler           /* 80: SPIM */
    def_irq_handler Reserved6_Handler           /* 81: ToDo: Add description to this Interrupt */
    def_irq_handler I2C2_IRQHandler           /* 82: I2C2 */
    def_irq_handler Reserved7_Handler           /* 83: */
    def_irq_handler QEI0_IRQHandler           /* 84: QEI0 */
    def_irq_handler QEI1_IRQHandler           /* 85: QEI1 */
    def_irq_handler ECAP0_IRQHandler          /* 86: ECAP0 */
    def_irq_handler ECAP1_IRQHandler          /* 87: ECAP1 */
    def_irq_handler GPH_IRQHandler            /* 88: */
    def_irq_handler EINT7_IRQHandler          /* 89: */
    def_irq_handler SDH1_IRQHandler           /* 90: SDH1 */
    def_irq_handler Reserved8_Handler           /* 91: */
    def_irq_handler EHCI_IRQHandler           /* 92: EHCI */
    def_irq_handler USBOTG20_IRQHandler       /* 93: */

    .end
