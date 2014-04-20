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
#include "read_uid.h"

#define IAP_LOCATION 0x1fff1ff1
static uint32_t command[5];
static uint32_t result[5];
typedef void (*IAP)(uint32_t[], uint32_t[]);
static IAP iap_entry;


void read_unique_id(uint32_t * id) {
    // readUID IAP call
    iap_entry = (IAP) IAP_LOCATION;
    command[0] = 58;
    iap_entry (command, result);
    *id = result[1] ^ result[2] ^ result[3] ^ result[4];
}

#ifdef BOARD_UBLOX_C027
#include "LPC11Uxx.h"
void __SVC_2 (void)
{
    // taken code from the Nxp App Note AN11305 
    // the calls to the usb stack are removed
    iap_entry = (IAP) IAP_LOCATION;
    
    /* make sure USB clock is turned on before calling ISP */
    LPC_SYSCON->SYSAHBCLKCTRL |= 0x04000;
    /* make sure 32-bit Timer 1 is turned on before calling ISP */
    LPC_SYSCON->SYSAHBCLKCTRL |= 0x00400;
    /* make sure GPIO clock is turned on before calling ISP */
    LPC_SYSCON->SYSAHBCLKCTRL |= 0x00040;
    /* make sure IO configuration clock is turned on before calling ISP */
    LPC_SYSCON->SYSAHBCLKCTRL |= 0x10000;

    /* make sure AHB clock divider is 1:1 */
    LPC_SYSCON->SYSAHBCLKDIV = 1;

    /* Send Reinvoke ISP command to ISP entry point*/
    command[0] = 57;
    
    /* Set stack pointer to ROM value (reset default) This must be the last
     piece of code executed before calling ISP, because most C expressions
     and function returns will fail after the stack pointer is changed. */
    __set_MSP(*((uint32_t *)0x00000000));

    /* Enter ISP. We call "iap_entry" to enter ISP because the ISP entry is done
     through the same command interface as IAP. */
    iap_entry(command, result);
    // Not supposed to come back!
}
#endif
