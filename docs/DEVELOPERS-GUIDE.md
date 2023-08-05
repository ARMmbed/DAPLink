# DAPLink Developers Guide

## Setup

DAPLink sources are compiled using `progen` (from [project-generator](https://github.com/project-generator/project_generator)) which can be run on Linux, MacOS and Windows.

Install the necessary tools listed below. Skip any step where a compatible tool already exists.

* Install [Python 3](https://www.python.org/downloads/) . Add to PATH.
* Install [Git](https://git-scm.com/downloads) . Add to PATH.
* Install a compiler:
    * [GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) . This compiler will be identified as `gcc_arm`. It is recommended to use version `10.3-2021.10`. Versions 11.3 and later of GCC (Arm GNU Toolchain / `arm-none-eabi-gcc`) are known to require some changes to build (see [#1043](https://github.com/ARMmbed/DAPLink/pull/1043)).
    * [Arm Compiler 6](https://developer.arm.com/tools-and-software/embedded/arm-compiler) . This compiler will be identified as `armclang`. Only supported on Linux and Windows.
    * [Keil MDK](https://developer.arm.com/tools-and-software/embedded/keil-mdk) or [Arm Compiler 5](https://developer.arm.com/tools-and-software/embedded/arm-compiler/downloads/legacy-compilers#arm-compiler-5). This compiler will be identified as `armcc`. Only supported on Linux and Windows.
* Install `make` (tested with [GNU Make](https://www.gnu.org/software/make)). [CMake](https://cmake.org) can alternatively be used in conjunction with different implementations of `make` as well as [ninja](https://ninja-build.org).
* Install virtualenv in your global Python installation eg: `pip install virtualenv`.

**Step 1.** Initial setup.

Get the sources and create a virtual environment

```
$ git clone https://github.com/mbedmicro/DAPLink
$ cd DAPLink
$ pip install virtualenv
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

There are two ways to build DAPLink. You can use the `progen` command-line tool from project-generator or the `tools/progen_compile.py` wrapper tool.

**Step 3.1.** Using `progen_compile.py`

```
(venv) $ python tools/progen_compile.py [-t <tool>] [--clean] [-v] [--parallel] [<project> [<project> ...]]
```

* `-t <tool>`: choose the toolchain to build. The default is `make_gcc_arm`. Other options tested are `make_gcc_arm`, `make_armclang`, `make_armcc`, `cmake_gcc_arm`, `cmake_armclang`, `cmake_armcc`.
* `--clean`: will clear existing compilation products and force recompilation of all files.
* `-v`: will make compilation process more verbose (typically listing all commands with their arguments)
* `--parallel`: enable parallel compilation within a project (projects are compiled sequentially).
* `<project>`: target project to compile (e.g. `stm32f103xb_bl`, `lpc11u35_if`), if none is specified all (140 to 150) projects will be compiled.

**Step 3.2.** Using `progen` with `make`

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

**Step 3.3.** Using `progen` with `cmake`

The following command combines generation and compilation:

```
(venv) $ progen generate -t cmake_gcc_arm -o generator=<generator> -p <project> -b
```
* `<generator>`: use `CMake` generators among the following options:
    * `make` (`Unix Makefiles`)
    * `mingw-make` (`MinGW Makefiles`)
    * `msys-make` (`MSYS Makefiles`, untested)
    * `ninja` (`Ninja`)
    * `nmake` (`NMake Makefiles`)
* `<project>`: target project to compile (e.g. `stm32f103xb_bl`, `lpc11u35_if`).

**Step 3.4.** Using `progen` for MDK compilation.

This command generates MDK project files under the `projectfiles/uvision` directory.
```
$ progen generate -t uvision
```

To only generate one specific project, use a command like this:
```
progen generate -f projects.yaml -p stm32f103xb_stm32f746zg_if -t uvision
```
These options to `progen` set the parameters:
- `-f` for the input projects file
- `-p` for the project name
- `-t` to specify the IDE name

## Contribute
We would love to have your changes! Pull requests should be made once a changeset is [rebased onto main](https://www.atlassian.com/git/tutorials/merging-vs-rebasing/workflow-walkthrough). See the [contributing guide](../CONTRIBUTING.md) for detailed requirements and guidelines for contributions.

## Port
There are three defined ways in which DAPLink can be extended. These are adding target support, adding board support and adding HIC support. Details on porting each of these can be found below.

* [Adding a new target](PORT_TARGET_FAMILY.md)
* [Adding a new board](PORT_BOARD.md)
* [Adding a new HIC](PORT_HIC.md)

## Test
DAPLink has an extensive set of automated tests written in Python. They are used for regression testing, but you can use them to validate your DAPLink port. Details are [here](AUTOMATED_TESTS.md)

An option to search for the daplink firmware build in uvision and mbedcli build folders.
`python test/run_test.py --project-tool make_gcc_arm ...` or `python test/run_test.py --project-tool uvision ...`.

## Release

### Release using `progen_compile.py`

* Create a tag with the correct release version and push it to github
* Clean the repo you will be building from by running 'git clean -xdf' followed by 'git reset --hard'
* Run the `progen_compile.py` command with the following parameters (see above for the `-t` flag):
```
(venv) $ python tools/progen_compile.py [-t <tool>] --clean -v --parallel --release
```
* All release deliverables will be created and stored in `firmware_<version>` (where `<version>` is the DAPLink version). Save this wherever your builds are stored.

### Release using uvision

DAPLink contains scripts to automate most of the steps of building a release. In addition to building the release, these scripts also save relevant build information such as git SHA and python tool versions so the same build can be reproduced. The recommended steps for creating a release are below.

* Create a tag with the correct release version and push it to github
* Clean the repo you will be building from by running 'git clean -xdf' followed by 'git reset --hard'
* Run the script ``build_release_uvision.bat`` to create all builds.
* All release deliverables will be created and stored in 'uvision_release'. Save this wherever your builds are stored.

Note: A previous build can be reproduced by using the ``build_requirements.txt`` of that build.
To do this add the additional argument ``build_requirements.txt`` when calling ``build_release_uvision.bat`` in step 2.
This will install and build with the exact version of the python packages used to create that build.

## MDK
If you want to use the MDK (uVision) IDE to work with the DAPLink code, you must launch it in the right environment. The project will fail to build otherwise. To launch uVision properly, use ``tools/launch_uvision.bat``

This script can take arguments to override default virtual environment and python packages to be installed. For example `tools\launch_uvision.bat other_env other_requirements.txt`
