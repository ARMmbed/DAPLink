#!/usr/bin/env python
import pexpect
from psutil import boot_time
from serial import Serial, SerialException
from pexpect.spawnbase import SpawnBase
import sys
import re
from datetime import datetime, timedelta

from udb_test_helper import UDBTestResources

class WatchdogError(Exception):
    pass

class OopsError(Exception):
    pass

class UDBSerialTestDeviceError(Exception):
    pass

class SerialExpect(SpawnBase):
    ''' Implementation of pexpect's SpawnBase for use with pyserial '''

    def __init__(self, ser: Serial, **kwargs) -> None:
        SpawnBase.__init__(self, **kwargs)

        self.closed = False
        self.name = ser.name
        self.ser = ser

    def close(self) -> None:
        self.flush()
        self.ser.close()

    def isalive(self) -> bool:
        return self.ser.is_open

    def send(self, s: str) -> int:
        s = self._coerce_send_string(s)
        self._log(s, 'send')

        s = self._encoder.encode(s, final=False)
        return self.ser.write(s)

    def sendline(self, s: str) -> int:
        s = self._coerce_send_string(s)
        return self.send(s + self.linesep)

    def write(self, s: str) -> None:
        self.send(s)

    def writelines(self, sequence: list[str]) -> None:
        for s in sequence:
            self.write(s)

    def read_nonblocking(self, size: int=1, timeout: float=-1) -> str:
        if timeout == -1:
            timeout = self.timeout

        to_read = min(self.ser.in_waiting, size)
        data = self.ser.read(to_read)
        data = self._decoder.decode(data, final=False)
        self._log(data, 'read')
        return data

class UDBSerialTestDevice:
    ser: Serial
    dev: SerialExpect
    cached_output: str
    time_to_open: timedelta

    def __init__(self, baudrate: int=1000000, debug: bool=False) -> None:
        start = datetime.now()
        while True:
            try:
                self.ser = Serial(UDBTestResources.get_serial_port_path(), baudrate)
                break
            except (SerialException, FileNotFoundError, UDBSerialTestDeviceError, OSError) as e:
                if datetime.now() - start > UDBTestResources.get_expected_boot_timedelta() * 2:
                    raise UDBSerialTestDeviceError("Timeout and error while trying to open " \
                                                   "serial port:", e)
                continue
        self.time_to_open = datetime.now() - start
        self.dev = SerialExpect(self.ser)

        # Log data coming in to stdout
        if debug:
            self.dev.logfile = sys.stdout.buffer

    def __enter__(self):
        self.flush()
        return self

    def __exit__(self, exc_type, exc_value, traceback) -> None:
        self.dev.close()

    def get_time_to_open(self):
        return self.time_to_open

    def wait_for(self, string: str, timeout: float=10) -> str:
        idx = self.dev.expect([
            # 0, EOF
            pexpect.EOF,
            # 1, TIMEOUT
            pexpect.TIMEOUT,
            # 2, watchdog happened
            '!!wdog!!',
            # 3, Oops occurred
            'Oops!',
            # 4, got expected string. No errors!
            string],
            timeout=timeout)

        output = self.dev.before.decode('utf-8')

        if idx == 0:
            raise EOFError(output)
        elif idx == 1:
            raise TimeoutError(output)
        elif idx == 2:
            raise WatchdogError(output)
        elif idx == 3:
            raise OopsError(output)
        else:
            match = re.search('DONE ([0-9-]*)\r', output)
            if match:
                code = int(match.group(1))
                if code != 0:
                    raise ValueError
            return output

    def wait_for_prompt(self, timeout: float=10) -> str:
        return self.wait_for('\n> ', timeout=timeout)

    def command(self, cmd: str, timeout: float=10) -> str:
        self.command_no_wait(cmd)
        return self.wait_for_prompt(timeout=timeout)

    def commands(self, cmds: str, timeout: float=10) -> list[str]:
        return [self.command(cmd, timeout) for cmd in cmds]

    def command_no_wait(self, cmd: str) -> None:
        self.dev.send(cmd + '\n')

    def update_baud_rate(self, baud: int) -> None:
        self.ser.baudrate = baud

    def flush(self) -> None:
        self.ser.flush()
        if sys.platform.lower() != 'darwin':
            self.ser.reset_input_buffer()
        self.ser.reset_output_buffer()
        try:
            while(True):
                self.wait_for_prompt(0.1)
        except Exception:
            pass
