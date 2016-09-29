//-------------------------------------------------------------------------------------------
/** @file 4gmx_intercpu_timer.h
 *
 * @brief The file contains declaration of the timer's inter-cpu
 *        commands parameters
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"

#ifdef MX_INTERCPU_ENABLED

#ifndef _4GMX_INTERCPU_TIMER_H_
#define _4GMX_INTERCPU_TIMER_H_

#include "4gmx_types.h"
#include "4gmx_const.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef MX_TIMER_ENABLED

#include "4gmx_timer.h"

/** @brief "Create timer" command's parameters */

typedef struct
{
    UINT32      nMode;
    UINT32      nPeriod;
    LPCSTR      Name;
    HANDLE      hEvent;
    HANDLE *    Handle;

}MxICTimerCreate;

/** @brief "Remove timer" command's parameters */

typedef struct
{
    HANDLE      Handle;

}MxICTimerRemove;

/** @brief "Start/Stop timer" command's parameters */

typedef struct
{
    HANDLE      Handle;

}MxICTimerStart;

/** @brief "Set timer period" command's parameters */

typedef struct
{
    HANDLE      Handle;
    UINT32      nPeriod;

}MxICTimerSetPeriod;

/** @brief "Set timer proc" command's parameters */

typedef struct
{
    HANDLE      Handle;
    MXTIMERPROC Proc;
    void*       Ptr;

}MxICTimerSetProc;

/** @brief "Get timer period" command's parameters */

typedef struct
{
    HANDLE      Handle;
    PUINT32     pnPeriod;

}MxICTimerGetPeriod;

/** @brief "Set timer event" command's parameters */

typedef struct
{
    HANDLE      Handle;
    HANDLE      Event;

}MxICTimerSetEvent;


#endif /*MX_TIMER_ENABLED*/

#ifdef __cplusplus
}
#endif

#endif /*_4GMX_INTERCPU_TIMER_H_*/

#endif /*MX_INTERCPU_ENABLED */
