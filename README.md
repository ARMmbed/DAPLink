# DAPLink

## Setup
Skip any step where a compatible tool already exists

1. Install [Python 2.7.x](https://www.python.org/downloads/)
2. Install [pip](https://pip.pypa.io/en/latest/installing.html)
3. Install DAPLink dependencies (see command below)
4. Install [Keil MDK-ARM](https://www.keil.com/download/product/)

```
daplink> pip install -r requirements.txt
```

__note:__ If include paths fail to generate properly in project file you may need to clone the [dev_v0.6](https://github.com/project-generator/project_generator/tree/dev_v0.6) branch and install

__note:__ The project is currently set for a generic ARM CM0p You must search the device database for kl26z128 to load the flash algorithm

```
project_generator>python setup.py install
```

## Develop
1. Generate project files. This should be done everytime you pull new changes
```
> pgen export -t uvision
```
For adding new targets start from template and use these docs...

## Contribute
Check out the issue tracker.

##ToDo
- Create a test
- Document how to use
- Document how to contribute
