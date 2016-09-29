/** @file console.h
 *
 * @brief Console command interpreter
 * @author Mindspeed Technologies
 * @version $Revision: 1.5 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "resultcodes.h"

//
// Limits
//
#define CON_MAXVAR              256         // Maximum number of registered variables

//
// Variable types
//
typedef enum VarType
{
    VAR_UNKNOWN, VAR_U8, VAR_U16, VAR_U32, VAR_U64, VAR_STR, VAR_PTR
} VARTYPE;

RESULTCODE ConInit(void);
// TODO: Add cleanup
RESULTCODE ConRegisterVar(char *name, VARTYPE type, PTR varptr, OSAL_SIZE bufsize);
RESULTCODE ConDispatch(char ch);
RESULTCODE ConPrompt(U32 cpuid);

#endif /* CONSOLE_H_ */
