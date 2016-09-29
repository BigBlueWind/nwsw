//-------------------------------------------------------------------------------------------
/** @file 4gmx_config_x86.h
 *
 * @brief Intel x86 - specific configuration used in case of building system
 *        for x86 platform
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------


#ifndef _4GMX_CONFIG_X86_H_
#define _4GMX_CONFIG_X86_H_

#ifndef MX_X86_SIM
#define MX_X86_SIM
#endif

/*
 * 4GMX functionality (API)
 */
#undef MX_SCH_SEARCH_FROM_BEGIN
#undef MX_EVENT_ENABLED
#undef MX_LIST_ENABLED
#undef MX_PART_ENABLED
#undef MX_HEAP_ENABLED
#undef MX_RHEAP_ENABLED
#undef MX_DRV_FRAMEWORK_ENABLED
#undef MX_TIMER_ENABLED
#undef MX_QUEUE_ENABLED
#undef MX_QUEUE_OVERFLOW_CHECK_ENABLED
#undef MX_QUEUE_4B_ELMS
#undef MX_NAMING_ENABLED
#undef MX_INTERCPU_ENABLED
#undef MX_ARM_SMP_ENABLED
#undef MX_ARM_MMU_ENABLED
#undef MX_HEAP_INTEGRITY
#undef MX_EXT_THREAD_TRACER_ENABLED
#undef MX_DIRECT_ICPU_IRQ_GENERATING_ENABLED
#undef MX_ARM_L2_CACHE_ENABLED
#undef MX_ONBOARD_TIMER_ENABLED
#undef MX_INTERCPU_ENABLED

/*
 *  4GMX global parameters (platform-independent)
 */
#define MX_MAX_THREADS     40
#define MX_MAX_EVENTS      40
#define MX_MAX_LISTS       40
#define MX_MAX_PARTITIONS  40
#define MX_MAX_HEAPS       40
#define MX_MAX_RHEAPS       5
#define MX_MAX_DRIVERS     20
#define MX_MAX_TIMERS      40
#define MX_MAX_QUEUES      40
#define MX_MAX_DATA_SHARED_OBJ  40
#define MX_MAX_OBJ_NAME         10        /**< The maximum number of ASCII symbols
                                               per object name*/
#define MX_MAX_INTERCPU         10        /**< The maximum number of 4GMX systems
                                               connected to each other (extended
                                               inter-cpu functionality)*/
#define MX_MAX_IC_QUEUE         (30)      /**< The maximum number of
                                                commands that can be placed to
                                                the inter-cpu queue for one 4GMX
                                                instance */
#define MX_INTERCPU_THR_PRIOR   0    /**< The highest system priority,
                                          it can be changed by calling
                                          MxSetInterCpuPriority()      */
#define MX_INTERCPU_IRQ_PRIOR   0    /**< The priority of inter-cpu interrupt
                                          handler */
#define MX_MAX_INTERCPU_TIMEOUT (1000*150*1000) /**< The wating limit of inter-cpu command (1 sec)
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

#endif /*_4GMX_CONFIG_X86_H_*/
