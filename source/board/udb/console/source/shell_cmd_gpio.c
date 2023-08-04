#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stm32h7xx.h>
#include "shell_cmd_gpio.h"

void cmd_gpio(int argc, char * argv[])
{
    bool skip_request = false;
    GPIO_PinState value = 0;
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_TypeDef * port;
    uint16_t pin;

    int port_arg = argv[2][0] - 'A';
    uint16_t pin_arg = strtoul(argv[3], NULL, 10);

    if ((port_arg < 0) ||
        (port_arg > (int)('I' - 'A')) ||
        (pin_arg  > 15))
    {
        printf("Port: %d Pin %u is not valid.\n", port_arg, pin_arg);
        goto done;
    }

    pin = (uint16_t)0x0001 << pin_arg;
    port = (GPIO_TypeDef *)(D3_AHB1PERIPH_BASE + (port_arg * 0x0400UL));

    if (strcmp(argv[1], "read") == 0)
    {
        skip_request = true;
    }
    else if (strcmp(argv[1], "in") == 0)
    {
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
    }
    else if (strcmp(argv[1], "in_pu") == 0)
    {
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Pull = GPIO_PULLUP;
    }
    else if (strcmp(argv[1], "in_pd") == 0)
    {
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Pull = GPIO_PULLDOWN;
    }
    else if (strcmp(argv[1], "pp_clear") == 0)
    {
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    }
    else if (strcmp(argv[1], "pp_set") == 0)
    {
        HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    }
    else if (strcmp(argv[1], "toggle") == 0)
    {
        HAL_GPIO_TogglePin(port, pin);
        skip_request = true;
    }
    else
    {
        printf("Unknown command '%s'\n", argv[1]);
        goto done;
    }

    if (!skip_request)
    {
        GPIO_InitStructure.Pin = pin;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(port, &GPIO_InitStructure);
    }
    value = HAL_GPIO_ReadPin(port, pin);
    printf("GPIO port %c pin %u is %u\n", argv[2][0], pin_arg, value);

done:
    return;
}