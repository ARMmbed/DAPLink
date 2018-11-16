/*!
    \file  gd32f30x_pmu.c
    \brief PMU driver
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-02-10, V1.0.0, firmware for GD32F30x
*/

#include "gd32f30x_pmu.h"

/*!
    \brief      reset PMU register
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_deinit(void)
{
    /* reset PMU */
    rcu_periph_reset_enable(RCU_PMURST);
    rcu_periph_reset_disable(RCU_PMURST);
}

/*!
    \brief      select low voltage detector threshold
    \param[in]  lvdt_n:
      \arg        PMU_LVDT_0: voltage threshold is 2.1V
      \arg        PMU_LVDT_1: voltage threshold is 2.3V
      \arg        PMU_LVDT_2: voltage threshold is 2.4V
      \arg        PMU_LVDT_3: voltage threshold is 2.6V
      \arg        PMU_LVDT_4: voltage threshold is 2.7V
      \arg        PMU_LVDT_5: voltage threshold is 2.9V
      \arg        PMU_LVDT_6: voltage threshold is 3.0V
      \arg        PMU_LVDT_7: voltage threshold is 3.1V
    \param[out] none
    \retval     none
*/
void pmu_lvd_select(uint32_t lvdt_n)
{
    /* disable LVD */
    PMU_CTL &= ~PMU_CTL_LVDEN;
    /* clear LVDT bits */
    PMU_CTL &= ~PMU_CTL_LVDT;
    /* set LVDT bits according to lvdt_n */
    PMU_CTL |= lvdt_n;
    /* enable LVD */
    PMU_CTL |= PMU_CTL_LVDEN;
}

/*!
    \brief      select LDO output voltage
                this bit set by software when the main PLL closed, before closing PLL, change the system clock to IRC16M or HXTAL
    \param[in]  ldo_output:
      \arg        PMU_LDOVS_LOW: low-driver mode enable in deep-sleep mode
      \arg        PMU_LDOVS_MID: low-driver mode disable in deep-sleep mode
      \arg        PMU_LDOVS_HIGH: low-driver mode disable in deep-sleep mode
    \param[out] none
    \retval     none
*/
void pmu_ldo_output_select(uint32_t ldo_output)
{
    PMU_CTL &= ~PMU_CTL_LDOVS;
    PMU_CTL |= ldo_output;
}

/*!
    \brief      switch high-driver mode
                this bit set by software only when IRC16M or HXTAL used as system clock
    \param[in]  highdr_switch:
      \arg        PMU_HIGHDR_SWITCH_NONE: disable high-driver mode switch
      \arg        PMU_HIGHDR_SWITCH_EN: enable high-driver mode switch
    \param[out] none
    \retval     none
*/
void pmu_highdriver_switch_select(uint32_t highdr_switch)
{
    /* wait for HDRF flag set */
    while(SET != pmu_flag_get(PMU_FLAG_HDRF)){
    }
    PMU_CTL &= ~PMU_CTL_HDS;
    PMU_CTL |= highdr_switch;
}

/*!
    \brief      enable low-driver mode in deep-sleep mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_lowdriver_mode_enable(void)
{
    PMU_CTL |= PMU_CTL_LDEN;
}

/*!
    \brief      disable low-driver mode in deep-sleep mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_lowdriver_mode_disable(void)
{
    PMU_CTL &= ~PMU_CTL_LDEN;
}

/*!
    \brief      enable high-driver mode
                this bit set by software only when IRC16M or HXTAL used as system clock
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_highdriver_mode_enable(void)
{
    PMU_CTL |= PMU_CTL_HDEN;
}

/*!
    \brief      disable high-driver mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_highdriver_mode_disable(void)
{
    PMU_CTL &= ~PMU_CTL_HDEN;
}

/*!
    \brief      disable PMU lvd
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_lvd_disable(void)
{
    /* disable LVD */
    PMU_CTL &= ~PMU_CTL_LVDEN;
}

/*!
    \brief      driver mode when use low power LDO
    \param[in]  mode:
      \arg        PMU_NORMALDR_LOWPWR:  normal driver when use low power LDO
      \arg        PMU_LOWDR_LOWPWR:  low-driver mode enabled when LDEN is 11 and use low power LDO
    \param[out] none
    \retval     none
*/
void pmu_lowpower_driver_config(uint32_t mode)
{
    PMU_CTL &= ~PMU_CTL_LDLP;
    PMU_CTL |= mode;
}

/*!
    \brief      driver mode when use normal power LDO
    \param[in]  mode:
      \arg        PMU_NORMALDR_NORMALPWR:  normal driver when use low power LDO
      \arg        PMU_LOWDR_NORMALPWR:  low-driver mode enabled when LDEN is 11 and use low power LDO
    \param[out] none
    \retval     none
*/
void pmu_normalpower_driver_config(uint32_t mode)
{
    PMU_CTL &= ~PMU_CTL_LDNP;
    PMU_CTL |= mode;
}

/*!
    \brief      PMU work at sleep mode
    \param[in]  sleepmodecmd:
      \arg        WFI_CMD: use WFI command
      \arg        WFE_CMD: use WFE command
    \param[out] none
    \retval     none
*/
void pmu_to_sleepmode(uint8_t sleepmodecmd)
{
    /* clear sleepdeep bit of Cortex-M4 system control register */
    SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
    
    /* select WFI or WFE command to enter sleep mode */
    if(WFI_CMD == sleepmodecmd){
        __WFI();
    }else{
        __WFE();
    }
}

/*!
    \brief      PMU work at deepsleep mode
    \param[in]  ldo
      \arg        PMU_LDO_NORMAL: LDO normal work when pmu enter deepsleep mode
      \arg        PMU_LDO_LOWPOWER: LDO work at low power mode when pmu enter deepsleep mode
    \param[in]  deepsleepmodecmd: 
      \arg        WFI_CMD: use WFI command
      \arg        WFE_CMD: use WFE command
    \param[out] none
    \retval     none
*/
void pmu_to_deepsleepmode(uint32_t ldo,uint8_t deepsleepmodecmd)
{
    /* clear stbmod and ldolp bits */
    PMU_CTL &= ~((uint32_t)(PMU_CTL_STBMOD | PMU_CTL_LDOLP));
    
    /* set ldolp bit according to pmu_ldo */
    PMU_CTL |= ldo;
    
    /* set sleepdeep bit of Cortex-M4 system control register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    
    /* select WFI or WFE command to enter deepsleep mode */
    if(WFI_CMD == deepsleepmodecmd){
        __WFI();
    }else{
        __SEV();
        __WFE();
        __WFE();
    }
    /* reset sleepdeep bit of Cortex-M4 system control register */
    SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
}

/*!
    \brief      pmu work at standby mode
    \param[in]  standbymodecmd:
      \arg        WFI_CMD: use WFI command
      \arg        WFE_CMD: use WFE command
    \param[out] none
    \retval     none
*/
void pmu_to_standbymode(uint8_t standbymodecmd)
{
    /* set sleepdeep bit of Cortex-M4 system control register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* set stbmod bit */
    PMU_CTL |= PMU_CTL_STBMOD;
        
    /* reset wakeup flag */
    PMU_CTL |= PMU_CTL_WURST;
    
    /* select WFI or WFE command to enter standby mode */
    if(WFI_CMD == standbymodecmd){
        __WFI();
    }else{
        __WFE();
    }
}

/*!
    \brief      clear flag bit
    \param[in]  flag_reset:
      \arg        PMU_FLAG_RESET_WAKEUP: reset wakeup flag
      \arg        PMU_FLAG_RESET_STANDBY: reset standby flag
    \param[out] none
    \retval     none
*/
void pmu_flag_clear(uint32_t flag_reset)
{
    switch(flag_reset){
    case PMU_FLAG_RESET_WAKEUP:
        /* reset wakeup flag */
        PMU_CTL |= PMU_CTL_WURST;
        break;
    case PMU_FLAG_RESET_STANDBY:
        /* reset standby flag */
        PMU_CTL |= PMU_CTL_STBRST;
        break;
    default :
        break;
    }
}

/*!
    \brief      get flag state
    \param[in]  flag:
      \arg        PMU_FLAG_WAKEUP: wakeup flag
      \arg        PMU_FLAG_STANDBY: standby flag
      \arg        PMU_FLAG_LVD: lvd flag
      \arg        PMU_FLAG_LDOVSRF: LDO voltage select ready flag
      \arg        PMU_FLAG_HDRF: high-driver ready flag
      \arg        PMU_FLAG_HDSRF: high-driver switch ready flag
      \arg        PMU_FLAG_LDRF: low-driver mode ready flag 
    \param[out] none
    \retval     FlagStatus SET or RESET
*/
FlagStatus pmu_flag_get(uint32_t flag)
{
    if(PMU_CS & flag){
        return  SET;
    }else{
        return  RESET;
    }
}

/*!
    \brief      enable backup domain write
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_backup_write_enable(void)
{
    PMU_CTL |= PMU_CTL_BKPWEN;
}

/*!
    \brief      disable backup domain write
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_backup_write_disable(void)
{
    PMU_CTL &= ~PMU_CTL_BKPWEN;
}

/*!
    \brief      enable wakeup pin
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_wakeup_pin_enable(void)
{
    PMU_CS |= PMU_CS_WUPEN;
}

/*!
    \brief      disable wakeup pin
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_wakeup_pin_disable(void)
{
    PMU_CS &= ~PMU_CS_WUPEN;
}
