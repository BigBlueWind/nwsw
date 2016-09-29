/** @file timer.c
 *
 * @brief Timers implementation for Win32
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_WINDOWS

#include "osal.h"

//
// Time tracking
//
V32 CurrentTimeMs = 0;

/** Initialize timers
 *
 * @param cpuid Current CPU where this function called
 * @return Standard result code
 */
RESULTCODE TimerInit(U32 cpuid)
{
    return SUCCESS;
}

RESULTCODE TimerDone(U32 cpuid)
{
    return SUCCESS;
}

#endif /* USE_WINDOWS */
