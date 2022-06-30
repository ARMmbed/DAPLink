#include <stdio.h>
#include "shell_cmd_reset_into_swu_mode.h"
#include "settings.h"
#include "udb_reset.h"
#include "stm32h7xx.h"

void cmd_reset_into_swu_mode(int argc, char *argv[])
{
    config_ram_set_hold_in_bl(true);
    // need some delay to write the config
    // if you reset immediately, it will not go to SWU mode
    HAL_Delay(10);
    udb_reset();
}
