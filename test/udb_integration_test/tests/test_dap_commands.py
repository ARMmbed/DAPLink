#!/usr/bin/env python
from typing import ClassVar, Generator
from udb_dap_device import UDBDapTestDevice
from udb_test_helper import ContextTest
from pyocd.probe.pydapaccess.cmsis_dap_core import Capabilities
import logging

logger = logging.getLogger("test.udb_integration_test")

class DAPCommandTest(ContextTest):
    udb: UDBDapTestDevice

    def context(self) -> Generator:
        with UDBDapTestDevice() as self.udb:
            yield

    def test_dap_cmd_info_for_swd_capability(self) -> None:
        # Verify the DAP info command returned the expected data when the pyocd device was
        # initialized
        self.assertTrue((self.udb.get_device_dap_capabilities() & Capabilities.SWD) != 0,
                        "No SWD capability returned in DAP info")

    def test_dap_vendor_command_version(self) -> None:
        # Verify device responds to vendor commands
        self.assertTrue((self.udb.get_udb_interface_version()[1:5] == "udb_",
                        "Wrong version returned"))
