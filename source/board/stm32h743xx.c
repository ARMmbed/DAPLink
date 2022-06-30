/**
 * @file    stm32h743xx.c
 * @brief   board ID for the STM32 NUCLEO-F103RB board
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2019, ARM Limited, All Rights Reserved
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

#include "target_family.h"
#include "target_board.h"
#include <stdio.h>

#ifdef UDB
#include "i2c.h"
#include "udb_version.h"
#include "udb_extended_features_task.h"
#include "udb_power_measurement.h"
#include "udb_log.h"
#include "udb_version.h"
#include "nluif_udb-daplink.h"
#include "util.h"
#include "udb_fault_info.h"
#include "udb_watchdog.h"
#include "udb_fault_info.h"
#include "udb_uptime.h"

#define UDB_WATCHDOG_TIMEOUT_S  (10U)

#define UDB_30MS_MULTIPLIER_TO_30SEC    (1000)
#define UDB_30MS_MULTIPLIER_TO_10MIN    (1000 * 20)

#define UDB_LED_30MS_MULTIPLER_NORMAL                   (50ULL)
#define UDB_LED_30MS_MULTIPLER_UNCLEARED_FAULT_INFO     (5ULL)

#ifdef UDB_DEBUG
#define UDB_CHECK_FAULT_INFO_TARGET_COUNT     UDB_30MS_MULTIPLIER_TO_30SEC
#else
#define UDB_CHECK_FAULT_INFO_TARGET_COUNT     UDB_30MS_MULTIPLIER_TO_10MIN
#endif

static uint64_t s_30ms_hook_counter = 0;
static uint64_t s_connected_led_blink_period = UDB_LED_30MS_MULTIPLER_NORMAL;

extern void udb_config_init(void);

static void udb_welcome_message(void)
{
    char ver_buf[UDB_VERSION_MAX_LENGTH];

    printf("Welcome to\n");
    printf("\t          ______   ______      \n");
    printf("\t|\\     /|(  __  \\ (  ___ \\  \n");
    printf("\t| )   ( || (  \\  )| (   ) )   \n");
    printf("\t| |   | || |   ) || (__/ /     \n");
    printf("\t| |   | || |   | ||  __ (      \n");
    printf("\t| |   | || |   ) || (  \\ \\   \n");
    printf("\t| (___) || (__/  )| )___) )    \n");
    printf("\t(_______)(______/ |/ \\___/    \n\n");

    udb_get_interface_version((uint8_t*)ver_buf, UDB_VERSION_MAX_LENGTH);
    printf("Interface version: %s\n", ver_buf);
    udb_get_bootloader_version((uint8_t*)ver_buf, UDB_VERSION_MAX_LENGTH);
    printf("Bootloader version: %s\n", ver_buf);

    printf("To know more about udb, visit go/udb.\n");
    printf("Please report issues at go/udb-bug.\n");
    printf("You can start typing commands.\n");

    uif_prompt();
}

static void prerun_board_config(void)
{
    int status;

    udb_uptime_init();

    status = i2c_init();
    util_assert(status == UDB_SUCCESS);

    udb_read_hw_version();
    udb_config_init();

    udb_extended_features_task_create();

    status = udb_power_measurement_init();
    util_assert(status == UDB_SUCCESS);

    udb_check_unexpected_watchdog_reset();
}

void board_30ms_hook()
{
    udb_uptime_update();
    if (udb_is_fault_info_uncleared())
    {
        s_connected_led_blink_period = UDB_LED_30MS_MULTIPLER_UNCLEARED_FAULT_INFO;
    }
    else
    {
        s_connected_led_blink_period = UDB_LED_30MS_MULTIPLER_NORMAL;
    }

    if (s_30ms_hook_counter == 0)
    {
        // Code that needs to run once after the bootloader update and all other
        // initialization finished

        udb_watchdog_init(UDB_WATCHDOG_TIMEOUT_S);
        udb_welcome_message();
    }

    udb_watchdog_refresh();

    if ((s_30ms_hook_counter % s_connected_led_blink_period) == 0)
    {
        HAL_GPIO_TogglePin( CONNECTED_LED_PORT, CONNECTED_LED_PIN);
    }

    if (((s_30ms_hook_counter % UDB_CHECK_FAULT_INFO_TARGET_COUNT) == 0) && udb_is_fault_info_uncleared())
    {
        udb_print_fault_info();
    }

    if (udb_log_cdc_ready())
    {
        udb_log_flush();
    }

    s_30ms_hook_counter++;
}
#endif // UDB

// return non-zero value to make all image incompatible
uint8_t board_detect_incompatible_image(const uint8_t *data, uint32_t size)
{
    return 1;
}

const board_info_t g_board_info =
{
    .info_version = kBoardInfoVersion,
    .board_id = "0000",
    .family_id = kStub_HWReset_FamilyID,
    .daplink_drive_name = "DAPLINK_APP",
    .prerun_board_config = prerun_board_config,
    // TODO(https://github.com/ARMmbed/DAPLink/issues/963):
    // MSC sometimes fails for unknown reason.
    // Disable mass storage as a workaround temporarily.
#if 0
    .target_cfg = &target_device,
#endif
};
