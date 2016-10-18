#
# DAPLink Interface Firmware
# Copyright (c) 2009-2016, ARM Limited, All Rights Reserved
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may
# not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

from __future__ import absolute_import
from __future__ import print_function

import argparse
import itertools
import binascii
import struct
import intelhex
import offset_update

VECTOR_FMT = "<7I"
CHECKSUM_FMT = "<1I"
CHECKSUM_OFFSET = 0x1C


def ranges(i):
    for _, b in itertools.groupby(enumerate(i), lambda x_y: x_y[1] - x_y[0]):
        b = list(b)
        yield b[0][1], b[-1][1]


def main():
    parser = argparse.ArgumentParser(description='Image CRC tool')
    parser.add_argument("input", type=str, help="Hex file to read from.")
    parser.add_argument("output", type=str,
                        help="Output base file name to write CRC32.")

    args = parser.parse_args()
    input_file = args.input
    output_file_hex = args.output + ".hex"
    output_file_binary = args.output + ".bin"
    output_file_txt = args.output + ".txt"
    output_file_legacy = args.output + "_legacy_0x8000.bin"
    output_file_legacy_5000 = args.output + "_legacy_0x5000.bin"
    output_file_legacy_txt = args.output + "_legacy.txt"

    # Read in hex file
    new_hex_file = intelhex.IntelHex()
    new_hex_file.padding = 0xFF
    new_hex_file.fromfile(input_file, format='hex')

    # Get the starting and ending address
    addresses = new_hex_file.addresses()
    addresses.sort()
    start_end_pairs = list(ranges(addresses))
    regions = len(start_end_pairs)
    assert regions == 1, ("Error - only 1 region allowed in "
                          "hex file %i found." % regions)
    start, end = start_end_pairs[0]

    # Checksum the vector table
    #
    # Note this is only required for NXP devices but
    # it doesn't hurt to checksum all builds

    # Compute a checksum on the first 7 vector nvic vectors
    vector_size = struct.calcsize(VECTOR_FMT)
    vector_data = new_hex_file.tobinarray(start=start, size=vector_size)
    vectors = struct.unpack(VECTOR_FMT, vector_data)
    assert len(vectors) == 7, "Incorrect size of %i" % len(vectors)
    checksum = 0
    for vector in vectors:
        checksum += vector
    checksum = (~checksum + 1) & 0xFFFFFFFF  # Two's compliment

    # Write checksum back to hex
    csum_start = CHECKSUM_OFFSET + start
    csum_data = struct.pack(CHECKSUM_FMT, checksum)
    assert len(csum_data) == 4
    new_hex_file.puts(csum_start, csum_data)

    # CRC the entire image
    #
    # This is required for all builds

    # Compute checksum over the range (don't include data at location of crc)
    size = end - start + 1
    crc_size = size - 4
    data = new_hex_file.tobinarray(start=start, size=crc_size)
    crc32 = binascii.crc32(data) & 0xFFFFFFFF

    # Write CRC to the file in little endian
    new_hex_file[end - 3] = (crc32 >> 0) & 0xFF
    new_hex_file[end - 2] = (crc32 >> 8) & 0xFF
    new_hex_file[end - 1] = (crc32 >> 16) & 0xFF
    new_hex_file[end - 0] = (crc32 >> 24) & 0xFF

    # Write out file(s)
    new_hex_file.tofile(output_file_hex, 'hex')
    new_hex_file.tofile(output_file_binary, 'bin')
    with open(output_file_txt, 'wb') as file_handle:
        file_handle.write("0x%08x\r\n" % crc32)

    # Print info on operation
    print("Start 0x%x, Length 0x%x, CRC32 0x%08x" % (start, size, crc32))

    if start == 0x8000 or start == 0x88000  or start == 0x8008000:
        pad_addr = start - 0x3000
        legacy_zero = start + 7 * 4
        legacy_size = 4 * 4
        legacy_hex_file = intelhex.IntelHex(new_hex_file)
        for addr in range(legacy_zero, legacy_zero + legacy_size):
            legacy_hex_file[addr] = 0
        data = legacy_hex_file.tobinarray(start=start, size=crc_size)
        crc32 = binascii.crc32(data) & 0xFFFFFFFF
        # Write CRC to the file in little endian
        legacy_hex_file[end - 3] = (crc32 >> 0) & 0xFF
        legacy_hex_file[end - 2] = (crc32 >> 8) & 0xFF
        legacy_hex_file[end - 1] = (crc32 >> 16) & 0xFF
        legacy_hex_file[end - 0] = (crc32 >> 24) & 0xFF
        legacy_hex_file.tofile(output_file_legacy, 'bin')
        with open(output_file_legacy_txt, 'wb') as file_handle:
            file_handle.write("0x%08x\r\n" % crc32)
        offset_update.create_padded_image(output_file_legacy,
                                          output_file_legacy_5000,
                                          start, pad_addr, 0x40)
    elif start == 0 or start == 0x80000  or start == 0x8000000:
        pass
    else:
        assert 0, "Unsupported start address 0x%x" % start

if __name__ == '__main__':
    main()
