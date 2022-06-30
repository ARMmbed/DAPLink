#include <string.h>
#include <stdio.h>
#include "udb_version.h"
#include "version_git.h"
#include "DAP_config.h"
#include "IO_Config.h"
#include "daplink_addr.h"
#include "compiler.h"
#include "util.h"
#include "pac193x.h"
#include "udb_errno.h"

#define PIN_UDB_HW_VERSION_PORT GPIOG
#define PIN_UDB_HW_VERSION      GPIO_PIN_15

#if defined(UDB_VERSION_BASE) && defined(UDB_BUILD_NUMBER)

#define VAL(str) #str
#define TO_STRING(str) VAL(str)
#define UDB_BUILD_VERSION        TO_STRING(UDB_VERSION_BASE) TO_STRING(UDB_BUILD_NUMBER)

#else

#if GIT_LOCAL_MODS == 1
#define GIT_LOCAL_MODS_STR      "_modified"
#else
#define GIT_LOCAL_MODS_STR      ""
#endif // GIT_LOCAL_MODS
#define UDB_BUILD_VERSION       "local_build" GIT_LOCAL_MODS_STR

#endif // defined(UDB_VERSION_BASE) && defined(UDB_BUILD_NUMBER)

#define UDB_BOOTLOADER_VERSION                  UDB_BUILD_VERSION
#define UDB_BOOTLOADER_VERSION_SECTION_ADDR     DAPLINK_ROM_CONFIG_ADMIN_START
#define BOOTLOADER_CFG_MAGIC_KEY                (0x5a5a5a5a)
#define BOOTLOADER_MAX_VERSION_LENGTH           UDB_VERSION_MAX_LENGTH

#define HW_VERSION_GPIO_STABILIZATION_TIME_MS           (10)
#define HW_VERSION_VOLTAGE_THRESHOLD_FOR_P2_AND_P3_mV   (4000)

enum {
    HW_VERSION_PIN_0,
    HW_VERSION_PIN_1,
    HW_VERSION_PIN_2,
    HW_VERSION_PIN_3,
    HW_VERSION_PIN_4,
    HW_VERSION_PIN_COUNT,
};

typedef struct
{
    GPIO_TypeDef *port;
    uint32_t pin;
} hw_version_pin_type_t;

typedef struct __attribute__((__packed__))
{
    uint32_t magic_key;
    char version[BOOTLOADER_MAX_VERSION_LENGTH];
} bootloader_version_t;

COMPILER_ASSERT(sizeof(UDB_BOOTLOADER_VERSION) < BOOTLOADER_MAX_VERSION_LENGTH);
COMPILER_ASSERT(sizeof(bootloader_version_t) < DAPLINK_SECTOR_SIZE);

#ifdef DAPLINK_BL
static volatile bootloader_version_t config_rom_bl __attribute__((section("cfgrom_bl"))) =
{
    .magic_key = BOOTLOADER_CFG_MAGIC_KEY,
    .version = UDB_BOOTLOADER_VERSION,
};
#endif

static const char s_build_version_str[] = "udb_" UDB_BUILD_VERSION "_" GIT_DESCRIPTION "_hw:";
static hw_version_t s_hw_version = HW_VERSION_UNKNOWN;

static const hw_version_pin_type_t s_hw_version_pins[HW_VERSION_PIN_COUNT] =
{
    [HW_VERSION_PIN_0] =
    {
        .port = GPIOI,
        .pin  = GPIO_PIN_5,
    },
    [HW_VERSION_PIN_1] =
    {
        .port = GPIOD,
        .pin  = GPIO_PIN_7,
    },
    [HW_VERSION_PIN_2] =
    {
        .port = GPIOB,
        .pin  = GPIO_PIN_2,
    },
    [HW_VERSION_PIN_3] =
    {
        .port = GPIOD,
        .pin  = GPIO_PIN_8,
    },
    [HW_VERSION_PIN_4] =
    {
        .port = GPIOD,
        .pin  = GPIO_PIN_5,
    },
};

static bool is_hw_version_p1(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_PinState bitstatus;
    GPIO_InitStructure.Pin = PIN_UDB_HW_VERSION;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(PIN_UDB_HW_VERSION_PORT, &GPIO_InitStructure);

    HAL_Delay(HW_VERSION_GPIO_STABILIZATION_TIME_MS);

    bitstatus = HAL_GPIO_ReadPin(PIN_UDB_HW_VERSION_PORT, PIN_UDB_HW_VERSION);
    HAL_GPIO_DeInit(PIN_UDB_HW_VERSION_PORT, PIN_UDB_HW_VERSION);

    return bitstatus != GPIO_PIN_RESET;
}

// Use pac193x driver directly instead of power measurement driver to
// break circular dependency.
static hw_version_t decide_hw_version_p2_or_p3(void)
{
    pac193x_cfg_t cfg;
    uint8_t vbus_reg[PAC193X_VBUSN_REG_SIZE];
    uint32_t vbus;
    uint16_t voltage_mV;
    hw_version_t ver;

    int status;
    int retries = 0;

    cfg.ctrl_cfg.val      = PAC193X_CTRL_POR_VALUE;
    cfg.chan_dis_cfg.val  = PAC1934_CHANNEL_DIS_POR_VALUE;
    cfg.neg_pwr_cfg.val   = PAC193X_NEG_PWR_POR_VALUE;
    cfg.slow_cfg.val      = PAC193X_SLOW_POR_VALUE;

    // We only need channel 3. Disable all other channels.
    cfg.chan_dis_cfg.ch1_dis = 1;
    cfg.chan_dis_cfg.ch2_dis = 1;
    cfg.chan_dis_cfg.ch4_dis = 1;

    status = pac193x_init(&cfg);
    util_assert(status == UDB_SUCCESS);

    HAL_Delay(PAC193X_INIT_STABLIZATION_TIME_MS);

    // Despite pac193x_init() already send PAC193X_COMMAND_REFRESH once. That
    // REFRESH command only enables channel3 and doesn't start to measure
    // channel3. Need another REFRESH command to have data available in channel 3.
    status = pac193x_send_command(PAC193X_COMMAND_REFRESH);
    util_assert(status == UDB_SUCCESS);

    HAL_Delay(PAC193X_REFRESH_STABLIZATION_TIME_MS);

    status = pac193x_read_reg(PAC193X_VBUS3_REG, PAC193X_VBUSN_REG_SIZE, vbus_reg);
    util_assert(status == UDB_SUCCESS);

    vbus = 0;
    for (uint8_t i = 0; i < PAC193X_VBUSN_REG_SIZE; ++i)
    {
        vbus = (vbus << 8) + vbus_reg[i];
    }

    voltage_mV = (uint16_t)(PAC193X_FULL_SCALE_VOLTAGE_MV * vbus / PAC193X_UNIPOLAR_DENOMINATOR);

    if (voltage_mV >= HW_VERSION_VOLTAGE_THRESHOLD_FOR_P2_AND_P3_mV)
    {
        ver = HW_VERSION_P3;
    }
    else
    {
        ver = HW_VERSION_P2;
    }

    return ver;
}

/*
 * If the values of s_hw_version_pins follow pull-ups and pull-downs, HW_VERSION is P2 or P3
 * Otherwise HW_VERSION is HW_VERSION_P3 + s_hw_version_pins.
 */
static hw_version_t read_hw_version_after_p1(void)
{
    uint8_t pull_up_reads = 0;
    uint8_t pull_down_reads = 0;
    hw_version_t ver;
    GPIO_InitTypeDef gpio_init =
    {
        .Pull = GPIO_PULLUP,
        .Mode = GPIO_MODE_INPUT,
        .Speed = GPIO_SPEED_FREQ_LOW
    };

    // Initialize hw_version_pins with internal pull-up enable
    for (unsigned i = 0; i < HW_VERSION_PIN_COUNT; ++i)
    {
        gpio_init.Pin = s_hw_version_pins[i].pin;
        HAL_GPIO_Init(s_hw_version_pins[i].port, &gpio_init);
    }

    HAL_Delay(HW_VERSION_GPIO_STABILIZATION_TIME_MS);

    for (unsigned i = 0; i < HW_VERSION_PIN_COUNT; ++i)
    {
        if (HAL_GPIO_ReadPin(s_hw_version_pins[i].port, s_hw_version_pins[i].pin) == GPIO_PIN_SET)
        {
            pull_up_reads |= 1 << i;
        }
    }

    // Re-initialize s_hw_version_pins with internal pull-down enable
    gpio_init.Pull = GPIO_PULLDOWN;
    for (unsigned i = 0; i < HW_VERSION_PIN_COUNT; ++i)
    {
        gpio_init.Pin = s_hw_version_pins[i].pin;
        HAL_GPIO_Init(s_hw_version_pins[i].port, &gpio_init);
    }

    HAL_Delay(HW_VERSION_GPIO_STABILIZATION_TIME_MS);

    for (unsigned i = 0; i < HW_VERSION_PIN_COUNT; ++i)
    {
        if (HAL_GPIO_ReadPin(s_hw_version_pins[i].port, s_hw_version_pins[i].pin) == GPIO_PIN_SET)
        {
            pull_down_reads |= 1 << i;
        }
        // DeInit so we can use the GPIO pin in other places later.
        HAL_GPIO_DeInit(s_hw_version_pins[i].port, s_hw_version_pins[i].pin);
    }

    // if pull-up reads and pull-down reads are the same, which means
    // s_hw_version_pins don't follow pull-ups and pull-downs, it is at least P3
    if (pull_up_reads == pull_down_reads)
    {
        ver = HW_VERSION_P3 + pull_down_reads;
    }
    else
    {
        ver = decide_hw_version_p2_or_p3();
    }

    return ver;
}

/*
 * P1, P2, and P3 didn't have a dedicated hardware to decide the version.
 * We relied on board-specific hardware in these early boards to
 * detect the version in software.
 *
 * The difference between P1 and others is that GPIO PG15 in P1
 * is floating and others have external pull-up 100k and pull-down 10k.
 * When we enable the internal pull-up of PG15 (min. 30k), the voltage of P1's PG15
 * would go high and others would stay low:
 *
 *   30k and 100k in parallel produce 23.08k pull-up. 23.08k PU and 10k PD in series
 *   would produce 10/(10 + 23.08) = 0.30Vdd, which is good enough for low.
 *
 * P4 and the boards after have a set of resistors connected to a set of pins
 * to detect version. But P2 and P3 does not have that and the voltage of those pins
 * would change when we switch between pull-up and pull-down modes. So if the
 * GPIO reads in pull-up and pull-down modes are not equal, it is P2 or P3.
 *
 * To differentiate P2 and P3, we relied on the power measurement IC pac193x. P2
 * wired pac193x channel 3 to nothing while P3 wired channel 3 to mainboard usb.
 * If we measure the voltage on P3, P3 would get 5V and P2 would get 0V.
 *
 * Finally, to decide versions for P4 and later, the dedicated resistors attached to
 * the pins listed in s_hw_version_pins would change each time we update the
 * hardware to reflect the hardware version.
 */
void udb_read_hw_version(void)
{
    if (is_hw_version_p1())
    {
        s_hw_version = HW_VERSION_P1;
    }
    else
    {
        s_hw_version = read_hw_version_after_p1();
    }
}

int udb_get_interface_version(uint8_t *buffer, unsigned size)
{
    return snprintf((char*)buffer, size, "%s%d", s_build_version_str, s_hw_version);
}

int udb_get_bootloader_version(uint8_t *buffer, unsigned size)
{
    char bootloader_version_str[BOOTLOADER_MAX_VERSION_LENGTH] = "unknown, note: bootloader ver are introduced after ver 0.11";
    bootloader_version_t* bl_version = (bootloader_version_t*)UDB_BOOTLOADER_VERSION_SECTION_ADDR;
    if (bl_version->magic_key == BOOTLOADER_CFG_MAGIC_KEY)
    {
        memcpy(bootloader_version_str, bl_version->version, BOOTLOADER_MAX_VERSION_LENGTH);
    }
    return snprintf((char*)buffer, size, "%s", bootloader_version_str);
}

hw_version_t udb_get_hw_version(void)
{
    // don't get hw version before udb_read_hw_version()
    util_assert(s_hw_version != HW_VERSION_UNKNOWN);

    return s_hw_version;
}
