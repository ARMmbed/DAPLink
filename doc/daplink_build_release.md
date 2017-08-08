# Building a Release

DAPLink contains scripts to automate most of the steps of building a release.  In addition to building the release, these scripts also save relevant build information such as git SHA and python tool versions so the same build can be reproduced.  The recommended steps for creating a release are presented below:

* Create a `tag` with the correct release version and push it to github.
* Clean the repo you will be building from by running `git clean -xdf` followed by `git reset --hard`.
* Run the script `build_release_uvision.bat` to create all builds.
* All release deliverables will be created and stored in `uvision_release` directory. Save this wherever your builds are stored.

**Note:** An existing build can be reproduced by creating and adjusting the `build_requirements.txt` configuration file.
Then pass `build_requirements.txt` as an argument to the `build_release_uvision.bat` script (as described above).
This will manage all dependencies and python packages used to create that particular build.
