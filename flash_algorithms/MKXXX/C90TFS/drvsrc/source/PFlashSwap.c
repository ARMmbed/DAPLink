/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  PFlashSwap.c                                         *
* DATE          :  April 20, 2011                                       *
*                                                                       *
* AUTHOR        :  FPT Team                                             *
* E-mail        :  b33626@freescale.com                                 *
*                                                                       *
*************************************************************************/

/************************** CHANGES *************************************
0.0.1       04.13.2011      FPT Team      Initial Version
0.0.2       04.20.2011      FPT Team      Fix bug when flash range is out
                                          of Pflash 0 (lower area)
0.1.0       04.27.2011      FPT Team      Add code to erase swap indicator
0.1.1       09.15.2011      FPT Team      Add code to convert sytem address
                                          to FTFx internal memory address
                                          Update the address alignment checking 
0.1.2       11.16.2011      FPT Team      Update the checking for transition condition
                                          Use _PFlashEraseSector() instead of FlashEraseSector()
                                          API in driver
0.1.3       12.23.2011      FPT Team      Update to support more Kinetis derivatives. 
*************************************************************************/
/* include the header files */
#include "SSD_Types.h"
#include "SSD_FTFx.h"
#include "SSD_FTFx_Internal.h"

/************************************************************************
*
*  Function Name    : _PFlashSwapCtl
*  Description      : Execute swap command represented by a control code
*                     
*                     
*  Arguments        : PFLASH_SSD_CONFIG, UINT32, UINT8,
*                     pFLASHCOMMANDSEQUENCE
*                     
*  Return Value     : UINT32
*
*************************************************************************/
static UINT32 _PFlashSwapCtl(PFLASH_SSD_CONFIG PSSDConfig, \
                             UINT32 addr, \
                             UINT8 swapcmd, \
                             pFLASHCOMMANDSEQUENCE FlashCommandSequence);

static UINT32 _PFlashEraseSector(PFLASH_SSD_CONFIG PSSDConfig, \
                             UINT32 destination, \
                             pFLASHCOMMANDSEQUENCE FlashCommandSequence);
/************************************************************************
*
*  Function Name    : PFlashSwap.c
*  Description      : Perform a swap between P-Flash block 0 and
*                     P-Flash block 1
*                     
*                     
*  Arguments        : PFLASH_SSD_CONFIG, UINT32, pFLASHCOMMANDSEQUENCE
*                     
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 PFlashSwap(PFLASH_SSD_CONFIG PSSDConfig, \
                  UINT32 flashAddress, \
                  pFLASHCOMMANDSEQUENCE FlashCommandSequence)
{
    UINT32 returnCode;      /* Return code */
    UINT8 currentSwapMode = 0xFF;

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

    /* Get current swap report */
    returnCode = _PFlashSwapCtl(PSSDConfig, flashAddress, FTFx_SWAP_REPORT_STATUS, FlashCommandSequence);

    /* Check for the success of command execution */
    if (FTFx_OK != returnCode)
    {
        goto EXIT;
    }

    /* Get current swap mode from FCCOB5 register */
    currentSwapMode = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB5_OFFSET);
    
    /* If current swap mode is Uninitialized */
    if (FTFx_SWAP_UNINIT == currentSwapMode)
    {
        /* Initialize Swap to Initialized/READY state */
        returnCode = _PFlashSwapCtl(PSSDConfig, flashAddress, FTFx_SWAP_SET_INDICATOR_ADDR, FlashCommandSequence);

        /* Check for the success of command execution */
        if (FTFx_OK != returnCode)
        {
            goto EXIT;
        }
        
        /* Wait for state transition from Uninitialized to others*/
        while ((currentSwapMode = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB5_OFFSET)) == FTFx_SWAP_UNINIT);
    }
    
    /* If current swap mode is Initialized/Ready */
    if (FTFx_SWAP_READY == currentSwapMode)
    {
        /* Progress Swap to UPDATE state */
        returnCode = _PFlashSwapCtl(PSSDConfig, flashAddress, FTFx_SWAP_SET_IN_PREPARE, FlashCommandSequence);

        /* Check for the success of command execution */
        if (FTFx_OK != returnCode)
        {
            goto EXIT;
        }
        
        /* Wait for state transition from Initialized/Ready to others */
        while ((currentSwapMode = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB5_OFFSET)) == FTFx_SWAP_READY);
    }

    /* If current swap mode is Update or Update-Erased */
    if ((FTFx_SWAP_UPDATE == currentSwapMode) || (FTFx_SWAP_UPDATE_ERASED == currentSwapMode))
    {
        if (FTFx_SWAP_UPDATE_ERASED != currentSwapMode)
        {
            /* If the current swap mode is not update-erase, erase swap indicator in non-active block 
            by erasing the sector contains the swap indicator */
            returnCode = _PFlashEraseSector(PSSDConfig, \
                                          flashAddress + (PSSDConfig->PFlashBlockSize) / 2, \
                                          FlashCommandSequence);
            if (FTFx_OK != returnCode)
            {
                goto EXIT;
            }
        }
        /* Progress Swap to COMPLETE State */
        returnCode = _PFlashSwapCtl(PSSDConfig, flashAddress, FTFx_SWAP_SET_IN_COMPLETE, FlashCommandSequence);

        /* Check for the success of command execution */
        if (FTFx_OK != returnCode)
        {
            goto EXIT;
        }

        /* Wait for state transition from Update-Erased to Complete */
        while ((currentSwapMode = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB5_OFFSET)) != FTFx_SWAP_COMPLETE);
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

/* Function to execute the swap control code */
static UINT32 _PFlashSwapCtl(PFLASH_SSD_CONFIG PSSDConfig, \
                             UINT32 addr, \
                             UINT8 swapcmd, \
                             pFLASHCOMMANDSEQUENCE FlashCommandSequence)
{
    UINT8 pCommandArray[8]; /* Command sequence array */
    UINT32 returnCode;      /* Return code variable */

    /* Set the default return code as FTFx_OK */
    returnCode = FTFx_OK;

    /* Preparing passing parameter to execute the swap control code */
    pCommandArray[0] = FTFx_PFLASH_SWAP;
    pCommandArray[1] = (UINT8)((addr & 0x00FF0000) >> 16);
    pCommandArray[2] = (UINT8)((addr & 0x0000FF00) >> 8);
    pCommandArray[3] = (UINT8)(addr & 0x000000FF);
    pCommandArray[4] = swapcmd;
    pCommandArray[5] = 0xFF;
    pCommandArray[6] = 0xFF;
    pCommandArray[7] = 0xFF;

    /* Call FlashCommandSequence to execute the command code */
    returnCode = FlashCommandSequence(PSSDConfig, 7, pCommandArray);

    return (returnCode);
}

/* Function to execute Pflash erase sector */
static UINT32 _PFlashEraseSector(PFLASH_SSD_CONFIG PSSDConfig, \
                             UINT32 destination, \
                             pFLASHCOMMANDSEQUENCE FlashCommandSequence)
{
    UINT8 pCommandArray[4]; /* Command sequence array */
    UINT32 returnCode;      /* Return code variable */

    /* Set the default return code as FTFx_OK */
    returnCode = FTFx_OK;

    /* preparing passing parameter to erase a flash block */
    pCommandArray[0] = FTFx_ERASE_SECTOR;
    pCommandArray[1] = (UINT8)(destination >> 16);
    pCommandArray[2] = (UINT8)((destination >> 8) & 0xFF);
    pCommandArray[3] = (UINT8)(destination & 0xFF);
      
    /* calling flash command sequence function to execute the command */
    returnCode = FlashCommandSequence(PSSDConfig, 3, pCommandArray);

    return (returnCode);
}

/* end of file */
