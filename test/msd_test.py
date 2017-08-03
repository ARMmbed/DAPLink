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
import os
import time
import shutil
import six
import info
import intelhex
from test_info import TestInfo


def _same(d1, d2):
    assert type(d1) is bytearray
    assert type(d2) is bytearray
    for i in range(min(len(d1), len(d2))):
        if d1[i] != d2[i]:
            return False
    if len(d1) != len(d2):
        return False
    return True

MOCK_DIR_LIST = [
    "test",
    "blarg",
    "very_long_directory_name",
    "very_long_directory_name/and_subdirectory_name"
]

MOCK_FILE_LIST = [
    (".test", "blarg"),
    ("test/file1", "asdofahweaw"),
    ("file.jpg", "file contents here")
]

MOCK_DIR_LIST_AFTER = [
    "test2",
    "blarg2",
    "very_long_directory_name2",
    "very_long_directory_name2/and_subdirectory_name"
]

MOCK_FILE_LIST_AFTER = [
    (".test2", "blarg"),
    ("test2/file12", "asdofahweaw"),
    ("file2.jpg", "file contents here")
]

class MassStorageTester(object):

    RETRY_COUNT = 5
    DELAY_BEFORE_RETRY_S = 30

    def __init__(self, board, parent_test, test_name):
        self.board = board
        self.parent_test = parent_test
        self.test_name = test_name
        self._expected_failure_msg = None
        self._flush_time = 0.1
        self._load_with_shutils = None
        self._flush_size = None
        self._programming_data = None
        self._mock_file_list = []
        self._mock_dir_list = []
        self._mock_file_list_after = []
        self._mock_dir_list_after = []
        self._programming_file_name = None
        self._start = 0

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
        self._programming_file_name = file_name

    def set_flush_size(self, size):
        """Set the block size to simulate a flush of"""
        assert isinstance(size, six.integer_types)
        self._flush_size = size

    def set_expected_data(self, data, start=0):
        """Data that should have been written to the device"""
        assert data is None or type(data) is bytearray
        self._expected_data = data
        self._start = start

    def set_expected_failure_msg(self, msg):
        """Set the expected failure message as a string"""
        assert msg is None or type(msg) is str
        self._expected_failure_msg = msg

    def add_mock_files(self, file_list):
        """Add a list of tuples containing a file and contents"""
        self._mock_file_list.extend(file_list)

    def add_mock_dirs(self, dir_list):
        """Add a list of directoies"""
        self._mock_dir_list.extend(dir_list)

    def add_mock_files_after_load(self, file_list):
        """Add a list of tuples containing a file and contents"""
        self._mock_file_list_after.extend(file_list)

    def add_mock_dirs_after_load(self, dir_list):
        """Add a list of directoies"""
        self._mock_dir_list_after.extend(dir_list)

    def _check_data_correct(self, expected_data, test_info):
        """Return True if the actual data written matches the expected"""
        print (test_info)
        data_len = len(expected_data)
        data_loaded = self.board.read_target_memory(self._start, data_len)
        return _same(expected_data, data_loaded)

    def run(self):
        for retry_count in range(self.RETRY_COUNT):
            test_info = TestInfo(self.test_name)
            if retry_count > 0:
                test_info.info('Previous attempts %s' % retry_count)
            try:
                self._run(test_info)
            except IOError:
                time.sleep(self.DELAY_BEFORE_RETRY_S)
                # Update board info since a remount could have occurred
                self.board.update_board_info()
                continue
            self.parent_test.attach_subtest(test_info)
            break
        else:
            raise Exception("Flashing failed after %i retries" %
                            self.RETRY_COUNT)

    def _run(self, test_info):
        # Expected data must be set, even if to None
        assert hasattr(self, '_expected_data')

        # Copy mock files before test
        self._mock_file_list = []
        for dir_name in self._mock_dir_list:
            dir_path = self.board.get_file_path(dir_name)
            os.mkdir(dir_path)
        for file_name, file_contents in self._mock_file_list:
            file_path = self.board.get_file_path(file_name)
            with open(file_path, 'wb') as file_handle:
                file_handle.write(file_contents)

        programming_file_name = None
        if self._programming_file_name is not None:
            programming_file_name = \
                self.board.get_file_path(self._programming_file_name)

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
                with open(programming_file_name, 'ab') as file_handle:
                    file_handle.write(data)
                time.sleep(self._flush_time)
        else:
            # Perform a normal copy
            with open(programming_file_name, 'wb') as load_file:
                load_file.write(self._programming_data)
        stop = time.time()
        diff = stop - start
        test_info.info('Loading took %ss' % diff)
        if self._expected_data is not None:
            test_info.info('Programming rate %sB/s' %
                           (len(self._expected_data) / diff))
        if self._programming_data is not None:
            test_info.info('MSD transfer rate %sB/s' %
                           (len(self._programming_data) / diff))

        # Copy mock files after loading
        self._mock_file_list = []
        for dir_name in self._mock_dir_list_after:
            dir_path = self.board.get_file_path(dir_name)
            os.mkdir(dir_path)
        for file_name, file_contents in self._mock_file_list_after:
            file_path = self.board.get_file_path(file_name)
            with open(file_path, 'wb') as file_handle:
                file_handle.write(file_contents)

        self.board.wait_for_remount(test_info)

        # Verify the disk is still valid
        self.board.test_fs(test_info)

        # Check various failure cases
        msg = self.board.get_failure_message()
        failure_expected = self._expected_failure_msg is not None
        failure_occured = msg is not None
        if failure_occured and not failure_expected:
            test_info.failure('Device reported failure: "%s"' % msg.strip())
            return
        if failure_expected and not failure_occured:
            test_info.failure('Failure expected but did not occur')
            return
        if failure_expected and failure_occured:
            if msg == self._expected_failure_msg:
                test_info.info('Failure as expected: "%s"' % msg.strip())
            else:
                test_info.failure('Failure but wrong string: "%s" vs "%s"' %
                                  (msg.strip(),
                                   self._expected_failure_msg.strip()))
            return

        # These cases should have been handled above
        assert not failure_expected
        assert not failure_occured

        # If there is expected data then compare
        if self._expected_data:
            if self._check_data_correct(self._expected_data, test_info):
                test_info.info("Data matches")
            else:
                test_info.failure('Data does not match')


def test_mass_storage(workspace, parent_test):
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
    board = workspace.board
    target = workspace.target
    bin_file = target.bin_path
    hex_file = target.hex_path
    with open(bin_file, 'rb') as test_file:
        bin_file_contents = bytearray(test_file.read())
    with open(hex_file, 'rb') as test_file:
        hex_file_contents = bytearray(test_file.read())
    blank_bin_contents = bytearray([0xff]) * 0x2000
    vectors_and_pad = bin_file_contents[0:32] + blank_bin_contents
    locked_when_erased = board.get_board_id() in info.BOARD_ID_LOCKED_WHEN_ERASED
    bad_vector_table = target.name in info.TARGET_WITH_BAD_VECTOR_TABLE_LIST

    intel_hex = intelhex.IntelHex(hex_file)
    addresses = intel_hex.addresses()
    addresses.sort()
    start = addresses[0]

    # Test loading a binary file with shutils
    if not bad_vector_table:
        test = MassStorageTester(board, test_info, "Shutil binary file load")
        test.set_shutils_copy(bin_file)
        test.set_expected_data(bin_file_contents, start)
        test.run()

    # Test loading a binary file with flushes
    if not bad_vector_table:
        test = MassStorageTester(board, test_info, "Load binary with flushes")
        test.set_programming_data(bin_file_contents, 'image.bin')
        test.set_expected_data(bin_file_contents, start)
        test.set_flush_size(0x1000)
        test.run()

    # Test loading a hex file with shutils
    test = MassStorageTester(board, test_info, "Shutil hex file load")
    test.set_shutils_copy(hex_file)
    test.set_expected_data(bin_file_contents, start)
    test.run()

    # Test loading a hex file with flushes
    test = MassStorageTester(board, test_info, "Load hex with flushes")
    test.set_programming_data(hex_file_contents, 'image.hex')
    test.set_expected_data(bin_file_contents, start)
    test.set_flush_size(0x1000)
    test.run()

    # Test loading a binary smaller than a sector
    if not bad_vector_table:
        test = MassStorageTester(board, test_info, "Load .bin smaller than sector")
        test_data_size = 0x789
        test_data = bin_file_contents[0:0 + test_data_size]
        test.set_programming_data(test_data, 'image.bin')
        test.set_expected_data(test_data, start)
        test.run()

    # Test loading a blank binary - this image should cause a timeout
    #    since it doesn't have a valid vector table
    test = MassStorageTester(board, test_info, "Load blank binary")
    test.set_programming_data(blank_bin_contents, 'image.bin')
    test.set_expected_failure_msg("The transfer timed out.\r\n")
    test.set_expected_data(None, start)
    test.run()

    # Test loading a blank binary with a vector table but padded with 0xFF.
    #    A blank image can lock some devices.
    if not bad_vector_table:
        test = MassStorageTester(board, test_info, "Load blank binary + vector table")
        test.set_programming_data(vectors_and_pad, 'image.bin')
        if locked_when_erased:
            test.set_expected_failure_msg("The interface firmware ABORTED programming. Image is trying to set security bits\r\n")
            test.set_expected_data(None, start)
        else:
            test.set_expected_data(vectors_and_pad, start)
        test.run()

    # Test a normal load with dummy files created beforehand
    test = MassStorageTester(board, test_info, "Extra Files")
    test.set_programming_data(hex_file_contents, 'image.hex')
    test.add_mock_dirs(MOCK_DIR_LIST)
    test.add_mock_files(MOCK_FILE_LIST)
    test.add_mock_dirs_after_load(MOCK_DIR_LIST_AFTER)
    test.add_mock_files_after_load(MOCK_FILE_LIST_AFTER)
    test.set_expected_data(bin_file_contents, start)
    test.run()
    # Note - it is not unexpected for an "Extra Files" test to fail
    #        when a binary file is loaded, since there is no way to
    #        tell where the end of the file is.

    # Finally, load a good binary
    test = MassStorageTester(board, test_info, "Load good file to restore state")
    test.set_programming_data(hex_file_contents, 'image.hex')
    test.set_expected_data(bin_file_contents, start)
    test.run()

    # Ideas for future tests - contributions welcome
    # -Zero length file
    # -Corrupt hex file
    # -Dummy files loaded before test
    # -Very large file
    # -Any MSD regressions
    # -Security bits in hex files
    # -Hex file with data at the end **
    # -Hidden files
    # -change file extension
    # -Change size (make smaller)
    # -change starting address
