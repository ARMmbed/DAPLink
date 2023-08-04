// -*- Mode: C++; tab-width: 4 indent-tabs-mode: nil -*-
//
//    Copyright (c) 2013 Nest Labs, Inc.
//    All rights reserved.
//
//    This document is the property of Nest. It is considered
//    confidential and proprietary information.
//
//    This document may not be reproduced or transmitted in any form,
//    in whole or in part, without the express written permission of
//    Nest.
//
//    Description:
//      This file implements the nlbacktrace() function.
//      It's used to retrieve the backtrace after a crash without prior knowledge
//      on what the stack frames look like (i.e. no need for frame pointer)
//

/*
    Theory of operation

    There are two types of functions:
        - non-leaf: functions that make a call to another function
        - leaf: functions that don't call other function.

    For non-leaf functions, the compiler always follows the pattern below when generating the code:

    The prologue of the function looks like this: a push (or store multiple and decrement with SP as destination),
    And an optional SP decrement for local variables.
        push/stmd  sp!, {rx, ry, ....., lr}
        sub sp, sp, #xx

    The epilogue has to undo what the prologue did in order to leave the SP where it was before the function call
        add sp, sp, #xx
        pop/ldm   sp!, {rx, ry, ....., pc}

    The nlbacktrace function takes two parameters: initialPC, initialSP.
    The logic to follow is:
        - make currentPC=initialPC, currentSP=initialSP.
        - walk backward from the currentPC until we find either a push or a stmfd with SP as destination
        - decode the push/stmfd instruction to find out how many registers got pushed.
        - look for the "sub sp" instruction and decode it (it's usually the first or second instruction down from the push.
        - check if there was an additional "sub sp" before the push (this appears to be used in conjunction with variable arguments)
        - compute currentSP = SP + 4 * num_of_registers_pushed + space that was subtracted for the local variables.
        - compute LR = currentSP[-1]
        - lather, rinse and repeat

    This procedure works fairly well for non-leaf function but doesn't work for leafs.
    So if we get a crasher in a leaf we don't get the right backtrace. Unfortunately leafs are
    just as likely to crash.
    As a first level heuristics, we call the nlbacktrace function twice: once with PC and once with LR,
    and we keep the deepest stack trace we receive.  For non-leaf functions, stack traces based on the
    PC should always be greater or equal in length to stack traces based on the LR.  For leaf functions,
    stack traces based on the PC will often be wrong because there won't be a push/stmfd in the leaf
    function and the search finds the push/stmfd of some other functon instead, and unwinds the SP
    incorrectly.  The stack trace based on the LR should give better results for the leaf case.
    This takes care of the vast majority of the crasher and lets us put together automated tools for stack tracing.

    When a stack is for a FreeRTOS task, FreeRTOS puts at the top of the stack
    at the LR position the address of a function set by configTASK_RETURN_ADDRESS.
    If not defined, it is a static function in FreeRTOS port.c typically called
    prvTaskExitError().  It's purpose is to assert if a task's main function
    ever returns, because that is not a valid usage case (a task is supposed
    to call vTaskDelete() if it every is done executing).

    To help backtrace find the end, it's recommended that projects set
    configTASK_RETURN_ADDRESS to &abort, and make sure abort is a terminal
    leaf function (something like have it do __builtin_trap()).  This function
    has no push/stmfd since the compiler knows it's never going to return,
    and if we find it on a backtrace, we're confident that we cannot have unwound
    to it (since nothing is called from it) so it must be the end of backtrace marker.

    If configTASK_RETURN_ADDRESS isn't set to &abort or abort isn't a terminal
    leaf function, backtrace will probably just keep unwinding and finding some
    false LR values.
*/

#include "nlbacktrace_udb.h"

#define INVALID_PC (0xffffffff)

// Not currently defined in nllibc-lite, though it is in stdlib.h
// Some platforms have it, like nlplatform_em358x, but just in case
// we define a weak default.
void abort(void) __attribute__((weak));

void abort(void)
{
    __builtin_trap();
}

// K&R style count bit set
unsigned countBitSet(uint32_t value)
{
    unsigned i;
    for (i = 0; value ; i++)
    {
        value &= (value - 1);
    }
    return i;
}

/* Make weak to allow a product to override with a narrower check.
 * This default version is very lenient, and is not aware of possible
 * restrictions like having an MPU that prevents execute from RAM,
 * or has guard regions in RAM with no access.
 */
bool isAddressValidForCode(uint32_t addr) __attribute__((weak));
bool isAddressValidForCode(uint32_t addr)
{
    return true;
}

/* Make weak to allow a product to override with a narrower check.
 * This default version is very lenient, and is not aware of possible
 * restrictions like having an MPU that prevents execute from RAM,
 * or has guard regions in RAM with no access.
 */
bool isAddressValidForStack(uint32_t addr) __attribute__((weak));
bool isAddressValidForStack(uint32_t addr)
{
    return true;
}

bool checkSP(uint32_t addr, uint32_t stackTop)
{
    bool result;
    if (stackTop)
    {
        result = addr < stackTop;
    }
    else
    {
        result = isAddressValidForStack(addr);
    }
    return result;
}

static uint32_t get_modified_immediate_constant(uint32_t inst)
{
    uint8_t i_imm3_a;

    /* imm_const will be set below but Coverity can't figure it out. */
    uint32_t imm_const = 0;

    /* See A5.3.2 "Modified immediate constants in Thumb instructions" in the
     * ARMv7-M Architectural Reference Manual, which explains how constants
     * are encoded.
     */
    i_imm3_a = ((inst & (0x1 << 26)) >> 22) | ((inst & (0x7 << 12)) >> 11) | ((inst & 0x80) >> 7);

    if (i_imm3_a < 0x8)
    {
        uint32_t imm8 = inst & 0xff;

        /* When i:imm3:a is less than 8, the lower two bits of imm3 give the
         * following special encodings. */
        switch (i_imm3_a >> 1)
        {
            case 0x0:
            imm_const = imm8;
            break;

            case 0x1:
            imm_const = imm8 << 16 | imm8;
            break;

            case 0x2:
            imm_const = imm8 << 24 | imm8 << 8;
            break;

            case 0x3:
            imm_const = imm8 << 24 | imm8 << 16 | imm8 << 8 | imm8;
            break;
        }
    }
    else
    {
        /* When i:imm3:a is greater than or equal to 8, it encodes a shift of
         * imm8. Since i:imm3:a is 5-bits, its range here will be between 8 and
         * 31 inclusive. */
        imm_const = ((inst & 0x7f) | 0x80) << (32 - i_imm3_a);
    }

    return imm_const;
}

/* Unwind the stack frame indicated by pc and sp.
 *
 * The return value is INVALID_PC if no lr-push could be found. Otherwise it is
 * the found lr.
 *
 * sp is modified according to any stack pointer adjustments found in the
 * current frame.
 *
 * The functions iterates backwards from pc, instruction by instruction, in two
 * phases:
 *
 * 1. "lr search":
 *    Searching backwards from a (likely) faulting pc, looking for the
 *    instruction which pushed the link register. Along the way, any stack
 *    pointer adjustments from sub-sp instructions should be counted. This
 *    search continues indefinitely until either the push is found or an
 *    invalid pc is encountered.
 *
 * 2. "prologue search":
 *    If the lr search phase is successful, the search for sp modifying
 *    instructions continues for PROLOGUE_SEARCH_LEN instructions. This search
 *    includes both sub-sp instructions and non-lr pushing instructions.
 */
static uint32_t find_lr_and_sp_for_frame(uint32_t pc, uint32_t *sp, uint32_t stackTop)
{
#define ARM_PUSH_T1_MASK        0xFF00
#define ARM_PUSH_W_LR_T1_COMP   0xB500
#define ARM_PUSH_NO_LR_T1_COMP  0xB400
#define ARM_PUSH_T1_REG_MASK    0x01FF

#define ARM_PUSH_T2_MASK        0xFFFFE000
#define ARM_PUSH_W_LR_T2_COMP   0xE92D4000
#define ARM_PUSH_NO_LR_T2_COMP  0xE92D0000
#define ARM_PUSH_T2_REG_MASK    0x0000FFFF

#define ARM_SUB_SP_IMM_T1_MASK  0xFF80
#define ARM_SUB_SP_IMM_T1_COMP  0xB080
#define ARM_SUB_SP_IMM_T2_MASK  0xFBEF8F00
#define ARM_SUB_SP_IMM_T2_COMP  0xF1AD0D00
#define ARM_SUB_SP_IMM_T3_MASK  0xFBFF8F00
#define ARM_SUB_SP_IMM_T3_COMP  0xF2AD0D00

#define ARM_VPUSH_MASK          0xFFBF0E00
#define ARM_VPUSH_COMP          0xED2D0A00
#define ARM_VPUSH_REG_MASK      0x000000FF
#define ARM_VPUSH_REG_POS       0

#define PROLOGUE_SEARCH_LEN     2

    uint32_t return_lr = INVALID_PC;

    /* sp_mod is used to accumulate stack pointer modifications in register
     * (4-byte) increments. */
    uint32_t sp_mod = 0;

    /* Only match lr-pushes during lr search. */
    uint32_t push32_comp = ARM_PUSH_W_LR_T2_COMP;
    uint16_t push16_comp = ARM_PUSH_W_LR_T1_COMP;

    uint16_t prologue_search_len = PROLOGUE_SEARCH_LEN;

    enum
    {
        lr_search,
        lr_search_done,
        prologue_search
    } search_phase = lr_search;

    uint16_t inst16 = 0;

    while (1)
    {
        uint16_t last_inst16;
        uint32_t inst32;

        if (!isAddressValidForCode(pc))
        {
            break;
        }

        last_inst16 = inst16;
        inst16 = *((uint16_t *) pc);
        inst32 = (inst16 << 16) | last_inst16;

        /* Look for relevant instructions. There are seven total, listed here
         * with the referenced section from the ARMv7-M Architecture Reference
         * Manual.
         * 1. 32-bit push       PUSH<c>.W <registers>           A7.7.99  (T2)
         * 2. 32-bit sub sp     SUB{S}<c>.W <Rd>,SP,#<const>    A7.7.173 (T2)
         * 3. 32-bit sub sp     SUBW<c> <Rd>,SP,#<imm12>        A7.7.173 (T3)
         * 4. 16-bit push       PUSH<c> <registers>             A7.7.99  (T1)
         * 5. 16-bit sub sp     SUB<c> SP,SP,#<imm7>            A7.7.173 (T1)
         * 6. 32-bit vpush 64   VPUSH<c> <64-bit registers>     A7.7.249 (T1)
         * 7. 32-bit vpush 32   VPUSH<c> <32-bit registers>     A7.7.249 (T2)
         */
        {
            if ((inst32 & ARM_PUSH_T2_MASK) == push32_comp)
            {
                uint32_t regs_pushed;

                regs_pushed = countBitSet(inst32 & ARM_PUSH_T2_REG_MASK);
                sp_mod += regs_pushed;

                if (search_phase == lr_search)
                {
                    search_phase = lr_search_done;
                }
            }
            else if ((inst32 & ARM_SUB_SP_IMM_T2_MASK) == ARM_SUB_SP_IMM_T2_COMP)
            {
                uint32_t imm32;

                /* Encoding T2 of SUB (SP minus immediate) encodes the immediate
                 * constant as a 'modified immediate constant'. */
                imm32 = get_modified_immediate_constant(inst32);
                sp_mod += imm32/4;
            }
            else if ((inst32 & ARM_SUB_SP_IMM_T3_MASK) == ARM_SUB_SP_IMM_T3_COMP)
            {
                uint32_t imm32;

                /* Encoding T3 of SUB (SP minus immediate) encodes the immediate
                 * constant in three disjoint bitfields, i:imm3:imm8. */
                imm32 = ((inst32 & (0x1 << 26)) >> 15) | ((inst32 & (0x7 << 12)) >> 4) | (inst32 & 0xff);
                sp_mod += imm32/4;
            }
            else if ((inst16 & ARM_PUSH_T1_MASK) == push16_comp)
            {
                uint32_t regs_pushed;

                regs_pushed = countBitSet(inst16 & ARM_PUSH_T1_REG_MASK);
                sp_mod += regs_pushed;

                if (search_phase == lr_search)
                {
                    search_phase = lr_search_done;
                }
            }
            else if ((inst16 & ARM_SUB_SP_IMM_T1_MASK) == ARM_SUB_SP_IMM_T1_COMP)
            {
                uint32_t imm32;

                /* Encoding T1 of SUB (SP minus immediate) encodes the immediate
                 * constant in the lower 7 bits (imm7). It is interpreted as imm7:'00'
                 * so that it doesn't need to be divided by 4 before being added to
                 * sp_change. */
                imm32 = inst16 & 0x7f;
                sp_mod += imm32;
            }
/* Our compiler will set this flag indicating that we are using ARM's vector
 * floating point extension. */
#if defined(__VFP_FP__)
            else if ((inst32 & ARM_VPUSH_MASK) == ARM_VPUSH_COMP)
            {
                uint32_t words_pushed;

                /* We can handle both encodings of VPUSH here since both encode
                 * the total number of words pushed directly regardless of
                 * whether they are ultimately pushing double or single registers. */
                words_pushed = (inst32 & ARM_VPUSH_REG_MASK) >> ARM_VPUSH_REG_POS;
                sp_mod += words_pushed;
            }
#endif
        }

        if (search_phase == lr_search_done)
        {
            /* Found the lr push. Check the modified sp before
             * dereferencing. */
            uint32_t tmp_lr;
            uint32_t *tmp_sp = (uint32_t *)(*sp) + sp_mod;

            if (!checkSP((uint32_t)tmp_sp, stackTop))
            {
                break;
            }

            /* lr should be last register pushed, which on a descending stack
             * means that it is at -1 offset. */
            tmp_lr = tmp_sp[-1];

            if (!isAddressValidForCode(tmp_lr))
            {
                break;
            }

            return_lr = tmp_lr;

            /* Switch to prologue search. */
            search_phase = prologue_search;
            push32_comp = ARM_PUSH_NO_LR_T2_COMP;
            push16_comp = ARM_PUSH_NO_LR_T1_COMP;
        }

        if ((search_phase == prologue_search) && (prologue_search_len-- == 0))
        {
            break;
        }

        /* Decrement the pc by one 16-bit instruction. */
        pc -= 2;
    }

    *sp += sp_mod*4;

    return return_lr;
}


int nlbacktrace(uint32_t initialPC, uint32_t initialSP, uint32_t stackTop, uint32_t* buffer, size_t maxLen)
{
    size_t level = 0;
    uint32_t currentSP = initialSP;
    uint32_t currentPC = initialPC;

    // we assume that initialPC is valid

    while (level < maxLen)
    {
        // save the PC
        *buffer++ = currentPC;
        level++;

        // strip the Thumb bit before passing to find_lr_and_sp_for_frame
        currentPC = currentPC & ~0x1;

        currentPC = find_lr_and_sp_for_frame(currentPC, &currentSP, stackTop);

        if (currentPC == INVALID_PC)
        {
            break;
        }

        // stop if lr is &abort since that's our special indicator
        // of top of task stack.  A real abort() would always be a leaf
        // and at end of stack, not somewhere in the middle.
        if (currentPC == (uint32_t)&abort)
        {
            break;
        }
    }

    return (int)level;
}

int nlbacktrace_with_lr(uint32_t initialPC, uint32_t initialLR, uint32_t initialSP, uint32_t stackTop, uint32_t* buffer, size_t maxLen)
{
    /* Generally, the backtrace from the PC is the best.  However,
     * if the PC is in a leaf function (which has no preamble that pushes
     * registers), the backtrace function will return either no result
     * or a wrong result because the search for the PUSH instruction will
     * find the PUSH of the wrong function.
     */
    int num_backtraces_lr;
    int num_backtraces;
    bool pc_is_valid = isAddressValidForCode(initialPC);
    if (((initialLR & ~0x1) == initialPC) || !isAddressValidForCode(initialLR))
    {
        /* If the LR is invalid, or same as the PC, don't bother */
        num_backtraces_lr = 0;
    }
    else
    {
        /* Regardless of whether the PC is valid or not, try a LR
         * based backtrace first
         */
        num_backtraces_lr = nlbacktrace(initialLR, initialSP, stackTop, buffer+1, maxLen-1);

        /* If num_backtraces_lr is 0, but PC is valid, we add the PC to
         * the buffer and increment num_backtraces_lr by 1.
         * If num_backtraces_lr > 0, regardless of whether PC is valid or not,
         * we add the PC to the buffer and increment num_backtraces_lr by 1
         * (this latter can happen if the fault was a bl to a bad address like 0x0).
         * Only case where we don't want to do the increment is if num_backtraces_lr
         * is 0 and PC is not valid.
         */
        if ((num_backtraces_lr > 0) || pc_is_valid)
        {
            buffer[0] = initialPC;
            num_backtraces_lr++;
        }
    }
    /* Get a backtrace from the PC if it is valid */
    if (!pc_is_valid)
    {
        /* Return the LR backtrace count, if any */
        num_backtraces = num_backtraces_lr;
    }
    else
    {
        num_backtraces = nlbacktrace(initialPC, initialSP, stackTop, buffer, maxLen);

        /* Check if the LR backtrace was deeper, which can happen if the PC
         * was in a leaf function without a preamble
         */
        if (num_backtraces_lr > num_backtraces)
        {
            /* Redo lr backtrace to the buffer*/
            buffer[0] = initialPC;
            num_backtraces = nlbacktrace(initialLR, initialSP, stackTop, buffer+1, maxLen-1) + 1;
        }
    }
    return num_backtraces;
}

/* We don't have a starting PC or LR.  Look in the stack for a possible
 * LR and backtrace from it.  We check at most maxAttempts from the stack
 * for a LR value that nlbacktrace() returns a backtrace of at least minLen
 * size.  If none is found using nlbacktrace(), we then fall back to
 * just returning an array of possible LR values scanning from the initialSP
 * to stackTop, stopping at maxLen entries or when we hit an invalid SP,
 * whichever is hit first.
 */
int nlbacktrace_no_context(uint32_t initialSP, uint32_t stackTop, uint32_t* buffer, size_t maxLen, size_t minLen, size_t maxAttempts)
{
    size_t i;
    uint32_t* currentSP = (uint32_t*)initialSP;
    int num_backtraces = 0;
    for (i = 0; i < maxAttempts && checkSP((uint32_t)currentSP, stackTop); i++)
    {
        uint32_t possibleLR = *currentSP++;
        if ((possibleLR & 0x1) && isAddressValidForCode(possibleLR))
        {
            num_backtraces = nlbacktrace(possibleLR, (uint32_t)currentSP, stackTop, buffer, maxLen);
            if ((size_t)num_backtraces >= minLen)
            {
                goto done;
            }
        }
    }
    if ((size_t)num_backtraces < minLen)
    {
        /* nlbacktrace unable to find a valid backtrace, just return
         * guesses of possible LRs
         */
        currentSP = (uint32_t*)initialSP;
        num_backtraces = 0;
        while ((size_t)num_backtraces < maxLen && checkSP((uint32_t)currentSP, stackTop))
        {
            uint32_t possibleLR = *currentSP++;
            if ((possibleLR & 0x1) && isAddressValidForCode(possibleLR))
            {
                buffer[num_backtraces++] = possibleLR;
            }
        }
    }
done:
    return num_backtraces;
}
