/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  FlashCheckSum.c                                      *
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
0.2.2       02.21.2011      FPT Team      Fixed boundary condition
************************************************************************/
/* include the header files */
#include "SSD_Types.h"
#include "SSD_FTFx.h"
#include "SSD_FTFx_Internal.h"

/***********************************************************************
*
*  Function Name    : FlashCheckSum.c
*  Description      : This function is used to calculate checksum value 
*                     for the specified flash range.
*  Arguments        : PFLASH_SSD_CONFIG,UINT32 ,UINT32 ,UINT32*
*  Return Value     : UINT32
*
************************************************************************/

UINT32 FlashCheckSum(PFLASH_SSD_CONFIG PSSDConfig, \
                                UINT32 destination, \
                                UINT32 size, \
                                UINT32* PSum)
{
    UINT32 counter;          /* Counter for callback operation */
    UINT32 data;             /* Data read from Flash address */
    UINT32 returnCode;       /* Return code variable */
    UINT32 endAddress;       /* P Flash end address */
    
    /* initialize counter variable */
    counter = 0;
    /* set the default return code as FTFx_OK */
    returnCode = FTFx_OK;
        
    /* calculating Flash end address */
    endAddress = destination + size;
    
    /* check for valid range of the target addresses */
    if((destination < PSSDConfig->PFlashBlockBase) || \
        (endAddress > (PSSDConfig->PFlashBlockBase + PSSDConfig->PFlashBlockSize)))
    {
        if((destination < PSSDConfig->DFlashBlockBase) || \
            (endAddress > (PSSDConfig->DFlashBlockBase + PSSDConfig->DFlashBlockSize)))
        {
            /* return an error code FTFx_ERR_RANGE */
            returnCode = FTFx_ERR_RANGE;
            goto EXIT;
        }
        else
        {
            /* No Error: Do nothing */
        }
    }
    else
    {
        /* No Error: Do nothing */
    }
    
    if(FTFx_OK == returnCode)
    {       
        *PSum = 0;
        /* doing sum operation */
        while(destination < endAddress)
        {          
            data = READ8(destination);
            *PSum += (UINT32)data;
            destination += 1;
            size -= 1;

            /* Check if need to serve callback function */
            if((++counter) >= FLASH_CALLBACK_CS)
            {
                /* serve callback function if counter reaches limitation */
                if(NULL_CALLBACK != PSSDConfig->CallBack)
                {
                    (PSSDConfig->CallBack)();
                }
                else
                {
                    /* do nothing */
                }

                /* Reset counter */
                counter = 0;
            }
            else
            {
                /* do nothing */
            }
        }            
    }
    else
    {
        /* error occurred */
    }

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