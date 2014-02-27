/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  EERAMGetProtection.c                                 *
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
*  Function Name    : EERAMGetProtection.c
*  Description      : This function retrieves current EERAM protection status. 
*  Arguments        : PFLASH_SSD_CONFIG, UINT8
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 EERAMSetProtection(PFLASH_SSD_CONFIG PSSDConfig, UINT8 protectStatus)
{
    UINT8  registerValue;    /* store data read from flash register */
    UINT32 returnCode;       /* return code variable */
    UINT8  temp;
    
    /* set the default return as FTFx_OK */
    returnCode = FTFx_OK;
    /* check CCIF bit of the flash status register, it's impossible to write to FEPROT if command is in progress */
    if(FALSE == (REG_BIT_TEST(PSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF)))
    {
        returnCode = FTFx_ERR_CHANGEPROT;
        goto EXIT;
    }
    /* Check if EERAM is set for EEE */
    if(!(REG_BIT_TEST(PSSDConfig->ftfxRegBase + FTFx_SSD_FCNFG_OFFSET, FTFx_SSD_FCNFG_EEERDY)))
    {
        /* EERAM is not set for EEE */
        returnCode = FTFx_ERR_NOEEE;
        goto EXIT;
    }
    else
    {
        /* Do nothing */
    }
    
    /* Read the value of FDPROT register */
    registerValue = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FEPROT_OFFSET);
    
    /* Check if normal mode */
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FEPROT_OFFSET, 0xFF);
    
    if(REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FEPROT_OFFSET) != 0xFF)
    {
        /* if normal mode */
        temp = (UINT8)(protectStatus ^ registerValue);
        if(!(temp & protectStatus))
        {
            REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FEPROT_OFFSET, protectStatus);
        }
        else
        {
            returnCode = FTFx_ERR_CHANGEPROT;
        }
        goto EXIT;
    }
    /* if unprotected or special mode */
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FEPROT_OFFSET, protectStatus);
    
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