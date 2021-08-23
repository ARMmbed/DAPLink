/**
 * @file    main_interface.c
 * @brief   Entry point for interface program logic
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2020 Arm Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include <stdio.h>
#include "cmsis_os2.h"
#include "rl_usb.h"
#include "main_interface.h"
#include "gpio.h"
#include "uart.h"
#include "tasks.h"
#include "swd_host.h"
#include "info.h"
#include "settings.h"
#include "daplink.h"
#include "util.h"
#include "DAP.h"
#include "bootloader.h"
#include "cortex_m.h"
#include "sdk.h"
#include "target_family.h"
#include "target_board.h"

#include "stm32wbxx_hal.h"

#ifdef DRAG_N_DROP_SUPPORT
#include "vfs_manager.h"
#include "flash_intf.h"
#include "flash_manager.h"
#endif

#ifndef USE_LEGACY_CMSIS_RTOS
#include "rtx_os.h"
#endif

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
#ifndef __MICROLIB
/* Avoids early implicit call to osKernelInitialize() */
void _platform_post_stackheap_init (void) {}
#endif
#if (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
/* Avoids the semihosting issue */
__asm("  .global __ARM_use_no_argv\n");
#endif
#elif defined(__GNUC__)
/* Avoids early implicit call to osKernelInitialize() */
void software_init_hook (void) {}
/* Disables part of C/C++ runtime startup/teardown */
void __libc_init_array (void) {}
#endif

// Event flags for main task
// Timers events
#define FLAGS_MAIN_90MS         (1 << 0)
#define FLAGS_MAIN_30MS         (1 << 1)
// Reset events
#define FLAGS_MAIN_RESET        (1 << 2)
// Other Events
#define FLAGS_MAIN_POWERDOWN    (1 << 4)
#define FLAGS_MAIN_DISABLEDEBUG (1 << 5)
#define FLAGS_MAIN_PROC_USB     (1 << 9)
// Used by cdc when an event occurs
#define FLAGS_MAIN_CDC_EVENT    (1 << 11)
// Used by msd when flashing a new binary
#define FLAGS_LED_BLINK_30MS    (1 << 6)

// Timing constants (in 90mS ticks)
// USB busy time (~3 sec)
#define USB_BUSY_TIME           (33)
// Delay before a USB device connect may occur (~1 sec)
#define USB_CONNECT_DELAY       (11)
// Timeout for USB being configured (~2 sec)
#define USB_CONFIGURE_TIMEOUT   (22)
// Delay before target may be taken out of reset or reprogrammed after startup
#define STARTUP_DELAY           (1)

// Decrement to zero
#define DECZERO(x)              (x ? --x : 0)

//default hid led settings
#ifndef HID_LED_DEF
#define HID_LED_DEF GPIO_LED_OFF
#endif

//default cdc led settings
#ifndef CDC_LED_DEF
#define CDC_LED_DEF GPIO_LED_OFF
#endif

//default msc led settings
#ifndef MSC_LED_DEF
#define MSC_LED_DEF GPIO_LED_OFF
#endif

ADC_HandleTypeDef hadc1;
I2C_HandleTypeDef hi2c3;
QSPI_HandleTypeDef hqspi;
SPI_HandleTypeDef hspi2;
USART_HandleTypeDef husart1;

uint8_t Rx_Buf[8];
uint8_t Receive_Buf[8];
uint8_t Tx_Buf[1];
uint8_t Tx_Buf1[1], Tx_Buf2[1], Tx_Buf3[1], Tx_Buf4[1], Tx_Buf5[1], Tx_Buf6[1], Tx_Buf7[1];
int command, protocol, buf;
int prot = 0;
int buf1, buf2, buf3, buf4, buf5, buf6, buf7;
int p=0;
int k, l, m =0;

int b[1] = {9};
int a = 0;
int n = 0;
int d,e = 0;

/* for SPI data transfer */
uint8_t spiTxBuf[2], spiRxBuf[2];
/* for USART data transfer */
uint8_t message[4];
/*for ADC data transfer */
uint16_t raw;
/* for I2C data transfer */
static const uint8_t ADDR = 0x48 << 1; // Use 8-bit address
static const uint8_t REG_TEMP = 0x00;
uint8_t i2c_buf[12];
uint8_t ret;
/* for QSPI data transfer */
uint8_t qspi_data;

void protocol_bridging(void);
static void MX_ADC1_Init(void);
static void MX_I2C3_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_USART1_Init(void);
static void MX_SPI2_Init(void);
static void SPI_Tx_Rx(void);
static void I2C_Tx_Rx(void);
static void USART_Tx_Rx(void);
static void ADC_Tx_Rx(void);
static void QSPI_Tx_Rx(void);

// Reference to our main task
osThreadId_t main_task_id;
#ifndef USE_LEGACY_CMSIS_RTOS
static uint32_t s_main_thread_cb[WORDS(sizeof(osRtxThread_t))];
static uint64_t s_main_task_stack[MAIN_TASK_STACK / sizeof(uint64_t)];
static const osThreadAttr_t k_main_thread_attr = {
        .name = "main",
        .cb_mem = s_main_thread_cb,
        .cb_size = sizeof(s_main_thread_cb),
        .stack_mem = s_main_task_stack,
        .stack_size = sizeof(s_main_task_stack),
        .priority = MAIN_TASK_PRIORITY,
    };

static uint32_t s_timer_30ms_cb[WORDS(sizeof(osRtxTimer_t))];
static const osTimerAttr_t k_timer_30ms_attr = {
        .name = "30ms",
        .cb_mem = s_timer_30ms_cb,
        .cb_size = sizeof(s_timer_30ms_cb),
    };
#endif

// USB busy LED state; when TRUE the LED will flash once using 30mS clock tick
static uint8_t hid_led_usb_activity = 0;
static uint8_t cdc_led_usb_activity = 0;
static uint8_t msc_led_usb_activity = 0;
static main_led_state_t hid_led_state = MAIN_LED_FLASH;
static main_led_state_t cdc_led_state = MAIN_LED_FLASH;
static main_led_state_t msc_led_state = MAIN_LED_FLASH;

// Global state of usb
main_usb_connect_t usb_state;
static bool usb_test_mode = false;

// Timer task, set flags every 30mS and 90mS
void timer_task_30mS(void * arg)
{
    static uint32_t i = 0;
    osThreadFlagsSet(main_task_id, FLAGS_MAIN_30MS);
    if (!(i++ % 3)) {
        osThreadFlagsSet(main_task_id, FLAGS_MAIN_90MS);
    }
}

// Functions called from other tasks to trigger events in the main task
// parameter should be reset type??
void main_reset_target(uint8_t send_unique_id)
{
    osThreadFlagsSet(main_task_id, FLAGS_MAIN_RESET);
    return;
}

// Flash HID LED using 30mS tick
void main_blink_hid_led(main_led_state_t state)
{
    hid_led_usb_activity = 1;
    hid_led_state = state;
    return;
}

// Flash CDC LED using 30mS tick
void main_blink_cdc_led(main_led_state_t state)
{
    cdc_led_usb_activity = 1;
    cdc_led_state = state;
    return;
}

// Flash MSC LED using 30mS tick
void main_blink_msc_led(main_led_state_t state)
{
    msc_led_usb_activity = 1;
    msc_led_state = state;
    return;
}

// Power down the interface
void main_powerdown_event(void)
{
    osThreadFlagsSet(main_task_id, FLAGS_MAIN_POWERDOWN);
    return;
}

// Disable debug on target
void main_disable_debug_event(void)
{
    osThreadFlagsSet(main_task_id, FLAGS_MAIN_DISABLEDEBUG);
    return;
}

// Start CDC processing
void main_cdc_send_event(void)
{
    osThreadFlagsSet(main_task_id, FLAGS_MAIN_CDC_EVENT);
    return;
}

void main_usb_set_test_mode(bool enabled)
{
    usb_test_mode = enabled;
}

void USBD_SignalHandler()
{
    osThreadFlagsSet(main_task_id, FLAGS_MAIN_PROC_USB);
}

extern void cdc_process_event(void);

void main_task(void * arg)
{
    // State processing
    uint16_t flags = 0;
    // LED
    gpio_led_state_t hid_led_value = HID_LED_DEF;
    gpio_led_state_t cdc_led_value = CDC_LED_DEF;
    gpio_led_state_t msc_led_value = MSC_LED_DEF;		
    // USB
    uint32_t usb_state_count = USB_BUSY_TIME;
    uint32_t usb_no_config_count = USB_CONFIGURE_TIMEOUT;
    // button state
    uint8_t reset_pressed = 0;
#ifdef PBON_BUTTON
    uint8_t power_on = 1;
#endif

    // Initialize settings - required for asserts to work
	
    config_init();

#ifdef USE_LEGACY_CMSIS_RTOS
    // Get a reference to this task
    main_task_id = osThreadGetId();
#endif
    
	// leds
    gpio_init(); 
		
	// Turn to LED default settings	
    //gpio_set_hid_led(hid_led_value);
   	//gpio_set_cdc_led(cdc_led_value);
    //gpio_set_msc_led(msc_led_value);

	
	// Initialize the DAP
    DAP_Setup();

    // make sure we have a valid board info structure.
    util_assert(g_board_info.info_version == kBoardInfoVersion);

    // do some init with the target before USB and files are configured
    if (g_board_info.prerun_board_config) {
        g_board_info.prerun_board_config();
    }

    //initialize the family
    init_family();

    if (g_target_family && g_target_family->prerun_target_config) {
        g_target_family->prerun_target_config();
    }

    //setup some flags
    if (g_board_info.flags & kEnableUnderResetConnect) {
        swd_set_reset_connect(CONNECT_UNDER_RESET);
    }
    if (g_board_info.flags & kEnablePageErase) {
#ifdef DRAG_N_DROP_SUPPORT
        flash_manager_set_page_erase(true);
#endif
    }
	
    // Update versions and IDs
	info_init();
    // Update bootloader if it is out of date
	
	bootloader_check_and_update();
    // USB
	usbd_init();
    
#ifdef DRAG_N_DROP_SUPPORT
    vfs_mngr_fs_enable((config_ram_get_disable_msd()==0));
#endif
    usbd_connect(0);
    usb_state = USB_CONNECTING;
    usb_state_count = USB_CONNECT_DELAY;

    // Start timer tasks
#ifndef USE_LEGACY_CMSIS_RTOS
    osTimerId_t tmr_id = osTimerNew(timer_task_30mS, osTimerPeriodic, NULL, &k_timer_30ms_attr);
#else
    osTimerId_t tmr_id = osTimerNew(timer_task_30mS, osTimerPeriodic, NULL, NULL);
#endif
    osTimerStart(tmr_id, 3);
	
	for(int i = 0; i < 8; i++)
	{
		Receive_Buf[i] = 0;
	}
	
    while (1) {
        flags = osThreadFlagsWait(FLAGS_MAIN_RESET             // Put target in reset state
                       | FLAGS_MAIN_90MS            // 90mS tick
                       | FLAGS_MAIN_30MS            // 30mS tick
                       | FLAGS_MAIN_POWERDOWN       // Power down interface
                       | FLAGS_MAIN_DISABLEDEBUG    // Disable target debug
                       | FLAGS_MAIN_PROC_USB        // process usb events
                       | FLAGS_MAIN_CDC_EVENT       // cdc event
                       , osFlagsWaitAny
                       , osWaitForever);

        if (flags & FLAGS_MAIN_PROC_USB) {
            if (usb_test_mode) {
                // When in USB test mode Insert a delay to
                // simulate worst-case behavior.
                osDelay(1);
            }
            USBD_Handler();
        }

        if (flags & FLAGS_MAIN_RESET) {
            target_set_state(RESET_RUN);
        }

        if (flags & FLAGS_MAIN_POWERDOWN) {
            // Disable debug
            target_set_state(NO_DEBUG);
            // Disable board power before USB is disconnected.
            gpio_set_board_power(false);
            // Disconnect USB
            usbd_connect(0);
            // Turn off LED
            gpio_set_hid_led(GPIO_LED_OFF);
            gpio_set_cdc_led(GPIO_LED_OFF);
            gpio_set_msc_led(GPIO_LED_OFF);

            // TODO: put the interface chip in sleep mode
            while (1);
        }

        if (flags & FLAGS_MAIN_DISABLEDEBUG) {
            // Disable debug
            target_set_state(NO_DEBUG);
        }

        if (flags & FLAGS_MAIN_CDC_EVENT) {
            cdc_process_event();
        }

        if (flags & FLAGS_MAIN_90MS) {
            // Update USB busy status
#ifdef DRAG_N_DROP_SUPPORT
            vfs_mngr_periodic(90); // FLAGS_MAIN_90MS
#endif

            // Update USB connect status
            switch (usb_state) {
                case USB_DISCONNECTING:
                    usb_state = USB_DISCONNECTED;
                    // Disable board power before USB is disconnected.
                    gpio_set_board_power(false);
                    usbd_connect(0);
                    break;

                case USB_CONNECTING:

                    // Wait before connecting
                    if (DECZERO(usb_state_count) == 0) {
                        usbd_connect(1);
                        usb_state = USB_CHECK_CONNECTED;
                        // Reset connect timeout
                        usb_no_config_count = USB_CONFIGURE_TIMEOUT;
                    }

                    break;

                case USB_CHECK_CONNECTED:
                    if (usbd_configured()) {
                        // Let the HIC enable power to the target now that high power has been negotiated.
                        gpio_set_board_power(true);

                        usb_state = USB_CONNECTED;
                    }
                    else if (DECZERO(usb_no_config_count) == 0) {
                        // USB configuration timed out, which most likely indicates that the HIC is
                        // powered by a USB wall wart or similar power source. Go ahead and enable
                        // board power.
                        gpio_set_board_power(true);
                    }
		
                    break;

                case USB_CONNECTED: protocol_bridging();
				
					break;
					
                case USB_DISCONNECTED:
                default:
                    break;
            }
        }

        // 30mS tick used for flashing LED when USB is busy
        if (flags & FLAGS_MAIN_30MS) {

            // handle reset button without eventing
            if (!reset_pressed && gpio_get_reset_btn_fwrd()) {
#ifdef DRAG_N_DROP_SUPPORT
               if (!flash_intf_target->flash_busy()) //added checking if flashing on target is in progress
#endif
                {
                    // Reset button pressed
                    target_set_state(RESET_HOLD);
                    reset_pressed = 1;
                }
            } else if (reset_pressed && !gpio_get_reset_btn_fwrd()) {
                // Reset button released
                target_set_state(RESET_RUN);
                reset_pressed = 0;
            }

#ifdef PBON_BUTTON
            // handle PBON pressed
            if(gpio_get_pbon_btn())
            {
                if(power_on)
                {
                    // Loop till PBON is pressed
                    while (gpio_get_pbon_btn()) {;}
                    // Power button released when target was running
                    target_set_state(SHUTDOWN);
                    power_on = 0;
                }
                else
                {
                    // Loop till PBON is pressed
                    while (gpio_get_pbon_btn()) {;}
                    // Power button released when target was already powered off
                    target_set_state(POWER_ON);
                    power_on = 1;
                }
            }
#endif

            // DAP LED
            if (hid_led_usb_activity) {

                if ((hid_led_state == MAIN_LED_FLASH) || (hid_led_state == MAIN_LED_FLASH_PERMANENT)) {
                    // Toggle LED value
                    hid_led_value = GPIO_LED_ON == hid_led_value ? GPIO_LED_OFF : GPIO_LED_ON;

                    // If in flash mode stop after one cycle
                    if ((HID_LED_DEF == hid_led_value) && (MAIN_LED_FLASH == hid_led_state)) {
                        hid_led_usb_activity = 0;
                        hid_led_state = MAIN_LED_DEF;
                    }
                } else {
                    //LED next state is MAIN_LED_DEF
                    hid_led_value = HID_LED_DEF;
                    hid_led_usb_activity = 0;
                }

                // Update hardware
                gpio_set_hid_led(hid_led_value);
            }

            // MSD LED
            if (msc_led_usb_activity) {

                if ((msc_led_state == MAIN_LED_FLASH) || (msc_led_state == MAIN_LED_FLASH_PERMANENT)) {
                    // Toggle LED value
                    msc_led_value = GPIO_LED_ON == msc_led_value ? GPIO_LED_OFF : GPIO_LED_ON;

                    // If in flash mode stop after one cycle
                    if ((MSC_LED_DEF == msc_led_value) && (MAIN_LED_FLASH == msc_led_state)) {
                        msc_led_usb_activity = 0;
                        msc_led_state = MAIN_LED_DEF;
                    }
                } else {
                    //LED next state is MAIN_LED_DEF
                    msc_led_value = MSC_LED_DEF;
                    msc_led_usb_activity = 0;
                }

                // Update hardware
                gpio_set_msc_led(msc_led_value);
            }

            // CDC LED
            if (cdc_led_usb_activity) {

                if ((cdc_led_state == MAIN_LED_FLASH) || (cdc_led_state == MAIN_LED_FLASH_PERMANENT)){
                    // Toggle LED value
                    cdc_led_value = GPIO_LED_ON == cdc_led_value ? GPIO_LED_OFF : GPIO_LED_ON;

                    // If in flash mode stop after one cycle
                    if ((CDC_LED_DEF == cdc_led_value) && (MAIN_LED_FLASH == cdc_led_state)) {
                        cdc_led_usb_activity = 0;
                        cdc_led_state = MAIN_LED_DEF;
                    }
                }else{
                    //LED next state is MAIN_LED_DEF
                    cdc_led_value = CDC_LED_DEF;
                    cdc_led_usb_activity = 0;
                }

                // Update hardware
                gpio_set_cdc_led(cdc_led_value);
            }
        }
    }
}


void protocol_bridging()
{
	USBD_CDC_ACM_DataRead((uint8_t *)b, 1);
	
	if(b[0] != 9)
	{	
		Receive_Buf[e] = b[0];
		e++;
		b[0] = 9;
		if(e == 8)
		{
			USBD_CDC_ACM_DataSend((uint8_t *)Receive_Buf, 8);
			e = 0;	
			a = 1;
		}
	}
	
	if(a == 1)
	{
		/** ASCII to Decimal conversion **/
		for(int i=0; i<8; i++)
		{
			Rx_Buf[i] = Receive_Buf[i] - 48U;
		}
		
		protocol = Rx_Buf[0];
		
		/** checking protocol **/

		if(protocol == 0)
		{
			// SPI Protocol
			
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);  //green led off
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);  //red led on
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);  //blue led off  
			HAL_Delay(1000);
			
			buf1 = Rx_Buf[1];   /*mode*/
			buf2 = Rx_Buf[2];   /*Baudrate prescaler*/
			buf3 = Rx_Buf[3];   /*Data Size*/
			buf4 = Rx_Buf[4];   /*CRC Check*/
			buf5 = Rx_Buf[5];   /*CRC Length*/
			buf6 = Rx_Buf[6];   /*TI mode*/
			buf7 = Rx_Buf[7];   /*First Bit*/
		
			/** Decimal to ASCII conversion **/
			Tx_Buf[0] = protocol + 48U;
			Tx_Buf1[0] = buf1 + 48U;
			Tx_Buf2[0] = buf2 + 48U;
			Tx_Buf3[0] = buf3 + 48U;
			Tx_Buf4[0] = buf4 + 48U;
			Tx_Buf5[0] = buf5 + 48U;
			Tx_Buf6[0] = buf6 + 48U;
			Tx_Buf7[0] = buf7 + 48U;
			
			HAL_Delay(100);
			MX_SPI2_Init();
			SPI_Tx_Rx();
			
			buf1 = 0;
			buf2 = 0;
			buf3 = 0;
			buf4 = 0;
			buf5 = 0;
			buf6 = 0;
			buf7 = 0;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);  //red led off
		} 
		else if (protocol == 1)
		{
			// I2C Protocol

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);  //green led off
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   //red led off
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);  //blue led on
			HAL_Delay(1000);
			
			buf1 = Rx_Buf[1];   /*Addressing Mode*/
			buf3 = Rx_Buf[2];   /*No Stretch Mode*/
			buf3 = 0;
			buf4 = 0;
			buf5 = 0;
			buf6 = 0;
			buf7 = 0;
			
			/** Decimal to ASCII conversion **/
			Tx_Buf1[0] = buf1 + 48U;
			Tx_Buf2[0] = buf2 + 48U;
			Tx_Buf2[0] = buf2 + 48U;
			Tx_Buf3[0] = buf3 + 48U;
			Tx_Buf4[0] = buf4 + 48U;
			Tx_Buf5[0] = buf5 + 48U;
			Tx_Buf6[0] = buf6 + 48U;
			Tx_Buf7[0] = buf7 + 48U;
			
			HAL_Delay(100);
			MX_I2C3_Init();
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);  //blue led off
			I2C_Tx_Rx();
			buf1 = 0;
			buf2 = 0;
			buf3 = 0;
			buf4 = 0;
			buf5 = 0;
			buf6 = 0;
			buf7 = 0;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   //red led off
			
		}
		else if(protocol == 2)
		{
			// USART Protocol

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);  //green led on
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   //red led off
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);  //blue led on
			HAL_Delay(1000);
			buf1 = Rx_Buf[1];   /*BaudRate*/
			buf2 = Rx_Buf[2];   /*WordLength*/
			buf3 = Rx_Buf[3];   /*StopBits*/
			buf4 = Rx_Buf[4];   /*Parity*/
			buf5 = Rx_Buf[5];   /*Mode*/
			
			/** Decimal to ASCII conversion **/
			Tx_Buf1[0] = buf1 + 48U;
			Tx_Buf2[0] = buf2 + 48U;
			Tx_Buf3[0] = buf3 + 48U;
			Tx_Buf4[0] = buf4 + 48U;
			Tx_Buf5[0] = buf5 + 48U;

			HAL_Delay(100);
			MX_USART1_Init();
			USART_Tx_Rx();
			buf1 = 0;
			buf2 = 0;
			buf3 = 0;
			buf4 = 0;
			buf5 = 0;
			buf6 = 0;
			buf7 = 0;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);  //green led off
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);  //blue led off
		}
		else if(protocol == 3)
		{
			// ADC Protocol

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);  //green led off
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);   //red led on
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);  //blue led on
			HAL_Delay(1000);
			buf1 = Rx_Buf[1];   /*ClockPrescaler*/
			buf2 = Rx_Buf[2];   /*Channel*/
			buf3 = Rx_Buf[3];   /*Sampling Time*/
			buf4 = Rx_Buf[4];   /*Data Align*/
			buf5 = Rx_Buf[5];   /*Overrun*/

			/** Decimal to ASCII conversion **/
			Tx_Buf1[0] = buf1 + 48U;
			Tx_Buf2[0] = buf2 + 48U;
			Tx_Buf3[0] = buf3 + 48U;
			Tx_Buf4[0] = buf4 + 48U;
			Tx_Buf5[0] = buf5 + 48U;
			Tx_Buf6[0] = buf6 + 48U;

			HAL_Delay(100);
			MX_ADC1_Init();
			ADC_Tx_Rx();
			buf1 = 0;
			buf2 = 0;
			buf3 = 0;
			buf4 = 0;
			buf5 = 0;
			buf6 = 0;
			buf7 = 0;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   //red led off
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);  //blue led off
		}
		else if(protocol == 4)
		{
			// QSPI Protocol

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);  //green led on
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);   //red led on
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);  //blue led off
			HAL_Delay(1000);
			buf1 = Rx_Buf[1];   /*Sample Shifting*/
			buf2 = Rx_Buf[2];   /*ChipSelectHighTime*/
			buf3 = Rx_Buf[3];   /*Clock Mode*/

			/** Decimal to ASCII conversion **/
			Tx_Buf1[0] = buf1 + 48U;
			Tx_Buf2[0] = buf2 + 48U;
			Tx_Buf3[0] = buf3 + 48U;

			HAL_Delay(100);
			MX_QUADSPI_Init();
			QSPI_Tx_Rx();
			buf1 = 0;
			buf2 = 0;
			buf3 = 0;
			buf4 = 0;
			buf5 = 0;
			buf6 = 0;
			buf7 = 0;
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);  //green led off
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); //red led off
		}
		a = 0;
	}
}
  
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 32;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    //Error_Handler();
  }
  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    //Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.PLLSAI1.PLLN = 24;
  PeriphClkInitStruct.PLLSAI1.PLLP = RCC_PLLP_DIV2;
  PeriphClkInitStruct.PLLSAI1.PLLQ = RCC_PLLQ_DIV2;
  PeriphClkInitStruct.PLLSAI1.PLLR = RCC_PLLR_DIV2;
  PeriphClkInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_USBCLK;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSI;
  PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    //Error_Handler();
  }
  /* USER CODE BEGIN Smps */

  /* USER CODE END Smps */
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}
//end biby

static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

	/*** mode selection ***/
	if (buf1 == 0)
	{
		hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
		hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	}
	else if (buf1 == 1)
	{
		hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
		hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
	}
	else if (buf1 == 2)
	{
		hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
		hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	}
	else if (buf1 == 3)
	{
		hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
		hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
	}
	else
	{
		hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
		hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	}

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */
  
	/*** baudrate prescaler selection ***/
	if (buf2 == 0)
	{
		hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	}
	else if (buf2 == 1)
	{
		hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	}
	else if (buf2 == 2)
	{
		hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	}
	else if (buf2 == 3)
	{
		hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	}
	else if (buf2 == 4)
	{
		hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	}
	else if (buf2 == 5)
	{
		hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	}
	else if (buf2 == 6)
	{
		hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	}
	else if (buf2 == 7)
	{
		hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	}
	else
	{
		hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	}

	/*** datasize selection ***/
	if (buf3 == 0)
	{
		hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	}
	else if (buf3  == 1)
	{
		hspi2.Init.DataSize = SPI_DATASIZE_16BIT;
	}
	else
	{
		hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	}

	/*** CRC enable/disable ***/
	if (buf4 == 0)
	{
		hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	}
	else if (buf4 == 1)
	{
		hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_ENABLE;
	}
	else
	{
		hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	}

	/*** CRCLength ***/
	if (buf5 == 0)
	{
		hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	}
	else if (buf5 == 1)
	{
		hspi2.Init.CRCLength = SPI_CRC_LENGTH_8BIT;
	}
	else if (buf5 == 2)
	{
		hspi2.Init.CRCLength = SPI_CRC_LENGTH_16BIT;
	}
	else
	{
		hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	}

	/*** TI Mode ***/
	if (buf6 == 0)
	{
		hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	}
	else if (buf6 == 1)
	{
		hspi2.Init.TIMode = SPI_TIMODE_ENABLE;
	}
	else
	{
		hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	}

	/** First Bit ***/
	if (buf7 == 0)
	{
		hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	}
	else if (buf7 == 1)
	{
		hspi2.Init.FirstBit = SPI_FIRSTBIT_LSB;
	}
	else
	{
		hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	}

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.NSS = SPI_NSS_HARD_INPUT;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

static void MX_I2C3_Init(void)
{

	/* USER CODE BEGIN I2C1_Init 0 */

	/*** AddressingMode ***/
	if (buf1 == 0)
	{
		hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	}
	else if (buf1 == 1)
	{
		hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_10BIT;
	}
	else
	{
		hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	}

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/*** No Stretch Mode ***/
	if (buf2 == 0)
	{
		hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	}
	else if (buf2 == 1)
	{
		hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;
	}
	else
	{
		hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	}

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x10707DBC;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

static void MX_USART1_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */
	/*** Baud Rate ***/
	if (buf1 == 0)
	{
		husart1.Init.BaudRate = 38400;
	}
	else if (buf1 == 1)
	{
		husart1.Init.BaudRate = 9600;
	}
	else if (buf1 == 2)
	{
		husart1.Init.BaudRate = 115200;
	}
	else
	{
		husart1.Init.BaudRate = 115200;
	}

	/*** WordLength ***/
	if (buf2 == 0)
	{
		husart1.Init.WordLength = USART_WORDLENGTH_8B;
	}
	else if (buf2 == 1)
	{
		husart1.Init.WordLength = USART_WORDLENGTH_9B;
	}
	else
	{
		husart1.Init.WordLength = USART_WORDLENGTH_8B;
	}

	/*** StopBits ***/
	if (buf3 == 0)
	{
		husart1.Init.StopBits = USART_STOPBITS_0_5;
	}
	else if (buf3 == 1)
	{
		husart1.Init.StopBits = USART_STOPBITS_1;
	}
	else if (buf3 == 2)
	{
		husart1.Init.StopBits = USART_STOPBITS_1_5;
	}
	else if (buf3 == 3)
	{
		husart1.Init.StopBits = USART_STOPBITS_2;
	}
	else
	{
		husart1.Init.StopBits = USART_STOPBITS_1;
	}

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */
	/*** Parity ***/
	if (buf4 == 0)
	{
		husart1.Init.Parity = USART_PARITY_NONE;
	}
	else if (buf4 == 1)
	{
		husart1.Init.Parity = USART_PARITY_EVEN;
	}
	else if (buf4 == 2)
	{
		husart1.Init.Parity = USART_PARITY_ODD;
	}
	else
	{
		husart1.Init.Parity = USART_PARITY_NONE;
	}

	/*** Mode ***/
	if (buf5 == 0)
	{
		husart1.Init.Mode = USART_MODE_RX;
	}
	else if (buf5 == 1)
	{
		husart1.Init.Mode = USART_MODE_TX;
	}
	else if (buf5 == 2)
	{
		husart1.Init.Mode = USART_MODE_TX_RX;
	}
	else
	{
		husart1.Init.Mode = USART_MODE_TX_RX;
	}

  /* USER CODE END USART1_Init 1 */
  husart1.Instance = USART1;
  husart1.Init.CLKPolarity = USART_POLARITY_LOW;
  husart1.Init.CLKPhase = USART_PHASE_1EDGE;
  husart1.Init.CLKLastBit = USART_LASTBIT_DISABLE;
  husart1.Init.ClockPrescaler = USART_PRESCALER_DIV1;
  husart1.SlaveMode = USART_SLAVEMODE_DISABLE;
  if (HAL_USART_Init(&husart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_USARTEx_SetTxFifoThreshold(&husart1, USART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_USARTEx_SetRxFifoThreshold(&husart1, USART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_USARTEx_DisableFifoMode(&husart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

static void MX_ADC1_Init(void)
{
  /* USER CODE BEGIN ADC1_Init 0 */
  
    ADC_ChannelConfTypeDef sConfig = {0};
	
	/*** Clock Prescaler ***/
	if (buf1 == 0)
	{
		hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
	}
	else if (buf1 == 1)
	{
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	}
	else if (buf1 == 2)
	{
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;
	}
	else if (buf1 == 3)
	{
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
	}
	else if (buf1 == 4)
	{
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV8;
	}
	else if (buf1 == 5)
	{
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV16;
	}
	else if (buf1 == 6)
	{
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV32;
	}
	else if (buf1 == 7)
	{
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV64;
	}
	else if (buf1 == 8)
	{
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV128;
	}
	else if (buf1 == 9)
	{
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV256;
	}
	else
	{
		hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	}

	/*** Channel ***/
	if (buf2 == 0)
	{
		sConfig.Channel = ADC_CHANNEL_0;
	}
	else if (buf2 == 1)
	{
		sConfig.Channel = ADC_CHANNEL_1;
	}
	else if (buf2 == 2)
	{
		sConfig.Channel = ADC_CHANNEL_2;
	}
	else if (buf2 == 3)
	{
		sConfig.Channel = ADC_CHANNEL_3;
	}
	else if (buf2 == 4)
	{
		sConfig.Channel = ADC_CHANNEL_4;
	}
	else
	{
		sConfig.Channel = ADC_CHANNEL_4;
	}
	
	/*** Sampling Time ***/
	if (buf3 == 0)
	{
		sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	}
	else if (buf3 == 1)
	{
		sConfig.SamplingTime = ADC_SAMPLETIME_6CYCLES_5;
	}
	else if (buf3 == 2)
	{
		sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;
	}
	else if (buf3 == 3)
	{
		sConfig.SamplingTime = ADC_SAMPLETIME_24CYCLES_5;
	}
	else if (buf3 == 4)
	{
		sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;
	}
	else if (buf3 == 5)
	{
		sConfig.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
	}
	else if (buf3 == 6)
	{
		sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
	}
	else if (buf3 == 7)
	{
		sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	}
	else
	{
		sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	}
  /* USER CODE END ADC1_Init 0 */

  /* USER CODE BEGIN ADC1_Init 1 */
 	/*** Data Align ***/
	if (buf4 == 0)
	{
		hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	}
	else if (buf4 == 1)
	{
		hadc1.Init.DataAlign = ADC_DATAALIGN_LEFT;
	}
	else
	{
		hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	}

	/*** Over run ***/
	if (buf5 == 0)
	{
		hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	}
	else if (buf5 == 1)
	{
		hadc1.Init.Overrun =  ADC_OVR_DATA_OVERWRITTEN;
	}
	else
	{
		hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	}

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */
  /*** Sample Shifting ***/
	if (buf1 == 0)
	{
		hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
	}
	else if (buf1 == 1)
	{
		hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
	}
	else
	{
		hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
	}

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */
  /*** Chip Select High Time ***/
	if (buf2 == 0)
	{
		hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
	}
	else if (buf2 == 1)
	{
		hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_2_CYCLE;
	}
	else if (buf2 == 2)
	{
		hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_3_CYCLE;
	}
	else if (buf2 == 3)
	{
		hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_4_CYCLE;
	}
	else if (buf2 == 4)
	{
		hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE;
	}
	else if (buf2 == 5)
	{
		hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_6_CYCLE;
	}
	else if (buf2 == 6)
	{
		hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_7_CYCLE;
	}
	else if (buf2 == 7)
	{
		hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_8_CYCLE;
	}
	else
	{
		hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
	}

	/*** Clock Mode ***/
	if (buf3 == 0)
	{
		hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
	}
	else if (buf3 == 1)
	{
		hqspi.Init.ClockMode = QSPI_CLOCK_MODE_3;
	}
	else
	{
		hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
	}

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 255;
  hqspi.Init.FifoThreshold = 1;
  hqspi.Init.FlashSize = 1;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

static void SPI_Tx_Rx(void)
{
	// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
	// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
	
	/*** SPI Transmit ***/
	// 1. Bring slave select low
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	// 2. Transmit register + data
	spiTxBuf[0] = 0x20;
	spiTxBuf[1] = 0x11;
	HAL_SPI_Transmit(&hspi2, spiTxBuf, 2, 50);
	// 3. Bring slave select high
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);

	/*** SPI Receive ***/
	// 1. Bring slave select low
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
	// 2. Transmit register + 0x80 (to set MSB bit to high, read mode)
	spiTxBuf[0] = 0x20|0x80;
	HAL_SPI_Transmit(&hspi2, spiTxBuf, 1, 50);
	// 3. Receive data
	HAL_SPI_Receive(&hspi2, spiTxBuf, 1, 50);
	// 4. Bring slave select high
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	// end of SPI
}

static void I2C_Tx_Rx(void)
{
	// i2c_buf[0] = REG_TEMP;
	// ret = HAL_I2C_Master_Transmit(&hi2c3, ADDR, i2c_buf, 1, 200);
	// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);  //green led on
	// HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);   //red led off
	// HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);  //blue led off
	// ret = HAL_I2C_Master_Receive(&hi2c3, ADDR, i2c_buf, 2, 200);
	// HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);   //green led off
	// HAL_Delay(1000);
}

static void ADC_Tx_Rx(void)
{
	// HAL_ADC_Start(&hadc1);
	// HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	// raw = HAL_ADC_GetValue(&hadc1);
}

static void USART_Tx_Rx(void)
{
	// HAL_USART_Receive(&husart1, message, sizeof(message), HAL_MAX_DELAY);
	// HAL_USART_Transmit(&husart1, message, sizeof(message), HAL_MAX_DELAY);
}

static void QSPI_Tx_Rx(void)
{
	// HAL_QSPI_Receive(&hqspi, qspi_data, HAL_MAX_DELAY);
	// HAL_QSPI_Transmit(&hqspi, qspi_data, HAL_MAX_DELAY);
}

int main(void)
{
  
#if DAPLINK_ROM_BL_SIZE > 0
    SCB->VTOR = SCB_VTOR_TBLOFF_Msk & DAPLINK_ROM_IF_START;
#endif
    // initialize vendor sdk
	SystemClock_Config(); //biby
	//sdk_init();
	
	
   // Initialize CMSIS-RTOS
    osKernelInitialize();
    // Create application main thread
#ifndef USE_LEGACY_CMSIS_RTOS
    main_task_id = osThreadNew(main_task, NULL, &k_main_thread_attr);
#else
    osThreadNew(main_task, NULL, NULL);
#endif
    // Start thread execution
    osKernelStart();

    // Should never reach here!
    for (;;) {}
}
