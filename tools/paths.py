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
from os.path import join, abspath, dirname
ROOT = abspath(join(dirname(__file__), ".."))

TMP_DIR = join(ROOT, 'tools', 'tmp')

def get_bootloader_path(interface):
    return join(ROOT, 'bootloader/mdk/%(interface)s/Obj/%(interface)s_bootloader.axf' % {
                            'interface': interface.lower()})


def get_interface_path(interface, target, bootloader=True):
    return join(ROOT, "interface/mdk/%(interface)s/Obj/%(interface)s_%(target)s_if_mbed%(has_bootloader)s.axf" % {
                        'interface': interface.lower(),
                        'target': target.lower(),
                        'has_bootloader': '_bootloader' if bootloader else ''})


def get_image_path(interface, target):
    return join(TMP_DIR, 'if_%(interface)s__target_%(target)s.bin' % {
                            'interface': interface.lower(),
                            'target': target.lower()})


TMP_BOOTLOADER_BIN_PATH = join(TMP_DIR, "tmp_bootloader.bin")
TMP_INTERFACE_BIN_PATH  = join(TMP_DIR, "tmp_interface.bin")

TMP_USB_LOG = join(TMP_DIR, 'usb_log.txt')
