# Testing HID / CMSIS-DAP

DAPLink Debug feature is provided by a HID endpoint, that transports the CMSIS-DAP protocol, with no drivers installation involved. Host side of the Debug is provided by the [pyOCD][pyocd] library, which is installed during the [development environment setup](daplink_sdk_setup.py) process. Setup by default uses the Python Packages from the public release repository. For most users and developers this setup is good enough.

Unfortunately, the public release of pyOCD may not yet contain all necessary features to work with your new DAPLink HID code. Sometimes you also want to modify the pyOCD itself and try out the changes with DAPLink HID. In that case you need to use the development version of both pyOCD and DAPLink using the same Python Virtualenv:
* create one common Python Virtualenv for development version of pyOCD and DAPLink.
* clone the DAPLink git repository, make your changes.
* clone the pyOCD git repository, make your changes.
* install the development version of pyOCD package to the Python Virtualenv with `pip install --editable ./` from pyOCD repository location.
* now you can use DAPLink with new development pyOCD package that you have just installed.

[pyocd]: https://github.com/mbedmicro/pyOCD "Open-Source Python Library for Programming and Debugging ARM Cortex-M microcontrollers using CMSIS-DAP"
