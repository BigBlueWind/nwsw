//-------------------------------------------------------------------------------------------
/** @file 4gmx_mkernel.h
 *
 * @brief The file contains types and multi-stack threads API declaration
 * @author Mindspeed Technologies
 * @version $Revision: 1.2 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_MKERNEL_H_
#define _4GMX_MKERNEL_H_

#include "4gmx_config.h"
#include "4gmx_types.h"
#include "4gmx_const.h"

#ifdef MX_MTHREAD_ENABLED

#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************/
/*     Module error codes                                               */
/************************************************************************/
#define MX_MKERNEL_THREAD_ERROR             DEF_MXRC(MXM_MKERNEL,  1)

/************************************************************************/
/*     MxWaitForSingleObject  return status                             */
/************************************************************************/
#define WAIT_OBJECT_0                       0x00000000L     // The state of the specified object is signaled
#define WAIT_TIMEOUT                        0x00000102L     // The time-out interval elapsed, and the object's state is nonsignaled

/** @brief This function initializes M-KERNEL and automatically called 
           by the OS kernel, the main purpose of this thread is to create SS thread 

    @return [MXRC] an error code */

MXRC MxMKernelInit (void);

void MxiRemoveThreadSwStat(PMTHREAD pThr);

/** @brief This function sets a pointer to the application handler 
           that will be called to process interrupts 

    @return [MXRC] an error code*/

MXRC MxSetAppIrqHandler (void (*AppIrqHandler)(void));

/** @brief 4GMX ARM IRQ handler, it has to be called to process threads switching 
           from IRQ mode */

void MxIrqHandler (void);

/** @brief This function is designed to process m-threads of sleep list
           and to put these threads to exec list if sleep time is over

*/           
void MxMProcSleepList (void);

/** @brief This function processes situation when SS m-thread has nothing to do,
           ans OS needs to switch to another thread or to call application idle handler 

    @param idle     [in] - the application IDLE handler 
    @param pParam   [in] - the idle parameter 

    @return [MXRC] an error code */

MXRC MxSwitchSSThread (TENTRY idle, LPVOID pParam);

/** @brief This function has to be called by system IRQ handler to provide the possibility 
           to switch threads context from IRQ handlers, if this function is not called 
           the threads context remains the same and switching can be initiated by m-threads 
*/           

void MxHandleMThreadQuantums(void);

/** @brief This function schedules SS m-thread to process S-threads

    @return [MXRC] an error code */

MXRC MxScheduleSSThread(void);

/** @brief This function switches context of the current thread by finding 
           another highest priority thread

    @return [MXRC] an error code */

MXRC MxSwitchContext (void);

MXRC MxSwitchContextEx (PMTHREAD pThr2);

/** @brief This function removes current thread and switches context of the current thread by finding 
           another highest priority thread

    @return [MXRC] an error code */

MXRC MxCutAndSwitchCurThread (void);

/** @brief This function is designed to schedule m-thread and internally 
           called by MxScheduleThread. The thread is putted to the list of 
           executed threads and context switching is called. The next 
           high priority thread takes processor control

    @param h [in] - the m-thread handle 

    @return [MXRC] an error code */

MXRC MxScheduleMThread (HANDLE h);

/** @brief This function is designed to un-schedule m-thread and internally 
           called from MxUnScheduleThread. The thread is removed from the list of 
           executed threads and context switching is called. The next 
           high priority thread takes processor control

    @param h [in] - the m-thread handle 

    @return [MXRC] an error code */

MXRC MxUnScheduleMThread (HANDLE h);

/** @brief This function removes the thread from the list of executed threads 
           and prevents thread execution till Resume event

    @param h [in] - the handle of the m-thread 

    @return [MXRC] an error code */

MXRC MxSuspendMThread (HANDLE h);

/** @brief This function adds the thread to the list of executed threads 
           and allows thread execution

    @param h [in] - the handle of the m-thread 

    @return [MXRC] an error code */

MXRC MxResumeMThread (HANDLE h);

/** @brief This function returns handle of current M-thread 
           executed on the processor

    @return [HANDLE]*/

HANDLE MxGetCurrentMThread (void);

/** @brief This function returns a pointer to the m-thread designed to handle S-threads
    @return [PMTHREAD] the pointer to the m-thread context*/

PMTHREAD MxMGetSSThread (void);

/** @brief This function waits for the object (event) specified by the handler
           if timeout is 0 - the function checks status of the object and returns immediately
           if timeout is INFINITE - the thread is removed from the execution and will be resumed when event is occured

    @param h [in] - the handle of SEMA, see SEMA API
    @param us[in] - the time of waiting in microseconds 

    @return [UINT32] the status , see: WAIT_xxxx */
           
UINT32 MxWaitForSingleObject (HANDLE h, UINT32 us);

/** @brief This function puts the thread to a sleep list and thread 
           will be putted to the executed list again when waiting time is over 

    @param ticks [in] - the time in ticks 

    @return [MXRC] an error code */
    
MXRC MxSleep (UINT32 ticks);

#endif // MX_MTHREAD_ENABLED

#ifdef __cplusplus
}
#endif

#endif // _4GMX_MKERNEL_H_

