/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  FlashGetSecurityState.c                              *
* DATE          :  January 28,2011                                      *
*                                                                       *
* AUTHOR        :  FPT Team                                             *
* E-mail        :  b33626@freescale.com                                 *
*                                                                       *
*************************************************************************/

/************************** CHANGES *************************************
0.0.1       06.09.2010      FPT Team      Initial Version
0.1.0       06.11.2010      FPT Team      Finalize to 0.1.0 
0.1.3       09.16.2010      FPT Team      Finalize to 0.1.3 
0.2.0       09.27.2010      FPT Team      Finalize to 0.2.0
0.2.1       01.28.2011      FPT Team      Updated the preprocesor 
                                          condition to enter Debug state
*************************************************************************/
/* include the header files */
#include "SSD_Types.h"
#include "SSD_FTFx.h"
#include "SSD_FTFx_Internal.h"

/************************************************************************
*
*  Function Name    : FlashGetSecurityState.c
*  Description      : This function retrieves the current Flash security 
*                     status, including the security enabling state and 
*                     the backdoor key enabling state.
*  Arguments        : PFLASH_SSD_CONFIG, UINT8* 
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 FlashGetSecurityState(PFLASH_SSD_CONFIG PSSDConfig, UINT8* securityState)
{
    /* store data read from flash register */
    UINT8  registerValue;

    /*Get flash security register value */
    registerValue = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FSEC_OFFSET);

    /* check the status of the flash security bits in the security register */
    if(FLASH_SECURITY_STATE_UNSECURED == (registerValue & FTFx_SSD_FSEC_SEC))
    {
        /* Flash in unsecured state */
        *securityState = FLASH_NOT_SECURE;
    }
    else
    {
        /* Flash in secured state */
        /* check for backdoor key security enable bit */
        if(FLASH_SECURITY_STATE_KEYEN == (registerValue & FTFx_SSD_FSEC_KEYEN))
        {
            /* Backdoor key security enabled */
            *securityState = FLASH_SECURE_BACKDOOR_ENABLED;
        }
        else
        {
            /* Backdoor key security disabled */
            *securityState = FLASH_SECURE_BACKDOOR_DISABLED;
        }
    }

    /* Enter Debug state if enabled */
    if (TRUE == (PSSDConfig->DebugEnable))
    {
#if ((CPU_CORE == ARM_CM4) && (COMPILER == IAR))    /* Kx Products */
        asm
        (
            " BKPT #0 \n "           /* enter Debug state */
        );
#endif
    }

    return(FTFx_OK);
}
/* end of file */
