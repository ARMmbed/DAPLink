/**
 * @file    swdp_sgpio.c
 * @brief   SWD implemented using LPC4322's SGPIOs.
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2022, ARM Limited, All Rights Reserved
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

#ifdef SWDP_SGPIO

#include "LPC43xx.h"
#include "DAP_config.h"
#include "DAP.h"

// Referenced documents:
// - ID072320: Arm Debug Interface Architecture Specification ARM IHI 0031F.
// - UM10503: LPC43xx ARM Cortex-M4/M0 multi-core microcontroller User Manual, Rev. 2.4 — 22 August 2018.
// - LPC435X_3X_2X_1X: LPC435x/3x/2x/1x Product data sheet, Rev. 5.4 — 10 January 2020.

#define SLICE_D                                  3
#define SLICE_F                                  5
#define SLICE_G                                  6
#define SLICE_H                                  7
#define SLICE_M                                  12
#define SLICE_N                                  13
#define SLICE_O                                  14
#define SLICE_P                                  15

// "Pin multiplexer configuration registers" UM10503.
// P_OUT_CFG
#define OUT_MUX_CFG_CLK_OUT                      0x08
#define OUT_MUX_CFG_DOUT_DOUTM2C                 0x03
#define OUT_MUX_CFG_DOUT_DOUTM8B                 0x0A
// P_OE_CFG
#define OUT_MUX_CFG_GPIO_OE                      0x00
#define OUT_MUX_CFG_DOUT_OEM1                    0x40
#define OUT_MUX_CFG_DOUT_OEM2                    0x50

// "SGPIO multiplexer configuration registers" UM10503.
#define SGPIO_MUX_CFG_EXT_CLK_ENABLE_INTERNAL    (0b00 << 0)
#define SGPIO_MUX_CFG_EXT_CLK_ENABLE_EXTERNAL    (0b01 << 0)

#define SGPIO_MUX_CFG_CLK_SOURCE_PIN_MODE_8      (0b00 << 1)
#define SGPIO_MUX_CFG_CLK_SOURCE_PIN_MODE_9      (0b01 << 1)
#define SGPIO_MUX_CFG_CLK_SOURCE_PIN_MODE_10     (0b10 << 1)
#define SGPIO_MUX_CFG_CLK_SOURCE_PIN_MODE_11     (0b11 << 1)

#define SGPIO_MUX_CFG_CLK_SOURCE_SLICE_MODE_D    (0b00 << 3)
#define SGPIO_MUX_CFG_CLK_SOURCE_SLICE_MODE_H    (0b01 << 3)
#define SGPIO_MUX_CFG_CLK_SOURCE_SLICE_MODE_O    (0b10 << 3)
#define SGPIO_MUX_CFG_CLK_SOURCE_SLICE_MODE_P    (0b11 << 3)

#define SGPIO_MUX_CFG_QUALIFIER_MODE_DEFAULT     (0b00 << 5)
#define SGPIO_MUX_CFG_QUALIFIER_MODE_DISABLE     (0b01 << 5)
#define SGPIO_MUX_CFG_QUALIFIER_MODE_SLICE       (0b10 << 5)
#define SGPIO_MUX_CFG_QUALIFIER_MODE_PIN         (0b11 << 5)

#define SGPIO_MUX_CFG_QUALIFIER_PIN_MODE_8       (0b00 << 7)
#define SGPIO_MUX_CFG_QUALIFIER_PIN_MODE_9       (0b01 << 7)
#define SGPIO_MUX_CFG_QUALIFIER_PIN_MODE_10      (0b10 << 7)
#define SGPIO_MUX_CFG_QUALIFIER_PIN_MODE_11      (0b11 << 7)

#define SGPIO_MUX_CFG_QUALIFIER_SLICE_MODE_A     (0b00 << 9)
#define SGPIO_MUX_CFG_QUALIFIER_SLICE_MODE_H     (0b01 << 9)
#define SGPIO_MUX_CFG_QUALIFIER_SLICE_MODE_I     (0b10 << 9)
#define SGPIO_MUX_CFG_QUALIFIER_SLICE_MODE_P     (0b11 << 9)

#define SGPIO_MUX_CFG_CONCAT_EXT_PIN             (0b00 << 11)
#define SGPIO_MUX_CFG_CONCAT_DISABLE             (0b00 << 11)
#define SGPIO_MUX_CFG_CONCAT_ENABLE              (0b01 << 11)

#define SGPIO_MUX_CFG_CONCAT_ORDER_LOOP          (0b00 << 12)
#define SGPIO_MUX_CFG_CONCAT_ORDER_2SLC          (0b01 << 12)
#define SGPIO_MUX_CFG_CONCAT_ORDER_4SLC          (0b10 << 12)
#define SGPIO_MUX_CFG_CONCAT_ORDER_8SLC          (0b11 << 12)


////////////////////////////////////////////////////////////
// "Slice multiplexer configuration registers" UM10503.
#define SLICE_MUX_CFG_MATCH_MODE_DISABLED        (0b00 << 0)
#define SLICE_MUX_CFG_MATCH_MODE_ENABLED         (0b01 << 0)

#define SLICE_MUX_CFG_CLK_CAPTURE_MODE_RISING    (0b00 << 1)
#define SLICE_MUX_CFG_CLK_CAPTURE_MODE_FALLING   (0b01 << 1)

#define SLICE_MUX_CFG_CLKGEN_MODE_INTERNAL       (0b00 << 2)
#define SLICE_MUX_CFG_CLKGEN_MODE_EXTERNAL       (0b01 << 2)
#define SLICE_MUX_CFG_INV_OUT_CLK_NORMAL         (0b00 << 3)
#define SLICE_MUX_CFG_INV_OUT_CLK_INVERTED       (0b01 << 3)

#define SLICE_MUX_CFG_DATA_CAPTURE_MODE_RISING   (0b00 << 4)
#define SLICE_MUX_CFG_DATA_CAPTURE_MODE_FALLING  (0b01 << 4)
#define SLICE_MUX_CFG_DATA_CAPTURE_MODE_LOW      (0b10 << 4)
#define SLICE_MUX_CFG_DATA_CAPTURE_MODE_HIGH     (0b11 << 4)

#define SLICE_MUX_CFG_PARALLEL_MODE_1BPCLK       (0b00 << 6)
#define SLICE_MUX_CFG_PARALLEL_MODE_2BPCLK       (0b01 << 6)
#define SLICE_MUX_CFG_PARALLEL_MODE_4BPCLK       (0b10 << 6)
#define SLICE_MUX_CFG_PARALLEL_MODE_8BPCLK       (0b11 << 6)

#define SLICE_MUX_CFG_INV_QUALIFIER_NORMAL       (0b00 << 8)
#define SLICE_MUX_CFG_INV_QUALIFIER_INVERTED     (0b01 << 8)

#define POS_POS(n)                               (n? ((n-1)&0xFF)<<0 : 0) // value at start
#define POS_PRESET(n)                            (n? ((n-1)&0xFF)<<8 : 0) // PRESET_POS can be used to indicate word boundaries for words that are not a multiple of 32 bit

#define PRESET_MAX                               (1<<12)


// "Pin multiplexing" UM10503.
// "SGPIO multiplexer" UM10503.
// "Slice I/O Multiplexing" UM10503.
// "Output pin multiplexing" UM10503.
//
//
// SWCLK/TCK.
//  SLICE_N     SWCLK
// [31....0] -> SGPIO11/P1_17/TCK_SWCLK.
//
#define SGPIO_SWCLK_TCK                          11
// Slice N can act as Clock on SGPIO11.
#define SLICE_SWCLK_TCK                          SLICE_N
#define SLICE_SWCLK_TCK_MASK                     (1 << SLICE_SWCLK_TCK)


// SWDIO/TMS.
// Input slices.
//
// SGPIO14 is connected to H31 (bit 31 of slice H).
//
//                  SWDIO                               SLICE_H      SLICE_P
// External Data -> TMS_SWDIO/P1_6/SGPIO14 -> [31....0] -> [31....0]
#define SGPIO_SWDIO_TMS                          14
#define SLICE_SWDIO_TMS_DIN1                     SLICE_P
#define SLICE_SWDIO_TMS_DIN1_MASK                (1 << SLICE_SWDIO_TMS_DIN1)
#define SLICE_SWDIO_TMS_DIN0                     SLICE_H
#define SLICE_SWDIO_TMS_DIN0_MASK                (1 << SLICE_SWDIO_TMS_DIN0)

// SWDIO/TMS.
// Output slices.
//
//  SLICE_D      SLICE_O     SWDIO.
// [31....0] -> [31....0] -> SGPIO14/P1_6/TMS_SWDIO -> External Data.
#define SLICE_SWDIO_TMS_DOUT1                    SLICE_D
#define SLICE_SWDIO_TMS_DOUT1_MASK               (1 << SLICE_SWDIO_TMS_DOUT1)
#define SLICE_SWDIO_TMS_DOUT0                    SLICE_O
#define SLICE_SWDIO_TMS_DOUT0_MASK               (1 << SLICE_SWDIO_TMS_DOUT0)

// SWDIO/TMS.
//  SLICE_F
// [31....0] -> SGPIO14_P_OE_CFG.
#define SLICE_SWDIO_TMS_OE                       SLICE_F
#define SLICE_SWDIO_TMS_OE_MASK                  (1 << SLICE_SWDIO_TMS_OE)



// SWDIO/TXEN.
//  SLICE_M
// [31....0] -> SGPIO15/P1_5/TMS_SWDIO_TXEN.
#define SGPIO_SWDIO_TXEN                         15
#define SLICE_SWDIO_TXEN                         SLICE_M
#define SLICE_SWDIO_TXEN_MASK                    (1 << SLICE_SWDIO_TXEN)


#define SLICE_POS(s)                             LPC_SGPIO->POS[s]
#define SLICE_CNT(s)                             LPC_SGPIO->COUNT[s]
#define SLICE_REG(s)                             LPC_SGPIO->REG[s]
#define SLICE_SHD(s)                             LPC_SGPIO->REG_SS[s]


#define REQUEST_READ(request)                    (request & DAP_TRANSFER_RnW)
#define REQUEST_TIMESTAMP(request)               (request & DAP_TRANSFER_TIMESTAMP)
#define REQUEST_WRITE(request)                   ((request & DAP_TRANSFER_RnW) == 0U)


#define SGPIO_CLK_HZ   120000000
#define XTAL_OSC_HZ     12000000



typedef struct _slice_reg {
    volatile uint32_t* addr;
    uint32_t           slice_mask;
} slice_reg_t;



typedef struct _swdp_sgpio_context {
    uint32_t shift_clk;
    uint32_t preset;
} swdp_sgpio_context_t;



static swdp_sgpio_context_t s_context;



__STATIC_FORCEINLINE uint8_t PARITY_U32(uint32_t v)
{
    v ^= v >> 16;
    v ^= v >> 8;
    v ^= v >> 4;
    v &= 0xF;
    return (0x6996 >> v) & 1;
}



__STATIC_FORCEINLINE uint8_t PARITY_U8(uint8_t v)
{
    v ^= v >> 4;
    v &= 0xF;
    return (0x6996 >> v) & 1;
}



__STATIC_FORCEINLINE void slice_reg_writeU8(uint32_t nb_bytes, const uint8_t *src, volatile uint32_t *dst)
{
    uint32_t rv = 0;

    for (unsigned i = 0; i < nb_bytes; ++i)
    {
        rv |= *src++<<(i*8);
    }
    *dst = rv;
}



__STATIC_FORCEINLINE void slice_reg_readU8(uint32_t nb_bytes, volatile uint32_t *src, uint8_t *dst)
{
    for (unsigned i = 0; i < nb_bytes; ++i)
    {
        *dst++ = (*src>>(i*8))&0xFF;
    }
}



// Enable the Output SGPIO pin.
// "GPIO output enable register" UM10503.
__STATIC_FORCEINLINE void sgpio_pin_oenable(uint32_t pin)
{
    uint32_t cur_oenreg;
    cur_oenreg = LPC_SGPIO->GPIO_OENREG;
    LPC_SGPIO->GPIO_OENREG = cur_oenreg | (1 << pin);
}



// Disable the Output SGPIO pin.
// "GPIO output enable register" UM10503.
__STATIC_FORCEINLINE void sgpio_pin_odisable(uint32_t pin)
{
    uint32_t cur_oenreg;
    cur_oenreg = LPC_SGPIO->GPIO_OENREG;
    LPC_SGPIO->GPIO_OENREG = cur_oenreg & ~(1 << pin);
}



// Set the CLR_EN_1 register bit to clear
// the corresponding bit in the ENABLE_1 register.
// "Exchange clock interrupt clear mask register" UM10503.
__STATIC_FORCEINLINE void exchg_clk_int_off(uint32_t mask)
{
    LPC_SGPIO->CLR_EN_1 = mask;
}



// "Exchange clock interrupt clear status register" UM10503.
__STATIC_FORCEINLINE void exchg_clk_cleanup(uint32_t slice_mask)
{
    LPC_SGPIO->CLR_STATUS_1 = slice_mask;
}



// The STATUS_1 register bits are set when
// the shadow and data registers are exchanged.
// "Exchange clock interrupt status register" UM10503.
__STATIC_FORCEINLINE void exchg_clk_wait_on(uint32_t slice_mask)
{
    while ((LPC_SGPIO->STATUS_1 & slice_mask) != slice_mask) { }
}



// Enable the slices' COUNT counter.
// 1 = Starts COUNTn or external shift clock.
// 0 = Disables slice shift clock.
// When COUNT reaches zero, it is loaded with PRESET.
// Each time COUNT reaches zero the register shifts right;
// loading bit REG[31] with data captured from DIN and
// loading DOUT with bit REG[0].
// Thus COUNT controls the serial data rate.
__STATIC_FORCEINLINE void shift_clk_kickoff(uint32_t slice_mask)
{
    LPC_SGPIO->CTRL_ENABLE |= slice_mask;
}



__STATIC_FORCEINLINE void shift_clk_int_off(uint32_t mask)
{
    LPC_SGPIO->CLR_EN_0 = mask;
}



__STATIC_FORCEINLINE void shift_clk_disable(uint32_t slice_mask)
{
    LPC_SGPIO->CTRL_ENABLE &= ~slice_mask;
}



// multiplier: compute mdec from msel.
static unsigned mdec_new (unsigned msel)
{
#define PLL0_MSEL_MAX (1<<15)
    unsigned x=0x4000, im;
    switch (msel) {
        case 0: return 0xFFFFFFFF;
        case 1: return 0x18003;
        case 2: return 0x10003;
        default:
                for (im = msel; im <= PLL0_MSEL_MAX; im++)
                    x = ((x ^ x>>1) & 1) << 14 | x>>1 & 0xFFFF;
                return x;
    }
}



// pre-divider: compute ndec from nsel.
static unsigned ndec_new (unsigned nsel)
{
#define PLL0_NSEL_MAX (1<<8)
    unsigned x=0x80, in;
    switch (nsel) {
        case 0: return 0xFFFFFFFF;
        case 1: return 0x302;
        case 2: return 0x202;
        default:for (in = nsel; in <= PLL0_NSEL_MAX; in++)
                    x = ((x ^ x>>2 ^ x>>3 ^ x>>4) & 1) << 7 | x>>1 & 0xFF;
                return x;
    }
}



// post-divider: compute pdec from psel.
static unsigned pdec_new (unsigned psel)
{
#define PLL0_PSEL_MAX (1<<5)
    unsigned x=0x10, ip;
    switch (psel) {
        case 0: return 0xFFFFFFFF;
        case 1: return 0x62;
        case 2: return 0x42;
        default:for (ip = psel; ip <= PLL0_PSEL_MAX; ip++)
                    x = ((x ^ x>>2) & 1) << 4 | x>>1 & 0x3F;
                return x;
    }
}



static void sgpio_clock_init()
{
    uint32_t msel = SGPIO_CLK_HZ / XTAL_OSC_HZ;    // multiplier (mdec)
    uint32_t nsel = 1 - 1; // predivider (ndec)
    uint32_t psel = 1 - 1; // postdivider (pdec)

    // To get at the output of the PLL (clkout) the best phase-noise and jitter
    // performance, the highest possible reference clock (clkref) at the PFD
    // has to be used.
    // Therefore mode 1a and 1b are recommended,
    // when it is possible to make the right output frequency without pre-divider.
    // Setup the PLL0AUDIO:
    //   1. Power down the PLL0AUDIO by setting bit 0 in the PLL0 control register PLL0AUDIO_CTRL to 1.
    //      This step is only needed if the PLL0 is currently enabled.
    //   2. Configure the PLL0AUDIO m, n, and p divider values in the
    //      PLL0AUDIO_M and PLL0AUDIO_NP registers.
    //   3. Power up the PLL0AUDIO by setting bit 0 in the
    //      PLL0AUDIO_CTRL register to 0.
    //   4. Wait for the PLL0AUDIO to lock by monitoring the LOCK bit in the
    //      PLL0AUDIO_STAT register.
    //   5. Enable the PLL0AUDIO clock output in the
    //      PLL0AUDIO_CTRL register.
    //   Remark: You can change the PLL0AUDIO settings while the PLL0AUDIO is
    //           running when you need to configure the PLL0AUDIO for high
    //           output frequencies.
    //           (see "Configuring the BASE_M4_CLK for high operating frequencies" UM10503).
    //   The AUTOBLOCK bit re-synchronizes the clock output during frequency
    //   changes that prevents glitches when switching clock frequencies.
    LPC_CGU->PLL0AUDIO_CTRL = (1 << 0)    // PD: 1=power off.
                            | (1 << 2)    // DIRECTI: 1=bypass pre-divider.
                            | (0 << 3)    // DIRECTO: 0=don't bypass post-divider.
                            | (0 << 4)    // CLKEN: 0=Disabled, 1=Enabled.
                            | (1 << 11)   // AUTOBLOCK: 0=Disabled, 1=Enabled.
                            | (1 << 12)   // PLLFRACT_REQ: 1=Enable Fractional Divier.
                            | (0 << 13)   // SEL_EXT: 0=Enable Fractional Divider, 1=Enable MDEC.
                            | (0 << 14)   // MOD_PD: 0=PowerUp the Sigma-Delta modulator.
                            | (6 << 24)   // 6:XTALOSC@12 MHz
                            ;
    // "Using the fractional divider" UM10503
    // 1. Set bit PLLFRACT_REQ = 1 and SEL_EXT = 0
    //    in the PLL0AUDIO_CTRL register, "PLL0AUDIO control register" UM10503.
    // 2. Calculate NDEC, PDEC, and PLLFRACT_CTRL for the output frequency Fout.
    // 3. Write the calculated NDEC and PDEC values to the PLL0AUDIO_NP_DIV register.
    // 4. Write the calculated PLLFRACT_CTRL value to the PLL0AUDIOFRAC register.
    LPC_CGU->PLL0AUDIO_NP_DIV = (ndec_new(nsel) << 12) | pdec_new(psel);
    // [21:15] = m
    // [14:0] = fractional value
    LPC_CGU->PLL0AUDIO_FRAC = ((msel) << 15) | (0 & 0x7FFF);

    // Power On
    LPC_CGU->PLL0AUDIO_CTRL &= ~1;

    // Wait for lock
    while (!(LPC_CGU->PLL0AUDIO_STAT & 1))
    {}

    // Enable clock output.
    LPC_CGU->PLL0AUDIO_CTRL |= 1 << 4;

    // SGPIO
    // See Table 266 and Table 162 for SGPIO clocking and power control.
    // The SGPIO is reset by the SGPIO_RST (reset #57) "Reset output configuration" UM10503.
    LPC_CCU1->CLK_PERIPH_SGPIO_CFG = 1<<0; // Start the SGPIO clock.

    // "BASE_PERIPH_CLK control register" UM10503.
    LPC_CGU->BASE_PERIPH_CLK = 0 << 1      // Power-On
                             | 0 << 11     // Autoblock
                             | 8 << 24     // clk_sel = PLL0AUDIO
                             ;
    // "Reset control register 1" UM10503.
    LPC_RGU->RESET_CTRL1 = 1 << 25         // Reset SGPIO
                         ;
    while (LPC_RGU->RESET_ACTIVE_STATUS1 & (1<<25) == 0)
    {}
}



// frequency(SHIFT_CLOCK) = frequency(SGPIO_CLK_HZ) / (PRESET+1).
// PRESET = frequency(SGPIO_CLK_HZ)/frequency(SHIFT_CLK) - 1.
// "CCU1 branch clocks" UM10503.
static void shift_clk_preset()
{
    if (s_context.shift_clk == DAP_Data.nominal_clock)
        return;

    s_context.shift_clk = DAP_Data.nominal_clock;
    s_context.preset = SGPIO_CLK_HZ / s_context.shift_clk - 1;

    if (s_context.preset >= PRESET_MAX)
    {
        uint32_t tmp;
        uint32_t divisor = SGPIO_CLK_HZ / (PRESET_MAX * s_context.shift_clk) + 1;

        tmp = LPC_CGU->IDIVE_CTRL & ~(0xFF << 2);       // clear IDIV.
        LPC_CGU->IDIVE_CTRL = tmp | ((divisor - 1) & 0xFF) << 2;

        tmp = LPC_CGU->IDIVE_CTRL & ~(0x0F << 24);      // clear clk_sel.
        LPC_CGU->IDIVE_CTRL = tmp | 0x08 << 24;         // clk_sel = PLL0AUDIO.

        tmp = LPC_CGU->BASE_PERIPH_CLK & ~(0x0F << 24); // clear clk_sel.
        LPC_CGU->BASE_PERIPH_CLK = tmp | 0x10 << 24;    // clk_sel = IDIVE.

        s_context.preset = SGPIO_CLK_HZ / (divisor * s_context.shift_clk * 1.0) - 1;
    }
}



static void shift_clk_preset_init(void)
{
    shift_clk_preset();

    // The COUNT duration can be preset with register PRESET.
    LPC_SGPIO->PRESET[SLICE_SWCLK_TCK] = s_context.preset;

    // These use SGPIO11 as External Clock.
    LPC_SGPIO->PRESET[SLICE_SWDIO_TXEN] = 0;
    LPC_SGPIO->PRESET[SLICE_SWDIO_TMS_OE] = 0;
    LPC_SGPIO->PRESET[SLICE_SWDIO_TMS_DIN1] = 0;
    LPC_SGPIO->PRESET[SLICE_SWDIO_TMS_DIN0] = 0;
    LPC_SGPIO->PRESET[SLICE_SWDIO_TMS_DOUT0] = 0;
    LPC_SGPIO->PRESET[SLICE_SWDIO_TMS_DOUT1] = 0;
}



// 0 = Enables COUNT and POS counters.
//     The counters start counting when the
//     CTRL_EN bit or bits are set in the
//     CTRL_ENABLED register.
// 1 = Disable the POSi counter of slice i
//     when the POSi counter reaches a zero
//     count.
// The POS counter consists of an 8-bit counter that is decremented when COUNT equals zero.
// When POS reaches zero it is loaded with the PRESET_POS value.
// Once a slice is enabled, it's POS register should not be modified.
//
// If only one POSi countdown is needed
// (when only one slice should be processed),
// then this should be called after COUNTi
// is started with shift_clk_kickoff().
__STATIC_FORCEINLINE void slice_pos_oneshot(uint32_t slice_mask)
{
    LPC_SGPIO->CTRL_DISABLE |= slice_mask;
}



__STATIC_FORCEINLINE void slice_pos_disable(uint32_t slice_mask)
{
    LPC_SGPIO->CTRL_DISABLE &= ~slice_mask;
}



// B4.1.3 "Line turnaround" ID072320.
// To prevent contention, a turnaround period
// is required when the device driving the wire
// changes. For the turnaround period, neither
// the host nor the target drives the wire, and
// the state of the wire is undefined.
static void line_turnaround(uint32_t period)
{
    // drive SWCLK_TCK for: period * shift_clock cycles.
    SLICE_POS(SLICE_SWCLK_TCK) = POS_POS(period) | POS_PRESET(0);
    SLICE_POS(SLICE_SWDIO_TXEN) = POS_POS(period) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TXEN) = 0b00000000000000000000000000000000;
    SLICE_SHD(SLICE_SWDIO_TXEN) = 0;

    SLICE_POS(SLICE_SWDIO_TMS_OE) = POS_POS(period) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TMS_OE) = 0b00000000000000000000000000000000;
    SLICE_SHD(SLICE_SWDIO_TMS_OE) = 0;
    SLICE_REG(SLICE_SWDIO_TMS_DOUT0) = 0;
    SLICE_SHD(SLICE_SWDIO_TMS_DOUT0) = 0;
    exchg_clk_cleanup(SLICE_SWCLK_TCK_MASK);
    slice_pos_oneshot(SLICE_SWCLK_TCK_MASK);
    shift_clk_kickoff(SLICE_SWCLK_TCK_MASK);
    exchg_clk_wait_on(SLICE_SWCLK_TCK_MASK);
    shift_clk_disable(SLICE_SWCLK_TCK_MASK);
    slice_pos_disable(SLICE_SWCLK_TCK_MASK);
}



// B4.1.4 "Idle cycles" ID072320.
// The host clocks the SWD interface with the line LOW to insert idle cycles.
static void idle_cycles(uint32_t period)
{
    // drive SWCLK_TCK for: period * shift_clock cycles.
    SLICE_POS(SLICE_SWCLK_TCK) = POS_POS(period) | POS_PRESET(0);
    SLICE_POS(SLICE_SWDIO_TXEN) = POS_POS(32) | POS_PRESET(32);
    SLICE_REG(SLICE_SWDIO_TXEN) = 0b11111111111111111111111111111111;
    SLICE_SHD(SLICE_SWDIO_TXEN) = 0b11111111111111111111111111111111;

    SLICE_POS(SLICE_SWDIO_TMS_OE) = POS_POS(32) | POS_PRESET(32);
    SLICE_REG(SLICE_SWDIO_TMS_OE) = 0b11111111111111111111111111111111;
    SLICE_SHD(SLICE_SWDIO_TMS_OE) = 0b11111111111111111111111111111111;
    SLICE_REG(SLICE_SWDIO_TMS_DOUT0) = 0;
    SLICE_SHD(SLICE_SWDIO_TMS_DOUT0) = 0;

    exchg_clk_cleanup(SLICE_SWDIO_TMS_DOUT0 | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
    slice_pos_oneshot(SLICE_SWCLK_TCK_MASK);
    shift_clk_kickoff(SLICE_SWDIO_TMS_DOUT0 | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);

    exchg_clk_wait_on(SLICE_SWCLK_TCK_MASK);

    shift_clk_disable(SLICE_SWDIO_TMS_DOUT0 | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
    slice_pos_disable(SLICE_SWDIO_TMS_DOUT0 | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
}


#define BYTES_PER_SLICE 4

// bits_sz bits  SLICE_D -> SLICE_O -> out
// Slice data is exchanged using a double buffering scheme.
// When POS reaches zero the slice register REG content is
// swapped with the slice buffer REG_SS content.
static void sequence_send(uint32_t bits_sz, const uint8_t *data)
{
    uint32_t bits_it = 0;
    uint32_t data_it = 0;
    uint32_t slices_started = 0;
    size_t slice_regs_len = 4;
    slice_reg_t slice_regs[] = {
        {&SLICE_REG(SLICE_SWDIO_TMS_DOUT0), SLICE_SWDIO_TMS_DOUT0_MASK},
        {&SLICE_REG(SLICE_SWDIO_TMS_DOUT1), SLICE_SWDIO_TMS_DOUT1_MASK},
        {&SLICE_SHD(SLICE_SWDIO_TMS_DOUT0), SLICE_SWDIO_TMS_DOUT0_MASK},
        {&SLICE_SHD(SLICE_SWDIO_TMS_DOUT1), SLICE_SWDIO_TMS_DOUT1_MASK},
    };

    uint32_t bits_per_chunk = bits_sz<=64 ? bits_sz : 64;

    SLICE_POS(SLICE_SWCLK_TCK) = POS_POS(bits_sz) | POS_PRESET(0);

    SLICE_POS(SLICE_SWDIO_TMS_OE) = POS_POS(bits_sz) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TMS_OE) = 0b11111111111111111111111111111111;
    SLICE_SHD(SLICE_SWDIO_TMS_OE) = 0b11111111111111111111111111111111;

    SLICE_POS(SLICE_SWDIO_TXEN) = POS_POS(bits_sz) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TXEN) = 0b11111111111111111111111111111111;
    SLICE_SHD(SLICE_SWDIO_TXEN) = 0b11111111111111111111111111111111;

    SLICE_POS(SLICE_SWDIO_TMS_DOUT0) = POS_POS(bits_per_chunk) | POS_PRESET(bits_per_chunk);
    SLICE_POS(SLICE_SWDIO_TMS_DOUT1) = POS_POS(bits_per_chunk) | POS_PRESET(bits_per_chunk);

    while (bits_it < bits_sz)
    {
        uint32_t bits_remaining = bits_sz - bits_it;
        uint32_t bytes_remaining = (bits_remaining + 7) / 8;
        uint32_t slice_it = 0;
        uint32_t slices_used = 0;
        uint32_t slices_required = bytes_remaining/BYTES_PER_SLICE;

        while (slice_it < slices_required && slice_it < slice_regs_len)
        {
            slice_reg_writeU8(BYTES_PER_SLICE, data + data_it, slice_regs[slice_it].addr);
            slices_used |= slice_regs[slice_it].slice_mask;
            bits_it += BYTES_PER_SLICE*8;
            data_it += BYTES_PER_SLICE;
            slice_it += 1;
        }

        SLICE_SHD(SLICE_SWDIO_TMS_OE) = 0xFFFFFFFF;
        SLICE_SHD(SLICE_SWDIO_TXEN) = 0xFFFFFFFF;

        if (bytes_remaining > BYTES_PER_SLICE*4)
        {
            slice_regs[0].addr = &SLICE_SHD(SLICE_SWDIO_TMS_DOUT0);
            slice_regs[0].slice_mask = SLICE_SWDIO_TMS_DOUT0_MASK;

            slice_regs[1].addr = &SLICE_SHD(SLICE_SWDIO_TMS_DOUT1);
            slice_regs[1].slice_mask = SLICE_SWDIO_TMS_DOUT1_MASK;
            slice_regs_len = 2;
        }

        if (bytes_remaining > slice_it*BYTES_PER_SLICE && slice_it < slice_regs_len)
        {
            slice_reg_writeU8(bytes_remaining - BYTES_PER_SLICE*slice_it, data + data_it, slice_regs[slice_it].addr);
            slices_used |= slice_regs[slice_it].slice_mask;
            bits_it = bits_sz;
            data_it += bytes_remaining - BYTES_PER_SLICE*slice_it;
        }
        if (!slices_started)
        {
            exchg_clk_cleanup(SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | slices_used);
            slice_pos_oneshot(SLICE_SWCLK_TCK_MASK);
            shift_clk_kickoff(SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | slices_used);
            slices_started = 1;
        }
        exchg_clk_wait_on(slices_used);
        exchg_clk_cleanup(slices_used);
        if (bits_it >= bits_sz)
        {
            SLICE_SHD(SLICE_SWDIO_TMS_DOUT0) = 1;
            SLICE_SHD(SLICE_SWDIO_TMS_DOUT1) = 1;
            exchg_clk_wait_on(SLICE_SWCLK_TCK_MASK);
        }
    }

    shift_clk_disable(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DOUT1_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
    slice_pos_disable(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DOUT1_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
}



// bits IN -> SLICE_H -> SLICE_P
static void sequence_read(uint32_t bits_sz, uint8_t *data)
{
    uint8_t bits_it = 0;
    uint8_t data_it = 0;
    uint32_t slices_started = 0;
    uint32_t slices_used = 0;
    size_t slice_regs_len = 2;
    slice_reg_t slice_regs[] = {
        {&SLICE_SHD(SLICE_SWDIO_TMS_DIN0), SLICE_SWDIO_TMS_DIN0_MASK},
        {&SLICE_SHD(SLICE_SWDIO_TMS_DIN1), SLICE_SWDIO_TMS_DIN1_MASK},
    };

    SLICE_POS(SLICE_SWCLK_TCK) = POS_POS(bits_sz) | POS_PRESET(0);

    // Disable Output.
    SLICE_POS(SLICE_SWDIO_TMS_OE) = POS_POS(0) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TMS_OE) = 0;
    SLICE_SHD(SLICE_SWDIO_TMS_OE) = 0;

    // Disable TXEN.
    SLICE_POS(SLICE_SWDIO_TXEN) = POS_POS(0) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TMS_OE) = 0;
    SLICE_SHD(SLICE_SWDIO_TXEN) = 0;

    uint32_t bits_per_chunk = bits_sz<=64 ? bits_sz : 64;
    SLICE_POS(SLICE_SWDIO_TMS_DIN1) = POS_POS(bits_per_chunk) | POS_PRESET(bits_per_chunk);

    if (bits_sz <= BYTES_PER_SLICE*8)
    {
        slices_used = SLICE_SWDIO_TMS_DIN0;
        SLICE_POS(SLICE_SWDIO_TMS_DIN0) = POS_POS(bits_per_chunk) | POS_PRESET(bits_per_chunk);
    }
    else
    {
        slices_used = SLICE_SWDIO_TMS_DIN0 | SLICE_SWDIO_TMS_DIN1;
        SLICE_POS(SLICE_SWDIO_TMS_DIN0) = POS_POS(bits_per_chunk) | POS_PRESET(bits_per_chunk);
        SLICE_POS(SLICE_SWDIO_TMS_DIN1) = POS_POS(bits_per_chunk) | POS_PRESET(bits_per_chunk);
    }

    while (bits_it < bits_sz)
    {
        uint32_t bits_remaining = bits_sz - bits_it;
        uint32_t bytes_remaining = (bits_remaining + 7) / 8;
        uint32_t slice_it = 0;
        uint32_t slices_used = 0;
        uint32_t slices_required = bytes_remaining/BYTES_PER_SLICE;

        if (!slices_started)
        {
            exchg_clk_cleanup(SLICE_SWCLK_TCK_MASK | slices_used);
            slice_pos_oneshot(SLICE_SWCLK_TCK_MASK);
            shift_clk_kickoff(SLICE_SWCLK_TCK_MASK | slices_used);
            slices_started = 1;
        }
        exchg_clk_wait_on(slices_used);
        exchg_clk_cleanup(slices_used);

        while (slice_it < slices_required && slice_it < slice_regs_len)
        {
            slice_reg_readU8(BYTES_PER_SLICE, slice_regs[slice_it].addr, data + data_it);
            bits_it += BYTES_PER_SLICE*8;
            data_it += BYTES_PER_SLICE;
            slice_it += 1;
        }

        if (bytes_remaining > slice_it*BYTES_PER_SLICE && slice_it < slice_regs_len)
        {
            bits_it = bits_sz;
            data_it += bytes_remaining - BYTES_PER_SLICE*slice_it;
        }
    }

    shift_clk_disable(SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWDIO_TMS_DIN1_MASK | SLICE_SWCLK_TCK_MASK);
    slice_pos_disable(SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWDIO_TMS_DIN1_MASK | SLICE_SWCLK_TCK_MASK);

    SLICE_REG(SLICE_SWDIO_TMS_OE) = 1;
    SLICE_REG(SLICE_SWDIO_TXEN) = 1;
}

// Figure B4-1 "SWD successful write operation" ID072320.
// Read HDR T ACK Data T
// Send HDR T ACK T Data
// Wait HDR T ACK T
__STATIC_FORCEINLINE uint8_t transfer_send(uint32_t hdr, uint32_t *data)
{
    uint32_t ack;
    uint32_t pkt[2];
    uint32_t parity;

    SLICE_POS(SLICE_SWDIO_TXEN) = POS_POS(8) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TXEN) = 0b00000000000000001111111111111111;
    SLICE_SHD(SLICE_SWDIO_TXEN) = 0b00000000000000000000000000000000;

    SLICE_POS(SLICE_SWDIO_TMS_OE) = POS_POS(8) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TMS_OE) = 0b00000000000000000000000011111111;

    //  header:8
    SLICE_POS(SLICE_SWDIO_TMS_DOUT0) = POS_POS(8) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TMS_DOUT0) = hdr;

    // header:8  turnaround:1  ack:3
    SLICE_POS(SLICE_SWDIO_TMS_DIN0) = POS_POS(8 + DAP_Data.swd_conf.turnaround + 3) | POS_PRESET(0);

    // header:8  turnaround:1  ack:3  turnaround:1
    SLICE_POS(SLICE_SWCLK_TCK) = POS_POS(8 + DAP_Data.swd_conf.turnaround + 3 +  DAP_Data.swd_conf.turnaround) | POS_PRESET(0);

    exchg_clk_cleanup(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
    slice_pos_oneshot(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
    shift_clk_kickoff(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);

    parity = PARITY_U32(*data);

    exchg_clk_wait_on(SLICE_SWCLK_TCK_MASK);

    shift_clk_disable(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
    slice_pos_disable(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);

    pkt[0] = SLICE_SHD(SLICE_SWDIO_TMS_DIN0);
    ack = (pkt[0] >> 29) & 0b111;

    if (ack != DAP_TRANSFER_OK)
    {
        if (ack == DAP_TRANSFER_WAIT || ack == DAP_TRANSFER_FAULT)
        {
            if (DAP_Data.swd_conf.data_phase == 0)
                return ack;
        }
        else
        {
            line_turnaround(DAP_Data.swd_conf.turnaround + 32U + 1U);
            return ack;
        }
    }

    SLICE_POS(SLICE_SWDIO_TMS_DOUT0) = POS_POS(32 + 1) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TMS_DOUT0) = *data;

    SLICE_POS(SLICE_SWDIO_TMS_DOUT1) = POS_POS(32 + 1) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TMS_DOUT1) = parity;

    SLICE_POS(SLICE_SWDIO_TXEN) = POS_POS(32) | POS_PRESET(32);
    SLICE_REG(SLICE_SWDIO_TXEN) = 0b11111111111111111111111111111111;
    SLICE_SHD(SLICE_SWDIO_TXEN) = 0b11111111111111111111111111111111;

    SLICE_POS(SLICE_SWDIO_TMS_OE) = POS_POS(32) | POS_PRESET(32);
    SLICE_REG(SLICE_SWDIO_TMS_OE) = 0b11111111111111111111111111111111;
    SLICE_SHD(SLICE_SWDIO_TMS_OE) = 0b11111111111111111111111111111111;

    // data:32  parity:1
    SLICE_POS(SLICE_SWCLK_TCK) = POS_POS(32 + 1) | POS_PRESET(0);

    exchg_clk_cleanup(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DOUT1_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
    slice_pos_oneshot(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DOUT1_MASK | SLICE_SWCLK_TCK_MASK);
    shift_clk_kickoff(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DOUT1_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);

    exchg_clk_wait_on(SLICE_SWCLK_TCK_MASK);

    shift_clk_disable(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DOUT1_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
    slice_pos_disable(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DOUT1_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);

    SLICE_REG(SLICE_SWDIO_TMS_OE) = 1;
    SLICE_REG(SLICE_SWDIO_TXEN) = 1;

    return ack;
}



// hdr:0x000000a5 -     10100101
// IDR 0x6ba02477  - 01101011101000000010010001110111

// Figure B4-2 "SWD successful read operation" ID072320.
static uint8_t transfer_read(uint32_t hdr, uint32_t *data)
{
    uint32_t ack = DAP_TRANSFER_OK;
    uint32_t pkt[2];

    SLICE_POS(SLICE_SWDIO_TXEN) = POS_POS(8) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TXEN) = 0b00000000000000001111111111111111;
    SLICE_SHD(SLICE_SWDIO_TXEN) = 0b00000000000000000000000000000000;

    SLICE_POS(SLICE_SWDIO_TMS_OE) = POS_POS(8) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TMS_OE) = 0b00000000000000000000000011111111;
    SLICE_SHD(SLICE_SWDIO_TMS_OE) = 0;

    // header:8
    SLICE_POS(SLICE_SWDIO_TMS_DOUT0) = POS_POS(8) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TMS_DOUT0) = hdr;
    SLICE_SHD(SLICE_SWDIO_TMS_DOUT0) = 0;

    // header:8  turnaround  ack:3
    SLICE_POS(SLICE_SWDIO_TMS_DIN0) = POS_POS(8 + DAP_Data.swd_conf.turnaround + 3) | POS_PRESET(0);

    // header:8  turnaround  ack:3
    SLICE_POS(SLICE_SWCLK_TCK) = POS_POS(8 + DAP_Data.swd_conf.turnaround + 3) | POS_PRESET(0);

    exchg_clk_cleanup(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
    slice_pos_oneshot(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
    shift_clk_kickoff(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);

    exchg_clk_wait_on(SLICE_SWCLK_TCK_MASK);

    shift_clk_disable(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
    slice_pos_disable(SLICE_SWDIO_TMS_DOUT0_MASK | SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);

    pkt[0] = SLICE_SHD(SLICE_SWDIO_TMS_DIN0);

    ack = (pkt[0] >> 29) & 0b111;

    if (ack != DAP_TRANSFER_OK)
    {
        if (ack == DAP_TRANSFER_WAIT || ack == DAP_TRANSFER_FAULT)
        {
            line_turnaround(DAP_Data.swd_conf.turnaround);
            if (DAP_Data.swd_conf.data_phase == 0)
                return ack;
        }
        else
        {
            line_turnaround(DAP_Data.swd_conf.turnaround + 32U + 1U);
            return ack;
        }
    }

    SLICE_REG(SLICE_SWDIO_TMS_DOUT0) = 0;
    SLICE_REG(SLICE_SWDIO_TMS_DOUT1) = 0;
    SLICE_REG(SLICE_SWDIO_TMS_OE) = 0;
    SLICE_REG(SLICE_SWDIO_TXEN) = 0;

    // data:32  parity:1
    SLICE_POS(SLICE_SWDIO_TMS_DIN0) = POS_POS(32 + 1) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TMS_DIN0) = 0;
    SLICE_SHD(SLICE_SWDIO_TMS_DIN0) = 0;

    SLICE_POS(SLICE_SWDIO_TMS_DIN1) = POS_POS(32 + 1) | POS_PRESET(0);
    SLICE_REG(SLICE_SWDIO_TMS_DIN1) = 0;
    SLICE_SHD(SLICE_SWDIO_TMS_DIN1) = 0;

    // data:32  parity:1  turnaround
    SLICE_POS(SLICE_SWCLK_TCK) = POS_POS(32 + 1 + DAP_Data.swd_conf.turnaround) | POS_PRESET(0);

    exchg_clk_cleanup(SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWDIO_TMS_DIN1_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
    slice_pos_oneshot(SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWDIO_TMS_DIN1_MASK | SLICE_SWCLK_TCK_MASK);
    shift_clk_kickoff(SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWDIO_TMS_DIN1_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);

    exchg_clk_wait_on(SLICE_SWCLK_TCK_MASK);

    shift_clk_disable(SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWDIO_TMS_DIN1_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);
    slice_pos_disable(SLICE_SWDIO_TMS_DIN0_MASK | SLICE_SWDIO_TMS_DIN1_MASK | SLICE_SWCLK_TCK_MASK | SLICE_SWDIO_TXEN_MASK | SLICE_SWDIO_TMS_OE_MASK);

    if (data)
    {
        pkt[0] = SLICE_SHD(SLICE_SWDIO_TMS_DIN0);
        pkt[1] = SLICE_SHD(SLICE_SWDIO_TMS_DIN1);
        *data = (pkt[0] << 1) | (pkt[1]>>31);

        uint32_t parity = (pkt[0] >> 31);
        if (parity != PARITY_U32(*data))
            return DAP_TRANSFER_ERROR;
    }

    return ack;
}



// Configures all slices, muxes, etc.
void sgpio_init(void)
{
    sgpio_clock_init();

    shift_clk_int_off(0xFFFF);
    exchg_clk_int_off(0xFFFF);
    shift_clk_disable(0xFFFF);
    slice_pos_disable(0xFFFF);

    LPC_SGPIO->OUT_MUX_CFG[SGPIO_SWCLK_TCK] = OUT_MUX_CFG_CLK_OUT
                                            | OUT_MUX_CFG_GPIO_OE;
    LPC_SGPIO->SGPIO_MUX_CFG[SLICE_SWCLK_TCK] = SGPIO_MUX_CFG_EXT_CLK_ENABLE_INTERNAL
                                              | SGPIO_MUX_CFG_CLK_SOURCE_PIN_MODE_8
                                              | SGPIO_MUX_CFG_CLK_SOURCE_SLICE_MODE_D
                                              | SGPIO_MUX_CFG_QUALIFIER_MODE_DEFAULT
                                              | SGPIO_MUX_CFG_QUALIFIER_PIN_MODE_8
                                              | SGPIO_MUX_CFG_QUALIFIER_SLICE_MODE_A
                                              | SGPIO_MUX_CFG_CONCAT_DISABLE
                                              | SGPIO_MUX_CFG_CONCAT_ORDER_LOOP
                                              ;

    LPC_SGPIO->SLICE_MUX_CFG[SLICE_SWCLK_TCK] = SLICE_MUX_CFG_MATCH_MODE_DISABLED
                                              | SLICE_MUX_CFG_CLK_CAPTURE_MODE_RISING
                                              | SLICE_MUX_CFG_CLKGEN_MODE_INTERNAL
                                              | SLICE_MUX_CFG_INV_OUT_CLK_INVERTED
                                              | SLICE_MUX_CFG_DATA_CAPTURE_MODE_RISING
                                              | SLICE_MUX_CFG_PARALLEL_MODE_1BPCLK
                                              | SLICE_MUX_CFG_INV_QUALIFIER_NORMAL
                                              ;

    // SWDIO/TXEN (SGPIO15).
    LPC_SGPIO->OUT_MUX_CFG[SGPIO_SWDIO_TXEN] = OUT_MUX_CFG_DOUT_DOUTM8B
                                             | OUT_MUX_CFG_GPIO_OE;
    LPC_SGPIO->SGPIO_MUX_CFG[SLICE_SWDIO_TXEN] = SGPIO_MUX_CFG_EXT_CLK_ENABLE_EXTERNAL
                                               | SGPIO_MUX_CFG_CLK_SOURCE_PIN_MODE_11
                                               | SGPIO_MUX_CFG_CLK_SOURCE_SLICE_MODE_D
                                               | SGPIO_MUX_CFG_QUALIFIER_MODE_DEFAULT
                                               | SGPIO_MUX_CFG_QUALIFIER_PIN_MODE_8
                                               | SGPIO_MUX_CFG_QUALIFIER_SLICE_MODE_A
                                               | SGPIO_MUX_CFG_CONCAT_ENABLE
                                               | SGPIO_MUX_CFG_CONCAT_ORDER_LOOP
                                               ;

    LPC_SGPIO->SLICE_MUX_CFG[SLICE_SWDIO_TXEN] = SLICE_MUX_CFG_MATCH_MODE_DISABLED
                                               | SLICE_MUX_CFG_CLK_CAPTURE_MODE_RISING
                                               | SLICE_MUX_CFG_CLKGEN_MODE_EXTERNAL
                                               | SLICE_MUX_CFG_INV_OUT_CLK_INVERTED
                                               | SLICE_MUX_CFG_DATA_CAPTURE_MODE_RISING
                                               | SLICE_MUX_CFG_PARALLEL_MODE_1BPCLK
                                               | SLICE_MUX_CFG_INV_QUALIFIER_NORMAL
                                               ;

    // output-enable slice configuration.
    LPC_SGPIO->SGPIO_MUX_CFG[SLICE_SWDIO_TMS_OE] = SGPIO_MUX_CFG_EXT_CLK_ENABLE_EXTERNAL
                                                 | SGPIO_MUX_CFG_CLK_SOURCE_PIN_MODE_11
                                                 | SGPIO_MUX_CFG_CLK_SOURCE_SLICE_MODE_D
                                                 | SGPIO_MUX_CFG_QUALIFIER_MODE_DEFAULT
                                                 | SGPIO_MUX_CFG_QUALIFIER_PIN_MODE_8
                                                 | SGPIO_MUX_CFG_QUALIFIER_SLICE_MODE_A
                                                 | SGPIO_MUX_CFG_CONCAT_ENABLE
                                                 | SGPIO_MUX_CFG_CONCAT_ORDER_LOOP
                                                 ;

    LPC_SGPIO->SLICE_MUX_CFG[SLICE_SWDIO_TMS_OE] = SLICE_MUX_CFG_MATCH_MODE_DISABLED
                                                 | SLICE_MUX_CFG_CLK_CAPTURE_MODE_RISING
                                                 | SLICE_MUX_CFG_CLKGEN_MODE_EXTERNAL
                                                 | SLICE_MUX_CFG_INV_OUT_CLK_INVERTED
                                                 | SLICE_MUX_CFG_DATA_CAPTURE_MODE_RISING
                                                 | SLICE_MUX_CFG_PARALLEL_MODE_1BPCLK
                                                 | SLICE_MUX_CFG_INV_QUALIFIER_NORMAL
                                                 ;

    // SWDIO/TMS (SGPIO14). "Pin multiplexer configuration registers" UM10503.
    LPC_SGPIO->OUT_MUX_CFG[SGPIO_SWDIO_TMS] = OUT_MUX_CFG_DOUT_DOUTM2C
                                            | OUT_MUX_CFG_DOUT_OEM1;
    // input slice configuration.
    LPC_SGPIO->SGPIO_MUX_CFG[SLICE_SWDIO_TMS_DIN0] = SGPIO_MUX_CFG_EXT_CLK_ENABLE_EXTERNAL
                                                   | SGPIO_MUX_CFG_CLK_SOURCE_PIN_MODE_11
                                                   | SGPIO_MUX_CFG_CLK_SOURCE_SLICE_MODE_D
                                                   | SGPIO_MUX_CFG_QUALIFIER_MODE_DEFAULT
                                                   | SGPIO_MUX_CFG_QUALIFIER_PIN_MODE_8
                                                   | SGPIO_MUX_CFG_QUALIFIER_SLICE_MODE_A
                                                   | SGPIO_MUX_CFG_CONCAT_EXT_PIN
                                                   | SGPIO_MUX_CFG_CONCAT_ORDER_LOOP
                                                   ;
    LPC_SGPIO->SGPIO_MUX_CFG[SLICE_SWDIO_TMS_DIN1] = SGPIO_MUX_CFG_EXT_CLK_ENABLE_EXTERNAL
                                                   | SGPIO_MUX_CFG_CLK_SOURCE_PIN_MODE_11
                                                   | SGPIO_MUX_CFG_CLK_SOURCE_SLICE_MODE_D
                                                   | SGPIO_MUX_CFG_QUALIFIER_MODE_DEFAULT
                                                   | SGPIO_MUX_CFG_QUALIFIER_PIN_MODE_8
                                                   | SGPIO_MUX_CFG_QUALIFIER_SLICE_MODE_A
                                                   | SGPIO_MUX_CFG_CONCAT_ENABLE
                                                   | SGPIO_MUX_CFG_CONCAT_ORDER_2SLC
                                                   ;
    LPC_SGPIO->SLICE_MUX_CFG[SLICE_SWDIO_TMS_DIN0] = SLICE_MUX_CFG_MATCH_MODE_DISABLED
                                                   | SLICE_MUX_CFG_CLK_CAPTURE_MODE_RISING
                                                   | SLICE_MUX_CFG_CLKGEN_MODE_EXTERNAL
                                                   | SLICE_MUX_CFG_INV_OUT_CLK_INVERTED
                                                   | SLICE_MUX_CFG_DATA_CAPTURE_MODE_RISING
                                                   | SLICE_MUX_CFG_PARALLEL_MODE_1BPCLK
                                                   | SLICE_MUX_CFG_INV_QUALIFIER_NORMAL
                                                   ;
    LPC_SGPIO->SLICE_MUX_CFG[SLICE_SWDIO_TMS_DIN1] = SLICE_MUX_CFG_MATCH_MODE_DISABLED
                                                   | SLICE_MUX_CFG_CLK_CAPTURE_MODE_RISING
                                                   | SLICE_MUX_CFG_CLKGEN_MODE_EXTERNAL
                                                   | SLICE_MUX_CFG_INV_OUT_CLK_INVERTED
                                                   | SLICE_MUX_CFG_DATA_CAPTURE_MODE_RISING
                                                   | SLICE_MUX_CFG_PARALLEL_MODE_1BPCLK
                                                   | SLICE_MUX_CFG_INV_QUALIFIER_NORMAL
                                                   ;
    // output slice configuration.
    LPC_SGPIO->SGPIO_MUX_CFG[SLICE_SWDIO_TMS_DOUT1] = SGPIO_MUX_CFG_EXT_CLK_ENABLE_EXTERNAL
                                                    | SGPIO_MUX_CFG_CLK_SOURCE_PIN_MODE_11
                                                    | SGPIO_MUX_CFG_CLK_SOURCE_SLICE_MODE_D
                                                    | SGPIO_MUX_CFG_QUALIFIER_MODE_DEFAULT
                                                    | SGPIO_MUX_CFG_QUALIFIER_PIN_MODE_8
                                                    | SGPIO_MUX_CFG_QUALIFIER_SLICE_MODE_A
                                                    | SGPIO_MUX_CFG_CONCAT_ENABLE
                                                    | SGPIO_MUX_CFG_CONCAT_ORDER_LOOP
                                                    ;
    LPC_SGPIO->SGPIO_MUX_CFG[SLICE_SWDIO_TMS_DOUT0] = SGPIO_MUX_CFG_EXT_CLK_ENABLE_EXTERNAL
                                                    | SGPIO_MUX_CFG_CLK_SOURCE_PIN_MODE_11
                                                    | SGPIO_MUX_CFG_CLK_SOURCE_SLICE_MODE_D
                                                    | SGPIO_MUX_CFG_QUALIFIER_MODE_DEFAULT
                                                    | SGPIO_MUX_CFG_QUALIFIER_PIN_MODE_8
                                                    | SGPIO_MUX_CFG_QUALIFIER_SLICE_MODE_A
                                                    | SGPIO_MUX_CFG_CONCAT_ENABLE
                                                    | SGPIO_MUX_CFG_CONCAT_ORDER_2SLC
                                                    ;
    LPC_SGPIO->SLICE_MUX_CFG[SLICE_SWDIO_TMS_DOUT1] = SLICE_MUX_CFG_MATCH_MODE_DISABLED
                                                    | SLICE_MUX_CFG_CLK_CAPTURE_MODE_RISING
                                                    | SLICE_MUX_CFG_CLKGEN_MODE_EXTERNAL
                                                    | SLICE_MUX_CFG_INV_OUT_CLK_INVERTED
                                                    | SLICE_MUX_CFG_DATA_CAPTURE_MODE_RISING
                                                    | SLICE_MUX_CFG_PARALLEL_MODE_1BPCLK
                                                    | SLICE_MUX_CFG_INV_QUALIFIER_NORMAL
                                                    ;
    LPC_SGPIO->SLICE_MUX_CFG[SLICE_SWDIO_TMS_DOUT0] = SLICE_MUX_CFG_MATCH_MODE_DISABLED
                                                    | SLICE_MUX_CFG_CLK_CAPTURE_MODE_RISING
                                                    | SLICE_MUX_CFG_CLKGEN_MODE_EXTERNAL
                                                    | SLICE_MUX_CFG_INV_OUT_CLK_INVERTED
                                                    | SLICE_MUX_CFG_DATA_CAPTURE_MODE_RISING
                                                    | SLICE_MUX_CFG_PARALLEL_MODE_1BPCLK
                                                    | SLICE_MUX_CFG_INV_QUALIFIER_NORMAL
                                                    ;

    sgpio_pin_oenable(SGPIO_SWCLK_TCK);
    sgpio_pin_oenable(SGPIO_SWDIO_TXEN);
}



// Generate SWJ Sequence
//   count:  Number of bits in sequence: 1..256 (256 encoded as 0)
//   data:   Sequence generated on SWDIO/TMS (with clock @SWCLK/TCK); LSB is transmitted first.
//   return: none
void SWJ_Sequence(uint32_t count, const uint8_t *request)
{
    shift_clk_preset_init();

    sequence_send(count, request);
}



// Generate SWD Sequence
//   info:   sequence information
//   swdo:   pointer to SWDIO generated data
//   swdi:   pointer to SWDIO captured data
//   return: none
void SWD_Sequence(uint32_t info, const uint8_t *swdo, uint8_t *swdi)
{
    uint32_t bytes_nb;

    shift_clk_preset_init();

    bytes_nb = info & SWD_SEQUENCE_CLK;
    if (bytes_nb == 0U)
        bytes_nb = 64U;

    if (info & SWD_SEQUENCE_DIN)
        sequence_read(bytes_nb*8, swdi);
    else
        sequence_send(bytes_nb*8, swdo);
}



// SWD Transfer I/O
//   request: A[3:2] RnW APnDP
//   data:    DATA[31:0]
//   return:  ACK[2:0]
uint8_t SWD_Transfer(uint32_t request, uint32_t *data)
{
    uint32_t parity = PARITY_U8(request);
    uint32_t ack = DAP_TRANSFER_OK;

    uint32_t hdr = 1                      // Start Bit
                 | ((request & 0xF) << 1) // APnDP, RnW, A2, A3 Bits
                 | (parity << 5)          // Parity Bit
                 | (0 << 6)               // Stop Bit
                 | (1 << 7)               // Park Bit
                 ;

    if (REQUEST_WRITE(request))
        ack = transfer_send(hdr, data);

    else if (REQUEST_READ(request))
        ack = transfer_read(hdr, data);

    else if (REQUEST_TIMESTAMP(request))
        DAP_Data.timestamp = TIMESTAMP_GET();

    if (DAP_Data.transfer.idle_cycles)
        idle_cycles(DAP_Data.transfer.idle_cycles);

    return (uint8_t)ack;
}

#endif /* SWDP_SGPIO */
