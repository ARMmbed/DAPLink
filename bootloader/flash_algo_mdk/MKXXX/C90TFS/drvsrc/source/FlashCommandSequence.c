/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2010, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  FlashCommandSequence.c                               *
* DATE          :  September 27,2010                                    *
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
*************************************************************************/
/* include the header files */
#include "SSD_Types.h"
#include "SSD_FTFx.h"
#include "SSD_FTFx_Internal.h"

/************************************************************************
*
*  Function Name    : FlashCommandSequence
*  Description      : Perform command write sequence for flash operation
*  Arguments        : PFLASH_SSD_CONFIG, UINT8, UINT8*
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 FlashCommandSequence (PFLASH_SSD_CONFIG PSSDConfig, \
                                         UINT8 index, \
                                         UINT8* pCommandArray)
{
    UINT8  counter;          /* for loop counter variable */
    UINT8  registerValue;    /* store data read from flash register */
    UINT32 returnCode;       /* return code variable */

    /* set the default return as FTFx_OK */
    returnCode = FTFx_OK;

    /* check CCIF bit of the flash status register */
    while(FALSE == (REG_BIT_TEST(PSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF)))
    {
        /* wait till CCIF bit is set */
    }

    /* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register */
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, \
              (FTFx_SSD_FSTAT_RDCOLERR | FTFx_SSD_FSTAT_ACCERR | FTFx_SSD_FSTAT_FPVIOL));

    /* load FCCOB registers */
    for(counter=0; counter<=index; counter++)
    {
        switch (counter)
        {
            case 0: REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB0_OFFSET, pCommandArray[counter]);
            break;
            case 1: REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB1_OFFSET, pCommandArray[counter]);
            break;
            case 2: REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB2_OFFSET, pCommandArray[counter]);
            break;
            case 3: REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB3_OFFSET, pCommandArray[counter]);
            break;
            case 4: REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB4_OFFSET, pCommandArray[counter]);
            break;
            case 5: REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB5_OFFSET, pCommandArray[counter]);
            break;
            case 6: REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB6_OFFSET, pCommandArray[counter]);
            break;
            case 7: REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB7_OFFSET, pCommandArray[counter]);
            break;
            case 8: REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB8_OFFSET, pCommandArray[counter]);
            break;
            case 9: REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB9_OFFSET, pCommandArray[counter]);
            break;
            case 10: REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOBA_OFFSET, pCommandArray[counter]);
            break;
            case 11: REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOBB_OFFSET, pCommandArray[counter]);
            break;
            default: break;
        }
    }

    /* clear CCIF bit */
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF);

    /* check CCIF bit */
    while(FALSE == (REG_BIT_TEST(PSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF)))
    {
        /* wait till CCIF bit is set */
        /* serve callback function if counter reaches limitation */
        if(NULL_CALLBACK != PSSDConfig->CallBack)
        {
            (PSSDConfig->CallBack)();
        }
        else
        {
            /* do nothing */
        }
    }
 
    /* Check error bits */
    /*Get flash status register value */
    registerValue = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET);

    /* checking access error */
    if(0 != (registerValue & FTFx_SSD_FSTAT_ACCERR))
    {
        /* return an error code FTFx_ERR_ACCERR */
        returnCode = FTFx_ERR_ACCERR;
    }
    /* checking protection error */
    else if(0 != (registerValue & FTFx_SSD_FSTAT_FPVIOL))
    {
        /* return an error code FTFx_ERR_PVIOL */
        returnCode = FTFx_ERR_PVIOL;
    }
    /* checking MGSTAT0 non-correctable error */
    else if(0 != (registerValue & FTFx_SSD_FSTAT_MGSTAT0))
    {
        /* return an error code FTFx_ERR_MGSTAT0 */
        returnCode = FTFx_ERR_MGSTAT0;
    }
    else
    {
        /* do nothing*/
    }

    return(returnCode);
}
/* end of file */



