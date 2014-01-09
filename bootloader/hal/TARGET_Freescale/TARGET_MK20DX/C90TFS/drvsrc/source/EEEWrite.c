/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  EEEWrite.c                                           *
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
0.1.4       09.20.2010      FPT Team      Added protection violation error
                                          checking.
0.2.0       09.27.2010      FPT Team      Added the checking for protection 
                                          violation error.
0.2.1       01.28.2011      FPT Team      Updated the preprocesor 
                                          condition to enter Debug state                                          
*************************************************************************/
/* include the header files */
#include "SSD_Types.h"
#include "SSD_FTFx.h"
#include "SSD_FTFx_Internal.h"

/************************************************************************
*
*  Function Name    : EEEWrite.c
*  Description      : This function is used to write data to EERAM 
*                     when it is used as EEPROM emulator              
*  Arguments        : PFLASH_SSD_CONFIG,UINT32 ,UINT32 ,UINT32
*  Return Value     : UINT32
*
************************************************************************/

UINT32 EEEWrite(PFLASH_SSD_CONFIG PSSDConfig, \
                           UINT32 destination, \
                           UINT32 size, \
                           UINT32 source)
{
    UINT32 returnCode;           /* Return code variable */
    /* set the default return code as FTFx_OK */
    returnCode = FTFx_OK;
    /* Check if EEE is enabled */
    if(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FCNFG_OFFSET) & FTFx_SSD_FCNFG_EEERDY)
    {
        if(destination < PSSDConfig->EERAMBlockBase || \
          ((destination + size) > (PSSDConfig->EERAMBlockBase + PSSDConfig->EEEBlockSize)))
        {
            returnCode = FTFx_ERR_RANGE;
            goto EXIT;
        }
        else
        {
            while(size > 0)
            {    
                while(FALSE == REG_BIT_TEST(PSSDConfig->ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY))
                {
                    /* wait till EEERDY bit is set */
                }
                WRITE8(destination,READ8(source));
        
                /* Check for protection violation error */ 
                if(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET) & FTFx_SSD_FSTAT_FPVIOL)
                {
                    returnCode = FTFx_ERR_PVIOL;
                    break;
                }
        
                /* update destination address for next iteration */
                destination += 1;
                /* update size for next iteration */
                size -= 1;
                /* increment the source adress by 1 */
                source += 1;
            }
        }
    }
    else
    {
        returnCode = FTFx_ERR_NOEEE;
        goto EXIT;
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