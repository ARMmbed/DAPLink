#ifndef SHELL_COMMAND_VERSION_H
#define SHELL_COMMAND_VERSION_H

#define UIF_CMD_VERSION \
    DECLARE_UIF_CMD("version", 0, UIF_MAX_ARGS, 0, cmd_version, \
        "Print UDB version", "") \

void cmd_version(int argc, char *argv[]);

#endif // SHELL_COMMAND_VERSION_H
