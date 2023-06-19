#include "daplink_addr.h"

MEMORY
{
    m_sector_buf       (RW)  : ORIGIN = DAPLINK_RAM_SECTOR_BUFFER_START, LENGTH = DAPLINK_RAM_SECTOR_BUFFER_SIZE
}

SECTIONS
{
    .sector_buf_section (NOLOAD) :
    {
        KEEP(*(sector_buf_section))
    } > m_sector_buf
}

#include "daplink.ld"