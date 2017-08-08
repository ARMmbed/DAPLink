# Debugging

Debugging allows you to trace step by step program execution on your Target MCU, setting breakpoints, watching variables, memory modification, etc.

You usually debug a Target System using Remote GDB Protocol. Dedicated GDB Server Application is connected to a physical device over DAPLink USB HID interface at one side, and provides standard Remote GDB Protocol where you can connect with Host Computer over TCP/IP from your favorite Debugger.
This not only allows you to connect to Target remotely over a network connection, but also makes it possible to debug embedded microcontroller firmware as it was local running program.
In most cases you will simply start GDB Server on your local machine also running IDE/SDK and Debugger.. but remote debugging is a nice feature to have.

Here are most common applications that can both talk to DAPLink via USB-HID + CMSIS-DAP and provide remote GDB Server for you to connect the Debugger, or simply provide all-in-one solution for embedded debug with no details involved:
* [pyOCD][pyocd] - Open-Source Remote GDB Server for DAPLink written in Python.
* [uVision][uvision] - Commercial All-In-One solution for embedded systems development (including Debug).
* [IAR][iar] - Commercial All-In-One solution for embedded systems development (including Debug).

*Note: Please remember that you need an ELF file for Debugger in order to debug at Source Code level (not the assembly only as in case of BIN/HEX files). This is important when you do not build the whole project from the sources yourself but only got the firmware image to test.*

[pyocd]: https://github.com/mbedmicro/pyOCD "Open-Source Python Library for Programming and Debugging ARM Cortex-M microcontrollers using CMSIS-DAP"
[uvision]: http://www2.keil.com/mdk5/uvision/ "Keil uVision IDE"
[iar]: https://www.iar.com/iar-embedded-workbench/ "IAR Embedded Workbench IDE"