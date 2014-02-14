"""
CMSIS-DAP Interface Firmware
Copyright (c) 2009-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
from os.path import getsize
from struct import pack

from options import get_options
from utils import gen_binary, is_lpc
from paths import *


BOOTLOADER_SIZES = {
    'LPC11U35': 0x5000,
    'K20DX128': 0x5000,
}


def merge(bootloader_path, interface_path, image_path, interface_start=None):
    bootloader_size = getsize(bootloader_path)
    interface_size = getsize(interface_path)
    if bootloader_size == 0 or interface_size == 0:
        raise Exception("empty program. Bootloader size: ", bootloader_size, " Interface size: ", interface_size)

    with open(bootloader_path, 'rb') as bootloader, open(interface_path, 'rb') as interface, open(image_path, 'wb') as image:
        if interface_start is None:
            offset = bootloader_size
        else:
            if bootloader_size > interface_start:
                raise Exception("bootloader overflow in interface area")
            offset = interface_start

        # Copy bootloader
        image.write(bootloader.read())

        # Write padding
        for _ in range(offset - bootloader_size):
            image.write(pack('B', 0xFF))

        # Copy interface
        image.write(interface.read())


if __name__ == '__main__':
    options = get_options()
    print 'Generating image for (if: %s, target: %s)' % (options.interface, options.target)

    bootloader_elf = get_bootloader_path(options.interface)
    print 'Bootloader: %s' % bootloader_elf

    interface_elf = get_interface_path(options.interface, options.target)
    print 'Interface : %s' % interface_elf

    image_bin = get_image_path(options.interface, options.target)
    print 'Image     : %s' % image_bin

    gen_binary(bootloader_elf, TMP_BOOTLOADER_BIN_PATH, is_lpc(options.interface))
    gen_binary(interface_elf , TMP_INTERFACE_BIN_PATH)

    merge(TMP_BOOTLOADER_BIN_PATH, TMP_INTERFACE_BIN_PATH, image_bin,
          BOOTLOADER_SIZES[options.interface])
