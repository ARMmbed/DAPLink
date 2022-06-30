#ifndef SHELL_CMD_SWD_DUT_H
#define SHELL_CMD_SWD_DUT_H

#define SHELL_CMD_SWD_DUT_MIN_ARGS  (0)
#define SHELL_CMD_SWD_DUT_MAX_ARGS  (1)

#define UIF_CMD_SWD_DUT \
    DECLARE_UIF_CMD("swd_dut", SHELL_CMD_SWD_DUT_MIN_ARGS, SHELL_CMD_SWD_DUT_MAX_ARGS, 0, cmd_swd_dut, \
        "You can redirect SWD communication over UDB to either the default DUT0 or to the DUT1 connection. Use this command to switch between the two\n", "[0|1]\n" \
        "    swd_dut 0 - use DUT 0 SWD\n" \
        "    swd_dut 1 - use DUT 1 SWD\n" \
        "    swd_dut - examine which DUT is in use") \

void cmd_swd_dut(int argc, char *argv[]);

#endif // SHELL_CMD_SWD_DUT_H
