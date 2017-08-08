# The DAPLink Handbook

<a href="https://developer.mbed.org" tagret="_blank"><img align="right" width="256" src="gfx/arm_mbed_lockup_horizontal.png" /></a>

Welcome to the
[ARM][arm]
[mbed][mbed]
[DAPLink][daplink] Handbook!

DAPLink is a versatile solution that allows rapid from scratch development of ARM-based embedded systems at no cost.
It is a bridge between target MCU and host SDK.
Using single USB connection, DAPLink provides Drag-n-Drop firmware programming (so called "flashing"), CMSIS-DAP based debugging, as well as easy interaction with target over Virtual-COM-Port based UART.
More features are planned and will show up gradually as project is constantly under heavy development, coordinated by founding [ARM Limited][arm], supported by numerous hardware vendors and the Open-Source community around the world.
DAPLink has superseded the "[mbed CMSIS-DAP][cmsisdap] interface firmware" project.
You are free to use and contribute. Enjoy!

If you find bugs or think some information is missing here do not hesitate to let us know.
Official DAPLink Project Website and GitHub Repository is located at [https://github.com/mbedmicro/DAPLink][daplink].

## Table of Contents
* [Intro](daplink_intro.md)
* [Overview](daplink_overview.md)
* [Architecture](daplink_architecture.md)
* [Releases](daplink_releases.md)
* [Upgrading](daplink_upgrade_firmware.md)
* [Compatibility](daplink_compatibility.md)
* DAPLink Endpoints
    * [UMS / USB Mass Storage](daplink_endpoint_ums.md)
        * [Flashing](daplink_endpoint_ums_flashing.md)
        * [Commands](daplink_endpoint_ums_commands.md)
        * [Automation](daplink_endpoint_ums_automation.md)
    * [USB CDC](daplink_endpoint_cdc.md)
        * [Virtual COM Port](daplink_endpoint_cdc_virtualcomport.md)
    * [USB HID](daplink_endpoint_hid.md)
        * [Debug](daplink_endpoint_hid_debug.md)
* Developing DAPLink
    * [Supported SDK](daplink_sdk_support.md)
    * [Development Environment Setup](daplink_sdk_setup.md)
    * [Building a Release](daplink_build_release.md)
    * [Building Firmware](daplink_build_firmware.md)
    * [Building Fimrware+Bootloader Bundle](daplink_build_firmware_bundle.md)
* [Porting DAPLink](daplink_porting.md)
    * [Adding new Target](daplink_porting_target.md)
    * [Adding new Board](daplink_porting_board.md)
* [Testing DAPLink](daplink_testing.md)
    * [Testing new Board](daplink_testing_newboard.md)
    * [Testing UMS and CDC endpoints](daplink_testing_ums_cdc.md)
    * [Testing HID/CMSIS-DAP endpoint](daplink_testing_hid.md)
* [Dictionary of Abbreviations](daplink_dictionary.md)
* [References](daplink_references.md)



[arm]: https://www.arm.com "ARM Limited website"
[mbed]: https://developer.mbed.org "arm mbed developer resources"
[daplink]: https://github.com/mbedmicro/DAPLink "DAPLink Project Website and GitHub Repository"
[daplinkreleases]: https://github.com/mbedmicro/DAPLink/releases "DAPLink Releses"
[daplinkissues]: https://github.com/mbedmicro/DAPLink/issues "DAPLink Issue Tracker"
[pyocd]: https://github.com/mbedmicro/pyOCD "Open-Source Python Library for Programming and Debugging ARM Cortex-M microcontrollers using CMSIS-DAP"
[cmsisdap]: https://github.com/mbedmicro/CMSIS-DAP/ "ARM mbed CMSIS-DAP project"
[mbedcagreement]: https://developer.mbed.org/contributor_agreement/ "ARM mbed Contributor Agreement"
[mbed_flashalgo]: https://github.com/mbedmicro/FlashAlgo "ARM mbed FlashAlgo Project"
[mbed_fdrm_k64f]: https://developer.mbed.org/platforms/FRDM-K64F/ "ARM mbed board FDRM-K64F"
[project_generator]: https://github.com/project-generator/project_generator/wiki/Getting_started "Project Generator"
[daplink_validation]: https://developer.mbed.org/users/c1728p9/code/daplink-validation/ "DAPLink Validation / Target Firmware Test Agent source code repository"
[mbed_winserial]: https://developer.mbed.org/handbook/Windows-serial-configuration "ARM mbed Windows Virtual COM Port configuration page"
[gnuarm]: https://developer.arm.com/open-source/gnu-toolchain/gnu-rm "GNU ARM Embedded Toolchain"
[eclipse]: https://docs.mbed.com/docs/mbed-os-handbook/en/latest/debugging/debugging_eclipse_pyocd/ "Debugging mbed OS 5 applications with Eclipse"
[uvision]: http://www2.keil.com/mdk5/uvision/ "Keil uVision IDE"
[iar]: https://www.iar.com/iar-embedded-workbench/ "IAR Embedded Workbench IDE"
[python]: https://www.python.org/downloads/ "Python is platform independent free and open programming language"
[git]: https://git-scm.com/downloads "GIT is a distributed version control system"
[git_merge_vs_rebase]: https://www.atlassian.com/git/tutorials/merging-vs-rebasing/workflow-walkthrough "Article on GIT Merge vs Rebase operations"
[github_fork]: https://help.github.com/articles/fork-a-repo/ "How to Fork a Project on GitHub"
[github_pullrequest]: https://help.github.com/articles/about-pull-requests/ "About Pull Request on GitHub"
