//-------------------------------------------------------------------------------------------
/** @file rtconfig.h
 *
 * @brief File containes configuration information of LTERT application
 * @author Mindspeed Technologies
 * @version $Revision: 1.36 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------
#ifndef _LTERTCONFIG_H_
#define _LTERTCONFIG_H_

//#define PHY_SMP_ENABLED

#ifdef PHY_SMP_ENABLED

#define PHY_SMP_THREAD      SCHED_SMP
#define PHY_EVENT_SCHED_SMP THREAD_EVENT_SMP

#else

#define PHY_SMP_THREAD      0
#define PHY_EVENT_SCHED_SMP 0

#endif

#define PHY_SCHEDULE_THREAD_ARM        0
#define API_DISPATCH_THREAD_ARM        0
#define PRACH_DISPATCH_THREAD_ARM      0
#define SRS_DISPATCH_THREAD_ARM        0
#define API_COPY_THREAD_ARM            0
#define PHY_STATS_STREAM_THREAD_ARM    1

#define FIFO_MAC_TO_PHY_API                0
#define FIFO_PHY_LOG_AND_CLEAN_API         1


#endif /*_LTERTCONFIG_H_*/
