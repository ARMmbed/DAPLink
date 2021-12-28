#!/usr/bin/env python
#
# DAPLink Interface Firmware
# Copyright (c) 2009-2021, Arm Limited, All Rights Reserved
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

from pyocd.probe.pydapaccess.dap_access_api import DAPAccessIntf
from pyocd.probe.cmsis_dap_probe import CMSISDAPProbe
from pyocd.probe.debug_probe import DebugProbe
from pyocd.core.session import Session

for probe in CMSISDAPProbe.get_all_connected_probes():
    probe.session = Session(probe, False)
    probe.open()
    probe.connect(DebugProbe.Protocol.SWD)

    # 0x01 = Get the Vendor Name (string).
    # 0x02 = Get the Product Name (string).
    # 0x03 = Get the Serial Number (string).
    # 0x04 = Get the CMSIS-DAP Protocol Version (string).
    # 0x05 = Get the Target Device Vendor (string).
    # 0x06 = Get the Target Device Name (string).
    # 0x07 = Get the Target Board Vendor (string).
    # 0x08 = Get the Target Board Name (string).
    # 0x09 = Get the Product Firmware Version (string, vendor-specific format).
    print("Vendor Name: %s" % probe._link._protocol.dap_info(DAPAccessIntf.ID.VENDOR))
    print("Product Name: %s" % probe._link._protocol.dap_info(DAPAccessIntf.ID.PRODUCT))
    print("Serial Number: %s" % probe._link._protocol.dap_info(DAPAccessIntf.ID.SER_NUM))
    print("CMSIS-DAP Protocol Version: %s" % probe._link._protocol.dap_info(DAPAccessIntf.ID.CMSIS_DAP_PROTOCOL_VERSION))
    print("Target Device Vendor: %s" % probe._link._protocol.dap_info(DAPAccessIntf.ID.DEVICE_VENDOR))
    print("Target Device Name: %s" % probe._link._protocol.dap_info(DAPAccessIntf.ID.DEVICE_NAME))
    print("Target Board Vendor: %s" % probe._link._protocol.dap_info(DAPAccessIntf.ID.BOARD_VENDOR))
    print("Target Board Name: %s" % probe._link._protocol.dap_info(DAPAccessIntf.ID.BOARD_NAME))
    print("Product Firmware Version: %s" % probe._link._protocol.dap_info(DAPAccessIntf.ID.PRODUCT_FW_VERSION))