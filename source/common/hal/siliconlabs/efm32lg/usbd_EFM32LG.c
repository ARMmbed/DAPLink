/* -----------------------------------------------------------------------------
 * Copyright (c) 2013-2015 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software. Permission is granted to anyone to use this
 * software for any purpose, including commercial applications, and to alter
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * -------------------------------------------------------------------------- */
 
 /* -----------------------------------------------------------------------------
 * Backported by Silicon Labs to CMSIS Driver v1 interface, in order to
 * support the DAPLink USB interface.
 * -------------------------------------------------------------------------- */

/* NOTE:
 *  Isochronous Endpoints are not supported
 */

#include <RTL.h>
#include <rl_usb.h>
#include <em_device.h> 
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_int.h"
#define USB_VBUS_SWITCH_NOT_PRESENT

#define __NO_USB_LIB_C
#include "usb_config.c"

#define USBD_MAX_ENDPOINT_NUM           6U

#define RX_FIFO_SIZE        896
#define TX0_FIFO_SIZE       192
#define TX1_FIFO_SIZE       192
#define TX2_FIFO_SIZE       192
#define TX3_FIFO_SIZE       192
#define TX4_FIFO_SIZE       192
#define TX5_FIFO_SIZE       192
#define TX6_FIFO_SIZE       192

#define TX_FIFO(n)          *((volatile uint32_t *)(USB->FIFO0D + (n * 1024U)))
#define RX_FIFO             *((volatile uint32_t *)(USB->FIFO0D))

#define DIEPTSIZ(ep_num)    *((volatile uint32_t *)(&USB->DIEP0TSIZ   + (ep_num * 8U)))
#define DIEPCTL(ep_num)     *((volatile uint32_t *)(&USB->DIEP0CTL    + (ep_num * 8U)))
#define DTXFSTS(ep_num)     *((volatile uint32_t *)(&USB->DIEP0TXFSTS + (ep_num * 8U)))
#define DOEPTSIZ(ep_num)    *((volatile uint32_t *)(&USB->DOEP0TSIZ   + (ep_num * 8U)))
#define DOEPCTL(ep_num)     *((volatile uint32_t *)(&USB->DOEP0CTL    + (ep_num * 8U)))
#define DIEPINT(ep_num)     *((volatile uint32_t *)(&USB->DIEP0INT    + (ep_num * 8U)))
#define DOEPINT(ep_num)     *((volatile uint32_t *)(&USB->DOEP0INT    + (ep_num * 8U)))

#define EP_IN_TYPE(num)      ((DIEPCTL(num) >> 18) & 3)
#define EP_OUT_TYPE(num)     ((DOEPCTL(num) >> 18) & 3)

uint32_t OutMaxPacketSize[6] = {USBD_MAX_PACKET0,0,0,0,0,0};

#if (USBD_HID_ENABLE == 1)
uint32_t HID_IntInPacketData     [(USBD_HID_MAX_PACKET + 3) / 4];
#endif
#if (USBD_MSC_ENABLE == 1)
uint32_t MSC_IntInPacketData     [(USBD_MSC_MAX_PACKET + 3) / 4];
#endif
#if (USBD_CDC_ACM_ENABLE == 1)
uint32_t CDC_ACM_IntInPacketData [(USBD_CDC_ACM_MAX_PACKET  + 3) / 4];
#endif

uint32_t *InPacketDataPtr[6] = { 0,
#if   ((USBD_HID_ENABLE     == 1) && (USBD_HID_EP_INTIN      == 1))
                       HID_IntInPacketData,
#elif ((USBD_CDC_ACM_ENABLE == 1) && (USBD_CDC_ACM_EP_INTIN  == 1))
                       CDC_ACM_IntInPacketData, 
#elif ((USBD_MSC_ENABLE     == 1) && (USBD_MSC_EP_INTIN      == 1))
                       MSC_IntInPacketData, 
#else
                            0, 
#endif

#if   ((USBD_HID_ENABLE     == 1) && (USBD_HID_EP_INTIN      == 2))
                       HID_IntInPacketData, 
#elif ((USBD_CDC_ACM_ENABLE == 1) && (USBD_CDC_ACM_EP_INTIN  == 2))
                       CDC_ACM_IntInPacketData, 
#elif ((USBD_MSC_ENABLE     == 1) && (USBD_MSC_EP_INTIN      == 2))
                       MSC_IntInPacketData, 
#else
                            0, 
#endif

#if   ((USBD_HID_ENABLE     == 1) && (USBD_HID_EP_INTIN      == 3))
                       HID_IntInPacketData, 
#elif ((USBD_CDC_ACM_ENABLE == 1) && (USBD_CDC_ACM_EP_INTIN  == 3))
                       CDC_ACM_IntInPacketData, 
#elif ((USBD_MSC_ENABLE     == 1) && (USBD_MSC_EP_INTIN      == 3))
                       MSC_IntInPacketData, 
#else
                            0, 
#endif
#if   ((USBD_HID_ENABLE     == 1) && (USBD_HID_EP_INTIN      == 4))
                       HID_IntInPacketData, 
#elif ((USBD_CDC_ACM_ENABLE == 1) && (USBD_CDC_ACM_EP_INTIN  == 4))
                       CDC_ACM_IntInPacketData, 
#elif ((USBD_MSC_ENABLE     == 1) && (USBD_MSC_EP_INTIN      == 4))
                       MSC_IntInPacketData, 
#else
                            0, 
#endif
#if   ((USBD_HID_ENABLE     == 1) && (USBD_HID_EP_INTIN      == 5))
                       HID_IntInPacketData, 
#elif ((USBD_CDC_ACM_ENABLE == 1) && (USBD_CDC_ACM_EP_INTIN  == 5))
                       CDC_ACM_IntInPacketData, 
#elif ((USBD_MSC_ENABLE     == 1) && (USBD_MSC_EP_INTIN      == 5))
                       MSC_IntInPacketData, 
#else
                            0, 
#endif
};


#if    (USBD_ADC_ENABLE == 1)

uint32_t ADC_IsoOutPacketData    [(USBD_ADC_WMAXPACKETSIZE + 3) / 4];

uint32_t *IsoOutPacketDataPtr[6] = { 0,
#if    (USBD_ADC_EP_ISOOUT == 1)
                                     ADC_IsoOutPacketData,
#else
                                     0,
#endif
  
#if    (USBD_ADC_EP_ISOOUT == 2)
                                     ADC_IsoOutPacketData,
#else
                                     0,
#endif
  
#if    (USBD_ADC_EP_ISOOUT == 3)
                                     ADC_IsoOutPacketData,
#else
                                     0,
#endif
#if    (USBD_ADC_EP_ISOOUT == 4)
                                     ADC_IsoOutPacketData,
#else
                                     0,
#endif
#if    (USBD_ADC_EP_ISOOUT == 5)
                                     ADC_IsoOutPacketData,
#else
                                     0,
#endif
};
#else
uint32_t *IsoOutPacketDataPtr[6] = { 0 };
#endif


uint32_t InPacketDataCnt[6]      = { 0 };
uint32_t InPacketDataReady       =   0  ;
uint32_t SyncWriteEP             =   0  ;

uint32_t IsoOutPacketDataCnt[6]  = { 0 };
uint32_t IsoOutTokenRead         =   0  ;

/*
 *  generic_delay
 *    Parameters:      delay:      Delay in 100 us ticks
 *    Return Value:    None
 */
void generic_delay (uint32_t delay) {
  delay *= SystemCoreClock / 100000;
  while (delay--) {
    __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop(); __nop();
  }
}


/*
 *  USB Device Interrupt enable
 *   Called by USBD_Init to enable the USB Interrupt
 *    Return Value:    None
 */

#ifdef __RTX
void __svc(1) USBD_IntrEna (void);
void __SVC_1               (void) {
#else
void          USBD_IntrEna (void) {
#endif
  NVIC_EnableIRQ   (USB_IRQn);
}


/*
 *  USB Device Initialize Function
 *   Called by the User to initialize USB
 *   Return Value:    None
 */

void USBD_Init (void) {
  /* Devices supporting crystal-less USB can use HFRCO or HFXO as core clock. */
  /* All other devices must use HFXO as core clock.                           */
  if ( CMU_ClockSelectGet( cmuClock_HF ) != cmuSelect_HFXO )
  {
    CMU_ClockSelectSet( cmuClock_HF, cmuSelect_HFXO );
  }
	
	//Enable clock to USB system part
	CMU_ClockEnable(cmuClock_USB, true);
	
	//Enable voltage sensing
	USB->CTRL |= USB_CTRL_VREGOSEN;
	
	//Enable PHY pins
	USB->ROUTE = USB_ROUTE_PHYPEN;
	
	//Enable LFXO reference for USB core
	CMU_OscillatorEnable(cmuOsc_LFXO,true,true);
	CMU_ClockEnable(cmuClock_USBC, true);
	CMU_ClockSelectSet(cmuClock_USBC, cmuSelect_LFXO);
	CMU_ClockEnable(cmuClock_USBC, true);
	
  CMU_ClockSelectSet( cmuClock_USBC, cmuSelect_HFCLK );
							
	while ((USB->GRSTCTL & USB_GRSTCTL_AHBIDLE) == 0U);

	USB->GRSTCTL  |=  USB_GRSTCTL_CSFTRST;            // Core soft reset
	while ((USB->GRSTCTL & USB_GRSTCTL_CSFTRST) != 0U);
	generic_delay (10);

	// Wait until AHB Master state machine is in the idle condition
	while ((USB->GRSTCTL & USB_GRSTCTL_AHBIDLE) == 0U);

	USBD_Reset ();                                    // Reset variables and endpoint settings
	
	// Set turnaround time
	USB->GUSBCFG   = ((USB->GUSBCFG & ~_USB_GUSBCFG_USBTRDTIM_MASK) |
										(5U << _USB_GUSBCFG_USBTRDTIM_SHIFT))         ;
	if (((USB->GUSBCFG & USB_GUSBCFG_FORCEDEVMODE) == 0U) || ((USB->GUSBCFG & USB_GUSBCFG_FORCEHSTMODE) == 1U)) {
		USB->GUSBCFG &= ~USB_GUSBCFG_FORCEHSTMODE;
		USB->GUSBCFG |=  USB_GUSBCFG_FORCEDEVMODE;
		generic_delay (50U);
	}
			
	// Program AHB config
	USB->GAHBCFG = USB_GAHBCFG_NPTXFEMPLVL | USB_GAHBCFG_GLBLINTRMSK;
	USB->GINTMSK = USB_GINTMSK_MODEMISMSK;
  
	// Clear SOF
	USB->GINTSTS = USB_GINTSTS_SOF;
	while((USB->GINTSTS & USB_GINTSTS_CURMOD)!= 0);
	
  generic_delay    (30);            // Wait 3ms

  USB->DCTL |= USB_DCTL_SFTDISCON;  // Soft disconnect enabled
	USB->DCFG |= USB_DCFG_DEVSPD_FS;  // Select Full-speed (48MHz)

  generic_delay    (1000);           // Wait 100ms

  USB->GINTMSK     = (USB_GINTMSK_USBSUSPMSK)  |      // suspend int unmask
										 //(USB_GINTMSK_ERLYSUSPMSK) |    // early suspend int unmask
                     (USB_GINTMSK_USBRSTMSK)   |      // reset int unmask
                     (USB_GINTMSK_ENUMDONEMSK) |      // enumeration done int unmask
                     (USB_GINTMSK_RXFLVLMSK)   |      // receive fifo non-empty int  unmask
                     (USB_GINTMSK_IEPINTMSK)   |      // IN EP int unmask
                     (USB_GINTMSK_OEPINTMSK)   |      // OUT EP int unmask
                     (USB_GINTMSK_WKUPINTMSK)  |      // resume int unmask
#ifdef __RTX
  ((USBD_RTX_DevTask   != 0) ? (USB_GINTMSK_SOFMSK) : 0);   // SOF int unmask
#else                            
  ((USBD_P_SOF_Event   != 0) ? (USB_GINTMSK_SOFMSK) : 0);   // SOF int unmask
#endif

  // Enable NVIC interrupt for USB
  USBD_IntrEna();

  // Enable interrupts and set TX FIFO interrupt level to EMPTY
  USB->GAHBCFG    |=  USB_GAHBCFG_GLBLINTRMSK | USB_GAHBCFG_NPTXFEMPLVL;
}


/*
 *  USB Device Connect Function
 *   Called by the User to Connect/Disconnect USB Device
 *    Parameters:      con:   Connect/Disconnect
 *    Return Value:    None
 */

void USBD_Connect (uint32_t con) {
  if (con) {
    USB->DCTL   &= ~USB_DCTL_SFTDISCON;          // soft disconnect disabled
  }
  else {
    USB->DCTL   |=  USB_DCTL_SFTDISCON;          // soft disconnect enabled
  }
}


/*
 *  USB Device Flush IN Endpoint Transmit Fifo
 *    Parameters:      adr:   USB Device Address
 *    Return Value:    None
 */
static void USBD_FlushInEpFifo (uint8_t ep_addr) {
	ep_addr &= ~0x80;
  if(ep_addr > USBD_MAX_ENDPOINT_NUM) return;
  USB->GRSTCTL = (USB->GRSTCTL & ~_USB_GRSTCTL_TXFNUM_MASK)         |
                 (((uint32_t)ep_addr) << _USB_GRSTCTL_TXFNUM_SHIFT) |
                  USB_GRSTCTL_TXFFLSH;
}


/*
 *  USB Device Reset Function
 *   Called automatically on USB Device Reset
 *    Return Value:    None
 */

void USBD_Reset (void) {
  uint32_t i;

  SyncWriteEP       = 0;
  InPacketDataReady = 0;
  USB->DOEPMSK      = 0;
  USB->DIEPMSK      = 0;  
  
  for (i = 1; i < (USBD_EP_NUM + 1); i++) {
    if ((DOEPCTL(i) & USB_DOEP_CTL_EPENA) != 0U) {
      DOEPCTL(i)   = USB_DOEP_CTL_EPDIS |       // Endpoint disable
                     USB_DOEP_CTL_SNAK;         // Endpoint set NAK
    }
    if ((DIEPCTL(i) & USB_DIEP_CTL_EPENA) != 0U) {
      DIEPCTL(i)   = USB_DIEP_CTL_EPDIS |       // Endpoint disable
                     USB_DIEP_CTL_SNAK;         // Endpoint set NAK
    }
    USBD_FlushInEpFifo(i);

    // Clear IN Endpoint interrupts
    DIEPINT(i) = USB_DIEP_INT_XFERCOMPL   |
								 USB_DIEP_INT_EPDISBLD    |
								 USB_DIEP_INT_INTKNTXFEMP |
								 USB_DIEP_INT_INEPNAKEFF  |
								 USB_DIEP_INT_TXFEMP      ;

    // Clear OUT Endpoint interrupts
    DOEPINT(i) = USB_DOEP_INT_XFERCOMPL   |
								 USB_DOEP_INT_EPDISBLD    |
								 USB_DOEP_INT_SETUP       |
								 USB_DOEP_INT_OUTTKNEPDIS ;
  }

	
  // Set device address to 0
  USB->DCFG       = (USB->DCFG & ~_USB_DCFG_DEVADDR_MASK);
  USB->DAINTMSK   =  USB_DAINTMSK_INEPMSK0    |     // Enable IN Endpoint0 interrupt
                     USB_DAINTMSK_OUTEPMSK0;        // Enable OUT Endpoint0 interrupt

  // Enable Setup phase done, OUT Endpoint disabled and OUT transfer complete interrupt
  USB->DOEPMSK    = USB_DOEPMSK_SETUPMSK      |
                    USB_DOEPMSK_XFERCOMPLMSK  ;

  // Enable In Endpoint disable and IN transfer complete interrupt
  USB->DIEPMSK    = USB_DIEPMSK_XFERCOMPLMSK;

  // Configure FIFOs
  USB->GRXFSIZ   =  RX_FIFO_SIZE / 4;
  USB->GNPTXFSIZ  = (RX_FIFO_SIZE/4) | ((TX0_FIFO_SIZE/4) << 16);

  USB->DIEPTXF1  = ((RX_FIFO_SIZE + TX0_FIFO_SIZE)/4) |
                   ((TX1_FIFO_SIZE/4) << 16);

  USB->DIEPTXF2  = ((RX_FIFO_SIZE + TX0_FIFO_SIZE + TX1_FIFO_SIZE)/4) |
                   ((TX2_FIFO_SIZE/4) << 16);

  USB->DIEPTXF3  = ((RX_FIFO_SIZE + TX0_FIFO_SIZE+ TX1_FIFO_SIZE +TX2_FIFO_SIZE)/4) |
                   ((TX3_FIFO_SIZE/4) << 16);

  USB->DOEP0TSIZ =  (3 << _USB_DOEP0TSIZ_SUPCNT_SHIFT) |  // setup count = 3
                    (USB_DOEP0TSIZ_PKTCNT) |              // packet count
                     USBD_MAX_PACKET0;
										 
	
}


/*
 *  USB Device Suspend Function
 *   Called automatically on USB Device Suspend
 *    Return Value:    None
 */

void USBD_Suspend (void) {
}


/*
 *  USB Device Resume Function
 *   Called automatically on USB Device Resume
 *    Return Value:    None
 */

void USBD_Resume (void) {
}


/*
 *  USB Device Remote Wakeup Function
 *   Called automatically on USB Device Remote Wakeup
 *    Return Value:    None
 */

void USBD_WakeUp (void) {
  USB->DCTL |= USB_DCTL_RMTWKUPSIG;  // remote wakeup signaling
  generic_delay (50);                // Wait ~5 ms
  USB->DCTL &= ~USB_DCTL_RMTWKUPSIG;
}


/*
 *  USB Device Remote Wakeup Configuration Function
 *    Parameters:      cfg:   Device Enable/Disable
 *    Return Value:    None
 */

void USBD_WakeUpCfg (uint32_t cfg) {
  // Not needed
}


/*
 *  USB Device Set Address Function
 *    Parameters:      adr:   USB Device Address
 *    Return Value:    None
 */

void USBD_SetAddress (uint32_t  adr, uint32_t setup) {
  if (setup) {
    USB->DCFG = (USB->DCFG & ~(_USB_DCFG_DEVADDR_MASK)) | (adr << _USB_DCFG_DEVADDR_SHIFT);
  }
}


/*
 *  USB Device Configure Function
 *    Parameters:      cfg:   Device Configure/Deconfigure
 *    Return Value:    None
 */

void USBD_Configure (uint32_t cfg) {
  InPacketDataReady &= ~1UL;
}


/*
 *  Configure USB Device Endpoint according to Descriptor
 *    Parameters:      pEPD:  Pointer to Device Endpoint Descriptor
 *    Return Value:    None
 */

void USBD_ConfigEP (USB_ENDPOINT_DESCRIPTOR *pEPD) {
  uint32_t num, val, type;

  num  = pEPD->bEndpointAddress & ~(0x80);
  val  = pEPD->wMaxPacketSize;
  type = pEPD->bmAttributes & USB_ENDPOINT_TYPE_MASK;
	
	if (num > USBD_MAX_ENDPOINT_NUM) return;

  if (pEPD->bEndpointAddress & USB_ENDPOINT_DIRECTION_MASK) {
    USB->DAINTMSK |= (1    << num);                             // unmask IN EP int
    DIEPCTL(num)   = (num  <<  _USB_DIEP_CTL_TXFNUM_SHIFT) |    // fifo number
                     (type <<  _USB_DIEP_CTL_EPTYPE_SHIFT) |    // ep type
                     (val  &  (_USB_DIEP_CTL_MPS_MASK >> _USB_DIEP_CTL_MPS_SHIFT));         // max packet size
                     
    if ((type & 3) > 1)                                         // if interrupt or bulk EP
      DIEPCTL(num) |= USB_DIEP_CTL_SETD0PIDEF;                  // DATA0 PID
  } else {
    OutMaxPacketSize[num] = val & (_USB_DIEP_CTL_MPS_MASK >> _USB_DIEP_CTL_MPS_SHIFT);

    USB->DAINTMSK |= (1 << (num + 16)); // unmask OUT EP int

    DOEPCTL(num)   = (type <<  _USB_DOEP_CTL_EPTYPE_SHIFT)                        |   // EP type
                     (val &   (_USB_DOEP_CTL_MPS_MASK >> _USB_DOEP_CTL_MPS_SHIFT));   // max packet size
                     
    DOEPTSIZ(num)  = (1 << _USB_DOEP_TSIZ_PKTCNT_SHIFT)                           |   // packet count = 1
                     (val  & (_USB_DOEP_CTL_MPS_MASK >> _USB_DOEP_CTL_MPS_SHIFT));    // transfer size
                     
    if ((type & 3) > 1)                         // if int or bulk EP
      DOEPCTL(num) |= USB_DOEP_CTL_SETD0PIDEF;  // DATA0 PID
  }
}


/*
 *  Set Direction for USB Device Control Endpoint
 *    Parameters:      dir:   Out (dir == 0), In (dir <> 0)
 *    Return Value:    None
 */

void USBD_DirCtrlEP (uint32_t dir) {
  // Not needed
}


/*
 *  Enable USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_EnableEP (uint32_t EPNum) {
  if((EPNum & 0x7F) > USBD_MAX_ENDPOINT_NUM) return;
  if (EPNum & 0x80) {
    EPNum &= ~0x80;
    DIEPCTL(EPNum)    |= USB_DIEP_CTL_USBACTEP |  // EP active
                         USB_DIEP_CTL_SNAK;       // set EP NAK
                         
    if (DIEPCTL(EPNum)&  USB_DIEP_CTL_EPENA) {
      DIEPCTL(EPNum)  |= USB_DIEP_CTL_EPDIS;      // disable EP
    }
    
    InPacketDataReady &= ~(1 << EPNum);
    if (EP_IN_TYPE(EPNum) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
      USB->GINTMSK |= USB_GINTMSK_INCOMPISOINMSK; // enable INCOMPISOIXFR
    }
  } else {
    if (EP_OUT_TYPE(EPNum) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
      USB->GINTMSK   |= USB_GINTMSK_EOPFMSK;      // enable end of periodic frame
      DOEPCTL(EPNum) |= USB_DOEP_CTL_USBACTEP;    // EP active
    } else {
      DOEPCTL(EPNum)  |= USB_DOEP_CTL_USBACTEP |  // EP active
                         USB_DOEP_CTL_EPENA    |  // enable EP
                         USB_DOEP_CTL_CNAK;       // clear EP NAK
    }
  }
}


/*
 *  Disable USB Endpoint
 *    Parameters:      EPNum: Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_DisableEP (uint32_t EPNum) {
  uint32_t num, wcnt, IsoEpEnCnt;

  IsoEpEnCnt = 0;
	
	if ((EPNum & 0x7F) > USBD_MAX_ENDPOINT_NUM) return;

  // Disable IN Endpoint
  if (EPNum & 0x80) {
    EPNum &= ~0x80;
    for (num = 1; num <= USBD_EP_NUM; num++) {
      if (DIEPCTL(num) & USB_DIEP_CTL_USBACTEP) {
        if (EP_IN_TYPE(num) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
          IsoEpEnCnt++;
        }
      }
    }

    InPacketDataReady &= ~(1 << EPNum);
    if (IsoEpEnCnt == 1) {                          // if all iso endpoints disabled
      USB->GINTMSK &= ~USB_GINTMSK_INCOMPISOINMSK;  // disable INCOMPISOIXFR
    }
    
    if (DIEPCTL(EPNum) &   USB_DIEP_CTL_EPENA) {    // if already enabled
      DIEPCTL(EPNum)   |=  USB_DIEP_CTL_EPDIS;      // disable EP
    }
    
    DIEPCTL(EPNum)     |=  USB_DIEP_CTL_SNAK;       // set EP NAK
    DIEPCTL(EPNum)     &= ~USB_DIEP_CTL_USBACTEP;   // deactivate EP
    
  // Disable OUT Endpoint
  } else {
    for (num = 1; num <= USBD_EP_NUM; num++) {
      if (DOEPCTL(num) & USB_DOEP_CTL_USBACTEP) {
        if (EP_OUT_TYPE(num) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
          IsoEpEnCnt++;
        }
      }
    }
    if (IsoEpEnCnt == 1)                            // if all iso endpoints disabled
      USB->GINTMSK &= ~USB_GINTMSK_EOPFMSK;         // disable EOPF
    
    USB->DCTL |= USB_DCTL_SGOUTNAK;                 // set global out nak

    wcnt = 1000;
    while (!(USB->GINTSTS & USB_GINTSTS_GOUTNAKEFF))  // wait until global NAK
      if ((wcnt--) == 0) break;
    
    if (DOEPCTL(EPNum) &   USB_DOEP_CTL_EPENA) {    // if ep enabled
      DOEPCTL(EPNum)   |=  USB_DOEP_CTL_EPDIS;      // disable EP
    }
    
    DOEPCTL(EPNum)     |=  USB_DOEP_CTL_SNAK;       // set EP NAK
    DOEPCTL(EPNum)     &= ~USB_DOEP_CTL_USBACTEP;   // deactivate EP

    // wait until EP disabled
    wcnt = 1000;
    while(!(DOEPINT(EPNum) & USB_DOEP_INT_EPDISBLD)) { 
      if ((wcnt--) == 0) {
        break;
      }
    }
    USB->DCTL |= USB_DCTL_CGOUTNAK;                 // clear global nak
  }
}


/*
 *  Reset USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ResetEP (uint32_t EPNum) {

	if ((EPNum & 0x7F) > USBD_MAX_ENDPOINT_NUM) return;
	
  // Reset IN Endpoint
  if (EPNum & 0x80) {
    EPNum &= ~0x80;
    InPacketDataReady &= ~(1 << EPNum);
    if (DIEPCTL(EPNum) &  USB_DIEP_CTL_EPENA) {// if Endpoint enabled
      DIEPCTL(EPNum)   |= USB_DIEP_CTL_EPDIS;  // disable EP
    }
    DIEPCTL(EPNum)     |= USB_DIEP_CTL_SNAK;  // set EP NAK

    USBD_FlushInEpFifo (EPNum | 0x80);  // Flush endpoint fifo

    // If endpoint is isochronous, set proper EVEN/ODD frame and enable Ep
    if (EP_IN_TYPE(EPNum) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
      if (USBD_GetFrame() & 1) {
        DIEPCTL(EPNum) |= USB_DIEP_CTL_SETD0PIDEF;  // set even frame
      } else {
        DIEPCTL(EPNum) |= USB_DIEP_CTL_SETD1PIDOF;  // set odd frame
      }
      DIEPCTL(EPNum)  |= USB_DIEP_CTL_EPENA | USB_DIEP_CTL_CNAK; // enable EP
    }
  }
}


/*
 *  Set Stall for USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_SetStallEP (uint32_t EPNum) {
  uint32_t wcnt;
	
	if ((EPNum & 0x7F) > USBD_MAX_ENDPOINT_NUM) return;
  
  // Stall OUT Endpoint
  if (!(EPNum & 0x80)) {
    USB->DCTL |= USB_DCTL_SGOUTNAK;                   // set global out nak
    wcnt = 1000;
    while (!(USB->GINTSTS & USB_GINTSTS_GOUTNAKEFF))  // wait until global NAK
      if ((wcnt--) == 0) break;
    
    if (DOEPCTL(EPNum) &  USB_DOEP_CTL_EPENA)         // if endpoint enabled
      DOEPCTL(EPNum)   |= USB_DOEP_CTL_EPDIS;         // disable EP
    DOEPCTL(EPNum)     |= USB_DOEP_CTL_STALL;         // set stall
    
    wcnt = 1000;
    while(!(DOEPINT(EPNum) & USB_DOEP_INT_EPDISBLD))  // wait until EP disabled
      if ((wcnt--) == 0) break;
      
    USB->DCTL |= USB_DCTL_CGOUTNAK;                   // clear global nak

  // Stall IN endpoint
  } else {
    EPNum &= ~0x80;
    if (DIEPCTL(EPNum) &  USB_DIEP_CTL_EPENA)         // if endpoint enabled
      DIEPCTL(EPNum)   |= USB_DIEP_CTL_EPDIS;         // disable endpoint
    DIEPCTL(EPNum)     |= USB_DIEP_CTL_STALL;         // set stall

    USBD_FlushInEpFifo (EPNum | 0x80);                // Flush endpoint fifo
  }
}


/*
 *  Clear Stall for USB Device Endpoint
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ClrStallEP (uint32_t EPNum) {
	if ((EPNum & 0xF) > USBD_MAX_ENDPOINT_NUM) return;
  // Clear OUT endpoint Stall
  if (!(EPNum & 0x80)) {

    if (((DOEPCTL(EPNum) >> _USB_DOEP_CTL_EPTYPE_SHIFT) & 3) > 1) { // if EP type Bulk or Interrupt
      DOEPCTL(EPNum) |= USB_DOEP_CTL_SETD0PIDEF;  // Set DATA0 PID

    DOEPCTL(EPNum) &= ~USB_DOEP_CTL_STALL;        // Clear stall
  }
 
  // Clear IN Endpoint Stall
  } else {
    EPNum &= ~0x80;

    if (DIEPCTL(EPNum) &  USB_DIEP_CTL_EPENA)     // if endpoint enabled
      DIEPCTL(EPNum)   |= USB_DIEP_CTL_EPDIS;     // disable endpoint

    USBD_FlushInEpFifo (EPNum | 0x80);            // Flush endpoint fifo

    if (((DIEPCTL(EPNum) >> _USB_DIEP_CTL_EPTYPE_SHIFT) & 3) > 1)// if Interrupt or bulk EP
      DIEPCTL(EPNum) |= USB_DIEP_CTL_SETD0PIDEF;  // Set DATA0 PID

    DIEPCTL(EPNum) &= ~USB_DOEP_CTL_STALL;        // clear Stall
  }
}


/*
 *  Clear USB Device Endpoint Buffer
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *    Return Value:    None
 */

void USBD_ClearEPBuf (uint32_t EPNum) {
	if ((EPNum & 0xF) > USBD_MAX_ENDPOINT_NUM) return;
  if (EPNum & 0x80) {
    USBD_FlushInEpFifo (EPNum | 0x80);
  } else {
    USB->GRSTCTL |= USB_GRSTCTL_RXFFLSH;
    generic_delay(2);
  }
}


/*
 *  Read USB Device Endpoint Data
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *    Return Value:    Number of bytes read
 */

uint32_t USBD_ReadEP (uint32_t EPNum, uint8_t *pData) {
  uint32_t i, sz, isoEpFlag;
	
	EPNum &= 0xF;
	if ((EPNum & 0xF) > USBD_MAX_ENDPOINT_NUM) return 0;

  if ((DOEPCTL(EPNum) & USB_DOEP_CTL_USBACTEP) == 0) return (0); // if Ep not active
  isoEpFlag = (EP_OUT_TYPE(EPNum) == USB_ENDPOINT_TYPE_ISOCHRONOUS);
  
  // Isochronous Ep: read data from intermediate buffer
  if (isoEpFlag) {                      // if Isochronouos endpoint
    if (IsoOutTokenRead == 0) {          
      sz = IsoOutPacketDataCnt[EPNum];  // get data size
      for (i = 0; i < sz; i++) {        // copy data from intermediate buffer
        pData[i] = ((uint8_t *)(IsoOutPacketDataPtr[EPNum]))[i];
      }
      IsoOutPacketDataCnt[EPNum] = 0;   // reset data count
      return (sz);
    }
  }
  
  sz = (( USB->GRXSTSP & _USB_GRXSTSP_BCNT_MASK) >> _USB_GRXSTSP_BCNT_SHIFT);    // get available data size
   if (isoEpFlag) {
    // if data pid = data0 and 
    //   number of packets in which this payload was received = 1, data is valid
    if ((DOEPTSIZ(EPNum) & (_USB_DOEP_TSIZ_PKTCNT_MASK | USB_DOEP_TSIZ_RXDPIDSUPCNT_MDATA)) == 0)  
      IsoOutPacketDataCnt[EPNum] = sz;  // save isochronous Ep data size
    else IsoOutPacketDataCnt[EPNum] = 0;// else data is unvalid
  }

  // copy data from fifo
  //  if Isochronous Ep: data is copied to intermediate buffer
  for (i = 0; i < (uint32_t)((sz+3)/4); i++) {
    *((__packed uint32_t *)pData) = RX_FIFO;
    pData += 4;
  }
  // wait RxFIFO non-empty (OUT transfer completed or Setup trans. completed)
  while ((USB->GINTSTS & USB_GINTSTS_RXFLVL) == 0);  
  USB->GRXSTSP;                         // pop register
  USB->GINTMSK |= USB_GINTMSK_RXFLVLMSK;// unmask RxFIFO non-empty interrupt

  return (sz);
}


/*
 *  Write USB Device Endpoint Data
 *  If write was requested synchronously from IRQ then data is written to FIFO directly 
 *  else data is written to the intermediate buffer and synchronously transferred to FIFO 
 *  on next NAK event.
 *    Parameters:      EPNum: Device Endpoint Number
 *                       EPNum.0..3: Address
 *                       EPNum.7:    Dir
 *                     pData: Pointer to Data Buffer
 *                     cnt:   Number of bytes to write
 *    Return Value:    Number of bytes written
 */

uint32_t USBD_WriteEP (uint32_t EPNum, uint8_t *pData, uint32_t cnt) {
  uint32_t *ptr, i, isoEpFlag;

  EPNum &= ~(0x80);
	if ((EPNum & 0xF) > USBD_MAX_ENDPOINT_NUM) return cnt;

  if ((DIEPCTL(EPNum) & USB_DIEP_CTL_USBACTEP) == 0) return (0); // if Ep not active
	
  isoEpFlag = (EP_IN_TYPE(EPNum) == USB_ENDPOINT_TYPE_ISOCHRONOUS);

  // Asynchronous write to intermediate buffer
  if (!SyncWriteEP && InPacketDataPtr[EPNum]) {
    if ((!(InPacketDataReady & (1 << EPNum))) || isoEpFlag) {
      InPacketDataCnt[EPNum] = cnt;     // save Data size
      ptr = InPacketDataPtr[EPNum];
      i   = (cnt+3)/4;
      if (i) { 
        while (i--) {                   // save data to intermediate buffer
          *ptr++ = *((__packed uint32_t *)pData);
          pData +=4;
        }
      }
      if (!isoEpFlag) {
        InPacketDataReady |=  1 << EPNum;
        DIEPCTL(EPNum)    |= USB_DIEP_CTL_SNAK;         // Set NAK to enable interrupt on NAK
        USB->DIEPMSK      |= USB_DIEPMSK_INEPNAKEFFMSK; // INEPNEM = 1, IN EP NAK efective msk
      }
    } else {                            // If packet already loaded to buffer
      return 0;
    }
  } else {
    if (cnt) while ((DTXFSTS(EPNum) * 4) < cnt); // get space in Ep TxFIFO
		// Make FIFO filling an atomic task
		INT_Disable();

    // Set transfer size and packet count
    DIEPTSIZ(EPNum) = cnt                                 | 
                      (1 << _USB_DIEP_TSIZ_PKTCNT_SHIFT)  | 
                      (1 << _USB_DIEP_TSIZ_MC_SHIFT); 
                             
    if (isoEpFlag) { // if Isochronous Ep: set packet frame
      if (USBD_GetFrame() & 1) DIEPCTL(EPNum) |= USB_DIEP_CTL_SETD0PIDEF; // even frame
      else                     DIEPCTL(EPNum) |= USB_DIEP_CTL_SETD1PIDOF; // odd frame
    }
    
    DIEPCTL(EPNum) |= USB_DIEP_CTL_EPENA | USB_DIEP_CTL_CNAK;  // enable ep and clear NAK
    if (cnt) {
      ptr = (uint32_t *)pData;
      i   = (cnt+3)/4;
      while (i--) {                     
        // Copy data to endpoint TxFIFO
        TX_FIFO(EPNum) = *(__packed uint32_t *)pData;
        pData +=4;
      }
    }
    if (isoEpFlag == 0) InPacketDataReady &= ~(1 << EPNum);
		
		// End of atomic block
		INT_Enable();
  }
  return (cnt);
}


/*
 *  Get USB Device Last Frame Number
 *    Parameters:      None
 *    Return Value:    Frame Number
 */

uint32_t USBD_GetFrame (void) {
  return ((USB->DSTS & _USB_DSTS_SOFFN_MASK) >> _USB_DSTS_SOFFN_SHIFT);
}


/*
 *  USB Device Interrupt Service Routine
 */
void USB_IRQHandler(void) {
  uint32_t istr, val, num, i, msk;
  static uint32_t IsoInIncomplete = 0;

  istr = USB->GINTSTS & USB->GINTMSK;

// reset interrupt
  if ((istr & USB_GINTSTS_USBRST) != 0U) {
    USBD_Reset();
    usbd_reset_core();
#ifdef __RTX
    if (USBD_RTX_DevTask) {
      isr_evt_set(USBD_EVT_RESET, USBD_RTX_DevTask);
    }
#else
    if (USBD_P_Reset_Event) {
      USBD_P_Reset_Event();
    }
#endif
    USB->GINTSTS = USB_GINTSTS_USBRST;
  }

// suspend interrupt
  if ((istr & USB_GINTSTS_USBSUSP) != 0U) {
    USBD_Suspend();
#ifdef __RTX
    if (USBD_RTX_DevTask) {
      isr_evt_set(USBD_EVT_SUSPEND, USBD_RTX_DevTask);
    }
#else
    if (USBD_P_Suspend_Event) {
      USBD_P_Suspend_Event();
    }
#endif
    USB->GINTSTS = USB_GINTSTS_USBSUSP;
  }

// resume interrupt
  if ((istr & USB_GINTSTS_WKUPINT) != 0U) {
    USBD_Resume();
#ifdef __RTX
    if (USBD_RTX_DevTask) {
      isr_evt_set(USBD_EVT_RESUME, USBD_RTX_DevTask);
    }
#else
    if (USBD_P_Resume_Event) {
      USBD_P_Resume_Event();
    }
#endif
    USB->GINTSTS = USB_GINTSTS_WKUPINT;
  }

// speed enumeration completed
  if ((istr & USB_GINTSTS_ENUMDONE) != 0U) {
    USB->DIEP0CTL &= ~(_USB_DIEP0CTL_MPS_MASK << _USB_DIEP0CTL_MPS_SHIFT);
    switch (USBD_MAX_PACKET0) {
      case 8:
        USB->DIEP0CTL |= USB_DIEP0CTL_MPS_8B;
      break;

      case 16:
        USB->DIEP0CTL |= USB_DIEP0CTL_MPS_16B;
      break;

      case 32:
        USB->DIEP0CTL |= USB_DIEP0CTL_MPS_32B;
      break;
			default:
				break;
    }
    USB->DCTL    |= USB_DCTL_CGNPINNAK;     // clear global IN NAK
    USB->DCTL    |= USB_DCTL_CGOUTNAK;      // clear global OUT NAK
    USB->GINTSTS  = USB_GINTSTS_ENUMDONE;
  }

// Start Of Frame
  if (istr & USB_GINTSTS_SOF) {
#ifdef __RTX
    if (USBD_RTX_DevTask) {
      isr_evt_set(USBD_EVT_SOF, USBD_RTX_DevTask);
    }
#else
    if (USBD_P_SOF_Event) {
      USBD_P_SOF_Event();
    }
#endif
     USB->GINTSTS = USB_GINTSTS_SOF;
  }

// RxFIFO non-empty
  if ((istr & USB_GINTSTS_RXFLVL) != 0U) {
    val = USB->GRXSTSR;
    num = val & _USB_GRXSTSR_CHEPNUM_MASK;

    switch ((val & _USB_GRXSTSR_PKTSTS_MASK) >> _USB_GRXSTSR_PKTSTS_SHIFT) {
// setup packet
      case _USB_GRXSTSR_PKTSTS_SETUPRCV:
#ifdef __RTX
      USB->GINTMSK &= ~USB_GINTMSK_RXFLVLMSK;
      if (USBD_RTX_EPTask[num]) {
        isr_evt_set(USBD_EVT_SETUP, USBD_RTX_EPTask[num]);
      }
#else
      if (USBD_P_EP[num]) {
        USBD_P_EP[num](USBD_EVT_SETUP);
      }
#endif
      break;

// OUT packet
      case _USB_GRXSTSR_PKTSTS_PKTRCV:
      USB->GINTMSK &= ~USB_GINTMSK_RXFLVLMSK;
      if (EP_OUT_TYPE(num) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
        IsoOutTokenRead = 1;
        USBD_ReadEP (num, (uint8_t *)IsoOutPacketDataPtr[num]);
        IsoOutTokenRead = 0;
      } else {
#ifdef __RTX
        if (USBD_RTX_EPTask[num]) {
          isr_evt_set(USBD_EVT_OUT, USBD_RTX_EPTask[num]);
        }
#else
        if (USBD_P_EP[num]) {
          USBD_P_EP[num](USBD_EVT_OUT);
        }
#endif
      }
      break;

      default:
        USB->GRXSTSP;
    }
  }

// OUT Packet
  if ((istr & USB_GINTSTS_OEPINT) != 0U) {
    msk = (((USB->DAINT & USB->DAINTMSK) >> 16) & 0xFFFF);
    i   = 0;
    while (msk) {
      num = 0;
      for (; i < (USBD_EP_NUM+1); i++) {
        if ((msk >> i) & 1) {
          num = i;
          msk &= ~(1 << i);
          break;
        }
      }

      // Endpoint disabled
      if (DOEPINT(num) & USB_DOEP_INT_EPDISBLD) {
        if (EP_OUT_TYPE(num) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
          DOEPTSIZ(num) = (1 << 19) |                // packet count
                          (OutMaxPacketSize[num]);   // transfer size
          
          if ((USBD_GetFrame() & 1)) DOEPCTL(num) |= USB_DOEP_CTL_SETD0PIDEF; // even frame
          else                       DOEPCTL(num) |= USB_DOEP_CTL_SETD1PIDOF; // odd frame

          DOEPCTL(num)    |= USB_DOEP_CTL_EPENA | USB_DOEP_CTL_CNAK;
        }
        DOEPINT(num) |= USB_DOEP_INT_EPDISBLD;
      }
      
      // Transfer complete interrupt
      if ((DOEPINT(num) & USB_DOEP_INT_XFERCOMPL) | (DOEPINT(num) & USB_DOEP_INT_SETUP)) {
        if (EP_OUT_TYPE(num) != USB_ENDPOINT_TYPE_ISOCHRONOUS) {

          DOEPTSIZ(num)    = (1 << _USB_DOEP_TSIZ_PKTCNT_SHIFT) | // packet count
                             (OutMaxPacketSize[num]);             // transfer size
          if (num == 0) {
            DOEPTSIZ(0)   |= (1 << _USB_DOEP_TSIZ_RXDPIDSUPCNT_SHIFT);
          }
          
          //Enable endpoint, clear NAK
          DOEPCTL(num)    |= USB_DOEP_CTL_EPENA | USB_DOEP_CTL_CNAK;
        }
        DOEPINT(num) |= USB_DOEP_INT_XFERCOMPL;
      }
    }
  }

// IN Packet
  if (istr & USB_GINTSTS_IEPINT) {
    msk = (USB->DAINT & USB->DAINTMSK & 0xFFFF);
    i   = 0;
    while (msk) {
      num = 0;
      for (; i < (USBD_EP_NUM+1); i++) {
        if ((msk >> i) & 1) {
          num = i;
          msk &= ~(1 << i);
          break;
        }
      }

      // Endpoint disabled
      if (DIEPINT(num) & USB_DIEP_INT_EPDISBLD) {
        DIEPINT(num) = USB_DIEP_INT_EPDISBLD;
        
        if (EP_IN_TYPE(num) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
          if ((IsoInIncomplete & (1 << num)) != 0) {

            USBD_FlushInEpFifo(num | 0x80);
            SyncWriteEP = 1;
            USBD_WriteEP (num, (uint8_t *)InPacketDataPtr[num], InPacketDataCnt[num]);
            SyncWriteEP = 0;

            IsoInIncomplete &= ~(1 << num);
          }
        }
      }

      // IN endpoint NAK effective
      if (DIEPINT(num) & USB_DIEP_INT_INEPNAKEFF) {
        if (EP_IN_TYPE(num) != USB_ENDPOINT_TYPE_ISOCHRONOUS) {
          if (InPacketDataPtr[num] && (InPacketDataReady & (1 << num))) {
            SyncWriteEP = 1;
            USBD_WriteEP (num, (uint8_t *)InPacketDataPtr[num], InPacketDataCnt[num]);
            SyncWriteEP = 0;
            if (!InPacketDataReady)                       // No more pending IN transfers
              USB->DIEPMSK &= ~USB_DIEPMSK_INEPNAKEFFMSK; // Disable IN NAK interrupts
            continue;
          } else 
              DIEPCTL(num) |= USB_DIEP_CTL_CNAK;          //clear NAK for endpoint
            DIEPINT(num)    = USB_DIEP_INT_INEPNAKEFF;    //clear NAK interrupt bit
        }
      }

      // Transmit completed
      if (DIEPINT(num) & USB_DIEP_INT_XFERCOMPL) {
        DIEPINT(num) = USB_DIEP_INT_XFERCOMPL;
        SyncWriteEP = 1;
        if (EP_IN_TYPE(num) == USB_ENDPOINT_TYPE_ISOCHRONOUS) {
          USBD_WriteEP (num, (uint8_t *)InPacketDataPtr[num], InPacketDataCnt[num]);
        } else {
#ifdef __RTX
          if (USBD_RTX_EPTask[num]) {
            isr_evt_set(USBD_EVT_IN,  USBD_RTX_EPTask[num]);
          }
#else
          if (USBD_P_EP[num]) {
            USBD_P_EP[num](USBD_EVT_IN);
          }
#endif
        }
        SyncWriteEP = 0;
      }
    }
  }

// End of periodic frame
  if (istr & USB_GINTSTS_EOPF) {
    for (num = 1; num <= USBD_EP_NUM; num++) {

      if (EP_OUT_TYPE(num) != USB_ENDPOINT_TYPE_ISOCHRONOUS) continue;
      if (((DOEPCTL(num) >> 15) & 1) == 0)                   continue;

      // incomplete isochronous out transfer
      if (USB->GINTSTS & USB_GINTSTS_INCOMPLP) {
        if ((USBD_GetFrame() & 1) == ((DOEPCTL(num) >> 16) & 1)) {
          if (DOEPCTL(num) & USB_DOEP_CTL_EPENA) {
            InPacketDataCnt[num] = 0;             // discard data
            DOEPCTL(num) |= USB_DOEP_CTL_EPDIS;   // disable endpoint
          }
        }

      // prepare for next isohronous transfer
      } else {
        DOEPTSIZ(num) = (1 << _USB_DOEP_TSIZ_PKTCNT_SHIFT) |  // packet count
                        (OutMaxPacketSize[num]);              // transfer size

        if ((USBD_GetFrame() & 1)) {
          DOEPCTL(num) |= USB_DOEP_CTL_SETD0PIDEF; // even frame
        } else {
          DOEPCTL(num) |= USB_DOEP_CTL_SETD1PIDOF; // odd frame
        }

        DOEPCTL(num)    |= USB_DOEP_CTL_EPENA | USB_DOEP_CTL_CNAK;
      }
    }
    USB->GINTSTS = USB_GINTSTS_EOPF | USB_GINTSTS_INCOMPLP;
  }

// incomplete isochronous IN transfer
  if (istr & USB_GINTSTS_INCOMPISOIN) {
    USB->GINTSTS = USB_GINTSTS_INCOMPISOIN;
    for (num = 1; num < (USBD_EP_NUM + 1); num++) {

      if (EP_IN_TYPE(num) != USB_ENDPOINT_TYPE_ISOCHRONOUS) continue;
      if (((DIEPCTL(num) >> 15) & 1) == 0)                  continue;

      if (DIEPCTL(num) & USB_DIEP_CTL_EPENA) { // if EP en & packet frame is incorect
        if ((USBD_GetFrame() & 1) == ((DIEPCTL(num) >> 16) & 1)) {
          
          IsoInIncomplete |= (1 << num);
          DIEPCTL(num)    |= USB_DIEP_CTL_EPDIS | USB_DIEP_CTL_SNAK;
        }
      }
    }
  }
}
