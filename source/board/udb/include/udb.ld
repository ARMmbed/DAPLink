#include "daplink_addr.h"

MEMORY
{
    m_udb_bl_version_rom           (RW)  : ORIGIN = DAPLINK_ROM_UDB_BL_VERSION_START, LENGTH = DAPLINK_ROM_UDB_BL_VERSION_SIZE
}

SECTIONS
{
    .udb_bl_version_rom :
    {
        KEEP(*(udb_bl_version_rom))
        FILL(0xffffffff)
        . = ORIGIN(m_udb_bl_version_rom) + LENGTH(m_udb_bl_version_rom) - 4;
        /* Need some contents in this section or it won't be copied to bin or hex. The CRC will
        * be placed here by post_build_script.py. */
        LONG(0x55555555)
    } > m_udb_bl_version_rom
}

#include "stm32h743xx.ld"
