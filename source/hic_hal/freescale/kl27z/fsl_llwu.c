/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_llwu.h"

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.llwu"
#endif

#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN)
/*!
 * brief Sets the external input pin source mode.
 *
 * This function sets the external input pin source mode that is used
 * as a wake up source.
 *
 * param base LLWU peripheral base address.
 * param pinIndex A pin index to be enabled as an external wakeup source starting from 1.
 * param pinMode A pin configuration mode defined in the llwu_external_pin_modes_t.
 */
void LLWU_SetExternalWakeupPinMode(LLWU_Type *base, uint32_t pinIndex, llwu_external_pin_mode_t pinMode)
{
#if (defined(FSL_FEATURE_LLWU_REG_BITWIDTH) && (FSL_FEATURE_LLWU_REG_BITWIDTH == 32))
    volatile uint32_t *regBase;
    uint32_t regOffset;
    uint32_t reg;

    switch (pinIndex >> 4U)
    {
        case 0U:
            regBase = &base->PE1;
            break;
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 16))
        case 1U:
            regBase = &base->PE2;
            break;
#endif
        default:
            regBase = NULL;
            break;
    }
#else
    volatile uint8_t *regBase;
    uint8_t regOffset;
    uint8_t reg;
    switch (pinIndex >> 2U)
    {
        case 0U:
            regBase = &base->PE1;
            break;
        case 1U:
            regBase = &base->PE2;
            break;
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 8))
        case 2U:
            regBase = &base->PE3;
            break;
#endif
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 12))
        case 3U:
            regBase = &base->PE4;
            break;
#endif
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 16))
        case 4U:
            regBase = &base->PE5;
            break;
#endif
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 20))
        case 5U:
            regBase = &base->PE6;
            break;
#endif
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 24))
        case 6U:
            regBase = &base->PE7;
            break;
#endif
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 28))
        case 7U:
            regBase = &base->PE8;
            break;
#endif
        default:
            regBase = NULL;
            break;
    }
#endif /* FSL_FEATURE_LLWU_REG_BITWIDTH == 32 */

    if (NULL != regBase)
    {
        reg = *regBase;
#if (defined(FSL_FEATURE_LLWU_REG_BITWIDTH) && (FSL_FEATURE_LLWU_REG_BITWIDTH == 32))
        regOffset = ((pinIndex & 0x0FU) << 1U);
#else
        regOffset = (uint8_t)((pinIndex & 0x03U) << 1U);
#endif
        reg &= ~(0x3U << regOffset);
        reg |= (uint8_t)((uint32_t)pinMode << regOffset);
        *regBase = reg;
    }
}

/*!
 * brief Gets the external wakeup source flag.
 *
 * This function checks the external pin flag to detect whether the MCU is
 * woken up by the specific pin.
 *
 * param base LLWU peripheral base address.
 * param pinIndex     A pin index, which starts from 1.
 * return True if the specific pin is a wakeup source.
 */
bool LLWU_GetExternalWakeupPinFlag(LLWU_Type *base, uint32_t pinIndex)
{
#if (defined(FSL_FEATURE_LLWU_REG_BITWIDTH) && (FSL_FEATURE_LLWU_REG_BITWIDTH == 32))
    return (bool)(base->PF & (1U << pinIndex));
#else
    volatile uint8_t *regBase;

    switch (pinIndex >> 3U)
    {
#if (defined(FSL_FEATURE_LLWU_HAS_PF) && FSL_FEATURE_LLWU_HAS_PF)
        case 0U:
            regBase = &base->PF1;
            break;
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 8))
        case 1U:
            regBase = &base->PF2;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN */
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 16))
        case 2U:
            regBase = &base->PF3;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN */
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 24))
        case 3U:
            regBase = &base->PF4;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN */
#else
        case 0U:
            regBase = &base->F1;
            break;
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 8))
        case 1U:
            regBase = &base->F2;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN */
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 16))
        case 2U:
            regBase = &base->F3;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN */
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 24))
        case 3U:
            regBase = &base->F4;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN */
#endif /* FSL_FEATURE_LLWU_HAS_PF */
        default:
            regBase = NULL;
            break;
    }

    if (NULL != regBase)
    {
        if (0U != (*regBase & (1U << pinIndex % 8U)))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
#endif /* FSL_FEATURE_LLWU_REG_BITWIDTH */
}

/*!
 * brief Clears the external wakeup source flag.
 *
 * This function clears the external wakeup source flag for a specific pin.
 *
 * param base LLWU peripheral base address.
 * param pinIndex A pin index, which starts from 1.
 */
void LLWU_ClearExternalWakeupPinFlag(LLWU_Type *base, uint32_t pinIndex)
{
#if (defined(FSL_FEATURE_LLWU_REG_BITWIDTH) && (FSL_FEATURE_LLWU_REG_BITWIDTH == 32))
    base->PF = (1U << pinIndex);
#else
    volatile uint8_t *regBase;
    switch (pinIndex >> 3U)
    {
#if (defined(FSL_FEATURE_LLWU_HAS_PF) && FSL_FEATURE_LLWU_HAS_PF)
        case 0U:
            regBase = &base->PF1;
            break;
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 8))
        case 1U:
            regBase = &base->PF2;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN */
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 16))
        case 2U:
            regBase = &base->PF3;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN */
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 24))
        case 3U:
            regBase = &base->PF4;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN */
#else
        case 0U:
            regBase = &base->F1;
            break;
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 8))
        case 1U:
            regBase = &base->F2;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN */
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 16))
        case 2U:
            regBase = &base->F3;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN */
#if (defined(FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN) && (FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN > 24))
        case 3U:
            regBase = &base->F4;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN */
#endif /* FSL_FEATURE_LLWU_HAS_PF */
        default:
            regBase = NULL;
            break;
    }
    if (NULL != regBase)
    {
        *regBase = (1U << pinIndex % 8U);
    }
#endif /* FSL_FEATURE_LLWU_REG_BITWIDTH */
}
#endif /* FSL_FEATURE_LLWU_HAS_EXTERNAL_PIN */

#if (defined(FSL_FEATURE_LLWU_HAS_PIN_FILTER) && FSL_FEATURE_LLWU_HAS_PIN_FILTER)
/*!
 * brief Sets the pin filter configuration.
 *
 * This function sets the pin filter configuration.
 *
 * param base LLWU peripheral base address.
 * param filterIndex A pin filter index used to enable/disable the digital filter, starting from 1.
 * param filterMode A filter mode configuration
 */
void LLWU_SetPinFilterMode(LLWU_Type *base, uint32_t filterIndex, llwu_external_pin_filter_mode_t filterMode)
{
#if (defined(FSL_FEATURE_LLWU_REG_BITWIDTH) && (FSL_FEATURE_LLWU_REG_BITWIDTH == 32))
    base->FILT = ((base->FILT) & ~((LLWU_FILT_FILTSEL1_MASK | LLWU_FILT_FILTE1_MASK) << ((filterIndex - 1U) * 8U))) |
                 ((LLWU_FILT_FILTSEL1(filterMode.pinIndex) | LLWU_FILT_FILTE1(filterMode.filterMode))
                  << ((filterIndex - 1U) * 8U)) |
                 LLWU_FILT_FILTF1_MASK /* W1C to clear the FILTF flag bit. */
        ;
#else
    volatile uint8_t *regBase;

    switch (filterIndex)
    {
        case 1U:
            regBase = &base->FILT1;
            break;
#if (defined(FSL_FEATURE_LLWU_HAS_PIN_FILTER) && (FSL_FEATURE_LLWU_HAS_PIN_FILTER > 1))
        case 2U:
            regBase = &base->FILT2;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_PIN_FILTER */
#if (defined(FSL_FEATURE_LLWU_HAS_PIN_FILTER) && (FSL_FEATURE_LLWU_HAS_PIN_FILTER > 2))
        case 3U:
            regBase = &base->FILT3;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_PIN_FILTER */
#if (defined(FSL_FEATURE_LLWU_HAS_PIN_FILTER) && (FSL_FEATURE_LLWU_HAS_PIN_FILTER > 3))
        case 4U:
            regBase = &base->FILT4;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_PIN_FILTER */
        default:
            regBase = NULL;
            break;
    }

    if (NULL != regBase)
    {
        *regBase = (uint8_t)((*regBase & ~(LLWU_FILT1_FILTSEL_MASK | LLWU_FILT1_FILTE_MASK)) |
                             LLWU_FILT1_FILTSEL(filterMode.pinIndex) | LLWU_FILT1_FILTE(filterMode.filterMode) |
                             LLWU_FILT1_FILTF_MASK) /* W1C to clear the FILTF flag bit. */
            ;
    }
#endif /* FSL_FEATURE_LLWU_REG_BITWIDTH */
}

/*!
 * brief Gets the pin filter configuration.
 *
 * This function gets the pin filter flag.
 *
 * param base LLWU peripheral base address.
 * param filterIndex A pin filter index, which starts from 1.
 * return True if the flag is a source of the existing low-leakage power mode.
 */
bool LLWU_GetPinFilterFlag(LLWU_Type *base, uint32_t filterIndex)
{
#if (defined(FSL_FEATURE_LLWU_REG_BITWIDTH) && (FSL_FEATURE_LLWU_REG_BITWIDTH == 32))
    return (bool)(base->FILT & (1U << (filterIndex * 8U - 1)));
#else
    bool status = false;

    switch (filterIndex)
    {
        case 1:
            status = ((base->FILT1 & LLWU_FILT1_FILTF_MASK) != 0U);
            break;
#if (defined(FSL_FEATURE_LLWU_HAS_PIN_FILTER) && (FSL_FEATURE_LLWU_HAS_PIN_FILTER > 1))
        case 2:
            status = ((base->FILT2 & LLWU_FILT2_FILTF_MASK) != 0U);
            break;
#endif /* FSL_FEATURE_LLWU_HAS_PIN_FILTER */
#if (defined(FSL_FEATURE_LLWU_HAS_PIN_FILTER) && (FSL_FEATURE_LLWU_HAS_PIN_FILTER > 2))
        case 3:
            status = (base->FILT3 & LLWU_FILT3_FILTF_MASK);
            break;
#endif /* FSL_FEATURE_LLWU_HAS_PIN_FILTER */
#if (defined(FSL_FEATURE_LLWU_HAS_PIN_FILTER) && (FSL_FEATURE_LLWU_HAS_PIN_FILTER > 3))
        case 4:
            status = (base->FILT4 & LLWU_FILT4_FILTF_MASK);
            break;
#endif /* FSL_FEATURE_LLWU_HAS_PIN_FILTER */
        default:
            break;
    }

    return status;
#endif /* FSL_FEATURE_LLWU_REG_BITWIDTH */
}

/*!
 * brief Clears the pin filter configuration.
 *
 * This function clears the pin filter flag.
 *
 * param base LLWU peripheral base address.
 * param filterIndex A pin filter index to clear the flag, starting from 1.
 */
void LLWU_ClearPinFilterFlag(LLWU_Type *base, uint32_t filterIndex)
{
#if (defined(FSL_FEATURE_LLWU_REG_BITWIDTH) && (FSL_FEATURE_LLWU_REG_BITWIDTH == 32))
    uint32_t reg;

    reg = base->FILT;
    switch (filterIndex)
    {
        case 1:
            reg |= LLWU_FILT_FILTF1_MASK;
            break;
#if (defined(FSL_FEATURE_LLWU_HAS_PIN_FILTER) && (FSL_FEATURE_LLWU_HAS_PIN_FILTER > 1))
        case 2:
            reg |= LLWU_FILT_FILTF2_MASK;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_PIN_FILTER > 1 */
#if (defined(FSL_FEATURE_LLWU_HAS_PIN_FILTER) && (FSL_FEATURE_LLWU_HAS_PIN_FILTER > 2))
        case 3:
            reg |= LLWU_FILT_FILTF3_MASK;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_PIN_FILTER > 2 */
#if (defined(FSL_FEATURE_LLWU_HAS_PIN_FILTER) && (FSL_FEATURE_LLWU_HAS_PIN_FILTER > 3))
        case 4:
            reg |= LLWU_FILT_FILTF4_MASK;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_PIN_FILTER > 3 */
        default:
            break;
    }
    base->FILT = reg;
#else
    volatile uint8_t *regBase;
    uint8_t reg;

    switch (filterIndex)
    {
        case 1:
            regBase = &base->FILT1;
            break;
#if (defined(FSL_FEATURE_LLWU_HAS_PIN_FILTER) && (FSL_FEATURE_LLWU_HAS_PIN_FILTER > 1))
        case 2:
            regBase = &base->FILT2;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_PIN_FILTER */
#if (defined(FSL_FEATURE_LLWU_HAS_PIN_FILTER) && (FSL_FEATURE_LLWU_HAS_PIN_FILTER > 2))
        case 3:
            regBase = &base->FILT3;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_PIN_FILTER */
#if (defined(FSL_FEATURE_LLWU_HAS_PIN_FILTER) && (FSL_FEATURE_LLWU_HAS_PIN_FILTER > 3))
        case 4:
            regBase = &base->FILT4;
            break;
#endif /* FSL_FEATURE_LLWU_HAS_PIN_FILTER */
        default:
            regBase = NULL;
            break;
    }

    if (NULL != regBase)
    {
        reg = *regBase;
        reg |= LLWU_FILT1_FILTF_MASK;
        *regBase = reg;
    }
#endif /* FSL_FEATURE_LLWU_REG_BITWIDTH */
}
#endif /* FSL_FEATURE_LLWU_HAS_PIN_FILTER */

#if (defined(FSL_FEATURE_LLWU_HAS_RESET_ENABLE) && FSL_FEATURE_LLWU_HAS_RESET_ENABLE)
/*!
 * brief Sets the reset pin mode.
 *
 * This function determines how the reset pin is used as a low leakage mode exit source.
 *
 * param pinEnable       Enable reset the pin filter
 * param pinFilterEnable Specify whether the pin filter is enabled in Low-Leakage power mode.
 */
void LLWU_SetResetPinMode(LLWU_Type *base, bool pinEnable, bool pinFilterEnable)
{
    uint8_t reg;

    reg = base->RST;
    reg &= ~(LLWU_RST_LLRSTE_MASK | LLWU_RST_RSTFILT_MASK);
    reg |= (((uint32_t)pinEnable << LLWU_RST_LLRSTE_SHIFT) | ((uint32_t)pinFilterEnable << LLWU_RST_RSTFILT_SHIFT));
    base->RST = reg;
}
#endif /* FSL_FEATURE_LLWU_HAS_RESET_ENABLE */
