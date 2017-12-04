# DAPLink Users Guide
There are three interfaces that DAPLink provides. These are drag-n-drop programming, a serial port and debugging support. In addition DAPLink firmware can be updated using the drag-n-drop programming interface of the bootloader.


## Drag-n-drop Programming
Program the target microcontroller by copying or saving a file in one of the supported formats to the DAPLink drive. Upon completion the drive will re-mount. If a failure occurs then the file ``FAIL.TXT`` will appear on the drive containing information about the failure.

Supported file formats:
* Raw binary file
* Intel Hex


## Serial Port
The serial port is connected directly to the target MCU allowing for bidirectional communication. It also allows the target to be reset by sending a break command over the serial port.

Supported baud rates:
* 9600
* 14400
* 19200
* 28800
* 38400
* 56000
* 57600
* 115200

Note - Most DAPLink implementations support other baud rates in addition to the ones listed here.


## Debugging

Debugging can be done with any IDE that supports the CMSIS-DAP protocol. Some tools capable of debugging are:
* [pyOCD](https://github.com/mbedmicro/pyOCD)
* [uVision](http://www.keil.com/)
* [IAR](https://www.iar.com/)



## Firmware Update

To update the firmware on a device hold the reset button while attaching USB. The device will boot into bootloader mode. From there copy the appropriate firmware onto the drive. If successful the device will leave bootloader mode and start running the new firmware. Otherwise the bootloader will display ``FAIL.TXT`` with an explanation of what went wrong.
