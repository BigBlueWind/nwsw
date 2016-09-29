//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL phydi.h
 *
 * @brief PHY data interface
 *       CPRI/PCIe/sRIO
 * @author Mindspeed Technologies
 * @version $Revision: 1.24 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef _PHY_DI_H_
#define _PHY_DI_H_

#include "dataint.h"
#include "ltephydef.h"
#include "ltephy.h"


#define MAX_MDMA_COPY_SIZE_BYTES 64000       // in bytes (64K max copy size per DMA call)

#define RT_PAUSE_MODE_IDLE          0
#define RT_PAUSE_MODE_ENABLED       1
#define RT_PAUSE_MODE_SKIP          2

#define PHY_STATE_INIT              (1<<0)
#define PHY_STATE_START             (1<<1)
#define PHY_STATE_STOP              (1<<2)
#define PHY_STATE_PAUSE             (1<<3)
#define PHY_STATE_SHUTDOWN          (1<<4)
#define PHY_STATE_SEND_SHUTDOWN     (1<<5)
#define PHY_STATE_SEND_STOP         (1<<6)




#define OPTICAL_LOOPBACK                1
#define RADIOCOMP                       2
#define OPTICAL_LOOPBACK_RADIOCOMP      3
#define CPRI_20MHZ_1AC                  4
#define CPRI_10MHZ_MIMO_2AC             5   // MIMO is configured like 2AC (2x2)


typedef enum _EXEC_STATE
{
    EXEC_LIST_IDLE, EXEC_LIST_PROCESSING, EXEC_LIST_DONE,
} EXE_LIST_STATE;

typedef enum _THREAD_STATE
{
   IDLE, PROCESSING, DONE,
} THREAD_STATE;



/** @brief This context is used to configure PHY code to make pause
           after processing of some number of SFs */

typedef struct _RT_PAUSE_CTX_
{
    UINT32      Mode;               // The mode of finite-state machine, see RT_PAUSE_MODE_XXX

    UINT32      PauseTTIThr;        // The TTI value used to make pause , 0 - pause is disabled
    UINT32      PauseTTIVal;        // The TTI counter used with PauseTTIThr to make pause, if PauseTTIThr == 0 - pause is disabled

    UINT32      SkipTTIThr;         // The numbre of TTI that has to be skipped before system resume,
                                    // it's used to start the system in stable mode
    UINT32      SkipTTIVal;         // The TTI counter used with SkipTTIThr to get condition

    UINT32      DstInstID;

}TRTPauseCtx, *PTRTPauseCtx;



typedef struct tPhyDiCtx
{
    PUINT8 dlIqBuf;
    UINT32 dlIqSizePerSfn;
    UINT32 dlIqBufSize;
    UINT32 dlIqCopyTime;
    UINT32 dlIqJumpSize;

    PUINT8 ulIqBuf;
    UINT32 ulIqSizePerSfn;
    UINT32 ulIqBufSize;
    UINT32 ulIqCopyTime;

    UINT32 phyStartMode;
    UINT32 phyStartTTI;
    UINT32 phyStartSfn;

    UINT32 NtxAntenna;
    UINT32 NrxAntenna;

    UINT32 PhyState;
    UINT32 PhyStartMode;
    UINT32 PhyStartPeriod;
    SINT32 PhyStartCount;       // Number of SFNs to run. if -1, keep running
    UINT32 PhyStartCurrCount;

    TRTPauseCtx rt_pause_ctx;

    UINT32 PhyPauseResume;
    UINT32 PhyPauseResumeSkipped;

    UINT32 PhyQueryTTIPeriod;
}PHYDICTX, *PPHYDICTX;


void PHYDiRxHandler(LPVOID pRxPtr, UINT32 nChID, PUINT8 pRxData, UINT32 nSize);
void PHYDiTxHandler(LPVOID pTxPtr, UINT32 nChID, PUINT8 pTxData, UINT32 nSize);

MXRC PhyDiInit(PLTEPHYDESC pPhy);
MXRC PhyDiDestroy(void);
U32 PhyDiStart(U32 phyInstance, PSTARTREQ pStartReq, U32 logIqEnable);
MXRC PhyDiActivate(UINT32 nMode, UINT32 nParam1, UINT32 nParam2);
UINT32 PhyDiPauseSetRunCtrl (UINT32 nTTIThr, UINT32 nSkipThr, UINT32 nDstInstID);
UINT32 PhyDiResume (void);
UINT32 PhyDiPauseGenInd (void);
UINT32 PhyDiPauseProc (void);
MXRC PhyDiSetStop(void);
MXRC PhyDiStop(LPVOID pCtx);
MXRC PhyDiStopNmm(LPVOID pCtx);
UINT32 PhyDiQueryMode(void);
MXRC PhyDiSetPauseResume(UINT32 PhyPauseResume, UINT32 PhyPauseResumeSkipped);
MXRC PhyDiSetShutdown(void);
MXRC PhyDiShutdown(LPVOID pCtx);
MXRC PhyDiStopHandler(char* fileName, UINT32 lineNum, char *stopString);
UINT32 PhyDiGetCurrentTTI(void);
UINT32 PhyDiIsStopPrintf(void);

PSINT16 PhyDiGetTxBufPos(UINT32 phyInstance, UINT8 antenna);
UINT32 PhyDiGetTxBufSize(UINT32 phyInstance, UINT8 antenna);
PSINT16 PhyDiGetRxBufPos(UINT32 phyInstance, UINT8 antenna);
UINT32 PhyDiGetRxBufSize(UINT32 phyInstance, UINT8 antenna);
MXRC PhyDiCopyIq(PLTEPHYDESC pPhy, UINT32 direction);
UINT32 PhyDiQueryState(void);
void PhyDiCopyIqUMTSNMM(PUINT8 sigIn);

#endif //_PHY_DI_H_

