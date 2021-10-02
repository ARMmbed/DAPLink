# Warning: Development branch

This is is a development branch:
- It adds support for the GCC (`gcc_arm`) and ARMC6 (`armclang`) compilers, ARMC5 (`armcc`) will be deprecated.
- It focuses on `progen` support using `make` and `cmake`.

## Setup

DAPLink sources are compiled using `progen` (from [project-generator](https://github.com/project-generator/project_generator)) which can be run on Linux, MacOS and Windows.

Install the necessary tools listed below. Skip any step where a compatible tool already exists.

* Install [Python 3](https://www.python.org/downloads/) . Add to PATH.
* Install [Git](https://git-scm.com/downloads) . Add to PATH.
* Install a compiler:
    * [GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) . This compiler will be identified as `gcc_arm`.
    * [Arm Compiler 6](https://developer.arm.com/tools-and-software/embedded/arm-compiler) . This compiler will be identified as `armclang`. Only supported on Linux and Windows.
    * [Keil MDK](https://developer.arm.com/tools-and-software/embedded/keil-mdk) or [Arm Compiler 5](https://developer.arm.com/tools-and-software/embedded/arm-compiler/downloads/legacy-compilers#arm-compiler-5). This compiler will be identified as `armcc`. Only supported on Linux and Windows.
* Install `make` (tested with [GNU Make](https://www.gnu.org/software/make)). [CMake](https://cmake.org) can alternatively be used in conjunction with different implementations of `make` as well as [ninja](https://ninja-build.org).
* Install virtualenv in your global Python installation eg: `pip install virtualenv`.

**Step 1.** Initial setup.

Get the sources and create a virtual environment

```
$ git clone https://github.com/ARMmbed/DAPLink
$ cd DAPLink
$ git checkout develop
$ virtualenv venv
```

## Activate virtual environment
**Step 2.** Activate the virtual environment and update requirements. This is necessary when you open a new shell. **This should be done every time you pull new changes**

```
$ venv/Scripts/activate   (For Linux)
$ venv/Scripts/activate.bat   (For Windows)
(venv) $ pip install -r requirements.txt
```

## Build
**This should be done every time you pull new changes**

You can use the `progen` command-line tool from project-generator or the `tools/progen_compile.py` wrapper tool.

**Step 4.1.** Using `progen_compile.py`

```
(venv) $ python tools/progen_compile.py [-t <tool>] [--clean] [-v] [--parallel] [<project> [<project> ...]]
```

* `-t <tool>`: choose the toolchain to build. The default is `make_gcc_arm`. Other options tested are `make_gcc_arm`, `make_armclang`, `make_armcc`, `cmake_gcc_arm`, `cmake_armclang`, `cmake_armcc`.
* `--clean`: will clear existing compilation products and force recompilation of all files.
* `-v`: will make compilation process more verbose (typically listing all commands with their arguments)
* `--parallel`: enable parallel compilation within a project (projects are compiled sequentially).
* `<project>`: target project to compile (e.g. `stm32f103xb_bl`, `lpc11u35_if`), if none is specified all (140 to 150) projects will be compiled.

**Step 4.2.** Using `progen` with `make`

The following command combines generation and compilation:

```
(venv) $ progen generate -t make_gcc_arm -p <project> -b
```

Alternatively one can separate those task:
```
(venv) $ progen generate -t make_gcc_arm -p <project>
(venv) $ make -C projectfiles/make_gcc_arm/<project> [<target>] [VERBOSE=1]
```
Where:
* `<project>`: target project to compile (e.g. `stm32f103xb_bl`, `lpc11u35_if`).
* `<target>`: build target, can be `all`, `clean` or `help`.
* `VERBOSE=1`: display additional compilation information.

**Step 4.3.** Using `progen` with `cmake`

The following command combines generation and compilation:

```
(venv) $ progen generate -t cmake_gcc_arm -o generator=<generator> -p <project> -b
```
* `<generator>`: use `CMake` generators among the following options:
    * `make` (`Unix Makefiles`)
    * `mingw-make` (`MinGW Makefiles`)
    * `msys-make` (`MSYS Makefiles`, untested)
    * `ninja` (`Ninja`)
    * `nmake` (`NMake Makefiles`, untested)
* `<project>`: target project to compile (e.g. `stm32f103xb_bl`, `lpc11u35_if`).

# End of warning

[![DAPLink](/docs/images/daplink-website-logo-link.png)](https://armmbed.github.io/DAPLink/)

**Note:** The master branch requires an Arm Compiler 5 or Keil MDK license to build. See the
[develop](https://github.com/ARMmbed/DAPLink/tree/develop) branch for gcc and Arm
Compiler 6 support. As soon as testing is complete, gcc will become the default compiler and AC5 support will be
dropped.

----

Arm Mbed DAPLink is an open-source software project that enables programming and debugging application software running on Arm Cortex CPUs. Commonly referred to as interface firmware, DAPLink runs on a secondary MCU that is attached to the SWD or JTAG port of the application MCU. This configuration is found on nearly all development boards. Enumerating as a USB composite device, it creates a bridge between your development computer and the CPU debug access port. DAPLink enables developers with:

* MSC - drag-n-drop programming flash memory
* CDC - virtual com port for log, trace and terminal emulation
* CMSIS-DAPv2 WinUSB (driver-less vendor-specific bulk) - CMSIS compliant debug channel
* CMSIS-DAPv1 HID - CMSIS compliant debug channel
* WebUSB CMSIS-DAP HID - CMSIS compliant debug channel

More features are planned and will show up gradually over time. The project is constantly under heavy development by Arm, its partners, numerous hardware vendors and the open-source community around the world. DAPLink has superseded the mbed CMSIS-DAP interface firmware project. You are free to use and contribute. Enjoy!

For more detailed usability information [see the users guide.](docs/USERS-GUIDE.md)

## Compatibility
There are many ARM microcontroller-based Hardware Interface Circuits (HICs) that DAPLink interface firmware runs on. These can be found as standalone boards (debugger) or as part of a development kit. Some branded circuits that are known to be IO compatible are:
* [NXP OpenSDA based on K20, K22 and KL26](http://www.nxp.com/products/software-and-tools/run-time-software/kinetis-software-and-tools/ides-for-kinetis-mcus/opensda-serial-and-debug-adapter:OPENSDA)
* [NXP LPC-Link2 based on LPC11U35 or LPC4322](https://www.nxp.com/support/developer-resources/hardware-development-tools/lpcxpresso-boards:LPCXPRESSO-BOARDS)
* [Segger J-Link OB based on Atmel SAM3U](https://www.segger.com/products/debug-probes/j-link/models/j-link-ob/)

## Releases
There are many board builds (board = HIC + target combination) created from this repository. Quarterly releases will contain new features and bugfixes. Standalone bugfixes are released once reported, verified and fixed. Both quarterly and bugfix releases will result in the build number being incremented. Many development kits and products ship with DAPLink interface firmware or are capable of running DAPLink firmware. **[The current release builds and instructions for updating DAPLink interface firmware is hosted on the DAPLink release site.](https://armmbed.github.io/DAPLink/)** Release notes and previous release builds can be found under GitHub releases.

## Contribute

We welcome contributions to DAPLink in any area. Look for an interesting feature or defect [under
issues](https://github.com/ARMmbed/DAPLink/issues), or start a new thread to engage with the
developers and maintainers.

Please see the [contribution guidelines](CONTRIBUTING.md) for detailed requirements for
contributions.

To report bugs, please [create an issue](https://github.com/ARMmbed/DAPLink/issues/new) in the
GitHub project.

## Develop
Information for setting up a development environment, running the tests or creating a release build [can be found in the developers guide.](docs/DEVELOPERS-GUIDE.md)

## License
DAPLink is licensed with the permissive Apache 2.0 license. See the [LICENSE](LICENSE) file for the
full text of the license.

Copyright © 2006-2021 Arm Ltd
