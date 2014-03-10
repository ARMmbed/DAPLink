/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  DEFlashPartition.c                                   *
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
*  Function Name    : DEFlashPartition.c
*  Description      : This function  prepares the D/E-Flash block for use
*                     as D-Flash, E-Flash or a combination of both and 
*                     initializes the EERAM.
*  Arguments        : PFLASH_SSD_CONFIG, UINT8,UINT8, pFLASHCOMMANDSEQUENCE
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 DEFlashPartition(PFLASH_SSD_CONFIG PSSDConfig, \
                        UINT8 EEEDataSizeCode, \
                        UINT8 DEPartitionCode, \
                        pFLASHCOMMANDSEQUENCE FlashCommandSequence)
{
    UINT8 pCommandArray[6]; /* command sequence array */
    UINT32 returnCode;      /* return code variable */

    /* set the default return code as FTFx_OK */
    returnCode = FTFx_OK;
    
    /* preparing passing parameter to program partition command */
    pCommandArray[0] = FTFx_PROGRAM_PARTITION;
    pCommandArray[4] = EEEDataSizeCode;
    pCommandArray[5] = DEPartitionCode;

    /* calling flash command sequence function to execute the command */
    returnCode = FlashCommandSequence(PSSDConfig, 5, pCommandArray);

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