/*
 * Copyright (c) 2013, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __PLATFORM_COMMON_H__
#define __PLATFORM_COMMON_H__

#include "stdio.h"
#include "stdarg.h"
#include "stdint.h"
#include "stdbool.h"
#include "fsl_platform_status.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

//! @name Min/max macros
//@{
#if !defined(MIN)
    #define MIN(a, b)   ((a) < (b) ? (a) : (b))
#endif

#if !defined(MAX)
    #define MAX(a, b)   ((a) > (b) ? (a) : (b))
#endif
//@}

//! @brief Computes the number of elements in an array.
#define ARRAY_SIZE(x)    (sizeof(x) / sizeof((x)[0]))

//! @name Byte swap macros
//@{
#define BSWAP_16(x)    (uint16_t)((((x) & 0xFF00) >> 0x8) | (((x) & 0xFF) << 0x8))
#define BSWAP_32(val)  (uint32_t)((BSWAP_16((uint32_t)(val) & (uint32_t)0xFFFF) << 0x10) |  \
                                  (BSWAP_16((uint32_t)((val) >> 0x10))))
//@}

//! @name Alignment macros
//@{
#define ALIGN_DOWN(x, a)    ((x) & -(a))
#define ALIGN_UP(x, a)      (-(-(x) & -(a)))
//@}

//! @brief Build a 32-bit code from four character values.
//!
//! The resulting value is built with a byte order such that the string
//! being readable in expected order when viewed in a hex editor, if the value
//! is treated as a 32-bit little endian value.
#define FOUR_CHAR_CODE(a,b,c,d) (((d) << 24) | ((c) << 16) | ((b) << 8) | ((a)))

#if (defined(DEBUG) || defined(_DEBUG)) && !defined(DEBUG_PRINT_DISABLE)
static inline void debug_printf(const char * format, ...);

//! @brief Debug print utility.
//!
//! This print function will only output text when the @a DEBUG macro is defined.
static inline void debug_printf(const char * format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
// Temporarily disable MISRA rule 14.2
#if defined(__ICCARM__)
#pragma diag_suppress=Pm049
#endif
    va_end(args);
#if defined(__ICCARM__)
#pragma diag_default=Pm049
#endif
}
#else // (DEBUG || _DEBUG) && !DEBUG_PRINT_DISABLE
// Empty macro to cause debug_printf() calls to disappear.
#define debug_printf(x, ...) do {} while (false)
#endif // (DEBUG || _DEBUG) && !DEBUG_PRINT_DISABLE

#endif // __PLATFORM_COMMON_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

