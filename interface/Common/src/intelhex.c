#include "intelhex.h"
#include "stdio.h"
#include "string.h"

// logic - Every buf 512 bytes
//  - Build and return structure
//  - partial structure needs to be persistent (2 of struct)
//  - line size doesnt matter
//  - always write decoded data to RAM

typedef enum hex_record_t hex_record_t;
enum hex_record_t {
    DATA_RECORD = 0,
    EOF_RECORD = 1,
    EXT_SEG_ADDR_RECORD = 2,
    START_SEG_ADDR_RECORD = 3,
    EXT_LINEAR_ADDR_RECORD = 4,
    START_LINEAR_ADDR_RECORD = 5
};
    

typedef union hex_line_t hex_line_t;
union __attribute__((packed)) hex_line_t {
    uint8_t buf[0x25];
    struct __attribute__((packed)) {
        uint8_t  byte_count;
        uint16_t address;
        uint8_t  record_type;
        uint8_t  data[0x20];
        uint8_t  checksum;
    };
};

/** Converts a character representation of a hex to real value.
 *   @param c is the hex value in char format
 *   @return the value of the hex
 */
static uint8_t atoh(char c)
{
    return (c & 0x10) ? /*0-9*/ c & 0xf : /*A-F, a-f*/ (c & 0xf) + 9;
}

/** Calculate checksum on a hex record
 *   @param data is the line of hex record
 *   @param size is the length of the data array
 *   @return 1 if the data provided is a valid hex record otherwise 0
 */
static uint8_t validate_checksum(hex_line_t *record)
{
    uint8_t i = 0, result = 0;
    for ( ; i < (record->byte_count+4); i++) {
        result += record->buf[i];
    }
    return (record->checksum == (uint8_t)(~result)+1);
}

void reset_hex_parser(void)
{

}
// still need to test need to complete a few states (using codepad.org)
uint32_t parse_hex_blob(uint8_t *buf, uint32_t *address, uint32_t *size)
{
    static hex_line_t line;
    static uint8_t low_nibble = 0, idx = 0, record_processed = 0;
    static uint32_t base_address = 0;
    static uint8_t unload_unaligned_record = 0;
    // were gonna store decoded data in ourself... scary
    uint8_t *input_buf = buf;
    uint8_t *end = buf + *size;
    uint32_t input_buf_size = *size;
    // reset the amount of data that is being return'd
    *size = 0;
    
    // we had an exit state when the data was unaligned. Need to pop this before decoding anthing else
    if (unload_unaligned_record) {
        unload_unaligned_record = 0;
    }
    
    while (buf != end) {
        switch (*buf) {
            // junk we dont care about
            case '\r':
            case '\n':
                // we've hit the end of an ascii line
                if (validate_checksum(&line) && !record_processed) {
                    record_processed = 1;
                    switch (line.record_type) {
                        case DATA_RECORD:
                            // verify this is a continous block of memory or need to exit and dump
                            if ((base_address + line.address) > (*address + line.byte_count)) {
                                *address = *address - *size; // figure the start address before returning
                                // need to unload this decoded data somewhere too
                                unload_unaligned_record = 1;
                                return HEX_PARSE_UNALIGNED;
                            }
                        
                            // move from line buffer back to input buffer
                            memcpy((uint8_t *)input_buf, (uint8_t *)line.data, line.byte_count);
                            input_buf += line.byte_count;
                            size += line.byte_count;
                            // this stores the last know address of decoded data
                            *address = base_address + line.address + line.byte_count;
                            break;
                        
                        case EOF_RECORD:
                            // fill in all FF here and force a return (or break from this logic)
                            memset(input_buf, 0xff, (input_buf_size - *size));
                            *address = *address - *size; // figure the start address before returning
                            *size = input_buf_size;
                            return HEX_PARSE_EOF;
                        
                        case EXT_LINEAR_ADDR_RECORD:
                            // update the address msb's
                            base_address = (line.data[0] << 24) | (line.data[1] << 16);
                            break;
                        
                        default:
                            break;
                    }
                }
                break;
        
            // found start of a new record. reset state variables
            case ':':
                memset(line.buf, 0, sizeof(hex_line_t));
                low_nibble = 0;
                idx = 0;
                record_processed = 0;
                break;
            
            // decoding lines
            default:
                if (low_nibble) {
                    line.buf[idx] |= atoh(*buf) & 0xf;
                    idx++;
                }
                else {
                    if (idx < sizeof(hex_line_t)) {
                        line.buf[idx] = atoh(*buf) << 4;
                    }
                }
                low_nibble = !low_nibble;
                break;
        }
        buf++;
    }
    
    memset(input_buf, 0xff, (input_buf_size - *size));
    *address = *address - *size; // figure the start address before returning
    return HEX_PARSE_OK;
}














///** 
//*   @brief  Extracting a hex record from the raw data provided
//*   @param  hexData is the hex file provided as a byte array
//*   @param  buffer is a hex record extracted from hexData (single line with ':', LF and CR removed)
//*   @param  totalBytesRead is a counter value for keeping track of our position in hexData
//*   @param  hexDataSize is the size of hexData array.
//*   @return size of the hex record extracted.
//*/
//uint32_t intelHexReadRecord(uint8_t *hexData, uint32_t hexDataSize, uint8_t *buffer, uint32_t *totalBytesRead)
//{
//    BOOL done;
//    BOOL lowNib;
//    BOOL startOfLine;
//    uint32_t byteCount;
//    uint8_t curChar;
//    uint8_t curHex;
//    uint32_t bytesRead = (*totalBytesRead);
//    
//    done=__FALSE;  
//    startOfLine = __TRUE;
//    lowNib = __FALSE;   
//    byteCount = 0;
//    
//    do
//    {        
//        if(bytesRead>=hexDataSize)
//        {
//            bytesRead = (*totalBytesRead);
//            done = __TRUE;
//            byteCount=0; 
//        }
//        curChar = hexData[bytesRead];
//        bytesRead++;
//        if(startOfLine)//it should be start of line,':'
//        {
//            switch(curChar)
//            {
//                case ':':
//                    startOfLine = __FALSE;
//                    byteCount=0; 
//                    lowNib = __FALSE;
//                    break;
//                case EOF:
//                    done = __TRUE;
//                    break;
//                default:
//                    //Keep looking
//                    break;
//            }      
//        }
//        else//it should be DATA
//        {
//            switch(curChar)
//            {
//                case '\r':
//                case '\n':
//                case EOF:
//                    // End of line or end of file
//                    done = __TRUE;
//                    break;
//                case ':':
//                    // Start of next record (normally see \r and/or \n before this)
//                    done = __TRUE;
//                    //don't count this character as data read
//                    bytesRead--;
//                    break;
//                default:
//                    curHex = asciiToHex(curChar);
//                    if (curHex==INVALID_HEX_CHAR)
//                    {
//                        //Illegal character, ignore the rest of this line
//                        startOfLine = __TRUE;
//                    }
//                    else
//                    {
//                        if (lowNib) // Low nibble
//                        {
//                            buffer[byteCount] |= curHex;
//                            byteCount++;
//                        }
//                        else        // High nibble 
//                        {
//                            buffer[byteCount] = curHex<<4;
//                        }
//                        lowNib = !lowNib;                            
//                    }
//                    break;
//            }      
//        }
//        
//        
//    }while(!done);
//    
//    (*totalBytesRead) = bytesRead;
//    return byteCount;
//    
//}

///**
//*   @brief  Extract the data fields from the hex format and arranges them in binary format.
//*   @param  hexData is the hex file provided as a byte array
//*   @param  hexDataBytesRead number of bytes read from the rawBuffer
//*   @param  buffer is the resulting byte array (ready to be flashed).
//*   @param  hexDataSize is the size for hexData.
//*   @param  flashAddress is the starting address for the buffer to be written: this is read from the hex file.
//*   @param  readSize is the used space in the resulting byte array (actual size to be written to flash).
//*   @param  initLoadOffset is the data offset for the first record of a new page, 
//*           i.e. it will only be updated if the hex record read is the start of a new page in the flash.
//*   @param  forcePageWrite is a boolean value to indicate if a page write is necessary even though there is 
//*           not enough data to fill a page
//*   @param  haveDataToWrite is a boolean value to indicate if the caller already has data to write and does not
//*           want a jump in the address space.
//*   @return TRUE if the parsing of hexData was succcessful, otherwise FALSE
//*/
//BOOL intelHexReadData(uint8_t *hexData, uint32_t hexDataSize, uint32_t *hexDataBytesRead, uint8_t *buffer, uint32_t *flashAddress, uint32_t *readSize, uint16_t *initLoadOffset, BOOL *forcePageWrite, BOOL haveDataToWrite)
//{
//    uint32_t bytesRead;      //Total size in bytes (2 hex characters from the file) of the hex record read from the raw buffer hexData.
//    uint32_t totalBytesRead; //Total size in bytes of the hex record read from the raw buffer hexData.
//    uint8_t recordLength;    //Size in bytes of the DATA or INFO field of the hex record.
//    uint8_t recordType;      //Record type.
//    uint16_t loadOffset;     //Load offset; where to write the data.
//    uint32_t bufferIdx;      //Current offset into "buffer" -- also the number of bytes read into buffer.
//    BOOL eof;                //TRUE if end of file record has been read from the file.
//    uint32_t lba;            //Linear base address
//    uint8_t *pData;          //Pointer to next byte of data to read in the DATA field of recordData.
//    uint8_t dataCount;       //Data in recordData array that has not been copied to the buffer
//    
//    loadOffset = *initLoadOffset;
//    
//    totalBytesRead = *hexDataBytesRead;
//    bufferIdx = 0;
//    eof = __FALSE;
//    *forcePageWrite = __FALSE;
//    dataCount = 0;
//    pData = NULL;
//     
//    do{

//        if (dataCount==0)
//        {
//            //If dataCount==0 then we need to read data from the hex file. 
//            bytesRead = intelHexReadRecord(hexData, hexDataSize, recordData, &totalBytesRead);
//            
//            //Check we have at least the minimum amount of data for a valid record. 
//            if (bytesRead < NON_DATA_RECORD_SIZE) 
//            {                
//                // Too short, return the data obtained from the preceding hex records
//               *readSize = bufferIdx;
//               *hexDataBytesRead = totalBytesRead;
//                return __TRUE;
//            }
//        
//            //Validate checksum 
//            if (!intelHexChecksum(recordData, bytesRead))
//            {
//                //Failed checksum
//                return __FALSE;
//            }
//        
//            //Read header information
//            recordLength = recordData[0];
//            loadOffset =  recordData[1] << 8;
//            loadOffset |= recordData[2];
//            recordType = recordData[3];

//            //Check record length and bytesRead add up correctly. 
//            if (bytesRead != (recordLength + NON_DATA_RECORD_SIZE)) 
//            {
//                //Invalid lengths
//                return __FALSE;
//            }
//    
//            //Process according to record type
//            switch (recordType)
//            {
//                case REC_DATA: //00
//                    if( prevLoadOffset!=loadOffset && ((prevLoadOffset + prevRecordLength) != loadOffset) )
//                    {
//                        //skipped flash address
//                        if(bufferIdx>0 || haveDataToWrite)
//                        {
//                        //break the data here; return
//                            *readSize = bufferIdx;
//                            *forcePageWrite = __TRUE;
//                            return __TRUE;
//                        }
//                        else
//                        {
//                            //update loadOffset
//                            *initLoadOffset = loadOffset;
//                        }
//                    }
//                    //Get a pointer to the data and a count of the number of bytes.
//                    pData = &recordData[4];
//                    dataCount = recordLength;
//                    prevLoadOffset = loadOffset;
//                    prevRecordLength = recordLength;
//                    *hexDataBytesRead = totalBytesRead;
//                    break;

//                case REC_EXT_LIN: //04 - The new base address (non-contiguous flash use), NRF: UICR Value address will be given here
//                    // Get the linear base address
//                    lba  = recordData[4] << 24;
//                    lba |= recordData[5] << 16;
//                
//                    if(bufferIdx> 0 || haveDataToWrite)
//                    {                        
//                       //address jump-- break the data here; return
//                       *readSize = bufferIdx;
//                       *forcePageWrite = __TRUE;
//                        //we don't update the hexDataBytesRead with totalBytesRead so that this line can be read once more
//                        return __TRUE;
//                    }
//                    else
//                    {
//                        *readSize = 0;
//                        *flashAddress = lba;
//                        *initLoadOffset = loadOffset;
//                        prevLoadOffset = loadOffset;
//                        prevRecordLength = recordLength;
//                        *hexDataBytesRead = totalBytesRead;
//                    }
//                    break;

//                case REC_SRT_LIN: //05
//                    //NRF application start address can be non-zero (e.g. 0x1C000000) when softdevice is used
//                    //For LPC17XX, uVision appears to use the address of main() instead of address zero. 
//#if (!defined(DBG_LPC1768) && !defined(DBG_NRF51822AA))
//                    //Others must start from address zero
//                    if ( (recordData[4]!=0x00) || (recordData[5]!=0x00) ||(recordData[6]!=0x00) ||(recordData[7]!=0x00))
//                    {
//                        // Invalid start address 
//                        return __FALSE;
//                    }
//#endif
//                    break;
// 
//                case REC_EXT_SEG: //02
//                case REC_SRT_SEG: //03
//                    // Not supported, should not occur in ARM hex files
//                    return __FALSE;                 
//                case REC_EOF: //01                    
//                    *forcePageWrite = eof = __TRUE;
//                    prevLoadOffset = loadOffset;
//                    prevRecordLength = recordLength;
//                    *hexDataBytesRead = totalBytesRead;
//                    break;
// 
//                default:
//                    //Unknown record type - ignore it.
//                    break;
//            }            
//        }
//       
//        if (dataCount>0)
//        {           
//            //We have some data to copy to the output buffer.
//            do {                    
//                *(buffer++) = *(pData++); //Copy data from the DATA field of the hex record to the output buffer.
//                bufferIdx++;
//                dataCount--;
//                //Repeat while there is unread in the DATA field and we have not filled the output buffer.
//            } while((dataCount>0));// && (bufferIdx<hexDataSize));
//        }

//    //Repeat until we have filled the output buffer or reached the end of the hex file.
//    } while (!eof);//(bufferIdx<hexDataSize) && 

//    //Return the number of bytes written to the ouput buffer.
//    *readSize = bufferIdx;

//    return __TRUE;
//}

//static int programHEXPage()
//{
//    uint32_t bytesRead;             //Amount of data used from the data available in usb_buffer (in bytes)
//    uint32_t bytesToWrite;          //Amount of data (in bytes) extracted from the HEX records (in the usb_buffer) that needs to be written to the flash
//    uint32_t sizeOfUSB_Buffer;      //Number of words in the usb_buffer from the current USB read
//    uint32_t sizeOfOldData;         //Amount of usb_buffer data from the previous USB read that was not processed
//    uint32_t amountCopiedToFlashBuf;//tracks the data written to flash buffer for the upcoming write
//    uint16_t loadOffset;            //local variable for passing flashPtr to and from intelHexReadData
//    uint32_t i;                     //counter variable
//    uint32_t hex_data_size;         //keep track of the amount of data in usb_buffer
//    uint32_t hex_data_used;         //amount of data used from the data available in usb_buffer
//    uint32_t no_words_to_used;
//    BOOL forcePageWrite;            //Flag for forcing a page write before there is enough data to fill a page
//    sizeOfOldData=0;
//    sizeOfUSB_Buffer=0;
//    bytesRead = 0;
//    bytesToWrite = 0;
//    amountCopiedToFlashBuf=0;
//    

//    if(!initCalled){
//        initCalled = __TRUE;
//        intelHexStartData();
//        for(i=0;i<FLASH_PROGRAM_PAGE_SIZE;i++)
//        {
//            //Erased cell in nRF51 is FF
//            flashBuffer[i]=0xFF;
//        }
//    }
//    //count the number of words to write
//    for(i=0;(i<(FLASH_PROGRAM_PAGE_SIZE/4)) && usb_buffer[i]>0 ;i++)
//    {            
//        sizeOfUSB_Buffer++;
//    }
//    
//    //count unwritten words from the previous pass
//    for(i=0;(i<FLASH_SECTOR_SIZE/4) && unrecordedUSBData[i]>0;i++)
//    {
//        sizeOfOldData++;
//    }
//    
//    //insert old data into the usb_buffer, if any
//    if(sizeOfOldData>0)
//    {
//        //shift usb_data
//        for(i=(sizeOfOldData + sizeOfUSB_Buffer -1);i>=sizeOfOldData ;i--)
//        {
//            usb_buffer[i] = usb_buffer[i-sizeOfOldData];
//        }
//        
//        //copy unused data from the old usb_data
//        for(i=0;i<sizeOfOldData;i++)
//        {
//            usb_buffer[i] = unrecordedUSBData[i];
//        }                           
//    }
//    //update data size
//    hex_data_size =(sizeOfOldData + sizeOfUSB_Buffer)*4;
//    loadOffset = (uint16_t)flashPtr;
//    
//    //Process hex data
//    if(intelHexReadData((uint8_t *)usb_buffer, hex_data_size, &bytesRead, intermediateBuffer, &flash_addr_offset, &bytesToWrite, &loadOffset, &forcePageWrite,usedIntermediateBufSize>0))
//    {   
//        //if buffered data is already as big as half the buffer, then force write otherwise risk overflow.
//        if(sizeOfOldData>(FLASH_SECTOR_SIZE/8)){
//            forcePageWrite = __TRUE;
//        }
//        flashPtr = (uint32_t)loadOffset;
//        //Copy processed data into flashBuffer
//        for(amountCopiedToFlashBuf=0;amountCopiedToFlashBuf<(bytesToWrite) &&  amountCopiedToFlashBuf<(FLASH_PROGRAM_PAGE_SIZE-usedIntermediateBufSize) ;amountCopiedToFlashBuf++)
//        {
//            flashBuffer[amountCopiedToFlashBuf+usedIntermediateBufSize] = intermediateBuffer[amountCopiedToFlashBuf];
//        }        
//        
//        if(((usedIntermediateBufSize+amountCopiedToFlashBuf)>=FLASH_PROGRAM_PAGE_SIZE) || forcePageWrite )
//        {//we have enough data or a write is forced, either way write the flashBuffer to device            
//            
//            if (!target_flash_program_page(flashPtr + flash_addr_offset, flashBuffer, FLASH_PROGRAM_PAGE_SIZE)) 
//            {//fail
//                return 1;
//            } 
//            
//            if((usedIntermediateBufSize+amountCopiedToFlashBuf)>=FLASH_PROGRAM_PAGE_SIZE)
//            {
//                flashPtr += FLASH_PROGRAM_PAGE_SIZE;   
//            }
//            else
//            {
//                flashPtr +=(usedIntermediateBufSize+amountCopiedToFlashBuf);
//            }
//            
//           
//           
//            //flush out flashBuffer
//            for(i=0;i<FLASH_PROGRAM_PAGE_SIZE;i++)
//            {
//                //Erased cell in nRF51 is FF
//                flashBuffer[i]=0xFF;
//            }

//            //copy the data omitted from the flash write to the top of flashBuffer
//            if((usedIntermediateBufSize+bytesToWrite) > FLASH_PROGRAM_PAGE_SIZE)
//            {
//                for(i=0;i<(bytesToWrite-amountCopiedToFlashBuf);i++)
//                {
//                    flashBuffer[i] = intermediateBuffer[i+amountCopiedToFlashBuf];
//                }
//                usedIntermediateBufSize=i;
//                if(forcePageWrite)
//                {
//                    //don't wait for the next usb_buffer write...
//                    if (!target_flash_program_page(flashPtr + flash_addr_offset, flashBuffer, FLASH_PROGRAM_PAGE_SIZE)) 
//                    {//fail
//                        return 1;
//                    } 
//                    flashPtr +=(bytesToWrite-amountCopiedToFlashBuf);
//                    
//                    //flush out flashBuffer
//                    for(i=0;i<(bytesToWrite-amountCopiedToFlashBuf);i++)
//                    {
//                        //Erased cell in nRF51 is FF
//                        flashBuffer[i]=0xFF;
//                    }
//                    usedIntermediateBufSize=0;
//                }
//            }
//            else
//            {
//                usedIntermediateBufSize=0;
//            }
//        }
//        else
//        {
//            usedIntermediateBufSize+=amountCopiedToFlashBuf;
//        }
//        
//        //Final page is received -- make sure everything is written to flash
//        if (current_sector == nb_sector) 
//        {
//            //make sure to clear the top element from the history before finishing the transfer
//            unrecordedUSBData[0] = 0; 
//            hex_data_used=bytesRead;
//            
//            //check if there is still unwritten data -- do the last writes
//            while(hex_data_size>hex_data_used){                    
//                bytesRead =0;
//                bytesToWrite=0;
//                no_words_to_used = (hex_data_used-1)/4;
//                if(intelHexReadData((uint8_t *)(&usb_buffer[no_words_to_used]), hex_data_size-(no_words_to_used*4), &bytesRead, intermediateBuffer, &flash_addr_offset, &bytesToWrite, &loadOffset, &forcePageWrite,usedIntermediateBufSize>0))
//                {
//                    flashPtr = (uint32_t)loadOffset;                
//                    for(amountCopiedToFlashBuf=0;amountCopiedToFlashBuf<(bytesToWrite) &&  amountCopiedToFlashBuf<(FLASH_PROGRAM_PAGE_SIZE-usedIntermediateBufSize) ;amountCopiedToFlashBuf++)
//                    {
//                        flashBuffer[amountCopiedToFlashBuf+usedIntermediateBufSize] = intermediateBuffer[amountCopiedToFlashBuf];
//                    }
//                    //write the data processed
//                    if (!target_flash_program_page(flashPtr + flash_addr_offset, flashBuffer, FLASH_PROGRAM_PAGE_SIZE)) 
//                    {
//                        return 1;
//                    } 
//                    
//                    //flush out flashBuffer
//                    for(amountCopiedToFlashBuf=0;amountCopiedToFlashBuf<(bytesToWrite) &&  amountCopiedToFlashBuf<(FLASH_PROGRAM_PAGE_SIZE-usedIntermediateBufSize) ;amountCopiedToFlashBuf++)
//                    {
//                        //Erased cell in nRF51 is FF
//                        flashBuffer[amountCopiedToFlashBuf]=0xFF;
//                    }                
//                    hex_data_used += bytesRead;
//                }
//                else{
//                    reason = CORRUPT_FILE;
//                    initDisconnect(0);
//                    return 1;
//                }
//            }
//            //flush arrays and reset variables here?
//            initDisconnect(1);
//            return 0;
//        }
//        
//        no_words_to_used = bytesRead/4;
//        hex_data_size = hex_data_size/4;
//        for(i=0;i < (hex_data_size-no_words_to_used);i++)
//        {
//            unrecordedUSBData[i] = usb_buffer[no_words_to_used+i];
//        }
//        if(i>FLASH_SECTOR_SIZE/4){
//            reason = OUT_OF_MEM;
//            initDisconnect(0);
//        }
//        
//        //clear unused buffer values
//        for(;i<FLASH_SECTOR_SIZE/4;i++){
//            unrecordedUSBData[i] =0;
//        }
//        
//        for(i=FLASH_SECTOR_SIZE/8;i <FLASH_SECTOR_SIZE/2;i++){
//            usb_buffer[i] =0;
//        }
//    }
//    else
//    {
//        reason = CORRUPT_FILE;
//        initDisconnect(0);
//        return 1;
//        
//    }
//    
//    return 0;  
//}
