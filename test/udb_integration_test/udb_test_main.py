#!/usr/bin/env python
from datetime import timedelta
from udb_test_helper import UDBTestResources
from argparse import ArgumentParser
import unittest
import logging
import os

if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("--serial_port_path", dest="serial_port_path", required=True,
                        help="Path to the UDB debug console serial port")
    parser.add_argument("--test_bin_path", dest="test_bin", required=True,
                        help="Path to the binary you are testing")
    parser.add_argument("--dummy_bin_path", dest="dummy_bin", required=True,
                        help="Path to a dummy binary that has a different version from the " \
                             "binary under test")
    parser.add_argument("-d", "--debug",
                        action="store_true", dest="debug", default=False,
                        help="don't print logs")
    parser.add_argument("-a", "--run-all",
                        action="store_true", dest="run_all", default=False,
                        help="With this flag, the script won't skip less convinient tests such " \
                             "as the assert test, which requires the debugger connected to UDB " \
                             "to be detached.")
    args = parser.parse_args()

    logging.basicConfig(format='%(message)s')
    logger = logging.getLogger("test.udb_integration_test")
    if args.debug:
        logger.setLevel(logging.DEBUG)
    else:
        logger.setLevel(logging.INFO)

    max_allowed_boot_time = timedelta(seconds=13)

    UDBTestResources.set_binary_paths(args.test_bin, args.dummy_bin)
    UDBTestResources.set_should_run_all_tests(args.run_all)
    UDBTestResources.set_expected_boot_timedelta(max_allowed_boot_time)
    UDBTestResources.set_serial_port_path(args.serial_port_path)

    suite = unittest.TestLoader().discover(os.path.dirname(os.path.realpath(__file__))+"/tests",
                                                           pattern = "*")
    unittest.TextTestRunner(verbosity=2, failfast=True).run(suite)
