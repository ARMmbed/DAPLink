/**
 * @file    microbitv2.c
 * @brief   board ID for the BBC Microbit board
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
 * Copyright 2020 NXP
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
 
#include "fsl_device_registers.h"
#include "IO_Config.h"
#include "DAP.h"
#include "target_family.h"
#include "target_board.h"
#include "flexio_pwm.h"
#include "gpio.h"
#include "power.h"
#include "rl_usb.h" 
#include "pwr_mon.h"
#include "main.h"
#include "i2c.h"
#include "i2c_commands.h"
#include "adc.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_i2c.h"
#include "led_error_app.h"
#include "flash_manager.h"
#include "virtual_fs.h"
#include "vfs_manager.h"
#include "cortex_m.h"
#include "fsl_flash.h"

#ifdef DRAG_N_DROP_SUPPORT
#include "flash_intf.h"
#endif

#define M0_RESERVED_VECT_OFFSET     (4 * 4)
#define M0_RESERVED_VECT_SIZE       (3 * 4) // Size for mem fault, bus fault and usage fault vectors

#define BRD_ID_1_UPPER_THR_V        935   // Upper threshold in mV for 100nF and 4700R
#define BRD_ID_1_LOWER_THR_V        268   // Lower threshold in mV for 100nF and 4700R

#define PWR_LED_ON_MAX_BRIGHTNESS       255 // Max LED Brightness (PC Connected)
#define PWR_LED_INIT_FADE_BRIGHTNESS    200 // Initial fade LED Brightness
#define PWR_LED_ON_BATT_BRIGHTNESS      100 // LED Brightness while being powered by battery
#define PWR_LED_FADEOUT_MIN_BRIGHTNESS  80  // Minimum LED brightness when fading out
#define PWR_LED_SLEEP_STATE_DEFAULT     true
#define AUTOMATIC_SLEEP_DEFAULT         true

#define FLASH_CONFIG_ADDRESS        (0x00020000)
#define FLASH_CONFIG_SIZE           (0x00000400)
#define FLASH_INTERFACE_SIZE        (128*1024)
#define FLASH_STORAGE_ADDRESS       (0x00020400)
#define FLASH_CFG_FILENAME          "DATA    BIN"
#define FLASH_CFG_FILESIZE          (126*1024)
#define FLASH_CFG_FILEVISIBLE       false

// 'kvld' in hex - key valid
#define CFG_KEY             0x6b766c64

const char * const board_id_mb_2_0 = "9903";
const char * const board_id_mb_2_1 = "9904";

uint16_t board_id_hex_min = 0x9903;
uint16_t board_id_hex = 0;

volatile uint8_t wake_from_reset = 0;
volatile uint8_t wake_from_usb = 0;
volatile bool usb_pc_connected = false;
uint8_t i2c_wake_timeout = 0;
bool i2c_allow_sleep = true;
power_source_t power_source;

typedef enum {
    BOARD_VERSION_2_0 = 0,
    BOARD_VERSION_2_1 = 1,
} mb_version_t;

extern target_cfg_t target_device_nrf52_64;
extern main_usb_connect_t usb_state;
extern bool go_to_sleep;
extern i2c_slave_handle_t g_s_handle;
extern flash_config_t g_flash;

typedef enum main_shutdown_state {
    MAIN_SHUTDOWN_WAITING = 0,
    MAIN_SHUTDOWN_WAITING_OFF,
    MAIN_SHUTDOWN_PENDING,
    MAIN_SHUTDOWN_REACHED,
    MAIN_SHUTDOWN_REACHED_FADE,
    MAIN_SHUTDOWN_REQUESTED,
    MAIN_USER_EVENT,
    MAIN_LED_BLINK_ONCE,
    MAIN_LED_BLINKING,
    MAIN_LED_FULL_BRIGHTNESS,
    MAIN_SHUTDOWN_CANCEL
} main_shutdown_state_t;

extern void main_powerdown_event(void);

static void i2c_write_comms_callback(uint8_t* pData, uint8_t size);
static void i2c_read_comms_callback(uint8_t* pData, uint8_t size);
static void i2c_write_flash_callback(uint8_t* pData, uint8_t size);
static void i2c_read_flash_callback(uint8_t* pData, uint8_t size);
static uint32_t read_file_data_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors);

// shutdown state
static main_shutdown_state_t main_shutdown_state = MAIN_SHUTDOWN_WAITING;
static uint8_t shutdown_led_dc = PWR_LED_ON_MAX_BRIGHTNESS;
static uint8_t final_fade_led_dc = 0;
static uint8_t power_led_max_duty_cycle = PWR_LED_ON_MAX_BRIGHTNESS;
static uint8_t initial_fade_brightness = PWR_LED_INIT_FADE_BRIGHTNESS;
static bool power_led_sleep_state_on = PWR_LED_SLEEP_STATE_DEFAULT;
static bool automatic_sleep_on = AUTOMATIC_SLEEP_DEFAULT;
static app_power_mode_t interface_power_mode = kAPP_PowerModeVlls0;
// reset button state count
static uint16_t gpio_reset_count = 0;
// button state
static uint8_t reset_pressed = 0;
static bool do_remount = false;

static flashConfig_t gflashConfig = {
    .key = CFG_KEY,
    .fileName = FLASH_CFG_FILENAME,
    .fileSize = FLASH_CFG_FILESIZE,
    .fileVisible = FLASH_CFG_FILEVISIBLE,
};

// Board Rev ID detection. Reads BRD_REV_ID voltage
// Depends on gpio_init() to have been executed already
static mb_version_t read_brd_rev_id_pin(void) {
    gpio_pin_config_t pin_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };
    mb_version_t board_version = BOARD_VERSION_2_0;
    uint32_t board_rev_id_adc = 0;
    uint32_t board_rev_id_mv = 0;

    // Set Board Rev ID pin as output but pin disabled
    PORT_SetPinMux(PIN_BOARD_REV_ID_PORT , PIN_BOARD_REV_ID_BIT,  kPORT_PinDisabledOrAnalog);
    PORT_SetPinDriveStrength(PIN_BOARD_REV_ID_PORT, PIN_BOARD_REV_ID_BIT, kPORT_HighDriveStrength);
    GPIO_PinInit(PIN_BOARD_REV_ID_GPIO, PIN_BOARD_REV_ID_BIT, &pin_config);

    adc_init();
    
    // 1. Discharge capacitor
    //    Drive BRD_REV_ID pin to low
    GPIO_PortClear(PIN_BOARD_REV_ID_GPIO, PIN_BOARD_REV_ID);
    PORT_SetPinMux(PIN_BOARD_REV_ID_PORT , PIN_BOARD_REV_ID_BIT,  kPORT_MuxAsGpio);
    //    Add a 3ms delay to allow the 100nF Cap to discharge 
    //    at least 5*RC with 4700R.
    for (uint32_t count = 16 * 3000; count > 0UL; count--);
    
    // 2. Charge capacitor for 100us
    //    Drive BRD_REV_ID pin to high
    GPIO_PortSet(PIN_BOARD_REV_ID_GPIO, PIN_BOARD_REV_ID);
    //    Add a ~100us delay
    //    3 clock cycles per loop at -O2 ARMCC optimization
    for (uint32_t count = 1600; count > 0UL; count--);
    //    Change pin to ADC (High-Z). Capacitor will stop charging
    PORT_SetPinMux(PIN_BOARD_REV_ID_PORT , PIN_BOARD_REV_ID_BIT,  kPORT_PinDisabledOrAnalog);
    
    // 3. Take ADC measurement
    board_rev_id_adc = adc_read_channel(0, PIN_BOARD_REV_ID_ADC_CH, PIN_BOARD_REV_ID_ADC_MUX);
    board_rev_id_mv = board_rev_id_adc * 3300 / 0xFFF;  // Convert ADC 12-bit value to mV with 3.3V reference
    
    // 4. Discharge capacitor
    //    Drive BRD_REV_ID pin to low
    GPIO_PortClear(PIN_BOARD_REV_ID_GPIO, PIN_BOARD_REV_ID);
    PORT_SetPinMux(PIN_BOARD_REV_ID_PORT , PIN_BOARD_REV_ID_BIT,  kPORT_MuxAsGpio);
    //    Add a 3ms delay to allow the 100nF Cap to discharge 
    //    at least 5*RC with 4700R.
    for (uint32_t count = 16 * 3000; count > 0UL; count--);
    
    // 5. Identify board ID depending on voltage
    if ( board_rev_id_mv > BRD_ID_1_LOWER_THR_V && board_rev_id_mv < BRD_ID_1_UPPER_THR_V) {
        board_version = BOARD_VERSION_2_1;
    } else {
        board_version = BOARD_VERSION_2_0;
    }
    
    return board_version;
}

static void set_board_id(mb_version_t board_version) {
    target_device = target_device_nrf52_64;
    switch (board_version) {
        case BOARD_VERSION_2_0:
            target_device.rt_board_id = board_id_mb_2_0;
            board_id_hex = 0x9903;
            break;
        case BOARD_VERSION_2_1:
            target_device.rt_board_id = board_id_mb_2_1;
            board_id_hex = 0x9904;
            break;
        default:
            target_device.rt_board_id = board_id_mb_2_0;
            board_id_hex = 0x9903;
            break;
    }
}

// Apply a gamma curve to the LED. Input brightness between 0-255
static inline uint8_t get_led_gamma(uint8_t brightness) {
    uint8_t duty_cycle;
    duty_cycle = (brightness * brightness * brightness + 32512) / (255*255);
    return duty_cycle;
}

// Called in main_task() to init before USB and files are configured
static void prerun_board_config(void)
{
    mb_version_t board_version = read_brd_rev_id_pin();
    set_board_id(board_version);
    
    // init power monitoring
    power_init();
    pwr_mon_init();

    power_source = pwr_mon_get_power_source();

    flexio_pwm_init();
    flexio_pwm_init_pins();
    
    if (power_source == PWR_BATT_ONLY){
        // Turn on the red LED with low duty cycle to conserve power.
        power_led_max_duty_cycle = PWR_LED_ON_BATT_BRIGHTNESS;
        
    } else {
        // Turn on the red LED with max duty cycle when powered by USB or EC
        power_led_max_duty_cycle = PWR_LED_ON_MAX_BRIGHTNESS;
    }
    uint8_t gamma_led_dc = get_led_gamma(power_led_max_duty_cycle);
    flexio_pwm_set_dutycycle(gamma_led_dc);
    
    i2c_initialize();
    i2c_RegisterWriteCallback(i2c_write_comms_callback, I2C_SLAVE_NRF_KL_COMMS);
    i2c_RegisterReadCallback(i2c_read_comms_callback, I2C_SLAVE_NRF_KL_COMMS);
    i2c_RegisterWriteCallback(i2c_write_flash_callback, I2C_SLAVE_FLASH);
    i2c_RegisterReadCallback(i2c_read_flash_callback, I2C_SLAVE_FLASH);
    
    gpio_pin_config_t pin_config = {
        .pinDirection = kGPIO_DigitalOutput,
        .outputLogic = 0U
    };

    /* COMBINED_SENSOR_INT pin mux ALT0 (Disabled High-Z) */
    PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_PinDisabledOrAnalog);
    /* COMBINED_SENSOR_INT as output default low when pin mux ALT1 */
    GPIO_PinInit(COMBINED_SENSOR_INT_GPIO, COMBINED_SENSOR_INT_PIN, &pin_config);
    
    // Load Config from Flash if present
    flashConfig_t * pflashConfigROM;
    pflashConfigROM = (void *)FLASH_CONFIG_ADDRESS;
    
    if (CFG_KEY == pflashConfigROM->key) {
        memcpy(&gflashConfig, pflashConfigROM, sizeof(flashConfig_t));
    }
}

// Handle the reset button behavior, this function is called in the main task every 30ms
void handle_reset_button()
{
#ifdef DRAG_N_DROP_SUPPORT
    if (!flash_intf_target->flash_busy()) //added checking if flashing on target is in progress
#endif
    {
        // handle reset button without eventing
        if (!reset_pressed && (gpio_get_reset_btn_fwrd() || wake_from_reset)) {
            // Reset button pressed
            target_set_state(RESET_PROGRAM);
            reset_pressed = 1;
            gpio_reset_count = 0;
            wake_from_reset = 0;
            main_shutdown_state = MAIN_LED_FULL_BRIGHTNESS;
        } else if (reset_pressed && !gpio_get_reset_btn_fwrd()) {
            // Reset button released
            target_set_state(RESET_RUN);
            reset_pressed = 0;
            power_led_sleep_state_on = PWR_LED_SLEEP_STATE_DEFAULT;

            if (gpio_reset_count <= RESET_SHORT_PRESS) {
                main_shutdown_state = MAIN_LED_BLINK_ONCE;
            }
            else if (gpio_reset_count < RESET_MID_PRESS) {
                // Indicate button has been released to stop to cancel the shutdown
                main_shutdown_state = MAIN_LED_BLINK_ONCE;
            }
            else if (gpio_reset_count >= RESET_MID_PRESS) {
                // Indicate the button has been released when shutdown is requested
                main_shutdown_state = MAIN_USER_EVENT;
            }
        } else if (reset_pressed && gpio_get_reset_btn_fwrd()) {
            // Reset button is still pressed
            if (gpio_reset_count <= RESET_SHORT_PRESS) {
                // Enter the shutdown pending state to begin LED dimming
                main_shutdown_state = MAIN_SHUTDOWN_PENDING;
            }
            else if (gpio_reset_count < RESET_MID_PRESS) {
                // Enter the shutdown pending state to begin LED dimming
                main_shutdown_state = MAIN_SHUTDOWN_PENDING;
            }
            else if (gpio_reset_count < RESET_LONG_PRESS) {
                // Enter the shutdown reached state to blink LED
                main_shutdown_state = MAIN_SHUTDOWN_REACHED;
            }
            else if (gpio_reset_count >= RESET_LONG_PRESS) {
                // Enter the shutdown reached state to blink LED
                main_shutdown_state = MAIN_SHUTDOWN_REACHED_FADE;
            }

            // Avoid overflow, stop counting after longest event
            if (gpio_reset_count <= RESET_MAX_LENGTH_PRESS) {
                gpio_reset_count++;
            }
        }
    }
}

void board_30ms_hook()
{
  static uint8_t blink_in_progress = 0;
    
    if (usb_state == USB_CONNECTED) {
        // configure pin as GPIO
        PIN_HID_LED_PORT->PCR[PIN_HID_LED_BIT] = PORT_PCR_MUX(1);
        power_led_max_duty_cycle = PWR_LED_ON_MAX_BRIGHTNESS;
    }
    else if (usb_state == USB_DISCONNECTED) {
        // Disable pin
        PIN_HID_LED_PORT->PCR[PIN_HID_LED_BIT] = PORT_PCR_MUX(0);
        power_led_max_duty_cycle = PWR_LED_ON_BATT_BRIGHTNESS;
    }
    
    if (wake_from_usb) {
        main_shutdown_state = MAIN_USER_EVENT;
        
        if (usb_state == USB_DISCONNECTED) {
            usb_state = USB_CONNECTING;
        }
    }
    
    if (i2c_wake_timeout > 0) {
        i2c_wake_timeout--;
    }
    
    // Enter light sleep if USB is not enumerated and main_shutdown_state is idle
    if (usb_state == USB_DISCONNECTED && !usb_pc_connected && main_shutdown_state == MAIN_SHUTDOWN_WAITING 
        && automatic_sleep_on == true && g_s_handle.isBusy == false && i2c_wake_timeout == 0 && i2c_allow_sleep) { 
        interface_power_mode = kAPP_PowerModeVlps;
        main_shutdown_state = MAIN_SHUTDOWN_REQUESTED;
    }

    switch (main_shutdown_state) {
      case MAIN_LED_FULL_BRIGHTNESS:
          // Jump power LED to initial fade brightness
          shutdown_led_dc = initial_fade_brightness;
          break;
      case MAIN_SHUTDOWN_CANCEL:
          main_shutdown_state = MAIN_SHUTDOWN_WAITING;
          // Set the PWM value back to max duty cycle
          shutdown_led_dc = power_led_max_duty_cycle;
          break;
      case MAIN_SHUTDOWN_PENDING:
          // Fade the PWM until the board is about to be shut down
          shutdown_led_dc = initial_fade_brightness - gpio_reset_count * (initial_fade_brightness - PWR_LED_FADEOUT_MIN_BRIGHTNESS)/(RESET_MID_PRESS);
          break;
      case MAIN_SHUTDOWN_REACHED:
          // Hold LED in min brightness
          shutdown_led_dc = PWR_LED_FADEOUT_MIN_BRIGHTNESS;
          final_fade_led_dc = get_led_gamma(PWR_LED_FADEOUT_MIN_BRIGHTNESS);
          break;
      case MAIN_SHUTDOWN_REACHED_FADE:
          // Fast fade to off
          if (final_fade_led_dc > 0) {
              final_fade_led_dc--;
          }
          break;
      case MAIN_USER_EVENT:
          {
          // Release COMBINED_SENSOR_INT in case it was previously asserted
          PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_PinDisabledOrAnalog);
              
          // Prepare I2C response
          i2cCommand_t i2cResponse = {0};
          i2cResponse.cmdId = gReadResponse_c;
          i2cResponse.cmdData.readRspCmd.propertyId = (uint8_t) gUserEvent_c;
          i2cResponse.cmdData.readRspCmd.dataSize = 1;
          
          if (wake_from_usb) {
              i2cResponse.cmdData.readRspCmd.data[0] = gWakeFromWakeOnEdge_c;
              wake_from_usb = 0;
              power_led_sleep_state_on = PWR_LED_SLEEP_STATE_DEFAULT;
          } else {
              i2cResponse.cmdData.readRspCmd.data[0] = gResetButtonLongPress_c;
          }

          i2c_fillBuffer((uint8_t*) &i2cResponse, 0, sizeof(i2cResponse));

          // Response ready, assert COMBINED_SENSOR_INT
          PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_MuxAsGpio);
      
          // Return LED to ON after release of long press
          main_shutdown_state = MAIN_SHUTDOWN_WAITING;
          }
          break;
      case MAIN_SHUTDOWN_REQUESTED:
          if (power_source == PWR_BATT_ONLY || (usb_state == USB_DISCONNECTED && !usb_pc_connected)) {
              main_powerdown_event();
              
              // In VLLS0, set the LED either ON or LOW, depending on power_led_sleep_state_on
              // When the duty cycle is 0% or 100%, the FlexIO driver will configure the pin as GPIO
              if (power_led_sleep_state_on == true && interface_power_mode == kAPP_PowerModeVlls0) {
                  shutdown_led_dc = PWR_LED_ON_MAX_BRIGHTNESS;
              } else if (power_led_sleep_state_on == true && interface_power_mode == kAPP_PowerModeVlps) {
                  shutdown_led_dc = PWR_LED_ON_BATT_BRIGHTNESS;
              }
              else {
                  shutdown_led_dc = 0;
              }
              
              main_shutdown_state = MAIN_SHUTDOWN_WAITING;
          }
          else if (usb_state == USB_CONNECTED){
              main_shutdown_state = MAIN_LED_BLINKING;
          }
          break;
      case MAIN_SHUTDOWN_WAITING:
          // Set the PWM value back to max duty cycle
          shutdown_led_dc = power_led_max_duty_cycle;
          break;
      case MAIN_SHUTDOWN_WAITING_OFF:
          shutdown_led_dc = 0;
          break;
      case MAIN_LED_BLINKING:
           // Blink the LED to indicate standby mode
          if (shutdown_led_dc < 10) {
              shutdown_led_dc++;
          } else if (shutdown_led_dc == 10) {
              shutdown_led_dc = PWR_LED_ON_MAX_BRIGHTNESS;
          } else if (shutdown_led_dc <= (PWR_LED_ON_MAX_BRIGHTNESS - 10)) {
              shutdown_led_dc = 0;
          } else if (shutdown_led_dc > (PWR_LED_ON_MAX_BRIGHTNESS - 10)) {
              shutdown_led_dc--;
          }
          
          // If we are no longer PC connected, go into idle mode
          if (usb_state == USB_DISCONNECTED && !usb_pc_connected) {
              main_shutdown_state = MAIN_SHUTDOWN_WAITING;
          }
          break;
      case MAIN_LED_BLINK_ONCE:
          
          if (blink_in_progress) {
            blink_in_progress--;
            if (blink_in_progress == 5) {
              shutdown_led_dc = 0;
            }
          } else {
            blink_in_progress = 10;
            shutdown_led_dc = PWR_LED_ON_MAX_BRIGHTNESS;
          }
          
          if (blink_in_progress == 0) { 
            main_shutdown_state = MAIN_SHUTDOWN_WAITING;
          }
          break;
      default:
          break;
    }

    // Use gamma curve except in final fade
    if (main_shutdown_state != MAIN_SHUTDOWN_REACHED_FADE) {
        uint8_t gamma_led_dc = get_led_gamma(shutdown_led_dc);
        flexio_pwm_set_dutycycle(gamma_led_dc);
    } else {
        flexio_pwm_set_dutycycle(final_fade_led_dc);
    }
    
    // Remount if requested.
    if (do_remount) {
        do_remount = false;
        vfs_mngr_fs_remount();
    }
}

void board_handle_powerdown()
{
    switch(interface_power_mode){
        case kAPP_PowerModeVlps:
            power_enter_VLPS();
            break;
        case kAPP_PowerModeVlls0:
            power_enter_VLLS0();
            break;
        default:
            break;
    }
}

void board_usb_sof_event(void)
{
    /* Set usb_pc_connected to true to prevent entering sleep mode when host does re-enumeration.
    *  Will be set to false when the USB cable is detached. */
    usb_pc_connected = true;
}

void board_vfs_stream_closed_hook()
{
    // Return reset button and LED to default state
    reset_pressed = 0;
    power_led_sleep_state_on = PWR_LED_SLEEP_STATE_DEFAULT;
    main_shutdown_state = MAIN_SHUTDOWN_WAITING;
    
    // Clear any pending I2C response
    i2c_clearBuffer();
    
    // Release COMBINED_SENSOR_INT
    PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_PinDisabledOrAnalog);
}

bool vfs_user_magic_file_hook(const vfs_filename_t filename, bool *do_remount)
{
    if (!memcmp(filename, "ERASE   ACT", sizeof(vfs_filename_t))) {
        // Erase last 128KB flash block
        FLASH_Erase(&g_flash, FLASH_CONFIG_ADDRESS, g_flash.PFlashTotalSize / g_flash.PFlashBlockCount, kFLASH_apiEraseKey);
    }
    
    return false;
}

void vfs_user_build_filesystem_hook() {
    uint32_t file_size;
    error_t status;
    error_t error = vfs_mngr_get_transfer_status();

    // Microbit error codes for DAPLink in the 500-599 range
    uint16_t microbit_error = error + 500;

    if (error != ERROR_SUCCESS) {
        // Error code offset after "0xFACEC0DE" magic word
        led_error_bin_data[led_error_bin_code] = (uint8_t) microbit_error & 0xFF;
        led_error_bin_data[led_error_bin_code + 1] = (uint8_t) (microbit_error >> 8) & 0xFF;

        status = flash_manager_init(flash_intf_target);
        if (status != ERROR_SUCCESS) {
            util_assert(0);
        }

        status = flash_manager_data(0, (const uint8_t*)led_error_bin_data, led_error_bin_len);
        if (status != ERROR_SUCCESS) {
            flash_manager_uninit();
            util_assert(0);
        }

        status = flash_manager_uninit();
        if (status != ERROR_SUCCESS) {
            util_assert(0);
        }
    }

    file_size = gflashConfig.fileSize;
    
    if (gflashConfig.fileVisible) {
        vfs_create_file(gflashConfig.fileName, read_file_data_txt, 0, file_size);
    }
}

// File callback to be used with vfs_add_file to return file contents
static uint32_t read_file_data_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors)
{
    // Ignore out of bound reads
    if ( (FLASH_STORAGE_ADDRESS + VFS_SECTOR_SIZE * sector_offset) < (FLASH_CONFIG_ADDRESS + FLASH_INTERFACE_SIZE) ) {
        memcpy(data, (uint8_t *) (FLASH_STORAGE_ADDRESS + VFS_SECTOR_SIZE * sector_offset), VFS_SECTOR_SIZE);
    }

    return VFS_SECTOR_SIZE;
}

uint8_t board_detect_incompatible_image(const uint8_t *data, uint32_t size)
{
    uint8_t result = 0;
    
    // Check difference in vectors (mem fault, bus fault, usage fault)
    // If these vectors are 0, we assume it's an M0 image (not compatible)
    result = memcmp(data + M0_RESERVED_VECT_OFFSET, (uint8_t[M0_RESERVED_VECT_SIZE]){0}, M0_RESERVED_VECT_SIZE);
    
    return result == 0;
}

// USB HID override function return 1 if the activity is trivial or response is null
uint8_t usbd_hid_no_activity(uint8_t *buf)
{
    if(buf[0] == ID_DAP_Vendor3 &&  buf[1] == 0)
        return 1;
    else
        return 0;
}

const board_info_t g_board_info = {
    .info_version = kBoardInfoVersion,
    .family_id = kNordic_Nrf52_FamilyID,
    .daplink_url_name =       "MICROBITHTM",
    .daplink_drive_name =       "MICROBIT",
    .daplink_target_url = "https://microbit.org/device/?id=@B&v=@V",
    .prerun_board_config = prerun_board_config,
    .target_cfg = &target_device,
};

static void i2c_write_comms_callback(uint8_t* pData, uint8_t size) {
    i2cCommand_t* pI2cCommand = (i2cCommand_t*) pData;
    i2cCommand_t i2cResponse = {0};
    bool assert_interrupt = true;

    switch (pI2cCommand->cmdId) {
        case gReadRequest_c:
            i2cResponse.cmdId = gReadResponse_c;
            i2cResponse.cmdData.readRspCmd.propertyId = pI2cCommand->cmdData.readReqCmd.propertyId;
            switch (pI2cCommand->cmdData.readReqCmd.propertyId) {
                case gDAPLinkBoardVersion_c:
                    i2cResponse.cmdData.readRspCmd.dataSize = sizeof(board_id_hex);
                    memcpy(&i2cResponse.cmdData.readRspCmd.data, &board_id_hex, sizeof(board_id_hex));
                break;
                case gI2CProtocolVersion_c: {
                    uint16_t i2c_version = 1;
                    i2cResponse.cmdData.readRspCmd.dataSize = sizeof(i2c_version);
                    memcpy(&i2cResponse.cmdData.readRspCmd.data, &i2c_version, sizeof(i2c_version));
                }
                break;
                case gDAPLinkVersion_c: {
                    uint16_t daplink_version = DAPLINK_VERSION;
                    i2cResponse.cmdData.readRspCmd.dataSize = sizeof(daplink_version);
                    memcpy(&i2cResponse.cmdData.readRspCmd.data, &daplink_version, sizeof(daplink_version));
                }
                break;
                case gPowerState_c:
                    power_source = pwr_mon_get_power_source();
                    i2cResponse.cmdData.readRspCmd.dataSize = sizeof(power_source);
                    memcpy(&i2cResponse.cmdData.readRspCmd.data, &power_source, sizeof(power_source));
                break;
                case gPowerConsumption_c: {
                    uint32_t vin_voltage_uv = pwr_mon_get_vin_mv() * 1000;
                    uint32_t vbat_voltage_uv = pwr_mon_get_vbat_mv() * 1000;
                    i2cResponse.cmdData.readRspCmd.dataSize = sizeof(vin_voltage_uv) + sizeof(vbat_voltage_uv);
                    memcpy(&i2cResponse.cmdData.readRspCmd.data[0], &vbat_voltage_uv, sizeof(vbat_voltage_uv));
                    memcpy(&i2cResponse.cmdData.readRspCmd.data[sizeof(vbat_voltage_uv)], &vin_voltage_uv, sizeof(vin_voltage_uv));
                }
                break;
                case gUSBEnumerationState_c:
                    i2cResponse.cmdData.readRspCmd.dataSize = sizeof(usb_state);
                    memcpy(&i2cResponse.cmdData.readRspCmd.data, &usb_state, sizeof(usb_state));
                break;
                case gPowerMode_c:
                    i2cResponse.cmdId = gErrorResponse_c;
                    i2cResponse.cmdData.errorRspCmd.errorCode = gErrorReadDisallowed_c;
                break;
                default:
                    i2cResponse.cmdId = gErrorResponse_c;
                    i2cResponse.cmdData.errorRspCmd.errorCode = gErrorUnknownProperty_c;
                break;
            }
        break;
        case gReadResponse_c:
            i2cResponse.cmdId = gErrorResponse_c;
            i2cResponse.cmdData.errorRspCmd.errorCode = gErrorCommandDisallowed_c;
        break;
        case gWriteRequest_c:
            switch (pI2cCommand->cmdData.writeReqCmd.propertyId) {
                case gDAPLinkBoardVersion_c:
                case gI2CProtocolVersion_c:
                case gDAPLinkVersion_c:
                case gPowerState_c:
                case gPowerConsumption_c:
                case gUSBEnumerationState_c:
                    i2cResponse.cmdId = gErrorResponse_c;
                    i2cResponse.cmdData.errorRspCmd.errorCode = gErrorWriteDisallowed_c;
                break;
                case gPowerMode_c:
                    if (pI2cCommand->cmdData.writeReqCmd.dataSize == 1) {
                        if (pI2cCommand->cmdData.writeReqCmd.data[0] == kAPP_PowerModeVlls0) {
                            interface_power_mode = kAPP_PowerModeVlls0;
                            i2cResponse.cmdId = gWriteResponse_c;
                            i2cResponse.cmdData.writeRspCmd.propertyId = pI2cCommand->cmdData.writeReqCmd.propertyId;
                        } else { 
                            i2cResponse.cmdId = gErrorResponse_c;
                            i2cResponse.cmdData.errorRspCmd.errorCode = gErrorWriteFail_c;
                        }
                    } else {
                        i2cResponse.cmdId = gErrorResponse_c;
                        i2cResponse.cmdData.errorRspCmd.errorCode = gErrorWrongPropertySize_c;
                    }
                break;
                case gPowerLedSleepState_c:
                    if (pI2cCommand->cmdData.writeReqCmd.dataSize == 1) {
                        if (pI2cCommand->cmdData.writeReqCmd.data[0] != 0) {
                            power_led_sleep_state_on = true;
                        }
                        else {
                            power_led_sleep_state_on = false;
                        }
                        i2cResponse.cmdId = gWriteResponse_c;
                        i2cResponse.cmdData.writeRspCmd.propertyId = pI2cCommand->cmdData.writeReqCmd.propertyId;
                    } else {
                        i2cResponse.cmdId = gErrorResponse_c;
                        i2cResponse.cmdData.errorRspCmd.errorCode = gErrorWrongPropertySize_c;
                    }
                break;
                case gAutomaticSleep_c:
                    if (pI2cCommand->cmdData.writeReqCmd.dataSize == 1) {
                        if (pI2cCommand->cmdData.writeReqCmd.data[0] != 0) {
                            automatic_sleep_on = true;
                        }
                        else {
                            automatic_sleep_on = false;
                        }
                        i2cResponse.cmdId = gWriteResponse_c;
                        i2cResponse.cmdData.writeRspCmd.propertyId = pI2cCommand->cmdData.writeReqCmd.propertyId;
                    } else {
                        i2cResponse.cmdId = gErrorResponse_c;
                        i2cResponse.cmdData.errorRspCmd.errorCode = gErrorWrongPropertySize_c;
                    }
                break;
                default:
                    i2cResponse.cmdId = gErrorResponse_c;
                    i2cResponse.cmdData.errorRspCmd.errorCode = gErrorUnknownProperty_c;
                break;
            }
        break;
        case gWriteResponse_c:
        break;
        case gErrorResponse_c:
        break;
        case gNopCmd_c:
            assert_interrupt = false;
        break;
        default:
            i2cResponse.cmdId = gErrorResponse_c;
            i2cResponse.cmdData.errorRspCmd.errorCode = gErrorUnknownCommand_c;
        break;
    }
    
    if (assert_interrupt) {
        i2c_fillBuffer((uint8_t*) &i2cResponse, 0, sizeof(i2cResponse));
        // Response ready, assert COMBINED_SENSOR_INT
        PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_MuxAsGpio);
    }
}

static void i2c_read_comms_callback(uint8_t* pData, uint8_t size) {
    i2cCommand_t* pI2cCommand = (i2cCommand_t*) pData;

    switch (pI2cCommand->cmdId) {
        case gWriteResponse_c:
            switch (pI2cCommand->cmdData.writeRspCmd.propertyId) {
                case gPowerMode_c:
                    main_shutdown_state = MAIN_SHUTDOWN_REQUESTED;
                break;
            }
        break;
    }
    
    i2c_clearBuffer();
    
    // Release COMBINED_SENSOR_INT
    PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_PinDisabledOrAnalog);
}

static bool file_extension_allowed(const vfs_filename_t  filename)
{
    const char *valid_extensions[] = {
        "BIN",
        "TXT",
        "CSV",
        "HTM",
        "WAV",
    };
    uint32_t i;

    // Check for invalid starting characters
    for (i = 0; i < (sizeof((valid_extensions))/sizeof((valid_extensions)[0])); i++) {
        if (0 == memcmp(&filename[8], valid_extensions[i], 3)) {
            return true;
        }
    }

    // Some checks failed so file extension is invalid
    return false;
}

static uint32_t erase_storage_sector(uint32_t adr)
{
    int status = FLASH_Erase(&g_flash, adr, g_flash.PFlashSectorSize, kFLASH_apiEraseKey);
    if (status == kStatus_Success)
    {
        status = FLASH_VerifyErase(&g_flash, adr, g_flash.PFlashSectorSize, kFLASH_marginValueNormal);
    }
    return status;
}

static uint32_t program_storage_page(uint32_t adr, uint32_t sz, uint8_t *buf)
{
    /* Verify data is word aligned */
    util_assert(!((uint32_t)buf & 0x3));
    
    int status = FLASH_Program(&g_flash, adr, (uint32_t *) buf, sz);
    if (status == kStatus_Success)
    {
        // Must use kFlashMargin_User, or kFlashMargin_Factory for verify program
        status = FLASH_VerifyProgram(&g_flash, adr, sz,
                              (uint32_t *) buf, kFLASH_marginValueUser,
                              NULL, NULL);
    }
    return status;
}

static void i2c_write_flash_callback(uint8_t* pData, uint8_t size) {
    i2cFlashCmd_t* pI2cCommand = (i2cFlashCmd_t*) pData;
    
    uint32_t status = 0;
    
    uint32_t storage_address = pI2cCommand->cmdData.write.addr2 << 16 |
                            pI2cCommand->cmdData.write.addr1 << 8 |
                            pI2cCommand->cmdData.write.addr0 << 0;
    uint32_t address = storage_address + FLASH_STORAGE_ADDRESS;
    uint32_t length = __REV(pI2cCommand->cmdData.write.length);
    uint32_t data = (uint32_t) pI2cCommand->cmdData.write.data;

    i2c_clearBuffer();
    
    switch (pI2cCommand->cmdId) {
        case gFlashDataWrite_c:
            /* Validate length field matches with I2C Write data */
            if (size == length + 8) { 
                /* Address range and alignment validation done inside program_storage_page() */
                status = program_storage_page(address, length, (uint8_t *) data);
            
                if (0 != status) {
                    pI2cCommand->cmdId = gFlashError_c;
                    i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                }
                else {
                    /* Fill TX Buffer with received command args */
                    i2c_fillBuffer((uint8_t*) pI2cCommand, 0, sizeof(i2cFlashCmd_t) - 1024);
                    /* Fill TX Buffer with Flash Data Written */
                    i2c_fillBuffer((uint8_t*) address, sizeof(i2cFlashCmd_t) - 1024, length);
                }
            } else {
                pI2cCommand->cmdId = gFlashError_c;
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
            }
        break;
        case gFlashDataRead_c: {
            /* Do address range validation */
            if (address + length > (FLASH_CONFIG_ADDRESS + FLASH_INTERFACE_SIZE)) {
                pI2cCommand->cmdId = gFlashError_c;
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
            } else { 
                /* Fill TX Buffer with received command args */
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, sizeof(i2cFlashCmd_t) - 1024);
                /* Fill TX Buffer with Flash Data Read */
                i2c_fillBuffer((uint8_t*) address, sizeof(i2cFlashCmd_t) - 1024, length);
            }
        }
        break;
        case gFlashDataErase_c: {
            uint32_t address = pI2cCommand->cmdData.erase.sAddr2 << 16 |
                            pI2cCommand->cmdData.erase.sAddr1 << 8 |
                            pI2cCommand->cmdData.erase.sAddr0 << 0;
            uint32_t start_addr = address + FLASH_STORAGE_ADDRESS;
            
            address = pI2cCommand->cmdData.erase.eAddr2 << 16 |
                            pI2cCommand->cmdData.erase.eAddr1 << 8 |
                            pI2cCommand->cmdData.erase.eAddr0 << 0;
            uint32_t end_addr = address + FLASH_STORAGE_ADDRESS;
            
            /* Do address range validation */
            if (start_addr % DAPLINK_SECTOR_SIZE == 0 &&
                end_addr % DAPLINK_SECTOR_SIZE == 0 &&
                start_addr <= end_addr &&
                start_addr < (FLASH_CONFIG_ADDRESS + FLASH_INTERFACE_SIZE) &&
                end_addr < (FLASH_CONFIG_ADDRESS + FLASH_INTERFACE_SIZE)) {
                for (uint32_t addr = start_addr; addr <= end_addr && status == 0; addr += DAPLINK_SECTOR_SIZE) {
                    status = erase_storage_sector(addr);
                }
                
                if (status != 0) {
                    pI2cCommand->cmdId = gFlashError_c;
                }
            } else {
                pI2cCommand->cmdId = gFlashError_c;
            }
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);

        }
        break;
        case gFlashCfgFileName_c:        
             if (size == 1) {
                /* If size is 1 (only cmd id), this means it's a read */
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                i2c_fillBuffer((uint8_t*) &gflashConfig.fileName, 1, sizeof(gflashConfig.fileName));
            } else if (size == 12) {
                /* If size is 12 (cmd id + 11B data), this means it's a write */
                /* Validate 8.3 filename  */
                if (filename_valid((char *) pI2cCommand->cmdData.data)) {
                    // Check allowed extensions (.bin, .txt, .csv, .htm, .wav)
                    if (file_extension_allowed((char *) pI2cCommand->cmdData.data)) {
                        memcpy(gflashConfig.fileName, pI2cCommand->cmdData.data, 11);
                    }
                    // If disallowed extension is requested, .bin will be used
                    else { 
                        memcpy(gflashConfig.fileName, pI2cCommand->cmdData.data, 8);
                        memcpy(&gflashConfig.fileName[8], "BIN", 3);
                    }
                    i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                    i2c_fillBuffer((uint8_t*) &gflashConfig.fileName, 1, sizeof(gflashConfig.fileName));
                }
                else { 
                    // Send error if invalid filename
                    pI2cCommand->cmdId = gFlashError_c;
                    i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                }
            } else {
                pI2cCommand->cmdId = gFlashError_c;
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
            }
        break;
        case gFlashCfgFileSize_c:        
            if (size == 1) {
                /* If size is 1 (only cmd id), this means it's a read */
                uint32_t tempFileSize = __REV(gflashConfig.fileSize);
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                i2c_fillBuffer((uint8_t*) &tempFileSize, 1, sizeof(gflashConfig.fileSize));
            } else if (size == 5) {
                /* If size is 5 (cmd id + 4B data), this means it's a write */
                uint32_t tempFileSize = pI2cCommand->cmdData.data[0] << 24 |
                                        pI2cCommand->cmdData.data[1] << 16 |
                                        pI2cCommand->cmdData.data[2] << 8 |
                                        pI2cCommand->cmdData.data[3] << 0;
                
                /* Validate file size */
                if (tempFileSize <= (FLASH_INTERFACE_SIZE - FLASH_CONFIG_SIZE)) {
                    gflashConfig.fileSize = tempFileSize;
                    tempFileSize = __REV(gflashConfig.fileSize);
                    i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                    i2c_fillBuffer((uint8_t*) &tempFileSize, 1, sizeof(gflashConfig.fileSize));
                } else {
                    pI2cCommand->cmdId = gFlashError_c;
                    i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                }
            } else {
                pI2cCommand->cmdId = gFlashError_c;
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
            }
        break;
        case gFlashCfgFileVisible_c:
            if (size == 1) {
                /* If size is 1 (only cmd id), this means it's a read */
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                i2c_fillBuffer((uint8_t*) &gflashConfig.fileVisible, 1, sizeof(gflashConfig.fileVisible));
            } else if (size == 2) {
                /* If size is 2 (cmd id + 1B data), this means it's a write */
                gflashConfig.fileVisible = pI2cCommand->cmdData.data[0];
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
                i2c_fillBuffer((uint8_t*) &gflashConfig.fileVisible, 1, sizeof(gflashConfig.fileVisible));
            } else {
                pI2cCommand->cmdId = gFlashError_c;
                i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
            }
        break;
        case gFlashCfgWrite_c:
            // Check first is config is already present in flash
            // If differences are found, erase and write new config
            if (0 != memcmp(&gflashConfig, (void *)FLASH_CONFIG_ADDRESS, sizeof(flashConfig_t))) {
                status = erase_storage_sector(FLASH_CONFIG_ADDRESS);

                if (status != 0) {
                    pI2cCommand->cmdId = gFlashError_c;
                }
                else {
                    status = program_storage_page(FLASH_CONFIG_ADDRESS, sizeof(flashConfig_t), (uint8_t *) &gflashConfig);
                }
            }
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
        break;
        case gFlashCfgErase_c:
            // Erase flash sector containing flash config
            status = erase_storage_sector(FLASH_CONFIG_ADDRESS);
            
            if (status != 0) {
                pI2cCommand->cmdId = gFlashError_c;
            }
            else {
                // Return flash config (RAM) to default values
                gflashConfig.key = CFG_KEY;
                memcpy(gflashConfig.fileName, FLASH_CFG_FILENAME, 11);
                gflashConfig.fileSize = FLASH_CFG_FILESIZE;
                gflashConfig.fileVisible = FLASH_CFG_FILEVISIBLE;
            }
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
        break;
        case gFlashStorageSize_c:
            pI2cCommand->cmdData.data[0] = (FLASH_INTERFACE_SIZE - FLASH_CONFIG_SIZE)/1024;
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 2);
        break;
        case gFlashSectorSize_c:
            pI2cCommand->cmdData.data[0] = (DAPLINK_SECTOR_SIZE >> 8) & 0xFF;
            pI2cCommand->cmdData.data[1] = DAPLINK_SECTOR_SIZE & 0xFF;
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 3);
        break;
        case gFlashRemountMSD_c:
            do_remount = true;
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
        break;
        default:
            pI2cCommand->cmdId = gFlashError_c;
            i2c_fillBuffer((uint8_t*) pI2cCommand, 0, 1);
        break;
    }
    
    // Response ready, assert COMBINED_SENSOR_INT
    PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_MuxAsGpio);
}

static void i2c_read_flash_callback(uint8_t* pData, uint8_t size) {
    i2c_clearBuffer();
    
    // Release COMBINED_SENSOR_INT
    PORT_SetPinMux(COMBINED_SENSOR_INT_PORT, COMBINED_SENSOR_INT_PIN, kPORT_PinDisabledOrAnalog);
}
