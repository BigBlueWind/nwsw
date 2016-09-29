//-------------------------------------------------------------------------------------------
/** @file 4gmx_sema.h
 *
 * @brief The file contains types and SEMAPHORE API definitions
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------


#ifndef _4GMX_SEMA_H_
#define _4GMX_SEMA_H_

#include "4gmx_config.h"
#include "4gmx_types.h"
#include "4gmx_const.h"
#include "4gmx_mthread.h"

#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************/
/*     Module error codes                                               */
/************************************************************************/
#define MX_SEMA_ALLOC_ERROR             DEF_MXRC(MXM_SEMA,  1)      // SEMA allocate context error 
#define MX_SEMA_ASSIGN_NAME_ERROR       DEF_MXRC(MXM_SEMA,  2)      // 

/************************************************************************/
/*    The semaphore control flags                                       */
/************************************************************************/
#define SEMA_ALLOCATED                  (1 << 0)
#define SEMA_NAMED                      (1 << 1)

#define MARK_SEMA_ALLOC(thr)            ((thr)->CtrlFlags |= SEMA_ALLOCATED)
#define MARK_SEMA_DEALLOC(thr)          ((thr)->CtrlFlags &= ~SEMA_ALLOCATED)
#define IS_SEMA_ALLOC(thr)              ((thr)->CtrlFlags & SEMA_ALLOCATED)

#define MARK_SEMA_NAMED(thr)            ((thr)->CtrlFlags |= SEMA_NAMED)
#define MARK_SEMA_UNNAMED(thr)          ((thr)->CtrlFlags &= ~SEMA_NAMED)
#define IS_SEMA_NAMED(thr)              ((thr)->CtrlFlags & SEMA_NAMED)


/************************************************************************/
/*    The sema context definition                                       */
/************************************************************************/

typedef struct _MXSEMA_
{
    UINT32      Value;                  // Current value of the sema 
    UINT32      CtrlFlags;              // The object control flags: see MTCF_xxx

    UINT32      ThrNum;
    PMTHREAD    ThrList;                // The list of threads are waiting for this sema

}MXSEMA, *PMXSEMA;

/** @brief This function creates semaphore 

    @param nInitVal     [in]    - the initial value for semaphore (0 - means locked by default) 
    @param pName        [in]    - object name (it can be NULL)
    @param pOutHandle   [out]   - the thread handle

    */
MXRC MxCreateSema (UINT32 nInitVal, const char * pName, HANDLE * pOutHandle);

/** @brief The function creates semaphore object on remote system.

    @param cpu        [in] - the cpu ID
    @param nInitVal   [in]    - the initial value for semaphore (0 - means locked by default) 
    @param name       [in] - the object name (it can be NULL)
    @param pOutHandle [out] - the handle of created object

    @return [MXRC] error code

    \ingroup group_lte_4gmx_event
*/
MXRC MxCreateSemaEx(CPUID cpu, UINT32 nInitVal, const char * pName, HANDLE * pOutHandle);

/** @brief This function removes semaphore and releases all waited threads 

    @param hSema [in] - the semaphore handle 

    @return [MXRC] an error code */

MXRC MxRemoveSema (HANDLE hSema);

/** @brief This function sets sema to some value and sema can be used 
           by the threads that are waiting for this sema
           
    @param h        [in] - the thread handler 
    @param nVal     [in] - new value for the sema

    @return [MXRC] an error code */

MXRC MxSetSema (HANDLE h, UINT32 nVal);

/** @brief This function locks sema and tries to get access to the resource 
           if no access the thread will be waiting 
           
    @param h        [in] - the thread handler 

    @return [MXRC] an error code */

MXRC MxLockSema (HANDLE h);

/** @brief This function locks sema and tries to get access to the resource 
           if no access the thread will be waiting 
           
    @param h        [in] - the thread handler 
    @param us       [in] - the time in microsecords 

    @return [MXRC] an error code */
    
MXRC MxLockSemaEx(HANDLE h, UINT32 us);

/** @brief This function unlocks sema and provides access to the resource 

    @param h        [in] - the thread handler 

    @return [MXRC] an error code */

MXRC MxUnlockSema (HANDLE h);

#ifdef __cplusplus
}
#endif

#endif

