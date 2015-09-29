/***************************************************************************//**
 * @file usbconfig.h
 * @brief USB protocol stack library, application supplied configuration options.
 * @version 4.1.0
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#ifndef __SILICON_LABS_USBCONFIG_H__
#define __SILICON_LABS_USBCONFIG_H__

#define __NO_USB_LIB_C
#include "usb_config.c"

#ifdef __cplusplus
extern "C" {
#endif

#define USB_DEVICE        /* Compile stack for device mode. */

/****************************************************************************
**                                                                         **
** Specify total number of endpoints used (in addition to EP0).            **
**                                                                         **
*****************************************************************************/
#define NUM_EP_USED USBD_EP_NUM

/****************************************************************************
**                                                                         **
** Specify number of application timers you need.                          **
**                                                                         **
*****************************************************************************/
#define NUM_APP_TIMERS 0

/****************************************************************************
**                                                                         **
** Misc. definitions for the interfaces of the composite device.           **
**                                                                         **
*****************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /* __SILICON_LABS_USBCONFIG_H__ */
