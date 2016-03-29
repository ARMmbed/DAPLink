# DAPLink Developers Guide

## Setup
Install the necessary tools listed below. Skip any step where a compatible tool already exists. All tools **MUST** be added to the system path.

* Install [Python 2.7.9 or above](https://www.python.org/downloads/)
* Install [Git](https://git-scm.com/downloads)
* Install [Keil MDK-ARM](https://www.keil.com/download/product/)
* Install virtualenv in your global Python installation eg: `pip install virtualenv`

**Step 1.** Get the sources and create a virtual environment

```
$ git clone https://github.com/mbedmicro/DAPLink
$ cd DAPLink
$ pip install virtualenv
$ virtualenv venv
```

**Step 2.** Update tools and generate project files. **This should be done every time you pull new changes**

```Windows
$ "venv/Scripts/activate"
$ pip install -r requirements.txt
$ progen generate -t uvision
$ "venv/Scripts/deactivate"
```

**Step 3.** Pull requests should be made once a changeset is [rebased onto Master](https://www.atlassian.com/git/tutorials/merging-vs-rebasing/workflow-walkthrough)


## Port
There are three defined ways in which DAPLink can be extended.  These are adding target support, adding board support and adding HIC support.  Details on porting each of these can be found below.

* [Adding a new target](PORT_TARGET.md)
* [Adding a new board](PORT_BOARD.md)
* [Adding a new HIC](PORT_HIC.md)


## Test
DAPLink has a framework that allows automatic loading and testing of software on one or more boards. This can be run on the current development environment or a release package. Test results are written to the current directory and printed to the console. Tests are launched using the run_test.py script in the test directory. Specify --help to view testing options (``run_test.py --help``). To run basic tests with the current development environment, see the steps below.

* Build the project to be tested.
* [Enable automation mode](ENABLE_AUTOMATION.md) on the board if is has not been enabled already
* Run the command ``python test/run_test.py --user <username> --password <password>``, using your developer.mbed.org account name and password.
* Test results will be printed to console 


## Release
DAPLink contains scripts to automate most of the steps of building a release.  In addition to building the release, these scripts also save relevant build information such as git SHA and python tool versions so the same build can be reproduced.  The recommended steps for creating a release are below.

* Create a tag with the correct release version and push it to github
* Clean the repo you will be building from by running 'git clean -xdf' followed by 'git reset --hard'
* Run the script 'build_release_uvision.bat' to create all builds.
* All release deliverables will be created and stored in 'uvision_release'.  Save this wherever your builds are stored.

Note: A previous build can be reproduced by using the 'build_requirements.txt' of that build.
To do this add the additional argument 'build_requirements.txt' when calling 'build_release_uvision.bat' in step 2.
This will install and build with the exact version of the python packages used to create that build.

## MDK
If you want to use the MDK (uVision) IDE to work with the DAPLink code, you must launch it in the right environment. The project will fail to build otherwise. To launch uVision properly, use tools\launch_uv4.bat
