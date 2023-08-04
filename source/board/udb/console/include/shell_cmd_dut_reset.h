#ifndef SHELL_CMD_DUT_RESET_H
#define SHELL_CMD_DUT_RESET_H

#define UIF_CMD_DUT_RESET \
    DECLARE_UIF_CMD("dut_reset", 1, 1, 0, cmd_dut_reset, \
        "Reset DUT", "<dut-id>\n" \
        "    <dut-id> - can be 0 or 1") \

void cmd_dut_reset(int argc, char *argv[]);

#endif // SHELL_CMD_DUT_RESET_H
