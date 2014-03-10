/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  FlashVerifyAllBlock.c                                *
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
*  Function Name    : FlashVerifyAllBlock.c
*  Description      : This function will check to see if the P-Flash 
*                     and D-Flash blocks as well as EERAM, E-Flash records
*                     and D-Flash IFR have been erased to the specifed read
*                     margin level, if applicable, and will release security
*                     if the readout passes.
*  Arguments        : PFLASH_SSD_CONFIG,UINT8, pFLASHCOMMANDSEQUENCE
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 FlashVerifyAllBlock(PFLASH_SSD_CONFIG PSSDConfig, \
                                                UINT8 marginLevel, \
                                                pFLASHCOMMANDSEQUENCE FlashCommandSequence)
{
    UINT8 pCommandArray[2]; /* command sequence array */
    UINT32 returnCode;      /* return code variable */
    
    /* set the default return code as FTFx_OK */
    returnCode = FTFx_OK;
        
    /* preparing passing parameter to verify all block command*/
    pCommandArray[0] = FTFx_VERIFY_ALL_BLOCK;
    pCommandArray[1] = marginLevel;
    
    /* calling flash command sequence function to execute the command */
    returnCode = FlashCommandSequence(PSSDConfig, 1, pCommandArray);

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