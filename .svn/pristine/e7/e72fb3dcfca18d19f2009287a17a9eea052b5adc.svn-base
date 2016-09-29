//-------------------------------------------------------------------------------------------
/** @file LteBsPhyStats.c
 *
 * @brief Phy Stats
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.18 $
 *
 * COPYRIGHT&copy; 2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#define _LTE_BS_PHY_STATS_C

#include "LteBsPhyStats.h"

#ifndef _WIN32
#include "ext_phy.h"
#include "prototypes.h"
#include "ltertconfig.h"
#include "cmgr.h"
#else
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#endif

#ifndef _WIN32
static LTE_BS_PHY_STATS_CONTROL LteBsPhyStatsControl;
static PLTE_BS_PHY_STATS LteBsPhyStats[PHYSTATS_NUM_CTX];

INLINE PLTE_BS_PHY_STATS_CONTROL LteBsPhyStatsGetControl(void)
{
    return &LteBsPhyStatsControl;
}

PLTE_BS_PHY_STATS LteBsPhyStatsGetCtx(void)
{
    PLTE_BS_PHY_STATS_CONTROL pLteBsPhyStatsControl = LteBsPhyStatsGetControl();

    return LteBsPhyStats[pLteBsPhyStatsControl->CtxNum];
}

U32 LteBsPhyStatsGetSize(void)
{
    return sizeof(LTE_BS_PHY_STATS);
}


PLTE_BS_PHY_STATS LteBsPhyStatsGetOtherCtx(void)
{
    PLTE_BS_PHY_STATS_CONTROL pLteBsPhyStatsControl = LteBsPhyStatsGetControl();

    return LteBsPhyStats[pLteBsPhyStatsControl->OtherCtxNum];
}

U32 LteBsPhyStatsSetCtx(PLTE_BS_PHY_STATS pMem)
{
    PLTE_BS_PHY_STATS_CONTROL pLteBsPhyStatsControl = LteBsPhyStatsGetControl();

    LteBsPhyStats[pLteBsPhyStatsControl->CtxNum] = pMem;

    return 0;
}

static void LteBsPhyStatsSwapCtx(void)
{
    PLTE_BS_PHY_STATS_CONTROL pLteBsPhyStatsControl = LteBsPhyStatsGetControl();

    if (PHYSTATS_NUM_CTX > 1)
    {
        pLteBsPhyStatsControl->CtxNum = 1 - pLteBsPhyStatsControl->CtxNum;
        pLteBsPhyStatsControl->OtherCtxNum = 1 - pLteBsPhyStatsControl->OtherCtxNum;
    }
    return;
}


//-------------------------------------------------------------------------------------------
/** @brief This function initializes the thread needed for Streaming Phy Stats
 *
 *  @param none
 *
 *  @return MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_rx_listrun
 *
**/
//-------------------------------------------------------------------------------------------
MXRC LtePhyInitPhyStatsStreamThread(void)
{
    MXRC rc = MX_OK;
    PLTE_BS_PHY_STATS_CONTROL pLteBsPhyStatsControl = LteBsPhyStatsGetControl();

    _ASSERT_PTR(pLteBsPhyStatsControl);

    rc |= MxCreateThreadEx(PHY_STATS_STREAM_THREAD_ARM, LteBsPhyStreamStats, NULL, pLteBsPhyStatsControl, 0,
                           &pLteBsPhyStatsControl->hPhyStatsStreamThread);

    if (FAILED(rc))
    {
        MxRemoveThread(pLteBsPhyStatsControl->hPhyStatsStreamThread);
        pLteBsPhyStatsControl->hPhyStatsStreamThread = HNULL;
    }

    return rc;
}

//-------------------------------------------------------------------------------------------
/** @brief This function destroys the thread needed for Streaming Phy Stats
 *
 *  @param none
 *
 *  @return MX_OK if success
 *
 *  \ingroup group_lte_phy_control_rt_rx_listrun
 *
**/
//-------------------------------------------------------------------------------------------
MXRC LtePhyDestroyPhyStatsStreamThread(void)
{
    MXRC rc = MX_OK;
    PLTE_BS_PHY_STATS_CONTROL pLteBsPhyStatsControl = LteBsPhyStatsGetControl();

    _ASSERT_PTR(pLteBsPhyStatsControl);

    if (HANDLE_NOT_NULL(pLteBsPhyStatsControl->hPhyStatsStreamThread))
    {
        MxRemoveThread(pLteBsPhyStatsControl->hPhyStatsStreamThread);
        pLteBsPhyStatsControl->hPhyStatsStreamThread = HNULL;
    }

    return rc;
}

//-------------------------------------------------------------------------------------------
/** @brief Schedule Streaming Phy Stats Thread to ARM Core
 *
 *
 *  @param   none
 *
 *  @return   none
 *
 *  \ingroup group_lte_phy_control_rt_mac_phy
 *
**/
//-------------------------------------------------------------------------------------------
void LtePhySchedulePhyStatsStreamThread(void)
{
    PLTE_BS_PHY_STATS_CONTROL pLteBsPhyStatsControl = LteBsPhyStatsGetControl();

    _ASSERT_PTR(pLteBsPhyStatsControl);

    MxScheduleThreadEx(pLteBsPhyStatsControl->hPhyStatsStreamThread, 0, PHY_SMP_THREAD);

    return;
}


UINT32 LteBsPhyStreamStats(LPVOID pParam)
{
#if 0
    PLTE_BS_PHY_STATS_CONTROL pLteBsPhyStatsControl = LteBsPhyStatsGetControl();
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
#if 0
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetOtherCtx();
#endif
    PUINT8 pSendPtr = NULL;
#if 0
    PUINT8 pClearPtr = NULL;
#endif
    UINT32 size = 0, id = 0, send = 0;
    UINT32 tt = GET_TICKS;

    _ASSERT_PTR(pLteBsPhyStatsControl);

    // If Send Unified Diag through UDP
    if (pLteBsPhyStatsControl->SendUnifiedDiag)
    {
        if (pLteBsPhyStatsControl->SendCounter == 0)                                        // Timer Stats
        {
            pSendPtr = (PUINT8)&pLteBsPhyStats->timerStats;
            size = sizeof(LTE_BS_PHY_STATS_TIMER_STATS) - sizeof(LTE_BS_PHY_STATS_IP_PACKET_HEADER);
            id = PHY_STATS_VERSION_TIMER;
            send = 1;
        }
        else if (pLteBsPhyStatsControl->SendCounter == 1)                                   // Rach Stats
        {
            pSendPtr = (PUINT8)&pLteBsPhyStats->rachStats;
            size = sizeof(LTE_BS_PHY_STATS_RACH_STATS) - sizeof(LTE_BS_PHY_STATS_IP_PACKET_HEADER);
            id = PHY_STATS_VERSION_RACH;
            send = 1;
        }
        else if (pLteBsPhyStatsControl->SendCounter < LTE_BS_PHY_STATS_UD_TRACE_COUNTER)    // User Stats
        {
            pSendPtr = (PUINT8)&pLteBsPhyStats->userStats[pLteBsPhyStatsControl->SendCounter-2];
            size = sizeof(LTE_BS_PHY_STATS_USER_STAT) - sizeof(LTE_BS_PHY_STATS_IP_PACKET_HEADER);
            id = PHY_STATS_VERSION_USER;
            send = pLteBsPhyStats->userStats[pLteBsPhyStatsControl->SendCounter-2].used;
        }

        // Send UD Traces
        //if (id != 0 && send)
            //UniDiagOverUDP(id, pSendPtr + sizeof(LTE_BS_PHY_STATS_IP_PACKET_HEADER), size);

        // Increment and set branch control for next entry
        pLteBsPhyStatsControl->SendCounter++;
        if (pLteBsPhyStatsControl->SendCounter >= LTE_BS_PHY_STATS_UD_TRACE_COUNTER)
        {
            pLteBsPhyStatsControl->SendCounter = 0;
            pLteBsPhyStatsControl->SendUnifiedDiag = 0;
#if 0
            pLteBsPhyStatsControl->ClearUnifiedDiag = 1;
#endif
        }

        MLogTask(PID_PHYSTATS_STREAM, RESOURCE_LARM, tt, GET_TICKS);
    }
#if 0
    else if (pLteBsPhyStatsControl->ClearUnifiedDiag)
    {
        if (pLteBsPhyStatsControl->ClearCounter == 0)                                        // Timer Stats
        {
            pClearPtr = (PUINT8)&pLteBsPhyStats->timerStats;
            pClearPtr += sizeof(LTE_BS_PHY_STATS_IP_PACKET_HEADER);
            size = sizeof(LTE_BS_PHY_STATS_TIMER_STATS)
                 - sizeof(LTE_BS_PHY_STATS_IP_PACKET_HEADER);
        }
        else if (pLteBsPhyStatsControl->ClearCounter == 1)                                   // Rach Stats
        {
            pClearPtr = (PUINT8)&pLteBsPhyStats->rachStats;
            pClearPtr += sizeof(LTE_BS_PHY_STATS_IP_PACKET_HEADER);
            size = sizeof(LTE_BS_PHY_STATS_RACH_STATS)
                 - sizeof(LTE_BS_PHY_STATS_IP_PACKET_HEADER);
        }
        else if (pLteBsPhyStatsControl->ClearCounter < LTE_BS_PHY_STATS_UD_TRACE_COUNTER)    // User Stats
        {
            pClearPtr = (PUINT8)&pLteBsPhyStats->userStats[pLteBsPhyStatsControl->SendCounter-2];
            pClearPtr += sizeof(LTE_BS_PHY_STATS_IP_PACKET_HEADER);
            pClearPtr += 4;     // First 4 bytes is used flag and RNTI
            size = sizeof(LTE_BS_PHY_STATS_USER_STAT)
                 - sizeof(LTE_BS_PHY_STATS_IP_PACKET_HEADER)
                 - 4;
        }

        // Clear the data
        if (pClearPtr)
            memset(pClearPtr, 0, size);

        // Increment and set branch control for next entry
        pLteBsPhyStatsControl->ClearCounter++;
        if (pLteBsPhyStatsControl->ClearCounter >= LTE_BS_PHY_STATS_UD_TRACE_COUNTER)
        {
            pLteBsPhyStatsControl->ClearCounter = 0;
            pLteBsPhyStatsControl->ClearUnifiedDiag = 0;
        }
        MLogTask(PID_PHYSTATS_CLEAR, RESOURCE_LARM, tt, GET_TICKS);
    }
#endif
#endif
    return 0;
}


U32 LteBsPhyAddApi(PUINT8 pPayload, UINT8 id, UINT32 size)
{
    ApiParam *pParam = NULL;
    ApiHeader * pApi;

    pApi = (ApiHeader *)pPayload;
    ApiCreate(pApi,
            API_VERSION(API_SUPPORTED_VERSION),
            IID_UNIDIAG,
            UARM_INSTID_HOST,
            API_TYPE_IND,
            DIAG_IND);

    pParam = (ApiParam *) ((PUINT8)pPayload + sizeof (ApiHeader));
    pParam->ParamID     = ((id & 0xFF) | PHY_STATS_VERSION) & 0xFFFF;
    pParam->ParamLength = size;
    pApi->Length = GetParamTotalSize(pParam);

    return (pApi->Length);
}


U32 LteBsPhyCreateApis(PLTE_BS_PHY_STATS pLteBsPhyStats)
{
    UINT32 i, size;
    PUINT8 pPayload;

    // Timer Stat
    pPayload = (PUINT8) &pLteBsPhyStats->timerStats.header;
    pPayload += sizeof(EXTRAUDHEADER);
    size = sizeof(LTE_BS_PHY_STATS_TIMER_STATS)
         - sizeof(LTE_BS_PHY_STATS_IP_PACKET_HEADER);
    LteBsPhyAddApi(pPayload, 0x00, size);

    // Rach Stat
    pPayload = (PUINT8) &pLteBsPhyStats->rachStats.header;
    pPayload += sizeof(EXTRAUDHEADER);
    size = sizeof(LTE_BS_PHY_STATS_RACH_STATS)
         - sizeof(LTE_BS_PHY_STATS_IP_PACKET_HEADER);
    LteBsPhyAddApi(pPayload, 0x01, size);

    // User Stat
    size = sizeof(LTE_BS_PHY_STATS_USER_STAT)
         - sizeof(LTE_BS_PHY_STATS_IP_PACKET_HEADER);
    for (i = 0; i < LTE_BS_PHY_STATS_UNIQUE_INDICES; i++)
    {
        pPayload = (PUINT8) &pLteBsPhyStats->userStats[i].header;
        pPayload += sizeof(EXTRAUDHEADER);
        LteBsPhyAddApi(pPayload, 0x02, size);
    }

    return 0;
}


UINT32 LteBsPhyStatsFromConsole(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    LteBsPhyStatsPrint(NULL);
    return 0;
}




U32 LteBsInitPhyStats(void)
{
    PLTE_BS_PHY_STATS_CONTROL pLteBsPhyStatsControl = LteBsPhyStatsGetControl();
    PLTE_BS_PHY_STATS pLteBsPhyStats;
    UINT32 i, j, ctx;

    // Initialize Structure Conext Variables
    pLteBsPhyStatsControl->CtxNum = 0;
    if (PHYSTATS_NUM_CTX > 1)
        pLteBsPhyStatsControl->OtherCtxNum = 1;
    else
        pLteBsPhyStatsControl->OtherCtxNum = 0;
    pLteBsPhyStatsControl->TTICntr = (UINT32)-1;
    pLteBsPhyStatsControl->CurrCntr = 0;
    pLteBsPhyStatsControl->SendUnifiedDiag = 0;
    pLteBsPhyStatsControl->SendCounter = 0;
    pLteBsPhyStatsControl->ClearUnifiedDiag = 0;
    pLteBsPhyStatsControl->ClearCounter = 0;

    uart_printf("\r\n");
    uart_printf("Phy Stats Parser Version %d\r\n", PHYSTATS_VERSION_NUM);
    uart_printf("---------------------------\r\n");

    for (ctx = 0; ctx < PHYSTATS_NUM_CTX; ctx++)
    {
        pLteBsPhyStats = LteBsPhyStatsGetCtx();

        if (pLteBsPhyStats == NULL)
        {
            pLteBsPhyStats = MemAlloc(sizeof(LTE_BS_PHY_STATS), DDR1_NCNB_MEM, 0);
            LteBsPhyStatsSetCtx(pLteBsPhyStats);
        }

        _ASSERT_PTR(pLteBsPhyStats);
        memset(pLteBsPhyStats, 0, sizeof(LTE_BS_PHY_STATS));

        uart_printf("PhyStats [Ctx %d] at: 0x%08x (size: %d bytes)\r\n", ctx, pLteBsPhyStats, sizeof(LTE_BS_PHY_STATS));

        // Initialize Header
        pLteBsPhyStats->header.Magic = PHYSTATS_HEAD;
        pLteBsPhyStats->header.Version = PHYSTATS_VERSION_NUM;
        pLteBsPhyStats->header.Size = sizeof(LTE_BS_PHY_STATS);

        // Initialize Rach Stats
        pLteBsPhyStats->rachStats.rachTimingOffsetMax = -32768;
        pLteBsPhyStats->rachStats.rachTimingOffsetMin = 32767;
        pLteBsPhyStats->rachStats.rachPeakToAvgPwrRatioMax = -32768;
        pLteBsPhyStats->rachStats.rachPeakToAvgPwrRatioMin = 32767;
        pLteBsPhyStats->rachStats.rachTimingOffsetGoodPwrRatioMax = -32768;
        pLteBsPhyStats->rachStats.rachTimingOffsetGoodPwrRatioMin = 32767;

        // Initialize User Stats
        for (i = 0; i < LTE_BS_PHY_STATS_UNIQUE_INDICES; i++)
        {
            pLteBsPhyStats->userStats[i].used = RNTI_VARS_NOT_USED;

            // PUSCH SINR
            pLteBsPhyStats->userStats[i].sinrPuschMax = 0;
            pLteBsPhyStats->userStats[i].sinrPuschMin = 255;

            // PUSCH Time Advance
            pLteBsPhyStats->userStats[i].timeAdvPuschMax = -32768;
            pLteBsPhyStats->userStats[i].timeAdvPuschMin = 32767;

            // PUCCH SINR and Time Advance
            for (j = 0; j < 6; j++)
            {
                pLteBsPhyStats->userStats[i].sinrPucchMax[j] = 0;
                pLteBsPhyStats->userStats[i].sinrPucchMin[j] = 255;
                pLteBsPhyStats->userStats[i].timeAdvPucchMax[j] = -32768;
                pLteBsPhyStats->userStats[i].timeAdvPucchMin[j] = 32767;
            }

            // PDSCH CQI / RI
            for (j = 0; j < 4; j++)
            {
                pLteBsPhyStats->userStats[i].pdschCqiMax[j] = 0;
                pLteBsPhyStats->userStats[i].pdschCqiMin[j] = 32767;
                pLteBsPhyStats->userStats[i].pdschRiMax[j] = 0;
                pLteBsPhyStats->userStats[i].pdschRiMin[j] = 32767;
            }

            // UL / DL MCS
            for (j = 0; j < 3; j++)
            {
                pLteBsPhyStats->userStats[i].maxMcs[j] = 0;
                pLteBsPhyStats->userStats[i].minMcs[j] = 32767;
            }

            // UL UCI Priodicity - Previous TTI
            pLteBsPhyStats->userStats[i].srPeriodicity[0]    = PHY_STATS_PERIODITICY_NOT_STATED;
            pLteBsPhyStats->userStats[i].dlCqiPeriodicity[0] = PHY_STATS_PERIODITICY_NOT_STATED;
            pLteBsPhyStats->userStats[i].dlRiPeriodicity[0]  = PHY_STATS_PERIODITICY_NOT_STATED;

            // UL UCI Priodicity - Max
            pLteBsPhyStats->userStats[i].srPeriodicity[1]    = 0;
            pLteBsPhyStats->userStats[i].dlCqiPeriodicity[1] = 0;
            pLteBsPhyStats->userStats[i].dlRiPeriodicity[1]  = 0;

            // UL UCI Priodicity - Min
            pLteBsPhyStats->userStats[i].srPeriodicity[2]    = 32767;
            pLteBsPhyStats->userStats[i].dlCqiPeriodicity[2] = 32767;
            pLteBsPhyStats->userStats[i].dlRiPeriodicity[2]  = 32767;

        }

        // Set the MIN stat for all timers to 150,000 = 1usec
        pLteBsPhyStats->timerStats.SubframeTtiIntervals[2]              = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.SlotTtiIntervals[2]                  = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.ValidApiDeliveryIntervals[2]         = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.DLListProcessingTime[2]              = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.ULListProcessingTime[2]              = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.PrachListProcessingTime[2]           = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.SrsListProcessingTime[2]             = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.DLSDUProcessing[2]                   = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.ULSDUPuschProcessing[2]              = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.ULSDUPucchHarqProcessing[2]          = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.ULSDUPuschMuxCtrlHarqProcessing[2]   = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.ULSDUPuschMuxCtrlCqiRiProcessing[2]  = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;

        // Intiialize API headers / ethernet for all of the Stats that would be sent over ethernet to TTBox real-time
        LteBsPhyCreateApis(pLteBsPhyStats);

        LteBsPhyStatsSwapCtx();
    }

    // Initialize the RNIT Strucuture used to store the Mapping from RNTI to Phy User Stats Index
    if (pLteBsPhyStatsControl->pLteBsPhyStatsRntiMapping == NULL)
    {
        pLteBsPhyStatsControl->pLteBsPhyStatsRntiMapping = MemAlloc(sizeof(LTE_BS_PHY_STATS_RNTI_MAPPING), DDR1_NCNB_MEM, 1);
        _ASSERT_PTR(pLteBsPhyStatsControl->pLteBsPhyStatsRntiMapping);
    }

    // Intialize all of the RNTI Maps to not used
    for (i = 0; i < NUM_UNIQUE_RNTI; i++)
        pLteBsPhyStatsControl->pLteBsPhyStatsRntiMapping->rnti_map[i] = RNTI_MAP_NOT_USED;

    for (i = 0; i < LTE_BS_PHY_STATS_UNIQUE_INDICES; i++)
        pLteBsPhyStatsControl->pLteBsPhyStatsRntiMapping->lteBsPhyStatsUsedIndex[i].used = RNTI_VARS_NOT_USED;

    if (gTwoRxMuxControlLists)
        pLteBsPhyStats->PhyStatsCfg = 1;
    else
        pLteBsPhyStats->PhyStatsCfg = 0;

    // Initialize threads used for streaming stats to TTBox
    LtePhyInitPhyStatsStreamThread();

    CMgrRegisterHandler("phystats",      LteBsPhyStatsFromConsole);

    //LteBsPhyStatsSetResetCounter(1000);     // 1000 TTIs = 1 second by default
    //UniDiagSetStreamEnableMask(API_MESSAGE_MASK);
    //UniDiagSetStreamOverUDP(0xEE22010A, 0x5555, NULL);
    return 0;
}


U32 LteBsDestroyPhyStats(void)
{
    PLTE_BS_PHY_STATS_CONTROL pLteBsPhyStatsControl = LteBsPhyStatsGetControl();
    PLTE_BS_PHY_STATS pLteBsPhyStats;
    UINT32 ctx;

    // Reset Structure Conext Variables
    pLteBsPhyStatsControl->CtxNum = 0;
    if (PHYSTATS_NUM_CTX > 1)
        pLteBsPhyStatsControl->OtherCtxNum = 1;
    else
        pLteBsPhyStatsControl->OtherCtxNum = 0;
    pLteBsPhyStatsControl->TTICntr = (UINT32)-1;
    pLteBsPhyStatsControl->CurrCntr = 0;
    pLteBsPhyStatsControl->SendUnifiedDiag = 0;
    pLteBsPhyStatsControl->SendCounter = 0;
    pLteBsPhyStatsControl->ClearUnifiedDiag = 0;
    pLteBsPhyStatsControl->ClearCounter = 0;

    for (ctx = 0; ctx < PHYSTATS_NUM_CTX; ctx++)
    {
        pLteBsPhyStats = LteBsPhyStatsGetCtx();
        _ASSERT_PTR(pLteBsPhyStats);

        //memset(pLteBsPhyStats, 0, sizeof(LTE_BS_PHY_STATS));

        // Free the PhyStats Structure
        //MemFree(pLteBsPhyStats, DDR1_NCNB_MEM);
        //pLteBsPhyStats = NULL;

        LteBsPhyStatsSetCtx(pLteBsPhyStats);

        LteBsPhyStatsSwapCtx();
    }

    // Free the RNIT Strucuture used to store the Mapping from RNTI to Phy User Stats Index
    //MemFree(pLteBsPhyStatsControl->pLteBsPhyStatsRntiMapping, DDR1_NCNB_MEM);

    LtePhyDestroyPhyStatsStreamThread();

    return 0;
}

U32 LteBsResetPhyStats(void)
{
    PLTE_BS_PHY_STATS_CONTROL pLteBsPhyStatsControl = LteBsPhyStatsGetControl();
    PLTE_BS_PHY_STATS pLteBsPhyStats;
    UINT32 i, j;
    UINT32 ctx;

    // Reset Structure Conext Variables
    pLteBsPhyStatsControl->CtxNum = 0;
    if (PHYSTATS_NUM_CTX > 1)
        pLteBsPhyStatsControl->OtherCtxNum = 1;
    else
        pLteBsPhyStatsControl->OtherCtxNum = 0;
    pLteBsPhyStatsControl->TTICntr = (UINT32)-1;
    pLteBsPhyStatsControl->CurrCntr = 0;
    pLteBsPhyStatsControl->SendUnifiedDiag = 0;
    pLteBsPhyStatsControl->SendCounter = 0;
    pLteBsPhyStatsControl->ClearUnifiedDiag = 0;
    pLteBsPhyStatsControl->ClearCounter = 0;

    for (ctx = 0; ctx < PHYSTATS_NUM_CTX; ctx++)
    {
        // Reset the PhyStats Structure
        pLteBsPhyStats = LteBsPhyStatsGetCtx();
        _ASSERT_PTR(pLteBsPhyStats);
        memset(pLteBsPhyStats, 0, sizeof(LTE_BS_PHY_STATS ));

        // Reset Rach Stats
        pLteBsPhyStats->rachStats.rachTimingOffsetMax = -32768;
        pLteBsPhyStats->rachStats.rachTimingOffsetMin = 32767;
        pLteBsPhyStats->rachStats.rachPeakToAvgPwrRatioMax = -32768;
        pLteBsPhyStats->rachStats.rachPeakToAvgPwrRatioMin = 32767;
        pLteBsPhyStats->rachStats.rachTimingOffsetGoodPwrRatioMax = -32768;
        pLteBsPhyStats->rachStats.rachTimingOffsetGoodPwrRatioMin = 32767;

        // Reset User Stats
        for (i = 0; i < LTE_BS_PHY_STATS_UNIQUE_INDICES; i++)
        {
            pLteBsPhyStats->userStats[i].used = RNTI_VARS_NOT_USED;

            // PUSCH SINR
            pLteBsPhyStats->userStats[i].sinrPuschMax = 0;
            pLteBsPhyStats->userStats[i].sinrPuschMin = 255;

            // PUSCH Timing Advance
            pLteBsPhyStats->userStats[i].timeAdvPuschMax = -32768;
            pLteBsPhyStats->userStats[i].timeAdvPuschMin = 32767;

            // PUCCH SINR and Timing Advance
            for (j = 0; j < 6; j++)
            {
                pLteBsPhyStats->userStats[i].sinrPucchMax[j] = 0;
                pLteBsPhyStats->userStats[i].sinrPucchMin[j] = 255;
                pLteBsPhyStats->userStats[i].timeAdvPucchMax[j] = -32768;
                pLteBsPhyStats->userStats[i].timeAdvPucchMin[j] = 32767;
            }

            // DL CQI and RI
            for (j = 0; j < 4; j++)
            {
                pLteBsPhyStats->userStats[i].pdschCqiMax[j] = 0;
                pLteBsPhyStats->userStats[i].pdschCqiMin[j] = 32767;
                pLteBsPhyStats->userStats[i].pdschRiMax[j] = 0;
                pLteBsPhyStats->userStats[i].pdschRiMin[j] = 32767;
            }

            // UL / DL MCS
            for (j = 0; j < 3; j++)
            {
                pLteBsPhyStats->userStats[i].maxMcs[j] = 0;
                pLteBsPhyStats->userStats[i].minMcs[j] = 32767;
            }

            // UL UCI Priodicity - Previous TTI
            pLteBsPhyStats->userStats[i].srPeriodicity[0]    = PHY_STATS_PERIODITICY_NOT_STATED;
            pLteBsPhyStats->userStats[i].dlCqiPeriodicity[0] = PHY_STATS_PERIODITICY_NOT_STATED;
            pLteBsPhyStats->userStats[i].dlRiPeriodicity[0]  = PHY_STATS_PERIODITICY_NOT_STATED;

            // UL UCI Priodicity - Max
            pLteBsPhyStats->userStats[i].srPeriodicity[1]    = 0;
            pLteBsPhyStats->userStats[i].dlCqiPeriodicity[1] = 0;
            pLteBsPhyStats->userStats[i].dlRiPeriodicity[1]  = 0;

            // UL UCI Priodicity - Min
            pLteBsPhyStats->userStats[i].srPeriodicity[2]    = 32767;
            pLteBsPhyStats->userStats[i].dlCqiPeriodicity[2] = 32767;
            pLteBsPhyStats->userStats[i].dlRiPeriodicity[2]  = 32767;

        }

        // Reset the MIN stat for all timers to 150,000 = 1usec
        pLteBsPhyStats->timerStats.SubframeTtiIntervals[2]              = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.SlotTtiIntervals[2]                  = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.ValidApiDeliveryIntervals[2]         = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.DLListProcessingTime[2]              = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.ULListProcessingTime[2]              = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.PrachListProcessingTime[2]           = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.SrsListProcessingTime[2]             = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.DLSDUProcessing[2]                   = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.ULSDUPuschProcessing[2]              = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.ULSDUPucchHarqProcessing[2]          = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.ULSDUPuschMuxCtrlHarqProcessing[2]   = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;
        pLteBsPhyStats->timerStats.ULSDUPuschMuxCtrlCqiRiProcessing[2]  = LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER;

        // System Throughputs
        pLteBsPhyStats->timerStats.systemThroughput[0][0] = _my_sfn;        // DL CW0
        pLteBsPhyStats->timerStats.systemThroughput[1][0] = _my_sfn;        // DL CW1
        pLteBsPhyStats->timerStats.systemThroughput[2][0] = _my_sfn;        // UL
        pLteBsPhyStats->timerStats.systemThroughput[0][1] = 3;        // DL CW0
        pLteBsPhyStats->timerStats.systemThroughput[1][1] = 3;        // DL CW1
        pLteBsPhyStats->timerStats.systemThroughput[2][1] = 3;        // UL


        // Reset API headers / ethernet for all of the Stats that would be sent over ethernet to TTBox real-time
        LteBsPhyCreateApis(pLteBsPhyStats);

        LteBsPhyStatsSwapCtx();
    }

    // Reset all of the RNTI Maps to not used
    for (i = 0; i < NUM_UNIQUE_RNTI; i++)
        pLteBsPhyStatsControl->pLteBsPhyStatsRntiMapping->rnti_map[i] = RNTI_MAP_NOT_USED;

    for (i = 0; i < LTE_BS_PHY_STATS_UNIQUE_INDICES; i++)
        pLteBsPhyStatsControl->pLteBsPhyStatsRntiMapping->lteBsPhyStatsUsedIndex[i].used = RNTI_VARS_NOT_USED;

    return 0;
}
#endif  // _WIN32

#ifdef LTE_BS_PHY_STATS_ENABLED

#ifndef _WIN32
void LteBsPhyStatsSetResetCounter(UINT32 counter)
{
    PLTE_BS_PHY_STATS_CONTROL pLteBsPhyStatsControl = LteBsPhyStatsGetControl();

    pLteBsPhyStatsControl->TTICntr = counter;
    return;
}

void LteBsPhyStatsIncrementCounter(void)
{
    PLTE_BS_PHY_STATS_CONTROL pLteBsPhyStatsControl = LteBsPhyStatsGetControl();

    if (pLteBsPhyStatsControl->SendUnifiedDiag || pLteBsPhyStatsControl->ClearUnifiedDiag)
    {
        // Dispatch thread for sending Stats through Unfiied Diagnostics
        LtePhySchedulePhyStatsStreamThread();
    }


    // If user has set a TTI Counter, then start to increment
    if (pLteBsPhyStatsControl->TTICntr != ((UINT32)-1))
    {
        pLteBsPhyStatsControl->CurrCntr++;
        if (pLteBsPhyStatsControl->CurrCntr == pLteBsPhyStatsControl->TTICntr)
        {
            pLteBsPhyStatsControl->CurrCntr = 0;
            pLteBsPhyStatsControl->SendUnifiedDiag = 1;     // Send Unified Diags in next TTI (wait for current TTI to complete writing everything)
            LteBsPhyStatsSwapCtx();
        }
    }

    return;
}

U32 LteBsPhyStatsUpdateRexStats(void)
{
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    RexServDiagCmd(REX_DIAG_CMD_GET_STAT_INFO, 0, 0, &pLteBsPhyStats->deviceStats.rexServStatInfo, sizeof (REX_SERV_STAT_INFO));
    return 0;
}

PLTE_BS_PHY_STATS_USER_STAT LteBsPhyStatsGetRntiId(U32 RNTI)
{
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    PLTE_BS_PHY_STATS pLteBsPhyStatsOther = LteBsPhyStatsGetOtherCtx();

    PLTE_BS_PHY_STATS_CONTROL pLteBsPhyStatsControl = LteBsPhyStatsGetControl();
    PLTE_BS_PHY_STATS_RNTI_MAPPING pRntiMapping = pLteBsPhyStatsControl->pLteBsPhyStatsRntiMapping;

    PLTE_BS_PHY_STATS_USER_STAT pUserStat, pUserStatOther;
    U32 i;

    // Send out the last one so code does not crash. This is invalid location
    if (RNTI > 65535)
    {
        pUserStat = &pLteBsPhyStats->userStats[LTE_BS_PHY_STATS_UNIQUE_INDICES-1];
        return pUserStat;
    }

    if (pRntiMapping->rnti_map[RNTI] == RNTI_MAP_NOT_USED)
    {
        for (i = 0; i < LTE_BS_PHY_STATS_UNIQUE_INDICES-1; i++)     // last 1 will be used for excess RNTIs
        {
            if (pRntiMapping->lteBsPhyStatsUsedIndex[i].used == RNTI_VARS_NOT_USED)
            {
                pRntiMapping->lteBsPhyStatsUsedIndex[i].used = RNTI_VARS_USED;
                pRntiMapping->lteBsPhyStatsUsedIndex[i].RNTI = RNTI;
                pRntiMapping->rnti_map[RNTI] = i;

                pUserStat = &pLteBsPhyStats->userStats[i];
                pUserStat->used = i+1;
                pUserStat->RNTI = RNTI;
                pUserStat->DLThroughput[0][0] = _my_sfn;
                pUserStat->DLThroughput[1][0] = _my_sfn;
                pUserStat->ULThroughput[0]    = _my_sfn;
                pUserStat->DLThroughput[0][1] = 3;
                pUserStat->DLThroughput[1][1] = 3;
                pUserStat->ULThroughput[1]    = 3;

                // Update the other contexts User Stats as well
                pUserStatOther = &pLteBsPhyStatsOther->userStats[i];
                pUserStatOther->used = i+1;
                pUserStatOther->RNTI = RNTI;
                pUserStatOther->DLThroughput[0][0] = _my_sfn;
                pUserStatOther->DLThroughput[1][0] = _my_sfn;
                pUserStatOther->ULThroughput[0]    = _my_sfn;
                pUserStatOther->DLThroughput[0][1] = 3;
                pUserStatOther->DLThroughput[1][1] = 3;
                pUserStatOther->ULThroughput[1]    = 3;

                break;
            }
        }
    }
    else
    {
        i = pRntiMapping->rnti_map[RNTI];
    }

    pUserStat = &pLteBsPhyStats->userStats[i];

    return (pUserStat);
}

U32 LteBsPhyStatsThroughput(PLTE_BS_PHY_STATS_USER_STAT pUserStat, U32 direction, U32 size, U32 cwId)
{
    U32 diff, currBin, currBinCnt;
    U32 *Throughput;
    U64 *pSize;

    if (direction == DL)
    {
        Throughput = pUserStat->DLThroughput[cwId];
        pSize = (U64*)pUserStat->totalDlData;
        *pSize += (size);
    }
    else if (direction == UL)
    {
        Throughput = pUserStat->ULThroughput;
        pSize = (U64*)pUserStat->totalUlData;
        *pSize += (size);
    }
    else if(direction == SYSTEM)
    {
        Throughput = (U32*)pUserStat;
        pSize = (U64*)&Throughput[8];
        *pSize += (size);
    }
    else
    {
        return 1;
    }

    diff = _my_sfn - Throughput[0];         // Difference between current SFN and last arrived SFN
    currBin = Throughput[1];                // Current Bin last size was added
    currBinCnt = Throughput[2];             // Counter at current Bin

    if (diff >= 5000)                       // Zero out all stats
    {
        for (currBin = 3; currBin < 8; currBin++)
            Throughput[currBin] = 0;
        currBin = 3;
        currBinCnt = 0;
    }
    else
    {
        while(diff >= 1000)                 // 1000 TTIs = 1 second
        {
            currBin++;
            if (currBin == 8)
                currBin = 3;
            Throughput[currBin] = 0;
            diff -= 1000;
        }

        currBinCnt += diff;
        if (currBinCnt >= 1000)
        {
            currBin++;
            if (currBin == 8)
                currBin = 3;
            Throughput[currBin] = 0;
            currBinCnt -= 1000;
        }
    }

    Throughput[0] = _my_sfn;
    Throughput[1] = currBin;
    Throughput[2] = currBinCnt;
    Throughput[currBin] += size;

    return 0;
}


U32 LteBsPhyStatsTimer(U32 *storePtr, U32 startTime, U32 stopTime)
{
    U32 diffTime = 0;
    U64 *pSum;

    if (LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_TMRSTS))
    {
        if (stopTime >= startTime)    // Because of Timer circling around
            diffTime = (stopTime - startTime)/150;
        else
            diffTime = (0xFFFFFFFF - (startTime - stopTime))/150;

        storePtr[0]++;
        if (storePtr[1] < diffTime)
            storePtr[1] = diffTime;
        if (storePtr[2] > diffTime)
            storePtr[2] = diffTime;

        pSum = (U64*)&storePtr[3];      // Store Sum as 64 bit
        *pSum += diffTime;
    }

    return diffTime;
}

U32 LteBsPhyStatsRachStats(U16 timingOffset, S16 peakToAvgPowerRatio)
{
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();

    S32 rachCalc;
    S32 PwrCalc;

    // All Timing Offset from Rach
    if (timingOffset > pLteBsPhyStats->rachStats.rachTimingOffsetMax)
        pLteBsPhyStats->rachStats.rachTimingOffsetMax = timingOffset;

    if (timingOffset < pLteBsPhyStats->rachStats.rachTimingOffsetMin)
        pLteBsPhyStats->rachStats.rachTimingOffsetMin = timingOffset;

    rachCalc = (S32)timingOffset/2;
    if (rachCalc <= 0)
    {
        rachCalc = 0;
    }
    else if (rachCalc >= 11)
    {
        rachCalc = 11;
    }
    pLteBsPhyStats->rachStats.rachTimingOffsetBins[rachCalc]++;

    // Peak to Average Power Ratio from Rach
    if (peakToAvgPowerRatio > pLteBsPhyStats->rachStats.rachPeakToAvgPwrRatioMax)
        pLteBsPhyStats->rachStats.rachPeakToAvgPwrRatioMax = peakToAvgPowerRatio;

    if (peakToAvgPowerRatio < pLteBsPhyStats->rachStats.rachPeakToAvgPwrRatioMin)
        pLteBsPhyStats->rachStats.rachPeakToAvgPwrRatioMin = peakToAvgPowerRatio;

    PwrCalc = (S32)peakToAvgPowerRatio/15 + 1;
    if (PwrCalc <= 0)
    {
        PwrCalc = 0;
    }
    else if (PwrCalc >= 11)
    {
        PwrCalc = 11;
    }
    pLteBsPhyStats->rachStats.rachPeakToAvgPwrRatioBins[PwrCalc]++;

    // Timing Offset from Rach if Peak to Average Power Ratio from Rach is beyond threshold
    if (peakToAvgPowerRatio >= PRACH_DETECTION_THRESHOLD_GOOD)
    {
        if (timingOffset > pLteBsPhyStats->rachStats.rachTimingOffsetGoodPwrRatioMax)
            pLteBsPhyStats->rachStats.rachTimingOffsetGoodPwrRatioMax = timingOffset;

        if (timingOffset < pLteBsPhyStats->rachStats.rachTimingOffsetGoodPwrRatioMin)
            pLteBsPhyStats->rachStats.rachTimingOffsetGoodPwrRatioMin = timingOffset;

        pLteBsPhyStats->rachStats.rachTimingOffsetGoodPwrRatioBins[rachCalc]++;
    }

    return 0;
}

U32 LteBsPhyStatsPdschCqiRi(PLTE_BS_PHY_STATS_USER_STAT pUserStat, U8 cqiRi, U32 ulType, U32 pucchType, U32 isCqi)
{
    U32 index = 5;
    S32 cqiRiCalc;
    U32 *pdschCqiRiMax, *pdschCqiRiMin, *pdschCqiRi;

    if(ulType == PUSCH)
    {
        index = 0;
    }
    else if (ulType == PUCCH)
    {
        index = (pucchType - FORMAT2) + 1;
    }

    if (isCqi)
    {
        pdschCqiRiMax = &pUserStat->pdschCqiMax[index];
        pdschCqiRiMin = &pUserStat->pdschCqiMin[index];
        pdschCqiRi = pUserStat->pdschCqi[index];
    }
    else
    {
        pdschCqiRiMax = &pUserStat->pdschRiMax[index];
        pdschCqiRiMin = &pUserStat->pdschRiMin[index];
        pdschCqiRi = pUserStat->pdschRi[index];
    }

    if (index != 5)
    {
        if (cqiRi > *pdschCqiRiMax)
            *pdschCqiRiMax = cqiRi;

        if (cqiRi < *pdschCqiRiMin)
            *pdschCqiRiMin = cqiRi;

        if (isCqi)
        {
            cqiRiCalc = cqiRi - 4;
            if (cqiRiCalc <= 0)
            {
                cqiRiCalc = 0;
            }
            else if (cqiRiCalc >= 11)
            {
                cqiRiCalc = 11;
            }
        }
        else
        {
            cqiRiCalc = cqiRi;
            if (cqiRiCalc <= 0)
            {
                cqiRiCalc = 0;
            }
            else if (cqiRiCalc >= 2)
            {
                cqiRiCalc = 2;
            }
        }
        pdschCqiRi[cqiRiCalc]++;
    }

    return 0;
}

U32 LteBsPhyStatsMcs(PLTE_BS_PHY_STATS_USER_STAT pUserStat, U8 mcs, U32 mcsIndex)
{
    S32 mcsCalc;

    if (mcs > pUserStat->maxMcs[mcsIndex])
        pUserStat->maxMcs[mcsIndex] = mcs;

    if (mcs < pUserStat->minMcs[mcsIndex])
        pUserStat->minMcs[mcsIndex] = mcs;

    mcsCalc = mcs - 14;
    if (mcs <= 14)
    {
        mcsCalc = 0;
    }
    else if (mcs > 27)
    {
        mcsCalc = 14;
    }
    pUserStat->mcs[mcsIndex][mcsCalc]++;

    return 0;
}


U32 LteBsPhyStatsCalcSinrTmAdv(PLTE_BS_PHY_STATS_USER_STAT pUserStat, U8 sinr, S16 tmAdv, U32 ulType, U32 pucchType)
{
    U32 sinrBin;
    S32 sinrCalc;
    S32 tmAdvCalc;

    if(ulType == PUSCH)
    {
        if (sinr > pUserStat->sinrPuschMax)
            pUserStat->sinrPuschMax = sinr;

        if (sinr < pUserStat->sinrPuschMin)
            pUserStat->sinrPuschMin = sinr;

        sinrCalc = (S32)(sinr - 116)/2;       // -116 = -6dB
        if (sinrCalc <= 0)
        {
            sinrBin = 0;
        }
        else if (sinrCalc >= 30)
        {
            sinrBin = 11;
        }
        else
        {
            sinrBin = (sinrCalc / 3) + 1;     // 3 dB Steps, 1 because index 0 is for anything lesser
        }
        pUserStat->sinrPuschBins[sinrBin]++;

        if (tmAdv > pUserStat->timeAdvPuschMax)
            pUserStat->timeAdvPuschMax = tmAdv;

        if (tmAdv < pUserStat->timeAdvPuschMin)
            pUserStat->timeAdvPuschMin = tmAdv;

        tmAdvCalc = (S32)tmAdv - 25 + 1;
        if (tmAdvCalc <= 0)
        {
            tmAdvCalc = 0;
        }
        else if (tmAdvCalc >= 11)
        {
            tmAdvCalc = 11;
        }
        pUserStat->timeAdvPuschBins[tmAdvCalc]++;

    }
    else if (ulType == PUCCH)
    {
        if (sinr > pUserStat->sinrPucchMax[pucchType])
            pUserStat->sinrPucchMax[pucchType] = sinr;

        if (sinr < pUserStat->sinrPucchMin[pucchType])
            pUserStat->sinrPucchMin[pucchType] = sinr;

        sinrCalc = (S32)(sinr - 116)/2;     // -116 = -6dB
        if (sinrCalc <= 0)
        {
            sinrBin = 0;
        }
        else if (sinrCalc >= 30)
        {
            sinrBin = 11;
        }
        else
        {
            sinrBin = (sinrCalc / 3) + 1;     // 3 dB Steps, 1 because index 0 is for anything lesser
        }
        pUserStat->sinrPucchBins[pucchType][sinrBin]++;

        if (tmAdv > pUserStat->timeAdvPucchMax[pucchType])
            pUserStat->timeAdvPucchMax[pucchType] = tmAdv;

        if (tmAdv < pUserStat->timeAdvPucchMin[pucchType])
            pUserStat->timeAdvPucchMin[pucchType] = tmAdv;

        tmAdvCalc = (S32)tmAdv - 25 + 1;
        if (tmAdvCalc <= 0)
        {
            tmAdvCalc = 0;
        }
        else if (tmAdvCalc >= 11)
        {
            tmAdvCalc = 11;
        }
        pUserStat->timeAdvPucchBins[pucchType][tmAdvCalc]++;
    }

    return 0;
}

U32 LteBsPhyStatsUlUciPeriodicity(PLTE_BS_PHY_STATS_USER_STAT pUserStat, U32 type, U32 currentTti)
{
    U32 diff;
    U32 prevTti;
    U32 *arr = NULL;

    switch(type)
    {
        case PHY_STATS_SR_PERIODICITY:
            arr = pUserStat->srPeriodicity;
        break;
        case PHY_STATS_DL_CQI_PERIODICITY:
            arr = pUserStat->dlCqiPeriodicity;
        break;
        case PHY_STATS_DL_RI_PERIODICITY:
            arr = pUserStat->dlRiPeriodicity;
        break;
        default:
            return 0;
    }


    if (arr[0] != PHY_STATS_PERIODITICY_NOT_STATED)
    {
        prevTti = arr[0];
        if (prevTti > currentTti)
            diff = (currentTti + 10240) - prevTti;
        else
            diff = currentTti - prevTti;
        if (arr[1] < diff)
            arr[1] = diff;
        if (arr[2] > diff)
            arr[2] = diff;
    }
    arr[0] = currentTti;

    return 0;

}


PLTE_BS_PHY_STATS_SUBFRAME_CTX LteBsPhyStatsTimeHistogramIncrement(U32 type, U16 numDataChs, U16 numCtrlChs, U16 sfn)
{
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
    PLTE_BS_PHY_STATS_SUBFRAME_CTX pCtx = NULL;
    U32 *pPtr = NULL;
    U16 totChs = numDataChs + numCtrlChs;

    if (type == DL)
    {
        pPtr = &pLteBsPhyStats->timeHistogram.maxDLChannelsPerSubframe;
        pLteBsPhyStats->timeHistogram.currDlCtx++;
        if (pLteBsPhyStats->timeHistogram.currDlCtx >= LTE_BS_PHY_STATS_TIME_HISTOGRAM_DURATION)
        {
            pLteBsPhyStats->timeHistogram.currDlCtx = 0;
            pLteBsPhyStats->timeHistogram.dlCtxCircle = 1;
        }

        pCtx = &pLteBsPhyStats->timeHistogram.subframeCtx[pLteBsPhyStats->timeHistogram.currDlCtx];
        pLteBsPhyStats->timeHistogram.pCurrDlCtxPtr = pCtx;
        pCtx->dlSfn = sfn;
        pCtx->numDlChs = totChs;
        pCtx->pDlCurrCtx = pCtx->dlCtx;
    }
    else if (type == UL)
    {
        pPtr = &pLteBsPhyStats->timeHistogram.maxULChannelsPerSubframe;
        pLteBsPhyStats->timeHistogram.currUlCtx++;
        if (pLteBsPhyStats->timeHistogram.currUlCtx >= LTE_BS_PHY_STATS_TIME_HISTOGRAM_DURATION)
        {
            pLteBsPhyStats->timeHistogram.currUlCtx = 0;
            pLteBsPhyStats->timeHistogram.ulCtxCircle = 1;
        }

        pCtx = &pLteBsPhyStats->timeHistogram.subframeCtx[pLteBsPhyStats->timeHistogram.currUlCtx];
        pLteBsPhyStats->timeHistogram.pCurrUlCtxPtr = pCtx;
        pCtx->ulSfn = sfn;
        pCtx->numUlChs = totChs;
        pCtx->pUlCurrCtx = pCtx->ulCtx;
    }
    else
    {
        return NULL;
    }

    if (pPtr[0] < totChs)
        pPtr[0] = totChs;

    if (pPtr[1] < numDataChs)
        pPtr[1] = numDataChs;

    if (pPtr[2] < numCtrlChs)
        pPtr[2] = numCtrlChs;

    return pCtx;
}

#endif  // _WIN32

U32 LteBsPhyStatsAddRnti(U32 *RNTIMap, U32 *totalRNTIs, U32 RNTI)
{
    UINT32 i;
    UINT32 found = 0;

    for (i = 0; i < *totalRNTIs; i++)
    {
        if (RNTIMap[i] == RNTI)
        {
            found = 1;
            break;
        }
    }

    if (found == 0)
    {
        RNTIMap[(*totalRNTIs)++] = RNTI;
    }

    return found;
}

U32 LteBsPhyStatsSortRntiMap(U32 *RNTIMap, U32 totalRNTIs, PLTE_BS_PHY_STATS_USER_STAT *pUserStatSave)
{
    UINT32 i, j;
    U32 storeRnti;
    PLTE_BS_PHY_STATS_USER_STAT pUserStat;

    for (i = 0; i < totalRNTIs; i++)
    {
        for (j = i+1; j < totalRNTIs; j++)
        {
            if (RNTIMap[i] > RNTIMap[j])
            {
                storeRnti = RNTIMap[i];
                RNTIMap[i] = RNTIMap[j];
                RNTIMap[j] = storeRnti;

                if (pUserStatSave)
                {
                    pUserStat = pUserStatSave[i];
                    pUserStatSave[i] = pUserStatSave[j];
                    pUserStatSave[j] = pUserStat;
                }
            }
        }
    }

    return 0;
}


U32 LteBsPhyStatsGetRntiMapIndex(U32 *RNTIMap, U32 totalRNTIs, U32 RNTI)
{
    U32 i, index = 0;

    for (i = 0; i < totalRNTIs; i++)
    {
        if (RNTIMap[i] == RNTI)
        {
            index = i;
            break;
        }
    }

    return index;
}

void LteBsPhyStatsConstructULCqiPmiHarqRiSrString(char *pString, U32 val)
{

    pString[0] = ((val & PHYSTATS_CQI_PMI_PT) ? 'C' : ' ');
    pString[1] = ((val & PHYSTATS_HARQ_PT)    ? 'H' : ' ');
    pString[2] = ((val & PHYSTATS_RI_PT)      ? 'R' : ' ');
    pString[3] = ((val & PHYSTATS_SR_PT)      ? 'S' : ' ');

    pString[4] = '\0';
}

void LteBsPhyStatsConstructString(U32 type, char *pString, PLTE_BS_PHY_STATS_UE_CTX pUeStatsCtx)
{
    char formatType[3];
    char cqiPmiHarqRiSrPt[5];

    if (pUeStatsCtx == NULL)
    {
        if (type == UL)
            sprintf(pString, "            (      ) (         )");
        else
            sprintf(pString, "            (      ) (      )");
    }
    else
    {
        if (type == DL)
        {
            switch (pUeStatsCtx->type)
            {
                case PBCH:
                    strcpy(formatType, " B");
                    break;
                case PDSCH:
                    strcpy(formatType, "DS");
                    break;
                case PDCCH:
                    strcpy(formatType, "DC");
                    break;
                case PDCCH_DCI0:
                    strcpy(formatType, "D0");
                    break;
                default :
                    strcpy(formatType, "BD");
                    break;
            }

            if (pUeStatsCtx->type == PDSCH)
            {
                if (pUeStatsCtx->numCw == 1)
                    sprintf(pString, " %s %4d %2d (%1d %2d %1d) (      )",
                    formatType, pUeStatsCtx->numCw, pUeStatsCtx->numRb,
                    pUeStatsCtx->rV0, pUeStatsCtx->mcs0, pUeStatsCtx->Qm0);
                else
                    sprintf(pString, " %s %4d %2d (%1d %2d %1d) (%1d %2d %1d)",
                    formatType, pUeStatsCtx->numCw, pUeStatsCtx->numRb,
                    pUeStatsCtx->rV0, pUeStatsCtx->mcs0, pUeStatsCtx->Qm0,
                    pUeStatsCtx->rV1, pUeStatsCtx->mcs1, pUeStatsCtx->Qm1);
            }
            else if (pUeStatsCtx->type == PBCH)
            {
                if (pUeStatsCtx->numCw == 1)
                    sprintf(pString, " %s %4d    (%1d %2d %1d) (      )",
                    formatType, pUeStatsCtx->numCw,
                    pUeStatsCtx->rV0, pUeStatsCtx->mcs0, pUeStatsCtx->Qm0);
                else
                    sprintf(pString, " %s %4d    (%1d %2d %1d) (%1d %2d %1d)",
                    formatType, pUeStatsCtx->numCw,
                    pUeStatsCtx->rV0, pUeStatsCtx->mcs0, pUeStatsCtx->Qm0,
                    pUeStatsCtx->rV1, pUeStatsCtx->mcs1, pUeStatsCtx->Qm1);
            }
            else if (pUeStatsCtx->type == PDCCH || pUeStatsCtx->type == PDCCH_DCI0)
            {
                sprintf(pString, " %s      %2d (      ) (      )", formatType, pUeStatsCtx->numRb);
            }
            else
            {
                sprintf(pString, " %s         (      ) (      )", formatType);
            }

        }
        else if (type == UL)
        {
            LteBsPhyStatsConstructULCqiPmiHarqRiSrString(cqiPmiHarqRiSrPt, pUeStatsCtx->cqiPmiHarqRiSrPt);
            if ((pUeStatsCtx->type >> 4) == PUSCH)
            {
                strcpy(formatType, "US");
                sprintf(pString, " %s %s %2d (%1d %2d %1d) (%2d %2d %3d)",
                    formatType, cqiPmiHarqRiSrPt, pUeStatsCtx->numRb,
                    pUeStatsCtx->rV0, pUeStatsCtx->mcs0, pUeStatsCtx->Qm0,
                    pUeStatsCtx->rV1, pUeStatsCtx->mcs1, ((pUeStatsCtx->Qm1-128)/2));
            }
            else if ((pUeStatsCtx->type >> 4) == PRACH)
            {
                strcpy(formatType, "RA");
                sprintf(pString, " %s %s %2d (      ) (         )",
                    formatType, cqiPmiHarqRiSrPt, pUeStatsCtx->numRb);
            }
            else if ((pUeStatsCtx->type >> 4) == PUCCH)
            {
                switch(pUeStatsCtx->type & 0xF)
                {
                    case FORMAT1:
                        strcpy(formatType, " 1");
                        break;
                    case FORMAT1A:
                        strcpy(formatType, "1A");
                        break;
                    case FORMAT1B:
                        strcpy(formatType, "1B");
                        break;
                    case FORMAT2:
                        strcpy(formatType, " 2");
                        break;
                    case FORMAT2A:
                        strcpy(formatType, "2A");
                        break;
                    case FORMAT2B:
                        strcpy(formatType, "2B");
                        break;
                    default:
                        strcpy(formatType, "BD");
                        break;
                }

                sprintf(pString, " %s %s %2d (%1d %2d  ) (%02x %2d %3d)",
                    formatType, cqiPmiHarqRiSrPt, pUeStatsCtx->numRb,
                    pUeStatsCtx->rV0, pUeStatsCtx->mcs0,
                    pUeStatsCtx->rV1, pUeStatsCtx->mcs1, ((pUeStatsCtx->Qm1-128)/2));

            }
            else
            {
                sprintf(pString, "            (      ) (         )");
            }
        }
    }
}

void LteBsPhyStatsPrintTimerStats(char *text, U32* data)
{
    U64 *pTotal = (U64*)&data[3];
    if (data[0] > 0)
    {
        uart_printf("%s%9d %9d %9d %9llu\r\n", text, data[0], data[1], data[2], ((*pTotal)/data[0]));
    }
    else
    {
        uart_printf("%s      N/A       N/A       N/A       N/A\r\n", text);
    }

    return;
}

void LteBsPhyStatsPrintTotalBytesTransferred(U32 type, U64 size, U8* dispStore)
{
    U32 i, len, j, extra;
    char disp[30], dispFinal[30];

    sprintf(disp, "%llu", size);
    len = strlen(disp);

    extra = len % 3;
    if (extra== 0)
    {
        extra = 3;
    }

    for (i = 0, j = 0; i < len; i++)
    {
        dispFinal[j++] = disp[i];
        extra--;
        if (extra == 0)
        {
            dispFinal[j++] = ',';
            extra = 3;
        }
    }
    dispFinal[j-1] = '\0';

    if (dispStore == NULL)
    {
        if (type == DL)
        {
            uart_printf("Total DL Data transferred:                                                      %s Bytes\r\n", dispFinal);
        }
        else
        {
            uart_printf("Total UL Data transferred:                                                      %s Bytes\r\n", dispFinal);
        }
    }
    else
    {
        // Store in 30 long string for Tabluar format
        len = strlen(dispFinal);
        for (i = 0; i < 30-len-1; i++)
        {
            dispStore[i] = ' ';
        }
        for (j = 0; i < 29; i++, j++)
        {
            dispStore[i] = dispFinal[j];
        }
        dispStore[i] = '\0';

    }

    return;
}

void LteBsPhyStatsUserStatPrintDelimeter(U32 totalRNTIs, U8 delimeter)
{
    U32 i, j;

    uart_printf("|");
    for (j = 0; j < 30; j++)
        uart_printf("%c", delimeter);
    uart_printf("|");

    for (i = 0; i < totalRNTIs; i++)
    {
        for (j = 0; j < 30; j++)
            uart_printf("%c", delimeter);
        uart_printf("|");
    }
    uart_printf("\r\n");
}


MXRC LteBsPhyStatsPrint(PLTE_BS_PHY_STATS pLteBsPhyStatsInput)
{
#ifndef _WIN32
    PLTE_BS_PHY_STATS pLteBsPhyStats = LteBsPhyStatsGetCtx();
#else
    PLTE_BS_PHY_STATS pLteBsPhyStats = pLteBsPhyStatsInput;
#endif
    PLTE_BS_TIME_HISTOGRAM pTimeHistogram = &pLteBsPhyStats->timeHistogram;
    PLTE_BS_PHY_STATS_UE_CTX pUeStatsCtx;
    U32 i, j, k, userCnt = 0, stop;
    PLTE_BS_PHY_STATS_USER_STAT pUserStat;
    PLTE_BS_PHY_STATS_USER_STAT pUserStatSave[LTE_BS_PHY_STATS_UNIQUE_INDICES];

    REX_SERV_STAT_INFO *pRexServStatInfo = &pLteBsPhyStats->deviceStats.rexServStatInfo;

    U32 *SubframeTtiIntervals   = pLteBsPhyStats->timerStats.SubframeTtiIntervals;
    U32 *SlotTtiIntervals       = pLteBsPhyStats->timerStats.SlotTtiIntervals;
    U32 *ValidApiDeliveryInt    = pLteBsPhyStats->timerStats.ValidApiDeliveryIntervals;
    U32 *DLList                 = pLteBsPhyStats->timerStats.DLListProcessingTime;
    U32 *ULList                 = pLteBsPhyStats->timerStats.ULListProcessingTime;
    U32 *PRACHList              = pLteBsPhyStats->timerStats.PrachListProcessingTime;
    U32 *SRSList                = pLteBsPhyStats->timerStats.SrsListProcessingTime;
    U32 *DLSDU                  = pLteBsPhyStats->timerStats.DLSDUProcessing;
    U32 *ULPuschSDU             = pLteBsPhyStats->timerStats.ULSDUPuschProcessing;
    U32 *ULPucchHarqSDU         = pLteBsPhyStats->timerStats.ULSDUPucchHarqProcessing;
    U32 *ULPuschMuxCtrlHarqSDU  = pLteBsPhyStats->timerStats.ULSDUPuschMuxCtrlHarqProcessing;
    U32 *ULPuschMuxCtrlCqiRiSDU = pLteBsPhyStats->timerStats.ULSDUPuschMuxCtrlCqiRiProcessing;
    U64 sum, sum1, sum2, sumStats[15], *pSize;
    U32 pucchFormat, pucchNum, ulPresent, index, maxStat, minStat;
    U32 dlCqiPresent, dlRiPresent, ulSinrPresent, ulTmgAdvPresent, mcsPresent[3], mcsCount;
    U32 puschAverageCqiPresent, pucchAverageCqiPresent, uciPeriodicity;
    char pucchChar;
    U32 RNTIMap[LTE_BS_PHY_STATS_MAX_CHANNELS_PER_TTI*4], totalRNTIs;
    U32 RNTIPresent[LTE_BS_PHY_STATS_MAX_CHANNELS_PER_TTI*4];
    char pString[1024];
    U32 frNum, sfNum, numData, numCtrl, numDci, foundBch;
    U32 *pThroughput0, *pThroughput1, *pThroughput2;
    U32 lastThreeThroughput[3];
    U8 storeDisp[30];

    U32 phyStatsEnabledTimer =          LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_TMRSTS);
    U32 phyStatsEnabledApi =            LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_APISTATS);
    U32 phyStatsEnabledRach =           LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_RACH_STATS);
    U32 phyStatsEnabledTimeHist =       LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_TIME_HISTOGRAM);
    U32 phyStatsEnabledPrint =          LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_PHYSTATS_PRINT);
    U32 phystatsEnabledUsrStat =        LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_USRSTS);

    if (phyStatsEnabledPrint)
    {

        if (phyStatsEnabledTimer)
        {
            MxDelayTicks(150000000);
            uart_printf("\r\n\r\n\r\n");
            uart_printf("-------------------------------------------------------------------------------------------------------\r\n");
            uart_printf("PHY-Stats:\r\n");
            uart_printf("----------\r\n");
            uart_printf("Number of TTIs Run:                %d\r\n", pLteBsPhyStats->timerStats.numSubframes-1);
            uart_printf("Number of Missed DL TTIs:          %d\r\n", pLteBsPhyStats->timerStats.numDlListMissed ? pLteBsPhyStats->timerStats.numDlListMissed-1 : 0);
            uart_printf("Number of Missed UL TTIs:          %d\r\n", pLteBsPhyStats->timerStats.numUlListMissed-1);
            uart_printf("Number of Missed PRACH TTIs:       %d\r\n", pLteBsPhyStats->timerStats.numPrachListMissed-1);
            uart_printf("Number of Missed SRS TTIs:         %d\r\n", pLteBsPhyStats->timerStats.numSrsListMissed-1);
            uart_printf("\r\n");

            uart_printf("Timer Stats:\r\n");
            uart_printf("------------\r\n\r\n");
            uart_printf("                                              Total    Max(us)   Min(us)   Avg(us)\r\n");
            LteBsPhyStatsPrintTimerStats("Subframe Irq TTI Internal time:           \0", SubframeTtiIntervals);
            LteBsPhyStatsPrintTimerStats("Inter TTI Irq Internal time:              \0", SlotTtiIntervals);
            LteBsPhyStatsPrintTimerStats("Valid API Delivery Internal time:         \0", ValidApiDeliveryInt);
            LteBsPhyStatsPrintTimerStats("DL SDU Processing time:                   \0", DLSDU);
            LteBsPhyStatsPrintTimerStats("DL List Processing time (DL SFN Done):    \0", DLList);
            LteBsPhyStatsPrintTimerStats("UL List Processing time:                  \0", ULList);
            LteBsPhyStatsPrintTimerStats("UL PUCCH HARQ/CQI/RI SDU Processing time: \0", ULPucchHarqSDU);
            LteBsPhyStatsPrintTimerStats("UL PUSCH SDU Processing time:             \0", ULPuschSDU);
            if (pLteBsPhyStats->PhyStatsCfg)
            {
                LteBsPhyStatsPrintTimerStats("UL PUSCH HARQ SDU Processing time:        \0", ULPuschMuxCtrlHarqSDU);
                LteBsPhyStatsPrintTimerStats("UL PUSCH CQI/RI SDU Processing time:      \0", ULPuschMuxCtrlCqiRiSDU);
            }
            else
            {
                LteBsPhyStatsPrintTimerStats("UL PUSCH HARQ/CQI/RI SDU Processing time: \0", ULPuschMuxCtrlCqiRiSDU);
            }
            LteBsPhyStatsPrintTimerStats("PRACH List Processing time:               \0", PRACHList);
            LteBsPhyStatsPrintTimerStats("SRS List Processing time:                 \0", SRSList);

            uart_printf("\r\n\r\n");

            // System Throughoput
            pThroughput0 = pLteBsPhyStats->timerStats.systemThroughput[0];
            pThroughput1 = pLteBsPhyStats->timerStats.systemThroughput[1];
            pThroughput2 = pLteBsPhyStats->timerStats.systemThroughput[2];
            uart_printf("SYSTEM CW0 DL Throughput (Last 5 seconds when UE was receiving):               (");
            for(j = 3; j < 8; j++)
                uart_printf("%6d ", pThroughput0[j]*8/1000);
            uart_printf(") Kbps\r\n");

            uart_printf("       CW1 DL Throughput:                                                      (");
            for(j = 3; j < 8; j++)
                uart_printf("%6d ", pThroughput1[j]*8/1000);
            uart_printf(") Kbps\r\n");

            uart_printf("       SUM:                                                                    (");
            for(j = 3; j < 8; j++)
                uart_printf("%6d ", (pThroughput0[j] + pThroughput1[j])*8/1000);
            uart_printf(") Kbps\r\n");

            uart_printf("                                                                                 ");
            for(j = 3; j < 7; j++)
            {
                if (pThroughput0[1] == j)
                    break;
                uart_printf("       ");
            }
            uart_printf("------\r\n");

            uart_printf("SYSTEM UL Throughput (Last 5 seconds when UE was transmitting):                (");
            for(j = 3; j < 8; j++)
                uart_printf("%6d ", pThroughput2[j]*8/1000);
            uart_printf(") Kbps\r\n");
            uart_printf("                                                                                 ");
            for(j = 3; j < 7; j++)
            {
                if (pThroughput2[1] == j)
                    break;
                uart_printf("       ");
            }
            uart_printf("------\r\n");

            // Total Data Transferred
            pSize = (U64*)&pLteBsPhyStats->timerStats.systemThroughput[0][8];
            sum = *pSize;
            pSize = (U64*)&pLteBsPhyStats->timerStats.systemThroughput[1][8];
            sum += *pSize;
            LteBsPhyStatsPrintTotalBytesTransferred(DL, sum, NULL);
            pSize = (U64*)&pLteBsPhyStats->timerStats.systemThroughput[2][8];
            LteBsPhyStatsPrintTotalBytesTransferred(UL, *pSize, NULL);

            MxDelayTicks(15000000);
            uart_printf("\r\n\r\n");
        }

        if (phyStatsEnabledApi)
        {

            uart_printf("Api Stats:\r\n");
            uart_printf("----------\r\n\r\n");

            uart_printf("Number of APIs arrived from MAC:                   %d\r\n", pLteBsPhyStats->apiStats.numApisArrived);
            uart_printf("Number of APIs from MAC freed:                     %d\r\n", pLteBsPhyStats->apiStats.numApisFreed);

            uart_printf("Number of ICPU / ICORE blocks from MAC:            %d\r\n", pLteBsPhyStats->apiStats.numMac2PhyBlocksArrived);
            uart_printf("Number of ICPU / ICORE blocks freed:               %d\r\n", pLteBsPhyStats->apiStats.numMac2PhyBlocksFreed);
            uart_printf("\r\n");

            uart_printf("Number of subframes with incorrect SFN from MAC:   %d\r\n", pLteBsPhyStats->apiStats.numMacToPhyMessageCleanSfn);
            uart_printf("Number of APIs with incorrect SFN from MAC:        %d\r\n", pLteBsPhyStats->apiStats.numMacToPhyMessageCleanApis);

            uart_printf("Number of subframes with duplicate SFN from MAC:   %d\r\n", pLteBsPhyStats->apiStats.numMacToPhyMessageCleanDuplicateSfn);
            uart_printf("Number of APIs with duplicate SFN from MAC:        %d\r\n", pLteBsPhyStats->apiStats.numMacToPhyMessageCleanDuplicateApis);
            uart_printf("\r\n");

            uart_printf("Number of subframes where PHY detected Bad Apis :  %d\r\n", pLteBsPhyStats->apiStats.numBadApiMessages);
            uart_printf("Bad Api Mask:                                      0x%08x\r\n", pLteBsPhyStats->apiStats.badApiMask);
            uart_printf("\r\n");

            uart_printf("Number of subframes where PHY inserted Dummy Apis: %d\r\n", pLteBsPhyStats->apiStats.numDumymApis);
            uart_printf("\r\n");

            uart_printf("Histogram of API Arrival time(s) to PHY\r\n");
            uart_printf("|");
            for(j = 0; j < 11; j++)
                uart_printf("===========");
            uart_printf("|\r\n");

            uart_printf("|");
            for(j = 0; j < 10; j++)
                uart_printf(" %3d - %3d ", j*100, (j+1)*100);
            uart_printf("    > %2d  |\r\n", j*100);
            uart_printf("|");
            for(j = 0; j < 11; j++)
                uart_printf("===========");
            uart_printf("|\r\n");

            uart_printf("|");
            for(j = 0; j < 11; j++)
                uart_printf("   %6d  ", pLteBsPhyStats->apiStats.apiArrivalHist[j]);
            uart_printf("|\r\n");
            uart_printf("|");
            for(j = 0; j < 11; j++)
                uart_printf("===========");
            uart_printf("|\r\n\r\n");


            uart_printf("Histogram of Number of subframes where PHY inserted Successive Dummy Apis\r\n");
            uart_printf("|");
            for(j = 0; j < 11; j++)
                uart_printf("==========");
            uart_printf("|\r\n");

            uart_printf("|");
            for(j = 0; j < 10; j++)
                uart_printf("  %6d  ", j+1);
            uart_printf("    > %2d  |\r\n", j);
            uart_printf("|");
            for(j = 0; j < 11; j++)
                uart_printf("==========");
            uart_printf("|\r\n");

            uart_printf("|");
            for(j = 0; j < 11; j++)
                uart_printf("  %6d  ", pLteBsPhyStats->apiStats.numConsecutiveDummyApis[j]);
            uart_printf("|\r\n");
            uart_printf("|");
            for(j = 0; j < 11; j++)
                uart_printf("==========");
            uart_printf("|\r\n");
            uart_printf("\r\n\r\n");
        }


        uart_printf("Device Stats:\r\n");
        uart_printf("-------------\r\n\r\n");

        uart_printf("Number of started lists:           %d\r\n", pRexServStatInfo->NumListRun);
        uart_printf("Number of incompleted lists:       %d\r\n", pRexServStatInfo->NumBadList);
        uart_printf("Number of restored CEVS:           %d\r\n", pRexServStatInfo->NumCevaRest);
        uart_printf("BAD CEVAs detected in work:        0x%x\r\n", pRexServStatInfo->CevaBadDevMask);
        uart_printf("\r\n\r\n");

        if (phyStatsEnabledRach)
        {

            if (pLteBsPhyStats->rachStats.numPrachDetected)
            {
                MxDelayTicks(150000000);
                uart_printf("RACH Stats:\r\n");
                uart_printf("-----------\r\n\r\n");

                uart_printf("Number of PRACH(s) detected:                                                    %d\r\n", pLteBsPhyStats->rachStats.numPrachDetected);
                uart_printf("Max Timing Advance / Min Timing Advance:                                        (%d / %d)\r\n",
                    pLteBsPhyStats->rachStats.rachTimingOffsetMax, pLteBsPhyStats->rachStats.rachTimingOffsetMin);
                uart_printf("Histogram:        ");
                for(j = 0; j < 11; j++)
                    uart_printf("  %5d  ", j*2);
                uart_printf("    >%d\r\n", j*2);

                uart_printf("                 ");
                for(j = 0; j < 12; j++)
                    uart_printf(" %7d ", pLteBsPhyStats->rachStats.rachTimingOffsetBins[j]);
                uart_printf("\r\n");

                uart_printf("Max / Min Peak To Avr Power Ratio :                                             (%d / %d)\r\n",
                    pLteBsPhyStats->rachStats.rachPeakToAvgPwrRatioMax, pLteBsPhyStats->rachStats.rachPeakToAvgPwrRatioMin);
                uart_printf("Histogram:        ");
                for(j = 0; j < 11; j++)
                    uart_printf("  %5d  ", j*15);
                uart_printf("    >%d\r\n", j*15);

                uart_printf("                 ");
                for(j = 0; j < 12; j++)
                    uart_printf(" %7d ", pLteBsPhyStats->rachStats.rachPeakToAvgPwrRatioBins[j]);
                uart_printf("\r\n");

                uart_printf("Max Timing Advance / Min Timing Advance (for Pk Avg Pwr Ratio > %4d):          (%d / %d)\r\n",
                    PRACH_DETECTION_THRESHOLD_GOOD, pLteBsPhyStats->rachStats.rachTimingOffsetGoodPwrRatioMax,
                    pLteBsPhyStats->rachStats.rachTimingOffsetGoodPwrRatioMin);
                uart_printf("Histogram:        ");
                for(j = 0; j < 11; j++)
                    uart_printf("  %5d  ", j*2);
                uart_printf("    >%d\r\n", j*2);

                uart_printf("                 ");
                for(j = 0; j < 12; j++)
                    uart_printf(" %7d ", pLteBsPhyStats->rachStats.rachTimingOffsetGoodPwrRatioBins[j]);
                uart_printf("\r\n\r\n");
            }
        }

        if (phystatsEnabledUsrStat)
        {

            // USER STATS display in tabluar format
            MxDelayTicks(150000000);

            // Find the Number of Users and the User IDs
            // Find the total number of RNTIs in all subframes logged and store them
            totalRNTIs = 0;
            for (i = 0; i < LTE_BS_PHY_STATS_UNIQUE_INDICES; i++)
            {
                pUserStat = &pLteBsPhyStats->userStats[i];
                if (pUserStat->used)
                {
                    if (LteBsPhyStatsAddRnti(RNTIMap, &totalRNTIs, pUserStat->RNTI) == 0)
                        pUserStatSave[totalRNTIs-1] = pUserStat;
                }
            }
            LteBsPhyStatsSortRntiMap(RNTIMap, totalRNTIs, pUserStatSave);

            if (totalRNTIs)
            {
                uart_printf("User Stats (SnapShot):\r\n");
                uart_printf("----------------------\r\n\n");

                // Print the Header
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '=');
                uart_printf("| Item                         |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    uart_printf("          RNTI %5d          |", RNTIMap[i]);
                }
                uart_printf("\r\n");

                // PDSCH Stats
                // PDSCH
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| Num PDSCH SDUs [cw0 / cw1]   |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    uart_printf("          [%7d / %7d] |", pUserStat->numPdschSdu[0], pUserStat->numPdschSdu[1]);
                }
                uart_printf("\r\n");

                // PDCCH
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| Num PDCCH SDUs               |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    uart_printf("                      %7d |", pUserStat->numPdcchSdu);
                }
                uart_printf("\r\n");
                uart_printf("| PDCCH Format (0 / 1a / 1b)   |");

                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    uart_printf("  %7d / %7d / %7d |",
                        pUserStat->numPdcchFormats[0], pUserStat->numPdcchFormats[1], pUserStat->numPdcchFormats[2]);
                }
                uart_printf("\r\n");

                // PUSCH
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| Num PUSCH [CRC Errs / SDUs]  |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = 0;
                    if (pUserStat->numPuschSdu)
                        sum = (((U64)pUserStat->numPuschErr * 100) / pUserStat->numPuschSdu);
                    uart_printf("   [%7d / %7d : %3llu%%] |", pUserStat->numPuschErr, pUserStat->numPuschSdu, sum);
                }
                uart_printf("\r\n");


                // PUCCH FORMAT 1
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| Num PUCCH Fr 1  [Det / SDUs] |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = 0;
                    if (pUserStat->numPucchNumSdu[FORMAT1])
                        sum = (((U64)pUserStat->numPucchFormat1Det * 100) / pUserStat->numPucchNumSdu[FORMAT1]);
                    uart_printf("   [%7d / %7d : %3llu%%] |", pUserStat->numPucchFormat1Det, pUserStat->numPucchNumSdu[FORMAT1], sum);
                }
                uart_printf("\r\n");
                uart_printf("| Simul SR  Fr 2  [Det / SDUs] |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = 0;
                    if (pUserStat->numPucchSimultSr[FORMAT2])
                        sum = (((U64)pUserStat->numPucchSimultSrDet[FORMAT2] * 100) / pUserStat->numPucchSimultSr[FORMAT2]);
                    uart_printf("   [%7d / %7d : %3llu%%] |",
                        pUserStat->numPucchSimultSrDet[FORMAT2], pUserStat->numPucchSimultSr[FORMAT2], sum);
                }
                uart_printf("\r\n");
                uart_printf("| Simul SR  Fr 1A [Det / SDUs] |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = 0;
                    if (pUserStat->numPucchSimultSr[FORMAT1A])
                        sum = (((U64)pUserStat->numPucchSimultSrDet[FORMAT1A] * 100) / pUserStat->numPucchSimultSr[FORMAT1A]);
                    uart_printf("   [%7d / %7d : %3llu%%] |",
                        pUserStat->numPucchSimultSrDet[FORMAT1A], pUserStat->numPucchSimultSr[FORMAT1A], sum);
                }
                uart_printf("\r\n");
                uart_printf("| Simul SR  Fr 2A [Det / SDUs] |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = 0;
                    if (pUserStat->numPucchSimultSr[FORMAT2A])
                        sum = (((U64)pUserStat->numPucchSimultSrDet[FORMAT2A] * 100) / pUserStat->numPucchSimultSr[FORMAT2A]);
                    uart_printf("   [%7d / %7d : %3llu%%] |",
                        pUserStat->numPucchSimultSrDet[FORMAT2A], pUserStat->numPucchSimultSr[FORMAT2A], sum);
                }
                uart_printf("\r\n");
                uart_printf("| Simul SR  Fr 1B [Det / SDUs] |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = 0;
                    if (pUserStat->numPucchSimultSr[FORMAT1B])
                        sum = (((U64)pUserStat->numPucchSimultSrDet[FORMAT1B] * 100) / pUserStat->numPucchSimultSr[FORMAT1B]);
                    uart_printf("   [%7d / %7d : %3llu%%] |",
                        pUserStat->numPucchSimultSrDet[FORMAT1B], pUserStat->numPucchSimultSr[FORMAT1B], sum);
                }
                uart_printf("\r\n");
                uart_printf("| Simul SR  Fr 2B [Det / SDUs] |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = 0;
                    if (pUserStat->numPucchSimultSr[FORMAT2B])
                        sum = (((U64)pUserStat->numPucchSimultSrDet[FORMAT2B] * 100) / pUserStat->numPucchSimultSr[FORMAT2B]);
                    uart_printf("   [%7d / %7d : %3llu%%] |",
                        pUserStat->numPucchSimultSrDet[FORMAT2B], pUserStat->numPucchSimultSr[FORMAT2B], sum);
                }
                uart_printf("\r\n");

                // PUCCH FORMAT 2
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| Num PUCCH Fr 2  [SDUs]       |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    uart_printf("                      %7d |", pUserStat->numPucchNumSdu[FORMAT2]);
                }
                uart_printf("\r\n");

                // PUCCH FORMAT 1A
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| Num PUCCH Fr 1A [N1 / D]     |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    uart_printf("            %7d / %7d |", pUserStat->numPucchFormat1ANacks, pUserStat->numPucchFormat1ANotReceived);
                }
                uart_printf("\r\n");
                uart_printf("| Num Occurances / Percetanges |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = 0;
                    sum1 = 0;
                    if (pUserStat->numPucchNumSdu[FORMAT1A])
                    {
                        sum  = (((U64)pUserStat->numPucchFormat1ANacks * 100) / pUserStat->numPucchNumSdu[FORMAT1A]);
                        sum1 = (((U64)pUserStat->numPucchFormat1ANotReceived * 100) / pUserStat->numPucchNumSdu[FORMAT1A]);
                    }
                    uart_printf("     %7d [ %3llu%% /  %3llu%% ] |", pUserStat->numPucchNumSdu[FORMAT1A], sum, sum1);
                }
                uart_printf("\r\n");


                // PUCCH FORMAT 2A
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| Num PUCCH Fr 2A [N1 / D]     |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    uart_printf("  %7d / %7d / %7d |", pUserStat->numPucchFormat2ANacks, pUserStat->numPucchFormat2ANotReceived, pUserStat->numPucchNumSdu[FORMAT2A]);
                }
                uart_printf("\r\n");
                uart_printf("| Num Occurances / Percetanges |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = 0;
                    sum1 = 0;
                    if (pUserStat->numPucchNumSdu[FORMAT2A])
                    {
                        sum  = (((U64)pUserStat->numPucchFormat2ANacks * 100) / pUserStat->numPucchNumSdu[FORMAT2A]);
                        sum1 = (((U64)pUserStat->numPucchFormat2ANotReceived * 100) / pUserStat->numPucchNumSdu[FORMAT2A]);
                    }
                    uart_printf("     %7d [ %3llu%% /  %3llu%% ] |", pUserStat->numPucchNumSdu[FORMAT2A], sum, sum1);
                }
                uart_printf("\r\n");


                // PUCCH FORMAT 1B
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| Num PUCCH Fr 1B [N1 / N2 / D]|");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    uart_printf("  %7d / %7d / %7d |", pUserStat->numPucchFormat1BNacksA, pUserStat->numPucchFormat1BNacksB, pUserStat->numPucchFormat1BNotReceived);
                }
                uart_printf("\r\n");
                uart_printf("| Num Occurances / Percetanges |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = 0;
                    sum1 = 0;
                    sum2 = 0;
                    if (pUserStat->numPucchNumSdu[FORMAT1B])
                    {
                        sum  = (((U64)pUserStat->numPucchFormat1BNacksA * 100) / pUserStat->numPucchNumSdu[FORMAT1B]);
                        sum1 = (((U64)pUserStat->numPucchFormat1BNacksB * 100) / pUserStat->numPucchNumSdu[FORMAT1B]);
                        sum2 = (((U64)pUserStat->numPucchFormat1BNotReceived * 100) / pUserStat->numPucchNumSdu[FORMAT1B]);
                    }
                    uart_printf(" %7d [%3llu%% / %3llu%% / %3llu%%] |", pUserStat->numPucchNumSdu[FORMAT1B], sum, sum1, sum2);
                }
                uart_printf("\r\n");


                // PUCCH FORMAT 2B
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| Num PUCCH Fr 2B [N1 / N2 / D]|");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    uart_printf("  %7d / %7d / %7d |", pUserStat->numPucchFormat2BNacksA, pUserStat->numPucchFormat2BNacksB, pUserStat->numPucchFormat2BNotReceived);
                }
                uart_printf("\r\n");
                uart_printf("| Num Occurances / Percetanges |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = 0;
                    sum1 = 0;
                    sum2 = 0;
                    if (pUserStat->numPucchNumSdu[FORMAT2B])
                    {
                        sum  = (((U64)pUserStat->numPucchFormat2BNacksA * 100) / pUserStat->numPucchNumSdu[FORMAT2B]);
                        sum1 = (((U64)pUserStat->numPucchFormat2BNacksB * 100) / pUserStat->numPucchNumSdu[FORMAT2B]);
                        sum2 = (((U64)pUserStat->numPucchFormat2BNotReceived * 100) / pUserStat->numPucchNumSdu[FORMAT2B]);
                    }
                    uart_printf(" %7d [%3llu%% / %3llu%% / %3llu%%] |", pUserStat->numPucchNumSdu[FORMAT2B], sum, sum1, sum2);
                }
                uart_printf("\r\n");

                // MUX PUSCH ACK / NACK
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| Num MUX PUSCH [N1 / N2 / D]  |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    uart_printf("  %7d / %7d / %7d |", pUserStat->numMuxPuschNack1, pUserStat->numMuxPuschNack2, pUserStat->numMuxPuschAckNackNotReceived);
                }
                uart_printf("\r\n");
                uart_printf("| Num Occurances / Percetanges |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = 0;
                    sum1 = 0;
                    sum2 = 0;
                    if (pUserStat->numMuxPuschAckNack)
                    {
                        sum  = (((U64)pUserStat->numMuxPuschNack1*100) / pUserStat->numMuxPuschAckNack);
                        sum1 = (((U64)pUserStat->numMuxPuschNack2*100) / pUserStat->numMuxPuschAckNack);
                        sum2 = (((U64)pUserStat->numMuxPuschAckNackNotReceived*100) / pUserStat->numMuxPuschAckNack);
                    }
                    uart_printf(" %7d [%3llu%% / %3llu%% / %3llu%%] |", pUserStat->numMuxPuschAckNack, sum, sum1, sum2);
                }
                uart_printf("\r\n");


                // MUX PUSCH CQI
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| Num MUX PUSCH CQI [Det / Tot]|");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = 0;
                    if (pUserStat->numMuxPuschCqiPusch)
                        sum = (((U64)pUserStat->numMuxPuschCqiNotReceived*100) / pUserStat->numMuxPuschCqiPusch);
                    uart_printf("   [%7d / %7d : %3llu%%] |", pUserStat->numMuxPuschCqiNotReceived, pUserStat->numMuxPuschCqiPusch, sum);
                }
                uart_printf("\r\n");

                // MUX PUSCH RI
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| Num MUX PUSCH RI [Det / Tot] |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = 0;
                    if (pUserStat->numMuxPuschRiPusch)
                        sum = (((U64)pUserStat->numMuxPuschRiNotReceived*100) / pUserStat->numMuxPuschRiPusch);
                    uart_printf("   [%7d / %7d : %3llu%%] |", pUserStat->numMuxPuschRiNotReceived, pUserStat->numMuxPuschRiPusch, sum);
                }
                uart_printf("\r\n");


                // HARQ Retransmission DL0
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| DL0 HARQ ReTx rv [0/ 1/ 2/ 3]|");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    uart_printf("%6d/ %6d/ %6d/ %6d|",
                        pUserStat->numDLHarqTransmissions[0][0], pUserStat->numDLHarqTransmissions[0][1],
                        pUserStat->numDLHarqTransmissions[0][2], pUserStat->numDLHarqTransmissions[0][3]);
                }
                uart_printf("\r\n");
                uart_printf("| Percentages                  |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = pUserStat->numDLHarqTransmissions[0][0] + pUserStat->numDLHarqTransmissions[0][1] +
                          pUserStat->numDLHarqTransmissions[0][2] + pUserStat->numDLHarqTransmissions[0][3] + 1;

                    uart_printf("  %3llu%%/   %3llu%%/   %3llu%%/  %3llu%% |",
                        (((U64)pUserStat->numDLHarqTransmissions[0][0]*100) / sum), (((U64)pUserStat->numDLHarqTransmissions[0][1]*100) / sum),
                        (((U64)pUserStat->numDLHarqTransmissions[0][2]*100) / sum), (((U64)pUserStat->numDLHarqTransmissions[0][3]*100) / sum));
                }
                uart_printf("\r\n");

                // HARQ Retransmission DL1
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| DL1 HARQ ReTx rv [0/ 1/ 2/ 3]|");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    uart_printf("%6d/ %6d/ %6d/ %6d|",
                        pUserStat->numDLHarqTransmissions[1][0], pUserStat->numDLHarqTransmissions[1][1],
                        pUserStat->numDLHarqTransmissions[1][2], pUserStat->numDLHarqTransmissions[1][3]);
                }
                uart_printf("\r\n");
                uart_printf("| Percentages                  |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = pUserStat->numDLHarqTransmissions[1][0] + pUserStat->numDLHarqTransmissions[1][1] +
                          pUserStat->numDLHarqTransmissions[1][2] + pUserStat->numDLHarqTransmissions[1][3] + 1;

                    uart_printf("  %3llu%%/   %3llu%%/   %3llu%%/  %3llu%% |",
                        (((U64)pUserStat->numDLHarqTransmissions[1][0]*100) / sum), (((U64)pUserStat->numDLHarqTransmissions[1][1]*100) / sum),
                        (((U64)pUserStat->numDLHarqTransmissions[1][2]*100) / sum), (((U64)pUserStat->numDLHarqTransmissions[1][3]*100) / sum));
                }
                uart_printf("\r\n");

                // HARQ Retransmission UL
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| UL HARQ ReTx rv [0/ 1/ 2/ 3] |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    uart_printf("%6d/ %6d/ %6d/ %6d|",
                        pUserStat->numULHarqTransmissions[0], pUserStat->numULHarqTransmissions[1],
                        pUserStat->numULHarqTransmissions[2], pUserStat->numULHarqTransmissions[3]);
                }
                uart_printf("\r\n");
                uart_printf("| Percentages                  |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    sum = pUserStat->numULHarqTransmissions[0] + pUserStat->numULHarqTransmissions[1] +
                          pUserStat->numULHarqTransmissions[2] + pUserStat->numULHarqTransmissions[3] + 1;

                    uart_printf("  %3llu%%/   %3llu%%/   %3llu%%/  %3llu%% |",
                        (((U64)pUserStat->numULHarqTransmissions[0]*100) / sum), (((U64)pUserStat->numULHarqTransmissions[1]*100) / sum),
                        (((U64)pUserStat->numULHarqTransmissions[2]*100) / sum), (((U64)pUserStat->numULHarqTransmissions[3]*100) / sum));
                }
                uart_printf("\r\n");


                // DL Throughputs CW0
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| DL CW0 Thput [Lst 3sec Kbps] |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];

                    // Find the current guy
                    for(j = 3; j < 7; j++)
                    {
                        if (pUserStat->DLThroughput[0][1] == j)
                            break;
                    }

                    for (sum = 0; sum < 2; sum++, j--)
                    {
                        if (j == 3)
                            j = 8;
                    }

                    for (k = j, sum = 0; sum < 3; sum++, k++)
                    {
                        lastThreeThroughput[sum] = (pUserStat->DLThroughput[0][k]*8)/1000;
                        if (k == 7)
                            k = 2;
                    }

                    uart_printf("  %7d / %7d / %7d |",
                        lastThreeThroughput[0], lastThreeThroughput[1], lastThreeThroughput[2]);
                }
                uart_printf("\r\n");

                // DL Throughputs CW1
                uart_printf("|    CW1 Thput                 |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];

                    // Find the current guy
                    for(j = 3; j < 7; j++)
                    {
                        if (pUserStat->DLThroughput[0][1] == j)
                            break;
                    }

                    for (sum = 0; sum < 2; sum++, j--)
                    {
                        if (j == 3)
                            j = 8;
                    }

                    for (k = j, sum = 0; sum < 3; sum++, k++)
                    {
                        lastThreeThroughput[sum] = (pUserStat->DLThroughput[1][k]*8)/1000;
                        if (k == 7)
                            k = 2;
                    }

                    uart_printf("  %7d / %7d / %7d |",
                        lastThreeThroughput[0], lastThreeThroughput[1], lastThreeThroughput[2]);
                }
                uart_printf("\r\n");

                // DL Throughputs Sum
                uart_printf("| DL Thput (SUM)               |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];

                    // Find the current guy
                    for(j = 3; j < 7; j++)
                    {
                        if (pUserStat->DLThroughput[0][1] == j)
                            break;
                    }

                    for (sum = 0; sum < 2; sum++, j--)
                    {
                        if (j == 3)
                            j = 8;
                    }

                    for (k = j, sum = 0; sum < 3; sum++, k++)
                    {
                        lastThreeThroughput[sum] = ((pUserStat->DLThroughput[0][k] + pUserStat->DLThroughput[1][k])*8)/1000;
                        if (k == 7)
                            k = 2;
                    }

                    uart_printf("  %7d / %7d / %7d |",
                        lastThreeThroughput[0], lastThreeThroughput[1], lastThreeThroughput[2]);
                }
                uart_printf("\r\n");


                // UL Throughputs
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| DL CW0 Thput [Lst 3sec Kbps] |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];

                    // Find the current guy
                    for(j = 3; j < 7; j++)
                    {
                        if (pUserStat->ULThroughput[1] == j)
                            break;
                    }

                    for (sum = 0; sum < 2; sum++, j--)
                    {
                        if (j == 3)
                            j = 8;
                    }

                    for (k = j, sum = 0; sum < 3; sum++, k++)
                    {
                        lastThreeThroughput[sum] = (pUserStat->ULThroughput[k]*8)/1000;
                        if (k == 7)
                            k = 2;
                    }

                    uart_printf("  %7d / %7d / %7d |",
                        lastThreeThroughput[0], lastThreeThroughput[1], lastThreeThroughput[2]);
                }
                uart_printf("\r\n");


                // Total Data Transferred
                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| Total DL Data Transf (bytes) |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    pSize = (U64*)pUserStat->totalDlData;
                    LteBsPhyStatsPrintTotalBytesTransferred(DL, *pSize, storeDisp);

                    uart_printf(" %s|",storeDisp);
                }
                uart_printf("\r\n");


                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '-');
                uart_printf("| Total UL Data Transf (bytes) |");
                for (i =0 ; i < totalRNTIs; i++)
                {
                    pUserStat = pUserStatSave[i];
                    pSize = (U64*)pUserStat->totalUlData;
                    LteBsPhyStatsPrintTotalBytesTransferred(UL, *pSize, storeDisp);

                    uart_printf(" %s|",storeDisp);
                }
                uart_printf("\r\n");

                LteBsPhyStatsUserStatPrintDelimeter(totalRNTIs, '=');
                uart_printf("\r\n\r\n");
            }











            // USER STATS display per USER
            MxDelayTicks(150000000);
            for (i = 0; i < LTE_BS_PHY_STATS_UNIQUE_INDICES; i++)
            {
                pUserStat = &pLteBsPhyStats->userStats[i];
                if (pUserStat->used)
                {
                    if (userCnt == 0)
                    {
                        uart_printf("User Stats by RNTI:\r\n");
                        uart_printf("-------------------\r\n\n");
                    }
                    userCnt++;
                    ulPresent = 0;
                    dlCqiPresent = 0;
                    dlRiPresent = 0;
                    ulSinrPresent = 0;
                    ulTmgAdvPresent = 0;
                    puschAverageCqiPresent = 0;
                    pucchAverageCqiPresent = 0;
                    uciPeriodicity = 0;

                    // General
                    uart_printf("User: %d\r\n", userCnt);
                    uart_printf("RNTI: %d\r\n", pUserStat->RNTI);
                    uart_printf("Num TTIs where DL Processing took > %4d usec:                                  [%d]\r\n",
                        LTE_BS_HY_STATS_MAX_DL_LIST_PROCESSING_TIME, pUserStat->numTtiListProcessingHigh);

                    // PDSCH
                    if (pUserStat->numPdschSdu[0]+pUserStat->numPdschSdu[1])
                    {
                        uart_printf("PDSCH (Total SDUs [cw0 / cw1]):                                                 [%d / %d]\r\n",
                            pUserStat->numPdschSdu[0], pUserStat->numPdschSdu[1]);
                    }

                    // PDCCH
                    if (pUserStat->numPdcchSdu)
                    {
                        uart_printf("PDCCH (Total SDUs):                                                             (%d)\r\n",
                            pUserStat->numPdcchSdu);
                        uart_printf("PDCCH Format (0 / 1a / 1b):                                                     (%d / %d / %d)\r\n",
                            pUserStat->numPdcchFormats[0], pUserStat->numPdcchFormats[1], pUserStat->numPdcchFormats[2]);
                    }

                    // PUSCH
                    if (pUserStat->numPuschSdu)
                    {
                        ulPresent = 1;
                        puschAverageCqiPresent = 1;
                        ulSinrPresent++;
                        ulTmgAdvPresent++;
                        uart_printf("PUSCH (Num SDUs with CRC Error / Total SDUs):                                   (%d / %d) : (%llu%%)\r\n",
                            pUserStat->numPuschErr, pUserStat->numPuschSdu,
                            (((U64)pUserStat->numPuschErr * 100) / pUserStat->numPuschSdu));
                    }

                    // PUCCH
                    if (pUserStat->numPucchNumSdu[FORMAT1])
                    {
                        ulPresent = 1;
                        ulSinrPresent++;
                        ulTmgAdvPresent++;
                        uciPeriodicity++;
                        uart_printf("PUCCH Format 1 (Num Detected / Total SDUs):                                     (%d / %d) : (%llu%%)\r\n",
                            pUserStat->numPucchFormat1Det, pUserStat->numPucchNumSdu[FORMAT1],
                            (((U64)pUserStat->numPucchFormat1Det * 100) / pUserStat->numPucchNumSdu[FORMAT1]));
                    }
                    if (pUserStat->numPucchNumSdu[FORMAT2])
                    {
                        ulPresent = 1;
                        ulSinrPresent++;
                        ulTmgAdvPresent++;
                        pucchAverageCqiPresent = 1;
                        if (pUserStat->pdschNumCqi[0])
                            dlCqiPresent++;
                        if (pUserStat->pdschNumRi[0])
                            dlRiPresent++;
                        sum = 0;
                        if (pUserStat->numPucchSimultSr[FORMAT2])
                            sum = (((U64)pUserStat->numPucchSimultSrDet[FORMAT2] * 100) / pUserStat->numPucchSimultSr[FORMAT2]);
                        uart_printf("PUCCH Format 2 (Total SDUs) | SimuSR(Det/Total):                                (%d) | (%d / %d: %llu%%)\r\n",
                            pUserStat->numPucchNumSdu[FORMAT2],
                            pUserStat->numPucchSimultSrDet[FORMAT2], pUserStat->numPucchSimultSr[FORMAT2],
                            sum);
                    }
                    if (pUserStat->numPucchNumSdu[FORMAT1A])
                    {
                        ulPresent = 1;
                        ulSinrPresent++;
                        ulTmgAdvPresent++;
                        pucchAverageCqiPresent = 1;
                        sum = 0;
                        if (pUserStat->numPucchSimultSr[FORMAT1A])
                            sum = (((U64)pUserStat->numPucchSimultSrDet[FORMAT1A] * 100) / pUserStat->numPucchSimultSr[FORMAT1A]);
                        uart_printf("PUCCH Format 1a (Num NACKS / Num DTX / Total SDUs) | SimuSR(Det/Tot):           (%d / %d / %d) : (%llu%% / %llu%%) | (%d / %d : %llu%%)\r\n",
                            pUserStat->numPucchFormat1ANacks, pUserStat->numPucchFormat1ANotReceived, pUserStat->numPucchNumSdu[FORMAT1A],
                            (((U64)pUserStat->numPucchFormat1ANacks * 100) / pUserStat->numPucchNumSdu[FORMAT1A]),
                            (((U64)pUserStat->numPucchFormat1ANotReceived * 100) / pUserStat->numPucchNumSdu[FORMAT1A]),
                            pUserStat->numPucchSimultSrDet[FORMAT1A], pUserStat->numPucchSimultSr[FORMAT1A],
                            sum);
                    }
                    if (pUserStat->numPucchNumSdu[FORMAT2A])
                    {
                        ulPresent = 1;
                        ulSinrPresent++;
                        ulTmgAdvPresent++;
                        if (pUserStat->pdschNumCqi[1])
                            dlCqiPresent++;
                        if (pUserStat->pdschNumRi[1])
                            dlRiPresent++;
                        sum = 0;
                        if (pUserStat->numPucchSimultSr[FORMAT2A])
                            sum = (((U64)pUserStat->numPucchSimultSrDet[FORMAT2A] * 100) / pUserStat->numPucchSimultSr[FORMAT2A]);
                        uart_printf("PUCCH Format 2a (Num NACKS / Num DTX / Total SDUs) | SimulSR(Det/Tot):          (%d / %d / %d) : (%u%% / %u%%) | (%d / %d : %llu%%)\r\n",
                            pUserStat->numPucchFormat2ANacks, pUserStat->numPucchFormat2ANotReceived, pUserStat->numPucchNumSdu[FORMAT2A],
                            (((U32)pUserStat->numPucchFormat2ANacks * 100) / pUserStat->numPucchNumSdu[FORMAT2A]),
                            (((U32)pUserStat->numPucchFormat2ANotReceived * 100) / pUserStat->numPucchNumSdu[FORMAT2A]),
                            pUserStat->numPucchSimultSrDet[FORMAT2A], pUserStat->numPucchSimultSr[FORMAT2A],
                            sum);
                    }
                    if (pUserStat->numPucchNumSdu[FORMAT1B])
                    {
                        ulPresent = 1;
                        ulSinrPresent++;
                        ulTmgAdvPresent++;
                        pucchAverageCqiPresent = 1;
                        sum = 0;
                        if (pUserStat->numPucchSimultSr[FORMAT1B])
                            sum = (((U64)pUserStat->numPucchSimultSrDet[FORMAT1B] * 100) / pUserStat->numPucchSimultSr[FORMAT1B]);
                        uart_printf("PUCCH Format 1b (Num NACKS [0, 1] / Num DTX / Total SDUs) | SimulSR(Det/Tot):   ([%d, %d] / %d / %d) : ([%llu%% / %llu%%] / %llu%%) | (%d / %d : %llu%%)\r\n",
                            pUserStat->numPucchFormat1BNacksA, pUserStat->numPucchFormat1BNacksB,
                            pUserStat->numPucchFormat1BNotReceived, pUserStat->numPucchNumSdu[FORMAT1B],
                            (((U64)pUserStat->numPucchFormat1BNacksA*100) / pUserStat->numPucchNumSdu[FORMAT1B]),
                            (((U64)pUserStat->numPucchFormat1BNacksB*100) / pUserStat->numPucchNumSdu[FORMAT1B]),
                            (((U64)pUserStat->numPucchFormat1BNotReceived*100) / pUserStat->numPucchNumSdu[FORMAT1B]),
                            pUserStat->numPucchSimultSrDet[FORMAT1B], pUserStat->numPucchSimultSr[FORMAT1B],
                            sum);
                    }
                    if (pUserStat->numPucchNumSdu[FORMAT2B])
                    {
                        ulPresent = 1;
                        ulSinrPresent++;
                        ulTmgAdvPresent++;
                        if (pUserStat->pdschNumCqi[2])
                            dlCqiPresent++;
                        if (pUserStat->pdschNumRi[2])
                            dlRiPresent++;
                        sum = 0;
                        if (pUserStat->numPucchSimultSr[FORMAT2B])
                            sum = (((U64)pUserStat->numPucchSimultSrDet[FORMAT2B] * 100) / pUserStat->numPucchSimultSr[FORMAT2B]);
                        uart_printf("PUCCH Format 2b (Num NACKS [0, 1] / Num DTX / Total SDUs) | SimulSR(Det/Tot):   ([%d, %d] / %d / %d) : ([%llu%% / %llu%%] / %llu%%) | (%d / %d : %llu%%)\r\n",
                            pUserStat->numPucchFormat2BNacksA, pUserStat->numPucchFormat2BNacksB,
                            pUserStat->numPucchFormat2BNotReceived, pUserStat->numPucchNumSdu[FORMAT2B],
                            (((U64)pUserStat->numPucchFormat2BNacksA*100) / pUserStat->numPucchNumSdu[FORMAT2B]),
                            (((U64)pUserStat->numPucchFormat2BNacksB*100) / pUserStat->numPucchNumSdu[FORMAT2B]),
                            (((U64)pUserStat->numPucchFormat2BNotReceived*100) / pUserStat->numPucchNumSdu[FORMAT2B]),
                            pUserStat->numPucchSimultSrDet[FORMAT2B], pUserStat->numPucchSimultSr[FORMAT2B],
                            sum);
                    }

                    uciPeriodicity += (dlCqiPresent ? 1 : 0);
                    uciPeriodicity += (dlRiPresent ? 1 : 0);


                    // Multiplexed PUSCH
                    if (pUserStat->numMuxPuschAckNack)
                    {
                        uart_printf("Mux PUSCH Ack / Nack (Num NACKS [0, 1] / Num DTX / Total SDUs):                 ([%d, %d] / %d / %d) : ([%llu%% / %llu%%] / %llu%%)\r\n",
                            pUserStat->numMuxPuschNack1, pUserStat->numMuxPuschNack2, pUserStat->numMuxPuschAckNackNotReceived, pUserStat->numMuxPuschAckNack,
                            (((U64)pUserStat->numMuxPuschNack1*100) / pUserStat->numMuxPuschAckNack),
                            (((U64)pUserStat->numMuxPuschNack2*100) / pUserStat->numMuxPuschAckNack),
                            (((U64)pUserStat->numMuxPuschAckNackNotReceived*100) / pUserStat->numMuxPuschAckNack));
                    }
                    if (pUserStat->numMuxPuschCqiPusch)
                    {
                        dlCqiPresent++;
                        uart_printf("Mux PUSCH CQI (Num DTX / Total SDUs):                                           (%d / %d) : (%llu%%)\r\n",
                            pUserStat->numMuxPuschCqiNotReceived, pUserStat->numMuxPuschCqiPusch,
                            (((U64)pUserStat->numMuxPuschCqiNotReceived*100) / pUserStat->numMuxPuschCqiPusch));
                    }
                    if (pUserStat->numMuxPuschRiPusch)
                    {
                        dlRiPresent++;
                        uart_printf("Mux PUSCH RI (Num DTX / Total SDUs):                                            (%d / %d) : (%llu%%)\r\n",
                            pUserStat->numMuxPuschRiNotReceived, pUserStat->numMuxPuschRiPusch,
                            (((U64)pUserStat->numMuxPuschRiNotReceived*100) / pUserStat->numMuxPuschRiPusch));
                    }

                    // HARQ Retransmission
                    if (pUserStat->numPdschSdu[0])
                    {
                        sum = pUserStat->numDLHarqTransmissions[0][0] + pUserStat->numDLHarqTransmissions[0][1] +
                              pUserStat->numDLHarqTransmissions[0][2] + pUserStat->numDLHarqTransmissions[0][3];

                        uart_printf("DL HARQ Transmissions Count cw0 (rv 0 / rv 1 / rv 2 / rv 3):                    (%d / %d / %d / %d) : (%llu%% / %llu%% / %llu%% / %llu%%)\r\n",
                            pUserStat->numDLHarqTransmissions[0][0], pUserStat->numDLHarqTransmissions[0][1],
                            pUserStat->numDLHarqTransmissions[0][2], pUserStat->numDLHarqTransmissions[0][3],
                            (((U64)pUserStat->numDLHarqTransmissions[0][0]*100) / sum), (((U64)pUserStat->numDLHarqTransmissions[0][1]*100) / sum),
                            (((U64)pUserStat->numDLHarqTransmissions[0][2]*100) / sum), (((U64)pUserStat->numDLHarqTransmissions[0][3]*100) / sum));
                    }
                    if (pUserStat->numPdschSdu[1])
                    {
                        sum = pUserStat->numDLHarqTransmissions[1][0] + pUserStat->numDLHarqTransmissions[1][1] +
                              pUserStat->numDLHarqTransmissions[1][2] + pUserStat->numDLHarqTransmissions[1][3];

                        uart_printf("DL HARQ Transmissions Count cw1 (rv 0 / rv 1 / rv 2 / rv 3):                    (%d / %d / %d / %d) : (%llu%% / %llu%% / %llu%% / %llu%%)\r\n",
                            pUserStat->numDLHarqTransmissions[1][0], pUserStat->numDLHarqTransmissions[1][1],
                            pUserStat->numDLHarqTransmissions[1][2], pUserStat->numDLHarqTransmissions[1][3],
                            (((U64)pUserStat->numDLHarqTransmissions[1][0]*100) / sum), (((U64)pUserStat->numDLHarqTransmissions[1][1]*100) / sum),
                            (((U64)pUserStat->numDLHarqTransmissions[1][2]*100) / sum), (((U64)pUserStat->numDLHarqTransmissions[1][3]*100) / sum));
                    }

                    if (pUserStat->numPuschSdu)
                    {
                        sum = pUserStat->numULHarqTransmissions[0] + pUserStat->numULHarqTransmissions[1] +
                              pUserStat->numULHarqTransmissions[2] + pUserStat->numULHarqTransmissions[3];

                        uart_printf("UL HARQ Transmissions Count (1st / 2nd / 3rd / 4th):                            (%d / %d / %d / %d) : (%llu%% / %llu%% / %llu%% / %llu%%)\r\n",
                            pUserStat->numULHarqTransmissions[0], pUserStat->numULHarqTransmissions[1],
                            pUserStat->numULHarqTransmissions[2], pUserStat->numULHarqTransmissions[3],
                            (((U64)pUserStat->numULHarqTransmissions[0]*100) / sum), (((U64)pUserStat->numULHarqTransmissions[1]*100) / sum),
                            (((U64)pUserStat->numULHarqTransmissions[2]*100) / sum), (((U64)pUserStat->numULHarqTransmissions[3]*100) / sum));
                    }

                    MxDelayTicks(15000000*5);

                    // DL Throughoput
                    if (pUserStat->numPdschSdu[0] + pUserStat->numPdschSdu[1])
                    {
                        uart_printf("PHY CW0 DL Throughput (Last 5 seconds when UE was receiving):                   (");
                        for(j = 3; j < 8; j++)
                            uart_printf("%6d ", pUserStat->DLThroughput[0][j]*8/1000);
                        uart_printf(") Kbps\r\n");

                        if (pUserStat->numPdschSdu[1])
                        {
                            uart_printf("    CW1 DL Throughput:                                                          (");
                            for(j = 3; j < 8; j++)
                                uart_printf("%6d ", pUserStat->DLThroughput[1][j]*8/1000);
                            uart_printf(") Kbps\r\n");

                            uart_printf("    SUM:                                                                        (");
                            for(j = 3; j < 8; j++)
                                uart_printf("%6d ", (pUserStat->DLThroughput[0][j]+ pUserStat->DLThroughput[1][j])*8/1000);
                            uart_printf(") Kbps\r\n");
                        }

                        uart_printf("                                                                                 ");
                        for(j = 3; j < 7; j++)
                        {
                            if (pUserStat->DLThroughput[0][1] == j)
                                break;
                            uart_printf("       ");
                        }
                        uart_printf("------\r\n");

                        MxDelayTicks(15000000);
                    }

                    // UL Throughoput
                    if (pUserStat->numPuschSdu)
                    {
                        uart_printf("PHY UL Throughput (Last 5 seconds when UE was transmitting):                    (");
                        for(j = 3; j < 8; j++)
                            uart_printf("%6d ", pUserStat->ULThroughput[j]*8/1000);
                        uart_printf(") Kbps\r\n");
                        uart_printf("                                                                                 ");
                        for(j = 3; j < 7; j++)
                        {
                            if (pUserStat->ULThroughput[1] == j)
                                break;
                            uart_printf("       ");
                        }
                        uart_printf("------\r\n");
                        MxDelayTicks(15000000);
                    }

                    // Total Data Transferred
                    pSize = (U64*)pUserStat->totalDlData;
                    if (*pSize)
                    {
                        LteBsPhyStatsPrintTotalBytesTransferred(DL, *pSize, NULL);
                    }
                    pSize = (U64*)pUserStat->totalUlData;
                    if (*pSize)
                    {
                        LteBsPhyStatsPrintTotalBytesTransferred(UL, *pSize, NULL);
                    }

                    // UL Average CQI
                    if (pucchAverageCqiPresent + puschAverageCqiPresent)
                    {
                        if (puschAverageCqiPresent)
                        {
                            uart_printf("PHY PUSCH Average CQI (Last 5 seconds when UE was transmitting):                (");
                            for(j = 0; j < 5; j++)
                                uart_printf("%6d ", pUserStat->puschAverageCqi[j+1]);
                            uart_printf(") dB\r\n");
                            uart_printf("                                                                                 ");
                            for(j = 0; j < 4; j++)
                            {
                                if (pUserStat->puschAverageCqi[0] == j)
                                    break;
                                uart_printf("       ");
                            }
                            uart_printf("------\r\n");
                        }

                        if (pucchAverageCqiPresent)
                        {
                            uart_printf("PHY PUCCH Average CQI (Last 5 seconds when UE was transmitting):                (");
                            for(j = 0; j < 5; j++)
                                uart_printf("%6d ", pUserStat->pucchAverageCqi[j+1]);
                            uart_printf(") dB\r\n");
                            uart_printf("                                                                                 ");
                            for(j = 0; j < 4; j++)
                            {
                                if (pUserStat->pucchAverageCqi[0] == j)
                                    break;
                                uart_printf("       ");
                            }
                            uart_printf("------\r\n");
                        }
                        MxDelayTicks(15000000);
                    }

                    // DL CQI
                    if (dlCqiPresent)
                    {
                        uart_printf("|============|================|===========");
                        for(j = 1; j < 11; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        uart_printf("|  DL CQI    |   Max      Min |      < 5  ");
                        for(j = 1; j < 11; j++)
                            uart_printf("  %6d  ", j + 4);
                        uart_printf("   >= %d |\r\n", j + 4);

                        uart_printf("|============|================|===========");
                        for(j = 1; j < 11; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        for (j = 0; j < 12; j++)
                            sumStats[j] = 0;
                        maxStat = 0;
                        minStat = 32767;

                        // DL CQI on PUCCH
                        for (pucchFormat = FORMAT2; pucchFormat <= FORMAT2B; pucchFormat++)
                        {
                            index = 1;
                            index += (pucchFormat - FORMAT2);
                            pucchNum = 1;
                            if (pucchFormat > FORMAT1B)
                                pucchNum = 2;

                            pucchChar = ' ';
                            if ((pucchFormat == FORMAT1A) || (pucchFormat == FORMAT2A))
                                pucchChar = 'A';
                            if ((pucchFormat == FORMAT1B) || (pucchFormat == FORMAT2B))
                                pucchChar = 'B';

                            if (pUserStat->pdschNumCqi[index-1])
                            {
                                sum = 0;
                                uart_printf("|PUCCH [%d%c]  | %6d  %6d |", pucchNum, pucchChar,
                                    pUserStat->pdschCqiMax[index], pUserStat->pdschCqiMin[index]);
                                if (maxStat < pUserStat->pdschCqiMax[index])
                                    maxStat = pUserStat->pdschCqiMax[index];
                                if (minStat > pUserStat->pdschCqiMin[index])
                                    minStat = pUserStat->pdschCqiMin[index];
                                for(j = 0; j < 12; j++)
                                {
                                    uart_printf(" %8d ", pUserStat->pdschCqi[index][j]);
                                    sum += pUserStat->pdschCqi[index][j];
                                    sumStats[j] += pUserStat->pdschCqi[index][j];
                                }
                                uart_printf("|\r\n");

                                // Percentage
                                uart_printf("|            |                |");
                                for(j = 0; j < 12; j++)
                                {
                                    uart_printf(" %7llu%% ", (((U64)pUserStat->pdschCqi[index][j]*100)/sum));
                                }
                                uart_printf("|\r\n");

                                dlCqiPresent--;
                                if (dlCqiPresent)
                                {
                                    uart_printf("|------------|----------------|-----------");
                                    for(j = 1; j < 11; j++)
                                        uart_printf("----------");
                                    uart_printf("---------|\r\n");
                                }
                            }
                        }

                        // DL CQI on MUX PUSCH
                        if (pUserStat->numMuxPuschCqiPusch)
                        {
                            sum = 0;
                            uart_printf("|MUX PUSCH   | %6d  %6d |",
                                pUserStat->pdschCqiMax[0], pUserStat->pdschCqiMin[0]);
                            if (maxStat < pUserStat->pdschCqiMax[0])
                                maxStat = pUserStat->pdschCqiMax[0];
                            if (minStat > pUserStat->pdschCqiMin[0])
                                minStat = pUserStat->pdschCqiMin[0];
                            for(j = 0; j < 12; j++)
                            {
                                uart_printf(" %8d ", pUserStat->pdschCqi[0][j]);
                                sum += pUserStat->pdschCqi[0][j];
                                sumStats[j] += pUserStat->pdschCqi[0][j];
                            }
                            uart_printf("|\r\n");

                            // Percentage
                            uart_printf("|            |                |");
                            for(j = 0; j < 12; j++)
                            {
                                uart_printf(" %7llu%% ", (((U64)pUserStat->pdschCqi[0][j]*100)/sum));
                            }
                            uart_printf("|\r\n");

                            dlCqiPresent--;
                            if (dlCqiPresent)
                            {
                                uart_printf("|------------|----------------|-----------");
                                for(j = 1; j < 11; j++)
                                    uart_printf("----------");
                                uart_printf("---------|\r\n");
                            }
                        }

                        uart_printf("|============|================|===========");
                        for(j = 1; j < 11; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        // SUM
                        //------
                        sum = 0;
                        uart_printf("|SUM         | %6d  %6d |", maxStat, minStat);
                        for(j = 0; j < 12; j++)
                        {
                            uart_printf(" %8llu ", sumStats[j]);
                            sum += sumStats[j];
                        }
                        uart_printf("|\r\n");

                        // Percentage
                        uart_printf("|            |                |");
                        for(j = 0; j < 12; j++)
                        {
                            uart_printf(" %7llu%% ", (((U64)sumStats[j]*100)/sum));
                        }
                        uart_printf("|\r\n");

                        uart_printf("|============|================|===========");
                        for(j = 1; j < 11; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        uart_printf("\r\n");

                        MxDelayTicks(15000000);
                    }


                    // DL RI
                    if (dlRiPresent)
                    {
                        uart_printf("|============|================|===========");
                        for(j = 1; j < 2; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        uart_printf("|   DL RI    |   Max      Min |       1   ");
                        for(j = 1; j < 2; j++)
                            uart_printf("  %6d  ", j+1);
                        uart_printf("    >= %d |\r\n", j+1);

                        uart_printf("|============|================|===========");
                        for(j = 1; j < 2; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        for (j = 0; j < 12; j++)
                            sumStats[j] = 0;
                        maxStat = 0;
                        minStat = 32767;

                        // DL RI on PUCCH
                        for (pucchFormat = FORMAT2; pucchFormat <= FORMAT2B; pucchFormat++)
                        {
                            index = 1;
                            index += (pucchFormat - FORMAT2);
                            pucchNum = 1;
                            if (pucchFormat > FORMAT1B)
                                pucchNum = 2;

                            pucchChar = ' ';
                            if ((pucchFormat == FORMAT1A) || (pucchFormat == FORMAT2A))
                                pucchChar = 'A';
                            if ((pucchFormat == FORMAT1B) || (pucchFormat == FORMAT2B))
                                pucchChar = 'B';

                            if (pUserStat->pdschNumRi[index-1])
                            {
                                sum = 0;
                                uart_printf("|PUCCH [%d%c]  | %6d  %6d |", pucchNum, pucchChar,
                                    pUserStat->pdschRiMax[index], pUserStat->pdschRiMin[index]);
                                if (maxStat < pUserStat->pdschRiMax[index])
                                    maxStat = pUserStat->pdschRiMax[index];
                                if (minStat > pUserStat->pdschRiMin[index])
                                    minStat = pUserStat->pdschRiMin[index];
                                for(j = 0; j < 3; j++)
                                {
                                    uart_printf(" %8d ", pUserStat->pdschRi[index][j]);
                                    sum += pUserStat->pdschRi[index][j];
                                    sumStats[j] += pUserStat->pdschRi[index][j];
                                }
                                uart_printf("|\r\n");

                                // Percentage
                                uart_printf("|            |                |");
                                for(j = 0; j < 3; j++)
                                {
                                    uart_printf(" %7llu%% ", (((U64)pUserStat->pdschRi[index][j]*100)/sum));
                                }
                                uart_printf("|\r\n");

                                dlRiPresent--;
                                if (dlRiPresent)
                                {
                                    uart_printf("|------------|----------------|-----------");
                                    for(j = 1; j < 2; j++)
                                        uart_printf("----------");
                                    uart_printf("---------|\r\n");
                                }
                            }
                        }

                        // DL RI on MUX PUSCH
                        if (pUserStat->numMuxPuschRiPusch)
                        {
                            sum = 0;
                            uart_printf("|MUX PUSCH   | %6d  %6d |",
                                pUserStat->pdschRiMax[0], pUserStat->pdschRiMin[0]);
                            if (maxStat < pUserStat->pdschRiMax[0])
                                maxStat = pUserStat->pdschRiMax[0];
                            if (minStat > pUserStat->pdschRiMin[0])
                                minStat = pUserStat->pdschRiMin[0];
                            for(j = 0; j < 3; j++)
                            {
                                uart_printf(" %8d ", pUserStat->pdschRi[0][j]);
                                sum += pUserStat->pdschRi[0][j];
                                sumStats[j] += pUserStat->pdschRi[0][j];
                            }
                            uart_printf("|\r\n");

                            // Percentage
                            uart_printf("|            |                |");
                            for(j = 0; j < 3; j++)
                            {
                                uart_printf(" %7llu%% ", (((U64)pUserStat->pdschRi[0][j]*100)/sum));
                            }
                            uart_printf("|\r\n");

                            dlRiPresent--;
                            if (dlRiPresent)
                            {
                                uart_printf("|------------|----------------|-----------");
                                for(j = 1; j < 2; j++)
                                    uart_printf("----------");
                                uart_printf("---------|\r\n");
                            }
                        }

                        uart_printf("|============|================|===========");
                        for(j = 1; j < 2; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        // SUM
                        //------
                        sum = 0;
                        uart_printf("|SUM         | %6d  %6d |", maxStat, minStat);
                        for(j = 0; j < 3; j++)
                        {
                            uart_printf(" %8llu ", sumStats[j]);
                            sum += sumStats[j];
                        }
                        uart_printf("|\r\n");

                        // Percentage
                        uart_printf("|            |                |");
                        for(j = 0; j < 3; j++)
                        {
                            uart_printf(" %7llu%% ", (((U64)sumStats[j]*100)/sum));
                        }
                        uart_printf("|\r\n");

                        uart_printf("|============|================|===========");
                        for(j = 1; j < 2; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        uart_printf("\r\n");

                        MxDelayTicks(15000000);
                    }


                    // UL SINR
                    if (ulPresent)
                    {
                        uart_printf("|============|================|===========");
                        for(j = 1; j < 11; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        uart_printf("|  UL SINR   |    Max     Min |      <-6  ");
                        for(j = 1; j < 11; j++)
                            uart_printf("  %6d  ", j*3 - 6);
                        uart_printf("     >%d |\r\n", (j-1)*3 - 6);

                        uart_printf("|============|================|===========");
                        for(j = 1; j < 11; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        for (j = 0; j < 12; j++)
                            sumStats[j] = 0;
                        maxStat = 0;
                        minStat = 32767;

                        if (pUserStat->numPuschSdu)
                        {
                            sum = 0;
                            uart_printf("|PUSCH       | %6d  %6d |",((pUserStat->sinrPuschMax - 128) / 2), ((pUserStat->sinrPuschMin - 128) / 2));
                            if (maxStat < pUserStat->sinrPuschMax)
                                maxStat = pUserStat->sinrPuschMax;
                            if (minStat > pUserStat->sinrPuschMin)
                                minStat = pUserStat->sinrPuschMin;
                            for(j = 0; j < 12; j++)
                            {
                                uart_printf(" %8d ", pUserStat->sinrPuschBins[j]);
                                sum += pUserStat->sinrPuschBins[j];
                                sumStats[j] += pUserStat->sinrPuschBins[j];
                            }
                            uart_printf("|\r\n");

                            // Percentage
                            uart_printf("|            |                |");
                            for(j = 0; j < 12; j++)
                            {
                                uart_printf(" %7llu%% ", (((U64)pUserStat->sinrPuschBins[j]*100)/sum));
                            }
                            uart_printf("|\r\n");

                            ulSinrPresent--;
                            if (ulSinrPresent)
                            {
                                uart_printf("|------------|----------------|-----------");
                                for(j = 1; j < 11; j++)
                                    uart_printf("----------");
                                uart_printf("---------|\r\n");
                            }

                        }

                        for (pucchFormat = FORMAT1; pucchFormat <= FORMAT2B; pucchFormat++)
                        {
                            pucchNum = 1;
                            if (pucchFormat > FORMAT1B)
                                pucchNum = 2;

                            pucchChar = ' ';
                            if ((pucchFormat == FORMAT1A) || (pucchFormat == FORMAT2A))
                                pucchChar = 'A';
                            if ((pucchFormat == FORMAT1B) || (pucchFormat == FORMAT2B))
                                pucchChar = 'B';

                            if (pUserStat->numPucchNumSdu[pucchFormat])
                            {
                                sum = 0;
                                uart_printf("|PUCCH [%d%c]  | %6d  %6d |", pucchNum, pucchChar,
                                    ((pUserStat->sinrPucchMax[pucchFormat] - 128) / 2), ((pUserStat->sinrPucchMin[pucchFormat] - 128) / 2));
                                if (maxStat < pUserStat->sinrPucchMax[pucchFormat])
                                    maxStat = pUserStat->sinrPucchMax[pucchFormat];
                                if (minStat > pUserStat->sinrPucchMin[pucchFormat])
                                    minStat = pUserStat->sinrPucchMin[pucchFormat];
                                for(j = 0; j < 12; j++)
                                {
                                    uart_printf(" %8d ", pUserStat->sinrPucchBins[pucchFormat][j]);
                                    sum += pUserStat->sinrPucchBins[pucchFormat][j];
                                    sumStats[j] += pUserStat->sinrPucchBins[pucchFormat][j];
                                }
                                uart_printf("|\r\n");

                                // Percentage
                                uart_printf("|            |                |");
                                for(j = 0; j < 12; j++)
                                {
                                    uart_printf(" %7llu%% ", (((U64)pUserStat->sinrPucchBins[pucchFormat][j]*100)/sum));
                                }
                                uart_printf("|\r\n");

                                ulSinrPresent--;
                                if (ulSinrPresent)
                                {
                                    uart_printf("|------------|----------------|-----------");
                                    for(j = 1; j < 11; j++)
                                        uart_printf("----------");
                                    uart_printf("---------|\r\n");
                                }

                            }
                        }

                        uart_printf("|============|================|===========");
                        for(j = 1; j < 11; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        // SUM
                        //------
                        sum = 0;
                        uart_printf("|SUM         | %6d  %6d |", ((maxStat - 128) / 2), (((S32)minStat - 128) / 2));
                        for(j = 0; j < 12; j++)
                        {
                            uart_printf(" %8llu ", sumStats[j]);
                            sum += sumStats[j];
                        }
                        uart_printf("|\r\n");

                        // Percentage
                        uart_printf("|            |                |");
                        for(j = 0; j < 12; j++)
                        {
                            uart_printf(" %7llu%% ", (((U64)sumStats[j]*100)/sum));
                        }
                        uart_printf("|\r\n");

                        uart_printf("|============|================|===========");
                        for(j = 1; j < 11; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        uart_printf("\r\n");

                        MxDelayTicks(15000000);
                    }

                    // UL Time Advance
                    if (ulPresent)
                    {
                        uart_printf("|============|================|===========");
                        for(j = 1; j < 11; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        uart_printf("| UL TM ADV  |    Max     Min |      <25  ");
                        for(j = 1; j < 11; j++)
                            uart_printf("  %6d  ", j+24);
                        uart_printf("     >%d |\r\n", (j-1)+24);

                        uart_printf("|============|================|===========");
                        for(j = 1; j < 11; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        for (j = 0; j < 12; j++)
                            sumStats[j] = 0;
                        maxStat = 0;
                        minStat = 32767;

                        if (pUserStat->numPuschSdu)
                        {
                            sum = 0;
                            uart_printf("|PUSCH       | %6d  %6d |", pUserStat->timeAdvPuschMax, pUserStat->timeAdvPuschMin);
                            if (maxStat < (U32)pUserStat->timeAdvPuschMax)
                                maxStat = (U32)pUserStat->timeAdvPuschMax;
                            if (minStat > (U32)pUserStat->timeAdvPuschMin)
                                minStat = (U32)pUserStat->timeAdvPuschMin;
                            for(j = 0; j < 12; j++)
                            {
                                uart_printf(" %8d ", pUserStat->timeAdvPuschBins[j]);
                                sum += pUserStat->timeAdvPuschBins[j];
                                sumStats[j] += pUserStat->timeAdvPuschBins[j];
                            }
                            uart_printf("|\r\n");

                            uart_printf("|            |                |");
                            for(j = 0; j < 12; j++)
                            {
                                uart_printf(" %7llu%% ", (((U64)pUserStat->timeAdvPuschBins[j]*100)/sum));
                            }
                            uart_printf("|\r\n");

                            ulTmgAdvPresent--;
                            if (ulTmgAdvPresent)
                            {
                                uart_printf("|------------|----------------|-----------");
                                for(j = 1; j < 11; j++)
                                    uart_printf("----------");
                                uart_printf("---------|\r\n");
                            }
                        }

                        for (pucchFormat = FORMAT1; pucchFormat <= FORMAT2B; pucchFormat++)
                        {
                            pucchNum = 1;
                            if (pucchFormat > FORMAT1B)
                                pucchNum = 2;

                            pucchChar = ' ';
                            if ((pucchFormat == FORMAT1A) || (pucchFormat == FORMAT2A))
                                pucchChar = 'A';
                            if ((pucchFormat == FORMAT1B) || (pucchFormat == FORMAT2B))
                                pucchChar = 'B';

                            if (pUserStat->numPucchNumSdu[pucchFormat])
                            {
                                sum = 0;
                                uart_printf("|PUCCH [%d%c]  | %6d  %6d |", pucchNum, pucchChar,
                                    pUserStat->timeAdvPucchMax[pucchFormat], pUserStat->timeAdvPucchMin[pucchFormat]);
                                if (maxStat < (U32)pUserStat->timeAdvPucchMax[pucchFormat])
                                    maxStat = (U32)pUserStat->timeAdvPucchMax[pucchFormat];
                                if (minStat > (U32)pUserStat->timeAdvPucchMin[pucchFormat])
                                    minStat = (U32)pUserStat->timeAdvPucchMin[pucchFormat];
                                for(j = 0; j < 12; j++)
                                {
                                    uart_printf(" %8d ", pUserStat->timeAdvPucchBins[pucchFormat][j]);
                                    sum += pUserStat->timeAdvPucchBins[pucchFormat][j];
                                    sumStats[j] += pUserStat->timeAdvPucchBins[pucchFormat][j];
                                }
                                uart_printf("|\r\n");

                                uart_printf("|            |                |");
                                for(j = 0; j < 12; j++)
                                {
                                    uart_printf(" %7llu%% ", (((U64)pUserStat->timeAdvPucchBins[pucchFormat][j]*100)/sum));
                                }
                                uart_printf("|\r\n");

                                ulTmgAdvPresent--;
                                if (ulTmgAdvPresent)
                                {
                                    uart_printf("|------------|----------------|-----------");
                                    for(j = 1; j < 11; j++)
                                        uart_printf("----------");
                                    uart_printf("---------|\r\n");
                                }
                            }
                        }

                        uart_printf("|============|================|===========");
                        for(j = 1; j < 11; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        // SUM
                        //------
                        sum = 0;
                        uart_printf("|SUM         | %6d  %6d |", maxStat, minStat);
                        for(j = 0; j < 12; j++)
                        {
                            uart_printf(" %8llu ", sumStats[j]);
                            sum += sumStats[j];
                        }
                        uart_printf("|\r\n");

                        // Percentage
                        uart_printf("|            |                |");
                        for(j = 0; j < 12; j++)
                        {
                            uart_printf(" %7llu%% ", (((U64)sumStats[j]*100)/sum));
                        }
                        uart_printf("|\r\n");

                        uart_printf("|============|================|===========");
                        for(j = 1; j < 11; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        uart_printf("\r\n");
                        MxDelayTicks(15000000);
                    }


                    // DL / UL MCS
                    if (pUserStat->numPdschSdu[0] + pUserStat->numPdschSdu[1] + pUserStat->numPuschSdu)
                    {
                        uart_printf("|============|================|===========");
                        for(j = 1; j < 14; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");
                        uart_printf("| DL/UL MCS  |    Max     Min |      <15  ");
                        for(j = 1; j < 14; j++)
                            uart_printf("  %6d  ", j+14);
                        uart_printf("     >%d |\r\n", (j-1)+14);

                        uart_printf("|============|================|===========");
                        for(j = 1; j < 14; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        mcsCount = 0;
                        for (k = 0; k < 3; k++)
                        {
                            mcsPresent[k] = 0;
                            sum = 0;
                            for(j = 0; j < 15; j++)
                            {
                                sum += pUserStat->mcs[k][j];
                            }
                            if (sum)
                            {
                                mcsCount++;
                                mcsPresent[k] = 1;
                            }
                        }

                        for(k = 0; k < 3; k++)
                        {
                            if (mcsPresent[k])
                            {
                                sum = 0;
                                if (k == 2)
                                {
                                    uart_printf("|   UL MCS   | %6d  %6d |", pUserStat->maxMcs[k], pUserStat->minMcs[k]);
                                }
                                else
                                {
                                    uart_printf("| DL MCS[%d]  | %6d  %6d |", k, pUserStat->maxMcs[k], pUserStat->minMcs[k]);
                                }
                                for(j = 0; j < 15; j++)
                                {
                                    uart_printf(" %8d ", pUserStat->mcs[k][j]);
                                    sum += pUserStat->mcs[k][j];
                                }
                                uart_printf("|\r\n");

                                // Percentage
                                uart_printf("|            |                |");
                                for(j = 0; j < 15; j++)
                                {
                                    uart_printf(" %7llu%% ", (((U64)pUserStat->mcs[k][j]*100)/sum));
                                }
                                uart_printf("|\r\n");

                                mcsCount--;
                                if (mcsCount)
                                {
                                    uart_printf("|------------|----------------|-----------");
                                    for(j = 1; j < 14; j++)
                                        uart_printf("----------");
                                    uart_printf("---------|\r\n");
                                }
                            }
                        }

                        uart_printf("|============|================|===========");
                        for(j = 1; j < 14; j++)
                            uart_printf("==========");
                        uart_printf("=========|\r\n");

                        uart_printf("\r\n");

                        MxDelayTicks(15000000);
                    }

                    if (uciPeriodicity)
                    {
                        uart_printf("|=====================|=================|\r\n");
                        uart_printf("| UL UCI Periodicity  |   Max  |   Min  |\r\n");
                        uart_printf("|=====================|=================|\r\n");

                        if (pUserStat->srPeriodicity[0] != PHY_STATS_PERIODITICY_NOT_STATED)
                            uart_printf("| Schedule Request    | %6d | %6d |\r\n", pUserStat->srPeriodicity[1], pUserStat->srPeriodicity[2]);
                        if (pUserStat->dlCqiPeriodicity[0] != PHY_STATS_PERIODITICY_NOT_STATED)
                            uart_printf("| DL CQI Request      | %6d | %6d |\r\n", pUserStat->dlCqiPeriodicity[1], pUserStat->dlCqiPeriodicity[2]);
                        if (pUserStat->dlRiPeriodicity[0] != PHY_STATS_PERIODITICY_NOT_STATED)
                            uart_printf("| DL RI Request       | %6d | %6d |\r\n", pUserStat->dlRiPeriodicity[1], pUserStat->dlRiPeriodicity[2]);

                        uart_printf("|=====================|=================|\r\n");

                        uart_printf("\r\n");

                        MxDelayTicks(15000000);
                    }

                    // CQI PMI Confidence Metric
                    if (pUserStat->cqiPmiConfMetricCnt[0] + pUserStat->cqiPmiConfMetricCnt[1] + pUserStat->cqiPmiConfMetricCnt[2] + pUserStat->cqiPmiConfMetricCnt[3])
                    {
                        uart_printf("|=====================|===========|===========|===========|===========|\r\n");
                        uart_printf("| CQI PMI Conf Metric | FORMAT 2  | FORMAT 2A | FORMAT 2B | MUX PUSCH |\r\n");
                        uart_printf("|=====================|===========|===========|===========|===========|\r\n");

                        uart_printf("| Good Confidence     |   %7d |   %7d |   %7d |   %7d |\r\n",
                            pUserStat->cqiPmiConfMetric[0], pUserStat->cqiPmiConfMetric[1],
                            pUserStat->cqiPmiConfMetric[2], pUserStat->cqiPmiConfMetric[3]);

                        uart_printf("| Total Occurances    |   %7d |   %7d |   %7d |   %7d |\r\n",
                            pUserStat->cqiPmiConfMetricCnt[0], pUserStat->cqiPmiConfMetricCnt[1],
                            pUserStat->cqiPmiConfMetricCnt[2], pUserStat->cqiPmiConfMetricCnt[3]);

                        uart_printf("|                     |");
                        for (k = 0; k < 4; k++)
                            uart_printf("  %7d%% |", (pUserStat->cqiPmiConfMetricCnt[k] ?
                            (U32)((pUserStat->cqiPmiConfMetric[k] * 100) / pUserStat->cqiPmiConfMetricCnt[k]) : 0));
                        uart_printf("\r\n");

                        uart_printf("|=====================|===========|===========|===========|===========|\r\n");

                        MxDelayTicks(15000000);
                    }

                    uart_printf("\r\n");
                    MxDelayTicks(150000000);
                }
            }
        }

        if (phyStatsEnabledTimeHist)
        {
            MxDelayTicks(150000000);
            uart_printf("\r\n\r\n");
            uart_printf("Time Histogram:\r\n");
            uart_printf("---------------\r\n\r\n");

            uart_printf("The First column is the Frame and Subframe Number\r\n");
            uart_printf("The Second column is has Number of Data channels (PDSCH / PUSCH) and Number of Control Channels (PDCCH / PUCCH)\r\n");

            uart_printf("Subsequent columns are UE column and the format of data in them is as follows:\r\n");
            uart_printf("A B C (D E F) (G H I)\r\n");
            uart_printf("A = Format Type: B = BCH, DS = PDSCH, DC = PDCCH, D0 = PDCCH_DCI0, US = PUSCH, (1, 1A, 1B, 2, 2A, 2B) = PUCCH\r\n");
            uart_printf("B = Number of Code Words (for DL Only)\r\n");
            uart_printf("B = CQI - PMI (C) / HARQ (H) / RI (R) / Simul SR (S) Present (for UL Only)\r\n");
            uart_printf("C = Number of Resource Blocks (for PDSCH and PUSCH). n1 (for PUCCH)\r\n");
            uart_printf("D = rv  (for DL CW0) or re-transmission Num (PUSCH). srPUCCHResourceIndex (for PUCCH)\r\n");
            uart_printf("E = mcs (for DL CW0 or PUSCH). n2 (for PUCCH)\r\n");
            uart_printf("F = Qm  (for DL CW0 or PUSCH)\r\n");
            uart_printf("G = rv  (for DL CW1) or Status (for PUSCH) or Out[1] (for PUCCH. If DTX = 0xFF)\r\n");
            uart_printf("H = mcs (for DL CW1) or Timing Advance (for PUSCH and PUCCH)\r\n");
            uart_printf("I = Qm  (for DL CW0) or SINR (for PUSCH and PUCCH)\r\n");
            uart_printf("\r\n\r\n");

            uart_printf("DL Histogram\r\n");
            uart_printf("Max Number of DL Channels in a Subframe:          %d\r\n", pTimeHistogram->maxDLChannelsPerSubframe);
            uart_printf("Max Number of DL Data Channels in a Subframe:     %d\r\n", pTimeHistogram->maxPdschChannelsPerSubframe);
            uart_printf("Max Number of DL Control Channels in a Subframe:  %d\r\n", pTimeHistogram->maxPdcchChannelsPerSubframe);

            uart_printf("\r\nLast %4d Subframes of DL Activity\r\n\r\n", LTE_BS_PHY_STATS_TIME_HISTOGRAM_DURATION);

            // Find the total number of RNTIs in all subframes logged and store them
            totalRNTIs = 0;
            for (i = 0; i < LTE_BS_PHY_STATS_TIME_HISTOGRAM_DURATION; i++)
            {
                for (j = 0; j < pTimeHistogram->subframeCtx[i].numDlChs; j++)
                {
                    pUeStatsCtx = &pTimeHistogram->subframeCtx[i].dlCtx[j];
                    LteBsPhyStatsAddRnti(RNTIMap, &totalRNTIs, pUeStatsCtx->RNTI);
                }
            }
            LteBsPhyStatsSortRntiMap(RNTIMap, totalRNTIs, NULL);

            // Print Header
            uart_printf("-----------|-----|");
            uart_printf("------------------------------|");
            for (i = 0; i < totalRNTIs; i++)
            {
                uart_printf("------------------------------|");
            }
            uart_printf("\r\n");
            uart_printf(" Subframe  |     |");
            uart_printf("              BCH             |");
            for (i = 0; i < totalRNTIs; i++)
            {
                uart_printf("            UE %5d          |", RNTIMap[i]);
            }
            uart_printf("\r\n");
            uart_printf("-----------|-----|");
            uart_printf("------------------------------|");
            for (i = 0; i < totalRNTIs; i++)
            {
                uart_printf("------------------------------|");
            }
            uart_printf("\r\n");

            // Print all of the UEs data
            if (pTimeHistogram->dlCtxCircle)
            {
                j = pTimeHistogram->currDlCtx;
                stop = LTE_BS_PHY_STATS_TIME_HISTOGRAM_DURATION;
            }
            else
            {
                j = LTE_BS_PHY_STATS_TIME_HISTOGRAM_DURATION-1;
                stop = pTimeHistogram->currDlCtx;
            }
            for (i = 0; i < stop; i++)
            {
                // j represents the subframes in reverse order and hence will be used for
                j++;
                if (j == LTE_BS_PHY_STATS_TIME_HISTOGRAM_DURATION)
                    j = 0;

                // Count the Number of Channels
                numData = 0;
                numCtrl = 0;
                numDci = 0;
                for (k = 0; k < pTimeHistogram->subframeCtx[j].numDlChs; k++)
                {
                    pUeStatsCtx = &pTimeHistogram->subframeCtx[j].dlCtx[k];
                    if (pUeStatsCtx->type == PDSCH || pUeStatsCtx->type == PBCH)
                    {
                        numData++;
                    }
                    else if (pUeStatsCtx->type == PDCCH)
                    {
                        numCtrl++;
                    }
                    else if (pUeStatsCtx->type == PDCCH_DCI0)
                    {
                        numDci++;
                    }
                }

                // Print Subframe Number
                frNum = pTimeHistogram->subframeCtx[j].dlSfn / 10;
                sfNum = (pTimeHistogram->subframeCtx[j].dlSfn - (frNum*10));
                uart_printf("(%4d, %2d) | %1d %1d |", frNum, sfNum,
                                    numData, numCtrl+numDci);

                // BCH
                // Fill the Present MAP
                foundBch = 0;
                for (k = 0; k < pTimeHistogram->subframeCtx[j].numDlChs; k++)
                {
                    pUeStatsCtx = &pTimeHistogram->subframeCtx[j].dlCtx[k];
                    if (pUeStatsCtx->type == PBCH)
                    {
                        LteBsPhyStatsConstructString(DL, pString, pUeStatsCtx);
                        uart_printf("%s |", pString);
                        foundBch = 1;
                    }
                }
                if (foundBch == 0)
                {
                    LteBsPhyStatsConstructString(DL, pString, NULL);
                    uart_printf("%s |", pString);
                }

                // PDSCH
                // Clear the RNTI Present MAP
                for (k = 0; k < LTE_BS_PHY_STATS_MAX_CHANNELS_PER_TTI*4; k++)
                    RNTIPresent[k] = 0;

                for (k = 0; k < pTimeHistogram->subframeCtx[j].numDlChs; k++)
                {
                    pUeStatsCtx = &pTimeHistogram->subframeCtx[j].dlCtx[k];
                    if (pUeStatsCtx->type == PDSCH)
                    {
                        index = LteBsPhyStatsGetRntiMapIndex(RNTIMap, totalRNTIs, pUeStatsCtx->RNTI);
                        RNTIPresent[index] = k+1;
                    }
                }

                // Start to print the UE data
                for (k = 0; k < totalRNTIs; k++)
                {
                    if (RNTIPresent[k])
                    {
                        pUeStatsCtx = &pTimeHistogram->subframeCtx[j].dlCtx[RNTIPresent[k]-1];
                        LteBsPhyStatsConstructString(DL, pString, pUeStatsCtx);
                        uart_printf("%s |", pString);
                    }
                    else
                    {
                        LteBsPhyStatsConstructString(DL, pString, NULL);
                        uart_printf("%s |", pString);
                    }
                }
                uart_printf("\r\n");
                uart_printf("           |     |");
                LteBsPhyStatsConstructString(DL, pString, NULL);
                uart_printf("%s |", pString);

                // PDCCH
                // Clear the RNTI Present MAP
                for (k = 0; k < LTE_BS_PHY_STATS_MAX_CHANNELS_PER_TTI*4; k++)
                    RNTIPresent[k] = 0;

                // Fill the Present MAP
                for (k = 0; k < pTimeHistogram->subframeCtx[j].numDlChs; k++)
                {
                    pUeStatsCtx = &pTimeHistogram->subframeCtx[j].dlCtx[k];
                    if (pUeStatsCtx->type == PDCCH)
                    {
                        index = LteBsPhyStatsGetRntiMapIndex(RNTIMap, totalRNTIs, pUeStatsCtx->RNTI);
                        RNTIPresent[index] = k+1;
                    }
                }

                // Start to print the UE data
                for (k = 0; k < totalRNTIs; k++)
                {
                    if (RNTIPresent[k])
                    {
                        pUeStatsCtx = &pTimeHistogram->subframeCtx[j].dlCtx[RNTIPresent[k]-1];
                        LteBsPhyStatsConstructString(DL, pString, pUeStatsCtx);
                        uart_printf("%s |", pString);
                    }
                    else
                    {
                        LteBsPhyStatsConstructString(DL, pString, NULL);
                        uart_printf("%s |", pString);
                    }
                }
                uart_printf("\r\n");
                uart_printf("           |     |");
                LteBsPhyStatsConstructString(DL, pString, NULL);
                uart_printf("%s |", pString);

                // PDCCH DCI0
                // Clear the RNTI Present MAP
                for (k = 0; k < LTE_BS_PHY_STATS_MAX_CHANNELS_PER_TTI*4; k++)
                    RNTIPresent[k] = 0;

                // Fill the Present MAP
                for (k = 0; k < pTimeHistogram->subframeCtx[j].numDlChs; k++)
                {
                    pUeStatsCtx = &pTimeHistogram->subframeCtx[j].dlCtx[k];
                    if (pUeStatsCtx->type == PDCCH_DCI0)
                    {
                        index = LteBsPhyStatsGetRntiMapIndex(RNTIMap, totalRNTIs, pUeStatsCtx->RNTI);
                        RNTIPresent[index] = k+1;
                    }
                }

                // Start to print the UE data
                for (k = 0; k < totalRNTIs; k++)
                {
                    if (RNTIPresent[k])
                    {
                        pUeStatsCtx = &pTimeHistogram->subframeCtx[j].dlCtx[RNTIPresent[k]-1];
                        LteBsPhyStatsConstructString(DL, pString, pUeStatsCtx);
                        uart_printf("%s |", pString);
                    }
                    else
                    {
                        LteBsPhyStatsConstructString(DL, pString, NULL);
                        uart_printf("%s |", pString);
                    }
                }

                uart_printf("\r\n");
                MxDelayTicks(1500000*3);
            }
            uart_printf("-----------|-----|");
            uart_printf("------------------------------|");
            for (i = 0; i < totalRNTIs; i++)
            {
                uart_printf("------------------------------|");
            }
            uart_printf("\r\n");

            MxDelayTicks(150000000);


            uart_printf("\r\n\r\nUL Histogram\r\n");
            uart_printf("Max Number of UL Channels in a Subframe:          %d\r\n", pTimeHistogram->maxULChannelsPerSubframe);
            uart_printf("Max Number of UL Data Channels in a Subframe:     %d\r\n", pTimeHistogram->maxPuschChannelsPerSubframe);
            uart_printf("Max Number of UL Control Channels in a Subframe:  %d\r\n", pTimeHistogram->maxPucchChannelsPerSubframe);

            uart_printf("\r\nLast %4d Subframes of UL Activity\r\n\r\n", LTE_BS_PHY_STATS_TIME_HISTOGRAM_DURATION);

            // Find the total number of RNTIs in all subframes logged and store them
            totalRNTIs = 0;
            for (i = 0; i < LTE_BS_PHY_STATS_TIME_HISTOGRAM_DURATION; i++)
            {
                for (j = 0; j < pTimeHistogram->subframeCtx[i].numUlChs; j++)
                {
                    pUeStatsCtx = &pTimeHistogram->subframeCtx[i].ulCtx[j];
                    if ((pUeStatsCtx->type >> 4) != PRACH)
                    {
                        LteBsPhyStatsAddRnti(RNTIMap, &totalRNTIs, pUeStatsCtx->RNTI);
                    }
                    else
                    {
                        LteBsPhyStatsAddRnti(RNTIMap, &totalRNTIs, 100000);
                    }
                }
            }
            LteBsPhyStatsSortRntiMap(RNTIMap, totalRNTIs, NULL);

            // Print Header
            uart_printf("-----------|-----|");
            for (i = 0; i < totalRNTIs; i++)
            {
                uart_printf("---------------------------------|");
            }
            uart_printf("\r\n");
            uart_printf(" Subframe  |     |");
            for (i = 0; i < totalRNTIs; i++)
            {
                if (RNTIMap[i] != 100000)
                {
                    uart_printf("             UE %5d            |", RNTIMap[i]);
                }
                else
                {
                    uart_printf("               RACH              |");
                }
            }
            uart_printf("\r\n");
            uart_printf("-----------|-----|");
            for (i = 0; i < totalRNTIs; i++)
            {
                uart_printf("---------------------------------|");
            }
            uart_printf("\r\n");

            // Print all of the UEs data
            if (pTimeHistogram->ulCtxCircle)
            {
                j = pTimeHistogram->currUlCtx;
                stop = LTE_BS_PHY_STATS_TIME_HISTOGRAM_DURATION;
            }
            else
            {
                j = LTE_BS_PHY_STATS_TIME_HISTOGRAM_DURATION-1;
                stop = pTimeHistogram->currDlCtx;
            }
            for (i = 0; i < stop; i++)
            {
                // j represents the subframes in reverse order and hence will be used for
                j++;
                if (j == LTE_BS_PHY_STATS_TIME_HISTOGRAM_DURATION)
                    j = 0;

                // Count the Number of Channels
                numData = 0;
                numCtrl = 0;
                for (k = 0; k < pTimeHistogram->subframeCtx[j].numUlChs; k++)
                {
                    pUeStatsCtx = &pTimeHistogram->subframeCtx[j].ulCtx[k];
                    if ((pUeStatsCtx->type >> 4) == PUSCH)
                    {
                        numData++;
                    }
                    else if ((pUeStatsCtx->type >> 4) == PUCCH)
                    {
                        numCtrl++;
                    }
                }

                // Print Subframe Number
                frNum = pTimeHistogram->subframeCtx[j].ulSfn / 10;
                sfNum = (pTimeHistogram->subframeCtx[j].ulSfn - (frNum*10));
                uart_printf("(%4d, %2d) | %1d %1d |", frNum, sfNum,
                            numData, numCtrl);

                // Clear the RNTI Present MAP
                for (k = 0; k < LTE_BS_PHY_STATS_MAX_CHANNELS_PER_TTI*4; k++)
                    RNTIPresent[k] = 0;

                // Fill the Present MAP
                for (k = 0; k < pTimeHistogram->subframeCtx[j].numUlChs; k++)
                {
                    pUeStatsCtx = &pTimeHistogram->subframeCtx[j].ulCtx[k];
                    if ((pUeStatsCtx->type >> 4) != PRACH)
                    {
                        index = LteBsPhyStatsGetRntiMapIndex(RNTIMap, totalRNTIs, pUeStatsCtx->RNTI);
                    }
                    else
                    {
                        index = LteBsPhyStatsGetRntiMapIndex(RNTIMap, totalRNTIs, 100000);
                    }
                    RNTIPresent[index] = k+1;
                }

                // Start to print the UE data
                for (k = 0; k < totalRNTIs; k++)
                {
                    if (RNTIPresent[k])
                    {
                        pUeStatsCtx = &pTimeHistogram->subframeCtx[j].ulCtx[RNTIPresent[k]-1];
                        LteBsPhyStatsConstructString(UL, pString, pUeStatsCtx);
                        uart_printf("%s |", pString);
                    }
                    else
                    {
                        LteBsPhyStatsConstructString(UL, pString, NULL);
                        uart_printf("%s |", pString);
                    }
                }

                uart_printf("\r\n");
                MxDelayTicks(1500000*3);
            }
            uart_printf("-----------|-----|");
            for (i = 0; i < totalRNTIs; i++)
            {
                uart_printf("---------------------------------|");
            }
            uart_printf("\r\n");


            MxDelayTicks(150000000);
        }
        uart_printf("-------------------------------------------------------------------------------------------------------\r\n");
        uart_printf("\r\n\r\n\r\n");

    }
    return MX_OK;
}

#ifdef _WIN32
void fCloseAll()
{
    if (pFileIn)
        fclose(pFileIn);

    if (pFileOut)
        fclose(pFileOut);

    if (pFileLog)
        fclose(pFileLog);

    return;
}

#ifdef LTE_BS_PHY_STATS_LIB
int LteBsPhyStatsLib(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    char *pBuffer;
    PLTE_BS_PHY_STATS pPhyStats;
    S32 i;

    printf("\r\n");
    printf("Phy Stats Parser Version %d\r\n", PHYSTATS_VERSION_NUM);
    printf("--------------------------\r\n\r\n");

    argc--;
    if (argc < 2)
    {
        printf("Usage:\n");
        printf("PhyStatsParser <required_args> [optional_args]\n\n");

        printf("where:\n\n");

        printf("Required Arguments:\n");
        printf("-------------------\n\n");

        printf("1. Input file location (binary file)\n");
        printf("2. Output file location (test file)\n\n");

        printf("Optional Arguments:\n");
        printf("-------------------\n\n");
        getchar();
        exit(-1);
    }

    pFileLog = fopen("PhyStatsParser_log.txt","a");
    fprintf(pFileLog, "Phy Stats Parser Version %d\r\n", PHYSTATS_VERSION_NUM);
    fprintf(pFileLog, "--------------------------\r\n\r\n");

    // Log in command line arguments
    //------------------------------
    fprintf(pFileLog, "User Arguments:\n");
    fprintf(pFileLog, "---------------\n\n");
    for(i = 0; i <= argc ; i++)
        fprintf(pFileLog, "%s ", argv[i]);
    fprintf(pFileLog, "\n\n");

    fprintf(pFileLog, "Number of command line arguments = %d\n\n", argc);


    // Parse the arguments
    //--------------------

    // Input File
    pFileIn = fopen(argv[1], "rb");
    if (pFileIn == NULL)
    {
        printf("\r\nERROR: Input file: %s cannot be opened\r\n", argv[1]);
        fCloseAll();
        exit(-1);
    }
    printf("1. Input Location:\t%s\n", argv[1]);

    // Output Location
    pFileOut = fopen(argv[2], "w");
    if (pFileOut == NULL)
    {
        printf("\r\nERROR: Output file: %s cannot be opened\r\n", argv[2]);
        fCloseAll();
        exit(-1);
    }
    printf("2. Output Location:\t%s\n", argv[2]);

    // Allocate Buffer
    pBuffer = malloc(sizeof(LTE_BS_PHY_STATS));
    if (pBuffer == NULL)
    {
        printf("\r\nERROR: Cannot allocate memory\r\n");
        fCloseAll();
        exit(-1);
    }

    // Read the Input file
    printf("Sizeof(LTE_BS_PHY_STATS) = %d\r\n", sizeof(LTE_BS_PHY_STATS));
    fread(pBuffer, sizeof(LTE_BS_PHY_STATS), 1, pFileIn);


    // Start writing Stats to file
    pPhyStats = (PLTE_BS_PHY_STATS)pBuffer;

    // Check for Version and report if needed
    if (pPhyStats->header.Magic != PHYSTATS_HEAD)
    {
        printf("\r\nERROR: This is not a valid PhyStats File. Please check if it was dumped from the correct address\r\n");
        free(pBuffer);
        fCloseAll();
        return 0;
    }
    if (pPhyStats->header.Version != PHYSTATS_VERSION_NUM)
    {
        printf("\r\nERROR: This version of PhyStats (%d) is not supported by this version of tool (%d)\r\n", pPhyStats->header.Version, PHYSTATS_VERSION_NUM);
        printf("Please get a different version of tool that matches this file\r\n");
        free(pBuffer);
        fCloseAll();
        return 0;
    }

    LteBsPhyStatsPrint(pPhyStats);


    free(pBuffer);
    fCloseAll();

    return 0;

}
#endif  // _WIN32


#endif // LTE_BS_PHY_STATS_ENABLED
