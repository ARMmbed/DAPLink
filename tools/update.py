#!/usr/bin/env python
#
# DAPLink Interface Firmware
# Copyright (c) 2009-2021, ARM Limited, All Rights Reserved
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

import yaml


import os
import sys
import shutil
import argparse
import yaml
import subprocess
import zipfile
from collections import OrderedDict

root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def ordered_dict_presenter(dumper, data):
    return dumper.represent_dict(data.items())
yaml.add_representer(OrderedDict, ordered_dict_presenter)

with open(os.path.join(root_dir, "_data", 'releases.yml'), "r") as yml_file:
    releases = yaml.safe_load(yml_file.read())
with open(os.path.join(root_dir, "_data", 'default.yml'), "r") as yml_file:
    default = yaml.safe_load(yml_file.read())
with open(os.path.join(root_dir, "_data", 'firmwares.yml'), "r") as yml_file:
    firmwares = yaml.safe_load(yml_file.read())

o = []

# sorted(firmwares, key=lambda i: i['name'])
for t in firmwares:
    ext = t.get('image_format', default['image_format'])
    f = OrderedDict()
    for r in reversed(releases):
        p = "%s_%s%s" % (r, t['fw_name'], ext)
        if os.path.exists(os.path.join('firmware', p)):
            f[r] = p
    o.append(OrderedDict(
        name=t['name'],
        product_code=t['product_code'],
        fw_name=t['fw_name'],
        fw_versions=f,
        instructions=t['instructions']
    ))

with open(os.path.join(root_dir, '_data', 'firmwares.yml'), "w") as yml_file:
    yml_file.write("---\n")
    yml_file.write(yaml.dump(o))
