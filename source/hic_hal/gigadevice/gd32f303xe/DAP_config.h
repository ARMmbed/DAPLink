/**************************************************************************//**
 * @file     DAP_config.h
 * @brief    CMSIS-DAP Configuration File for STM32F103C6/8/B
 * @version  V1.00
 * @date     02. Oct 2012
 *
 * @note
 * Copyright (C) 2012 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#ifndef __DAP_CONFIG_H__
#define __DAP_CONFIG_H__

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

typedef const struct
{
    void (* LED_CONNECTED)  (uint16_t);
    void (* LED_RUNNING)    (uint16_t);
} CoreDescriptor_t;

extern const CoreDescriptor_t * pCoreDescriptor;

typedef const struct
{
    void     (*UserInit)    (CoreDescriptor_t * core);
    uint32_t (*UserProcess) (uint8_t *, uint8_t *);
    void     (*UserAbort)   (void);
} UserAppDescriptor_t;

#if defined( __GNUC__ )
    /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
       set to 'Yes') calls __io_putchar() */
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#elif defined( __CC_ARM )
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#else
    #error "Unknown compiler"
#endif

#ifdef USE_DEBUG
    #define DEBUG(...)  printf(__VA_ARGS__)
    #define INFO(...)   printf(__VA_ARGS__)
    #define ERROR(...)  printf(__VA_ARGS__)
#else
    #ifdef USE_INFO
        #define DEBUG(...)
        #define INFO(...)   printf(__VA_ARGS__)
        #define ERROR(...)  printf(__VA_ARGS__)
    #else
        #ifdef USE_ERROR
            #define DEBUG(...)
            #define INFO(...)
            #define ERROR(...)  printf(__VA_ARGS__)
        #else
            #define DEBUG(...)
            #define INFO(...)
            #define ERROR(...)
        #endif
    #endif
#endif

//**************************************************************************************************
/** 
\defgroup DAP_Config_Debug_gr CMSIS-DAP Debug Unit Information
\ingroup DAP_ConfigIO_gr 
@{
Provides definitions about:
 - Definition of Cortex-M processor parameters used in CMSIS-DAP Debug Unit.
 - Debug Unit communication packet size.
 - Debug Access Port communication mode (JTAG or SWD).
 - Optional information about a connected Target Device (for Evaluation Boards).
*/

#include "IO_Config.h"

/* Processor Clock of the Cortex-M MCU used in the Debug Unit.
 * This value is used to calculate the SWD/JTAG clock speed.
 */
#define CPU_CLOCK               SystemCoreClock /*< Specifies the CPU Clock in Hz */

/* Number of processor cycles for I/O Port write operations.
 * This value is used to calculate the SWD/JTAG clock speed that is generated with I/O
 * Port write operations in the Debug Unit by a Cortex-M MCU. Most Cortex-M processors
 * requrie 2 processor cycles for a I/O Port Write operation.  If the Debug Unit uses
 * a Cortex-M0+ processor with high-speed peripheral I/O only 1 processor cycle might be 
 * requrired.
 */
#define IO_PORT_WRITE_CYCLES    2               /*< I/O Cycles: 2=default, 1=Cortex-M0+ fast I/0 */

/* Indicate that Serial Wire Debug (SWD) communication mode is available at the Debug Access Port.
 * This information is returned by the command \ref DAP_Info as part of <b>Capabilities</b>.
 */
#define DAP_SWD                 1               /*< SWD Mode: 1 = available, 0 = not available */

/* Indicate that JTAG communication mode is available at the Debug Port.
 * This information is returned by the command \ref DAP_Info as part of <b>Capabilities</b>.
 */
#define DAP_JTAG                0               /*< JTAG Mode: 1 = available, 0 = not available. */

/* Configure maximum number of JTAG devices on the scan chain connected to the Debug Access Port.
 * This setting impacts the RAM requirements of the Debug Unit. Valid range is 1 .. 255.
 */
#define DAP_JTAG_DEV_CNT        8               /*< Maximum number of JTAG devices on scan chain */

/* Default communication mode on the Debug Access Port.
 * Used for the command \ref DAP_Connect when Port Default mode is selected.
 */
#define DAP_DEFAULT_PORT        1               /*< Default JTAG/SWJ Port Mode: 1 = SWD, 2 = JTAG. */

/* Default communication speed on the Debug Access Port for SWD and JTAG mode.
 * Used to initialize the default SWD/JTAG clock frequency.
 * The command \ref DAP_SWJ_Clock can be used to overwrite this default setting.
 */
#define DAP_DEFAULT_SWJ_CLOCK   1000000         /*< Default SWD/JTAG clock frequency in Hz. */

/* Maximum Package Size for Command and Response data.
 * This configuration settings is used to optimized the communication performance with the
 * debugger and depends on the USB peripheral. Change setting to 1024 for High-Speed USB.
 */
#define DAP_PACKET_SIZE         64              /*< USB: 64 = Full-Speed, 1024 = High-Speed. */

/* Maximum Package Buffers for Command and Response data.
 * This configuration settings is used to optimized the communication performance with the
 * debugger and depends on the USB peripheral. For devices with limited RAM or USB buffer the
 * setting can be reduced (valid range is 1 .. 255). Change setting to 4 for High-Speed USB.
 */
#define DAP_PACKET_COUNT        64              /*< Buffers: 64 = Full-Speed, 4 = High-Speed. */

/// Indicate that UART Serial Wire Output (SWO) trace is available.
/// This information is returned by the command \ref DAP_Info as part of <b>Capabilities</b>.
#define SWO_UART                0               ///< SWO UART:  1 = available, 0 = not available

/// Maximum SWO UART Baudrate
#define SWO_UART_MAX_BAUDRATE   10000000U       ///< SWO UART Maximum Baudrate in Hz

/// Indicate that Manchester Serial Wire Output (SWO) trace is available.
/// This information is returned by the command \ref DAP_Info as part of <b>Capabilities</b>.
#define SWO_MANCHESTER          0               ///< SWO Manchester:  1 = available, 0 = not available

/// SWO Trace Buffer Size.
#define SWO_BUFFER_SIZE         4096U           ///< SWO Trace Buffer Size in bytes (must be 2^n)


/* Debug Unit is connected to fixed Target Device.
 * The Debug Unit may be part of an evaluation board and always connected to a fixed
 * known device.  In this case a Device Vendor and Device Name string is stored which
 * may be used by the debugger or IDE to configure device parameters.
 */
#define TARGET_DEVICE_FIXED     0               /*< Target Device: 1 = known, 0 = unknown. */

#if TARGET_DEVICE_FIXED
    #define TARGET_DEVICE_VENDOR    ""          /*< String indicating the Silicon Vendor */
    #define TARGET_DEVICE_NAME      ""          /*< String indicating the Target Device */
#endif

/* Number of times to retry reading the CTLR/STAT
 * register while waiting for powerup acknowledge */
#define PWRUP_TIMEOUT          100

/* Number of times to retry reading status registers while
 * waiting for a debug event (such as a halt of soft reset) */
#define DEBUG_EVENT_TIMEOUT    200

/* Number of times to wait for MSC operation to complete */
#define MSC_TIMEOUT           60000

/* Number of times to retry an SWD operation when receiving 
 * a WAIT response */
#define SWD_RETRY_COUNT 2000
///@}

//**************************************************************************************************
/** 
\defgroup DAP_Config_PortIO_gr CMSIS-DAP Hardware I/O Pin Access
\ingroup DAP_ConfigIO_gr 
@{

Standard I/O Pins of the CMSIS-DAP Hardware Debug Port support standard JTAG mode
and Serial Wire Debug (SWD) mode. In SWD mode only 2 pins are required to implement the debug 
interface of a device. The following I/O Pins are provided:

JTAG I/O Pin                 | SWD I/O Pin          | CMSIS-DAP Hardware pin mode
---------------------------- | -------------------- | ---------------------------------------------
TCK: Test Clock              | SWCLK: Clock         | Output Push/Pull
TMS: Test Mode Select        | SWDIO: Data I/O      | Output Push/Pull; Input (for receiving data)
TDI: Test Data Input         |                      | Output Push/Pull
TDO: Test Data Output        |                      | Input             
nTRST: Test Reset (optional) |                      | Output Open Drain with pull-up resistor
nRESET: Device Reset         | nRESET: Device Reset | Output Open Drain with pull-up resistor


DAP Hardware I/O Pin Access Functions
-------------------------------------
The various I/O Pins are accessed by functions that implement the Read, Write, Set, or Clear to 
these I/O Pins. 

For the SWDIO I/O Pin there are additional functions that are called in SWD I/O mode only.
This functions are provided to achieve faster I/O that is possible with some advanced GPIO 
peripherals that can independently write/read a single I/O pin without affecting any other pins 
of the same I/O port. The following SWDIO I/O Pin functions are provided:
 - \ref PIN_SWDIO_OUT_ENABLE to enable the output mode from the DAP hardware.
 - \ref PIN_SWDIO_OUT_DISABLE to enable the input mode to the DAP hardware.
 - \ref PIN_SWDIO_IN to read from the SWDIO I/O pin with utmost possible speed.
 - \ref PIN_SWDIO_OUT to write to the SWDIO I/O pin with utmost possible speed.
*/


/* Configure DAP I/O pins ------------------------------ */

/** Setup JTAG I/O pins: TCK, TMS, TDI, TDO, nTRST, and nRESET.
Configures the DAP Hardware I/O pins for JTAG mode:
 - TCK, TMS, TDI, nTRST, nRESET to output mode and set to high level.
 - TDO to input mode.
*/
static inline void PORT_JTAG_SETUP(void)
{
#if (DAP_JTAG != 0)
    GPIO_BOP(PIN_SWCLK_TCK_PORT) = PIN_SWCLK_TCK | PIN_SWDIO_TMS | PIN_TDI | PIN_TDO;
    GPIO_BC(PIN_nRESET_PORT) = PIN_nRESET;
    PIN_nRESET_HIGH();

    gpio_initial(PIN_TDO_PORT, GPIO_MODE_IPU, 0, PIN_TDO);
    gpio_initial(PIN_SWCLK_TCK_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, PIN_SWCLK_TCK | PIN_SWDIO_TMS | PIN_TDI);
#endif
}

/** Setup SWD I/O pins: SWCLK, SWDIO, and nRESET.
Configures the DAP Hardware I/O pins for Serial Wire Debug (SWD) mode:
 - SWCLK, SWDIO, nRESET to output mode and set to default high level.
 - TDI, TMS, nTRST to HighZ mode (pins are unused in SWD mode).
*/
static __inline void PORT_SWD_SETUP(void)
{
    RCU_APB2EN |= PIN_SWDIO_TMS_RCU;
    RCU_APB2EN |= PIN_SWCLK_TCK_RCU;

    GPIO_BOP(PIN_SWCLK_TCK_PORT) = (PIN_SWCLK_TCK | PIN_SWDIO_TMS);
    GPIO_BC(PIN_nRESET_PORT) = PIN_nRESET;

#if (DAP_JTAG != 0)
    gpio_initial(PIN_TDI_PORT, GPIO_MODE_IPU, 0, PIN_TDO | PIN_TDI);
#endif

    gpio_initial(PIN_SWCLK_TCK_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, PIN_SWCLK_TCK | PIN_SWDIO_TMS);

    PIN_nRESET_HIGH();
}

/** Disable JTAG/SWD I/O Pins.
Disables the DAP Hardware I/O pins which configures:
 - TCK/SWCLK, TMS/SWDIO, TDI, TDO, nTRST, nRESET to High-Z mode.
*/
static __inline void PORT_OFF(void)
{
#if (DAP_JTAG != 0)
    gpio_initial(PIN_SWCLK_TCK_PORT, GPIO_MODE_IPU, 0, PIN_SWCLK_TCK | PIN_SWDIO_TMS | PIN_TDI | PIN_TDO);
#else
    gpio_initial(PIN_SWCLK_TCK_PORT, GPIO_MODE_IPU, 0, PIN_SWCLK_TCK | PIN_SWDIO_TMS);
#endif

    gpio_initial(PIN_nRESET_PORT, GPIO_MODE_IPU, 0, PIN_nRESET);
}

/* SWCLK/TCK I/O pin ------------------------------------- */

/**
  * @brief  SWCLK/TCK I/O pin: Get Input
  * @param  None
  * @retval Current status of the SWCLK/TCK DAP hardware I/O pin
  */
static __forceinline uint32_t PIN_SWCLK_TCK_IN(void)
{
    return (GPIO_OCTL(PIN_SWCLK_TCK_PORT) & PIN_SWCLK_TCK) ? 1 : 0;
}

/**
  * @brief  SWCLK/TCK I/O pin: Set Output to High
            Set the SWCLK/TCK DAP hardware I/O pin to high level.
  * @param  None
  * @retval None
  */
static __forceinline void PIN_SWCLK_TCK_SET(void)
{
    GPIO_BOP(PIN_SWCLK_TCK_PORT) = PIN_SWCLK_TCK;
}

/**
  * @brief  SWCLK/TCK I/O pin: Set Output to Low
            Set the SWCLK/TCK DAP hardware I/O pin to low level.
  * @param  None
  * @retval None
  */
static __forceinline void PIN_SWCLK_TCK_CLR (void)
{
    GPIO_BC(PIN_SWCLK_TCK_PORT) = PIN_SWCLK_TCK;
}


/* SWDIO/TMS Pin I/O -------------------------------------- */

/**
  * @brief  SWDIO/TMS I/O pin: Get Input
  * @param  None
  * @retval Current status of the SWDIO/TMS DAP hardware I/O pin
  */
static __forceinline uint32_t PIN_SWDIO_TMS_IN(void)
{
    return (GPIO_ISTAT(PIN_SWDIO_TMS_PORT) & PIN_SWDIO_TMS) ? 1 : 0;
}

/**
  * @brief  SWDIO/TMS I/O pin: Set Output to High
            Set the SWDIO/TMS DAP hardware I/O pin to high level
  * @param  None
  * @retval None
  */
static __forceinline void PIN_SWDIO_TMS_SET(void)
{
    GPIO_BOP(PIN_SWDIO_TMS_PORT) = PIN_SWDIO_TMS;
}

/**
  * @brief  SWDIO/TMS I/O pin: Set Output to Low
            Set the SWDIO/TMS DAP hardware I/O pin to low level
  * @param  None
  * @retval None
  */
static __forceinline void PIN_SWDIO_TMS_CLR(void)
{
    GPIO_BC(PIN_SWDIO_TMS_PORT) = PIN_SWDIO_TMS;
}

/**
  * @brief  SWDIO I/O pin: Get Input (used in SWD mode only)
  * @param  None
  * @retval Current status of the SWDIO DAP hardware I/O pin
  */
static __forceinline uint8_t PIN_SWDIO_IN (void)
{
    if (GPIO_ISTAT(PIN_SWDIO_TMS_PORT) & PIN_SWDIO_TMS) return 1;

    return 0;
}

/**
  * @brief  SWDIO I/O pin: Set Output (used in SWD mode only)
  * @param  None
  * @retval bit Output value for the SWDIO DAP hardware I/O pin
  */
static __forceinline void PIN_SWDIO_OUT(uint32_t bit)
{
    if (bit & 1)
        GPIO_BOP(PIN_SWDIO_TMS_PORT) = PIN_SWDIO_TMS;
    else
        GPIO_BC(PIN_SWDIO_TMS_PORT) = PIN_SWDIO_TMS;
}

/**
  * @brief  SWDIO I/O pin: Switch to Output mode (used in SWD mode only)
            Configure the SWDIO DAP hardware I/O pin to output mode. This function is
            called prior \ref PIN_SWDIO_OUT function calls
  * @param  None
  * @retval None
  */
static __forceinline void PIN_SWDIO_OUT_ENABLE(void)
{
    PIN_SWDIO_TMS_OUT_ENABLE();
}

/**
  * @brief  SWDIO I/O pin: Switch to Input mode (used in SWD mode only)
            Configure the SWDIO DAP hardware I/O pin to input mode. This function is
            called prior \ref PIN_SWDIO_IN function calls
  * @param  None
  * @retval None
  */
static __forceinline void PIN_SWDIO_OUT_DISABLE(void)
{
    PIN_SWDIO_TMS_OUT_DISABLE();
}


/* TDI Pin I/O --------------------------------------------- */

/**
  * @brief  TDI I/O pin: Get Input
  * @param  None
  * @retval Current status of the TDI DAP hardware I/O pin
  */
__STATIC_INLINE uint8_t PIN_TDI_IN(void)
{
#if (DAP_JTAG != 0)
    return (GPIO_OCTL(PIN_TDI_PORT) & PIN_TDI) ? 1 : 0;
#else
    return 0;
#endif
}

/**
  * @brief  TDI I/O pin: Set Output
  * @param  None
  * @retval bit Output value for the TDI DAP hardware I/O pin
  */
__STATIC_INLINE void PIN_TDI_OUT(uint8_t bit)
{
#if (DAP_JTAG != 0)
    if (bit & 1)
        GPIO_BOP(PIN_TDI_PORT) = PIN_TDI;
    else
        GPIO_BC(PIN_TDI_PORT) = PIN_TDI;
#else
    
#endif
}


/* TDO Pin I/O --------------------------------------------- */

/**
  * @brief  TDO I/O pin: Set Output
  * @param  None
  * @retval bit Output value for the TDO DAP hardware I/O pin
  */
__STATIC_INLINE uint8_t PIN_TDO_IN(void)
{
#if (DAP_JTAG != 0)
    return (GPIO_ISTAT(PIN_TDO_PORT) & PIN_TDO) ? 1 : 0;
#else
    return 0;
#endif
}


/* nTRST Pin I/O ------------------------------------------- */

/**
  * @brief  nTRST I/O pin: Get Input
  * @param  None
  * @retval Current status of the nTRST DAP hardware I/O pin
  */
static __forceinline uint8_t PIN_nTRST_IN(void)
{
    return (0);   // Not available
}

/**
  * @brief  nTRST I/O pin: Set Output
  * @param  bit: JTAG TRST Test Reset pin status:
                 - 0: issue a JTAG TRST Test Reset.
                 - 1: release JTAG TRST Test Reset.
  * @retval None
  */
static __forceinline void PIN_nTRST_OUT(uint8_t bit)
{

}

/* nRESET Pin I/O------------------------------------------ */

/**
  * @brief  nRESET  I/O pin: Get Input
  * @param  None
  * @retval Current status of the nRESET DAP hardware I/O pin
  */
__STATIC_INLINE uint8_t PIN_nRESET_IN(void)
{
    if (GPIO_ISTAT(PIN_nRESET_PORT) & PIN_nRESET) {
        return 1;
    }

    return 0;
}

/**
  * @brief  nRESET I/O pin: Set Output
  * @param  bit: target device hardware reset pin status:
                 - 0: issue a device hardware reset.
                 - 1: release device hardware reset.
  * @retval None
  */
static __forceinline void PIN_nRESET_OUT(uint8_t bit)
{
    if (bit) {
        PIN_nRESET_HIGH();
    } else {
        PIN_nRESET_LOW();
    }
}

///@}

//**************************************************************************************************
/** 
\defgroup DAP_Config_LEDs_gr CMSIS-DAP Hardware Status LEDs
\ingroup DAP_ConfigIO_gr
@{

CMSIS-DAP Hardware may provide LEDs that indicate the status of the CMSIS-DAP Debug Unit.

It is recommended to provide the following LEDs for status indication:
 - Connect LED: is active when the DAP hardware is connected to a debugger.
 - Running LED: is active when the debugger has put the target device into running state.
*/

/**
  * @brief  Debug Unit: Set status of Connected LED
  * @param  bit: status of the Connect LED:
                 - 1: Connect LED ON: debugger is connected to CMSIS-DAP Debug Unit.
                 - 0: Connect LED OFF: debugger is not connected to CMSIS-DAP Debug Unit.
  * @retval None
  */
static inline void LED_CONNECTED_OUT(uint32_t bit)
{

}

/**
  * @brief  Debug Unit: Set status Target Running LED
  * @param  bit: status of the Target Running LED:
                 - 1: Target Running LED ON: program execution in target started.
                 - 0: Target Running LED OFF: program execution in target stopped.
  * @retval None
  */
static inline void LED_RUNNING_OUT(uint32_t bit)
{
    ;             // Not available
}


///@}


//**************************************************************************************************
/** 
\defgroup DAP_Config_Initialization_gr CMSIS-DAP Initialization
\ingroup DAP_ConfigIO_gr
@{

CMSIS-DAP Hardware I/O and LED Pins are initialized with the function \ref DAP_SETUP.
*/

/** Setup of the Debug Unit I/O pins and LEDs (called when Debug Unit is initialized).
This function performs the initialization of the CMSIS-DAP Hardware I/O Pins and the
Status LEDs. In detail the operation of Hardware I/O and LED pins are enabled and set:
 - I/O clock system enabled.
 - all I/O pins: input buffer enabled, output pins are set to HighZ mode.
 - for nTRST, nRESET a weak pull-up (if available) is enabled.
 - LED output pins are enabled and LEDs are turned off.
*/
static __inline void DAP_SETUP(void)
{
    RCU_APB2EN |= PIN_nRESET_RCU;

    gpio_initial(PIN_nRESET_PORT, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, PIN_nRESET);
}

/**
  * @brief Reset Target Device with custom specific I/O pin or command sequence.
  * This function allows the optional implementation of a device specific reset sequence.
  * It is called when the command \ref DAP_ResetTarget and is for example required 
  * when a device needs a time-critical unlock sequence that enables the debug port.
  * @retval 0 = no device specific reset sequence is implemented.
            1 = a device specific reset sequence is implemented.
  */
__STATIC_INLINE uint8_t RESET_TARGET(void)
{
    return (0); // change to '1' when a device reset sequence is implemented
}

///@}


#endif /* __DAP_CONFIG_H__ */
