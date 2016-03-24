# MSD Commands

DAPLink allows you to give it some simple commands via its MSD interface. By 
copying a file with a certain name to the root of the DAPLink MSD drive, you 
can get DAPLink to either perform an action or configure a flash-persisted 
option. The contents of the file are ignored; you can pass it an empty file.

These MSD commands work only if you are either holding down the RESET button 
when you add the file to the MSD drive, or if you've turned on DAPLink's 
automation-allowed mode. Of course, the way you turn on automation-allowed 
mode is via an MSD command (see below).  So, you'll either hold the RESET 
button each time you invoke an MSD command or you do so only once to turn 
on automation-allowed mode. This mode is saved in flash and so it persists 
across board reboots.

By convention, files that end with ".act" trigger an action; files that end 
with ".cfg" trigger an option change. The mechanism is case-insensitive.

Every command completes by remounting the DAPLink MSD drive. Every command 
is supported in both interface and bootload mode.

`start_bl.act` This file will force DAPLink to remount in bootloader mode. 
It is equivalent to unplugging the USB cable, and plugging it back in while 
holding the RESET button. This command has no effect if DAPLink is already 
in interface mode.

`start_if.act` This file will force DAPLink to remount in interface mode. It 
is equivalent to unplugging the USB cable and plugging it back in, when 
DAPLink is currently in bootloader mode. This command has no effect if DAPLink 
is already in bootloader mode.

`auto_rst.cfg` This file will turn on Auto Reset mode. In this mode, the RESET 
pin is automatically asserted after programming an application image to the 
target processor flash via the DAPLink MSD drive (in interface mode). Note 
that this mode does not affect when happens when you update the Hardware 
Interface Circuit firmware (DAPLink) via the DAPLink MSD drive in bootloader 
mode.

`hard_rst.cfg` This file turns off Auto Reset mode.

`assert.act` This file can be used to test DAPLink's assert utility. When you 
copy this file to the DAPLink MSD drive, DAPLink generates a call to the 
util_assert() method. An assert call causes the DAPLink MSD drive to remount
with an additional file, ASSERT.TXT, appearing at the root of the drive. This file
details where the assertion failure occurred (source file, line number). An
assert call is a no-op if there is already an outstanding assert failure (there 
can only be one). The outstanding assertion failure can be cleared by deleting
ASSERT.TXT. The deletion causes a remount of the DAPLink MSD drive. In actuality, 
any file operation (addition, deletion, content change) could be used to trigger 
a DAPLink command. Deleting ASSERT.TXT is actually a DAPLink MSD command in its 
own right.


`refresh.act` This file forces a remount of the DAPLink MSD drive

`auto_on.cfg` This file turns on automation-allowed mode. In this mode, DAPLink 
MSD commands can be triggered without needing to hold down the RESET button 
while copying the command file to the DAPLink MSD drive. Also, bootloader updates 
are allowed only in automation-allowed mode.

`auto_off.cfg` This file turns off automation-allowed mode.