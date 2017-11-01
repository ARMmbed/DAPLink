# Firmware Update

It is possible, and sometimes necessary, to firmware upgrade your DAPLink device.
**By holding the RESET button on your DAPLink board at Power-ON or USB-attach you will trigger a Bootloader mode** (USB drive will now have `BOOTLOADER` label).
This will allow firmware upgrade of the DAPLink itself.

Just as with firmware flashing, copy DAPLink firmware onto USB drive when in Bootloader mode and wait for the write process to finish.
On success device will reboot and start new Daplink firmware (`DAPLINK` USB drive label).
On failure you will see a `FAIL.TXT` file on a DAPLink USB drive with an explanation of what went wrong (you can open and read this file with standard text editor like `Notepad` or `ViM`).

*Note: With a simple HIC devices DAPLink consists of only of one firmware file (`_IF` suffix). On more advanced devices two files are provided for Booloader (`_BL` file suffix) and the Interface Firmware (`_IF` file suffix) - in that case you need to upgrade Interface Firmware using Bootloader, but you can only upgrade Bootloader using Interface Firmware (just as it was target firmware) because the bootloader cannot update itself replacing the memory it runs from..*
