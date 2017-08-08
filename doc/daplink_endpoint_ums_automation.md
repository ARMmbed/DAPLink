# Automation Mode

`AUTOMATION MODE` allows you to leverage UMS Commands mechanism without manual interaction with the Board (pressing RESET button each time you want to execute a UMS Command), which comes really handy in remote work and testing.
In order to enable Automation Mode:
* Update the interface software to a version at or above `0241`.
* Copy an empty text file names `AUTO_ON.CFG` to the DAPLink UMS drive while the reset button is pressed.
* Release the reset button when the drive unmounts. When it re-mounts, confirm `Automation Allowed` is set to `1` in the `DETAILS.TXT` file.
* Update the bootloader software to a version at or above `0241`.
* Confirm that creating `START_BL.ACT` file causes the device to enter `BOOTLOADER` mode, and that creating `START_IF.ACT` file causes the device to enter `INTERFACE` mode.

**Note:** This `AUTOMATION MODE` is mandatory for Bootloader updates.