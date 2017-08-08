# UMS Commands

DAPLink allows you to execute simple commands using USB Mass Storage (UMS) interface.
You can copy a file to the root of the DAPLink UMS drive to make DAPLink perform an action or configure a flash-persisted option.
So far, the contents of the file is ignored, therefore you can simply pass an empty file.

UMS Commands only work if you are either holding down the RESET button on the DAPLink HIC while copying the file, or if you have turned on DAPLink `AUTOMATION MODE` (see [Automation Mode](daplink_endpoint_ums_automation.md) section).
The Automation Mode is more convenient if you intend to use UMS Commands quite often or remotely, because it is preserved between reboots, so no manual interaction with the board is necessary.

By convention, files that end with `.ACT` trigger an action, wile files that end with `.CFG` configure a behavior.
The mechanism is case-insensitive (you can use either lowercase or uppercase letters).

Every command completes by remounting the DAPLink UMS drive.
Every command is supported in both interface and bootloader mode.
In fact, any file (create, delete, write) operation could be used to trigger a DAPLink command, if the handler is created in the code.

## Action Commands

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

## Configuration Commands

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
