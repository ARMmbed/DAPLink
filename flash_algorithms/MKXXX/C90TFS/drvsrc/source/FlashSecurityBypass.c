/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  FlashSecurityBypass.c                                *
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
*  Function Name    : FlashSecurityBypass.c
*  Description      : If the MCU is secured state, this function will 
*                     unsecure the MCU by comparing the provided backdoor
*                     key with ones in the Flash Configuration Field.
*  Arguments        : PFLASH_SSD_CONFIG, UINT8*, pFLASHCOMMANDSEQUENCE
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 FlashSecurityBypass(PFLASH_SSD_CONFIG PSSDConfig, \
                                                UINT8* keyBuffer, \
                                                pFLASHCOMMANDSEQUENCE FlashCommandSequence)
{
    UINT8 pCommandArray[12]; /* command sequence array */
    UINT8 registerValue;    /* registerValue */
    UINT32 returnCode;      /* return code variable */

    /* set the default return code as FTFx_OK */
    returnCode = FTFx_OK;

    /*Get flash security register value */
    registerValue = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FSEC_OFFSET);
    
    if (0x02 != (registerValue & 0x03)) 
    {
        /* preparing passing parameter to erase a flash block */
        pCommandArray[0] = FTFx_SECURITY_BY_PASS;
        pCommandArray[4] = keyBuffer[0];
        pCommandArray[5] = keyBuffer[1];
        pCommandArray[6] = keyBuffer[2];
        pCommandArray[7] = keyBuffer[3];
        pCommandArray[8] = keyBuffer[4];
        pCommandArray[9] = keyBuffer[5];
        pCommandArray[10] = keyBuffer[6];
        pCommandArray[11] = keyBuffer[7];
        
        /* calling flash command sequence function to execute the command */
        returnCode = FlashCommandSequence(PSSDConfig, 11, pCommandArray);
    }

    /* Giving Control to BDM if enabled */
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

    return(returnCode);
}
/* end of file */