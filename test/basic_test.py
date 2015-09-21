# CMSIS-DAP Interface Firmware
# Copyright (c) 2009-2013 ARM Limited
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
#

import argparse, os, sys
from time import sleep
from random import randrange
import math

import pyOCD
from pyOCD.board import MbedBoard
from pyOCD.utility.conversion import float32beToU32be
import logging

def basic_test(board_id, file):
    with MbedBoard.chooseBoard(board_id=board_id) as board:
        addr = 0
        size = 0
        f = None
        binary_file = "l1_"

        interface = None

        target_type = board.getTargetType()

        if file is None:
            binary_file = os.path.join(parentdir, 'binaries', board.getTestBinary())
        else:
            binary_file = file

        print "binary file: %s" % binary_file

        addr_bin = 0x00000000

        if target_type == "lpc1768":
            addr = 0x10000001
            size = 0x1102
            addr_flash = 0x10000
        elif target_type == "lpc11u24":
            addr = 0x10000001
            size = 0x502
            addr_flash = 0x4000
        elif target_type == "kl25z":
            addr = 0x20000001
            size = 0x502
            addr_flash = 0x10000
        elif target_type == "kl28z":
            addr = 0x20000001
            size = 0x502
            addr_flash = 0x10000
        elif target_type == "k64f":
            addr = 0x20000001
            size = 0x502
            addr_flash = 0x10000
        elif target_type == "k22f":
            addr = 0x20000001
            size = 0x502
            addr_flash = 0x10000
        elif target_type == "k20d50m":
            addr = 0x20000001
            size = 0x502
            addr_flash = 0x10000
        elif target_type == "kl46z":
            addr = 0x20000001
            size = 0x502
            addr_flash = 0x10000
        elif target_type == "lpc800":
            addr = 0x10000001
            size = 0x502
            addr_flash = 0x2000
        elif target_type == "nrf51":
            addr = 0x20000001
            size = 0x502
            addr_flash = 0x20000
        elif target_type == "lpc4330":
            addr = 0x10000001
            size = 0x1102
            addr_flash = 0x14010000
            addr_bin = 0x14000000
        elif target_type == "maxwsnenv":
            addr = 0x20000001
            size = 0x502
            addr_flash = 0x10000
        elif target_type == "max32600mbed":
            addr = 0x20000001
            size = 0x502
            addr_flash = 0x10000
        elif target_type == "w7500":
            addr = 0x20000001
            size = 0x1102
            addr_flash = 0x00000000
        else:
            raise Exception("A board is not supported by this test script.")

        target = board.target
        transport = board.transport
        flash = board.flash
        interface = board.interface

        print "\r\n\r\n----- FLASH NEW BINARY -----"
        flash.flashBinary(binary_file, addr_bin)

        print "\r\n\r\n------ GET Unique ID ------"
        print "Unique ID: %s" % board.getUniqueID()

        print "\r\n\r\n------ TEST READ / WRITE CORE REGISTER ------"
        pc = target.readCoreRegister('pc')
        print "initial pc: 0x%X" % target.readCoreRegister('pc')
        # write in pc dummy value
        target.writeCoreRegister('pc', 0x3D82)
        print "now pc: 0x%X" % target.readCoreRegister('pc')
        # write initial pc value
        target.writeCoreRegister('pc', pc)
        print "initial pc value rewritten: 0x%X" % target.readCoreRegister('pc')

        msp = target.readCoreRegister('msp')
        psp = target.readCoreRegister('psp')
        print "MSP = 0x%08x; PSP = 0x%08x" % (msp, psp)

        control = target.readCoreRegister('control')
        faultmask = target.readCoreRegister('faultmask')
        basepri = target.readCoreRegister('basepri')
        primask = target.readCoreRegister('primask')
        print "CONTROL = 0x%02x; FAULTMASK = 0x%02x; BASEPRI = 0x%02x; PRIMASK = 0x%02x" % (control, faultmask, basepri, primask)

        target.writeCoreRegister('primask', 1)
        newPrimask = target.readCoreRegister('primask')
        print "New PRIMASK = 0x%02x" % newPrimask
        target.writeCoreRegister('primask', primask)
        newPrimask = target.readCoreRegister('primask')
        print "Restored PRIMASK = 0x%02x" % newPrimask

        if target.has_fpu:
            s0 = target.readCoreRegister('s0')
            print "S0 = %g (0x%08x)" % (s0, float32beToU32be(s0))
            target.writeCoreRegister('s0', math.pi)
            newS0 = target.readCoreRegister('s0')
            print "New S0 = %g (0x%08x)" % (newS0, float32beToU32be(newS0))
            target.writeCoreRegister('s0', s0)
            newS0 = target.readCoreRegister('s0')
            print "Restored S0 = %g (0x%08x)" % (newS0, float32beToU32be(newS0))


        print "\r\n\r\n------ TEST HALT / RESUME ------"

        print "resume"
        target.resume()
        sleep(0.2)

        print "halt"
        target.halt()
        print "HALT: pc: 0x%X" % target.readCoreRegister('pc')
        sleep(0.2)


        print "\r\n\r\n------ TEST STEP ------"

        print "reset and halt"
        target.resetStopOnReset()
        currentPC = target.readCoreRegister('pc')
        print "HALT: pc: 0x%X" % currentPC
        sleep(0.2)

        for i in range(4):
            print "step"
            target.step()
            newPC = target.readCoreRegister('pc')
            print "STEP: pc: 0x%X" % newPC
            currentPC = newPC
            sleep(0.2)


        print "\r\n\r\n------ TEST READ / WRITE MEMORY ------"
        target.halt()
        print "READ32/WRITE32"
        val = randrange(0, 0xffffffff)
        print "write32 0x%X at 0x%X" % (val, addr)
        target.writeMemory(addr, val)
        res = target.readMemory(addr)
        print "read32 at 0x%X: 0x%X" % (addr, res)
        if res != val:
            print "ERROR in READ/WRITE 32"

        print "\r\nREAD16/WRITE16"
        val = randrange(0, 0xffff)
        print "write16 0x%X at 0x%X" % (val, addr + 2)
        target.writeMemory(addr + 2, val, 16)
        res = target.readMemory(addr + 2, 16)
        print "read16 at 0x%X: 0x%X" % (addr + 2, res)
        if res != val:
            print "ERROR in READ/WRITE 16"

        print "\r\nREAD8/WRITE8"
        val = randrange(0, 0xff)
        print "write8 0x%X at 0x%X" % (val, addr + 1)
        target.writeMemory(addr + 1, val, 8)
        res = target.readMemory(addr + 1, 8)
        print "read8 at 0x%X: 0x%X" % (addr + 1, res)
        if res != val:
            print "ERROR in READ/WRITE 8"


        print "\r\n\r\n------ TEST READ / WRITE MEMORY BLOCK ------"
        data = [randrange(1, 50) for x in range(size)]
        target.writeBlockMemoryUnaligned8(addr, data)
        block = target.readBlockMemoryUnaligned8(addr, size)
        error = False
        for i in range(len(block)):
            if (block[i] != data[i]):
                error = True
                print "ERROR: 0x%X, 0x%X, 0x%X!!!" % ((addr + i), block[i], data[i])
        if error:
            print "TEST FAILED"
        else:
            print "TEST PASSED"


        print "\r\n\r\n------ TEST RESET ------"
        target.reset()
        sleep(0.1)
        target.halt()

        for i in range(5):
            target.step()
            print "pc: 0x%X" % target.readCoreRegister('pc')

        print "\r\n\r\n------ TEST PROGRAM/ERASE PAGE ------"
        # Fill 3 pages with 0x55
        page_size = flash.getPageInfo(addr_flash).size
        fill = [0x55] * page_size
        flash.init()
        for i in range(0, 3):
            address = addr_flash + page_size * i
            # Test only supports a location with 3 aligned
            # pages of the same size
            current_page_size = flash.getPageInfo(addr_flash).size
            assert page_size == current_page_size
            assert address % current_page_size == 0
            flash.erasePage(address)
            flash.programPage(address, fill)
        # Erase the middle page
        flash.erasePage(addr_flash + page_size)
        # Verify the 1st and 3rd page were not erased, and that the 2nd page is fully erased
        data = target.readBlockMemoryUnaligned8(addr_flash, page_size * 3)
        expected = fill + [0xFF] * page_size + fill
        if data == expected:
            print "TEST PASSED"
        else:
            print "TEST FAILED"

        target.reset()
        print "HID test complete"

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='A CMSIS-DAP python debugger')
    parser.add_argument('-f', help='binary file', dest="file")
    parser.add_argument('-d', '--debug', action="store_true", help='Enable debug logging')
    args = parser.parse_args()
    level = logging.DEBUG if args.debug else logging.INFO
    logging.basicConfig(level=level)
    file = args.file
    basic_test("1050e72fef76000000000000000000000000f77fef76", file)
