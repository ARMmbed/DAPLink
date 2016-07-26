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
from __future__ import division
from threading import Thread
import serial


def _same(d1, d2):
    d1 = bytearray(d1)
    d2 = bytearray(d2)

    for i in range(min(len(d1), len(d2))):
        if d1[i] != d2[i]:
            return False
    if len(d1) != len(d2):
        return False
    return True

# http://digital.ni.com/public.nsf/allkb/D37754FFA24F7C3F86256706005B9BE7
standard_baud = [
    9600,
    14400,
    19200,
    28800,
    38400,
    56000,
    57600,
    115200,
    ]
timing_test_baud = standard_baud[3:]


def calc_timeout(data, baud):
    """Calculate a timeout given the data and baudrate

    Positional arguments:
        data - data to be sent
        baud - baud rate to send data

    Calculate a reasonable timeout given the supplied parameters.
    This function adds slightly more time then is needed, to accont
    for latency and various configurations.
    """
    return 12 * len(data) / float(baud) + 0.2


def test_serial(workspace, parent_test):
    """Test the serial port endpoint

    Requirements:
        -daplink-validation must be loaded for the target.

    Positional arguments:
        port - the serial port to open as a string

    Return:
        True if the test passed, False otherwise
    """
    test_info = parent_test.create_subtest("Serial test")
    port = workspace.board.get_serial_port()
    test_info.info("Testing serial port %s" % port)

    # Note: OSX sends a break command when a serial port is closed.
    # To avoid problems while testing keep the serial port open the
    # whole time.  Use the property 'baudrate' to change the baud
    # instead of opening a new instance.

    # Generate a 8 KB block of dummy data
    # and test a large block transfer
    test_data = [i for i in range(0, 256)] * 4 * 8
    test_data = str(bytearray(test_data))
    baud = 115200
    timeout = calc_timeout(test_data, baud)
    with serial.Serial(port, baudrate=baud, timeout=timeout) as sp:

        # Reset the target
        sp.sendBreak()

        # Wait until the target is initialized
        expected_resp = "{init}"
        resp = sp.read(len(expected_resp))
        if not _same(resp, expected_resp):
            test_info.failure("Fail on init: %s" % resp)

        write_thread = Thread(target=sp.write, args=(test_data,))
        write_thread.start()
        resp = sp.read(len(test_data))
        write_thread.join()
        if _same(resp, test_data):
            test_info.info("Block test passed")
        else:
            test_info.failure("Block test failed")

        # Generate a 4KB block of dummy data
        # and test supported baud rates
        test_data = [i for i in range(0, 256)] * 4 * 4
        test_data = str(bytearray(test_data))
        for baud in standard_baud:
            # Set baud to 115200
            sp.baudrate = 115200
            sp.timeout = 1.0

            # Reset the target
            sp.sendBreak()

            # Wait until the target is initialized
            expected_resp = "{init}"
            resp = sp.read(len(expected_resp))
            if not _same(resp, expected_resp):
                test_info.failure("Fail on init: %s" % resp)
                continue

            # Change baudrate to that of the first test
            command = "{baud:%i}" % baud
            sp.write(command)
            resp = sp.read(len(command))
            if not _same(resp, command):
                test_info.failure("Fail on baud command: %s" % resp)
                continue

            # Update baud for test
            test_info.info("Testing baud %i" % baud)
            test_time = calc_timeout(test_data, baud)
            sp.baudrate = baud
            sp.timeout = test_time

            # Read the response indicating that the baudrate
            # on the target has changed
            expected_resp = "{change}"
            resp = sp.read(len(expected_resp))
            if not _same(resp, expected_resp):
                test_info.failure("Fail on baud change %s" % resp)
                continue

            # Perform test
            write_thread = Thread(target=sp.write, args=(test_data,))
            write_thread.start()
            resp = sp.read(len(test_data))
            write_thread.join()
            resp = bytearray(resp)
            if _same(test_data, resp):
                test_info.info("Pass")
            else:
                test_info.failure("Fail on baud %s" % baud)

        # Timing stress test - send data at critical points
        # in time like right as the transmitter is turned off
        # ------------------
        # Test sequence
        # 1. Send a block of data (vary size for the test)
        # 2. Wait until 1 bytes is ready back
        # 3. Write 1 byte
        # 4. Read back all data
        test_data = [i for i in range(0, 256)] * 4 * 4
        test_data = str(bytearray(test_data))
        for baud in timing_test_baud:
            # Set baud to 115200
            sp.baudrate = 115200
            sp.timeout = 1.0

            # Reset the target
            sp.sendBreak()

            # Wait until the target is initialized
            expected_resp = "{init}"
            resp = sp.read(len(expected_resp))
            if not _same(resp, expected_resp):
                test_info.failure("Fail on init: %s" % resp)
                continue

            # Change baudrate to that of the first test
            command = "{baud:%i}" % baud
            sp.write(command)
            resp = sp.read(len(command))
            if not _same(resp, command):
                test_info.failure("Fail on baud command: %s" % resp)
                continue

            # Update baud for test
            test_info.info("Timing test baud %i" % baud)
            test_time = calc_timeout(test_data, baud)
            sp.baudrate = baud
            sp.timeout = test_time

            # Read the response indicating that the baudrate
            # on the target has changed
            expected_resp = "{change}"
            resp = sp.read(len(expected_resp))
            if not _same(resp, expected_resp):
                test_info.failure("Fail on baud change %s" % resp)
                continue

            test_pass = True
            for data_size in range(1, 10):
                data = test_data[0:data_size + 1]
                for _ in range(0, 1000):
                    resp = bytearray()

                    sp.write(data[0:data_size])
                    resp += sp.read(1)
                    sp.write(data[-1:])
                    resp += sp.read(data_size)

                    if not _same(data, resp):
                        test_pass = False
                        test_info.info("fail size - %s" % data_size)
                        break

            if test_pass:
                test_info.info("Pass")
            else:
                test_info.failure("Fail on timing test with baud %s"
                                  % baud)
