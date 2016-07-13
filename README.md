# DAPLink
DAPLink is firmware that runs on a microcontroller that is connected according to a Hardware Interface Circuit (HIC) guideline. This provides methods to program and debug an application running on ARM processors via JTAG or SWD. The interface firmware exposes a USB composite device to the host OS with CDC, HID and MSC endpoints. These endpoints are critical when developing software for microcontrollers. Description of endpoints:
* MSC - drag-n-drop programming flash memory
* CDC - virtual com port for log, trace and terminal emulation
* HID - CMSIS-DAP compliant debug channel

For more detailed usability information [see the users guide.](docs/USERS-GUIDE.md)

This project supersedes the [mbed CMSIS-DAP interface firmware project.](https://github.com/mbedmicro/CMSIS-DAP/)

## Compatibility
There are many ARM microcontroller-based Hardware Interface Circuits (HICs) that DAPLink interface firmware runs on. These can be found as standalone boards or as part of development kits. Known supported circuits are based on and IO compatible with:
* Segger J-Link OB (Atmel SAM3U) - coming soon
* Maxim Epsilon (MAX32550) - coming soon
* [NXP OpenSDA](http://www.nxp.com/products/software-and-tools/run-time-software/kinetis-software-and-tools/ides-for-kinetis-mcus/opensda-serial-and-debug-adapter:OPENSDA)
* [NXP Link based on LPC11U35 or LPC4322](https://www.lpcware.com/LPCXpressoBoards)

## Releases
There are many board builds (board = HIC + target combination) created from this repository. Quarterly releases will contain new features and bugfixes. Standalone bugfixes are released once reported, verified and fixed. Both quarterly and bugfix releases will result in the build number being incremented; however, standalone bugfix release builds may only be made for affected boards. Release notes and all release builds can be found under releases. **Products shipping with or compatible with this firmware should have instructions on how to upgrade and the most up to date release build on the product page.**

## Contribute
Look for an interesting feature or defect [under issues](https://github.com/mbedmicro/DAPLink/issues) or start a new thread to engage with the developers and maintainers. You must sign the [contributor agreement](https://developer.mbed.org/contributor_agreement/) before any contributions can be accepted.

## Develop
Information for setting up a development environment, running the tests or creating a release build [can be found in the developers guide.](docs/DEVELOPERS-GUIDE.md)
