/**
 * @file    microbitv2.c
 * @brief   board ID for the BBC Microbit board
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
 * Copyright 2020 NXP
 * Copyright 2021 Micro:bit Educational Foundation
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

#include "IO_Config.h"
#include "DAP.h"
#include "target_family.h"
#include "target_board.h"
#include "gpio.h"
#include "rl_usb.h"
#include "flash_manager.h"
#include "virtual_fs.h"
#include "vfs_manager.h"
#include "device.h"
#include "main_interface.h"

#include "microbitv2.h"
#include "pwm.h"
#include "power.h"
#include "pwr_mon.h"
#include "i2c_commands.h"
#include "i2c.h"
#include "led_error_app.h"
#include "storage.h"
#include "gpio_extra.h"
#include "board_id.h"

#ifdef DRAG_N_DROP_SUPPORT
#include "flash_intf.h"
#endif

// Declared in intelhex.c
uint16_t board_id_hex_default = BOARD_VERSION_2_DEF;
uint16_t board_id_hex = BOARD_VERSION_2_DEF;

extern target_cfg_t target_device_nrf52833;

volatile uint8_t wake_from_reset = 0;
volatile uint8_t wake_from_usb = 0;
volatile bool usb_pc_connected = false;
power_source_t power_source;
microbit_if_power_mode_t interface_power_mode = MB_POWER_DOWN;
bool power_led_sleep_state_on = PWR_LED_SLEEP_STATE_DEFAULT;
bool automatic_sleep_on = AUTOMATIC_SLEEP_DEFAULT;
main_shutdown_state_t main_shutdown_state = MAIN_SHUTDOWN_WAITING;
bool do_remount = false;

extern main_usb_connect_t usb_state;

extern void main_powerdown_event(void);

static uint32_t read_file_data_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors);

// shutdown state
static uint8_t shutdown_led_dc = PWR_LED_ON_MAX_BRIGHTNESS;
static uint8_t final_fade_led_dc = 0;
static uint8_t power_led_max_duty_cycle = PWR_LED_ON_MAX_BRIGHTNESS;
static uint8_t initial_fade_brightness = PWR_LED_INIT_FADE_BRIGHTNESS;
// reset button state count
static uint16_t gpio_reset_count = 0;
// button state
static uint8_t reset_pressed = 0;


static void set_board_id(mb_version_t board_version) {
    switch (board_version) {
        case BOARD_VERSION_2_0:
            g_board_info.target_cfg->rt_board_id = BOARD_ID_MB_2_0;
            board_id_hex = BOARD_VERSION_2_0;
            break;
        case BOARD_VERSION_2_2_833:
            g_board_info.target_cfg->rt_board_id = BOARD_ID_MB_2_2_833;
            board_id_hex = BOARD_VERSION_2_2_833;
            break;
        case BOARD_VERSION_2_2_820:
            g_board_info.target_cfg->rt_board_id = BOARD_ID_MB_2_2_820;
            board_id_hex = BOARD_VERSION_2_2_820;
            break;
        case BOARD_VERSION_2_DEF:
            /* Intentional fall-through */
        default:
            g_board_info.target_cfg->rt_board_id = BOARD_ID_MB_2_DEFAULT;
            board_id_hex = BOARD_VERSION_2_DEF;
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
    // HID_LED_DEF is on so the resting state of the orange LED after flashing is on
    // but turn it off here so it's initially off, then stays off when on battery,
    // and comes on only when USB enumerates
    gpio_set_hid_led(GPIO_LED_OFF);

    mb_version_t board_version = board_id_detect();
    set_board_id(board_version);

    // init power monitoring
    power_init();
    pwr_mon_init();

    power_source = pwr_mon_get_power_source();

    pwm_init();
    pwm_init_pins();

    if (power_source == PWR_BATT_ONLY){
        // Turn on the red LED with low duty cycle to conserve power.
        power_led_max_duty_cycle = PWR_LED_ON_BATT_BRIGHTNESS;

    } else {
        // Turn on the red LED with max duty cycle when powered by USB or EC
        power_led_max_duty_cycle = PWR_LED_ON_MAX_BRIGHTNESS;
    }
    uint8_t gamma_led_dc = get_led_gamma(power_led_max_duty_cycle);
    pwm_set_dutycycle(gamma_led_dc);

    storage_init();
    i2c_cmds_init();

    gpio_init_combined_int();
}

// Return reset button and LED to default state
static void reset_power_led_state()
{
    reset_pressed = 0;
    power_led_sleep_state_on = PWR_LED_SLEEP_STATE_DEFAULT;
    main_shutdown_state = MAIN_SHUTDOWN_WAITING;
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
        gpio_enable_hid_led();
        power_led_max_duty_cycle = PWR_LED_ON_MAX_BRIGHTNESS;
    }
    else if (usb_state == USB_DISCONNECTED) {
        // Disable pin
        gpio_disable_hid_led();
        power_led_max_duty_cycle = PWR_LED_ON_BATT_BRIGHTNESS;
    }

    if (wake_from_usb) {
        main_shutdown_state = MAIN_USER_EVENT;

        if (usb_state == USB_DISCONNECTED) {
            usb_state = USB_CONNECTING;
        }
    }

    i2c_30ms_tick();

    // Enter light sleep if USB is not enumerated and main_shutdown_state is idle
    if (usb_state == USB_DISCONNECTED && !usb_pc_connected && main_shutdown_state == MAIN_SHUTDOWN_WAITING
        && automatic_sleep_on == true && i2c_canSleep()) {
        interface_power_mode = MB_POWER_SLEEP;
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
          gpio_disable_combined_int();

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
          gpio_assert_combined_int();

          // Return LED to ON after release of long press
          main_shutdown_state = MAIN_SHUTDOWN_WAITING;
          }
          break;
      case MAIN_SHUTDOWN_REQUESTED:
          if (power_source == PWR_BATT_ONLY || (usb_state == USB_DISCONNECTED && !usb_pc_connected)) {
              main_powerdown_event();

              // In VLLS0, set the LED either ON or LOW, depending on power_led_sleep_state_on
              // When the duty cycle is 0% or 100%, the FlexIO driver will configure the pin as GPIO
              if (power_led_sleep_state_on == true && interface_power_mode == MB_POWER_DOWN) {
                  shutdown_led_dc = PWR_LED_ON_MAX_BRIGHTNESS;
              } else if (power_led_sleep_state_on == true && interface_power_mode == MB_POWER_SLEEP) {
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
        pwm_set_dutycycle(gamma_led_dc);
    } else {
        pwm_set_dutycycle(final_fade_led_dc);
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
        case MB_POWER_SLEEP:
            power_sleep();
            break;
        case MB_POWER_DOWN:
            power_down();
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
    reset_power_led_state();

    // Clear any pending I2C response
    i2c_clearState();

    // Release COMBINED_SENSOR_INT
    gpio_disable_combined_int();
}

bool vfs_user_magic_file_hook(const vfs_filename_t filename, bool *do_remount)
{
    if (!memcmp(filename, "ERASE   ACT", sizeof(vfs_filename_t))) {
        storage_erase_all();
    }

    return false;
}

void vfs_user_build_filesystem_hook() {
    error_t status;
    error_t error = vfs_mngr_get_transfer_status();

    // Microbit error codes for DAPLink in the 500-599 range
    uint16_t microbit_error = error + 500;

    if (error != ERROR_SUCCESS) {
        // Error code offset after "0xFACEC0DE" magic word
        g_led_error_bin_data[g_led_error_bin_code] = (uint8_t) microbit_error & 0xFF;
        g_led_error_bin_data[g_led_error_bin_code + 1] = (uint8_t) (microbit_error >> 8) & 0xFF;

        status = flash_manager_init(flash_intf_target);
        if (status != ERROR_SUCCESS) {
            util_assert(0);
        }

        status = flash_manager_data(0, (const uint8_t*)g_led_error_bin_data, g_led_error_bin_len);
        if (status != ERROR_SUCCESS) {
            flash_manager_uninit();
            util_assert(0);
        }

        status = flash_manager_uninit();
        if (status != ERROR_SUCCESS) {
            util_assert(0);
        }
    }

    uint32_t storage_enc_start = storage_cfg_get_encoding_start();
    uint32_t storage_enc_end = storage_cfg_get_encoding_end();
    uint32_t file_size = storage_cfg_get_file_size();
    // Each byte within the encoding window is converted into an ASCII 2 byte character
    file_size += storage_enc_end - storage_enc_start;

    if (storage_cfg_get_file_visible()) {
        vfs_create_file(storage_cfg_get_filename(), read_file_data_txt, 0, file_size);
    }
}

// File callback to be used with vfs_add_file to return file contents
static uint32_t read_file_data_txt(uint32_t sector_offset, uint8_t *data, uint32_t num_sectors)
{
    uint32_t read_address = STORAGE_ADDRESS_START + (VFS_SECTOR_SIZE * sector_offset);
    uint32_t storage_enc_start = storage_cfg_get_encoding_start();
    uint32_t storage_enc_end = storage_cfg_get_encoding_end();
    uint32_t encoded_data_offset = (storage_enc_end - storage_enc_start);

    // Ignore out of bound reads
    if ( read_address < (STORAGE_ADDRESS_END + encoded_data_offset) ) {
        for (uint32_t i = 0; i < VFS_SECTOR_SIZE; i++) {
            if (i + (VFS_SECTOR_SIZE * sector_offset) < storage_enc_start) {
                // If data is before encoding window, no offset is needed
                data[i] = *(uint8_t *) (read_address + i);
            } else if(i + (VFS_SECTOR_SIZE * sector_offset) < (storage_enc_start + encoded_data_offset * 2)) {
                // Data inside encoding window needs to consider encoding window start and size
                uint8_t enc_byte = *(uint8_t *) (STORAGE_ADDRESS_START + ((VFS_SECTOR_SIZE * sector_offset) + storage_enc_start + i ) / 2);
                if (i % 2 == 0) {
                    // High nibble
                    enc_byte = 0x0F & (enc_byte >> 4);
                } else {
                    // Low nibble
                    enc_byte = 0x0F & enc_byte;
                }
                // Encode one nibble to one ASCII byte
                data[i] = enc_byte <= 9 ? enc_byte + 0x30 : enc_byte + 0x37;
            } else {
                // If data is after encoding window, adjustment is needed
                data[i] = *(uint8_t *) (read_address + i - encoded_data_offset);
            }
        }
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

// This function is called before the rest of target_set_state code, so it will
// reset the micro:bit specific features state before the target state is executed
static uint8_t target_set_state_microbit(target_state_t state)
{
    if (state == RESET_RUN) {
        i2c_clearState();
        reset_power_led_state();
    }
    return 0;
}

const board_info_t g_board_info = {
    .info_version = kBoardInfoVersion,
    .family_id = kNordic_Nrf52_FamilyID,
    .daplink_url_name = "MICROBITHTM",
    .daplink_drive_name = "MICROBIT",
    .daplink_target_url = "https://microbit.org/device/?id=@B&v=@V",
    .prerun_board_config = prerun_board_config,
    .target_cfg = &target_device_nrf52833,
    .target_set_state = target_set_state_microbit,
    .board_vendor = "Micro:bit Educational Foundation",
    .board_name = "BBC micro:bit V2",
};
