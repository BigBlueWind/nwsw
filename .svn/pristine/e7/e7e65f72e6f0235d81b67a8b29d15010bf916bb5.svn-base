//-------------------------------------------------------------------------------------------
/** @file schtaskdrv.h
 *
 * @brief This file is a header of the scheduler task driver designed
*       to be used by the scheduler to run DSP tasks on ARM processor(s)
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _SCH_TASK_DRV_H_
#define _SCH_TASK_DRV_H_

#include "4gmx.h"
#include "appids.h"
#include "hal.h"
#include "tcb.h"
#include "sirqassign.h"
#include "heaps.h"

/****************************************************************************
*              Driver error codes                                           *
****************************************************************************/
#define STD_RC_OK MX_OK
#define STD_RC_NOT_MEMORY       DEF_USRRC(APP_DRIVER, SCH_TASK_DRV, 1)
#define STD_RC_UNSUPPORTED_CPU  DEF_USRRC(APP_DRIVER, SCH_TASK_DRV, 2)
#define STD_RC_PARAM_ERROR      DEF_USRRC(APP_DRIVER, SCH_TASK_DRV, 3)
#define STD_RC_LOOKUP_TABLE_ERR DEF_USRRC(APP_DRIVER, SCH_TASK_DRV, 4)
/****************************************************************************
*              Default driver initialization parameters                     *
****************************************************************************/
#define STD_CPUS                    CPU_NUM         // the number of CPU supported by the driver

//#define STDrv_DEBUG

/****************************************************************************
*                     Driver types declaration                              *
****************************************************************************/

/**@brief The type of scheduler notifier (announcer), this type defines the function
          to be called in IRQ mode and notify the scheduler TCB block has been processed

   @param pTask [in] - the pointer to the task context */

typedef void (*STDNotifier) (LPVOID pDrvCtx, LPVOID pParam);

/** @brief The type describes task handler used to process task with some task ID */

typedef struct
{
    void  (*TaskHandler)(TCB*);

}STDHANDLER;


/** @brief The driver context */

typedef struct
{
    UINT32        MasterCpuID;
    UINT32        SupportedCpus;
    UINT32        TableSize;
    STDHANDLER  * LookupTable;
    STDNotifier   Notifier;
    LPVOID        NotifierPtr;

    HANDLE        hThread [STD_CPUS];
    HANDLE        MasterEventHandler;

    TCB *         RTaskBegin; // This Queue contains pointers of the processed tasks
    TCB *         RTaskEnd; // This Queue contains pointers of the processed tasks

    TCB *         PTaskBegin[STD_CPUS];  // This Queue contains pointers of the tasks to be processed
    TCB *         PTaskEnd[STD_CPUS];  // This Queue contains pointers of the tasks to be processed

}STDCTX;

/****************************************************************************
*   This interface is implemented to be used by the scheduler               *
****************************************************************************/

/** @brief This function should be called by the application/scheduler
           to allocate the tasks table in run-time mode

    @param nElmNum   [in] - the number of elements in the task table
    @param bSaveData [in] - if TRUE - the data from the previous table
                            will be copied to the new table

    @return [MXRC] an error code */

MXRC STDrvAllocTable (UINT32 nElmNum, BOOL bSaveData);

/** @brief This function registers new task in the task table, the name of function
           is compatible with scheduler definition

    @param nTaskID [in] - the task ID, see: tcb.h file
    @param fnptr   [in] - the pointer to the task handler

    @return [MXRC] an error code of operation */

UINT32 STDrvRegARMTask (UINT32 nTaskID,  void (*fnptr)(TCB*));

/** @brief The function runs task in ARM supervisor mode, this function is
           designed to be called in ARM IRQ mode

	@param nDstCpu [in] - the destination CPU (0 or 1)
    @param pTcb    [in] - the task descriptor

    @return [MXRC] an error code of operation */

MXRC STDrvRunTask (UINT32 nDstCpu, TCB * pTcb);

/** @brief This function registers callback handler, the callback handler will
           be called in ARM IRQ mode

    @param proc [in] - the pointer to the function, it can be NULL

    @return [MXRC] an error code*/

MXRC STDrvRegNotifier (STDNotifier proc, LPVOID pParam);

TCB * STDGetTcb (STDCTX * pCtx, BOOL bReadyList);

BOOL STDIsScheduled(STDCTX * pCtx);

INLINE TCB * STDGetReadyTcb (LPVOID pDrvCtx)
{
    extern VUINT32 stdlock_ready;
    STDCTX * pCtx = (STDCTX *) pDrvCtx;
    TCB * p;
    IRQTYPE irq = ARM_INT_disable ();
    MxGetLock (&stdlock_ready);

    if (pCtx->RTaskBegin != NULL)
    {
        p = pCtx->RTaskBegin;

        pCtx->RTaskBegin = p->NextTcb;

        if (pCtx->RTaskBegin == NULL)
            pCtx->RTaskEnd = NULL;

        p->NextTcb = NULL;
    }
    else
    {
        p = NULL;
    }

    MxReleaseLock (&stdlock_ready);
    ARM_INT_restore (irq);

    return p;
}

/** @brief This function sets master cpu id, master id is an id
           where completed tasks are processed

    @param nMasterCpu [in] - master cpu id [0 ... N]

    @return [MXRC] an error code */

MXRC STDrvSetMasterCpu (UINT32 nMasterCpu);

/****************************************************************************
*   This interface is implemented to make the driver compatible with UDI    *
*   and have driver initialization automatically                            *
****************************************************************************/

MXRC STDrv_init(UINT32 nInit, LPVOID * pStorageDrvCtx);

INLINE MXRC STDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData,
                     UINT32 nCfgDataSize) { return MX_OK; };

INLINE MXRC STDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                    UINT32 nCmdDataSize) { return MX_OK; };

INLINE MXRC STDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                   UINT32 * pnCmdDataSize) { return MX_OK; };

#endif  // _SCH_TASK_DRV_H_


#ifdef __cplusplus
}
#endif

