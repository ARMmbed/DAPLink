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
#include <RTL.h>

#include "target_flash.h"
#include "target_reset.h"
#include "swd_host.h"
#include "debug_cm.h"
#include "DAP_config.h"
#include "DAP.h"

// Default NVIC and Core debug base addresses
// TODO: Read these addresses from ROM.
#define NVIC_Addr    (0xe000e000)
#define DBG_Addr     (0xe000edf0)

// AP CSW register, base value
#define CSW_VALUE (CSW_RESERVED | CSW_MSTRDBG | CSW_HPROT | CSW_DBGSTAT | CSW_SADDRINC)

// SWD register access
#define SWD_REG_AP        (1)
#define SWD_REG_DP        (0)
#define SWD_REG_R        (1<<1)
#define SWD_REG_W        (0<<1)
#define SWD_REG_ADR(a)    (a & 0x0c)

#define DCRDR 0xE000EDF8
#define DCRSR 0xE000EDF4
#define DHCSR 0xE000EDF0
#define REGWnR (1 << 16)

#define MAX_SWD_RETRY 10
#define MAX_TIMEOUT   10000  // Timeout for syscalls on target

// Some targets require a soft reset for flash programming (RESET_PROGRAM).
// Otherwise a hardware reset is the default. This will not affect
// DAP operations as they are controlled by the remote debugger.
#if defined(BOARD_BAMBINO_210) || defined(BOARD_BAMBINO_210E)
#define CONF_SYSRESETREQ
#elif defined(BOARD_LPC4337)
#define CONF_VECTRESET
#endif

#if defined(CONF_SYSRESETREQ)
// SYSRESETREQ - Software reset of the Cortex-M core and on-chip peripherals
#define SOFT_RESET  SYSRESETREQ

#elif defined(CONF_VECTRESET)
// VECTRESET - Software reset of Cortex-M core
// For some Cortex-M devices, VECTRESET is the only way to reset the core.
// VECTRESET is not supported on Cortex-M0 and Cortex-M1 cores.
#define SOFT_RESET  VECTRESET

#endif

typedef struct {
    uint32_t select;
    uint32_t csw;
} DAP_STATE;

typedef struct {
    uint32_t r[16];
    uint32_t xpsr;
} DEBUG_STATE;

static DAP_STATE dap_state;

static uint8_t swd_read_core_register(uint32_t n, uint32_t *val);
static uint8_t swd_write_core_register(uint32_t n, uint32_t val);

static void int2array(uint8_t * res, uint32_t data, uint8_t len) {
    uint8_t i = 0;
    for (i = 0; i < len; i++) {
        res[i] = (data >> 8*i) & 0xff;
    }
}

static uint8_t swd_transfer_retry(uint32_t req, uint32_t * data) {
    uint8_t i, ack;
    for (i = 0; i < MAX_SWD_RETRY; i++) {
        ack = SWD_Transfer(req, data);
        // if ack != WAIT
        if (ack != 0x02) {
            return ack;
        }
    }
    return ack;
}


uint8_t swd_init(void) {
    DAP_Setup();
    PORT_SWD_SETUP();
    return 1;
}

// Read debug port register.
uint8_t swd_read_dp(uint8_t adr, uint32_t *val) {
    uint32_t tmp_in;
    uint8_t tmp_out[4];
    uint8_t ack;

    tmp_in = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(adr);
    ack = swd_transfer_retry(tmp_in, (uint32_t *)tmp_out);

    *val = (tmp_out[3] << 24) | (tmp_out[2] << 16) | (tmp_out[1] << 8) | tmp_out[0];

    return (ack == 0x01);
}

// Write debug port register
uint8_t swd_write_dp(uint8_t adr, uint32_t val) {
    uint32_t req;
    uint8_t data[4];
    uint8_t ack;

    switch(adr) {
        case DP_SELECT:
            if (dap_state.select == val)
                return 1;
            dap_state.select = val;
            break;
        default:
            break;
    }

    req = SWD_REG_DP | SWD_REG_W | SWD_REG_ADR(adr);
    int2array(data, val, 4);

    ack = swd_transfer_retry(req, (uint32_t *)data);

    return (ack == 0x01);
}

// Read access port register.
uint8_t swd_read_ap(uint32_t adr, uint32_t *val) {
    uint8_t tmp_in, ack;
    uint8_t tmp_out[4];

    uint32_t apsel = adr & 0xff000000;
    uint32_t bank_sel = adr & APBANKSEL;

    if (!swd_write_dp(DP_SELECT, apsel | bank_sel)) {
        return 0;
    }

    tmp_in = SWD_REG_AP | SWD_REG_R | SWD_REG_ADR(adr);

    // first dummy read
    swd_transfer_retry(tmp_in, (uint32_t *)tmp_out);
    ack = swd_transfer_retry(tmp_in, (uint32_t *)tmp_out);

    *val = (tmp_out[3] << 24) | (tmp_out[2] << 16) | (tmp_out[1] << 8) | tmp_out[0];

    return (ack == 0x01);
}

// Write access port register
uint8_t swd_write_ap(uint32_t adr, uint32_t val) {
    uint8_t data[4];
    uint8_t req, ack;
    uint32_t apsel = adr & 0xff000000;
    uint32_t bank_sel = adr & APBANKSEL;

    if (!swd_write_dp(DP_SELECT, apsel | bank_sel)) {
        return 0;
    }

    switch(adr) {
        case AP_CSW:
            if (dap_state.csw == val)
                return 1;
            dap_state.csw = val;
            break;
        default:
            break;
    }

    req = SWD_REG_AP | SWD_REG_W | SWD_REG_ADR(adr);
    int2array(data, val, 4);

    if (swd_transfer_retry(req, (uint32_t *)data) != 0x01) {
        return 0;
    }

    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = swd_transfer_retry(req, NULL);

    return (ack == 0x01);
}


// Write 32-bit word aligned values to target memory using address auto-increment.
// size is in bytes.
static uint8_t swd_write_block(uint32_t address, uint8_t *data, uint32_t size) {
    uint8_t tmp_in[4], req;
    uint32_t size_in_words;
    uint32_t i, ack;

    if (size==0)
        return 0;

    size_in_words = size/4;

    if (!swd_write_ap(AP_CSW, CSW_VALUE | CSW_SIZE32)) {
        return 0;
    }

    // TAR write
    req = SWD_REG_AP | SWD_REG_W | (1 << 2);
    int2array(tmp_in, address, 4);
    if (swd_transfer_retry(req, (uint32_t *)tmp_in) != 0x01) {
        return 0;
    }

    // DRW write
    req = SWD_REG_AP | SWD_REG_W | (3 << 2);
    for (i = 0; i < size_in_words; i++) {
        if (swd_transfer_retry(req, (uint32_t *)data) != 0x01) {
            return 0;
        }
        data+=4;
    }

    // dummy read
    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = swd_transfer_retry(req, NULL);

    return (ack == 0x01);
}

// Read 32-bit word aligned values from target memory using address auto-increment.
// size is in bytes.
static uint8_t swd_read_block(uint32_t address, uint8_t *data, uint32_t size) {
    uint8_t tmp_in[4], req, ack;
    uint32_t size_in_words;
    uint32_t i;

    if (size == 0) {
        return 0;
    }

    size_in_words = size/4;

    if (!swd_write_ap(AP_CSW, CSW_VALUE | CSW_SIZE32)) {
        return 0;
    }

    // TAR write
    req = SWD_REG_AP | SWD_REG_W | (1 << 2);
    int2array(tmp_in, address, 4);
    if (swd_transfer_retry(req, (uint32_t *)tmp_in) != 0x01) {
        return 0;
    }

    // read data
    req = SWD_REG_AP | SWD_REG_R | (3 << 2);
    // dummy read
    if (swd_transfer_retry(req, (uint32_t *)data) != 0x01) {
        return 0;
    }

    for (i = 0; i< size_in_words; i++) {
        if (swd_transfer_retry(req, (uint32_t *)data) != 0x01) {
            return 0;
        }
        data += 4;
    }

    // dummy read
    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = swd_transfer_retry(req, NULL);

    return (ack == 0x01);
}

// Read target memory.
static uint8_t swd_read_data(uint32_t addr, uint32_t *val) {
    uint8_t tmp_in[4];
    uint8_t tmp_out[4];
    uint8_t req, ack;

    // put addr in TAR register
    int2array(tmp_in, addr, 4);
    req = SWD_REG_AP | SWD_REG_W | (1 << 2);
    if (swd_transfer_retry(req, (uint32_t *)tmp_in) != 0x01) {
        return 0;
    }

    // read data
    req = SWD_REG_AP | SWD_REG_R | (3 << 2);
    if (swd_transfer_retry(req, (uint32_t *)tmp_out) != 0x01) {
        return 0;
    }

    // dummy read
    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = swd_transfer_retry(req, (uint32_t *)tmp_out);

    *val = (tmp_out[3] << 24) | (tmp_out[2] << 16) | (tmp_out[1] << 8) | tmp_out[0];

    return (ack == 0x01);
}

// Write target memory.
static uint8_t swd_write_data(uint32_t address, uint32_t data) {
    uint8_t tmp_in[4];
    uint8_t req, ack;

    // put addr in TAR register
    int2array(tmp_in, address, 4);
    req = SWD_REG_AP | SWD_REG_W | (1 << 2);
    if (swd_transfer_retry(req, (uint32_t *)tmp_in) != 0x01) {
        return 0;
    }

    // write data
    int2array(tmp_in, data, 4);
    req = SWD_REG_AP | SWD_REG_W | (3 << 2);
    if (swd_transfer_retry(req, (uint32_t *)tmp_in) != 0x01) {
        return 0;
    }

    // dummy read
    req = SWD_REG_DP | SWD_REG_R | SWD_REG_ADR(DP_RDBUFF);
    ack = swd_transfer_retry(req, NULL);

    return (ack == 0x01) ? 1 : 0;
}

// Read 32-bit word from target memory.
static uint8_t swd_read_word(uint32_t addr, uint32_t *val) {
    if (!swd_write_ap(AP_CSW, CSW_VALUE | CSW_SIZE32)) {
        return 0;
    }

    if (!swd_read_data(addr, val)) {
        return 0;
    }

    return 1;
}

// Write 32-bit word to target memory.
static uint8_t swd_write_word(uint32_t addr, uint32_t val) {
    if (!swd_write_ap(AP_CSW, CSW_VALUE | CSW_SIZE32)) {
        return 0;
    }

    if (!swd_write_data(addr, val)) {
        return 0;
    }

    return 1;
}

// Read 8-bit byte from target memory.
static uint8_t swd_read_byte(uint32_t addr, uint8_t *val) {
    uint32_t tmp;
    if (!swd_write_ap(AP_CSW, CSW_VALUE | CSW_SIZE8)) {
        return 0;
    }

    if (!swd_read_data(addr, &tmp)) {
        return 0;
    }

    *val = (uint8_t)(tmp >> ((addr & 0x03) << 3));
    return 1;
}

// Write 8-bit byte to target memory.
static uint8_t swd_write_byte(uint32_t addr, uint8_t val) {
    uint32_t tmp;

    if (!swd_write_ap(AP_CSW, CSW_VALUE | CSW_SIZE8)) {
        return 0;
    }

    tmp = val << ((addr & 0x03) << 3);
    if (!swd_write_data(addr, tmp)) {
        return 0;
    }

    return 1;
}

// Read unaligned data from target memory.
// size is in bytes.
uint8_t swd_read_memory(uint32_t address, uint8_t *data, uint32_t size) {
    uint32_t n;

    // Read bytes until word aligned
    while ((size > 0) && (address & 0x3)) {
        if (!swd_read_byte(address, data)) {
            return 0;
        }
        address++;
        data++;
        size--;
    }

    // Read word aligned blocks
    while (size > 3) {
        // Limit to auto increment page size
        n = TARGET_AUTO_INCREMENT_PAGE_SIZE - (address & (TARGET_AUTO_INCREMENT_PAGE_SIZE - 1));
        if (size < n) {
            n = size & 0xFFFFFFFC; // Only count complete words remaining
        }

        if (!swd_read_block(address, data, n)) {
            return 0;
        }

        address += n;
        data += n;
        size -= n;
    }

    // Read remaining bytes
    while (size > 0) {
        if (!swd_read_byte(address, data)) {
            return 0;
        }
        address++;
        data++;
        size--;
    }

    return 1;
}

// Write unaligned data to target memory.
// size is in bytes.
uint8_t swd_write_memory(uint32_t address, uint8_t *data, uint32_t size) {
    uint32_t n;
    // Write bytes until word aligned
    while ((size > 0) && (address & 0x3)) {
        if (!swd_write_byte(address, *data)) {
            return 0;
        }
        address++;
        data++;
        size--;
    }

    // Write word aligned blocks
    while (size > 3) {
        // Limit to auto increment page size
        n = TARGET_AUTO_INCREMENT_PAGE_SIZE - (address & (TARGET_AUTO_INCREMENT_PAGE_SIZE - 1));
        if (size < n) {
            n = size & 0xFFFFFFFC; // Only count complete words remaining
        }

        if (!swd_write_block(address, data, n)) {
            return 0;
        }

        address += n;
        data += n;
        size -= n;
    }

    // Write remaining bytes
    while (size > 0) {
        if (!swd_write_byte(address, *data)) {
            return 0;
        }
        address++;
        data++;
        size--;
    }

    return 1;
}

// Execute system call.
static uint8_t swd_write_debug_state(DEBUG_STATE *state) {
    uint32_t i, status;

    if (!swd_write_dp(DP_SELECT, 0)) {
        return 0;
    }

    // R0, R1, R2, R3
    for (i = 0; i < 4; i++) {
        if (!swd_write_core_register(i, state->r[i])) {
            return 0;
        }
    }

    // R9
    if (!swd_write_core_register(9, state->r[9])) {
        return 0;
    }

    // R13, R14, R15
    for (i=13; i<16; i++) {
        if (!swd_write_core_register(i, state->r[i])) {
            return 0;
        }
    }

    // xPSR
    if (!swd_write_core_register(16, state->xpsr)) {
        return 0;
    }

    if (!swd_write_word(DBG_HCSR, DBGKEY | C_DEBUGEN)) {
        return 0;
    }

    // check status
    if (!swd_read_dp(DP_CTRL_STAT, &status)){
        return 0;
    }

    if (status & (STICKYERR | WDATAERR)) {
        return 0;
    }

    return 1;
}

static uint8_t swd_read_core_register(uint32_t n, uint32_t *val) {
    int i = 0, timeout = 100;
    if (!swd_write_word(DCRSR, n)) {
        return 0;
    }

    // wait for S_REGRDY
    for (i = 0; i < timeout; i++) {

        if (!swd_read_word(DHCSR, val)) {
            return 0;
        }

        if (*val & S_REGRDY) {
            break;
        }
    }

    if (i == timeout) {
        return 0;
    }

    if (!swd_read_word(DCRDR, val)) {
        return 0;
    }

    return 1;
}

static uint8_t swd_write_core_register(uint32_t n, uint32_t val) {
    int i = 0, timeout = 100;
    if (!swd_write_word(DCRDR, val))
        return 0;

    if (!swd_write_word(DCRSR, n | REGWnR)) {
        return 0;
    }

    // wait for S_REGRDY
    for (i = 0; i < timeout; i++) {

        if (!swd_read_word(DHCSR, &val)) {
            return 0;
        }

        if (val & S_REGRDY) {
            return 1;
        }
    }

    return 0;
}

uint8_t swd_is_semihost_event(uint32_t *r0, uint32_t *r1) {
    uint32_t val;

    if (!swd_read_word(DBG_HCSR, &val)) {
        return 0;
    }

    // Not hit breakpoint
    if ((val & S_HALT) == 0) {
        return 0;
    }

    // Has hit breakpoint
    // Read r0 and r1
    if (!swd_read_core_register(0, r0)) {
        return 0;
    }

    if (!swd_read_core_register(1, r1)) {
        return 0;
    }

    return 1;
}

static uint8_t swd_wait_until_halted(void) {
    // Wait for target to stop
    uint32_t val, i, timeout = MAX_TIMEOUT;
    for (i = 0; i < timeout; i++) {

        if (!swd_read_word(DBG_HCSR, &val)) {
            return 0;
        }

        if (val & S_HALT) {
            return 1;
        }
    }
    return 0;
}

// Restart target after BKPT
uint8_t swd_semihost_restart(uint32_t r0) {
    uint32_t pc;

    // Update r0
    if (!swd_write_core_register(0, r0)) {
        return 0;
    }

    // Update PC
    if (!swd_read_core_register(15, &pc)) {
        return 0;
    }

    if (!swd_write_core_register(15, pc + 2)) {
        return 0;
    }

    // Restart
    if (!swd_write_word(DBG_HCSR, DBGKEY | C_DEBUGEN)) {
        return 0;
    }

    return 1;
}

uint8_t swd_flash_syscall_exec(const FLASH_SYSCALL *sysCallParam, uint32_t entry, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4) {
    DEBUG_STATE state;

    // Call flash algorithm function on target and wait for result.
    state.xpsr     = 0x01000000;          // xPSR: T = 1, ISR = 0
    state.r[0]     = arg1;                   // R0: Argument 1
    state.r[1]     = arg2;                   // R1: Argument 2
    state.r[2]     = arg3;                   // R2: Argument 3
    state.r[3]     = arg4;                   // R3: Argument 4

    state.r[9]     = sysCallParam->static_base;    // SB: Static Base

    state.r[13]    = sysCallParam->stack_pointer;  // SP: Stack Pointer
    state.r[14]    = sysCallParam->breakpoint;       // LR: Exit Point
    state.r[15]    = entry;                           // PC: Entry Point

    if (!swd_write_debug_state(&state)) {
        return 0;
    }

    if (!swd_wait_until_halted()) {
        return 0;
    }

    if (!swd_read_core_register(0, &state.r[0])) {
        return 0;
    }

    // Flash functions return 0 if successful.
    if (state.r[0] != 0) {
        return 0;
    }

    return 1;
}

// SWD Reset
static uint8_t swd_reset(void) {
    uint8_t tmp_in[8];
    uint8_t i = 0;
    for (i = 0; i < 8; i++) {
        tmp_in[i] = 0xff;
    }

    SWJ_Sequence(51, tmp_in);

    return 1;
}

// SWD Switch
static uint8_t swd_switch(uint16_t val) {
    uint8_t tmp_in[2];

    tmp_in[0] = val & 0xff;
    tmp_in[1] = (val >> 8) & 0xff;

    SWJ_Sequence(16, tmp_in);

    return 1;
}

// SWD Read ID
static uint8_t swd_read_idcode(uint32_t *id) {
    uint8_t tmp_in[1];
    uint8_t tmp_out[4];

    tmp_in[0] = 0x00;

    SWJ_Sequence(8, tmp_in);

    if (swd_read_dp(0, (uint32_t *)tmp_out) != 0x01) {
        return 0;
    }

    *id = (tmp_out[3] << 24) | (tmp_out[2] << 16) | (tmp_out[1] << 8) | tmp_out[0];

    return 1;
}


static uint8_t JTAG2SWD() {
    uint32_t tmp = 0;

    if (!swd_reset()) {
        return 0;
    }

    if (!swd_switch(0xE79E)) {
        return 0;
    }

    if (!swd_reset()) {
        return 0;
    }

    if (!swd_read_idcode(&tmp)) {
        return 0;
    }

    return 1;
}

static uint8_t swd_init_debug(void) {
    uint32_t tmp = 0;

    // init dap state with fake values
    dap_state.select = 0xffffffff;
    dap_state.csw = 0xffffffff;

    DAP_Setup();
    PORT_SWD_SETUP();

    // call a target dependant function
    // this function can do several stuff before really
    // initing the debug
    target_before_init_debug();

    if (!JTAG2SWD()) {
        return 0;
    }

    if (!swd_write_dp(DP_ABORT, STKCMPCLR | STKERRCLR | WDERRCLR | ORUNERRCLR)) {
        return 0;
    }

    // Ensure CTRL/STAT register selected in DPBANKSEL
    if (!swd_write_dp(DP_SELECT, 0)) {
        return 0;
    }

    // Power up
    if (!swd_write_dp(DP_CTRL_STAT, CSYSPWRUPREQ | CDBGPWRUPREQ)) {
        return 0;
    }

    do {
        if (!swd_read_dp(DP_CTRL_STAT, &tmp)) {
            return 0;
        }
    } while ((tmp & (CDBGPWRUPACK | CSYSPWRUPACK)) != (CDBGPWRUPACK | CSYSPWRUPACK));

    if (!swd_write_dp(DP_CTRL_STAT, CSYSPWRUPREQ | CDBGPWRUPREQ | TRNNORMAL | MASKLANE)) {
        return 0;
    }

    // call a target dependant function:
    // some target can enter in a lock state
    // this function can unlock these targets
    target_unlock_sequence();

    if (!swd_write_dp(DP_SELECT, 0)) {
        return 0;
    }

    return 1;
}


void swd_set_target_reset(uint8_t asserted) {
    if (asserted) {
        PIN_nRESET_OUT(0);
    } else {
        PIN_nRESET_OUT(1);
    }
}

uint8_t swd_set_target_state(TARGET_RESET_STATE state) {
    uint32_t val;
    switch (state) {
        case RESET_HOLD:
            swd_set_target_reset(1);
            break;

        case RESET_RUN:
            swd_set_target_reset(1);
            os_dly_wait(2);

            swd_set_target_reset(0);
            os_dly_wait(2);
            break;

        case RESET_RUN_WITH_DEBUG:
            // First reset
            swd_set_target_reset(1);
            os_dly_wait(1);

            swd_set_target_reset(0);
            os_dly_wait(1);

            if (!swd_init_debug()) {
                return 0;
            }

            // Enable debug (for semihost)
            if (!swd_write_word(DBG_HCSR, DBGKEY | C_DEBUGEN)) {
                return 0;
            }

            // Reset again
            swd_set_target_reset(1);
            os_dly_wait(1);

            swd_set_target_reset(0);
            os_dly_wait(1);
            break;

        case RESET_PROGRAM:
#if !defined(SOFT_RESET)
            // Use hardware reset (HW RESET)
            // First reset
            swd_set_target_reset(1);
            os_dly_wait(2);

            swd_set_target_reset(0);
            os_dly_wait(2);

            if (!swd_init_debug()) {
                return 0;
            }

            // Enable debug
            if (!swd_write_word(DBG_HCSR, DBGKEY | C_DEBUGEN)) {
                return 0;
            }

            // Enable halt on reset
            if (!swd_write_word(DBG_EMCR, VC_CORERESET)) {
                return 0;
            }

            // Reset again
            swd_set_target_reset(1);
            os_dly_wait(2);

            swd_set_target_reset(0);
#else            
            if (!swd_init_debug()) {
                return 0;
            }

            // Enable debug and halt the core (DHCSR <- 0xA05F0003)
            if (!swd_write_word(DBG_HCSR, DBGKEY | C_DEBUGEN | C_HALT)) {
                return 0;
            }
            
            // Wait until core is halted
            do {
                if (!swd_read_word(DBG_HCSR, &val)) {
                    return 0;
                }
            } while((val & S_HALT) == 0);

            // Enable halt on reset
            if (!swd_write_word(DBG_EMCR, VC_CORERESET)) {
                return 0;
            }

	        // Perform a soft reset
            if (!swd_write_word(NVIC_AIRCR, VECTKEY | SOFT_RESET)) {
                return 0;
            }
#endif
            os_dly_wait(2);

            do {
                if (!swd_read_word(DBG_HCSR, &val)) {
                    return 0;
                }
            } while((val & S_HALT) == 0);

            // Disable halt on reset
            if (!swd_write_word(DBG_EMCR, 0)) {
                return 0;
            }

            break;

        case NO_DEBUG:
            if (!swd_write_word(DBG_HCSR, DBGKEY)) {
                return 0;
            }
            break;

        case DEBUG:
            DAP_Setup();
            PORT_SWD_SETUP();

            if (!JTAG2SWD()) {
                return 0;
            }

            if (!swd_write_dp(DP_ABORT, STKCMPCLR | STKERRCLR | WDERRCLR | ORUNERRCLR)) {
                return 0;
            }

            // Ensure CTRL/STAT register selected in DPBANKSEL
            if (!swd_write_dp(DP_SELECT, 0)) {
                return 0;
            }

            // Power up
            if (!swd_write_dp(DP_CTRL_STAT, CSYSPWRUPREQ | CDBGPWRUPREQ)) {
                return 0;
            }

            // Enable debug (for semihost)
            if (!swd_write_word(DBG_HCSR, DBGKEY | C_DEBUGEN)) {
                return 0;
            }

            break;

        default:
            return 0;
    }
    return 1;
}
