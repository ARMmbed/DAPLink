/*----------------------------------------------------------------------------
 * U S B  -  K e r n e l
 *----------------------------------------------------------------------------
 * Name:    usbhw_SAM3U.h
 * Purpose: USB Hardware Layer Definitions
 * Version: V1.21
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.21 Added HighSpeed Handling
 *          V1.20 Added USB_ClearEPBuf 
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

#ifndef __USBHW_H_SAM3U
#define __USBHW_H_SAM3U


/* USB Hardware Variables */
extern volatile BOOL USB_HighSpeed;

/* USB Hardware Functions */
extern void USB_Init        (void);
extern void USB_Connect     (BOOL con);
extern void USB_Reset       (void);
extern void USB_Suspend     (void);
extern void USB_Resume      (void);
extern void USB_WakeUp      (void);
extern void USB_WakeUpCfg   (BOOL cfg);
extern void USB_SetAddress  (U32  adr);
extern void USB_Configure   (BOOL cfg);
extern void USB_ConfigEP    (USB_ENDPOINT_DESCRIPTOR *pEPD);
extern void USB_DirCtrlEP   (U32  dir);
extern void USB_EnableEP    (U32  EPNum);
extern void USB_DisableEP   (U32  EPNum);
extern void USB_ResetEP     (U32  EPNum);
extern void USB_SetStallEP  (U32  EPNum);
extern void USB_ClrStallEP  (U32  EPNum);
extern void USB_ClearEPBuf  (U32  EPNum);
extern U32  USB_ReadEP      (U32  EPNum, U8 *pData);
extern U32  USB_WriteEP     (U32  EPNum, U8 *pData, U32 cnt);
extern U32  USB_GetFrame    (void);


#endif  /* __USBHW_H_SAM3U */
