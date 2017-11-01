# Building Bootloader and Firmware Bundle

One of the new DAPLink features, created for the boards that can have a dedicated Bootloader, is that both `DAPLink_Interface` and the `DAPLink_Bootloader` binaries can be bundled within a single Firmware Image File. This makes it possible to distribute just one firmware file, with no deidcated instructions on how to update the interface or bootloader part, to perform full upgrade of the DAPLink HIC at once.

To enable the bootloader updates for a Board or the Hardware Interface Circuit, you should define `DAPLINK_BOOTLOADER_UPDATE` in your device's project configuration (`yaml`) file. Once the variable is defined, you can build the firmware as described in the [Building a Release](daplink_build_release.md) section.

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
