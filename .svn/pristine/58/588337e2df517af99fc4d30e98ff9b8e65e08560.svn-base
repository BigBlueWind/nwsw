//-------------------------------------------------------------------------------------------
/** @file FECdrv.h
 *
 * @brief FEC (Channel Coding) HAL functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.31 $
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

#ifndef _FEC_DRV_H_
#define _FEC_DRV_H_


#include "4gmx.h"
#include "hal.h"
#include "tcb.h"
#include "appids.h"
#include "logmgr.h"

//#define FEC_ADDITIONAL_CHECK_ENABLED
//#define FEC_FIQ_USAGE_ENABLED

#define FEC_ROBUST_CTRL_ENABLED
#define FEC_ROBUST_CTRL_DEF_TIMER_PERIOD    1   // Fraction timer (some number per 1ms, see: MX_TIMER_FRACTION)

#ifdef FEC_FIQ_USAGE_ENABLED
    #define FEC_INT_MODE       INT_TYPE_FIQ
#else
    #define FEC_INT_MODE       INT_TYPE_IRQ
#endif

#define FEC_HEAP		IramHeap

// This file is included here to make possible to enable/disable
// usage of configuration macro (see above )
#include "mdmafectypes.h"

/******************************************************************************
*                The module error codes                                       *
*******************************************************************************/
#define RC_FEC_DRV_NO_HW_FOUND      DEF_USRRC(APP_MODULE, FECDIAG, 1)
#define RC_FEC_DRV_CTX_ALLOC_ERROR  DEF_USRRC(APP_MODULE, FECDIAG, 2)
#define RC_FEC_DRV_ALLOC_BUF_ERROR  DEF_USRRC(APP_MODULE, FECDIAG, 3)
#define RC_FEC_DRV_INIT_ERROR       DEF_USRRC(APP_MODULE, FECDIAG, 4)
#define RC_FEC_DRV_BUFFER_ERROR     DEF_USRRC(APP_MODULE, FECDIAG, 5)
#define RC_FEC_DRV_RUN_TIMEOUT      DEF_USRRC(APP_MODULE, FECDIAG, 6)
#define RC_FEC_DRV_LOGGER_ERROR     DEF_USRRC(APP_MODULE, FECDIAG, 7)
#define RC_FEC_DRV_SHARE_INFO_ERR   DEF_USRRC(APP_MODULE, FECDIAG, 8)
#define RC_FEC_DRV_DEV_ID_ERROR     DEF_USRRC(APP_MODULE, FECDIAG, 9)

/******************************************************************************
*                The FEC objects addresses                                    *
*******************************************************************************/

#define FEC_RESET_REG                0xFE440070
#define FEC_CTRL_REGS_BASE           0xFE410000
#define FEC_CONFIG_REGS_BASE        (FEC_CTRL_REGS_BASE + 0x400)

#define FEC_RUN_TIMEOUT             (150000) // 1 ms
#define FEC_FDESC_POOL_SIZE         64
#define FECDRV_FDESC_POOL_SIZE      (FEC_FDESC_POOL_SIZE)

#define FEC_HWCONF_WIMAX            0x00000001
#define FEC_HWCONF_LTE              0x00000000
#define FEC_HWCONF_BYPASS_CHAN_ITL  0x00040000
#define FEC_HWCONF_BYPASS_SOFT_COMB 0x00400000

#define FEC_HWCONF_DL_BYPASS        0x00010000
#define FEC_HWCONF_UL_BYPASS        0x00020000
#define FEC_HWCONF_CI_SYMNUM        0x01000000

//#define TASKID_FEC_TX               10
//#define TASKID_FEC_RX               16

#define FEC_SMR_SEL_M0              0x00000000
#define FEC_SMR_SEL_M1              0x00000001
#define FEC_SMR_M0                  0x0000FFF0
#define FEC_SMR_M1                  0xFFF00000

//The modes for logger
#define FEC_LOGFDESCMODE            0x01 // writes Frame Descriptor
#define FEC_LOGDATAMODE             0x02 // writes data

#define FEC_BLAST                   0x00010000  // BLAST bit from BDesc
#define FEC_BLEN                    0x0000FFFF  // size of data from BDesc
#define FEC_INBUF                   1         // input buffers
#define FEC_OUTBUF                  0         // output buffers

#define ID_FEC_DL                   1
#define ID_FEC_UL                   2

#define FEC_DL_CORE_0_FDESC_NUM     64
#define FEC_DL_CORE_1_FDESC_NUM     0
#define FEC_DL_CORE_2_FDESC_NUM     0
#define FEC_DL_CORE_3_FDESC_NUM     0
#define FEC_DL_CEVAX_FDESC_NUM      64
#define FEC_DL_CEVAX_OWNER          0

#define FEC_UL_CORE_0_FDESC_NUM     64
#define FEC_UL_CORE_1_FDESC_NUM     0
#define FEC_UL_CORE_2_FDESC_NUM     0
#define FEC_UL_CORE_3_FDESC_NUM     0
#define FEC_UL_CEVAX_FDESC_NUM      64
#define FEC_UL_CEVAX_OWNER          0

#define FEC_NUM_MEM_REGIONS         10

#define FEC_UL_HANGED               1
#define FEC_DL_HANGED               1

// TBD: remove
typedef struct tFEC_LTE_IOParams
{
    UINT16 CodingType;          // fec_mode (1=CTC, 0=CC to the HW interface)
    UINT16 BypassInterleaver;   // Bypass Inter/DeInterleaver: 1 bypass (Enc)(dec)
    UINT16 BypassEncoder;       // Bypass Encoder: 1 bypass (Enc)
    UINT16 BypassRateMatching;  // Bypass Rate Matching: 1 bypass
    UINT16 Blocksize;           // In bits, minimum 40 bits, maximum 6144 bits for Enc,
    UINT16 CRC_Sel;             // (0:disabled, 1 crc24a. 2 crc24b) (Enc and dec)
    UINT16 Filler;              // Number of filler bits(Enc in)
    UINT16 RateMatchingK0;      // Rate Matching K0
    UINT32 RateMatchingE;       // Rate Matching Output Size
    UINT16 RateMatchingNcb;     // Rate Matching Ncb
    UINT16 ScrambEnable;        // Scrambler Enable 0 disable (Enc)
    UINT16 ScrambMask;          // Scrambler Mask(Enc)
    UINT16 numFECHalfIterations;// Half Iterations control (default to 16)
    SINT16 tc_rate;             // tc7000 (tc_rate goes to rate index in HW
    SINT16 tc_modi;             // tc7000 (tc_modi / 2 goes to modulation in WiMAX, 0 always for LTE)
    SINT16 tc_nout;             // tc7000 Encoder Output Ptr (Bit array)(Enc),(Dec)Frame Size in bits

} FEC_LTE_IOParams, *PFEC_LTE_IOParams;

typedef struct tFEC_IO_PARAMS
{
    //UINT16  OperationMode;
    UINT16  CodingType;
    UINT16  BlockSize; // bits for LTE, bytes for WiMAX
    UINT16  CrcSelector;
    UINT16  FillerBits;
    UINT16  Modulation;
    UINT16  CodingRate;
    UINT16  HalfIterations;
    UINT16  DynamicStopIteration;
    UINT16  DynamicStopThreshold;
    UINT16  ScrambleEnable;
    UINT16  ScrambleMask;
    UINT16  SubpacketId;
    UINT16  BpBlockInterleaving;
    UINT16  BpEncoder;
    UINT16  BpDecoder;
    UINT16  EnableCombine;
    UINT16  BpRateMatching;
    UINT32  RateMatchingE;
    UINT16  RateMatchingk0;
    UINT16  RateMatchingNcb;
    UINT16  BpChannelInterleaving;
    UINT16  CiSymbolNumber;
    //PUINT32 CiInsertions;

} FEC_IO_PARAMS, *PFEC_IO_PARAMS;

typedef struct tFEC_DL_CTX
{
    volatile FEC_CTRL_REGS *pFecCtrlRegs;

    volatile BOOL            bInited;
    volatile UINT32          StartTicks;
    volatile UINT32          EndTicks;
    volatile UINT32          CompFlag;
    

    volatile PFECDL_LIST_INF ListInfPtr;    // depends on mode: to cacheable or to CRAM memory
    HANDLE                   ListInfHeap;
    
    
    volatile UINT32          CpuID;
    volatile UINT32          StorageOwner;
             BOOL            bLogEnable;    // indicates whether Log was enabled or disabled
             LOGHANDLE       hLog;          // Log handler
             UINT8           logctrl;       // Log control

             MXPOOL          Pool [CPU_NUM];// The pool of FDESC blocks for each core  
             MXPOOL *        CevaPoolPtr;   // one general pool for CEVAs
             PUINT8          PoolStorPtr;
             PUINT32         PoolIndex;

    volatile UINT32          status;
} FEC_DL_CTX, *PFEC_DL_CTX;

typedef struct tFEC_UL_CTX
{
    volatile FEC_CTRL_REGS *pFecCtrlRegs;

    volatile BOOL            bInited;
    volatile UINT32          StartTicks;
    volatile UINT32          EndTicks;
    volatile UINT32          CompFlag;
    volatile PFECUL_LIST_INF ListInfPtr;    // depends on mode: to cacheable or to CRAM memory
    HANDLE                   ListInfHeap;
    volatile UINT32          CpuID;
    volatile UINT32          StorageOwner;
             BOOL            bLogEnable;    // indicates whether Log was enabled or disabled
             LOGHANDLE       hLog;          // Log handler
             UINT8           logctrl;       // Log control

             MXPOOL          Pool [CPU_NUM];// The pool of FDESC blocks for each core  
             MXPOOL *        CevaPoolPtr;   // one general pool for CEVAs
             PUINT8          PoolStorPtr;
             PUINT32         PoolIndex;

    volatile UINT32          status;             
} FEC_UL_CTX, *PFEC_UL_CTX;

typedef struct tMEMORY_REGIONS
{
    UINT32 start;
    UINT32 end;
}MEMORY_REGIONS,PMEMORY_REGIONS;

typedef struct _FEC_ROBUST_CTRL_CTX_
{
    UINT32 Inited;
    UINT32 CpuID;
    UINT32 Period;
    HANDLE hTimer;
    UINT32 ResetNum;

}FEC_ROBUST_CTRL_CTX;

void FecReset(void);
void FecDLReset(void);
MXRC FecInit(PFEC_CONFIG_REGS pFecConfigRegs);
MXRC FecShutdown(void);
MXRC FecGenShedding(void);
void FecAckIrq(UINT32 irq);
MXRC FecRobustCtrl (UINT32 bInit, UINT32 nPeriod);
UINT32 FecGetResetNum(UINT32 nResetCounter);
MXRC FecRobustInit(void);
MXRC FecRobustShutdown(void);
UINT32 FecRobustGetMasterCpu(void);

PFEC_DL_FDESC FecDlAllocDesc(UINT32 NumDesc);
MXRC FecDlSetHwCtx(PFEC_DL_FDESC pFDesc, DLHWCTX HwContext);
MXRC FecDlSetData(PFEC_DL_FDESC pFDesc, LPVOID pInData, UINT32 InDataSize, LPVOID pOutData, UINT32 OutDataSize);
MXRC FecDlSetDataEx(PFEC_DL_FDESC pFDesc, LPVOID pInData, UINT32 InDataSize, LPVOID pOutData, UINT32 OutDataSize, UINT32 CacheCtrl);
MXRC FecDlSetLastDesc(PFEC_DL_FDESC pFDesc, void (*CbDone)(LPVOID), LPVOID CbContext);
MXRC FecDlRun(PFEC_DL_FDESC pFDesc);
MXRC FecDlRunQ(PFEC_DL_FDESC pFDesc, UINT32 nNumUs);
UINT32 FecDlGetExecTicks(void);
UINT32 FecDLGetMasterCpu (void);
MXRC FecDlShutdown(void);
MXRC FecDlGenShedding(void);
MXRC FecDlInit(PFEC_CONFIG_REGS pFecConfigRegs);
MXRC FecDlSetLog(UINT8 logctrl, LPVOID pDst, UINT32 datasize);
MXRC FecDlCloseLog(void);


PFEC_UL_FDESC FecUlAllocDesc(UINT32 NumDesc);
PFEC_UL_FDESC FecUlAllocNewDesc(PFEC_UL_FDESC pold);
MXRC FecUlSetHwCtx(PFEC_UL_FDESC pFDesc, ULHWCTX HwContext);
MXRC FecUlSetData(PFEC_UL_FDESC pFDesc, LPVOID pInData[], UINT32 InDataSize[], LPVOID pOutData, UINT32 OutDataSize);
MXRC FecUlSetDataEx(PFEC_UL_FDESC pFDesc, LPVOID pInData[], UINT32 InDataSize[], LPVOID pOutData, UINT32 OutDataSize, UINT32 CacheCtrl);
MXRC FecUlSetDataOpt(PFEC_UL_FDESC pFDesc, LPVOID pInData[], UINT32 InDataSize, LPVOID pOutData, UINT32 OutDataSize);
MXRC FecUlSetLastDesc(PFEC_UL_FDESC pFDesc, void (*CbDone)(LPVOID), LPVOID CbContext);
MXRC FecUlRun(PFEC_UL_FDESC pFDesc);
MXRC FecUlRunQ(PFEC_UL_FDESC pFDesc, UINT32 nNumUs);
UINT32 FecUlGetExecTicks(void);
void FECDrvDLIrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, LPVOID pParam);
void FECDrvULIrqHandler(volatile UINT32 *pIntClearReg, UINT32 nIrqID, LPVOID pParam);
UINT32 FecULGetMasterCpu (void);
MXRC FecUlShutdown(void);
MXRC FecUlGenShedding(void);
MXRC FecUlInit(PFEC_CONFIG_REGS pFecConfigRegs);
MXRC FecUlSetLog(UINT8 logctrl, LPVOID pDst, UINT32 datasize);
MXRC FecUlCloseLog(void);

extern HANDLE g_FecDlPart;
extern HANDLE g_FecUlPart;
extern PUINT32 g_FecDlBuf;
extern PUINT32 g_FecUlBuf;

void FecDlMmuCfg(UINT32 nCpuID, UINT32 nParam);
void FecUlMmuCfg(UINT32 nCpuID, UINT32 nParam);
UINT32 FecIsMemUsed(UINT32 nAddr, UINT32 nSize);
#endif /*_FEC_DRV_H_*/

#ifdef __cplusplus
}
#endif

