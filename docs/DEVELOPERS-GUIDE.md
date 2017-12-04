# DAPLink Developers Guide

## Setup
Install the necessary tools listed below. Skip any step where a compatible tool already exists.

* Install [Python 2, 2.7.9 or above](https://www.python.org/downloads/) . Add to PATH.
* Install [Git](https://git-scm.com/downloads) . Add to PATH.
* Install [Keil MDK-ARM](https://www.keil.com/download/product/), preferably version 5. Set environment variable "UV4" to the absolute path of the UV4 executable if you don't install to the default location. Note that "UV4" is what's used for both MDK versions 4 and 5.
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
$ venv/Scripts/activate
$ pip install -r requirements.txt
$ progen generate -t uvision
$ venv/Scripts/deactivate
```

**Step 3.** Pull requests should be made once a changeset is [rebased onto Master](https://www.atlassian.com/git/tutorials/merging-vs-rebasing/workflow-walkthrough)


## Port
There are three defined ways in which DAPLink can be extended. These are adding target support, adding board support and adding HIC support. Details on porting each of these can be found below.

* [Adding a new target](PORT_TARGET.md)
* [Adding a new board](PORT_BOARD.md)
* [Adding a new HIC](PORT_HIC.md)


## Test
DAPLink has an extensive set of automated tests written in Python. They are used for regression testing, but you can use them to validate your DAPLink port. Details are [here](AUTOMATED_TESTS.md)


## Release
DAPLink contains scripts to automate most of the steps of building a release. In addition to building the release, these scripts also save relevant build information such as git SHA and python tool versions so the same build can be reproduced. The recommended steps for creating a release are below.

* Create a tag with the correct release version and push it to github
* Clean the repo you will be building from by running 'git clean -xdf' followed by 'git reset --hard'
* Run the script ``build_release_uvision.bat`` to create all builds.
* All release deliverables will be created and stored in 'uvision_release'. Save this wherever your builds are stored.

Note: A previous build can be reproduced by using the ``build_requirements.txt`` of that build.
To do this add the additional argument ``build_requirements.txt`` when calling ``build_release_uvision.bat`` in step 2.
This will install and build with the exact version of the python packages used to create that build.

## MDK
If you want to use the MDK (uVision) IDE to work with the DAPLink code, you must launch it in the right environment. The project will fail to build otherwise. To launch uVision properly, use ``tools/launch_uv4.bat``
