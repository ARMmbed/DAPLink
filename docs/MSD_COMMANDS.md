# MSD Commands

DAPLink allows you to give it some simple commands via its MSD interface. By 
copying a file with a certain name to the root of the DAPLink MSD drive, you 
can get DAPLink to either perform an action or configure a flash-persisted 
option. The contents of the file are ignored; you can pass it an empty file.

MSD commands only work if you are either holding down the RESET button 
when you add the file to the MSD drive, or if you've turned on DAPLink's 
automation-allowed mode. Of course, the way you turn on automation-allowed 
mode is via an MSD command (see below). So, you'll either hold the RESET 
button each time you invoke an MSD command or you'll do so only once to turn 
on automation-allowed mode. The automation-allowed mode is saved in flash and 
so it persists across board reboots.

By convention, files that end with ``.act`` trigger an action; files that end 
with ``.cfg`` configure a behavior. The mechanism is case-insensitive.

Every command completes by remounting the DAPLink MSD drive. Every command 
is supported in both interface and bootloader mode.

### Action Commands

``start_bl.act`` This file will force DAPLink to remount in bootloader mode. 
It is equivalent to unplugging the USB cable, and plugging it back in while 
holding the RESET button. This command has no effect if DAPLink is already 
in bootloader mode.

``start_if.act`` This file will force DAPLink to remount in interface mode. It 
is equivalent to unplugging the USB cable and plugging it back in. This command 
has no effect if DAPLink is already in interface mode.

``assert.act`` This file can be used to test DAPLink's assert utility. When you 
copy this file to the DAPLink MSD drive, DAPLink generates a call to the 
util_assert() method. An assert call causes the DAPLink MSD drive to remount
with an additional file, ``ASSERT.TXT``, appearing at the root of the drive. This file
details where the assertion failure occurred (source file, line number). An
assert call is a no-op if there is already an outstanding assert failure (there 
can only be one). The outstanding assertion failure can be cleared by deleting
``ASSERT.TXT``. The deletion causes a remount of the DAPLink MSD drive. In actuality, 
any file operation (addition, deletion, content change) could be used to trigger 
a DAPLink command. Deleting ``ASSERT.TXT`` is actually a DAPLink MSD command in its 
own right.

``refresh.act`` This file forces a remount of the DAPLink MSD drive.

``erase.act`` This file triggers an erase of the target's flash.

``msd_off.act`` Mass storage device can be disabled by this file command regardless of presence of target flash algo for drag and drop. MSD will always be disabled without the target flash algo.

``msd_on.act`` Mass storage device can be enabled back by this file command on bootloader mode only if there is a target flash algo.

``page_off.act`` This file temporary enables page programming and chip erasing until the next restart occurred for drag and drop.

``page_on.act`` This file temporary enables page programming and sector erasing until the next restart occurred for drag and drop.

### Configuration Commands

``auto_rst.cfg`` This file will turn on Auto Reset mode. In this mode, 
a reset is performed at the end of the programming sequence. From the 
user's perspective, the program starts running as soon as programming 
completes. The default behavior is for Auto Reset to be off. This means that
after programming, the application is left halted and does not run. Note that 
this mode does not affect what happens when you update the Hardware 
Interface Circuit firmware itself (DAPLink) via the DAPLink MSD drive in 
bootloader mode.

``hard_rst.cfg`` This file turns off Auto Reset mode. Auto Reset mode is off by 
default.


``auto_on.cfg`` This file turns on automation-allowed mode. In this mode, DAPLink 
MSD commands can be triggered without needing to hold down the RESET button. Also, 
bootloader updates are allowed only in automation-allowed mode.

``auto_off.cfg`` This file turns off automation-allowed mode. This mode
is off by default


``ovfl_on.cfg`` This file turns on serial overflow reporting. If the host PC is not reading
data fast enough from DAPLink and an overflow occurs the text ```<DAPLink:Overflow>```
will show up in the serial data. Serial overflow reporting is turned off by default.

``ovfl_off.cfg`` This file turns off serial overflow reporting.


``comp_on.cfg`` This file turns on the incompatible target image detection. The interface project must define a board specific `board_detect_incompatible_image()` function with the criteria to validate the target image over the first 12 vectors. Otherwise, the incompatible target image detection won't have an effect.

``comp_off.cfg`` This file turns off the incompatible target image detection (off by default).
