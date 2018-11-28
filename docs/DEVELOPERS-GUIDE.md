# DAPLink Developers Guide

## Setup
DAPLink sources can be compiled using Keil MDK-ARM or mbed cli tool with arm compiler, which could be run both on Linux and Windows. See  [here](AUTOMATED_TESTS.md) for test instructions on both OS and Mac.

Install the necessary tools listed below. Skip any step where a compatible tool already exists.

* Install [Python 2, 2.7.11 or above](https://www.python.org/downloads/) . Add to PATH.
* Install [Git](https://git-scm.com/downloads) . Add to PATH.
* Install [Keil MDK-ARM](https://www.keil.com/download/product/), preferably version 5. Set environment variable "UV4" to the absolute path of the UV4 executable if you don't install to the default location. Note that "UV4" is what's used for both MDK versions 4 and 5. This step can be skipped if you plan to use mbed cli.
* Install virtualenv in your global Python installation eg: `pip install virtualenv`.


**Step 1.** Get the sources and create a virtual environment

```
$ git clone https://github.com/mbedmicro/DAPLink
$ cd DAPLink
$ pip install virtualenv
$ virtualenv venv
```

**Step 2.1** For uvision progen compilation, update tools and generate project files. **This should be done every time you pull new changes**

```
$ venv/Scripts/activate
$ pip install -r requirements.txt
$ progen generate -t uvision
$ venv/Scripts/deactivate
```

**Step 2.2** For mbed cli project compilation
```
$ venv/Scripts/activate
$ pip install -r requirements.txt
$ mbed deploy
$ mbed config root .
$ mbed config ARM_PATH FULL_PATH_TO_ARMCC_FOLDER 
$ tools/mbedcli_compile.py project1 project2 project3 --clean
$ venv/Scripts/deactivate
```
Valid project names are listed on help.


**Step 3.** Pull requests should be made once a changeset is [rebased onto Master](https://www.atlassian.com/git/tutorials/merging-vs-rebasing/workflow-walkthrough)

## Mbedcli compile environment

### Features
- Support both Python 2.x and 3.x versions.
- Can compile a list of projects or the all of the projects. 
- Can generate the release directory with one command.

### Prerequisite
mbedcli should be included in the python package or requirements.txt. Currently requirement3.txt contains the list of python packages needed to be able to compile in the mbedcli environment.

### mbedcli_compile.py script
Arguments
```
positional arguments:
  projects              Selectively compile only the firmware specified
                        otherwise all projects

optional arguments:
  -h, --help            show this help message and exit
  --release             Create a release with the yaml version file
  --build-folder BUILD_FOLDER
                        Release directory to grab files from
  --release-folder RELEASE_FOLDER
                        Directory to create and place files in
  --toolchain TOOLCHAIN
                        Toolchain directory if present
  --clean               Rebuild or delete build folder before compile
  -v                    Pass verbosity level to mbed compile  -vv for more
```
Valid projects are listed on help.

Generate files needed by mbedcli
* `custom_profile.json` lists toolchain profile or compile flags parsed from the yaml files
* `custom_targets.json` contains platform information for specific hics.
* `.mbedignore` filters all files not needed for the project.

## Port
There are three defined ways in which DAPLink can be extended. These are adding target support, adding board support and adding HIC support. Details on porting each of these can be found below.

* [Adding a new target](PORT_TARGET.md)
* [Adding a new board](PORT_BOARD.md)
* [Adding a new HIC](PORT_HIC.md)


## Test
DAPLink has an extensive set of automated tests written in Python. They are used for regression testing, but you can use them to validate your DAPLink port. Details are [here](AUTOMATED_TESTS.md)

An option to search for the daplink firmware build in uvision and mbedcli build folders.
`python test/run_test.py --projecttool mbedcli ...` or `python test/run_test.py --projecttool uvision ...`.

## Release

###Release using uvision

DAPLink contains scripts to automate most of the steps of building a release. In addition to building the release, these scripts also save relevant build information such as git SHA and python tool versions so the same build can be reproduced. The recommended steps for creating a release are below.

* Create a tag with the correct release version and push it to github
* Clean the repo you will be building from by running 'git clean -xdf' followed by 'git reset --hard'
* Run the script ``build_release_uvision.bat`` to create all builds.
* All release deliverables will be created and stored in 'uvision_release'. Save this wherever your builds are stored.

Note: A previous build can be reproduced by using the ``build_requirements.txt`` of that build.
To do this add the additional argument ``build_requirements.txt`` when calling ``build_release_uvision.bat`` in step 2.
This will install and build with the exact version of the python packages used to create that build.

###Release using mbedcli

If the project list is not specify, all interface and booloader projects will be compiled. If `--release_version` is given, a folder (`firmware` on default or specified by `--release_folder`, to be concatenated with the version number), will be generated with the bin, update.yml and zip file containing the bins for release
```
$ venv/Scripts/activate
$ pip install -r requirements3.txt
$ tools/mbedcli_compile.py --release_version 0250 --release_folder firmware
```

There is an intermediate step in uvision environment in creating a release directory. This step is not needed in mbedcli environment but to make this equivalent directory invoke
`copy_release_files.py --project-tool mbedcli`
To make a release directory from the step above.
`package_release_files.py SRC_DIR DEST_DIR VERSION_NUMBER --toolchain ARM`

## MDK
If you want to use the MDK (uVision) IDE to work with the DAPLink code, you must launch it in the right environment. The project will fail to build otherwise. To launch uVision properly, use ``tools/launch_uvision.bat``

This script can take arguments to override default virtual environment and python packages to be installed. For example `tools\launch_uvision.bat other_env other_requirements.txt`
