/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  FlashInit.c                                          *
* DATE          :  Dec 27, 2011                                         *
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
0.2.2       08.03.2011      FPT Team      Fix a bug of missing writing 0
                                          to FCCOB8 to select IFR for
                                          Read Resource
0.2.3       09.15.2011      FPT Team      Add to support K70
0.2.4       12.27.2011      FPT Team      Update to support more Kinetis derivatives.
*************************************************************************/
/* include the header files */
#include "SSD_Types.h"
#include "SSD_FTFx.h"
#include "SSD_FTFx_Internal.h"

/************************************************************************
*
*  Function Name    : FlashInit
*  Description      : Initialize the Flash memory.
*  Arguments        : PFLASH_SSD_CONFIG
*  Return Value     : UINT32
*
*************************************************************************/

UINT32 FlashInit (PFLASH_SSD_CONFIG PSSDConfig)
{
#if (DEBLOCK_SIZE != 0)
    UINT8  EEEDataSetSize;    /* store EEE Data Set Size */
    UINT8  DEPartitionCode;    /* store D/E-Flash Partition Code */
#endif
    UINT32 returnCode;      /* return code variable */
  
    /* set the default return code as FTFx_OK */
    returnCode = FTFx_OK;

#if (DEBLOCK_SIZE != 0)    
    /* check CCIF bit of the flash status register */
    while(FALSE == (REG_BIT_TEST(PSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF)))
    {
        /* wait till CCIF bit is set */
    }

    /* clear RDCOLERR & ACCERR & FPVIOL flag in flash status register */
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, \
                      (FTFx_SSD_FSTAT_RDCOLERR | FTFx_SSD_FSTAT_ACCERR | FTFx_SSD_FSTAT_FPVIOL)); 
    
    /* Write Command Code to FCCOB0 */
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB0_OFFSET, FTFx_READ_RESOURCE);
    /* Write address to FCCOB1/2/3 */
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB1_OFFSET, ((UINT8)(DFLASH_IFR_READRESOURCE_ADDRESS >> 16)));
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB2_OFFSET, ((UINT8)((DFLASH_IFR_READRESOURCE_ADDRESS >> 8) & 0xFF)));
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB3_OFFSET, ((UINT8)(DFLASH_IFR_READRESOURCE_ADDRESS & 0xFF)));
    
    /* Write Resource Select Code of 0 to FCCOB8 to select IFR. Without this, */
    /* an access error may occur if the register contains data from a previous command. */
#if (FTFx_KX_512K_512K_16K_4K_4K == FLASH_DERIVATIVE)
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB4_OFFSET, 0);
#else
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB8_OFFSET, 0);
#endif
    /* clear CCIF bit */
    REG_WRITE(PSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF);

    /* check CCIF bit */
    while((REG_BIT_TEST(PSSDConfig->ftfxRegBase + FTFx_SSD_FSTAT_OFFSET, FTFx_SSD_FSTAT_CCIF)) == FALSE)
    {
        /* wait till CCIF bit is set */
    }

    /* Read returned value of FCCOB6/7 or FCCOBA,B to the variables */
#if (FTFx_KX_512K_512K_16K_4K_4K == FLASH_DERIVATIVE)
    EEEDataSetSize = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOBA_OFFSET);
    DEPartitionCode = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOBB_OFFSET);
#else
    EEEDataSetSize = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB6_OFFSET);
    DEPartitionCode = REG_READ(PSSDConfig->ftfxRegBase + FTFx_SSD_FCCOB7_OFFSET);
#endif    
    /* Calculate D-Flash size and EEE size */
    switch (DEPartitionCode & 0x0F)
    {
        case 0: PSSDConfig->DFlashBlockSize = DEPART_0000; break;
        case 1: PSSDConfig->DFlashBlockSize = DEPART_0001; break;
        case 2: PSSDConfig->DFlashBlockSize = DEPART_0010; break;
        case 3: PSSDConfig->DFlashBlockSize = DEPART_0011; break;
        case 4: PSSDConfig->DFlashBlockSize = DEPART_0100; break;
        case 5: PSSDConfig->DFlashBlockSize = DEPART_0101; break;
        case 6: PSSDConfig->DFlashBlockSize = DEPART_0110; break;
        case 7: PSSDConfig->DFlashBlockSize = DEPART_0111; break;
        case 8: PSSDConfig->DFlashBlockSize = DEPART_1000; break;
        case 9: PSSDConfig->DFlashBlockSize = DEPART_1001; break;
        case 10: PSSDConfig->DFlashBlockSize = DEPART_1010; break;
        case 11: PSSDConfig->DFlashBlockSize = DEPART_1011; break;
        case 12: PSSDConfig->DFlashBlockSize = DEPART_1100; break;
        case 13: PSSDConfig->DFlashBlockSize = DEPART_1101; break;
        case 14: PSSDConfig->DFlashBlockSize = DEPART_1110; break;
        case 15: PSSDConfig->DFlashBlockSize = DEPART_1111; break;
        default: break;
    }
        
    switch (EEEDataSetSize & 0x0F)
    {
        case 0: PSSDConfig->EEEBlockSize = EEESIZE_0000; break;
        case 1: PSSDConfig->EEEBlockSize = EEESIZE_0001; break;
        case 2: PSSDConfig->EEEBlockSize = EEESIZE_0010; break;
        case 3: PSSDConfig->EEEBlockSize = EEESIZE_0011; break;
        case 4: PSSDConfig->EEEBlockSize = EEESIZE_0100; break;
        case 5: PSSDConfig->EEEBlockSize = EEESIZE_0101; break;
        case 6: PSSDConfig->EEEBlockSize = EEESIZE_0110; break;
        case 7: PSSDConfig->EEEBlockSize = EEESIZE_0111; break;
        case 8: PSSDConfig->EEEBlockSize = EEESIZE_1000; break;
        case 9: PSSDConfig->EEEBlockSize = EEESIZE_1001; break;
        case 10: PSSDConfig->EEEBlockSize = EEESIZE_1010; break;
        case 11: PSSDConfig->EEEBlockSize = EEESIZE_1011; break;
        case 12: PSSDConfig->EEEBlockSize = EEESIZE_1100; break;
        case 13: PSSDConfig->EEEBlockSize = EEESIZE_1101; break;
        case 14: PSSDConfig->EEEBlockSize = EEESIZE_1110; break;
        case 15: PSSDConfig->EEEBlockSize = EEESIZE_1111; break;
        default: break;
    }        
#else /* DEBLOCK_SIZE == 0 */
    /* If size of D/E-Flash = 0 */
    PSSDConfig->DFlashBlockSize = 0;
    PSSDConfig->EEEBlockSize = 0;
#endif /* of DEBLOCK_SIZE */

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
