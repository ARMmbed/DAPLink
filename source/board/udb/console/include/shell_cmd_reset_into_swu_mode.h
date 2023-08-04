#ifndef SHELL_COMMAND_RESET_INTO_SWU_MODE_H
#define SHELL_COMMAND_RESET_INTO_SWU_MODE_H

#define UIF_CMD_RESET_INTO_SWU_MODE \
    DECLARE_UIF_CMD("reset_into_swu_mode", 0, UIF_MAX_ARGS, 0, cmd_reset_into_swu_mode, \
        "Request to stay in bootloader mode for SWU\t", "") \

void cmd_reset_into_swu_mode(int argc, char *argv[]);

#endif // SHELL_COMMAND_RESET_INTO_SWU_MODE_H
