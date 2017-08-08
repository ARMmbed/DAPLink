# CDC Enpoint Features

[USB CDC](daplink_dictionary.md) endpoint is a standard method for providing Virtual COM Port to the Operating System over USB.
[Virtual COM Port Drivers for Windows][mbed_winserial] needs to be installed first to use the VCP.
Unix-like operating systems (macOS, FreeBSD, Linux) should have the USB CDC support implemented by default.
After setup you can connect your DAPLink to your PC USB port and you have a serial port connection with your Target established!

[mbed_winserial]: https://developer.mbed.org/handbook/Windows-serial-configuration "ARM mbed Windows Virtual COM Port configuration page"
