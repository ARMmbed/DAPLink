/*
 *
 *    Copyright (c) 2012-2018 Nest Labs, Inc.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *      This file defines a framework for an interactive command line
 *      user interface.
 *
 *      @note The actual commands, set/show parameters, and prompt are
 *            configured at the project level.
 *
 */

/*
 * Copied to this repository from platform/nlutilities on 2021-12-22 and
 * removed dependencies on other unneeded headers.
 */

#ifndef NLUTILITIES_NLUIF_H
#define NLUTILITIES_NLUIF_H

#include <stdbool.h>
#include <stdint.h>

/********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  @def NLUIF_USE_MINISHELL
 *
 *  @brief
 *    The minishell build feature uses smaller default values for max-args,
 *    max-line and max-prompt, and uses a pared down UIF command
 *    structure, eliding both syntax and descripton.
 */
#ifndef NLUIF_USE_MINISHELL
#define NLUIF_USE_MINISHELL   0
#endif /* NLUIF_USE_MINISHELL */

/**
 *  @def NLUIF_USE_VERBOSE
 *
 *  @brief
 *    The verbose build feature enables both usage syntax and command
 *    description UIF fields.
 */
#ifndef NLUIF_USE_VERBOSE
#define NLUIF_USE_VERBOSE     !NLUIF_USE_MINISHELL
#endif /* NLUIF_USE_VERBOSE */

/**
 *  @def NLUIF_USE_DESCRIPTION
 *
 *  @brief
 *    The description build feature enables the UIF description field.
 */
#ifndef NLUIF_USE_DESCRIPTION
#define NLUIF_USE_DESCRIPTION (NLUIF_USE_VERBOSE && !NLUIF_USE_MINISHELL)
#endif /* NLUIF_USE_DESCRIPTION */

/**
 *  @def NLUIF_USE_SYNTAX
 *
 *  @brief
 *    The syntax build feature enables the UIF command syntax UIF field.
 */
#ifndef NLUIF_USE_SYNTAX
#define NLUIF_USE_SYNTAX      (NLUIF_USE_VERBOSE && !NLUIF_USE_MINISHELL)
#endif /* NLUIF_USE_SYNTAX */

/**
 *  @def NLUIF_USE_VOID_FUNCS
 *
 *  @brief
 *    The void functions build feature enables shell function entry
 *    points that have a void return value; otherwise, shell function
 *    entry points return a signed integer.
 */
#ifndef NLUIF_USE_VOID_FUNCS
#define NLUIF_USE_VOID_FUNCS 1
#endif /* NLUIF_USE_VOID_FUNCS */

/**
 *  @def NLUIF_PRINT_RESULT
 *
 *  @brief
 *    Enable the printing of the return code after a command is run,
 *    in the format of "DONE %d"
 */
#ifndef NLUIF_PRINT_RESULT
#define NLUIF_PRINT_RESULT 0
#endif /* NLUIF_PRINT_RESULT */

#if !NLUIF_USE_MINISHELL
/*
 * Maximum command line arguments
 */
#ifndef UIF_MAX_ARGS
#define UIF_MAX_ARGS     29
#endif

/*
 * Maximum length of the command line
 */
#ifndef UIF_MAX_LINE
#define UIF_MAX_LINE    768
#endif

/*
 * Maximum length of the prompt string
 */
#ifndef UIF_MAX_PROMPT
#define UIF_MAX_PROMPT    8
#endif

#else /* !NLUIF_USE_MINISHELL */

/*
 * Maximum command line arguments
 */
#ifndef UIF_MAX_ARGS
#define UIF_MAX_ARGS      3
#endif

/*
 * Maximum length of the command line
 */
#ifndef UIF_MAX_LINE
#define UIF_MAX_LINE    100
#endif

/*
 * Maximum length of the prompt string
 */
#ifndef UIF_MAX_PROMPT
#define UIF_MAX_PROMPT    2
#endif

#endif /* !NLUIF_USE_MINISHELL */

/*
 * Function prototypes
 */
void
uif_set_prompt(const char *s);

void
uif_prompt(void);

int
uif_handle_input_char(int ch);

uint32_t
uif_get_value (const char *, bool *, int);

void
uif_run_cmd (void);

void
uif_run_cmd_string (const char *command);

void
uif_cmd_help (int, char **);

int
uif_cmd_minishell_help (int, char **);

void
uif_cmd_set (int, char **);

void
uif_cmd_show (int, char **);

#define UIF_HISTORY_CNT 5

/*
 * The command table entry data structure
 */

#if NLUIF_USE_VOID_FUNCS
typedef void nl_uif_shell_func_return_t;
#else
typedef int  nl_uif_shell_func_return_t;
#endif /* NLUIF_USE_VOID_FUNCS */

typedef nl_uif_shell_func_return_t (* nl_uif_shell_func_t)(int argc, char **argv);

typedef struct
{
    const char *  cmd;              /* command name user types, ie. GO  */
    nl_uif_shell_func_t func;       /* actual function to call          */
#if !NLUIF_USE_MINISHELL
    int     min_args;               /* min num of args command accepts  */
    int     max_args;               /* max num of args command accepts  */
    int     flags;                  /* command flags (e.g. repeat)      */
#endif /* !NLUIF_USE_MINISHELL */
#if NLUIF_USE_DESCRIPTION
    const char *  description;      /* brief description of command     */
#endif /* NLUIF_USE_DESCRIPTION */
#if !NLUIF_USE_MINISHELL
#if NLUIF_USE_SYNTAX
    const void *  syntax;           /* syntax of command                */
#endif /* NLUIF_USE_SYNTAX*/
#endif /* !NLUIF_USE_MINISHELL */
} UIF_CMD;

typedef UIF_CMD nl_uif_cmd_t;

typedef void (*UIF_SYNTAX_FUNC)(const char *name);

#if NLUIF_USE_DESCRIPTION && NLUIF_USE_SYNTAX
#define DECLARE_UIF_CMD(name, min, max, flags, func, desc, syntax) \
        { name, func, min, max, flags, desc, syntax }
#elif NLUIF_USE_DESCRIPTION && !NLUIF_USE_SYNTAX && !NLUIF_USE_MINISHELL
#define DECLARE_UIF_CMD(name, min, max, flags, func, desc, syntax) \
        { name, func, min, max, flags, desc }
#elif !NLUIF_USE_DESCRIPTION && NLUIF_USE_SYNTAX
#define DECLARE_UIF_CMD(name, min, max, flags, func, desc, syntax) \
        { name, func, min, max, flags, syntax }
#elif NLUIF_USE_MINISHELL && !NLUIF_USE_DESCRIPTION
#define DECLARE_UIF_CMD(name, min, max, flags, func, desc, syntax) \
        { name, func }
#define DECLARE_UIF_MINISHELL_CMD(name, func) \
        DECLARE_UIF_CMD(name, 0, 0, 0, func, NULL, NULL)
#elif NLUIF_USE_MINISHELL && NLUIF_USE_DESCRIPTION
#define DECLARE_UIF_CMD(name, min, max, flags, func, desc, syntax) \
        { name, func, desc }
#define DECLARE_UIF_MINISHELL_CMD(name, func) \
        DECLARE_UIF_CMD(name, 0, 0, 0, func, #func, NULL)
#else
#define DECLARE_UIF_CMD(name, min, max, flags, func, desc, syntax) \
        { name, func, min, max, flags }
#endif /* UIF_USE_DESCRIPTION && UIF_USE_SYNTAX */

/*
 * Prototype and macro for size of the command table
 */
extern const UIF_CMD UIF_CMDTAB[];
extern const int UIF_NUM_CMD;
#define UIF_CMDTAB_SIZE             (sizeof(UIF_CMDTAB)/sizeof(UIF_CMD))

#define UIF_CMD_FLAG_REPEAT         0x1
#define UIF_CMD_FLAG_SYNTAX_FUNC    0x2

/*
 * Macros for User InterFace command table entries
 */
#ifndef UIF_CMD_HELP
#define UIF_CMD_HELP    \
    DECLARE_UIF_CMD("?", 0, 1, 0, uif_cmd_help, "show available commands", ""), \
    DECLARE_UIF_CMD("help", 0, 1, 0, uif_cmd_help, "show available commands", "")
#endif

#ifndef UIF_CMD_SET
#define UIF_CMD_SET \
    DECLARE_UIF_CMD("set", 0, 2, 0, uif_cmd_set, "Set Config", "<option value>")
#endif

#ifndef UIF_CMD_SHOW
#define UIF_CMD_SHOW    \
    DECLARE_UIF_CMD("show", 0, 1, 0, uif_cmd_show, "Show Config", "<option>")
#endif

/*
 * DO NOT USE UIF_MINISHELL_CMD_HELP
 *
 * ...unless you are a simulator. The first rule of 'minishell' is 'be a MINI
 * shell'. Want help?? Get it from a wiki!!
 *
 * Seriously: simulator only.
 */
#ifdef NLUIF_USE_MINISHELL
#ifndef UIF_MINISHELL_CMD_HELP
#define UIF_MINISHELL_CMD_HELP \
    DECLARE_UIF_MINISHELL_CMD("?", uif_cmd_minishell_help), \
    DECLARE_UIF_MINISHELL_CMD("help", uif_cmd_minishell_help)
#endif // UIF_MINISHELL_CMD_HELP
#endif // NLUIF_USE_MINISHELL

/*
 * Macro to include all standard user interface commands
 */
#define UIF_CMDS_ALL    \
    UIF_CMD_HELP,       \
    UIF_CMD_SET,        \
    UIF_CMD_SHOW

/*
 * The set/show table entry data structure
 */
typedef struct
{
    const char *  option;
    int           min_args;
    int           max_args;
    void          (*func)(int, char **);
    const char *  syntax;
} UIF_SETCMD;

/*
 * Prototype and macro for size of the table
 */
extern const UIF_SETCMD UIF_SETCMDTAB[];
extern const int UIF_NUM_SETCMD;
#define UIF_SETCMDTAB_SIZE      (sizeof(UIF_SETCMDTAB)/sizeof(UIF_SETCMD))

/********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* NLUTILITIES_NLUIF_H */
