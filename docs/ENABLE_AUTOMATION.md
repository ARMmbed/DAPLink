# Enable Automation Features
1. Update the interface software to a version at or above 0241
1. Copy an empty text file called ``auto_on.cfg`` to the DAPLink MSD drive while the reset button is held.
1. Release the reset button when the drive unmounts. When it remounts, confirm "Automation Allowed" is set to 1 in ``details.txt``
1. Update the bootloader software to a version at or above 0241
1. Confirm that the ``start_bl.act`` MSD command causes the device to enter bootloader mode, and that the ``start_if.act`` command causes the device to enter interface mode. See [MSD Commands](MSD_COMMANDS.md)