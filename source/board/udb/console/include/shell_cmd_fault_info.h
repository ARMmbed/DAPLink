#ifndef SHELL_CMD_FAULT_INFO_H
#define SHELL_CMD_FAULT_INFO_H

#define UIF_CMD_FAULT_INFO_MIN_ARGS   (1)
#define UIF_CMD_FAULT_INFO_MAX_ARGS   (1)

#define UIF_CMD_FAULT_INFO \
    DECLARE_UIF_CMD("fault_info", UIF_CMD_FAULT_INFO_MIN_ARGS, UIF_CMD_FAULT_INFO_MAX_ARGS, 0, cmd_fault_info, \
        "fault info utility", "print|clear\n" \
        "    fault_info print - print fault info\n" \
        "    fault_info clear - clear fault info\n") \

void cmd_fault_info(int argc, char *argv[]);

#endif // SHELL_CMD_FAULT_INFO_H
