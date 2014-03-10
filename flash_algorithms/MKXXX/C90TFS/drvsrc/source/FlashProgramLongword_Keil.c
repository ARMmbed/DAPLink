/*************************************************************************/
/*  This file is part of the uVision/ARM development tools               */
/*  Copyright (c) 2003-2012 Keil Software. All rights reserved.          */
/*************************************************************************/
/*                                                                       */
/*  FlashProgramLongWord_Keil.C: Flash Programming Functions adapted for */
/*                               Freescale MKL04 - Series                */
/*                                                                       */
/*************************************************************************/
/* include the header files */
#include "SSD_Types.h"
#include "SSD_FTFx.h"
#include "SSD_FTFx_Internal.h"

/************************************************************************
*
*  Function Name    : FlashProgramLongword.c
*  Description      : Program data into Flash
*  Arguments        : PFLASH_SSD_CONFIG, UINT32, UINT32, UINT32,
*                     pFLASHCOMMANDSEQUENCE
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 FlashProgramLongword(PFLASH_SSD_CONFIG PSSDConfig, \
                                       UINT32 destination, \
                                       UINT32 size, \
                                       UINT32 source, \
                                       pFLASHCOMMANDSEQUENCE FlashCommandSequence)
{
    UINT8 pCommandArray[8]; /* command sequence array */
    UINT32 returnCode;      /* return code variable */
 //   UINT32 endAddress;      /* storing end address */
    
    /* set the default return code as FTFx_OK */
    returnCode = FTFx_OK;
    size = (size + FTFx_LONGWORD_SIZE) & ~FTFx_LONGWORD_SIZE;    
    destination -= PSSDConfig->PFlashBlockBase;
    /* check for error return code */
    if(FTFx_OK == returnCode)
    {
        while(size > 0)
        {
            /* preparing passing parameter to program the flash block */
            pCommandArray[0] = FTFx_PROGRAM_LONGWORD;
            pCommandArray[1] = (UINT8)(destination >> 16);
            pCommandArray[2] = (UINT8)((destination >> 8) & 0xFF);
            pCommandArray[3] = (UINT8)(destination & 0xFF);
        
#if (ENDIANNESS == BIG_ENDIAN)  /* Big Endian */ 
            pCommandArray[4] = READ8(source);
            pCommandArray[5] = READ8(source + 1);
            pCommandArray[6] = READ8(source + 2);
            pCommandArray[7] = READ8(source + 3);
#else /* Little Endian */
            pCommandArray[4] = READ8(source + 3);
            pCommandArray[5] = READ8(source + 2);
            pCommandArray[6] = READ8(source + 1);
            pCommandArray[7] = READ8(source);
#endif
            
            /* calling flash command sequence function to execute the command */
            returnCode = FlashCommandSequence(PSSDConfig, 7, pCommandArray);
    
            /* checking for the success of command execution */
            if(FTFx_OK != returnCode)
            {
                break;
            }
            else
            {
                /* update destination address for next iteration */
                destination += FTFx_LONGWORD_SIZE;
                /* update size for next iteration */
                size -= FTFx_LONGWORD_SIZE;
                        /* increment the source adress by 1 */
                source += FTFx_LONGWORD_SIZE;
            }
        }
    }
    return(returnCode);
}
/* end of file */


