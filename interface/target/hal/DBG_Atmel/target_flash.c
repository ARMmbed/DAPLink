/* CMSIS-DAP Interface Firmware
 * Copyright (c) 2009-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include "sam3u.h"
#include "RTL.h"
#include "debug_cm.h"
#include "target_reset.h"
#include "swd_host.h"
#include "gpio.h"
#include "target_flash.h"
#include "target_config.h"
#include "flash_blob.h"
#include "DAP_config.h"
#include "intelhex.h"

#include "string.h"

// maybe a parameter in write but this is just for now.
extension_t file_extension;

static /*inline*/ uint32_t test_range(const uint32_t test, const uint32_t min, const uint32_t max)
{
    return ((test < min) || (test > max)) ? 0 : 1;
}

uint8_t validate_bin_nvic(uint8_t *buf)
{
    // test for known required NVIC entries
    //  0 is stack pointer (RAM address)
    //  1 is Reset vector  (FLASH address)
    uint32_t nvic_sp = 0;
    uint32_t nvic_rv = 0;
    // test the initial SP value
    memcpy(&nvic_sp, buf, sizeof(nvic_sp));
    if (0 == test_range(nvic_sp, target_device.ram_start, target_device.ram_end)) {
        return 0;
    }
    // test the initial reset vector
    memcpy(&nvic_rv, buf+4, sizeof(nvic_rv));
    if (0 == test_range(nvic_rv, target_device.flash_start, target_device.flash_end)) {
        return 0;
    }
    
    return 1;
}

uint8_t validate_hexfile(uint8_t *buf)
{
    // look here for known hex records
    // add hex identifier b[0] == ':' && b[8] == {'0', '2', '3', '4', '5'}
    if (buf[0] == ':') {
        if ((buf[8] == '0') ||
            (buf[8] == '2') ||
            (buf[8] == '3') ||
            (buf[8] == '4') ||
            (buf[8] == '5'))
        {
            return 1;
        }
    }
    return 0;
}

target_flash_status_t target_flash_init(extension_t ext)
{
    if (!target_set_state(RESET_PROGRAM)) {
        return TARGET_FAIL_RESET;
    }
    
    // Download flash programming algorithm to target and initialise.
    if (!swd_write_memory(flash.algo_start, (uint8_t *)flash.image, flash.algo_size)) {
        return TARGET_FAIL_ALGO_DL;
    }

    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.init, target_device.flash_start, 0 /* clk value is not used */, 0, 0)) {
        return TARGET_FAIL_INIT;
    }
    
    file_extension = ext;
    if (ext == HEX) {
        intelHexStartData();
    }
    
    return TARGET_OK;
}


target_flash_status_t target_flash_erase_sector(unsigned int sector) {
    if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_sector, sector*target_device.sector_size, 0, 0, 0)) {
        return TARGET_FAIL_ERASE_SECTOR;
    }

    return TARGET_OK;
}

target_flash_status_t target_flash_erase_chip(void) {
    //
	// 1 == O.K.
	// 0 == Error
	//
   if (!swd_flash_syscall_exec(&flash.sys_call_param, flash.erase_chip, 0, 0, 0, 0)) {   // 1 == O.K., 0 == Error
        return TARGET_FAIL_ERASE_ALL;
    }
    
    target_set_state(RESET_PROGRAM);
    //target_flash_init();    
    
    return TARGET_OK;
}

int programHEXPage(void);

target_flash_status_t target_flash_program_page(uint32_t addr, uint8_t * buf, uint32_t size)
{
    uint32_t bytes_written = 0;
    target_flash_status_t status = TARGET_OK;
    addr += target_device.flash_start;
    
    // we need to erase a sector
    if (addr % target_device.sector_size == 0) {
        status = target_flash_erase_sector(addr / target_device.sector_size);
        if (status != TARGET_OK) {
            return status;
        }
    }

    // Program a page in target RAM to be programmed.
    if (!swd_write_memory(flash.program_buffer, buf, size)) {
        return TARGET_FAIL_ALGO_DATA_SEQ;
    }

    while(bytes_written < size) {
        if (!swd_flash_syscall_exec(&flash.sys_call_param,
                                    flash.program_page,
                                    addr,
                                    flash.ram_to_flash_bytes_to_be_written,
                                    flash.program_buffer + bytes_written, 0)) {
            return TARGET_FAIL_WRITE;
        }

        bytes_written += flash.ram_to_flash_bytes_to_be_written;
        addr += flash.ram_to_flash_bytes_to_be_written;
    }

    return TARGET_OK;
}

static int programHEXPage()
{
    uint32_t bytesRead;             //Amount of data used from the data available in usb_buffer (in bytes)
    uint32_t bytesToWrite;          //Amount of data (in bytes) extracted from the HEX records (in the usb_buffer) that needs to be written to the flash
    uint32_t sizeOfUSB_Buffer;      //Number of words in the usb_buffer from the current USB read
    uint32_t sizeOfOldData;         //Amount of usb_buffer data from the previous USB read that was not processed
    uint32_t amountCopiedToFlashBuf;//tracks the data written to flash buffer for the upcoming write
    uint16_t loadOffset;            //local variable for passing flashPtr to and from intelHexReadData
    uint32_t i;                     //counter variable
    uint32_t hex_data_size;         //keep track of the amount of data in usb_buffer
    uint32_t hex_data_used;         //amount of data used from the data available in usb_buffer
    uint32_t no_words_to_used;
    BOOL forcePageWrite;            //Flag for forcing a page write before there is enough data to fill a page
    sizeOfOldData=0;
    sizeOfUSB_Buffer=0;
    bytesRead = 0;
    bytesToWrite = 0;
    amountCopiedToFlashBuf=0;
    

    if(!initCalled){
        initCalled = __TRUE;
        intelHexStartData();
        for(i=0;i<FLASH_PROGRAM_PAGE_SIZE;i++)
        {
            //Erased cell in nRF51 is FF
            flashBuffer[i]=0xFF;
        }
    }
    //count the number of words to write
    for(i=0;(i<(FLASH_PROGRAM_PAGE_SIZE/4)) && usb_buffer[i]>0 ;i++)
    {            
        sizeOfUSB_Buffer++;
    }
    
    //count unwritten words from the previous pass
    for(i=0;(i<FLASH_SECTOR_SIZE/4) && unrecordedUSBData[i]>0;i++)
    {
        sizeOfOldData++;
    }
    
    //insert old data into the usb_buffer, if any
    if(sizeOfOldData>0)
    {
        //shift usb_data
        for(i=(sizeOfOldData + sizeOfUSB_Buffer -1);i>=sizeOfOldData ;i--)
        {
            usb_buffer[i] = usb_buffer[i-sizeOfOldData];
        }
        
        //copy unused data from the old usb_data
        for(i=0;i<sizeOfOldData;i++)
        {
            usb_buffer[i] = unrecordedUSBData[i];
        }                           
    }
    //update data size
    hex_data_size =(sizeOfOldData + sizeOfUSB_Buffer)*4;
    loadOffset = (uint16_t)flashPtr;
    
    //Process hex data
    if(intelHexReadData((uint8_t *)usb_buffer, hex_data_size, &bytesRead, intermediateBuffer, &flash_addr_offset, &bytesToWrite, &loadOffset, &forcePageWrite,usedIntermediateBufSize>0))
    {   
        //if buffered data is already as big as half the buffer, then force write otherwise risk overflow.
        if(sizeOfOldData>(FLASH_SECTOR_SIZE/8)){
            forcePageWrite = __TRUE;
        }
        flashPtr = (uint32_t)loadOffset;
        //Copy processed data into flashBuffer
        for(amountCopiedToFlashBuf=0;amountCopiedToFlashBuf<(bytesToWrite) &&  amountCopiedToFlashBuf<(FLASH_PROGRAM_PAGE_SIZE-usedIntermediateBufSize) ;amountCopiedToFlashBuf++)
        {
            flashBuffer[amountCopiedToFlashBuf+usedIntermediateBufSize] = intermediateBuffer[amountCopiedToFlashBuf];
        }        
        
        if(((usedIntermediateBufSize+amountCopiedToFlashBuf)>=FLASH_PROGRAM_PAGE_SIZE) || forcePageWrite )
        {//we have enough data or a write is forced, either way write the flashBuffer to device            
            
            if (!target_flash_program_page(flashPtr + flash_addr_offset, flashBuffer, FLASH_PROGRAM_PAGE_SIZE)) 
            {//fail
                return 1;
            } 
            
            if((usedIntermediateBufSize+amountCopiedToFlashBuf)>=FLASH_PROGRAM_PAGE_SIZE)
            {
                flashPtr += FLASH_PROGRAM_PAGE_SIZE;   
            }
            else
            {
                flashPtr +=(usedIntermediateBufSize+amountCopiedToFlashBuf);
            }
            
           
           
            //flush out flashBuffer
            for(i=0;i<FLASH_PROGRAM_PAGE_SIZE;i++)
            {
                //Erased cell in nRF51 is FF
                flashBuffer[i]=0xFF;
            }

            //copy the data omitted from the flash write to the top of flashBuffer
            if((usedIntermediateBufSize+bytesToWrite) > FLASH_PROGRAM_PAGE_SIZE)
            {
                for(i=0;i<(bytesToWrite-amountCopiedToFlashBuf);i++)
                {
                    flashBuffer[i] = intermediateBuffer[i+amountCopiedToFlashBuf];
                }
                usedIntermediateBufSize=i;
                if(forcePageWrite)
                {
                    //don't wait for the next usb_buffer write...
                    if (!target_flash_program_page(flashPtr + flash_addr_offset, flashBuffer, FLASH_PROGRAM_PAGE_SIZE)) 
                    {//fail
                        return 1;
                    } 
                    flashPtr +=(bytesToWrite-amountCopiedToFlashBuf);
                    
                    //flush out flashBuffer
                    for(i=0;i<(bytesToWrite-amountCopiedToFlashBuf);i++)
                    {
                        //Erased cell in nRF51 is FF
                        flashBuffer[i]=0xFF;
                    }
                    usedIntermediateBufSize=0;
                }
            }
            else
            {
                usedIntermediateBufSize=0;
            }
        }
        else
        {
            usedIntermediateBufSize+=amountCopiedToFlashBuf;
        }
        
        //Final page is received -- make sure everything is written to flash
        if (current_sector == nb_sector) 
        {
            //make sure to clear the top element from the history before finishing the transfer
            unrecordedUSBData[0] = 0; 
            hex_data_used=bytesRead;
            
            //check if there is still unwritten data -- do the last writes
            while(hex_data_size>hex_data_used){                    
                bytesRead =0;
                bytesToWrite=0;
                no_words_to_used = (hex_data_used-1)/4;
                if(intelHexReadData((uint8_t *)(&usb_buffer[no_words_to_used]), hex_data_size-(no_words_to_used*4), &bytesRead, intermediateBuffer, &flash_addr_offset, &bytesToWrite, &loadOffset, &forcePageWrite,usedIntermediateBufSize>0))
                {
                    flashPtr = (uint32_t)loadOffset;                
                    for(amountCopiedToFlashBuf=0;amountCopiedToFlashBuf<(bytesToWrite) &&  amountCopiedToFlashBuf<(FLASH_PROGRAM_PAGE_SIZE-usedIntermediateBufSize) ;amountCopiedToFlashBuf++)
                    {
                        flashBuffer[amountCopiedToFlashBuf+usedIntermediateBufSize] = intermediateBuffer[amountCopiedToFlashBuf];
                    }
                    //write the data processed
                    if (!target_flash_program_page(flashPtr + flash_addr_offset, flashBuffer, FLASH_PROGRAM_PAGE_SIZE)) 
                    {
                        return 1;
                    } 
                    
                    //flush out flashBuffer
                    for(amountCopiedToFlashBuf=0;amountCopiedToFlashBuf<(bytesToWrite) &&  amountCopiedToFlashBuf<(FLASH_PROGRAM_PAGE_SIZE-usedIntermediateBufSize) ;amountCopiedToFlashBuf++)
                    {
                        //Erased cell in nRF51 is FF
                        flashBuffer[amountCopiedToFlashBuf]=0xFF;
                    }                
                    hex_data_used += bytesRead;
                }
                else{
                    reason = CORRUPT_FILE;
                    initDisconnect(0);
                    return 1;
                }
            }
            //flush arrays and reset variables here?
            initDisconnect(1);
            return 0;
        }
        
        no_words_to_used = bytesRead/4;
        hex_data_size = hex_data_size/4;
        for(i=0;i < (hex_data_size-no_words_to_used);i++)
        {
            unrecordedUSBData[i] = usb_buffer[no_words_to_used+i];
        }
        if(i>FLASH_SECTOR_SIZE/4){
            reason = OUT_OF_MEM;
            initDisconnect(0);
        }
        
        //clear unused buffer values
        for(;i<FLASH_SECTOR_SIZE/4;i++){
            unrecordedUSBData[i] =0;
        }
        
        for(i=FLASH_SECTOR_SIZE/8;i <FLASH_SECTOR_SIZE/2;i++){
            usb_buffer[i] =0;
        }
    }
    else
    {
        reason = CORRUPT_FILE;
        initDisconnect(0);
        return 1;
        
    }
    
    return 0;  
}

