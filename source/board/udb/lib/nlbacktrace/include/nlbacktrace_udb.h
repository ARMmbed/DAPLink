#ifndef __NLBACKTRACE_H_INCLUDED__
#define __NLBACKTRACE_H_INCLUDED__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int nlbacktrace(uint32_t initialPC, uint32_t initialSP, uint32_t stackTop, uint32_t* buffer, size_t maxLen);
int nlbacktrace_with_lr(uint32_t initialPC, uint32_t initialLR, uint32_t initialSP, uint32_t stackTop, uint32_t* buffer, size_t maxLen);
int nlbacktrace_no_context(uint32_t initialSP, uint32_t stackTop, uint32_t* buffer, size_t maxLen, size_t minLen, size_t maxAttempts);

/* Weak functions that can be overrided by a product to provide
 * more product specific checks for validity.  Current checks
 * assume all of RAM is valid for stacks.  Also, all of RAM,
 * and the FLASH regions for bootloader and app are valid for
 * code.
 */
bool isAddressValidForCode(uint32_t addr);
bool isAddressValidForStack(uint32_t addr);

#endif // __NLBACKTRACE_H_INCLUDED__
