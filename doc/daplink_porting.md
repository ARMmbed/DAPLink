# Porting New Hardware

Because DAPLink is an Open-Source project, you can port it to work on/with new hardware. You can include DAPLink to your own design or make it work with an ARM based device that is not yet supported. Following sections contain detailed information on porting:
* **Target** is an MCU that you want to flash and debug using DAPLink.
* **HIC** is an MCU connected to a Target that will run DAPLink firmware on it.
* **Board** is usually a Target based device with various peripherals included for operations (i.e. Development Kit that may have Debugger included) that you can create a custom DAPLink firmware for.
