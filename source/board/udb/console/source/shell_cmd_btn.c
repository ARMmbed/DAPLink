#include "shell_cmd_btn.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "IO_Config.h"

static const char *s_dut_btn_names[DUT_PIN_GROUP_ID_COUNT] =
{
    [DUT_PIN_GROUP_ID_UDC0_RST_L] = "RST0_L",
    [DUT_PIN_GROUP_ID_UDC0_BOOT_L] = "BOOT0_L",
    [DUT_PIN_GROUP_ID_UDC0_BUTTON_L] = "BTN0_L",
    [DUT_PIN_GROUP_ID_UDC1_RST] = "RST1",
    [DUT_PIN_GROUP_ID_UDC1_BOOT] = "BOOT1",
    [DUT_PIN_GROUP_ID_UDC1_BUTTON] = "BTN1",
};

void cmd_btn(int argc, char *argv[])
{
    dut_pin_group_id_t dut_pin_grp = DUT_PIN_GROUP_ID_COUNT;
    GPIO_PinState pin_state;

    for (dut_pin_group_id_t grp_id = 0; grp_id < DUT_PIN_GROUP_ID_COUNT; ++grp_id)
    {
        if (strcmp(argv[1], s_dut_btn_names[grp_id]) == 0)
        {
            dut_pin_grp = grp_id;
            break;
        }
    }

    if (dut_pin_grp < DUT_PIN_GROUP_ID_COUNT)
    {
        if (strcmp(argv[2], "press") == 0)
        {
            if (gpio_dut_pin_group_is_active_high(dut_pin_grp) == true)
            {
                gpio_config_dut_pin_group(dut_pin_grp, DUT_PIN_GROUP_STATE_OUTPUT, GPIO_PULLUP);
            }
            else
            {
                gpio_config_dut_pin_group(dut_pin_grp, DUT_PIN_GROUP_STATE_OUTPUT, GPIO_PULLDOWN);
            }
            HAL_Delay(GPIO_CONFIG_DUT_DELAY_MS);
        }
        else if (strcmp(argv[2], "release") == 0)
        {
            if (gpio_dut_pin_group_is_active_high(dut_pin_grp) == true)
            {
                gpio_config_dut_pin_group(dut_pin_grp, DUT_PIN_GROUP_STATE_OUTPUT, GPIO_PULLDOWN);
            }
            else
            {
                gpio_config_dut_pin_group(dut_pin_grp, DUT_PIN_GROUP_STATE_OUTPUT, GPIO_PULLUP);
            }
            HAL_Delay(GPIO_CONFIG_DUT_DELAY_MS);
            gpio_config_dut_pin_group(dut_pin_grp, DUT_PIN_GROUP_STATE_INPUT, GPIO_NOPULL);
        }
        else if (strcmp(argv[2], "tap") == 0)
        {
            if (gpio_dut_pin_group_is_active_high(dut_pin_grp) == true)
            {
                gpio_config_dut_pin_group(dut_pin_grp, DUT_PIN_GROUP_STATE_OUTPUT, GPIO_PULLUP);
                HAL_Delay(GPIO_CONFIG_DUT_DELAY_MS);
                gpio_write_dut_pin(dut_pin_grp, GPIO_PIN_RESET);
                HAL_Delay(GPIO_WRITE_DUT_DELAY_MS);
            }
            else
            {
                gpio_config_dut_pin_group(dut_pin_grp, DUT_PIN_GROUP_STATE_OUTPUT, GPIO_PULLDOWN);
                HAL_Delay(GPIO_CONFIG_DUT_DELAY_MS);
                gpio_write_dut_pin(dut_pin_grp, GPIO_PIN_SET);
                HAL_Delay(GPIO_WRITE_DUT_DELAY_MS);
            }
            gpio_config_dut_pin_group(dut_pin_grp, DUT_PIN_GROUP_STATE_INPUT, GPIO_NOPULL);
        }
        else
        {
            printf("%s is not a valid cmd\n", argv[2]);
        }
    }
    else
    {
        printf("%s is not a valid button name\n", argv[1]);
    }
}
