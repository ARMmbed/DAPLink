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
from subprocess import Popen, PIPE, call
from os.path import split, splitext

from settings import FROMELF
from patch import patch


def run_cmd(command, wd=None, check_rc=True):
    p = Popen(command, stdout=PIPE, stderr=PIPE, cwd=wd)
    stdout, stderr = p.communicate()
    rc = p.returncode

    if check_rc and rc != 0:
        raise Exception('"%s" returned %d\n%s' % (' '.join(command), rc, stderr))

    return stdout, stderr, rc


def gen_binary(input, output, lpc_patch=False):
    run_cmd([FROMELF, '--bin', '--output=%s' % output, input])
    if lpc_patch == True:
        patch(output)


def is_lpc(name):
    return name.startswith('LPC')


def split_path(path):
    base, file = split(path)
    name, ext = splitext(file)
    return base, name, ext
