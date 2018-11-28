# Automated Tests

DAPLink has an extensive automated test suite. It can be run on the current development environment (Windows and Linux) or a release package (Windows, Mac or Linux). Test results are written to the current directory and printed to the console.

Tests are launched by running ``python test/run_test.py`` in the Python virtual environment. The test logic discovers connected DAPLink boards and targets only those boards. Likewise, it detects what firmware you have built and targets only that firmware. Specify --help to view testing options (``run_test.py --help``).

The tests rely on information in ``test/info.py``. If you are porting DAPLink to a new board, you will need to add to info.py. See [Adding a new board](PORT_BOARD.md) for details.

## Preparing bootloader for Tests
* Build the bootloader or interface project to be tested.
* [Enable automation mode](ENABLE_AUTOMATION.md) on the board if it has not been enabled already.

## Test on Windows
### Prepare Host Machine
Windows drive indexing needs to be turned off prior to running the tests.
 
* Disable indexing on removable drives 
 Run gpedit.msc (<Windows>+<R> to open run window) then go to:
 Computer Configuration \ Administrative Templates \ Windows Components \ Search \ Do not allow locations on removable drives to be added to  libraries. Enable the setting there.
 
### Running Tests
 * Open CMD and cd to DAPLink directory.
 * Activate virtual env, launch tests by running ``python test/run_test.py``
   Specifying --firmwaredir is optional. For target images, either provide mbed.org authentication or use --targetdir to specify the location of pre-built target app binaries.
 * Test results will be printed to console

## Test on Linux
###  Prepare Host Machine
 Linux may have a daemon called modemmanager that sends unsolicitated traffic to DAPLink. This will cause serial port tests to fail, typically at band rate initialization.
 To check if modemmanager service is running, run ``systemctl | grep Modem``. Modem Manager will be displayed if so. To disable this service, run ``systemctl disable ModemManager.service``.
 Alternatively, this daemon can be removed with ``sudo apt-get purge modemmanager``.

**Step 1.** Build Firmware Releases
 The firmware images under test need to be built on Windows. Under tools directory, run ``build_release_uvision.bat``. Then copy the generated  uvision_release directory to Linux.
 
**Step 2.** Install Tools
 Install [Python 2, 2.7.9 or above](https://www.python.org/downloads/).
 Install [Git](https://git-scm.com/downloads).
 
**Step 3.** Get the sources and create a virtual environment.

```
$ git clone https://github.com/mbedmicro/DAPLink
$ cd DAPLink
$ pip install virtualenv
$ virtualenv venv
```
**Step 4.** Update tools

```
$ source venv/bin/activate
$ sudo pip install -r requirements.txt
```

### Running Tests
```
$ sudo python test/run_test.py --force --firmwaredir <path_to_uvision_release>
```

--firmwaredir is required. For target images, either provide mbed.org authentication or use --targetdir to specify the location of pre-built target app binaries.
Test results will be printed to console.

## Test on Mac
### Prepare Host Machine
  Refer to the steps for Linux. 
  
### Running Tests
  Refer to the steps for Linux.

## Two Areas of Testing
Tests are grouped into two categories

* Tests that validate how DAPLink is used for programming the target device and even the HIC itself via MSD--by copying an image file to the MSD drive (most users will drag-and-drop). These tests do not run by default but can be turned on by specifying --testdl.


* Tests that validate the MSD, CDC, and CMSIS-DAP endpoints. These tests run by default, but can be turned off by specifying --notestendpt

The endpoint tests rely on a target application--a small UART program that implements a simple protocol. The target app is used by all three endpoint sub-tests.

For mbed boards that are registered on mbed.org, the test builds the target application code on the fly in the cloud using the mbed RESTful Compile API. It then downloads the resulting image and downloads it to the target. The UART application source is in the mbed mercurial repository [daplink-validation](https://developer.mbed.org/users/c1728p9/code/daplink-validation/). For this workflow, you must specify your mbed.org user id and password via --user and --password.

For boards that are not mbed enabled or not registered on mbed.org, the test accepts pre-built applications via the --targetdir option. You will need to build the UART application, porting it to whatever platform is suitable if the board is not mbed enabled.

Currently, the tests only work in one fashion in any given run--using the Compile API or using pre-built binaries. That is, you cannot test (in one run) multiple boards if some have their target app built with Compile API and others have pre-built binaries.

## CMSIS-DAP Tests
The CMSIS-DAP tests (referred to as "HID" tests in the python code) require pyOCD. Fortunately, pyOCD is listed in ``requirements.txt``, and thus it is downloaded and made available to the tests automatically when you set up your DAPLink python virtual environment. This is fine if you're doing regression testing, but won't be of much help if you're trying to test a new DAPLink port. The publicly released pyOCD is unlikely to support your new board. You will need to combine your DAPLink porting efforts with a pyOCD porting effort if you want to fully validate your DAPLink firmware with the automated tests.

Assuming you have a pyOCD workspace on your local machine that supports your board, you'll need to tell the DAPLink tests to use that pyOCD instead of the one it downloaded from the Internet. The way to do that is to, while in the DAPLink virtual environment, cd to the root of your pyOCD workspace and run ``pip install --editable ./``, then cd back to the DAPLink workspace to run the tests.
