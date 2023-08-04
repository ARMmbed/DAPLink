#ifndef SHELL_CMD_EXT_RELAY_H
#define SHELL_CMD_EXT_RELAY_H

#define SHELL_CMD_EXT_RELAY_MIN_ARGS    (1)
#define SHELL_CMD_EXT_RELAY_MAX_ARGS    (1)

#define UIF_CMD_EXT_RELAY \
    DECLARE_UIF_CMD("ext_relay", SHELL_CMD_EXT_RELAY_MIN_ARGS, SHELL_CMD_EXT_RELAY_MAX_ARGS, 0, cmd_ext_relay, \
        "control external relay", "on|off|status") \

void cmd_ext_relay(int argc, char *argv[]);

#endif // SHELL_CMD_EXT_RELAY_H
