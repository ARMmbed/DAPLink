#include "shell_cmd_swd_dut.h"

#include <stdio.h>
#include <string.h>
#include "DAP_config.h"

void cmd_swd_dut(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("DUT %u\n", g_cur_swd_dut);
    }
    else if (strcmp(argv[1], "0") == 0)
    {
        change_swd_dut(SWD_DUT0);
    }
    else if (strcmp(argv[1], "1") == 0)
    {
        change_swd_dut(SWD_DUT1);
    }
    else
    {
        printf("Invalid arg %s\n", argv[1]);
    }
}
