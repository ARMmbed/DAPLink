#!/usr/bin/env python
#
# DAPLink Interface Firmware
# Copyright (c) 2009-2021, Arm Limited, All Rights Reserved
# Copyright (c) 2021 Mathias Brossard
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
import re
import sys
import yaml

PROJECTS_YAML = "projects.yaml"

project_list = []
with open(PROJECTS_YAML, 'r') as top_yaml:
    try:
        topdict = yaml.safe_load(top_yaml)
        for dict_key in topdict:
            if dict_key == 'projects':
                for project in topdict[dict_key]:
                    project_list.append(project)
                break
    except yaml.YAMLError as ex:
        print("Found yaml parse error", ex)

_IF_RE = re.compile("^[a-z0-9]+_if$")
_BL_RE = re.compile("^[a-z0-9]+_bl$")

hics = []
boards = []
for p in project_list:
    if _IF_RE.match(p) or _BL_RE.match(p):
        hics.append(p)
    else:
        boards.append(p)

hics.sort()
boards.sort()
all_sorted = hics + boards

for p in hics:
    print(p)
print()
for p in boards:
    print(p)

for i in range(len(project_list)):
    if project_list[i] != all_sorted[i]:
        sys.stderr.write("Found '%s' expected '%s'\n" % (project_list[i], all_sorted[i]))
        exit(-1)

exit(0)
