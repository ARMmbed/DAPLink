#!/usr/bin/env python
from udb_serial_device import UDBSerialTestDevice
from udb_dap_device import UDBDapTestDevice
from datetime import datetime
from udb_mass_storage_device import UDBMassStorageDevice
from unittest import TestCase
from udb_test_helper import UDBTestResources, indent_string
import logging

logger = logging.getLogger("test.udb_integration_test")

class SoftwareUpdateTest(TestCase):
    def test_swu(self) -> None:
        with UDBDapTestDevice() as udb_dap:
            version_1 = udb_dap.get_udb_interface_version()
            logger.info("\n" + indent_string(f"Version before test: {version_1}"))
        with UDBSerialTestDevice() as udb:
            logger.info(indent_string("Resetting into SWU mode..."))
            udb.command_no_wait("\nreset_into_swu_mode")
        with UDBMassStorageDevice() as udb:
            start = datetime.now()
            logger.info(indent_string("Copying dummy binary with different version..."))
            udb.copy_firmware(UDBTestResources.get_path_to_binary_with_diff_version())
        with UDBSerialTestDevice() as udb:
            self.assertLess(udb.get_time_to_open(),
                            UDBTestResources.get_expected_boot_timedelta(),
                            msg="Regression in boot time")
        with UDBDapTestDevice() as udb_dap:
            version_2 = udb_dap.get_udb_interface_version()
            swu_time_taken = datetime.now() - start
            logger.info(indent_string(f"Version after update:{version_2}"))
        with UDBSerialTestDevice() as udb:
            logger.info(indent_string("Resetting into SWU mode..."))
            udb.command_no_wait("\nreset_into_swu_mode")
        with UDBMassStorageDevice() as udb:
            logger.info(indent_string("Copying back the original test binary..."))
            udb.copy_firmware(UDBTestResources.get_path_to_current_binary())
        with UDBSerialTestDevice() as udb:
            self.assertLess(udb.get_time_to_open(),
                            UDBTestResources.get_expected_boot_timedelta(),
                            msg="Regression in boot time")
        with UDBDapTestDevice() as udb_dap:
            version_3 = udb_dap.get_udb_interface_version()
            logger.info(indent_string(f"Version after the test:{version_3}"))

        logger.info(indent_string(f"The software update test took {swu_time_taken.seconds}s"))

        expected_swu_time_sec = 30
        self.assertLess(swu_time_taken.seconds, expected_swu_time_sec, f"SWU took too long")
        self.assertEqual(version_1, version_3, "The firmware version is not the same after the " \
                         "test as before the test. You probably provide the wrong binary.")
        self.assertNotEqual(version_1, version_2, "The version after the test software update " \
                            "is the same as before, SWU probably failed or the wrong dummy " \
                            "binary was provided.")
