/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  FlashVerifyBlock.c                                   *
* DATE          :  Dec 27,2011                                          *
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
0.2.2       09.15.2011      FPT Team      Update the address alignment checking
0.2.3       12.27.2011      FPT Team      Update to support more Kinetis derivatives.
*************************************************************************/
/* include the header files */
#include "SSD_Types.h"
#include "SSD_FTFx.h"
#include "SSD_FTFx_Internal.h"

/************************************************************************
*
*  Function Name    : FlashVerifyBlock.c
*  Description      : This function will check to see if an entire 
*                     P-Flash or D-Flash block has been erased to the
*                     specifed margin level.
*  Arguments        : PFLASH_SSD_CONFIG, UINT32,UINT8, pFLASHCOMMANDSEQUENCE
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 FlashVerifyBlock(PFLASH_SSD_CONFIG PSSDConfig, \
                                   UINT32 destination, \
                                   UINT8 marginLevel, \
                                   pFLASHCOMMANDSEQUENCE FlashCommandSequence)
{
    UINT8 pCommandArray[5]; /* command sequence array */
    UINT32 returnCode;      /* return code variable */
    
    /* set the default return code as FTFx_OK */
    returnCode = FTFx_OK;
        
    /* check if the destination is longword aligned or not */
    if (0 != (destination % RD1BLK_ALIGN_SIZE))
    {
        /* return an error code FTFx_ERR_ADDR */
        returnCode = FTFx_ERR_ADDR;
        goto EXIT;
    }
    
    /* check for valid range of the target addresses */    
    if((destination < PSSDConfig->PFlashBlockBase) || \
        (destination >= (PSSDConfig->PFlashBlockBase + PSSDConfig->PFlashBlockSize)))
    {
        if((destination < PSSDConfig->DFlashBlockBase) || \
            (destination >= (PSSDConfig->DFlashBlockBase + PSSDConfig->DFlashBlockSize)))
        {
            /* return an error code FTFx_ERR_RANGE */
            returnCode = FTFx_ERR_RANGE;
            goto EXIT;
        }
        else
        {
            /* Convert System memory address to FTFx internal memory address */
            destination = destination - PSSDConfig->DFlashBlockBase + 0x800000;
        }
    }
    else
    {
        /* Convert System memory address to FTFx internal memory address */
        destination -= PSSDConfig->PFlashBlockBase;
    }
    
    /* preparing passing parameter to verify block command */
    pCommandArray[0] = FTFx_VERIFY_BLOCK;
    pCommandArray[1] = (UINT8)(destination >> 16);
    pCommandArray[2] = (UINT8)((destination >> 8) & 0xFF);
    pCommandArray[3] = (UINT8)(destination & 0xFF);
    pCommandArray[4] = marginLevel;

    /* calling flash command sequence function to execute the command */
    returnCode = FlashCommandSequence(PSSDConfig, 4, pCommandArray);

EXIT:
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