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

Extract and patch the interface without bootloader
"""
from options import get_options
from paths import get_interface_path, TMP_DIR
from utils import gen_binary, is_lpc, split_path

from os.path import join


if __name__ == '__main__':
    options = get_options()
    
    in_path = get_interface_path(options.interface, options.target, bootloader=False)
    _, name, _ = split_path(in_path)
    out_path = join(TMP_DIR, name + '.bin')
    
    print '\nELF: %s' % in_path
    gen_binary(in_path, out_path, is_lpc(options.interface))
    print "\nBINARY: %s" % out_path
