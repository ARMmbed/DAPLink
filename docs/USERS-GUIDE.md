# DAPLink users guide

DAPLink provides three interfaces. These are drag-and-drop programming, a serial port and debugging support. In addition, you can update DAPLink firmware using the drag-and-drop programming interface of the bootloader.

## Drag-and-drop programming

Program the target microcontroller by copying or saving a file in one of the supported formats to the DAPLink drive. Upon completion, the drive remounts. If a failure occurs, the file `FAIL.TXT` appears on the drive containing information about the failure.

Supported file formats:

- Raw binary file.
- Intel Hex.

## Serial port

The serial port is connected directly to the target MCU allowing for bidirectional communication. It also allows the target to be reset by sending a break command over the serial port.

Supported baud rates:

- 9600.
- 14400.
- 19200.
- 28800.
- 38400.
- 56000.
- 57600.
- 115200.

Note: Most DAPLink implementations support other baud rates in addition to the ones listed here.

## Debugging

You can debug with any IDE that supports the CMSIS-DAP protocol. Some tools capable of debugging are:

- [pyOCD](https://github.com/mbedmicro/pyOCD).
- [uVision](http://www.keil.com/).
- [IAR](https://www.iar.com/).

## Firmware update

To update the firmware on a device, hold the reset button while attaching USB. The device boots into bootloader mode. From there, copy the appropriate firmware onto the drive. If successful, the device leaves bootloader mode and starts running the new firmware. Otherwise, the bootloader displays `FAIL.TXT` with an explanation of what went wrong.
