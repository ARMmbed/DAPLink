# DAPLink

## Setup
Skip any step where a compatible tool already exists

1. Install [Python 2.7.9 or above](https://www.python.org/downloads/) and make sure it's added to path
2. Install [Git](https://git-scm.com/downloads) and make sure it's added to path
3. Install [Keil MDK-ARM](https://www.keil.com/download/product/)
4. Install virtualenv in python

```
> git clone https://github.com/mbedmicro/DAPLink
> pip install virtualenv
> virtualenv venv
> 
```

## Develop
1. Update tools and generate project files. This should be done everytime you pull new changes
```
> "venv/Scripts/activate"
> pip install -r requirements.txt
> progen generate -t uvision
> "venv/Scripts/deactivate"
```

For adding new targets start from template and use these docs...

## Release
1. Create a tag with the correct release version and push it to github
2. Clean the repo you will be building from by running 'git clean -xdf' followed by 'git reset --hard'
3. Run the script 'build_release_uvision.bat' to create all builds.
4. All release deliverables will be created and stored in 'uvision_release'.  Save this wherever your builds are stored.

Note: A previous build can be reproduced by using the 'build_requirements.txt' of that build.
To do this add the additional argument 'build_requirements.txt' when calling 'build_release_uvision.bat' in step 2.
This will install and build with the exact version of the python packages used to create that build.

## Contribute
Check out the issue tracker.

##ToDo
- Create a test
- Document how to use
- Document how to contribute
