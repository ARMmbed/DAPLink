#ifndef SHELL_COMMAND_GPIO_H
#define SHELL_COMMAND_GPIO_H

#define UIF_CMD_GPIO \
    DECLARE_UIF_CMD("gpio", 3, UIF_MAX_ARGS, 0, cmd_gpio, \
        "GPIO utility", \
        "read|in|pp_set|in_pu|in_pd|pp_clear|toggle <port:A-I> <pin: 0-15>") \

void cmd_gpio(int argc, char *argv[]);

#endif // SHELL_COMMAND_GPIO_H
