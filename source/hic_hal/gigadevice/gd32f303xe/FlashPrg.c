/* -----------------------------------------------------------------------------
 * Copyright (c) 2015 ARM Ltd.
 *
 * This software is provided 'as-is', without any express or implied warranty. 
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software. Permission is granted to anyone to use this 
 * software for any purpose, including commercial applications, and to alter 
 * it and redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not 
 *    claim that you wrote the original software. If you use this software in
 *    a product, an acknowledgment in the product documentation would be 
 *    appreciated but is not required. 
 * 
 * 2. Altered source versions must be plainly marked as such, and must not be 
 *    misrepresented as being the original software. 
 * 
 * 3. This notice may not be removed or altered from any source distribution.
 *   
 *
 * $Date:        23. April 2015
 * $Revision:    V1.00
 *  
 * Project:      Flash Programming Functions for GigaDevice GD32F30x Flash
 * --------------------------------------------------------------------------- */

/* History:
 *  Version 1.00
 *    Initial release
 */ 

#include "flash_hal.h"
#include "gd32f30x.h"

uint32_t Init(uint32_t adr, uint32_t clk, uint32_t fnc)
{
    fmc_wscnt_set(WS_WSCNT_0);

    fmc_unlock();

    return(0);
}

uint32_t UnInit(uint32_t fnc) 
{
    fmc_lock();

    return(0);
}

uint32_t EraseChip(void)
{
    fmc_mass_erase();

    return(0);                                        // Done
}

uint32_t EraseSector(uint32_t adr)
{
    fmc_page_erase(adr);

    return(0);
}

uint32_t ProgramPage(uint32_t adr, uint32_t sz, uint32_t *buf) 
{
    sz = (sz + 3) & ~3;                             // Adjust size for Words

    while(sz){
        fmc_word_program(adr, *buf);

        adr += 4;
        buf += 1;
        sz  -= 4;
    }

    return(0);                                      // Done
}
