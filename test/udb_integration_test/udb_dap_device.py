#!/usr/bin/env python3
from typing import ClassVar
from pyocd.probe import pydapaccess
from datetime import datetime, timedelta
import logging
from udb_test_helper import UDBTestResources

logger = logging.getLogger("test.udb_integration_test")

class DapDeviceError(Exception):
    pass

class UDBDapTestDevice:
    timeout: ClassVar[timedelta] = timedelta(seconds=0.5)

    def __enter__(self):
        self.device = None
        unique_id = None
        start = datetime.now()
        while unique_id == None:
            for dev in pydapaccess.DAPAccess.get_connected_devices():
                if dev.product_name[0:7] == "DAPLink":
                    if unique_id is None:
                        unique_id = dev.get_unique_id()
                    else:
                        logger.warning("WARNING: multiple DAPLinks are connected! Picking the" \
                                       " first one...")
            if datetime.now() - start > UDBTestResources.get_expected_boot_timedelta():
                raise DapDeviceError("Timeout: Can't find any DAPLink device connected")

        self.device = pydapaccess.DAPAccess.get_device(unique_id)
        if self.device is None:
            raise DapDeviceError("Can't get DAPLink device object")

        self.device.open()
        return self

    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.device.close()

    def run_dap_test(self,
                     command_number: int,
                     send_data: list[int],
                     expected_data: list[int]=None,
                     expected_data_bit_mask: list[int]=None) -> list:
        result = self.device.vendor(command_number, send_data)
        logger.debug("Result: ", result)
        if expected_data and expected_data_bit_mask:
            # Not yet implemented
            assert False
        return result

    def get_device_dap_capabilities(self) -> int:
        return self.device._capabilities

    def get_udb_interface_version(self) -> str:
        vendor_cmd_id_get_version = 36
        result = self.device.vendor(vendor_cmd_id_get_version, [0])
        version = "".join(map(chr, result))
        logger.debug("".join(map(chr, result)))
        return version
