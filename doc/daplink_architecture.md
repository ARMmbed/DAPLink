# Architecture

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
