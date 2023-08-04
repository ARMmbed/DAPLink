#ifndef SHELL_COMMAND_PWM_H
#define SHELL_COMMAND_PWM_H

#define UIF_CMD_PWM \
    DECLARE_UIF_CMD("pwm", 1, UIF_MAX_ARGS, 0, cmd_pwm, \
        "PWM utility", \
        "start <freq Hz> <duty_cycle percentage>|stop - Start to output TIM1 CH1 1.8V PWM to GPIOE 9 pin or stop it\n") \

void cmd_pwm(int argc, char *argv[]);

#endif // SHELL_COMMAND_PWM_H
