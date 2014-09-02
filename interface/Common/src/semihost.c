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
#ifdef SEMIHOST

#include <RTL.h>
#include <string.h>

#include "tasks.h"
#include "main.h"

#include "target_reset.h"
#include "version.h"
#include "swd_host.h"
#include "semihost.h"

#define RESERVED_FOR_USER_APPLICATIONS  (0x100) /* 0x100 - 0x1ff */
#define USR_UID                         (RESERVED_FOR_USER_APPLICATIONS + 1)
#define USR_RESET                       (RESERVED_FOR_USER_APPLICATIONS + 2)
#define USR_POWERDOWN                   (RESERVED_FOR_USER_APPLICATIONS + 4)
#define USR_DISABLEDEBUG                (RESERVED_FOR_USER_APPLICATIONS + 5)
#define angel_SWIreason_ReportException (0x18)

#define FLAGS_SH_START                  (1)
#define FLAGS_SH_STOP                   (2)

static OS_TID semihostTask;
static OS_SEM semihostStoppedSem;
static uint8_t semihostEnabled;
static uint32_t r0, r1;

static int shReadWord(uint32_t address, uint32_t *ptr) {
    return swd_read_memory(address, (uint8_t *)ptr, sizeof(uint32_t));
}

static int shWriteBytes(uint32_t address, uint8_t *ptr, uint32_t count) {
    return swd_write_memory(address, ptr, count);
}

static int sh_usr_uid(uint32_t *r0, uint32_t r1) {
    uint32_t p1, p2;
    uint32_t uidSize;
    uint32_t i;
    uint8_t null = '\0';
    uint8_t * id_str;

    // Read parameter block
    if (!shReadWord(r1, &p1))
        return 0;

    if (!shReadWord(r1+4, &p2))
        return 0;

    id_str = get_uid_string();
    uidSize = strlen((const char *)id_str) - 4;

    // Validate parameters
    if (p1 == NULL) return 1;
    if (p2 != (uidSize+1)) return 1; // +1 for null string terminator

    // skip " $ $ $ length"
    id_str += 4;

    // Write data to target
    for (i = 0; i < uidSize; i++) {
        if (!shWriteBytes(p1++, id_str, 1))
            return 0;
        id_str++;
    }
    // Write null terminator to target
    if (!shWriteBytes(p1, &null, 1))
        return 0;

    *r0 = 0; // Successful
    return 1;
}

static int sh_usr_reset(uint32_t *r0, uint32_t r1) {
    main_reset_target(0);

    // Don't restart target as the target will be resetted
    return 0;
}

static int sh_report_exception(uint32_t *r0, uint32_t r1) {
    uint32_t p1;

    // Read paramter block
    if (!shReadWord(r1, &p1)) return 0;

    // TODO: r1 does not appear to be set a to a useful value, contrary to the ARM documentation? See:
    // DUI0205G_rvct_compiler_and_libraries_guide.pdf 7.4.2 angel_SWIreason_ReportException (0x18)
    return 1;
}


static int sh_usr_powerdown(uint32_t *r0, uint32_t r1) {
    main_powerdown_event();

    // successful
    *r0 = 0;
    return 1;
}

static int sh_usr_disabledebug(uint32_t *r0, uint32_t r1) {
    main_disable_debug_event();

    // successful
    *r0 = 0;
    return 1;
}

static int process_event(void) {
    // Returns TRUE if successful, FALSE if an error occurs
    uint32_t svc;

    svc = r0;

    // Set default return value
    r0 = 0xffffffff;

    switch(svc) {
        case USR_UID:
            if (!sh_usr_uid(&r0,r1)) return 0;
            break;
        case USR_RESET:
            if (!sh_usr_reset(&r0,r1)) return 0;
            break;
        case USR_POWERDOWN:
            if (!sh_usr_powerdown(&r0,r1)) return 0;
            break;
        case USR_DISABLEDEBUG:
            if (!sh_usr_disabledebug(&r0,r1)) return 0;
            break;
        case angel_SWIreason_ReportException:
            if (!sh_report_exception(&r0,r1)) return 0;
            break;
        default:
            break;
    }

    return 1;
}

static void sh_main(void) {
    while(1) {
        // Wait for start flag
        os_evt_wait_or(FLAGS_SH_START, NO_TIMEOUT);

        do {
            if (swd_is_semihost_event(&r0, &r1)) {
                if (process_event()) {
                    swd_semihost_restart(r0);
                }
            } else {
                // Wait for 2 scheduler tick; the serial task may run at this point
                    os_dly_wait(2);
            }
        // Loop until stop flag is set
        } while (os_evt_wait_or(FLAGS_SH_STOP, 0 )==OS_R_TMO);

        // Stopped
        os_sem_send(semihostStoppedSem);
    }
}

void semihost_init(void) {
    // Called from main task

    semihostEnabled = 0;
    os_sem_init(semihostStoppedSem,0);

    // Create semihost task
    semihostTask = os_tsk_create(sh_main, SEMIHOST_TASK_PRIORITY);
    return;
}

void semihost_enable(void) {
    // Called from:
    //   - main task when the interface firmware starts
    //   - cmsis-dap when a debugger closes the swd port
    //   - drag n drop when a binary has been flashed

    if (semihostEnabled) return;
    if (semihostTask==0) return;

    // enable debug
    target_set_state(DEBUG);

    os_evt_set(FLAGS_SH_START, semihostTask);

    semihostEnabled = 1;

    return;
}

void semihost_disable(void) {
    // Called from:
    //   - main task when the the interface receives the POWERDOWN semihost call
    //   - cmsis-dap when a debugger opens the swd port
    //   - drag n drop when a binary will be flashed

    if (!semihostEnabled) return;
    if (semihostTask==0) return;

    os_evt_set(FLAGS_SH_STOP, semihostTask);

    // Wait for semihost task to stop
    os_sem_wait(semihostStoppedSem, NO_TIMEOUT);
    semihostEnabled = 0;
    return;
}

#else /* #ifndef SEMIHOST */
void semihost_init(void) { }
void semihost_enable(void) { }
void semihost_disable(void){ }
#endif
