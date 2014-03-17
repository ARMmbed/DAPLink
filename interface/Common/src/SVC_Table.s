; * CMSIS-DAP Interface Firmware
; * Copyright (c) 2009-2013 ARM Limited
; *
; * Licensed under the Apache License, Version 2.0 (the "License");
; * you may not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; *     http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an "AS IS" BASIS,
; * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.

                AREA    SVC_TABLE, CODE, READONLY

                EXPORT  SVC_Count

SVC_Cnt         EQU    (SVC_End-SVC_Table)/4
SVC_Count       DCD     SVC_Cnt

; Import user SVC functions here.
                IMPORT  __SVC_1
                IMPORT  __SVC_2[WEAK]

                EXPORT  SVC_Table
SVC_Table
; Insert user SVC functions here. SVC 0 used by RTL Kernel.
                DCD     __SVC_1               ; user SVC function
                DCD     __SVC_2               ; user SVC function

SVC_End

                END

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
