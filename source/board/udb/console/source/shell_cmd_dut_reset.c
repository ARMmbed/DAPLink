#include "shell_cmd_dut_reset.h"

#include <stdio.h>
#include <string.h>
#include <stm32h7xx.h>
#include "IO_Config.h"

/*
 * Reset pins going to the DUT go through level shifters.
 * We can only drive DUT pins by programming the pull-up/pull-down registers.
 * If we change a DUT pin to output, it will conflict with the level-transition IC.
 *
 * To reset DUT0, we change the direction to output, drive UDC0_RST_L low, delay,
 *   drive UDC0_RST_L high, delay, change direction back to input.
 * To reset DUT1, we change the direction to output, drive UDC1_RST high, delay,
 *   drive UDC1_RST low, delay, change direction back to input.
 */
void cmd_dut_reset(int argc, char *argv[])
{
    if (strcmp(argv[1], "0") == 0)
    {
        gpio_config_dut_pin_group(DUT_PIN_GROUP_ID_UDC0_RST_L, DUT_PIN_GROUP_STATE_OUTPUT, GPIO_PULLDOWN);
        HAL_Delay(GPIO_CONFIG_DUT_DELAY_MS);
        gpio_write_dut_pin(DUT_PIN_GROUP_ID_UDC0_RST_L, GPIO_PIN_SET);
        HAL_Delay(GPIO_WRITE_DUT_DELAY_MS);
        gpio_config_dut_pin_group(DUT_PIN_GROUP_ID_UDC0_RST_L, DUT_PIN_GROUP_STATE_INPUT, GPIO_NOPULL);
    }
    else if (strcmp(argv[1], "1") == 0)
    {
        gpio_config_dut_pin_group(DUT_PIN_GROUP_ID_UDC1_RST, DUT_PIN_GROUP_STATE_OUTPUT, GPIO_PULLUP);
        HAL_Delay(GPIO_CONFIG_DUT_DELAY_MS);
        gpio_write_dut_pin(DUT_PIN_GROUP_ID_UDC1_RST, GPIO_PIN_RESET);
        HAL_Delay(GPIO_WRITE_DUT_DELAY_MS);
        gpio_config_dut_pin_group(DUT_PIN_GROUP_ID_UDC1_RST, DUT_PIN_GROUP_STATE_INPUT, GPIO_NOPULL);
    }
    else
    {
        printf("dut_reset can not reset dut %s\n", argv[1]);
    }
}
