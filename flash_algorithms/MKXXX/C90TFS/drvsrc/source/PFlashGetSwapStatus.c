/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  PFlashGetSwapStatus.c                                *
* DATE          :  Dec 27, 2011                                         *
*                                                                       *
* AUTHOR        :  FPT Team                                             *
* E-mail        :  b33626@freescale.com                                 *
*                                                                       *
*************************************************************************/

/************************** CHANGES *************************************
0.0.1       04.13.2011      FPT Team      Initial Version
0.0.2       04.20.2011      FPT Team      Fix bug when flash range is out
                                          of Pflash 0 (lower area)
0.1.0       04.28.2011      FPT Team      Finalize to this version    
0.1.1       09.15.2011      FPT Team      Add code to convert sytem address
                                          to FTFx internal memory address
                                          Update the address alignment checking 
0.1.2       12.27.2011      FPT Team      Update to support more Kinetis derivatives.
*************************************************************************/
/* include the header files */
#include "SSD_Types.h"
#include "SSD_FTFx.h"
#include "SSD_FTFx_Internal.h"

/************************************************************************
*
*  Function Name    : PFlashGetSwapStatus.c
*  Description      : Retrieve current swap status from the swap system
*                     
*                     
*  Arguments        : PFLASH_SSD_CONFIG, UINT32, UINT8*, UINT8*, UINT8*
*                     pFLASHCOMMANDSEQUENCE
*                     
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 PFlashGetSwapStatus(PFLASH_SSD_CONFIG PSSDConfig, \
                                UINT32 flashAddress, \
                                UINT8* pCurrentSwapMode, \
                                UINT8* pCurrentSwapBlockStatus, \
                                UINT8* pNextSwapBlockStatus, \
                                pFLASHCOMMANDSEQUENCE FlashCommandSequence)
{
    UINT8 pCommandArray[8]; /* Command sequence array */
    UINT32 returnCode;      /* Return code */
    
    /* Set the default return code as FTFx_OK */
    returnCode = FTFx_OK;
    
    /* Check if the destination is not Longword aligned */
    if (0 != (flashAddress % SWAP_ALIGN_SIZE))
    {
        /* Return error code FTFx_ERR_ADDR */
        returnCode = FTFx_ERR_ADDR;
        goto EXIT;
    }
    
    /* Check if the flash address is not in the P-Flash block 0 */
    if (!(flashAddress >= PSSDConfig->PFlashBlockBase) || \
        !(flashAddress < (PSSDConfig->PFlashBlockBase + (PSSDConfig->PFlashBlockSize / 2))))
    {
        /* Return error code FTFx_ERR_RANGE */
        returnCode = FTFx_ERR_RANGE;
        goto EXIT;
    }
    /* Convert System memory address to FTFx internal memory address */
    flashAddress -= PSSDConfig->PFlashBlockBase;
    /* Check if the flash address is in the P-Flash block 0 but is in Flash Configuration Field */
    if ((flashAddress >= FLASH_CNFG_START_ADDRESS) && (flashAddress <= FLASH_CNFG_END_ADDRESS))
    {
        /* Return error code FTFx_ERR_RANGE */
        returnCode = FTFx_ERR_RANGE;
        goto EXIT;
    }
    
    /* Prepare passing parameter */
    pCommandArray[0] = FTFx_PFLASH_SWAP;
    pCommandArray[1] = (UINT8)(flashAddress >> 16);
    pCommandArray[2] = (UINT8)((flashAddress >> 8) & 0xFF);
    pCommandArray[3] = (UINT8)(flashAddress & 0xFF);
    pCommandArray[4] = FTFx_SWAP_REPORT_STATUS; /* Swap control code */
    pCommandArray[5] = 0xFF;
    pCommandArray[6] = 0xFF;
    pCommandArray[7] = 0xFF;

    /* Call flash command sequence function to execute the command */
    returnCode = FlashCommandSequence(PSSDConfig, 7, pCommandArray);

    /* Check for the success of command execution */
    if (FTFx_OK == returnCode)
    {
        *pCurrentSwapMode = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB5_OFFSET);
        *pCurrentSwapBlockStatus = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB6_OFFSET);
        *pNextSwapBlockStatus = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB7_OFFSET);

        goto EXIT;
    }

EXIT:
    /* Enter Debug state if enabled */
    if (TRUE == (PSSDConfig->DebugEnable))
    {
#if ((CPU_CORE == ARM_CM4) && (COMPILER == IAR))    /* Kx Products */
        asm
        (
            " BKPT #0 \n "           /* Enter Debug state */
        );
#endif
    }

    return(returnCode);
}

/* end of file */


