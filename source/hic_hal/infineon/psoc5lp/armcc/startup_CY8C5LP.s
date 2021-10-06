;-------------------------------------------------------------------------------
; * @file:    startup_CY8C5LP.s
; * @purpose: CMSIS Cortex-M3 Core Device Startup File
; *           for the Cypress CY8C5xxxLP
; * @version: V1.01
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

Heap_Size       EQU     0x00000100

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


; <h> Non-Volatile Configuration
;   <e> Flash Protection Data
FP_DATA         EQU     0
;     <e> Rows 0..255
FP_R0           EQU     1
;       <o.0..1>  Row 0    <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 1    <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 2    <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 3    <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B0           EQU     0x00
;       <o.0..1>  Row 4    <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 5    <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 6    <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 7    <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B1           EQU     0x00
;       <o.0..1>  Row 8    <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 9    <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 10   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 11   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B2           EQU     0x00
;       <o.0..1>  Row 12   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 13   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 14   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 15   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B3           EQU     0x00
;       <o.0..1>  Row 16   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 17   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 18   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 19   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B4           EQU     0x00
;       <o.0..1>  Row 20   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 21   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 22   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 23   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B5           EQU     0x00
;       <o.0..1>  Row 24   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 25   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 26   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 27   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B6           EQU     0x00
;       <o.0..1>  Row 28   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 29   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 30   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 31   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B7           EQU     0x00
;       <o.0..1>  Row 32   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 33   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 34   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 35   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B8           EQU     0x00
;       <o.0..1>  Row 36   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 37   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 38   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 39   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B9           EQU     0x00
;       <o.0..1>  Row 40   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 41   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 42   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 43   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B10          EQU     0x00
;       <o.0..1>  Row 44   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 45   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 46   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 47   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B11          EQU     0x00
;       <o.0..1>  Row 48   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 49   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 50   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 51   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B12          EQU     0x00
;       <o.0..1>  Row 52   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 53   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 54   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 55   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B13          EQU     0x00
;       <o.0..1>  Row 56   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 57   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 58   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 59   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B14          EQU     0x00
;       <o.0..1>  Row 60   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 61   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 62   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 63   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B15          EQU     0x00
;       <o.0..1>  Row 64   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 65   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 66   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 67   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B16          EQU     0x00
;       <o.0..1>  Row 68   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 69   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 70   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 71   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B17          EQU     0x00
;       <o.0..1>  Row 72   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 73   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 74   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 75   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B18          EQU     0x00
;       <o.0..1>  Row 76   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 77   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 78   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 79   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B19          EQU     0x00
;       <o.0..1>  Row 80   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 81   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 82   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 83   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B20          EQU     0x00
;       <o.0..1>  Row 84   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 85   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 86   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 87   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B21          EQU     0x00
;       <o.0..1>  Row 88   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 89   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 90   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 91   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B22          EQU     0x00
;       <o.0..1>  Row 92   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 93   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 94   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 95   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B23          EQU     0x00
;       <o.0..1>  Row 96   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 97   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 98   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 99   <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B24          EQU     0x00
;       <o.0..1>  Row 100  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 101  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 102  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 103  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B25          EQU     0x00
;       <o.0..1>  Row 104  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 105  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 106  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 107  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B26          EQU     0x00
;       <o.0..1>  Row 108  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 109  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 110  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 111  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B27          EQU     0x00
;       <o.0..1>  Row 112  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 113  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 114  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 115  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B28          EQU     0x00
;       <o.0..1>  Row 116  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 117  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 118  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 119  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B29          EQU     0x00
;       <o.0..1>  Row 120  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 121  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 122  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 123  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B30          EQU     0x00
;       <o.0..1>  Row 124  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 125  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 126  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 127  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B31          EQU     0x00
;       <o.0..1>  Row 128  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 129  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 130  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 131  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B32          EQU     0x00
;       <o.0..1>  Row 132  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 133  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 134  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 135  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B33          EQU     0x00
;       <o.0..1>  Row 136  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 137  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 138  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 139  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B34          EQU     0x00
;       <o.0..1>  Row 140  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 141  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 142  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 143  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B35          EQU     0x00
;       <o.0..1>  Row 144  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 145  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 146  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 147  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B36          EQU     0x00
;       <o.0..1>  Row 148  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 149  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 150  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 151  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B37          EQU     0x00
;       <o.0..1>  Row 152  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 153  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 154  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 155  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B38          EQU     0x00
;       <o.0..1>  Row 156  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 157  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 158  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 159  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B39          EQU     0x00
;       <o.0..1>  Row 160  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 161  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 162  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 163  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B40          EQU     0x00
;       <o.0..1>  Row 164  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 165  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 166  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 167  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B41          EQU     0x00
;       <o.0..1>  Row 168  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 169  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 170  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 171  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B42          EQU     0x00
;       <o.0..1>  Row 172  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 173  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 174  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 175  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B43          EQU     0x00
;       <o.0..1>  Row 176  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 177  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 178  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 179  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B44          EQU     0x00
;       <o.0..1>  Row 180  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 181  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 182  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 183  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B45          EQU     0x00
;       <o.0..1>  Row 184  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 185  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 186  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 187  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B46          EQU     0x00
;       <o.0..1>  Row 188  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 189  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 190  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 191  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B47          EQU     0x00
;       <o.0..1>  Row 192  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 193  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 194  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 195  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B48          EQU     0x00
;       <o.0..1>  Row 196  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 197  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 198  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 199  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B49          EQU     0x00
;       <o.0..1>  Row 200  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 201  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 202  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 203  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B50          EQU     0x00
;       <o.0..1>  Row 204  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 205  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 206  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 207  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B51          EQU     0x00
;       <o.0..1>  Row 208  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 209  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 210  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 211  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B52          EQU     0x00
;       <o.0..1>  Row 212  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 213  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 214  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 215  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B53          EQU     0x00
;       <o.0..1>  Row 216  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 217  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 218  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 219  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B54          EQU     0x00
;       <o.0..1>  Row 220  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 221  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 222  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 223  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B55          EQU     0x00
;       <o.0..1>  Row 224  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 225  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 226  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 227  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B56          EQU     0x00
;       <o.0..1>  Row 228  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 229  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 230  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 231  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B57          EQU     0x00
;       <o.0..1>  Row 232  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 233  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 234  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 235  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B58          EQU     0x00
;       <o.0..1>  Row 236  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 237  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 238  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 239  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B59          EQU     0x00
;       <o.0..1>  Row 240  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 241  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 242  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 243  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B60          EQU     0x00
;       <o.0..1>  Row 244  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 245  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 246  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 247  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B61          EQU     0x00
;       <o.0..1>  Row 248  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 249  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 250  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 251  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B62          EQU     0x00
;       <o.0..1>  Row 252  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 253  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 254  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 255  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B63          EQU     0x00
;     </e>
;     <e> Rows 256..511
FP_R1           EQU     1
;       <o.0..1>  Row 256  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 257  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 258  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 259  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B64          EQU     0x00
;       <o.0..1>  Row 260  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 261  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 262  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 263  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B65          EQU     0x00
;       <o.0..1>  Row 264  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 265  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 266  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 267  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B66          EQU     0x00
;       <o.0..1>  Row 268  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 269  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 270  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 271  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B67          EQU     0x00
;       <o.0..1>  Row 272  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 273  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 274  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 275  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B68          EQU     0x00
;       <o.0..1>  Row 276  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 277  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 278  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 279  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B69          EQU     0x00
;       <o.0..1>  Row 280  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 281  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 282  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 283  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B70          EQU     0x00
;       <o.0..1>  Row 284  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 285  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 286  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 287  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B71          EQU     0x00
;       <o.0..1>  Row 288  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 289  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 290  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 291  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B72          EQU     0x00
;       <o.0..1>  Row 292  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 293  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 294  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 295  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B73          EQU     0x00
;       <o.0..1>  Row 296  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 297  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 298  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 299  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B74          EQU     0x00
;       <o.0..1>  Row 300  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 301  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 302  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 303  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B75          EQU     0x00
;       <o.0..1>  Row 304  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 305  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 306  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 307  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B76          EQU     0x00
;       <o.0..1>  Row 308  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 309  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 310  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 311  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B77          EQU     0x00
;       <o.0..1>  Row 312  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 313  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 314  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 315  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B78          EQU     0x00
;       <o.0..1>  Row 316  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 317  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 318  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 319  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B79          EQU     0x00
;       <o.0..1>  Row 320  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 321  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 322  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 323  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B80          EQU     0x00
;       <o.0..1>  Row 324  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 325  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 326  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 327  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B81          EQU     0x00
;       <o.0..1>  Row 328  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 329  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 330  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 331  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B82          EQU     0x00
;       <o.0..1>  Row 332  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 333  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 334  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 335  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B83          EQU     0x00
;       <o.0..1>  Row 336  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 337  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 338  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 339  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B84          EQU     0x00
;       <o.0..1>  Row 340  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 341  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 342  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 343  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B85          EQU     0x00
;       <o.0..1>  Row 344  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 345  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 346  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 347  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B86          EQU     0x00
;       <o.0..1>  Row 348  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 349  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 350  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 351  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B87          EQU     0x00
;       <o.0..1>  Row 352  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 353  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 354  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 355  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B88          EQU     0x00
;       <o.0..1>  Row 356  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 357  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 358  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 359  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B89          EQU     0x00
;       <o.0..1>  Row 360  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 361  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 362  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 363  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B90          EQU     0x00
;       <o.0..1>  Row 364  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 365  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 366  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 367  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B91          EQU     0x00
;       <o.0..1>  Row 368  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 369  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 370  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 371  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B92          EQU     0x00
;       <o.0..1>  Row 372  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 373  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 374  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 375  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B93          EQU     0x00
;       <o.0..1>  Row 376  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 377  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 378  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 379  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B94          EQU     0x00
;       <o.0..1>  Row 380  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 381  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 382  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 383  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B95          EQU     0x00
;       <o.0..1>  Row 384  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 385  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 386  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 387  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B96          EQU     0x00
;       <o.0..1>  Row 388  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 389  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 390  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 391  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B97          EQU     0x00
;       <o.0..1>  Row 392  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 393  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 394  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 395  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B98          EQU     0x00
;       <o.0..1>  Row 396  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 397  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 398  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 399  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B99          EQU     0x00
;       <o.0..1>  Row 400  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 401  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 402  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 403  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B100         EQU     0x00
;       <o.0..1>  Row 404  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 405  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 406  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 407  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B101         EQU     0x00
;       <o.0..1>  Row 408  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 409  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 410  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 411  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B102         EQU     0x00
;       <o.0..1>  Row 412  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 413  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 414  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 415  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B103         EQU     0x00
;       <o.0..1>  Row 416  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 417  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 418  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 419  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B104         EQU     0x00
;       <o.0..1>  Row 420  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 421  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 422  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 423  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B105         EQU     0x00
;       <o.0..1>  Row 424  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 425  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 426  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 427  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B106         EQU     0x00
;       <o.0..1>  Row 428  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 429  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 430  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 431  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B107         EQU     0x00
;       <o.0..1>  Row 432  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 433  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 434  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 435  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B108         EQU     0x00
;       <o.0..1>  Row 436  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 437  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 438  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 439  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B109         EQU     0x00
;       <o.0..1>  Row 440  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 441  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 442  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 443  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B110         EQU     0x00
;       <o.0..1>  Row 444  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 445  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 446  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 447  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B111         EQU     0x00
;       <o.0..1>  Row 448  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 449  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 450  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 451  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B112         EQU     0x00
;       <o.0..1>  Row 452  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 453  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 454  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 455  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B113         EQU     0x00
;       <o.0..1>  Row 456  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 457  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 458  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 459  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B114         EQU     0x00
;       <o.0..1>  Row 460  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 461  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 462  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 463  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B115         EQU     0x00
;       <o.0..1>  Row 464  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 465  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 466  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 467  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B116         EQU     0x00
;       <o.0..1>  Row 468  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 469  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 470  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 471  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B117         EQU     0x00
;       <o.0..1>  Row 472  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 473  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 474  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 475  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B118         EQU     0x00
;       <o.0..1>  Row 476  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 477  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 478  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 479  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B119         EQU     0x00
;       <o.0..1>  Row 480  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 481  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 482  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 483  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B120         EQU     0x00
;       <o.0..1>  Row 484  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 485  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 486  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 487  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B121         EQU     0x00
;       <o.0..1>  Row 488  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 489  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 490  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 491  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B122         EQU     0x00
;       <o.0..1>  Row 492  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 493  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 494  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 495  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B123         EQU     0x00
;       <o.0..1>  Row 496  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 497  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 498  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 499  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B124         EQU     0x00
;       <o.0..1>  Row 500  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 501  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 502  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 503  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B125         EQU     0x00
;       <o.0..1>  Row 504  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 505  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 506  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 507  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B126         EQU     0x00
;       <o.0..1>  Row 508  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 509  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 510  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 511  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B127         EQU     0x00
;     </e>
;     <e> Rows 512..767
FP_R2           EQU     1
;       <o.0..1>  Row 512  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 513  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 514  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 515  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B128         EQU     0x00
;       <o.0..1>  Row 516  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 517  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 518  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 519  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B129         EQU     0x00
;       <o.0..1>  Row 520  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 521  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 522  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 523  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B130         EQU     0x00
;       <o.0..1>  Row 524  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 525  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 526  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 527  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B131         EQU     0x00
;       <o.0..1>  Row 528  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 529  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 530  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 531  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B132         EQU     0x00
;       <o.0..1>  Row 532  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 533  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 534  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 535  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B133         EQU     0x00
;       <o.0..1>  Row 536  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 537  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 538  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 539  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B134         EQU     0x00
;       <o.0..1>  Row 540  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 541  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 542  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 543  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B135         EQU     0x00
;       <o.0..1>  Row 544  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 545  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 546  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 547  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B136         EQU     0x00
;       <o.0..1>  Row 548  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 549  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 550  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 551  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B137         EQU     0x00
;       <o.0..1>  Row 552  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 553  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 554  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 555  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B138         EQU     0x00
;       <o.0..1>  Row 556  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 557  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 558  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 559  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B139         EQU     0x00
;       <o.0..1>  Row 560  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 561  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 562  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 563  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B140         EQU     0x00
;       <o.0..1>  Row 564  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 565  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 566  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 567  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B141         EQU     0x00
;       <o.0..1>  Row 568  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 569  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 570  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 571  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B142         EQU     0x00
;       <o.0..1>  Row 572  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 573  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 574  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 575  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B143         EQU     0x00
;       <o.0..1>  Row 576  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 577  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 578  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 579  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B144         EQU     0x00
;       <o.0..1>  Row 580  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 581  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 582  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 583  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B145         EQU     0x00
;       <o.0..1>  Row 584  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 585  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 586  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 587  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B146         EQU     0x00
;       <o.0..1>  Row 588  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 589  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 590  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 591  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B147         EQU     0x00
;       <o.0..1>  Row 592  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 593  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 594  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 595  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B148         EQU     0x00
;       <o.0..1>  Row 596  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 597  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 598  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 599  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B149         EQU     0x00
;       <o.0..1>  Row 600  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 601  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 602  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 603  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B150         EQU     0x00
;       <o.0..1>  Row 604  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 605  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 606  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 607  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B151         EQU     0x00
;       <o.0..1>  Row 608  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 609  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 610  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 611  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B152         EQU     0x00
;       <o.0..1>  Row 612  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 613  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 614  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 615  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B153         EQU     0x00
;       <o.0..1>  Row 616  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 617  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 618  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 619  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B154         EQU     0x00
;       <o.0..1>  Row 620  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 621  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 622  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 623  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B155         EQU     0x00
;       <o.0..1>  Row 624  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 625  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 626  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 627  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B156         EQU     0x00
;       <o.0..1>  Row 628  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 629  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 630  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 631  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B157         EQU     0x00
;       <o.0..1>  Row 632  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 633  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 634  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 635  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B158         EQU     0x00
;       <o.0..1>  Row 636  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 637  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 638  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 639  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B159         EQU     0x00
;       <o.0..1>  Row 640  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 641  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 642  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 643  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B160         EQU     0x00
;       <o.0..1>  Row 644  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 645  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 646  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 647  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B161         EQU     0x00
;       <o.0..1>  Row 648  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 649  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 650  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 651  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B162         EQU     0x00
;       <o.0..1>  Row 652  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 653  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 654  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 655  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B163         EQU     0x00
;       <o.0..1>  Row 656  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 657  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 658  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 659  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B164         EQU     0x00
;       <o.0..1>  Row 660  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 661  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 662  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 663  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B165         EQU     0x00
;       <o.0..1>  Row 664  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 665  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 666  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 667  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B166         EQU     0x00
;       <o.0..1>  Row 668  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 669  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 670  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 671  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B167         EQU     0x00
;       <o.0..1>  Row 672  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 673  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 674  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 675  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B168         EQU     0x00
;       <o.0..1>  Row 676  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 677  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 678  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 679  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B169         EQU     0x00
;       <o.0..1>  Row 680  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 681  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 682  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 683  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B170         EQU     0x00
;       <o.0..1>  Row 684  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 685  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 686  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 687  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B171         EQU     0x00
;       <o.0..1>  Row 688  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 689  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 690  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 691  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B172         EQU     0x00
;       <o.0..1>  Row 692  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 693  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 694  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 695  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B173         EQU     0x00
;       <o.0..1>  Row 696  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 697  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 698  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 699  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B174         EQU     0x00
;       <o.0..1>  Row 700  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 701  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 702  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 703  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B175         EQU     0x00
;       <o.0..1>  Row 704  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 705  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 706  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 707  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B176         EQU     0x00
;       <o.0..1>  Row 708  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 709  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 710  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 711  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B177         EQU     0x00
;       <o.0..1>  Row 712  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 713  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 714  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 715  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B178         EQU     0x00
;       <o.0..1>  Row 716  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 717  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 718  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 719  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B179         EQU     0x00
;       <o.0..1>  Row 720  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 721  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 722  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 723  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B180         EQU     0x00
;       <o.0..1>  Row 724  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 725  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 726  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 727  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B181         EQU     0x00
;       <o.0..1>  Row 728  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 729  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 730  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 731  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B182         EQU     0x00
;       <o.0..1>  Row 732  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 733  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 734  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 735  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B183         EQU     0x00
;       <o.0..1>  Row 736  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 737  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 738  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 739  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B184         EQU     0x00
;       <o.0..1>  Row 740  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 741  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 742  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 743  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B185         EQU     0x00
;       <o.0..1>  Row 744  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 745  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 746  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 747  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B186         EQU     0x00
;       <o.0..1>  Row 748  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 749  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 750  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 751  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B187         EQU     0x00
;       <o.0..1>  Row 752  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 753  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 754  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 755  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B188         EQU     0x00
;       <o.0..1>  Row 756  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 757  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 758  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 759  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B189         EQU     0x00
;       <o.0..1>  Row 760  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 761  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 762  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 763  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B190         EQU     0x00
;       <o.0..1>  Row 764  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 765  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 766  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 767  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B191         EQU     0x00
;     </e>
;     <e> Rows 768..1023
FP_R3           EQU     1
;       <o.0..1>  Row 768  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 769  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 770  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 771  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B192         EQU     0x00
;       <o.0..1>  Row 772  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 773  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 774  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 775  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B193         EQU     0x00
;       <o.0..1>  Row 776  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 777  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 778  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 779  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B194         EQU     0x00
;       <o.0..1>  Row 780  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 781  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 782  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 783  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B195         EQU     0x00
;       <o.0..1>  Row 784  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 785  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 786  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 787  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B196         EQU     0x00
;       <o.0..1>  Row 788  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 789  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 790  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 791  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B197         EQU     0x00
;       <o.0..1>  Row 792  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 793  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 794  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 795  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B198         EQU     0x00
;       <o.0..1>  Row 796  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 797  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 798  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 799  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B199         EQU     0x00
;       <o.0..1>  Row 800  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 801  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 802  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 803  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B200         EQU     0x00
;       <o.0..1>  Row 804  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 805  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 806  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 807  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B201         EQU     0x00
;       <o.0..1>  Row 808  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 809  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 810  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 811  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B202         EQU     0x00
;       <o.0..1>  Row 812  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 813  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 814  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 815  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B203         EQU     0x00
;       <o.0..1>  Row 816  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 817  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 818  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 819  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B204         EQU     0x00
;       <o.0..1>  Row 820  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 821  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 822  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 823  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B205         EQU     0x00
;       <o.0..1>  Row 824  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 825  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 826  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 827  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B206         EQU     0x00
;       <o.0..1>  Row 828  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 829  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 830  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 831  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B207         EQU     0x00
;       <o.0..1>  Row 832  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 833  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 834  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 835  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B208         EQU     0x00
;       <o.0..1>  Row 836  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 837  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 838  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 839  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B209         EQU     0x00
;       <o.0..1>  Row 840  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 841  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 842  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 843  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B210         EQU     0x00
;       <o.0..1>  Row 844  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 845  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 846  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 847  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B211         EQU     0x00
;       <o.0..1>  Row 848  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 849  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 850  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 851  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B212         EQU     0x00
;       <o.0..1>  Row 852  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 853  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 854  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 855  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B213         EQU     0x00
;       <o.0..1>  Row 856  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 857  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 858  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 859  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B214         EQU     0x00
;       <o.0..1>  Row 860  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 861  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 862  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 863  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B215         EQU     0x00
;       <o.0..1>  Row 864  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 865  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 866  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 867  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B216         EQU     0x00
;       <o.0..1>  Row 868  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 869  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 870  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 871  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B217         EQU     0x00
;       <o.0..1>  Row 872  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 873  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 874  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 875  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B218         EQU     0x00
;       <o.0..1>  Row 876  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 877  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 878  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 879  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B219         EQU     0x00
;       <o.0..1>  Row 880  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 881  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 882  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 883  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B220         EQU     0x00
;       <o.0..1>  Row 884  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 885  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 886  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 887  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B221         EQU     0x00
;       <o.0..1>  Row 888  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 889  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 890  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 891  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B222         EQU     0x00
;       <o.0..1>  Row 892  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 893  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 894  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 895  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B223         EQU     0x00
;       <o.0..1>  Row 896  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 897  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 898  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 899  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B224         EQU     0x00
;       <o.0..1>  Row 900  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 901  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 902  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 903  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B225         EQU     0x00
;       <o.0..1>  Row 904  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 905  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 906  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 907  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B226         EQU     0x00
;       <o.0..1>  Row 908  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 909  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 910  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 911  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B227         EQU     0x00
;       <o.0..1>  Row 912  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 913  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 914  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 915  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B228         EQU     0x00
;       <o.0..1>  Row 916  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 917  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 918  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 919  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B229         EQU     0x00
;       <o.0..1>  Row 920  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 921  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 922  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 923  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B230         EQU     0x00
;       <o.0..1>  Row 924  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 925  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 926  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 927  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B231         EQU     0x00
;       <o.0..1>  Row 928  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 929  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 930  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 931  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B232         EQU     0x00
;       <o.0..1>  Row 932  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 933  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 934  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 935  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B233         EQU     0x00
;       <o.0..1>  Row 936  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 937  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 938  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 939  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B234         EQU     0x00
;       <o.0..1>  Row 940  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 941  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 942  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 943  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B235         EQU     0x00
;       <o.0..1>  Row 944  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 945  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 946  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 947  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B236         EQU     0x00
;       <o.0..1>  Row 948  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 949  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 950  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 951  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B237         EQU     0x00
;       <o.0..1>  Row 952  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 953  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 954  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 955  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B238         EQU     0x00
;       <o.0..1>  Row 956  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 957  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 958  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 959  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B239         EQU     0x00
;       <o.0..1>  Row 960  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 961  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 962  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 963  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B240         EQU     0x00
;       <o.0..1>  Row 964  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 965  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 966  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 967  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B241         EQU     0x00
;       <o.0..1>  Row 968  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 969  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 970  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 971  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B242         EQU     0x00
;       <o.0..1>  Row 972  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 973  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 974  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 975  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B243         EQU     0x00
;       <o.0..1>  Row 976  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 977  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 978  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 979  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B244         EQU     0x00
;       <o.0..1>  Row 980  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 981  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 982  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 983  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B245         EQU     0x00
;       <o.0..1>  Row 984  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 985  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 986  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 987  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B246         EQU     0x00
;       <o.0..1>  Row 988  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 989  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 990  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 991  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B247         EQU     0x00
;       <o.0..1>  Row 992  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 993  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 994  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 995  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B248         EQU     0x00
;       <o.0..1>  Row 996  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 997  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 998  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 999  <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B249         EQU     0x00
;       <o.0..1>  Row 1000 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 1001 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 1002 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 1003 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B250         EQU     0x00
;       <o.0..1>  Row 1004 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 1005 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 1006 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 1007 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B251         EQU     0x00
;       <o.0..1>  Row 1008 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 1009 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 1010 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 1011 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B252         EQU     0x00
;       <o.0..1>  Row 1012 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 1013 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 1014 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 1015 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B253         EQU     0x00
;       <o.0..1>  Row 1016 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 1017 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 1018 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 1019 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B254         EQU     0x00
;       <o.0..1>  Row 1020 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.2..3>  Row 1021 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.4..5>  Row 1022 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
;       <o.6..7>  Row 1023 <0=> Unprotected <1=> Read Protect <2=> Disable External Write <3=> Disable Internal Write
FP_B255         EQU     0x00
;     </e>
;   </e>
;   <e> Device Configuration Nonvolatile Latch (NVL)
CFG_NVL         EQU     0
;     <h> Byte 0
;       <o.0..1>  PRT0RDM[1:0]  <0x0-0x3>
;       <o.2..3>  PRT1RDM[1:0]  <0x0-0x3>
;       <o.4..5>  PRT2RDM[1:0]  <0x0-0x3>
;       <o.6..7>  PRT3RDM[1:0]  <0x0-0x3>
CFG_NVL0        EQU     0x00
;     </h>
;     <h> Byte 1
;       <o.0..1>  PRT4RDM[1:0]  <0x0-0x3>
;       <o.2..3>  PRT5RDM[1:0]  <0x0-0x3>
;       <o.4..5>  PRT6RDM[1:0]  <0x0-0x3>
;       <o.6..7>  PRT12RDM[1:0] <0x0-0x3>
CFG_NVL1        EQU     0x00
;     </h>
;     <h> Byte 2
;       <o.0..1>  PRT15RDM[1:0] <0x0-0x3>
;       <o.2..5>  RESERVED      <0x0-0xF>
;       <o.6>     DEBUG_EN
;       <o.7>     XRESMEN
CFG_NVL2        EQU     0x40
;     </h>
;     <h> Byte 3
;       <o.0>     CFGSPEED
;       <o.1..2>  DPS <0=> 5-wire JTAG <1=> 4-wire JTAG <2=> SWD <3=> Debug Ports Disabled
;       <o.3>     ECCEN
;       <o.4..7>  DIG_PHS_DLY <0x0-0xF>
CFG_NVL3        EQU     0x05
;     </h>
;   </e>
;   <e> Write-once Nonvolatile Latch (WO NVL)
;     <i> Enables device security
;     <i> WARNING: Programming the WO NVL with correct 32-bit key locks the device!
WO_NVL          EQU     0
;     <o0>  Byte 0  <0x0-0xFF>
;     <o1>  Byte 1  <0x0-0xFF>
;     <o2>  Byte 2  <0x0-0xFF>
;     <o3>  Byte 3  <0x0-0xFF>
WO_NVL0         EQU     0x00
WO_NVL1         EQU     0x00
WO_NVL2         EQU     0x00
WO_NVL3         EQU     0x00
;   </e>
; </h>

                IF      FP_DATA <> 0
                IF      FP_R0 <> 0
                AREA    |.ARM.__at_0x90400000|, CODE, READONLY
                DCB     FP_B0,   FP_B1,   FP_B2,   FP_B3,   FP_B4,   FP_B5,   FP_B6,    FP_B7
                DCB     FP_B8,   FP_B9,   FP_B10,  FP_B11,  FP_B12,  FP_B13,  FP_B14,   FP_B15
                DCB     FP_B16,  FP_B17,  FP_B18,  FP_B19,  FP_B20,  FP_B21,  FP_B22,   FP_B23
                DCB     FP_B24,  FP_B25,  FP_B26,  FP_B27,  FP_B28,  FP_B29,  FP_B30,   FP_B31
                DCB     FP_B32,  FP_B33,  FP_B34,  FP_B35,  FP_B36,  FP_B37,  FP_B38,   FP_B39
                DCB     FP_B40,  FP_B41,  FP_B42,  FP_B43,  FP_B44,  FP_B45,  FP_B46,   FP_B47
                DCB     FP_B48,  FP_B49,  FP_B50,  FP_B51,  FP_B52,  FP_B53,  FP_B54,   FP_B55
                DCB     FP_B56,  FP_B57,  FP_B58,  FP_B59,  FP_B60,  FP_B61,  FP_B62,   FP_B63
                ENDIF
                IF      FP_R1 <> 0
                AREA    |.ARM.__at_0x90400040|, CODE, READONLY
                DCB     FP_B64,  FP_B65,  FP_B66,  FP_B67,  FP_B68,  FP_B69,  FP_B70,   FP_B71
                DCB     FP_B72,  FP_B73,  FP_B74,  FP_B75,  FP_B76,  FP_B77,  FP_B78,   FP_B79
                DCB     FP_B80,  FP_B81,  FP_B82,  FP_B83,  FP_B84,  FP_B85,  FP_B86,   FP_B87
                DCB     FP_B88,  FP_B89,  FP_B90,  FP_B91,  FP_B92,  FP_B93,  FP_B94,   FP_B95
                DCB     FP_B96,  FP_B97,  FP_B98,  FP_B99,  FP_B100, FP_B101, FP_B102,  FP_B103
                DCB     FP_B104, FP_B105, FP_B106, FP_B107, FP_B108, FP_B109, FP_B110,  FP_B111
                DCB     FP_B112, FP_B113, FP_B114, FP_B115, FP_B116, FP_B117, FP_B118,  FP_B119
                DCB     FP_B120, FP_B121, FP_B122, FP_B123, FP_B124, FP_B125, FP_B126,  FP_B127
                ENDIF
                IF      FP_R2 <> 0
                AREA    |.ARM.__at_0x90400080|, CODE, READONLY
                DCB     FP_B128, FP_B129, FP_B130, FP_B131, FP_B132, FP_B133, FP_B134,  FP_B135
                DCB     FP_B136, FP_B137, FP_B138, FP_B139, FP_B140, FP_B141, FP_B142,  FP_B143
                DCB     FP_B144, FP_B145, FP_B146, FP_B147, FP_B148, FP_B149, FP_B150,  FP_B151
                DCB     FP_B152, FP_B153, FP_B154, FP_B155, FP_B156, FP_B157, FP_B158,  FP_B159
                DCB     FP_B160, FP_B161, FP_B162, FP_B163, FP_B164, FP_B165, FP_B166,  FP_B167
                DCB     FP_B168, FP_B169, FP_B170, FP_B171, FP_B172, FP_B173, FP_B174,  FP_B175
                DCB     FP_B176, FP_B177, FP_B178, FP_B179, FP_B180, FP_B181, FP_B182,  FP_B183
                DCB     FP_B184, FP_B185, FP_B186, FP_B187, FP_B188, FP_B189, FP_B190,  FP_B191
                ENDIF
                IF      FP_R3 <> 0
                AREA    |.ARM.__at_0x904000C0|, CODE, READONLY
                DCB     FP_B192, FP_B193, FP_B194, FP_B195, FP_B196, FP_B197, FP_B198,  FP_B199
                DCB     FP_B200, FP_B201, FP_B202, FP_B203, FP_B204, FP_B205, FP_B206,  FP_B207
                DCB     FP_B208, FP_B209, FP_B210, FP_B211, FP_B212, FP_B213, FP_B214,  FP_B215
                DCB     FP_B216, FP_B217, FP_B218, FP_B219, FP_B220, FP_B221, FP_B222,  FP_B223
                DCB     FP_B224, FP_B225, FP_B226, FP_B227, FP_B228, FP_B229, FP_B230,  FP_B231
                DCB     FP_B232, FP_B233, FP_B234, FP_B235, FP_B236, FP_B237, FP_B238,  FP_B239
                DCB     FP_B240, FP_B241, FP_B242, FP_B243, FP_B244, FP_B245, FP_B246,  FP_B247
                DCB     FP_B248, FP_B249, FP_B250, FP_B251, FP_B252, FP_B253, FP_B254,  FP_B255
                ENDIF
                ENDIF

                IF      CFG_NVL <> 0
                AREA    |.ARM.__at_0x90000000|, CODE, READONLY
                DCB     CFG_NVL0, CFG_NVL1, CFG_NVL2, CFG_NVL3
                ENDIF

                IF      WO_NVL <> 0
                AREA    |.ARM.__at_0x90100000|, CODE, READONLY
                DCB     WO_NVL0, WO_NVL1, WO_NVL2, WO_NVL3
                ENDIF


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                IMPORT  g_board_info
__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     DAPLINK_BUILD_KEY         ;  8: Build type - BL/IF
                DCD     DAPLINK_HIC_ID            ;  9: Compatibility
                DCD     DAPLINK_VERSION           ; 10:Version
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     g_board_info              ; Ptr to Board info, family info other target details
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     WDT_IRQHandler            ; 0:  Watchdog Timer
                DCD     RTC_IRQHandler            ; 1:  Real Time Clock
                DCD     TIM0_IRQHandler           ; 2:  Timer0 / Timer1
                DCD     TIM2_IRQHandler           ; 3:  Timer2 / Timer3
                DCD     MCIA_IRQHandler           ; 4:  MCIa
                DCD     MCIB_IRQHandler           ; 5:  MCIb
                DCD     UART0_IRQHandler          ; 6:  UART0 - DUT FPGA
                DCD     UART1_IRQHandler          ; 7:  UART1 - DUT FPGA
                DCD     UART2_IRQHandler          ; 8:  UART2 - DUT FPGA
                DCD     UART4_IRQHandler          ; 9:  UART4 - not connected
                DCD     AACI_IRQHandler           ; 10: AACI / AC97
                DCD     CLCD_IRQHandler           ; 11: CLCD Combined Interrupt
                DCD     ENET_IRQHandler           ; 12: Ethernet
                DCD     USBDC_IRQHandler          ; 13: USB Device
                DCD     USBHC_IRQHandler          ; 14: USB Host Controller
                DCD     CHLCD_IRQHandler          ; 15: Character LCD
                DCD     FLEXRAY_IRQHandler        ; 16: Flexray
                DCD     CAN_IRQHandler            ; 17: CAN
                DCD     LIN_IRQHandler            ; 18: LIN
                DCD     I2C_IRQHandler            ; 19: I2C ADC/DAC
                DCD     0                         ; 20: Reserved
                DCD     0                         ; 21: Reserved
                DCD     0                         ; 22: Reserved
                DCD     0                         ; 23: Reserved
                DCD     0                         ; 24: Reserved
                DCD     0                         ; 25: Reserved
                DCD     0                         ; 26: Reserved
                DCD     0                         ; 27: Reserved
                DCD     CPU_CLCD_IRQHandler       ; 28: Reserved - CPU FPGA CLCD
                DCD     0                         ; 29: Reserved - CPU FPGA
                DCD     UART3_IRQHandler          ; 30: UART3    - CPU FPGA
                DCD     SPI_IRQHandler            ; 31: SPI Touchscreen - CPU FPGA


                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
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

                EXPORT  WDT_IRQHandler            [WEAK]
                EXPORT  RTC_IRQHandler            [WEAK]
                EXPORT  TIM0_IRQHandler           [WEAK]
                EXPORT  TIM2_IRQHandler           [WEAK]
                EXPORT  MCIA_IRQHandler           [WEAK]
                EXPORT  MCIB_IRQHandler           [WEAK]
                EXPORT  UART0_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  UART2_IRQHandler          [WEAK]
                EXPORT  UART3_IRQHandler          [WEAK]
                EXPORT  UART4_IRQHandler          [WEAK]
                EXPORT  AACI_IRQHandler           [WEAK]
                EXPORT  CLCD_IRQHandler           [WEAK]
                EXPORT  ENET_IRQHandler           [WEAK]
                EXPORT  USBDC_IRQHandler          [WEAK]
                EXPORT  USBHC_IRQHandler          [WEAK]
                EXPORT  CHLCD_IRQHandler          [WEAK]
                EXPORT  FLEXRAY_IRQHandler        [WEAK]
                EXPORT  CAN_IRQHandler            [WEAK]
                EXPORT  LIN_IRQHandler            [WEAK]
                EXPORT  I2C_IRQHandler            [WEAK]
                EXPORT  CPU_CLCD_IRQHandler       [WEAK]
                EXPORT  SPI_IRQHandler            [WEAK]

WDT_IRQHandler
RTC_IRQHandler
TIM0_IRQHandler
TIM2_IRQHandler
MCIA_IRQHandler
MCIB_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
UART4_IRQHandler
AACI_IRQHandler
CLCD_IRQHandler
ENET_IRQHandler
USBDC_IRQHandler
USBHC_IRQHandler
CHLCD_IRQHandler
FLEXRAY_IRQHandler
CAN_IRQHandler
LIN_IRQHandler
I2C_IRQHandler
CPU_CLCD_IRQHandler
SPI_IRQHandler
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
