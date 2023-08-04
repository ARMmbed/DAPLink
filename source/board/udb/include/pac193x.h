/*
 *  Description:
 *    Define an interface for PMIC pac193xx.
 */
#ifndef _PAC193x_H_INCLUDED_
#define _PAC193x_H_INCLUDED_

#include <stdint.h>

typedef enum
{
    PAC193X_REFRESH_REG         = 0x00,
    PAC193X_CTRL_REG            = 0x01,
    PAC193X_ACC_COUNT_REG       = 0x02,
    PAC193X_VPOWER1ACC_REG      = 0x03,
    PAC193X_VPOWER2ACC_REG      = 0x04,
    PAC193X_VPOWER3ACC_REG      = 0x05,
    PAC193X_VPOWER4ACC_REG      = 0x06,
    PAC193X_VBUS1_REG           = 0x07,
    PAC193X_VBUS2_REG           = 0x08,
    PAC193X_VBUS3_REG           = 0x09,
    PAC193X_VBUS4_REG           = 0x0a,
    PAC193X_VSENSE1_REG         = 0x0b,
    PAC193X_VSENSE2_REG         = 0x0c,
    PAC193X_VSENSE3_REG         = 0x0d,
    PAC193X_VSENSE4_REG         = 0x0e,
    PAC193X_VBUS1_AVG_REG       = 0x0f,
    PAC193X_VBUS2_AVG_REG       = 0x10,
    PAC193X_VBUS3_AVG_REG       = 0x11,
    PAC193X_VBUS4_AVG_REG       = 0x12,
    PAC193X_VSENSE1_AVG_REG     = 0x13,
    PAC193X_VSENSE2_AVG_REG     = 0x14,
    PAC193X_VSENSE3_AVG_REG     = 0x15,
    PAC193X_VSENSE4_AVG_REG     = 0x16,
    PAC193X_VPOWER1_AVG_REG     = 0x17,
    PAC193X_VPOWER2_AVG_REG     = 0x18,
    PAC193X_VPOWER3_AVG_REG     = 0x19,
    PAC193X_VPOWER4_AVG_REG     = 0x1a,
    PAC193X_CHANNEL_DIS_REG     = 0x1c,
    PAC193X_NEG_PWR_REG         = 0x1d,
    PAC193X_REFRESH_G_REG       = 0x1e,
    PAC193X_REFRESH_V_REG       = 0x1f,
    PAC193X_SLOW_REG            = 0x20,
    PAC193X_CTRL_ACT_REG        = 0x21,
    PAC193X_CHANNEL_DIS_ACT_REG = 0x22,
    PAC193X_NEG_PWR_ACT_REG     = 0x23,
    PAC193X_CTRL_LAT_REG        = 0x24,
    PAC193X_CHANNEL_DIS_LAT_REG = 0x25,
    PAC193X_NEG_PWR_LAT_REG     = 0x26,
    PAC193X_INVALID_REG         = 0x30,
    PAC193X_PRODUCT_ID_REG      = 0xfd,
    PAC193X_MANUFACTURER_ID_REG = 0xfe,
    PAC193X_REVISION_ID_REG     = 0xff,
} pac193x_reg_t;

#define PAC193X_ACC_COUNT_REG_SIZE      (3)
#define PAC193X_VPOWERNACC_REG_SIZE     (6)
#define PAC193X_VBUSN_REG_SIZE          (2)
#define PAC193X_VSENSEN_REG_SIZE        (2)
#define PAC193X_VBUSN_AVG_REG_SIZE      (2)
#define PAC193X_VSENSEN_AVG_REG_SIZE    (2)
#define PAC193X_VPOWERN_AVG_REG_SIZE    (4)
#define PAC193X_DEFAULT_REG_SIZE        (1)

typedef union
{
    struct
    {
        uint8_t ovf:1;
        uint8_t ovf_alert_en:1;
        uint8_t conv_alert_en:1;
        uint8_t alert_pin_en:1;
        uint8_t sing_mode:1;
        uint8_t sleep_mode:1;
        uint8_t sample_rate:2;
    };
    uint8_t val;
} pac193x_ctrl_reg_t;

typedef enum
{
    PAC193X_SAMPLE_RATE_1024    = 0x00,
    PAC193X_SAMPLE_RATE_256     = 0x01,
    PAC193X_SAMPLE_RATE_64      = 0x02,
    PAC193X_SAMPLE_RATE_32      = 0x03
} pac193x_sample_rate_t;

#define PAC193X_CTRL_POR_VALUE  (0x00)

typedef union
{
    struct
    {
        uint8_t nop:1;
        uint8_t noskip_en:1;
        uint8_t byte_count_en:1;
        uint8_t timeout_en:1;
        uint8_t ch4_dis:1;
        uint8_t ch3_dis:1;
        uint8_t ch2_dis:1;
        uint8_t ch1_dis:1;
    };
    uint8_t val;
} pac193x_chan_dis_reg_t;

#define PAC1931_CHANNEL_DIS_POR_VALUE   (0x70)
#define PAC1932_CHANNEL_DIS_POR_VALUE   (0x30)
#define PAC1933_CHANNEL_DIS_POR_VALUE   (0x10)
#define PAC1934_CHANNEL_DIS_POR_VALUE   (0x00)

typedef union
{
    struct
    {
        uint8_t vbus4_bi_en:1;
        uint8_t vbus3_bi_en:1;
        uint8_t vbus2_bi_en:1;
        uint8_t vbus1_bi_en:1;
        uint8_t vsense4_bi_en:1;
        uint8_t vsense3_bi_en:1;
        uint8_t vsense2_bi_en:1;
        uint8_t vsense1_bi_en:1;
    };
    uint8_t val;
} pac193x_neg_pwr_reg_t;

#define PAC193X_NEG_PWR_POR_VALUE   (0x00)

typedef union
{
    struct
    {
        uint8_t por_status:1;
        uint8_t fall_refresh_v_en:1;
        uint8_t fall_refresh_en:1;
        uint8_t rise_refresh_v_en:1;
        uint8_t rise_refresh_en:1;
        uint8_t is_hi_to_lo:1;
        uint8_t is_lo_to_hi:1;
        uint8_t is_pull_high:1;
    };
    uint8_t val;
} pac193x_slow_reg_t;

#define PAC193X_SLOW_POR_VALUE   (0x15)

typedef struct
{
    pac193x_ctrl_reg_t      ctrl_cfg;
    pac193x_chan_dis_reg_t  chan_dis_cfg;
    pac193x_neg_pwr_reg_t   neg_pwr_cfg;
    pac193x_slow_reg_t      slow_cfg;
} pac193x_cfg_t;

typedef enum
{
    PAC193X_COMMAND_REFRESH     = 0,
    PAC193X_COMMAND_REFRESH_V   = 1,
    PAC193X_COMMAND_REFRESH_G   = 2,

    // This is not a real command, it represents how many commands there are.
    PAC193X_COMMAND_SIZE        = 3,
} pac193x_command_type_t;

// Need 1 ms for REFRESH to take effect, choose 3 to have some margin
#define PAC193X_REFRESH_STABLIZATION_TIME_MS    (3)
// Although data sheet mentions 14.25 ms for the time to first communication,
// choose 100 based on experiments
#define PAC193X_INIT_STABLIZATION_TIME_MS       (100)

#define PAC193X_FULL_SCALE_VOLTAGE_MV           (32000)
#define PAC193X_FULL_SCALE_RANGE_MV             (100)

#define PAC193X_UNIPOLAR_DENOMINATOR            (1<<16)
#define PAC193X_BIPOLAR_DENOMINATOR             (1<<15)

int pac193x_init(const pac193x_cfg_t* cfg);
int pac193x_send_command(pac193x_command_type_t command_type);
int pac193x_read_reg(pac193x_reg_t reg_addr, uint32_t reg_size, void* out);

#endif // _PAC193x_H_INCLUDED_
