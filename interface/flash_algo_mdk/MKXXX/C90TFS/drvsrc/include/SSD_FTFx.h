/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  SSD_FTFx.h                                           *
* DATE          :  Dec 23, 2011                                         *
*                                                                       *
* AUTHOR        :  FPT Team                                             *
* E-mail        :  b33626@freescale.com                                 *
*                                                                       *
*************************************************************************/

/************************** CHANGES *************************************
0.0.1       06.09.2010      FPT Team      Initial Version
0.1.0       06.11.2010      FPT Team      Finalize to 0.1.0
0.1.1       08.16.2010      FPT Team      Update some macros for
                                          FTFx_KX_256K_256K_4K_2K_2K derivative
0.1.2       08.26.2010      FPT Team      Removed EEEBlockBase element in
                                          _ssd_config structure, 
0.1.3       09.16.2010      FPT Team      Updated some macros for
                                          FTFx_KX_256K_256K_4K_2K_2K derivative
0.2.0       09.27.2010      FPT Team      Removed some macros that is not 
                                          used.
0.2.1       01.28.2011      FPT Team      Updated to support 
                                          FTFx_KX_512K_0K_0K_2K_0K,
                                          FTFx_JX_128K_32K_2K_1K_1K,
                                          and FTFx_FX_256K_32K_2K_1K_1K
                                          derivatives.
0.2.2       04.18.2011      FPT Team      Add Swap state definitions of
                                          FTFx_PFLASH_SWAP.
0.2.2       09.15.2011      FPT Team      Add FlashProgramPhrase
                                          Add macros for K70
                                          Remove unused macros
0.2.3       11.15.2011      FPT Team      Updated some macros for
                                          FTFx_KX_1024K_0K_16K_4K_0K derivative.    
0.2.4       12.23.2011      FPT Team      Update to support more Kinetis derivatives.                                          
*************************************************************************/
#ifndef _SSD_FTFx_H_
#define _SSD_FTFx_H_

/*------------------------- Configuration Macros -----------------------*/
/* Define compiler */
#define CW                          0   /* CodeWarrior Compiler */
#define IAR                         1   /* IAR Compiler */
#define ARM                         2   /* ARM RVCT Compiler */
/* Define derivatives with rule: FTFx_KX_Pblocksize_Dblocksize_EERAMsize_Psectorsize_Dsectorsize */
#define FTFx_KX_256K_256K_4K_2K_2K        1    /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_512K_0K_0K_2K_0K          2    /* Kinetis - ARM Cortex M4 core */
#define FTFx_JX_128K_32K_2K_1K_1K         3    /* ColdFire core */
#define FTFx_FX_256K_32K_2K_1K_1K         4    /* ColdFire core */
#define FTFx_KX_512K_512K_16K_4K_4K       5    /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_1024K_0K_16K_4K_0K        6    /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_32K_0K_0K_1K_0K           7    /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_32K_32K_2K_1K_1K          8    /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_64K_0K_0K_1K_0K           9    /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_64K_32K_2K_1K_1K          10   /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_128K_0K_0K_1K_0K          11   /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_128K_32K_2K_1K_1K         12   /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_64K_32K_2K_2K_1K          13   /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_128K_32K_2K_2K_1K         14   /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_256K_32K_2K_2K_1K         15   /* Kinetis - ARM Cortex M4 core */
#define FTFx_KX_256K_64K_2K_1K_1K         16   /* CL 08.08.2012: K12/K12/K21/K22*/
#define FTFx_KX_512K_0K_0K_1K_0K          17   

/* Compiler selection */
#define COMPILER                    RVCT

/* Derivative selection */      // Added by ARM
#if defined (MKP512)                                  
  #define FLASH_DERIVATIVE      FTFx_KX_512K_0K_0K_2K_0K
#elif defined (MKP512_50MHZ)  /* CL 08.08.2012: K12/K12/K21/K22*/
  #define FLASH_DERIVATIVE      FTFx_KX_512K_0K_0K_1K_0K
#elif defined (MKP512X) || defined (MKD512)
  #define FLASH_DERIVATIVE      FTFx_KX_512K_512K_16K_4K_4K
#elif defined (MKP1024) || defined (MKPIFR_120MHZ)
  #define FLASH_DERIVATIVE      FTFx_KX_1024K_0K_16K_4K_0K
#elif defined (MKP128_50MHZ) || defined (MKP64_50MHZ) || defined (MKPIFR_50MHZ) ||\
  defined (MKP32_50MHZ) || defined (MKD32_50MHZ) || defined (MKDIFR_50MHZ) || defined (MKPIFR_48MHZ) || defined (MKP128_48MHZ) ||\
    defined (MKP64_48MHZ) || defined (MKP32_48MHZ) || defined (MKP16_48MHZ) || defined (MKP8_48MHZ) || defined (MKD32_72MHZ)
  #define FLASH_DERIVATIVE      FTFx_KX_128K_32K_2K_1K_1K
#elif defined (MKP256_50MHZ) || defined (MKD64_50MHZ) /* CL 08.08.2012: K12/K12/K21/K22*/
  #define FLASH_DERIVATIVE      FTFx_KX_256K_64K_2K_1K_1K
#else
  #define FLASH_DERIVATIVE      FTFx_KX_256K_256K_4K_2K_2K
#endif
/* Endianness */
#define BIG_ENDIAN                  0   /* Big Endian */
#define LITTLE_ENDIAN               1   /* Little Endian */

/* cpu cores */
#define COLDFIRE                    0   /* ColdFire core */
#define ARM_CM4                     1   /* ARM Cortex M4 core */

/*------------ Return Code Definition for FTFx SSD ---------------------*/
#define FTFx_OK                          0x0000
#define FTFx_ERR_SIZE                    0x0001
#define FTFx_ERR_RANGE                   0x0002
#define FTFx_ERR_ACCERR                  0x0004
#define FTFx_ERR_PVIOL                   0x0008
#define FTFx_ERR_MGSTAT0                 0x0010
#define FTFx_ERR_CHANGEPROT              0x0020
#define FTFx_ERR_EEESIZE                 0x0040
#define FTFx_ERR_EFLASHSIZE              0x0080
#define FTFx_ERR_ADDR                    0x0100
#define FTFx_ERR_NOEEE                   0x0200
#define FTFx_ERR_EFLASHONLY              0x0400
#define FTFx_ERR_DFLASHONLY              0x0800
#define FTFx_ERR_RDCOLERR                0x1000
#define FTFx_ERR_RAMRDY                  0x2000

/* Flash security status */
#define FLASH_SECURITY_STATE_KEYEN         0x80
#define FLASH_SECURITY_STATE_UNSECURED     0x02

#define FLASH_NOT_SECURE                   0x01
#define FLASH_SECURE_BACKDOOR_ENABLED      0x02
#define FLASH_SECURE_BACKDOOR_DISABLED     0x04

/* macro for flash configuration field offset */
#define FLASH_CNFG_START_ADDRESS          0x400
#define FLASH_CNFG_END_ADDRESS            0x40F

/* EERAM Function Control Code */
#define EEE_ENABLE                         0x00
#define EEE_DISABLE                        0xFF

/*-------------- Read/Write/Set/Clear Operation Macros -----------------*/
#define REG_BIT_SET(address, mask)      (*(VUINT8*)(address) |= (mask))
#define REG_BIT_CLEAR(address, mask)    (*(VUINT8*)(address) &= ~(mask))
#define REG_BIT_TEST(address, mask)     (*(VUINT8*)(address) & (mask))
#define REG_WRITE(address, value)       (*(VUINT8*)(address) = (value))
#define REG_READ(address)               ((UINT8)(*(VUINT8*)(address)))
#define REG_WRITE32(address, value)     (*(VUINT32*)(address) = (value))
#define REG_READ32(address)             ((UINT32)(*(VUINT32*)(address)))

#define WRITE8(address, value)          (*(VUINT8*)(address) = (value))
#define READ8(address)                  ((UINT8)(*(VUINT8*)(address)))
#define SET8(address, value)            (*(VUINT8*)(address) |= (value))
#define CLEAR8(address, value)          (*(VUINT8*)(address) &= ~(value))
#define TEST8(address, value)           (*(VUINT8*)(address) & (value))

#define WRITE16(address, value)         (*(VUINT16*)(address) = (value))
#define READ16(address)                 ((UINT16)(*(VUINT16*)(address)))
#define SET16(address, value)           (*(VUINT16*)(address) |= (value))
#define CLEAR16(address, value)         (*(VUINT16*)(address) &= ~(value))
#define TEST16(address, value)          (*(VUINT16*)(address) & (value))

#define WRITE32(address, value)         (*(VUINT32*)(address) = (value))
#define READ32(address)                 ((UINT32)(*(VUINT32*)(address)))
#define SET32(address, value)           (*(VUINT32*)(address) |= (value))
#define CLEAR32(address, value)         (*(VUINT32*)(address) &= ~(value))
#define TEST32(address, value)          (*(VUINT32*)(address) & (value))

/* Word size */
#define FTFx_WORD_SIZE                  0x0002     /* 2 bytes */
  
/* Longword size */
#define FTFx_LONGWORD_SIZE              0x0004     /* 4 bytes */

/* Phrase size */
#define FTFx_PHRASE_SIZE                0x0008     /* 8 bytes */

/* Double-phrase size */
#define FTFx_DPHRASE_SIZE               0x0010     /* 16 bytes */

/*--------------------- CallBack function period -----------------------*/
#define FLASH_CALLBACK_CS               1          /* Check Sum */

/*-------------------- Callback function prototype ---------------------*/
typedef void (* PCALLBACK)(void);

/*--------------------Null Callback function defination ----------------*/
#define NULL_CALLBACK                   ((PCALLBACK)0xFFFFFFFF)

/*---------------- Flash SSD Configuration Structure -------------------*/
typedef struct _ssd_config
{
    UINT32      ftfxRegBase;        /* FTFx control register base */
    UINT32      PFlashBlockBase;    /* base address of PFlash block */
    UINT32      PFlashBlockSize;    /* size of PFlash block */
    UINT32      DFlashBlockBase;    /* base address of DFlash block */
    UINT32      DFlashBlockSize;    /* size of DFlash block */
    UINT32      EERAMBlockBase;     /* base address of EERAM block */
    UINT32      EERAMBlockSize;     /* size of EERAM block */
    UINT32      EEEBlockSize;       /* size of EEE block */
    BOOL        DebugEnable;        /* background debug mode enable bit */
    PCALLBACK   CallBack;           /* pointer to callback function */
} FLASH_SSD_CONFIG, *PFLASH_SSD_CONFIG;

#if ((FTFx_JX_128K_32K_2K_1K_1K == FLASH_DERIVATIVE) || (FTFx_FX_256K_32K_2K_1K_1K == FLASH_DERIVATIVE))
               
    /* Endianness */
    #define ENDIANNESS              BIG_ENDIAN

    /* cpu core */
    #define CPU_CORE                COLDFIRE
   
    /* PFlash sector size */
    #define FTFx_PSECTOR_SIZE       0x00000400      /* 1 KB size */    
    /* DFlash sector size */
    #define FTFx_DSECTOR_SIZE       0x00000400      /* 1 KB size */
        
    #define DEBLOCK_SIZE            0x00008000      /* 32 KB size */
    /* EEE Data Set Size Field Description */     
    #define EEESIZE_0000            0x00000000      /* Reserved */
    #define EEESIZE_0001            0x00000000      /* Reserved */
    #define EEESIZE_0010            0x00000000      /* Reserved */
    #define EEESIZE_0011            0x00000800
    #define EEESIZE_0100            0x00000400
    #define EEESIZE_0101            0x00000200
    #define EEESIZE_0110            0x00000100
    #define EEESIZE_0111            0x00000080
    #define EEESIZE_1000            0x00000040
    #define EEESIZE_1001            0x00000020
    #define EEESIZE_1010            0x00000000      /* Reserved */
    #define EEESIZE_1011            0x00000000      /* Reserved */
    #define EEESIZE_1100            0x00000000      /* Reserved */
    #define EEESIZE_1101            0x00000000      /* Reserved */
    #define EEESIZE_1110            0x00000000      /* Reserved */
    #define EEESIZE_1111            0x00000000      /* Default value */
    
    /* D/E-Flash Partition Code Field Description */
    #define DEPART_0000             0x00008000
    #define DEPART_0001             0x00006000
    #define DEPART_0010             0x00004000
    #define DEPART_0011             0x00000000
    #define DEPART_0100             0x00008000      /* Reserved */
    #define DEPART_0101             0x00008000      /* Reserved */
    #define DEPART_0110             0x00008000      /* Reserved */
    #define DEPART_0111             0x00008000      /* Reserved */
    #define DEPART_1000             0x00000000      
    #define DEPART_1001             0x00002000      
    #define DEPART_1010             0x00004000      
    #define DEPART_1011             0x00008000      
    #define DEPART_1100             0x00008000      /* Reserved */
    #define DEPART_1101             0x00008000      /* Reserved */
    #define DEPART_1110             0x00008000      /* Reserved */
    #define DEPART_1111             0x00008000      /* Default value */
    /*destination to read Dflash IFR area*/
    #define DFLASH_IFR_READRESOURCE_ADDRESS   0x8000FC
    /* Address offset and size of PFlash IFR and DFlash IFR */
    #define PFLASH_IFR_OFFSET                 0x00000000
    #define PFLASH_IFR_SIZE                   0x00000100
    #define DFLASH_IFR_OFFSET                 0x00000000
    #define DFLASH_IFR_SIZE                   0x00000100
    
    /* Size for checking alignment of a function */
    #define ERSBLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of erase block function */
    #define PGMCHK_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of program check function */
    #define PPGMSEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of program section function on Pflash */
    #define DPGMSEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of program section function on Dflash */
    #define RD1BLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of verify block function */
    #define SWAP_ALIGN_SIZE         0xFFFFFFFF                /* check align of swap function*/
    #define PRD1SEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of verify section function on Pflash */
    #define DRD1SEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of verify section function on Dflash */
    #define RDRSRC_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of read resource function */
    #define RDONCE_INDEX_MAX        0xF                       /* maximum index in read once command */    
    
#elif (FTFx_KX_256K_256K_4K_2K_2K == FLASH_DERIVATIVE)
    /* Endianness */
    #define ENDIANNESS              LITTLE_ENDIAN

    /* cpu core */
    #define CPU_CORE                ARM_CM4
       
    /* PFlash sector size */ 
    #define FTFx_PSECTOR_SIZE       0x00000800      /* 2 KB size */
    /* DFlash sector size */ 
    #define FTFx_DSECTOR_SIZE       0x00000800      /* 2 KB size */    
    #define DEBLOCK_SIZE            0x00040000      /* 256 KB size */
    
    /* EEE Data Set Size Field Description */
    #define EEESIZE_0000            0x00000000      /* Reserved */
    #define EEESIZE_0001            0x00000000      /* Reserved */
    #define EEESIZE_0010            0x00001000
    #define EEESIZE_0011            0x00000800
    #define EEESIZE_0100            0x00000400
    #define EEESIZE_0101            0x00000200
    #define EEESIZE_0110            0x00000100
    #define EEESIZE_0111            0x00000080
    #define EEESIZE_1000            0x00000040
    #define EEESIZE_1001            0x00000020
    #define EEESIZE_1010            0x00000000      /* Reserved */
    #define EEESIZE_1011            0x00000000      /* Reserved */
    #define EEESIZE_1100            0x00000000      /* Reserved */
    #define EEESIZE_1101            0x00000000      /* Reserved */
    #define EEESIZE_1110            0x00000000      /* Reserved */
    #define EEESIZE_1111            0x00000000      /* Default value */
    
    /* D/E-Flash Partition Code Field Description */
    #define DEPART_0000             0x00040000
    #define DEPART_0001             0x00040000      /* Reserved */
    #define DEPART_0010             0x0003C000
    #define DEPART_0011             0x00038000
    #define DEPART_0100             0x00030000  
    #define DEPART_0101             0x00020000  
    #define DEPART_0110             0x00000000  
    #define DEPART_0111             0x00040000      /* Reserved */
    #define DEPART_1000             0x00000000
    #define DEPART_1001             0x00040000      /* Reserved */
    #define DEPART_1010             0x00004000
    #define DEPART_1011             0x00008000
    #define DEPART_1100             0x00010000  
    #define DEPART_1101             0x00020000  
    #define DEPART_1110             0x00040000  
    #define DEPART_1111             0x00040000      /* Reserved */
    /*destination to read Dflash IFR area*/
    #define DFLASH_IFR_READRESOURCE_ADDRESS   0x8000FC
    
    /* Address offset and size of PFlash IFR and DFlash IFR */
    #define PFLASH_IFR_OFFSET                 0x00000000
    #define PFLASH_IFR_SIZE                   0x00000100
    #define DFLASH_IFR_OFFSET                 0x00000000
    #define DFLASH_IFR_SIZE                   0x00000100

    /* Size for checking alignment of a section */
    #define ERSBLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of erase block function */
    #define PGMCHK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of program check function */
    #define PPGMSEC_ALIGN_SIZE      FTFx_PHRASE_SIZE        /* check align of program section function */
    #define DPGMSEC_ALIGN_SIZE      FTFx_PHRASE_SIZE        /* check align of program section function */
    #define RD1BLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of verify block function */
    #define PRD1SEC_ALIGN_SIZE      FTFx_PHRASE_SIZE        /* check align of verify section function */
    #define DRD1SEC_ALIGN_SIZE      FTFx_PHRASE_SIZE        /* check align of verify section function */
    #define SWAP_ALIGN_SIZE         0xFFFFFFFF                /* check align of swap function*/
    #define RDRSRC_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of read resource function */
    #define RDONCE_INDEX_MAX        0xF                     /* maximum index in read once command */

#elif (FTFx_KX_512K_0K_0K_2K_0K  == FLASH_DERIVATIVE)
    /* Endianness */
    #define ENDIANNESS              LITTLE_ENDIAN

    /* cpu core */
    #define CPU_CORE                ARM_CM4
    /* PFlash sector size */ 
    #define FTFx_PSECTOR_SIZE       0x00000800      /* 2 KB size */
    /* DFlash sector size */ 
    #define FTFx_DSECTOR_SIZE       0x00000000      /* 0 KB size */
    #define DEBLOCK_SIZE            0x00000000      /* 0 KB size */
    /*destination to read Dflash IFR area*/
    #define DFLASH_IFR_READRESOURCE_ADDRESS   0x8000FC 
    
    /* Address offset and size of PFlash IFR and DFlash IFR */
    #define PFLASH_IFR_OFFSET                 0x00000000
    #define PFLASH_IFR_SIZE                   0x00000100
    #define DFLASH_IFR_OFFSET                 0xFFFFFFFF /* reserved */
    #define DFLASH_IFR_SIZE                   0xFFFFFFFF /* reserved */
          
    /* Size for checking alignment of a section */
    #define ERSBLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of erase block function */
    #define PGMCHK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of program check function */
    #define PPGMSEC_ALIGN_SIZE      FTFx_PHRASE_SIZE        /* check align of program section function */
    #define DPGMSEC_ALIGN_SIZE      FTFx_PHRASE_SIZE        /* check align of program section function */
    #define RD1BLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of verify block function */
    #define PRD1SEC_ALIGN_SIZE      FTFx_PHRASE_SIZE        /* check align of verify section function */
    #define DRD1SEC_ALIGN_SIZE      FTFx_PHRASE_SIZE        /* check align of verify section function */
    #define SWAP_ALIGN_SIZE         0xFFFFFFFF              /* check align of swap function*/
    #define RDRSRC_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of read resource function */
    #define RDONCE_INDEX_MAX        0xF                     /* maximum index in read once command */

#elif (FTFx_KX_512K_0K_0K_1K_0K  == FLASH_DERIVATIVE)  /* CL 08.08.2012: K12/K12/K21/K22*/
    /* Endianness */
    #define ENDIANNESS              LITTLE_ENDIAN

    /* cpu core */
    #define CPU_CORE                ARM_CM4
    /* PFlash sector size */ 
    #define FTFx_PSECTOR_SIZE       0x00000400      /* 1 KB size */
    /* DFlash sector size */ 
    #define FTFx_DSECTOR_SIZE       0x00000000      /* 0 KB size */
    #define DEBLOCK_SIZE            0x00000000      /* 0 KB size */
    /*destination to read Dflash IFR area*/
    #define DFLASH_IFR_READRESOURCE_ADDRESS   0x8000FC 
    
    /* Address offset and size of PFlash IFR and DFlash IFR */
    #define PFLASH_IFR_OFFSET                 0x00000000
    #define PFLASH_IFR_SIZE                   0x00000100
    #define DFLASH_IFR_OFFSET                 0x00000000 
    #define DFLASH_IFR_SIZE                   0x00000100 
    
    /* Size for checking alignment of a section */
    #define ERSBLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of erase block function */
    #define PGMCHK_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of program check function */
    #define PPGMSEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of program section function */
    #define DPGMSEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of program section function */
    #define RD1BLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of verify block function */
    #define PRD1SEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of verify section function */
    #define DRD1SEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of verify section function */
    #define SWAP_ALIGN_SIZE         0xFFFFFFFF                /* check align of swap function*/
    #define RDRSRC_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of read resource function */
    #define RDONCE_INDEX_MAX        0xF                       /* maximum index in read once command */     

#elif ((FTFx_KX_32K_0K_0K_1K_0K == FLASH_DERIVATIVE) \
        ||(FTFx_KX_64K_0K_0K_1K_0K == FLASH_DERIVATIVE) \
        ||(FTFx_KX_128K_0K_0K_1K_0K == FLASH_DERIVATIVE))
    /* Endianness */
    #define ENDIANNESS              LITTLE_ENDIAN

    /* cpu core */
    #define CPU_CORE                ARM_CM4
    /* PFlash sector size */ 
    #define FTFx_PSECTOR_SIZE       0x00000400      /* 1 KB size */
    /* DFlash sector size */ 
    #define FTFx_DSECTOR_SIZE       0x00000000      /* 0 KB size */
    #define DEBLOCK_SIZE            0x00000000      /* 0 KB size */
    /*destination to read Dflash IFR area*/
    #define DFLASH_IFR_READRESOURCE_ADDRESS   0x8000FC 
    
    /* Address offset and size of PFlash IFR and DFlash IFR */
    #define PFLASH_IFR_OFFSET                 0x00000000
    #define PFLASH_IFR_SIZE                   0x00000100
    #define DFLASH_IFR_OFFSET                 0xFFFFFFFF /* reserved */
    #define DFLASH_IFR_SIZE                   0xFFFFFFFF /* reserved */    

    /* Size for checking alignment of a section */
    #define ERSBLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of erase block function */
    #define PGMCHK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of program check function */
    #define PPGMSEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE      /* check align of program section function */
    #define DPGMSEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE      /* check align of program section function */
    #define RD1BLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of verify block function */
    #define PRD1SEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE      /* check align of verify section function */
    #define DRD1SEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE      /* check align of verify section function */
    #define SWAP_ALIGN_SIZE         0xFFFFFFFF              /* check align of swap function*/
    #define RDRSRC_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of read resource function */
    #define RDONCE_INDEX_MAX        0xF                     /* maximum index in read once command */
    
#elif ((FTFx_KX_512K_512K_16K_4K_4K == FLASH_DERIVATIVE) || (FTFx_KX_1024K_0K_16K_4K_0K == FLASH_DERIVATIVE)) /*if derivative of K70 is selected*/
    /* Endianness */
    #define ENDIANNESS              LITTLE_ENDIAN

    /* cpu core */
    #define CPU_CORE                ARM_CM4
            
#if (FTFx_KX_512K_512K_16K_4K_4K == FLASH_DERIVATIVE)    

    /* PFlash sector size */ 
    #define FTFx_PSECTOR_SIZE       0x00001000      /* 4 KB size */
    /* DFlash sector size */ 
    #define FTFx_DSECTOR_SIZE       0x00001000      /* 4 KB size */
    #define DEBLOCK_SIZE            0x00080000      /* 512 KB size */
    
    /* EEE Data Set Size Field Description */
    #define EEESIZE_0000            0x00004000      /* 16386 bytes */
    #define EEESIZE_0001            0x00002000      /* 8192 bytes */
    #define EEESIZE_0010            0x00001000      /* 4096 bytes */
    #define EEESIZE_0011            0x00000800      /* 2048 bytes */
    #define EEESIZE_0100            0x00000400      /* 1024 bytes */
    #define EEESIZE_0101            0x00000200      /* 512 bytes */
    #define EEESIZE_0110            0x00000100      /* 256 bytes */
    #define EEESIZE_0111            0x00000080      /* 128 bytes */
    #define EEESIZE_1000            0x00000040      /* 64 bytes */
    #define EEESIZE_1001            0x00000020      /* 32 bytes */
    #define EEESIZE_1010            0x00000000      /* Reserved */
    #define EEESIZE_1011            0x00000000      /* Reserved */
    #define EEESIZE_1100            0x00000000      /* Reserved */
    #define EEESIZE_1101            0x00000000      /* Reserved */
    #define EEESIZE_1110            0x00000000      /* Reserved */
    #define EEESIZE_1111            0x00000000      /* 0 byte */
    
    /* D/E-Flash Partition Code Field Description */
    #define DEPART_0000             0x00080000      /* 512 KB */
    #define DEPART_0001             0x00080000      /* Reserved */
    #define DEPART_0010             0x00080000      /* Reserved */
    #define DEPART_0011             0x00080000      /* Reserved */
    #define DEPART_0100             0x00070000      /* 448 KB */
    #define DEPART_0101             0x00060000      /* 384 KB */
    #define DEPART_0110             0x00040000      /* 256 KB */
    #define DEPART_0111             0x00000000      /* 0 KB */
    #define DEPART_1000             0x00000000      /* 0 KB */
    #define DEPART_1001             0x00080000      /* Reserved */
    #define DEPART_1010             0x00080000      /* Reserved */
    #define DEPART_1011             0x00080000      /* Reserved */
    #define DEPART_1100             0x00010000      /* 64 KB */
    #define DEPART_1101             0x00020000      /* 128 KB */
    #define DEPART_1110             0x00040000      /* 256 KB */
    #define DEPART_1111             0x00080000      /* 512 KB */
    
    /* Address offset (compare to start addr of P/D flash) and size of PFlash IFR and DFlash IFR */
    #define PFLASH_IFR_OFFSET       0x00000000
    #define PFLASH_IFR_SIZE         0x00000400
    #define DFLASH_IFR_OFFSET       0x00000000
    #define DFLASH_IFR_SIZE         0x00000400
    
#else /* FTFx_KX_1024K_0K_16K_4K_0K == FLASH_DERIVATIVE */
    /* PFlash sector size */ 
    #define FTFx_PSECTOR_SIZE       0x00001000      /* 4 KB size */
    /* DFlash sector size */ 
    #define FTFx_DSECTOR_SIZE       0x00000000      /* 0 KB size */
    #define DEBLOCK_SIZE            0x00000000      /* 0 KB size */
    
    /* Address offset (compare to start addr of P/D flash) and size of PFlash IFR and DFlash IFR */
    #define PFLASH_IFR_OFFSET       0x00000000
    #define PFLASH_IFR_SIZE         0x00000400
    #define DFLASH_IFR_OFFSET       0xFFFFFFFF      /* reserved */
    #define DFLASH_IFR_SIZE         0xFFFFFFFF      /* reserved */
#endif    /* of FTFx_KX_512K_512K_16K_4K_4K == FLASH_DERIVATIVE */
    
    /*destination to read Dflash IFR area*/
    #define DFLASH_IFR_READRESOURCE_ADDRESS       0x8003F8  
 
    /* Size for checking alignment of a section */
    #define ERSBLK_ALIGN_SIZE       FTFx_DPHRASE_SIZE       /* check align of erase block function */
    #define PGMCHK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of program check function */
    #define PPGMSEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of program section function */
    #define DPGMSEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of program section function */
    #define RD1BLK_ALIGN_SIZE       FTFx_DPHRASE_SIZE       /* check align of verify block function */
    #define PRD1SEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of verify section function */
    #define DRD1SEC_ALIGN_SIZE      FTFx_DPHRASE_SIZE       /* check align of verify section function */
    #define SWAP_ALIGN_SIZE         FTFx_DPHRASE_SIZE       /* check align of swap function*/
    #define RDRSRC_ALIGN_SIZE       FTFx_PHRASE_SIZE        /* check align of read resource function */
    #define RDONCE_INDEX_MAX        0x7                     /* maximum index in read once command */
    
#elif ((FTFx_KX_32K_32K_2K_1K_1K == FLASH_DERIVATIVE) \
        ||(FTFx_KX_64K_32K_2K_1K_1K == FLASH_DERIVATIVE) \
        ||(FTFx_KX_128K_32K_2K_1K_1K == FLASH_DERIVATIVE))    
    /* Endianness */
    #define ENDIANNESS              LITTLE_ENDIAN

    /* cpu core */
    #define CPU_CORE                ARM_CM4
        
    /* PFlash sector size */ 
    #define FTFx_PSECTOR_SIZE        0x00000400     /* 1 KB size */
    /* DFlash sector size */ 
    #define FTFx_DSECTOR_SIZE        0x00000400     /* 1 KB size */
    #define DEBLOCK_SIZE             0x00008000     /* 32 KB size */
    
    /* EEE Data Set Size Field Description */
    #define EEESIZE_0000            0x00000000      /* Reserved */
    #define EEESIZE_0001            0x00000000      /* Reserved */
    #define EEESIZE_0010            0x00000000      /* Reserved */
    #define EEESIZE_0011            0x00000800
    #define EEESIZE_0100            0x00000400
    #define EEESIZE_0101            0x00000200
    #define EEESIZE_0110            0x00000100
    #define EEESIZE_0111            0x00000080
    #define EEESIZE_1000            0x00000040
    #define EEESIZE_1001            0x00000020
    #define EEESIZE_1010            0x00000000      /* Reserved */
    #define EEESIZE_1011            0x00000000      /* Reserved */
    #define EEESIZE_1100            0x00000000      /* Reserved */
    #define EEESIZE_1101            0x00000000      /* Reserved */
    #define EEESIZE_1110            0x00000000      /* Reserved */
    #define EEESIZE_1111            0x00000000      /* Default value */
    
    /* D/E-Flash Partition Code Field Description */
    #define DEPART_0000             0x00008000
    #define DEPART_0001             0x00006000
    #define DEPART_0010             0x00004000
    #define DEPART_0011             0x00000000
    #define DEPART_0100             0x00008000      /* Reserved */
    #define DEPART_0101             0x00008000      /* Reserved */
    #define DEPART_0110             0x00008000      /* Reserved */
    #define DEPART_0111             0x00008000      /* Reserved */
    #define DEPART_1000             0x00000000      
    #define DEPART_1001             0x00002000      
    #define DEPART_1010             0x00004000      
    #define DEPART_1011             0x00008000      
    #define DEPART_1100             0x00008000      /* Reserved */
    #define DEPART_1101             0x00008000      /* Reserved */
    #define DEPART_1110             0x00008000      /* Reserved */
    #define DEPART_1111             0x00008000      /* Default value */
    
    /*destination to read Dflash IFR area*/
    #define DFLASH_IFR_READRESOURCE_ADDRESS   0x8000FC 
    
    /* Address offset and size of PFlash IFR and DFlash IFR */
    #define PFLASH_IFR_OFFSET                 0x00000000
    #define PFLASH_IFR_SIZE                   0x00000100
    #define DFLASH_IFR_OFFSET                 0x00000000 
    #define DFLASH_IFR_SIZE                   0x00000100 
    
    /* Size for checking alignment of a section */
    #define ERSBLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of erase block function */
    #define PGMCHK_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of program check function */
    #define PPGMSEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of program section function */
    #define DPGMSEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of program section function */
    #define RD1BLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of verify block function */
    #define PRD1SEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of verify section function */
    #define DRD1SEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of verify section function */
    #define SWAP_ALIGN_SIZE         0xFFFFFFFF                /* check align of swap function*/
    #define RDRSRC_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of read resource function */
    #define RDONCE_INDEX_MAX        0xF                       /* maximum index in read once command */     
#elif (FTFx_KX_256K_64K_2K_1K_1K == FLASH_DERIVATIVE)         /* CL 08.08.2012: K12/K12/K21/K22*/
    /* Endianness */
    #define ENDIANNESS              LITTLE_ENDIAN

    /* cpu core */
    #define CPU_CORE                ARM_CM4
        
    /* PFlash sector size */ 
    #define FTFx_PSECTOR_SIZE        0x00000400     /* 1 KB size */
    /* DFlash sector size */ 
    #define FTFx_DSECTOR_SIZE        0x00000400     /* 1 KB size */
    #define DEBLOCK_SIZE             0x00010000     /* 64 KB size */
    
    /* EEE Data Set Size Field Description */
    #define EEESIZE_0000            0x00000000      /* Reserved */
    #define EEESIZE_0001            0x00000000      /* Reserved */
    #define EEESIZE_0010            0x00001000
    #define EEESIZE_0011            0x00000800
    #define EEESIZE_0100            0x00000400
    #define EEESIZE_0101            0x00000200
    #define EEESIZE_0110            0x00000100
    #define EEESIZE_0111            0x00000080
    #define EEESIZE_1000            0x00000040
    #define EEESIZE_1001            0x00000020
    #define EEESIZE_1010            0x00000000      /* Reserved */
    #define EEESIZE_1011            0x00000000      /* Reserved */
    #define EEESIZE_1100            0x00000000      /* Reserved */
    #define EEESIZE_1101            0x00000000      /* Reserved */
    #define EEESIZE_1110            0x00000000      /* Reserved */
    #define EEESIZE_1111            0x00000000      /* Default value */
    
    /* D/E-Flash Partition Code Field Description */
    #define DEPART_0000             0x00010000
    #define DEPART_0001             0x00000000      /* Reserved */
    #define DEPART_0010             0x00000000      /* Reserved */
    #define DEPART_0011             0x00008000
    #define DEPART_0100             0x00000000      
    #define DEPART_0101             0x00000000      /* Reserved */
    #define DEPART_0110             0x00000000      /* Reserved */
    #define DEPART_0111             0x00000000      /* Reserved */
    #define DEPART_1000             0x00000000      
    #define DEPART_1001             0x00000000      /* Reserved */
    #define DEPART_1010             0x00000000      /* Reserved */
    #define DEPART_1011             0x00008000      
    #define DEPART_1100             0x00010000
    #define DEPART_1101             0x00000000      /* Reserved */
    #define DEPART_1110             0x00000000      /* Reserved */
    #define DEPART_1111             0x00010000      /* Default value */
    
    /*destination to read Dflash IFR area*/
    #define DFLASH_IFR_READRESOURCE_ADDRESS   0x8000FC 
    
    /* Address offset and size of PFlash IFR and DFlash IFR */
    #define PFLASH_IFR_OFFSET                 0x00000000
    #define PFLASH_IFR_SIZE                   0x00000100
    #define DFLASH_IFR_OFFSET                 0x00000000 
    #define DFLASH_IFR_SIZE                   0x00000100 
    
    /* Size for checking alignment of a section */
    #define ERSBLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of erase block function */
    #define PGMCHK_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of program check function */
    #define PPGMSEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of program section function */
    #define DPGMSEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of program section function */
    #define RD1BLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of verify block function */
    #define PRD1SEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of verify section function */
    #define DRD1SEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE        /* check align of verify section function */
    #define SWAP_ALIGN_SIZE         0xFFFFFFFF                /* check align of swap function*/
    #define RDRSRC_ALIGN_SIZE       FTFx_LONGWORD_SIZE        /* check align of read resource function */
    #define RDONCE_INDEX_MAX        0xF                       /* maximum index in read once command */     

#elif ((FTFx_KX_64K_32K_2K_2K_1K == FLASH_DERIVATIVE) \
        ||(FTFx_KX_128K_32K_2K_2K_1K == FLASH_DERIVATIVE) \
        ||(FTFx_KX_256K_32K_2K_2K_1K == FLASH_DERIVATIVE))
    /* Endianness */
    #define ENDIANNESS              LITTLE_ENDIAN

    /* cpu core */
    #define CPU_CORE                ARM_CM4
        
    /* PFlash sector size */ 
    #define FTFx_PSECTOR_SIZE       0x00000800      /* 2 KB size */
    /* DFlash sector size */ 
    #define FTFx_DSECTOR_SIZE       0x00000400      /* 1 KB size */
    #define DEBLOCK_SIZE            0x00008000      /* 32 KB size */
    
    /* EEE Data Set Size Field Description */
    #define EEESIZE_0000            0x00000000      /* Reserved */
    #define EEESIZE_0001            0x00000000      /* Reserved */
    #define EEESIZE_0010            0x00000000
    #define EEESIZE_0011            0x00000800
    #define EEESIZE_0100            0x00000400
    #define EEESIZE_0101            0x00000200
    #define EEESIZE_0110            0x00000100
    #define EEESIZE_0111            0x00000080
    #define EEESIZE_1000            0x00000040
    #define EEESIZE_1001            0x00000020
    #define EEESIZE_1010            0x00000000      /* Reserved */
    #define EEESIZE_1011            0x00000000      /* Reserved */
    #define EEESIZE_1100            0x00000000      /* Reserved */
    #define EEESIZE_1101            0x00000000      /* Reserved */
    #define EEESIZE_1110            0x00000000      /* Reserved */
    #define EEESIZE_1111            0x00000000      /* Default value */
    
    /* D/E-Flash Partition Code Field Description */
    #define DEPART_0000             0x00008000
    #define DEPART_0001             0x00006000
    #define DEPART_0010             0x00004000
    #define DEPART_0011             0x00000000
    #define DEPART_0100             0x00008000      /* Reserved */
    #define DEPART_0101             0x00008000      /* Reserved */
    #define DEPART_0110             0x00008000      /* Reserved */
    #define DEPART_0111             0x00008000      /* Reserved */
    #define DEPART_1000             0x00000000      
    #define DEPART_1001             0x00002000      
    #define DEPART_1010             0x00004000      
    #define DEPART_1011             0x00008000      
    #define DEPART_1100             0x00008000      /* Reserved */
    #define DEPART_1101             0x00008000      /* Reserved */
    #define DEPART_1110             0x00008000      /* Reserved */
    #define DEPART_1111             0x00008000      /* Default value */
    
    /*destination to read Dflash IFR area*/
    #define DFLASH_IFR_READRESOURCE_ADDRESS   0x8000FC 
    
    /* Address offset and size of PFlash IFR and DFlash IFR */
    #define PFLASH_IFR_OFFSET                 0x00000000
    #define PFLASH_IFR_SIZE                   0x00000100
    #define DFLASH_IFR_OFFSET                 0x00000000 
    #define DFLASH_IFR_SIZE                   0x00000100 
    
    /* Size for checking alignment of a section */
    #define ERSBLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of erase block function */
    #define PGMCHK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of program check function */
    #define PPGMSEC_ALIGN_SIZE      FTFx_PHRASE_SIZE        /* check align of program section function */
    #define DPGMSEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE      /* check align of program section function */
    #define RD1BLK_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of verify block function */
    #define PRD1SEC_ALIGN_SIZE      FTFx_PHRASE_SIZE        /* check align of verify section function */
    #define DRD1SEC_ALIGN_SIZE      FTFx_LONGWORD_SIZE      /* check align of verify section function */    
    #define SWAP_ALIGN_SIZE         FTFx_LONGWORD_SIZE      /* check align of swap function*/
    #define RDRSRC_ALIGN_SIZE       FTFx_LONGWORD_SIZE      /* check align of read resource function */
    #define RDONCE_INDEX_MAX        0xF                     /* maximum index in read once command */  

#endif  /* FLASH_DERIVATIVE */

/* PFlash swap states */

#define FTFx_SWAP_UNINIT            0x00
#define FTFx_SWAP_READY             0x01
#define FTFx_SWAP_INIT              0x01
#define FTFx_SWAP_UPDATE            0x02
#define FTFx_SWAP_UPDATE_ERASED     0x03
#define FTFx_SWAP_COMPLETE          0x04



/* -------------------- Function Pointer ------------------------------- */
typedef UINT32 (*pFLASHCOMMANDSEQUENCE) (PFLASH_SSD_CONFIG PSSDConfig, \
                                         UINT8 index, \
                                         UINT8* pCommandArray);

/* Flash initialization */
typedef UINT32 (*pFLASHINIT) (PFLASH_SSD_CONFIG PSSDConfig);

/* P-Flash get protection */
typedef UINT32 (*pPFLASHGETPROTECTION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                        UINT32*  protectStatus);
                                              
/* P-Flash set protection */
typedef UINT32 (*pPFLASHSETPROTECTION) (PFLASH_SSD_CONFIG  PSSDConfig, \
                                        UINT32  protectStatus);
                                                        
/* Flash get interrupt enable */
typedef UINT32 (*pFLASHGETINTERRUPTENABLE) (PFLASH_SSD_CONFIG PSSDConfig, \
                                            UINT8* interruptState);

/* Flash set interrupt enable */
typedef UINT32 (*pFLASHSETINTERRUPTENABLE) (PFLASH_SSD_CONFIG PSSDConfig, \
                                            UINT8 interruptState);

/* Flash get security state */
typedef UINT32 (*pFLASHGETSECURITYSTATE) (PFLASH_SSD_CONFIG PSSDConfig, \
                                          UINT8* securityState);
                                                        
/* Flash security bypass */
typedef UINT32 (*pFLASHSECURITYBYPASS) (PFLASH_SSD_CONFIG PSSDConfig, \
                                        UINT8* keyBuffer, \
                                        pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash erase all Block */
typedef UINT32 (*pFLASHERASEALLBLOCK) (PFLASH_SSD_CONFIG PSSDConfig, \
                                       pFLASHCOMMANDSEQUENCE FlashCommandSequence);
                                
/* Flash erase Block */
typedef UINT32 (*pFLASHERASEBLOCK) (PFLASH_SSD_CONFIG PSSDConfig, \
                                    UINT32 destination, \
                                    pFLASHCOMMANDSEQUENCE FlashCommandSequence);
                                    
/* Flash erase sector */
typedef UINT32 (*pFLASHERASESECTOR) (PFLASH_SSD_CONFIG PSSDConfig, \
                                     UINT32 destination, \
                                     UINT32 size, \
                                     pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash erase suspend */
typedef UINT32 (*pFLASHERASESUSPEND) (PFLASH_SSD_CONFIG PSSDConfig);

/* Flash erase resume */
typedef UINT32 (*pFLASHERASERESUME) (PFLASH_SSD_CONFIG PSSDConfig);                                                       
    
/* Flash program Section */
typedef UINT32 (*pFLASHPROGRAMSECTION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                        UINT32 destination, \
                                        UINT16 Number, \
                                        pFLASHCOMMANDSEQUENCE FlashCommandSequence);                                                
                                   
/* Flash checksum */
typedef UINT32 (*pFLASHCHECKSUM) (PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT32 destination, \
                                  UINT32 size, \
                                  UINT32* PSum);
                                           
/* Flash verify all block */
typedef UINT32 (*pFLASHVERIFYALLBLOCK) (PFLASH_SSD_CONFIG PSSDConfig, \
                                        UINT8 marginLevel, \
                                        pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash verify block */
typedef UINT32 (*pFLASHVERIFYBLOCK) (PFLASH_SSD_CONFIG PSSDConfig, \
                                     UINT32 destination, \
                                     UINT8 marginLevel, \
                                     pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash verify section*/
typedef UINT32 (*pFLASHVERIFYSECTION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                       UINT32 destination, \
                                       UINT16 Number, \
                                       UINT8 marginLevel, \
                                       pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash read once */
typedef UINT32 (*pFLASHREADONCE) (PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT8* pDataArray, \
                                  pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash program once */
typedef UINT32 (*pFLASHPROGRAMONCE) (PFLASH_SSD_CONFIG PSSDConfig, \
                                     UINT8* pDataArray, \
                                     pFLASHCOMMANDSEQUENCE FlashCommandSequence);
/* Flash program check */
typedef UINT32 (*pFLASHPROGRAMCHECK) (PFLASH_SSD_CONFIG PSSDConfig, \
                                      UINT32  destination, \
                                      UINT32  size, \
                                      UINT8*  pExpectedData, \
                                      UINT32* pFailAddr, \
                                      UINT8*  pFailData, \
                                      UINT8   marginLevel, \
                                      pFLASHCOMMANDSEQUENCE FlashCommandSequence);
                                                      
/* Flash read resource */
typedef UINT32 (*pFLASHREADRESOURCE) (PFLASH_SSD_CONFIG PSSDConfig, \
                                      UINT32 destination, \
                                      UINT8* pDataArray, \
                                      pFLASHCOMMANDSEQUENCE FlashCommandSequence);                                                                                    

/*---------------- Function Prototypes for Flash SSD --------------------*/

/* Flash initialization */
extern UINT32 FlashInit(PFLASH_SSD_CONFIG PSSDConfig);

/* Internal function. Called by driver APIs only */
/* Flash command sequence */
extern UINT32 FlashCommandSequence(PFLASH_SSD_CONFIG PSSDConfig, \
                                   UINT8 index, \
                                   UINT8* pCommandArray);

/* P-Flash get protection */
extern UINT32 PFlashGetProtection(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT32*  protectStatus);      

/* P-Flash set protection */
extern UINT32 PFlashSetProtection(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT32  protectStatus);                                                
                                                
/* Flash get interrupt enable */
extern UINT32 FlashGetInterruptEnable(PFLASH_SSD_CONFIG PSSDConfig, \
                                      UINT8* interruptState);

/* Flash set interrupt enable */
extern UINT32 FlashSetInterruptEnable(PFLASH_SSD_CONFIG PSSDConfig, \
                                      UINT8 interruptState);

/* Flash get security state */
extern UINT32 FlashGetSecurityState(PFLASH_SSD_CONFIG PSSDConfig, \
                                    UINT8* securityState);

/* Flash security bypass */
extern UINT32 FlashSecurityBypass(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT8* keyBuffer, \
                                  pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash erase all Block */
extern UINT32 FlashEraseAllBlock(PFLASH_SSD_CONFIG PSSDConfig, \
                                 pFLASHCOMMANDSEQUENCE FlashCommandSequence);
                                
/* Flash erase Block */
extern UINT32 FlashEraseBlock(PFLASH_SSD_CONFIG PSSDConfig, \
                                   UINT32 destination, \
                                   pFLASHCOMMANDSEQUENCE FlashCommandSequence);
                                    
/* Flash erase sector */
extern UINT32 FlashEraseSector(PFLASH_SSD_CONFIG PSSDConfig, \
                               UINT32 destination, \
                               UINT32 size, \
                               pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash erase suspend */
extern UINT32 FlashEraseSuspend(PFLASH_SSD_CONFIG PSSDConfig);

/* Flash erase resume */
extern UINT32 FlashEraseResume(PFLASH_SSD_CONFIG PSSDConfig);
                                                
/* Flash program Section */
extern UINT32 FlashProgramSection(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT32 destination, \
                                  UINT16 Number, \
                                  pFLASHCOMMANDSEQUENCE FlashCommandSequence);                                                
   
/* Flash checksum */
extern UINT32 FlashCheckSum(PFLASH_SSD_CONFIG PSSDConfig, \
                            UINT32 destination, \
                            UINT32 size, \
                            UINT32* PSum);
                                           
/* Flash verify all block */
extern UINT32 FlashVerifyAllBlock(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT8 marginLevel, \
                                  pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash verify block */
extern UINT32 FlashVerifyBlock(PFLASH_SSD_CONFIG PSSDConfig, \
                               UINT32 destination, \
                               UINT8 marginLevel, \
                               pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash verify section*/
extern UINT32 FlashVerifySection(PFLASH_SSD_CONFIG PSSDConfig, \
                                 UINT32 destination, \
                                 UINT16 Number, \
                                 UINT8 marginLevel, \
                                 pFLASHCOMMANDSEQUENCE FlashCommandSequence);        

/* Flash read once */
extern UINT32 FlashReadOnce(PFLASH_SSD_CONFIG PSSDConfig, \
                            UINT8* pDataArray, \
                            pFLASHCOMMANDSEQUENCE FlashCommandSequence);

/* Flash program once */
extern UINT32 FlashProgramOnce(PFLASH_SSD_CONFIG PSSDConfig, \
                               UINT8* pDataArray, \
                               pFLASHCOMMANDSEQUENCE FlashCommandSequence);
/* Flash program check */
extern UINT32 FlashProgramCheck(PFLASH_SSD_CONFIG PSSDConfig, \
                                UINT32  destination, \
                                UINT32  size, \
                                UINT8*  pExpectedData, \
                                UINT32* pFailAddr, \
                                UINT8*  pFailData, \
                                UINT8   marginLevel, \
                                pFLASHCOMMANDSEQUENCE FlashCommandSequence);
                                        
/* Flash read resource */
extern UINT32 FlashReadResource(PFLASH_SSD_CONFIG PSSDConfig, \
                                UINT32 destination, \
                                UINT8* pDataArray, \
                                pFLASHCOMMANDSEQUENCE FlashCommandSequence); 

#if ((FTFx_KX_512K_512K_16K_4K_4K == FLASH_DERIVATIVE) || (FTFx_KX_1024K_0K_16K_4K_0K == FLASH_DERIVATIVE))
/* Flash program Phrase */
typedef UINT32 (*pFLASHPROGRAMPHRASE) (PFLASH_SSD_CONFIG PSSDConfig, \
                                         UINT32 destination, \
                                         UINT32 size, \
                                         UINT32 source, \
                                         pFLASHCOMMANDSEQUENCE FlashCommandSequence); 
/* Flash program phrase */
extern UINT32 FlashProgramPhrase(PFLASH_SSD_CONFIG PSSDConfig, \
                                   UINT32 destination, \
                                   UINT32 size, \
                                   UINT32 source, \
                                   pFLASHCOMMANDSEQUENCE FlashCommandSequence);
#else                                                    
/* Flash program Longword */
typedef UINT32 (*pFLASHPROGRAMLONGWORD) (PFLASH_SSD_CONFIG PSSDConfig, \
                                         UINT32 destination, \
                                         UINT32 size, \
                                         UINT32 source, \
                                         pFLASHCOMMANDSEQUENCE FlashCommandSequence);
                                         
/* Flash program Longword */
extern UINT32 FlashProgramLongword(PFLASH_SSD_CONFIG PSSDConfig, \
                                   UINT32 destination, \
                                   UINT32 size, \
                                   UINT32 source, \
                                   pFLASHCOMMANDSEQUENCE FlashCommandSequence);         
#endif                                
                                
#if ((FTFx_FX_256K_32K_2K_1K_1K == FLASH_DERIVATIVE)||(FTFx_KX_512K_512K_16K_4K_4K == FLASH_DERIVATIVE) \
        ||(FTFx_KX_1024K_0K_16K_4K_0K == FLASH_DERIVATIVE)||(FTFx_KX_64K_32K_2K_2K_1K == FLASH_DERIVATIVE) \
        ||(FTFx_KX_128K_32K_2K_2K_1K == FLASH_DERIVATIVE)||(FTFx_KX_256K_32K_2K_2K_1K == FLASH_DERIVATIVE))
/* PFlash Get swap status */
typedef UINT32 (*pPFLASHGETSWAPSTATUS) (PFLASH_SSD_CONFIG PSSDConfig, \
                                        UINT32 flashAddress, \
                                        UINT8* pCurrentSwapMode, \
                                        UINT8* pCurrentSwapBlockStatus, \
                                        UINT8* pNextSwapBlockStatus, \
                                        pFLASHCOMMANDSEQUENCE FlashCommandSequence);
/* Swap the two P-Flash blocks with each other */
typedef UINT32 (*PFLASHSWAP)(PFLASH_SSD_CONFIG PSSDConfig, \
                             UINT32 flashAddress, \
                             pFLASHCOMMANDSEQUENCE FlashCommandSequence);
        
/* Get Swap status of P-Flash */
extern UINT32 PFlashGetSwapStatus(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT32 flashAddress, \
                                  UINT8* pCurrentSwapMode, \
                                  UINT8* pCurrentSwapBlockStatus, \
                                  UINT8* pNextSwapBlockStatus, \
                                  pFLASHCOMMANDSEQUENCE FlashCommandSequence);
/* Swap the two P-Flash blocks with each other */
extern UINT32 PFlashSwap(PFLASH_SSD_CONFIG PSSDConfig, \
                         UINT32 flashAddress, \
                         pFLASHCOMMANDSEQUENCE FlashCommandSequence);

#endif
#if (DEBLOCK_SIZE)
/* D-Flash get protection */
typedef UINT32 (*pDFLASHGETPROTECTION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                        UINT8*  protectStatus);    
/* D-Flash set protection */
typedef UINT32 (*pDFLASHSETPROTECTION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                        UINT8  protectStatus);
                                    
/* EERAM get protection */
typedef UINT32 (*pEERAMGETPROTECTION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                       UINT8*  protectStatus);      

/* EERAM set protection */
typedef UINT32 (*pEERAMSETPROTECTION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                       UINT8  protectStatus);
/* Flash D/E-Flash Partition */
typedef UINT32 (*pDEFLASHPARTITION) (PFLASH_SSD_CONFIG PSSDConfig, \
                                     UINT8 EEEDataSizeCode, \
                                     UINT8 DEPartitionCode, \
                                     pFLASHCOMMANDSEQUENCE FlashCommandSequence);
/* Flash Set EEEEnable */
typedef UINT32 (*pSETEEEENABLE) (PFLASH_SSD_CONFIG PSSDConfig, \
                                 UINT8 EEEEnable);                                    

/* EEPROM Emulator Write */
typedef UINT32 (*pEEEWRITE) (PFLASH_SSD_CONFIG PSSDConfig, \
                             UINT32 destination, \
                             UINT32 size, \
                             UINT32 source);      
/* D-Flash get protection */
extern UINT32 DFlashGetProtection(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT8*  protectStatus);      

/* D-Flash set protection */
extern UINT32 DFlashSetProtection(PFLASH_SSD_CONFIG PSSDConfig, \
                                  UINT8  protectStatus);
                                                
/* EERAM get protection */
extern UINT32 EERAMGetProtection(PFLASH_SSD_CONFIG PSSDConfig, \
                                 UINT8*  protectStatus);      

/* EERAM set protection */
extern UINT32 EERAMSetProtection(PFLASH_SSD_CONFIG PSSDConfig, \
                                 UINT8  protectStatus);    
/* Flash D/E-Flash Partition */
extern UINT32 DEFlashPartition(PFLASH_SSD_CONFIG PSSDConfig, \
                               UINT8 EEEDataSizeCode, \
                               UINT8 DEPartitionCode, \
                               pFLASHCOMMANDSEQUENCE FlashCommandSequence);
/* Flash Set EEEEnable */
extern UINT32 SetEEEEnable(PFLASH_SSD_CONFIG PSSDConfig, \
                           UINT8 EEEEnable);

/* EEPROM Emulator Write */
extern UINT32 EEEWrite(PFLASH_SSD_CONFIG PSSDConfig, \
                       UINT32 destination, \
                       UINT32 size, \
                       UINT32 source);
#endif /* DEBLOCK_SIZE */                                              
#endif /* _SSD_FTFx_H_ */

