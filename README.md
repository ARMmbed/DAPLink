CMSIS-DAP Refactoring Efforts
============================
**Goals are:**

- merge in mbed build system tools
- automate testing for validating port contributions and patches
- use mbed exporters for creating project files to debug when needed
- share common source USB, RTOS, IO, etc. 
- Add hooks where custom handlers are needed but generally not break buids when adding new targets and features

Current Status
-------------
**Nothing on this branch is guaranteed to work. Current efforts are with the bootloader and then the interface project**

Bootloader:

- K20DX128: working / work in progress

Interface:

- Not working. Changes to the directory structure and hooks broke these

ToDo
------

- Serial driver installer without mbed connected
- Change USB descriptors to expose HID without needing the CDC driver
- Implement XON/XOFF flowcontrol
- Incorporate mbed build system
- Change offset address to 0x8000 to match OpenSDA for Freescale only targets. NXP stays at 0x5000
- Only erase sectors that code fits in
- Use media eject for MSD
- Verify semi-hosting on HID connection
- RAM allocation for virtual file-system hidden files and folders
- version.txt file for offline, non html-access to the bootloader and CMSIS-DAP version
- Add hex file support (interface only I think). Exists on master but needs verification and testing
- Add srec file support (interface only I think)
- etc...

<!---

**Contribution is welcomed on this branch**

Documentation
-------------
* [Porting the FW to new boards](http://mbed.org/handbook/cmsis-dap-interface-firmware)

Community
---------
For discussing the development of the CMSIS-DAP Interface Firmware please join our [mbed-devel mailing list](https://groups.google.com/forum/?fromgroups#!forum/mbed-devel).
-->
