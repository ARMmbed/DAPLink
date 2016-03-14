/*
** ###################################################################
**     Version:             rev. 2.1, 2013-06-24
**     Build:               b141218
**
**     Abstract:
**         Register bit field access macros.
**
**     Copyright (c) 2013 Freescale Semiconductor, Inc.
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2011-12-15)
**         Initial version
**     - rev. 2.0 (2012-03-19)
**         PDB Peripheral register structure updated.
**         DMA Registers and bits for unsupported DMA channels removed.
**     - rev. 2.1 (2013-06-24)
**         NV_FOPT register - NMI_DIS bit added.
**
** ###################################################################
*/

#ifndef _FSL_BITACCESS_H
#define _FSL_BITACCESS_H  1

#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Macro to access a single bit of a 32-bit peripheral register (bit band region
 *        0x40000000 to 0x400FFFFF) using the bit-band alias region access.
 * @param Reg Register to access.
 * @param Bit Bit number to access.
 * @return Value of the targeted bit in the bit band region.
 */
#define BITBAND_ACCESS32(Reg,Bit) (*((uint32_t volatile*)(0x42000000u + (32u*((uintptr_t)(Reg) - (uintptr_t)0x40000000u)) + (4u*((uintptr_t)(Bit))))))

/**
 * @brief Macro to access a single bit of a 16-bit peripheral register (bit band region
 *        0x40000000 to 0x400FFFFF) using the bit-band alias region access.
 * @param Reg Register to access.
 * @param Bit Bit number to access.
 * @return Value of the targeted bit in the bit band region.
 */
#define BITBAND_ACCESS16(Reg,Bit) (*((uint16_t volatile*)(0x42000000u + (32u*((uintptr_t)(Reg) - (uintptr_t)0x40000000u)) + (4u*((uintptr_t)(Bit))))))

/**
 * @brief Macro to access a single bit of an 8-bit peripheral register (bit band region
 *        0x40000000 to 0x400FFFFF) using the bit-band alias region access.
 * @param Reg Register to access.
 * @param Bit Bit number to access.
 * @return Value of the targeted bit in the bit band region.
 */
#define BITBAND_ACCESS8(Reg,Bit) (*((uint8_t volatile*)(0x42000000u + (32u*((uintptr_t)(Reg) - (uintptr_t)0x40000000u)) + (4u*((uintptr_t)(Bit))))))

#endif /* _FSL_BITACCESS_H */

/******************************************************************************/
