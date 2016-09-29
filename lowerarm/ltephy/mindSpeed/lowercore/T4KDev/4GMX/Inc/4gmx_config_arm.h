//-------------------------------------------------------------------------------------------
/** @file 4gmx_config_arm.h
 *
 * @brief The ARM's specific configuration used in case of building system
 *        for arm platform
 * @author Mindspeed Technologies
 * @version $Revision: 1.39 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------


#ifndef _4GMX_CONFIG_ARM_H_
#define _4GMX_CONFIG_ARM_H_

/************************************************************************/
/*       4GMX functionality (API)                                       */
/************************************************************************/
#define MX_SCH_SEARCH_FROM_BEGIN
#define MX_MTHREAD_ENABLED
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
//#define MX_TRACER_ENABLED
#define MX_NAMING_ENABLED
#define MX_INTERCPU_ENABLED
#define MX_ARM_SMP_ENABLED
//#define MX_ARM_ERRATA_FIX_ENABLED
#define MX_ARM_MMU_ENABLED
#define MX_HEAP_INTEGRITY
#define MX_HEAP_LEAKAGE_CONTROL
#define MX_EXT_THREAD_TRACER_ENABLED
#define MX_DIRECT_SHARE_OBJ_ACCESS
#define MX_HWSEMA_ENABLED
#define MX_DIRECT_ICPU_IRQ_GENERATING_ENABLED
#define MX_MTHREAD_MANUAL_QUANTUM_CONTROL
#define MX_TIMER_AUTO_LAUNCH

#define MX_ICPU_GEN_REG         (0xFE000000 + 0x400000 + 0x030000 + 0x3C)
#define MX_ICPU_CLR_REG         (0xFE000000 + 0x400000 + 0x030000 + 0x40)

//#define MX_HW_SEMA_L1_SYNC      (0xFBF00000 + 4 * 5)
//#define MX_HW_SEMA_L2_SYNC      (0xFBF00000 + 4 * 6)

#define MX_HW_SEMA_L1_SYNC      (5)
#define MX_HW_SEMA_L2_SYNC      (6)

#define MX_L1_HW_SYNC_ENABLED   
#define MX_L2_HW_SYNC_ENABLED
#define MX_ARM_L2_CACHE_ENABLED
//#define MX_ARM_L2_CACHE_WRITE_THROUGH_ENABLED

#define MX_ARM_L2_BASE          (0xFFF10000)
#define MX_ARM_L2_CODE_PREF     (1 << 29)
#define MX_ARM_L2_DATA_PREF     (1 << 28)
#define MX_ARM_L2_WRITE_ALLOC   (2 << 23)
#define MX_ARM_L2_AUX_CFG       (MX_ARM_L2_CODE_PREF | MX_ARM_L2_DATA_PREF | MX_ARM_L2_WRITE_ALLOC)

#ifdef MX_ARM_ERRATA_FIX_ENABLED

#define MX_ARM_L2_TAG_RAM_READ  (7) // 2 cycles of latency
#define MX_ARM_L2_TAG_RAM_WRITE (7) // 2 cycles of latency
#define MX_ARM_L2_TAG_RAM_SETUP (7) // 2 cycles of latency

#define MX_ARM_L2_DATA_RAM_READ  (7)// 2 cycles of latency
#define MX_ARM_L2_DATA_RAM_WRITE (7)// 2 cycles of latency
#define MX_ARM_L2_DATA_RAM_SETUP (7)// 2 cycles of latency

#else

#define MX_ARM_L2_TAG_RAM_READ  (1) // 2 cycles of latency
#define MX_ARM_L2_TAG_RAM_WRITE (1) // 2 cycles of latency
#define MX_ARM_L2_TAG_RAM_SETUP (1) // 2 cycles of latency

#define MX_ARM_L2_DATA_RAM_READ  (1)// 2 cycles of latency
#define MX_ARM_L2_DATA_RAM_WRITE (1)// 2 cycles of latency
#define MX_ARM_L2_DATA_RAM_SETUP (1)// 2 cycles of latency

#endif

#define MX_ONBOARD_TIMER_ENABLED
#define MX_ONBOARD_TIMER_REG    (0xFE000000 + 0x050000 + 0x04)


#ifdef MX_TRACER_ENABLED

#define MX_TRACE_PART_LR                // to track link register 
#define MX_TRACE_PART_CPU_ID            // to track CPU id where allocation or deallocation is done 
#define MX_TRACE_PART_TIME              // to track time of operation
#define MX_TRACE_PART_FREE_BLOCKS       // to track free blocks 
#define MX_TRACE_PART_PTR               // to track allocated/deallocated blocks pointer

#define MX_TRACE_QUEUE_LR               // to track link register 
#define MX_TRACE_QUEUE_CPU_ID           // to track CPU id where allocation or deallocation is done 
#define MX_TRACE_QUEUE_TIME             // to track time of operation
#define MX_TRACE_QUEUE_FREE_SIZE        // to track the number of free cells in the queue before 'put' operation
#define MX_TRACE_QUEUE_VAL              // to track queue put/get value

#endif

/************************************************************************/
/*         The 4GMX inter-cpu commands configuration                    */
/************************************************************************/
#ifdef MX_INTERCPU_ENABLED

// The thread's inter-cpu commands configuration
// If a macro is defined, the inter-cpu command will be treated like asynchronous

#define MX_IC_THREAD_REMOVE_ASYNC
#define MX_IC_THREAD_SCHEDULE_ASYNC
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
#define MX_IC_TIMER_START_FRAC_ASYNC
#define MX_IC_TIMER_STOP_ASYNC
#define MX_IC_TIMER_SET_EVENT_ASYNC
#define MX_IC_TIMER_SET_PROC

#define MX_IC_SEMA_REMOVE_ASYNC
#define MX_IC_SEMA_SET_ASYNC

#endif //MX_INTERCPU_ENABLED

/************************************************************************/
/*  The 4GMX global parameters (platform-independent)                   */
/************************************************************************/
#define MX_MAX_THREADS          40
#define MX_MAX_MTHREADS         10
#define MX_MAX_SEMA             20
#define MX_MAX_THREAD_MSG       20
#define MX_MAX_EVENTS           40
#define MX_MAX_LISTS            40
#define MX_MAX_PARTITIONS       40
#define MX_MAX_HEAPS            40
#define MX_MAX_RHEAPS           5
#define MX_MAX_DRIVERS          20
#define MX_MAX_TIMERS           40
#define MX_MAX_QUEUES           40
#define MX_MAX_TRACERS          40
#define MX_MAX_DATA_SHARED_OBJ  40
#define MX_MAX_OBJ_NAME         10          /**< The maximum number of ASCII symbols
                                               per object name*/

#define MX_MAX_INTERCPU         4           /**< The maximum number of 4GMX systems
                                               connected to each other (extended
                                               inter-cpu functionality)*/

#define MX_MAX_IC_QUEUE         (32)        /**< The maximum number of
                                                commands that can be placed to
                                                the inter-cpu queue for one 4GMX
                                                instance */

#define MX_INTERCPU_THR_PRIOR   0           /**< The highest system priority,
                                                it can be changed by calling
                                                MxSetInterCpuPriority()      */

#define MX_INTERCPU_IRQ_PRIOR   0           /**< The priority of inter-cpu interrupt
                                                handler */

#define MX_MAX_INTERCPU_TIMEOUT (1000*150*1000) /**< The wating limit of inter-cpu command (1sec)
                                                    processing (in milliseconds)*/

#define MX_MAX_SYS_INIT_TIMEOUT 5000        /**< The timeout of remote system initialization                */
#define MX_SAFE_SECTION_NUM     32          /**< The number of safe sections                                */
#define MX_INT_STATUS_ARRAY     32          /**< The size of array used in IRQ control API (IRQ-off/IRQ-on) */

#define MX_MTHREAD_STACK_SIZE   (4*1024)    /**< The default stack size for the m-thread in bytes           */
#define MX_MAX_MTHREAD_PRIOR    16          /**< The number of m-threads priorities                         */
#define MX_SS_MTHREAD_PRIOR     8           /**< The default priority of SS m-thread                        */
#define MX_MTHREAD_QUANTUM      (0)         /**< The quantum of time after with thread has to be switched   */

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

#if MX_MAX_THREAD_MSG > 0
#define MX_THREAD_MSG_ENABLED
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

#endif /*_4GMX_CONFIG_ARM_H_*/
