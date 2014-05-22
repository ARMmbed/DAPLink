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


This script takes all .axf files in the TMP_DIR folder containing the flash algorithm to be
loaded in the target RAM and it converts it to a binary array ready to be
included in the CMSIS-DAP Interface Firmware source code.
"""
from struct import unpack
import os

from utils import run_cmd
from settings import *
from paths import TMP_DIR


# INPUT
ALGO_ELFS_PATH = [ os.path.join(TMP_DIR,f) for f in os.listdir(TMP_DIR) if (os.path.isfile(os.path.join(TMP_DIR,f)) and f.endswith(".axf")) ]

ALGO_OFFSET = 0x20

# OUTPUT
DEV_INFO_PATH = os.path.join(TMP_DIR, "DevDscr")
ALGO_BIN_PATH = os.path.join(TMP_DIR, "PrgCode")
TMP_DIR_W_TERM = TMP_DIR + '/'

# Algorithm start addresses for each TARGET (compared with DevName in the
# FlashDevice structure in FlashDev.c
ALGO_START_ADDRESSES = {
    'LPC1700':  0x10000000,
    'LPC11xx':  0x10000000,
    'LPC8xx':   0x10000000,
    'LPC11U68': 0x10000000,
    'LPC1549':  0x02000000,
    'LPC18xx':  0x10000000,
    'LPC43xx':  0x10000000,
    'MKXX':     0x20000000,
}

class FlashInfo(object):
    def __init__(self, path):
        with open(path, "rb") as f:
            # Read Device Information struct (defined in FlashOS.H, declared in FlashDev.c).
            self.version  = unpack("H", f.read(2))[0]
            self.devName  = f.read(128).split(b'\0',1)[0]
            self.devType  = unpack("H", f.read(2))[0]
            self.devAddr  = unpack("L", f.read(4))[0]
            self.szDev    = unpack("L", f.read(4))[0]
            self.szPage   = unpack("L", f.read(4))[0]
            skipped = f.read(4)
            self.valEmpty = unpack("B", f.read(1))[0]
            skipped = f.read(3)
            self.toProg   = unpack("L", f.read(4))[0]
            self.toErase  = unpack("L", f.read(4))[0]
            self.sectSize = []
            self.sectAddr = []
            while 1:
                addr = unpack("L", f.read(4))[0]
                size = unpack("L", f.read(4))[0]
                if addr == 0xffffffff:
                    break
                elif size == 0xffffffff:
                    break
                else:
                    self.sectSize.append(size)
                    self.sectAddr.append(addr)


    def get_algo_start(self):
        # Search the DevName part of the FlashDevice description (FlashDev.c)
        # for anything matching the ALGO_START_ADDRESSES dictionary
        for target in ALGO_START_ADDRESSES:
            if target in self.devName:
                print 'Identified target as %s' % (target)
                return ALGO_START_ADDRESSES[target]
        print 'Found no match in ALGO_START_ADDRESSES for "%s"' % (self.devName)
        raise Error()

    def printInfo(self):
        print "Extracted device information:"
        print "----------------------------"
        print "Version:        0x%04x" % (self.version)
        print "Device Name:    %s" % (self.devName)
        print "Device Type:    %u" % (self.devType)
        print "Device Address: 0x%08x" % (self.devAddr)
        print "Device Size:    0x%08x" % (self.szDev)
        print "Prog Page Size: %u" % (self.szPage)
        print "valEmpty:       0x%02x" % (self.valEmpty)
        print "Timeout Prog:   %u" % (self.toProg)
        print "Timeout Erase:  %u" % (self.toErase)
        for i in range(len(self.sectSize)):
            print "Sectors[%d]: { 0x%08x, 0x%08x }" % (i, self.sectSize[i], self.sectAddr[i])


def gen_flash_algo(filename_path, dev_info_path, algo_bin_path):

    output_file = os.path.splitext(filename_path)[0]
    output_file += ".txt"

    run_cmd([FROMELF, '--bin', filename_path, '-o', TMP_DIR_W_TERM])
    try:
        flash_info = FlashInfo(DEV_INFO_PATH)
        ALGO_START = flash_info.get_algo_start()
        flash_info.printInfo()
    except IOError, e:
        print "No flash info file found: %s, using default start address." % DEV_INFO_PATH
        ALGO_START = 0x20000000

    print "ALGO_START = 0x%08x\n" % ALGO_START

    with open(ALGO_BIN_PATH, "rb") as f, open(output_file, mode="w+") as res:
        # Flash Algorithm
        res.write("""
const uint32_t flash_algo_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    """);

        nb_bytes = ALGO_OFFSET

        bytes_read = f.read(1024)
        while bytes_read:
            bytes_read = unpack(str(len(bytes_read)/4) + 'I', bytes_read)
            for i in range(len(bytes_read)):
                res.write(("0x%08x" % bytes_read[i]) + ", ")
                nb_bytes += 4
                if (nb_bytes % 0x20) == 0:
                    res.write("\n    ") # % nb_bytes)
            bytes_read = f.read(1024)

        res.write("\n};\n")

        # Address of the functions within the flash algorithm
        stdout, _, _ = run_cmd([FROMELF, '-s', filename_path])
        res.write("""
static const TARGET_FLASH flash = {
""")
        for line in stdout.splitlines():
            t = line.strip().split()
            if len(t) != 8: continue
            name, loc = t[1], t[2]

            if name in ['Init', 'UnInit', 'EraseChip', 'EraseSector', 'ProgramPage']:
                addr = ALGO_START + ALGO_OFFSET + int(loc, 16)
                res.write("    0x%08X, // %s\n" % (addr,  name))


if __name__ == '__main__':
    for elf_file in ALGO_ELFS_PATH:
        gen_flash_algo( elf_file,
                        elf_file.replace(".axf", "_" + "DevDscr"),
                        elf_file.replace(".axf", "_" + "PrgCode") )
