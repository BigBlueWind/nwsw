//-------------------------------------------------------------------------------------------
/** @file fpdrv.h
 *
 * @brief MAP (Mindspeed Advanced Processor) HAL functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.50 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _FP_DRV_H_
#define _FP_DRV_H_

#include "4gmx.h"
#include "hal.h"
#include "tcb.h"
#include "appids.h"
#include "fp_idft_data.h"
#include "mdmadrv.h"
#include "fptypes.h"

/******************************************************************************
*                The module error codes                                       *
*******************************************************************************/
#define FP_DRV_RC_NO_HW_FOUND           DEF_USRRC(APP_MODULE, FPDIAG, 1)
#define FP_DRV_RC_INIT_ERROR            DEF_USRRC(APP_MODULE, FPDIAG, 2)
#define FP_DRV_RC_ALLOC_ERROR           DEF_USRRC(APP_MODULE, FPDIAG, 3)
#define FP_DRV_RC_UNKNOWN_API           DEF_USRRC(APP_MODULE, FPDIAG, 4)
#define FP_DRV_RC_UNSUP_TYPE            DEF_USRRC(APP_MODULE, FPDIAG, 5)
#define FP_DRV_RC_RUN_TIMEOUT           DEF_USRRC(APP_MODULE, FPDIAG, 6)
#define FP_DRV_RC_PARAM_ERROR           DEF_USRRC(APP_MODULE, FPDIAG, 7)
#define FP_DRV_RC_INVALID_FP_MODE       DEF_USRRC(APP_MODULE, FPDIAG, 8)
#define FP_DRV_RC_CPU_INTERSECTION      DEF_USRRC(APP_MODULE, FPDIAG, 9)
#define FP_DRV_RC_RUN_ERROR             DEF_USRRC(APP_MODULE, FPDIAG, 10)
#define FP_DRV_RC_IDFT_INOUTERR         DEF_USRRC(APP_MODULE, FPDIAG, 11)
#define FP_DRV_RC_IDFT_LOAD_ERROR       DEF_USRRC(APP_MODULE, FPDIAG, 12)

#define FPDRV_NUM_FP                     10
#define FPDRV_NUM_FP_IN_GROUP            (FPDRV_NUM_FP/2)


#define FFT_DRV_QSIZE                     56
#define IFFT_DRV_QSIZE                    56
#define IDFT_DRV_QSIZE                    12

#define TASK_FFT                          10
#define TASK_IFFT                         11

#define FFT_MASK                          0x3FFF
#define IFFT_MASK                         0xFFFC000

/******************************************************************************
*                The FP objects addresses                                    *
*******************************************************************************/

#define FP_BASE_ADDR                    0xC0000000
#define FP_RESET_REG                    0xC0001000
#define FP_CTRL_REGS_BASE               0xC0010000
#define FP_APB_CTRL_REG_OFFSET          0x00000000
#define FP_APB_IADDR_REG_OFFSET         0x00000004
#define FP_APB_IDATA0_REG_OFFSET        0x00000100
#define FP_APB_IDATA1_REG_OFFSET        0x00000104
#define FP_APB_IDATA2_REG_OFFSET        0x00000108
#define FP_APB_IDATA3_REG_OFFSET        0x0000010c
#define FP_APB_IDATA4_REG_OFFSET        0x00000110

#define FP_CTRL_PMEM_ACCESS_ENABLE      (1<<0)
#define FP_CTRL_RESET                   (1<<1)
#define FP_CTRL_RESET_IO                (1<<2)
#define FP_CTRL_DEBUG_MODE              (1<<3)
#define FP_CTRL_MEM_ACCESS_ENABLE       (1<<4)
#define FP_CTRL_FORCE_PMEM_CE_ASSERT    (1<<5)
#define FP_CTRL_FORCE_MSEG_CE_ASSERT    (1<<6)

#define FP_DMA_REG_OFFSET(GroupID)      (0x0000E000+GroupID*0x800)
#define FP_MASTER_CTRL_REG_OFFSET        0x0000F000
#define FP_WRITE_BCST_REG_OFFSET        0x0000F800

#define FP_SEGMENT_SIZE                 2048
#define FP_PRAM_SIZE                    (10*1024)

#define FP_RUN_TIMEOUT                  150000 // 1ms @ 150 MHz XP clock freq

#define SEGMENT_MODE                    0x00
#define BANK_MODE                       0x10

#define FPDRV_FDESC_POOL_SIZE           (2*FPDRV_NUM_FP)

#define FPMODE_FFT0128                  7
#define FPMODE_FFT0256                  8
#define FPMODE_FFT0512                  9
#define FPMODE_FFT1024                  10
#define FPMODE_FFT2048                  11
#define FPMODE_FFT4096                  12

#define NUM_MAP_IN_GROUP                5

#define FFT_DRV_QSIZE_LONG              5   // x2
#define FFT_DRV_QSIZE_SHORT             4

#define FP_DRV_MODE_NONE                0x00
#define FP_DRV_MODE_POOL                0x10
#define FP_DRV_MODE_FEXP                0x20
#define FP_DRV_MODE_FEXP_POOL           0x40

#define FP_DRV_LOAD_TYPE0               2
#define FP_DRV_LOAD_TYPE1               3


#define SWAP_FFT                        (1<<1)
#define SWAP_IFFT                       (1<<2)

#define FP_LOG_FFT                      0
#define FP_LOG_IFFT                     1
#define FP_LOG_IDFT                     2

#define NUM_SYMBOLS                     14
#define NUM_MAP_SLOTS                   2

#define FFT_SLOT0                       0
#define FFT_SLOT1                       1
#define IFFT_SLOT0                      2
#define IFFT_SLOT1                      3
#define NOT_RUNNING                     5

#define FP_DRV_MAX_NUM_USERS_PER_TTI    10

#define FP_DRV_GET_MASK(start, stop) (  ((1 << (stop - start + 1)) - 1) << start    )

#define FP_DRV_CFG_IDFT_ON_CEVA         (1<<0)

/******************************************************************************
*                The module types definition                                  *
*******************************************************************************/

typedef struct tMAP_MASTER_CTRL_REGS
{
    VUINT32  Ctrl1;
    VUINT32  Ctrl2;
    VUINT32  Ctrl3;
    VUINT32  BCastMask;
    VUINT32  IrqStatAck;
    VUINT32  IrqEna;
    VUINT32  Grp0OutTrLen;
    VUINT32  Grp1OutTrLen;
    VUINT64  Grp0OutLoad0Ctrl;
    VUINT64  Grp0OutLoad1Ctrl;
    VUINT64  Grp1OutLoad0Ctrl;
    VUINT64  Grp1OutLoad1Ctrl;
    VUINT32  InterFpAddrPhase;
} MAP_MASTER_CTRL_REGS, *PMAP_MASTER_CTRL_REGS;

typedef void (*MAPDONECB)(void *);

typedef struct tMAP_APB_REGS
{
    VUINT32  Ctrl;           // 0x00 offset
    VUINT32  IAddr;          // 0x04 offset
    VUINT32  reserved_0;     // 0x08 offset
    VUINT32  reserved_1;     // 0x0C offset
    VUINT32  TReqEna;        // 0x10 offset
    VUINT32  FPCom;          // 0x14 offset
    VUINT32  FPStat;         // 0x18 offset
    VUINT32  reserved_2;     // 0x1C offset
    VUINT32  DbgRun;         // 0x20 offset
    VUINT32  DbgPcTrig;      // 0x24 offset
    VUINT32  DbgCntTrig;     // 0x28 offset
    VUINT32  reserved_3;     // 0x2C offset
    VUINT32  Min_31_0;       // 0x30 offset
    VUINT32  Min_63_32;      // 0x34 offset
    VUINT32  Mout_31_0;      // 0x38 offset
    VUINT32  Mout_63_32;     // 0x3C offset
    VUINT32  reserved_4[48]; // 0x40-0xFC offsets
    VUINT32  MAcc_31_0;      // 0x100 offset
    VUINT32  MAcc_63_32;     // 0x104 offset
    VUINT32  MAcc_95_64;     // 0x108 offset
    VUINT32  MAcc_127_96;    // 0x10C offset
    VUINT32  MAcc_159_128;   // 0x110 offset
} MAP_APB_REGS, *PMAP_APB_REGS;

typedef struct tMAP_DMA_CTRL_REGS
{
    VUINT32     Control;
    VUINT32     HeadFDesc;
    VUINT32     BurstLength;
    VUINT32     DmaIrqEnable;
    VUINT32     IrqStatus;
    VUINT32     IrqClear;
    VUINT32     Reserved[2];
    VUINT32     SoftReset;
    VUINT32     DiagRegs[23];
} MAP_DMA_CTRL_REGS, *PMAP_DMA_CTRL_REGS;

typedef struct tMAP_DMA_REGS
{
    MAP_DMA_CTRL_REGS   DMAInCtrl;
    MAP_DMA_CTRL_REGS   DMAOutCtrl;
} MAP_DMA_REGS, *PMAP_DMA_REGS;

#ifndef _MAP_DMA_TR_CTRL_
#define _MAP_DMA_TR_CTRL_
typedef struct tMAP_DMA_TR_CTRL
{
    VUINT64  FpId:5;
    VUINT64  LoadType:2;
    VUINT64  LoadCont:1;
    VUINT64  StartBank:8;
    VUINT64  SegStart:1;
    VUINT64  BusSize:2;
    VUINT64  DataType:1;
    VUINT64  TrInt:3;
    VUINT64  StartAddr:11;
    VUINT64  SegCount:11;
    VUINT64  TrId:6;
    VUINT64  Res:13;
} MAP_DMA_TR_CTRL, *PMAP_DMA_TR_CTRL;
#endif // _MAP_DMA_TR_CTRL_

typedef struct _FP_LOG_INFO_
{
    PUINT8      Buf;            // The pointer to the first buffer in the chain
    PMDMA_FDESC pMdmaChain;     // The MDMA descriptors list, it should cover all FP-DMA buffers

}FP_LOG_INFO;

typedef struct _FP_LOG_CTX_
{
    PUINT8            LogPtr;
    UINT32            LogOffs;
    UINT32            LogSize;
    UINT32            LogFlag;

}FP_LOG_CTX;

#ifndef _MAP_DRV_IDFT_CONFIG_
#define _MAP_DRV_IDFT_CONFIG_
typedef struct tMAP_DRV_IDFT_CONFIG
{
    UINT16 InputLoc[2];      // 2: I/Q
    UINT16 OutputLoc[2];     // 2: I/Q
    UINT16 ConfigLoc[2];     // 2: I/Q
    UINT16 MultTableLoc[2];  // 2: I/Q
} MAP_DRV_IDFT_CONFIG, *PMAP_DRV_IDFT_CONFIG;
#endif // _MAP_DRV_IDFT_CONFIG_

#ifndef _MAP_DRV_IDFT_MULT_TABLE_
#define _MAP_DRV_IDFT_MULT_TABLE_
typedef struct tMAP_DRV_IDFT_MULT_TABLE
{
    PUINT8 TableDMALoc;
    UINT32 TableSize;
}MAP_DRV_IDFT_MULT_TABLE, *PMAP_DRV_IDFT_MULT_TABLE;
#endif // _MAP_DRV_IDFT_MULT_TABLE_

typedef struct tMAP_DRV_CTX
{
    // General Params for Driver
    UINT16                          FftSize;
    UINT16                          DbgLogEna;
    volatile PMAP_MASTER_CTRL_REGS  pMasterCtrlRegs;
    volatile PMAP_APB_REGS          pApbRegs[FPDRV_NUM_FP];
    volatile PMAP_DMA_REGS          pFpDmaRegs[2];
    volatile PMAP_DMA_EXP_REGS      pDmaExpRegs[2];
    VUINT32                         CpuID;      // The cpu id used to initialize FP processors
    UINT32                          FpDrvCfg;   // FP driver configuration flags , see : FP_DRV_CFG_xxx

    VUINT32                         BusyDev;
    VUINT32                         FpDevNum;

    // IFFT Control Params
    PDMAFDESC         pIfftDmaDesc;
    MAP_DMA_TR_CTRL   IfftInTrCtrl[2];
    MAP_DMA_TR_CTRL   IfftOutTrCtrl[2];
    UINT16            IfftInpSize, IfftRndInpSize, IfftOutSize;
    UINT16            IfftCp1OutSize, IfftCp2OutSize;
    MAPDONECB         IfftPostProcCb;
    LPVOID            IfftPostProcParam;
    UINT32            nSymIFFT;
    UINT32            nNumIfftSlot;

    // IDFT Control Params
    PUINT8            IdftAppHeapAlloc;
    PUINT8            IdftCramHeapAlloc;
    UINT32            IdftUseTcbQueue;
    PDMAFDESC         pIdftDmaDesc;
    MAP_DMA_TR_CTRL   IdftInTrCtrl[3];
    MAP_DMA_TR_CTRL   IdftOutTrCtrl;
    PTCB              IdftTcb;
    VUINT32           IdftRun; // FP run mask
    MAPDONECB         IdftPostProcCb;
    LPVOID            IdftPostProcParam;
    VUINT32           IdftQueueToRun, IdftQueueRun;
    UINT32            IdftOutputSize;

    // FFT Control Params
    PDMAFDESC         pFftDmaDesc;
    MAP_DMA_TR_CTRL   FftInTrCtrl;
    MAP_DMA_TR_CTRL   FftInTrCtrl128;
    MAP_DMA_TR_CTRL   FftOutTrCtrl;
    UINT16            FftOutBufSize, FftCp1InSize, FftCp2InSize;
    MAPDONECB         FftPostProcCb;
    LPVOID            FftPostProcParam;
    UINT16            nSymFFT;
    UINT32            swapFftSlot0Ctx;


    // Queing Mechanism
    PDMAFDESC         pReadyListHead[2][FPDRV_NUM_FP];
    PDMAFDESC         pReadyListTail[2][FPDRV_NUM_FP];
    UINT16            IdftFpMask, IdftFpStart, IdftFpStop, IdftCurFp;
    UINT16            IfftFpMask, IfftFpStart, IfftFpStop, IfftCurFp;
    UINT16            FFtFpMask, FftFpStart, FftFpStop, FftCurFp;
    UINT16            nRxAnt, nTxAnt;
    PDMAFDESC         pReadyListFftIfftHead[2][NUM_MAP_SLOTS*2];
    PDMAFDESC         pReadyListFftIfftTail[2][NUM_MAP_SLOTS*2];
    UINT32            QueueFftIfft[NUM_MAP_SLOTS*2];
    UINT32            UpdateQueueFftIfft[2][NUM_MAP_SLOTS*2];
    UINT32            QueueLocationCurr, QueueLocationToDo;
    VUINT32           QueueContext[NUM_MAP_SLOTS*2];
    VUINT32           QueueToDoContext[NUM_MAP_SLOTS*2];

    UINT32            pool1FpBusy;

    UINT32            FpLogMask;
    UINT32            FpLogStream;
    FP_LOG_INFO       FpInLog [FPDRV_NUM_FP];
    FP_LOG_INFO       FpOutLog [FPDRV_NUM_FP];
    FP_LOG_CTX        FpLogCtx [3]; // 3- IFFT/FFT/IDFT

    // IDFT Tables
    UINT16 MaxIdftSizeInDesc[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    UINT16 NumIdftSizeInDesc[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    PUINT16 pIdftSizesInDesc[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    PUINT16 pIdftInpLocInDesc[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    PUINT16 pIdftOutLocInDesc[MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    PUINT8 ppConfigTables[2][MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    PUINT8 ppMultTables[2][MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    UINT32 ConfigTablesSize[2][MAX_IDFT_TRANSACTIONS_PER_SYMBOL];
    UINT32 NumDesc[2];

    // Slot based Processing
    LPVOID            FftCb[2], IfftCb[2];      // PTCB storage used for call back to Scheduler
} MAP_DRV_CTX, *PMAP_DRV_CTX;


/******************************************************************************
*                The API declaration                                          *
*******************************************************************************/


// IRQ Handlers
//-------------
void MapIrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, LPVOID pParam);
//void MapDma1IrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, void *pParam);


// Generic FP Functions
//--------------------
PMAP_DRV_CTX MapGetDrvCtx(void);
MXRC MapDrvReset(void);
MXRC MapDrvInit(UINT32 fp_mode, UINT32 fp_cfg);
MXRC MapDrvShutdown (void);
MXRC MapDrvGenShedding (void);
MXRC MapDrvGetMasterCpu (void);
MXRC MapSetDevLogMask (UINT32 nMask);
MXRC MapSetLogStorage (UINT32 FpStream, LPVOID LogPtr, UINT32 nLogSize);
BOOL MapIsInLogEnabled(UINT32 nFpID);
BOOL MapIsOutLogEnabled(UINT32 nFpID);
MXRC MapEnableLogStream (UINT32 FpStream, BOOL bEnable);
MXRC MapDrvRestoreMapMask (void);
BOOL MapDrvIsPendIrq (void);


// IDFT Functions
//-------------

MXRC MapIdftDrvShutdown(void);
MXRC MapIdftDrvInit(void);
MXRC MapIdftDrvInitLoadBigTablesToB7(UINT32 FpId);
MXRC MapIdftDrvSetMapMask(UINT16 mask);
MXRC MapIdftDrvSetCallback(MAPDONECB CbProc, LPVOID CbCtx);
MXRC MapIdftDrvSetFpNum(UINT32 fpNum);
MXRC MapIdftDrvSetInBuf(PUINT8 pInBuf, UINT32 SymNum, UINT32 InputSize);
MXRC MapIdftDrvSetOutBuf(PUINT8 pOutBuf, UINT32 SymNum, UINT32 OutputSize);
MXRC MapIdftQueue(PTCB pTcb, UINT32 SymMask);
MXRC MapIdftUseQueueFromTcb(UINT32 useTcbQueue);
MXRC MapIdftDrvRun(PTCB pTcb);
MXRC MapIdftGenerateTables(PUINT16 pIdftSizeArr, UINT32 ArrSize);



// IFFT Functions
//-------------

MXRC MapIfftDrvShutdown(void);
MXRC MapIfftDrvInit(UINT32 IfftExp);
MXRC MapIfftDrvSetMapMask(UINT16 mask);
MXRC MapIfftDrvSetNumAntennas(UINT32 nTxAnt);
MXRC MapIfftDrvSetInpBuf(PUINT32 pInBuf, UINT32 Size, UINT32 Start);
MXRC MapIfftDrvSetOutBuf(PUINT32 pOutBuf, UINT32 Size, UINT32 Start);
MXRC MapIfftDrvSetCallback(MAPDONECB CbProc, LPVOID CbCtx);
MXRC MapIfftDrvSetFpNum(UINT32 fpNum);
MXRC MapIfftDrvSetNumSym(UINT32 numSym);
MXRC MapIfftDrvPreRun(LPVOID CbCtx);
MXRC MapIfftDrvRun(LPVOID CbCtx);




// FFT Functions
//-------------
MXRC MapFftDrvShutdown(void);
MXRC MapFftDrvInit(UINT32 FftExp);
MXRC MapFftDrvSetMapMask(UINT16 mask);
MXRC MapFftDrvSetCallback(MAPDONECB CbProc, LPVOID CbCtx);
MXRC MapFftDrvSetNumAntennas(UINT32 nRxAnt);
MXRC MapFftDrvSetInpBuf(PUINT32 pInpBuf, UINT32 Size, UINT32 Start);
MXRC MapFftDrvSetOutBuf(PUINT32 pOutBuf, UINT32 Size, UINT32 Start);
MXRC MapFftDrvSetFpNum(UINT32 fpNum);
MXRC MapFftDrvSetNumSym(UINT32 numSym);
MXRC MapFftDrvPreRun(LPVOID CbCtx);
MXRC MapFftDrvRun(LPVOID CbCtx);
MXRC MapFftIfftDrvContextSwap(UINT32 swapFftIfft);
MXRC MapFttIfftDrvFlushQueue(void);


// Other Functions
//----------------

void MapDispatchFexp(UINT32 fpID, PDMAFDESC pInFDesc, PDMAFDESC pOutFDesc);
void MapDispatchPool(UINT32 poolId, PDMAFDESC pInFDesc, PDMAFDESC pOutFDesc);
void MapDispatchFexpPool(UINT32 poolId, PDMAFDESC pInFDesc, PDMAFDESC pOutFDesc);
MXRC MapDrvSetMode(UINT32 GrpId, UINT32 Mode);
UINT32 MapDrvSetModeEx(UINT32 GrpIdModeLoadType, UINT32 fp_mask, UINT64 tr_ctrl_out, UINT32 len_out, UINT32 init);
MXRC MapDrvLoadCode(UINT16 Mask, BOOL bIdft);
void MapRegWrite(UINT32 fpID, UINT32 reg_offset, UINT32 reg_val);
UINT32 MapRegRead(UINT32 fpID, UINT32 reg_offset);
void MapUploadPram(UINT32 fpID, LPVOID prog_buf, UINT32 prog_buf_size);
void MapUnloadPram(UINT32 fpID, LPVOID prog_buf, UINT32 prog_buf_size);
void MapDbgUploadDram(UINT32 fpID, BOOL auto_inc, UINT32 bank, UINT32 offset, LPVOID data_buf, UINT32 data_buf_size);
void MapDbgUnloadDram(UINT32 fpID, BOOL auto_inc, UINT32 bank, UINT32 offset, LPVOID data_buf, UINT32 data_buf_size);
UINT32 MapDbgRun(UINT32 fpID, UINT32 nCycles);
void MapDbgSetPcTrigger(UINT32 fpID, UINT32 pc);
void MapDbgSetDebugTrigger(UINT32 fpID, UINT32 counter);
void FpMmuCfg(UINT32 nCpuID, UINT32 nParam);
#endif /*_FP_DRV_H_*/

#ifdef __cplusplus
}
#endif

