/*
** ###################################################################
**     Version:             rev. 2.7, 2015-06-08
**     Build:               b151210
**
**     Abstract:
**         Chip specific module features.
**
**     Copyright (c) 2015 Freescale Semiconductor, Inc.
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
**     - rev. 2.2 (2014-01-30)
**         Added single maximum value generation and a constrain to varying feature values that only numbers can have maximum.
**     - rev. 2.3 (2015-01-21)
**         Added FSL_FEATURE_SOC_peripheral_COUNT with number of peripheral instances
**     - rev. 2.4 (2015-05-19)
**         FSL_FEATURE_SOC_CAU_COUNT remamed to FSL_FEATURE_SOC_MMCAU_COUNT.
**         Added FSL_FEATURE_SOC_peripheral_COUNT for TRNG and HSADC.
**         Added features for PDB and PORT.
**     - rev. 2.5 (2015-05-25)
**         Added FSL_FEATURE_FLASH_PFLASH_START_ADDRESS
**     - rev. 2.6 (2015-05-27)
**         Several USB features added.
**     - rev. 2.7 (2015-06-08)
**         FTM features BUS_CLOCK and FAST_CLOCK removed.
**
** ###################################################################
*/

#ifndef _MK20D5_FEATURES_H_
#define _MK20D5_FEATURES_H_

/* SOC module features */

#if defined(CPU_MK20DX128VFM5) || defined(CPU_MK20DN128VFM5) || defined(CPU_MK20DX64VFM5) || defined(CPU_MK20DN64VFM5) || \
    defined(CPU_MK20DX32VFM5) || defined(CPU_MK20DN32VFM5)
    /* @brief ACMP availability on the SoC. */
    #define FSL_FEATURE_SOC_ACMP_COUNT (0)
    /* @brief ADC16 availability on the SoC. */
    #define FSL_FEATURE_SOC_ADC16_COUNT (1)
    /* @brief ADC12 availability on the SoC. */
    #define FSL_FEATURE_SOC_ADC12_COUNT (0)
    /* @brief AFE availability on the SoC. */
    #define FSL_FEATURE_SOC_AFE_COUNT (0)
    /* @brief AIPS availability on the SoC. */
    #define FSL_FEATURE_SOC_AIPS_COUNT (0)
    /* @brief AOI availability on the SoC. */
    #define FSL_FEATURE_SOC_AOI_COUNT (0)
    /* @brief AXBS availability on the SoC. */
    #define FSL_FEATURE_SOC_AXBS_COUNT (0)
    /* @brief ASMC availability on the SoC. */
    #define FSL_FEATURE_SOC_ASMC_COUNT (0)
    /* @brief CADC availability on the SoC. */
    #define FSL_FEATURE_SOC_CADC_COUNT (0)
    /* @brief FLEXCAN availability on the SoC. */
    #define FSL_FEATURE_SOC_FLEXCAN_COUNT (0)
    /* @brief MMCAU availability on the SoC. */
    #define FSL_FEATURE_SOC_MMCAU_COUNT (0)
    /* @brief CMP availability on the SoC. */
    #define FSL_FEATURE_SOC_CMP_COUNT (2)
    /* @brief CMT availability on the SoC. */
    #define FSL_FEATURE_SOC_CMT_COUNT (1)
    /* @brief CNC availability on the SoC. */
    #define FSL_FEATURE_SOC_CNC_COUNT (0)
    /* @brief CRC availability on the SoC. */
    #define FSL_FEATURE_SOC_CRC_COUNT (1)
    /* @brief DAC availability on the SoC. */
    #define FSL_FEATURE_SOC_DAC_COUNT (0)
    /* @brief DAC32 availability on the SoC. */
    #define FSL_FEATURE_SOC_DAC32_COUNT (0)
    /* @brief DCDC availability on the SoC. */
    #define FSL_FEATURE_SOC_DCDC_COUNT (0)
    /* @brief DDR availability on the SoC. */
    #define FSL_FEATURE_SOC_DDR_COUNT (0)
    /* @brief DMA availability on the SoC. */
    #define FSL_FEATURE_SOC_DMA_COUNT (0)
    /* @brief DMAMUX availability on the SoC. */
    #define FSL_FEATURE_SOC_DMAMUX_COUNT (1)
    /* @brief DRY availability on the SoC. */
    #define FSL_FEATURE_SOC_DRY_COUNT (0)
    /* @brief DSPI availability on the SoC. */
    #define FSL_FEATURE_SOC_DSPI_COUNT (1)
    /* @brief EDMA availability on the SoC. */
    #define FSL_FEATURE_SOC_EDMA_COUNT (1)
    /* @brief EMVSIM availability on the SoC. */
    #define FSL_FEATURE_SOC_EMVSIM_COUNT (0)
    /* @brief ENC availability on the SoC. */
    #define FSL_FEATURE_SOC_ENC_COUNT (0)
    /* @brief ENET availability on the SoC. */
    #define FSL_FEATURE_SOC_ENET_COUNT (0)
    /* @brief EWM availability on the SoC. */
    #define FSL_FEATURE_SOC_EWM_COUNT (1)
    /* @brief FB availability on the SoC. */
    #define FSL_FEATURE_SOC_FB_COUNT (0)
    /* @brief FGPIO availability on the SoC. */
    #define FSL_FEATURE_SOC_FGPIO_COUNT (0)
    /* @brief FLEXIO availability on the SoC. */
    #define FSL_FEATURE_SOC_FLEXIO_COUNT (0)
    /* @brief FMC availability on the SoC. */
    #define FSL_FEATURE_SOC_FMC_COUNT (1)
    /* @brief FSKDT availability on the SoC. */
    #define FSL_FEATURE_SOC_FSKDT_COUNT (0)
    /* @brief FTFA availability on the SoC. */
    #define FSL_FEATURE_SOC_FTFA_COUNT (0)
    /* @brief FTFE availability on the SoC. */
    #define FSL_FEATURE_SOC_FTFE_COUNT (0)
    /* @brief FTFL availability on the SoC. */
    #define FSL_FEATURE_SOC_FTFL_COUNT (1)
    /* @brief FTM availability on the SoC. */
    #define FSL_FEATURE_SOC_FTM_COUNT (2)
    /* @brief FTMRA availability on the SoC. */
    #define FSL_FEATURE_SOC_FTMRA_COUNT (0)
    /* @brief FTMRE availability on the SoC. */
    #define FSL_FEATURE_SOC_FTMRE_COUNT (0)
    /* @brief FTMRH availability on the SoC. */
    #define FSL_FEATURE_SOC_FTMRH_COUNT (0)
    /* @brief GPIO availability on the SoC. */
    #define FSL_FEATURE_SOC_GPIO_COUNT (5)
    /* @brief HSADC availability on the SoC. */
    #define FSL_FEATURE_SOC_HSADC_COUNT (0)
    /* @brief I2C availability on the SoC. */
    #define FSL_FEATURE_SOC_I2C_COUNT (1)
    /* @brief I2S availability on the SoC. */
    #define FSL_FEATURE_SOC_I2S_COUNT (1)
    /* @brief ICS availability on the SoC. */
    #define FSL_FEATURE_SOC_ICS_COUNT (0)
    /* @brief IRQ availability on the SoC. */
    #define FSL_FEATURE_SOC_IRQ_COUNT (0)
    /* @brief INTMUX availability on the SoC. */
    #define FSL_FEATURE_SOC_INTMUX_COUNT (0)
    /* @brief KBI availability on the SoC. */
    #define FSL_FEATURE_SOC_KBI_COUNT (0)
    /* @brief SLCD availability on the SoC. */
    #define FSL_FEATURE_SOC_SLCD_COUNT (0)
    /* @brief LCDC availability on the SoC. */
    #define FSL_FEATURE_SOC_LCDC_COUNT (0)
    /* @brief LDO availability on the SoC. */
    #define FSL_FEATURE_SOC_LDO_COUNT (0)
    /* @brief LLWU availability on the SoC. */
    #define FSL_FEATURE_SOC_LLWU_COUNT (1)
    /* @brief LMEM availability on the SoC. */
    #define FSL_FEATURE_SOC_LMEM_COUNT (0)
    /* @brief LPSCI availability on the SoC. */
    #define FSL_FEATURE_SOC_LPSCI_COUNT (0)
    /* @brief LPTMR availability on the SoC. */
    #define FSL_FEATURE_SOC_LPTMR_COUNT (1)
    /* @brief LPTPM availability on the SoC. */
    #define FSL_FEATURE_SOC_LPTPM_COUNT (0)
    /* @brief LPUART availability on the SoC. */
    #define FSL_FEATURE_SOC_LPUART_COUNT (0)
    /* @brief LTC availability on the SoC. */
    #define FSL_FEATURE_SOC_LTC_COUNT (0)
    /* @brief LPI2C availability on the SoC. */
    #define FSL_FEATURE_SOC_LPI2C_COUNT (0)
    /* @brief LPSPI availability on the SoC. */
    #define FSL_FEATURE_SOC_LPSPI_COUNT (0)
    /* @brief LPIT availability on the SoC. */
    #define FSL_FEATURE_SOC_LPIT_COUNT (0)
    /* @brief MC availability on the SoC. */
    #define FSL_FEATURE_SOC_MC_COUNT (0)
    /* @brief MCG availability on the SoC. */
    #define FSL_FEATURE_SOC_MCG_COUNT (1)
    /* @brief MCGLITE availability on the SoC. */
    #define FSL_FEATURE_SOC_MCGLITE_COUNT (0)
    /* @brief MCM availability on the SoC. */
    #define FSL_FEATURE_SOC_MCM_COUNT (0)
    /* @brief MMAU availability on the SoC. */
    #define FSL_FEATURE_SOC_MMAU_COUNT (0)
    /* @brief MMDVSQ availability on the SoC. */
    #define FSL_FEATURE_SOC_MMDVSQ_COUNT (0)
    /* @brief MPU availability on the SoC. */
    #define FSL_FEATURE_SOC_MPU_COUNT (0)
    /* @brief MSCAN availability on the SoC. */
    #define FSL_FEATURE_SOC_MSCAN_COUNT (0)
    /* @brief MSCM availability on the SoC. */
    #define FSL_FEATURE_SOC_MSCM_COUNT (0)
    /* @brief MTB availability on the SoC. */
    #define FSL_FEATURE_SOC_MTB_COUNT (0)
    /* @brief MTBDWT availability on the SoC. */
    #define FSL_FEATURE_SOC_MTBDWT_COUNT (0)
    /* @brief MU availability on the SoC. */
    #define FSL_FEATURE_SOC_MU_COUNT (0)
    /* @brief NFC availability on the SoC. */
    #define FSL_FEATURE_SOC_NFC_COUNT (0)
    /* @brief OPAMP availability on the SoC. */
    #define FSL_FEATURE_SOC_OPAMP_COUNT (0)
    /* @brief OSC availability on the SoC. */
    #define FSL_FEATURE_SOC_OSC_COUNT (1)
    /* @brief OSC32 availability on the SoC. */
    #define FSL_FEATURE_SOC_OSC32_COUNT (0)
    /* @brief OTFAD availability on the SoC. */
    #define FSL_FEATURE_SOC_OTFAD_COUNT (0)
    /* @brief PDB availability on the SoC. */
    #define FSL_FEATURE_SOC_PDB_COUNT (1)
    /* @brief PGA availability on the SoC. */
    #define FSL_FEATURE_SOC_PGA_COUNT (0)
    /* @brief PIT availability on the SoC. */
    #define FSL_FEATURE_SOC_PIT_COUNT (1)
    /* @brief PMC availability on the SoC. */
    #define FSL_FEATURE_SOC_PMC_COUNT (1)
    /* @brief PORT availability on the SoC. */
    #define FSL_FEATURE_SOC_PORT_COUNT (5)
    /* @brief PWM availability on the SoC. */
    #define FSL_FEATURE_SOC_PWM_COUNT (0)
    /* @brief PWT availability on the SoC. */
    #define FSL_FEATURE_SOC_PWT_COUNT (0)
    /* @brief PCC availability on the SoC. */
    #define FSL_FEATURE_SOC_PCC_COUNT (0)
    /* @brief QuadSPI availability on the SoC. */
    #define FSL_FEATURE_SOC_QuadSPI_COUNT (0)
    /* @brief RCM availability on the SoC. */
    #define FSL_FEATURE_SOC_RCM_COUNT (1)
    /* @brief RFSYS availability on the SoC. */
    #define FSL_FEATURE_SOC_RFSYS_COUNT (1)
    /* @brief RFVBAT availability on the SoC. */
    #define FSL_FEATURE_SOC_RFVBAT_COUNT (1)
    /* @brief RNG availability on the SoC. */
    #define FSL_FEATURE_SOC_RNG_COUNT (0)
    /* @brief RNGB availability on the SoC. */
    #define FSL_FEATURE_SOC_RNGB_COUNT (0)
    /* @brief ROM availability on the SoC. */
    #define FSL_FEATURE_SOC_ROM_COUNT (0)
    /* @brief RSIM availability on the SoC. */
    #define FSL_FEATURE_SOC_RSIM_COUNT (0)
    /* @brief RTC availability on the SoC. */
    #define FSL_FEATURE_SOC_RTC_COUNT (1)
    /* @brief SCI availability on the SoC. */
    #define FSL_FEATURE_SOC_SCI_COUNT (0)
    /* @brief SDHC availability on the SoC. */
    #define FSL_FEATURE_SOC_SDHC_COUNT (0)
    /* @brief SDRAM availability on the SoC. */
    #define FSL_FEATURE_SOC_SDRAM_COUNT (0)
    /* @brief SIM availability on the SoC. */
    #define FSL_FEATURE_SOC_SIM_COUNT (1)
    /* @brief SMC availability on the SoC. */
    #define FSL_FEATURE_SOC_SMC_COUNT (1)
    /* @brief SPI availability on the SoC. */
    #define FSL_FEATURE_SOC_SPI_COUNT (0)
    /* @brief SCG availability on the SoC. */
    #define FSL_FEATURE_SOC_SCG_COUNT (0)
    /* @brief SEMA42 availability on the SoC. */
    #define FSL_FEATURE_SOC_SEMA42_COUNT (0)
    /* @brief TMR availability on the SoC. */
    #define FSL_FEATURE_SOC_TMR_COUNT (0)
    /* @brief TPM availability on the SoC. */
    #define FSL_FEATURE_SOC_TPM_COUNT (0)
    /* @brief TRIAMP availability on the SoC. */
    #define FSL_FEATURE_SOC_TRIAMP_COUNT (0)
    /* @brief TRNG availability on the SoC. */
    #define FSL_FEATURE_SOC_TRNG_COUNT (0)
    /* @brief TSI availability on the SoC. */
    #define FSL_FEATURE_SOC_TSI_COUNT (1)
    /* @brief TRGMUX availability on the SoC. */
    #define FSL_FEATURE_SOC_TRGMUX_COUNT (0)
    /* @brief TSTMR availability on the SoC. */
    #define FSL_FEATURE_SOC_TSTMR_COUNT (0)
    /* @brief UART availability on the SoC. */
    #define FSL_FEATURE_SOC_UART_COUNT (3)
    /* @brief USB availability on the SoC. */
    #define FSL_FEATURE_SOC_USB_COUNT (1)
    /* @brief USBDCD availability on the SoC. */
    #define FSL_FEATURE_SOC_USBDCD_COUNT (1)
    /* @brief USBHSDCD availability on the SoC. */
    #define FSL_FEATURE_SOC_USBHSDCD_COUNT (0)
    /* @brief USBPHY availability on the SoC. */
    #define FSL_FEATURE_SOC_USBPHY_COUNT (0)
    /* @brief VREF availability on the SoC. */
    #define FSL_FEATURE_SOC_VREF_COUNT (0)
    /* @brief WDOG availability on the SoC. */
    #define FSL_FEATURE_SOC_WDOG_COUNT (1)
    /* @brief XBAR availability on the SoC. */
    #define FSL_FEATURE_SOC_XBAR_COUNT (0)
    /* @brief XBARA availability on the SoC. */
    #define FSL_FEATURE_SOC_XBARA_COUNT (0)
    /* @brief XBARB availability on the SoC. */
    #define FSL_FEATURE_SOC_XBARB_COUNT (0)
    /* @brief XCVR availability on the SoC. */
    #define FSL_FEATURE_SOC_XCVR_COUNT (0)
    /* @brief XRDC availability on the SoC. */
    #define FSL_FEATURE_SOC_XRDC_COUNT (0)
    /* @brief ZLL availability on the SoC. */
    #define FSL_FEATURE_SOC_ZLL_COUNT (0)
#elif defined(CPU_MK20DX128VMP5) || defined(CPU_MK20DN128VMP5) || defined(CPU_MK20DX64VMP5) || defined(CPU_MK20DN64VMP5) || \
    defined(CPU_MK20DX32VMP5) || defined(CPU_MK20DN32VMP5) || defined(CPU_MK20DX128VLH5) || defined(CPU_MK20DN128VLH5) || \
    defined(CPU_MK20DX64VLH5) || defined(CPU_MK20DN64VLH5) || defined(CPU_MK20DX32VLH5) || defined(CPU_MK20DN32VLH5) || \
    defined(CPU_MK20DX128VFT5) || defined(CPU_MK20DN128VFT5) || defined(CPU_MK20DX64VFT5) || defined(CPU_MK20DN64VFT5) || \
    defined(CPU_MK20DX32VFT5) || defined(CPU_MK20DN32VFT5) || defined(CPU_MK20DX128VLF5) || defined(CPU_MK20DN128VLF5) || \
    defined(CPU_MK20DX64VLF5) || defined(CPU_MK20DN64VLF5) || defined(CPU_MK20DX32VLF5) || defined(CPU_MK20DN32VLF5)
    /* @brief ACMP availability on the SoC. */
    #define FSL_FEATURE_SOC_ACMP_COUNT (0)
    /* @brief ADC16 availability on the SoC. */
    #define FSL_FEATURE_SOC_ADC16_COUNT (1)
    /* @brief ADC12 availability on the SoC. */
    #define FSL_FEATURE_SOC_ADC12_COUNT (0)
    /* @brief AFE availability on the SoC. */
    #define FSL_FEATURE_SOC_AFE_COUNT (0)
    /* @brief AIPS availability on the SoC. */
    #define FSL_FEATURE_SOC_AIPS_COUNT (0)
    /* @brief AOI availability on the SoC. */
    #define FSL_FEATURE_SOC_AOI_COUNT (0)
    /* @brief AXBS availability on the SoC. */
    #define FSL_FEATURE_SOC_AXBS_COUNT (0)
    /* @brief ASMC availability on the SoC. */
    #define FSL_FEATURE_SOC_ASMC_COUNT (0)
    /* @brief CADC availability on the SoC. */
    #define FSL_FEATURE_SOC_CADC_COUNT (0)
    /* @brief FLEXCAN availability on the SoC. */
    #define FSL_FEATURE_SOC_FLEXCAN_COUNT (0)
    /* @brief MMCAU availability on the SoC. */
    #define FSL_FEATURE_SOC_MMCAU_COUNT (0)
    /* @brief CMP availability on the SoC. */
    #define FSL_FEATURE_SOC_CMP_COUNT (2)
    /* @brief CMT availability on the SoC. */
    #define FSL_FEATURE_SOC_CMT_COUNT (1)
    /* @brief CNC availability on the SoC. */
    #define FSL_FEATURE_SOC_CNC_COUNT (0)
    /* @brief CRC availability on the SoC. */
    #define FSL_FEATURE_SOC_CRC_COUNT (1)
    /* @brief DAC availability on the SoC. */
    #define FSL_FEATURE_SOC_DAC_COUNT (0)
    /* @brief DAC32 availability on the SoC. */
    #define FSL_FEATURE_SOC_DAC32_COUNT (0)
    /* @brief DCDC availability on the SoC. */
    #define FSL_FEATURE_SOC_DCDC_COUNT (0)
    /* @brief DDR availability on the SoC. */
    #define FSL_FEATURE_SOC_DDR_COUNT (0)
    /* @brief DMA availability on the SoC. */
    #define FSL_FEATURE_SOC_DMA_COUNT (0)
    /* @brief DMAMUX availability on the SoC. */
    #define FSL_FEATURE_SOC_DMAMUX_COUNT (1)
    /* @brief DRY availability on the SoC. */
    #define FSL_FEATURE_SOC_DRY_COUNT (0)
    /* @brief DSPI availability on the SoC. */
    #define FSL_FEATURE_SOC_DSPI_COUNT (1)
    /* @brief EDMA availability on the SoC. */
    #define FSL_FEATURE_SOC_EDMA_COUNT (1)
    /* @brief EMVSIM availability on the SoC. */
    #define FSL_FEATURE_SOC_EMVSIM_COUNT (0)
    /* @brief ENC availability on the SoC. */
    #define FSL_FEATURE_SOC_ENC_COUNT (0)
    /* @brief ENET availability on the SoC. */
    #define FSL_FEATURE_SOC_ENET_COUNT (0)
    /* @brief EWM availability on the SoC. */
    #define FSL_FEATURE_SOC_EWM_COUNT (1)
    /* @brief FB availability on the SoC. */
    #define FSL_FEATURE_SOC_FB_COUNT (0)
    /* @brief FGPIO availability on the SoC. */
    #define FSL_FEATURE_SOC_FGPIO_COUNT (0)
    /* @brief FLEXIO availability on the SoC. */
    #define FSL_FEATURE_SOC_FLEXIO_COUNT (0)
    /* @brief FMC availability on the SoC. */
    #define FSL_FEATURE_SOC_FMC_COUNT (1)
    /* @brief FSKDT availability on the SoC. */
    #define FSL_FEATURE_SOC_FSKDT_COUNT (0)
    /* @brief FTFA availability on the SoC. */
    #define FSL_FEATURE_SOC_FTFA_COUNT (0)
    /* @brief FTFE availability on the SoC. */
    #define FSL_FEATURE_SOC_FTFE_COUNT (0)
    /* @brief FTFL availability on the SoC. */
    #define FSL_FEATURE_SOC_FTFL_COUNT (1)
    /* @brief FTM availability on the SoC. */
    #define FSL_FEATURE_SOC_FTM_COUNT (2)
    /* @brief FTMRA availability on the SoC. */
    #define FSL_FEATURE_SOC_FTMRA_COUNT (0)
    /* @brief FTMRE availability on the SoC. */
    #define FSL_FEATURE_SOC_FTMRE_COUNT (0)
    /* @brief FTMRH availability on the SoC. */
    #define FSL_FEATURE_SOC_FTMRH_COUNT (0)
    /* @brief GPIO availability on the SoC. */
    #define FSL_FEATURE_SOC_GPIO_COUNT (5)
    /* @brief HSADC availability on the SoC. */
    #define FSL_FEATURE_SOC_HSADC_COUNT (0)
    /* @brief I2C availability on the SoC. */
    #define FSL_FEATURE_SOC_I2C_COUNT (1)
    /* @brief I2S availability on the SoC. */
    #define FSL_FEATURE_SOC_I2S_COUNT (1)
    /* @brief ICS availability on the SoC. */
    #define FSL_FEATURE_SOC_ICS_COUNT (0)
    /* @brief IRQ availability on the SoC. */
    #define FSL_FEATURE_SOC_IRQ_COUNT (0)
    /* @brief INTMUX availability on the SoC. */
    #define FSL_FEATURE_SOC_INTMUX_COUNT (0)
    /* @brief KBI availability on the SoC. */
    #define FSL_FEATURE_SOC_KBI_COUNT (0)
    /* @brief SLCD availability on the SoC. */
    #define FSL_FEATURE_SOC_SLCD_COUNT (0)
    /* @brief LCDC availability on the SoC. */
    #define FSL_FEATURE_SOC_LCDC_COUNT (0)
    /* @brief LDO availability on the SoC. */
    #define FSL_FEATURE_SOC_LDO_COUNT (0)
    /* @brief LLWU availability on the SoC. */
    #define FSL_FEATURE_SOC_LLWU_COUNT (1)
    /* @brief LMEM availability on the SoC. */
    #define FSL_FEATURE_SOC_LMEM_COUNT (0)
    /* @brief LPSCI availability on the SoC. */
    #define FSL_FEATURE_SOC_LPSCI_COUNT (0)
    /* @brief LPTMR availability on the SoC. */
    #define FSL_FEATURE_SOC_LPTMR_COUNT (1)
    /* @brief LPTPM availability on the SoC. */
    #define FSL_FEATURE_SOC_LPTPM_COUNT (0)
    /* @brief LPUART availability on the SoC. */
    #define FSL_FEATURE_SOC_LPUART_COUNT (0)
    /* @brief LTC availability on the SoC. */
    #define FSL_FEATURE_SOC_LTC_COUNT (0)
    /* @brief LPI2C availability on the SoC. */
    #define FSL_FEATURE_SOC_LPI2C_COUNT (0)
    /* @brief LPSPI availability on the SoC. */
    #define FSL_FEATURE_SOC_LPSPI_COUNT (0)
    /* @brief LPIT availability on the SoC. */
    #define FSL_FEATURE_SOC_LPIT_COUNT (0)
    /* @brief MC availability on the SoC. */
    #define FSL_FEATURE_SOC_MC_COUNT (0)
    /* @brief MCG availability on the SoC. */
    #define FSL_FEATURE_SOC_MCG_COUNT (1)
    /* @brief MCGLITE availability on the SoC. */
    #define FSL_FEATURE_SOC_MCGLITE_COUNT (0)
    /* @brief MCM availability on the SoC. */
    #define FSL_FEATURE_SOC_MCM_COUNT (0)
    /* @brief MMAU availability on the SoC. */
    #define FSL_FEATURE_SOC_MMAU_COUNT (0)
    /* @brief MMDVSQ availability on the SoC. */
    #define FSL_FEATURE_SOC_MMDVSQ_COUNT (0)
    /* @brief MPU availability on the SoC. */
    #define FSL_FEATURE_SOC_MPU_COUNT (0)
    /* @brief MSCAN availability on the SoC. */
    #define FSL_FEATURE_SOC_MSCAN_COUNT (0)
    /* @brief MSCM availability on the SoC. */
    #define FSL_FEATURE_SOC_MSCM_COUNT (0)
    /* @brief MTB availability on the SoC. */
    #define FSL_FEATURE_SOC_MTB_COUNT (0)
    /* @brief MTBDWT availability on the SoC. */
    #define FSL_FEATURE_SOC_MTBDWT_COUNT (0)
    /* @brief MU availability on the SoC. */
    #define FSL_FEATURE_SOC_MU_COUNT (0)
    /* @brief NFC availability on the SoC. */
    #define FSL_FEATURE_SOC_NFC_COUNT (0)
    /* @brief OPAMP availability on the SoC. */
    #define FSL_FEATURE_SOC_OPAMP_COUNT (0)
    /* @brief OSC availability on the SoC. */
    #define FSL_FEATURE_SOC_OSC_COUNT (1)
    /* @brief OSC32 availability on the SoC. */
    #define FSL_FEATURE_SOC_OSC32_COUNT (0)
    /* @brief OTFAD availability on the SoC. */
    #define FSL_FEATURE_SOC_OTFAD_COUNT (0)
    /* @brief PDB availability on the SoC. */
    #define FSL_FEATURE_SOC_PDB_COUNT (1)
    /* @brief PGA availability on the SoC. */
    #define FSL_FEATURE_SOC_PGA_COUNT (0)
    /* @brief PIT availability on the SoC. */
    #define FSL_FEATURE_SOC_PIT_COUNT (1)
    /* @brief PMC availability on the SoC. */
    #define FSL_FEATURE_SOC_PMC_COUNT (1)
    /* @brief PORT availability on the SoC. */
    #define FSL_FEATURE_SOC_PORT_COUNT (5)
    /* @brief PWM availability on the SoC. */
    #define FSL_FEATURE_SOC_PWM_COUNT (0)
    /* @brief PWT availability on the SoC. */
    #define FSL_FEATURE_SOC_PWT_COUNT (0)
    /* @brief PCC availability on the SoC. */
    #define FSL_FEATURE_SOC_PCC_COUNT (0)
    /* @brief QuadSPI availability on the SoC. */
    #define FSL_FEATURE_SOC_QuadSPI_COUNT (0)
    /* @brief RCM availability on the SoC. */
    #define FSL_FEATURE_SOC_RCM_COUNT (1)
    /* @brief RFSYS availability on the SoC. */
    #define FSL_FEATURE_SOC_RFSYS_COUNT (1)
    /* @brief RFVBAT availability on the SoC. */
    #define FSL_FEATURE_SOC_RFVBAT_COUNT (1)
    /* @brief RNG availability on the SoC. */
    #define FSL_FEATURE_SOC_RNG_COUNT (0)
    /* @brief RNGB availability on the SoC. */
    #define FSL_FEATURE_SOC_RNGB_COUNT (0)
    /* @brief ROM availability on the SoC. */
    #define FSL_FEATURE_SOC_ROM_COUNT (0)
    /* @brief RSIM availability on the SoC. */
    #define FSL_FEATURE_SOC_RSIM_COUNT (0)
    /* @brief RTC availability on the SoC. */
    #define FSL_FEATURE_SOC_RTC_COUNT (1)
    /* @brief SCI availability on the SoC. */
    #define FSL_FEATURE_SOC_SCI_COUNT (0)
    /* @brief SDHC availability on the SoC. */
    #define FSL_FEATURE_SOC_SDHC_COUNT (0)
    /* @brief SDRAM availability on the SoC. */
    #define FSL_FEATURE_SOC_SDRAM_COUNT (0)
    /* @brief SIM availability on the SoC. */
    #define FSL_FEATURE_SOC_SIM_COUNT (1)
    /* @brief SMC availability on the SoC. */
    #define FSL_FEATURE_SOC_SMC_COUNT (1)
    /* @brief SPI availability on the SoC. */
    #define FSL_FEATURE_SOC_SPI_COUNT (0)
    /* @brief SCG availability on the SoC. */
    #define FSL_FEATURE_SOC_SCG_COUNT (0)
    /* @brief SEMA42 availability on the SoC. */
    #define FSL_FEATURE_SOC_SEMA42_COUNT (0)
    /* @brief TMR availability on the SoC. */
    #define FSL_FEATURE_SOC_TMR_COUNT (0)
    /* @brief TPM availability on the SoC. */
    #define FSL_FEATURE_SOC_TPM_COUNT (0)
    /* @brief TRIAMP availability on the SoC. */
    #define FSL_FEATURE_SOC_TRIAMP_COUNT (0)
    /* @brief TRNG availability on the SoC. */
    #define FSL_FEATURE_SOC_TRNG_COUNT (0)
    /* @brief TSI availability on the SoC. */
    #define FSL_FEATURE_SOC_TSI_COUNT (1)
    /* @brief TRGMUX availability on the SoC. */
    #define FSL_FEATURE_SOC_TRGMUX_COUNT (0)
    /* @brief TSTMR availability on the SoC. */
    #define FSL_FEATURE_SOC_TSTMR_COUNT (0)
    /* @brief UART availability on the SoC. */
    #define FSL_FEATURE_SOC_UART_COUNT (3)
    /* @brief USB availability on the SoC. */
    #define FSL_FEATURE_SOC_USB_COUNT (1)
    /* @brief USBDCD availability on the SoC. */
    #define FSL_FEATURE_SOC_USBDCD_COUNT (1)
    /* @brief USBHSDCD availability on the SoC. */
    #define FSL_FEATURE_SOC_USBHSDCD_COUNT (0)
    /* @brief USBPHY availability on the SoC. */
    #define FSL_FEATURE_SOC_USBPHY_COUNT (0)
    /* @brief VREF availability on the SoC. */
    #define FSL_FEATURE_SOC_VREF_COUNT (1)
    /* @brief WDOG availability on the SoC. */
    #define FSL_FEATURE_SOC_WDOG_COUNT (1)
    /* @brief XBAR availability on the SoC. */
    #define FSL_FEATURE_SOC_XBAR_COUNT (0)
    /* @brief XBARA availability on the SoC. */
    #define FSL_FEATURE_SOC_XBARA_COUNT (0)
    /* @brief XBARB availability on the SoC. */
    #define FSL_FEATURE_SOC_XBARB_COUNT (0)
    /* @brief XCVR availability on the SoC. */
    #define FSL_FEATURE_SOC_XCVR_COUNT (0)
    /* @brief XRDC availability on the SoC. */
    #define FSL_FEATURE_SOC_XRDC_COUNT (0)
    /* @brief ZLL availability on the SoC. */
    #define FSL_FEATURE_SOC_ZLL_COUNT (0)
#endif

/* ADC16 module features */

#if defined(CPU_MK20DX128VFM5) || defined(CPU_MK20DN128VFM5) || defined(CPU_MK20DX64VFM5) || defined(CPU_MK20DN64VFM5) || \
    defined(CPU_MK20DX32VFM5) || defined(CPU_MK20DN32VFM5)
    /* @brief Has Programmable Gain Amplifier (PGA) in ADC (register PGA). */
    #define FSL_FEATURE_ADC16_HAS_PGA (0)
    /* @brief Has PGA chopping control in ADC (bit PGA[PGACHPb] or PGA[PGACHP]). */
    #define FSL_FEATURE_ADC16_HAS_PGA_CHOPPING (0)
    /* @brief Has PGA offset measurement mode in ADC (bit PGA[PGAOFSM]). */
    #define FSL_FEATURE_ADC16_HAS_PGA_OFFSET_MEASUREMENT (0)
    /* @brief Has DMA support (bit SC2[DMAEN] or SC4[DMAEN]). */
    #define FSL_FEATURE_ADC16_HAS_DMA (1)
    /* @brief Has differential mode (bitfield SC1x[DIFF]). */
    #define FSL_FEATURE_ADC16_HAS_DIFF_MODE (0)
    /* @brief Has FIFO (bit SC4[AFDEP]). */
    #define FSL_FEATURE_ADC16_HAS_FIFO (0)
    /* @brief FIFO size if available (bitfield SC4[AFDEP]). */
    #define FSL_FEATURE_ADC16_FIFO_SIZE (0)
    /* @brief Has channel set a/b multiplexor (bitfield CFG2[MUXSEL]). */
    #define FSL_FEATURE_ADC16_HAS_MUX_SELECT (1)
    /* @brief Has HW trigger masking (bitfield SC5[HTRGMASKE]. */
    #define FSL_FEATURE_ADC16_HAS_HW_TRIGGER_MASK (0)
    /* @brief Has calibration feature (bit SC3[CAL] and registers CLPx, CLMx). */
    #define FSL_FEATURE_ADC16_HAS_CALIBRATION (1)
    /* @brief Has HW averaging (bit SC3[AVGE]). */
    #define FSL_FEATURE_ADC16_HAS_HW_AVERAGE (1)
    /* @brief Has offset correction (register OFS). */
    #define FSL_FEATURE_ADC16_HAS_OFFSET_CORRECTION (1)
    /* @brief Maximum ADC resolution. */
    #define FSL_FEATURE_ADC16_MAX_RESOLUTION (16)
    /* @brief Number of SC1x and Rx register pairs (conversion control and result registers). */
    #define FSL_FEATURE_ADC16_CONVERSION_CONTROL_COUNT (2)
#elif defined(CPU_MK20DX128VMP5) || defined(CPU_MK20DN128VMP5) || defined(CPU_MK20DX64VMP5) || defined(CPU_MK20DN64VMP5) || \
    defined(CPU_MK20DX32VMP5) || defined(CPU_MK20DN32VMP5) || defined(CPU_MK20DX128VLH5) || defined(CPU_MK20DN128VLH5) || \
    defined(CPU_MK20DX64VLH5) || defined(CPU_MK20DN64VLH5) || defined(CPU_MK20DX32VLH5) || defined(CPU_MK20DN32VLH5) || \
    defined(CPU_MK20DX128VFT5) || defined(CPU_MK20DN128VFT5) || defined(CPU_MK20DX64VFT5) || defined(CPU_MK20DN64VFT5) || \
    defined(CPU_MK20DX32VFT5) || defined(CPU_MK20DN32VFT5) || defined(CPU_MK20DX128VLF5) || defined(CPU_MK20DN128VLF5) || \
    defined(CPU_MK20DX64VLF5) || defined(CPU_MK20DN64VLF5) || defined(CPU_MK20DX32VLF5) || defined(CPU_MK20DN32VLF5)
    /* @brief Has Programmable Gain Amplifier (PGA) in ADC (register PGA). */
    #define FSL_FEATURE_ADC16_HAS_PGA (0)
    /* @brief Has PGA chopping control in ADC (bit PGA[PGACHPb] or PGA[PGACHP]). */
    #define FSL_FEATURE_ADC16_HAS_PGA_CHOPPING (0)
    /* @brief Has PGA offset measurement mode in ADC (bit PGA[PGAOFSM]). */
    #define FSL_FEATURE_ADC16_HAS_PGA_OFFSET_MEASUREMENT (0)
    /* @brief Has DMA support (bit SC2[DMAEN] or SC4[DMAEN]). */
    #define FSL_FEATURE_ADC16_HAS_DMA (1)
    /* @brief Has differential mode (bitfield SC1x[DIFF]). */
    #define FSL_FEATURE_ADC16_HAS_DIFF_MODE (1)
    /* @brief Has FIFO (bit SC4[AFDEP]). */
    #define FSL_FEATURE_ADC16_HAS_FIFO (0)
    /* @brief FIFO size if available (bitfield SC4[AFDEP]). */
    #define FSL_FEATURE_ADC16_FIFO_SIZE (0)
    /* @brief Has channel set a/b multiplexor (bitfield CFG2[MUXSEL]). */
    #define FSL_FEATURE_ADC16_HAS_MUX_SELECT (1)
    /* @brief Has HW trigger masking (bitfield SC5[HTRGMASKE]. */
    #define FSL_FEATURE_ADC16_HAS_HW_TRIGGER_MASK (0)
    /* @brief Has calibration feature (bit SC3[CAL] and registers CLPx, CLMx). */
    #define FSL_FEATURE_ADC16_HAS_CALIBRATION (1)
    /* @brief Has HW averaging (bit SC3[AVGE]). */
    #define FSL_FEATURE_ADC16_HAS_HW_AVERAGE (1)
    /* @brief Has offset correction (register OFS). */
    #define FSL_FEATURE_ADC16_HAS_OFFSET_CORRECTION (1)
    /* @brief Maximum ADC resolution. */
    #define FSL_FEATURE_ADC16_MAX_RESOLUTION (16)
    /* @brief Number of SC1x and Rx register pairs (conversion control and result registers). */
    #define FSL_FEATURE_ADC16_CONVERSION_CONTROL_COUNT (2)
#endif /* defined(CPU_MK20DX128VFM5) || defined(CPU_MK20DN128VFM5) || defined(CPU_MK20DX64VFM5) || defined(CPU_MK20DN64VFM5) || \
    defined(CPU_MK20DX32VFM5) || defined(CPU_MK20DN32VFM5) */

/* CMP module features */

/* @brief Has Trigger mode in CMP (register bit field CR1[TRIGM]). */
#define FSL_FEATURE_CMP_HAS_TRIGGER_MODE (0)
/* @brief Has Window mode in CMP (register bit field CR1[WE]). */
#define FSL_FEATURE_CMP_HAS_WINDOW_MODE (1)
/* @brief Has External sample supported in CMP (register bit field CR1[SE]). */
#define FSL_FEATURE_CMP_HAS_EXTERNAL_SAMPLE_SUPPORT (1)
/* @brief Has DMA support in CMP (register bit field SCR[DMAEN]). */
#define FSL_FEATURE_CMP_HAS_DMA (1)
/* @brief Has Pass Through mode in CMP (register bit field MUXCR[PSTM]). */
#define FSL_FEATURE_CMP_HAS_PASS_THROUGH_MODE (0)
/* @brief Has DAC Test function in CMP (register DACTEST). */
#define FSL_FEATURE_CMP_HAS_DAC_TEST (0)

/* CRC module features */

/* @brief Has data register with name CRC */
#define FSL_FEATURE_CRC_HAS_CRC_REG (1)

/* EDMA module features */

/* @brief Number of DMA channels (related to number of registers TCD, DCHPRI, bit fields ERQ[ERQn], EEI[EEIn], INT[INTn], ERR[ERRn], HRS[HRSn] and bit field widths ES[ERRCHN], CEEI[CEEI], SEEI[SEEI], CERQ[CERQ], SERQ[SERQ], CDNE[CDNE], SSRT[SSRT], CERR[CERR], CINT[CINT], TCDn_CITER_ELINKYES[LINKCH], TCDn_CSR[MAJORLINKCH], TCDn_BITER_ELINKYES[LINKCH]). (Valid only for eDMA modules.) */
#define FSL_FEATURE_EDMA_MODULE_CHANNEL (4)
/* @brief Total number of DMA channels on all modules. */
#define FSL_FEATURE_EDMA_DMAMUX_CHANNELS (FSL_FEATURE_SOC_EDMA_COUNT * 4)
/* @brief Number of DMA channel groups (register bit fields CR[ERGA], CR[GRPnPRI], ES[GPE], DCHPRIn[GRPPRI]). (Valid only for eDMA modules.) */
#define FSL_FEATURE_EDMA_CHANNEL_GROUP_COUNT (1)
/* @brief Has DMA_Error interrupt vector. */
#define FSL_FEATURE_EDMA_HAS_ERROR_IRQ (1)
/* @brief Number of DMA channels with asynchronous request capability (register EARS). (Valid only for eDMA modules.) */
#define FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT (0)

/* DMAMUX module features */

/* @brief Number of DMA channels (related to number of register CHCFGn). */
#define FSL_FEATURE_DMAMUX_MODULE_CHANNEL (4)
/* @brief Total number of DMA channels on all modules. */
#define FSL_FEATURE_DMAMUX_DMAMUX_CHANNELS (FSL_FEATURE_SOC_DMAMUX_COUNT * 4)
/* @brief Has the periodic trigger capability for the triggered DMA channel (register bit CHCFG0[TRIG]). */
#define FSL_FEATURE_DMAMUX_HAS_TRIG (1)

/* EWM module features */

/* @brief Has clock select (register CLKCTRL). */
#define FSL_FEATURE_EWM_HAS_CLOCK_SELECT  (0)
/* @brief Has clock prescaler (register CLKPRESCALER). */
#define FSL_FEATURE_EWM_HAS_PRESCALER  (0)

/* FLASH module features */

#if defined(CPU_MK20DX128VFM5) || defined(CPU_MK20DX128VMP5) || defined(CPU_MK20DX128VLH5) || defined(CPU_MK20DX128VFT5) || \
    defined(CPU_MK20DX128VLF5)
    /* @brief Is of type FTFA. */
    #define FSL_FEATURE_FLASH_IS_FTFA (0)
    /* @brief Is of type FTFE. */
    #define FSL_FEATURE_FLASH_IS_FTFE (0)
    /* @brief Is of type FTFL. */
    #define FSL_FEATURE_FLASH_IS_FTFL (1)
    /* @brief Has flags indicating the status of the FlexRAM (register bits FCNFG[EEERDY], FCNFG[RAMRDY] and FCNFG[PFLSH]). */
    #define FSL_FEATURE_FLASH_HAS_FLEX_RAM_FLAGS (1)
    /* @brief Has program flash swapping status flag (register bit FCNFG[SWAP]). */
    #define FSL_FEATURE_FLASH_HAS_PFLASH_SWAPPING_STATUS_FLAG (0)
    /* @brief Has EEPROM region protection (register FEPROT). */
    #define FSL_FEATURE_FLASH_HAS_EEROM_REGION_PROTECTION (1)
    /* @brief Has data flash region protection (register FDPROT). */
    #define FSL_FEATURE_FLASH_HAS_DATA_FLASH_REGION_PROTECTION (1)
    /* @brief Has flash access control (registers XACCHn, SACCHn, where n is a number, FACSS and FACSN). */
    #define FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL (0)
    /* @brief Has flash cache control in FMC module. */
    #define FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS (1)
    /* @brief Has flash cache control in MCM module. */
    #define FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS (0)
    /* @brief P-Flash start address. */
    #define FSL_FEATURE_FLASH_PFLASH_START_ADDRESS (0x00000000)
    /* @brief P-Flash block count. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT (1)
    /* @brief P-Flash block size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE (131072)
    /* @brief P-Flash sector size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE (1024)
    /* @brief P-Flash write unit size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE (4)
    /* @brief P-Flash data path width. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_DATA_PATH_WIDTH (4)
    /* @brief P-Flash block swap feature. */
    #define FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP (0)
    /* @brief Has FlexNVM memory. */
    #define FSL_FEATURE_FLASH_HAS_FLEX_NVM (1)
    /* @brief FlexNVM start address. (Valid only if FlexNVM is available.) */
    #define FSL_FEATURE_FLASH_FLEX_NVM_START_ADDRESS (0x10000000)
    /* @brief FlexNVM block count. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT (1)
    /* @brief FlexNVM block size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE (32768)
    /* @brief FlexNVM sector size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE (1024)
    /* @brief FlexNVM write unit size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_WRITE_UNIT_SIZE (4)
    /* @brief FlexNVM data path width. */
    #define FSL_FEATURE_FLASH_FLEX_BLOCK_DATA_PATH_WIDTH (4)
    /* @brief Has FlexRAM memory. */
    #define FSL_FEATURE_FLASH_HAS_FLEX_RAM (1)
    /* @brief FlexRAM start address. (Valid only if FlexRAM is available.) */
    #define FSL_FEATURE_FLASH_FLEX_RAM_START_ADDRESS (0x14000000)
    /* @brief FlexRAM size. */
    #define FSL_FEATURE_FLASH_FLEX_RAM_SIZE (2048)
    /* @brief Has 0x00 Read 1s Block command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_BLOCK_CMD (0)
    /* @brief Has 0x01 Read 1s Section command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_SECTION_CMD (1)
    /* @brief Has 0x02 Program Check command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_CHECK_CMD (1)
    /* @brief Has 0x03 Read Resource command. */
    #define FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD (1)
    /* @brief Has 0x06 Program Longword command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_LONGWORD_CMD (1)
    /* @brief Has 0x07 Program Phrase command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_PHRASE_CMD (0)
    /* @brief Has 0x08 Erase Flash Block command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_FLASH_BLOCK_CMD (0)
    /* @brief Has 0x09 Erase Flash Sector command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_FLASH_SECTOR_CMD (1)
    /* @brief Has 0x0B Program Section command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_SECTION_CMD (1)
    /* @brief Has 0x40 Read 1s All Blocks command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_ALL_BLOCKS_CMD (0)
    /* @brief Has 0x41 Read Once command. */
    #define FSL_FEATURE_FLASH_HAS_READ_ONCE_CMD (1)
    /* @brief Has 0x43 Program Once command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_ONCE_CMD (1)
    /* @brief Has 0x44 Erase All Blocks command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_ALL_BLOCKS_CMD (0)
    /* @brief Has 0x45 Verify Backdoor Access Key command. */
    #define FSL_FEATURE_FLASH_HAS_VERIFY_BACKDOOR_ACCESS_KEY_CMD (1)
    /* @brief Has 0x46 Swap Control command. */
    #define FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD (0)
    /* @brief Has 0x80 Program Partition command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_PARTITION_CMD (1)
    /* @brief Has 0x81 Set FlexRAM Function command. */
    #define FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD (1)
    /* @brief P-Flash Erase/Read 1st all block command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Erase sector command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SECTOR_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Rrogram/Verify section command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SECTION_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Read resource command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_RESOURCE_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Program check command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_CHECK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Program check command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SWAP_CONTROL_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Erase/Read 1st all block command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM Erase sector command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_SECTOR_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM Rrogram/Verify section command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_SECTION_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM Read resource command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_RESOURCE_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM Program check command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_CHECK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM partition code 0000 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0000 (0x00008000)
    /* @brief FlexNVM partition code 0001 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0001 (0x00006000)
    /* @brief FlexNVM partition code 0010 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0010 (0x00004000)
    /* @brief FlexNVM partition code 0011 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0011 (0x00000000)
    /* @brief FlexNVM partition code 0100 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0100 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0101 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0101 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0110 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0110 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0111 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0111 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1000 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1000 (0x00000000)
    /* @brief FlexNVM partition code 1001 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1001 (0x00002000)
    /* @brief FlexNVM partition code 1010 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1010 (0x00004000)
    /* @brief FlexNVM partition code 1011 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1011 (0x00008000)
    /* @brief FlexNVM partition code 1100 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1100 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1101 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1101 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1110 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1110 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1111 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1111 (0x00008000)
    /* @brief Emulated eeprom size code 0000 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0000 (0xFFFF)
    /* @brief Emulated eeprom size code 0001 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0001 (0xFFFF)
    /* @brief Emulated eeprom size code 0010 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0010 (0xFFFF)
    /* @brief Emulated eeprom size code 0011 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0011 (0x0800)
    /* @brief Emulated eeprom size code 0100 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0100 (0x0400)
    /* @brief Emulated eeprom size code 0101 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0101 (0x0200)
    /* @brief Emulated eeprom size code 0110 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0110 (0x0100)
    /* @brief Emulated eeprom size code 0111 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0111 (0x0080)
    /* @brief Emulated eeprom size code 1000 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1000 (0x0040)
    /* @brief Emulated eeprom size code 1001 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1001 (0x0020)
    /* @brief Emulated eeprom size code 1010 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1010 (0xFFFF)
    /* @brief Emulated eeprom size code 1011 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1011 (0xFFFF)
    /* @brief Emulated eeprom size code 1100 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1100 (0xFFFF)
    /* @brief Emulated eeprom size code 1101 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1101 (0xFFFF)
    /* @brief Emulated eeprom size code 1110 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1110 (0xFFFF)
    /* @brief Emulated eeprom size code 1111 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1111 (0x0000)
#elif defined(CPU_MK20DN128VFM5) || defined(CPU_MK20DN128VMP5) || defined(CPU_MK20DN128VLH5) || defined(CPU_MK20DN128VFT5) || \
    defined(CPU_MK20DN128VLF5)
    /* @brief Is of type FTFA. */
    #define FSL_FEATURE_FLASH_IS_FTFA (0)
    /* @brief Is of type FTFE. */
    #define FSL_FEATURE_FLASH_IS_FTFE (0)
    /* @brief Is of type FTFL. */
    #define FSL_FEATURE_FLASH_IS_FTFL (1)
    /* @brief Has flags indicating the status of the FlexRAM (register bits FCNFG[EEERDY], FCNFG[RAMRDY] and FCNFG[PFLSH]). */
    #define FSL_FEATURE_FLASH_HAS_FLEX_RAM_FLAGS (1)
    /* @brief Has program flash swapping status flag (register bit FCNFG[SWAP]). */
    #define FSL_FEATURE_FLASH_HAS_PFLASH_SWAPPING_STATUS_FLAG (0)
    /* @brief Has EEPROM region protection (register FEPROT). */
    #define FSL_FEATURE_FLASH_HAS_EEROM_REGION_PROTECTION (1)
    /* @brief Has data flash region protection (register FDPROT). */
    #define FSL_FEATURE_FLASH_HAS_DATA_FLASH_REGION_PROTECTION (1)
    /* @brief Has flash access control (registers XACCHn, SACCHn, where n is a number, FACSS and FACSN). */
    #define FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL (0)
    /* @brief Has flash cache control in FMC module. */
    #define FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS (1)
    /* @brief Has flash cache control in MCM module. */
    #define FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS (0)
    /* @brief P-Flash start address. */
    #define FSL_FEATURE_FLASH_PFLASH_START_ADDRESS (0x00000000)
    /* @brief P-Flash block count. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT (1)
    /* @brief P-Flash block size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE (131072)
    /* @brief P-Flash sector size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE (1024)
    /* @brief P-Flash write unit size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE (4)
    /* @brief P-Flash data path width. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_DATA_PATH_WIDTH (4)
    /* @brief P-Flash block swap feature. */
    #define FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP (0)
    /* @brief Has FlexNVM memory. */
    #define FSL_FEATURE_FLASH_HAS_FLEX_NVM (0)
    /* @brief FlexNVM start address. (Valid only if FlexNVM is available.) */
    #define FSL_FEATURE_FLASH_FLEX_NVM_START_ADDRESS (0x00000000)
    /* @brief FlexNVM block count. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT (0)
    /* @brief FlexNVM block size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE (0)
    /* @brief FlexNVM sector size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE (0)
    /* @brief FlexNVM write unit size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_WRITE_UNIT_SIZE (0)
    /* @brief FlexNVM data path width. */
    #define FSL_FEATURE_FLASH_FLEX_BLOCK_DATA_PATH_WIDTH (0)
    /* @brief Has FlexRAM memory. */
    #define FSL_FEATURE_FLASH_HAS_FLEX_RAM (1)
    /* @brief FlexRAM start address. (Valid only if FlexRAM is available.) */
    #define FSL_FEATURE_FLASH_FLEX_RAM_START_ADDRESS (0x14000000)
    /* @brief FlexRAM size. */
    #define FSL_FEATURE_FLASH_FLEX_RAM_SIZE (2048)
    /* @brief Has 0x00 Read 1s Block command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_BLOCK_CMD (0)
    /* @brief Has 0x01 Read 1s Section command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_SECTION_CMD (1)
    /* @brief Has 0x02 Program Check command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_CHECK_CMD (1)
    /* @brief Has 0x03 Read Resource command. */
    #define FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD (1)
    /* @brief Has 0x06 Program Longword command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_LONGWORD_CMD (1)
    /* @brief Has 0x07 Program Phrase command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_PHRASE_CMD (0)
    /* @brief Has 0x08 Erase Flash Block command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_FLASH_BLOCK_CMD (0)
    /* @brief Has 0x09 Erase Flash Sector command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_FLASH_SECTOR_CMD (1)
    /* @brief Has 0x0B Program Section command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_SECTION_CMD (1)
    /* @brief Has 0x40 Read 1s All Blocks command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_ALL_BLOCKS_CMD (0)
    /* @brief Has 0x41 Read Once command. */
    #define FSL_FEATURE_FLASH_HAS_READ_ONCE_CMD (1)
    /* @brief Has 0x43 Program Once command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_ONCE_CMD (1)
    /* @brief Has 0x44 Erase All Blocks command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_ALL_BLOCKS_CMD (0)
    /* @brief Has 0x45 Verify Backdoor Access Key command. */
    #define FSL_FEATURE_FLASH_HAS_VERIFY_BACKDOOR_ACCESS_KEY_CMD (1)
    /* @brief Has 0x46 Swap Control command. */
    #define FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD (0)
    /* @brief Has 0x80 Program Partition command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_PARTITION_CMD (0)
    /* @brief Has 0x81 Set FlexRAM Function command. */
    #define FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD (0)
    /* @brief P-Flash Erase/Read 1st all block command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Erase sector command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SECTOR_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Rrogram/Verify section command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SECTION_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Read resource command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_RESOURCE_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Program check command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_CHECK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Program check command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SWAP_CONTROL_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Erase/Read 1st all block command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Erase sector command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_SECTOR_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Rrogram/Verify section command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_SECTION_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Read resource command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_RESOURCE_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Program check command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_CHECK_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM partition code 0000 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0000 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0001 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0001 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0010 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0010 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0011 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0011 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0100 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0100 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0101 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0101 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0110 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0110 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0111 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0111 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1000 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1000 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1001 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1001 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1010 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1010 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1011 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1011 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1100 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1100 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1101 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1101 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1110 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1110 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1111 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1111 (0xFFFFFFFF)
    /* @brief Emulated eeprom size code 0000 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0000 (0xFFFF)
    /* @brief Emulated eeprom size code 0001 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0001 (0xFFFF)
    /* @brief Emulated eeprom size code 0010 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0010 (0xFFFF)
    /* @brief Emulated eeprom size code 0011 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0011 (0x0800)
    /* @brief Emulated eeprom size code 0100 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0100 (0x0400)
    /* @brief Emulated eeprom size code 0101 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0101 (0x0200)
    /* @brief Emulated eeprom size code 0110 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0110 (0x0100)
    /* @brief Emulated eeprom size code 0111 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0111 (0x0080)
    /* @brief Emulated eeprom size code 1000 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1000 (0x0040)
    /* @brief Emulated eeprom size code 1001 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1001 (0x0020)
    /* @brief Emulated eeprom size code 1010 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1010 (0xFFFF)
    /* @brief Emulated eeprom size code 1011 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1011 (0xFFFF)
    /* @brief Emulated eeprom size code 1100 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1100 (0xFFFF)
    /* @brief Emulated eeprom size code 1101 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1101 (0xFFFF)
    /* @brief Emulated eeprom size code 1110 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1110 (0xFFFF)
    /* @brief Emulated eeprom size code 1111 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1111 (0x0000)
#elif defined(CPU_MK20DX64VFM5) || defined(CPU_MK20DX64VMP5) || defined(CPU_MK20DX64VLH5) || defined(CPU_MK20DX64VFT5) || \
    defined(CPU_MK20DX64VLF5)
    /* @brief Is of type FTFA. */
    #define FSL_FEATURE_FLASH_IS_FTFA (0)
    /* @brief Is of type FTFE. */
    #define FSL_FEATURE_FLASH_IS_FTFE (0)
    /* @brief Is of type FTFL. */
    #define FSL_FEATURE_FLASH_IS_FTFL (1)
    /* @brief Has flags indicating the status of the FlexRAM (register bits FCNFG[EEERDY], FCNFG[RAMRDY] and FCNFG[PFLSH]). */
    #define FSL_FEATURE_FLASH_HAS_FLEX_RAM_FLAGS (1)
    /* @brief Has program flash swapping status flag (register bit FCNFG[SWAP]). */
    #define FSL_FEATURE_FLASH_HAS_PFLASH_SWAPPING_STATUS_FLAG (0)
    /* @brief Has EEPROM region protection (register FEPROT). */
    #define FSL_FEATURE_FLASH_HAS_EEROM_REGION_PROTECTION (1)
    /* @brief Has data flash region protection (register FDPROT). */
    #define FSL_FEATURE_FLASH_HAS_DATA_FLASH_REGION_PROTECTION (1)
    /* @brief Has flash access control (registers XACCHn, SACCHn, where n is a number, FACSS and FACSN). */
    #define FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL (0)
    /* @brief Has flash cache control in FMC module. */
    #define FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS (1)
    /* @brief Has flash cache control in MCM module. */
    #define FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS (0)
    /* @brief P-Flash start address. */
    #define FSL_FEATURE_FLASH_PFLASH_START_ADDRESS (0x00000000)
    /* @brief P-Flash block count. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT (1)
    /* @brief P-Flash block size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE (65536)
    /* @brief P-Flash sector size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE (1024)
    /* @brief P-Flash write unit size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE (4)
    /* @brief P-Flash data path width. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_DATA_PATH_WIDTH (4)
    /* @brief P-Flash block swap feature. */
    #define FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP (0)
    /* @brief Has FlexNVM memory. */
    #define FSL_FEATURE_FLASH_HAS_FLEX_NVM (1)
    /* @brief FlexNVM start address. (Valid only if FlexNVM is available.) */
    #define FSL_FEATURE_FLASH_FLEX_NVM_START_ADDRESS (0x10000000)
    /* @brief FlexNVM block count. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT (1)
    /* @brief FlexNVM block size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE (32768)
    /* @brief FlexNVM sector size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE (1024)
    /* @brief FlexNVM write unit size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_WRITE_UNIT_SIZE (4)
    /* @brief FlexNVM data path width. */
    #define FSL_FEATURE_FLASH_FLEX_BLOCK_DATA_PATH_WIDTH (4)
    /* @brief Has FlexRAM memory. */
    #define FSL_FEATURE_FLASH_HAS_FLEX_RAM (1)
    /* @brief FlexRAM start address. (Valid only if FlexRAM is available.) */
    #define FSL_FEATURE_FLASH_FLEX_RAM_START_ADDRESS (0x14000000)
    /* @brief FlexRAM size. */
    #define FSL_FEATURE_FLASH_FLEX_RAM_SIZE (2048)
    /* @brief Has 0x00 Read 1s Block command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_BLOCK_CMD (0)
    /* @brief Has 0x01 Read 1s Section command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_SECTION_CMD (1)
    /* @brief Has 0x02 Program Check command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_CHECK_CMD (1)
    /* @brief Has 0x03 Read Resource command. */
    #define FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD (1)
    /* @brief Has 0x06 Program Longword command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_LONGWORD_CMD (1)
    /* @brief Has 0x07 Program Phrase command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_PHRASE_CMD (0)
    /* @brief Has 0x08 Erase Flash Block command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_FLASH_BLOCK_CMD (0)
    /* @brief Has 0x09 Erase Flash Sector command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_FLASH_SECTOR_CMD (1)
    /* @brief Has 0x0B Program Section command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_SECTION_CMD (1)
    /* @brief Has 0x40 Read 1s All Blocks command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_ALL_BLOCKS_CMD (0)
    /* @brief Has 0x41 Read Once command. */
    #define FSL_FEATURE_FLASH_HAS_READ_ONCE_CMD (1)
    /* @brief Has 0x43 Program Once command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_ONCE_CMD (1)
    /* @brief Has 0x44 Erase All Blocks command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_ALL_BLOCKS_CMD (0)
    /* @brief Has 0x45 Verify Backdoor Access Key command. */
    #define FSL_FEATURE_FLASH_HAS_VERIFY_BACKDOOR_ACCESS_KEY_CMD (1)
    /* @brief Has 0x46 Swap Control command. */
    #define FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD (0)
    /* @brief Has 0x80 Program Partition command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_PARTITION_CMD (1)
    /* @brief Has 0x81 Set FlexRAM Function command. */
    #define FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD (1)
    /* @brief P-Flash Erase/Read 1st all block command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Erase sector command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SECTOR_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Rrogram/Verify section command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SECTION_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Read resource command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_RESOURCE_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Program check command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_CHECK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Program check command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SWAP_CONTROL_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Erase/Read 1st all block command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM Erase sector command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_SECTOR_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM Rrogram/Verify section command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_SECTION_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM Read resource command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_RESOURCE_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM Program check command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_CHECK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM partition code 0000 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0000 (0x00008000)
    /* @brief FlexNVM partition code 0001 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0001 (0x00006000)
    /* @brief FlexNVM partition code 0010 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0010 (0x00004000)
    /* @brief FlexNVM partition code 0011 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0011 (0x00000000)
    /* @brief FlexNVM partition code 0100 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0100 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0101 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0101 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0110 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0110 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0111 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0111 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1000 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1000 (0x00000000)
    /* @brief FlexNVM partition code 1001 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1001 (0x00002000)
    /* @brief FlexNVM partition code 1010 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1010 (0x00004000)
    /* @brief FlexNVM partition code 1011 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1011 (0x00008000)
    /* @brief FlexNVM partition code 1100 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1100 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1101 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1101 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1110 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1110 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1111 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1111 (0x00008000)
    /* @brief Emulated eeprom size code 0000 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0000 (0xFFFF)
    /* @brief Emulated eeprom size code 0001 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0001 (0xFFFF)
    /* @brief Emulated eeprom size code 0010 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0010 (0xFFFF)
    /* @brief Emulated eeprom size code 0011 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0011 (0x0800)
    /* @brief Emulated eeprom size code 0100 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0100 (0x0400)
    /* @brief Emulated eeprom size code 0101 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0101 (0x0200)
    /* @brief Emulated eeprom size code 0110 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0110 (0x0100)
    /* @brief Emulated eeprom size code 0111 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0111 (0x0080)
    /* @brief Emulated eeprom size code 1000 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1000 (0x0040)
    /* @brief Emulated eeprom size code 1001 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1001 (0x0020)
    /* @brief Emulated eeprom size code 1010 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1010 (0xFFFF)
    /* @brief Emulated eeprom size code 1011 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1011 (0xFFFF)
    /* @brief Emulated eeprom size code 1100 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1100 (0xFFFF)
    /* @brief Emulated eeprom size code 1101 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1101 (0xFFFF)
    /* @brief Emulated eeprom size code 1110 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1110 (0xFFFF)
    /* @brief Emulated eeprom size code 1111 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1111 (0x0000)
#elif defined(CPU_MK20DN64VFM5) || defined(CPU_MK20DN64VMP5) || defined(CPU_MK20DN64VLH5) || defined(CPU_MK20DN64VFT5) || \
    defined(CPU_MK20DN64VLF5)
    /* @brief Is of type FTFA. */
    #define FSL_FEATURE_FLASH_IS_FTFA (0)
    /* @brief Is of type FTFE. */
    #define FSL_FEATURE_FLASH_IS_FTFE (0)
    /* @brief Is of type FTFL. */
    #define FSL_FEATURE_FLASH_IS_FTFL (1)
    /* @brief Has flags indicating the status of the FlexRAM (register bits FCNFG[EEERDY], FCNFG[RAMRDY] and FCNFG[PFLSH]). */
    #define FSL_FEATURE_FLASH_HAS_FLEX_RAM_FLAGS (1)
    /* @brief Has program flash swapping status flag (register bit FCNFG[SWAP]). */
    #define FSL_FEATURE_FLASH_HAS_PFLASH_SWAPPING_STATUS_FLAG (0)
    /* @brief Has EEPROM region protection (register FEPROT). */
    #define FSL_FEATURE_FLASH_HAS_EEROM_REGION_PROTECTION (1)
    /* @brief Has data flash region protection (register FDPROT). */
    #define FSL_FEATURE_FLASH_HAS_DATA_FLASH_REGION_PROTECTION (1)
    /* @brief Has flash access control (registers XACCHn, SACCHn, where n is a number, FACSS and FACSN). */
    #define FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL (0)
    /* @brief Has flash cache control in FMC module. */
    #define FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS (1)
    /* @brief Has flash cache control in MCM module. */
    #define FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS (0)
    /* @brief P-Flash start address. */
    #define FSL_FEATURE_FLASH_PFLASH_START_ADDRESS (0x00000000)
    /* @brief P-Flash block count. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT (1)
    /* @brief P-Flash block size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE (65536)
    /* @brief P-Flash sector size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE (1024)
    /* @brief P-Flash write unit size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE (4)
    /* @brief P-Flash data path width. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_DATA_PATH_WIDTH (4)
    /* @brief P-Flash block swap feature. */
    #define FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP (0)
    /* @brief Has FlexNVM memory. */
    #define FSL_FEATURE_FLASH_HAS_FLEX_NVM (0)
    /* @brief FlexNVM start address. (Valid only if FlexNVM is available.) */
    #define FSL_FEATURE_FLASH_FLEX_NVM_START_ADDRESS (0x00000000)
    /* @brief FlexNVM block count. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT (0)
    /* @brief FlexNVM block size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE (0)
    /* @brief FlexNVM sector size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE (0)
    /* @brief FlexNVM write unit size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_WRITE_UNIT_SIZE (0)
    /* @brief FlexNVM data path width. */
    #define FSL_FEATURE_FLASH_FLEX_BLOCK_DATA_PATH_WIDTH (0)
    /* @brief Has FlexRAM memory. */
    #define FSL_FEATURE_FLASH_HAS_FLEX_RAM (1)
    /* @brief FlexRAM start address. (Valid only if FlexRAM is available.) */
    #define FSL_FEATURE_FLASH_FLEX_RAM_START_ADDRESS (0x14000000)
    /* @brief FlexRAM size. */
    #define FSL_FEATURE_FLASH_FLEX_RAM_SIZE (2048)
    /* @brief Has 0x00 Read 1s Block command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_BLOCK_CMD (0)
    /* @brief Has 0x01 Read 1s Section command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_SECTION_CMD (1)
    /* @brief Has 0x02 Program Check command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_CHECK_CMD (1)
    /* @brief Has 0x03 Read Resource command. */
    #define FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD (1)
    /* @brief Has 0x06 Program Longword command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_LONGWORD_CMD (1)
    /* @brief Has 0x07 Program Phrase command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_PHRASE_CMD (0)
    /* @brief Has 0x08 Erase Flash Block command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_FLASH_BLOCK_CMD (0)
    /* @brief Has 0x09 Erase Flash Sector command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_FLASH_SECTOR_CMD (1)
    /* @brief Has 0x0B Program Section command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_SECTION_CMD (1)
    /* @brief Has 0x40 Read 1s All Blocks command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_ALL_BLOCKS_CMD (0)
    /* @brief Has 0x41 Read Once command. */
    #define FSL_FEATURE_FLASH_HAS_READ_ONCE_CMD (1)
    /* @brief Has 0x43 Program Once command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_ONCE_CMD (1)
    /* @brief Has 0x44 Erase All Blocks command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_ALL_BLOCKS_CMD (0)
    /* @brief Has 0x45 Verify Backdoor Access Key command. */
    #define FSL_FEATURE_FLASH_HAS_VERIFY_BACKDOOR_ACCESS_KEY_CMD (1)
    /* @brief Has 0x46 Swap Control command. */
    #define FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD (0)
    /* @brief Has 0x80 Program Partition command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_PARTITION_CMD (0)
    /* @brief Has 0x81 Set FlexRAM Function command. */
    #define FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD (0)
    /* @brief P-Flash Erase/Read 1st all block command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Erase sector command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SECTOR_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Rrogram/Verify section command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SECTION_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Read resource command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_RESOURCE_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Program check command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_CHECK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Program check command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SWAP_CONTROL_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Erase/Read 1st all block command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Erase sector command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_SECTOR_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Rrogram/Verify section command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_SECTION_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Read resource command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_RESOURCE_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Program check command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_CHECK_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM partition code 0000 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0000 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0001 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0001 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0010 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0010 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0011 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0011 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0100 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0100 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0101 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0101 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0110 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0110 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0111 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0111 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1000 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1000 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1001 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1001 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1010 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1010 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1011 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1011 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1100 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1100 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1101 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1101 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1110 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1110 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1111 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1111 (0xFFFFFFFF)
    /* @brief Emulated eeprom size code 0000 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0000 (0xFFFF)
    /* @brief Emulated eeprom size code 0001 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0001 (0xFFFF)
    /* @brief Emulated eeprom size code 0010 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0010 (0xFFFF)
    /* @brief Emulated eeprom size code 0011 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0011 (0x0800)
    /* @brief Emulated eeprom size code 0100 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0100 (0x0400)
    /* @brief Emulated eeprom size code 0101 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0101 (0x0200)
    /* @brief Emulated eeprom size code 0110 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0110 (0x0100)
    /* @brief Emulated eeprom size code 0111 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0111 (0x0080)
    /* @brief Emulated eeprom size code 1000 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1000 (0x0040)
    /* @brief Emulated eeprom size code 1001 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1001 (0x0020)
    /* @brief Emulated eeprom size code 1010 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1010 (0xFFFF)
    /* @brief Emulated eeprom size code 1011 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1011 (0xFFFF)
    /* @brief Emulated eeprom size code 1100 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1100 (0xFFFF)
    /* @brief Emulated eeprom size code 1101 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1101 (0xFFFF)
    /* @brief Emulated eeprom size code 1110 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1110 (0xFFFF)
    /* @brief Emulated eeprom size code 1111 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1111 (0x0000)
#elif defined(CPU_MK20DX32VFM5) || defined(CPU_MK20DX32VMP5) || defined(CPU_MK20DX32VLH5) || defined(CPU_MK20DX32VFT5) || \
    defined(CPU_MK20DX32VLF5)
    /* @brief Is of type FTFA. */
    #define FSL_FEATURE_FLASH_IS_FTFA (0)
    /* @brief Is of type FTFE. */
    #define FSL_FEATURE_FLASH_IS_FTFE (0)
    /* @brief Is of type FTFL. */
    #define FSL_FEATURE_FLASH_IS_FTFL (1)
    /* @brief Has flags indicating the status of the FlexRAM (register bits FCNFG[EEERDY], FCNFG[RAMRDY] and FCNFG[PFLSH]). */
    #define FSL_FEATURE_FLASH_HAS_FLEX_RAM_FLAGS (1)
    /* @brief Has program flash swapping status flag (register bit FCNFG[SWAP]). */
    #define FSL_FEATURE_FLASH_HAS_PFLASH_SWAPPING_STATUS_FLAG (0)
    /* @brief Has EEPROM region protection (register FEPROT). */
    #define FSL_FEATURE_FLASH_HAS_EEROM_REGION_PROTECTION (1)
    /* @brief Has data flash region protection (register FDPROT). */
    #define FSL_FEATURE_FLASH_HAS_DATA_FLASH_REGION_PROTECTION (1)
    /* @brief Has flash access control (registers XACCHn, SACCHn, where n is a number, FACSS and FACSN). */
    #define FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL (0)
    /* @brief Has flash cache control in FMC module. */
    #define FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS (1)
    /* @brief Has flash cache control in MCM module. */
    #define FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS (0)
    /* @brief P-Flash start address. */
    #define FSL_FEATURE_FLASH_PFLASH_START_ADDRESS (0x00000000)
    /* @brief P-Flash block count. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT (1)
    /* @brief P-Flash block size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE (32768)
    /* @brief P-Flash sector size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE (1024)
    /* @brief P-Flash write unit size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE (4)
    /* @brief P-Flash data path width. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_DATA_PATH_WIDTH (4)
    /* @brief P-Flash block swap feature. */
    #define FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP (0)
    /* @brief Has FlexNVM memory. */
    #define FSL_FEATURE_FLASH_HAS_FLEX_NVM (1)
    /* @brief FlexNVM start address. (Valid only if FlexNVM is available.) */
    #define FSL_FEATURE_FLASH_FLEX_NVM_START_ADDRESS (0x10000000)
    /* @brief FlexNVM block count. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT (1)
    /* @brief FlexNVM block size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE (32768)
    /* @brief FlexNVM sector size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE (1024)
    /* @brief FlexNVM write unit size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_WRITE_UNIT_SIZE (4)
    /* @brief FlexNVM data path width. */
    #define FSL_FEATURE_FLASH_FLEX_BLOCK_DATA_PATH_WIDTH (4)
    /* @brief Has FlexRAM memory. */
    #define FSL_FEATURE_FLASH_HAS_FLEX_RAM (1)
    /* @brief FlexRAM start address. (Valid only if FlexRAM is available.) */
    #define FSL_FEATURE_FLASH_FLEX_RAM_START_ADDRESS (0x14000000)
    /* @brief FlexRAM size. */
    #define FSL_FEATURE_FLASH_FLEX_RAM_SIZE (2048)
    /* @brief Has 0x00 Read 1s Block command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_BLOCK_CMD (0)
    /* @brief Has 0x01 Read 1s Section command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_SECTION_CMD (1)
    /* @brief Has 0x02 Program Check command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_CHECK_CMD (1)
    /* @brief Has 0x03 Read Resource command. */
    #define FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD (1)
    /* @brief Has 0x06 Program Longword command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_LONGWORD_CMD (1)
    /* @brief Has 0x07 Program Phrase command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_PHRASE_CMD (0)
    /* @brief Has 0x08 Erase Flash Block command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_FLASH_BLOCK_CMD (0)
    /* @brief Has 0x09 Erase Flash Sector command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_FLASH_SECTOR_CMD (1)
    /* @brief Has 0x0B Program Section command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_SECTION_CMD (1)
    /* @brief Has 0x40 Read 1s All Blocks command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_ALL_BLOCKS_CMD (0)
    /* @brief Has 0x41 Read Once command. */
    #define FSL_FEATURE_FLASH_HAS_READ_ONCE_CMD (1)
    /* @brief Has 0x43 Program Once command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_ONCE_CMD (1)
    /* @brief Has 0x44 Erase All Blocks command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_ALL_BLOCKS_CMD (0)
    /* @brief Has 0x45 Verify Backdoor Access Key command. */
    #define FSL_FEATURE_FLASH_HAS_VERIFY_BACKDOOR_ACCESS_KEY_CMD (1)
    /* @brief Has 0x46 Swap Control command. */
    #define FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD (0)
    /* @brief Has 0x80 Program Partition command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_PARTITION_CMD (1)
    /* @brief Has 0x81 Set FlexRAM Function command. */
    #define FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD (1)
    /* @brief P-Flash Erase/Read 1st all block command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Erase sector command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SECTOR_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Rrogram/Verify section command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SECTION_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Read resource command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_RESOURCE_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Program check command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_CHECK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Program check command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SWAP_CONTROL_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Erase/Read 1st all block command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM Erase sector command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_SECTOR_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM Rrogram/Verify section command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_SECTION_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM Read resource command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_RESOURCE_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM Program check command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_CHECK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief FlexNVM partition code 0000 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0000 (0x00008000)
    /* @brief FlexNVM partition code 0001 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0001 (0x00006000)
    /* @brief FlexNVM partition code 0010 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0010 (0x00004000)
    /* @brief FlexNVM partition code 0011 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0011 (0x00000000)
    /* @brief FlexNVM partition code 0100 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0100 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0101 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0101 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0110 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0110 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0111 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0111 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1000 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1000 (0x00000000)
    /* @brief FlexNVM partition code 1001 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1001 (0x00002000)
    /* @brief FlexNVM partition code 1010 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1010 (0x00004000)
    /* @brief FlexNVM partition code 1011 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1011 (0x00008000)
    /* @brief FlexNVM partition code 1100 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1100 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1101 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1101 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1110 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1110 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1111 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1111 (0x00008000)
    /* @brief Emulated eeprom size code 0000 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0000 (0xFFFF)
    /* @brief Emulated eeprom size code 0001 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0001 (0xFFFF)
    /* @brief Emulated eeprom size code 0010 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0010 (0xFFFF)
    /* @brief Emulated eeprom size code 0011 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0011 (0x0800)
    /* @brief Emulated eeprom size code 0100 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0100 (0x0400)
    /* @brief Emulated eeprom size code 0101 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0101 (0x0200)
    /* @brief Emulated eeprom size code 0110 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0110 (0x0100)
    /* @brief Emulated eeprom size code 0111 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0111 (0x0080)
    /* @brief Emulated eeprom size code 1000 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1000 (0x0040)
    /* @brief Emulated eeprom size code 1001 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1001 (0x0020)
    /* @brief Emulated eeprom size code 1010 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1010 (0xFFFF)
    /* @brief Emulated eeprom size code 1011 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1011 (0xFFFF)
    /* @brief Emulated eeprom size code 1100 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1100 (0xFFFF)
    /* @brief Emulated eeprom size code 1101 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1101 (0xFFFF)
    /* @brief Emulated eeprom size code 1110 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1110 (0xFFFF)
    /* @brief Emulated eeprom size code 1111 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1111 (0x0000)
#elif defined(CPU_MK20DN32VFM5) || defined(CPU_MK20DN32VMP5) || defined(CPU_MK20DN32VLH5) || defined(CPU_MK20DN32VFT5) || \
    defined(CPU_MK20DN32VLF5)
    /* @brief Is of type FTFA. */
    #define FSL_FEATURE_FLASH_IS_FTFA (0)
    /* @brief Is of type FTFE. */
    #define FSL_FEATURE_FLASH_IS_FTFE (0)
    /* @brief Is of type FTFL. */
    #define FSL_FEATURE_FLASH_IS_FTFL (1)
    /* @brief Has flags indicating the status of the FlexRAM (register bits FCNFG[EEERDY], FCNFG[RAMRDY] and FCNFG[PFLSH]). */
    #define FSL_FEATURE_FLASH_HAS_FLEX_RAM_FLAGS (1)
    /* @brief Has program flash swapping status flag (register bit FCNFG[SWAP]). */
    #define FSL_FEATURE_FLASH_HAS_PFLASH_SWAPPING_STATUS_FLAG (0)
    /* @brief Has EEPROM region protection (register FEPROT). */
    #define FSL_FEATURE_FLASH_HAS_EEROM_REGION_PROTECTION (1)
    /* @brief Has data flash region protection (register FDPROT). */
    #define FSL_FEATURE_FLASH_HAS_DATA_FLASH_REGION_PROTECTION (1)
    /* @brief Has flash access control (registers XACCHn, SACCHn, where n is a number, FACSS and FACSN). */
    #define FSL_FEATURE_FLASH_HAS_ACCESS_CONTROL (0)
    /* @brief Has flash cache control in FMC module. */
    #define FSL_FEATURE_FLASH_HAS_FMC_FLASH_CACHE_CONTROLS (1)
    /* @brief Has flash cache control in MCM module. */
    #define FSL_FEATURE_FLASH_HAS_MCM_FLASH_CACHE_CONTROLS (0)
    /* @brief P-Flash start address. */
    #define FSL_FEATURE_FLASH_PFLASH_START_ADDRESS (0x00000000)
    /* @brief P-Flash block count. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_COUNT (1)
    /* @brief P-Flash block size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE (32768)
    /* @brief P-Flash sector size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_SECTOR_SIZE (1024)
    /* @brief P-Flash write unit size. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_WRITE_UNIT_SIZE (4)
    /* @brief P-Flash data path width. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_DATA_PATH_WIDTH (4)
    /* @brief P-Flash block swap feature. */
    #define FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP (0)
    /* @brief Has FlexNVM memory. */
    #define FSL_FEATURE_FLASH_HAS_FLEX_NVM (0)
    /* @brief FlexNVM start address. (Valid only if FlexNVM is available.) */
    #define FSL_FEATURE_FLASH_FLEX_NVM_START_ADDRESS (0x00000000)
    /* @brief FlexNVM block count. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_COUNT (0)
    /* @brief FlexNVM block size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE (0)
    /* @brief FlexNVM sector size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SECTOR_SIZE (0)
    /* @brief FlexNVM write unit size. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_WRITE_UNIT_SIZE (0)
    /* @brief FlexNVM data path width. */
    #define FSL_FEATURE_FLASH_FLEX_BLOCK_DATA_PATH_WIDTH (0)
    /* @brief Has FlexRAM memory. */
    #define FSL_FEATURE_FLASH_HAS_FLEX_RAM (1)
    /* @brief FlexRAM start address. (Valid only if FlexRAM is available.) */
    #define FSL_FEATURE_FLASH_FLEX_RAM_START_ADDRESS (0x14000000)
    /* @brief FlexRAM size. */
    #define FSL_FEATURE_FLASH_FLEX_RAM_SIZE (2048)
    /* @brief Has 0x00 Read 1s Block command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_BLOCK_CMD (0)
    /* @brief Has 0x01 Read 1s Section command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_SECTION_CMD (1)
    /* @brief Has 0x02 Program Check command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_CHECK_CMD (1)
    /* @brief Has 0x03 Read Resource command. */
    #define FSL_FEATURE_FLASH_HAS_READ_RESOURCE_CMD (1)
    /* @brief Has 0x06 Program Longword command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_LONGWORD_CMD (1)
    /* @brief Has 0x07 Program Phrase command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_PHRASE_CMD (0)
    /* @brief Has 0x08 Erase Flash Block command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_FLASH_BLOCK_CMD (0)
    /* @brief Has 0x09 Erase Flash Sector command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_FLASH_SECTOR_CMD (1)
    /* @brief Has 0x0B Program Section command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_SECTION_CMD (1)
    /* @brief Has 0x40 Read 1s All Blocks command. */
    #define FSL_FEATURE_FLASH_HAS_READ_1S_ALL_BLOCKS_CMD (0)
    /* @brief Has 0x41 Read Once command. */
    #define FSL_FEATURE_FLASH_HAS_READ_ONCE_CMD (1)
    /* @brief Has 0x43 Program Once command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_ONCE_CMD (1)
    /* @brief Has 0x44 Erase All Blocks command. */
    #define FSL_FEATURE_FLASH_HAS_ERASE_ALL_BLOCKS_CMD (0)
    /* @brief Has 0x45 Verify Backdoor Access Key command. */
    #define FSL_FEATURE_FLASH_HAS_VERIFY_BACKDOOR_ACCESS_KEY_CMD (1)
    /* @brief Has 0x46 Swap Control command. */
    #define FSL_FEATURE_FLASH_HAS_SWAP_CONTROL_CMD (0)
    /* @brief Has 0x80 Program Partition command. */
    #define FSL_FEATURE_FLASH_HAS_PROGRAM_PARTITION_CMD (0)
    /* @brief Has 0x81 Set FlexRAM Function command. */
    #define FSL_FEATURE_FLASH_HAS_SET_FLEXRAM_FUNCTION_CMD (0)
    /* @brief P-Flash Erase/Read 1st all block command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_BLOCK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Erase sector command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SECTOR_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Rrogram/Verify section command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SECTION_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Read resource command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_RESOURCE_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Program check command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_CHECK_CMD_ADDRESS_ALIGMENT (4)
    /* @brief P-Flash Program check command address alignment. */
    #define FSL_FEATURE_FLASH_PFLASH_SWAP_CONTROL_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Erase/Read 1st all block command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Erase sector command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_SECTOR_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Rrogram/Verify section command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_SECTION_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Read resource command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_RESOURCE_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM Program check command address alignment. */
    #define FSL_FEATURE_FLASH_FLEX_NVM_CHECK_CMD_ADDRESS_ALIGMENT (0)
    /* @brief FlexNVM partition code 0000 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0000 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0001 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0001 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0010 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0010 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0011 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0011 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0100 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0100 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0101 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0101 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0110 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0110 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 0111 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_0111 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1000 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1000 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1001 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1001 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1010 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1010 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1011 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1011 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1100 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1100 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1101 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1101 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1110 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1110 (0xFFFFFFFF)
    /* @brief FlexNVM partition code 1111 mapping to data flash size in bytes (0xFFFFFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_DFLASH_SIZE_FOR_DEPART_1111 (0xFFFFFFFF)
    /* @brief Emulated eeprom size code 0000 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0000 (0xFFFF)
    /* @brief Emulated eeprom size code 0001 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0001 (0xFFFF)
    /* @brief Emulated eeprom size code 0010 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0010 (0xFFFF)
    /* @brief Emulated eeprom size code 0011 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0011 (0x0800)
    /* @brief Emulated eeprom size code 0100 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0100 (0x0400)
    /* @brief Emulated eeprom size code 0101 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0101 (0x0200)
    /* @brief Emulated eeprom size code 0110 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0110 (0x0100)
    /* @brief Emulated eeprom size code 0111 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_0111 (0x0080)
    /* @brief Emulated eeprom size code 1000 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1000 (0x0040)
    /* @brief Emulated eeprom size code 1001 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1001 (0x0020)
    /* @brief Emulated eeprom size code 1010 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1010 (0xFFFF)
    /* @brief Emulated eeprom size code 1011 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1011 (0xFFFF)
    /* @brief Emulated eeprom size code 1100 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1100 (0xFFFF)
    /* @brief Emulated eeprom size code 1101 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1101 (0xFFFF)
    /* @brief Emulated eeprom size code 1110 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1110 (0xFFFF)
    /* @brief Emulated eeprom size code 1111 mapping to emulated eeprom size in bytes (0xFFFF = reserved). */
    #define FSL_FEATURE_FLASH_FLEX_NVM_EEPROM_SIZE_FOR_EEESIZE_1111 (0x0000)
#endif /* defined(CPU_MK20DX128VFM5) || defined(CPU_MK20DX128VMP5) || defined(CPU_MK20DX128VLH5) || defined(CPU_MK20DX128VFT5) || \
    defined(CPU_MK20DX128VLF5) */

/* FTM module features */

/* @brief Number of channels. */
#define FSL_FEATURE_FTM_CHANNEL_COUNTn(x) \
    ((x) == FTM0 ? (8) : \
    ((x) == FTM1 ? (2) : (-1)))
/* @brief Has counter reset by the selected input capture event (register bits C0SC[ICRST], C1SC[ICRST], ...). */
#define FSL_FEATURE_FTM_HAS_COUNTER_RESET_BY_CAPTURE_EVENT (0)
/* @brief Enable pwm output for the module. */
#define FSL_FEATURE_FTM_HAS_ENABLE_PWM_OUTPUT (0)
/* @brief Has half-cycle reload for the module. */
#define FSL_FEATURE_FTM_HAS_HALFCYCLE_RELOAD (0)
/* @brief Has reload interrupt. */
#define FSL_FEATURE_FTM_HAS_RELOAD_INTERRUPT (0)
/* @brief Has reload initialization trigger. */
#define FSL_FEATURE_FTM_HAS_RELOAD_INITIALIZATION_TRIGGER (0)

/* I2C module features */

/* @brief Has System Management Bus support (registers SMB, A2, SLTL and SLTH). */
#define FSL_FEATURE_I2C_HAS_SMBUS (1)
/* @brief Maximum supported baud rate in kilobit per second. */
#define FSL_FEATURE_I2C_MAX_BAUD_KBPS (400)
/* @brief Is affected by errata with ID 6070 (repeat start cannot be generated if the F[MULT] bit field is set to a non-zero value). */
#define FSL_FEATURE_I2C_HAS_ERRATA_6070 (0)
/* @brief Has DMA support (register bit C1[DMAEN]). */
#define FSL_FEATURE_I2C_HAS_DMA_SUPPORT (1)
/* @brief Has I2C bus start and stop detection (register bits FLT[SSIE], FLT[STARTF] and FLT[STOPF]). */
#define FSL_FEATURE_I2C_HAS_START_STOP_DETECT (0)
/* @brief Has I2C bus stop detection (register bits FLT[STOPIE] and FLT[STOPF]). */
#define FSL_FEATURE_I2C_HAS_STOP_DETECT (0)
/* @brief Has I2C bus stop hold off (register bit FLT[SHEN]). */
#define FSL_FEATURE_I2C_HAS_STOP_HOLD_OFF (0)
/* @brief Maximum width of the glitch filter in number of bus clocks. */
#define FSL_FEATURE_I2C_MAX_GLITCH_FILTER_WIDTH (31)
/* @brief Has control of the drive capability of the I2C pins. */
#define FSL_FEATURE_I2C_HAS_HIGH_DRIVE_SELECTION (1)
/* @brief Has double buffering support (register S2). */
#define FSL_FEATURE_I2C_HAS_DOUBLE_BUFFERING (0)

/* SAI module features */

/* @brief Receive/transmit FIFO size in item count (register bit fields TCSR[FRDE], TCSR[FRIE], TCSR[FRF], TCR1[TFW], RCSR[FRDE], RCSR[FRIE], RCSR[FRF], RCR1[RFW], registers TFRn, RFRn). */
#define FSL_FEATURE_SAI_FIFO_COUNT (8)
/* @brief Receive/transmit channel number (register bit fields TCR3[TCE], RCR3[RCE], registers TDRn and RDRn). */
#define FSL_FEATURE_SAI_CHANNEL_COUNT (2)
/* @brief Maximum words per frame (register bit fields TCR3[WDFL], TCR4[FRSZ], TMR[TWM], RCR3[WDFL], RCR4[FRSZ], RMR[RWM]). */
#define FSL_FEATURE_SAI_MAX_WORDS_PER_FRAME (32)
/* @brief Has support of combining multiple data channel FIFOs into single channel FIFO (register bit fields TCR3[CFR], TCR4[FCOMB], TFR0[WCP], TFR1[WCP], RCR3[CFR], RCR4[FCOMB], RFR0[RCP], RFR1[RCP]). */
#define FSL_FEATURE_SAI_HAS_FIFO_COMBINE_MODE (0)
/* @brief Has packing of 8-bit and 16-bit data into each 32-bit FIFO word (register bit fields TCR4[FPACK], RCR4[FPACK]). */
#define FSL_FEATURE_SAI_HAS_FIFO_PACKING (0)
/* @brief Configures when the SAI will continue transmitting after a FIFO error has been detected (register bit fields TCR4[FCONT], RCR4[FCONT]). */
#define FSL_FEATURE_SAI_HAS_FIFO_FUNCTION_AFTER_ERROR (0)
/* @brief Configures if the frame sync is generated internally, a frame sync is only generated when the FIFO warning flag is clear or continuously (register bit fields TCR4[ONDEM], RCR4[ONDEM]). */
#define FSL_FEATURE_SAI_HAS_ON_DEMAND_MODE (0)
/* @brief Simplified bit clock source and asynchronous/synchronous mode selection (register bit fields TCR2[CLKMODE], RCR2[CLKMODE]), in comparison with the exclusively implemented TCR2[SYNC,BCS,BCI,MSEL], RCR2[SYNC,BCS,BCI,MSEL]. */
#define FSL_FEATURE_SAI_HAS_CLOCKING_MODE (0)
/* @brief Has register for configuration of the MCLK divide ratio (register bit fields MDR[FRACT], MDR[DIVIDE]). */
#define FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER (1)
/* @brief Ihe interrupt source number */
#define FSL_FEATURE_SAI_INT_SOURCE_NUM (2)
/* @brief Has register of MCR. */
#define FSL_FEATURE_SAI_HAS_MCR (1)
/* @brief Has register of MDR */
#define FSL_FEATURE_SAI_HAS_MDR (1)

/* LLWU module features */

#if defined(CPU_MK20DX128VFM5) || defined(CPU_MK20DN128VFM5) || defined(CPU_MK20DX64VFM5) || defined(CPU_MK20DN64VFM5) || \
    defined(CPU_MK20DX32VFM5) || defined(CPU_MK20DN32VFM5)
    /* @brief Maximum number of pins connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN (16)
    /* @brief Has pins 8-15 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_EXTERNAL_PIN_GROUP2 (1)
    /* @brief Maximum number of internal modules connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE (8)
    /* @brief Number of digital filters. */
    #define FSL_FEATURE_LLWU_HAS_PIN_FILTER (2)
    /* @brief Has MF5 register. */
    #define FSL_FEATURE_LLWU_HAS_MF (0)
    /* @brief Has PF register. */
    #define FSL_FEATURE_LLWU_HAS_PF (0)
    /* @brief Has possibility to enable reset in low leakage power mode and enable digital filter for RESET pin (register LLWU_RST). */
    #define FSL_FEATURE_LLWU_HAS_RESET_ENABLE (1)
    /* @brief Has external pin 0 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN0 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN0_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN0_GPIO_PIN (0)
    /* @brief Has external pin 1 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN1 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN1_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN1_GPIO_PIN (0)
    /* @brief Has external pin 2 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN2 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN2_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN2_GPIO_PIN (0)
    /* @brief Has external pin 3 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN3 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN3_GPIO_IDX (GPIOA_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN3_GPIO_PIN (4)
    /* @brief Has external pin 4 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN4 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN4_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN4_GPIO_PIN (0)
    /* @brief Has external pin 5 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN5 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN5_GPIO_IDX (GPIOB_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN5_GPIO_PIN (0)
    /* @brief Has external pin 6 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN6 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN6_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN6_GPIO_PIN (1)
    /* @brief Has external pin 7 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN7 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN7_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN7_GPIO_PIN (3)
    /* @brief Has external pin 8 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN8 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN8_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN8_GPIO_PIN (4)
    /* @brief Has external pin 9 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN9 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN9_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN9_GPIO_PIN (5)
    /* @brief Has external pin 10 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN10 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN10_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN10_GPIO_PIN (6)
    /* @brief Has external pin 11 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN11 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN11_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN11_GPIO_PIN (0)
    /* @brief Has external pin 12 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN12 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN12_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN12_GPIO_PIN (0)
    /* @brief Has external pin 13 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN13 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN13_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN13_GPIO_PIN (0)
    /* @brief Has external pin 14 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN14 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN14_GPIO_IDX (GPIOD_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN14_GPIO_PIN (4)
    /* @brief Has external pin 15 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN15 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN15_GPIO_IDX (GPIOD_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN15_GPIO_PIN (6)
    /* @brief Has external pin 16 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN16 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN16_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN16_GPIO_PIN (0)
    /* @brief Has external pin 17 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN17 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN17_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN17_GPIO_PIN (0)
    /* @brief Has external pin 18 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN18 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN18_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN18_GPIO_PIN (0)
    /* @brief Has external pin 19 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN19 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN19_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN19_GPIO_PIN (0)
    /* @brief Has external pin 20 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN20 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN20_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN20_GPIO_PIN (0)
    /* @brief Has external pin 21 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN21 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN21_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN21_GPIO_PIN (0)
    /* @brief Has external pin 22 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN22 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN22_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN22_GPIO_PIN (0)
    /* @brief Has external pin 23 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN23 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN23_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN23_GPIO_PIN (0)
    /* @brief Has external pin 24 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN24 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN24_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN24_GPIO_PIN (0)
    /* @brief Has external pin 25 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN25 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN25_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN25_GPIO_PIN (0)
    /* @brief Has external pin 26 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN26 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN26_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN26_GPIO_PIN (0)
    /* @brief Has external pin 27 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN27 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN27_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN27_GPIO_PIN (0)
    /* @brief Has external pin 28 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN28 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN28_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN28_GPIO_PIN (0)
    /* @brief Has external pin 29 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN29 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN29_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN29_GPIO_PIN (0)
    /* @brief Has external pin 30 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN30 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN30_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN30_GPIO_PIN (0)
    /* @brief Has external pin 31 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN31 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN31_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN31_GPIO_PIN (0)
    /* @brief Has internal module 0 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE0 (1)
    /* @brief Has internal module 1 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE1 (1)
    /* @brief Has internal module 2 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE2 (1)
    /* @brief Has internal module 3 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE3 (0)
    /* @brief Has internal module 4 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE4 (1)
    /* @brief Has internal module 5 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE5 (1)
    /* @brief Has internal module 6 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE6 (0)
    /* @brief Has internal module 7 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE7 (1)
    /* @brief Has Version ID Register (LLWU_VERID). */
    #define FSL_FEATURE_LLWU_HAS_VERID (0)
    /* @brief Has Parameter Register (LLWU_PARAM). */
    #define FSL_FEATURE_LLWU_HAS_PARAM (0)
    /* @brief Width of registers of the LLWU. */
    #define FSL_FEATURE_LLWU_REG_BITWIDTH (8)
    /* @brief Has DMA Enable register (LLWU_DE). */
    #define FSL_FEATURE_LLWU_HAS_DMA_ENABLE_REG (0)
#elif defined(CPU_MK20DX128VMP5) || defined(CPU_MK20DN128VMP5) || defined(CPU_MK20DX64VMP5) || defined(CPU_MK20DN64VMP5) || \
    defined(CPU_MK20DX32VMP5) || defined(CPU_MK20DN32VMP5) || defined(CPU_MK20DX128VLH5) || defined(CPU_MK20DN128VLH5) || \
    defined(CPU_MK20DX64VLH5) || defined(CPU_MK20DN64VLH5) || defined(CPU_MK20DX32VLH5) || defined(CPU_MK20DN32VLH5)
    /* @brief Maximum number of pins connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN (16)
    /* @brief Has pins 8-15 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_EXTERNAL_PIN_GROUP2 (1)
    /* @brief Maximum number of internal modules connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE (8)
    /* @brief Number of digital filters. */
    #define FSL_FEATURE_LLWU_HAS_PIN_FILTER (2)
    /* @brief Has MF5 register. */
    #define FSL_FEATURE_LLWU_HAS_MF (0)
    /* @brief Has PF register. */
    #define FSL_FEATURE_LLWU_HAS_PF (0)
    /* @brief Has possibility to enable reset in low leakage power mode and enable digital filter for RESET pin (register LLWU_RST). */
    #define FSL_FEATURE_LLWU_HAS_RESET_ENABLE (1)
    /* @brief Has external pin 0 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN0 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN0_GPIO_IDX (GPIOE_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN0_GPIO_PIN (1)
    /* @brief Has external pin 1 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN1 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN1_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN1_GPIO_PIN (0)
    /* @brief Has external pin 2 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN2 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN2_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN2_GPIO_PIN (0)
    /* @brief Has external pin 3 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN3 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN3_GPIO_IDX (GPIOA_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN3_GPIO_PIN (4)
    /* @brief Has external pin 4 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN4 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN4_GPIO_IDX (GPIOA_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN4_GPIO_PIN (13)
    /* @brief Has external pin 5 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN5 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN5_GPIO_IDX (GPIOB_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN5_GPIO_PIN (0)
    /* @brief Has external pin 6 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN6 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN6_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN6_GPIO_PIN (1)
    /* @brief Has external pin 7 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN7 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN7_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN7_GPIO_PIN (3)
    /* @brief Has external pin 8 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN8 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN8_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN8_GPIO_PIN (4)
    /* @brief Has external pin 9 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN9 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN9_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN9_GPIO_PIN (5)
    /* @brief Has external pin 10 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN10 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN10_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN10_GPIO_PIN (6)
    /* @brief Has external pin 11 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN11 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN11_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN11_GPIO_PIN (11)
    /* @brief Has external pin 12 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN12 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN12_GPIO_IDX (GPIOD_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN12_GPIO_PIN (0)
    /* @brief Has external pin 13 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN13 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN13_GPIO_IDX (GPIOD_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN13_GPIO_PIN (2)
    /* @brief Has external pin 14 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN14 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN14_GPIO_IDX (GPIOD_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN14_GPIO_PIN (4)
    /* @brief Has external pin 15 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN15 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN15_GPIO_IDX (GPIOD_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN15_GPIO_PIN (6)
    /* @brief Has external pin 16 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN16 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN16_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN16_GPIO_PIN (0)
    /* @brief Has external pin 17 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN17 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN17_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN17_GPIO_PIN (0)
    /* @brief Has external pin 18 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN18 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN18_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN18_GPIO_PIN (0)
    /* @brief Has external pin 19 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN19 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN19_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN19_GPIO_PIN (0)
    /* @brief Has external pin 20 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN20 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN20_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN20_GPIO_PIN (0)
    /* @brief Has external pin 21 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN21 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN21_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN21_GPIO_PIN (0)
    /* @brief Has external pin 22 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN22 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN22_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN22_GPIO_PIN (0)
    /* @brief Has external pin 23 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN23 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN23_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN23_GPIO_PIN (0)
    /* @brief Has external pin 24 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN24 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN24_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN24_GPIO_PIN (0)
    /* @brief Has external pin 25 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN25 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN25_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN25_GPIO_PIN (0)
    /* @brief Has external pin 26 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN26 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN26_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN26_GPIO_PIN (0)
    /* @brief Has external pin 27 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN27 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN27_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN27_GPIO_PIN (0)
    /* @brief Has external pin 28 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN28 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN28_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN28_GPIO_PIN (0)
    /* @brief Has external pin 29 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN29 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN29_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN29_GPIO_PIN (0)
    /* @brief Has external pin 30 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN30 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN30_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN30_GPIO_PIN (0)
    /* @brief Has external pin 31 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN31 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN31_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN31_GPIO_PIN (0)
    /* @brief Has internal module 0 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE0 (1)
    /* @brief Has internal module 1 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE1 (1)
    /* @brief Has internal module 2 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE2 (1)
    /* @brief Has internal module 3 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE3 (0)
    /* @brief Has internal module 4 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE4 (1)
    /* @brief Has internal module 5 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE5 (1)
    /* @brief Has internal module 6 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE6 (0)
    /* @brief Has internal module 7 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE7 (1)
    /* @brief Has Version ID Register (LLWU_VERID). */
    #define FSL_FEATURE_LLWU_HAS_VERID (0)
    /* @brief Has Parameter Register (LLWU_PARAM). */
    #define FSL_FEATURE_LLWU_HAS_PARAM (0)
    /* @brief Width of registers of the LLWU. */
    #define FSL_FEATURE_LLWU_REG_BITWIDTH (8)
    /* @brief Has DMA Enable register (LLWU_DE). */
    #define FSL_FEATURE_LLWU_HAS_DMA_ENABLE_REG (0)
#elif defined(CPU_MK20DX128VFT5) || defined(CPU_MK20DN128VFT5) || defined(CPU_MK20DX64VFT5) || defined(CPU_MK20DN64VFT5) || \
    defined(CPU_MK20DX32VFT5) || defined(CPU_MK20DN32VFT5) || defined(CPU_MK20DX128VLF5) || defined(CPU_MK20DN128VLF5) || \
    defined(CPU_MK20DX64VLF5) || defined(CPU_MK20DN64VLF5) || defined(CPU_MK20DX32VLF5) || defined(CPU_MK20DN32VLF5)
    /* @brief Maximum number of pins connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN (16)
    /* @brief Has pins 8-15 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_EXTERNAL_PIN_GROUP2 (1)
    /* @brief Maximum number of internal modules connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE (8)
    /* @brief Number of digital filters. */
    #define FSL_FEATURE_LLWU_HAS_PIN_FILTER (2)
    /* @brief Has MF5 register. */
    #define FSL_FEATURE_LLWU_HAS_MF (0)
    /* @brief Has PF register. */
    #define FSL_FEATURE_LLWU_HAS_PF (0)
    /* @brief Has possibility to enable reset in low leakage power mode and enable digital filter for RESET pin (register LLWU_RST). */
    #define FSL_FEATURE_LLWU_HAS_RESET_ENABLE (1)
    /* @brief Has external pin 0 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN0 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN0_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN0_GPIO_PIN (0)
    /* @brief Has external pin 1 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN1 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN1_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN1_GPIO_PIN (0)
    /* @brief Has external pin 2 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN2 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN2_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN2_GPIO_PIN (0)
    /* @brief Has external pin 3 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN3 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN3_GPIO_IDX (GPIOA_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN3_GPIO_PIN (4)
    /* @brief Has external pin 4 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN4 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN4_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN4_GPIO_PIN (0)
    /* @brief Has external pin 5 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN5 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN5_GPIO_IDX (GPIOB_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN5_GPIO_PIN (0)
    /* @brief Has external pin 6 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN6 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN6_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN6_GPIO_PIN (1)
    /* @brief Has external pin 7 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN7 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN7_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN7_GPIO_PIN (3)
    /* @brief Has external pin 8 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN8 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN8_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN8_GPIO_PIN (4)
    /* @brief Has external pin 9 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN9 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN9_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN9_GPIO_PIN (5)
    /* @brief Has external pin 10 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN10 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN10_GPIO_IDX (GPIOC_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN10_GPIO_PIN (6)
    /* @brief Has external pin 11 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN11 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN11_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN11_GPIO_PIN (0)
    /* @brief Has external pin 12 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN12 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN12_GPIO_IDX (GPIOD_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN12_GPIO_PIN (0)
    /* @brief Has external pin 13 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN13 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN13_GPIO_IDX (GPIOD_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN13_GPIO_PIN (2)
    /* @brief Has external pin 14 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN14 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN14_GPIO_IDX (GPIOD_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN14_GPIO_PIN (4)
    /* @brief Has external pin 15 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN15 (1)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN15_GPIO_IDX (GPIOD_IDX)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN15_GPIO_PIN (6)
    /* @brief Has external pin 16 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN16 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN16_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN16_GPIO_PIN (0)
    /* @brief Has external pin 17 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN17 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN17_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN17_GPIO_PIN (0)
    /* @brief Has external pin 18 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN18 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN18_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN18_GPIO_PIN (0)
    /* @brief Has external pin 19 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN19 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN19_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN19_GPIO_PIN (0)
    /* @brief Has external pin 20 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN20 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN20_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN20_GPIO_PIN (0)
    /* @brief Has external pin 21 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN21 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN21_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN21_GPIO_PIN (0)
    /* @brief Has external pin 22 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN22 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN22_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN22_GPIO_PIN (0)
    /* @brief Has external pin 23 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN23 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN23_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN23_GPIO_PIN (0)
    /* @brief Has external pin 24 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN24 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN24_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN24_GPIO_PIN (0)
    /* @brief Has external pin 25 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN25 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN25_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN25_GPIO_PIN (0)
    /* @brief Has external pin 26 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN26 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN26_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN26_GPIO_PIN (0)
    /* @brief Has external pin 27 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN27 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN27_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN27_GPIO_PIN (0)
    /* @brief Has external pin 28 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN28 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN28_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN28_GPIO_PIN (0)
    /* @brief Has external pin 29 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN29 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN29_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN29_GPIO_PIN (0)
    /* @brief Has external pin 30 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN30 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN30_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN30_GPIO_PIN (0)
    /* @brief Has external pin 31 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN31 (0)
    /* @brief Index of port of external pin. */
    #define FSL_FEATURE_LLWU_PIN31_GPIO_IDX (0)
    /* @brief Number of external pin port on specified port. */
    #define FSL_FEATURE_LLWU_PIN31_GPIO_PIN (0)
    /* @brief Has internal module 0 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE0 (1)
    /* @brief Has internal module 1 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE1 (1)
    /* @brief Has internal module 2 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE2 (1)
    /* @brief Has internal module 3 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE3 (0)
    /* @brief Has internal module 4 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE4 (1)
    /* @brief Has internal module 5 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE5 (1)
    /* @brief Has internal module 6 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE6 (0)
    /* @brief Has internal module 7 connected to LLWU device. */
    #define FSL_FEATURE_LLWU_HAS_INTERNAL_MODULE7 (1)
    /* @brief Has Version ID Register (LLWU_VERID). */
    #define FSL_FEATURE_LLWU_HAS_VERID (0)
    /* @brief Has Parameter Register (LLWU_PARAM). */
    #define FSL_FEATURE_LLWU_HAS_PARAM (0)
    /* @brief Width of registers of the LLWU. */
    #define FSL_FEATURE_LLWU_REG_BITWIDTH (8)
    /* @brief Has DMA Enable register (LLWU_DE). */
    #define FSL_FEATURE_LLWU_HAS_DMA_ENABLE_REG (0)
#endif /* defined(CPU_MK20DX128VFM5) || defined(CPU_MK20DN128VFM5) || defined(CPU_MK20DX64VFM5) || defined(CPU_MK20DN64VFM5) || \
    defined(CPU_MK20DX32VFM5) || defined(CPU_MK20DN32VFM5) */

/* LPTMR module features */

/* No feature definitions */

/* MCG module features */

/* @brief PRDIV base value (divider of register bit field [PRDIV] zero value). */
#define FSL_FEATURE_MCG_PLL_PRDIV_BASE (1)
/* @brief Maximum PLL external reference divider value (max. value of register bit field C5[PRVDIV]). */
#define FSL_FEATURE_MCG_PLL_PRDIV_MAX (24)
/* @brief VCO divider base value (multiply factor of register bit field C6[VDIV] zero value). */
#define FSL_FEATURE_MCG_PLL_VDIV_BASE (24)
/* @brief PLL reference clock low range. OSCCLK/PLL_R. */
#define FSL_FEATURE_MCG_PLL_REF_MIN (2000000)
/* @brief PLL reference clock high range. OSCCLK/PLL_R. */
#define FSL_FEATURE_MCG_PLL_REF_MAX (4000000)
/* @brief The PLL clock is divided by 2 before VCO divider. */
#define FSL_FEATURE_MCG_HAS_PLL_INTERNAL_DIV (0)
/* @brief FRDIV supports 1280. */
#define FSL_FEATURE_MCG_FRDIV_SUPPORT_1280 (1)
/* @brief FRDIV supports 1536. */
#define FSL_FEATURE_MCG_FRDIV_SUPPORT_1536 (1)
/* @brief MCGFFCLK divider. */
#define FSL_FEATURE_MCG_FFCLK_DIV (1)
/* @brief Is PLL clock divided by 2 before MCG PLL/FLL clock selection in the SIM module. */
#define FSL_FEATURE_MCG_HAS_PLL_EXTRA_DIV (0)
/* @brief Has 32kHz RTC external reference clock (register bits C8[LOCS1], C8[CME1], C8[LOCRE1] and RTC module are present). */
#define FSL_FEATURE_MCG_HAS_RTC_32K (1)
/* @brief Has PLL1 external reference clock (registers C10, C11, C12, S2). */
#define FSL_FEATURE_MCG_HAS_PLL1 (0)
/* @brief Has 48MHz internal oscillator. */
#define FSL_FEATURE_MCG_HAS_IRC_48M (0)
/* @brief Has OSC1 external oscillator (registers C10, C11, C12, S2). */
#define FSL_FEATURE_MCG_HAS_OSC1 (0)
/* @brief Has fast internal reference clock fine trim (register bit C2[FCFTRIM]). */
#define FSL_FEATURE_MCG_HAS_FCFTRIM (0)
/* @brief Has PLL loss of lock reset (register bit C8[LOLRE]). */
#define FSL_FEATURE_MCG_HAS_LOLRE (1)
/* @brief Has MCG OSC clock selection (register bit C7[OSCSEL]). */
#define FSL_FEATURE_MCG_USE_OSCSEL (1)
/* @brief Has PLL external reference selection (register bits C5[PLLREFSEL0] and C11[PLLREFSEL1]). */
#define FSL_FEATURE_MCG_USE_PLLREFSEL (0)
/* @brief TBD */
#define FSL_FEATURE_MCG_USE_SYSTEM_CLOCK (0)
/* @brief Has phase-locked loop (PLL) (register C5 and bits C6[VDIV], C6[PLLS], C6[LOLIE0], S[PLLST], S[LOCK0], S[LOLS]). */
#define FSL_FEATURE_MCG_HAS_PLL (1)
/* @brief Has phase-locked loop (PLL) PRDIV (register C5[PRDIV]. */
#define FSL_FEATURE_MCG_HAS_PLL_PRDIV (1)
/* @brief Has phase-locked loop (PLL) VDIV (register C6[VDIV]. */
#define FSL_FEATURE_MCG_HAS_PLL_VDIV (1)
/* @brief PLL/OSC related register bit fields have PLL/OSC index in their name. */
#define FSL_FEATURE_MCG_HAS_PLL_OSC_INDEX (1)
/* @brief Has frequency-locked loop (FLL) (register ATCVH, ATCVL and bits C1[IREFS], C1[FRDIV]). */
#define FSL_FEATURE_MCG_HAS_FLL (1)
/* @brief Has PLL external to MCG (C9[PLL_CME], C9[PLL_LOCRE], C9[EXT_PLL_LOCS]). */
#define FSL_FEATURE_MCG_HAS_EXTERNAL_PLL (0)
/* @brief Has crystal oscillator or external reference clock low power controls (register bits C2[HGO], C2[RANGE]). */
#define FSL_FEATURE_MCG_HAS_EXT_REF_LOW_POWER_CONTROL (1)
/* @brief Has PLL/FLL selection as MCG output (register bit C6[PLLS]). */
#define FSL_FEATURE_MCG_HAS_PLL_FLL_SELECTION (1)
/* @brief Has PLL output selection (PLL0/PLL1, PLL/external PLL) (register bit C11[PLLCS]). */
#define FSL_FEATURE_MCG_HAS_PLL_OUTPUT_SELECTION (0)
/* @brief Has automatic trim machine (registers ATCVH, ATCVL and bits SC[ATMF], SC[ATMS], SC[ATME]). */
#define FSL_FEATURE_MCG_HAS_AUTO_TRIM_MACHINE (1)
/* @brief Has external clock monitor (register bit C6[CME]). */
#define FSL_FEATURE_MCG_HAS_EXTERNAL_CLOCK_MONITOR (1)
/* @brief Has low frequency internal reference clock (IRC) (registers LTRIMRNG, LFRIM, LSTRIM and bit MC[LIRC_DIV2]). */
#define FSL_FEATURE_MCG_HAS_LOW_FREQ_IRC (0)
/* @brief Has high frequency internal reference clock (IRC) (registers HCTRIM, HTTRIM, HFTRIM and bit MC[HIRCEN]). */
#define FSL_FEATURE_MCG_HAS_HIGH_FREQ_IRC (0)
/* @brief Has PEI mode or PBI mode. */
#define FSL_FEATURE_MCG_HAS_PLL_INTERNAL_MODE (0)
/* @brief Reset clock mode is BLPI. */
#define FSL_FEATURE_MCG_RESET_IS_BLPI (0)

/* interrupt module features */

/* @brief Lowest interrupt request number. */
#define FSL_FEATURE_INTERRUPT_IRQ_MIN (-14)
/* @brief Highest interrupt request number. */
#define FSL_FEATURE_INTERRUPT_IRQ_MAX (105)

/* OSC module features */

/* @brief Has OSC1 external oscillator. */
#define FSL_FEATURE_OSC_HAS_OSC1 (0)
/* @brief Has OSC0 external oscillator. */
#define FSL_FEATURE_OSC_HAS_OSC0 (1)
/* @brief Has OSC external oscillator (without index). */
#define FSL_FEATURE_OSC_HAS_OSC (0)
/* @brief Number of OSC external oscillators. */
#define FSL_FEATURE_OSC_OSC_COUNT (1)
/* @brief Has external reference clock divider (register bit field DIV[ERPS]). */
#define FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER (0)

/* PDB module features */

/* @brief Define the count of supporting ADC pre-trigger for each channel. */
#define FSL_FEATURE_PDB_ADC_PRE_CHANNEL_COUNT (2)
/* @brief Has DAC support. */
#define FSL_FEATURE_PDB_HAS_DAC (0)
/* @brief Has shared interrupt handler (has not individual interrupt handler for each channel). */
#define FSL_FEATURE_PDB_HAS_SHARED_IRQ_HANDLER (0)

/* PIT module features */

/* @brief Number of channels (related to number of registers LDVALn, CVALn, TCTRLn, TFLGn). */
#define FSL_FEATURE_PIT_TIMER_COUNT (4)
/* @brief Has lifetime timer (related to existence of registers LTMR64L and LTMR64H). */
#define FSL_FEATURE_PIT_HAS_LIFETIME_TIMER (0)
/* @brief Has chain mode (related to existence of register bit field TCTRLn[CHN]). */
#define FSL_FEATURE_PIT_HAS_CHAIN_MODE (0)
/* @brief Has shared interrupt handler (has not individual interrupt handler for each channel). */
#define FSL_FEATURE_PIT_HAS_SHARED_IRQ_HANDLER (0)

/* PMC module features */

/* @brief Has Bandgap Enable In VLPx Operation support. */
#define FSL_FEATURE_PMC_HAS_BGEN (0)
/* @brief Has Bandgap Buffer Enable. */
#define FSL_FEATURE_PMC_HAS_BGBE (1)
/* @brief Has Bandgap Buffer Drive Select. */
#define FSL_FEATURE_PMC_HAS_BGBDS (0)
/* @brief Has Low-Voltage Detect Voltage Select support. */
#define FSL_FEATURE_PMC_HAS_LVDV (1)
/* @brief Has Low-Voltage Warning Voltage Select support. */
#define FSL_FEATURE_PMC_HAS_LVWV (1)
/* @brief Has LPO. */
#define FSL_FEATURE_PMC_HAS_LPO (0)
/* @brief Has VLPx option PMC_REGSC[VLPO]. */
#define FSL_FEATURE_PMC_HAS_VLPO (0)
/* @brief Has acknowledge isolation support. */
#define FSL_FEATURE_PMC_HAS_ACKISO (1)
/* @brief Has Regulator In Full Performance Mode Status Bit PMC_REGSC[REGFPM]. */
#define FSL_FEATURE_PMC_HAS_REGFPM (0)
/* @brief Has Regulator In Run Regulation Status Bit PMC_REGSC[REGONS]. */
#define FSL_FEATURE_PMC_HAS_REGONS (1)
/* @brief Has PMC_HVDSC1. */
#define FSL_FEATURE_PMC_HAS_HVDSC1 (0)
/* @brief Has PMC_PARAM. */
#define FSL_FEATURE_PMC_HAS_PARAM (0)
/* @brief Has PMC_VERID. */
#define FSL_FEATURE_PMC_HAS_VERID (0)

/* PORT module features */

/* @brief Has control lock (register bit PCR[LK]). */
#define FSL_FEATURE_PORT_HAS_PIN_CONTROL_LOCK (1)
/* @brief Has open drain control (register bit PCR[ODE]). */
#define FSL_FEATURE_PORT_HAS_OPEN_DRAIN (1)
/* @brief Has digital filter (registers DFER, DFCR and DFWR). */
#define FSL_FEATURE_PORT_HAS_DIGITAL_FILTER (1)
/* @brief Has DMA request (register bit field PCR[IRQC] values). */
#define FSL_FEATURE_PORT_HAS_DMA_REQUEST (1)
/* @brief Has pull resistor selection available. */
#define FSL_FEATURE_PORT_HAS_PULL_SELECTION (1)
/* @brief Has pull resistor enable (register bit PCR[PE]). */
#define FSL_FEATURE_PORT_HAS_PULL_ENABLE (1)
/* @brief Has slew rate control (register bit PCR[SRE]). */
#define FSL_FEATURE_PORT_HAS_SLEW_RATE (1)
/* @brief Has passive filter (register bit field PCR[PFE]). */
#define FSL_FEATURE_PORT_HAS_PASSIVE_FILTER (1)
/* @brief Has drive strength control (register bit PCR[DSE]). */
#define FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH (1)
/* @brief Has separate drive strength register (HDRVE). */
#define FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH_REGISTER (0)
/* @brief Has glitch filter (register IOFLT). */
#define FSL_FEATURE_PORT_HAS_GLITCH_FILTER (0)
/* @brief Defines width of PCR[MUX] field. */
#define FSL_FEATURE_PORT_PCR_MUX_WIDTH (3)
/* @brief Has dedicated interrupt vector. */
#define FSL_FEATURE_PORT_HAS_INTERRUPT_VECTOR (0)
/* @brief Defines whether PCR[IRQC] bit-field has flag states. */
#define FSL_FEATURE_PORT_HAS_IRQC_FLAG (0)
/* @brief Defines whether PCR[IRQC] bit-field has trigger states. */
#define FSL_FEATURE_PORT_HAS_IRQC_TRIGGER (0)

/* GPIO module features */

/* @brief Has fast (single cycle) access capability via a dedicated memory region. */
#define FSL_FEATURE_GPIO_HAS_FAST_GPIO (0)
/* @brief Has port input disable register (PIDR). */
#define FSL_FEATURE_GPIO_HAS_INPUT_DISABLE (0)
/* @brief Has dedicated interrupt vector. */
#define FSL_FEATURE_GPIO_HAS_PORT_INTERRUPT_VECTOR (1)

/* RCM module features */

/* @brief Has Loss-of-Lock Reset support. */
#define FSL_FEATURE_RCM_HAS_LOL (1)
/* @brief Has Loss-of-Clock Reset support. */
#define FSL_FEATURE_RCM_HAS_LOC (1)
/* @brief Has JTAG generated Reset support. */
#define FSL_FEATURE_RCM_HAS_JTAG (1)
/* @brief Has EzPort generated Reset support. */
#define FSL_FEATURE_RCM_HAS_EZPORT (1)
/* @brief Has bit-field indicating EZP_MS_B pin state during last reset. */
#define FSL_FEATURE_RCM_HAS_EZPMS (1)
/* @brief Has boot ROM configuration, MR[BOOTROM], FM[FORCEROM] */
#define FSL_FEATURE_RCM_HAS_BOOTROM (0)
/* @brief Has sticky system reset status register RCM_SSRS0 and RCM_SSRS1. */
#define FSL_FEATURE_RCM_HAS_SSRS (0)
/* @brief Has Version ID Register (RCM_VERID). */
#define FSL_FEATURE_RCM_HAS_VERID (0)
/* @brief Has Parameter Register (RCM_PARAM). */
#define FSL_FEATURE_RCM_HAS_PARAM (0)
/* @brief Has Reset Interrupt Enable Register RCM_SRIE. */
#define FSL_FEATURE_RCM_HAS_SRIE (0)
/* @brief Width of registers of the RCM. */
#define FSL_FEATURE_RCM_REG_WIDTH (8)
/* @brief Has Core 1 generated Reset support RCM_SRS[CORE1] */
#define FSL_FEATURE_RCM_HAS_CORE1 (0)
/* @brief Has MDM-AP system reset support RCM_SRS1[MDM_AP] */
#define FSL_FEATURE_RCM_HAS_MDM_AP (1)
/* @brief Has wakeup reset feature. Register bit SRS[WAKEUP]. */
#define FSL_FEATURE_RCM_HAS_WAKEUP (1)

/* RTC module features */

/* @brief Has wakeup pin. */
#define FSL_FEATURE_RTC_HAS_WAKEUP_PIN (0)
/* @brief Has wakeup pin selection (bit field CR[WPS]). */
#define FSL_FEATURE_RTC_HAS_WAKEUP_PIN_SELECTION (0)
/* @brief Has low power features (registers MER, MCLR and MCHR). */
#define FSL_FEATURE_RTC_HAS_MONOTONIC (0)
/* @brief Has read/write access control (registers WAR and RAR). */
#define FSL_FEATURE_RTC_HAS_ACCESS_CONTROL (1)
/* @brief Has security features (registers TTSR, MER, MCLR and MCHR). */
#define FSL_FEATURE_RTC_HAS_SECURITY (0)
/* @brief Has RTC_CLKIN available. */
#define FSL_FEATURE_RTC_HAS_RTC_CLKIN (0)
/* @brief Has prescaler adjust for LPO. */
#define FSL_FEATURE_RTC_HAS_LPO_ADJUST (0)
/* @brief Has Clock Pin Enable field. */
#define FSL_FEATURE_RTC_HAS_CPE (0)
/* @brief Has Timer Seconds Interrupt Configuration field. */
#define FSL_FEATURE_RTC_HAS_TSIC (0)
/* @brief Has OSC capacitor setting RTC_CR[SC2P ~ SC16P] */
#define FSL_FEATURE_RTC_HAS_OSC_SCXP (1)

/* SMC module features */

/* @brief Has partial stop option (register bit STOPCTRL[PSTOPO]). */
#define FSL_FEATURE_SMC_HAS_PSTOPO (0)
/* @brief Has LPO power option (register bit STOPCTRL[LPOPO]). */
#define FSL_FEATURE_SMC_HAS_LPOPO (0)
/* @brief Has POR power option (register bit STOPCTRL[PORPO] or VLLSCTRL[PORPO]). */
#define FSL_FEATURE_SMC_HAS_PORPO (1)
/* @brief Has low power wakeup on interrupt (register bit PMCTRL[LPWUI]). */
#define FSL_FEATURE_SMC_HAS_LPWUI (1)
/* @brief Has LLS or VLLS mode control (register bit STOPCTRL[LLSM]). */
#define FSL_FEATURE_SMC_HAS_LLS_SUBMODE (0)
/* @brief Has RAM partition 2 power option (register bit STOPCTRL[RAM2PO]). */
#define FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION (0)
/* @brief Has VLLS mode control (register bit VLLSCTRL[VLLSM]). */
#define FSL_FEATURE_SMC_USE_VLLSCTRL_REG (1)
/* @brief Has VLLS mode control (register bit STOPCTRL[VLLSM]). */
#define FSL_FEATURE_SMC_USE_STOPCTRL_VLLSM (0)
/* @brief Has high speed run mode (register bit PMPROT[AHSRUN]). */
#define FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE (0)
/* @brief Has low leakage stop mode (register bit PMPROT[ALLS]). */
#define FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE (1)
/* @brief Has very low leakage stop mode (register bit PMPROT[AVLLS]). */
#define FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE (1)
/* @brief Has stop submode. */
#define FSL_FEATURE_SMC_HAS_SUB_STOP_MODE (1)
/* @brief Has stop submode 0(VLLS0). */
#define FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 (1)
/* @brief Has stop submode 2(VLLS2). */
#define FSL_FEATURE_SMC_HAS_STOP_SUBMODE2 (1)
/* @brief Has SMC_PARAM. */
#define FSL_FEATURE_SMC_HAS_PARAM (0)
/* @brief Has SMC_VERID. */
#define FSL_FEATURE_SMC_HAS_VERID (0)

/* DSPI module features */

#if defined(CPU_MK20DX128VFM5) || defined(CPU_MK20DN128VFM5) || defined(CPU_MK20DX64VFM5) || defined(CPU_MK20DN64VFM5) || \
    defined(CPU_MK20DX32VFM5) || defined(CPU_MK20DN32VFM5)
    /* @brief Receive/transmit FIFO size in number of items. */
    #define FSL_FEATURE_DSPI_FIFO_SIZEn(x) (4)
    /* @brief Maximum transfer data width in bits. */
    #define FSL_FEATURE_DSPI_MAX_DATA_WIDTH (16)
    /* @brief Maximum number of chip select pins. (Reflects the width of register bit field PUSHR[PCS].) */
    #define FSL_FEATURE_DSPI_MAX_CHIP_SELECT_COUNT (6)
    /* @brief Number of chip select pins. */
    #define FSL_FEATURE_DSPI_CHIP_SELECT_COUNT (4)
    /* @brief Has chip select strobe capability on the PCS5 pin. */
    #define FSL_FEATURE_DSPI_HAS_CHIP_SELECT_STROBE (1)
    /* @brief Has separated TXDATA and CMD FIFOs (register SREX). */
    #define FSL_FEATURE_DSPI_HAS_SEPARATE_TXDATA_CMD_FIFO (0)
    /* @brief Has 16-bit data transfer support. */
    #define FSL_FEATURE_DSPI_16BIT_TRANSFERS (1)
    /* @brief Has separate DMA RX and TX requests. */
    #define FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(x) (1)
#elif defined(CPU_MK20DX128VMP5) || defined(CPU_MK20DN128VMP5) || defined(CPU_MK20DX64VMP5) || defined(CPU_MK20DN64VMP5) || \
    defined(CPU_MK20DX32VMP5) || defined(CPU_MK20DN32VMP5) || defined(CPU_MK20DX128VLH5) || defined(CPU_MK20DN128VLH5) || \
    defined(CPU_MK20DX64VLH5) || defined(CPU_MK20DN64VLH5) || defined(CPU_MK20DX32VLH5) || defined(CPU_MK20DN32VLH5) || \
    defined(CPU_MK20DX128VFT5) || defined(CPU_MK20DN128VFT5) || defined(CPU_MK20DX64VFT5) || defined(CPU_MK20DN64VFT5) || \
    defined(CPU_MK20DX32VFT5) || defined(CPU_MK20DN32VFT5) || defined(CPU_MK20DX128VLF5) || defined(CPU_MK20DN128VLF5) || \
    defined(CPU_MK20DX64VLF5) || defined(CPU_MK20DN64VLF5) || defined(CPU_MK20DX32VLF5) || defined(CPU_MK20DN32VLF5)
    /* @brief Receive/transmit FIFO size in number of items. */
    #define FSL_FEATURE_DSPI_FIFO_SIZEn(x) (4)
    /* @brief Maximum transfer data width in bits. */
    #define FSL_FEATURE_DSPI_MAX_DATA_WIDTH (16)
    /* @brief Maximum number of chip select pins. (Reflects the width of register bit field PUSHR[PCS].) */
    #define FSL_FEATURE_DSPI_MAX_CHIP_SELECT_COUNT (6)
    /* @brief Number of chip select pins. */
    #define FSL_FEATURE_DSPI_CHIP_SELECT_COUNT (5)
    /* @brief Has chip select strobe capability on the PCS5 pin. */
    #define FSL_FEATURE_DSPI_HAS_CHIP_SELECT_STROBE (1)
    /* @brief Has separated TXDATA and CMD FIFOs (register SREX). */
    #define FSL_FEATURE_DSPI_HAS_SEPARATE_TXDATA_CMD_FIFO (0)
    /* @brief Has 16-bit data transfer support. */
    #define FSL_FEATURE_DSPI_16BIT_TRANSFERS (1)
    /* @brief Has separate DMA RX and TX requests. */
    #define FSL_FEATURE_DSPI_HAS_SEPARATE_DMA_RX_TX_REQn(x) (1)
#endif /* defined(CPU_MK20DX128VFM5) || defined(CPU_MK20DN128VFM5) || defined(CPU_MK20DX64VFM5) || defined(CPU_MK20DN64VFM5) || \
    defined(CPU_MK20DX32VFM5) || defined(CPU_MK20DN32VFM5) */

/* SysTick module features */

/* @brief Systick has external reference clock. */
#define FSL_FEATURE_SYSTICK_HAS_EXT_REF (0)
/* @brief Systick external reference clock is core clock divided by this value. */
#define FSL_FEATURE_SYSTICK_EXT_REF_CORE_DIV (0)

/* TSI module features */

/* @brief TSI module version. */
#define FSL_FEATURE_TSI_VERSION (2)
/* @brief Has end-of-scan DMA transfer request enable (register bit GENCS[EOSDMEO]). */
#define FSL_FEATURE_TSI_HAS_END_OF_SCAN_DMA_ENABLE (0)
/* @brief Number of TSI channels. */
#define FSL_FEATURE_TSI_CHANNEL_COUNT (16)

/* UART module features */

#if defined(CPU_MK20DX128VMP5) || defined(CPU_MK20DN128VMP5) || defined(CPU_MK20DX64VMP5) || defined(CPU_MK20DN64VMP5) || \
    defined(CPU_MK20DX32VMP5) || defined(CPU_MK20DN32VMP5) || defined(CPU_MK20DX128VLH5) || defined(CPU_MK20DN128VLH5) || \
    defined(CPU_MK20DX64VLH5) || defined(CPU_MK20DN64VLH5) || defined(CPU_MK20DX32VLH5) || defined(CPU_MK20DN32VLH5) || \
    defined(CPU_MK20DX128VFT5) || defined(CPU_MK20DN128VFT5) || defined(CPU_MK20DX64VFT5) || defined(CPU_MK20DN64VFT5) || \
    defined(CPU_MK20DX32VFT5) || defined(CPU_MK20DN32VFT5) || defined(CPU_MK20DX128VLF5) || defined(CPU_MK20DN128VLF5) || \
    defined(CPU_MK20DX64VLF5) || defined(CPU_MK20DN64VLF5) || defined(CPU_MK20DX32VLF5) || defined(CPU_MK20DN32VLF5)
    /* @brief Has receive FIFO overflow detection (bit field CFIFO[RXOFE]). */
    #define FSL_FEATURE_UART_HAS_IRQ_EXTENDED_FUNCTIONS (0)
    /* @brief Has low power features (can be enabled in wait mode via register bit C1[DOZEEN] or CTRL[DOZEEN] if the registers are 32-bit wide). */
    #define FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT (0)
    /* @brief Has extended data register ED (or extra flags in the DATA register if the registers are 32-bit wide). */
    #define FSL_FEATURE_UART_HAS_EXTENDED_DATA_REGISTER_FLAGS (1)
    /* @brief Capacity (number of entries) of the transmit/receive FIFO (or zero if no FIFO is available). */
    #define FSL_FEATURE_UART_HAS_FIFO (1)
    /* @brief Hardware flow control (RTS, CTS) is supported. */
    #define FSL_FEATURE_UART_HAS_MODEM_SUPPORT (1)
    /* @brief Infrared (modulation) is supported. */
    #define FSL_FEATURE_UART_HAS_IR_SUPPORT (1)
    /* @brief 2 bits long stop bit is available. */
    #define FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT (0)
    /* @brief Maximal data width without parity bit. */
    #define FSL_FEATURE_UART_HAS_10BIT_DATA_SUPPORT (0)
    /* @brief Baud rate fine adjustment is available. */
    #define FSL_FEATURE_UART_HAS_BAUD_RATE_FINE_ADJUST_SUPPORT (1)
    /* @brief Baud rate oversampling is available (has bit fields C4[OSR], C5[BOTHEDGE], C5[RESYNCDIS] or BAUD[OSR], BAUD[BOTHEDGE], BAUD[RESYNCDIS] if the registers are 32-bit wide). */
    #define FSL_FEATURE_UART_HAS_BAUD_RATE_OVER_SAMPLING_SUPPORT (0)
    /* @brief Baud rate oversampling is available. */
    #define FSL_FEATURE_UART_HAS_RX_RESYNC_SUPPORT (0)
    /* @brief Baud rate oversampling is available. */
    #define FSL_FEATURE_UART_HAS_BOTH_EDGE_SAMPLING_SUPPORT (0)
    /* @brief Peripheral type. */
    #define FSL_FEATURE_UART_IS_SCI (0)
    /* @brief Capacity (number of entries) of the transmit/receive FIFO (or zero if no FIFO is available). */
    #define FSL_FEATURE_UART_FIFO_SIZEn(x) \
        ((x) == UART0 ? (8) : \
        ((x) == UART1 ? (1) : \
        ((x) == UART2 ? (1) : (-1))))
    /* @brief Maximal data width without parity bit. */
    #define FSL_FEATURE_UART_MAX_DATA_WIDTH_WITH_NO_PARITY (9)
    /* @brief Maximal data width with parity bit. */
    #define FSL_FEATURE_UART_MAX_DATA_WIDTH_WITH_PARITY (10)
    /* @brief Supports two match addresses to filter incoming frames. */
    #define FSL_FEATURE_UART_HAS_ADDRESS_MATCHING (1)
    /* @brief Has transmitter/receiver DMA enable bits C5[TDMAE]/C5[RDMAE] (or BAUD[TDMAE]/BAUD[RDMAE] if the registers are 32-bit wide). */
    #define FSL_FEATURE_UART_HAS_DMA_ENABLE (0)
    /* @brief Has transmitter/receiver DMA select bits C4[TDMAS]/C4[RDMAS], resp. C5[TDMAS]/C5[RDMAS] if IS_SCI = 0. */
    #define FSL_FEATURE_UART_HAS_DMA_SELECT (1)
    /* @brief Data character bit order selection is supported (bit field S2[MSBF] or STAT[MSBF] if the registers are 32-bit wide). */
    #define FSL_FEATURE_UART_HAS_BIT_ORDER_SELECT (1)
    /* @brief Has smart card (ISO7816 protocol) support and no improved smart card support. */
    #define FSL_FEATURE_UART_HAS_SMART_CARD_SUPPORT (1)
    /* @brief Has improved smart card (ISO7816 protocol) support. */
    #define FSL_FEATURE_UART_HAS_IMPROVED_SMART_CARD_SUPPORT (0)
    /* @brief Has local operation network (CEA709.1-B protocol) support. */
    #define FSL_FEATURE_UART_HAS_LOCAL_OPERATION_NETWORK_SUPPORT (1)
    /* @brief Has 32-bit registers (BAUD, STAT, CTRL, DATA, MATCH, MODIR) instead of 8-bit (BDH, BDL, C1, S1, D, etc.). */
    #define FSL_FEATURE_UART_HAS_32BIT_REGISTERS (0)
    /* @brief Lin break detect available (has bit BDH[LBKDIE]). */
    #define FSL_FEATURE_UART_HAS_LIN_BREAK_DETECT (1)
    /* @brief UART stops in Wait mode available (has bit C1[UARTSWAI]). */
    #define FSL_FEATURE_UART_HAS_WAIT_MODE_OPERATION (1)
    /* @brief Has separate DMA RX and TX requests. */
    #define FSL_FEATURE_UART_HAS_SEPARATE_DMA_RX_TX_REQn(x) (1)
#elif defined(CPU_MK20DX128VFM5) || defined(CPU_MK20DN128VFM5) || defined(CPU_MK20DX64VFM5) || defined(CPU_MK20DN64VFM5) || \
    defined(CPU_MK20DX32VFM5) || defined(CPU_MK20DN32VFM5)
    /* @brief Has receive FIFO overflow detection (bit field CFIFO[RXOFE]). */
    #define FSL_FEATURE_UART_HAS_IRQ_EXTENDED_FUNCTIONS (0)
    /* @brief Has low power features (can be enabled in wait mode via register bit C1[DOZEEN] or CTRL[DOZEEN] if the registers are 32-bit wide). */
    #define FSL_FEATURE_UART_HAS_LOW_POWER_UART_SUPPORT (0)
    /* @brief Has extended data register ED (or extra flags in the DATA register if the registers are 32-bit wide). */
    #define FSL_FEATURE_UART_HAS_EXTENDED_DATA_REGISTER_FLAGS (1)
    /* @brief Capacity (number of entries) of the transmit/receive FIFO (or zero if no FIFO is available). */
    #define FSL_FEATURE_UART_HAS_FIFO (1)
    /* @brief Hardware flow control (RTS, CTS) is supported. */
    #define FSL_FEATURE_UART_HAS_MODEM_SUPPORT (1)
    /* @brief Infrared (modulation) is supported. */
    #define FSL_FEATURE_UART_HAS_IR_SUPPORT (1)
    /* @brief 2 bits long stop bit is available. */
    #define FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT (0)
    /* @brief Maximal data width without parity bit. */
    #define FSL_FEATURE_UART_HAS_10BIT_DATA_SUPPORT (0)
    /* @brief Baud rate fine adjustment is available. */
    #define FSL_FEATURE_UART_HAS_BAUD_RATE_FINE_ADJUST_SUPPORT (1)
    /* @brief Baud rate oversampling is available (has bit fields C4[OSR], C5[BOTHEDGE], C5[RESYNCDIS] or BAUD[OSR], BAUD[BOTHEDGE], BAUD[RESYNCDIS] if the registers are 32-bit wide). */
    #define FSL_FEATURE_UART_HAS_BAUD_RATE_OVER_SAMPLING_SUPPORT (0)
    /* @brief Baud rate oversampling is available. */
    #define FSL_FEATURE_UART_HAS_RX_RESYNC_SUPPORT (0)
    /* @brief Baud rate oversampling is available. */
    #define FSL_FEATURE_UART_HAS_BOTH_EDGE_SAMPLING_SUPPORT (0)
    /* @brief Peripheral type. */
    #define FSL_FEATURE_UART_IS_SCI (0)
    /* @brief Capacity (number of entries) of the transmit/receive FIFO (or zero if no FIFO is available). */
    #define FSL_FEATURE_UART_FIFO_SIZEn(x) \
        ((x) == UART0 ? (8) : \
        ((x) == UART1 ? (1) : (-1)))
    /* @brief Maximal data width without parity bit. */
    #define FSL_FEATURE_UART_MAX_DATA_WIDTH_WITH_NO_PARITY (9)
    /* @brief Maximal data width with parity bit. */
    #define FSL_FEATURE_UART_MAX_DATA_WIDTH_WITH_PARITY (10)
    /* @brief Supports two match addresses to filter incoming frames. */
    #define FSL_FEATURE_UART_HAS_ADDRESS_MATCHING (1)
    /* @brief Has transmitter/receiver DMA enable bits C5[TDMAE]/C5[RDMAE] (or BAUD[TDMAE]/BAUD[RDMAE] if the registers are 32-bit wide). */
    #define FSL_FEATURE_UART_HAS_DMA_ENABLE (0)
    /* @brief Has transmitter/receiver DMA select bits C4[TDMAS]/C4[RDMAS], resp. C5[TDMAS]/C5[RDMAS] if IS_SCI = 0. */
    #define FSL_FEATURE_UART_HAS_DMA_SELECT (1)
    /* @brief Data character bit order selection is supported (bit field S2[MSBF] or STAT[MSBF] if the registers are 32-bit wide). */
    #define FSL_FEATURE_UART_HAS_BIT_ORDER_SELECT (1)
    /* @brief Has smart card (ISO7816 protocol) support and no improved smart card support. */
    #define FSL_FEATURE_UART_HAS_SMART_CARD_SUPPORT (1)
    /* @brief Has improved smart card (ISO7816 protocol) support. */
    #define FSL_FEATURE_UART_HAS_IMPROVED_SMART_CARD_SUPPORT (0)
    /* @brief Has local operation network (CEA709.1-B protocol) support. */
    #define FSL_FEATURE_UART_HAS_LOCAL_OPERATION_NETWORK_SUPPORT (1)
    /* @brief Has 32-bit registers (BAUD, STAT, CTRL, DATA, MATCH, MODIR) instead of 8-bit (BDH, BDL, C1, S1, D, etc.). */
    #define FSL_FEATURE_UART_HAS_32BIT_REGISTERS (0)
    /* @brief Lin break detect available (has bit BDH[LBKDIE]). */
    #define FSL_FEATURE_UART_HAS_LIN_BREAK_DETECT (1)
    /* @brief UART stops in Wait mode available (has bit C1[UARTSWAI]). */
    #define FSL_FEATURE_UART_HAS_WAIT_MODE_OPERATION (1)
    /* @brief Has separate DMA RX and TX requests. */
    #define FSL_FEATURE_UART_HAS_SEPARATE_DMA_RX_TX_REQn(x) (1)
#endif /* defined(CPU_MK20DX128VMP5) || defined(CPU_MK20DN128VMP5) || defined(CPU_MK20DX64VMP5) || defined(CPU_MK20DN64VMP5) || \
    defined(CPU_MK20DX32VMP5) || defined(CPU_MK20DN32VMP5) || defined(CPU_MK20DX128VLH5) || defined(CPU_MK20DN128VLH5) || \
    defined(CPU_MK20DX64VLH5) || defined(CPU_MK20DN64VLH5) || defined(CPU_MK20DX32VLH5) || defined(CPU_MK20DN32VLH5) || \
    defined(CPU_MK20DX128VFT5) || defined(CPU_MK20DN128VFT5) || defined(CPU_MK20DX64VFT5) || defined(CPU_MK20DN64VFT5) || \
    defined(CPU_MK20DX32VFT5) || defined(CPU_MK20DN32VFT5) || defined(CPU_MK20DX128VLF5) || defined(CPU_MK20DN128VLF5) || \
    defined(CPU_MK20DX64VLF5) || defined(CPU_MK20DN64VLF5) || defined(CPU_MK20DX32VLF5) || defined(CPU_MK20DN32VLF5) */

/* USB module features */

/* @brief HOST mode enabled */
#define FSL_FEATURE_USB_KHCI_HOST_ENABLED (1)
/* @brief OTG mode enabled */
#define FSL_FEATURE_USB_KHCI_OTG_ENABLED (1)
/* @brief Size of the USB dedicated RAM */
#define FSL_FEATURE_USB_KHCI_USB_RAM (0)
/* @brief Has KEEP_ALIVE_CTRL register */
#define FSL_FEATURE_USB_KHCI_KEEP_ALIVE_ENABLED (0)
/* @brief Has the Dynamic SOF threshold compare support */
#define FSL_FEATURE_USB_KHCI_DYNAMIC_SOF_THRESHOLD_COMPARE_ENABLED (0)
/* @brief Has the VBUS detect support */
#define FSL_FEATURE_USB_KHCI_VBUS_DETECT_ENABLED (0)
/* @brief Has the IRC48M module clock support */
#define FSL_FEATURE_USB_KHCI_IRC48M_MODULE_CLOCK_ENABLED (0)
/* @brief Number of endpoints supported */
#define FSL_FEATURE_USB_ENDPT_COUNT (16)

/* VREF module features */

#if defined(CPU_MK20DX128VMP5) || defined(CPU_MK20DN128VMP5) || defined(CPU_MK20DX64VMP5) || defined(CPU_MK20DN64VMP5) || \
    defined(CPU_MK20DX32VMP5) || defined(CPU_MK20DN32VMP5) || defined(CPU_MK20DX128VLH5) || defined(CPU_MK20DN128VLH5) || \
    defined(CPU_MK20DX64VLH5) || defined(CPU_MK20DN64VLH5) || defined(CPU_MK20DX32VLH5) || defined(CPU_MK20DN32VLH5) || \
    defined(CPU_MK20DX128VFT5) || defined(CPU_MK20DN128VFT5) || defined(CPU_MK20DX64VFT5) || defined(CPU_MK20DN64VFT5) || \
    defined(CPU_MK20DX32VFT5) || defined(CPU_MK20DN32VFT5) || defined(CPU_MK20DX128VLF5) || defined(CPU_MK20DN128VLF5) || \
    defined(CPU_MK20DX64VLF5) || defined(CPU_MK20DN64VLF5) || defined(CPU_MK20DX32VLF5) || defined(CPU_MK20DN32VLF5)
    /* @brief Has chop oscillator (bit TRM[CHOPEN]) */
    #define FSL_FEATURE_VREF_HAS_CHOP_OSC (1)
    /* @brief Has second order curvature compensation (bit SC[ICOMPEN]) */
    #define FSL_FEATURE_VREF_HAS_COMPENSATION (0)
    /* @brief If high/low buffer mode supported */
    #define FSL_FEATURE_VREF_MODE_LV_TYPE (0)
    /* @brief Module has also low reference (registers VREFL/VREFH) */
    #define FSL_FEATURE_VREF_HAS_LOW_REFERENCE (0)
    /* @brief Has VREF_TRM4. */
    #define FSL_FEATURE_VREF_HAS_TRM4 (0)
#endif /* defined(CPU_MK20DX128VMP5) || defined(CPU_MK20DN128VMP5) || defined(CPU_MK20DX64VMP5) || defined(CPU_MK20DN64VMP5) || \
    defined(CPU_MK20DX32VMP5) || defined(CPU_MK20DN32VMP5) || defined(CPU_MK20DX128VLH5) || defined(CPU_MK20DN128VLH5) || \
    defined(CPU_MK20DX64VLH5) || defined(CPU_MK20DN64VLH5) || defined(CPU_MK20DX32VLH5) || defined(CPU_MK20DN32VLH5) || \
    defined(CPU_MK20DX128VFT5) || defined(CPU_MK20DN128VFT5) || defined(CPU_MK20DX64VFT5) || defined(CPU_MK20DN64VFT5) || \
    defined(CPU_MK20DX32VFT5) || defined(CPU_MK20DN32VFT5) || defined(CPU_MK20DX128VLF5) || defined(CPU_MK20DN128VLF5) || \
    defined(CPU_MK20DX64VLF5) || defined(CPU_MK20DN64VLF5) || defined(CPU_MK20DX32VLF5) || defined(CPU_MK20DN32VLF5) */

/* WDOG module features */

/* @brief Watchdog is available. */
#define FSL_FEATURE_WDOG_HAS_WATCHDOG (1)
/* @brief Has Wait mode support. */
#define FSL_FEATURE_WDOG_HAS_WAITEN (1)

#endif /* _MK20D5_FEATURES_H_ */

