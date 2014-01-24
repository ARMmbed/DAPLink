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

#ifndef __DEVICE_REGISTERS_H__
#define __DEVICE_REGISTERS_H__

#include <stdint.h>

/*
 * Include the cpu specific register header file.
 *
 * The CPU macro should be declared in the project or makefile.
 */
#if (defined(CPU_MK22FN512VMC12))
    #define K22F51212_SERIES
    // Extension register headers. (These will eventually be merged into the CMSIS-style header.)
    #include "device/MK22F51212/MK22F51212_adc.h"
    #include "device/MK22F51212/MK22F51212_aips.h"
    #include "device/MK22F51212/MK22F51212_cmp.h"
    #include "device/MK22F51212/MK22F51212_crc.h"
    #include "device/MK22F51212/MK22F51212_dac.h"
    #include "device/MK22F51212/MK22F51212_dma.h"
    #include "device/MK22F51212/MK22F51212_dmamux.h"
    #include "device/MK22F51212/MK22F51212_ewm.h"
    #include "device/MK22F51212/MK22F51212_fb.h"
    #include "device/MK22F51212/MK22F51212_fmc.h"
    #include "device/MK22F51212/MK22F51212_ftfa.h"
    #include "device/MK22F51212/MK22F51212_ftm.h"
    #include "device/MK22F51212/MK22F51212_gpio.h"
    #include "device/MK22F51212/MK22F51212_i2c.h"
    #include "device/MK22F51212/MK22F51212_i2s.h"
    #include "device/MK22F51212/MK22F51212_llwu.h"
    #include "device/MK22F51212/MK22F51212_lptmr.h"
    #include "device/MK22F51212/MK22F51212_mcg.h"
    #include "device/MK22F51212/MK22F51212_mcm.h"
    #include "device/MK22F51212/MK22F51212_osc.h"
    #include "device/MK22F51212/MK22F51212_pdb.h"
    #include "device/MK22F51212/MK22F51212_pit.h"
    #include "device/MK22F51212/MK22F51212_pmc.h"
    #include "device/MK22F51212/MK22F51212_port.h"
    #include "device/MK22F51212/MK22F51212_rcm.h"
    #include "device/MK22F51212/MK22F51212_rfsys.h"
    #include "device/MK22F51212/MK22F51212_rfvbat.h"
    #include "device/MK22F51212/MK22F51212_rng.h"
    #include "device/MK22F51212/MK22F51212_rtc.h"
    #include "device/MK22F51212/MK22F51212_sim.h"
    #include "device/MK22F51212/MK22F51212_smc.h"
    #include "device/MK22F51212/MK22F51212_spi.h"
    #include "device/MK22F51212/MK22F51212_uart.h"
    #include "device/MK22F51212/MK22F51212_usb.h"
    #include "device/MK22F51212/MK22F51212_vref.h"
    #include "device/MK22F51212/MK22F51212_wdog.h"

    // CMSIS-style register definitions
    #include "device/MK22F51212/MK22F51212.h"

#elif (defined(CPU_MK64FN1M0VMD12) || defined(CPU_MK64FX512VMD12))
    #define K64F12_SERIES
    // Extension register headers. (These will eventually be merged into the CMSIS-style header.)
    #include "device/MK64F12/MK64F12_adc.h"
    #include "device/MK64F12/MK64F12_aips.h"
    #include "device/MK64F12/MK64F12_axbs.h"
    #include "device/MK64F12/MK64F12_can.h"
    #include "device/MK64F12/MK64F12_cau.h"
    #include "device/MK64F12/MK64F12_cmp.h"
    #include "device/MK64F12/MK64F12_cmt.h"
    #include "device/MK64F12/MK64F12_crc.h"
    #include "device/MK64F12/MK64F12_dac.h"
    #include "device/MK64F12/MK64F12_dma.h"
    #include "device/MK64F12/MK64F12_dmamux.h"
    #include "device/MK64F12/MK64F12_enet.h"
    #include "device/MK64F12/MK64F12_ewm.h"
    #include "device/MK64F12/MK64F12_fb.h"
    #include "device/MK64F12/MK64F12_fmc.h"
    #include "device/MK64F12/MK64F12_ftfe.h"
    #include "device/MK64F12/MK64F12_ftm.h"
    #include "device/MK64F12/MK64F12_gpio.h"
    #include "device/MK64F12/MK64F12_i2c.h"
    #include "device/MK64F12/MK64F12_i2s.h"
    #include "device/MK64F12/MK64F12_llwu.h"
    #include "device/MK64F12/MK64F12_lptmr.h"
    #include "device/MK64F12/MK64F12_mcg.h"
    #include "device/MK64F12/MK64F12_mcm.h"
    #include "device/MK64F12/MK64F12_mpu.h"
    #include "device/MK64F12/MK64F12_nv.h"
    #include "device/MK64F12/MK64F12_osc.h"
    #include "device/MK64F12/MK64F12_pdb.h"
    #include "device/MK64F12/MK64F12_pit.h"
    #include "device/MK64F12/MK64F12_pmc.h"
    #include "device/MK64F12/MK64F12_port.h"
    #include "device/MK64F12/MK64F12_rcm.h"
    #include "device/MK64F12/MK64F12_rfsys.h"
    #include "device/MK64F12/MK64F12_rfvbat.h"
    #include "device/MK64F12/MK64F12_rng.h"
    #include "device/MK64F12/MK64F12_rtc.h"
    #include "device/MK64F12/MK64F12_sdhc.h"
    #include "device/MK64F12/MK64F12_sim.h"
    #include "device/MK64F12/MK64F12_smc.h"
    #include "device/MK64F12/MK64F12_spi.h"
    #include "device/MK64F12/MK64F12_uart.h"
    #include "device/MK64F12/MK64F12_usb.h"
    #include "device/MK64F12/MK64F12_usbdcd.h"
    #include "device/MK64F12/MK64F12_vref.h"
    #include "device/MK64F12/MK64F12_wdog.h"

    // CMSIS-style register definitions
    #include "device/MK64F12/MK64F12.h"

#elif (defined(CPU_MKL25Z32VFM4) || defined(CPU_MKL25Z64VFM4) || defined(CPU_MKL25Z128VFM4) || \
       defined(CPU_MKL25Z32VFT4) || defined(CPU_MKL25Z64VFT4) || defined(CPU_MKL25Z128VFT4) || \
       defined(CPU_MKL25Z32VLH4) || defined(CPU_MKL25Z64VLH4) || defined(CPU_MKL25Z128VLH4) || \
       defined(CPU_MKL25Z32VLK4) || defined(CPU_MKL25Z64VLK4) || defined(CPU_MKL25Z128VLK4))
    #define KL25Z4_SERIES
    #include "device/MKL25Z4/MKL25Z4_dma.h"
    #include "device/MKL25Z4/MKL25Z4_dmamux.h"
    #include "device/MKL25Z4/MKL25Z4_ftfa.h"
    #include "device/MKL25Z4/MKL25Z4_gpio.h"
    #include "device/MKL25Z4/MKL25Z4_i2c.h"
    #include "device/MKL25Z4/MKL25Z4_mcg.h"
    #include "device/MKL25Z4/MKL25Z4_mcm.h"
    #include "device/MKL25Z4/MKL25Z4_osc.h"
    #include "device/MKL25Z4/MKL25Z4_pctl.h"
    #include "device/MKL25Z4/MKL25Z4_pit.h"
    #include "device/MKL25Z4/MKL25Z4_sim.h"
    #include "device/MKL25Z4/MKL25Z4_smc.h"
    #include "device/MKL25Z4/MKL25Z4_spi.h"
    #include "device/MKL25Z4/MKL25Z4_uart.h"
    #include "device/MKL25Z4/MKL25Z4_uart0.h"
    #include "device/MKL25Z4/MKL25Z4_usb.h"

    // Now include CMSIS style. This must come last because it defines some macros like
    // I2C0, and there are some register bitfield names in SIM that have the same name,
    // thus causing a conflict.
    #include "device/MKL25Z4/MKL25Z4.h"

#elif (defined(CPU_MKL02Z32VFM4) || defined(CPU_MKL02Z16VFM4) || defined(CPU_MKL02Z16VFK4) || defined(CPU_MKL02Z32VFK4) || defined(CPU_MKL02Z8VFG4) || defined(CPU_MKL02Z16VFG4) || defined(CPU_MKL02Z32VFG4) || defined(CPU_MKL02Z32CAF4))

    #include "device/MKL02Z4/MKL02Z4_ftfa.h"
    #include "device/MKL02Z4/MKL02Z4_gpio.h"
    #include "device/MKL02Z4/MKL02Z4_i2c.h"
    #include "device/MKL02Z4/MKL02Z4_mcg.h"
    #include "device/MKL02Z4/MKL02Z4_mcm.h"
    #include "device/MKL02Z4/MKL02Z4_osc.h"
    #include "device/MKL02Z4/MKL02Z4_pctl.h"
    #include "device/MKL02Z4/MKL02Z4_sim.h"
    #include "device/MKL02Z4/MKL02Z4_smc.h"
    #include "device/MKL02Z4/MKL02Z4_spi.h"
    #include "device/MKL02Z4/MKL02Z4_uart0.h"

    // CMSIS-style register definitions
    #include "device/MKL02Z4/MKL02Z4.h"

#elif (defined(CPU_MKL03Z32VFM4) || defined(CPU_MKL03Z16VFM4) || defined(CPU_MKL03Z16VFK4) || defined(CPU_MKL03Z32VFK4) || defined(CPU_MKL03Z8VFG4) || defined(CPU_MKL03Z16VFG4) || defined(CPU_MKL03Z32VFG4) || defined(CPU_MKL03Z32CAF4))

    #define KL03_SERIES
    #include "device/MKL03Z4/MKL03Z4_ftfa.h"
    #include "device/MKL03Z4/MKL03Z4_gpio.h"
    #include "device/MKL03Z4/MKL03Z4_i2c.h"
    #include "device/MKL03Z4/MKL03Z4_mcg.h"
    #include "device/MKL03Z4/MKL03Z4_mcm.h"
    #include "device/MKL03Z4/MKL03Z4_osc.h"
    #include "device/MKL03Z4/MKL03Z4_pctl.h"
    #include "device/MKL03Z4/MKL03Z4_sim.h"
    #include "device/MKL03Z4/MKL03Z4_smc.h"
    #include "device/MKL03Z4/MKL03Z4_spi.h"
    #include "device/MKL03Z4/MKL03Z4_lpuart.h"
    #include "device/MKL03Z4/MKL03Z4_rcm.h"

    // CMSIS-style register definitions
    #include "device/MKL03Z4/MKL03Z4.h"

#elif (defined(CPU_MKL43Z64VFT4) || defined(CPU_MKL43Z128VFT4) || defined(CPU_MKL43Z64VLH4) || defined(CPU_MKL43Z128VLH4) || defined(CPU_MKL43Z256VLH4) || defined(CPU_MKL43Z64VMP4) || defined(CPU_MKL43Z128VMP4) || defined(CPU_MKL43Z256VMP4))

    #define KL43Z4_SERIES
    #include "device/MKL43Z4/MKL43Z4_dma.h"
    #include "device/MKL43Z4/MKL43Z4_dmamux.h"
    #include "device/MKL43Z4/MKL43Z4_ftfa.h"
    #include "device/MKL43Z4/MKL43Z4_gpio.h"
    #include "device/MKL43Z4/MKL43Z4_i2c.h"
    #include "device/MKL43Z4/MKL43Z4_mcg.h"
    #include "device/MKL43Z4/MKL43Z4_mcm.h"
    #include "device/MKL43Z4/MKL43Z4_osc.h"
    #include "device/MKL43Z4/MKL43Z4_pctl.h"
    #include "device/MKL43Z4/MKL43Z4_pit.h"
    #include "device/MKL43Z4/MKL43Z4_sim.h"
    #include "device/MKL43Z4/MKL43Z4_smc.h"
    #include "device/MKL43Z4/MKL43Z4_spi.h"
    #include "device/MKL43Z4/MKL43Z4_uart.h"
    #include "device/MKL43Z4/MKL43Z4_lpuart.h"
    #include "device/MKL43Z4/MKL43Z4_usb.h"
    #include "device/MKL43Z4/MKL43Z4_rcm.h"

    // CMSIS-style register definitions
    #include "device/MKL43Z4/MKL43Z4.h"

#elif (defined(CPU_MK70FN1M0VMJ12) || defined(CPU_MK70FX512VMJ12))
    // Extension register headers. (These will eventually be merged into the CMSIS-style header.)

    #define K27F12_SERIES
    #include "device/MK70F12/K70_adc.h"
    #include "device/MK70F12/K70_cau.h"
    #include "device/MK70F12/K70_crc.h"
    #include "device/MK70F12/K70_dma.h"
    #include "device/MK70F12/K70_dmamux.h"
    #include "device/MK70F12/K70_flexcan.h"
    #include "device/MK70F12/K70_fmc.h"
    #include "device/MK70F12/K70_ftfe.h"
    #include "device/MK70F12/K70_ftm.h"
    #include "device/MK70F12/K70_gpio.h"
    #include "device/MK70F12/K70_i2c.h"
    #include "device/MK70F12/K70_lcdc.h"
    #include "device/MK70F12/K70_lptmr.h"
    #include "device/MK70F12/K70_mcg.h"
    #include "device/MK70F12/K70_mcm.h"
    #include "device/MK70F12/K70_osc.h"
    #include "device/MK70F12/K70_pdb.h"
    #include "device/MK70F12/K70_pit.h"
    #include "device/MK70F12/K70_port.h"
    #include "device/MK70F12/K70_sai.h"
    #include "device/MK70F12/K70_sdhc.h"
    #include "device/MK70F12/K70_sim.h"
    #include "device/MK70F12/K70_smc.h"
    #include "device/MK70F12/K70_spi.h"
    #include "device/MK70F12/K70_uart.h"
    #include "device/MK70F12/K70_usb.h"
    #include "device/MK70F12/K70_usbhs.h"

    // Now include CMSIS style.
    #include "device/MK70F12/MK70F12.h"

#else
    #error "No valid CPU defined"
#endif



#endif // __DEVICE_REGISTERS_H__
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

