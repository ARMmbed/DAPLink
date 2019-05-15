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

## No MSD builds

MSD or drag-and-drop support is automatically detected and enabled when the target flash algorithm is in the DAPLink build. But there are builds that does not have a particular target and can retarget any families which have DAPLink support by using a debugger or pyocd without the MSD DAPLink drive. The script `tools/post_build_script.py` can take a board id, family id, flm elf or axf file as flash algo bin and bind it to a DAPLink binary build. The ram boundaries also needed to be specified as parameters. This is useful in all-family builds which has no board target originally but can target all existing families and retarget a particular target. The DAPLink MSD drive will be automatically enabled when there is a flash algoritm embedded in the binary file. The reverse use case of disabling the MSD even with a target support can also be done, please see `docs/MSD_COMMANDS.md` for details.
