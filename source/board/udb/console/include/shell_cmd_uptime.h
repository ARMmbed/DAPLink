#ifndef SHELL_CMD_UPTIME_H
#define SHELL_CMD_UPTIME_H

#define UIF_CMD_UPTIME \
    DECLARE_UIF_CMD("uptime", 0, 0, 0, cmd_uptime, \
        "UDB uptime", "") \

void cmd_uptime(int argc, char *argv[]);

#endif // SHELL_CMD_UPTIME_H
