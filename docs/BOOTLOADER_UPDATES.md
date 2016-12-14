# Bootloader updates

DAPLink has the ability to bundle the bootloader firmware with the interface firmware and apply a bootloader update on the first boot of the interface. This allows bootloader updates to be applied as the same time as interface updates and without any special instructions for the user.

### Enabling bootloader updates

To enable bootloader updates for a board or interface circuit the value ```DAPLINK_BOOTLOADER_UPDATE``` must be defined. Once defined, the normal release process which documented on the [developers guide](DEVELOPERS-GUIDE.md) can be used to create firmware for all targets.

### Safety when updating the bootloader

The vector table of the bootloader is erased and replaced during the update. Because of this, there is a small window of time where if power is lost the device will be in a non-bootable state since it does not have a valid vector table. To minimize the time without a valid vector table and the reduce risk the bootloader vector table is replaced with the interface's vector table before the update is performed. This way while the bulk of the bootloader is being loaded there is a valid vector table and a loss of power event does not prevent the device from booting. The update process is shown below with the critical sections where power cannot be lost showin in bold:
1. **Erase the boot vector table and program the intenterface's vector table to this location**
1. Erase and program each sector of the bootloader with the new firmware
1. **Erase the boot vector table and program the new bootloader's vector table to this location**

Other checks
* If the current bootloader has a valid signature and a version greater than the bootloader stored in interfacer has, the update will not be applied.
* The interface does a checksum over itself to check for any corruption. If the interface is found to be corrupt the bootloader update will not be applied and an assert will appear on the device.
* If the current bootloader is identical to the bootloader stored in the interface the update will not be applied.

### Dangers
* The DAPLink bootloader has a fixed location for where the interface is loaded (typically an offset of 0x8000 from the start of rom). If the bootloader you updated from had a different offset then the interface firmware you used with that bootloader will no longer work. You must use interface firmware built for the new offset.
* This mechanism does not support downgrading or loading 3rd party bootloaders. To do this a debugger must be used or a custom build of DAPLink must be created.
* The LPC11U35 does not have a bootloader, so bootloader updates cannot be used on this interface.
