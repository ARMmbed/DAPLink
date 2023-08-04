#ifndef SHELL_COMMAND_RESET_H
#define SHELL_COMMAND_RESET_H

#define UIF_CMD_RESET \
    DECLARE_UIF_CMD("reset", 0, UIF_MAX_ARGS, 0, cmd_reset, \
        "Reset UDB", "") \

void cmd_reset(int argc, char *argv[]);

#endif // SHELL_COMMAND_RESET_H
