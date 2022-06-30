#ifndef SHELL_COMMAND_I2C_H
#define SHELL_COMMAND_I2C_H

#define UIF_CMD_I2C \
    DECLARE_UIF_CMD("i2c", 2, UIF_MAX_ARGS, 0, cmd_i2c, \
        "I2C2 utility", "probe <bus>|<bus> <address> <startreg> <len> [<data>]\n" \
        "    i2c probe <bus> - Probe for slaves\n" \
        "    i2c <bus> <address> <startreg> <len> [<data>] - Send and receive bits\n" \
        "\t<bus> - i2c bus (UDB only supports bus 2)\n" \
        "\t<address> - Chip address\n" \
        "\t<startreg> - Register addr to start transfer at\n" \
        "\t<len> - Number of bytes to xfer(read max: 32, write max: 4)\n" \
        "\t<data> - Data to be sent. Omit if reading") \

void cmd_i2c(int argc, char *argv[]);

#endif // SHELL_COMMAND_I2C_H
