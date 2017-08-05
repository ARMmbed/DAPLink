# DAPLink

<a href="https://developer.mbed.org" tagret="_blank"><img align="right" width="256" src="docs/gfx/arm_mbed_lockup_horizontal.png" /></a>

Welcome to the
[ARM][arm]
[mbed][mbed]
[DAPLink][daplink] Free-and-Open-Source-Software project!

DAPLink is a versatile solution that allows rapid from scratch development of ARM-based embedded systems at no cost. It is a bridge between target MCU and host SDK. Using single USB connection, DAPLink provides Drag-And-Drop firmware programming (so called "flashing"), CMSIS-DAP based debugging, as well as easy interaction with target over Virtual-COM-Port based UART. More features are planned and will show up gradually as project is constantly under heavy development, coordinated by founding [ARM Limited][arm], supported by numerous hardware vendors and the Open-Source community around the world. DAPLink has superseded the "[mbed CMSIS-DAP][cmsisdap] interface firmware" project. You are free to use and contribute. Enjoy!

Official DAPLink Project Website and GitHub Repository is located at [https://github.com/mbedmicro/DAPLink][daplink].


## Table of Contents

 * [Overview](#overview)
 * [Architecture](#architecture)
 * [Releases](#releases)
 * [Compatibility](#compatibility)
 * [Development](#development)
 * [Documentation](#documentation)


## Overview

DAPLink is a platform-independent firmware project created for advanced and rapid development of an ARM-based embedded systems. You can often find DAPLink on a development boards as this tiny circuit for USB flashing and debug. You can also make one yourself because DAPLink can be cross-compiled to run on a different ARM MCU chips. In any case, DAPLink is your best friend when it comes to efficient embedded systems development..



	+---------+       +---------+       +----------------+
	|         |  USB  |         |  HIC  |                |
	| HOST PC <-------> DAPLink <-------> TARGET ARM CPU |
	|         |       |         |       |                |
	+---------+       +---------+       +----------------+

DAPLink Debug Board connects to a PC over USB port. At the same time it also connects to Target CPU using dedicated Hardware Interface Circuit (HIC) physical connection providing JTAG/SWD, UART, and other signals necessary for various development tasks such as memory programming, internals access, or debugging.

DAPLink adopted standards to bring you painless out-of-the-box experience. With a single USB connection it provides several virtual function-specific devices, which you can use all at the same time:
* **USB MSC** or **UMS** provides firmware flashing, from a computer to USB-Disk like copy, with a single file drag&drop operation.
* **USB CDC** provides logging, tracing, or terminal interaction with Target MCU UART over USB Virtual-COM-Port.
* **USB HID** provides debugging in accordance to CMSIS-DAP standard.

There are different Host Tools / IDE / SDK that allows you to work with favorite ARM CPU using DAPLink from your Personal Computer. It is not only possible to upload new firmware to a target board with a simple file copy-paste, but you can also talk over terminal with the board, or search for bugs and improvements using live debug session tracing step by step source code and/or assembly execution on a remote electronic board just as it was local software. In case you did not choose your favorite SDK yet please consider:
* [ARM mbedOS SDK+IDE][mbed] (Free both Online and Offline).
* [Keil uVision IDE][keil] (Commercial).
* [IAR Embedded Workbench IDE][iar] (Commercial).
* GNU/BSD Free-and-Open-Source-Software utilities ([Eclipse][eclipse], [pyOCD][pyocd], [GNU ARM Embedded Toolchain][gnuarm], etc).

For more details on how to use DAPLink please take a look at [The DAPLink User Guide.](docs/USERS-GUIDE.md)


## Architecture

As stated in the Overview section above, the DAPLink is a platform-independent firmware project created to allow free and rapid from-scratch development of the ARM CPU based embedded systems and devices. Generic DAPLink Architecture within its common environment are presented on a diagram below.

	 End-User Workstation
	+----------------------------------+        +---------------------------------+          +----------------------+
	| STANDARD OS (macOS/Linux/Windows)|        |                                 |          |                      |
	| * Firmware Upgrade (Drag&Drop)   |        | +-----------------------------+ |          |  +----------------+  |
	| * Scripted Automation Possible   |        | |                             | |          |  |                |  |
	+----------------------------------+        | |  +-----------------------+  | |          |  |  FLASH / RAM   |  |
	            ( . . . )                       | |  |MASS STORAGE (FLASHING)<----------+    |  |                |  |
	            ( . . . )                       | |  +-----------------------+  | |     |    |  +----------------+  |
	+----------------------------------+        | |  +-----------------------+  | |     |    |  +----------------+  |
	|                                  |        | |  |HID DEVICE  (DEBUGGING)<----------v-------> ARM Cortex CPU |  |
	| ONLINE/OFFILNE ARM EMBEDDED SDK  |        | |  +-----------------------+  | | SWD/JTAG |  +----------------+  |
	| * Source Code Editor             |        | |  +-----------------------+  | |          |  +----------------+  |
	| * Target MCU Configuration       |        | |  |VIRTUAL COM PORT (UART)<------------------>   PERIPHERALS  |  |
	| * Target BOARD Configuration     |        | |  +-----------------------+  | |   UART   |  +----------------+  |
	| * Peripherals Support (HAL)      |  USB   | |                             | |          |                      |
	| * User Libraries                 <--------> |   DAPLink_if (INTERFACE)    | |          +----------------------+
	| * ARM mbedOS                     |        | +-----------------------------+ |            Target MCU / System
	| * Compiler                       |        |                                 |
	| * Linker                         |        | +-----------------------------+ |
	| * Firmware (BIN/HEX)             |        | |   DAPLink_bl (BOOTLOADER)   | |
	| * Debugger (ELF)                 |        | +-----------------------------+ |
	|                                  |        |        DAPLink Firmware         |
	+----------------------------------+        +---------------------------------+
	 Developer Workstation                                 DAPLink HIC


## Releases

This Open-Source repository provides a source code for different variations of DAPLink implementation, along with boards configuration, documentation, build scripts, and testing framework. For most users only the final result of our work is important, therefore precompiled-binary-ready-for-use images are provided in a form of Release Package

Firmware image files are named in a manner that allows unique identification of the RELEASE, HIC, BOARD, TARGET, so you will easily know what firmware image matches your hardware. 

You can download Release Packages navigating to [RELEASES][daplinkreleases] link located at the top of the project website. Release Package will allso contain Release Notes with useful and important information on new features, bugfixes, problems and solutions, therefore it is usually good idea to read them before firmware upgrade your DAPLink.

Each Release has its own unique incremental build number as well as build ID. Release usually contains new fetures or bug fixes. Sometimes, unfortunately, a bug can be identified after release has been made. In that case a standalone bugfix can be applied for affected boards. Standalone bugfixes are released after careful consideration, once reported, verified and fixed. Both quarterly and bugfix releases will result in the build number being incremented.

**Please note that products that are shipped or advertied to be compatible with DAPLink should provide detailed and board specific instructions on how to upgrade firmware using Release Packages from this website.**


## Compatibility

There are numerous different ARM MCU based Hardware Interface Circuits (HIC) that DAPLink firmware already operates on. As mentioned above these Debug Circuits can be standalone or embedded onto a bigger board (development kit or final product). Below is a list of devices known to be Compatible with DAPLink:
* Segger J-Link OB (Atmel SAM3U)
* Maxim Epsilon (MAX32550) - coming soon
* [NXP OpenSDA](http://www.nxp.com/products/software-and-tools/run-time-software/kinetis-software-and-tools/ides-for-kinetis-mcus/opensda-serial-and-debug-adapter:OPENSDA)
* [NXP Link based on LPC11U35 or LPC4322](https://www.lpcware.com/LPCXpressoBoards)

Vendors are encouraged and free to add their new boards and devices to support DAPLink!

## Development

If you are interested in development of DAPLink, please keep in mind that first you need to sign the [ARM mbed Contributor Agreement][mbedcagreement].

If you find a bug, want to send a patch, have an improvement idea, please contact project developers using [Project Issue Tracker][daplinkissues] link located at the top of the project website.

## Documentation

All important documentation, The DAPLink Handbook, is stored in the [doc][doc/] directory.


[arm]: https://www.arm.com "ARM Limited website"
[mbed]: https://developer.mbed.org "arm mbed developer resources"
[daplink]: https://github.com/mbedmicro/DAPLink "DAPLink Project Website and GitHub Repository"
[daplinkreleases]: https://github.com/mbedmicro/DAPLink/releases "DAPLink Releses"
[daplinkissues]: https://github.com/mbedmicro/DAPLink/issues "DAPLink Issue Tracker"
[pyocd]: https://github.com/mbedmicro/pyOCD "Open-Source Python Library for Programming and Debugging ARM Cortex-M microcontrollers using CMSIS-DAP"
[cmsisdap]: https://github.com/mbedmicro/CMSIS-DAP/ "ARM mbed CMSIS-DAP project"
[mbedcagreement]: https://developer.mbed.org/contributor_agreement/ "ARM mbed Contributor Agreement"
[gnuarm]: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm "GNU ARM Embedded Toolchain"
[eclipse]: https://docs.mbed.com/docs/mbed-os-handbook/en/latest/debugging/debugging_eclipse_pyocd/ "Debugging mbed OS 5 applications with Eclipse"
[keil]: http://www2.keil.com/mdk5/uvision/ "Keil uVision IDE"
[iar]: https://www.iar.com/iar-embedded-workbench/ "IAR Embedded Workbench IDE"
