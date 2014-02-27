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
#include <rl_usb.h>
#include <string.h>
#if defined(TARGET_LPC11U35)
#include <LPC11Uxx.h>
#elif defined(TARGET_MK20DX)
#include <MK20D5.h>
#endif


#include "tasks.h"
#include "flash.h"
#include "main.h"
#include "gpio.h"


#ifdef LPC11U35
// for __at attribute for lpc11u35
#include <absacc.h>
#endif

typedef struct sector {
    const char * sect;
    unsigned int length;
} SECTOR;

const char mbr[] = {
    //sector 0: boot sector
    0xEB,0x3C,0x90,0x4D,0x53,0x44,0x4F,0x53,0x35,0x2E,0x30,    0x00,0x02,/*bytes per sector: 512*/   0x01, /*sectors per cluster*/ 0x01,0x00,/*number of reserved sector*/
    0x02, /* number of FATs*/ 0x20,0x00, /*Maximum number of root directory entries: 32*/ 0x61,0x00, /*Total sector count: 1024*/ 0xF0,  0x01,0x00,/*sectors per FAT: 1*/  0x01,0x00,/*Sectors per track*/  0x01,0x00, /*Number of heads*/  0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,   0x00,    0x00,  0x29, /*boot signature*/ 0x74,0x19,0x02,0x27, /*volume ID*/ 'M','b','e','d',' ',
    'U','S','B',' ',' ',' ', /*volume label: Mbed USB*/     0x46,0x41,0x54,0x31,0x32,0x20,0x20,0x20,0x33,0xC9,
    0x8E,0xD1,0xBC,0xF0,0x7B,0x8E,0xD9,0xB8,0x00,0x20,0x8E,0xC0,0xFC,0xBD,0x00,0x7C,
    0x38,0x4E,0x24,0x7D,0x24,0x8B,0xC1,0x99,0xE8,0x3C,0x01,0x72,0x1C,0x83,0xEB,0x3A,
    0x66,0xA1,0x1C,0x7C,0x26,0x66,0x3B,0x07,0x26,0x8A,0x57,0xFC,0x75,0x06,0x80,0xCA,
    0x02,0x88,0x56,0x02,0x80,0xC3,0x10,0x73,0xEB,0x33,0xC9,0x8A,0x46,0x10,0x98,0xF7,
    0x66,0x16,0x03,0x46,0x1C,0x13,0x56,0x1E,0x03,0x46,0x0E,0x13,0xD1,0x8B,0x76,0x11,
    0x60,0x89,0x46,0xFC,0x89,0x56,0xFE,0xB8,0x20,0x00,0xF7,0xE6,0x8B,0x5E,0x0B,0x03,
    0xC3,0x48,0xF7,0xF3,0x01,0x46,0xFC,0x11,0x4E,0xFE,0x61,0xBF,0x00,0x00,0xE8,0xE6,
    0x00,0x72,0x39,0x26,0x38,0x2D,0x74,0x17,0x60,0xB1,0x0B,0xBE,0xA1,0x7D,0xF3,0xA6,
    0x61,0x74,0x32,0x4E,0x74,0x09,0x83,0xC7,0x20,0x3B,0xFB,0x72,0xE6,0xEB,0xDC,0xA0,
    0xFB,0x7D,0xB4,0x7D,0x8B,0xF0,0xAC,0x98,0x40,0x74,0x0C,0x48,0x74,0x13,0xB4,0x0E,
    0xBB,0x07,0x00,0xCD,0x10,0xEB,0xEF,0xA0,0xFD,0x7D,0xEB,0xE6,0xA0,0xFC,0x7D,0xEB,
    0xE1,0xCD,0x16,0xCD,0x19,0x26,0x8B,0x55,0x1A,0x52,0xB0,0x01,0xBB,0x00,0x00,0xE8,
    0x3B,0x00,0x72,0xE8,0x5B,0x8A,0x56,0x24,0xBE,0x0B,0x7C,0x8B,0xFC,0xC7,0x46,0xF0,
    0x3D,0x7D,0xC7,0x46,0xF4,0x29,0x7D,0x8C,0xD9,0x89,0x4E,0xF2,0x89,0x4E,0xF6,0xC6,
    0x06,0x96,0x7D,0xCB,0xEA,0x03,0x00,0x00,0x20,0x0F,0xB6,0xC8,0x66,0x8B,0x46,0xF8,
    0x66,0x03,0x46,0x1C,0x66,0x8B,0xD0,0x66,0xC1,0xEA,0x10,0xEB,0x5E,0x0F,0xB6,0xC8,
    0x4A,0x4A,0x8A,0x46,0x0D,0x32,0xE4,0xF7,0xE2,0x03,0x46,0xFC,0x13,0x56,0xFE,0xEB,
    0x4A,0x52,0x50,0x06,0x53,0x6A,0x01,0x6A,0x10,0x91,0x8B,0x46,0x18,0x96,0x92,0x33,
    0xD2,0xF7,0xF6,0x91,0xF7,0xF6,0x42,0x87,0xCA,0xF7,0x76,0x1A,0x8A,0xF2,0x8A,0xE8,
    0xC0,0xCC,0x02,0x0A,0xCC,0xB8,0x01,0x02,0x80,0x7E,0x02,0x0E,0x75,0x04,0xB4,0x42,
    0x8B,0xF4,0x8A,0x56,0x24,0xCD,0x13,0x61,0x61,0x72,0x0B,0x40,0x75,0x01,0x42,0x03,
    0x5E,0x0B,0x49,0x75,0x06,0xF8,0xC3,0x41,0xBB,0x00,0x00,0x60,0x66,0x6A,0x00,0xEB,
    0xB0,0x4E,0x54,0x4C,0x44,0x52,0x20,0x20,0x20,0x20,0x20,0x20,0x0D,0x0A,0x52,0x65,
    0x6D,0x6F,0x76,0x65,0x20,0x64,0x69,0x73,0x6B,0x73,0x20,0x6F,0x72,0x20,0x6F,0x74,
    0x68,0x65,0x72,0x20,0x6D,0x65,0x64,0x69,0x61,0x2E,0xFF,0x0D,0x0A,0x44,0x69,0x73,
    0x6B,0x20,0x65,0x72,0x72,0x6F,0x72,0xFF,0x0D,0x0A,0x50,0x72,0x65,0x73,0x73,0x20,
    0x61,0x6E,0x79,0x20,0x6B,0x65,0x79,0x20,0x74,0x6F,0x20,0x72,0x65,0x73,0x74,0x61,
    0x72,0x74,0x0D,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xAC,0xCB,0xD8,0x55,0xAA,
};

const char fat1[] = {
    0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

const char fat2[] = {0};

const char fail[] = {   'F', 'A', 'I', 'L', 0x20, 0x20, 0x20, 0x20, 'T', 'X', 'T', 0x20, 0x18, 0xB1, 0x74, 0x76,
                        0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x8E, 0x76, 0x8E, 0x41, 0x6, 0x0, 0x7, 0x0, 0x0, 0x0};

const char root_dir1[] = {
    // sector3: root dir
    'M', 'B', 'E', 'D', ' ', 'L', 'O', 'A', 'D', 'E', 'R', 0x28, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x85, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    // support for mac...
    0x41, 0x2E, 0x0, 0x66, 0x0, 0x73, 0x0, 0x65, 0x0, 0x76, 0x0, 0xF, 0x0, 0xDA, 0x65, 0x0,
    0x6E, 0x0, 0x74, 0x0, 0x73, 0x0, 0x64, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF,

    0x46, 0x53, 0x45, 0x56, 0x45, 0x4E, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x12, 0x0, 0x47, 0x7D, 0x75,
    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x7D, 0x75, 0x8E, 0x41, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0,

    0x42, 0x65, 0x0, 0x72, 0x0, 0x5F, 0x0, 0x69, 0x0, 0x6E, 0x0, 0xF, 0x0, 0xA8, 0x64, 0x0,
    0x65, 0x0, 0x78, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF,

    0x1, 0x2E, 0x0, 0x6D, 0x0, 0x65, 0x0, 0x74, 0x0, 0x61, 0x0, 0xF, 0x0, 0xA8, 0x64, 0x0,
    0x61, 0x0, 0x74, 0x0, 0x61, 0x0, 0x5F, 0x0, 0x6E, 0x0, 0x0, 0x0, 0x65, 0x0, 0x76, 0x0,

    0x4D, 0x45, 0x54, 0x41, 0x44, 0x41, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x22, 0x0, 0x32, 0x85, 0x75,
    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x85, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    0x41, 0x2E, 0x0, 0x54, 0x0, 0x72, 0x0, 0x61, 0x0, 0x73, 0x0, 0xF, 0x0, 0x25, 0x68, 0x0,
    0x65, 0x0, 0x73, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF,

    0x54, 0x52, 0x41, 0x53, 0x48, 0x45, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x22, 0x0, 0x32, 0x85, 0x75,
    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x85, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,

    // mbed html file
    'M', 'B', 'E', 'D', '_', 'B', 'L', 0x20, 'H', 'T', 'M', 0x20, 0x18, 0xB1, 0x74, 0x76,
    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x8E, 0x76, 0x8E, 0x41, 0x3, 0x0, 0x00, 0x01, 0x0, 0x0,

    // Hidden files to support windows 8.1
    0x42, 0x20, 0x00, 0x49, 0x00, 0x6E, 0x00, 0x66, 0x00, 0x6F, 0x00, 0x0F, 0x00, 0x72, 0x72, 0x00,
    0x6D, 0x00, 0x61, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6F, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x00, 0x00,

    0x01, 0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x0F, 0x00, 0x72, 0x6D, 0x00,
    0x20, 0x00, 0x56, 0x00, 0x6F, 0x00, 0x6C, 0x00, 0x75, 0x00, 0x00, 0x00, 0x6D, 0x00, 0x65, 0x00,

    0x53, 0x59, 0x53, 0x54, 0x45, 0x4D, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x16, 0x00, 0xA5, 0x85, 0x8A,
    0x73, 0x43, 0x73, 0x43, 0x00, 0x00, 0x86, 0x8A, 0x73, 0x43, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const char root_dir2[] = {0};

const char sect5[] = {
    0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x32, 0x0, 0x47, 0x7D, 0x75,
    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x88, 0x75, 0x8E, 0x41, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x2E, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x0, 0x47, 0x7D, 0x75,
    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x7D, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x4E, 0x4F, 0x5F, 0x4C, 0x4F, 0x47, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x8, 0x32, 0x85, 0x75,
    0x8E, 0x41, 0x8E, 0x41, 0x0, 0x0, 0x85, 0x75, 0x8E, 0x41, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

static const char sect6[] = {
    0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x00, 0xA5, 0x85, 0x8A,
    0x73, 0x43, 0x73, 0x43, 0x00, 0x00, 0x86, 0x8A, 0x73, 0x43, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x2E, 0x2E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x00, 0xA5, 0x85, 0x8A,
    0x73, 0x43, 0x73, 0x43, 0x00, 0x00, 0x86, 0x8A, 0x73, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // IndexerVolumeGuid (size0, cluster 0)
    0x42, 0x47, 0x00, 0x75, 0x00, 0x69, 0x00, 0x64, 0x00, 0x00, 0x00, 0x0F, 0x00, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,

    0x01, 0x49, 0x00, 0x6E, 0x00, 0x64, 0x00, 0x65, 0x00, 0x78, 0x00, 0x0F, 0x00, 0xFF, 0x65, 0x00,
    0x72, 0x00, 0x56, 0x00, 0x6F, 0x00, 0x6C, 0x00, 0x75, 0x00, 0x00, 0x00, 0x6D, 0x00, 0x65, 0x00,

    0x49, 0x4E, 0x44, 0x45, 0x58, 0x45, 0x7E, 0x31, 0x20, 0x20, 0x20, 0x20, 0x00, 0xA7, 0x85, 0x8A,
    0x73, 0x43, 0x73, 0x43, 0x00, 0x00, 0x86, 0x8A, 0x73, 0x43, 0x05, 0x00, 0x4C, 0x00, 0x00, 0x00
};

static const char sect7[] = {
	0x7B, 0x00, 0x39, 0x00, 0x36, 0x00, 0x36, 0x00, 0x31, 0x00, 0x39, 0x00, 0x38, 0x00, 0x32, 0x00,
	0x30, 0x00, 0x2D, 0x00, 0x37, 0x00, 0x37, 0x00, 0x44, 0x00, 0x31, 0x00, 0x2D, 0x00, 0x34, 0x00,
	0x46, 0x00, 0x38, 0x00, 0x38, 0x00, 0x2D, 0x00, 0x38, 0x00, 0x46, 0x00, 0x35, 0x00, 0x33, 0x00,
	0x2D, 0x00, 0x36, 0x00, 0x32, 0x00, 0x44, 0x00, 0x39, 0x00, 0x37, 0x00, 0x46, 0x00, 0x35, 0x00,
	0x46, 0x00, 0x34, 0x00, 0x46, 0x00, 0x46, 0x00, 0x39, 0x00, 0x7D, 0x00, 0x00, 0x00, 0x00, 0x00
};


uint32_t BlockBuf[1024/4];

//3 sectors
SECTOR sectors[9] = {
    {mbr , 512},
    {fat1, 11},
    {fat2, 0},
    {root_dir1, sizeof(root_dir1)},
    {root_dir2, 0},

    // support for mac
    {sect5, 6*16},

    // contains mbed.htm
    {(const char *)((const char *)BlockBuf + 512), 512},

    // support for window 8.1
    {sect6, sizeof(sect6)},

    {sect7, sizeof(sect7)},

};


static uint32_t size;
static uint32_t nb_sector;
static uint32_t current_sector;
static uint8_t sector_received_first;
static uint8_t root_dir_received_first;
static uint32_t jtag_flash_init;
static uint32_t flashPtr = START_APP_ADDRESS;
static uint8_t need_restart_usb;
static uint8_t flash_started;
static uint32_t start_sector;
static uint32_t theoretical_start_sector = 7;
static uint8_t msc_event_timeout;
static uint8_t good_file;
static uint8_t program_page_error;
static uint8_t maybe_erase;
static uint32_t previous_sector;
static uint32_t begin_sector;
static uint8_t task_first_started;
static uint8_t listen_msc_isr = 1;
static uint8_t drag_success = 1;
static uint8_t reason = 0;

#define SWD_ERROR 0
#define BAD_EXTENSION_FILE 1
#define NOT_CONSECUTIVE_SECTORS 2
#define SWD_PORT_IN_USE 3
#define RESERVED_BITS 4
#define BAD_START_SECTOR 5
#define TIMEOUT 6

static uint8_t * reason_array[] = {
    "SWD ERROR",
    "BAD EXTENSION FILE",
    "NOT CONSECUTIVE SECTORS",
    "SWD PORT IN USE",
    "RESERVED BITS",
    "BAD START SECTOR",
    "TIMEOUT",
};

static U64 msc_task_stack[MSC_TASK_STACK/8];

// Reference to the msc task
static OS_TID msc_valid_file_timeout_task_id;
static OS_TID flash_programming_task_id;

static void init(uint8_t jtag);
static void initDisconnect(uint8_t success);


#define MSC_TIMEOUT_SPLIT_FILES_EVENT   (0x1000)
#define MSC_TIMEOUT_START_EVENT         (0x2000)
#define MSC_TIMEOUT_STOP_EVENT          (0x4000)

#define PROGRAM_PAGE_EVENT              (0x0008)
#define FLASH_INIT_EVENT                (0x0004)

// 30 s timeout
#define TIMEOUT_S 3000

static void init(unsigned char jtag) {
    size = 0;
    nb_sector = 0;
    current_sector = 0;
    if (jtag) {
        jtag_flash_init = 0;
        theoretical_start_sector = (drag_success) ? 7 : 8;
        good_file = 0;
        program_page_error = 0;
        maybe_erase = 0;
        previous_sector = 0;
    }
    begin_sector = 0;
    flashPtr = START_APP_ADDRESS;
    sector_received_first = 0;
    root_dir_received_first = 0;
    need_restart_usb = 0;
    flash_started = 0;
    start_sector = 0;
    msc_event_timeout = 0;
    USBD_MSC_BlockBuf   = (uint8_t *)BlockBuf;
    listen_msc_isr = 1;
}

static void initDisconnect(uint8_t success) {
    drag_success = success;
    init(1);
    main_transfer_finished(success);
    isr_evt_set(MSC_TIMEOUT_STOP_EVENT, msc_valid_file_timeout_task_id);
}

static void disable_usb_irq(void){
#if defined(TARGET_LPC11U35)
    NVIC_DisableIRQ(USB_IRQn);
#elif defined(TARGET_MK20DX)
    NVIC_DisableIRQ(USB0_IRQn);
#endif
}

static void enable_usb_irq(void){
#if defined(TARGET_LPC11U35)
    NVIC_EnableIRQ(USB_IRQn);
#elif defined(TARGET_MK20DX)
    NVIC_EnableIRQ(USB0_IRQn);
#endif
}

static int program_page(unsigned long adr, unsigned long sz, unsigned char *buf) {
    // USB interrupt will then be re-enabled in the flash programming thread
    disable_usb_irq();
    isr_evt_set(PROGRAM_PAGE_EVENT, flash_programming_task_id);
    return 0;
}

static int program_sector() {
    // if we have received two sectors, write into flash
    if (!(current_sector % 2)) {
        //flash_erase_sector(flashPtr);
        if (program_page(flashPtr, 1024, (uint8_t *)BlockBuf)) {
            // even if there is an error, adapt flashptr
            flashPtr += 1024;
            return 1;
        }

        // if we just wrote the last sector -> disconnect usb
        if (current_sector == nb_sector) {
            initDisconnect(1);
            return 0;
        }

        flashPtr += 1024;

    }

    // we have to write the last sector which has a size of half a page
    if (current_sector == nb_sector) {
        if (current_sector % 2) {
            //flash_erase_sector(flashPtr);
            if (program_page(flashPtr, 1024, (uint8_t *)BlockBuf)) {
                return 1;
            }
        }
        initDisconnect(1);
    }
    return 0;
}

extern uint32_t SystemCoreClock;
__task void flash_programming_task(void) {
    uint32_t flags = 0, i;
    OS_RESULT res;
    flash_programming_task_id = os_tsk_self();
    while(1) {
        res = os_evt_wait_or(PROGRAM_PAGE_EVENT | FLASH_INIT_EVENT, NO_TIMEOUT);
        if (res == OS_R_EVT) {
            flags = os_evt_get();
            if (flags & PROGRAM_PAGE_EVENT) {
                flash_program_page_svc(flashPtr, 1024, (uint8_t *)BlockBuf);
                enable_usb_irq();

            }

            if (flags & FLASH_INIT_EVENT) {
                // init flash
                flash_init(SystemCoreClock);

                // erase flash for new binary
                for(i = START_APP_ADDRESS; i < END_FLASH; i += SECTOR_SIZE) {
                    flash_erase_sector_svc(i);
                }
                enable_usb_irq();
            }
        }
    }
}



// this task is responsible to check
// when we receive a root directory where there
// is a valid .bin file and when we have received
// all the sectors that we don't receive new valid sectors
// after a certain timeout
__task void msc_valid_file_timeout_task(void)
{
    uint32_t flags = 0;
    OS_RESULT res;
    uint32_t start_timeout_time = 0, time_now = 0;
    uint8_t timer_started = 0;
    msc_valid_file_timeout_task_id = os_tsk_self();
    while(1) {
        res = os_evt_wait_or(MSC_TIMEOUT_SPLIT_FILES_EVENT | MSC_TIMEOUT_START_EVENT | MSC_TIMEOUT_STOP_EVENT, 100);

        if (res == OS_R_EVT) {

            flags = os_evt_get();

            if (flags & MSC_TIMEOUT_SPLIT_FILES_EVENT) {
                msc_event_timeout = 1;
                os_dly_wait(50);

                if (msc_event_timeout == 1) {
                    // if the program reaches this point -> it means that no sectors have been received in the meantime
                    if (current_sector % 2) {
                        program_page(flashPtr, 1024, (uint8_t *)BlockBuf);
                    }
                    initDisconnect(1);
                    msc_event_timeout = 0;
                }
            }

            if (flags & MSC_TIMEOUT_START_EVENT) {
                start_timeout_time = os_time_get();
                timer_started = 1;
            }

            if (flags & MSC_TIMEOUT_STOP_EVENT) {
                timer_started = 0;
            }

        } else {
            if (timer_started) {
                time_now = os_time_get();
                // timeout
                if ((time_now - start_timeout_time) > TIMEOUT_S) {
                    timer_started = 0;
                    reason = TIMEOUT;
                    initDisconnect(0);
                }
            }
        }
    }
}


int jtag_init() {
    if (jtag_flash_init != 1) {
        if (need_restart_usb == 1) {
            reason = SWD_PORT_IN_USE;
            initDisconnect(0);
            return 1;
        }

        // USB interrupt will then be re-enabled in the flash programming thread
        disable_usb_irq();

        isr_evt_set(FLASH_INIT_EVENT, flash_programming_task_id);

        jtag_flash_init = 1;
    }
    return 0;
}


typedef enum {
    BIN_FILE,
    PAR_FILE,
    DOW_FILE,
    CRD_FILE,
} FILE_TYPE;


// take a look here: http://cs.nyu.edu/~gottlieb/courses/os/kholodov-fat.html
// to have info on fat file system
int search_bin_file(uint8_t * root, uint8_t sector) {
    // idx is a pointer inside the root dir
    // we begin after all the existing entries
    int idx = 8, end = 16;
    char found = 0;
    uint32_t i = 0;
    uint32_t nb_sector_to_move = 0;
    FILE_TYPE file_type;
    uint8_t hidden_file = 0, adapt_th_sector = 0;

    if (sector == 3) {
        idx = (drag_success == 1) ? 8 + 32*12 : 8 + 32*13;
        end = (drag_success == 1) ? 7 : 6;
    }

    // first check that we did not receive any directory
    // if we detect a directory -> disconnect / failed
    for (i = 0; i < end; i++) {
        if (root[idx + 3] & 0x10) {
            reason = BAD_EXTENSION_FILE;
            initDisconnect(0);
            return -1;
        }
    }


    if (sector == 3) {
        idx = (drag_success == 1) ? 8 + 32*9 : 8 + 32*10;
        end = (drag_success == 1) ? 7 : 6;
    }

    // now do the real search for a valid .bin file
    for (i = 0; i < end; i++) {
        if (((root[idx] == 'B') && (root[idx + 1] == 'I') && (root[idx + 2] == 'N')) ||
            ((root[idx] == 'b') && (root[idx + 1] == 'i') && (root[idx + 2] == 'n')) ||

            //strange extension on win IE 9...
            ((root[idx] == 'P') && (root[idx + 1] == 'A') && (root[idx + 2] == 'R')) ||

            //strange extension on mac...
            ((root[idx] == 'D') && (root[idx + 1] == 'O') && (root[idx + 2] == 'W')) ||

            /* strange extension on linux ... */
            ((root[idx] == 'C') && (root[idx + 1] == 'R') && (root[idx + 2] == 'D'))  ) {

            if ((root[idx] == 'B') && (root[idx + 1] == 'I') && (root[idx + 2] == 'N'))
                file_type = BIN_FILE;
            else if ((root[idx] == 'b') && (root[idx + 1] == 'i') && (root[idx + 2] == 'n'))
                file_type = BIN_FILE;
            else if ((root[idx] == 'P') && (root[idx + 1] == 'A') && (root[idx + 2] == 'R'))
                file_type = PAR_FILE;
            else if ((root[idx] == 'D') && (root[idx + 1] == 'O') && (root[idx + 2] == 'W'))
                file_type = DOW_FILE;
            else if ((root[idx] == 'C') && (root[idx + 1] == 'R') && (root[idx + 2] == 'D'))
                file_type = CRD_FILE;

            hidden_file = (root[idx + 3] & 0x02) ? 1 : 0;

            // compute the size of the file
            size = *(root + idx - 8 + 31) << 24;
            size |= *(root + idx - 8 + 30) << 16;
            size |= *(root + idx - 8 + 29) << 8;
            size |= *(root + idx - 8 + 28);

            // read the sector number where data are stored
            begin_sector = *(root + idx - 8 + 27) << 8;
            begin_sector |= *(root + idx - 8 + 26);

            // add 3 as there are two sectors for the root dir and one more for the fat
            begin_sector += 3;

            if (size == 0) {
                idx += 32;
                continue;
            }

            // compute the number of sectors
            if (!(size % 512))
                nb_sector = size / 512;
            else
                nb_sector = size / 512 + 1;

            if (    (root[idx - 8] == '_') ||
                    (root[idx - 8] == '.') ||
                    (hidden_file && ((root[idx - 8] == '_') || (root[idx - 8] == '.'))) ||
                    ((root[idx - 8] == 0xE5) && (file_type != CRD_FILE) && (file_type != PAR_FILE))) {
                if (theoretical_start_sector == begin_sector) {
                    adapt_th_sector = 1;
                }
                size = 0;
                nb_sector = 0;
                idx += 32;
                continue;
            }

            // if we receive a file with a valid extension
            // but there has been program page error previously
            // we fail / disconnect usb
            if ((program_page_error == 1) && (maybe_erase == 0) && (start_sector >= begin_sector)) {
                reason = RESERVED_BITS;
                initDisconnect(0);
                return -1;
            }

            adapt_th_sector = 0;

            // on mac, with safari, we receive all the files with some more sectors at the beginning
            // we have to move the sectors... -> 2x slower
            if ((start_sector != 0) && (start_sector < begin_sector) && (current_sector - (begin_sector - start_sector) >= nb_sector)) {

                // we need to copy all the sectors
                // we don't listen to msd interrupt
                listen_msc_isr = 0;

                if (current_sector % 2) {
                    program_page(flashPtr, 1024, (uint8_t *)BlockBuf);
                }

                nb_sector_to_move = (nb_sector % 2) ? nb_sector/2 + 1 : nb_sector/2;
                for (i = 0; i < nb_sector_to_move; i++) {
                    program_page(START_APP_ADDRESS + i*1024, 1024, (uint8_t *)BlockBuf);
                }
                initDisconnect(1);
                return -1;
            }

            found = 1;
            idx -= 8;
            good_file = 1;
            break;
        }
        // if we receive a new file which does not have the good extension
        // fail and disconnect usb
        else if(    ( ((root[idx] >= 'a') && (root[idx] <= 'z')) || ((root[idx] >= 'A') && (root[idx] <= 'Z')) ) &&
                    ( ((root[idx + 1] >= 'a') && (root[idx + 1] <= 'z')) || ((root[idx + 1] >= 'A') && (root[idx + 1] <= 'Z')) || (root[idx + 1] == 0x20) ) &&
                    ( ((root[idx + 2] >= 'a') && (root[idx + 2] <= 'z')) || ((root[idx + 2] >= 'A') && (root[idx + 2] <= 'Z')) || (root[idx + 2] == 0x20) ) &&
                    ( ((root[idx - 8] >= 'a') && (root[idx - 8] <= 'z')) || ((root[idx - 8] >= 'A') && (root[idx - 8] <= 'Z')) ) &&
                    (root[idx - 8] != '.' &&
                    (root[idx - 8] != '_'))) {
            reason = BAD_EXTENSION_FILE;
            initDisconnect(0);
            return -1;
        } else {
            idx += 32;
        }
    }

    if (adapt_th_sector) {
        theoretical_start_sector += nb_sector;
        init(0);
    }
    return (found == 1) ? idx : -1;
}


void usbd_msc_read_sect (uint32_t block, uint8_t *buf, uint32_t num_of_blocks) {

    if ((listen_msc_isr == 0))
        return;

    if (USBD_MSC_MediaReady) {
        // blink led not permanently
        //mainBlinkMsdLED(0);
        memset(buf, 0, 512);

        if (block < 9) {
            memcpy(buf, sectors[block].sect, sectors[block].length);

            // add new entry in FAT
            if ((block == 1) && (drag_success == 0)) {
                buf[9] = 0xff;
                buf[10] = 0x0f;
            } else if ((block == 3) && (drag_success == 0)) {
                memcpy(buf + sectors[block].length, fail, 16*2);
                // adapt size of file according fail reason
                buf[sectors[block].length + 28] = strlen((const char *)reason_array[reason]);
                buf[sectors[block].length + 26] = 6;
            }
        }

        else if (block == 9) {
            memcpy(buf, reason_array[reason], strlen((const char *)reason_array[reason]));
        }
    }
}


void usbd_msc_write_sect (uint32_t block, uint8_t *buf, uint32_t num_of_blocks) {
    int idx_size = 0;

    if ((listen_msc_isr == 0))
        return;

    // we recieve the root directory
    if ((block == 3) || (block == 4)) {
        // try to find a .bin file in the root directory
        idx_size = search_bin_file(buf, block);

        // .bin file exists
        if (idx_size != -1) {

            if (sector_received_first == 0)
                root_dir_received_first = 1;

            // this means that we have received the sectors before root dir (linux)
            // we have to flush the last page into the target flash
            if ((sector_received_first == 1) && (current_sector == nb_sector) && (jtag_flash_init == 1)) {
                if (msc_event_timeout == 0) {
                    msc_event_timeout = 1;
                    isr_evt_set(MSC_TIMEOUT_SPLIT_FILES_EVENT, msc_valid_file_timeout_task_id);
                }
                return;
            }

            // means that we are receiving additionnal sectors
            // at the end of the file ===> we ignore them
            if ((sector_received_first == 1) && (start_sector == begin_sector) && (current_sector > nb_sector) && (jtag_flash_init == 1)) {
                initDisconnect(1);
                return;
            }
        }
    }
    if (block > 4) {

        //mainUSBBusyEvent();

        if (root_dir_received_first == 0)
            sector_received_first = 1;

        // if we don't receive consecutive sectors
        // set maybe erase in case we receive other sectors
        if ((previous_sector != 0) && ((previous_sector + 1) != block)) {
            maybe_erase = 1;
            return;
        }

        if (!flash_started && (block > theoretical_start_sector)) {
            theoretical_start_sector = block;
        }

        // init jtag if needed
        if (jtag_init() == 1)
            return;

        if (jtag_flash_init == 1) {


            // we erase the chip if we received unrelated data before (mac compatibility)
            if (maybe_erase && (block == theoretical_start_sector)) {
                //if (!jtagEraseFlash())
                //    return;
                maybe_erase = 0;
                program_page_error = 0;
            }

            // drop block < theoretical_sector
            if (theoretical_start_sector > block) {
                return;
            }

            if ((flash_started == 0) && (theoretical_start_sector == block)) {
                flash_started = 1;
                isr_evt_set(MSC_TIMEOUT_START_EVENT, msc_valid_file_timeout_task_id);
                start_sector = block;
            }

            // at the beginning, we need theoretical_start_sector == block
            if ((flash_started == 0) && (theoretical_start_sector != block)) {
                reason = BAD_START_SECTOR;
                initDisconnect(0);
                return;
            }

            // not consecutive sectors detected
            if ((flash_started == 1) && (maybe_erase == 0) && (start_sector != block) && (block != (start_sector + current_sector))) {
                reason = NOT_CONSECUTIVE_SECTORS;
                initDisconnect(0);
                return;
            }

            // if we receive a new sector
            // and the msc thread has been started (we thought that the file has been fully received)
            // we kill the thread and write the sector in flash
            if (msc_event_timeout == 1) {
                msc_event_timeout = 0;
            }

            if (flash_started && (block == theoretical_start_sector)) {
                //if (!jtagEraseFlash())
                //    return;
                maybe_erase = 0;
                program_page_error = 0;
            }


            previous_sector = block;
            // adapt index in buffer
            current_sector++;
            USBD_MSC_BlockBuf = (current_sector % 2) ? (uint8_t *)BlockBuf + 512 : (uint8_t *)BlockBuf;
            if (program_sector() == 1) {
                if (good_file) {
                    reason = RESERVED_BITS;
                    initDisconnect(0);
                    return;
                }
                program_page_error = 1;
                return;
            }
        }
    }
}


void usbd_msc_init () {

    if (!task_first_started) {
        task_first_started = 1;
        os_tsk_create_user(msc_valid_file_timeout_task, MSC_TASK_PRIORITY, msc_task_stack, MSC_TASK_STACK);
        os_tsk_create(flash_programming_task, FLASH_PROGRAMMING_TASK_PRIORITY);
    }

    USBD_MSC_MemorySize = 512*512;
    USBD_MSC_BlockSize  = 512;
    USBD_MSC_BlockGroup = 1;
    USBD_MSC_BlockCount = USBD_MSC_MemorySize / USBD_MSC_BlockSize;
    USBD_MSC_BlockBuf   = (uint8_t *)BlockBuf;
    USBD_MSC_MediaReady = 1;
}

