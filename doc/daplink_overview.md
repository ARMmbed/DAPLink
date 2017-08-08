# Overview

DAPLink is a platform-independent firmware project created for advanced and rapid development of an ARM-based embedded systems. You can often find DAPLink on a development boards as this tiny circuit for USB flashing and debug. You can also make one yourself because DAPLink can be cross-compiled to run on a different ARM MCU chips. In any case, DAPLink is your best friend when it comes to efficient embedded systems development..



	+---------+       +---------+       +----------------+
	|         |  USB  |         |  HIC  |                |
	| HOST PC <-------> DAPLink <-------> TARGET ARM CPU |
	|         |       |         |       |                |
	+---------+       +---------+       +----------------+

DAPLink Debug Board connects to a PC over USB port. At the same time it also connects to Target CPU using dedicated Hardware Interface Circuit (HIC) physical connection providing JTAG/SWD, UART, and other signals necessary for various development tasks such as memory programming, internals access, or debugging.

DAPLink adopted standards to bring you painless out-of-the-box experience. With a single USB connection it provides several virtual function-specific devices, which you can use all at the same time:
* **USB MSC** provides firmware flashing, from a computer to USB-Disk like copy, with a single file drag-n-drop operation.
* **USB CDC** provides logging, tracing, or terminal interaction with Target MCU UART over USB Virtual-COM-Port.
* **USB HID** provides debugging in accordance to CMSIS-DAP standard.

There are different Host Tools / IDE / SDK that allows you to work with favorite ARM CPU using DAPLink from your Personal Computer. It is not only possible to upload new firmware to a target board with a simple file copy-paste, but you can also talk over terminal with the board, or search for bugs and improvements using live debug session tracing step by step source code and/or assembly execution on a remote electronic board just as it was local software. In case you did not choose your favorite SDK yet please consider:
* [ARM mbedOS SDK+IDE][mbed] (Free both Online and Offline).
* [Keil uVision IDE][uvision] (Commercial).
* [IAR Embedded Workbench IDE][iar] (Commercial).
* GNU/BSD Free-and-Open-Source-Software utilities ([Eclipse][eclipse], [pyOCD][pyocd], [GNU ARM Embedded Toolchain][gnuarm], etc).

[mbed]: https://developer.mbed.org "arm mbed developer resources"
[uvision]: http://www2.keil.com/mdk5/uvision/ "Keil uVision IDE"
[iar]: https://www.iar.com/iar-embedded-workbench/ "IAR Embedded Workbench IDE"
[eclipse]: https://docs.mbed.com/docs/mbed-os-handbook/en/latest/debugging/debugging_eclipse_pyocd/ "Debugging mbed OS 5 applications with Eclipse"
[pyocd]: https://github.com/mbedmicro/pyOCD "Open-Source Python Library for Programming and Debugging ARM Cortex-M microcontrollers using CMSIS-DAP"
[gnuarm]: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm "GNU ARM Embedded Toolchain"
