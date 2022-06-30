#!/usr/bin/env python
from typing import ClassVar, Generator, Optional
from datetime import timedelta
from unittest import TestCase
import logging

logger = logging.getLogger("test.udb_integration_test")

def indent_string(string: str) -> str:
    return '\t' + '\t'.join(string.splitlines(True))

class ContextTest(TestCase):
    __context: Generator

    # A unit test where setUp/tearDown are combined into a single generator to make sure
    # closing happens even if an exception is raised during the test and to minimize the amount of
    # code needed
    def context(self) -> Generator:
        # Put both setUp and tearDown code in this generator method with a single `yield`
        # between
        yield

    def setUp(self) -> None:
        self.__context = self.context()
        next(self.__context)

    def tearDown(self) -> None:
        for _ in self.__context:
            raise RuntimeError("Context method should only yield once")

class UDBTestResources:
    serial_port_path: ClassVar[Optional[str]] = None
    path_to_binary_with_diff_version: ClassVar[Optional[str]] = None
    path_to_current_binary: ClassVar[Optional[str]] = None
    boot_timedelta: ClassVar[timedelta] = timedelta(seconds=20)
    run_all_tests: ClassVar[bool] = False

    @classmethod
    def set_serial_port_path(cls,
                             serial_port_path: str) -> None:
        cls.serial_port_path = serial_port_path

    @classmethod
    def get_serial_port_path(cls) -> Optional[str]:
        if cls.serial_port_path is None:
            raise Exception("Error: serial port path needs to be set before trying to access it")
        return cls.serial_port_path

    @classmethod
    def set_binary_paths(cls, path_to_current_binary: str,
                         path_to_binary_with_diff_version: str) -> None:
        cls.path_to_current_binary = path_to_current_binary
        cls.path_to_binary_with_diff_version = path_to_binary_with_diff_version

    @classmethod
    def get_path_to_binary_with_diff_version(cls) -> Optional[str]:
        if cls.path_to_binary_with_diff_version is None:
            raise Exception("Error: path_to_binary_with_diff_version needs to be set before trying to access it")
        return cls.path_to_binary_with_diff_version

    @classmethod
    def get_path_to_current_binary(cls) -> Optional[str]:
        if cls.path_to_current_binary is None:
            raise Exception("Error: path_to_current_binary needs to be set before trying to access it")
        return cls.path_to_current_binary

    @classmethod
    def set_expected_boot_timedelta(cls, timeout: timedelta) -> None:
        cls.boot_timedelta = timeout

    @classmethod
    def get_expected_boot_timedelta(cls) -> timedelta:
        return cls.boot_timedelta

    @classmethod
    def set_should_run_all_tests(cls, run_all_tests: bool) -> None:
        cls.run_all_tests = run_all_tests

    @classmethod
    def should_run_all_tests(cls) -> bool:
        return cls.run_all_tests
