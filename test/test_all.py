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
"""
DAPLink validation and testing tool

optional arguments:
  -h, --help           show this help message and exit
  --user USER          MBED username
  --password PASSWORD  MBED password

This script runs test to validate that the DAPLink firmware running
on all boards attached to this pc is working correctly.  In specific
this patch validates that the MSD, CDC and HID endpoints are
functioning correctly.

To run this script the user name and password to a valid MBED account
must be supplied.  This is so test firmware for the board being tested
can be built using the mbed build api, which requires a valid username
and password.
"""
import os
import traceback
import serial
import shutil
import argparse
import mbedapi
import mbed_lstools
import basic_test

TEST_REPO = 'https://developer.mbed.org/users/c1728p9/code/daplink-validation/'

name_to_build_target = {
    'K22F': 'FRDM-K22F',
    'LPC812': 'NXP-LPC800-MAX'
}


def same(d1, d2):
    d1 = bytearray(d1)
    d2 = bytearray(d2)

    for i in range(min(len(d1), len(d2))):
        if d1[i] != d2[i]:
            print("at %i %i != %i" % (i, d1[i], d2[i]))
            return False
    if len(d1) != len(d2):
        print("Lengths differ: %i, %i" % (len(d1), len(d2)))
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

low_standard_baud = [
    110,
    300,
    600,
    1200,
    2400,
    4800,
]

high_standard_baud = [
    128000,
    153600,
    230400,
    256000,
    460800,
    921600,
    ]


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


def test_serial(port):
    """Test the serial port endpoint

    Requirements:
        -daplink-validation must be loaded for the target.

    Positional arguments:
        port - the serial port to open as a string

    Return:
        True if the test passed, False otherwise
    """
    test_passed = True

    # Generate a 8 KB block of dummy data
    # and test a large block transfer
    test_data = [i for i in range(0, 256)] * 4 * 8
    baud = 115200
    timeout = calc_timeout(test_data, baud)
    with serial.Serial(port, baudrate=baud, timeout=timeout) as sp:

        # Reset the target
        sp.sendBreak()

        # Wait until the target is initialized
        expected_resp = "{init}"
        resp = sp.read(len(expected_resp))
        if not same(resp, expected_resp):
            print "Fail on init: %s" % resp
            test_passed = False

        sp.write(test_data)
        resp = sp.read(len(test_data))
        if same(resp, test_data):
            print "Block test passed"
        else:
            print "Block test failed"
            test_passed = False

    # Generate a 4KB block of dummy data
    # and test supported baud rates
    test_data = [i for i in range(0, 256)] * 4 * 4
    for baud in standard_baud:
        # Setup with a baud of 115200
        with serial.Serial(port, baudrate=115200, timeout=1) as sp:

            # Reset the target
            sp.sendBreak()

            # Wait until the target is initialized
            expected_resp = "{init}"
            resp = sp.read(len(expected_resp))
            if not same(resp, expected_resp):
                print "Fail on init: %s" % resp
                test_passed = False
                continue

            # Change baudrate to that of the first test
            command = "{baud:%i}" % baud
            sp.write(command)
            resp = sp.read(len(command))
            if not same(resp, command):
                print "Fail on baud command: %s" % resp
                test_passed = False
                continue

        # Open serial port at the new baud
        print("Testing baud %i" % baud)
        test_time = calc_timeout(test_data, baud)
        with serial.Serial(port, baudrate=baud, timeout=test_time) as sp:

            # Read the response indicating that the baudrate
            # on the target has changed
            expected_resp = "{change}"
            resp = sp.read(len(expected_resp))
            if not same(resp, expected_resp):
                sp.write("Baud test")
                print "Fail on baud change %s" % resp
                test_passed = False
                continue

            # Perform test
            sp.write(test_data)
            resp = sp.read(len(test_data))
            resp = bytearray(resp)
            if same(test_data, resp):
                print "Pass"
            else:
                print "Fail"
                test_passed = False

    return test_passed


def test_mass_storage(filename, mount_point):
    """Test the mass storage endpoint

    Requirements:
        None

    Positional arguments:
        filename - A string containing the name of the file to load

    Return:
        True if the test passed, False otherwise
    """
    # TODO - parse filesystem
    shutil.copy(filename, mount_point)
    # TODO - verify binary is correct
    # TODO - test copy speed
    # TODO - test all supported file formats
    # TODO - test expected failure cases / corrupt files
    return True


def test_hid(target_id, filename):
    """Test the HID endpoint

    Requirements:
        None

    Positional arguments:
        target_id - A string containing board identifier
        filename - A string containing the name of the file to load

    Return:
        True if the test passed, False otherwise
    """
    basic_test.basic_test(target_id, filename)
    # TODO - make a dedicated test
    # TODO - test all DapLink commands
    # TODO - test various clock speeds
    # TODO - test turnaround settings
    # TODO - test HID speed
    # TODO - test ram/rom transfer speeds
    return True


def test_board(user, password, target_id, serial_port,
               mount_point, platform_name, cached=False):
    """Run tests to validate DAPLINK fimrware"""
    print("**Testing board %s**" % platform_name)
    test_passed = True
    try:
        platform = name_to_build_target[platform_name]
        repo = TEST_REPO
        destdir = 'tmp'
        if not os.path.isdir(destdir):
            os.mkdir(destdir)

        print
        filename = os.path.join("tmp",  platform_name + ".bin")
        if cached and os.path.isfile(filename):
            print "Using cached file"
        else:
            print 'Starting remote build'
            if os.path.isfile(filename):
                os.remove(filename)
            built_file = mbedapi.build_repo(user, password, repo, platform,
                                            destdir)
            os.rename(built_file, filename)
            print 'Remote build finished'

        # TODO - Load boot block
        # TODO - Load interface
        print
        print 'Starting HID'
        test_passed &= test_hid(target_id, filename)
        print
        print 'Starting Serial port test'
        test_passed &= test_serial(serial_port)
        print
        print 'Starting Mass storage test'
        test_passed &= test_mass_storage(filename, mount_point)
    except Exception:
        traceback.print_exc()
        test_passed = False

    if test_passed:
        print "Board %s passed" % platform_name
    else:
        print "Board %s failed" % platform_name
    return test_passed


if __name__ == "__main__":
    description = 'DAPLink validation and testing tool'
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('--user', type=str,
                        help='MBED username', required=True)
    parser.add_argument('--password', type=str,
                        help='MBED password', required=True)
    parser.add_argument('--cached', help='Use cached build',
                        action="store_true", default=False, required=False)
    args = parser.parse_args()

    test_passed = True
    lstools = mbed_lstools.create()
    mbed_list = lstools.list_mbeds()
    for mbed in mbed_list:
        target_id = mbed['target_id']
        serial_port = mbed['serial_port']
        mount_point = mbed['mount_point']
        platform_name = mbed['platform_name']
        test_passed &= test_board(args.user, args.password, target_id,
                                  serial_port, mount_point, platform_name,
                                  args.cached)
    if test_passed:
        print "All boards passed"
        exit(0)
    else:
        print "Test Failed"
        exit(-1)
