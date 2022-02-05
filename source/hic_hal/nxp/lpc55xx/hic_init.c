/**
 * @file    hic_init.c
 * @brief
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2020 Arm Limited, All Rights Reserved
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

#include "hic_init.h"
#include "daplink.h"
#include "fsl_clock.h"
#include "fsl_power.h"
#include "fsl_reset.h"
#include "fsl_usb.h"
#include "fsl_iocon.h"
#include "usb_phy.h"
#include "util.h"
#include "flash_hal.h"
#include "pin_mux.h"

#define FLASH_CMD_BLANK_CHECK (0x5)

/*******************************************************************************
 ******************* Configuration BOARD_BootClockFROHF96M *********************
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!Configuration
name: BOARD_BootClockFROHF96M
outputs:
- {id: System_clock.outFreq, value: 96 MHz}
settings:
- {id: ANALOG_CONTROL_FRO192M_CTRL_ENDI_FRO_96M_CFG, value: Enable}
- {id: SYSCON.MAINCLKSELA.sel, value: ANACTRL.fro_hf_clk}
sources:
- {id: ANACTRL.fro_hf.outFreq, value: 96 MHz}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/*******************************************************************************
 * Variables for BOARD_BootClockFROHF96M configuration
 ******************************************************************************/
/*******************************************************************************
 * Code for BOARD_BootClockFROHF96M configuration
 ******************************************************************************/
void BOARD_BootClockFROHF96M(void)
{
#ifndef SDK_SECONDARY_CORE
    /*!< Set up the clock sources */
    /*!< Configure FRO192M */
    POWER_DisablePD(kPDRUNCFG_PD_FRO192M); /*!< Ensure FRO is on  */
    CLOCK_SetupFROClocking(12000000U);     /*!< Set up FRO to the 12 MHz, just for sure */
    CLOCK_AttachClk(kFRO12M_to_MAIN_CLK);  /*!< Switch to FRO 12MHz first to ensure we can change the clock setting */

    CLOCK_SetupFROClocking(96000000U); /* Enable FRO HF(96MHz) output */

    POWER_SetVoltageForFreq(
        96000000U); /*!< Set voltage for the one of the fastest clock outputs: System clock output */
    CLOCK_SetFLASHAccessCyclesForFreq(96000000U); /*!< Set FLASH wait states for core */

    /*!< Set up dividers */
    CLOCK_SetClkDiv(kCLOCK_DivAhbClk, 1U, false); /*!< Set AHBCLKDIV divider to value 1 */

    /*!< Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kFRO_HF_to_MAIN_CLK); /*!< Switch MAIN_CLK to FRO_HF */

    /*< Set SystemCoreClock variable. */
    SystemCoreClock = BOARD_BOOTCLOCKFROHF96M_CORE_CLOCK;
#endif
}

/*******************************************************************************
 ******************** Configuration BOARD_BootClockPLL150M *********************
 ******************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!Configuration
name: BOARD_BootClockPLL150M
called_from_default_init: true
outputs:
- {id: System_clock.outFreq, value: 150 MHz}
settings:
- {id: PLL0_Mode, value: Normal}
- {id: ENABLE_CLKIN_ENA, value: Enabled}
- {id: ENABLE_SYSTEM_CLK_OUT, value: Enabled}
- {id: SYSCON.MAINCLKSELB.sel, value: SYSCON.PLL0_BYPASS}
- {id: SYSCON.PLL0CLKSEL.sel, value: SYSCON.CLK_IN_EN}
- {id: SYSCON.PLL0M_MULT.scale, value: '150', locked: true}
- {id: SYSCON.PLL0N_DIV.scale, value: '8', locked: true}
- {id: SYSCON.PLL0_PDEC.scale, value: '2', locked: true}
sources:
- {id: SYSCON.XTAL32M.outFreq, value: 16 MHz, enabled: true}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/*******************************************************************************
 * Variables for BOARD_BootClockPLL150M configuration
 ******************************************************************************/
/*******************************************************************************
 * Code for BOARD_BootClockPLL150M configuration
 ******************************************************************************/
void BOARD_BootClockPLL150M(void)
{
#ifndef SDK_SECONDARY_CORE
    /*!< Set up the clock sources */
    /*!< Configure FRO192M */
//     POWER_DisablePD(kPDRUNCFG_PD_FRO192M); /*!< Ensure FRO is on  */
//     CLOCK_SetupFROClocking(12000000U);     /*!< Set up FRO to the 12 MHz, just for sure */
//     CLOCK_AttachClk(kFRO12M_to_MAIN_CLK);  /*!< Switch to FRO 12MHz first to ensure we can change the clock setting */

    /*!< Configure XTAL32M */
    POWER_DisablePD(kPDRUNCFG_PD_XTAL32M);                                /* Ensure XTAL32M is powered */
    POWER_DisablePD(kPDRUNCFG_PD_LDOXO32M);                               /* Ensure XTAL32M is powered */
    CLOCK_SetupExtClocking(16000000U);                                    /* Enable clk_in clock */
    SYSCON->CLOCK_CTRL |= SYSCON_CLOCK_CTRL_CLKIN_ENA_MASK;               /* Enable clk_in from XTAL32M clock  */
    ANACTRL->XO32M_CTRL |= ANACTRL_XO32M_CTRL_ENABLE_SYSTEM_CLK_OUT_MASK; /* Enable clk_in to system  */

    POWER_SetVoltageForFreq(
        150000000U); /*!< Set voltage for the one of the fastest clock outputs: System clock output */
    CLOCK_SetFLASHAccessCyclesForFreq(150000000U); /*!< Set FLASH wait states for core */

    /*!< Set up PLL */
    CLOCK_AttachClk(kEXT_CLK_to_PLL0);  /*!< Switch PLL0CLKSEL to EXT_CLK */
    POWER_DisablePD(kPDRUNCFG_PD_PLL0); /* Ensure PLL is on  */
    POWER_DisablePD(kPDRUNCFG_PD_PLL0_SSCG);
    const pll_setup_t pll0Setup = {
        .pllctrl = SYSCON_PLL0CTRL_CLKEN_MASK | SYSCON_PLL0CTRL_SELI(53U) | SYSCON_PLL0CTRL_SELP(31U),
        .pllndec = SYSCON_PLL0NDEC_NDIV(8U),
        .pllpdec = SYSCON_PLL0PDEC_PDIV(1U),
        .pllsscg = {0x0U, (SYSCON_PLL0SSCG1_MDIV_EXT(150U) | SYSCON_PLL0SSCG1_SEL_EXT_MASK)},
        .pllRate = 150000000U,
        .flags   = PLL_SETUPFLAG_WAITLOCK};
    CLOCK_SetPLL0Freq(&pll0Setup); /*!< Configure PLL0 to the desired values */

    /*!< Set up dividers */
    CLOCK_SetClkDiv(kCLOCK_DivAhbClk, 1U, false); /*!< Set AHBCLKDIV divider to value 1 */

    /*!< Set up clock selectors - Attach clocks to the peripheries */
    CLOCK_AttachClk(kPLL0_to_MAIN_CLK); /*!< Switch MAIN_CLK to PLL0 */

    /*< Set SystemCoreClock variable. */
    SystemCoreClock = BOARD_BOOTCLOCKPLL150M_CORE_CLOCK;
#endif
}

//! Setup clocks to run from the FRO HF at 96 MHz.
void sdk_init(void)
{
    BOARD_BootClockFROHF96M();

    hic_crc_init();
}

//! - Configure the VBUS pin.
//! - Switch USB1 to device mode.
//! - Turn on 16MHz crystal oscillator.
//! - Switch main clock to PLL0 at 150 MHz.
//! - Ungate USBPHY and USBHS.
//! - Configure the USB PHY and USB1 clocks.
void hic_enable_usb_clocks(void)
{
    // Switching to 150 MHz for interface is disabled because it prevents
    // interface from writing configuration and updating bootloader.
    // #if defined(DAPLINK_IF)
    //     // For the interface, switch to 150 MHz before enabling USB.
    //     // The bootloader will stay at 96 MHz so it can always write
    //     // internal flash.
    //     BOARD_BootClockPLL150M();
    // #endif

    NVIC_ClearPendingIRQ(USB1_IRQn);
    NVIC_ClearPendingIRQ(USB1_NEEDCLK_IRQn);

    /* reset the IP to make sure it's in reset state. */
    RESET_PeripheralReset(kUSB1H_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1D_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kUSB1RAM_RST_SHIFT_RSTn);

    // Enable VBUS pin.
    init_vbus_pin();

    // Switch IP to device mode. First enable the USB1 host clock.
    CLOCK_EnableClock(kCLOCK_Usbh1);
    // Put PHY powerdown under software control
    USBHSH->PORTMODE = USBHSH_PORTMODE_SW_PDCOM_MASK;
    // According to reference mannual, device mode setting has to be set by access usb host register
    USBHSH->PORTMODE |= USBHSH_PORTMODE_DEV_ENABLE_MASK;
    // Disable usb1 host clock
    CLOCK_DisableClock(kCLOCK_Usbh1);

    // Enable clocks.
    CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_UsbPhySrcExt, BOARD_XTAL0_CLK_HZ);
    CLOCK_EnableUsbhs0DeviceClock(kCLOCK_UsbSrcUnused, 0U);

    // Init PHY.
    USB_EhciPhyInit(kUSB_ControllerLpcIp3511Hs0, BOARD_XTAL0_CLK_HZ, NULL);
}

void hic_power_target(void)
{
    // Keep powered off in bootloader mode
    // to prevent the target from effecting the state
    // of the reset line / reset button
    if (!daplink_is_bootloader()) {
        // Nothing to do for MCU-Link schematic.
    }
}

// Override the default weak implementation.
bool flash_is_readable(uint32_t addr, uint32_t length)
{
    // Make sure the core clock is less than 100 MHz, or flash commands won't work.
    util_assert(SystemCoreClock < 100000000);

    // Return true if the address is within internal flash and the flash sector is not erased.
    if (!(addr >= DAPLINK_ROM_START && addr < (DAPLINK_ROM_START + DAPLINK_ROM_SIZE))) {
        return false;
    }

    // Use the blank check command directly. The address is right-shifted to convert to
    // a flash word (16 bytes) address.
    FLASH->STARTA = addr >> 4;
    FLASH->STOPA = (addr + length - 1) >> 4;
    FLASH->INT_CLR_STATUS = FLASH_INT_CLR_STATUS_FAIL_MASK
                            | FLASH_INT_CLR_STATUS_ERR_MASK
                            | FLASH_INT_CLR_STATUS_DONE_MASK
                            | FLASH_INT_CLR_STATUS_ECC_ERR_MASK;
    FLASH->CMD = FLASH_CMD_BLANK_CHECK;

    // Wait until command is complete.
    while (((FLASH->INT_STATUS) & FLASH_INT_STATUS_DONE_MASK) == 0) {
    }

    // Return true (is readable) if the blank check failed, meaning the page is programmed.
    // Return false (not readable) for erased page or failure.
    return ((FLASH->INT_STATUS & (FLASH_INT_STATUS_FAIL_MASK
                                | FLASH_INT_STATUS_ERR_MASK
                                | FLASH_INT_STATUS_ECC_ERR_MASK)) == FLASH_INT_STATUS_FAIL_MASK);
}
