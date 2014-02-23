/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "flash_erase_read_write.h"
#include "flash.h"
#include <string.h>

void initDisconnect(uint8_t success) 
{
//    drag_success = success;
//#if 0       // reset and run target
//    if (success) {
//        swd_set_target_state(RESET_RUN);
//    }
//#endif
//    main_blink_msd_led(0);
//    //init(1); //TODO - should be done when we start a connection rather then at the end of one
//    isr_evt_set(MSC_TIMEOUT_STOP_EVENT, msc_valid_file_timeout_task_id);
//    // event to disconnect the usb
//    main_usb_disconnect_event();
//    semihost_enable();
}

int jtag_init() {
//    if (DAP_Data.debug_port != DAP_PORT_DISABLED) {
//        need_restart_usb = 1;
//    }

//    if ((jtag_flash_init != 1) && (DAP_Data.debug_port == DAP_PORT_DISABLED)) {
//        if (need_restart_usb == 1) {
//            reason = SWD_PORT_IN_USE;
//            initDisconnect(0);
//            return 1;
//        }

//        semihost_disable();

//        PORT_SWD_SETUP();

//        target_set_state(RESET_PROGRAM);
//        if (!target_flash_init(SystemCoreClock)) {
//            failSWD();
//            return 1;
//        }

//        jtag_flash_init = 1;
//    }
    return 0;
}

uint8_t _flash_init(uint32_t clk)
{
		return flash_init(clk);
}

uint8_t _flash_uninit(void)
{
		return flash_uninit(0);
}

uint8_t _flash_erase_chip(void)
{
		return 0;
}

uint8_t _flash_erase_sector(uint32_t adr)
{
		return flash_erase_sector_svc(adr);
}

uint8_t _flash_program_page(uint32_t adr, uint8_t * buf, uint32_t size)
{
		return flash_program_page_svc(adr, size, buf);
}

uint32_t _read_memory(uint32_t address, uint8_t *data, uint32_t size)
{
		return (uint32_t)memcpy(data, (void *)address, size);
}
