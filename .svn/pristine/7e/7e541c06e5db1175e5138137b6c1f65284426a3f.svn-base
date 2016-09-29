/** @file timers.c
 *
 * @brief Timers
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_BARE

#include "timers.h"
#include "apb.h"
#include "clkcore.h"
#include "intc.h"
#include "isr.h"

//
// Time tracking
//
V32 CurrentTimeMs = 0;

/*! \brief Timer[A] IRQ Handler
 *
*/
void TimerIntHandler(void)
{
    CurrentTimeMs++;
}

/** Initialize timers
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
RESULTCODE TimerInit(U32 cpuid)
{
    // Clocks per millisecond
    IOWrite32A(TIMER0_CTRL, ClkGetAmbaClock() / 1000);

    // Clocks per microsecond
    IOWrite32A(TIMER1_CTRL, ClkGetAmbaClock() / 1000000);

    // Milliseconds counter (the same as CurrentTimeMs)
    // TODO: Choose single time source
    IOWrite32A(TIMER2_LBOUND, 0);
    IOWrite32A(TIMER2_HBOUND, 0xFFFFFFFF);
    // Chain Timer 2 with Timer 0
    IOWrite32A(TIMER2_CTRL, 1);

    // Microseconds counter
    IOWrite32A(TIMER3_LBOUND, 0);
    IOWrite32A(TIMER3_HBOUND, 0xFFFFFFFF);
    // Chain Timer 3 with Timer 1
    IOWrite32A(TIMER3_CTRL, 1);

    // Enable TimerA mask
    IOWriteOr32A(TIMER_MASK, 0x01);
    IsrAddIrqHandler(INTC_TIMERA, TimerIntHandler);
    IntcClearInt(INTC_TIMERA);
    IntcEnableIrq(cpuid, INTC_TIMERA);

    return SUCCESS;
}

#endif /* USE_BARE */
