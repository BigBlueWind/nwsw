//-------------------------------------------------------------------------------------------
/** @file 4gmx_config_ceva.h
 *
 * @brief The CEVA's specific configuration used in case of building system
 *        for arm platform
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_CONFIG_CEVA_H_
#define _4GMX_CONFIG_CEVA_H_

/************************************************************************/
/*       4GMX functionality (API)                                       */
/************************************************************************/

#define MX_SCH_SEARCH_FROM_BEGIN

#define MX_EVENT_ENABLED

#define MX_LIST_ENABLED

#define MX_PART_ENABLED

#define MX_HEAP_ENABLED

#define MX_RHEAP_ENABLED

#define MX_DRV_FRAMEWORK_ENABLED

#define MX_TIMER_ENABLED

#define MX_QUEUE_ENABLED

#define MX_QUEUE_OVERFLOW_CHECK_ENABLED

#define MX_QUEUE_4B_ELMS

#define MX_NAMING_ENABLED

//#define MX_INTERCPU_ENABLED

/************************************************************************/
/*         The 4GMX inter-cpu commands configuration                    */
/************************************************************************/
#ifdef MX_INTERCPU_ENABLED

// The thread's inter-cpu commands configuration
// If a macro is defined, the inter-cpu command will be treated like asynchronous

#define MX_IC_THREAD_REMOVE_ASYNC
//#define MX_IC_THREAD_SCHEDULE_ASYNC
#define MX_IC_THREAD_SCHEDULE_EX_ASYNC
#define MX_IC_THREAD_UNSCHEDULE_ASYNC
#define MX_IC_THREAD_SCHEDULE_BY_EVENT_ASYNC
#define MX_IC_THREAD_UNSCHEDULE_BY_EVENT_ASYNC
#define MX_IC_THREAD_SETPARAM_ASYNC
#define MX_IC_THREAD_SETPRIOR_ASYNC

// The event's inter-cpu commands configuration
// If a macro is defined, the inter-cpu command will be treated like asynchronous

#define MX_IC_EVENT_REMOVE_ASYNC
#define MX_IC_EVENT_SET_ASYNC
#define MX_IC_EVENT_RESET_ASYNC

// The timer's inter-cpu commands configuration
// If a macro is defined, the inter-cpu command will be treated like asynchronous

#define MX_IC_TIMER_REMOVE_ASYNC
#define MX_IC_TIMER_SETPERIOD_ASYNC
#define MX_IC_TIMER_START_ASYNC
#define MX_IC_TIMER_STOP_ASYNC
#define MX_IC_TIMER_SET_EVENT_ASYNC

#endif //MX_INTERCPU_ENABLED

/************************************************************************/
/*  The 4GMX global parameters (platform-independent)                   */
/************************************************************************/
#define MX_MAX_THREADS     40

#define MX_MAX_EVENTS      40

#define MX_MAX_LISTS       40

#define MX_MAX_PARTITIONS  40

#define MX_MAX_HEAPS       40

#define MX_MAX_RHEAPS      5

#define MX_MAX_DRIVERS     20

#define MX_MAX_TIMERS      40

#define MX_MAX_QUEUES      40

#define MX_MAX_DATA_SHARED_OBJ  10

#define MX_MAX_OBJ_NAME         10        /**< The maximum number of ASCII symbols
                                               per object name*/

#define MX_MAX_INTERCPU         4        /**< The maximum number of 4GMX systems
                                               connected to each other (extended
                                               inter-cpu functionality)*/

#define MX_MAX_IC_QUEUE         (MX_MAX_INTERCPU * 4) /**< The maximum number of
                                                commands that can be placed to
                                                the inter-cpu queue for one 4GMX
                                                instance */

#define MX_INTERCPU_THR_PRIOR   0    /**< The highest system priority,
                                          it can be changed by calling
                                          MxSetInterCpuPriority()      */

#define MX_INTERCPU_IRQ_PRIOR   0    /**< The priority of inter-cpu interrupt
                                          handler */

#define MX_MAX_INTERCPU_TIMEOUT 5000 /**< The wating limit of inter-cpu command
                                          processing (in milliseconds)*/

#define MX_MAX_SYS_INIT_TIMEOUT 5000 /**< The timeout of remote system initialization */

#ifndef MX_EVENT_ENABLED
#undef MX_MAX_EVENTS
#define MX_MAX_EVENTS  0
#endif

#ifndef MX_LIST_ENABLED
#undef MX_MAX_LISTS
#define MX_MAX_LISTS  0
#endif

#ifndef MX_PART_ENABLED
#undef MX_MAX_PARTITIONS
#define MX_MAX_PARTITIONS 0
#endif

#ifndef MX_HEAP_ENABLED
#undef MX_MAX_HEAPS
#define MX_MAX_HEAPS  0
#endif

#ifndef MX_TIMER_ENABLED
#undef MX_MAX_TIMERS
#define MX_MAX_TIMERS  0
#endif

#ifndef MX_QUEUE_ENABLED
#undef MX_MAX_QUEUES
#define MX_MAX_QUEUES  0
#endif

#define MX_MAX_NAMETABLE_SIZE (MX_MAX_THREADS  + \
        MX_MAX_EVENTS  + \
        MX_MAX_LISTS  + \
        MX_MAX_HEAPS  + \
        MX_MAX_QUEUES  + \
        MX_MAX_PARTITIONS + \
        MX_MAX_TIMERS  + \
        MX_MAX_DRIVERS  + \
        0)

/************************************************************************/
/*     Checking the correctness of conditional compilation              */
/************************************************************************/

#ifdef MX_TIMER_ENABLED

#ifndef MX_EVENT_ENABLED
#error "The timers cannot be used without event objects, please define MX_EVENT_ENABLED macro"
#endif

#endif

#endif /*_4GMX_CONFIG_CEVA_H_*/
