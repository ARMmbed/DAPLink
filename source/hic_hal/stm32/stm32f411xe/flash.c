#include "flash_hal.h"
#include "target_config.h"
#include "stm32f4xx.h"
#include "string.h"
#include "target_board.h"
#include "daplink_addr.h"
#include "util.h"

static uint32_t get_sector(uint32_t address)
{
    if (address < 0x08004000) return FLASH_SECTOR_0;
    if (address < 0x08008000) return FLASH_SECTOR_1;
    if (address < 0x0800C000) return FLASH_SECTOR_2;
    if (address < 0x08010000) return FLASH_SECTOR_3;
    if (address < 0x08020000) return FLASH_SECTOR_4;
    if (address < 0x08040000) return FLASH_SECTOR_5;
    if (address < 0x08060000) return FLASH_SECTOR_6;
    return FLASH_SECTOR_7;
}

// Начала физических секторов STM32F411CE
static const uint32_t s_sector_start[] = {
    0x08000000, 0x08004000, 0x08008000, 0x0800C000,
    0x08010000, 0x08020000, 0x08040000, 0x08060000
};

static int is_sector_start(uint32_t address)
{
    for (uint32_t i = 0; i < sizeof(s_sector_start) / sizeof(s_sector_start[0]); i++) {
        if (s_sector_start[i] == address) {
            return 1;
        }
    }
    return 0;
}

uint32_t Init(uint32_t adr, uint32_t clk, uint32_t fnc)
{
    return 0;
}

uint32_t UnInit(uint32_t fnc)
{
    return 0;
}

uint32_t EraseChip(void)
{
    FLASH_EraseInitTypeDef erase_init;
    uint32_t error;
    uint32_t ret = 0;

    if (g_board_info.target_cfg) {
        uint32_t start_addr = g_board_info.target_cfg->flash_regions[0].start;
        uint32_t end_addr = g_board_info.target_cfg->flash_regions[0].end;
        uint32_t start_sector = get_sector(start_addr);
        uint32_t end_sector = get_sector(end_addr - 1);

        memset(&erase_init, 0, sizeof(erase_init));
        erase_init.TypeErase = FLASH_TYPEERASE_SECTORS;
        erase_init.Sector = start_sector;
        erase_init.NbSectors = (end_sector - start_sector) + 1;
        erase_init.VoltageRange = FLASH_VOLTAGE_RANGE_3;

        HAL_FLASH_Unlock();
        if (HAL_FLASHEx_Erase(&erase_init, &error) != HAL_OK) {
            ret = 1;
        }
        HAL_FLASH_Lock();
    } else {
        ret = 1;
    }
    return ret;
}

uint32_t EraseSector(uint32_t adr)
{
    FLASH_EraseInitTypeDef erase_init;
    uint32_t error;
    uint32_t ret = 0;

    // DAPLink стирает "секторами" по DAPLINK_SECTOR_SIZE (16 KB), а физические
    // сектора F411 бывают 16/64/128 KB. Если adr лежит внутри уже стёртого сектора,
    // повторное стирание уничтожило бы только что записанные данные -> пропускаем.
    if (!is_sector_start(adr)) {
        return 0;
    }

    memset(&erase_init, 0, sizeof(erase_init));
    erase_init.TypeErase = FLASH_TYPEERASE_SECTORS;
    erase_init.Sector = get_sector(adr);
    erase_init.NbSectors = 1;
    erase_init.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    HAL_FLASH_Unlock();
    if (HAL_FLASHEx_Erase(&erase_init, &error) != HAL_OK) {
        ret = 1;
    }
    HAL_FLASH_Lock();

    return ret;
}

uint32_t ProgramPage(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    uint32_t ret = 0;
    util_assert(sz % 4 == 0);

    HAL_FLASH_Unlock();
    for (uint32_t i = 0; i < sz / 4; i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, adr + i * 4, buf[i]) != HAL_OK) {
            ret = 1;
            break;
        }
    }
    HAL_FLASH_Lock();

    return ret;
}
