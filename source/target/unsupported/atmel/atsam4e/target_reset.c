/**
 * @file    target_reset.c
 * @brief   Target reset for the atsam4e
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
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

#include "sam3u.h"
#include "RTL.h"
#include "debug_cm.h"
#include "target_reset.h"
#include "swd_host.h"
#include "gpio.h"

/*static const uint32_t nrfBlinkyApp[200] = {0x20000810,0x000000F5,0x00000107,0x00000109,0x00000000,
		0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x0000010B,0x00000000,
		0x00000000,0x0000010D,0x0000010F,0x00000111,0x00000111,0x00000111,0x00000111,0x00000111,
		0x00000000,0x00000111,0x00000111,0x00000111,0x00000111,0x00000111,0x00000111,0x00000111,
		0x00000111,0x00000111,0x00000111,0x00000111,0x00000111,0x00000111,0x00000111,0x00000111,
		0x00000111,0x00000111,0x00000111,0x00000111,0x00000111,0x00000000,0x00000000,0x00000000,
		0x00000000,0x00000000,0x00000000,0x46854803,0xF82CF000,0x47004800,0x00000235,0x20000810,
		0xBF001E40,0xBF00BF00,0xBF00BF00,0xBF00BF00,0xBF00BF00,0xBF00BF00,0xD1F1BF00,0x00004770,
		0x68024807,0x430A210F,0x48066002,0x48064780,0xE7FE4700,0xE7FEE7FE,0xE7FEE7FE,0x0000E7FE,
		0x40000524,0x00000145,0x000000C1,0x25014C06,0xE0054E06,0x68E34620,0x432BC807,0x34104798,
		0xD3F742B4,0xFFC6F7FF,0x000002F8,0x00000308,0xF000B510,0x2800F843,0x4807D005,0x60484907,
		0x490713C8,0xF0006188,0x2800F81F,0x2001D002,0x60884904,0x0000BD10,0xC007FFDF,0x40000500,
		0x40006C00,0x40000600,0xC808E002,0xC1081F12,0xD1FA2A00,0x47704770,0xE0012000,0x1F12C101,
		0xD1FB2A00,0x00004770,0x8C00480B,0x2801B2C0,0x4809D110,0x07008C80,0xD10B0F00,0x6A804806,
		0x400821F0,0xD1052840,0x6AC04803,0xD1014208,0x47702001,0xE7FC2000,0xF0000FC0,0x8C004817,
		0x2801B2C0,0x4815D127,0x07008C80,0xD1220F00,0x6A804812,0x420821F0,0x4810D105,0x42086AC0,
		0x2001D101,0x480D4770,0x21F06A80,0x28104008,0x480AD105,0x42086AC0,0x2001D101,0x4807E7F2,
		0x21F06A80,0x28304008,0x4804D105,0x42086AC0,0x2001D101,0x2000E7E6,0x0000E7E4,0xF0000FC0,
		0x68004819,0xF844F000,0x68004818,0xF840F000,0x68004815,0xF84EF000,0x68004814,0xF84AF000,
		0x0100207D,0xF826F000,0x480FE01D,0xF0006800,0x480EF839,0xF0006800,0x20C8F83D,0xF81AF000,
		0x6800480A,0xF82EF000,0x68004807,0xF832F000,0xF0002064,0x2000F80F,0xBF00E002,0x1C40BF00,
		0x42884903,0xE7E0DBF9,0x20000000,0x20000004,0x00001388,0xE005B501,0x1E409800,0x48039000,
		0xFF0EF7FF,0x28009800,0xBD08D1F6,0x000003E7,0x22052101,0x23070712,0x18D2021B,0x50D10083,
		0x00004770,0x40812101,0x60D14A01,0x00004770,0x50000500,0x40812101,0x60914A01,0x00004770,
		0x50000500,0x00000308,0x20000000,0x0000000C,0x0000017C,0x00000012,0x00000013,0x00F42400,
		0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF};*/

//Blink with 50ms interval
//static void blinkLED(){
//    gpio_set_dap_led(1);
//    os_dly_wait(5);
//    gpio_set_dap_led(0);
//    os_dly_wait(5);
//    gpio_set_dap_led(1);
//}

uint8_t security_bits_set(uint32_t addr, uint8_t *data, uint32_t size)
{
    return 0;
}


//Erase NRF and blink every 50ms in the process
static void nrf_Emergency_Erase()
{
    target_flash_init(BIN);
    target_flash_erase_chip();
//    //make sure SWD is initialized
//    if (!swd_init_debug()) {
//		return;
//	}
//
//    blinkLED();
//
//    //Set NVMC->CONFIG on NRF to 2
//    if (!swd_write_ap(AP_TAR, 0x4001E000 + 0x504)) {
//		return;
//	}
//    if (!swd_write_ap(AP_DRW, 2)) {
//		return;
//	}
//    blinkLED();
//    blinkLED();
//
//    //Set NVMC->ERASEALL on NRF to 1 to start chip erase
//    if (!swd_write_ap(AP_TAR, 0x4001E000 + 0x50C)) {
//		return;
//	}
//    if (!swd_write_ap(AP_DRW, 1)) {
//		return;
//	}
//
//    blinkLED();
//    blinkLED();
//    blinkLED();
//    blinkLED();
//
//    //Set NVMC->CONFIG on NRF to 0
//    if (!swd_write_ap(AP_TAR, 0x4001E000 + 0x504)) {
//		return;
//	}
//    if (!swd_write_ap(AP_DRW, 0)) {
//		return;
//	}
//
//    blinkLED();
//    blinkLED();
//	//swd_set_target_state(RESET_PROGRAM);
//	//target_flash_init(SystemCoreClock);
//	//target_flash_program_page(0,(uint8_t *)nrfBlinkyApp,800);
}

void target_before_init_debug(void)
{
    return;
}

uint8_t target_unlock_sequence(void)
{
    return 1;
}

uint8_t target_set_state(TARGET_RESET_STATE state)
{
    uint32_t  count = 0;

    //Check for 5 Second emergency erase routine
    while (!((PIOA->PIO_PDSR >> 25) & 1)) {
        os_dly_wait(1);
        count++;
        gpio_set_dap_led((count >> 4) & 1); //Blink every 160ms

        if (count > 500) {
            nrf_Emergency_Erase();
            return swd_set_target_state(state);
        }
    }

    gpio_set_dap_led(1);
    return swd_set_target_state(state);
}
