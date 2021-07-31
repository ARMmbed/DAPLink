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
//#include "stm32wbxx.h"
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
    //gpio_init(); //biby
		
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

                case USB_CONNECTED: 
					
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

// start biby
HAL_StatusTypeDef Syam_HAL_Init(void)
{
  HAL_StatusTypeDef  status = HAL_OK;
  /* Configure Flash prefetch, Instruction cache, Data cache */
  /* Default configuration at reset is:                      */
  /* - Prefetch disabled                                     */
  /* - Instruction cache enabled                             */
  /* - Data cache enabled                                    */
#if (INSTRUCTION_CACHE_ENABLE == 0U)
   __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();
#endif /* INSTRUCTION_CACHE_ENABLE */

#if (DATA_CACHE_ENABLE == 0U)
   __HAL_FLASH_DATA_CACHE_DISABLE();
#endif /* DATA_CACHE_ENABLE */

#if (PREFETCH_ENABLE != 0U)
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
#endif /* PREFETCH_ENABLE */

  /* Set Interrupt Group Priority */
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  
  /* Use SysTick as time base source and configure 1ms tick (default clock after Reset is MSI) */
  if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Init the low level hardware */
    HAL_MspInit();
  }

  /* Return function status */
  return status;
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

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);
						  
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_15, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);   

  /*Configure GPIO pins : PC13 PC15 PC0 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC14 PC1 PC2 PC4
                           PC5 PC6 PC10 PC11
                           PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PH3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : PB8 PB9 PB10 PB11
                           PB14 PB15 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA4 PA5 PA13
                           PA14 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA1 PA2 PA3 PA6
                           PA7 PA8 PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB2 PB0 PB1 PB12
                           PB13 PB3 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_12
                          |GPIO_PIN_13|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PE4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

// void MX_USB_Device_Init(void)
// {
  // /* USER CODE BEGIN USB_Device_Init_PreTreatment */

  // /* USER CODE END USB_Device_Init_PreTreatment */

  // /* Init Device Library, add supported class and start the library. */
  // if (USBD_Init(&hUsbDeviceFS, &CDC_Desc, DEVICE_FS) != USBD_OK) {
    // //Error_Handler();
  // }
  // if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK) {
    // //Error_Handler();
  // }
  // if (USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS) != USBD_OK) {
    // //Error_Handler();
  // }
  // if (USBD_Start(&hUsbDeviceFS) != USBD_OK) {
    // //Error_Handler();
  // }
  // /* USER CODE BEGIN USB_Device_Init_PostTreatment */

  // /* USER CODE END USB_Device_Init_PostTreatment */
// }



void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}
//end biby

int main(void)
{
  
#if DAPLINK_ROM_BL_SIZE > 0
    SCB->VTOR = SCB_VTOR_TBLOFF_Msk & DAPLINK_ROM_IF_START;
#endif
    // initialize vendor sdk
	
	Syam_HAL_Init(); //biby
	SystemClock_Config(); //biby
	gpio_init(); //biby
	
	//HAL_NVIC_SetPriority(USB_LP_IRQn, 0, 0);
    // HAL_NVIC_EnableIRQ(USB_LP_IRQn);
	// GPIO_InitTypeDef GPIO_InitStructure;
	// __HAL_RCC_GPIOC_CLK_ENABLE(); 
    // GPIO_InitStructure.Pin = RUNNING_LED_PIN;
    // GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    // GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    // HAL_GPIO_Init(RUNNING_LED_PORT, &GPIO_InitStructure);
	// HAL_GPIO_WritePin(RUNNING_LED_PORT, RUNNING_LED_PIN, GPIO_PIN_RESET);  //blue led
	
    //MX_GPIO_Init(); //biby
	// MX_USB_Device_Init(); //biby
	// while(1);
	
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
