/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
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

#ifndef _FSL_CLOCK_H_
#define _FSL_CLOCK_H_

#include "fsl_device_registers.h"
#include "fsl_common.h"
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/*! @addtogroup clock */
/*! @{ */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief CLOCK driver version 2.2.0. */
#define FSL_CLOCK_DRIVER_VERSION (MAKE_VERSION(2, 2, 0))
/*@}*/

/*! @brief External XTAL0 (OSC0) clock frequency.
 *
 * The XTAL0/EXTAL0 (OSC0) clock frequency in Hz, when the clock is setup, use the
 * function CLOCK_SetXtal0Freq to set the value in to clock driver. For example,
 * if XTAL0 is 8MHz,
 * @code
 * CLOCK_InitOsc0(...); // Setup the OSC0
 * CLOCK_SetXtal0Freq(80000000); // Set the XTAL0 value to clock driver.
 * @endcode
 *
 * This is important for the multicore platforms, only one core needs to setup
 * OSC0 using CLOCK_InitOsc0, all other cores need to call CLOCK_SetXtal0Freq
 * to get valid clock frequency.
 */
extern uint32_t g_xtal0Freq;

/*! @brief External XTAL32/EXTAL32/RTC_CLKIN clock frequency.
 *
 * The XTAL32/EXTAL32/RTC_CLKIN clock frequency in Hz, when the clock is setup, use the
 * function CLOCK_SetXtal32Freq to set the value in to clock driver.
 *
 * This is important for the multicore platforms, only one core needs to setup
 * the clock, all other cores need to call CLOCK_SetXtal32Freq
 * to get valid clock frequency.
 */
extern uint32_t g_xtal32Freq;

/*! @brief IRC48M clock frequency in Hz. */
#define MCG_INTERNAL_IRC_48M 48000000U

#if (defined(OSC) && !(defined(OSC0)))
#define OSC0 OSC
#endif

/*! @brief Clock ip name array for DMAMUX. */
#define DMAMUX_CLOCKS  \
    {                  \
        kCLOCK_Dmamux0 \
    }

/*! @brief Clock ip name array for RTC. */
#define RTC_CLOCKS  \
    {               \
        kCLOCK_Rtc0 \
    }

/*! @brief Clock ip name array for PORT. */
#define PORT_CLOCKS                                                          \
    {                                                                        \
        kCLOCK_PortA, kCLOCK_PortB, kCLOCK_PortC, kCLOCK_PortD, kCLOCK_PortE \
    }

/*! @brief Clock ip name array for SAI. */
#define SAI_CLOCKS  \
    {               \
        kCLOCK_Sai0 \
    }

/*! @brief Clock ip name array for FLEXBUS. */
#define FLEXBUS_CLOCKS  \
    {                   \
        kCLOCK_Flexbus0 \
    }

/*! @brief Clock ip name array for TSI. */
#define TSI_CLOCKS  \
    {               \
        kCLOCK_Tsi0 \
    }

/*! @brief Clock ip name array for LPUART. */
#define LPUART_CLOCKS  \
    {                  \
        kCLOCK_Lpuart0 \
    }

/*! @brief Clock ip name array for EWM. */
#define EWM_CLOCKS  \
    {               \
        kCLOCK_Ewm0 \
    }

/*! @brief Clock ip name array for PIT. */
#define PIT_CLOCKS  \
    {               \
        kCLOCK_Pit0 \
    }

/*! @brief Clock ip name array for DSPI. */
#define DSPI_CLOCKS                           \
    {                                         \
        kCLOCK_Spi0, kCLOCK_Spi1, kCLOCK_Spi2 \
    }

/*! @brief Clock ip name array for LPTMR. */
#define LPTMR_CLOCKS  \
    {                 \
        kCLOCK_Lptmr0 \
    }

/*! @brief Clock ip name array for SDHC. */
#define SDHC_CLOCKS  \
    {                \
        kCLOCK_Sdhc0 \
    }

/*! @brief Clock ip name array for FTM. */
#define FTM_CLOCKS                                         \
    {                                                      \
        kCLOCK_Ftm0, kCLOCK_Ftm1, kCLOCK_Ftm2, kCLOCK_Ftm3 \
    }

/*! @brief Clock ip name array for EDMA. */
#define EDMA_CLOCKS \
    {               \
        kCLOCK_Dma0 \
    }

/*! @brief Clock ip name array for FLEXCAN. */
#define FLEXCAN_CLOCKS                   \
    {                                    \
        kCLOCK_Flexcan0, kCLOCK_Flexcan1 \
    }

/*! @brief Clock ip name array for DAC. */
#define DAC_CLOCKS               \
    {                            \
        kCLOCK_Dac0, kCLOCK_Dac1 \
    }

/*! @brief Clock ip name array for ADC16. */
#define ADC16_CLOCKS             \
    {                            \
        kCLOCK_Adc0, kCLOCK_Adc1 \
    }

/*! @brief Clock ip name array for SDRAM. */
#define SDRAM_CLOCKS   \
    {                  \
        kCLOCK_Sdramc0 \
    }

/*! @brief Clock ip name array for MPU. */
#define MPU_CLOCKS  \
    {               \
        kCLOCK_Mpu0 \
    }

/*! @brief Clock ip name array for VREF. */
#define VREF_CLOCKS  \
    {                \
        kCLOCK_Vref0 \
    }

/*! @brief Clock ip name array for CMT. */
#define CMT_CLOCKS  \
    {               \
        kCLOCK_Cmt0 \
    }

/*! @brief Clock ip name array for UART. */
#define UART_CLOCKS                                                          \
    {                                                                        \
        kCLOCK_Uart0, kCLOCK_Uart1, kCLOCK_Uart2, kCLOCK_Uart3, kCLOCK_Uart4 \
    }

/*! @brief Clock ip name array for TPM. */
#define TPM_CLOCKS                                 \
    {                                              \
        kCLOCK_IpInvalid, kCLOCK_Tpm1, kCLOCK_Tpm2 \
    }

/*! @brief Clock ip name array for RNGA. */
#define RNGA_CLOCKS  \
    {                \
        kCLOCK_Rnga0 \
    }

/*! @brief Clock ip name array for CRC. */
#define CRC_CLOCKS  \
    {               \
        kCLOCK_Crc0 \
    }

/*! @brief Clock ip name array for I2C. */
#define I2C_CLOCKS                                         \
    {                                                      \
        kCLOCK_I2c0, kCLOCK_I2c1, kCLOCK_I2c2, kCLOCK_I2c3 \
    }

/*! @brief Clock ip name array for FTF. */
#define FTF_CLOCKS  \
    {               \
        kCLOCK_Ftf0 \
    }

/*! @brief Clock ip name array for PDB. */
#define PDB_CLOCKS  \
    {               \
        kCLOCK_Pdb0 \
    }

/*! @brief Clock ip name array for CMP. */
#define CMP_CLOCKS                                         \
    {                                                      \
        kCLOCK_Cmp0, kCLOCK_Cmp1, kCLOCK_Cmp2, kCLOCK_Cmp3 \
    }

/*!
 * @brief LPO clock frequency.
 */
#define LPO_CLK_FREQ 1000U

/*! @brief Peripherals clock source definition. */
#define SYS_CLK kCLOCK_CoreSysClk
#define BUS_CLK kCLOCK_BusClk

#define I2C0_CLK_SRC BUS_CLK
#define I2C1_CLK_SRC BUS_CLK
#define I2C2_CLK_SRC BUS_CLK
#define I2C3_CLK_SRC BUS_CLK
#define DSPI0_CLK_SRC BUS_CLK
#define DSPI1_CLK_SRC BUS_CLK
#define DSPI2_CLK_SRC BUS_CLK
#define UART0_CLK_SRC SYS_CLK
#define UART1_CLK_SRC SYS_CLK
#define UART2_CLK_SRC BUS_CLK
#define UART3_CLK_SRC BUS_CLK
#define UART4_CLK_SRC BUS_CLK

/*! @brief Clock name used to get clock frequency. */
typedef enum _clock_name
{

    /* ----------------------------- System layer clock -------------------------------*/
    kCLOCK_CoreSysClk,    /*!< Core/system clock                                         */
    kCLOCK_PlatClk,       /*!< Platform clock                                            */
    kCLOCK_BusClk,        /*!< Bus clock                                                 */
    kCLOCK_FlexBusClk,    /*!< FlexBus clock                                             */
    kCLOCK_FlashClk,      /*!< Flash clock                                               */
    kCLOCK_FastPeriphClk, /*!< Fast peripheral clock                                     */
    kCLOCK_PllFllSelClk,  /*!< The clock after SIM[PLLFLLSEL].                           */

    /* ---------------------------------- OSC clock -----------------------------------*/
    kCLOCK_Er32kClk,       /*!< External reference 32K clock (ERCLK32K)                   */
    kCLOCK_Osc0ErClk,      /*!< OSC0 external reference clock (OSC0ERCLK)                 */
    kCLOCK_Osc1ErClk,      /*!< OSC1 external reference clock (OSC1ERCLK)                 */
    kCLOCK_Osc0ErClkUndiv, /*!< OSC0 external reference undivided clock(OSC0ERCLK_UNDIV). */

    /* ----------------------------- MCG and MCG-Lite clock ---------------------------*/
    kCLOCK_McgFixedFreqClk,   /*!< MCG fixed frequency clock (MCGFFCLK)                      */
    kCLOCK_McgInternalRefClk, /*!< MCG internal reference clock (MCGIRCLK)                   */
    kCLOCK_McgFllClk,         /*!< MCGFLLCLK                                                 */
    kCLOCK_McgPll0Clk,        /*!< MCGPLL0CLK                                                */
    kCLOCK_McgPll1Clk,        /*!< MCGPLL1CLK                                                */
    kCLOCK_McgExtPllClk,      /*!< EXT_PLLCLK                                                */
    kCLOCK_McgPeriphClk,      /*!< MCG peripheral clock (MCGPCLK)                            */
    kCLOCK_McgIrc48MClk,      /*!< MCG IRC48M clock                                          */

    /* --------------------------------- Other clock ----------------------------------*/
    kCLOCK_LpoClk, /*!< LPO clock                                                 */

} clock_name_t;

/*! @brief USB clock source definition. */
typedef enum _clock_usb_src
{
    kCLOCK_UsbSrcPll0 = SIM_SOPT2_USBSRC(1U) | SIM_SOPT2_PLLFLLSEL(1U),   /*!< Use PLL0.      */
    kCLOCK_UsbSrcUsbPfd = SIM_SOPT2_USBSRC(1U) | SIM_SOPT2_PLLFLLSEL(2U), /*!< Use USBPFDCLK. */
    kCLOCK_UsbSrcIrc48M = SIM_SOPT2_USBSRC(1U) | SIM_SOPT2_PLLFLLSEL(3U), /*!< Use IRC48M.    */
    kCLOCK_UsbSrcExt = SIM_SOPT2_USBSRC(0U)                               /*!< Use USB_CLKIN. */
} clock_usb_src_t;

/*------------------------------------------------------------------------------

 clock_gate_t definition:

 31                              16                              0
 -----------------------------------------------------------------
 | SIM_SCGC register offset       |   control bit offset in SCGC |
 -----------------------------------------------------------------

 For example, the SDHC clock gate is controlled by SIM_SCGC3[17], the
 SIM_SCGC3 offset in SIM is 0x1030, then kClockGateSdhc0 is defined as

              kClockGateSdhc0 = (0x1030 << 16) | 17;

------------------------------------------------------------------------------*/

#define CLK_GATE_REG_OFFSET_SHIFT 16U
#define CLK_GATE_REG_OFFSET_MASK 0xFFFF0000U
#define CLK_GATE_BIT_SHIFT_SHIFT 0U
#define CLK_GATE_BIT_SHIFT_MASK 0x0000FFFFU

#define CLK_GATE_DEFINE(reg_offset, bit_shift)                                  \
    ((((reg_offset) << CLK_GATE_REG_OFFSET_SHIFT) & CLK_GATE_REG_OFFSET_MASK) | \
     (((bit_shift) << CLK_GATE_BIT_SHIFT_SHIFT) & CLK_GATE_BIT_SHIFT_MASK))

#define CLK_GATE_ABSTRACT_REG_OFFSET(x) (((x)&CLK_GATE_REG_OFFSET_MASK) >> CLK_GATE_REG_OFFSET_SHIFT)
#define CLK_GATE_ABSTRACT_BITS_SHIFT(x) (((x)&CLK_GATE_BIT_SHIFT_MASK) >> CLK_GATE_BIT_SHIFT_SHIFT)

/*! @brief Clock gate name used for CLOCK_EnableClock/CLOCK_DisableClock. */
typedef enum _clock_ip_name
{
    kCLOCK_IpInvalid = 0U,
    kCLOCK_I2c2 = CLK_GATE_DEFINE(0x1028U, 6U),
    kCLOCK_I2c3 = CLK_GATE_DEFINE(0x1028U, 7U),
    kCLOCK_Uart4 = CLK_GATE_DEFINE(0x1028U, 10U),

    kCLOCK_Lpuart0 = CLK_GATE_DEFINE(0x102CU, 4U),
    kCLOCK_Tpm1 = CLK_GATE_DEFINE(0x102CU, 9U),
    kCLOCK_Tpm2 = CLK_GATE_DEFINE(0x102CU, 10U),
    kCLOCK_Dac0 = CLK_GATE_DEFINE(0x102CU, 12U),
    kCLOCK_Dac1 = CLK_GATE_DEFINE(0x102CU, 13U),

    kCLOCK_Rnga0 = CLK_GATE_DEFINE(0x1030U, 0U),
    kCLOCK_Usbhs0 = CLK_GATE_DEFINE(0x1030U, 1U),
    kCLOCK_UsbhsPhy0 = CLK_GATE_DEFINE(0x1030U, 2U),
    kCLOCK_UsbhsDcd0 = CLK_GATE_DEFINE(0x1030U, 3U),
    kCLOCK_Flexcan1 = CLK_GATE_DEFINE(0x1030U, 4U),
    kCLOCK_Spi2 = CLK_GATE_DEFINE(0x1030U, 12U),
    kCLOCK_Sdhc0 = CLK_GATE_DEFINE(0x1030U, 17U),
    kCLOCK_Ftm3 = CLK_GATE_DEFINE(0x1030U, 25U),
    kCLOCK_Adc1 = CLK_GATE_DEFINE(0x1030U, 27U),

    kCLOCK_Ewm0 = CLK_GATE_DEFINE(0x1034U, 1U),
    kCLOCK_Cmt0 = CLK_GATE_DEFINE(0x1034U, 2U),
    kCLOCK_I2c0 = CLK_GATE_DEFINE(0x1034U, 6U),
    kCLOCK_I2c1 = CLK_GATE_DEFINE(0x1034U, 7U),
    kCLOCK_Uart0 = CLK_GATE_DEFINE(0x1034U, 10U),
    kCLOCK_Uart1 = CLK_GATE_DEFINE(0x1034U, 11U),
    kCLOCK_Uart2 = CLK_GATE_DEFINE(0x1034U, 12U),
    kCLOCK_Uart3 = CLK_GATE_DEFINE(0x1034U, 13U),
    kCLOCK_Usbfs0 = CLK_GATE_DEFINE(0x1034U, 18U),
    kCLOCK_Cmp0 = CLK_GATE_DEFINE(0x1034U, 19U),
    kCLOCK_Cmp1 = CLK_GATE_DEFINE(0x1034U, 19U),
    kCLOCK_Cmp2 = CLK_GATE_DEFINE(0x1034U, 19U),
    kCLOCK_Cmp3 = CLK_GATE_DEFINE(0x1034U, 19U),
    kCLOCK_Vref0 = CLK_GATE_DEFINE(0x1034U, 20U),

    kCLOCK_Lptmr0 = CLK_GATE_DEFINE(0x1038U, 0U),
    kCLOCK_Tsi0 = CLK_GATE_DEFINE(0x1038U, 5U),
    kCLOCK_PortA = CLK_GATE_DEFINE(0x1038U, 9U),
    kCLOCK_PortB = CLK_GATE_DEFINE(0x1038U, 10U),
    kCLOCK_PortC = CLK_GATE_DEFINE(0x1038U, 11U),
    kCLOCK_PortD = CLK_GATE_DEFINE(0x1038U, 12U),
    kCLOCK_PortE = CLK_GATE_DEFINE(0x1038U, 13U),

    kCLOCK_Ftf0 = CLK_GATE_DEFINE(0x103CU, 0U),
    kCLOCK_Dmamux0 = CLK_GATE_DEFINE(0x103CU, 1U),
    kCLOCK_Flexcan0 = CLK_GATE_DEFINE(0x103CU, 4U),
    kCLOCK_Spi0 = CLK_GATE_DEFINE(0x103CU, 12U),
    kCLOCK_Spi1 = CLK_GATE_DEFINE(0x103CU, 13U),
    kCLOCK_Sai0 = CLK_GATE_DEFINE(0x103CU, 15U),
    kCLOCK_Crc0 = CLK_GATE_DEFINE(0x103CU, 18U),
    kCLOCK_Usbdcd0 = CLK_GATE_DEFINE(0x103CU, 21U),
    kCLOCK_Pdb0 = CLK_GATE_DEFINE(0x103CU, 22U),
    kCLOCK_Pit0 = CLK_GATE_DEFINE(0x103CU, 23U),
    kCLOCK_Ftm0 = CLK_GATE_DEFINE(0x103CU, 24U),
    kCLOCK_Ftm1 = CLK_GATE_DEFINE(0x103CU, 25U),
    kCLOCK_Ftm2 = CLK_GATE_DEFINE(0x103CU, 26U),
    kCLOCK_Adc0 = CLK_GATE_DEFINE(0x103CU, 27U),
    kCLOCK_Rtc0 = CLK_GATE_DEFINE(0x103CU, 29U),

    kCLOCK_Flexbus0 = CLK_GATE_DEFINE(0x1040U, 0U),
    kCLOCK_Dma0 = CLK_GATE_DEFINE(0x1040U, 1U),
    kCLOCK_Mpu0 = CLK_GATE_DEFINE(0x1040U, 2U),
    kCLOCK_Sdramc0 = CLK_GATE_DEFINE(0x1040U, 3U),
} clock_ip_name_t;

/*!@brief SIM configuration structure for clock setting. */
typedef struct _sim_clock_config
{
    uint8_t pllFllSel;  /*!< PLL/FLL/IRC48M selection.         */
    uint8_t pllFllDiv;  /*!< PLLFLLSEL clock divider divisor.  */
    uint8_t pllFllFrac; /*!< PLLFLLSEL clock divider fraction. */
    uint8_t er32kSrc;   /*!< ERCLK32K source selection.        */
    uint32_t clkdiv1;   /*!< SIM_CLKDIV1.                      */
} sim_clock_config_t;

/*! @brief OSC work mode. */
typedef enum _osc_mode
{
    kOSC_ModeExt = 0U, /*!< Use external clock.   */
#if (defined(MCG_C2_EREFS_MASK) && !(defined(MCG_C2_EREFS0_MASK)))
    kOSC_ModeOscLowPower = MCG_C2_EREFS_MASK, /*!< Oscillator low power. */
#else
    kOSC_ModeOscLowPower = MCG_C2_EREFS0_MASK, /*!< Oscillator low power. */
#endif
    kOSC_ModeOscHighGain = 0U
#if (defined(MCG_C2_EREFS_MASK) && !(defined(MCG_C2_EREFS0_MASK)))
                           |
                           MCG_C2_EREFS_MASK
#else
                           |
                           MCG_C2_EREFS0_MASK
#endif
#if (defined(MCG_C2_HGO_MASK) && !(defined(MCG_C2_HGO0_MASK)))
                           |
                           MCG_C2_HGO_MASK, /*!< Oscillator high gain. */
#else
                           |
                           MCG_C2_HGO0_MASK, /*!< Oscillator high gain. */
#endif
} osc_mode_t;

/*! @brief Oscillator capacitor load setting.*/
enum _osc_cap_load
{
    kOSC_Cap2P = OSC_CR_SC2P_MASK,  /*!< 2  pF capacitor load */
    kOSC_Cap4P = OSC_CR_SC4P_MASK,  /*!< 4  pF capacitor load */
    kOSC_Cap8P = OSC_CR_SC8P_MASK,  /*!< 8  pF capacitor load */
    kOSC_Cap16P = OSC_CR_SC16P_MASK /*!< 16 pF capacitor load */
};

/*! @brief OSCERCLK enable mode. */
enum _oscer_enable_mode
{
    kOSC_ErClkEnable = OSC_CR_ERCLKEN_MASK,       /*!< Enable.              */
    kOSC_ErClkEnableInStop = OSC_CR_EREFSTEN_MASK /*!< Enable in stop mode. */
};

/*! @brief OSC configuration for OSCERCLK. */
typedef struct _oscer_config
{
    uint8_t enableMode; /*!< OSCERCLK enable mode. OR'ed value of @ref _oscer_enable_mode. */

    uint8_t erclkDiv; /*!< Divider for OSCERCLK.*/
} oscer_config_t;

/*!
 * @brief OSC Initialization Configuration Structure
 *
 * Defines the configuration data structure to initialize the OSC.
 * When porting to a new board, please set the following members
 * according to board setting:
 * 1. freq: The external frequency.
 * 2. workMode: The OSC module mode.
 */
typedef struct _osc_config
{
    uint32_t freq;              /*!< External clock frequency.    */
    uint8_t capLoad;            /*!< Capacitor load setting.      */
    osc_mode_t workMode;        /*!< OSC work mode setting.       */
    oscer_config_t oscerConfig; /*!< Configuration for OSCERCLK.  */
} osc_config_t;

/*! @brief MCG FLL reference clock source select. */
typedef enum _mcg_fll_src
{
    kMCG_FllSrcExternal, /*!< External reference clock is selected          */
    kMCG_FllSrcInternal  /*!< The slow internal reference clock is selected */
} mcg_fll_src_t;

/*! @brief MCG internal reference clock select */
typedef enum _mcg_irc_mode
{
    kMCG_IrcSlow, /*!< Slow internal reference clock selected */
    kMCG_IrcFast  /*!< Fast internal reference clock selected */
} mcg_irc_mode_t;

/*! @brief MCG DCO Maximum Frequency with 32.768 kHz Reference */
typedef enum _mcg_dmx32
{
    kMCG_Dmx32Default, /*!< DCO has a default range of 25% */
    kMCG_Dmx32Fine     /*!< DCO is fine-tuned for maximum frequency with 32.768 kHz reference */
} mcg_dmx32_t;

/*! @brief MCG DCO range select */
typedef enum _mcg_drs
{
    kMCG_DrsLow,     /*!< Low frequency range       */
    kMCG_DrsMid,     /*!< Mid frequency range       */
    kMCG_DrsMidHigh, /*!< Mid-High frequency range  */
    kMCG_DrsHigh     /*!< High frequency range      */
} mcg_drs_t;

/*! @brief MCG PLL reference clock select */
typedef enum _mcg_pll_ref_src
{
    kMCG_PllRefOsc0, /*!< Selects OSC0 as PLL reference clock                 */
    kMCG_PllRefOsc1  /*!< Selects OSC1 as PLL reference clock                 */
} mcg_pll_ref_src_t;

/*! @brief MCGOUT clock source. */
typedef enum _mcg_clkout_src
{
    kMCG_ClkOutSrcOut,      /*!< Output of the FLL is selected (reset default)  */
    kMCG_ClkOutSrcInternal, /*!< Internal reference clock is selected           */
    kMCG_ClkOutSrcExternal, /*!< External reference clock is selected           */
} mcg_clkout_src_t;

/*! @brief MCG Automatic Trim Machine Select */
typedef enum _mcg_atm_select
{
    kMCG_AtmSel32k, /*!< 32 kHz Internal Reference Clock selected  */
    kMCG_AtmSel4m   /*!< 4 MHz Internal Reference Clock selected   */
} mcg_atm_select_t;

/*! @brief MCG OSC Clock Select */
typedef enum _mcg_oscsel
{
    kMCG_OscselOsc, /*!< Selects System Oscillator (OSCCLK) */
    kMCG_OscselRtc, /*!< Selects 32 kHz RTC Oscillator      */
    kMCG_OscselIrc  /*!< Selects 48 MHz IRC Oscillator      */
} mcg_oscsel_t;

/*! @brief MCG PLLCS select */
typedef enum _mcg_pll_clk_select
{
    kMCG_PllClkSelPll0,  /*!< PLL0 output clock is selected  */
    kMCG_PllClkSelExtPll /* External PLL clock is selected   */
} mcg_pll_clk_select_t;

/*! @brief MCG clock monitor mode. */
typedef enum _mcg_monitor_mode
{
    kMCG_MonitorNone, /*!< Clock monitor is disabled.         */
    kMCG_MonitorInt,  /*!< Trigger interrupt when clock lost. */
    kMCG_MonitorReset /*!< System reset when clock lost.      */
} mcg_monitor_mode_t;

/*! @brief MCG status. */
enum _mcg_status
{
    kStatus_MCG_ModeUnreachable = MAKE_STATUS(kStatusGroup_MCG, 0),       /*!< Can't switch to target mode. */
    kStatus_MCG_ModeInvalid = MAKE_STATUS(kStatusGroup_MCG, 1),           /*!< Current mode invalid for the specific
                                                                               function. */
    kStatus_MCG_AtmBusClockInvalid = MAKE_STATUS(kStatusGroup_MCG, 2),    /*!< Invalid bus clock for ATM. */
    kStatus_MCG_AtmDesiredFreqInvalid = MAKE_STATUS(kStatusGroup_MCG, 3), /*!< Invalid desired frequency for ATM. */
    kStatus_MCG_AtmIrcUsed = MAKE_STATUS(kStatusGroup_MCG, 4),            /*!< IRC is used when using ATM. */
    kStatus_MCG_AtmHardwareFail = MAKE_STATUS(kStatusGroup_MCG, 5),       /*!< Hardware fail occurs during ATM. */
    kStatus_MCG_SourceUsed = MAKE_STATUS(kStatusGroup_MCG, 6)             /*!< Could not change clock source because
                                                                               it is used currently. */
};

/*! @brief MCG status flags. */
enum _mcg_status_flags_t
{
    kMCG_Osc0LostFlag = (1U << 0U),   /*!< OSC0 lost.         */
    kMCG_Osc0InitFlag = (1U << 1U),   /*!< OSC0 crystal initialized. */
    kMCG_RtcOscLostFlag = (1U << 4U), /*!< RTC OSC lost.      */
    kMCG_Pll0LostFlag = (1U << 5U),   /*!< PLL0 lost.         */
    kMCG_Pll0LockFlag = (1U << 6U),   /*!< PLL0 locked.       */
    kMCG_ExtPllLostFlag = (1U << 9U), /*!< External PLL lost. */
};

/*! @brief MCG internal reference clock (MCGIRCLK) enable mode definition. */
enum _mcg_irclk_enable_mode
{
    kMCG_IrclkEnable = MCG_C1_IRCLKEN_MASK,       /*!< MCGIRCLK enable.              */
    kMCG_IrclkEnableInStop = MCG_C1_IREFSTEN_MASK /*!< MCGIRCLK enable in stop mode. */
};

/*! @brief MCG PLL clock enable mode definition. */
enum _mcg_pll_enable_mode
{
    kMCG_PllEnableIndependent = MCG_C5_PLLCLKEN0_MASK, /*!< MCGPLLCLK enable indepencent of
                                                           MCG clock mode. Generally, PLL
                                                           is disabled in FLL modes
                                                           (FEI/FBI/FEE/FBE), set PLL clock
                                                           enable independent will enable
                                                           PLL in the FLL modes.          */
    kMCG_PllEnableInStop = MCG_C5_PLLSTEN0_MASK        /*!< MCGPLLCLK enable in STOP mode. */
};

/*! @brief MCG mode definitions */
typedef enum _mcg_mode
{
    kMCG_ModeFEI = 0U, /*!< FEI   - FLL Engaged Internal         */
    kMCG_ModeFBI,      /*!< FBI   - FLL Bypassed Internal        */
    kMCG_ModeBLPI,     /*!< BLPI  - Bypassed Low Power Internal  */
    kMCG_ModeFEE,      /*!< FEE   - FLL Engaged External         */
    kMCG_ModeFBE,      /*!< FBE   - FLL Bypassed External        */
    kMCG_ModeBLPE,     /*!< BLPE  - Bypassed Low Power External  */
    kMCG_ModePBE,      /*!< PBE   - PLL Bypassed External        */
    kMCG_ModePEE,      /*!< PEE   - PLL Engaged External         */
    kMCG_ModeError     /*!< Unknown mode                         */
} mcg_mode_t;

/*! @brief MCG PLL configuration. */
typedef struct _mcg_pll_config
{
    uint8_t enableMode; /*!< Enable mode. OR'ed value of @ref _mcg_pll_enable_mode. */
    uint8_t prdiv;      /*!< Reference divider PRDIV.    */
    uint8_t vdiv;       /*!< VCO divider VDIV.           */
} mcg_pll_config_t;

/*! @brief MCG configure structure for mode change.
 *
 * When porting to a new board, please set the following members
 * according to board setting:
 * 1. frdiv: If FLL uses the external reference clock, please set this
 *    value to make sure external reference clock divided by frdiv is
 *    in the range 31.25kHz to 39.0625kHz.
 * 2. The PLL reference clock divider PRDIV: PLL reference clock frequency after
 *    PRDIV should be in the range of FSL_FEATURE_MCG_PLL_REF_MIN to
 *    FSL_FEATURE_MCG_PLL_REF_MAX.
 */
typedef struct _mcg_config
{
    mcg_mode_t mcgMode; /*!< MCG mode.                   */

    /* ----------------------- MCGIRCCLK settings ------------------------ */
    uint8_t irclkEnableMode; /*!< MCGIRCLK enable mode.       */
    mcg_irc_mode_t ircs;     /*!< Source, MCG_C2[IRCS].       */
    uint8_t fcrdiv;          /*!< Divider, MCG_SC[FCRDIV].    */

    /* ------------------------ MCG FLL settings ------------------------- */
    uint8_t frdiv;       /*!< Divider MCG_C1[FRDIV].      */
    mcg_drs_t drs;       /*!< DCO range MCG_C4[DRST_DRS]. */
    mcg_dmx32_t dmx32;   /*!< MCG_C4[DMX32].              */
    mcg_oscsel_t oscsel; /*!< OSC select MCG_C7[OSCSEL].  */

    /* ------------------------ MCG PLL settings ------------------------- */
    mcg_pll_config_t pll0Config; /*!< MCGPLL0CLK configuration.   */

    mcg_pll_clk_select_t pllcs; /*!< PLL select as output, PLLCS.*/

} mcg_config_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Enable the clock for specific IP.
 *
 * @param name  Which clock to enable, see \ref clock_ip_name_t.
 */
static inline void CLOCK_EnableClock(clock_ip_name_t name)
{
    uint32_t regAddr = SIM_BASE + CLK_GATE_ABSTRACT_REG_OFFSET((uint32_t)name);
    (*(volatile uint32_t *)regAddr) |= (1U << CLK_GATE_ABSTRACT_BITS_SHIFT((uint32_t)name));
}

/*!
 * @brief Disable the clock for specific IP.
 *
 * @param name  Which clock to disable, see \ref clock_ip_name_t.
 */
static inline void CLOCK_DisableClock(clock_ip_name_t name)
{
    uint32_t regAddr = SIM_BASE + CLK_GATE_ABSTRACT_REG_OFFSET((uint32_t)name);
    (*(volatile uint32_t *)regAddr) &= ~(1U << CLK_GATE_ABSTRACT_BITS_SHIFT((uint32_t)name));
}

/*!
 * @brief Set ERCLK32K source.
 *
 * @param src The value to set ERCLK32K clock source.
 */
static inline void CLOCK_SetEr32kClock(uint32_t src)
{
    SIM->SOPT1 = ((SIM->SOPT1 & ~SIM_SOPT1_OSC32KSEL_MASK) | SIM_SOPT1_OSC32KSEL(src));
}

/*!
 * @brief Set SDHC0 clock source.
 *
 * @param src The value to set SDHC0 clock source.
 */
static inline void CLOCK_SetSdhc0Clock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_SDHCSRC_MASK) | SIM_SOPT2_SDHCSRC(src));
}

/*!
 * @brief Set LPUART clock source.
 *
 * @param src The value to set LPUART clock source.
 */
static inline void CLOCK_SetLpuartClock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_LPUARTSRC_MASK) | SIM_SOPT2_LPUARTSRC(src));
}

/*!
 * @brief Set TPM clock source.
 *
 * @param src The value to set TPM clock source.
 */
static inline void CLOCK_SetTpmClock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_TPMSRC_MASK) | SIM_SOPT2_TPMSRC(src));
}

/*!
 * @brief Set debug trace clock source.
 *
 * @param src The value to set debug trace clock source.
 */
static inline void CLOCK_SetTraceClock(uint32_t src, uint32_t divValue, uint32_t fracValue)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_TRACECLKSEL_MASK) | SIM_SOPT2_TRACECLKSEL(src));
    SIM->CLKDIV4 = SIM_CLKDIV4_TRACEDIV(divValue) | SIM_CLKDIV4_TRACEFRAC(fracValue);
}

/*!
 * @brief Set PLLFLLSEL clock source.
 *
 * @param src The value to set PLLFLLSEL clock source.
 */
static inline void CLOCK_SetPllFllSelClock(uint32_t src, uint32_t divValue, uint32_t fracValue)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_PLLFLLSEL_MASK) | SIM_SOPT2_PLLFLLSEL(src));
    SIM->CLKDIV3 = SIM_CLKDIV3_PLLFLLDIV(divValue) | SIM_CLKDIV3_PLLFLLFRAC(fracValue);
}

/*!
 * @brief Set CLKOUT source.
 *
 * @param src The value to set CLKOUT source.
 */
static inline void CLOCK_SetClkOutClock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_CLKOUTSEL_MASK) | SIM_SOPT2_CLKOUTSEL(src));
}

/*!
 * @brief Set RTC_CLKOUT source.
 *
 * @param src The value to set RTC_CLKOUT source.
 */
static inline void CLOCK_SetRtcClkOutClock(uint32_t src)
{
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_RTCCLKOUTSEL_MASK) | SIM_SOPT2_RTCCLKOUTSEL(src));
}

/*! @brief Enable USB HS clock.
 *
 * @param src  USB HS clock source.
 * @param freq The frequency specified by src.
 * @retval true The clock is set successfully.
 * @retval false The clock source is invalid to get proper USB HS clock.
 */
bool CLOCK_EnableUsbhs0Clock(clock_usb_src_t src, uint32_t freq);

/*! @brief Disable USB HS clock.
 *
 * Disable USB HS clock.
 */
static inline void CLOCK_DisableUsbhs0Clock(void)
{
    SIM->SOPT2 &= ~SIM_SOPT2_USBREGEN_MASK;
    SIM->SCGC3 &= ~(SIM_SCGC3_USBHS_MASK | SIM_SCGC3_USBHSPHY_MASK);
}

/*! @brief Enable USB FS clock.
 *
 * @param src  USB FS clock source.
 * @param freq The frequency specified by src.
 * @retval true The clock is set successfully.
 * @retval false The clock source is invalid to get proper USB FS clock.
 */
bool CLOCK_EnableUsbfs0Clock(clock_usb_src_t src, uint32_t freq);

/*! @brief Disable USB FS clock.
 *
 * Disable USB FS clock.
 */
static inline void CLOCK_DisableUsbfs0Clock(void)
{
    CLOCK_DisableClock(kCLOCK_Usbfs0);
}

/*!
 * @brief System clock divider
 *
 * Set the SIM_CLKDIV1[OUTDIV1], SIM_CLKDIV1[OUTDIV2], SIM_CLKDIV1[OUTDIV3], SIM_CLKDIV1[OUTDIV4].
 *
 * @param outdiv1 Clock 1 output divider value.
 *
 * @param outdiv2 Clock 2 output divider value.
 *
 * @param outdiv3 Clock 3 output divider value.
 *
 * @param outdiv4 Clock 4 output divider value.
 */
static inline void CLOCK_SetOutDiv(uint32_t outdiv1, uint32_t outdiv2, uint32_t outdiv3, uint32_t outdiv4)
{
    SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(outdiv1) | SIM_CLKDIV1_OUTDIV2(outdiv2) | SIM_CLKDIV1_OUTDIV3(outdiv3) |
                   SIM_CLKDIV1_OUTDIV4(outdiv4);
}

/*!
 * @brief Gets the clock frequency for a specific clock name.
 *
 * This function checks the current clock configurations and then calculates
 * the clock frequency for a specific clock name defined in clock_name_t.
 * The MCG must be properly configured before using this function.
 *
 * @param clockName Clock names defined in clock_name_t
 * @return Clock frequency value in Hertz
 */
uint32_t CLOCK_GetFreq(clock_name_t clockName);

/*!
 * @brief Get the core clock or system clock frequency.
 *
 * @return Clock frequency in Hz.
 */
uint32_t CLOCK_GetCoreSysClkFreq(void);

/*!
 * @brief Get the platform clock frequency.
 *
 * @return Clock frequency in Hz.
 */
uint32_t CLOCK_GetPlatClkFreq(void);

/*!
 * @brief Get the bus clock frequency.
 *
 * @return Clock frequency in Hz.
 */
uint32_t CLOCK_GetBusClkFreq(void);

/*!
 * @brief Get the flexbus clock frequency.
 *
 * @return Clock frequency in Hz.
 */
uint32_t CLOCK_GetFlexBusClkFreq(void);

/*!
 * @brief Get the flash clock frequency.
 *
 * @return Clock frequency in Hz.
 */
uint32_t CLOCK_GetFlashClkFreq(void);

/*!
 * @brief Get the output clock frequency selected by SIM[PLLFLLSEL].
 *
 * @return Clock frequency in Hz.
 */
uint32_t CLOCK_GetPllFllSelClkFreq(void);

/*!
 * @brief Get the external reference 32K clock frequency (ERCLK32K).
 *
 * @return Clock frequency in Hz.
 */
uint32_t CLOCK_GetEr32kClkFreq(void);

/*!
 * @brief Get the OSC0 external reference clock frequency (OSC0ERCLK).
 *
 * @return Clock frequency in Hz.
 */
uint32_t CLOCK_GetOsc0ErClkFreq(void);

/*!
 * @brief Get the OSC0 external reference undivided clock frequency (OSC0ERCLK_UNDIV).
 *
 * @return Clock frequency in Hz.
 */
uint32_t CLOCK_GetOsc0ErClkUndivFreq(void);

/*!
 * @brief Set the clock configure in SIM module.
 *
 * This function sets system layer clock settings in SIM module.
 *
 * @param config Pointer to the configure structure.
 */
void CLOCK_SetSimConfig(sim_clock_config_t const *config);

/*!
 * @brief Set the system clock dividers in SIM to safe value.
 *
 * The system level clocks (core clock, bus clock, flexbus clock and flash clock)
 * must be in allowed ranges. During MCG clock mode switch, the MCG output clock
 * changes then the system level clocks may be out of range. This function could
 * be used before MCG mode change, to make sure system level clocks are in allowed
 * range.
 *
 * @param config Pointer to the configure structure.
 */
static inline void CLOCK_SetSimSafeDivs(void)
{
    SIM->CLKDIV1 = 0x03770000U;
}

/*! @name MCG frequency functions. */
/*@{*/

/*!
 * @brief Get the MCG output clock(MCGOUTCLK) frequency.
 *
 * This function gets the MCG output clock frequency (Hz) based on current MCG
 * register value.
 *
 * @return The frequency of MCGOUTCLK.
 */
uint32_t CLOCK_GetOutClkFreq(void);

/*!
 * @brief Get the MCG FLL clock(MCGFLLCLK) frequency.
 *
 * This function gets the MCG FLL clock frequency (Hz) based on current MCG
 * register value. The FLL is only enabled in FEI/FBI/FEE/FBE mode, in other
 * modes, FLL is disabled in low power state.
 *
 * @return The frequency of MCGFLLCLK.
 */
uint32_t CLOCK_GetFllFreq(void);

/*!
 * @brief Get the MCG internal reference clock(MCGIRCLK) frequency.
 *
 * This function gets the MCG internal reference clock frequency (Hz) based
 * on current MCG register value.
 *
 * @return The frequency of MCGIRCLK.
 */
uint32_t CLOCK_GetInternalRefClkFreq(void);

/*!
 * @brief Get the MCG fixed frequency clock(MCGFFCLK) frequency.
 *
 * This function gets the MCG fixed frequency clock frequency (Hz) based
 * on current MCG register value.
 *
 * @return The frequency of MCGFFCLK.
 */
uint32_t CLOCK_GetFixedFreqClkFreq(void);

/*!
 * @brief Get the MCG PLL0 clock(MCGPLL0CLK) frequency.
 *
 * This function gets the MCG PLL0 clock frequency (Hz) based on current MCG
 * register value.
 *
 * @return The frequency of MCGPLL0CLK.
 */
uint32_t CLOCK_GetPll0Freq(void);

/*!
 * @brief Get the MCG external PLL frequency.
 *
 * This function gets the MCG external PLL frequency (Hz).
 *
 * @return The frequency of MCG external PLL.
 */
uint32_t CLOCK_GetExtPllFreq(void);

/*!
 * @brief Set the MCG external PLL frequency.
 *
 * This function sets the MCG external PLL frequency (Hz), the MCG external PLL
 * frequency is passed in to MCG driver through this function. Please call this
 * function after the external PLL frequency is changed, otherwise the APIs for
 * get frequency may returns wrong value.
 *
 * @param The frequency of MCG external PLL.
 */
void CLOCK_SetExtPllFreq(uint32_t freq);

/*@}*/

/*! @name MCG clock configuration. */
/*@{*/

/*!
 * @brief Enable or disable MCG low power.
 *
 * Enable MCG low power will disable the PLL and FLL in bypass modes. That is,
 * in FBE and PBE modes, enable low power will set MCG to BLPE mode, in FBI and
 * PBI mode, enable low power will set MCG to BLPI mode.
 * When disable MCG low power, the PLL or FLL will be enabled based on MCG setting.
 *
 * @param enable True to enable MCG low power, false to disable MCG low power.
 */
static inline void CLOCK_SetLowPowerEnable(bool enable)
{
    if (enable)
    {
        MCG->C2 |= MCG_C2_LP_MASK;
    }
    else
    {
        MCG->C2 &= ~MCG_C2_LP_MASK;
    }
}

/*!
 * @brief Configure the Internal Reference clock (MCGIRCLK)
 *
 * This function setups the \c MCGIRCLK base on parameters. It selects the IRC
 * source, if fast IRC is used, this function also sets the fast IRC divider.
 * This function also sets whether enable \c MCGIRCLK in stop mode.
 * Calling this function in FBI/PBI/BLPI modes may change the system clock, so
 * it is not allowed to use this in these modes.
 *
 * @param enableMode MCGIRCLK enable mode, OR'ed value of @ref _mcg_irclk_enable_mode.
 * @param ircs       MCGIRCLK clock source, choose fast or slow.
 * @param fcrdiv     Fast IRC divider setting (\c FCRDIV).
 * @retval kStatus_MCG_SourceUsed MCGIRCLK is used as system clock, should not configure MCGIRCLK.
 * @retval kStatus_Success MCGIRCLK configuration finished successfully.
 */
status_t CLOCK_SetInternalRefClkConfig(uint8_t enableMode, mcg_irc_mode_t ircs, uint8_t fcrdiv);

/*!
 * @brief Select the MCG external reference clock.
 *
 * Select the MCG external reference clock source, it changes the MCG_C7[OSCSEL]
 * and wait for the clock source stable. Should not change external reference
 * clock in FEE/FBE/BLPE/PBE/PEE mdes, so don't call this function in these modes.
 *
 * @param oscsel MCG external reference clock source, MCG_C7[OSCSEL].
 * @retval kStatus_MCG_SourceUsed External reference clock is used, should not change.
 * @retval kStatus_Success External reference clock set successfully.
 */
status_t CLOCK_SetExternalRefClkConfig(mcg_oscsel_t oscsel);

/*!
 * @brief Enables the PLL0 in FLL mode.
 *
 * This function setups the PLL0 in FLL mode, make sure the PLL reference
 * clock is enabled before calling this function. This function reconfigures
 * the PLL0, make sure the PLL0 is not used as a clock source while calling
 * this function. The function CLOCK_CalcPllDiv can help to get the proper PLL
 * divider values.
 *
 * @param config Pointer to the configuration structure.
 */
void CLOCK_EnablePll0(mcg_pll_config_t const *config);

/*!
 * @brief Disables the PLL0 in FLL mode.
 *
 * This function disables the PLL0 in FLL mode, it should be used together with
 * @ref CLOCK_EnablePll0.
 */
static inline void CLOCK_DisablePll0(void)
{
    MCG->C5 &= ~(MCG_C5_PLLCLKEN0_MASK | MCG_C5_PLLSTEN0_MASK);
}

/*!
 * @brief Calculates the PLL divider setting for desired output frequency.
 *
 * This function calculates the proper reference clock divider (\c PRDIV) and
 * VCO divider (\c VDIV) to generate desired PLL output frequency. It returns the
 * closest frequency PLL could generate, the corresponding \c PRDIV/VDIV are
 * returned from parameters. If desired frequency is not valid, this function
 * returns 0.
 *
 * @param refFreq    PLL reference clock frequency.
 * @param desireFreq Desired PLL output frequency.
 * @param prdiv      PRDIV value to generate desired PLL frequency.
 * @param vdiv       VDIV value to generate desired PLL frequency.
 * @return Closest frequency PLL could generate.
 */
uint32_t CLOCK_CalcPllDiv(uint32_t refFreq, uint32_t desireFreq, uint8_t *prdiv, uint8_t *vdiv);

/*@}*/

/*! @name MCG clock lock monitor functions. */
/*@{*/

/*!
 * @brief Set the OSC0 clock monitor mode.
 *
 * Set the OSC0 clock monitor mode, see @ref mcg_monitor_mode_t for details.
 *
 * @param mode The monitor mode to set.
 */
void CLOCK_SetOsc0MonitorMode(mcg_monitor_mode_t mode);

/*!
 * @brief Set the RTC OSC clock monitor mode.
 *
 * Set the RTC OSC clock monitor mode, see @ref mcg_monitor_mode_t for details.
 *
 * @param mode The monitor mode to set.
 */
void CLOCK_SetRtcOscMonitorMode(mcg_monitor_mode_t mode);

/*!
 * @brief Set the PLL0 clock monitor mode.
 *
 * Set the PLL0 clock monitor mode, see @ref mcg_monitor_mode_t for details.
 *
 * @param mode The monitor mode to set.
 */
void CLOCK_SetPll0MonitorMode(mcg_monitor_mode_t mode);

/*!
 * @brief Set the external PLL clock monitor mode.
 *
 * Set the external PLL clock monitor mode, see @ref mcg_monitor_mode_t
 * for details.
 *
 * @param mode The monitor mode to set.
 */
void CLOCK_SetExtPllMonitorMode(mcg_monitor_mode_t mode);

/*!
 * @brief Get the MCG status flags.
 *
 * This function gets the MCG clock status flags, all the status flags are
 * returned as a logical OR of the enumeration @ref _mcg_status_flags_t. To
 * check specific flags, compare the return value with the flags.
 *
 * Example:
 * @code
   // To check the clock lost lock status of OSC0 and PLL0.
   uint32_t mcgFlags;

   mcgFlags = CLOCK_GetStatusFlags();

   if (mcgFlags & kMCG_Osc0LostFlag)
   {
       // OSC0 clock lock lost. Do something.
   }
   if (mcgFlags & kMCG_Pll0LostFlag)
   {
       // PLL0 clock lock lost. Do something.
   }
   @endcode
 *
 * @return  Logical OR value of the @ref _mcg_status_flags_t.
 */
uint32_t CLOCK_GetStatusFlags(void);

/*!
 * @brief Clears the MCG status flags.
 *
 * This function clears the MCG clock lock lost status. The parameter is logical
 * OR value of the flags to clear, see @ref _mcg_status_flags_t.
 *
 * Example:
 * @code
   // To clear the clock lost lock status flags of OSC0 and PLL0.

   CLOCK_ClearStatusFlags(kMCG_Osc0LostFlag | kMCG_Pll0LostFlag);
   @endcode
 *
 * @param mask The status flags to clear. This is a logical OR of members of the
 *             enumeration @ref _mcg_status_flags_t.
 */
void CLOCK_ClearStatusFlags(uint32_t mask);

/*@}*/

/*!
 * @name OSC configuration
 * @{
 */

/*!
 * @brief Configures the OSC external reference clock (OSCERCLK).
 *
 * This function configures the OSC external reference clock (OSCERCLK).
 * For example, to enable the OSCERCLK in normal mode and stop mode, and also set
 * the output divider to 1, as follows:
 *
   @code
   oscer_config_t config =
   {
       .enableMode = kOSC_ErClkEnable | kOSC_ErClkEnableInStop,
       .erclkDiv   = 1U,
   };

   OSC_SetExtRefClkConfig(OSC, &config);
   @endcode
 *
 * @param base   OSC peripheral address.
 * @param config Pointer to the configuration structure.
 */
static inline void OSC_SetExtRefClkConfig(OSC_Type *base, oscer_config_t const *config)
{
    uint8_t reg = base->CR;

    reg &= ~(OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK);
    reg |= config->enableMode;

    base->CR = reg;

    base->DIV = OSC_DIV_ERPS(config->erclkDiv);
}

/*!
 * @brief Sets the capacitor load configuration for the oscillator.
 *
 * This function sets the specified capacitors configuration for the oscillator.
 * This should be done in the early system level initialization function call
 * based on the system configuration.
 *
 * @param base   OSC peripheral address.
 * @param capLoad OR'ed value for the capacitor load option, see \ref _osc_cap_load.
 *
 * Example:
   @code
   // To enable only 2 pF and 8 pF capacitor load, please use like this.
   OSC_SetCapLoad(OSC, kOSC_Cap2P | kOSC_Cap8P);
   @endcode
 */
static inline void OSC_SetCapLoad(OSC_Type *base, uint8_t capLoad)
{
    uint8_t reg = base->CR;

    reg &= ~(OSC_CR_SC2P_MASK | OSC_CR_SC4P_MASK | OSC_CR_SC8P_MASK | OSC_CR_SC16P_MASK);
    reg |= capLoad;

    base->CR = reg;
}

/*!
 * @brief Initialize OSC0.
 *
 * This function initializes OSC0 according to board configuration.
 *
 * @param  config Pointer to the OSC0 configuration structure.
 */
void CLOCK_InitOsc0(osc_config_t const *config);

/*!
 * @brief Deinitialize OSC0.
 *
 * This function deinitializes OSC0.
 */
void CLOCK_DeinitOsc0(void);

/* @} */

/*!
 * @name External clock frequency
 * @{
 */

/*!
 * @brief Set the XTAL0 frequency based on board setting.
 *
 * @param freq The XTAL0/EXTAL0 input clock frequency in Hz.
 */
static inline void CLOCK_SetXtal0Freq(uint32_t freq)
{
    g_xtal0Freq = freq;
}

/*!
 * @brief Set the XTAL32/RTC_CLKIN frequency based on board setting.
 *
 * @param freq The XTAL32/EXTAL32/RTC_CLKIN input clock frequency in Hz.
 */
static inline void CLOCK_SetXtal32Freq(uint32_t freq)
{
    g_xtal32Freq = freq;
}
/* @} */

/*!
 * @name MCG auto-trim machine.
 * @{
 */

/*!
 * @brief Auto trim the internal reference clock.
 *
 * This function trims the internal reference clock using external clock. If
 * successful, it returns the kStatus_Success and the frequency after
 * trimming is received in the parameter @p actualFreq. If an error occurs,
 * the error code is returned.
 *
 * @param extFreq      External clock frequency, should be bus clock.
 * @param desireFreq   Frequency want to trim to.
 * @param actualFreq   Actual frequency after trim.
 * @param atms         Trim fast or slow internal reference clock.
 * @retval kStatus_Success ATM success.
 * @retval kStatus_MCG_AtmBusClockInvalid The bus clock is not in allowed range for ATM.
 * @retval kStatus_MCG_AtmDesiredFreqInvalid MCGIRCLK could not be trimmed to the desired frequency.
 * @retval kStatus_MCG_AtmIrcUsed Could not trim because MCGIRCLK is used as bus clock source.
 * @retval kStatus_MCG_AtmHardwareFail Hardware fails during trim.
 */
status_t CLOCK_TrimInternalRefClk(uint32_t extFreq, uint32_t desireFreq, uint32_t *actualFreq, mcg_atm_select_t atms);
/* @} */

/*! @name MCG mode functions. */
/*@{*/

/*!
 * @brief Gets the current MCG mode.
 *
 * This function checks the MCG registers and determine current MCG mode.
 *
 * @return Current MCG mode or error code, see @ref mcg_mode_t.
 */
mcg_mode_t CLOCK_GetMode(void);

/*!
 * @brief Set MCG to FEI mode.
 *
 * This function sets MCG to FEI mode. If could not set to FEI mode directly
 * from current mode, this function returns error.
 *
 * @param       dmx32  DMX32 in FEI mode.
 * @param       drs The DCO range selection.
 * @param       fllStableDelay Delay function to make sure FLL is stable, if pass
 *              in NULL, then does not delay.
 * @retval kStatus_MCG_ModeUnreachable Could not switch to the target mode.
 * @retval kStatus_Success Switch to target mode successfully.
 * @note If @p dmx32 is set to kMCG_Dmx32Fine, the slow IRC must not be trimmed
 * to frequency above 32768Hz.
 */
status_t CLOCK_SetFeiMode(mcg_dmx32_t dmx32, mcg_drs_t drs, void (*fllStableDelay)(void));

/*!
 * @brief Set MCG to FEE mode.
 *
 * This function sets MCG to FEE mode. If could not set to FEE mode directly
 * from current mode, this function returns error.
 *
 * @param   frdiv  FLL reference clock divider setting, FRDIV.
 * @param   dmx32  DMX32 in FEE mode.
 * @param   drs    The DCO range selection.
 * @param   fllStableDelay Delay function to make sure FLL is stable, if pass
 *          in NULL, then does not delay.
 *
 * @retval kStatus_MCG_ModeUnreachable Could not switch to the target mode.
 * @retval kStatus_Success Switch to target mode successfully.
 */
status_t CLOCK_SetFeeMode(uint8_t frdiv, mcg_dmx32_t dmx32, mcg_drs_t drs, void (*fllStableDelay)(void));

/*!
 * @brief Set MCG to FBI mode.
 *
 * This function sets MCG to FBI mode. If could not set to FBI mode directly
 * from current mode, this function returns error.
 *
 * @param  dmx32  DMX32 in FBI mode.
 * @param  drs  The DCO range selection.
 * @param  fllStableDelay Delay function to make sure FLL is stable. If FLL
 *         is not used in FBI mode, this parameter could be NULL. Pass in
 *         NULL does not delay.
 * @retval kStatus_MCG_ModeUnreachable Could not switch to the target mode.
 * @retval kStatus_Success Switch to target mode successfully.
 * @note If @p dmx32 is set to kMCG_Dmx32Fine, the slow IRC must not be trimmed
 * to frequency above 32768Hz.
 */
status_t CLOCK_SetFbiMode(mcg_dmx32_t dmx32, mcg_drs_t drs, void (*fllStableDelay)(void));

/*!
 * @brief Set MCG to FBE mode.
 *
 * This function sets MCG to FBE mode. If could not set to FBE mode directly
 * from current mode, this function returns error.
 *
 * @param   frdiv  FLL reference clock divider setting, FRDIV.
 * @param   dmx32  DMX32 in FBE mode.
 * @param   drs    The DCO range selection.
 * @param   fllStableDelay Delay function to make sure FLL is stable. If FLL
 *          is not used in FBE mode, this parameter could be NULL. Pass in NULL
 *          does not delay.
 * @retval kStatus_MCG_ModeUnreachable Could not switch to the target mode.
 * @retval kStatus_Success Switch to target mode successfully.
 */
status_t CLOCK_SetFbeMode(uint8_t frdiv, mcg_dmx32_t dmx32, mcg_drs_t drs, void (*fllStableDelay)(void));

/*!
 * @brief Set MCG to BLPI mode.
 *
 * This function sets MCG to BLPI mode. If could not set to BLPI mode directly
 * from current mode, this function returns error.
 *
 * @retval kStatus_MCG_ModeUnreachable Could not switch to the target mode.
 * @retval kStatus_Success Switch to target mode successfully.
 */
status_t CLOCK_SetBlpiMode(void);

/*!
 * @brief Set MCG to BLPE mode.
 *
 * This function sets MCG to BLPE mode. If could not set to BLPE mode directly
 * from current mode, this function returns error.
 *
 * @retval kStatus_MCG_ModeUnreachable Could not switch to the target mode.
 * @retval kStatus_Success Switch to target mode successfully.
 */
status_t CLOCK_SetBlpeMode(void);

/*!
 * @brief Set MCG to PBE mode.
 *
 * This function sets MCG to PBE mode. If could not set to PBE mode directly
 * from current mode, this function returns error.
 *
 * @param   pllcs  The PLL selection, PLLCS.
 * @param   config Pointer to the PLL configuration.
 * @retval kStatus_MCG_ModeUnreachable Could not switch to the target mode.
 * @retval kStatus_Success Switch to target mode successfully.
 *
 * @note
 * 1. The parameter \c pllcs selects the PLL, for some platforms, there is
 * only one PLL, the parameter pllcs is kept for interface compatible.
 * 2. The parameter \c config is the PLL configuration structure, on some
 * platforms, could choose the external PLL directly. This means that the
 * configuration structure is not necessary, pass in NULL for this case.
 * For example: CLOCK_SetPbeMode(kMCG_OscselOsc, kMCG_PllClkSelExtPll, NULL);
 */
status_t CLOCK_SetPbeMode(mcg_pll_clk_select_t pllcs, mcg_pll_config_t const *config);

/*!
 * @brief Set MCG to PEE mode.
 *
 * This function sets MCG to PEE mode.
 *
 * @retval kStatus_MCG_ModeUnreachable Could not switch to the target mode.
 * @retval kStatus_Success Switch to target mode successfully.
 *
 * @note This function only change CLKS to use PLL/FLL output. If the
 *       PRDIV/VDIV are different from PBE mode, please setup these
 *       settings in PBE mode and wait for stable then switch to PEE mode.
 */
status_t CLOCK_SetPeeMode(void);

/*!
 * @brief Switch MCG to FBE mode quickly from external mode.
 *
 * This function changes MCG from external modes (PEE/PBE/BLPE/FEE) to FBE mode quickly.
 * It only changes to use external clock as the system clock souce and disable PLL, but does not
 * configure FLL settings. This is a lite function with small code size, it is useful
 * during mode switch. For example, to switch from PEE mode to FEI mode:
 *
 * @code
 * CLOCK_ExternalModeToFbeModeQuick();
 * CLOCK_SetFeiMode(...);
 * @endcode
 *
 * @retval kStatus_Success Change successfully.
 * @retval kStatus_MCG_ModeInvalid Current mode is not external modes, should not call this function.
 */
status_t CLOCK_ExternalModeToFbeModeQuick(void);

/*!
 * @brief Switch MCG to FBI mode quickly from internal modes.
 *
 * This function changes MCG from internal modes (PEI/PBI/BLPI/FEI) to FBI mode quickly.
 * It only changes to use MCGIRCLK as the system clock souce and disable PLL, but does not
 * configure FLL settings. This is a lite function with small code size, it is useful
 * during mode switch. For example, to switch from PEI mode to FEE mode:
 *
 * @code
 * CLOCK_InternalModeToFbiModeQuick();
 * CLOCK_SetFeeMode(...);
 * @endcode
 *
 * @retval kStatus_Success Change successfully.
 * @retval kStatus_MCG_ModeInvalid Current mode is not internal mode, should not call this function.
 */
status_t CLOCK_InternalModeToFbiModeQuick(void);

/*!
 * @brief Set MCG to FEI mode during system boot up.
 *
 * This function sets MCG to FEI mode from reset mode, it could be used to
 * set up MCG during system boot up.
 *
 * @param  dmx32  DMX32 in FEI mode.
 * @param  drs The DCO range selection.
 * @param  fllStableDelay Delay function to make sure FLL is stable.
 *
 * @retval kStatus_MCG_ModeUnreachable Could not switch to the target mode.
 * @retval kStatus_Success Switch to target mode successfully.
 * @note If @p dmx32 is set to kMCG_Dmx32Fine, the slow IRC must not be trimmed
 * to frequency above 32768Hz.
 */
status_t CLOCK_BootToFeiMode(mcg_dmx32_t dmx32, mcg_drs_t drs, void (*fllStableDelay)(void));

/*!
 * @brief Set MCG to FEE mode during system bootup.
 *
 * This function sets MCG to FEE mode from reset mode, it could be used to
 * set up MCG during system boot up.
 *
 * @param   oscsel OSC clock select, OSCSEL.
 * @param   frdiv  FLL reference clock divider setting, FRDIV.
 * @param   dmx32  DMX32 in FEE mode.
 * @param   drs    The DCO range selection.
 * @param   fllStableDelay Delay function to make sure FLL is stable.
 *
 * @retval kStatus_MCG_ModeUnreachable Could not switch to the target mode.
 * @retval kStatus_Success Switch to target mode successfully.
 */
status_t CLOCK_BootToFeeMode(
    mcg_oscsel_t oscsel, uint8_t frdiv, mcg_dmx32_t dmx32, mcg_drs_t drs, void (*fllStableDelay)(void));

/*!
 * @brief Set MCG to BLPI mode during system boot up.
 *
 * This function sets MCG to BLPI mode from reset mode, it could be used to
 * setup MCG during sytem boot up.
 *
 * @param  fcrdiv Fast IRC divider, FCRDIV.
 * @param  ircs   The internal reference clock to select, IRCS.
 * @param  ircEnableMode  The MCGIRCLK enable mode, OR'ed value of @ref _mcg_irclk_enable_mode.
 *
 * @retval kStatus_MCG_SourceUsed Could not change MCGIRCLK setting.
 * @retval kStatus_Success Switch to target mode successfully.
 */
status_t CLOCK_BootToBlpiMode(uint8_t fcrdiv, mcg_irc_mode_t ircs, uint8_t ircEnableMode);

/*!
 * @brief Set MCG to BLPE mode during sytem boot up.
 *
 * This function sets MCG to BLPE mode from reset mode, it could be used to
 * setup MCG during sytem boot up.
 *
 * @param  oscsel OSC clock select, MCG_C7[OSCSEL].
 *
 * @retval kStatus_MCG_ModeUnreachable Could not switch to the target mode.
 * @retval kStatus_Success Switch to target mode successfully.
 */
status_t CLOCK_BootToBlpeMode(mcg_oscsel_t oscsel);

/*!
 * @brief Set MCG to PEE mode during system boot up.
 *
 * This function sets MCG to PEE mode from reset mode, it could be used to
 * setup MCG during system boot up.
 *
 * @param   oscsel OSC clock select, MCG_C7[OSCSEL].
 * @param   pllcs  The PLL selection, PLLCS.
 * @param   config Pointer to the PLL configuration.
 *
 * @retval kStatus_MCG_ModeUnreachable Could not switch to the target mode.
 * @retval kStatus_Success Switch to target mode successfully.
 */
status_t CLOCK_BootToPeeMode(mcg_oscsel_t oscsel, mcg_pll_clk_select_t pllcs, mcg_pll_config_t const *config);

/*!
 * @brief Set MCG to some target mode.
 *
 * This function sets MCG to some target mode defined by the configure
 * structure, if cannot switch to target mode directly, this function will
 * choose the proper path.
 *
 * @param  config Pointer to the target MCG mode configuration structure.
 * @return Return kStatus_Success if switch successfully, otherwise return error code #_mcg_status.
 *
 * @note If external clock is used in the target mode, please make sure it is
 * enabled, for example, if the OSC0 is used, please setup OSC0 correctly before
 * this funciton.
 */
status_t CLOCK_SetMcgConfig(mcg_config_t const *config);

/*@}*/

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @} */

#endif /* _FSL_CLOCK_H_ */
