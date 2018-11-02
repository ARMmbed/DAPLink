;/**
; * @file    startup_TMPM366.s
; * @brief   
; *
; * DAPLink Interface Firmware
; * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the "License"); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000200

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x0000100

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     DAPLINK_BUILD_KEY         ; Build type - BL/IF
                DCD     DAPLINK_HIC_ID            ; Compatibility
                DCD     DAPLINK_VERSION           ; Version
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     INT0_IRQHandler           ; 0:  Interrupt pin 0
                DCD     INT1_IRQHandler           ; 1:  Interrupt pin 1
                DCD     INT2_IRQHandler           ; 2:  Interrupt pin 2
                DCD     INT3_IRQHandler           ; 3:  Interrupt pin 3
                DCD     INT4_IRQHandler           ; 4:  Interrupt pin 4
                DCD     INT5_IRQHandler           ; 5:  Interrupt pin 5
                DCD     INT6_IRQHandler           ; 6:  Interrupt pin 6
                DCD     INT7_IRQHandler           ; 7:  Interrupt pin 7
                DCD     INTRX0_IRQHandler         ; 8:  Serial reception (channel.0)
                DCD     INTTX0_IRQHandler         ; 9:  Serial transmission (channel.0)
                DCD     INTRX1_IRQHandler         ; 10: Serial reception (channel.1)
                DCD     INTTX1_IRQHandler         ; 11: Serial transmission (channel.1)
                DCD     INTUSBWKUP_IRQHandler     ; 12: USB WakeUp
                DCD     0                         ; 13: Reserved
                DCD     INTSBI0_IRQHandler        ; 14: Serial bus interface 0
                DCD     INTSBI1_IRQHandler        ; 15: Serial bus interface 1
                DCD     INTADHP_IRQHandler        ; 16: Highest priority AD conversion complete interrupt
                DCD     INTAD_IRQHandler          ; 17: AD conversion interrupt
                DCD     INTADM0_IRQHandler        ; 18: AD conversion monitoring function interrupt 0
                DCD     INTADM1_IRQHandler        ; 19: AD conversion monitoring function interrupt 1
                DCD     INTTB0_IRQHandler         ; 20: 16-bit TMRB match detection 0
                DCD     INTTB1_IRQHandler         ; 21: 16-bit TMRB match detection 1
                DCD     INTTB2_IRQHandler         ; 22: 16-bit TMRB match detection 2
                DCD     INTTB3_IRQHandler         ; 23: 16-bit TMRB match detection 3
                DCD     INTTB4_IRQHandler         ; 24: 16-bit TMRB match detection 4
                DCD     INTTB5_IRQHandler         ; 25: 16-bit TMRB match detection 5
                DCD     INTTB6_IRQHandler         ; 26: 16-bit TMRB match detection 6
                DCD     INTTB7_IRQHandler         ; 27: 16-bit TMRB match detection 7
                DCD     INTTB8_IRQHandler         ; 28: 16-bit TMRB match detection 8
                DCD     INTTB9_IRQHandler         ; 29: 16-bit TMRB match detection 9
                DCD     INTUSB_IRQHandler         ; 30: USB Interrupt
                DCD     INTSSP2_IRQHandler        ; 31: SPI serial interface 2
                DCD     0                         ; 32: Reserved
                DCD     0                         ; 33: Reserved
                DCD     INTUSBPON_IRQHandler      ; 34: USB V-Bus _Power on Detect
                DCD     INTUART_IRQHandler        ; 35: UART Interrupt
                DCD     INTCAP00_IRQHandler       ; 36: 16-bit TMRB input capture 00
                DCD     INTCAP01_IRQHandler       ; 37: 16-bit TMRB input capture 01
                DCD     INTCAP10_IRQHandler       ; 38: 16-bit TMRB input capture 10
                DCD     INTCAP11_IRQHandler       ; 39: 16-bit TMRB input capture 11
                DCD     INTCAP20_IRQHandler       ; 40: 16-bit TMRB input capture 20
                DCD     INTCAP21_IRQHandler       ; 41: 16-bit TMRB input capture 21
                DCD     INTCAP30_IRQHandler       ; 42: 16-bit TMRB input capture 30
                DCD     INTCAP31_IRQHandler       ; 43: 16-bit TMRB input capture 31
                DCD     INTCAP40_IRQHandler       ; 44: 16-bit TMRB input capture 40
                DCD     INTCAP41_IRQHandler       ; 45: 16-bit TMRB input capture 41
                DCD     INTCAP50_IRQHandler       ; 46: 16-bit TMRB input capture 50
                DCD     INTCAP51_IRQHandler       ; 47: 16-bit TMRB input capture 51
                DCD     INTCAP60_IRQHandler       ; 48: 16-bit TMRB input capture 60
                DCD     INTCAP61_IRQHandler       ; 49: 16-bit TMRB input capture 61
                DCD     INTCAP70_IRQHandler       ; 50: 16-bit TMRB input capture 70
                DCD     INTCAP71_IRQHandler       ; 51: 16-bit TMRB input capture 71
                DCD     INTCAP80_IRQHandler       ; 52: 16-bit TMRB input capture 80
                DCD     INTCAP81_IRQHandler       ; 53: 16-bit TMRB input capture 81
                DCD     INTCAP90_IRQHandler       ; 54: 16-bit TMRB input capture 90
                DCD     INTCAP91_IRQHandler       ; 55: 16-bit TMRB input capture 91
                DCD     INT8_IRQHandler           ; 56: Interrupt pin 8
                DCD     INT9_IRQHandler           ; 57: Interrupt pin 9
                DCD     INTSSP1_IRQHandler        ; 58: SPI serial interface 1
                DCD     INTSSP0_IRQHandler        ; 59: SPI serial interface 0
                DCD     INTDMAC0TC_IRQHandler     ; 60: DMA terminal count status interrupt 0
                DCD     INTDMAC1TC_IRQHandler     ; 61: DMA terminal count status interrupt 1
                DCD     INTDMAC0ERR_IRQHandler    ; 62: DMA error status interrupt 0
                DCD     INTDMAC1ERR_IRQHandler    ; 63: DMA error status interrupt 1

__Vectors_End

__Vectors_Size 	EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC
                EXPORT  INT0_IRQHandler           [WEAK]
                EXPORT  INT1_IRQHandler           [WEAK]
                EXPORT  INT2_IRQHandler           [WEAK]
                EXPORT  INT3_IRQHandler           [WEAK]
                EXPORT  INT4_IRQHandler           [WEAK]
                EXPORT  INT5_IRQHandler           [WEAK]
                EXPORT  INT6_IRQHandler           [WEAK]
                EXPORT  INT7_IRQHandler           [WEAK]
                EXPORT  INTRX0_IRQHandler         [WEAK]
                EXPORT  INTTX0_IRQHandler         [WEAK]
                EXPORT  INTRX1_IRQHandler         [WEAK]
                EXPORT  INTTX1_IRQHandler         [WEAK]
                EXPORT  INTUSBWKUP_IRQHandler     [WEAK]
                EXPORT  INTSBI0_IRQHandler        [WEAK]
                EXPORT  INTSBI1_IRQHandler        [WEAK]
                EXPORT  INTADHP_IRQHandler        [WEAK]
                EXPORT  INTAD_IRQHandler          [WEAK]
                EXPORT  INTADM0_IRQHandler        [WEAK]
                EXPORT  INTADM1_IRQHandler        [WEAK]
                EXPORT  INTTB0_IRQHandler         [WEAK]
                EXPORT  INTTB1_IRQHandler         [WEAK]
                EXPORT  INTTB2_IRQHandler         [WEAK]
                EXPORT  INTTB3_IRQHandler         [WEAK]
                EXPORT  INTTB4_IRQHandler         [WEAK]
                EXPORT  INTTB5_IRQHandler         [WEAK]
                EXPORT  INTTB6_IRQHandler         [WEAK]
                EXPORT  INTTB7_IRQHandler         [WEAK]
                EXPORT  INTTB8_IRQHandler         [WEAK]
                EXPORT  INTTB9_IRQHandler         [WEAK]
                EXPORT  INTUSB_IRQHandler         [WEAK]
                EXPORT  INTSSP2_IRQHandler        [WEAK]
                EXPORT  INTUSBPON_IRQHandler      [WEAK]
                EXPORT  INTUART_IRQHandler        [WEAK]
                EXPORT  INTCAP00_IRQHandler       [WEAK]
                EXPORT  INTCAP01_IRQHandler       [WEAK]
                EXPORT  INTCAP10_IRQHandler       [WEAK]
                EXPORT  INTCAP11_IRQHandler       [WEAK]
                EXPORT  INTCAP20_IRQHandler       [WEAK]
                EXPORT  INTCAP21_IRQHandler       [WEAK]
                EXPORT  INTCAP30_IRQHandler       [WEAK]
                EXPORT  INTCAP31_IRQHandler       [WEAK]
                EXPORT  INTCAP40_IRQHandler       [WEAK]
                EXPORT  INTCAP41_IRQHandler       [WEAK]
                EXPORT  INTCAP50_IRQHandler       [WEAK]
                EXPORT  INTCAP51_IRQHandler       [WEAK]
                EXPORT  INTCAP60_IRQHandler       [WEAK]
                EXPORT  INTCAP61_IRQHandler       [WEAK]
                EXPORT  INTCAP70_IRQHandler       [WEAK]
                EXPORT  INTCAP71_IRQHandler       [WEAK]
                EXPORT  INTCAP80_IRQHandler       [WEAK]
                EXPORT  INTCAP81_IRQHandler       [WEAK]
                EXPORT  INTCAP90_IRQHandler       [WEAK]
                EXPORT  INTCAP91_IRQHandler       [WEAK]
                EXPORT  INT8_IRQHandler           [WEAK]
                EXPORT  INT9_IRQHandler           [WEAK]
                EXPORT  INTSSP1_IRQHandler        [WEAK]
                EXPORT  INTSSP0_IRQHandler        [WEAK]
                EXPORT  INTDMAC0TC_IRQHandler     [WEAK]
                EXPORT  INTDMAC1TC_IRQHandler     [WEAK]
                EXPORT  INTDMAC0ERR_IRQHandler    [WEAK]
                EXPORT  INTDMAC1ERR_IRQHandler    [WEAK]

INT0_IRQHandler
INT1_IRQHandler
INT2_IRQHandler
INT3_IRQHandler
INT4_IRQHandler
INT5_IRQHandler
INT6_IRQHandler
INT7_IRQHandler
INTRX0_IRQHandler
INTTX0_IRQHandler
INTRX1_IRQHandler
INTTX1_IRQHandler
INTUSBWKUP_IRQHandler 
INTSBI0_IRQHandler
INTSBI1_IRQHandler
INTADHP_IRQHandler
INTAD_IRQHandler
INTADM0_IRQHandler
INTADM1_IRQHandler
INTTB0_IRQHandler
INTTB1_IRQHandler
INTTB2_IRQHandler
INTTB3_IRQHandler
INTTB4_IRQHandler
INTTB5_IRQHandler
INTTB6_IRQHandler
INTTB7_IRQHandler
INTTB8_IRQHandler
INTTB9_IRQHandler
INTUSB_IRQHandler
INTSSP2_IRQHandler
INTUSBPON_IRQHandler
INTUART_IRQHandler
INTCAP00_IRQHandler
INTCAP01_IRQHandler
INTCAP10_IRQHandler
INTCAP11_IRQHandler
INTCAP20_IRQHandler
INTCAP21_IRQHandler
INTCAP30_IRQHandler
INTCAP31_IRQHandler
INTCAP40_IRQHandler
INTCAP41_IRQHandler
INTCAP50_IRQHandler
INTCAP51_IRQHandler
INTCAP60_IRQHandler
INTCAP61_IRQHandler
INTCAP70_IRQHandler
INTCAP71_IRQHandler
INTCAP80_IRQHandler
INTCAP81_IRQHandler
INTCAP90_IRQHandler
INTCAP91_IRQHandler
INT8_IRQHandler
INT9_IRQHandler
INTSSP1_IRQHandler
INTSSP0_IRQHandler
INTDMAC0TC_IRQHandler
INTDMAC1TC_IRQHandler
INTDMAC0ERR_IRQHandler
INTDMAC1ERR_IRQHandler
                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
