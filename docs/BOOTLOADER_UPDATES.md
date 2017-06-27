# Bootloader updates

DAPLink has the ability to bundle the bootloader firmware with the interface firmware and apply a bootloader update on the first boot of the interface. This allows bootloader updates to occur at the same time as interface updates and without any special instructions for the user.

## Enabling bootloader updates

To enable bootloader updates for a board or interface circuit, you must define the value ```DAPLINK_BOOTLOADER_UPDATE```. Once you define it, you can use the normal release process, which the [developers guide](DEVELOPERS-GUIDE.md) documents, to create firmware for all targets.

## Safety when updating the bootloader

The interface firmware erases and updates the vector table when updating the bootloader. Because of this, there is a small window of time during which the device will be in a nonbootable state if the device loses power. The cause of this is the lack of a valid vector table. To minimize the time without a valid vector table and to reduce risk, interface firmware replaces the bootloader's vector table with its own before performing the update. This way, while the bulk of the bootloader is being loaded, there is a valid vector table, and a loss of power event does not prevent the device from booting. The update process is below. The critical sections during which the device cannot lose power are in bold:

1. **Erase the boot vector table, and program the intenterface's vector table to this location**.
1. Erase and program each sector of the bootloader with the new firmware.
1. **Erase the boot vector table, and program the new bootloader's vector table to this location**.

Other checks
* The interface firmware will not downgrade the bootloader. If the current bootloader has a valid signature and a version greater than the interface's copy of the bootloader then the interface will not replace the bootloader.
* The interface firmware does a checksum over itself to check for any corruption. If it finds itself to be corrupt, it will not apply the bootloader update. In addition, an assert will appear on the on mass storage.
* Before attempting to update the bootloader the interface firmware compares its internal copy of the bootloader to the curent bootloader. If they are the same then it does not apply an update.

## Dangers

* The DAPLink bootloader has a fixed location for where the interface firmware is loaded (typically an offset of 0x8000 from the start of ROM). If you update to a bootloader that has a different offset then you will no longer be able to use the same interface firmware. Instead, you must use interface firmware built for the new offset.

* This mechanism does not support downgrading or loading third party bootloaders. To do this, you must use a debugger or create a custom build of DAPLink.
* The LPC11U35 does not have a bootloader, so you cannot use bootloader updates on this interface.
