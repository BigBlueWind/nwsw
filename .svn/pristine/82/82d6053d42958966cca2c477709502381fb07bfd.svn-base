/** @file timers.h
 *
 * @brief Timer interfaces
 * @author Mindspeed Technologies
 * @version $Revision: 1.13 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "basetypes.h"
#include "resultcodes.h"

#if defined (USE_BARE) || defined (USE_RTXC)

#include "apb.h"
#include "portio.h"

extern V32 CurrentTimeMs;

/*! \brief Read microseconds counter
 *
*/
static inline U32 CurrentTimeUs()
{
    return IORead32A(TIMER3_CURR_COUNT);
}

#endif /* USE_BARE || USE_RTXC */

RESULTCODE TimerInit(U32 cpuid);
RESULTCODE TimerDone(U32 cpuid);
void TimerEnable(int enable);

#if defined (C4KEVM)

// TIMER0 on APB bus
extern V32 *Timer0;

#ifdef __GNUC__
static
#endif
__inline U32 Timer0GetCntr(void)
{
    // TIMER0_CURR_COUNT (virtual address)
    return *(Timer0 + 1);
}

#endif /* C4KEVM */

#endif /*TIMERS_H_*/
