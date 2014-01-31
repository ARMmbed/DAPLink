/************************************************************************
* (c) Copyright Freescale Semiconductor, Inc 2010, All Rights Reserved  *
*************************************************************************

*************************************************************************
*                                                                       *
*        Standard Software Flash Driver For FTFL                        *
*                                                                       *
* FILE NAME     :  SSD_Types.h                                          *
* DATE          :  September 27,2010                                    *
*                                                                       *
* AUTHOR        :  FPT Team                                             *
* E-mail        :  b33626@freescale.com                                 *
*                                                                       *
*************************************************************************/

/************************** CHANGES *************************************
0.0.1       06.09.2010      FPT Team        Initial Version
0.1.0       06.11.2010      FPT Team        Finalize to 0.1.0
0.1.1       08.16.2010      FPT Team        Finalize to 0.1.1
0.1.2       08.26.2010      FPT Team        Finalize to 0.1.2 
0.1.3       09.16.2010      FPT Team        Finalize to 0.1.3 
0.2.0       09.27.2010      FPT Team        Finalize to 0.2.0
*************************************************************************/

#ifndef _SSD_TYPES_H_
#define _SSD_TYPES_H_

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

typedef unsigned char BOOL;

typedef signed char INT8;
typedef unsigned char UINT8;
typedef volatile signed char VINT8;
typedef volatile unsigned char VUINT8;

typedef signed short INT16;
typedef unsigned short UINT16;
typedef volatile signed short VINT16;
typedef volatile unsigned short VUINT16;

typedef signed long INT32;
typedef unsigned long UINT32;
typedef volatile signed long VINT32;
typedef volatile unsigned long VUINT32;

#endif /* _SSD_TYPES_H_ */