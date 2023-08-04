#include "shell_cmd_ext_relay.h"

#include <stdio.h>
#include <string.h>
#include <stm32h743xx.h>

#include "IO_Config.h"

void cmd_ext_relay(int argc, char * argv[])
{
    if (strcmp(argv[1], "on") == 0)
    {
        HAL_GPIO_WritePin(UDC_EXT_RELAY_PORT, UDC_EXT_RELAY_PIN, GPIO_PIN_SET);
    }
    else if (strcmp(argv[1], "off") == 0)
    {
        HAL_GPIO_WritePin(UDC_EXT_RELAY_PORT, UDC_EXT_RELAY_PIN, GPIO_PIN_RESET);
    }
    else if (strcmp(argv[1], "status") == 0)
    {
        if (HAL_GPIO_ReadPin(UDC_EXT_RELAY_PORT, UDC_EXT_RELAY_PIN) == GPIO_PIN_SET)
        {
            printf("external relay is on\n");
        }
        else
        {
            printf("external relay is off\n");
        }
    }
    else
    {
        printf("Invalid arg: %s\n", argv[1]);
    }
}
