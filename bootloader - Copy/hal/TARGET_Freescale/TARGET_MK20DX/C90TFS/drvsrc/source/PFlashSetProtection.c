/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  PFlashSetProtection.c                                *
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
0.2.2       09.15.2011      FPT Team      Add code for checking CCIF bit
*************************************************************************/
/* include the header files */
#include "SSD_Types.h"
#include "SSD_FTFx.h"
#include "SSD_FTFx_Internal.h"

/************************************************************************
*
*  Function Name    : PFlashSetProtection.c
*  Description      : This function sets the P-Flash protection to the 
*                     intended protection status 
*  Arguments        : PFLASH_SSD_CONFIG, UINT32
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 PFlashSetProtection(PFLASH_SSD_CONFIG PSSDConfig, \
                                      UINT32 protectStatus)
{
    UINT32 registerValue;    /* store data read from flash register */
    UINT32 returnCode;       /* return code variable */
    UINT32 temp;
    UINT32 registerValue0;
    UINT32 registerValue1;
    UINT32 registerValue2;
    UINT32 registerValue3;
  
    /* set the default return as FTFx_OK */
    returnCode = FTFx_OK;
    /* check CCIF bit of the flash status register, it's impossible to write to FPROT if command is in progress */
    if(FALSE == (REG_BIT_TEST(PSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF)))
    {
        returnCode = FTFx_ERR_CHANGEPROT;
        goto EXIT;
    }
    /* Read the value of FPPROT registers */
    registerValue0=(UINT32)(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT0_OFFSET));
    registerValue1=(UINT32)(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT1_OFFSET));
    registerValue2=(UINT32)(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT2_OFFSET));
    registerValue3=(UINT32)(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT3_OFFSET));
    
    registerValue = ((registerValue0 << 24 ) | \
                    (registerValue1 << 16 ) | \
                    (registerValue2 << 8 ) | \
                     registerValue3); 
    
    /* Check if normal mode */
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT0_OFFSET, 0xFF);
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT1_OFFSET, 0xFF);
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT2_OFFSET, 0xFF);
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT3_OFFSET, 0xFF);
  
    /* Read the value of FPPROT registers */
    registerValue0=(UINT32)(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT0_OFFSET));
    registerValue1=(UINT32)(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT1_OFFSET));
    registerValue2=(UINT32)(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT2_OFFSET));
    registerValue3=(UINT32)(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT3_OFFSET));
    
    if((registerValue0 != 0xFF) | (registerValue1 != 0xFF) | \
       (registerValue2 != 0xFF) | (registerValue3 != 0xFF))
    {
        /* if normal mode */
        temp = protectStatus ^ registerValue;
        if(!(temp & protectStatus))
        {
            REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT0_OFFSET, (UINT8)(protectStatus >> 24));
            REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT1_OFFSET, (UINT8)(protectStatus >> 16));
            REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT2_OFFSET, (UINT8)(protectStatus >> 8));
            REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT3_OFFSET, (UINT8)protectStatus);            
        }
        else
        {
            returnCode = FTFx_ERR_CHANGEPROT;
        }
        goto EXIT;
    }
    /* if unprotected or special mode */
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT0_OFFSET, (UINT8)(protectStatus >> 24));
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT1_OFFSET, (UINT8)(protectStatus >> 16));
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT2_OFFSET, (UINT8)(protectStatus >> 8));
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FPROT3_OFFSET, (UINT8)protectStatus);
    
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