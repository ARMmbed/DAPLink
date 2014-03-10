/***********************************************************************/
/*  This file is part of the uVision/ARM development tools             */
/*  Copyright (c) 2003-2012 Keil Software. All rights reserved.        */
/***********************************************************************/
/*                                                                     */
/*  FlashEraseSector_Keil.C: Flash Programming Functions adapted for   */
/*                           Freescale MKL04 - Series                  */
/*                                                                     */
/***********************************************************************/

/* include the header files */
#include "SSD_Types.h"
#include "SSD_FTFx.h"
#include "SSD_FTFx_Internal.h"

/****************************************************************************
*
*  Function Name    : FlashEraseSector
*  Description      : Perform erase operation on Flash
*  Arguments        : PFLASH_SSD_CONFIG, UINT32, UINT32, pFLASHCOMMANDSEQUENCE
*  Return Value     : UINT32
*
*****************************************************************************/

UINT32 FlashEraseSector(PFLASH_SSD_CONFIG PSSDConfig, \
                                   UINT32 destination, \
                                   UINT32 size, \
                                   pFLASHCOMMANDSEQUENCE FlashCommandSequence)
{
    UINT8 pCommandArray[4]; /* command sequence array */
    UINT32 returnCode;      /* return code variable */
//    UINT32 endAddress;      /* storing end address */
    UINT32 sectorSize = size;        /* size of one sector */
    
    /* set the default return code as FTFx_OK */
    returnCode = FTFx_OK;

	/* check for error return code */
    if(FTFx_OK == returnCode)
    {

        while(size > 0)
        {
            /* preparing passing parameter to erase a flash block */
            pCommandArray[0] = FTFx_ERASE_SECTOR;
            pCommandArray[1] = (UINT8)(destination >> 16);
            pCommandArray[2] = (UINT8)((destination >> 8) & 0xFF);
            pCommandArray[3] = (UINT8)(destination & 0xFF);
              
            /* calling flash command sequence function to execute the command */
            returnCode = FlashCommandSequence(PSSDConfig, 3, pCommandArray);
            
            /* checking the success of command execution */
            if(FTFx_OK != returnCode)
            {
                break;
            }
            else
            {
                /* update size and destination address */
                size -= sectorSize;
                destination += sectorSize;
            }
        }
    }
    return(returnCode);
}
/* end of file */

