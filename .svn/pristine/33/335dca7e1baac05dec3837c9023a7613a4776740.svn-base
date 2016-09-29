//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL phy_constants.c
 *
 * @brief LTE PHY Global Constants
 * @author Mindspeed Technologies
 * @version $Revision: 1.105 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "typedef.h"
#include "ltephy.h"

#if defined (RT_API_PROC)
#include "../Arm/Prj/LtePhyRt/LteBsTxDataSdu.h"
#include "../Arm/Prj/LtePhyRt/LteBsRxDataSdu.h"
#include "../../../T4KDev/SYSCORE/SERVICES/dataint.h"
#endif

// Coredump
U32 glbObjStart[512] = {0};
U32 glbObjSize[512] = {0};
U32 glbObjNum[512] = {0};
U32 glbNumObj = 0;
U32 glbTotalSize = 0;

SCHEDSTRC gInitScheduler;
LTEPHYDESC Inst0[MAX_NUM_OF_SF_CTX];
BS bs[MAX_NUM_OF_SF_CTX];
BS_TX bsTx[MAX_NUM_OF_SF_CTX];
BS_RX bsRx[MAX_NUM_OF_SF_CTX];
BS_SRS bsSrs[MAX_NUM_OF_SF_CTX];
BS_RX_PRACH bsRxPrach[2*MAX_NUM_OF_SF_CTX];

/* Not sure if it's needed ? */
BS_DL_INIT_MEMBUF glb_bsDlInitMemBuf[MAX_NUM_OF_SF_CTX];
BS_DL_INIT_PARAM  glb_bsDlInitParam[MAX_NUM_OF_SF_CTX];

#ifdef _WIN32
BS_RX_PRACHDet bsRxPrachDet[2*MAX_NUM_OF_SF_CTX];
#else
BS_RX_PRACHDet NCNBVAR bsRxPrachDet[2*MAX_NUM_OF_SF_CTX];
#endif

// move to separate module
static PLTEPHYDESC PhyInst[MAXNUMPHYINSTANCES][MAX_NUM_OF_SF_CTX];
static U8 PingPongPhyCtxSwitcher = 0;

S16 **RBAssignment_ul;
S16 ROW_RBAssignment_ul;
S16 COL_RBAssignment_ul;
S16 **rgQm_ul;
S16 numrows_rgQm_ul, numcols_rgQm_ul;

DATA_ALIGN(32) U8 macSdu[5000];

S16 *CA_srsBandwidthConfigTable_cram;

#if defined (RT_API_PROC)
volatile U32 __align(32) NCNBVAR _my_sfn = 0;
volatile U32 __align(32) NCNBVAR gPhyRobustControl = 0;
volatile U32 __align(32) NCNBVAR gPhyInternalBch = 0;
volatile U32 __align(32) NCNBVAR gUseTxSduPointer = 0;
volatile U32 __align(32) NCNBVAR gTwoRxMuxControlLists = 0;
volatile U32 __align(32) NCNBVAR gRxSduCtx = 0;
volatile U32 __align(32) NCNBVAR gSendSecondTTIIndication = 0;
volatile U32 __align(32) NCNBVAR gMuteRxSduForSrRach = 0;
volatile U32 __align(32) NCNBVAR gLowLatencyFlag = 0;
volatile PLTE_PHY_UL_SDU_DESC __align(32) pRxSduDesc[NUM_UL_SDU_CONTEXT][MAXCHSUBFRAME] = {NULL};
volatile LTE_PHY_UL_PUSCH_SDU_DESC __align(32)  SduPuschDesc[NUM_UL_SDU_CONTEXT];
volatile LTE_PHY_UL_SHARED_PARAMS __align(32) CRAMVAR UlSharedParams[NUM_UL_SDU_CONTEXT];
volatile PMAC2PHY_QUEUE_EL __align(32) pRxSduRxEnd[NUM_UL_SDU_CONTEXT] = {NULL};
volatile PLTE_PHY_UL_SDU_DESC __align(32) pRxSduFirstPusch[NUM_UL_SDU_CONTEXT] = {NULL};
volatile PLTE_PHY_UL_SDU_DESC __align(32) pRxSduFirstPucch[NUM_UL_SDU_CONTEXT] = {NULL};
volatile PLTE_PHY_UL_SDU_DESC __align(32) pRxSduFirstMuxCtrlAckNack[NUM_UL_SDU_CONTEXT] = {NULL};
volatile PLTE_PHY_UL_SDU_DESC __align(32) pRxSduFirstMuxCtrlCqiRi[NUM_UL_SDU_CONTEXT] = {NULL};
volatile U32 __align(32) *pRxRipMeas[NUM_UL_SDU_CONTEXT] = {NULL};
volatile U32 __align(32) *pRxThermalMeas[NUM_UL_SDU_CONTEXT] = {NULL};
volatile U32 __align(32) gMissedApiTable[P_HARQ_PROC] = {0};
volatile U32 __align(32) gRxSduListState[NUM_UL_SDU_CONTEXT];
volatile U32 __align(32) RxControlSduSyncCtrl[NUM_UL_SDU_CONTEXT] = {0, 0, 0, 0};
volatile U32 __align(32) stopPhy = 0;
volatile U32 __align(32) gStartSubfrTimeMark = 0;
volatile U32 gPhyLogsEnableMask = (LTE_BS_PHY_ENABLE_MLOG | LTE_BS_PHY_ENABLE_APIERRORCHECK);
volatile U32 gDefaultApiLoggingMask = (1<<PHY_RXSTART_REQ | 1<<PHY_RXSDU_IND | 1<<PHY_RXEND_IND| 1<<PHY_RXSTATUS_IND | \
                                                   1<<PHY_TXSTART_REQ | 1<<PHY_TXSDU_REQ | 1<<PHY_TXHIADCIUL_REQ | \
                                                   1<<PHY_TXHISDU_REQ | 1<<PHY_TXDCIULSDU_REQ | 1<<PHY_ERROR_IND);
volatile U32 gDefaultApiLoggingResetCounter = 200;      // TTIs
volatile U32 gDefaultApiLoggingTxSduMaxSize = 10;       // bytes
volatile U32 gDefaultApiLoggingRxSduMaxSize = 10;       // bytes
volatile U32 gDefaultApiLoggingCore = 0;                // CoreID
volatile U32 gDefaultIqEnableMask = (DI_LOG_TX0_LOG_ENABLED | DI_LOG_TX1_LOG_ENABLED | DI_LOG_RX0_LOG_ENABLED | DI_LOG_RX1_LOG_ENABLED | DI_LOG_TX_LOG_RR | DI_LOG_RX_LOG_RR);
volatile U32 gDefaultPhyStatsEnableMask = 0xFFFFFFFF;
volatile INITPARM gStoredInitParm = {0};
volatile U32 gMode = INIT_MODE;
volatile U32 gRunFftSlot0 = 0, gNumSlotIrq = 0;
U32 CRAMVAR gTTIPauseResumeCount = 0;
U32 CRAMVAR gLastTxSfn = 0, gLastRxSfn = 0;
U32 CRAMVAR gFirstMessageArrived = 0;
U32 CRAMVAR gCtxDlFec = 0;
U8 CRAMVAR *gScrInBufs[2][2];
U8 CRAMVAR *gCRC24A_buf[2];
U8 CRAMVAR *gScrOutBufs[2][2];
U32 CRAMVAR *gScrIndex[2][2];
U32 CRAMVAR g_totalsizescr[2][2];
U32 CRAMVAR g_scrsize[2][2];
PLTE_PHY_SDU_DESC CRAMVAR gpSduDesc[2];
volatile U32 NCNBVAR __align(32) rxVectorProcDone;

//Used when Ceva DSP functions are moved to ARM for debugging
U8 NCNBVAR __align(32) CevaDataSectionBase[0x15000];
U8 NCNBVAR __align(32) *DataBuff_Bank1, *DataBuff_Bank2, *DataBuff_Bank3;
U8 NCNBVAR __align(32) *DataBuff_Bank1a, *DataBuff_Bank3a;
U8 NCNBVAR __align(32) *DataBuff_Bank1_Off, *DataBuff_Bank2_Off, *DataBuff_Bank3_Off;

#else   //RT_API_PROC

U32 gRxSduCtx                = 0;
U32 gPhyRobustControl        = 0;
U32 gPhyInternalBch          = 0;
U32 gUseTxSduPointer         = 0;
U32 gTwoRxMuxControlLists    = 0;
U32 gSendSecondTTIIndication = 0;
U32 gMuteRxSduForSrRach      = 0;
U32 gLowLatencyFlag          = 0;
U32 gMode = INIT_MODE;
U32 gCtxDlFec = 0;
U32 gMissedApiTable[P_HARQ_PROC] = {0};
U8 *gScrInBufs[2][2];
U8 *gCRC24A_buf[2];
U8 *gScrOutBufs[2][2];
U32 *gScrIndex[2][2];
U32 g_totalsizescr[2][2];
U32 g_scrsize[2][2];
U32 gLastTxSfn = 0, gLastRxSfn = 0;

//Used when Ceva DSP functions are moved to run in C-code for debugging
U8  CevaDataSectionBase[0x15000];
U8 *DataBuff_Bank1, *DataBuff_Bank2, *DataBuff_Bank3;
U8 *DataBuff_Bank1a, *DataBuff_Bank3a;
U8 *DataBuff_Bank1_Off, *DataBuff_Bank2_Off, *DataBuff_Bank3_Off;

#endif


void addObj(U8* start, U32 size, U32 num)
{
//    MxDelayTicks(15000000);
//uart_printf("Obj %d at 0x%x, size %d, Total %d\n", glbNumObj, start, size, glbTotalSize);

    glbObjStart[glbNumObj] = (U32)start;
    glbObjSize[glbNumObj] = size;
    glbObjNum[glbNumObj++] = num;
    glbTotalSize += size;

//    MxDelayTicks(15000000);

    return;
}


//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   phyInstance DOXYGEN_TO_DO
 *  @param   sfCxtNum DOXYGEN_TO_DO
 *  @param   pPhy DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |phyInstance   |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |sfCxtNum      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |pPhy          |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void setPhyCtx(U32 phyInstance, U32 sfCxtNum, PLTEPHYDESC pPhy)
{
    PhyInst[phyInstance][sfCxtNum] = pPhy;
    return;
}

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   phyInstance DOXYGEN_TO_DO
 *  @param   sfCxtNum DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |phyInstance   |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  |sfCxtNum      |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
PLTEPHYDESC getPhyCtxByNum(U32 phyInstance, U32 sfCxtNum)
{
    return PhyInst[phyInstance][sfCxtNum];
}

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   phyInstance DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |phyInstance   |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
PLTEPHYDESC getPhyCtxApiProc(U32 phyInstance)
{
    return PhyInst[phyInstance][PingPongPhyCtxSwitcher & 0x1];
}

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   phyInstance DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |phyInstance   |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
PLTEPHYDESC getPhyCtxListProc(U32 phyInstance)
{
    return PhyInst[phyInstance][(~PingPongPhyCtxSwitcher) & 0x1];
}


//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   phyInstance DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |phyInstance   |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
void swapPhyCtx(U32 phyInstance)
{
    PingPongPhyCtxSwitcher ^= 0x1;
    //printf("%s api %d list %d\n", __FUNCTION__, PingPongPhyCtxSwitcher & 0x1, (~PingPongPhyCtxSwitcher) & 0x1);
    return;
}

//-------------------------------------------------------------------------------------------
/** @brief DOXYGEN_TO_DO
 *
 *  <1> Hardware Resource:
 *  DOXYGEN_TO_DO
 *
 *
 *  @param   phyInstance DOXYGEN_TO_DO
 *
 *  @return  DOXYGEN_TO_DO DOXYGEN_TO_DO
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START DOXYGEN_TO_DO
 *  ----------------------------------------------------------------------------------------------------------
 *  |Name          |TCB Type       |Size          |Data Format   |Valid Range   |Default       |Units        |
 *  ----------------------------------------------------------------------------------------------------------
 *  |phyInstance   |              |              |              |              |              |              |
 *  ----------------------------------------------------------------------------------------------------------
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  --------------------------------------
 *  |Block Name          |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Input Stream        |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Output Stream       |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Config Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  |Status Parameters   |DOXYGEN_TO_DO  |
 *  --------------------------------------
 *  BLOCK_END
 *
 *
 *  <4> Detailed Description:
 *  DOXYGEN_TO_DO
 *
 *
 *  <5> Cycle Count Formula:
 *  DOXYGEN_TO_DO
 *
 *
 *  <6> References:
 *  DOXYGEN_TO_DO
 *
 *
 *  \ingroup group_lte_no_group
 *
 **/
//-------------------------------------------------------------------------------------------
U8 getPhyCtxListIndx(U32 phyInstance)
{
    return (~PingPongPhyCtxSwitcher) & 0x1;
}


