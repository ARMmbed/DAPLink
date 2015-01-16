#include "intelhex.h"
#include "stdio.h"

#define INVALID_HEX_CHAR (0xff)     /* Indicates a character not in the sets 0-9 or A-F */
#define NON_DATA_RECORD_SIZE (5)    /* Total size of RECLEN (1), LOAD OFFSET (2), RECTYP (1) and CHKSUM (1) fields. */
#define MAX_DATA_SIZE (255)         /* Maximum size of DATA (or INFO) field (maximum value of RECLEN). */
#define MAX_RECORD_DATA (NON_DATA_RECORD_SIZE+MAX_DATA_SIZE) /* Maximum number of bytes we expect in a HEX record */

/* Intel HEX record types: */
#define REC_DATA (0)
#define REC_EOF (1)
#define REC_EXT_SEG (2)
#define REC_SRT_SEG (3)
#define REC_EXT_LIN (4)
#define REC_SRT_LIN (5)

static  uint8_t recordData[MAX_RECORD_DATA]; // The complete hex record read from the raw buffer. 
static  uint16_t prevLoadOffset;    //Address Offset from the previous hex record
static  uint8_t prevRecordLength;   //Data length of the previous hex record
/**
*   @brief Converts a character representation of a hex to real value.
*   @param c is the hex value in char format
*   @return the value of the hex otherwise INVALID_HEX_CHARACTER
*/

__inline static uint8_t asciiToHex(char c)
{
    const char a[]="0123456789ABCDEF";
    uint8_t x=0;

    do 
    {
        if (a[x]==c)
        {
            return x;
        }
        x++;
    } while (x<16);

    return INVALID_HEX_CHAR;
}

/**
*   @brief Check that the provided line is an uncorrupted hex record
*   @param data is the line of hex record
*   @param size is the length of the data array
*   @return __TRUE if the data provided is a valid hex record otherwise __FALSE
*/
static BOOL intelHexChecksum(uint8_t *data, uint32_t size)//__inline
{
    uint32_t n;
    uint8_t c;

    c=0;

    for (n=0; n<size; n++)
    {
        c += data[n];
    }

    return (c==0);
} 

/**
*   @brief Init function that needs to be called before reading a new file
*/
void intelHexStartData(void)
{
    /// Variables to initialise before reading a hex file.
   // lba = 0;
   // dataCount = 0;
   prevLoadOffset = 0;
   prevRecordLength = 0;
}
/** 
*   @brief  Extracting a hex record from the raw data provided
*   @param  hexData is the hex file provided as a byte array
*   @param  buffer is a hex record extracted from hexData (single line with ':', LF and CR removed)
*   @param  totalBytesRead is a counter value for keeping track of our position in hexData
*   @param  hexDataSize is the size of hexData array.
*   @return size of the hex record extracted.
*/
uint32_t intelHexReadRecord(uint8_t *hexData, uint32_t hexDataSize, uint8_t *buffer, uint32_t *totalBytesRead)
{
    BOOL done;
    BOOL lowNib;
    BOOL startOfLine;
    uint32_t byteCount;
    uint8_t curChar;
    uint8_t curHex;
    uint32_t bytesRead = (*totalBytesRead);
    
    done=__FALSE;  
    startOfLine = __TRUE;
    lowNib = __FALSE;   
    byteCount = 0;
    
    do
    {        
        if(bytesRead>=hexDataSize)
        {
            bytesRead = (*totalBytesRead);
            done = __TRUE;
            byteCount=0; 
        }
        curChar = hexData[bytesRead];
        bytesRead++;
        if(startOfLine)//it should be start of line,':'
        {
            switch(curChar)
            {
                case ':':
                    startOfLine = __FALSE;
                    byteCount=0; 
                    lowNib = __FALSE;
                    break;
                case EOF:
                    done = __TRUE;
                    break;
                default:
                    //Keep looking
                    break;
            }      
        }
        else//it should be DATA
        {
            switch(curChar)
            {
                case '\r':
                case '\n':
                case EOF:
                    // End of line or end of file
                    done = __TRUE;
                    break;
                case ':':
                    // Start of next record (normally see \r and/or \n before this)
                    done = __TRUE;
                    //don't count this character as data read
                    bytesRead--;
                    break;
                default:
                    curHex = asciiToHex(curChar);
                    if (curHex==INVALID_HEX_CHAR)
                    {
                        //Illegal character, ignore the rest of this line
                        startOfLine = __TRUE;
                    }
                    else
                    {
                        if (lowNib) // Low nibble
                        {
                            buffer[byteCount] |= curHex;
                            byteCount++;
                        }
                        else        // High nibble 
                        {
                            buffer[byteCount] = curHex<<4;
                        }
                        lowNib = !lowNib;                            
                    }
                    break;
            }      
        }
        
        
    }while(!done);
    
    (*totalBytesRead) = bytesRead;
    return byteCount;
    
}

/**
*   @brief  Extract the data fields from the hex format and arranges them in binary format.
*   @param  hexData is the hex file provided as a byte array
*   @param  hexDataBytesRead number of bytes read from the rawBuffer
*   @param  buffer is the resulting byte array (ready to be flashed).
*   @param  hexDataSize is the size for hexData.
*   @param  flashAddress is the starting address for the buffer to be written: this is read from the hex file.
*   @param  readSize is the used space in the resulting byte array (actual size to be written to flash).
*   @param  initLoadOffset is the data offset for the first record of a new page, 
*           i.e. it will only be updated if the hex record read is the start of a new page in the flash.
*   @param  forcePageWrite is a boolean value to indicate if a page write is necessary even though there is 
*           not enough data to fill a page
*   @param  haveDataToWrite is a boolean value to indicate if the caller already has data to write and does not
*           want a jump in the address space.
*   @return TRUE if the parsing of hexData was succcessful, otherwise FALSE
*/
BOOL intelHexReadData(uint8_t *hexData, uint32_t hexDataSize, uint32_t *hexDataBytesRead, uint8_t *buffer, uint32_t *flashAddress, uint32_t *readSize, uint16_t *initLoadOffset, BOOL *forcePageWrite, BOOL haveDataToWrite)
{
    uint32_t bytesRead;      //Total size in bytes (2 hex characters from the file) of the hex record read from the raw buffer hexData.
    uint32_t totalBytesRead; //Total size in bytes of the hex record read from the raw buffer hexData.
    uint8_t recordLength;    //Size in bytes of the DATA or INFO field of the hex record.
    uint8_t recordType;      //Record type.
    uint16_t loadOffset;     //Load offset; where to write the data.
    uint32_t bufferIdx;      //Current offset into "buffer" -- also the number of bytes read into buffer.
    BOOL eof;                //TRUE if end of file record has been read from the file.
    uint32_t lba;            //Linear base address
    uint8_t *pData;          //Pointer to next byte of data to read in the DATA field of recordData.
    uint8_t dataCount;       //Data in recordData array that has not been copied to the buffer
    
    loadOffset = *initLoadOffset;
    
    totalBytesRead = *hexDataBytesRead;
    bufferIdx = 0;
    eof = __FALSE;
    *forcePageWrite = __FALSE;
    dataCount = 0;
    pData = NULL;
     
    do{

        if (dataCount==0)
        {
            //If dataCount==0 then we need to read data from the hex file. 
            bytesRead = intelHexReadRecord(hexData, hexDataSize, recordData, &totalBytesRead);
            
            //Check we have at least the minimum amount of data for a valid record. 
            if (bytesRead < NON_DATA_RECORD_SIZE) 
            {                
                // Too short, return the data obtained from the preceding hex records
               *readSize = bufferIdx;
               *hexDataBytesRead = totalBytesRead;
                return __TRUE;
            }
        
            //Validate checksum 
            if (!intelHexChecksum(recordData, bytesRead))
            {
                //Failed checksum
                return __FALSE;
            }
        
            //Read header information
            recordLength = recordData[0];
            loadOffset =  recordData[1] << 8;
            loadOffset |= recordData[2];
            recordType = recordData[3];

            //Check record length and bytesRead add up correctly. 
            if (bytesRead != (recordLength + NON_DATA_RECORD_SIZE)) 
            {
                //Invalid lengths
                return __FALSE;
            }
    
            //Process according to record type
            switch (recordType)
            {
                case REC_DATA: //00
                    if( prevLoadOffset!=loadOffset && ((prevLoadOffset + prevRecordLength) != loadOffset) )
                    {
                        //skipped flash address
                        if(bufferIdx>0 || haveDataToWrite)
                        {
                        //break the data here; return
                            *readSize = bufferIdx;
                            *forcePageWrite = __TRUE;
                            return __TRUE;
                        }
                        else
                        {
                            //update loadOffset
                            *initLoadOffset = loadOffset;
                        }
                    }
                    //Get a pointer to the data and a count of the number of bytes.
                    pData = &recordData[4];
                    dataCount = recordLength;
                    prevLoadOffset = loadOffset;
                    prevRecordLength = recordLength;
                    *hexDataBytesRead = totalBytesRead;
                    break;

                case REC_EXT_LIN: //04 - The new base address (non-contiguous flash use), NRF: UICR Value address will be given here
                    // Get the linear base address
                    lba  = recordData[4] << 24;
                    lba |= recordData[5] << 16;
                
                    if(bufferIdx> 0 || haveDataToWrite)
                    {                        
                       //address jump-- break the data here; return
                       *readSize = bufferIdx;
                       *forcePageWrite = __TRUE;
                        //we don't update the hexDataBytesRead with totalBytesRead so that this line can be read once more
                        return __TRUE;
                    }
                    else
                    {
                        *readSize = 0;
                        *flashAddress = lba;
                        *initLoadOffset = loadOffset;
                        prevLoadOffset = loadOffset;
                        prevRecordLength = recordLength;
                        *hexDataBytesRead = totalBytesRead;
                    }
                    break;

                case REC_SRT_LIN: //05
                    //NRF application start address can be non-zero (e.g. 0x1C000000) when softdevice is used
                    //For LPC17XX, uVision appears to use the address of main() instead of address zero. 
#if (!defined(DBG_LPC1768) && !defined(DBG_NRF51822AA))
                    //Others must start from address zero
                    if ( (recordData[4]!=0x00) || (recordData[5]!=0x00) ||(recordData[6]!=0x00) ||(recordData[7]!=0x00))
                    {
                        // Invalid start address 
                        return __FALSE;
                    }
#endif
                    break;
 
                case REC_EXT_SEG: //02
                case REC_SRT_SEG: //03
                    // Not supported, should not occur in ARM hex files
                    return __FALSE;                 
                case REC_EOF: //01                    
                    *forcePageWrite = eof = __TRUE;
                    prevLoadOffset = loadOffset;
                    prevRecordLength = recordLength;
                    *hexDataBytesRead = totalBytesRead;
                    break;
 
                default:
                    //Unknown record type - ignore it.
                    break;
            }            
        }
       
        if (dataCount>0)
        {           
            //We have some data to copy to the output buffer.
            do {                    
                *(buffer++) = *(pData++); //Copy data from the DATA field of the hex record to the output buffer.
                bufferIdx++;
                dataCount--;
                //Repeat while there is unread in the DATA field and we have not filled the output buffer.
            } while((dataCount>0));// && (bufferIdx<hexDataSize));
        }

    //Repeat until we have filled the output buffer or reached the end of the hex file.
    } while (!eof);//(bufferIdx<hexDataSize) && 

    //Return the number of bytes written to the ouput buffer.
    *readSize = bufferIdx;

    return __TRUE;
}
