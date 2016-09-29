/** @file macMemUtil.c
 *
 * @brief Memory managment routines
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "osal.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "macMemUtil.h"
#include "ltemaccore.h"

void* MacMalloc(U32 size)
{
    U8* p;
    p = malloc(size);
    // TODO: Remove memset
    if (p)
    {
        memset(p, 0, size);
    }
    return p;
}

void MacFree(void ** p)
{
    free(*p);
    *p = NULL;
}

void* MacAllocBlock(U32 size)
{
    U8* p;
    p = malloc(size);
    // TODO: Remove memset
    memset(p, 0, size);
    return p;
}

void MacFreeBlock(void** p)
{
    free(*p);
    *p = NULL;
}
