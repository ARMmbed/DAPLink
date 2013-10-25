"""
CMSIS-DAP Interface Firmware
Copyright (c) 2009-2013 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""
from paths import TMP_USB_LOG

USBC = "55 53 42 43"

SCSI = {
    0x00: 'TEST_UNIT_READY',
    0x03: 'REQUEST_SENSE',
    0x04: 'FORMAT_UNIT',
    0x12: 'INQUIRY',
    0x15: 'MODE_SELECT6',
    0x1A: 'MODE_SENSE6',
    0x1B: 'START_STOP_UNIT',
    0x1E: 'MEDIA_REMOVAL',
    0x23: 'READ_FORMAT_CAPACITIES',
    0x25: 'READ_CAPACITY',
    0x28: 'READ10',
    0x2A: 'WRITE10',
    0x2F: 'VERIFY10',
    0x35: 'SYNC_CACHE10',
    0xA8: 'READ12',
    0xAA: 'WRITE12',
    0x55: 'MODE_SELECT10',
    0x5A: 'MODE_SENSE10',
    0x91: 'SYNC_CACHE16',
    0xA1: 'ATA_COMMAND_PASS_THROUGH12',
    0x85: 'ATA_COMMAND_PASS_THROUGH16',
    0xAB: 'SERVICE_ACTION_IN12',
    0x9E: 'SERVICE_ACTION_IN16',
    0xA9: 'SERVICE_ACTION_OUT12',
    0x9F: 'SERVICE_ACTION_OUT16',
}

log = open(TMP_USB_LOG)

for packet in log:
    if packet.startswith(USBC):
        fields = [int(x, 16) for x in packet.strip().split()]
        scsi = fields[15]
        
        if scsi == 0x2A:
            block_n = fields[17] << 24 | fields[18] << 16 | fields[19] << 8 | fields[20]
            print SCSI[scsi], block_n
        