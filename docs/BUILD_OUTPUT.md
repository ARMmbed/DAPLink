# DAPLink Build Output

## Variants

Building a DAPLink target produces several variants of the build output. Files with the following names will be located in the project build directory after a successful build.

``*_if.bin``

- Base file created by build, should not be used.

``*_if_crc.bin``

- Preferred image for new boards. Does not work with some old bootloaders due to the vector table validation

``*_if_crc.hex``

- Same as "``*_if_crc.bin``" just in hex form.

``*_if_crc_legacy_0x5000.bin``

- Image required for existing boards with a bootloader built for an application starting at 0x5000.

``*_if_crc_legacy_0x8000.bin``

- Image required for existing boards with a bootloader built for an application starting at 0x8000.

The ``tools/post_compute_crc.py`` script is automatically run on the build output. It produces the variants listed above by modifying the output images in several ways.

The image ``*_if_crc_legacy_0x5000.bin`` is a copy of ``*_if_crc_legacy_0x8000.bin`` but with 0x3000 bytes of extra padding pre-pended to the image. The first 0x40 bytes are an exact copy of the vector table and the remaining 0x2FC0 bytes are filled with 0xFF. This makes an image that can be loaded onto bootloaders with the old offset (0x5000) without requiring a complete rebuild.

Not all targets will produce the legacy output files.

## Image structure

Aside from the code itself, the following are the important features of a DAPLink binary image. The ``post_compute_crc.py`` script adds the checksum and CRC into the images.

### Vector table

All images must begin with a valid NVIC vector table.

The vector table is defined by the ``startup_MCUNAME.S`` file for each of the HIC HALs. The build info fields below are set directly in the vector table definition.

### Checksum

The first 7 vectors from the NVIC vector table (offsets 0x00-0x18) are checksummed. The two's complement of the checksum is stored in the unused, reserved vector at offset 0x1C. This checksum is required for certain NXP MCUs in order to boot. For other MCUs, the checksum is ignored.

### Build info fields

The vector table for DAPLink images contains several build info fields. These fields are placed in the unused, reserved vector table entries starting at offset 0x20.

The information here is as follows (in order):

* ``DAPLINK_BUILD_KEY``
    * Key used to indicate if the image is bootloader or interface.
* ``DAPLINK_HIC_ID``
    * Key used to determine hardware compatibility.
    * This ensures that for example a bootloader update mean for a k20dx could never be loaded on a kl26z or any other chip
* ``DAPLINK_VERSION``
    * Software version.
    * Meant for checking for incompatible legacy versions.
    * Currently it is not used for this purpose and is just displayed in ``details.txt``.

This change caused a problem with some old bootloaders, which require the 3 vector table entries starting at 0x20 to be set to 0. To overcome this the build scripts create the legacy images which zero out 0x20 and recompute the checksum. If you compare ``*_if_crc.bin`` and ``*_if_crc_legacy_0x8000.bin`` you should be able to see this difference.

The corresponding struct in the code for these fields is ``daplink_info_t``, defined in ``daplink.h``. Constants with the valid values for these fields are also defined in ``daplink.h``.

### CRC

A CRC-32 is computed over the base output file (``*_if.bin``) and stored in the last 4 bytes of the image in little endian byte order. This produces the ``*_if_crc.bin`` file.

At the moment the interface CRC isn't actually enforced so existing and interface images without a CRC can still be loaded.

