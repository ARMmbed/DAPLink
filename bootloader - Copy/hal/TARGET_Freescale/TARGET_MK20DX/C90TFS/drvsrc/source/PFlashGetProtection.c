/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  PFlashGetProtection.c                                *
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
*  Function Name    : PFlashGetProtection.c
*  Description      : This function retrieves current P-Flash protection status. 
*  Arguments        : PFLASH_SSD_CONFIG, UINT32*
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 PFlashGetProtection(PFLASH_SSD_CONFIG PSSDConfig, UINT32* protectStatus)
{
    UINT32 registerValue0;
    UINT32 registerValue1;
    UINT32 registerValue2;
    UINT32 registerValue3;
    
    registerValue0=(UINT32)(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT0_OFFSET));
    registerValue1=(UINT32)(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT1_OFFSET));
    registerValue2=(UINT32)(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT2_OFFSET));
    registerValue3=(UINT32)(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT3_OFFSET));
    
    *protectStatus = ((registerValue0 << 24 ) | \
                     (registerValue1 << 16 ) | \
                     (registerValue2 << 8 ) | \
                      registerValue3); 
                   
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