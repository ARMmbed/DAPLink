# The DAPLink Handbook

<a href="https://developer.mbed.org" tagret="_blank"><img align="right" width="256" src="gfx/arm_mbed_lockup_horizontal.png" /></a>

Welcome to the
[ARM][arm]
[mbed][mbed]
[DAPLink][daplink] Handbook!

DAPLink is a versatile solution that allows rapid from scratch development of ARM-based embedded systems at no cost. It is a bridge between target MCU and host SDK. Using single USB connection, DAPLink provides Drag-n-Drop firmware programming (so called "flashing"), CMSIS-DAP based debugging, as well as easy interaction with target over Virtual-COM-Port based UART. More features are planned and will show up gradually as project is constantly under heavy development, coordinated by founding [ARM Limited][arm], supported by numerous hardware vendors and the Open-Source community around the world. DAPLink has superseded the "[mbed CMSIS-DAP][cmsisdap] interface firmware" project. You are free to use and contribute. Enjoy!

This document is designed to be all-in-one information source for both DAPLink Users and Developers. Whether online or if you want to print it as an offline reference, here you will find:
* User Guide: important information to make best use of all DAPLink features in your project.
* Developer Guide: important information if you want to contribute with new features, new/custom hardware, fix a bug, or simply build and test DAPLink on your own.

If you find bugs or think some information is missing here do not hesitate to let us know.
Official DAPLink Project Website and GitHub Repository is located at [https://github.com/mbedmicro/DAPLink][daplink].


## Table of Contents

* [Hello DAPLink](#hello_daplink)
    * [Overview](#overview)
    * [Architecture](#architecture)
    * [Releases](#releases)
    * [Compatibility](#compatibility)
* [DAPLink User Guide](#daplink_user_guide)
    * [UMS Endpoint Features](#ums_endpoint)
        * [Drag-n-Drop Flashing](#ums_dragndrop_flashing)
        * [UMS Commands](#ums_commands)
            * [Automation Mode](#ums_automation_mode)
            * [Action Commands](#ums_action_commands)
            * [Configuration Commands](#ums_configuration_commands)
    * [CDC Endpoint Features](#cdc_endpoint)
        * [Virtual COM Port](#cdc_virtual_com_port)
    * [HID Endpoint Features](#hid_endpoint)
        * [Debugging](#hid_debugging)
    * [Firmware Update](#firmware_update)
* [DAPLink Developer Guide](#daplink_developer_guide)
    * [Development Environment Setup](#development_environment_setup)
    * [Building DAPLink](#building_daplink)
    * [Porting New Hardware](#porting_new_hardware)
        * [Adding New Target](#porting_add_new_target)
        * [Adding New Board](#porting_add_new_board)
        * [Adding New Target/Board to Automated Test Suite](#porting_add_target_board_to_testing)
    * [Testing](#testing)
        * [Testing Target UMS/CDC](#testing_target)
        * [Testing HID / CMSIS-DAP](#testing_hid)
    * [Building a Release](#release_build)
        * [Building Bootloader and Firmware Bundle](#build_release_bundle)
    * [Supported IDE/SDK](#supported_ide_sdk)
* [Dictionary of Abbreviations](#dictionary)


## <a name="hello_daplink"></a>Hello DAPLink

### Overview

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

For more details on how to use DAPLink please take a look at [The DAPLink User Guide.](docs/USERS-GUIDE.md)


### Architecture

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


### Releases

This Open-Source repository provides a source code for different variations of DAPLink implementation, along with boards configuration, documentation, build scripts, and testing framework. For most users only the final result of our work is important, therefore precompiled-binary-ready-for-use images are provided in a form of Release Package

Firmware image files are named in a manner that allows unique identification of the RELEASE, HIC, BOARD, TARGET, so you will easily know what firmware image matches your hardware. 

You can download Release Packages navigating to [RELEASES][daplinkreleases] link located at the top of the project website. Release Package will allso contain Release Notes with useful and important information on new features, bugfixes, problems and solutions, therefore it is usually good idea to read them before firmware upgrade your DAPLink.

Each Release has its own unique incremental build number as well as build ID. Release usually contains new fetures or bug fixes. Sometimes, unfortunately, a bug can be identified after release has been made. In that case a standalone bugfix can be applied for affected boards. Standalone bugfixes are released after careful consideration, once reported, verified and fixed. Both quarterly and bugfix releases will result in the build number being incremented.

**Please note that products that are shipped or advertied to be compatible with DAPLink should provide detailed and board specific instructions on how to upgrade firmware using Release Packages from this website.**


### Compatibility

There are numerous different ARM MCU based Hardware Interface Circuits (HIC) that DAPLink firmware already operates on. As mentioned above these Debug Circuits can be standalone or embedded onto a bigger board (development kit or final product). Below is a list of devices known to be Compatible with DAPLink:
* Segger J-Link OB (Atmel SAM3U)
* Maxim Epsilon (MAX32550) - coming soon
* [NXP OpenSDA](http://www.nxp.com/products/software-and-tools/run-time-software/kinetis-software-and-tools/ides-for-kinetis-mcus/opensda-serial-and-debug-adapter:OPENSDA)
* [NXP Link based on LPC11U35 or LPC4322](https://www.lpcware.com/LPCXpressoBoards)

Vendors are encouraged and free to add their new boards and devices to support DAPLink!




## <a name="daplink_user_guide"></a>DAPLink User Guide

This part of Handbook is intended for Users that want to get familiar with all features offered by the DAPLink Project.

### <a name="ums_endpoint"></a>UMS Endpoint Features

[USB MSC or simply UMS](#dictionary) is a standard for providing access to Mass Storage devices over USB. Just plug in the DAPLink to your PC USB port to see a new hard drive installed. You are now ready to flash firmware into a target system!

#### <a name="ums_dragndrop_flashing"></a>Drag-n-drop Flashing
You can program target board by simply storing a firmware file to a virtual USB hard drive exposed by DAPLink UMS (USB Mass Storage) interface.
You will see a `DAPLINK` labelled hard drive on your host computer after connecting DAPLink do USB port.
You can use CP, Copy-Pase, Drag-n-Drop, or Save As, which method you like most to transfer files.

Storing Firmware into Microcontroller Flash Memory (so called flashing) will start as soon as file is put on the USB drive.
You will notice a DAPLink HIC LED blinking during the write operation.
As soon as write ends, USB drive will re-mount (disappear and reappear) in your host computer.
If the write operation fails for some reason, a `FAIL.TXT` text file will show up on the USB drive and it will contain information on failure cause (you can open and read this file with standard text editor like `Notepad` or `ViM`).


DAPLink supports following firmware file formats:
* Raw Binary (`*.BIN`)
* Intel Hex (`*.HEX`)

**Note:** By default your **Target will be left Halted for Debugging after Flashing** (you can change this behavior with [AUTO RESET MODE](#ums_configuration_commands) configuration command).

#### <a name="ums_commands"></a>UMS Commands

DAPLink allows you to execute simple commands using USB Mass Storage (UMS) interface.
You can copy a file to the root of the DAPLink UMS drive to make DAPLink perform an action or configure a flash-persisted option.
So far, the contents of the file is ignored, therefore you can simply pass an empty file.

UMS Commands only work if you are either holding down the RESET button on the DAPLink HIC while copying the file, or if you have turned on DAPLink `AUTOMATION MODE` (see [Automation Mode](#ums_automation_mode) section).
The Automation Mode is more convenient if you intend to use UMS Commands quite often or remotely, because it is preserved between reboots, so no manual interaction with the board is necessary.

By convention, files that end with `.ACT` trigger an action, wile files that end with `.CFG` configure a behavior.
The mechanism is case-insensitive (you can use either lowercase or uppercase letters).

Every command completes by remounting the DAPLink UMS drive.
Every command is supported in both interface and bootloader mode.
In fact, any file (create, delete, write) operation could be used to trigger a DAPLink command, if the handler is created in the code.


##### <a name="ums_automation_mode"></a>Automation Mode

`AUTOMATION MODE` allows you to leverage UMS Commands mechanism without manual interaction with the Board (pressing RESET each time you want to execute a UMS Command), which comes really handy in remote work and testing.
In order to enable Automation Mode:
* Update the interface software to a version at or above `0241`.
* Copy an empty text file names `AUTO_ON.CFG` to the DAPLink UMS drive while the reset button is pressed.
* Release the reset button when the drive unmounts. When it re-mounts, confirm `Automation Allowed` is set to `1` in the `DETAILS.TXT` file.
* Update the bootloader software to a version at or above `0241`.
* Confirm that creating `START_BL.ACT` file causes the device to enter `BOOTLOADER` mode, and that creating `START_IF.ACT` file causes the device to enter `INTERFACE` mode.

**Note:** This `AUTOMATION MODE` is mandatory for Bootloader updates.

##### <a name="ums_action_commands"></a>Action Commands

`START_BL.ACT` This file will force DAPLink to remount in `BOOTLOADER MODE`.
It is equivalent to unplugging the USB cable, and plugging it back in while holding the RESET button.
This command has no effect if DAPLink is already in bootloader mode.

`START_IF.ACTt` This file will force DAPLink to remount in `INTERFACE MODE`.
It is equivalent to unplugging the USB cable and plugging it back in.
This command has no effect if DAPLink is already in interface mode.

`ASSERT.ACT` This file can be used to enable DAPLink's `ASSERT UTILITY`, a very handy code testing feature.
When you copy this file to the DAPLink USB Mass Storage drive, DAPLink generates a call to the `util_assert()` method.
An assert call causes the DAPLink UMS drive to remount with an additional file `ASSERT.TXT` created.
This file details where the assertion failure occurred in the code (source file, line number).
An assert call is a no-op if there is already an outstanding assert failure (there can only be one assertion).
The outstanding assertion failure can be cleared by deleting the `ASSERT.TXT` file (if you do so DAPLink will re-mount).
Deleting `ASSERT.TXT` is actually a DAPLink UMS command with a separate handler.

`REFRESH.ACT` This file forces a remount of the DAPLink UMS drive

`ERASE.ACT` This file triggers an erase of the Target Flash Memory.

##### <a name="ums_configuration_commands"></a>Configuration Commands

`AUTO_RST.CFG` This file will turn on `AUTO RESET MODE`.
In this mode Target Reset is performed at the end of the Target Flash write sequence.
You can use it to start your program just after flashing is finished.
**By default Target will be left Halted for Debugging after Flashing**
This mode does not apply for DAPLink HIC firmware upgrade from Bootloader.

`HARD_RST.CFG` This file turns off the `AUTO RESET MODE`.
Auto Reset mode is off by default.

`AUTO_ON.CFG` This file turns on the `AUTOMATION MODE`.
In this mode, DAPLink UMS commands can be triggered without needing to hold down the RESET button.
Also, bootloader updates are allowed only when automation mode is enabled.

`AUTO_OFF.CFG` This file turns off the `AUTOMATION MODE`.
This mode is turned off by default.

`OVFL_ON.CFG` This file turns on serial overflow reporting.
If the host PC is not reading data fast enough from DAPLink and an overflow occurs the text `<DAPLink:Overflow>` will show up in the serial data.
Serial overflow reporting is turned off by default.

`OVFL_OFF.CFG` This file turns off serial overflow reporting (back to default).


### <a name="cdc_endpoint"></a>CDC Enpoint Features

[USB CDC](#dictionary) endpoint is a standard method for providing Virtual COM Port to the Operating System over USB.
[Virtual COM Port Drivers for Windows][mbed_winserial] needs to be installed first to use the VCP.
Unix-like operating systems (macOS, FreeBSD, Linux) should have the USB CDC support implemented by default.
After setup you can connect your DAPLink to your PC USB port and you have a serial port connection with your Target established!


#### <a name="cdc_virtual_com_port"></a>Virtual COM Port

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


### <a name="hid_endpoint"></a>HID Endpoint Features

[USB HID](#dictionary) has been selected as the transport layer for DAPLink features mainly because it needs no additional drivers to be installed on the user PC in order to work.
HID is also amazingly platform independent standard.
Just connect the DAPLink, launch the favorite debugger that supports DAPLink/CMSIS-DAP, and it works!

#### <a name="hid_debugging"></a>Debugging

Debugging allows you to trace step by step program execution on your Target MCU, setting breakpoints, watching variables, memory modification, etc.

You usually debug a Target System using Remote GDB Protocol. Dedicated GDB Server Application is connected to a physical device over DAPLink USB HID interface at one side, and provides standard Remote GDB Protocol where you can connect with Host Computer over TCP/IP from your favorite Debugger.
This not only allows you to connect to Target remotely over a network connection, but also makes it possible to debug embedded microcontroller firmware as it was local running program.
In most cases you will simply start GDB Server on your local machine also running IDE/SDK and Debugger.. but remote debugging is a nice feature to have.

Here are most common applications that can both talk to DAPLink via USB-HID + CMSIS-DAP and provide remote GDB Server for you to connect the Debugger, or simply provide all-in-one solution for embedded debug with no details involved:
* [pyOCD][pyocd] - Open-Source Remote GDB Server for DAPLink written in Python.
* [uVision][uvision] - Commercial All-In-One solution for embedded systems development (including Debug).
* [IAR][iar] - Commercial All-In-One solution for embedded systems development (including Debug).

*Note: Please remember that you need an ELF file for Debugger in order to debug at Source Code level (not the assembly only as in case of BIN/HEX files). This is important when you do not build the whole project from the sources yourself but only got the firmware image to test.*

### <a name="firmware_update"></a>Firmware Update

It is possible, and sometimes necessary, to firmware upgrade your DAPLink device.
**By holding the RESET button on your DAPLink board at Power-ON or USB-attach you will trigger a Bootloader mode** (USB drive will now have `BOOTLOADER` label).
This will allow firmware upgrade of the DAPLink itself.

Just as with firmware flashing, copy DAPLink firmware onto USB drive when in Bootloader mode and wait for the write process to finish.
On success device will reboot and start new Daplink firmware (`DAPLINK` USB drive label).
On failure you will see a `FAIL.TXT` file on a DAPLink USB drive with an explanation of what went wrong (you can open and read this file with standard text editor like `Notepad` or `ViM`).

*Note: With a simple HIC devices DAPLink consists of only of one firmware file (_IF suffix). On more advanced devices two files are provided for Booloader (_BL file suffix) and the Interface Firmware (_IF file suffix) - in that case you need to upgrade Interface Firmware using Bootloader, but you can only upgrade Bootloader using Interface Firmware (just as it was target firmware) because the bootloader cannot update itself replacing the memory it runs from..*




## <a name="daplink_developer_guide"></a>DAPLink Developer Guide

This part of the Handbook is intended for Developers that want to controbute to the DAPLink Project.

### <a name="development_environment_setup"></a>Development Environment Setup

In order to compile build and test DAPLink from the source code you need to install all necessary tools first, so called dependencies, which are:
* [Python 2.7][python] is a platform independent free and open programming language (get the most up tp date version along with `pip` and `virtualenv`).
* [Git][git] is a distributed version control system for collaborative remote source code management.
* [Keil MDK-ARM][uvision] is a commercial high quality Embedded ARM SDK that we use at the moment to build the DAPLink firmwares. Use version 5 as preferred. Set environment variable "UV4" to the absolute path of the UV4 executable if you don't install to the default location. Note that "UV4" is what's used for both MDK versions 4 and 5.

Now in few steps we will create a working directory and set it up to contain the source code from remote git repository along with other necessary components. Maybe we will create an automation script one day that will make it for you, but it is really good to know the tools you are going to work with:

**Step 1:** Get the DAPLink Source Code using GIT:

	$ cd <your_project_location>
	$ git clone https://github.com/mbedmicro/DAPLink DAPLink.git
	$ cd DAPLink.git

**Step 2:** Create a Python Virtual Environment, then activate it:

	$ pip install virtualenv
	$ virtualenv27 venv
	$ source venv/bin/activate

This Virtual Environment is a nice concept to create local copy of whole Python environment to work with, modify packages set, and then remove if necessary, with no impact on system-wide Python installation. In case anything goes wrong we can easily remove and re-create virtual environment without messing up whole operating system. Just remember that you need to `activate` it first, otherwise you will be using defaul system Python interpreter, and `deactivate` after:

	$ which python
	/opt/local/bin/python
	$ python
	Python 3.5.3 (default, Apr 23 2017, 18:09:27)
	[GCC 4.2.1 Compatible Apple LLVM 8.0.0 (clang-800.0.42.1)] on darwin
	Type "help", "copyright", "credits" or "license" for more information.
	>>> ^D>>>
	
	$ source venv/bin/activate
	(venv) $ which python
	/<your_working_directory>/venv/bin/python
	(venv) $ python
	Python 2.7.13 (default, Apr 23 2017, 16:50:35)
	[GCC 4.2.1 Compatible Apple LLVM 8.0.0 (clang-800.0.42.1)] on darwin
	Type "help", "copyright", "credits" or "license" for more information.
	>>>

Depending on the Operating System you are using the names and invocation method of the `activate` and `deactivate` scripts may differ:
* `source venv/bin/activate` (use for Linux/Unix bash).
* `venv/Scripts/activate` (use for Windows).

**Step 3:** Update Python tools and install required libraries/packages: 

	$ pip install -r requirements.txt
	
**Step 4:** Generate set of Keil uVision project files for each supported variation of DAPLink firmware. **This should be done every time you pull new changes from a git repository!**

	$ progen generate -t uvision
	

**Step 5.** You are ready to make your own work now. If you intend to make modifications to the project, please create your own [Fork at GitHub][github_fork], then commit local changes, push them to your fork, then [make a Pull Request to the Upstream project][github_pullrequest]. Pull requests should be made once a changeset is [rebased onto Master][git_merge_vs_rebase].

**Note:** We are currently using Keil uVision MDK 5 to build DAPLink firmware because of high quality of the produced code and small footprint of the binary.


### <a name="building_daplink"></a>Building DAPLink

Building a DAPLink target produces several variants of the build output. Files with the following names will be located in the project build directory after a successful build:

| Firmware Image File | Description |
| :---: | --- |
| `*_if.bin` | Base file created by build, should not be used.|
| `*_if_crc.bin` | Preferred image for new boards.  Does not work with some old bootloaders due to the vector table validation|
| `*_if_crc.hex` | Same as "``*_if_crc.bin``" just in hex form.|
| `*_if_crc_legacy_0x5000.bin` | Image required for existing boards with a bootloader built for an application starting at `0x5000`.|
| `*_if_crc_legacy_0x8000.bin` | Image required for existing boards with a bootloader built for an application starting at `0x8000`.|

The `tools/post_compute_crc.py` script is automatically executed at the end of each build.
It produces the variants listed above by modifying the output images in several ways.

The image `*_if_crc_legacy_0x5000.bin` is a copy of `*_if_crc_legacy_0x8000.bin` but with `0x3000` bytes of extra padding pre-pended to the image.
The first `0x40` bytes are an exact copy of the vector table and the remaining `0x2FC0` bytes are filled with `0xFF`.
This makes an image that can be loaded onto bootloaders with the old offset (`0x5000`) without requiring a complete rebuild.

Not all targets will produce the legacy output files.
The `post_compute_crc.py` script adds the checksum and CRC into the images.

DAPLink firmware binary image structure is presented below:
* **`VECTOR TABLE:`** All images must begin with a valid `NVIC` vector table. The vector table is defined by the `startup_MCUNAME.s` file for each of the `HIC HAL`. The build info fields below are set directly in the vector table definition.
* **`CHECKSUM:`** The first `7` vectors from the `NVIC` vector table (offsets `0x00-0x18`) are checksummed. The two's complement of the checksum is stored in the unused, reserved vector at offset `0x1C`. This checksum is required for certain NXP MCUs in order to boot. For other MCUs, the checksum is ignored.
* **`BUILD INFO:`** The vector table for DAPLink images contains several build info fields. These fields are placed in the unused, reserved vector table entries starting at offset `0x20`.
The information here is as follows (in order):
    * **`DAPLINK_BUILD_KEY:`** Key used to indicate if the image is bootloader or interface.
    * **`DAPLINK_HIC_ID:`** Key used to determine hardware compatibility. This ensures that for example a bootloader update mean for a `k20dx` could never be loaded on a `kl26z` or any other chip
    * **`DAPLINK_VERSION:`** Software version. Meant for checking for incompatible legacy versions. Currently it is not used for this purpose and is just displayed in `details.txt`.
This change caused a problem with some old bootloaders, which require the `3` vector table entries starting at `0x20` to be set to `0`. To overcome this the build scripts create the legacy images which zero out `0x20` and recompute the checksum.  If you compare `*_if_crc.bin` and `*_if_crc_legacy_0x8000.bin` you should be able to see this difference.
The corresponding struct in the code for these fields is `daplink_info_t`, defined in `daplink.h`. Constants with the valid values for these fields are also defined in `daplink.h`.
* **`CRC:`** A `CRC-32` is computed over the base output file (`*_if.bin`) and stored in the last `4` bytes of the image in **little endian** byte order. This produces the `*_if_crc.bin` file.
At the moment the interface `CRC` isn't actually enforced so existing and interface images without a `CRC` can still be loaded.


## <a name="porting_new_hardware"></a>Porting New Hardware

Because DAPLink is an Open-Source project, you can port it to work on/with new hardware. You can include DAPLink to your own design or make it work with an ARM based device that is not yet supported. Following sections contain detailed information on porting:
* **Target** is an MCU that you want to flash and debug using DAPLink.
* **HIC** is an MCU connected to a Target that will run DAPLink firmware on it.
* **Board** is usually a Target based device with various peripherals included for operations (i.e. Development Kit that may have Debugger included) that you can create a custom DAPLink firmware for.


### <a name="porting_add_new_target"></a>Adding New Target

In order to add new Target, which needs to be ARM based MCU, you need to create at least 3 files located in `source/target/<vendor>/<target>` directory:
* `target.c` defines MCU internals, such as RAM/Flash organization and layout, memory programming routines, etc.
* `target_reset.c` contains reset and other handy routines that can control Target to work with DAPLink.
* `flash_blob.c` is the code that, when uploaded into RAM, allows rapid Flash programming.

As you can see there is no Debug code specific to a Target, because that part is done by generic DAP (Debug Access Port) accessed over JTAG or SWD, which is common to all ARM Cortex devices.

Below a `source/targets/<vendor>/<targetname>/target.c` is presented This file contains information about the size of ROM, RAM and sector operations needed to be performed on the target MCU while programming an image across the drag-n-drop channel:

```c
/**
 * @file    target.c
 * @brief   Target information for the target MCU
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "target_config.h"

// The file flash_blob.c must only be included in target.c
#include "flash_blob.c"

// target information
target_cfg_t target_device = {
    .sector_size    = 4096,
    .sector_cnt     = (MB(1) / 4096),
    .flash_start    = 0,
    .flash_end      = MB(1),
    .ram_start      = 0x20000000,
    .ram_end        = 0x20010000,
    .flash_algo     = (program_target_t *) &flash,
};
```


Then you have `source/target/<vendor>/target_reset.c` that contains code for reset, setup and control Target to work with DAPLink:

```c
	/**
	 * @file    target_reset.c
	 * @brief   Target reset for the new target
	 *
	 * DAPLink Interface Firmware
	 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
	 * SPDX-License-Identifier: Apache-2.0
	 *
	 * Licensed under the Apache License, Version 2.0 (the "License"); you may
	 * not use this file except in compliance with the License.
	 * You may obtain a copy of the License at
	 *
	 * http://www.apache.org/licenses/LICENSE-2.0
	 *
	 * Unless required by applicable law or agreed to in writing, software
	 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
	 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	 * See the License for the specific language governing permissions and
	 * limitations under the License.
	 */
	
#include "target_reset.h"
#include "swd_host.h"

void target_before_init_debug(void)
{
    // any target specific sequences needed before attaching
    //	to the DAP across JTAG or SWD
    return;
}

uint8_t target_unlock_sequence(void)
{
    // if the device can secure the flash and there is a way to
    //	erase all it should be implemented here.
    return 1;
}

uint8_t target_set_state(TARGET_RESET_STATE state)
{
    // invoke reset by sw (VECT_REQ or SYS_REQ) or hw (hardware IO toggle)
    //return swd_set_target_state_sw(state);
    //or 
    return swd_set_target_state_hw(state);
}

uint8_t security_bits_set(uint32_t addr, uint8_t *data, uint32_t size)
{
    // if there are security bits in the programmable flash region
    //	a check should be performed. This method is used when programming
    //	by drag-n-drop and should refuse to program an image requesting
    //	to set the device security. This can be performed with the debug channel
    //	if needed.
    return 0;
}
```

A `source/target/<vendor>/<targetname>/flash_blob.c` contains "flash algorithm blob" required to program the target MCU internal (or external) Flash Memory. This blob contains position independent functions for erasing, reading and writing to the flash controller. Flash algorithm blobs are created from the [mbed FlashAlgo project][mbed_flashalgo].

This is quite complex and nice trick to upload a binary image of a program (shellcode?) directly into RAM memory in order to efficiently write into Flash memory a large chunks of data. You don't really want to know how slow it was before to write Flash byte-by-byte with DAP over USB (well you could take a coffee or two only with a single KB if you really want to know) :-)

```c
/**
 * @file    flash_blob.c
 * @brief   Flash algorithm for the new target MCU
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "flash_blob.h"

static const uint32_t targetname_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    0x4770ba40, 0x4770bac0, 0x4c0cb510, 0xf04068a0, 0x60a00001, 0x301af246, 0xf44f6560, 0x60e07040,
    0x20012100, 0xf83ef000, 0x65612100, 0xf02168a1, 0x60a10101, 0xbf182800, 0xbd102001, 0x400c0000,
    0x4684b510, 0xf44f2300, 0xf8505180, 0x1f092b04, 0x1c52d002, 0xe001d0f9, 0xd0131c50, 0x68a04c0c,
    0x0001f040, 0xf8c460a0, 0x2001c010, 0x200260e0, 0x210060e0, 0xf0002001, 0x4603f815, 0xf02068a0,
    0x60a00001, 0xbf142b00, 0x20002001, 0x0000bd10, 0x400c0000, 0xf6414902, 0x63c83071, 0x47702000,
    0x400c0000, 0x4b14b410, 0xc050f8df, 0x201cf8dc, 0x0f16f012, 0xf8dcd014, 0xf0200008, 0xf8cc0005,
    0xf0120008, 0xbf1e0f02, 0x0001f06f, 0x4770bc10, 0x0f04f012, 0xf04fbf1e, 0xbc1030ff, 0xe0074770,
    0x428a4002, 0xbc10d102, 0x47702000, 0xd1dd1e5b, 0xf06fbc10, 0x47700002, 0x00989680, 0x400c0000,
    0x4df0e92d, 0x1cc84604, 0x0603f020, 0x46904859, 0xf0416881, 0x60810101, 0xd9732e07, 0x0f07f014,
    0xf8d8d011, 0x61041000, 0x60c22201, 0x21086181, 0x210060c1, 0xf7ff4610, 0x2800ffb5, 0x1d24d17e,
    0x0810f108, 0x484b1f36, 0xf0416881, 0x60810104, 0xbf882e07, 0x0a02f06f, 0xf504d966, 0x46015080,
    0x010bf36f, 0x42b11b09, 0x4635bf8e, 0x000bf36f, 0xf0151b05, 0xbf180f04, 0x46471f2d, 0x20012100,
    0xff90f7ff, 0xd13b2800, 0xe0e8f8df, 0x4010f8ce, 0xf8ce2001, 0xf857000c, 0xf8ce0b04, 0xf8570018,
    0xf8ce0b04, 0x20080018, 0x000cf8ce, 0x0208f1a5, 0xd9332a07, 0x0b08f04f, 0x46dc482f, 0x101cf8de,
    0x0f16f011, 0xf8ded012, 0xf0200008, 0xf8ce0005, 0xf0110008, 0xbf180f02, 0x0001f06f, 0xf011d10f,
    0xbf180f04, 0x30fff04f, 0xe007d109, 0x010bea01, 0xd1014561, 0xe0022000, 0xd1df1e40, 0xb1104650,
    0xe00bb9e0, 0xf857e01c, 0xf8ce0b04, 0xf8570018, 0xf8ce0b04, 0x3a080018, 0xd8cb2a07, 0xeb081b76,
    0x442c0885, 0xd8982e07, 0x20012100, 0xff3af7ff, 0x4810b920, 0xf0216881, 0xe0000104, 0x6081e00f,
    0x480cb186, 0x1000f8d8, 0x22016104, 0x618160c2, 0x60c12108, 0x46102100, 0xff24f7ff, 0x2001b110,
    0x8df0e8bd, 0x68814803, 0x0101f021, 0x20006081, 0x8df0e8bd, 0x400c0000, 0x00989680, 0x68814803,
    0x0105f021, 0x20006081, 0x00004770, 0x400c0000, 0x00000000,
};

static const program_target_t flash = {
    0x200000B5, // Init
    0x2000029D, // UnInit
    0x20000029, // EraseChip
    0x20000061, // EraseSector
    0x20000121, // ProgramPage

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        0x20000000 + 0x00000001,
        0x20000000 + 0x00000020 + 0x00000290,
        0x20000800
    },

    0x20000000 + 0x00000A00,   // mem buffer location
    0x20000000,                // location to write prog_blob in target RAM
    sizeof(targetname_blob),   // prog_blob size
    targetname_blob,           // address of prog_blob
    0x00000200                 // program_buffer_size, largest size that can be written in a single call to program page
};
```

You can now add those files to a new Target description (see Adding New Board), build the code, and try it out!

**Note:** We are currently using Keil uVision MDK 5 to build DAPLink firmware because of high quality of the produced code and small footprint of the binary.


### <a name="porting_add_new_board"></a>Adding New Board

Board is composed of a Hardware Interface Circuit (HIC) and Target MCU. In order to add a new Board, you need to create its description in `projects.yaml` file. The yaml descriptions are used to create modules that can be inherited.

You can add new Board to the build system by adding the product name and all required modules to `yaml` file, i.e. if the HIC is OpenSDA based on the NXP MK20D50 MCU that would be:

	k20dx_myboardname_if:
	    - *module_if
	    - *module_hic_k20dx
	    - records/board/myboardname.yaml

Next create a new file in the `records/board` directory called `myboardname.yaml`. This file defines the target MCU and allows  board parameters to be configured. The target MCU in this example exists and is a Nordic nRF51822 (16k RAM variant):

	common:
	    sources:
	        board:
	            - source/board/myboardname.c
	        target:
	            - source/target/nordic/nrf51822/target_16.c
	            - source/target/nordic/target_reset.c

This assumes Target support is already present in the codebase (as explained in [Adding New Target section](#porting_add_new_target)). Now `source/board/myboardname.c` file needs to be created with a `BOARD ID`. In order to obtain uqnique `BOARD ID` (each board needs that) please contact support@mbed.org.

```c
/**
 * @file    myboardname.c
 * @brief   board ID for myboard
 *
 * DAPLink Interface Firmware
 * Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

const char *board_id = "0000";
```

Executing (from within a [Python Virtual Environment](#development_environment_setup)):

	progen generate -t uvision
	
will create all project files to build the DAPLink for your new Board and Target. Have fun!

If you want to read more about the `yaml` format see the [project_generator](#project_generator) documentation.

**Note:** We are currently using Keil uVision MDK 5 to build DAPLink firmware because of high quality of the produced code and small footprint of the binary.


### <a name="porting_add_target_board_to_testing"></a>Adding New Target/Board to Automated Test Suite

DAPLink source code comes with test suite that automates most of the actions performed on/by DAPLink in order to verify its proper functionality in various environments (i.e. different operating systems, targets, boards, configurations, etc).
Test suite performs intense and detailed experimentation by putting a dedicated agent firmware (so called "Target Firmware" or "UART Application") onto tested Target.
Test suite is written in Python.
**You should really add newly created Target/Board to the Automated Test Suite.**

In order for your Target/Board to be tested, you need to extend `SUPPORTED_CONFIGURATIONS` dictionary from `test/info.py` file:

```python
SUPPORTED_CONFIGURATIONS = [
    #   Board ID    Firmware                            Bootloader          Target
    ...
    (   0x240,      'k20dx_frdmk64f_if',                'k20dx_bl',         'FRDM-K64F'                     ),
    ...
]
```

where:
* `Board ID` is the unique ID assigned to the Board.
* `Firmware` is the name of the firmware as declared in `projects.yaml`.
* `Bootloader` is the name of the bootloader image as declared in `projects.yaml`. This field is only required if given HIC supports custom bootloader (not all HIC can have it).
* `Target` is the Target name related to that particular Board, but keep in mind that:
    * if this is an mbed official Board then the Target should match the name in the mbed platform page URL. For example the `K64F` is located at [https://developer.mbed.org/platforms/FRDM-K64F/][mbed_fdrm_k64f] therefore its name would be `FRDM-K64F`. This nomenclature allows automated tests to leverage RESTful API of ARM the mbed Online Compiler. The automated testing framework will build "UART Application" firmware for that Target on-the-fly using the mbed Online Compiler, download it to the PC, then flash the firmware into Target in order to perform various tests (i.e. filesystem, flashing, serial console, misconfigurations, etc).
    * if this is not an official Target/Board, you need to build the "UART Application" yourself and supply it to `test\run_tests.py` via `--targetdir` parameter. In this case, the target is the application image filename without extension.

You may need to update one or more dictionaries (see comments in the testing source code for guidance).

If you want to know more, please take a look at the [Automated Testing](#testing) section.


## Testing

We are working on an Automated Test Suite, written in [Python][python], that will perform all of the necessary steps to verify proper functionality of the DAPLink and related hardware.
Test suite performs intense and detailed experimentation by putting a dedicated agent firmware (so called "Target Firmware" or "UART Application") onto tested Target.
Yes, test suite can work both on software and hardware behaviors simulating various real life situations like (corrupted) firmware upgrade, user interactions, flashing, debug, serial port, misconfigurations, various operating system versions, etc.
We have even created a dedicated laboratory environment that performs continous stress testing for corner situations with a low probabily of occurance to detect and remove even smallest glitches!

Test logic discovers connected DAPLink boards and targets only those boards. It can be run on the current development environment or a release package. Test results are written to the current directory and printed to the console.

In order to perform testing, you should get familiar with current setup in `test/info.py` (see [Adding new Target/Board for Automated Test Suite](#porting_add_target_board_to_testing) chapter for details) and help for `test/run_test.py` (using [Python Virtual Environment](#development_environment_setup)):
```
$ python test/run_test.py --help
usage: run_test.py [-h] [--targetdir TARGETDIR] [--user USER]
                   [--password PASSWORD] [--firmwaredir FIRMWAREDIR]
                   [--firmware {k20dx_frdmkl02z_if,lpc11u35_sscity_if,lpc11u35_vbluno51_if,k20dx_frdmkl27z_if,lpc11u35_wizwiki_w7500_eco_if,kl26z_nina_b1_if,k20dx_frdmk66f_if,k20dx_frdmk20dx_if,lpc11u35_ssci1114_if,lpc11u35_arm_watch_nrf51_if,lpc11u35_nz32_sc151_if,k20dx_xdot_l151_if,k20dx_frdmke15z_if,sam3u2c_ncs36510rf_if,lpc11u35_arm_watch_stm32f411_if,lpc11u35_lpc4088dm_if,lpc11u35_gr_peach_if,k20dx_frdmkl26z_if,k20dx_frdmk28f_if,k20dx_frdmkl46z_if,lpc11u35_lpc824xpresso_if,k20dx_frdmk82f_if,lpc11u35_wizwiki_w7500p_if,kl26z_microbit_if,lpc4322_lpc54114xpresso_if,lpc11u35_hrm1017_if,lpc11u35_lpc4088qsb_if,lpc11u35_6lowpan_borderrouterhat_if,lpc11u35_cocorico_if,lpc11u35_6lowpan_borderrouterusb_if,lpc11u35_archmax_if,lpc11u35_wizwiki_w7500_if,lpc11u35_archpro_if,lpc11u35_ssci824_if,lpc4322_lpc54608xpresso_if,k20dx_frdmkl25z_if,lpc11u35_c027_if,lpc11u35_arm_watch_efm32_if,k20dx_frdmkl05z_if,k20dx_frdmkl43z_if,lpc11u35_blueninja_if,k20dx_rblnano_if,k20dx_rbl_if,lpc11u35_ssci_chibi_if,k20dx_frdmkl82z_if,lpc11u35_tiny_if,lpc11u35_archlink_if,lpc11u35_lpc812xpresso_if,k20dx_frdmk22f_if,lpc11u35_6lowpan_borderrouterethernet_if,k20dx_frdmkw24d_if,k20dx_frdmkl28z_if,k20dx_twrke18f_if,k20dx_twrkl28z72m_if,sam3u2c_mkit_dk_dongle_nrf5x_if,k20dx_hvpke18f_if,sam3u2c_ublox_evk_nina_b1_if,lpc11u35_archble_if,k20dx_frdmk64f_if}]
                   [--logdir LOGDIR] [--noloadif] [--notestendpt] [--loadbl]
                   [--testdl] [--testfirst]
                   [--verbose {Minimal,Normal,Verbose,All}] [--dryrun]
                   [--force]

DAPLink validation and testing tool

optional arguments:
  -h, --help            show this help message and exit
  --targetdir TARGETDIR
                        Directory with pre-built target test images.
  --user USER           MBED username (required for compile-api)
  --password PASSWORD   MBED password (required for compile-api)
  --firmwaredir FIRMWAREDIR
                        Directory with firmware images to test
  --firmware {k20dx_frdmkl02z_if,lpc11u35_sscity_if,lpc11u35_vbluno51_if,k20dx_frdmkl27z_if,lpc11u35_wizwiki_w7500_eco_if,kl26z_nina_b1_if,k20dx_frdmk66f_if,k20dx_frdmk20dx_if,lpc11u35_ssci1114_if,lpc11u35_arm_watch_nrf51_if,lpc11u35_nz32_sc151_if,k20dx_xdot_l151_if,k20dx_frdmke15z_if,sam3u2c_ncs36510rf_if,lpc11u35_arm_watch_stm32f411_if,lpc11u35_lpc4088dm_if,lpc11u35_gr_peach_if,k20dx_frdmkl26z_if,k20dx_frdmk28f_if,k20dx_frdmkl46z_if,lpc11u35_lpc824xpresso_if,k20dx_frdmk82f_if,lpc11u35_wizwiki_w7500p_if,kl26z_microbit_if,lpc4322_lpc54114xpresso_if,lpc11u35_hrm1017_if,lpc11u35_lpc4088qsb_if,lpc11u35_6lowpan_borderrouterhat_if,lpc11u35_cocorico_if,lpc11u35_6lowpan_borderrouterusb_if,lpc11u35_archmax_if,lpc11u35_wizwiki_w7500_if,lpc11u35_archpro_if,lpc11u35_ssci824_if,lpc4322_lpc54608xpresso_if,k20dx_frdmkl25z_if,lpc11u35_c027_if,lpc11u35_arm_watch_efm32_if,k20dx_frdmkl05z_if,k20dx_frdmkl43z_if,lpc11u35_blueninja_if,k20dx_rblnano_if,k20dx_rbl_if,lpc11u35_ssci_chibi_if,k20dx_frdmkl82z_if,lpc11u35_tiny_if,lpc11u35_archlink_if,lpc11u35_lpc812xpresso_if,k20dx_frdmk22f_if,lpc11u35_6lowpan_borderrouterethernet_if,k20dx_frdmkw24d_if,k20dx_frdmkl28z_if,k20dx_twrke18f_if,k20dx_twrkl28z72m_if,sam3u2c_mkit_dk_dongle_nrf5x_if,k20dx_hvpke18f_if,sam3u2c_ublox_evk_nina_b1_if,lpc11u35_archble_if,k20dx_frdmk64f_if}
                        Firmware to test
  --logdir LOGDIR       Directory to log test results to
  --noloadif            Skip load step for interface.
  --notestendpt         Dont test the interface USB endpoints.
  --loadbl              Load bootloader before test.
  --testdl              Run DAPLink specific tests. The DAPLink test tests
                        bootloader updates so usewith caution
  --testfirst           If multiple boards of the same type are found only
                        test the first one.
  --verbose {Minimal,Normal,Verbose,All}
                        Verbose output
  --dryrun              Print info on configurations but dont actually run
                        tests.
  --force               Try to run tests even if there are problems. Delete
                        logs from previous run.
```

Tests are grouped into two categories:
* Tests that validate DAPLink flashing the Target and HIC. This involves automated variations of copying the firmware image file using UMS endpoint (the one that most users use for drag-and-drop). These are not triggered by default, but yout can turn them on with `--testdl` paremeter.
* Tests that validate the UMS (drag-n-drop flashing), CDC (Virtual COM Port), and HID (CMSIS-DAP) endpoints. These are triggered by default, but you can disable them with `--notestendpt` parameter.

**Note:** You really want to take a look at [UMS Commands](#ums_commands) that will help you a lot with automated testing!

### <a name="testing_target"></a>Testing Target UMS/CDC

Test suite performs intense and detailed experimentation by putting a dedicated agent firmware (so called "Target Firmware" or "UART Application") onto tested Target.
With a little help of simple protocol all Target functions and Endpoints can be tested efficiently in a smart way.

For supported mbed boards that are registered on [mbed.org][mbed], Target Firmware Test Agent is built automatically on-the-fly using RESTful API of the [ARM mbed Online Compiler][mbed], then downloaded locally and flashed into Target memory.
However, in order to use this feature you need to specify your mbed.org user id and password via `--user` and `--password` parameters.

For boards that are not mbed enabled or not registered on [mbed.org][mbed], it is not possible to use ARM mbed Online Compiler, but test framework can still use pre-compiled binaries located at `--targetdir`. You need to create the binaries on your own, probably by adapting existing source code to match your Target/Board. **We encourage you to share back the results and register your hardware at [mbed.org][mbed]**.

**Note:** At the moment you can only choode one way of running the test suite, either using online compiler, or using binary firmware images, but not both.

**Note:** [DAPLink Target Validation][daplink_validation] source code is freely available at mbed mercurial repository.

### <a name="testing_hid"></a>Testing HID / CMSIS-DAP

DAPLink Debug feature is provided by a HID endpoint, that transports the CMSIS-DAP protocol, with no drivers installation involved. Host side of the Debug is provided by the [pyOCD][pyocd] library, which is installed during the [development environment setup](#development_environment_setup) process. Setup by default uses the Python Packages from the public release repository. For most users and developers this setup is good enough.

Unfortunately, the public release of pyOCD may not yet contain all necessary features to work with your new DAPLink HID code. Sometimes you also want to modify the pyOCD itself and try out the changes with DAPLink HID. In that case you need to use the development version of both pyOCD and DAPLink using the same Python Virtualenv:
* create one common Python Virtualenv for development version of pyOCD and DAPLink.
* clone the DAPLink git repository, make your changes.
* clone the pyOCD git repository, make your changes.
* install the development version of pyOCD package to the Python Virtualenv with `pip install --editable ./` from pyOCD repository location.
* now you can use DAPLink with new development pyOCD package that you have just installed.


## <a name="release_build"></a>Building a Release

DAPLink contains scripts to automate most of the steps of building a release.  In addition to building the release, these scripts also save relevant build information such as git SHA and python tool versions so the same build can be reproduced.  The recommended steps for creating a release are presented below:

* Create a `tag` with the correct release version and push it to github.
* Clean the repo you will be building from by running `git clean -xdf` followed by `git reset --hard`.
* Run the script `build_release_uvision.bat` to create all builds.
* All release deliverables will be created and stored in `uvision_release` directory. Save this wherever your builds are stored.

**Note:** An existing build can be reproduced by creating and adjusting the `build_requirements.txt` configuration file.
Then pass `build_requirements.txt` as an argument to the `build_release_uvision.bat` script (as described above).
This will manage all dependencies and python packages used to create that particular build.


### <a name="build_release_bundle"></a>Building Bootloader and Firmware Bundle

One of the new DAPLink features, created for the boards that can have a dedicated Bootloader, is that both `DAPLink_Interface` and the `DAPLink_Bootloader` binaries can be bundled within a single Firmware Image File. This makes it possible to distribute just one firmware file, with no deidcated instructions on how to update the interface or bootloader part, to perform full upgrade of the DAPLink HIC at once.

To enable the bootloader updates for a Board or the Hardware Interface Circuit, you should define `DAPLINK_BOOTLOADER_UPDATE` in your device's project configuration (`yaml`) file. Once the variable is defined, you can build the firmware as described in the [Building a Release](#release_build) section.

**Warning:** You should pay special attention to this kind of update, as producing invalid firmware bundle may brick your device.. and that would require manual flashing of Bootloader anyway.

The interface firmware erases and updates the vector table when updating the bootloader. Because of this, there is a small window of time during which the device will be in a nonbootable state if the device loses power. The cause of this is the lack of a valid vector table. To minimize the time without a valid vector table and to reduce risk, interface firmware replaces the bootloader's vector table with its own before performing the update. This way, while the bulk of the bootloader is being loaded, there is a valid vector table, and a loss of power event does not prevent the device from booting. The update process is below. The critical sections during which the device cannot lose power are in bold:
1. **Erase the boot vector table, and program the intenterface's vector table to this location**.
1. Erase and program each sector of the bootloader with the new firmware.
1. **Erase the boot vector table, and program the new bootloader's vector table to this location**.

Other checks:
* The interface firmware will not downgrade the bootloader. If the current bootloader has a valid signature and a version greater than the interface's copy of the bootloader then the interface will not replace the bootloader.
* The interface firmware does a checksum over itself to check for any corruption. If it finds itself to be corrupt, it will not apply the bootloader update. In addition, an assert will appear on the on mass storage.
* Before attempting to update the bootloader the interface firmware compares its internal copy of the bootloader to the curent bootloader. If they are the same then it does not apply an update.

Dangers:
* The DAPLink bootloader has a fixed location for where the interface firmware is loaded (typically an offset of `0x8000` from the start of ROM). If you update to a bootloader that has a different offset then you will no longer be able to use the same interface firmware. Instead, you must use interface firmware built for the new offset.
* This mechanism does not support downgrading or loading third party bootloaders. To do this, you must use a debugger or create a custom build of DAPLink.
* The LPC11U35 does not have a bootloader, so you cannot use bootloader updates on this interface.



## <a name="supported_ide_sdk"></a>Supported IDE/SDK

### Keil uVision MDK

Please note that we are currently using Keil uVision MDK 5 to build DAPLink firmware because of high quality of the produced code and small footprint of the binary, therefore you need this one to build the firmware on your own.

If you want to use the Keil uVision MDK IDE, you must launch it in the correct OS environment if you did not install it in the default location. The project will fail to build otherwise. To verify a proper launch of uVision use the `tools\launch_uv4.bat` script.




## <a name="dictionary"></a>Dictionary of Abbreviations

abbreviation | meaning | reference
:---: | --- | ---
CPU | Central Processing Unit | https://en.wikipedia.org/wiki/Central_processing_unit
MCU | MicroController Unit | https://en.wikipedia.org/wiki/Microcontroller
USB | Universal Serial Bus | https://en.wikipedia.org/wiki/USB
UMS | USB Mass Storage | https://en.wikipedia.org/wiki/USB_mass_storage_device_class
HID | Human Interface Device | https://en.wikipedia.org/wiki/USB_human_interface_device_class
CDC | Communication Device Class | https://en.wikipedia.org/wiki/USB_communications_device_class
VCP | Virtual COM Port (Serial Port over USB) | https://en.wikipedia.org/wiki/Serial_port
DAP | Debug Access Port | http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0314h/Babdficb.html
HIC | Hardware Interface Circuit | TODO
SDK | Software Development Kit | https://en.wikipedia.org/wiki/Software_development_kit
IDE | Itegrated Development Environment | https://en.wikipedia.org/wiki/Integrated_development_environment



[arm]: https://www.arm.com "ARM Limited website"
[mbed]: https://developer.mbed.org "arm mbed developer resources"
[daplink]: https://github.com/mbedmicro/DAPLink "DAPLink Project Website and GitHub Repository"
[daplinkreleases]: https://github.com/mbedmicro/DAPLink/releases "DAPLink Releses"
[daplinkissues]: https://github.com/mbedmicro/DAPLink/issues "DAPLink Issue Tracker"
[pyocd]: https://github.com/mbedmicro/pyOCD "Open-Source Python Library for Programming and Debugging ARM Cortex-M microcontrollers using CMSIS-DAP"
[cmsisdap]: https://github.com/mbedmicro/CMSIS-DAP/ "ARM mbed CMSIS-DAP project"
[mbedcagreement]: https://developer.mbed.org/contributor_agreement/ "ARM mbed Contributor Agreement"
[mbed_flashalgo]: https://github.com/mbedmicro/FlashAlgo "ARM mbed FlashAlgo Project"
[mbed_fdrm_k64f]: https://developer.mbed.org/platforms/FRDM-K64F/ "ARM mbed board FDRM-K64F"
[project_generator]: https://github.com/project-generator/project_generator/wiki/Getting_started "Project Generator"
[daplink_validation]: https://developer.mbed.org/users/c1728p9/code/daplink-validation/ "DAPLink Validation / Target Firmware Test Agent source code repository"
[mbed_winserial]: https://developer.mbed.org/handbook/Windows-serial-configuration "ARM mbed Windows Virtual COM Port configuration page"
[gnuarm]: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm "GNU ARM Embedded Toolchain"
[eclipse]: https://docs.mbed.com/docs/mbed-os-handbook/en/latest/debugging/debugging_eclipse_pyocd/ "Debugging mbed OS 5 applications with Eclipse"
[uvision]: http://www2.keil.com/mdk5/uvision/ "Keil uVision IDE"
[iar]: https://www.iar.com/iar-embedded-workbench/ "IAR Embedded Workbench IDE"
[python]: https://www.python.org/downloads/ "Python is platform independent free and open programming language"
[git]: https://git-scm.com/downloads "GIT is a distributed version control system"
[git_merge_vs_rebase]: https://www.atlassian.com/git/tutorials/merging-vs-rebasing/workflow-walkthrough "Article on GIT Merge vs Rebase operations"
[github_fork]: https://help.github.com/articles/fork-a-repo/ "How to Fork a Project on GitHub"
[github_pullrequest]: https://help.github.com/articles/about-pull-requests/ "About Pull Request on GitHub"
