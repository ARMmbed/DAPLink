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

from time import sleep
from random import randrange
import math

from pyOCD.board import MbedBoard
from pyOCD.utility.conversion import float32beToU32be

# TODO - make a dedicated test
# TODO - test all DapLink commands
# TODO - test various clock speeds
# TODO - test turnaround settings
# TODO - test HID speed
# TODO - test ram/rom transfer speeds


def test_hid(workspace, parent_test):
    test_info = parent_test.create_subtest("HID test")
    board = workspace.board
    with MbedBoard.chooseBoard(board_id=board.get_unique_id()) as mbed_board:
        binary_file = workspace.target.bin_path

        memory_map = mbed_board.target.getMemoryMap()
        ram_regions = [region for region in memory_map if region.type == 'ram']
        ram_region = ram_regions[0]
        rom_region = memory_map.getBootMemory()

        addr = ram_region.start + 4
        size = 0x502
        addr_bin = rom_region.start
        addr_flash = rom_region.start + rom_region.length // 2

        target = mbed_board.target
        flash = mbed_board.flash

        test_info.info("\r\n\r\n----- FLASH NEW BINARY -----")
        flash.flashBinary(binary_file, addr_bin)

        test_info.info("\r\n\r\n------ GET Unique ID ------")
        test_info.info("Unique ID: %s" % mbed_board.getUniqueID())

        test_info.info("\r\n\r\n------ TEST READ / WRITE CORE REGISTER ------")
        pc = target.readCoreRegister('pc')
        test_info.info("initial pc: 0x%X" % target.readCoreRegister('pc'))
        # write in pc dummy value
        target.writeCoreRegister('pc', 0x3D82)
        test_info.info("now pc: 0x%X" % target.readCoreRegister('pc'))
        # write initial pc value
        target.writeCoreRegister('pc', pc)
        test_info.info("initial pc value rewritten: 0x%X" %
                       target.readCoreRegister('pc'))

        msp = target.readCoreRegister('msp')
        psp = target.readCoreRegister('psp')
        test_info.info("MSP = 0x%08x; PSP = 0x%08x" % (msp, psp))

        control = target.readCoreRegister('control')
        faultmask = target.readCoreRegister('faultmask')
        basepri = target.readCoreRegister('basepri')
        primask = target.readCoreRegister('primask')
        test_info.info("CONTROL = 0x%02x; FAULTMASK = 0x%02x; BASEPRI = 0x%02x; PRIMASK = 0x%02x" % (control, faultmask, basepri, primask))

        target.writeCoreRegister('primask', 1)
        newPrimask = target.readCoreRegister('primask')
        test_info.info("New PRIMASK = 0x%02x" % newPrimask)
        target.writeCoreRegister('primask', primask)
        newPrimask = target.readCoreRegister('primask')
        test_info.info("Restored PRIMASK = 0x%02x" % newPrimask)

        if target.has_fpu:
            s0 = target.readCoreRegister('s0')
            test_info.info("S0 = %g (0x%08x)" % (s0, float32beToU32be(s0)))
            target.writeCoreRegister('s0', math.pi)
            newS0 = target.readCoreRegister('s0')
            test_info.info("New S0 = %g (0x%08x)" %
                           (newS0, float32beToU32be(newS0)))
            target.writeCoreRegister('s0', s0)
            newS0 = target.readCoreRegister('s0')
            test_info.info("Restored S0 = %g (0x%08x)" %
                           (newS0, float32beToU32be(newS0)))

        test_info.info("\r\n\r\n------ TEST HALT / RESUME ------")

        test_info.info("resume")
        target.resume()
        sleep(0.2)

        test_info.info("halt")
        target.halt()
        test_info.info("HALT: pc: 0x%X" % target.readCoreRegister('pc'))
        sleep(0.2)

        test_info.info("\r\n\r\n------ TEST STEP ------")

        test_info.info("reset and halt")
        target.resetStopOnReset()
        currentPC = target.readCoreRegister('pc')
        test_info.info("HALT: pc: 0x%X" % currentPC)
        sleep(0.2)

        for i in range(4):
            test_info.info("step")
            target.step()
            newPC = target.readCoreRegister('pc')
            test_info.info("STEP: pc: 0x%X" % newPC)
            currentPC = newPC
            sleep(0.2)

        test_info.info("\r\n\r\n------ TEST READ / WRITE MEMORY ------")
        target.halt()
        test_info.info("READ32/WRITE32")
        val = randrange(0, 0xffffffff)
        test_info.info("write32 0x%X at 0x%X" % (val, addr))
        target.writeMemory(addr, val)
        res = target.readMemory(addr)
        test_info.info("read32 at 0x%X: 0x%X" % (addr, res))
        if res != val:
            test_info.failure("ERROR in READ/WRITE 32")

        test_info.info("\r\nREAD16/WRITE16")
        val = randrange(0, 0xffff)
        test_info.info("write16 0x%X at 0x%X" % (val, addr + 2))
        target.writeMemory(addr + 2, val, 16)
        res = target.readMemory(addr + 2, 16)
        test_info.info("read16 at 0x%X: 0x%X" % (addr + 2, res))
        if res != val:
            test_info.failure("ERROR in READ/WRITE 16")

        test_info.info("\r\nREAD8/WRITE8")
        val = randrange(0, 0xff)
        test_info.info("write8 0x%X at 0x%X" % (val, addr + 1))
        target.writeMemory(addr + 1, val, 8)
        res = target.readMemory(addr + 1, 8)
        test_info.info("read8 at 0x%X: 0x%X" % (addr + 1, res))
        if res != val:
            test_info.failure("ERROR in READ/WRITE 8")

        test_info.info("\r\n\r\n------ TEST READ / WRITE MEMORY BLOCK ------")
        data = [randrange(1, 50) for _ in range(size)]
        target.writeBlockMemoryUnaligned8(addr, data)
        block = target.readBlockMemoryUnaligned8(addr, size)
        error = False
        for i in range(len(block)):
            if (block[i] != data[i]):
                error = True
                test_info.info("ERROR: 0x%X, 0x%X, 0x%X!!!" %
                               ((addr + i), block[i], data[i]))
        if error:
            test_info.failure("TEST FAILED")
        else:
            test_info.info("TEST PASSED")

        test_info.info("\r\n\r\n------ TEST RESET ------")
        target.reset()
        sleep(0.1)
        target.halt()

        for i in range(5):
            target.step()
            test_info.info("pc: 0x%X" % target.readCoreRegister('pc'))

        test_info.info("\r\n\r\n------ TEST PROGRAM/ERASE PAGE ------")
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
        # Verify the 1st and 3rd page were not erased, and that
        # the 2nd page is fully erased
        data = target.readBlockMemoryUnaligned8(addr_flash, page_size * 3)
        expected = fill + [0xFF] * page_size + fill
        if data == expected:
            test_info.info("TEST PASSED")
        else:
            test_info.failure("TEST FAILED")

        test_info.info("\r\n\r\n----- Restoring image -----")
        flash.flashBinary(binary_file, addr_bin)

        target.reset()
        test_info.info("HID test complete")
