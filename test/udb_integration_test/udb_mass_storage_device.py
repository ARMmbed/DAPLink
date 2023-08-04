#!/usr/bin/env python
import subprocess
from datetime import datetime
from time import sleep
import os
import logging
from udb_test_helper import UDBTestResources, indent_string

logger = logging.getLogger("test.udb_integration_test")

class UDBMassStorageError(Exception):
    pass

class UDBMassStorageDevice:
    temp_folder_name: str = "./temp_udb_drive"

    def __enter__(self):
        start = datetime.now()
        self.path_to_udb_drive = None
        self.tried_mounting = False
        drive_name_to_search = "DAPLINK"
        logger.info("\tWaiting for DAPLINK_BL drive to automount...")
        while self.path_to_udb_drive == None:
            output = self.run_cmd_and_find_string("mount", drive_name_to_search)
            if output:
                self.path_to_udb_drive = output[2]
                search_dur_s = (datetime.now() - start).seconds
                logger.info(f"\tFound UDB drive at {self.path_to_udb_drive} after {search_dur_s}s")
                break
            if datetime.now() - start > UDBTestResources.get_expected_boot_timedelta():
                if not self.tried_mounting:
                    self.tried_mounting = True
                    logger.info("\tAutomount failed, mounting manually")
                    drive_name_to_search = self.mount_udb_device()
                else:
                    raise UDBMassStorageError("Timeout: can't find UDB drive")
            sleep(0.1)
        return self

    def __exit__(self, exc_type, exc_value, traceback) -> None:
        if self.tried_mounting:
            self.run_shell_command(f"sudo umount {self.temp_folder_name}")
            sleep(0.1)
            self.run_shell_command(f"rm -rf {self.temp_folder_name}")

    def run_shell_command(self, cmd: str, secs_before_kill: float =0.1) -> str:
        proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        try:
            proc_out, err = proc.communicate(timeout=secs_before_kill)
            proc.kill()
        except subprocess.TimeoutExpired:
            proc.kill()
            proc_out, err = proc.communicate()

        if err is not None:
            logger.error(str(proc_out[:60]) + "...")
            raise UDBMassStorageError(f"Error running command \"{cmd}\": ", err)
        elif len(proc_out) > 0:
            logger.debug(indent_string(str(proc_out[:120]) + "..."))
        return proc_out.decode("utf-8")

    def run_cmd_and_find_string(self, command: str, string: str) -> list[str]:
        out = self.run_shell_command(command)
        drive_list = out.split("\n")
        for drive in drive_list:
            if string in drive:
                return drive.split(" ")
        return []

    def mount_udb_device(self) -> str:
        path_to_udb_device = self.run_cmd_and_find_string('lsblk -o LABEL,PATH', "DAPLINK")[1]
        if path_to_udb_device is None:
            raise UDBMassStorageError("Can't find unmounted UDB mass storage devices")
        logger.warning(f"\tRunning \"sudo mount\" to mount UDB mass storage device " \
                       f"{path_to_udb_device} to {self.temp_folder_name}")
        self.run_shell_command(f"mkdir {self.temp_folder_name}")

        self.run_shell_command(f"sudo mount {path_to_udb_device} {self.temp_folder_name} " \
                               f"--options nosuid,nodev,flush,showexec,uid={os.getuid()}," \
                               f"gid={os.getgid()}", 25)

        return path_to_udb_device

    def copy_firmware(self, path_to_binary: str) -> None:
        self.run_shell_command(f"cp {path_to_binary} {self.path_to_udb_drive}", 5)
