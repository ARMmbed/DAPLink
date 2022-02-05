#!/usr/bin/env python3
#
# DAPLink Interface Firmware
# Copyright (c) 2011-2021 Arm Limited
# Copyright (c) 2021 Chris Reed
# Copyright (c) 2021 Mathias Brossard
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import argparse
import binascii
import colorama
import hashlib
import jinja2
import os
import struct
from datetime import datetime
from pyocd.target.pack.flash_algo import PackFlashAlgo

# This header consists of two instructions:
#
# ```
# bkpt  #0
# b     .-2     # branch to the bkpt
# ```
#
# Before running a flash algo operation, LR is set to the address of the `bkpt` instruction,
# so when the operation function returns it will halt the CPU.
BLOB_HEADER = '0xe7fdbe00,'
HEADER_SIZE = 4
STACK_SIZE = 0x800

DAPLINK_TEMPLATE = \
"""/*{%- if algo.flash_info.name %} Flash algorithm for {{ algo.flash_info.name.decode('utf-8') }}
 *{% endif %}
 * DAPLink Interface Firmware
 * Copyright (c) {{year}} {{copyright_owner}}
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Generated from '{{ filename }}'{%- if algo.flash_info.name %}{{ " (%s)" % algo.flash_info.name.decode('utf-8') }}{% endif %}
{%- if pack_file %}
// Originating from '{{ pack_file }}'
{%- endif %}
// digest = {{ digest }}, file size = {{ file_size}}
// {{ 'algo version = 0x%x, algo size = %d (0x%x)' % (algo.flash_info.version, algo_size + header_size, algo_size + header_size) }}
static const uint32_t {{name}}_flash_prog_blob[] = {
    {{prog_header}}
    {{algo.format_algo_data(4, 8, "c")}}
};

// Start address of flash
static const uint32_t flash_start = {{"0x%08x" % algo.flash_start}};
// Size of flash
static const uint32_t flash_size = {{"0x%08x" % algo.flash_size}};

/**
* List of start and size for each size of flash sector - even indexes are start, odd are size
* The size will apply to all sectors between the listed address and the next address
* in the list.
* The last pair in the list will have sectors starting at that address and ending
* at address flash_start + flash_size.
*/
static const sector_info_t sectors_info[] = {
{%- for start, size  in algo.sector_sizes %}
    {{ "{0x%08x, 0x%08x}" % (start + algo.flash_start, size) }},
{%- endfor %}
};

static const program_target_t flash = {
    {{'0x%08x' % (algo.symbols['Init'] + header_size + entry)}}, // Init
    {{'0x%08x' % (algo.symbols['UnInit'] + header_size + entry)}}, // UnInit
    {{'0x%08x' % (algo.symbols['EraseChip'] + header_size + entry)}}, // EraseChip
    {{'0x%08x' % (algo.symbols['EraseSector'] + header_size + entry)}}, // EraseSector
    {{'0x%08x' % (algo.symbols['ProgramPage'] + header_size + entry)}}, // ProgramPage
{%- if 'Verify' in algo.symbols and algo.symbols['Verify'] < 0xFFFFFFFF %}
    {{'0x%08x' % (algo.symbols['Verify'] + header_size + entry)}}, // Verify
{%- else %}
    0x00000000, // Verify
{%- endif %}

    // BKPT : start of blob + 1
    // RSB  : blob start + header + rw data offset
    // RSP  : stack pointer
    {
        {{'0x%08x' % (entry + 1)}},
        {{'0x%08x' % (entry + header_size + algo.rw_start)}},
        {{'0x%08x' % stack_pointer}}
    },

    // mem buffer location
    {{'0x%08x' % stack_pointer}},
    // location to write prog_blob in target RAM
    {{'0x%08x' % entry}},
    // prog_blob size
    sizeof({{name}}_flash_prog_blob),
    // address of prog_blob
    {{name}}_flash_prog_blob,
    // ram_to_flash_bytes_to_be_written
    {{'0x%08x' % algo.page_size}}
};

"""

colorama.init()

def str_to_num(val):
    return int(val, 0)  #convert string to number and automatically handle hex conversion

class PackFlashAlgoGenerator(PackFlashAlgo):
    """
    Class to wrap a flash algo

    This class is intended to provide easy access to the information
    provided by a flash algorithm, such as symbols and the flash
    algorithm itself.
    """

    def format_algo_data(self, spaces, group_size, fmt):
        """"
        Return a string representing algo_data suitable for use in a template

        The string is intended for use in a template.

        :param spaces: The number of leading spaces for each line
        :param group_size: number of elements per line (element type
            depends of format)
        :param fmt: - format to create - can be either "hex" or "c"
        """
        padding = " " * spaces
        if fmt == "hex":
            blob = binascii.b2a_hex(self.algo_data)
            line_list = []
            for i in range(0, len(blob), group_size):
                line_list.append('"' + blob[i:i + group_size] + '"')
            return ("\n" + padding).join(line_list)
        elif fmt == "c":
            blob = self.algo_data[:]
            pad_size = 0 if len(blob) % 4 == 0 else 4 - len(blob) % 4
            blob = blob + b"\x00" * pad_size
            integer_list = struct.unpack("<" + "L" * (len(blob) // 4), blob)
            line_list = []
            for pos in range(0, len(integer_list), group_size):
                group = ["0x%08x" % value for value in
                         integer_list[pos:pos + group_size]]
                line_list.append(", ".join(group))
            return (",\n" + padding).join(line_list)
        else:
            raise Exception("Unsupported format %s" % fmt)

    def process_template(self, template_text, data_dict=None):
        """
        Generate output from the supplied template

        All the public methods and fields of this class can be accessed from
        the template via "algo".

        :param template_path: Relative or absolute file path to the template
        :param data_dict: Additional data to use when generating
        """
        if data_dict is None:
            data_dict = {}
        else:
            assert isinstance(data_dict, dict)
            data_dict = dict(data_dict)
        assert "algo" not in data_dict, "algo already set by user data"
        data_dict["algo"] = self

        template = jinja2.Template(template_text)
        return template.render(data_dict)

def main():
    parser = argparse.ArgumentParser(description="Blob generator")
    parser.add_argument("elf_path", help="Elf, axf, or flm to extract flash algo from")
    parser.add_argument("--blob-start", default=0x20000000, type=str_to_num, help="Starting "
                        "address of the flash blob in target RAM.")
    parser.add_argument("--stack-size", default=STACK_SIZE, type=str_to_num, help="Stack size for the algo "
                        f"(default {STACK_SIZE}).")
    parser.add_argument("--pack-path", default=None, help="Path to pack file from which flash algo is from")
    parser.add_argument("-i", "--info-only", action="store_true", help="Only print information about the flash "
                        "algo, do not generate a blob.")
    parser.add_argument("-o", "--output", default="target_blob.c", help="Path of output file "
                        "(default 'target_blob.c').")
    parser.add_argument("-t", "--template", help="Path to Jinja template file (default is an internal "
                        "template for DAPLink).")
    parser.add_argument('-c', '--copyright', help="Set copyright owner.")
    args = parser.parse_args()

    if not args.copyright:
        print(f"{colorama.Fore.YELLOW}Warning! No copyright owner was specified. Defaulting to \"Arm Limited\". "
            f"Please set via --copyright, or edit output.{colorama.Style.RESET_ALL}")

    if args.template:
        with open(args.template, "r") as tmpl_file:
            tmpl = tmpl_file.read()
    else:
        tmpl = DAPLINK_TEMPLATE

    with open(args.elf_path, "rb") as file_handle:
        algo = PackFlashAlgoGenerator(file_handle)

        print(algo.flash_info)

        # Allocate stack after algo and its rw/zi data, with bottom rounded to 8 bytes.
        stack_base = (args.blob_start + HEADER_SIZE
                        + algo.rw_start + algo.rw_size # rw_start incorporates instruction size
                        + algo.zi_size)
        stack_base = (stack_base + 7) // 8 * 8
        # Stack top rounded to at least 256 bytes
        sp = stack_base + args.stack_size
        if algo.page_size > 256:
            sp = (sp + algo.page_size - 1) // algo.page_size * algo.page_size
        else:
            sp = (sp + 255) // 256 * 256

        print(f"load addr:   {args.blob_start:#010x}")
        print(f"header:      {HEADER_SIZE:#x} bytes")
        print(f"data:        {len(algo.algo_data):#x} bytes")
        print(f"ro:          {algo.ro_start:#010x} + {algo.ro_size:#x} bytes")
        print(f"rw:          {algo.rw_start:#010x} + {algo.rw_size:#x} bytes")
        print(f"zi:          {algo.zi_start:#010x} + {algo.zi_size:#x} bytes")
        print(f"stack:       {stack_base:#010x} .. {sp:#010x} ({sp - stack_base:#x} bytes)")
        print(f"buffer:      {sp:#010x} .. {sp + algo.page_size:#010x} ({algo.page_size:#x} bytes)")

        print("\nSymbol offsets:")
        for n, v in sorted(algo.symbols.items(), key=lambda x: x[1]):
            if v >= 0xffffffff:
                continue
            print(f"{n}:{' ' * (11 - len(n))} {v:#010x}")

        if args.info_only:
            return

        pack_file = None
        if args.pack_path and os.path.exists(args.pack_path) and 'cmsis-pack-manager' in args.pack_path:
            (rest, version) = (os.path.split(args.pack_path))
            (rest, pack) = (os.path.split(rest))
            (_, vendor) = (os.path.split(rest))
            pack_file = '%s.%s.%s' % (vendor, pack, version)
        elif args.pack_path:
            pack_file = os.path.split(args.pack_path)[-1]
        else:
            print(f"{colorama.Fore.YELLOW}Warning! No CMSIS Pack was set."
                f"Please set the path or file name of the CMSIS pack via --pack-path.{colorama.Style.RESET_ALL}")

        file_handle.seek(0)
        flm_content = file_handle.read()
        hash = hashlib.sha256()
        hash.update(flm_content)

        data_dict = {
            'filename': os.path.split(args.elf_path)[-1],
            'digest': hash.hexdigest(),
            'file_size': len(flm_content),
            'pack_file': pack_file,
            'algo_size': len(algo.algo_data),
            'name': os.path.splitext(os.path.split(args.elf_path)[-1])[0],
            'prog_header': BLOB_HEADER,
            'header_size': HEADER_SIZE,
            'entry': args.blob_start,
            'stack_pointer': sp,
            'year': datetime.now().year if args.copyright else ("2009-%d" % datetime.now().year),
            'copyright_owner': args.copyright or "Arm Limited, All Rights Reserved",
        }

        text = algo.process_template(tmpl, data_dict)

        with open(args.output, "w") as file_handle:
            file_handle.write(text)

        print(f"Wrote flash algo dict to {args.output}")

if __name__ == '__main__':
    main()
