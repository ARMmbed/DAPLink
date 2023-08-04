#!/usr/bin/env python
from typing import Generator
from udb_serial_device import UDBSerialTestDevice
from udb_dap_device import UDBDapTestDevice
from datetime import datetime
from unittest import skipUnless
import logging
from udb_test_helper import UDBTestResources, ContextTest, indent_string

logger = logging.getLogger("test.udb_integration_test")

class USBStressTest(ContextTest):
    udb_dap: UDBDapTestDevice
    udb_serial: UDBSerialTestDevice

    def context(self) -> Generator:
        with UDBSerialTestDevice(baudrate=3000000) as self.udb_serial:
            with UDBDapTestDevice() as self.udb_dap:
                yield

    def test_usb(self) -> None:
        start = datetime.now()
        count = 0
        while True:
            self.udb_serial.command_no_wait("TEST1TEST2TEST3TEST4TEST5TEST6TEST7TEST8TEST9TEST" \
                                            "TEST1TEST2TEST3TEST4TEST5TEST6TEST7TEST8TEST9TEST")
            self.assertEqual(self.udb_dap.get_udb_interface_version()[1:5],
                             "udb_", "DAP commandreplies are bad")
            count += 1
            if (datetime.now() - start).seconds > 15:
                break
        self.udb_serial.flush()
        output = self.udb_serial.command("help")
        self.assertRegex(output, "show available commands")
        logger.info("\n" + indent_string(f"Read and wrote {count} times..."))
