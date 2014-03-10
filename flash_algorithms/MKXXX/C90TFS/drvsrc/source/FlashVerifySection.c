/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  FlashVerifySection.c                                 *
* DATE          :  Dec 27,2011                                          *
*                                                                       *
* AUTHOR        :  FPT Team                                             *
* E-mail        :  b33626@freescale.com                                 *
*                                                                       *
*************************************************************************/

/************************** CHANGES *************************************
0.0.1       06.09.2010      FPT Team      Initial Version
0.1.0       06.11.2010      FPT Team      Finalize to 0.1.0 
0.1.3       09.16.2010      FPT Team      Changed type of AlignedChecking 
                                          variable from UINT32 to UINT8 
0.2.0       09.27.2010      FPT Team      Finalize to 0.2.0
0.2.1       01.28.2011      FPT Team      Used SECTION_ALIGN_SIZE instead
                                          of AlignedChecking variable for
                                          checking alignment
0.2.2      12.27.2011        FPT Team     Update to support more Kinetis derivatives.                                                      
*************************************************************************/
/* include the header files */
#include "SSD_Types.h"
#include "SSD_FTFx.h"
#include "SSD_FTFx_Internal.h"

/************************************************************************
*
*  Function Name    : FlashVerifySection.c
*  Description      : This function will check to see if a section of
*                                P-Flash or D-Flash memory is erased to the specifed 
*                                read margin level.
*  Arguments        : PFLASH_SSD_CONFIG,UINT32,UINT16,UINT8, pFLASHCOMMANDSEQUENCE
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 FlashVerifySection(PFLASH_SSD_CONFIG PSSDConfig, \
                                    UINT32 destination, \
                                    UINT16 Number, \
                                    UINT8 marginLevel, \
                                    pFLASHCOMMANDSEQUENCE FlashCommandSequence)
{
    UINT8 pCommandArray[7]; /* command sequence array */
    UINT32 returnCode;      /* return code variable */
    UINT32 endAddress;      /* storing end address. This is the starting point of the next block */   
    UINT32 sectionAlign;
    
    /* set the default return code as FTFx_OK */
    returnCode = FTFx_OK;
    
    /* check for valid range of the destination */
    if((destination >= PSSDConfig->PFlashBlockBase) && \
        (destination < (PSSDConfig->PFlashBlockBase + PSSDConfig->PFlashBlockSize)))
    {
        /* calculating Flash end address */
        endAddress = destination + Number * PRD1SEC_ALIGN_SIZE;
    }
    else if((destination >= PSSDConfig->DFlashBlockBase) && \
            (destination < (PSSDConfig->DFlashBlockBase + PSSDConfig->DFlashBlockSize)))
    {
        /* calculating Flash end address */
        endAddress = destination + Number * DRD1SEC_ALIGN_SIZE;
    }
    else
    {
        /* return an error code FTFx_ERR_RANGE */
        returnCode = FTFx_ERR_RANGE;
        goto EXIT;
    }
            
    /* check for valid range of end address */
    if((endAddress >= PSSDConfig->PFlashBlockBase) && \
        (endAddress <= (PSSDConfig->PFlashBlockBase + PSSDConfig->PFlashBlockSize)))
    {
        /* Convert System memory address to FTFx internal memory address */
        destination -= PSSDConfig->PFlashBlockBase;
        sectionAlign = PRD1SEC_ALIGN_SIZE;
    }
    else if((endAddress >= PSSDConfig->DFlashBlockBase) || \
            (endAddress <= (PSSDConfig->DFlashBlockBase + PSSDConfig->DFlashBlockSize)))
    {
        /* Convert System memory address to FTFx internal memory address */
        destination = destination - PSSDConfig->DFlashBlockBase + 0x800000;
        sectionAlign = DRD1SEC_ALIGN_SIZE;
    }    
    else
    {
        /* return an error code FTFx_ERR_RANGE */
        returnCode = FTFx_ERR_RANGE;
        goto EXIT;
    }
    
    /* check if the destination is aligned or not */
    if (0 != (destination % sectionAlign))
    {
        /* return an error code FTFx_ERR_ADDR */
        returnCode = FTFx_ERR_ADDR;
        goto EXIT;
    }
        
    /* preparing passing parameter to verify section command */
    pCommandArray[0] = FTFx_VERIFY_SECTION;
    pCommandArray[1] = (UINT8)(destination >> 16);
    pCommandArray[2] = (UINT8)((destination >> 8) & 0xFF);
    pCommandArray[3] = (UINT8)(destination & 0xFF);
    pCommandArray[4] = (UINT8)(Number >> 8);
    pCommandArray[5] = (UINT8)(Number & 0xFF);
    pCommandArray[6] = marginLevel;

    /* calling flash command sequence function to execute the command */
    returnCode = FlashCommandSequence(PSSDConfig, 6, pCommandArray);

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