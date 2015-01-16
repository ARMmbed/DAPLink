#ifndef INTELHEX_H
#define INTELHEX_H

#include "stdint.h"
typedef unsigned int BOOL;
#define __TRUE          1
#define __FALSE         0

void intelHexStartData(void);
BOOL intelHexReadData(uint8_t *hexData, uint32_t hexDataSize, uint32_t *hexDataBytesRead, uint8_t *buffer, uint32_t *flashAddress, uint32_t *readSize, uint16_t *initLoadOffset, BOOL *forcePageWrite, BOOL haveDataToWrite);

#endif
