//-------------------------------------------------------------------------------------------
/** @file 4gmx_intercpu.c
 *
 * @brief The file contains implementation of 4GMX inter-cpu mechanism
 * @author Mindspeed Technologies
 * @version $Revision: 1.24 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"

#ifdef MX_INTERCPU_ENABLED

#include "4gmx.h"
#include "4gmx_intercpu.h"
#include "4gmx_serv.h"
#include "4gmx_const.h"
#include "4gmx_serv.h"
#include "4gmx_arm_types.h"

/** @brief The function processes kernels specific inter-cpu commands: <B>MXIC_KERNEL_CLASS</B>

    @param pCtx   [in] - the pointer to the system context
    @param pSrcCpu[in] - the IC context of requiring CPU
    @param pICCmd [in] - the IC command

    @return [MXRC] error code

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxICProcKernelCmd (PMXCONTEXT pCtx, MXICCTX * pSrcCpu, MXICCMD * pICCmd)
{
    MXRC rc = MX_OK;
    HANDLE h;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pICCmd);

    switch (pICCmd->CmdID)
    {
        case MXIC_KERNEL_GETOBJ:
        {
            MxICKrnGetObj * pGetObj = &pICCmd->Param.KrnGetObj;

            _ASSERT_PTR(pGetObj->Name);
            _ASSERT_PTR(pGetObj->Handle);

            #ifndef MX_ARM_SMP_ENABLED
            ICMxiCacheInvalid(pGetObj->Name,
                              ROUND((Mxistrlen (pGetObj->Name) + 1), 4));
            #endif

            h = MxiGetObjByName(pCtx, pGetObj->Name);

            *pGetObj->Handle = h;

            #ifndef MX_ARM_SMP_ENABLED
            ICMxiCacheClean(pGetObj->Handle, sizeof (HANDLE));
            #endif

            break;
        }

        case MXIC_KERNEL_SETOBJ_NAME:
        {
            MxICKrnSetObjName * pSetObjName = &pICCmd->Param.KrnSetObjName;

            _ASSERT_HANDLE(pSetObjName->Handle);

            if (pSetObjName->Name != NULL)
            {
                #ifndef MX_ARM_SMP_ENABLED
                ICMxiCacheInvalid (pSetObjName->Name,
                                 ROUND(Mxistrlen (pSetObjName->Name) + 1, 4));
                #endif
            }

            rc = MxSetObjName (pSetObjName->Handle, pSetObjName->Name);

            break;
        }

        case MXIC_KERNEL_GETOBJ_NAME:
        {
            MxICKrnGetObjName * pGetObjName = &pICCmd->Param.KrnGetObjName;
            LPCSTR pName;

            _ASSERT_PTR(pGetObjName->Name);
            _ASSERT_HANDLE(pGetObjName->Handle);

            pName = MxGetObjName (pGetObjName->Handle);

            if (pName != NULL)
            {
                #ifndef MX_ARM_SMP_ENABLED
                ICMxiCacheClean(pName, ROUND(Mxistrlen (pName) + 1, 4));
                #endif

                *pGetObjName->Name = pName;
            }
            else
            {
                *pGetObjName->Name = NULL;
            }

            break;
        }

        case MXIC_KERNEL_GETCPU_INFO:
        {
            MxICKrnGetCpuCtx * pCpuCtx = &pICCmd->Param.KrnGetCpuCtx;
            _ASSERT_PTR(pCpuCtx->pCpuCtx);
            
            #ifdef MX_ARM_ENABLED
            memcpy (pCpuCtx->pCpuCtx, MxGetArmCpuCtx(), sizeof (ARMCPUCONTEXT));
            #endif
            
            break;
        }

        default:
        {
            rc = MX_IC_UNKNOWN_CMD;
            break;
        }
    }

    _ASSERT_RC(rc);

    return rc;
}

/** @brief The function prepares inter-cpu commands (cache control operations): <B>MXIC_KERNEL_CLASS</B>

    @param pCtx   [in] - the pointer to the system context
    @param pICCmd [in] - the IC command

    @return [MXRC] error code

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxICPrepKernelCmd (PMXCONTEXT pCtx, MXICCMD * pICCmd)
{
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pICCmd);

    switch (pICCmd->CmdID)
    {
        case MXIC_KERNEL_GETOBJ:
        {
#ifndef MX_ARM_SMP_ENABLED
            MxICKrnGetObj * pGetObj = &pICCmd->Param.KrnGetObj;

            _ASSERT_PTR(pGetObj->Name);
            _ASSERT_PTR(pGetObj->Handle);

            ICMxiCacheClean(pGetObj->Name,
                              ROUND((Mxistrlen (pGetObj->Name) + 1), 4));


            ICMxiCacheInvalid(pGetObj->Handle, sizeof (HANDLE));
#endif

            break;
        }

        case MXIC_KERNEL_SETOBJ_NAME:
        {
#ifndef MX_ARM_SMP_ENABLED
            MxICKrnSetObjName * pSetObjName = &pICCmd->Param.KrnSetObjName;

            _ASSERT_HANDLE(pSetObjName->Handle);

            if (pSetObjName->Name != NULL)
            {
                ICMxiCacheClean (pSetObjName->Name,
                                 ROUND(Mxistrlen (pSetObjName->Name) + 1, 4));
            }
#endif
            break;
        }

        case MXIC_KERNEL_GETOBJ_NAME:
        {
#ifndef MX_ARM_SMP_ENABLED
            MxICKrnGetObjName * pGetObjName = &pICCmd->Param.KrnGetObjName;

            _ASSERT_PTR(pGetObjName->Name);
            _ASSERT_HANDLE(pGetObjName->Handle);

            ICMxiCacheInvalid(pGetObjName->Name, sizeof (pGetObjName->Name));
#endif
            break;
        }

        case MXIC_KERNEL_GETCPU_INFO:
        {
#ifndef MX_ARM_SMP_ENABLED                
            MxICKrnGetCpuCtx * pCpuCtx = &pICCmd->Param.KrnGetCpuCtx;
            _ASSERT_PTR(pCpuCtx->pCpuCtx);
#endif
            break;
        }

        default:
        {
            rc = MX_IC_UNKNOWN_CMD;
            break;
        }
    }

    _ASSERT_RC(rc);

    return rc;
}


/** @brief The function processes threads specific inter-cpu commands: <B>MXIC_THREAD_CLASS</B>

    @param pCtx   [in] - the pointer to the system context
    @param pSrcCpu[in] - the IC context of requiring CPU
    @param pICCmd [in] - the IC command

    @return [MXRC] error code

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxICProcThreadCmd (PMXCONTEXT pCtx, MXICCTX * pSrcCpu, MXICCMD * pICCmd)
{
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pICCmd);

    switch (pICCmd->CmdID)
    {
        case MXIC_THREAD_CREATE:
        {
            MxICThrCreate * pData = &pICCmd->Param.ThrCreate;

            _ASSERT_PTR(pData->Handle);

            if (pData->Name != NULL)
            {
                ICMxiCacheInvalid(pData->Name,
                              ROUND((Mxistrlen (pData->Name) + 1), 4));
            }

            rc = MxCreateThread(pData->Proc,
                                  pData->Name,
                                  pData->Param,
                                  pData->Prior,
                                  pData->Handle);

            ICMxiCacheClean(pData->Handle, sizeof (HANDLE));

            break;
        }

        case MXIC_THREAD_REMOVE:
        {
            MxICThrRemove * pData = &pICCmd->Param.ThrRemove;

            rc = MxRemoveThread(pData->Handle);

            break;
        }

        case MXIC_THREAD_SCHEDULE:
        {
            MxICThrSchedule* pData = &pICCmd->Param.ThrSchedule;

            rc = MxScheduleThread (pData->Handle);

            break;
        }

        case MXIC_THREAD_UNSCHEDULE:
        {
            MxICThrSchedule* pData = &pICCmd->Param.ThrSchedule;

            rc = MxUnscheduleThread (pData->Handle);

            break;
        }

        case MXIC_THREAD_SETPARAM:
        {
            MxICThrSetParam* pData = &pICCmd->Param.ThrSetParam;

            rc = MxSetThreadParam(pData->Handle, pData->Param);

            break;
        }

        case MXIC_THREAD_GETPARAM:
        {
            MxICThrGetParam* pData = &pICCmd->Param.ThrGetParam;

            _ASSERT_PTR(pData->Param);

            rc = MxGetThreadParam(pData->Handle, pData->Param);

            ICMxiCacheClean(pData->Param, sizeof (*pData->Param));

            break;
        }

        case MXIC_THREAD_GETRET:
        {
            MxICThrRet * pData = &pICCmd->Param.ThrRet;

            _ASSERT_PTR(pData->Ret);

            *pData->Ret = MxGetThreadRetVal(pData->Handle);

            ICMxiCacheClean(pData->Ret, sizeof (*pData->Ret));

            break;
        }

        case MXIC_THREAD_SETPRIOR:
        {
            MxICThrSetPrior* pData = &pICCmd->Param.ThrSetPrior;

            rc = MxSetThreadPriority(pData->Handle, pData->Prior);

            break;
        }

        case MXIC_THREAD_GETPRIOR:
        {
            MxICThrGetPrior * pData = &pICCmd->Param.ThrGetPrior;

            rc = MxGetThreadPriority(pData->Handle, pData->Prior);

            ICMxiCacheClean(pData->Prior, sizeof (*pData->Prior));

            break;
        }

        case MXIC_THREAD_INJECT_PROC:
        {
            MxICThrInjectProc * pCmd = &pICCmd->Param.ThrInjectProc;

            _ASSERT_PTR (pCmd->Entry);

            pCmd->Entry (pCmd->Param);

            break;
        }

        case MXIC_THREAD_INJECT_HANDLE:
        {
            MxICThrInjectHandle* pCmd = &pICCmd->Param.ThrInjectHandle;

            _ASSERT_HANDLE(pCmd->Handle);

            rc = MxRunThread (pCmd->Handle, NULL);

            _ASSERT_RC (rc);

            break;
        }

        case MXIC_THREAD_SCHEDULE_EX:
        {
            MxICThrScheduleEx* pData = &pICCmd->Param.ThrScheduleEx;

            rc = MxScheduleThreadEx (pData->Handle, pData->Prior, pData->SchedCtrl);

            break;
        }

#ifdef MX_MTHREAD_ENABLED
        case MXIC_THREAD_CREATE_M:
        {
            MxICMThrCreate * pData = &pICCmd->Param.MThrCreate;

            _ASSERT_PTR(pData->Handle);

            if (pData->Name != NULL)
            {
                ICMxiCacheInvalid(pData->Name,
                              ROUND((Mxistrlen (pData->Name) + 1), 4));
            }

            rc = MxMCreateThread  (pData->Proc,
                                  pData->Name,
                                  pData->Param,
                                  pData->Prior,
                                  pData->flag,
                                  pData->pStack,
                                  pData->nStackSize,
                                  pData->Handle);

            ICMxiCacheClean(pData->Handle, sizeof (HANDLE));

            break;
        }
#endif        

        default:
        {
            rc = MX_IC_UNKNOWN_CMD;
            break;
        }
    }

    _ASSERT_RC(rc);

    return rc;
}

/** @brief The function prepares threads specific inter-cpu commands (cache control): <B>MXIC_THREAD_CLASS</B>

    @param pCtx   [in] - the pointer to the system context
    @param pICCmd [in] - the IC command

    @return [MXRC] error code

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxICPrepThreadCmd (PMXCONTEXT pCtx, MXICCMD * pICCmd)
{
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pICCmd);

    switch (pICCmd->CmdID)
    {
        case MXIC_THREAD_CREATE:
        {
            MxICThrCreate * pData = &pICCmd->Param.ThrCreate;

            _ASSERT_PTR(pData->Handle);

            if (pData->Name != NULL)
            {
                ICMxiCacheClean(pData->Name,
                              ROUND((Mxistrlen (pData->Name) + 1), 4));
            }

            ICMxiCacheInvalid(pData->Handle, sizeof (HANDLE));

            break;
        }

#ifdef MX_IC_THREAD_REMOVE_ASYNC
        case MXIC_THREAD_REMOVE:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }

#endif //MX_IC_THREAD_REMOVE_ASYNC

#ifdef MX_IC_THREAD_SCHEDULE_ASYNC
        case MXIC_THREAD_SCHEDULE:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }

#endif //MX_IC_THREAD_SCHEDULE_ASYNC

#ifdef MX_IC_THREAD_SCHEDULE_EX_ASYNC
        case MXIC_THREAD_SCHEDULE_EX:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }

#endif //MX_IC_THREAD_SCHEDULE_EX_ASYNC

#ifdef MX_IC_THREAD_UNSCHEDULE_ASYNC
        case MXIC_THREAD_UNSCHEDULE:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }

#endif //MX_IC_THREAD_UNSCHEDULE_ASYNC

#ifdef MX_IC_THREAD_SCHEDULE_BY_EVENT_ASYNC
        case MXIC_THREAD_SCH_BY_EVENT:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }

#endif //MX_IC_THREAD_SCHEDULE_BY_EVENT_ASYNC

#ifdef MX_IC_THREAD_UNSCHEDULE_BY_EVENT_ASYNC
        case MXIC_THREAD_UNSCH_BY_EVENT:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }

#endif //MX_IC_THREAD_UNSCHEDULE_BY_EVENT_ASYNC

#ifdef MX_IC_THREAD_SETPARAM_ASYNC
        case MXIC_THREAD_SETPARAM:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }

#endif //MX_IC_THREAD_SETPARAM_ASYNC

        case MXIC_THREAD_GETPARAM:
        {
#ifndef MX_ARM_SMP_ENABLED
            MxICThrGetParam* pData = &pICCmd->Param.ThrGetParam;

            _ASSERT_PTR(pData->Param);

            ICMxiCacheInvalid(pData->Param, sizeof (*pData->Param));
#endif

            break;
        }

        case MXIC_THREAD_GETRET:
        {
#ifndef MX_ARM_SMP_ENABLED
            MxICThrRet * pData = &pICCmd->Param.ThrRet;

            _ASSERT_PTR(pData->Ret);

            ICMxiCacheInvalid(pData->Ret, sizeof (*pData->Ret));
#endif

            break;
        }

#ifdef MX_IC_THREAD_SETPRIOR_ASYNC
        case MXIC_THREAD_SETPRIOR:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }

#endif //MX_IC_THREAD_SETPRIOR_ASYNC


        case MXIC_THREAD_GETPRIOR:
        {
#ifndef MX_ARM_SMP_ENABLED
            MxICThrGetPrior * pData = &pICCmd->Param.ThrGetPrior;

            _ASSERT_PTR(pData->Prior);

            ICMxiCacheInvalid(pData->Prior, sizeof (*pData->Prior));
#endif
            break;
        }

        case MXIC_THREAD_INJECT_PROC:
        case MXIC_THREAD_INJECT_HANDLE:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }

#ifdef MX_MTHREAD_ENABLED
        case MXIC_THREAD_CREATE_M:
        {
            MxICMThrCreate * pData = &pICCmd->Param.MThrCreate;

            _ASSERT_PTR(pData->Handle);

            if (pData->Name != NULL)
            {
                ICMxiCacheClean(pData->Name,
                              ROUND((Mxistrlen (pData->Name) + 1), 4));
            }

            ICMxiCacheInvalid(pData->Handle, sizeof (HANDLE));

            break;
        }
#endif        

        default:
        {
            break;
        }
    }

    _ASSERT_RC(rc);

    return rc;
}


/** @brief The function processes events specific inter-cpu commands: <B>MXIC_EVENT_CLASS</B>

    @param pCtx   [in] - the pointer to the system context
    @param pSrcCpu[in] - the IC context of requiring CPU
    @param pICCmd [in] - the IC command

    @return [MXRC] error code

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxICProcEventCmd (PMXCONTEXT pCtx, MXICCTX * pSrcCpu, MXICCMD * pICCmd)
{
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pICCmd);

    switch (pICCmd->CmdID)
    {
        case MXIC_EVENT_CREATE:
        {
            MxICEventCreate * pData = &pICCmd->Param.EventCreate;

            _ASSERT_PTR(pData->Handle);

            if (pData->Name != NULL)
            {
                ICMxiCacheInvalid(pData->Name,
                              ROUND((Mxistrlen (pData->Name) + 1), 4));
            }

            rc = MxCreateEvent(pData->InitState,
                                pData->Name,
                                pData->Handle);

            ICMxiCacheClean(pData->Handle, sizeof (HANDLE));

            break;
        }

        case MXIC_EVENT_REMOVE:
        {
            MxICEventRemove * pData = &pICCmd->Param.EventRemove;

            rc = MxRemoveEvent(pData->Handle);

            break;
        }

        case MXIC_EVENT_SET:
        {
            MxICEventSet * pData = &pICCmd->Param.EventSetReset;

            rc = MxSetEvent(pData->Handle);

            break;
        }

        case MXIC_EVENT_RESET:
        {
            MxICEventSet * pData = &pICCmd->Param.EventSetReset;

            rc = MxResetEvent(pData->Handle);

            break;
        }

        case MXIC_EVENT_IS_SET:
        {
            MxICEventIsSet * pData = &pICCmd->Param.EventIsSet;

            rc = MxIsEventSet(pData->Handle);

            break;
        }

        default:
        {
            rc = MX_IC_UNKNOWN_CMD;
            break;
        }
    }

    _ASSERT_RC(rc);

    return rc;
}


/** @brief The function prepares events specific inter-cpu commands (cache control): <B>MXIC_EVENT_CLASS</B>

    @param pCtx   [in] - the pointer to the system context
    @param pICCmd [in] - the IC command

    @return [MXRC] error code

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxICPrepEventCmd (PMXCONTEXT pCtx, MXICCMD * pICCmd)
{
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pICCmd);

    switch (pICCmd->CmdID)
    {
        case MXIC_EVENT_CREATE:
        {
            MxICEventCreate * pData = &pICCmd->Param.EventCreate;

            _ASSERT_PTR(pData->Handle);

            if (pData->Name != NULL)
            {
                ICMxiCacheClean(pData->Name,
                              ROUND((Mxistrlen (pData->Name) + 1), 4));
            }

            ICMxiCacheInvalid(pData->Handle, sizeof (*pData->Handle));

            break;
        }

#ifdef MX_IC_EVENT_REMOVE_ASYNC
        case MXIC_EVENT_REMOVE:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }
#endif //MX_IC_EVENT_REMOVE_ASYNC

#ifdef MX_IC_EVENT_SET_ASYNC
        case MXIC_EVENT_SET:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }
#endif //MX_IC_EVENT_SET_ASYNC

#ifdef MX_IC_EVENT_RESET_ASYNC
        case MXIC_EVENT_RESET:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }
#endif //MX_IC_EVENT_RESET_ASYNC

        default:
        {
            break;
        }
    }

    _ASSERT_RC(rc);

    return rc;
}

/** @brief The function processes timers specific inter-cpu commands: <B>MXIC_TIMER_CLASS</B>

    @param pCtx   [in] - the pointer to the system context
    @param pSrcCpu[in] - the IC context of requiring CPU
    @param pICCmd [in] - the IC command

    @return [MXRC] error code

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxICProcTimerCmd (PMXCONTEXT pCtx, MXICCTX * pSrcCpu, MXICCMD * pICCmd)
{
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pICCmd);

    switch (pICCmd->CmdID)
    {
        case MXIC_TIMER_CREATE:
        {
            MxICTimerCreate * pData = &pICCmd->Param.TimerCreate;

            _ASSERT_PTR(pData->Handle);

            if (pData->Name != NULL)
            {
                ICMxiCacheInvalid(pData->Name,
                              ROUND((Mxistrlen (pData->Name) + 1), 4));
            }

            rc = MxCreateTimer(pData->nMode,
                                 pData->nPeriod,
                                 pData->Name,
                                 pData->hEvent,
                                 pData->Handle);

            ICMxiCacheClean(pData->Handle, sizeof (*pData->Handle));

            break;
        }

        case MXIC_TIMER_REMOVE:
        {
            MxICTimerRemove * pData = &pICCmd->Param.TimerRemove;

            rc = MxRemoveTimer(pData->Handle);

            break;
        }

        case MXIC_TIMER_START:
        {
            MxICTimerStart* pData = &pICCmd->Param.TimerStart;

            rc = MxStartTimer(pData->Handle);

            break;
        }

        case MXIC_TIMER_START_FRAC:
        {
            MxICTimerStart* pData = &pICCmd->Param.TimerStart;

            rc = MxStartFracTimer(pData->Handle);

            break;
        }

        case MXIC_TIMER_STOP:
        {
            MxICTimerStart* pData = &pICCmd->Param.TimerStart;

            rc = MxStopTimer(pData->Handle);

            break;
        }

        case MXIC_TIMER_SETEVENT:
        {
            MxICTimerSetEvent* pData = &pICCmd->Param.TimerSetEvent;

            rc = MxSetTimerEvent(pData->Handle, pData->Event);

            break;
        }

        case MXIC_TIMER_SETPERIOD:
        {
            MxICTimerSetPeriod* pData = &pICCmd->Param.TimerSetPeriod;

            rc = MxSetTimerPeriod(pData->Handle, pData->nPeriod);

            break;
        }

        case MXIC_TIMER_GETPERIOD:
        {
            MxICTimerGetPeriod* pData = &pICCmd->Param.TimerGetPeriod;

            _ASSERT_PTR(pData->pnPeriod);

            rc = MxGetTimerPeriod(pData->Handle, pData->pnPeriod);

            ICMxiCacheClean(pData->pnPeriod, sizeof (*pData->pnPeriod));

            break;
        }

        case MXIC_TIMER_SETPROC:
        {
            MxICTimerSetProc * pData = &pICCmd->Param.TimerSetProc;

            rc = MxSetTimerProc(pData->Handle, pData->Proc, pData->Ptr);

            break;
        }

        default:
        {
            rc = MX_IC_UNKNOWN_CMD;
            break;
        }
    }

    _ASSERT_RC(rc);

    return rc;
}

/** @brief The function prepares timers specific inter-cpu commands (cache control): <B>MXIC_TIMER_CLASS</B>

    @param pCtx   [in] - the pointer to the system context
    @param pICCmd [in] - the IC command

    @return [MXRC] error code

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxICPrepTimerCmd (PMXCONTEXT pCtx, MXICCMD * pICCmd)
{
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pICCmd);

    switch (pICCmd->CmdID)
    {
        case MXIC_TIMER_CREATE:
        {
            MxICTimerCreate * pData = &pICCmd->Param.TimerCreate;

            _ASSERT_PTR(pData->Handle);

            if (pData->Name != NULL)
            {
                ICMxiCacheClean(pData->Name,
                              ROUND((Mxistrlen (pData->Name) + 1), 4));
            }

            ICMxiCacheInvalid(pData->Handle, sizeof (*pData->Handle));

            break;
        }

#ifdef MX_IC_TIMER_REMOVE_ASYNC
        case MXIC_TIMER_REMOVE:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }
#endif //MX_IC_TIMER_REMOVE_ASYNC

#ifdef MX_IC_TIMER_SETPERIOD_ASYNC
        case MXIC_TIMER_SETPERIOD:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }
#endif //MX_IC_TIMER_SETPARIOD_ASYNC

        case MXIC_TIMER_GETPERIOD:
        {
#ifndef MX_ARM_SMP_ENABLED
            MxICTimerGetPeriod* pData = &pICCmd->Param.TimerGetPeriod;

            _ASSERT_PTR(pData->pnPeriod);

            ICMxiCacheInvalid(pData->pnPeriod, sizeof (*pData->pnPeriod));
#endif
            break;
        }

#ifdef MX_IC_TIMER_START_ASYNC
        case MXIC_TIMER_START:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }
#endif //MX_IC_TIMER_START_ASYNC        

#ifdef MX_IC_TIMER_START_FRAC_ASYNC
        case MXIC_TIMER_START_FRAC:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }
       
#endif //MX_IC_TIMER_START_FRAC_ASYNC

#ifdef MX_IC_TIMER_STOP_ASYNC
        case MXIC_TIMER_STOP:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }
#endif //MX_IC_TIMER_STOP_ASYNC

#ifdef MX_IC_TIMER_SET_EVENT_ASYNC
        case MXIC_TIMER_SETEVENT:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }
#endif //MX_IC_TIMER_SET_EVENT_ASYNC

#ifdef MX_IC_TIMER_SET_PROC
        case MXIC_TIMER_SETPROC:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }
#endif //MX_IC_TIMER_SET_EVENT_ASYNC

        default:
        {
            break;
        }
    }

    _ASSERT_RC(rc);

    return rc;
}

#ifdef MX_MTHREAD_ENABLED

/** @brief The function prepares semaphores specific inter-cpu commands (cache control): <B>MXIC_SEMA_CLASS</B>

    @param pCtx   [in] - the pointer to the system context
    @param pICCmd [in] - the IC command

    @return [MXRC] error code

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxICPrepSemaCmd (PMXCONTEXT pCtx, MXICCMD * pICCmd)
{
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pICCmd);

    switch (pICCmd->CmdID)
    {
        case MXIC_SEMA_CREATE:
        {
            MxICSemaCreate * pData = &pICCmd->Param.SemaCreate;

            _ASSERT_PTR(pData->Handle);

            if (pData->Name != NULL)
            {
                ICMxiCacheClean(pData->Name,
                              ROUND((Mxistrlen (pData->Name) + 1), 4));
            }

            ICMxiCacheInvalid(pData->Handle, sizeof (*pData->Handle));

            break;
        }

#ifdef MX_IC_SEMA_REMOVE_ASYNC
        case MXIC_SEMA_REMOVE:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }
#endif //MX_IC_SEMA_REMOVE_ASYNC

#ifdef MX_IC_SEMA_SET_ASYNC
        case MXIC_SEMA_SET:
        {
            SET_ASYNC_CMD(pICCmd);
            break;
        }
#endif //MX_IC_SEMA_SET_ASYNC

        default:
        {
            break;
        }
    }

    _ASSERT_RC(rc);

    return rc;
}


/** @brief The function processes semaphores specific inter-cpu commands: <B>MXIC_SEMA_CLASS</B>

    @param pCtx   [in] - the pointer to the system context
    @param pSrcCpu[in] - the IC context of requiring CPU
    @param pICCmd [in] - the IC command

    @return [MXRC] error code

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxICProcSemaCmd (PMXCONTEXT pCtx, MXICCTX * pSrcCpu, MXICCMD * pICCmd)
{
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pICCmd);

    switch (pICCmd->CmdID)
    {
        case MXIC_SEMA_CREATE:
        {
            MxICSemaCreate * pData = &pICCmd->Param.SemaCreate;

            _ASSERT_PTR(pData->Handle);

            if (pData->Name != NULL)
            {
                ICMxiCacheInvalid(pData->Name,
                              ROUND((Mxistrlen (pData->Name) + 1), 4));
            }

            rc = MxCreateSema(pData->InitState, pData->Name, pData->Handle);

            ICMxiCacheClean(pData->Handle, sizeof (*pData->Handle));

            break;
        }

        case MXIC_SEMA_REMOVE:
        {
            MxICSemaRemove * pData = &pICCmd->Param.SemaRemove;

            rc = MxRemoveSema(pData->Handle);

            break;
        }

        case MXIC_SEMA_SET:
        {
            MxICSemaSet* pData = &pICCmd->Param.SemaSet;

            rc = MxSetSema(pData->Handle, pData->Value);

            break;
        }

        default:
        {
            rc = MX_IC_UNKNOWN_CMD;
            break;
        }
    }

    _ASSERT_RC(rc);

    return rc;
}


#endif // #ifdef MX_MTHREAD_ENABLED

/** @brief This command receives 4GMX I-OS command(s) by reading 
           all pools(of all supported cores)

    @param pMyICCtx[in] - the pointer to local 4GMX I-CPU context
    @param pPool_id[out]- the pool index where command was found

    @return [MXICCMD *] the pointer or NULL 

    \ingroup group_lte_4gmx_inter_cpu
*/
MXICCMD * MxICRecvCmd (MXICCTX * pMyICCtx, UINT32* pPool_id)
{
    MXICCMD * pCmd;
    UINT32 num = 0;

    _ASSERT_PTR(pMyICCtx);

    // this loop scans POOLs sequentially 
    // and prevents to process all messages of pool#0 prior pool#1 or pool#2
    // in this way the last core message will not wait till messages of other cores are processed

    MxDMB();

    while (num < MX_MAX_INTERCPU)
    {
        pCmd = MxiPoolRecv (&pMyICCtx->CmdPool[pMyICCtx->ScanPool++]);

        if (pMyICCtx->ScanPool >= MX_MAX_INTERCPU)
            pMyICCtx->ScanPool = 0;

        if (pCmd != NULL)
        {
            *pPool_id = (pMyICCtx->ScanPool == 0) ? (MX_MAX_INTERCPU - 1) : (pMyICCtx->ScanPool - 1);
            return pCmd;
        }

        num++;
    }

    return NULL;
}

/** @brief The function processes inter-cpu queue

    @param pParam [in] - the pointer to the 4GMX context

    @return [UINT32] error code

    \ingroup group_lte_4gmx_inter_cpu
*/
UINT32 MxiInterCpuThread(LPVOID pParam)
{
    PMXCONTEXT pCtx;
    MXICCTX * pMyICCtx, *pRemICCtx;
    MXICCMD * pICCmd;
    MXRC rc;
    UINT32 pool_id = 0;
    DEF_ASSERT_LR();

    pCtx = (PMXCONTEXT) pParam;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCtx->ICTable);

    // The ICTable should be allocated in non-cacheable memory
    // so, the operations of cache invalidate/clean are not required

    pMyICCtx = (pCtx->ICTable [MxGetCpuID()]);

    _ASSERT_PTR(pMyICCtx);

    while ((pICCmd = MxICRecvCmd (pMyICCtx, &pool_id)) != NULL)
    {
        _ASSERT_LR(pICCmd->DstCpuID == MxGetCpuID ());
    
        pRemICCtx = (pCtx->ICTable [pICCmd->SrcCpuID]);

        _ASSERT_PTR(pRemICCtx);

        switch (MXIC_GETCLASS(pICCmd->CmdID))
        {
            case MXIC_KERNEL_CLASS:
                {
                    rc = MxICProcKernelCmd (pCtx, pRemICCtx, pICCmd);
                    break;
                }

            case MXIC_THREAD_CLASS:
                {
                    rc = MxICProcThreadCmd (pCtx, pRemICCtx, pICCmd);
                    break;
                }

            case MXIC_EVENT_CLASS:
                {
                    rc = MxICProcEventCmd (pCtx, pRemICCtx, pICCmd);
                    break;
                }

            case MXIC_TIMER_CLASS:
                {
                    rc = MxICProcTimerCmd (pCtx, pRemICCtx, pICCmd);
                    break;
                }

#ifdef MX_MTHREAD_ENABLED
            case MXIC_SEMA_CLASS:
                {
                    rc = MxICProcSemaCmd (pCtx, pRemICCtx, pICCmd);
                    break;
                }
#endif            

            default:
                {
                    rc = MX_IC_UNKNOWN_CLASS;
                    break;
                }
        }

        _ASSERT_RC(rc);

        if (IS_SYNC_CMD(pICCmd))
        {
            pMyICCtx->CmdRC[pICCmd->SrcCpuID] = rc;
            SET_CMD_COMPLETED(pICCmd);
            
            _NOP();
            _NOP();
            _NOP();
        }

        MxiPoolFree(&pMyICCtx->CmdPool[pool_id], pICCmd);
    }

    return 0;
}

/** @brief The inter-cpu handler, this function processes inter-cpu commands
           that are placed by the 4GMX systems executed on other CPUs

    @param pAckReg [in] - interrupt acknowledge register
    @param nIrqID  [in] - the IrqID (it includes source CPU id)
    @param pParam  [in] - the interrupt ID

    \ingroup group_lte_4gmx_inter_cpu
*/
void MxiInterCpuHandler(volatile UINT32 * pAckReg, UINT32 nIrqID, LPVOID pParam)
{
    PMXCONTEXT pCtx;

#ifdef _MX_DEBUG
    MXRC rc = MX_OK;
#endif

    UINT32 nCpuID = MxGetCpuID();

    pCtx = MxInqContext();

    // to clear the inter-CPU interrupt generated to this processor
    // from other processor(s)

    MxClearInterCpu(nCpuID, pCtx->ICTable [nCpuID]->Irq);

    // to acknowledge the interrupt 
    *pAckReg = nIrqID;

    // when interrupt is ackowledged, 
    // we can call commands handler
    // if interrupt will be raised again 
    // the system calls handler automatically 

#ifdef _MX_DEBUG
    rc = MxiInterCpuThread (pCtx);
    _ASSERT_RC(rc);
#else
    MxiInterCpuThread (pCtx);
#endif

}

/** @brief The function initializes inter-cpu sub-system and called
           automatically by system kernel.

    @param pSysCtx[in] - the pointer to the system context
    @param nIrq   [in] - the Inter-CPU IRQ id
    @param pTable [in] - the pointer to the array of the 4GMX inter-cpu contexts
    @param pICCtx [in] - the pointer to the storage for the inter-cpu context
    @param pSmpCtx[in] - the pointer to the OSs SMP context

    @return [MXRC] an initialization error code

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxiInterCpuInit (LPVOID pSysCtx, UINT32 nIrq,
                      MXICCTX ** pTable, MXICCTX * pICCtx, MXSMPCTX * pSmpCtx)
{
    UINT32 nCpu, coreID;
    MXRC rc = MX_OK;
    PMXCONTEXT pCtx = (PMXCONTEXT)pSysCtx;

    _ASSERT_PTR(pCtx);

    /* The inter-CPU functionality can be disabled */

    if (pTable == NULL)
        return MX_OK;

    _ASSERT_PTR(pICCtx);

    nCpu = MxGetCpuID();

    pCtx->ICTable = pTable;
    pCtx->ICSMPCtx= pSmpCtx;

    if (pSmpCtx != NULL)
    {
        Mximemset(pSmpCtx, 0, sizeof (*pSmpCtx));
    }

    pTable[nCpu] = pICCtx;

    Mximemset(pICCtx, 0x0, sizeof (MXICCTX));

#ifdef MX_ARM_ENABLED
    pICCtx->CoreCPUID = MxGetARMCpuIDReg() & 0xFF;
#else
    pICCtx->CoreCPUID = 0;
#endif

    // to set the pointer to the IC context owner
    pICCtx->lpMxCtx = pSysCtx;

    // To initialize I-CPU pools for each suuported core 
    for (coreID = 0; coreID < MX_MAX_INTERCPU; coreID++)
    {
        MxiPoolInit (&(pICCtx->CmdPool[coreID]),
                     pICCtx->CmdPStor[coreID],
                     MX_MAX_IC_QUEUE,
                     sizeof (pICCtx->CmdPStor[0][0]),
                     pICCtx->CmdQFree[coreID],
                     pICCtx->CmdQUsed[coreID]);
    }

#if (MX_MAX_DATA_SHARED_OBJ > 0)
    MxiDefSysPart(&pICCtx->ShrPart, pICCtx->ShrStorage,
                   sizeof(MXICShrObj), MX_MAX_DATA_SHARED_OBJ);

#else
	Mximemset (&pICCtx->ShrPart, 0, sizeof (pICCtx->ShrPart));
#endif

    rc = MxCreateThread(MxiInterCpuThread, NULL, pCtx,
                          MX_INTERCPU_THR_PRIOR,
                          &pCtx->hInterCpuThr);

    if (FAILED(rc))
        return MX_IC_ALLOC_THR_ERROR;

    rc = MxSetIntHandler(nIrq, INT_TYPE_IRQ, (TPRIORITY)MX_INTERCPU_IRQ_PRIOR,
                           MxiInterCpuHandler, (LPVOID)nIrq);

    if (FAILED(rc))
    {
        MxRemoveThread(pCtx->hInterCpuThr);
        pCtx->hInterCpuThr = HNULL;

        return MX_IC_SET_IRQ_ERROR;
    }

    pICCtx->Irq = nIrq;

    rc = MxEnableInt(nIrq);

    if (FAILED(rc))
    {
        MxRemoveThread(pCtx->hInterCpuThr);
        pCtx->hInterCpuThr = HNULL;

        return MX_IC_ENABLE_IRQ_ERROR;
    }

    // The flag of IC initialization will be set in
    // the MxStart function

    return MX_OK;
}

/** @brief The function allocates a space for sharing of the 4GMX data object

    @param pMxCtx [in] - the pointer to the 4GMX context

    @return [MXICShrObj *] the pointer to the allocated memory

    \ingroup group_lte_4gmx_inter_cpu
*/
MXICShrObj * MxiAllocSharedSpace(LPVOID pMxCtx)
{
#if (MX_MAX_DATA_SHARED_OBJ > 0)

    IRQTYPE  irq;
    PMXICCTX pICCtx;
    LPVOID   pSpace = NULL;

    _ASSERT_PTR (pMxCtx);

    if (((PMXCONTEXT)pMxCtx)->ICTable == NULL)
        return NULL;

    MxiDisInt(&irq);

    pICCtx = (((PMXCONTEXT)pMxCtx)->ICTable [MxGetCpuID ()]);

    _ASSERT_PTR(pICCtx);

    MxGetLock(&pICCtx->Lock);

    pSpace = MxiAllocSysPart (&pICCtx->ShrPart);

    _ASSERT_PTR (pSpace);

    MxReleaseLock(&pICCtx->Lock);

    MxiRestInt(irq);

    return (MXICShrObj *)pSpace;

#else
    return NULL;
#endif
}

/** @brief The function frees an allocated data object space

    @param pMxCtx [in] - the pointer to the 4GMX context
    @param pSpace [in] - the pointer to the allocated space (see: MxiAllocSharedSpace)

    @return [MXRC] error code

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxiFreeSharedSpace(LPVOID pMxCtx, LPVOID pSpace)
{
#if (MX_MAX_DATA_SHARED_OBJ > 0)

    IRQTYPE  irq;
    PMXICCTX pICCtx;

    _ASSERT_PTR (pMxCtx);
    _ASSERT_PTR (pSpace);

    if (((PMXCONTEXT)pMxCtx)->ICTable == NULL)
        return MX_IC_DOESNOT_INIT;

    pICCtx = (((PMXCONTEXT)pMxCtx)->ICTable [MxGetCpuID ()]);

    _ASSERT_PTR(pICCtx);

    MxiDisInt(&irq);

    MxGetLock(&pICCtx->Lock);

    Mximemset (pSpace, 0, sizeof (MXICShrObj));

    MxiFreeSysPart (&pICCtx->ShrPart, pSpace);

    MxReleaseLock(&pICCtx->Lock);

    MxiRestInt(irq);

    return MX_OK;

#else
    return MX_IC_DOESNOT_INIT;
#endif
}

/** @brief This function returns a pointer to the informational block of
           the shared object

    @param pMxCtx [in] - the pointer to the 4GMX context
    @param h      [in] - the handle of the searched object

    @return [MXICShrObj *] the pointer to the informational block or NULL

    \ingroup group_lte_4gmx_inter_cpu
*/
MXICShrObj *MxiFindSharedBlock (LPVOID pMxCtx, HANDLE h)
{
#if (MX_MAX_DATA_SHARED_OBJ > 0)

    IRQTYPE  irq;
    PMXICCTX pICCtx;
    UINT32 i;
    MXICShrObj * pObjList;

    _ASSERT_PTR (pMxCtx);

    if (((PMXCONTEXT)pMxCtx)->ICTable == NULL)
        return NULL;

    pICCtx = (((PMXCONTEXT)pMxCtx)->ICTable [HANDLE_GET_CPU(h)]);

    _ASSERT_PTR(pICCtx);

    MxiDisInt(&irq);

    MxGetLock(&pICCtx->Lock);

    pObjList = (MXICShrObj *)pICCtx->ShrPart.storage;

    for (i = 0; i < MX_MAX_DATA_SHARED_OBJ; i++)
    {
        if (HANDLE_NOT_NULL(pObjList[i].hHand) &&
            HANDLE_TOINT(pObjList[i].hHand) == HANDLE_TOINT(h))
        {
            MxReleaseLock(&pICCtx->Lock);

            MxiRestInt(irq);

            return &pICCtx->ShrStorage [i];
        }
    }

    MxReleaseLock(&pICCtx->Lock);

    MxiRestInt(irq);

    return NULL;

#else
    return NULL;
#endif
}

/** @brief This function returns a pointer to the object descriptor

    @param pMxCtx [in] - the pointer to the 4GMX context
    @param h      [in] - the handle of the searched object

    @return [void *] the pointer to the object descriptor or NULL

    \ingroup group_lte_4gmx_inter_cpu
*/
void * MxiFindSharedDescr (LPVOID pMxCtx, HANDLE h)
{
#ifdef MX_DIRECT_SHARE_OBJ_ACCESS

    MXCONTEXT * pRemCtx;
    pRemCtx = MxInqRemContext(HANDLE_GET_CPU(h));

    if (pRemCtx == NULL)
    {
        _ASSERT_PTR(pRemCtx);
        return NULL;
    }

    switch (HANDLE_GET_TYPE(h))
    {
        case OBJ_TYPE_QUEUE:
            return MxiGetQueueDescr(pRemCtx, h);

        case OBJ_TYPE_PARTITION:
            return MxiGetPartDescr(pRemCtx, h);

        case OBJ_TYPE_HEAP:
            return MxiGetHeapDescr(pRemCtx, h);

        case OBJ_TYPE_RHEAP:
            return MxiGetRHeapDescr(pRemCtx, h);

        default:
            return NULL;
    }

#else
    MXICShrObj * pBlock;

    pBlock = MxiFindSharedBlock (pMxCtx, h);

    _ASSERT_PTR(pBlock);

    return pBlock->pObjDescr;
#endif    
}

/** @brief The function sends inter-cpu command to the remote system and
           returns result of command execution to the caller.

    @param DstCpu [in] - the destination CPU id
    @param id     [in] - the IC command (see: MXICCMDID type)
    @param pParam [in] - the pointer to command the parameter
    @param nSize  [in] - the size of parameter

    @return [MXRC] error code: MX_IC_xxx | remoute 4GMX error code

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxICExec (CPUID DstCpu, MXICCMDID id, LPVOID pParam, UINT32 nSize)
{
    PMXCONTEXT pCtx;
    PMXICCTX   pDstICCtx;
    MXRC       rc = MX_OK;
    MXICCMD*   pCmd;
    UINT32     nTime, sync_flag;
    IRQTYPE    irq;

    irq = ARM_INT_disable ();

    pCtx = MxInqContext ();

    _ASSERT_PTR(pCtx);

    _ASSERT(DstCpu < MX_MAX_INTERCPU);

    if (pCtx->ICTable == NULL)
    {
        ARM_INT_restore (irq);
        return MX_IC_DOESNOT_INIT;
    }

    pDstICCtx = pCtx->ICTable [DstCpu];

    // to check a status of remote system initialization

    if (!IS_IC_INIT(pDstICCtx))
    {
        rc = MxICWaitSystemInit (DstCpu, MX_MAX_SYS_INIT_TIMEOUT);

        if (FAILED(rc))
        {
            MxReleaseLock(&pDstICCtx->Lock);
            ARM_INT_restore (irq);
            _ASSERT_RC(rc);
            return rc;
        }
    }

    // At this point pCmd is pointing to the ICPU command
    // located in ICPU partition

    pCmd = (MXICCMD*)MxiPoolAlloc(&pDstICCtx->CmdPool[MxGetCpuID()]);

	// This is a critical situation when i-cpu storage is so small and
	// should be expanded for the developed application

    if (pCmd == NULL)
    {
        ARM_INT_restore(irq);
        rc = MX_IC_OVERFLOW;
        _ASSERT_RC(rc);

        return rc;
    }

    pCmd->CmdStat   = 0;
    pCmd->CmdID     = id;
    pCmd->SrcCpuID  = MxGetCpuID();
    #ifdef _MX_DEBUG
    pCmd->DstCpuID  = DstCpu;
    #endif

    // By the default all inter-cpu commands are synchronized,
    // but for some commands a <sync> flag can be reset in MxICPrepXXX
    // functions (please see below to the switch)
    // the commands type (sync or async) can be selected by using macros
    // like MX_IC_xxxx_xxxx_ASYNC of the configuration file

    SET_SYNC_CMD(pCmd);

    pCmd->Param = *(MXICDATA*)pParam;

    // To prepare the inter-cpu command (cache control operations)

    switch (MXIC_GETCLASS(id))
    {
        case MXIC_KERNEL_CLASS:
            {
                rc = MxICPrepKernelCmd(pCtx, pCmd);
                break;
            }

        case MXIC_THREAD_CLASS:
            {
                rc = MxICPrepThreadCmd(pCtx, pCmd);
                break;
            }

        case MXIC_EVENT_CLASS:
            {
                rc = MxICPrepEventCmd(pCtx, pCmd);
                break;
            }

        case MXIC_TIMER_CLASS:
            {
                rc = MxICPrepTimerCmd(pCtx, pCmd);
                break;
            }
#ifdef MX_MTHREAD_ENABLED
        case MXIC_SEMA_CLASS:
            {
                rc = MxICPrepSemaCmd(pCtx, pCmd);
                break;
            }
#endif
        default:
            {
                break;
            }
    }

    if (FAILED(rc))
    {
        ARM_INT_restore (irq);
		_ASSERT_RC(rc);
        
        return rc;
    }

    sync_flag = IS_SYNC_CMD(pCmd);

	// The queue overflow cannot be happen because of queue size more than
    // size of partition (number of q. elements > p. elements)

    MxiPoolSend (&pDstICCtx->CmdPool[MxGetCpuID()], pCmd);

    MxDMB();

    ARM_INT_restore(irq);

    if (SUCCESSFUL(rc = MxRaiseInterCpu((DstCpu), pDstICCtx->Irq)))
    {
        if (sync_flag)
        {
            nTime = MxGetTicks();

            // To wait of the completeness status

            rc = MX_IC_TIMEOUT;

            while ((MxGetTicks() - nTime) < MX_MAX_INTERCPU_TIMEOUT)
            {
                if (IS_CMD_COMPLETED(pCmd))
                {
                    rc = MX_OK;
                    break;
                }
            }

            irq = ARM_INT_disable();

            if (SUCCESSFUL(rc))
            {
                rc = pDstICCtx->CmdRC[MxGetCpuID()];
                _ASSERT_RC (rc);
            }

            ARM_INT_restore(irq);
        }

    }

    return rc;
}


/** @brief Teh function returns initialization status of remote system.

    @param cpu [in] - the remote system ID

    @return [BOOL] status

    \ingroup group_lte_4gmx_inter_cpu
*/
BOOL MxICIsSystemInit (CPUID cpu)
{
    PMXCONTEXT pCtx;

    _ASSERT(cpu < MX_MAX_INTERCPU);

    pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);

    if (pCtx->ICTable == NULL)
        return FALSE;

    _ASSERT_PTR(pCtx->ICTable [cpu]);

    if (!IS_IC_INIT(pCtx->ICTable [cpu]))
    {
        if(FAILED(MxICWaitSystemInit (cpu, MX_MAX_SYS_INIT_TIMEOUT)))
            return FALSE;
    }

    return TRUE;
}

/** @brief This function waits a status of remote system initialization

    @param nCFAddr [in] - the pointer to the system context

    @return [MXRC] an error code of waiting

    \ingroup group_lte_4gmx_inter_cpu
*/
ASMPROC MXRC MxiICWaitSysInit (UINT32 nCFAddr)
{
	;ISB
	;DSB

	MOV r2, #0
	LDR r3, =0xFFFFFFF

MyLabel

    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP

	CMP 	r2, r3
	LDREQ 	r0, =1
	BXEQ 	LR

	ADD r2, r2, #1

	NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP

	LDR r1, [r0]

	CMP r1, #1
	BNE MyLabel

	MOV r0, #0
	BX LR
}

/** @brief The function waits a status of remote system initialization

    @param cpu     [in] - the remote system id
    @param nNumMs  [in] - the limit of waiting (in milliseconds)

    @return [MXRC] an error code of waiting

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxICWaitSystemInit (CPUID cpu, UINT32 nNumMs)
{
    UINT32 nTime;

    PMXCONTEXT pCtx;

    _ASSERT(cpu < MX_MAX_INTERCPU);

    pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);

    if (pCtx->ICTable == NULL)
        return MX_IC_DOESNOT_INIT;

    nTime = MxGetTime();

    while ( (MxGetTime() - nTime) < nNumMs)
    {
        if (pCtx->ICTable [cpu] == NULL)
            continue;

        if (IS_IC_INIT(pCtx->ICTable [cpu]))
            return MX_OK;
    }

    return MX_IC_REMSYS_DOESNOT_INIT;
}

MXRC MxICSetInit (BOOL b)
{
    PMXCONTEXT pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);

    if (pCtx->ICTable == NULL)
        return MX_IC_DOESNOT_INIT;

    _ASSERT_PTR(pCtx->ICTable [MxGetCpuID()]);

    if (b)
    {
        IC_SET_INIT(pCtx->ICTable [MxGetCpuID()]);
    }
    else
    {
        IC_RESET_INIT(pCtx->ICTable [MxGetCpuID()]);
    }

    return MX_OK;
}

/** @brief This function returns information about CPU core
            of the remote 4GMX system, it's implemented to
            address remote systems like 0, 1, 2 ...

    @param cpu  [in] - the logical CPU ID
    @param pnVal[out]- the information of remote CPU

    @return [MXRC]

    \ingroup group_lte_4gmx_inter_cpu
*/
MXRC MxGetRemoteCoreID (CPUID cpu, PUINT32 pnVal)
{
    PMXCONTEXT pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pnVal);

    if (pCtx->ICTable == NULL)
        return MX_IC_DOESNOT_INIT;

    *pnVal = pCtx->ICTable [cpu]->CoreCPUID;

    return MX_OK;
}

#endif /*MX_INTERCPU_ENABLED */

