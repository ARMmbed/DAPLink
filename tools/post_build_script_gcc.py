#!/usr/bin/env python
#
# DAPLink Interface Firmware
# Copyright (c) 2009-2020, ARM Limited, All Rights Reserved
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

import os
from post_build_script import post_build_script

project_dir = 'build'
project_dir = os.path.join(os.getcwd(), 'build')
project_name = os.path.basename(os.getcwd())
infile = os.path.join(project_dir, project_name + '.hex')
outbase = os.path.join(project_dir, project_name + '_crc')

if not os.path.exists(infile):
    print("File '%s' is missing" % infile)
    exit(-1)

run = False
if not os.path.exists(outbase + '.bin'):
    run = True
elif os.path.getmtime(infile) >= os.path.getmtime(outbase + '.bin'):
    run = True
# else:
#     print("%s already up-to-date" % outbase)

if run:
    # print("%s -> %s" % (infile, outbase))
    post_build_script(infile, outbase)
