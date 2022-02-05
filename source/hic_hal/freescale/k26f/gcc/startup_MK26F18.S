/* ------------------------------------------------------------------------- */
/*  @file:    startup_MK26F18.s                                              */
/*  @purpose: CMSIS Cortex-M4 Core Device Startup File                       */
/*            MK26F18                                                        */
/*  @version: 2.0                                                            */
/*  @date:    2015-3-25                                                      */
/*  @build:   b180801                                                        */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Copyright 1997-2016 Freescale Semiconductor, Inc.                         */
/* Copyright 2016-2018 NXP                                                   */
/*                                                                           */
/* SPDX-License-Identifier: BSD-3-Clause              */
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
    .long   DMA0_DMA16_IRQHandler                           /* DMA Channel 0, 16 Transfer Complete*/
    .long   DMA1_DMA17_IRQHandler                           /* DMA Channel 1, 17 Transfer Complete*/
    .long   DMA2_DMA18_IRQHandler                           /* DMA Channel 2, 18 Transfer Complete*/
    .long   DMA3_DMA19_IRQHandler                           /* DMA Channel 3, 19 Transfer Complete*/
    .long   DMA4_DMA20_IRQHandler                           /* DMA Channel 4, 20 Transfer Complete*/
    .long   DMA5_DMA21_IRQHandler                           /* DMA Channel 5, 21 Transfer Complete*/
    .long   DMA6_DMA22_IRQHandler                           /* DMA Channel 6, 22 Transfer Complete*/
    .long   DMA7_DMA23_IRQHandler                           /* DMA Channel 7, 23 Transfer Complete*/
    .long   DMA8_DMA24_IRQHandler                           /* DMA Channel 8, 24 Transfer Complete*/
    .long   DMA9_DMA25_IRQHandler                           /* DMA Channel 9, 25 Transfer Complete*/
    .long   DMA10_DMA26_IRQHandler                          /* DMA Channel 10, 26 Transfer Complete*/
    .long   DMA11_DMA27_IRQHandler                          /* DMA Channel 11, 27 Transfer Complete*/
    .long   DMA12_DMA28_IRQHandler                          /* DMA Channel 12, 28 Transfer Complete*/
    .long   DMA13_DMA29_IRQHandler                          /* DMA Channel 13, 29 Transfer Complete*/
    .long   DMA14_DMA30_IRQHandler                          /* DMA Channel 14, 30 Transfer Complete*/
    .long   DMA15_DMA31_IRQHandler                          /* DMA Channel 15, 31 Transfer Complete*/
    .long   DMA_Error_IRQHandler                            /* DMA Error Interrupt*/
    .long   MCM_IRQHandler                                  /* Normal Interrupt*/
    .long   FTFE_IRQHandler                                 /* FTFE Command complete interrupt*/
    .long   Read_Collision_IRQHandler                       /* Read Collision Interrupt*/
    .long   LVD_LVW_IRQHandler                              /* Low Voltage Detect, Low Voltage Warning*/
    .long   LLWU_IRQHandler                                 /* Low Leakage Wakeup Unit*/
    .long   WDOG_EWM_IRQHandler                             /* WDOG Interrupt*/
    .long   RNG_IRQHandler                                  /* RNG Interrupt*/
    .long   I2C0_IRQHandler                                 /* I2C0 interrupt*/
    .long   I2C1_IRQHandler                                 /* I2C1 interrupt*/
    .long   SPI0_IRQHandler                                 /* SPI0 Interrupt*/
    .long   SPI1_IRQHandler                                 /* SPI1 Interrupt*/
    .long   I2S0_Tx_IRQHandler                              /* I2S0 transmit interrupt*/
    .long   I2S0_Rx_IRQHandler                              /* I2S0 receive interrupt*/
    .long   Reserved46_IRQHandler                           /* Reserved interrupt 46*/
    .long   UART0_RX_TX_IRQHandler                          /* UART0 Receive/Transmit interrupt*/
    .long   UART0_ERR_IRQHandler                            /* UART0 Error interrupt*/
    .long   UART1_RX_TX_IRQHandler                          /* UART1 Receive/Transmit interrupt*/
    .long   UART1_ERR_IRQHandler                            /* UART1 Error interrupt*/
    .long   UART2_RX_TX_IRQHandler                          /* UART2 Receive/Transmit interrupt*/
    .long   UART2_ERR_IRQHandler                            /* UART2 Error interrupt*/
    .long   UART3_RX_TX_IRQHandler                          /* UART3 Receive/Transmit interrupt*/
    .long   UART3_ERR_IRQHandler                            /* UART3 Error interrupt*/
    .long   ADC0_IRQHandler                                 /* ADC0 interrupt*/
    .long   CMP0_IRQHandler                                 /* CMP0 interrupt*/
    .long   CMP1_IRQHandler                                 /* CMP1 interrupt*/
    .long   FTM0_IRQHandler                                 /* FTM0 fault, overflow and channels interrupt*/
    .long   FTM1_IRQHandler                                 /* FTM1 fault, overflow and channels interrupt*/
    .long   FTM2_IRQHandler                                 /* FTM2 fault, overflow and channels interrupt*/
    .long   CMT_IRQHandler                                  /* CMT interrupt*/
    .long   RTC_IRQHandler                                  /* RTC interrupt*/
    .long   RTC_Seconds_IRQHandler                          /* RTC seconds interrupt*/
    .long   PIT0_IRQHandler                                 /* PIT timer channel 0 interrupt*/
    .long   PIT1_IRQHandler                                 /* PIT timer channel 1 interrupt*/
    .long   PIT2_IRQHandler                                 /* PIT timer channel 2 interrupt*/
    .long   PIT3_IRQHandler                                 /* PIT timer channel 3 interrupt*/
    .long   PDB0_IRQHandler                                 /* PDB0 Interrupt*/
    .long   USB0_IRQHandler                                 /* USB0 interrupt*/
    .long   USBDCD_IRQHandler                               /* USBDCD Interrupt*/
    .long   Reserved71_IRQHandler                           /* Reserved interrupt 71*/
    .long   DAC0_IRQHandler                                 /* DAC0 interrupt*/
    .long   MCG_IRQHandler                                  /* MCG Interrupt*/
    .long   LPTMR0_IRQHandler                               /* LPTimer interrupt*/
    .long   PORTA_IRQHandler                                /* Port A interrupt*/
    .long   PORTB_IRQHandler                                /* Port B interrupt*/
    .long   PORTC_IRQHandler                                /* Port C interrupt*/
    .long   PORTD_IRQHandler                                /* Port D interrupt*/
    .long   PORTE_IRQHandler                                /* Port E interrupt*/
    .long   SWI_IRQHandler                                  /* Software interrupt*/
    .long   SPI2_IRQHandler                                 /* SPI2 Interrupt*/
    .long   UART4_RX_TX_IRQHandler                          /* UART4 Receive/Transmit interrupt*/
    .long   UART4_ERR_IRQHandler                            /* UART4 Error interrupt*/
    .long   Reserved84_IRQHandler                           /* Reserved interrupt 84*/
    .long   Reserved85_IRQHandler                           /* Reserved interrupt 85*/
    .long   CMP2_IRQHandler                                 /* CMP2 interrupt*/
    .long   FTM3_IRQHandler                                 /* FTM3 fault, overflow and channels interrupt*/
    .long   DAC1_IRQHandler                                 /* DAC1 interrupt*/
    .long   ADC1_IRQHandler                                 /* ADC1 interrupt*/
    .long   I2C2_IRQHandler                                 /* I2C2 interrupt*/
    .long   CAN0_ORed_Message_buffer_IRQHandler             /* CAN0 OR'd message buffers interrupt*/
    .long   CAN0_Bus_Off_IRQHandler                         /* CAN0 bus off interrupt*/
    .long   CAN0_Error_IRQHandler                           /* CAN0 error interrupt*/
    .long   CAN0_Tx_Warning_IRQHandler                      /* CAN0 Tx warning interrupt*/
    .long   CAN0_Rx_Warning_IRQHandler                      /* CAN0 Rx warning interrupt*/
    .long   CAN0_Wake_Up_IRQHandler                         /* CAN0 wake up interrupt*/
    .long   SDHC_IRQHandler                                 /* SDHC interrupt*/
    .long   Reserved98_IRQHandler                           /* Reserved Interrupt 98*/
    .long   Reserved99_IRQHandler                           /* Reserved Interrupt 99*/
    .long   Reserved100_IRQHandler                          /* Reserved Interrupt 100*/
    .long   Reserved101_IRQHandler                          /* Reserved Interrupt 101*/
    .long   LPUART0_IRQHandler                              /* LPUART0 status/error interrupt*/
    .long   TSI0_IRQHandler                                 /* TSI0 interrupt*/
    .long   TPM1_IRQHandler                                 /* TPM1 fault, overflow and channels interrupt*/
    .long   TPM2_IRQHandler                                 /* TPM2 fault, overflow and channels interrupt*/
    .long   USBHSDCD_IRQHandler                             /* USBHSDCD, USBHS Phy Interrupt*/
    .long   I2C3_IRQHandler                                 /* I2C3 interrupt*/
    .long   CMP3_IRQHandler                                 /* CMP3 interrupt*/
    .long   USBHS_IRQHandler                                /* USB high speed OTG interrupt*/
    .long   CAN1_ORed_Message_buffer_IRQHandler             /* CAN1 OR'd message buffers interrupt*/
    .long   CAN1_Bus_Off_IRQHandler                         /* CAN1 bus off interrupt*/
    .long   CAN1_Error_IRQHandler                           /* CAN1 error interrupt*/
    .long   CAN1_Tx_Warning_IRQHandler                      /* CAN1 Tx warning interrupt*/
    .long   CAN1_Rx_Warning_IRQHandler                      /* CAN1 Rx warning interrupt*/
    .long   CAN1_Wake_Up_IRQHandler                         /* CAN1 wake up interrupt*/

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
 *      __noncachedata_start__/__noncachedata_end__ : none cachable region
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
#ifdef __STARTUP_INITIALIZE_NONCACHEDATA
    ldr    r2, =__noncachedata_start__
    ldr    r3, =__noncachedata_init_end__
#if 1
.LC2:
    cmp     r2, r3
    ittt    lt
    ldrlt   r0, [r1], #4
    strlt   r0, [r2], #4
    blt    .LC2
#else
    subs    r3, r2
    ble    .LC3
.LC2:
    subs    r3, #4
    ldr    r0, [r1, r3]
    str    r0, [r2, r3]
    bgt    .LC2
.LC3:
#endif
/* zero inited ncache section initialization */
    ldr r3, =__noncachedata_end__
    movs    r0,0
.LC4:
    cmp    r2,r3
    itt    lt
    strlt   r0,[r2],#4
    blt    .LC4
#endif /* __STARTUP_INITIALIZE_NONCACHEDATA */

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
.LC5:
    cmp     r1, r2
    itt    lt
    strlt   r0, [r1], #4
    blt    .LC5
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
    def_irq_handler    MemManage_Handler
    def_irq_handler    BusFault_Handler
    def_irq_handler    UsageFault_Handler
    def_irq_handler    DebugMon_Handler
    def_irq_handler    DMA0_DMA16_IRQHandler
    def_irq_handler    DMA1_DMA17_IRQHandler
    def_irq_handler    DMA2_DMA18_IRQHandler
    def_irq_handler    DMA3_DMA19_IRQHandler
    def_irq_handler    DMA4_DMA20_IRQHandler
    def_irq_handler    DMA5_DMA21_IRQHandler
    def_irq_handler    DMA6_DMA22_IRQHandler
    def_irq_handler    DMA7_DMA23_IRQHandler
    def_irq_handler    DMA8_DMA24_IRQHandler
    def_irq_handler    DMA9_DMA25_IRQHandler
    def_irq_handler    DMA10_DMA26_IRQHandler
    def_irq_handler    DMA11_DMA27_IRQHandler
    def_irq_handler    DMA12_DMA28_IRQHandler
    def_irq_handler    DMA13_DMA29_IRQHandler
    def_irq_handler    DMA14_DMA30_IRQHandler
    def_irq_handler    DMA15_DMA31_IRQHandler
    def_irq_handler    DMA_Error_IRQHandler
    def_irq_handler    MCM_IRQHandler
    def_irq_handler    FTFE_IRQHandler
    def_irq_handler    Read_Collision_IRQHandler
    def_irq_handler    LVD_LVW_IRQHandler
    def_irq_handler    LLWU_IRQHandler
    def_irq_handler    WDOG_EWM_IRQHandler
    def_irq_handler    RNG_IRQHandler
    def_irq_handler    I2C0_IRQHandler
    def_irq_handler    I2C1_IRQHandler
    def_irq_handler    SPI0_IRQHandler
    def_irq_handler    SPI1_IRQHandler
    def_irq_handler    I2S0_Tx_IRQHandler
    def_irq_handler    I2S0_Rx_IRQHandler
    def_irq_handler    Reserved46_IRQHandler
    def_irq_handler    UART0_RX_TX_IRQHandler
    def_irq_handler    UART0_ERR_IRQHandler
    def_irq_handler    UART1_RX_TX_IRQHandler
    def_irq_handler    UART1_ERR_IRQHandler
    def_irq_handler    UART2_RX_TX_IRQHandler
    def_irq_handler    UART2_ERR_IRQHandler
    def_irq_handler    UART3_RX_TX_IRQHandler
    def_irq_handler    UART3_ERR_IRQHandler
    def_irq_handler    ADC0_IRQHandler
    def_irq_handler    CMP0_IRQHandler
    def_irq_handler    CMP1_IRQHandler
    def_irq_handler    FTM0_IRQHandler
    def_irq_handler    FTM1_IRQHandler
    def_irq_handler    FTM2_IRQHandler
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
    def_irq_handler    Reserved71_IRQHandler
    def_irq_handler    DAC0_IRQHandler
    def_irq_handler    MCG_IRQHandler
    def_irq_handler    LPTMR0_IRQHandler
    def_irq_handler    PORTA_IRQHandler
    def_irq_handler    PORTB_IRQHandler
    def_irq_handler    PORTC_IRQHandler
    def_irq_handler    PORTD_IRQHandler
    def_irq_handler    PORTE_IRQHandler
    def_irq_handler    SWI_IRQHandler
    def_irq_handler    SPI2_IRQHandler
    def_irq_handler    UART4_RX_TX_IRQHandler
    def_irq_handler    UART4_ERR_IRQHandler
    def_irq_handler    Reserved84_IRQHandler
    def_irq_handler    Reserved85_IRQHandler
    def_irq_handler    CMP2_IRQHandler
    def_irq_handler    FTM3_IRQHandler
    def_irq_handler    DAC1_IRQHandler
    def_irq_handler    ADC1_IRQHandler
    def_irq_handler    I2C2_IRQHandler
    def_irq_handler    CAN0_ORed_Message_buffer_IRQHandler
    def_irq_handler    CAN0_Bus_Off_IRQHandler
    def_irq_handler    CAN0_Error_IRQHandler
    def_irq_handler    CAN0_Tx_Warning_IRQHandler
    def_irq_handler    CAN0_Rx_Warning_IRQHandler
    def_irq_handler    CAN0_Wake_Up_IRQHandler
    def_irq_handler    SDHC_IRQHandler
    def_irq_handler    Reserved98_IRQHandler
    def_irq_handler    Reserved99_IRQHandler
    def_irq_handler    Reserved100_IRQHandler
    def_irq_handler    Reserved101_IRQHandler
    def_irq_handler    LPUART0_IRQHandler
    def_irq_handler    TSI0_IRQHandler
    def_irq_handler    TPM1_IRQHandler
    def_irq_handler    TPM2_IRQHandler
    def_irq_handler    USBHSDCD_IRQHandler
    def_irq_handler    I2C3_IRQHandler
    def_irq_handler    CMP3_IRQHandler
    def_irq_handler    USBHS_IRQHandler
    def_irq_handler    CAN1_ORed_Message_buffer_IRQHandler
    def_irq_handler    CAN1_Bus_Off_IRQHandler
    def_irq_handler    CAN1_Error_IRQHandler
    def_irq_handler    CAN1_Tx_Warning_IRQHandler
    def_irq_handler    CAN1_Rx_Warning_IRQHandler
    def_irq_handler    CAN1_Wake_Up_IRQHandler

    .end
