/** @file nulltask.c
 *
 * @brief Null task for bare metal
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_BARE

#include "basetypes.h"
#include "sysctrl.h"
#include "osal.h"

OSAL_SEMAPHORE NullTaskSema;

void NullTask0(void)
{
    OsalCreateSema(&NullTaskSema);
    while (1)
    {
        if (OsalWaitForSema(&NullTaskSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            break;
        }
        __nop();
    }
    OsalDestroySema(&NullTaskSema);
}

void NullTask1(void)
{
    while (1)
    {
        __nop();
    }
}

void NullTask(U32 cpuid)
{
    if (cpuid == UARM)
        NullTask0();
    else if (cpuid == LARM)
        NullTask1();
}

void NullTaskStop(void)
{
}

#endif /* USE_BARE */
