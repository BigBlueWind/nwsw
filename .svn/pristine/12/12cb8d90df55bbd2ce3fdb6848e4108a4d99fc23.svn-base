//-------------------------------------------------------------------------------------------
/** @file 4gmx_config.h
 *
 * @brief The file contains macros of 4GMX configuration for every supported platforms
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_CONFIG_H_
#define _4GMX_CONFIG_H_
//#define _MX_DEBUG                 /**< The debug build of 4GMX, _ASSERTs are used, Tracing is used                    */
//#define _APP_ASSERT_HANDLER       /**< An application should implement MxAlertHandle function for alerts processing   */
//#define MX_ARM_ENABLED
//#define MX_CEVA_ENABLED
//#define MX_CREATE_OBJ_LOG_ENABLED

#ifdef _MX_DEBUG
/*
#define TRACE_THR_CREATE
#define TRACE_THR_REMOVE
#define TRACE_THR_SCHEDULE
#define TRACE_THR_UNSCHEDULE
#define TRACE_THR_START
#define TRACE_THR_FINISH
#define TRACE_THR_PRIOR

#define TRACE_EVENT_CREATE
#define TRACE_EVENT_REMOVE
#define TRACE_EVENT_SET
#define TRACE_EVENT_RESET

#define TRACE_TIMER_CREATE
#define TRACE_TIMER_REMOVE
#define TRACE_TIMER_PULSE
#define TRACE_TIMER_START
#define TRACE_TIMER_STOP

#define TRACE_PART_CREATE
#define TRACE_PART_REMOVE
#define TRACE_PART_OPEN
#define TRACE_PART_ALLOC
#define TRACE_PART_FREE

#define TRACE_QUEUE_CREATE
#define TRACE_QUEUE_REMOVE
#define TRACE_QUEUE_OPEN
#define TRACE_QUEUE_PUT
#define TRACE_QUEUE_GET
#define TRACE_QUEUE_EMPTY

#define TRACE_HEAP_CREATE
#define TRACE_HEAP_REMOVE
#define TRACE_HEAP_ALLOC
#define TRACE_HEAP_FREE

#define TRACE_RHEAP_CREATE
#define TRACE_RHEAP_REMOVE
#define TRACE_RHEAP_ALLOC
#define TRACE_RHEAP_FREE

#define TRACE_LIST_CREATE
#define TRACE_LIST_REMOVE
#define TRACE_LIST_ADD_ELM
#define TRACE_LIST_DEL_ELM
#define TRACE_LIST_EMPTY
#define TRACE_LIST_INSERT
*/
#else

// Checking the correctness of macros definition

#if defined(TRACE_THR_CREATE)       ||\
    defined(TRACE_THR_REMOVE)       ||\
    defined(TRACE_THR_SCHEDULE)     ||\
    defined(TRACE_THR_UNSCHEDULE)   ||\
    defined(TRACE_THR_START)        ||\
    defined(TRACE_THR_FINISH)       ||\
    defined(TRACE_THR_PRIOR)        ||\
    defined(TRACE_EVENT_CREATE)     ||\
    defined(TRACE_EVENT_REMOVE)     ||\
    defined(TRACE_EVENT_SET)        ||\
    defined(TRACE_EVENT_RESET)      ||\
    defined(TRACE_TIMER_CREATE)     ||\
    defined(TRACE_TIMER_REMOVE)     ||\
    defined(TRACE_TIMER_PULSE)      ||\
    defined(TRACE_TIMER_START)      ||\
    defined(TRACE_TIMER_STOP)       ||\
    defined(TRACE_PART_CREATE)      ||\
    defined(TRACE_PART_REMOVE)      ||\
    defined(TRACE_PART_OPEN)        ||\
    defined(TRACE_PART_ALLOC)       ||\
    defined(TRACE_PART_FREE)        ||\
    defined(TRACE_QUEUE_CREATE)     ||\
    defined(TRACE_QUEUE_REMOVE)     ||\
    defined(TRACE_QUEUE_OPEN)       ||\
    defined(TRACE_QUEUE_PUT)        ||\
    defined(TRACE_QUEUE_GET)        ||\
    defined(TRACE_QUEUE_EMPTY)      ||\
    defined(TRACE_HEAP_CREATE)      ||\
    defined(TRACE_HEAP_REMOVE)      ||\
    defined(TRACE_HEAP_ALLOC)       ||\
    defined(TRACE_HEAP_FREE)        ||\
    defined(TRACE_RHEAP_CREATE)     ||\
    defined(TRACE_RHEAP_REMOVE)     ||\
    defined(TRACE_RHEAP_ALLOC)      ||\
    defined(TRACE_RHEAP_FREE)       ||\
    defined(TRACE_LIST_CREATE)      ||\
    defined(TRACE_LIST_REMOVE)      ||\
    defined(TRACE_LIST_ADD_ELM)     ||\
    defined(TRACE_LIST_DEL_ELM)     ||\
    defined(TRACE_LIST_EMPTY)       ||\
    defined(TRACE_LIST_INSERT)

#error "The _MX_DEBUG macro should be defined!"

#endif

#endif /*_MX_DEBUG*/

/***********************************************************/
#ifdef MX_ARM_ENABLED
/***********************************************************/

#include "4gmx_config_arm.h"

#define MX_MAX_INTERRUPTS  160      /**< The maximum number of interrupt vectors supported by the system */

//#define MX_ARM_MMU_ENABLED        /**< ARM MMU block     */

#define MX_ARM_MPCORE_ENABLED       /**< ARM MPCORE functionality   */

#endif /*#ifdef MX_ARM_ENABLED */

/***********************************************************/
#ifdef MX_CEVA_ENABLED
/***********************************************************/

#include "4gmx_config_ceva.h"

#define MX_MAX_INTERRUPTS  160       /**< The maximum number of interrupt vectors supported by the system */

#endif /*#ifdef MX_CEVA_ENABLED */

#endif /*_4GMX_CONFIG_H_*/
