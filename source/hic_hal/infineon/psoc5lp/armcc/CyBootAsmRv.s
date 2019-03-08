;-------------------------------------------------------------------------------
; FILENAME: CyBootAsmRv.s
; Version 5.70
;
;  DESCRIPTION:
;    Assembly routines for RealView.
;
;-------------------------------------------------------------------------------
; Copyright (2019-2021) Cypress Semiconductor Corporation (an Infineon company) 
; or an affiliate of Cypress Semiconductor Corporation.
;
; Licensed under the Apache License, Version 2.0 (the "License"); you may
; not use this file except in compliance with the License.
; You may obtain a copy of the License at
;
; http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
; WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.
;-------------------------------------------------------------------------------

    AREA |.text|,CODE,ALIGN=3
    THUMB
    EXTERN Reset

    IF :DEF:__UVISION_VERSION
        GET cyfitterrv.inc
    ELSE
;       workaraund for mbedcli_compile.py due to use armcc instead of armasm to compile asm code
        GET source\hic_hal\infineon\psoc5lp\armcc\cyfitterrv.inc
    ENDIF

;-------------------------------------------------------------------------------
; Function Name: CyDelayCycles
;-------------------------------------------------------------------------------
;
; Summary:
;  Delays for the specified number of cycles.
;
; Parameters:
;  uint32 cycles: number of cycles to delay.
;
; Return:
;  None
;
;-------------------------------------------------------------------------------
; void CyDelayCycles(uint32 cycles)
    ALIGN 8
CyDelayCycles FUNCTION
    EXPORT CyDelayCycles
    IF CYDEV_INSTRUCT_CACHE_ENABLED == 1
                              ; cycles bytes
    ADDS r0, r0, #2           ;   1   2  Round to nearest multiple of 4
    LSRS r0, r0, #2           ;   1   2  Divide by 4 and set flags
    BEQ CyDelayCycles_done    ;   2   2  Skip if 0
    NOP                       ;   1   2  Loop alignment padding
CyDelayCycles_loop
    SUBS r0, r0, #1           ;   1   2
    MOV r0, r0                ;   1   2  Pad loop to power of two cycles
    BNE CyDelayCycles_loop    ;   2   2
    NOP                       ;   1   2  Loop alignment padding
CyDelayCycles_done
    BX lr                     ;   3   2

    ELSE

    CMP r0, #20               ;   1   2  If delay is short - jump to cycle
    BLS CyDelayCycles_short   ;   1   2
    PUSH {r1}                 ;   2   2  PUSH r1 to stack
    MOVS r1, #1               ;   1   2

    SUBS r0, r0, #20          ;   1   2  Subtract overhead
    LDR r1,=CYREG_CACHE_CC_CTL;   2   2  Load flash wait cycles value
    LDRB r1, [r1, #0]         ;   2   2
    ANDS r1, #0xC0            ;   1   2

    LSRS r1, r1, #6           ;   1   2
    PUSH {r2}                 ;   1   2  PUSH r2 to stack
    LDR r2, =cy_flash_cycles  ;   2   2
    LDRB r1, [r2, r1]         ;   2   2

    POP {r2}                  ;   2   2  POP r2 from stack
    NOP                       ;   1   2  Alignment padding
    NOP                       ;   1   2  Alignment padding
    NOP                       ;   1   2  Alignment padding

CyDelayCycles_loop
    SBCS r0, r0, r1           ;   1   2
    BPL CyDelayCycles_loop    ;   3   2
    NOP                       ;   1   2  Loop alignment padding
    NOP                       ;   1   2  Loop alignment padding

    POP {r1}                  ;   2   2  POP r1 from stack
CyDelayCycles_done
    BX lr                     ;   3   2
    NOP                       ;   1   2  Alignment padding
    NOP                       ;   1   2  Alignment padding

CyDelayCycles_short
    SBCS r0, r0, #4           ;   1   2
    BPL CyDelayCycles_short   ;   3   2
    BX lr                     ;   3   2

cy_flash_cycles
byte_1 DCB 0x0B
byte_2 DCB 0x05
byte_3 DCB 0x07
byte_4 DCB 0x09

    ENDIF
    ENDFUNC


;-------------------------------------------------------------------------------
; Function Name: CyEnterCriticalSection
;-------------------------------------------------------------------------------
;
; Summary:
;  CyEnterCriticalSection disables interrupts and returns a value indicating
;  whether interrupts were previously enabled (the actual value depends on
;  whether the device is PSoC 3 or PSoC 5).
;
;  Note Implementation of CyEnterCriticalSection manipulates the IRQ enable bit
;  with interrupts still enabled. The test and set of the interrupt bits is not
;  atomic; this is true for both PSoC 3 and PSoC 5. Therefore, to avoid a
;  corrupting processor state, it must be the policy that all interrupt routines
;  restore the interrupt enable bits as they were found on entry.
;
; Parameters:
;  None
;
; Return:
;  uint8
;   Returns 0 if interrupts were previously enabled or 1 if interrupts
;   were previously disabled.
;
;-------------------------------------------------------------------------------
; uint8 CyEnterCriticalSection(void)
CyEnterCriticalSection FUNCTION
    EXPORT CyEnterCriticalSection
    MRS r0, PRIMASK         ; Save and return interrupt state
    CPSID I                 ; Disable interrupts
    BX lr
    ENDFUNC


;-------------------------------------------------------------------------------
; Function Name: CyExitCriticalSection
;-------------------------------------------------------------------------------
;
; Summary:
;  CyExitCriticalSection re-enables interrupts if they were enabled before
;  CyEnterCriticalSection was called. The argument should be the value returned
;  from CyEnterCriticalSection.
;
; Parameters:
;  uint8 savedIntrStatus:
;   Saved interrupt status returned by the CyEnterCriticalSection function.
;
; Return:
;  None
;
;-------------------------------------------------------------------------------
; void CyExitCriticalSection(uint8 savedIntrStatus)
CyExitCriticalSection FUNCTION
    EXPORT CyExitCriticalSection
    MSR PRIMASK, r0         ; Restore interrupt state
    BX lr
    ENDFUNC

    END

; [] END OF FILE
