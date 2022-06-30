#include "daplink_addr.h"

MEMORY
{
    m_cfgrom_bl           (RW)  : ORIGIN = DAPLINK_ROM_CONFIG_ADMIN_START, LENGTH = DAPLINK_ROM_CONFIG_ADMIN_SIZE
}

SECTIONS
{
    .cfgrom_bl :
    {
        KEEP(*(cfgrom_bl))
        FILL(0xffffffff)
        . = ORIGIN(m_cfgrom_bl) + LENGTH(m_cfgrom_bl) - 4;
        /* Need some contents in this section or it won't be copied to bin or hex. The CRC will
        * be placed here by post_build_script.py. */
        LONG(0x55555555)
    } > m_cfgrom_bl
}

#include "daplink.ld"
