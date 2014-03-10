/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2011, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFx                        *
*                                                                       *
* FILE NAME     :  SSD_FTFx_Internal.h                                  *
* DATE          :  Sep 15, 2011                                         *
*                                                                       *
* AUTHOR        :  FPT Team                                             *
* E-mail        :  b33626@freescale.com                                 *
*                                                                       *
*************************************************************************/

/************************** CHANGES *************************************
0.0.1       06.09.2010      FPT Team      Initial Version
0.1.0       06.11.2010      FPT Team      Finalize to 0.1.0 
0.1.1       08.16.2010      FPT Team      Finalize to 0.1.1
0.1.2       08.26.2010      FPT Team      Finalize to 0.1.2
0.1.3       09.16.2010      FPT Team      Updated to support little endian
0.2.0       06.27.2010      FPT Team      Finalize to 0.2.0 
0.2.1       01.28.2011      FPT Team      Updated to support 
                                          FTFx_KX_512K_0K_0K,
                                          FTFx_JX_128K_32K_2K,
                                          and FTFx_FX_256K_32K_2K
                                          derivatives.
0.2.2       04.18.2011      FPT Team      Add swap control code definitions
                                          of FTFx_PFLASH_SWAP.
0.2.3       09.15.2011      FPT Team      Add command for program phrase
*************************************************************************/
#ifndef _SSD_FTFx_INTERNAL_H_
#define _SSD_FTFx_INTERNAL_H_

/*--------------- FTFx Flash Module Memory Offset Map -----------------*/
#if(ENDIANNESS == BIG_ENDIAN)    /* Big Endian */
    /* Flash Status Register (FSTAT)*/
    #define FTFx_SSD_FSTAT_OFFSET               0x00000003
    #define FTFx_SSD_FSTAT_CCIF                 0x80
    #define FTFx_SSD_FSTAT_RDCOLERR             0x40
    #define FTFx_SSD_FSTAT_ACCERR               0x20
    #define FTFx_SSD_FSTAT_FPVIOL               0x10
    #define FTFx_SSD_FSTAT_MGSTAT0              0x01

    /* Flash configuration register (FCNFG)*/
    #define FTFx_SSD_FCNFG_OFFSET               0x00000002
    #define FTFx_SSD_FCNFG_CCIE                 0x80
    #define FTFx_SSD_FCNFG_RDCOLLIE             0x40
    #define FTFx_SSD_FCNFG_ERSAREQ              0x20
    #define FTFx_SSD_FCNFG_ERSSUSP              0x10
    #define FTFx_SSD_FCNFG_RAMRDY               0x02
    #define FTFx_SSD_FCNFG_EEERDY               0x01

    /* Flash security register (FSEC) */
    #define FTFx_SSD_FSEC_OFFSET                0x00000001
    #define FTFx_SSD_FSEC_KEYEN                 0xC0
    #define FTFx_SSD_FSEC_FSLACC                0x0C
    #define FTFx_SSD_FSEC_SEC                   0x03

    /* Flash Option Register (FOPT) */
    #define FTFx_SSD_FOPT_OFFSET                0x00000000

    /* Flash common command object registers (FCCOB0-B) */
    #define FTFx_SSD_FCCOB0_OFFSET              0x00000004
    #define FTFx_SSD_FCCOB1_OFFSET              0x00000005
    #define FTFx_SSD_FCCOB2_OFFSET              0x00000006
    #define FTFx_SSD_FCCOB3_OFFSET              0x00000007
    #define FTFx_SSD_FCCOB4_OFFSET              0x00000008
    #define FTFx_SSD_FCCOB5_OFFSET              0x00000009
    #define FTFx_SSD_FCCOB6_OFFSET              0x0000000A
    #define FTFx_SSD_FCCOB7_OFFSET              0x0000000B
    #define FTFx_SSD_FCCOB8_OFFSET              0x0000000C
    #define FTFx_SSD_FCCOB9_OFFSET              0x0000000D
    #define FTFx_SSD_FCCOBA_OFFSET              0x0000000E
    #define FTFx_SSD_FCCOBB_OFFSET              0x0000000F

    /* P-Flash protection registers (FPROT0-3) */
    #define FTFx_SSD_FPROT0_OFFSET              0x00000010
    #define FTFx_SSD_FPROT1_OFFSET              0x00000011
    #define FTFx_SSD_FPROT2_OFFSET              0x00000012
    #define FTFx_SSD_FPROT3_OFFSET              0x00000013

    /* D-Flash protection registers (FDPROT) */
    #define FTFx_SSD_FDPROT_OFFSET              0x00000014

    /* EERAM Protection Register (FEPROT)  */
    #define FTFx_SSD_FEPROT_OFFSET              0x00000015
#else   /* Little Endian */
    /* Flash Status Register (FSTAT)*/
    #define FTFx_SSD_FSTAT_OFFSET               0x00000000
    #define FTFx_SSD_FSTAT_CCIF                 0x80
    #define FTFx_SSD_FSTAT_RDCOLERR             0x40
    #define FTFx_SSD_FSTAT_ACCERR               0x20
    #define FTFx_SSD_FSTAT_FPVIOL               0x10
    #define FTFx_SSD_FSTAT_MGSTAT0              0x01

    /* Flash configuration register (FCNFG)*/
    #define FTFx_SSD_FCNFG_OFFSET               0x00000001
    #define FTFx_SSD_FCNFG_CCIE                 0x80
    #define FTFx_SSD_FCNFG_RDCOLLIE             0x40
    #define FTFx_SSD_FCNFG_ERSAREQ              0x20
    #define FTFx_SSD_FCNFG_ERSSUSP              0x10
    #define FTFx_SSD_FCNFG_RAMRDY               0x02
    #define FTFx_SSD_FCNFG_EEERDY               0x01

    /* Flash security register (FSEC) */
    #define FTFx_SSD_FSEC_OFFSET                0x00000002
    #define FTFx_SSD_FSEC_KEYEN                 0xC0
    #define FTFx_SSD_FSEC_FSLACC                0x0C
    #define FTFx_SSD_FSEC_SEC                   0x03

    /* Flash Option Register (FOPT) */
    #define FTFx_SSD_FOPT_OFFSET                0x00000003

    /* Flash common command object registers (FCCOB0-B) */
    #define FTFx_SSD_FCCOB0_OFFSET              0x00000007
    #define FTFx_SSD_FCCOB1_OFFSET              0x00000006
    #define FTFx_SSD_FCCOB2_OFFSET              0x00000005
    #define FTFx_SSD_FCCOB3_OFFSET              0x00000004
    #define FTFx_SSD_FCCOB4_OFFSET              0x0000000B
    #define FTFx_SSD_FCCOB5_OFFSET              0x0000000A
    #define FTFx_SSD_FCCOB6_OFFSET              0x00000009
    #define FTFx_SSD_FCCOB7_OFFSET              0x00000008
    #define FTFx_SSD_FCCOB8_OFFSET              0x0000000F
    #define FTFx_SSD_FCCOB9_OFFSET              0x0000000E
    #define FTFx_SSD_FCCOBA_OFFSET              0x0000000D
    #define FTFx_SSD_FCCOBB_OFFSET              0x0000000C

    /* P-Flash protection registers (FPROT0-3) */
    #define FTFx_SSD_FPROT0_OFFSET              0x00000013
    #define FTFx_SSD_FPROT1_OFFSET              0x00000012
    #define FTFx_SSD_FPROT2_OFFSET              0x00000011
    #define FTFx_SSD_FPROT3_OFFSET              0x00000010

    /* D-Flash protection registers (FDPROT) */
    #define FTFx_SSD_FDPROT_OFFSET              0x00000017

    /* EERAM Protection Register (FEPROT)  */
    #define FTFx_SSD_FEPROT_OFFSET              0x00000016
#endif

/*------------- Flash hardware algorithm operation commands -------------*/
#define FTFx_VERIFY_BLOCK               0x00
#define FTFx_VERIFY_SECTION             0x01
#define FTFx_PROGRAM_CHECK              0x02
#define FTFx_READ_RESOURCE              0x03
#define FTFx_PROGRAM_LONGWORD           0x06
#define FTFx_PROGRAM_PHRASE             0x07
#define FTFx_ERASE_BLOCK                0x08
#define FTFx_ERASE_SECTOR               0x09
#define FTFx_PROGRAM_SECTION            0x0B
#define FTFx_VERIFY_ALL_BLOCK           0x40
#define FTFx_READ_ONCE                  0x41
#define FTFx_PROGRAM_ONCE               0x43
#define FTFx_ERASE_ALL_BLOCK            0x44
#define FTFx_SECURITY_BY_PASS           0x45
#define FTFx_PFLASH_SWAP                0x46
#define FTFx_PROGRAM_PARTITION          0x80
#define FTFx_SET_EERAM                  0x81

/* Flash margin read settings */
#define FTFx_MARGIN_NORMAL              0x0000
#define FTFx_USER_MARGIN_LEVEL1         0x0001
#define FTFx_USER_MARGIN_LEVEL0         0x0002
#define FTFx_FACTORY_MARGIN_LEVEL0      0x0003
#define FTFx_FACTORY_MARGIN_LEVEL1      0x0004

/* PFlash swap control codes */
#define FTFx_SWAP_SET_INDICATOR_ADDR    0x01
#define FTFx_SWAP_SET_IN_PREPARE        0x02
#define FTFx_SWAP_SET_IN_COMPLETE       0x04
#define FTFx_SWAP_REPORT_STATUS         0x08

#endif  /* _SSD_FTFx_INTERNAL_H_ */
