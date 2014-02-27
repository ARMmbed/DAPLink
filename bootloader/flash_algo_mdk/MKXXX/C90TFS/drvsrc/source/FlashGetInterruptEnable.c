/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  FlashGetInterruptEnable.c                            *
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
*  Function Name    : FlashGetInterruptEnable
*  Description      : This function will read the FCNFG register and 
*                     return the interrupt enable states for Flash 
*                     interrupt types.
*  Arguments        : PFLASH_SSD_CONFIG, UINT8*
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 FlashGetInterruptEnable(PFLASH_SSD_CONFIG PSSDConfig, UINT8* interruptState)
{
    /* store data read from flash register */
    UINT8  registerValue;

    /* Initial interrupt state value */
    *interruptState = 0;

    /*Get flash configration register value */
    registerValue = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FCNFG_OFFSET);

    /* Add command complete interrupt enable bit value */
    /* CCIE bit of the FCNFG register */
    *interruptState |= (registerValue & (FTFx_SSD_FCNFG_CCIE | FTFx_SSD_FCNFG_RDCOLLIE));

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

