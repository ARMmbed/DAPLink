# Automated Tests

DAPLink has an extensive automated test suite. It can be run on the current development environment or a release package. Test results are written to the current directory and printed to the console.

Tests are launched by running ``python test/run_test.py`` in the Python virtual environment. The test logic discovers connected DAPLink boards and targets only those boards. Likewise, it detects what firmware you have built and targets only that firmware.

The tests rely on information in ``test/info.py``. If you are porting DAPLink to a new board, you will need to add to info.py. See [Adding a new board](PORT_BOARD.md) for details.

### Two Areas of Testing
Tests are grouped into two categories

* Tests that validate how DAPLink is used for programming the target device and even the HIC itself via MSD--by copying an image file to the MSD drive (most users will drag-and-drop). These tests do not run by default but can be turned on by specifying --testdl.


* Tests that validate the MSD, CDC, and CMSIS-DAP endpoints. These tests run by default, but can be turned off by specifying --notestendpt

The endpoint tests rely on a target application--a small UART program that implements a simple protocol. The target app is used by all three endpoint sub-tests.

For mbed boards that are registered on mbed.org, the test builds the target application code on the fly in the cloud using the mbed RESTful Compile API. It then downloads the resulting image and downloads it to the target. The UART application source is in the mbed mercurial repository [daplink-validation](https://developer.mbed.org/users/c1728p9/code/daplink-validation/). For this workflow, you must specify your mbed.org user id and password via --user and --password.

For boards that are not mbed enabled or not registered on mbed.org, the test accepts pre-built applications via the --targetdir option. You will need to build the UART application, porting it to whatever platform is suitable if the board is not mbed enabled.

Currently, the tests only work in one fashion in any given run--using the Compile API or using pre-built binaries. That is, you cannot test (in one run) multiple boards if some have their target app built with Compile API and others have pre-built binaries.

### CMSIS-DAP Tests
The CMSIS-DAP tests (referred to as "HID" tests in the python code) require pyOCD. Fortunately, pyOCD is listed in ``requirements.txt``, and thus it is downloaded and made available to the tests automatically when you set up your DAPLink python virtual environment. This is fine if you're doing regression testing, but won't be of much help if you're trying to test a new DAPLink port. The publicly released pyOCD is unlikely to support your new board. You will need to combine your DAPLink porting efforts with a pyOCD porting effort if you want to fully validate your DAPLink firmware with the automated tests.

Assuming you have a pyOCD workspace on your local machine that supports your board, you'll need to tell the DAPLink tests to use that pyOCD instead of the one it downloaded from the Internet. The way to do that is to, while in the DAPLink virtual environment, cd to the root of your pyOCD workspace and run ``pip install --editable ./``, then cd back to the DAPLink workspace to run the tests.

### Running the Tests
Specify --help to view testing options (``run_test.py --help``). To run basic tests with the current development environment, see the steps below.

* Build the bootloader or interface project to be tested.
* [Enable automation mode](ENABLE_AUTOMATION.md) on the board if it has not been enabled already
* Run the command ``python test/run_test.py``, with options that either provide mbed.org authentication or that specify the location of pre-built target app binaries.
* Test results will be printed to console

