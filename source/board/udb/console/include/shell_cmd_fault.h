#ifndef SHELL_COMMAND_FAULT_H
#define SHELL_COMMAND_FAULT_H

#define UIF_CMD_FAULT \
    DECLARE_UIF_CMD("fault", 1, UIF_MAX_ARGS, 0, cmd_fault, \
        "Fault testing command", "test_assert|test_watchdog") \

void cmd_fault(int argc, char *argv[]);

#endif // SHELL_COMMAND_FAULT_H
