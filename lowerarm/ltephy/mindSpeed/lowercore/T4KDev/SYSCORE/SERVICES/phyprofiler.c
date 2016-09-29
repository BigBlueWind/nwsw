//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Phyprofiler.c
 *
 * @brief ARM L2 PL310 control (based on ARM code)
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "appprintf.h"
#include "supervisor.h"
#include "larmmsgid.h"
#include "phyprofiler.h"

#include "timerdrv.h"

GENPROFILECTX gGenProfileCtx;
HANDLE ProfTimer = {0};
UINT32 ProfTimerPeriod = 0;

extern const char LTmrName[];

#if 0
static UINT32 GetCurrTimeUs(void)
{
    MXRC rc = MX_OK;
    UINT32 TimeUs = 0;

    if (HANDLE_NOT_NULL(ProfTimer))
    {
        IRQTYPE irq;
        UINT32 CurrMs;
        UINT32 CurrTicks;
        UINT32 tmp=0;

        irq = ARM_INT_disable();
        CurrMs = MxGetTime();
        CurrTicks = TimerDrvGetTicks();
        ARM_INT_restore(irq);

        TimeUs = CurrMs * 1000;

        MxReadDrvData(ProfTimer, TIMER_CMD_GET_PERIOD, &tmp, NULL);

        if(tmp)
        {
            TimeUs += (CurrTicks + tmp / (2*1000)) / (tmp / 1000);
        }
    }
    else
    {
        rc = MX_HANDLE_ERROR;
    }
    return TimeUs;
}

static INLINE UINT64 GetCurrTimeTicks(void)
{
    UINT64 retVal = 0;

    if (HANDLE_NOT_NULL(ProfTimer))
    {
        IRQTYPE irq;
        UINT64 CurrMs;
        UINT64 CurrTicks;

        irq = ARM_INT_disable();
        CurrMs = MxGetTime();
        CurrTicks = TimerDrvGetTicks();
        ARM_INT_restore(irq);

        retVal = CurrMs * ProfTimerPeriod + (ProfTimerPeriod - CurrTicks);
        if(retVal == 0)
            retVal++;
    }
    return retVal;
}
#endif

MXRC PhyProfilerApiProc(ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * pParam;
    MXRC rc = MX_OK;
    int i;

    if(pApi->Type == API_TYPE_CFG_PARAM)
    {
        switch(pApi->MessageID)
        {
            case APP_DIAG_PROFILE_ENA:

                if(pApi->Length == 0)
                {// enable all by default
                    gGenProfileCtx.ProfileEnaMask = 0xFFFFFFFF;
                    for (i = 0; i < PROF_MAXCNTRS; i++)
                    {
                        gGenProfileCtx.FrmProcTime[i] = 0;
                        gGenProfileCtx.FrmProcTimeStart[i] = 0;
                    }
#if PHYPROF_DBG
                    printf("\t-enabled all\n");
#endif
                }
                else
                {
                    UINT16 ParamId = PAR_DIAG_MIN;
                    while (ParamId < PAR_DIAG_MAX)
                    {
                        pParam =  ApiGetParamByID(pApi, ParamId);
                        if(pParam)
                        {
                            gGenProfileCtx.ProfileEnaMask |= PhyProfParam2Mode(ParamId);
                            gGenProfileCtx.FrmProcTime[PhyProfParam2Idx(ParamId)] = 0;
                            gGenProfileCtx.FrmProcTimeStart[PhyProfParam2Idx(ParamId)] = 0;
#if PHYPROF_DBG
                            printf("\t-enabled %d Time measurement\n", PhyProfParam2Idx(ParamId));
#endif
                        }
                        ParamId++;
                    }
                }
                // Get timer object from system
                if (HANDLE_NULL(ProfTimer))
                {
                    rc = MxGetObjByName(CUR_CPU, LTmrName, &ProfTimer);
                    if (FAILED(rc))
                    {
                        ProfTimer = HNULL;
#if PHYPROF_DBG
                        printf ("Warning: LTmr is not found - 1\r\n");
#endif
                    }
                    else
                    {
                        UINT32 tmp;
                        // Read timer's period
                        MxReadDrvData(ProfTimer, TIMER_CMD_GET_PERIOD, &tmp, NULL);
                        ProfTimerPeriod = tmp;
#if PHYPROF_DBG
                        // FIXME: Debug
                        printf("TimerPeriod is: %i\n", tmp);
                        printf("CurrTime is: %i, %i\n", MxGetTime(), TimerDrvGetTicks());
#endif
                    }
                }

                break;

            case APP_DIAG_PROFILE_DIS:

                if(pApi->Length == 0)
                {// disable all by default
                    gGenProfileCtx.ProfileEnaMask = 0;
#if PHYPROF_DBG
                    printf("\t-disabled all\n");
#endif
                }
                else
                {
                    UINT16 ParamId = PAR_DIAG_MIN;
                    while (ParamId < PAR_DIAG_MAX)
                    {
                        pParam =  ApiGetParamByID(pApi, ParamId);
                        if(pParam)
                        {
                            gGenProfileCtx.ProfileEnaMask &= ~ PhyProfParam2Mode(ParamId);
#if PHYPROF_DBG
                            printf("\t-disabled %d Time measurement\n", PhyProfParam2Idx(ParamId));
#endif
                        }
                        ParamId++;
                    }
                }
                break;
            default:
                break;
        }
    }
    else if(pApi->Type == API_TYPE_QRY_PARAM)
    {
        switch(pApi->MessageID)
        {
            case APP_DIAG_PROFILE_STAT:

                if(gGenProfileCtx.ProfileEnaMask)
                {
                    UINT16 ParamId = PAR_DIAG_MIN;
                    while (ParamId < PAR_DIAG_MAX)
                    {
                        if(gGenProfileCtx.ProfileEnaMask & PhyProfParam2Mode(ParamId))
                        {// Full
                            ApiAddParam(pResp, NULL, ParamId,
                                (LPVOID)&gGenProfileCtx.FrmProcTime[PhyProfParam2Idx(ParamId)], sizeof(UINT32));
#if PHYPROF_DBG
                            printf("%d time %d ticks\n", PhyProfParam2Idx(ParamId),
                                gGenProfileCtx.FrmProcTime[PhyProfParam2Idx(ParamId)]);
#endif
                        }
                        ParamId++;
                    }
                }
                else
                {
                    rc = RC_PHY_PROF_NO_VALID_DATA_ERROR;
                }
                break;
            default:
                break;
        }
    }
    return rc;
}

