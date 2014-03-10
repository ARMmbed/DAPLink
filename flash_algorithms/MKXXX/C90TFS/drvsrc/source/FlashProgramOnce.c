/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  FlashProgramOnce.c                                   *
* DATE          :  Dec 27,2011                                          *
*                                                                       *
* AUTHOR        :  FPT Team                                             *
* E-mail        :  b33626@freescale.com                                 *
*                                                                       *
*************************************************************************/

/************************** CHANGES *************************************
0.0.1       06.09.2010      FPT Team      Initial Version
0.1.0       06.11.2010      FPT Team      Finalize to 0.1.0 
0.1.3       09.16.2010      FPT Team      Updated to support little endian 
0.2.0       09.27.2010      FPT Team      Finalize to 0.2.0
0.2.1       01.28.2011      FPT Team      Updated the preprocesor 
                                          condition to enter Debug state
0.2.2       02.21.2011      FPT Team      Removed endianness condition
                                          preprocessor
0.2.3       09.15.2011      FPT Team      Update to support K70         
0.2.4       11.15.2011      FPT Team      Update to support FTFx_KX_1024K_0K_16K_4K_0K
                                          derivative.       
0.2.5       12.27.2011      FPT Team      Update to support more Kinetis derivatives.
*************************************************************************/
/* include the header files */
#include "SSD_Types.h"
#include "SSD_FTFx.h"
#include "SSD_FTFx_Internal.h"

/************************************************************************
*
*  Function Name    : FlashProgramOnce.c
*  Description      : Program data into a dedicated 64 bytes region in
*                     the P-Flash IFR which stores critical information
*                     for the user
*  Arguments        : PFLASH_SSD_CONFIG, UINT8*, pFLASHCOMMANDSEQUENCE
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 FlashProgramOnce(PFLASH_SSD_CONFIG PSSDConfig, \
                                   UINT8* pDataArray, \
                                   pFLASHCOMMANDSEQUENCE FlashCommandSequence)
{
    UINT8  counter;          /* outer loop counter */
#if ((FTFx_KX_512K_512K_16K_4K_4K == FLASH_DERIVATIVE) || (FTFx_KX_1024K_0K_16K_4K_0K == FLASH_DERIVATIVE)) //Added by ARM
    UINT8  pCommandArray[12];/* command sequence array */
#else
    UINT8  pCommandArray[8]; /* command sequence array */
#endif
    UINT32 returnCode;       /* return code variable */
    UINT8 i;                 /* variable to store record number index */

    /* set the default return code as FTFx_OK */
    returnCode = FTFx_OK;
    /* set the counter value for 0 */
    counter = 0;
    /* calculate record number index */
    i = (64 / (RDONCE_INDEX_MAX + 1));

    /* preparing passing parameter for program once command */
    /* 1st element for the FCCOB register */
    pCommandArray[0] = FTFx_PROGRAM_ONCE;

    while (counter <= RDONCE_INDEX_MAX)
    {
        /* preparing the rest of the parameters to be passed for the FCCOB registers */
        pCommandArray[1] = counter;
        pCommandArray[2] = 0x00;
        pCommandArray[3] = 0x00;
        pCommandArray[4] = pDataArray[counter*i];
        pCommandArray[5] = pDataArray[counter*i + 1];
        pCommandArray[6] = pDataArray[counter*i + 2];
        pCommandArray[7] = pDataArray[counter*i + 3];
#if ((FTFx_KX_512K_512K_16K_4K_4K == FLASH_DERIVATIVE) || (FTFx_KX_1024K_0K_16K_4K_0K == FLASH_DERIVATIVE))
        pCommandArray[8] = pDataArray[counter*i + 4];
        pCommandArray[9] = pDataArray[counter*i + 5];
        pCommandArray[10] = pDataArray[counter*i + 6];
        pCommandArray[11] = pDataArray[counter*i + 7];
#endif

        /* calling flash command sequence API to execute the command */
        returnCode = FlashCommandSequence(PSSDConfig, i + 3, pCommandArray);

        /* checking for the success of command execution */
        if(FTFx_OK != returnCode)
        {
            break;
        }
        else
        {
            /* do nothing */
        }
        /* decrement the counter value */
        counter++;
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

    return(returnCode);
}
/* end of file */
