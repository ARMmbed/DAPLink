/**
 * @file    debug_ca.h
 * @brief   Access to ARM DAP (Cortex-A) using CMSIS-DAP protocol
 *
 * DAPLink Interface Firmware
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

#ifndef DEBUG_CA_H
#define DEBUG_CA_H

#include "DAP.h"

#ifdef __cplusplus
extern "C" {
#endif

// Abort Register definitions
#define DAPABORT       0x00000001  // DAP Abort
#define STKCMPCLR      0x00000002  // Clear STICKYCMP Flag (SW Only)
#define STKERRCLR      0x00000004  // Clear STICKYERR Flag (SW Only)
#define WDERRCLR       0x00000008  // Clear WDATAERR Flag (SW Only)
#define ORUNERRCLR     0x00000010  // Clear STICKYORUN Flag (SW Only)

// Debug Control and Status definitions
#define ORUNDETECT     0x00000001  // Overrun Detect
#define STICKYORUN     0x00000002  // Sticky Overrun
#define TRNMODE        0x0000000C  // Transfer Mode Mask
#define TRNNORMAL      0x00000000  // Transfer Mode: Normal
#define TRNVERIFY      0x00000004  // Transfer Mode: Pushed Verify
#define TRNCOMPARE     0x00000008  // Transfer Mode: Pushed Compare
#define STICKYCMP      0x00000010  // Sticky Compare
#define STICKYERR      0x00000020  // Sticky Error
#define READOK         0x00000040  // Read OK (SW Only)
#define WDATAERR       0x00000080  // Write Data Error (SW Only)
#define MASKLANE       0x00000F00  // Mask Lane Mask
#define MASKLANE0      0x00000100  // Mask Lane 0
#define MASKLANE1      0x00000200  // Mask Lane 1
#define MASKLANE2      0x00000400  // Mask Lane 2
#define MASKLANE3      0x00000800  // Mask Lane 3
#define TRNCNT         0x001FF000  // Transaction Counter Mask
#define CDBGRSTREQ     0x04000000  // Debug Reset Request
#define CDBGRSTACK     0x08000000  // Debug Reset Acknowledge
#define CDBGPWRUPREQ   0x10000000  // Debug Power-up Request
#define CDBGPWRUPACK   0x20000000  // Debug Power-up Acknowledge
#define CSYSPWRUPREQ   0x40000000  // System Power-up Request
#define CSYSPWRUPACK   0x80000000  // System Power-up Acknowledge

// Debug Select Register definitions
#define CTRLSEL        0x00000001  // CTRLSEL (SW Only)
#define APBANKSEL      0x000000F0  // APBANKSEL Mask
#define APSEL          0xFF000000  // APSEL Mask

// Access Port Register Addresses
#define AP_CSW         0x00        // Control and Status Word
#define AP_TAR         0x04        // Transfer Address
#define AP_DRW         0x0C        // Data Read/Write
#define AP_BD0         0x10        // Banked Data 0
#define AP_BD1         0x14        // Banked Data 1
#define AP_BD2         0x18        // Banked Data 2
#define AP_BD3         0x1C        // Banked Data 3
#define AP_ROM         0xF8        // Debug ROM Address
#define AP_IDR         0xFC        // Identification Register

// AP Control and Status Word definitions
#define CSW_SIZE       0x00000007  // Access Size: Selection Mask
#define CSW_SIZE8      0x00000000  // Access Size: 8-bit
#define CSW_SIZE16     0x00000001  // Access Size: 16-bit
#define CSW_SIZE32     0x00000002  // Access Size: 32-bit
#define CSW_ADDRINC    0x00000030  // Auto Address Increment Mask
#define CSW_NADDRINC   0x00000000  // No Address Increment
#define CSW_SADDRINC   0x00000010  // Single Address Increment
#define CSW_PADDRINC   0x00000020  // Packed Address Increment
#define CSW_DBGSTAT    0x00000040  // Debug Status
#define CSW_TINPROG    0x00000080  // Transfer in progress
#define CSW_HPROT      0x02000000  // User/Privilege Control
#define CSW_MSTRTYPE   0x20000000  // Master Type Mask
#define CSW_MSTRCORE   0x00000000  // Master Type: Core
#define CSW_MSTRDBG    0x60000000  // Master Type: Debug
#define CSW_RESERVED   0x01000000  // Reserved Value

// Core Debug Register Address Offsets
#define DBG_OFS        0x0DF0      // Debug Register Offset inside NVIC
#define DBG_HCSR_OFS   0x00        // Debug Halting Control & Status Register
#define DBG_CRSR_OFS   0x04        // Debug Core Register Selector Register
#define DBG_CRDR_OFS   0x08        // Debug Core Register Data Register
#define DBG_EMCR_OFS   0x0C        // Debug Exception & Monitor Control Register

// Core Debug Register Addresses
#define DBG_HCSR       (DBG_Addr + DBG_HCSR_OFS)
#define DBG_CRSR       (DBG_Addr + DBG_CRSR_OFS)
#define DBG_CRDR       (DBG_Addr + DBG_CRDR_OFS)
#define DBG_EMCR       (DBG_Addr + DBG_EMCR_OFS)

// Debug Halting Control and Status Register definitions
#define C_DEBUGEN      0x00000001  // Debug Enable
#define C_HALT         0x00000002  // Halt
#define C_STEP         0x00000004  // Step
#define C_MASKINTS     0x00000008  // Mask Interrupts
#define C_SNAPSTALL    0x00000020  // Snap Stall
#define S_REGRDY       0x00010000  // Register R/W Ready Flag
#define S_HALT         0x00020000  // Halt Flag
#define S_SLEEP        0x00040000  // Sleep Flag
#define S_LOCKUP       0x00080000  // Lockup Flag
#define S_RETIRE_ST    0x01000000  // Sticky Retire Flag
#define S_RESET_ST     0x02000000  // Sticky Reset Flag
#define DBGKEY         0xA05F0000  // Debug Key

// Debug Exception and Monitor Control Register definitions
#define VC_CORERESET   0x00000001  // Reset Vector Catch
#define VC_MMERR       0x00000010  // Debug Trap on MMU Fault
#define VC_NOCPERR     0x00000020  // Debug Trap on No Coprocessor Fault
#define VC_CHKERR      0x00000040  // Debug Trap on Checking Error Fault
#define VC_STATERR     0x00000080  // Debug Trap on State Error Fault
#define VC_BUSERR      0x00000100  // Debug Trap on Bus Error Fault
#define VC_INTERR      0x00000200  // Debug Trap on Interrupt Error Fault
#define VC_HARDERR     0x00000400  // Debug Trap on Hard Fault
#define MON_EN         0x00010000  // Monitor Enable
#define MON_PEND       0x00020000  // Monitor Pend
#define MON_STEP       0x00040000  // Monitor Step
#define MON_REQ        0x00080000  // Monitor Request
#define TRCENA         0x01000000  // Trace Enable (DWT, ITM, ETM, TPIU)

// NVIC: Interrupt Controller Type Register
#define NVIC_ICT       (NVIC_Addr + 0x0004)
#define INTLINESNUM    0x0000001F  // Interrupt Line Numbers

// NVIC: CPUID Base Register
#define NVIC_CPUID     (NVIC_Addr + 0x0D00)
#define CPUID_PARTNO   0x0000FFF0  // Part Number Mask
#define CPUID_REVISION 0x0000000F  // Revision Mask
#define CPUID_VARIANT  0x00F00000  // Variant Mask

// NVIC: Application Interrupt/Reset Control Register
#define NVIC_AIRCR     (NVIC_Addr + 0x0D0C)
#define VECTRESET      0x00000001  // Reset Cortex-M (except Debug)
#define VECTCLRACTIVE  0x00000002  // Clear Active Vector Bit
#define SYSRESETREQ    0x00000004  // Reset System (except Debug)
#define VECTKEY        0x05FA0000  // Write Key

// NVIC: Debug Fault Status Register
#define NVIC_DFSR      (NVIC_Addr + 0x0D30)
#define HALTED         0x00000001  // Halt Flag
#define BKPT           0x00000002  // BKPT Flag
#define DWTTRAP        0x00000004  // DWT Match
#define VCATCH         0x00000008  // Vector Catch Flag
#define EXTERNAL       0x00000010  // External Debug Request

// Debug registers
#define DEBUG_REGSITER_BASE (0x80030000)

#define DBGDIDR         (DEBUG_REGSITER_BASE +  (0    * 4))   // Debug ID
#define DBGWFAR         (DEBUG_REGSITER_BASE +  (6    * 4))   // Watchpoint Fault Address
#define DBGVCR          (DEBUG_REGSITER_BASE +  (7    * 4))   // Vector Catch
#define DBGECR          (DEBUG_REGSITER_BASE +  (9    * 4))   // Event Catch
#define DBGDTRRX        (DEBUG_REGSITER_BASE +  (32   * 4))   // Host to Target Data Transfer
#define DBGITR          (DEBUG_REGSITER_BASE +  (33   * 4))   // WO Instruction Transfer
#define DBGDSCR         (DEBUG_REGSITER_BASE +  (34   * 4))   // Debug Status and Control
#define DBGDTRTX        (DEBUG_REGSITER_BASE +  (35   * 4))   // Target to Host Data Transfer
#define DBGDRCR         (DEBUG_REGSITER_BASE +  (36   * 4))   // Debug Run Control
#define DBGEACR         (DEBUG_REGSITER_BASE +  (37   * 4))   // External Auxiliary Control
#define DBGPCSR         (DEBUG_REGSITER_BASE +  (40   * 4))   // Program Counter Sampling
#define DBGCIDSR        (DEBUG_REGSITER_BASE +  (41   * 4))   // Context ID Sampling
#define DBGVIDSR        (DEBUG_REGSITER_BASE +  (42   * 4))   // Virtualization ID Sampling
#define DBGBVR0         (DEBUG_REGSITER_BASE +  (64   * 4))   // Breakpoint Value
#define DBGBVR1         (DEBUG_REGSITER_BASE +  (65   * 4))   // Breakpoint Value
#define DBGBVR2         (DEBUG_REGSITER_BASE +  (66   * 4))   // Breakpoint Value
#define DBGBVR3         (DEBUG_REGSITER_BASE +  (67   * 4))   // Breakpoint Value
#define DBGBVR4         (DEBUG_REGSITER_BASE +  (68   * 4))   // Breakpoint Value
#define DBGBVR5         (DEBUG_REGSITER_BASE +  (69   * 4))   // Breakpoint Value
#define DBGBVR6         (DEBUG_REGSITER_BASE +  (70   * 4))   // Breakpoint Value
#define DBGBVR7         (DEBUG_REGSITER_BASE +  (71   * 4))   // Breakpoint Value
#define DBGBVR8         (DEBUG_REGSITER_BASE +  (72   * 4))   // Breakpoint Value
#define DBGBVR9         (DEBUG_REGSITER_BASE +  (73   * 4))   // Breakpoint Value
#define DBGBVR10        (DEBUG_REGSITER_BASE +  (74   * 4))   // Breakpoint Value
#define DBGBVR11        (DEBUG_REGSITER_BASE +  (75   * 4))   // Breakpoint Value
#define DBGBVR12        (DEBUG_REGSITER_BASE +  (76   * 4))   // Breakpoint Value
#define DBGBVR13        (DEBUG_REGSITER_BASE +  (77   * 4))   // Breakpoint Value
#define DBGBVR14        (DEBUG_REGSITER_BASE +  (78   * 4))   // Breakpoint Value
#define DBGBVR15        (DEBUG_REGSITER_BASE +  (79   * 4))   // Breakpoint Value
#define DBGBCR0         (DEBUG_REGSITER_BASE +  (80   * 4))   // Breakpoint Control
#define DBGBCR1         (DEBUG_REGSITER_BASE +  (81   * 4))   // Breakpoint Control
#define DBGBCR2         (DEBUG_REGSITER_BASE +  (82   * 4))   // Breakpoint Control
#define DBGBCR3         (DEBUG_REGSITER_BASE +  (83   * 4))   // Breakpoint Control
#define DBGBCR4         (DEBUG_REGSITER_BASE +  (84   * 4))   // Breakpoint Control
#define DBGBCR5         (DEBUG_REGSITER_BASE +  (85   * 4))   // Breakpoint Control
#define DBGBCR6         (DEBUG_REGSITER_BASE +  (86   * 4))   // Breakpoint Control
#define DBGBCR7         (DEBUG_REGSITER_BASE +  (87   * 4))   // Breakpoint Control
#define DBGBCR8         (DEBUG_REGSITER_BASE +  (88   * 4))   // Breakpoint Control
#define DBGBCR9         (DEBUG_REGSITER_BASE +  (89   * 4))   // Breakpoint Control
#define DBGBCR10        (DEBUG_REGSITER_BASE +  (90   * 4))   // Breakpoint Control
#define DBGBCR11        (DEBUG_REGSITER_BASE +  (91   * 4))   // Breakpoint Control
#define DBGBCR12        (DEBUG_REGSITER_BASE +  (92   * 4))   // Breakpoint Control
#define DBGBCR13        (DEBUG_REGSITER_BASE +  (93   * 4))   // Breakpoint Control
#define DBGBCR14        (DEBUG_REGSITER_BASE +  (94   * 4))   // Breakpoint Control
#define DBGBCR15        (DEBUG_REGSITER_BASE +  (95   * 4))   // Breakpoint Control
#define DBGWVR0         (DEBUG_REGSITER_BASE +  (96   * 4))   // Watchpoint Value
#define DBGWVR1         (DEBUG_REGSITER_BASE +  (97   * 4))   // Watchpoint Value
#define DBGWVR2         (DEBUG_REGSITER_BASE +  (98   * 4))   // Watchpoint Value
#define DBGWVR3         (DEBUG_REGSITER_BASE +  (99   * 4))   // Watchpoint Value
#define DBGWVR4         (DEBUG_REGSITER_BASE +  (100  * 4))   // Watchpoint Value
#define DBGWVR5         (DEBUG_REGSITER_BASE +  (101  * 4))   // Watchpoint Value
#define DBGWVR6         (DEBUG_REGSITER_BASE +  (102  * 4))   // Watchpoint Value
#define DBGWVR7         (DEBUG_REGSITER_BASE +  (103  * 4))   // Watchpoint Value
#define DBGWVR8         (DEBUG_REGSITER_BASE +  (104  * 4))   // Watchpoint Value
#define DBGWVR9         (DEBUG_REGSITER_BASE +  (105  * 4))   // Watchpoint Value
#define DBGWVR10        (DEBUG_REGSITER_BASE +  (106  * 4))   // Watchpoint Value
#define DBGWVR11        (DEBUG_REGSITER_BASE +  (107  * 4))   // Watchpoint Value
#define DBGWVR12        (DEBUG_REGSITER_BASE +  (108  * 4))   // Watchpoint Value
#define DBGWVR13        (DEBUG_REGSITER_BASE +  (109  * 4))   // Watchpoint Value
#define DBGWVR14        (DEBUG_REGSITER_BASE +  (110  * 4))   // Watchpoint Value
#define DBGWVR15        (DEBUG_REGSITER_BASE +  (111  * 4))   // Watchpoint Value
#define DBGWCR0         (DEBUG_REGSITER_BASE +  (112  * 4))   // Watchpoint Control
#define DBGWCR1         (DEBUG_REGSITER_BASE +  (113  * 4))   // Watchpoint Control
#define DBGWCR2         (DEBUG_REGSITER_BASE +  (114  * 4))   // Watchpoint Control
#define DBGWCR3         (DEBUG_REGSITER_BASE +  (115  * 4))   // Watchpoint Control
#define DBGWCR4         (DEBUG_REGSITER_BASE +  (116  * 4))   // Watchpoint Control
#define DBGWCR5         (DEBUG_REGSITER_BASE +  (117  * 4))   // Watchpoint Control
#define DBGWCR6         (DEBUG_REGSITER_BASE +  (118  * 4))   // Watchpoint Control
#define DBGWCR7         (DEBUG_REGSITER_BASE +  (119  * 4))   // Watchpoint Control
#define DBGWCR8         (DEBUG_REGSITER_BASE +  (120  * 4))   // Watchpoint Control
#define DBGWCR9         (DEBUG_REGSITER_BASE +  (121  * 4))   // Watchpoint Control
#define DBGWCR10        (DEBUG_REGSITER_BASE +  (122  * 4))   // Watchpoint Control
#define DBGWCR11        (DEBUG_REGSITER_BASE +  (123  * 4))   // Watchpoint Control
#define DBGWCR12        (DEBUG_REGSITER_BASE +  (124  * 4))   // Watchpoint Control
#define DBGWCR13        (DEBUG_REGSITER_BASE +  (125  * 4))   // Watchpoint Control
#define DBGWCR14        (DEBUG_REGSITER_BASE +  (126  * 4))   // Watchpoint Control
#define DBGWCR15        (DEBUG_REGSITER_BASE +  (127  * 4))   // Watchpoint Control
#define DBGBXVR0        (DEBUG_REGSITER_BASE +  (144  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR1        (DEBUG_REGSITER_BASE +  (145  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR2        (DEBUG_REGSITER_BASE +  (146  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR3        (DEBUG_REGSITER_BASE +  (147  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR4        (DEBUG_REGSITER_BASE +  (148  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR5        (DEBUG_REGSITER_BASE +  (149  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR6        (DEBUG_REGSITER_BASE +  (150  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR7        (DEBUG_REGSITER_BASE +  (151  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR8        (DEBUG_REGSITER_BASE +  (152  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR9        (DEBUG_REGSITER_BASE +  (153  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR10       (DEBUG_REGSITER_BASE +  (154  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR11       (DEBUG_REGSITER_BASE +  (155  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR12       (DEBUG_REGSITER_BASE +  (156  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR13       (DEBUG_REGSITER_BASE +  (157  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR14       (DEBUG_REGSITER_BASE +  (158  * 4))   // Breakpoint Extended Valueb
#define DBGBXVR15       (DEBUG_REGSITER_BASE +  (159  * 4))   // Breakpoint Extended Valueb
#define DBGOSLAR        (DEBUG_REGSITER_BASE +  (192  * 4))   // OS Lock Access
#define DBGOSLSR        (DEBUG_REGSITER_BASE +  (193  * 4))   // OS Lock Status
#define DBGPRCR         (DEBUG_REGSITER_BASE +  (196  * 4))   // Powerdown and Reset Control
#define DBGPRSR         (DEBUG_REGSITER_BASE +  (197  * 4))   // Powerdown and Reset Status
#define DBGITCTRL       (DEBUG_REGSITER_BASE +  (960  * 4))   // Integration Mode Control
#define DBGCLAIMSET     (DEBUG_REGSITER_BASE +  (1000 * 4))   // Claim Tag Set
#define DBGCLAIMCLR     (DEBUG_REGSITER_BASE +  (1001 * 4))   // Claim Tag Clear
#define DBGLAR          (DEBUG_REGSITER_BASE +  (1004 * 4))   // Lock Access
#define DBGLSR          (DEBUG_REGSITER_BASE +  (1005 * 4))   // Lock Status
#define DBGAUTHSTATUS   (DEBUG_REGSITER_BASE +  (1006 * 4))   // Authentication Status
#define DBGDEVID2       (DEBUG_REGSITER_BASE +  (1008 * 4))   // Debug Device ID 2
#define DBGDEVID1       (DEBUG_REGSITER_BASE +  (1009 * 4))   // Debug Device ID 1
#define DBGDEVID        (DEBUG_REGSITER_BASE +  (1010 * 4))   // Debug Device ID
#define DBGDEVTYPE      (DEBUG_REGSITER_BASE +  (1011 * 4))   // Device Type
#define DBGPID0         (DEBUG_REGSITER_BASE +  (1012 * 4))   // Debug Peripheral ID
#define DBGPID1         (DEBUG_REGSITER_BASE +  (1013 * 4))   // Debug Peripheral ID
#define DBGPID2         (DEBUG_REGSITER_BASE +  (1014 * 4))   // Debug Peripheral ID
#define DBGPID3         (DEBUG_REGSITER_BASE +  (1015 * 4))   // Debug Peripheral ID
#define DBGPID4         (DEBUG_REGSITER_BASE +  (1016 * 4))   // Debug Peripheral ID
#define DBGCID0         (DEBUG_REGSITER_BASE +  (1020 * 4))   // Debug Component ID
#define DBGCID1         (DEBUG_REGSITER_BASE +  (1021 * 4))   // Debug Component ID
#define DBGCID2         (DEBUG_REGSITER_BASE +  (1022 * 4))   // Debug Component ID
#define DBGCID3         (DEBUG_REGSITER_BASE +  (1023 * 4))   // Debug Component ID

#ifdef __cplusplus
}
#endif

#endif
