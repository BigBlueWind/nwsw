//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Phydatacomm.h
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
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

#include "4gmx.h"
#include "apimgr.h"

#include "timerdrv.h"
#include "gemdrv.h"
#include "heaps.h"

#define PAR_COMMIQ_PARAM_GROUP     20
#define PAR_COMMIQ_MODE     10
#define PAR_COMMIQ_L1HDR   11
#define PAR_COMMIQ_L2MODE   12
#define PAR_COMMIQ_L2HDR   13
#define PAR_COMMIQ_DATACHUNKSIZE   14
#define PAR_COMMIQ_NUMANT 15
#define PAR_COMMIQ_CHANBW 16

#define GEN_TIMEOUT 200000000
#define RETX_QUEUE_SIZE 1
#define RETX_TIMEOUT 20000000
#define RETX_ATTEMPTS 3

#define ETH_TXBUF_HEAP NcnbHeap
#define ETH_DATA_CHUNK (512 + 28*16)
#define ETH_PROTOID_T4K 0x9C88

#define CPRI_DATA_CHUNK (16 * 1024)

typedef enum _PHYCOMMIF_
{
    PHYCOMMIF_ETH,
    PHYCOMMIF_CPRI,
    PHYCOMMIF_MAX
}PHYCOMMIF;

typedef enum _L2HDRMODE_
{
    L2HDRMODE_NO,
    L2HDRMODE_MSPD,
    L2HDRMODE_MSPD_SN,
    L2HDRMODE_MSPD_SN_ACK,
    L2HDRMODE_MAX
}L2HDRMODE;

typedef struct _RETX_CTX_
{
    UINT8       Idx;
    UINT8       bAcked;
    UINT8      ReTxTid;
    UINT8       ReTxNumAttempts;
    UINT32      ReTxOffset;
    UINT32      ReTxDataSize;
    HANDLE     ReTxTimer;
    HANDLE     ReTxTimerEvent;
    HANDLE     ReTxTimeoutThr;
    UINT32      ReTxTimeout;
    LPVOID      RefCtx;
}ReTxCtx, *PReTxCtx;

typedef struct _ASYNCPROC_CTX_
{
    HANDLE      AEvent;     // The event for AThrTimeout
    HANDLE      ATimer;     // The timer for AThrTimeout object
    HANDLE      AThrComplete;   // The thread processes compliteness
    HANDLE      AThrTimeout;    // This thread monitors timeout
    HANDLE      AThrMainTx;    // This is main thread
    HANDLE      AThrMainRx;    // This is main thread, e.g. used for Acks

    UINT32      ATimeout;

    UINT16      IndMsgID;
    UINT16      DstInstID;     // The instance (for the indication)
    UINT16      SrcInstID;
    UINT16      Mode;

    UINT32      DataChunkSize;
    UINT32      Offset;
    UINT32      DataSize;
    UINT32      ActualDataSize; // what part of total array to be transferred
    UINT32      ChunkCounter;
    UINT8        Tid;
    UINT8        RxCbCreated;

    UINT8        ReTxNum;
    UINT8        ReTxNumCurr;
    UINT32      ReTxTimeout;
    PReTxCtx     pReTxCtxArr[RETX_QUEUE_SIZE];
}AsyncProcCtx, *PAsyncProcCtx;

typedef struct _LTE_COMMSMP_CTX_
{
    AsyncProcCtx AProcCtx;
    UINT8   PhyComm;
    UINT8   L1HdrSize;
    UINT8   L2HdrSize;
    UINT8   TotalHdrSize;

    UINT16  L2HdrMode; // 1 - MSPD header, 2 - MSPD header with TID, 3 - MSPD header with TID with ACKs
    UINT8   NumAntennas; // 1, 2, 3, 4
    UINT8   ChanBW;    //  0 - 1.4 Mhz,  1 - 3.0 Mhz,  2 - 5.0 Mhz, 3 - 10.0 Mhz, 4 - 15.0 Mhz, 5 - 20.0 Mhz

    PUINT8 pTotalHdr;
    UINT8   L1Hdr[16];  // Eth
    UINT8   L2Hdr[32];  // like MSPD
    UINT8   TotalHdr[48 + 32];
}LteCommSmpCtx, *PLteCommSmpCtx;

extern UINT16 PhyCommIFTx;
extern UINT16 PhyCommIFRx;
extern LteCommSmpCtx gLteSendOutSmp[];
extern LteCommSmpCtx gLteRcvInSmp[];

MXRC LteSendOutSmp(unsigned int mode);
MXRC LteRcvInSmp(unsigned int mode);
MXRC PhyDataCommApiProc(ApiHeader * pApi, ApiHeader * pResp);

#ifdef __cplusplus
}
#endif

