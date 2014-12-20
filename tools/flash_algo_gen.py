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
This script takes as input an .axf file containing the flash algorithm to be
loaded in the target RAM and it converts it to a binary array ready to be
included in the CMSIS-DAP Interface Firmware source code.
"""
from struct import unpack
from os.path import join
import os
import sys

from utils import run_cmd
from settings import *
from paths import TMP_DIR

# FIXED LENGTH
ALGO_OFFSET = 0x20

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
    'LPC4337':  0x10000000,
    'MKXX':     0x20000000,
    '':     0x20000000,
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


def gen_flash_algo():
    if len(sys.argv) < 2:
        print "usage: >python flash_algo_gen.py <abs_path_w_elf_name>"
        sys.exit()
        
    ALGO_ELF_PATH_NAME = sys.argv[1]
    ALGO_ELF_PATH, ALGO_ELF_NAME = os.path.split(ALGO_ELF_PATH_NAME)
    DEV_INFO_PATH = join(ALGO_ELF_PATH, "DevDscr")
    ALGO_BIN_PATH = join(ALGO_ELF_PATH, "PrgCode")
    # need some work here to name and locate to a collective folder
    ALGO_TXT_PATH = join(ALGO_ELF_PATH, "flash_algo.txt")

    run_cmd([FROMELF, '--bin', ALGO_ELF_PATH_NAME, '-o', ALGO_ELF_PATH])
    try:
        flash_info = FlashInfo(DEV_INFO_PATH)
        ALGO_START = flash_info.get_algo_start()
    except IOError, e:
        print repr(e), e
        ALGO_START = 0x20000000
    print "ALGO_START = 0x%08x\n" % ALGO_START

    flash_info.printInfo()

    with open(ALGO_BIN_PATH, "rb") as f, open(ALGO_TXT_PATH, mode="w+") as res:
        # Flash Algorithm - these instructions are the ALGO_OFFSET
        res.write("""
const uint32_t flash_algorithm_blob[] = {
    0xE00ABE00, 0x062D780D, 0x24084068, 0xD3000040, 0x1E644058, 0x1C49D1FA, 0x2A001E52, 0x4770D1F2,
    """);

        nb_bytes = ALGO_OFFSET
        prg_data = ''

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
        stdout, _, _ = run_cmd([FROMELF, '-s', ALGO_ELF_PATH_NAME])
        # run a diagnostic if prompted to
        if sys.argv[1] == '-v':
            print stdout
        res.write("""
static const TARGET_FLASH flash_algorithm_struct = {
""")
        for line in stdout.splitlines():
            t = line.strip().split()
            if len(t) < 5: continue
            name, loc, sec = t[1], t[2], t[4]
            
            if name in ['Init', 'UnInit', 'EraseChip', 'EraseSector', 'ProgramPage']:
                addr = ALGO_START + ALGO_OFFSET + int(loc, 16)
                res.write("    0x%08X, // %s\n" % (addr,  name))

            if name == '$d.realdata':
                if sec == '2':
                    prg_data = int(loc, 16)

        res.write("    // breakpoint = RAM start + 1\n")
        res.write("    // RSB : base address is address of Execution Region PrgData in map file\n")
        res.write("    //       to access global/static data\n")
        res.write("    // RSP : Initial stack pointer\n")
        res.write("    {\n")
        res.write("        0x%08X, // breakpoint instruction address\n" % (ALGO_START+1))
        res.write("        0x%08X + 0x%X + 0x%X,  // static base register value (image start + header + static base offset)\n" % (ALGO_START, ALGO_OFFSET, prg_data))
        res.write("        0x%08X // initial stack pointer\n" % (ALGO_START+2048))
        res.write("    },\n\n")
        res.write("    0x%08X, // flash_buffer, any valid RAM location with > 512 bytes of working room and proper alignment\n" % (ALGO_START+2048+256))
        res.write("    0x%08X, // algo_start, start of RAM\n" % ALGO_START)
        res.write("    sizeof(flash_algorithm_blob), // algo_size, size of array above\n")
        res.write("    flash_algorithm, // image, flash algo instruction array\n")
        res.write("    512              // ram_to_flash_bytes_to_be_written\n")
        res.write("};\n\n")


if __name__ == '__main__':
    gen_flash_algo()
