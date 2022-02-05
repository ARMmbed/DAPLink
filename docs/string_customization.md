# Customizing DAPLink strings

There are a number of strings reported by DAPLink through USB descriptors, CMSIS-DAP, and USB MSD. These
can be configured by a build or set in board and target info.

Preprocessor macros should be set in the yaml records for either the board, found under `./records/board`,
or the HIC, found under `./records/hic_hal`.


## USB descriptor strings

**USB product string**: \
Macro: `USB_PROD_STR` \
The value of this macro _must_ include the string "CMSIS-DAP". Otherwise debuggers will not recognize
the USB device. An example is the string used for micro:bit, "BBC micro:bit CMSIS-DAP".


## MSD strings

**MSD volume name**: \
Struct member: `board_info::daplink_drive_name`

**URL file name**: \
Struct member: `board_info::daplink_url_name`

**URL file contents**: \
Struct member: `board_info::daplink_target_url`


## CMSIS-DAP string

**CMSIS-DAP product name**: \
Macro: `CMSIS_DAP_VENDOR_NAME` \
Not usually set.

**CMSIS-DAP product name**: \
Macro: `CMSIS_DAP_PRODUCT_NAME` \
Defaults to "DAPLink".

**CMSIS-DAP v2.1 target device vendor**: \
Struct member: `target_family_descriptor::target_vendor`

**CMSIS-DAP v2.1 target device name**: \
Struct member: `target_family_descriptor::target_part_number`

**CMSIS-DAP v2.1 target board vendor**: \
Struct member: `board_info::board_vendor`

**CMSIS-DAP v2.1 target board name**: \
Struct member: `board_info::board_name`

