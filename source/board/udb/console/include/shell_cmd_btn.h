#ifndef SHELL_CMD_BTN_H
#define SHELL_CMD_BTN_H

#define UIF_CMD_BTN_MIN_ARGS  (2)
#define UIF_CMD_BTN_MAX_ARGS  (2)

#define UIF_CMD_BTN \
    DECLARE_UIF_CMD("btn", UIF_CMD_BTN_MIN_ARGS, UIF_CMD_BTN_MAX_ARGS, 0, cmd_btn, \
        "The UDB board has a number of buttons to control gpio lines going to the DUTs. Use this command to remotely press the button.\n", "<btn-name> <press|release|tap>\n" \
        "    <btn-name> - can be RST0_L, BOOT0_L, BTN0_L, RST1, BOOT1, BTN1\n" \
        "    RST0_L   - reset button for DUT 0 (active low)\n" \
        "    BOOT0_L  - boot  button for DUT 0 (active low)\n" \
        "    BTN0_L   - user  button for DUT 0 (active low)\n" \
        "    RST1     - reset button for DUT 1 (active high)\n" \
        "    BOOT1    - boot  button for DUT 1 (active high)\n" \
        "    BTN1     - user  button for DUT 1 (active high)") \

void cmd_btn(int argc, char *argv[]);

#endif // SHELL_CMD_BTN_H
