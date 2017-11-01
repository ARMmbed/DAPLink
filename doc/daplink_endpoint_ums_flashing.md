# UMS Flashing

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

**Note:** By default your **Target will be left Halted for Debugging after Flashing** (you can change this behavior with [AUTO RESET MODE](daplink_endpoint_ums_commands.md) configuration command).
