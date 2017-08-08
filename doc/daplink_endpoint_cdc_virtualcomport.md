# Virtual COM Port

Hardware Serial Port is connected directly between DAPLink HIC and the target MCU UART allowing bidirectional communication.
DAPLink HIC also exposes Virtual COM Port (VCP) over USB to the Host Computer.

This allows Terminal Emulation over USB port for your Target device, which may be used for logging, tracing, interaction and command line control (CLI).
It is also possible to reset Target MCU by sending a `break` command over from Host Computer.

Supported UART baud rates:
* 9600
* 14400
* 19200
* 28800
* 38400
* 56000
* 57600
* 115200

*Note: Most DAPLink implementations may also support other baud rates than listed above.*
