#ifndef SHELL_COMMAND_MEASURE_POWER_H
#define SHELL_COMMAND_MEASURE_POWER_H

#define UIF_CMD_MEASURE_POWER \
    DECLARE_UIF_CMD("measure_power", 0, UIF_MAX_ARGS, 0, cmd_measure_power, \
        "measure the voltage and current going through the type-c usb"      \
        " and the usb on the debug adapter board connected to UDC\t", "") \

void cmd_measure_power(int argc, char *argv[]);

#endif // SHELL_COMMAND_MEASURE_POWER_H
