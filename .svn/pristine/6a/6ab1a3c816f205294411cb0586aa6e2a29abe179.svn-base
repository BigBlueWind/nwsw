//-------------------------------------------------------------------------------------------
/** @file 4gmx_intercpu_thread.h
 *
 * @brief The file contains declaration of the thread's inter-cpu
 *        commands parameters
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------


#include "4gmx_config.h"

#ifdef MX_INTERCPU_ENABLED

#ifndef _4GMX_INTERCPU_THREAD_H_
#define _4GMX_INTERCPU_THREAD_H_

#include "4gmx_types.h"
#include "4gmx_const.h"

#ifdef __cplusplus
extern "C"
{
#endif

/** @brief This type describes parameters of "create thread" command*/

typedef struct
{
    TENTRY      Proc;       // An antry point to the procedure
    LPCSTR      Name;       // The name of thread
    LPVOID      Param;      // The pointer to the parameters
    TPRIORITY   Prior;      // The procedule priopity
    HANDLE   *  Handle;     // The output handle of created thread

}MxICThrCreate;


#ifdef MX_MTHREAD_ENABLED

/** @brief This type describes parameters of "create thread" command*/

typedef struct
{
    TENTRY      Proc;       // An antry point to the procedure
    LPCSTR      Name;       // The name of thread
    LPVOID      Param;      // The pointer to the parameters
    UINT32      Prior;      // The thread priopity
    UINT32      flag;       // The initial thread state  
    LPVOID      pStack;     // The thread stack 
    UINT32      nStackSize; // The thread stack size in bytes 
    HANDLE   *  Handle;     // The output handle of created thread

}MxICMThrCreate;

#endif // MX_MTHREAD_ENABLED

/** @brief This type describes parameters of "remove thread" command*/

typedef struct
{
    HANDLE      Handle;     // The thread handle
}MxICThrRemove;


/** @brief This type describes parameters of "schedule/unschedule thread" command*/

typedef struct
{
    HANDLE      Handle;     // The thread handle
}MxICThrSchedule;

typedef struct
{
    HANDLE      Handle;     // The thread handle
    TPRIORITY   Prior;
    UINT32      SchedCtrl;

}MxICThrScheduleEx;

typedef struct
{
    TENTRY      Entry;      // The thread entry
    LPVOID      Param;      // The thread parameter

}MxICThrInjectProc;

typedef struct
{
    HANDLE      Handle;     // The thread handle

}MxICThrInjectHandle;

/** @brief Schedule thread by event command */

typedef struct
{
    UINT32      Mode;

    HANDLE      ThrHandle;     // The thread handle
    HANDLE      EventHandle;   // The thread handle

}MxICThrSchByEvent;

/** @brief This type describes parameters of "set/get thread param" command*/

typedef struct
{
    HANDLE      Handle;     // The thread handle
    LPVOID      Param;      // In/Out buffer for thread parameter

}MxICThrSetParam;

/** @brief This type describes parameters of "set/get thread param" command*/

typedef struct
{
    HANDLE      Handle;     // The thread handle
    LPVOID  *   Param;      // Out buffer for thread parameter

}MxICThrGetParam;


/** @brief This type describes parameters of "get thread ret val" command*/

typedef struct
{
    HANDLE      Handle;     // The thread handle
    PUINT32     Ret;        // Out: buffer for thread parameter

}MxICThrRet;

/** @brief Thread priority setting*/

typedef struct
{
    HANDLE      Handle;     // The thread handle
    TPRIORITY   Prior;      // New thread priority

}MxICThrSetPrior;

/** @brief Thread priority getting*/

typedef struct
{
    HANDLE      Handle;     // The thread handle
    TPRIORITY * Prior;      // Thread priority

}MxICThrGetPrior;

#ifdef __cplusplus
}
#endif

#endif /*_4GMX_INTERCPU_THREAD_H_*/

#endif /*MX_INTERCPU_ENABLED */
