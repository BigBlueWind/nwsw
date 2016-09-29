/** @file macMemUtil.h
 *
 * @brief Memory managment routines header file
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef _MAC_MEM_UTIL_
#define _MAC_MEM_UTIL_

#include "basetypes.h"

extern void* MacMalloc(U32 size);
extern void MacFree(void ** p);
extern void* MacAllocBlock(U32 size);
extern void MacFreeBlock(void** p);
extern void MacMemInit(HANDLE heap);

#endif //_MAC_MEM_UTIL_
