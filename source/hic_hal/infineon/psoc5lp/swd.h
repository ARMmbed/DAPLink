/*******************************************************************************
* @file    swd.h
* @brief   This file contains the function prototypes and constants used in
*          the swd.c
*
********************************************************************************
* Copyright (2019-2021) Cypress Semiconductor Corporation (an Infineon company) 
* or an affiliate of Cypress Semiconductor Corporation.
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
*****************************************************************************/
#if !defined(SWD_H)
#define SWD_H

#include <stdint.h>
#include "cypins.h"
#include "SWDCLK.h"
#include "SWDIO.h"
#include "SWDXRES.h"

/* Programming pin drive modes */
#define SWD_SET_SDA_OUT             CyPins_SetPinDriveMode(SWDIO_0, SWDIO_DM_STRONG)
#define SWD_SET_SDA_IN              CyPins_SetPinDriveMode(SWDIO_0, SWDIO_DM_DIG_HIZ)
#define SWD_SET_SCK_OUT             CyPins_SetPinDriveMode(SWDCLK_0, SWDCLK_DM_STRONG)
#define SWD_SET_SCK_IN              CyPins_SetPinDriveMode(SWDCLK_0, SWDCLK_DM_DIG_HIZ)
#define SWD_SET_XRES_OUT            CyPins_SetPinDriveMode(SWDXRES_0, SWDXRES_DM_STRONG)
#define SWD_SET_XRES_IN             CyPins_SetPinDriveMode(SWDXRES_0, SWDXRES_DM_DIG_HIZ)

/* Acquire result macros */
#define ACQUIRE_WAIT                (0x02u)
#define ACQUIRE_PASS                (0x01u)
#define ACQUIRE_FAIL                (0x00u)

/* Bit banding of the peripheral addresses for flexibility in addressing SWDIO and SWDCLK */
/* Convert Peripheral address to peripheral bit map region */
#define BITBAND_PERI_REF            (0x40000000u)
#define BITBAND_PERI_BASE           (0x42000000u)

#define SWD_BITS                    (SWDCLK__DR)
#define SWD_SDA                     (*((volatile uint8_t *)((BITBAND_PERI_BASE + (SWD_BITS-BITBAND_PERI_REF)*32u + (SWDIO_SHIFT*4u)))))
#define SWD_SCK                     (*((volatile uint8_t *)((BITBAND_PERI_BASE + (SWD_BITS-BITBAND_PERI_REF)*32u + (SWDCLK_SHIFT*4u)))))
#define SDA_PS                      (*((volatile uint8_t *)((BITBAND_PERI_BASE + (SWDIO__PS-BITBAND_PERI_REF)*32u + (SWDIO_SHIFT*4u)))))
#define SCL_PS                      (*((volatile uint8_t *)(((BITBAND_PERI_BASE + (SWDCLK__PS-BITBAND_PERI_REF)*32u + (SWDCLK_SHIFT*4u))))))
#define XRES_PS                     (*((volatile uint8_t *)(((BITBAND_PERI_BASE + (SWDXRES__PS-BITBAND_PERI_REF)*32u + (SWDXRES_SHIFT*4u))))))

#define SWD_SET_SCK_LO              (SWD_SCK = 0u)
#define SWD_SET_SCK_HI              (SWD_SCK = 1u)
#define SWD_SET_SDA_LO              (SWD_SDA = 0u)
#define SWD_SET_SDA_HI              (SWD_SDA = 1u)
#define SWD_SET_XRES_HI             CyPins_SetPin(SWDXRES_0)
#define SWD_SET_XRES_LO             CyPins_ClearPin(SWDXRES_0)

#define SWD_GET_SDA                 (SDA_PS)
#define SWD_GET_SCL                 (SCL_PS)
#define SWD_GET_XRES                (XRES_PS)

#endif /* SWD_H */


/* [] END OF FILE */
