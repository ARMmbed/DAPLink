# Releases

This Open-Source repository provides a source code for different variations of DAPLink implementation, along with boards configuration, documentation, build scripts, and testing framework. For most users only the final result of our work is important, therefore precompiled-binary-ready-for-use images are provided in a form of Release Package

Firmware image files are named in a manner that allows unique identification of the RELEASE, HIC, BOARD, TARGET, so you will easily know what firmware image matches your hardware. 

You can download Release Packages navigating to [RELEASES][daplinkreleases] link located at the top of the project website. Release Package will allso contain Release Notes with useful and important information on new features, bugfixes, problems and solutions, therefore it is usually good idea to read them before firmware upgrade your DAPLink.

Each Release has its own unique incremental build number as well as build ID. Release usually contains new fetures or bug fixes. Sometimes, unfortunately, a bug can be identified after release has been made. In that case a standalone bugfix can be applied for affected boards. Standalone bugfixes are released after careful consideration, once reported, verified and fixed. Both quarterly and bugfix releases will result in the build number being incremented.

**Please note that products that are shipped or advertied to be compatible with DAPLink should provide detailed and board specific instructions on how to upgrade firmware using Release Packages from this website.**

[daplinkreleases]: https://github.com/mbedmicro/DAPLink/releases "DAPLink Releses"
