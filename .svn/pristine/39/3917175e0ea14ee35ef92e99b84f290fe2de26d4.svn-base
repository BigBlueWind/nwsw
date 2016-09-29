/** @file timers.c
 *
 * @brief Timers
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_LINUX

#include "timers.h"
#include "osal.h"
#include <semaphore.h>
#if defined (C4KEVM)
#include "apb.h"
#endif

#define TIMER_GRAN_MS                   1000
#define TIMER_GRAN_US                   ((TIMER_GRAN_MS)*1000)
#define TIMER_GRAN_NS                   ((TIMER_GRAN_US)*1000)

// APB TIMER0
volatile U32 *Timer0 = NULL;

//
// Time tracking
//
V32 CurrentTimeMs = 0;
int StopTimer = 0;
OSAL_TASK TimerTaskID;
volatile int TimerEnabled = 0;

//
// External dependencies
//
extern OSAL_SEMAPHORE PhyTickSema;

#if defined (C4KEVM)
extern PTR IcpuMapIo(PTR addr, U32 len);
extern void IcpuUnmapIo(PTR addr);
#endif

//
// Forward declarations
//
void TimerTask(PTR data);

/** Initialize timers
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
RESULTCODE TimerInit(U32 cpuid)
{
#if defined (C4KEVM)
    // APB TIMER0
    Timer0 = (V32 *)IcpuMapIo((PTR) TIMER_APB_BASEADDR, 0x1000);
#endif

    return SUCCESS;
}

/** Enables or disables timer events
 *
 * @param enable boolean value, 0 == disable
 */
void TimerEnable(int enable)
{
    TimerEnabled = enable;
}

/**
 *
 * @param data
 */
void TimerTask(PTR data)
{
	struct timespec timeout;

	timeout.tv_sec = 1;
	timeout.tv_nsec = 0;//TIMER_GRAN_NS;

	while (1)
	{
//		if (sem_timedwait(&StopTimer, &timeout) == 0)
	    pselect(0, NULL, NULL, NULL, &timeout, NULL);
	    if (StopTimer)
		{
		    // About to exit
		    break;
		}
		CurrentTimeMs += TIMER_GRAN_MS;
		if (TimerEnabled)
		{
	        // TODO: add task list
		    OsalRaiseSema(&PhyTickSema);
		}
	}
}

/**
 *
 * @param cpuid
 */
RESULTCODE TimerDone(U32 cpuid)
{
#if defined (C4KEVM)
    if (Timer0)
    {
        IcpuUnmapIo((PTR) Timer0);
        Timer0 = NULL;
    }
#endif
    return SUCCESS;
}

#endif /* USE_LINUX */
