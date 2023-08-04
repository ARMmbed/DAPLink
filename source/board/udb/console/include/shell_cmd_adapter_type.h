#ifndef SHELL_COMMAND_ADAPTER_TYPE_H
#define SHELL_COMMAND_ADAPTER_TYPE_H

#define UIF_CMD_ADAPTER_TYPE \
    DECLARE_UIF_CMD("adapter_type", 0, UIF_MAX_ARGS, 0, cmd_adapter_type, \
        "adapter flex type connected to the 60-pin UDC\t", "") \

void cmd_adapter_type(int argc, char *argv[]);

#endif // SHELL_COMMAND_ADAPTER_TYPE_H
