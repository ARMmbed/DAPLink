
#ifndef DAPLINK_DEBUG_H
#define DAPLINK_DEBUG_H

#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "RTL.h"
#include "rl_usb.h"

//#define DEBUG

#ifdef __cplusplus
 extern "C" {
#endif
     
#if defined (DEBUG)
static inline uint32_t daplink_debug_print(const char* format, ...)
{
    char buf[128] = {0};
    int r = 0;
    __va_list arg;
    
    va_start(arg, format);
    r = vsprintf(buf, format, arg);
    if(r > sizeof(buf)) {
        return 0;
    }
    va_end(arg);
    USBD_CDC_ACM_DataSend((uint8_t *)&buf, strlen(buf));
    os_dly_wait(1);
    return r;
}

static inline void daplink_debug(uint8_t *buf, uint32_t size)
{
    USBD_CDC_ACM_DataSend((uint8_t *)&buf, size);
    os_dly_wait(5);
}

#else

static inline uint32_t daplink_debug_print(const char* format, ...)
{
    return 1;
}

static inline uint32_t daplink_debug(uint8_t *data, uint32_t size)
{
    return 1;
}

#endif

#define debug_msg(fmt, args...) daplink_debug_print(fmt, ## args);
#define debug_data(buf, size) daplink_debug(buf, size);

#ifdef __cplusplus
 }
#endif

#endif
