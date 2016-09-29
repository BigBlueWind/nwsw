//-------------------------------------------------------------------------------------------
/** @file LteBsPhyStats.h
 *
 * @brief Phy Stats
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.11 $
 *
 * COPYRIGHT&copy; 2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */
//-------------------------------------------------------------------------------------------

#ifndef LTEBSPHYSTATS_H
#define LTEBSPHYSTATS_H

#include "ltephydef.h"
#include "ltephy.h"
#include "LtePhyL2Api.h"

#ifndef _WIN32
#include "apimgr.h"
#include "unified_diag.h"
#include "rex_serv.h"
#endif

#define LTE_BS_PHY_STATS_ENABLED


// Global flag storing PhyStats Enable status. This is used to disable certain stats bydefault
#ifndef _WIN32
#ifdef _LTE_BS_PHY_STATS_C
U32 phyStatsEnableFlag;
#else
extern U32 phyStatsEnableFlag;
#endif
#else
#ifdef _LTE_BS_PHY_STATS_C
U32 phyStatsEnableFlag = 0xFFFFFFFF;
FILE *pFileIn, *pFileOut, *pFileLog;
#endif
#endif

#define PHYSTATS_ENABLE_TMRSTS               1<<0
#define PHYSTATS_ENABLE_APISTATS             1<<1
#define PHYSTATS_ENABLE_RACH_STATS           1<<2
#define PHYSTATS_ENABLE_TIME_HISTOGRAM       1<<3
#define PHYSTATS_ENABLE_PHYSTATS_PRINT       1<<4

#define PHYSTATS_ENABLE_USRSTS_PDSCH         1<<5
#define PHYSTATS_ENABLE_USRSTS_PDCCH         1<<6
#define PHYSTATS_ENABLE_USRSTS_PUSCH         1<<7
#define PHYSTATS_ENABLE_USRSTS_PUCCH         1<<8
#define PHYSTATS_ENABLE_USRSTS_MUX_PUSCH     1<<9
#define PHYSTATS_ENABLE_USRSTS (PHYSTATS_ENABLE_USRSTS_PDSCH | \
                                PHYSTATS_ENABLE_USRSTS_PDCCH | \
                                PHYSTATS_ENABLE_USRSTS_PUSCH | \
                                PHYSTATS_ENABLE_USRSTS_PUCCH | \
                                PHYSTATS_ENABLE_USRSTS_MUX_PUSCH)




#define PHYSTATS_NUM_CTX              1

#define PHYSTATS_HEAD                 0x50485354  // "PHST"
#define PHYSTATS_VERSION_NUM          18

#define PHY_STATS_VERSION                        (0x01 << 8)
#define PHY_STATS_VERSION_TIMER                  (((0x0 & 0xFF) | PHY_STATS_VERSION) & 0xFFFF)
#define PHY_STATS_VERSION_RACH                   (((0x1 & 0xFF) | PHY_STATS_VERSION) & 0xFFFF)
#define PHY_STATS_VERSION_USER                   (((0x2 & 0xFF) | PHY_STATS_VERSION) & 0xFFFF)
#define LTE_BS_PHY_STATS_DEFAULT_MAX_TIMER       150000
#define LTE_BS_PHY_STATS_UNIQUE_INDICES          NUM_CONNECTED_USERS
#define LTE_BS_PHY_STATS_UD_TRACE_COUNTER        (NUM_CONNECTED_USERS + 1 + 1)   // User Stats + Timer Stats + Rach Stats

#define LTE_BS_PHY_STATS_MAX_CHANNELS_PER_TTI    16
#define LTE_BS_PHY_STATS_TIME_HISTOGRAM_DURATION 200        // last 200 TTIs

#define PHY_STATS_PERIODITICY_NOT_STATED   20000
#define PHY_STATS_SR_PERIODICITY        0
#define PHY_STATS_DL_CQI_PERIODICITY    1
#define PHY_STATS_DL_RI_PERIODICITY     2

#define PDCCH_DCI0                      20
#define SYSTEM                          30

#define LTE_BS_HY_STATS_MAX_DL_LIST_PROCESSING_TIME     700

#define PHYSTATS_CQI_PMI_PT             1<<0
#define PHYSTATS_HARQ_PT                1<<1
#define PHYSTATS_RI_PT                  1<<2
#define PHYSTATS_SR_PT                  1<<3

#ifdef _WIN32
#define MxDelayTicks
#ifdef _LTE_BS_PHY_STATS_C
#ifdef LTE_BS_PHY_STATS_LIB
#define uart_printf(...) do {                  \
  char string[1024];                           \
  sprintf(string, __VA_ARGS__);                \
  fprintf(pFileOut, "%s", string);             \
} while(0);
#else
#define uart_printf(...) do {                  \
  char string[1024];                           \
  printf (__VA_ARGS__);                        \
  sprintf(string, __VA_ARGS__);                \
  fprintf(pFileOut, "%s", string);             \
} while(0);
#endif
#endif
#endif

////////////////////////////////////////////
// Phy Stat Structures
////////////////////////////////////////////

typedef struct tPhyStatsHeader
{
    U32 Magic;
    U32 Version;
    U32 Size;
    U32 reserved;
} PHY_STATS_HEADER, *PPHY_STATS_HEADER;


#ifndef _WIN32
/////////////////////////////////////////////////////////////////////////////////////
// IP Packet Header (contains UD Header, Api Header and ApiParam Header)
/////////////////////////////////////////////////////////////////////////////////////
typedef struct tIpPacketHeader
{
    EXTRAUDHEADER   udHeader;
    ApiHeader       apiHeader;
    ApiParamHeader  apiParamHeader;
}LTE_BS_PHY_STATS_IP_PACKET_HEADER, *PLTE_BS_PHY_STATS_IP_PACKET_HEADER;


#else  // _WIN32
typedef struct tIpPacketHeader
{
    U32   udHeader[2];
    U32   apiHeader[4];
    U32   apiParamHeader;
}LTE_BS_PHY_STATS_IP_PACKET_HEADER, *PLTE_BS_PHY_STATS_IP_PACKET_HEADER;


typedef struct _REX_SERV_STAT_INFO_
{
    U32      NumListRun;             // The total number of lists started to be processed
    U32      NumBadList;             // The total number of list incorrectly executed due to bad CEVAs
    U32      NumCevaRest;            // The number of CEVAs restored in a process of work
    U32      CevaBadDevMask;         // The number of bad CEVAs that was not restored
}REX_SERV_STAT_INFO;

/************************************************************************/
/*     UINT64, UINT32, UINT16 and UINT8 definition                      */
/************************************************************************/
#ifndef _UINT64_
#define _UINT64_
typedef unsigned __int64 UINT64, *PUINT64;
#endif /*_UINT64_*/

#ifndef _UINT32_
#define _UINT32_
typedef unsigned int UINT32, *PUINT32;
#endif /*_UINT32_*/

#ifndef _UINT16_
#define _UINT16_
typedef unsigned short UINT16, *PUINT16;
#endif /*_UINT16_*/

#ifndef _UINT8_
#define _UINT8_
typedef unsigned char UINT8, *PUINT8;
#endif /*_UINT8_*/

#ifndef _VUINT64_
#define _VUINT64_
#if defined (__CC_ARM)
typedef volatile unsigned __int64 VUINT64, *PVUINT64;
#elif defined (__GNUC__)
typedef volatile long long unsigned int VUINT64, *PVUINT64;
#endif
#endif /*_UINT64_*/

#ifndef _VUINT32_
#define _VUINT32_
typedef volatile unsigned int VUINT32, *PVUINT32;
#endif /*_UINT32_*/

#ifndef _VUINT16_
#define _VUINT16_
typedef volatile unsigned short VUINT16, *PVUINT16;
#endif /*_UINT16_*/

#ifndef _VUINT8_
#define _VUINT8_
typedef volatile unsigned char VUINT8, *PVUINT8;
#endif /*_UINT8_*/

/************************************************************************/
/*     SINT64, SINT32, SINT16 and SINT8 definition                      */
/************************************************************************/
#ifndef _SINT64_
#define _SINT64_
#if defined (__CC_ARM)
typedef __int64 SINT64, *PSINT64;
#elif defined (__GNUC__)
typedef signed long long SINT64, *PSINT64;
#endif
#endif /*_SINT64_*/

#ifndef _SINT32_
#define _SINT32_
typedef int SINT32, *PSINT32;
#endif /*_SINT32_*/

#ifndef _SINT16_
#define _SINT16_
typedef signed short SINT16, *PSINT16;
#endif /*_SINT16_*/

#ifndef _SINT8_
#define _SINT8_
typedef signed char SINT8, *PSINT8;
#endif /*_SINT8_*/

#ifndef _MXRC_
#define _MXRC_
typedef UINT32 MXRC;
typedef VUINT32 VMXRC;
#endif /*_MXRC_*/

#ifndef _LPVOID_
#define _LPVOID_
typedef void *LPVOID;
typedef const void *LPCVOID;
#endif /*_LPVOID_*/

#ifndef _HANDLE_
#define _HANDLE_
typedef void *HANDLE;
#endif /*_HANDLE_*/


#endif // _WIN32

////////////////////////////////////////////
// Timer Stats Structure
////////////////////////////////////////////
typedef struct tLteBsPhyStatsTimerStats
{
    LTE_BS_PHY_STATS_IP_PACKET_HEADER header;

    U32 numSubframes;
    U32 numDlListMissed;
    U32 numUlListMissed;
    U32 numPrachListMissed;
    U32 numSrsListMissed;

    U32 numNmmSubframes;

    U32 SubframeTtiIntervals[5];                // Cnt Max Min Total
    U32 SlotTtiIntervals[5];                    // Cnt Max Min Total
    U32 ValidApiDeliveryIntervals[5];           // Cnt Max Min Total
    U32 DLListProcessingTime[5];                // Cnt Max Min Total
    U32 DLSDUProcessing[5];                     // Cnt Max Min Total
    U32 ULListProcessingTime[5];                // Cnt Max Min Total
    U32 ULSDUPuschProcessing[5];                // Cnt Max Min Total
    U32 ULSDUPucchHarqProcessing[5];            // Cnt Max Min Total
    U32 ULSDUPuschMuxCtrlHarqProcessing[5];     // Cnt Max Min Total
    U32 ULSDUPuschMuxCtrlCqiRiProcessing[5];    // Cnt Max Min Total
    U32 PrachListProcessingTime[5];             // Cnt Max Min Total
    U32 SrsListProcessingTime[5];               // Cnt Max Min Total

    U32 systemThroughput[3][10];
} LTE_BS_PHY_STATS_TIMER_STATS, *PLTE_BS_PHY_STATS_TIMER_STATS;

////////////////////////////////////////////
// Rach Stats Structure
////////////////////////////////////////////
typedef struct tLteBsPhyStatsRachStats
{
    LTE_BS_PHY_STATS_IP_PACKET_HEADER header;

    U32 numPrachDetected;

    S16 rachTimingOffsetMax;
    S16 rachTimingOffsetMin;
    U32 rachTimingOffsetBins[12];

    S16 rachPeakToAvgPwrRatioMax;
    S16 rachPeakToAvgPwrRatioMin;
    U32 rachPeakToAvgPwrRatioBins[12];

    S16 rachTimingOffsetGoodPwrRatioMax;
    S16 rachTimingOffsetGoodPwrRatioMin;
    U32 rachTimingOffsetGoodPwrRatioBins[12];
} LTE_BS_PHY_STATS_RACH_STATS, *PLTE_BS_PHY_STATS_RACH_STATS;

////////////////////////////////////////////
// User Stats Structure
////////////////////////////////////////////
typedef struct tLteBsPhyStatsUserStat
{
    LTE_BS_PHY_STATS_IP_PACKET_HEADER header;

    // User Info
    U16 used;
    U16 RNTI;

    // PDSCH
    U32 numPdschSdu[2];
    U32 numDLHarqTransmissions[2][4];
    U32 DLThroughput[2][8];
    U32 pdschNumCqi[4];
    U32 pdschCqi[4][12];
    U32 pdschCqiMax[4];
    U32 pdschCqiMin[4];
    U32 pdschNumRi[4];
    U32 pdschRi[4][3];
    U32 pdschRiMax[4];
    U32 pdschRiMin[4];
    U32 totalDlData[2];
    U32 numTtiListProcessingHigh;

    // PDCCH
    U32 numPdcchSdu;
    U32 numPdcchFormats[16];

    // PUSCH
    U32 numPuschSdu;
    U32 numPuschErr;
    U32 numULHarqTransmissions[4];
    U32 ULThroughput[8];
    S32 puschAverageCqi[6];
    U32 sinrPuschBins[12];
    U8  sinrPuschMax;
    U8  sinrPuschMin;
    U32 timeAdvPuschBins[12];
    S16 timeAdvPuschMax;
    S16 timeAdvPuschMin;
    U32 totalUlData[2];

    // PUCCH
    U32 numPucchNumSdu[6];
    U32 numPucchSimultSr[6];
    U32 numPucchSimultSrDet[6];
    U32 numPucchFormat1Det;
    U32 numPucchFormat1ANotReceived;
    U32 numPucchFormat1ANacks;
    U32 numPucchFormat1BNotReceived;
    U32 numPucchFormat1BNacksA;
    U32 numPucchFormat1BNacksB;
    U32 numPucchFormat2ANotReceived;
    U32 numPucchFormat2ANacks;
    U32 numPucchFormat2BNotReceived;
    U32 numPucchFormat2BNacksA;
    U32 numPucchFormat2BNacksB;
    U32 sinrPucchBins[6][12];
    U8  sinrPucchMax[6];
    U8  sinrPucchMin[6];
    U32 timeAdvPucchBins[6][12];
    S16 timeAdvPucchMax[6];
    S16 timeAdvPucchMin[6];
    S32 pucchAverageCqi[6];

    // Multiplexed PUSCH
    U32 numMuxPuschAckNack;
    U32 numMuxPuschNack1;
    U32 numMuxPuschNack2;
    U32 numMuxPuschAckNackNotReceived;
    U32 numMuxPuschCqiPusch;
    U32 numMuxPuschCqiNotReceived;
    U32 numMuxPuschRiPusch;
    U32 numMuxPuschRiNotReceived;

    U32 cqiPmiConfMetric[4];
    U32 cqiPmiConfMetricCnt[4];

    // Common
    U32 maxMcs[3];
    U32 minMcs[3];
    U32 mcs[3][15];   //0: DL CW0. 1: DL CW1, 2: UL

    // PUCCH Periodicity
    U32 srPeriodicity[3];       // Max, Min and lastOccured
    U32 dlCqiPeriodicity[3];    // Max, Min and lastOccured
    U32 dlRiPeriodicity[3];     // Max, Min and lastOccured
} LTE_BS_PHY_STATS_USER_STAT, *PLTE_BS_PHY_STATS_USER_STAT;

typedef struct tLteBsPhyStatsUeCtx
{
    U16 RNTI;
    U8 type, numCw;
    U8 rV0, rV1;
    U8 mcs0, mcs1;
    U8 Qm0, Qm1;
    U8 numRb;
    U8 cqiPmiHarqRiSrPt;
}LTE_BS_PHY_STATS_UE_CTX, *PLTE_BS_PHY_STATS_UE_CTX;

typedef struct tLteBsPhyStatsSubframeCtx
{
    U16 dlSfn;
    U16 numDlChs;
    PLTE_BS_PHY_STATS_UE_CTX pDlCurrCtx;
    LTE_BS_PHY_STATS_UE_CTX dlCtx[LTE_BS_PHY_STATS_MAX_CHANNELS_PER_TTI];

    U16 ulSfn;
    U16 numUlChs;
    PLTE_BS_PHY_STATS_UE_CTX pUlCurrCtx;
    LTE_BS_PHY_STATS_UE_CTX ulCtx[LTE_BS_PHY_STATS_MAX_CHANNELS_PER_TTI];
}LTE_BS_PHY_STATS_SUBFRAME_CTX, *PLTE_BS_PHY_STATS_SUBFRAME_CTX;

typedef struct tLteBsPhyStatsTimeHistogram
{
    U32 maxDLChannelsPerSubframe;
    U32 maxPdschChannelsPerSubframe;
    U32 maxPdcchChannelsPerSubframe;

    U32 maxULChannelsPerSubframe;
    U32 maxPuschChannelsPerSubframe;
    U32 maxPucchChannelsPerSubframe;

    U32 dlCtxCircle;
    U32 ulCtxCircle;

    U32 currDlCtx;
    U32 currUlCtx;
    PLTE_BS_PHY_STATS_SUBFRAME_CTX pCurrDlCtxPtr;
    PLTE_BS_PHY_STATS_SUBFRAME_CTX pCurrUlCtxPtr;
    LTE_BS_PHY_STATS_SUBFRAME_CTX subframeCtx[LTE_BS_PHY_STATS_TIME_HISTOGRAM_DURATION];
}LTE_BS_TIME_HISTOGRAM, *PLTE_BS_TIME_HISTOGRAM;


typedef struct tLteBsDeviceStats
{
    LTE_BS_PHY_STATS_IP_PACKET_HEADER header;

    REX_SERV_STAT_INFO rexServStatInfo;

}LTE_BS_DEVICE_STATS, *PLTE_BS_DEVICE_STATS;

typedef struct tLteBsApiStats
{
    // Number of Apis Stats
    U32 numApisArrived;
    U32 numApisFreed;
    U32 numMac2PhyBlocksArrived;
    U32 numMac2PhyBlocksFreed;

    // Late ApiStats
    U32 numMacToPhyMessageCleanSfn;
    U32 numMacToPhyMessageCleanApis;
    U32 numMacToPhyMessageCleanDuplicateSfn;
    U32 numMacToPhyMessageCleanDuplicateApis;

    // Api Error Checking Stats
    U32 numBadApiMessages;
    U32 badApiMask;

    // Dummy Api Insertions
    U32 numDumymApis;
    U32 numConsecutiveDummyApis[11];

    // API Arrival Histogram
    U32 apiArrivalHist[11];
}LTE_BS_API_STATS, *PLTE_BS_API_STATS;


////////////////////////////////////////////
// Big Structure
////////////////////////////////////////////
typedef struct tLteBsPhyStats
{
    PHY_STATS_HEADER                    header;
    LTE_BS_PHY_STATS_TIMER_STATS        timerStats;
    LTE_BS_API_STATS                    apiStats;
    LTE_BS_PHY_STATS_RACH_STATS         rachStats;
    LTE_BS_DEVICE_STATS                 deviceStats;
    LTE_BS_PHY_STATS_USER_STAT          userStats[LTE_BS_PHY_STATS_UNIQUE_INDICES];
    LTE_BS_TIME_HISTOGRAM               timeHistogram;
    U32                                 PhyStatsCfg;
    U32                                 reserved[9];
} LTE_BS_PHY_STATS, *PLTE_BS_PHY_STATS;


////////////////////////////////////////////





typedef struct tLteBsPhyStatsUsedIndex
{
    U8 used;
    U32 RNTI;
} LTE_BS_PHY_STATS_USED_INDEX, *PLTE_BS_PHY_STATS_USED_INDEX;

typedef struct tLteBsPhyStatsRntiMapping
{
    S8 rnti_map[NUM_UNIQUE_RNTI];
    LTE_BS_PHY_STATS_USED_INDEX lteBsPhyStatsUsedIndex[LTE_BS_PHY_STATS_UNIQUE_INDICES];
} LTE_BS_PHY_STATS_RNTI_MAPPING, *PLTE_BS_PHY_STATS_RNTI_MAPPING;

typedef struct tLteBsPhyStatsControl
{
    U32 CtxNum;
    U32 OtherCtxNum;
    U32 TTICntr;
    U32 CurrCntr;
    U32 SendUnifiedDiag;
    U32 SendCounter;
    U32 ClearUnifiedDiag;
    U32 ClearCounter;
    HANDLE  hPhyStatsStreamThread;
    PLTE_BS_PHY_STATS_RNTI_MAPPING pLteBsPhyStatsRntiMapping;
} LTE_BS_PHY_STATS_CONTROL, *PLTE_BS_PHY_STATS_CONTROL;

////////////////////////////////////////////


PLTE_BS_PHY_STATS LteBsPhyStatsGetCtx(void);
U32 LteBsPhyStatsGetSize(void);
U32 LteBsInitPhyStats(void);
U32 LteBsDestroyPhyStats(void);
U32 LteBsResetPhyStats(void);
MXRC LtePhyInitPhyStatsStreamThread(void);
MXRC LtePhyDestroyPhyStatsStreamThread(void);
void LtePhySchedulePhyStatsStreamThread(void);
UINT32 LteBsPhyStreamStats(LPVOID pParam);



#ifdef LTE_BS_PHY_STATS_ENABLED

// PhyStats Enable Mask
#define LteBsPhyStatsSetEnableFlag(enableFlag) (phyStatsEnableFlag = enableFlag)
#define LteBsPhyStatsDisableMask(mask) (phyStatsEnableFlag &= (~(U32)mask))
#define LteBsPhyStatsCheckEnableMask(mask) ((phyStatsEnableFlag & mask)?1:0)

// Timer Stats
#define LteBsPhyStatsNumSubframe(numSubframes)                      {if (LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_TMRSTS)) {numSubframes++;}}
#define LteBsPhyStatsULMissed(numUlListMissed)                      {if (LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_TMRSTS)) {numUlListMissed++;}}
#define LteBsPhyStatsDLMissed(numDlListMissed)                      {if (LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_TMRSTS)) {numDlListMissed++;}}
#define LteBsPhyStatsPrachMissed(numPrachListMissed)                {if (LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_TMRSTS)) {numPrachListMissed++;}}
#define LteBsPhyStatsSrsMissed(numSrsListMissed)                    {if (LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_TMRSTS)) {numSrsListMissed++;}}
#define LteBsPhyStatsNumNmmSubframe(numSubframes)                   {if (LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_TMRSTS)) {numSubframes++;}}
#define LteBsPhyStatsNumPrachDet(numPrachDetected, num)             {if (LteBsPhyStatsCheckEnableMask(PHYSTATS_ENABLE_RACH_STATS)) {numPrachDetected += num;}}

// Api Stats
#define LteBsPhyStatsNumMac2PhyBlocksArrived(pPhyStats, numApi)     (pPhyStats->apiStats.numMac2PhyBlocksArrived += numApi)
#define LteBsPhyStatsNumMac2PhyBlocksFreed(pPhyStats, numApi)       (pPhyStats->apiStats.numMac2PhyBlocksFreed += numApi)
#define LteBsPhyStatsNumApisArrived(pPhyStats, numApi)              (pPhyStats->apiStats.numApisArrived += numApi)
#define LteBsPhyStatsNumApisFreed(pPhyStats, numApi)                (pPhyStats->apiStats.numApisFreed += numApi)

#define LteBsPhyStatsMacToPhyMessageCleanSfn(pPhyStats)             (pPhyStats->apiStats.numMacToPhyMessageCleanSfn++)
#define LteBsPhyStatsMacToPhyMessageCleanSfnApis(pPhyStats, numApi) (pPhyStats->apiStats.numMacToPhyMessageCleanApis += numApi)
#define LteBsPhyStatsMacToPhyDuplicateSfn(pPhyStats)                (pPhyStats->apiStats.numMacToPhyMessageCleanDuplicateSfn++)
#define LteBsPhyStatsMacToPhyDuplicateSfnApis(pPhyStats, numApi)    (pPhyStats->apiStats.numMacToPhyMessageCleanDuplicateApis += numApi)

#define LteBsPhyStatsBadApiMessages(pPhyStats)                      (pPhyStats->apiStats.numBadApiMessages++)
#define LteBsPhyStatsBadApiMask(pPhyStats, mask)                    (pPhyStats->apiStats.badApiMask |= mask)

#define LteBsPhyStatsNumDummyApis(pPhyStats)                        (pPhyStats->apiStats.numDumymApis++)
#define LteBsPhyStatsNumConsecutiveDummyApis(pPhyStats, loc)        (pPhyStats->apiStats.numConsecutiveDummyApis[loc]++)

#define LteBsPhyStatsApiArrivalHistorgram(pPhyStats, loc)           (pPhyStats->apiStats.apiArrivalHist[loc]++)

// PDSCH
#define LteBsPhyStatsUserStatNumPdschSdu(pUserStat, cwId) (pUserStat->numPdschSdu[cwId]++)
#define LteBsPhyStatsUserStatDlHarqTransmissions(pUserStat, cwId, rV) (pUserStat->numDLHarqTransmissions[cwId][rV]++)
#define LteBsPhyStatsUserStatDlNumDlCqi(pUserStat, pucchType) (pUserStat->pdschNumCqi[pucchType]++)
#define LteBsPhyStatsUserStatDlNumDlRi(pUserStat, pucchType) (pUserStat->pdschNumRi[pucchType]++)
#define LteBsPhyStatsUserStatDlNumTtiListProcessingHigh(pUserStat) (pUserStat->numTtiListProcessingHigh ++)

// PDCCH
#define LteBsPhyStatsUserStatNumPdcchSdu(pUserStat) (pUserStat->numPdcchSdu++)
#define LteBsPhyStatsUserStatNumPdcchTypeSdu(pUserStat, type) (pUserStat->numPdcchFormats[type]++)

// PUSCH
#define LteBsPhyStatsUserStatNumPuschSdu(pUserStat) (pUserStat->numPuschSdu++)
#define LteBsPhyStatsUserStatPuschError(pUserStat) (pUserStat->numPuschErr++)
#define LteBsPhyStatsUserStatUlHarqTransmissions(pUserStat, iter) (pUserStat->numULHarqTransmissions[iter]++)
#define LteBsPhyStatsAveragePuschCqi(pUserStat, averageCqi) (pUserStat->puschAverageCqi[pUserStat->puschAverageCqi[0]+1] = (averageCqi - 128)/2)
#define LteBsPhyStatsAveragePuschCqiIncrementCtr(pUserStat) ((pUserStat->puschAverageCqi[0] == 4) ? (pUserStat->puschAverageCqi[0] = 0) : (pUserStat->puschAverageCqi[0]++))


// PUCCH
#define LteBsPhyStatsUserStatNumPucch(pUserStat, pucch_format) (pUserStat->numPucchNumSdu[pucch_format]++)
#define LteBsPhyStatsUserStatSimultaneousSr(pUserStat, pucch_format, simultSr) (pUserStat->numPucchSimultSr[pucch_format] += simultSr)
#define LteBsPhyStatsUserStatSimultaneousSrDet(pUserStat, pucch_format, detected) (pUserStat->numPucchSimultSrDet[pucch_format] += detected)
#define LteBsPhyStatsUserStatPucchFormat1Det(pUserStat) (pUserStat->numPucchFormat1Det++)
#define LteBsPhyStatsUserStatPucchFormat1ANack(pUserStat, nackCnt) (pUserStat->numPucchFormat1ANacks += nackCnt)
#define LteBsPhyStatsUserStatPucchFormat1ADtx(pUserStat) (pUserStat->numPucchFormat1ANotReceived++)
#define LteBsPhyStatsUserStatPucchFormat1BaNack(pUserStat, nackCnt) (pUserStat->numPucchFormat1BNacksA += nackCnt)
#define LteBsPhyStatsUserStatPucchFormat1BbNack(pUserStat, nackCnt) (pUserStat->numPucchFormat1BNacksB += nackCnt)
#define LteBsPhyStatsUserStatPucchFormat1BDtx(pUserStat) (pUserStat->numPucchFormat1BNotReceived++)
#define LteBsPhyStatsUserStatPucchFormat2ANack(pUserStat, nackCnt) (pUserStat->numPucchFormat2ANacks += nackCnt)
#define LteBsPhyStatsUserStatPucchFormat2ADtx(pUserStat) (pUserStat->numPucchFormat2ANotReceived++)
#define LteBsPhyStatsUserStatPucchFormat2BaNack(pUserStat, nackCnt) (pUserStat->numPucchFormat2BNacksA += nackCnt)
#define LteBsPhyStatsUserStatPucchFormat2BbNack(pUserStat, nackCnt) (pUserStat->numPucchFormat2BNacksB += nackCnt)
#define LteBsPhyStatsUserStatPucchFormat2BDtx(pUserStat) (pUserStat->numPucchFormat2BNotReceived++)
#define LteBsPhyStatsAveragePucchCqi(pUserStat, averageCqi) (pUserStat->pucchAverageCqi[pUserStat->pucchAverageCqi[0]+1] = (averageCqi - 128)/2)
#define LteBsPhyStatsAveragePucchCqiIncrementCtr(pUserStat) ((pUserStat->pucchAverageCqi[0] == 4) ? (pUserStat->pucchAverageCqi[0] = 0) : (pUserStat->pucchAverageCqi[0]++))

// PUSCH Mux Control
#define LteBsPhyStatsUserStatNumMuxPuschAckNack(pUserStat) (pUserStat->numMuxPuschAckNack++)
#define LteBsPhyStatsUserStatNumMuxPuschNack1(pUserStat, ackStore) (pUserStat->numMuxPuschNack1 += ackStore)
#define LteBsPhyStatsUserStatNumMuxPuschNack2(pUserStat, ackStore) (pUserStat->numMuxPuschNack2 += ackStore)
#define LteBsPhyStatsUserStatNumMuxPuschAckNackDtx(pUserStat, ackStore) (pUserStat->numMuxPuschAckNackNotReceived += ackStore)
#define LteBsPhyStatsUserStatNumMuxPuschCqi(pUserStat) (pUserStat->numMuxPuschCqiPusch++)
#define LteBsPhyStatsUserStatNumMuxPuschCqiDtx(pUserStat, ackStore) (pUserStat->numMuxPuschCqiNotReceived += ackStore)
#define LteBsPhyStatsUserStatNumMuxPuschRi(pUserStat) (pUserStat->numMuxPuschRiPusch++)
#define LteBsPhyStatsUserStatNumMuxPuschRiDtx(pUserStat, ackStore) (pUserStat->numMuxPuschRiNotReceived += ackStore)

#define LteBsPhyStatsUserStatCqiPmiConfMetric(pUserStat, format, conf) {pUserStat->cqiPmiConfMetric[format] += conf; pUserStat->cqiPmiConfMetricCnt[format]++;}

// Time Histogram
#define LteBsPhyStatsTimeHistogramSfnCtxIncrementDlChs(pSfnCtx, numDci0) (pSfnCtx->numDlChs += numDci0)
#define LteBsPhyStatsTimeHistogramSfnCtxIncrementUlChs(pSfnCtx, numChs) (pSfnCtx->numUlChs += numChs)
#define LteBsPhyStatsTimeHistogramUeCtxRNTI(pUeCurrCtx, localRNTI) (pUeCurrCtx->RNTI = localRNTI)
#define LteBsPhyStatsTimeHistogramUeCtxType(pUeCurrCtx, localtype) (pUeCurrCtx->type = localtype)
#define LteBsPhyStatsTimeHistogramUeCtxNumCw(pUeCurrCtx, localnumCw) (pUeCurrCtx->numCw = localnumCw)
#define LteBsPhyStatsTimeHistogramUeCtxRv0(pUeCurrCtx, localrV0) (pUeCurrCtx->rV0 = localrV0)
#define LteBsPhyStatsTimeHistogramUeCtxRv1(pUeCurrCtx, localrV1) (pUeCurrCtx->rV1 = localrV1)
#define LteBsPhyStatsTimeHistogramUeCtxMcs0(pUeCurrCtx, localmcs0) (pUeCurrCtx->mcs0 = localmcs0)
#define LteBsPhyStatsTimeHistogramUeCtxMcs1(pUeCurrCtx, localmcs1) (pUeCurrCtx->mcs1 = localmcs1)
#define LteBsPhyStatsTimeHistogramUeCtxQm0(pUeCurrCtx, localQm0) (pUeCurrCtx->Qm0 = localQm0)
#define LteBsPhyStatsTimeHistogramUeCtxQm1(pUeCurrCtx, localQm1) (pUeCurrCtx->Qm1 = localQm1)
#define LteBsPhyStatsTimeHistogramUeCtxNumRb(pUeCurrCtx, localnumRb) (pUeCurrCtx->numRb = localnumRb)
#define LteBsPhyStatsTimeHistogramUeCtxResult(pUeCurrCtx, result) (pUeCurrCtx->rV1 = result)
#define LteBsPhyStatsTimeHistogramUeCtxTmgAdv(pUeCurrCtx, tmgAdv) (pUeCurrCtx->mcs1 = tmgAdv)
#define LteBsPhyStatsTimeHistogramUeCtxSinr(pUeCurrCtx, sinr) (pUeCurrCtx->Qm1 = sinr)
#define LteBsPhyStatsTimeHistogramUeCtxNumRach(pUeCurrCtx, numRach) (pUeCurrCtx->numRb = numRach)
#define LteBsPhyStatsTimeHistogramUeCtxCqiPmiHarqRiSrPt(pUeCurrCtx, cqiPmiHarqRiSrPt) (pUeCurrCtx->cqiPmiHarqRiSrPt = cqiPmiHarqRiSrPt)

#define LteBsPhyStatsTimeHistogramDLUeCtxIncrement(pCurrCtxPtr) (pCurrCtxPtr->pDlCurrCtx++)
#define LteBsPhyStatsTimeHistogramULUeCtxIncrement(pCurrCtxPtr) (pCurrCtxPtr->pUlCurrCtx++)

#define LteBsPhyStatsTimerHistogramGetDlSubFrameCtx(pLteBsPhyStats) (pLteBsPhyStats->timeHistogram.pCurrDlCtxPtr)
#define LteBsPhyStatsTimerHistogramGetUlSubFrameCtx(pLteBsPhyStats) (pLteBsPhyStats->timeHistogram.pCurrUlCtxPtr)
#define LteBsPhyStatsTimerHistogramGetDlUeCtx(pCurrCtxPtr) (pCurrCtxPtr->pDlCurrCtx)
#define LteBsPhyStatsTimerHistogramGetUlUeCtx(pCurrCtxPtr) (pCurrCtxPtr->pUlCurrCtx)

PLTE_BS_PHY_STATS_SUBFRAME_CTX LteBsPhyStatsTimeHistogramIncrement(U32 type, U16 numDataChs, U16 numCtrlChs, U16 sfn);

U32 LteBsPhyStatsThroughput(PLTE_BS_PHY_STATS_USER_STAT pUserStat, U32 direction, U32 size, U32 cwId);
U32 LteBsPhyStatsTimer(U32 *storePtr, U32 startTime, U32 stopTime);
U32 LteBsPhyStatsUpdateRexStats(void);
PLTE_BS_PHY_STATS_USER_STAT LteBsPhyStatsGetRntiId(U32 RNTI);
U32 LteBsPhyStatsCalcSinrTmAdv(PLTE_BS_PHY_STATS_USER_STAT pUserStat, U8 sinr, S16 tmAdv, U32 ulType, U32 pucchType);
U32 LteBsPhyStatsPdschCqiRi(PLTE_BS_PHY_STATS_USER_STAT pUserStat, U8 cqiRi, U32 ulType, U32 pucchType, U32 isCqi);
U32 LteBsPhyStatsMcs(PLTE_BS_PHY_STATS_USER_STAT pUserStat, U8 mcs, U32 mcsIndex);
U32 LteBsPhyStatsUlUciPeriodicity(PLTE_BS_PHY_STATS_USER_STAT pUserStat, U32 type, U32 currentTti);
U32 LteBsPhyStatsRachStats(U16 timingOffset, S16 peakToAvgPowerRatio);
MXRC LteBsPhyStatsPrint(PLTE_BS_PHY_STATS pLteBsPhyStatsInput);
void LteBsPhyStatsSetResetCounter(UINT32 counter);
void LteBsPhyStatsIncrementCounter(void);

#else   // LTE_BS_PHY_STATS_ENABLED

// PhyStats Mask
#define LteBsPhyStatsSetEnableFlag(enableFlag)
#define LteBsPhyStatsDisableMask(mask)
#define LteBsPhyStatsCheckEnableMask(mask) (0)

// Timer Stats
#define LteBsPhyStatsNumSubframe(numSubframes)
#define LteBsPhyStatsULMissed(numUlListMissed)
#define LteBsPhyStatsDLMissed(numDlListMissed)
#define LteBsPhyStatsPrachMissed(numPrachListMissed)
#define LteBsPhyStatsSrsMissed(numSrsListMissed)
#define LteBsPhyStatsNumNmmSubframe(numSubframes)
#define LteBsPhyStatsNumPrachDet(numPrachDetected, num)

// Api Stats
#define LteBsPhyStatsNumMac2PhyBlocksArrived(pPhyStats, numApi)
#define LteBsPhyStatsNumMac2PhyBlocksFreed(pPhyStats, numApi)
#define LteBsPhyStatsNumApisArrived(pPhyStats, numApi)
#define LteBsPhyStatsNumApisFreed(pPhyStats, numApi)

#define LteBsPhyStatsMacToPhyMessageCleanSfn(pPhyStats)
#define LteBsPhyStatsMacToPhyMessageCleanSfnApis(pPhyStats, numApi)
#define LteBsPhyStatsMacToPhyDuplicateSfn(pPhyStats)
#define LteBsPhyStatsMacToPhyDuplicateSfnApis(pPhyStats, numApi)

#define LteBsPhyStatsBadApiMessages(pPhyStats)
#define LteBsPhyStatsBadApiMask(pPhyStats, mask)

#define LteBsPhyStatsNumDummyApis(pPhyStats)
#define LteBsPhyStatsNumConsecutiveDummyApis(pPhyStats, loc)

#define LteBsPhyStatsApiArrivalHistorgram(pPhyStats, loc)


// PDSCH
#define LteBsPhyStatsUserStatNumPdschSdu(pUserStat, cwId)
#define LteBsPhyStatsUserStatDlHarqTransmissions(pUserStat, cwId, rV)
#define LteBsPhyStatsUserStatDlNumDlCqi(pUserStat, pucchType)
#define LteBsPhyStatsUserStatDlNumDlRi(pUserStat, pucchType)
#define LteBsPhyStatsUserStatDlNumTtiListProcessingHigh(pUserStat) (pUserStat->numTtiListProcessingHigh++)

// PDCCH
#define LteBsPhyStatsUserStatNumPdcchSdu(pUserStat)
#define LteBsPhyStatsUserStatNumPdcchTypeSdu(pUserStat, type)

// PUSCH
#define LteBsPhyStatsUserStatNumPuschSdu(pUserStat)
#define LteBsPhyStatsUserStatPuschError(pUserStat)
#define LteBsPhyStatsUserStatUlHarqTransmissions(pUserStat, iter)
#define LteBsPhyStatsAveragePuschCqi(pUserStat, averageCqi)
#define LteBsPhyStatsAveragePuschCqiIncrementCtr(pUserStat)

// PUCCH
#define LteBsPhyStatsUserStatNumPucch(pUserStat, pucchFormat)
#define LteBsPhyStatsUserStatSimultaneousSr(pUserStat, pucch_format, simultSr)
#define LteBsPhyStatsUserStatSimultaneousSrDet(pUserStat, pucch_format, detected)
#define LteBsPhyStatsUserStatPucchFormat1Det(pUserStat)
#define LteBsPhyStatsUserStatPucchFormat1ANack(pUserStat, nackCnt)
#define LteBsPhyStatsUserStatPucchFormat1ADtx(pUserStat)
#define LteBsPhyStatsUserStatPucchFormat1BaNack(pUserStat, nackCnt)
#define LteBsPhyStatsUserStatPucchFormat1BbNack(pUserStat, nackCnt)
#define LteBsPhyStatsUserStatPucchFormat1BDtx(pUserStat)
#define LteBsPhyStatsUserStatPucchFormat2ANack(pUserStat, nackCnt)
#define LteBsPhyStatsUserStatPucchFormat2ADtx(pUserStat)
#define LteBsPhyStatsUserStatPucchFormat2BaNack(pUserStat, nackCnt)
#define LteBsPhyStatsUserStatPucchFormat2BbNack(pUserStat, nackCnt)
#define LteBsPhyStatsUserStatPucchFormat2BDtx(pUserStat)
#define LteBsPhyStatsAveragePucchCqi(pUserStat, averageCqi)
#define LteBsPhyStatsAveragePucchCqiIncrementCtr(pUserStat)

// Multiplexed PUSCH
#define LteBsPhyStatsUserStatNumMuxPuschAckNack(pUserStat)
#define LteBsPhyStatsUserStatNumMuxPuschNack1(pUserStat, ackStore)
#define LteBsPhyStatsUserStatNumMuxPuschNack2(pUserStat, ackStore)
#define LteBsPhyStatsUserStatNumMuxPuschAckNackDtx(pUserStat, ackStore)
#define LteBsPhyStatsUserStatNumMuxPuschCqi(pUserStat)
#define LteBsPhyStatsUserStatNumMuxPuschCqiDtx(pUserStat, ackStore)
#define LteBsPhyStatsUserStatNumMuxPuschRi(pUserStat)
#define LteBsPhyStatsUserStatNumMuxPuschRiDtx(pUserStat, ackStore)

#define LteBsPhyStatsUserStatCqiPmiConfMetric(pUserStat, format, conf)

// Time Histogram
#define LteBsPhyStatsTimeHistogramSfnCtxIncrementDlChs(pSfnCtx, numDci0)
#define LteBsPhyStatsTimeHistogramSfnCtxIncrementUlChs(pSfnCtx, numChs)
#define LteBsPhyStatsTimeHistogramUeCtxRNTI(pUeCurrCtx, localRNTI)
#define LteBsPhyStatsTimeHistogramUeCtxType(pUeCurrCtx, localtype)
#define LteBsPhyStatsTimeHistogramUeCtxNumCw(pUeCurrCtx, localnumCw)
#define LteBsPhyStatsTimeHistogramUeCtxRv0(pUeCurrCtx, localrV0)
#define LteBsPhyStatsTimeHistogramUeCtxRv1(pUeCurrCtx, localrV1)
#define LteBsPhyStatsTimeHistogramUeCtxMcs0(pUeCurrCtx, localmcs0)
#define LteBsPhyStatsTimeHistogramUeCtxMcs1(pUeCurrCtx, localmcs1)
#define LteBsPhyStatsTimeHistogramUeCtxQm0(pUeCurrCtx, localQm0)
#define LteBsPhyStatsTimeHistogramUeCtxQm1(pUeCurrCtx, localQm1)
#define LteBsPhyStatsTimeHistogramUeCtxNumRb(pUeCurrCtx, localnumRb)
#define LteBsPhyStatsTimeHistogramUeCtxResult(pUeCurrCtx, result)
#define LteBsPhyStatsTimeHistogramUeCtxTmgAdv(pUeCurrCtx, tmgAdv)
#define LteBsPhyStatsTimeHistogramUeCtxSinr(pUeCurrCtx, sinr)
#define LteBsPhyStatsTimeHistogramUeCtxNumRach(pUeCurrCtx, numRach)
#define LteBsPhyStatsTimeHistogramUeCtxCqiPmiHarqRiSrPt(pUeCurrCtx, cqiPmiHarqRiSrPt)


#define LteBsPhyStatsTimeHistogramDLUeCtxIncrement(pCurrCtxPtr)
#define LteBsPhyStatsTimeHistogramULUeCtxIncrement(pCurrCtxPtr)

#define LteBsPhyStatsTimerHistogramGetDlSubFrameCtx(pLteBsPhyStats) (NULL)
#define LteBsPhyStatsTimerHistogramGetUlSubFrameCtx(pLteBsPhyStats) (NULL)
#define LteBsPhyStatsTimerHistogramGetDlUeCtx(pCurrCtxPtr) (NULL)
#define LteBsPhyStatsTimerHistogramGetUlUeCtx(pCurrCtxPtr) (NULL)

#define LteBsPhyStatsTimeHistogramIncrement(type, numDataChs, numCtrlChs, sfn) (NULL)

#define LteBsPhyStatsThroughput(pUserStat, direction, size, cwId)
#define LteBsPhyStatsTimer(storePtr, startTime, stopTime) (0)
#define LteBsPhyStatsUpdateRexStats()
#define LteBsPhyStatsGetRntiId(RNTI) (NULL)
#define LteBsPhyStatsCalcSinrTmAdv(pUserStat, sinr, tmAdv, ulType, pucchType)
#define LteBsPhyStatsRachStats(timingOffset, peakToAvgPowerRatio)
#define LteBsPhyStatsMcs(pUserStat, mcs, mcsIndex)
#define LteBsPhyStatsUlUciPeriodicity(pUserStat, type, currentTti)
#define LteBsPhyStatsPrint(pLteBsPhyStatsInput)
#define LteBsPhyStatsSetResetCounter(a)
#define LteBsPhyStatsIncrementCounter()
#define LteBsPhyStatsPdschCqiRi(pUserStat, cqiRi, ulType, pucchType, isCqi)

#endif  // LTE_BS_PHY_STATS_ENABLED

#endif  // LTEBSPHYSTATS_H

