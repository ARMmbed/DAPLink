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
import yaml
from collections import OrderedDict

root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def ordered_dict_presenter(dumper, data):
    return dumper.represent_dict(data.items())
yaml.add_representer(OrderedDict, ordered_dict_presenter)

with open(os.path.join(root_dir, "_data", 'update.yml'), "r") as yml_file:
    yml = yaml.safe_load(yml_file.read())

t = []

for i in yml:
    if 'name' in i:
        t.append(OrderedDict(
            name=i['name'],
            product_code=i['product_code'],
            fw_name=i['fw_name'],
            instructions='lpc11u35' if ('lpc11u35' in i['fw_name']) else 'default'
        ))

t.sort(key=lambda i: i['name'])
with open(os.path.join(root_dir, '_data', 'firmwares.yml'), "w") as yml_file:
    yml_file.write("---\n")
    yml_file.write(yaml.dump(t))
