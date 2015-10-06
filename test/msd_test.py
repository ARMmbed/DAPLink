# CMSIS-DAP Interface Firmware
# Copyright (c) 2009-2015 ARM Limited
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

from __future__ import absolute_import
from __future__ import division
import os
import time
import shutil
import six

BOARD_ID_LOCKED_WHEN_ERASED = set([
    0x0231,  # K22F
    0x0240,  # K64F
])


def _same(d1, d2):
    assert type(d1) is bytearray
    assert type(d2) is bytearray
    for i in range(min(len(d1), len(d2))):
        if d1[i] != d2[i]:
            return False
    if len(d1) != len(d2):
        return False
    return True


class MassStorageTester(object):

    def __init__(self, board, parent_test, test_name):
        self.board = board
        self.parent_test = parent_test
        self.test_name = test_name
        self._expected_failure_msg = None
        self._flush_time = 0.1
        self._load_with_shutils = None
        self._flush_size = None
        self._programming_data = None

    def set_shutils_copy(self, source_file_name):
        """
        Change mode to shutil file copy

        This option cannot be used with set_programming_data.
        """
        assert type(source_file_name) is str
        assert self._load_with_shutils is None
        self._source_file_name = source_file_name
        self._load_with_shutils = True

    def set_programming_data(self, data, file_name):
        """
        Set data to program over mass storage

        Data should be the conetents of the hex or binary file
        being loaded. This option cannot be used with set_shutils_copy.
        """
        assert type(data) is bytearray
        assert type(file_name) is str
        assert(self._load_with_shutils is False or
               self._load_with_shutils is None)
        self._load_with_shutils = False
        self._programming_data = data
        mount_point = self.board.get_mount_point()
        self._programming_file_name = os.path.normpath(mount_point + os.sep +
                                                       file_name)

    def set_flush_size(self, size):
        """Set the block size to simulate a flush of"""
        assert isinstance(size, six.integer_types)
        self._flush_size = size

    def set_expected_data(self, data):
        """Data that should have been written to the device"""
        assert data is None or type(data) is bytearray
        self._expected_data = data

    def set_expected_failure_msg(self, msg):
        """Set the expected failure message as a string"""
        assert msg is None or type(msg) is str
        self._expected_failure_msg = msg

    def run(self):
        # Expected data must be set, even if to None
        assert hasattr(self, '_expected_data')
        test_info = self.parent_test.create_subtest(self.test_name)

        # Write data to the file
        start = time.time()
        if self._load_with_shutils:
            # Copy with shutils
            shutil.copy(self._source_file_name, self.board.get_mount_point())
        elif self._flush_size is not None:
            # Simulate flushes during the file transfer
            # Note - The file is explicitly opened and closed to more
            #        consistently simulate the undesirable behavior flush can
            #        cause.  On Windows flushing a file causes the data to be
            #        written out immediately, but only sometimes causes the
            #        filesize to get updated.
            size = len(self._programming_data)
            for addr in range(0, size, self._flush_size):
                data = self._programming_data[addr:addr + self._flush_size]
                with open(self._programming_file_name, 'ab') as file_handle:
                    file_handle.write(data)
                time.sleep(self._flush_time)
        else:
            # Perform a normal copy
            with open(self._programming_file_name, 'wb') as load_file:
                load_file.write(self._programming_data)
        stop = time.time()
        diff = stop - start
        test_info.info('Loading took %ss' % diff)
        if self._expected_data is not None:
            test_info.info('Programming rate %sB/s' %
                           (len(self._expected_data) / diff))
        if self._programming_data is not None:
            test_info.info('MSD tranfer rate %sB/s' %
                           (len(self._programming_data) / diff))

        self.board.wait_for_remount(test_info)

        # Verify the disk is still valid
        self.board.test_fs(test_info)

        # Check various failure cases
        msg = self.board.get_failure_message()
        failure_expected = self._expected_failure_msg is not None
        failure_occured = msg is not None
        if failure_occured and not failure_expected:
            test_info.failure('Device reported failure: %s' % msg)
            return
        if failure_expected and not failure_occured:
            test_info.failure('Failure expected but did not occur')
            return
        if failure_expected and failure_occured:
            if msg == self._expected_failure_msg:
                test_info.info('Failure as expected: %s' % msg)
            else:
                test_info.failure('Failure but wrong string: %s vs %s' %
                                  (msg, self._expected_failure_msg))
            return

        # These cases should have been handled above
        assert not failure_expected
        assert not failure_occured

        # If there is expected data then compare
        if self._expected_data:
            data_len = len(self._expected_data)
            data_loaded = self.board.read_target_memory(0, data_len)
            if not _same(self._expected_data, data_loaded):
                test_info.failure('Data does not match')
            else:
                test_info.info("Data matches")


def test_mass_storage(board, parent_test):
    """Test the mass storage endpoint

    Requirements:
        None

    Positional arguments:
        filename - A string containing the name of the file to load

    Return:
        True if the test passed, False otherwise
    """
    test_info = parent_test.create_subtest('test_mass_storage')

    # Setup test
    bin_file = board.get_target_bin_path()
    hex_file = board.get_target_hex_path()
    with open(bin_file, 'rb') as test_file:
        bin_file_contents = bytearray(test_file.read())
    with open(hex_file, 'rb') as test_file:
        hex_file_contents = bytearray(test_file.read())
    blank_bin_contents = bytearray([0xff]) * 0x2000
    vectors_and_pad = bin_file_contents[0:16] + blank_bin_contents
    locked_when_erased = board.get_board_id() in BOARD_ID_LOCKED_WHEN_ERASED

    # Test loading a binary file with shutils
    test = MassStorageTester(board, test_info, "Shutil binary file load")
    test.set_shutils_copy(bin_file)
    test.set_expected_data(bin_file_contents)
    test.run()

    # Test loading a binary file with flushes
    test = MassStorageTester(board, test_info, "Load binary with flushes")
    test.set_programming_data(bin_file_contents, 'image.bin')
    test.set_expected_data(bin_file_contents)
    test.set_flush_size(0x1000)
    test.run()

    # Test loading a hex file with shutils
    test = MassStorageTester(board, test_info, "Shutil hex file load")
    test.set_shutils_copy(hex_file)
    test.set_expected_data(bin_file_contents)
    test.run()

    # Test loading a hex file with flushes
    test = MassStorageTester(board, test_info, "Load hex with flushes")
    test.set_programming_data(hex_file_contents, 'image.hex')
    test.set_expected_data(bin_file_contents)
    test.run()

    # Test loading a binary smaller than a sector
    test = MassStorageTester(board, test_info, "Load .bin smaller than sector")
    test_data_size = 0x789
    test_data = bin_file_contents[0:0 + test_data_size]
    test.set_programming_data(test_data, 'image.bin')
    test.set_expected_data(test_data)
    test.run()

    # Test loading a blank binary - this image should cause a timeout
    #    since it doesn't have a valid vector table
    test = MassStorageTester(board, test_info, "Load blank binary")
    test.set_programming_data(blank_bin_contents, 'image.bin')
    test.set_expected_failure_msg("The transfer timed out.\r\n")
    test.set_expected_data(None)
    test.run()

    # Test loading a blank binary with a vector table but padded with 0xFF.
    #    A blank image can lock some devices.
    test = MassStorageTester(board, test_info, "Load blank binary + vector table")
    test.set_programming_data(vectors_and_pad, 'image.bin')
    if locked_when_erased:
        test.set_expected_failure_msg("The interface firmware ABORTED programming. Image is trying to set security bits\r\n")
        test.set_expected_data(None)
    else:
        test.set_expected_data(vectors_and_pad)
    test.run()

    # Finally, load a good binary
    test = MassStorageTester(board, test_info, "Load good binary to restore state")
    test.set_programming_data(bin_file_contents, 'image.bin')
    test.set_expected_data(bin_file_contents)
    test.run()

    # Ideas for future tests - contributions welcome
    # -Zero length file
    # -Corrupt hex file
    # -Dummy files loaded before test
    # -Very large file
    # -Any MSD regressions
    # -Security bits in hex files
