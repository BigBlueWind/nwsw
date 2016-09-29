//-------------------------------------------------------------------------------------------
/** @file Mdmadrv.h
 *
 * @brief MDMA (Memory DMA) HAL functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.52 $
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

#ifndef __MDMADRV_H__
#define __MDMADRV_H__


#include "4gmx.h"
#include "tcb.h"
#include "hal.h"
#include "appids.h"
#include "initcpu.h"
#include "logmgr.h"

#define MDMA_POOL_USAGE_ENABLED
//#define MDMA_AXI_BUS_ERROR_PROCESS

#define MDMA_SYS_ROBUST_CTRL_ENABLED
#define MDMA_SPU_ROBUST_CTRL_ENABLED
#define MDMA_RAD_ROBUST_CTRL_ENABLED

#define MDMA_SYS_ROBUST_CTRL_DEF_TIMER_PERIOD   1   // Fraction timer (some number per 1ms, see: MX_TIMER_FRACTION)
#define MDMA_SPU_ROBUST_CTRL_DEF_TIMER_PERIOD   1   // Fraction timer (some number per 1ms, see: MX_TIMER_FRACTION)
#define MDMA_RAD_ROBUST_CTRL_DEF_TIMER_PERIOD   1   // Fraction timer (some number per 1ms, see: MX_TIMER_FRACTION)

//#define MDMA_FIQ_USAGE_ENABLED

#ifdef MDMA_FIQ_USAGE_ENABLED
    #define MDMA_INT_MODE       INT_TYPE_FIQ
#else
    #define MDMA_INT_MODE       INT_TYPE_IRQ
#endif

// This file is included here to make possible to enable/disable
// usage of configuration macro (see above )
#include "mdmafectypes.h"

#define RC_MDMA_DRV_NO_HW_FOUND     DEF_USRRC(APP_MODULE, MDMADIAG, 1)
#define RC_MDMA_DRV_CTX_ALLOC_ERROR DEF_USRRC(APP_MODULE, MDMADIAG, 2)
#define RC_MDMA_DRV_ALLOC_BUF_ERROR DEF_USRRC(APP_MODULE, MDMADIAG, 3)
#define RC_MDMA_DRV_INIT_ERROR      DEF_USRRC(APP_MODULE, MDMADIAG, 4)
#define RC_MDMA_DRV_BUFFER_ERROR    DEF_USRRC(APP_MODULE, MDMADIAG, 5)
#define RC_MDMA_DRV_RUN_TIMEOUT     DEF_USRRC(APP_MODULE, MDMADIAG, 6)
#define RC_MDMA_DRV_PARAM_ERROR     DEF_USRRC(APP_MODULE, MDMADIAG, 7)
#define RC_MDMA_DRV_INVALID_OP_CODE DEF_USRRC(APP_MODULE, MDMADIAG, 8)
#define RC_MDMA_DRV_UNSUPPORTED_CPU DEF_USRRC(APP_MODULE, MDMADIAG, 9)
#define RC_MDMA_DRV_LOGGER_ERROR    DEF_USRRC(APP_MODULE, MDMADIAG, 10)
#define RC_MDMA_DRV_ALLOC_INF_ERROR DEF_USRRC(APP_MODULE, MDMADIAG, 11)
#define RC_MDMA_DRV_SHARE_INFO_ERR  DEF_USRRC(APP_MODULE, MDMADIAG, 12)
#define RC_MDMA_DRV_DEV_ID_ERROR    DEF_USRRC(APP_MODULE, MDMADIAG, 14)

// Note: Non-Blocking MDMA InterCpu calls are slow.
// (For better performance) Use SYS, RAD and SPU MDMA by cores on which they are initialized
#define MDMA_RUN_TIMEOUT            (150000)    // 1 ms
#define MDMA_CTRL_REGS_BASE         MDMA_SYS_BASEADDR // 0xFE400000
#define MDMA_SYS_CTRL_REGS_BASE     MDMA_SYS_BASEADDR // 0xFE400000
#define MDMA_SPU_CTRL_REGS_BASE     MDMA_SPU_BASEADDR // 0xFD010000
#define MDMA_RAD_CTRL_REGS_BASE     RAD_APB_MDMA_BASEADDR // 0xFE8C0000
#define MDMA_FDESC_POOL_SIZE        50
#define MDMA_TRSIZE                 64000

#define MDMA_CACHE_PROC_NONE        (3)
#define MDMA_CACHE_PROC_IN          (1 << 1)
#define MDMA_CACHE_PROC_OUT         (1 << 0)
#define MDMA_CACHE_PROC_BOTH        (MDMA_CACHE_PROC_IN | MDMA_CACHE_PROC_OUT)

#define MDMA_IRQ_ERR_AXI_READ       (1 << 1)
#define MDMA_IRQ_ERR_AXI_WRITE      (1 << 0)

//#define MDMA_BURST_CONTROL_ENABLED

#define SYS_MDMA_CPU_0_FDESC_NUM    64
#define SYS_MDMA_CPU_1_FDESC_NUM    0
#define SYS_MDMA_CPU_2_FDESC_NUM    0
#define SYS_MDMA_CPU_3_FDESC_NUM    0
#define SYS_MDMA_CEVAX_FDESC_NUM    64
#define SYS_MDMA_CEVAX_OWNER        0

#define SPU_MDMA_CPU_0_FDESC_NUM    54
#define SPU_MDMA_CPU_1_FDESC_NUM    0
#define SPU_MDMA_CPU_2_FDESC_NUM    0
#define SPU_MDMA_CPU_3_FDESC_NUM    10
#define SPU_MDMA_CEVAX_FDESC_NUM    0
#define SPU_MDMA_CEVAX_OWNER        0

#define RAD_MDMA_CPU_0_FDESC_NUM    32
#define RAD_MDMA_CPU_1_FDESC_NUM    32
#define RAD_MDMA_CPU_2_FDESC_NUM    16
#define RAD_MDMA_CPU_3_FDESC_NUM    16
#define RAD_MDMA_CEVAX_FDESC_NUM    0
#define RAD_MDMA_CEVAX_OWNER        0

#define MDMA_HW_NAME_LEN            10

#if (SYS_MDMA_CEVAX_OWNER != 0 || SPU_MDMA_CEVAX_OWNER != 0 || RAD_MDMA_CEVAX_OWNER != 0)
#error "At this moment CEVA owner mode is not supported"
#endif

typedef struct tMDMA_PROMPT
{
    UINT32 idCmd;
    UINT8 name[MDMA_HW_NAME_LEN];
}MDMA_PROMPT, *PMDMA_PROMPT;

typedef struct _MDMA_ROBUST_CTRL_CTX_
{
    UINT32 Inited;
    UINT32 CpuID;
    UINT32 Period;
    HANDLE hTimer;
    UINT32 ResetNum;

}MDMA_ROBUST_CTRL_CTX;


typedef struct tMDMA_CTX
{
    volatile MDMA_CTRL_REGS *MdmaCtrlRegs;

    BOOL            bInited;
    UINT32          StartTicks;
    UINT32          EndTicks;
    UINT32          CompFlag;

    PMDMA_LIST_INF  ListInfPtr;         // depends on mode: to cacheable or to CRAM memory
    HANDLE          ListInfHeap;

    VUINT32         CpuID;              // this value is used to detect CPU ID where driver was started
    VUINT32         StorageOwner;       // TRUE is this driver has created partition and allocated memory
    BOOL            bLogEnable;         // indicates whether Log was enabled or disabled
    LOGHANDLE       hLog;               // Log handler
    UINT32          opcode;             // operation code to be saved
    UINT8           logctrl;            // Log control
    UINT16			MdmaIrqId;
	PVUINT32		pSysLock;
	HANDLE	*		pPart;
    HANDLE          hPart;
    MXPOOL          Pool[CPU_NUM];      // CPU_NUM - ARM cores
    MXPOOL  *       CevaPoolPtr;        // 1 public CEVA pool
    PUINT8          PoolStorPtr;
    PUINT32         PoolIndex;
    UINT32          HwSema;

} MDMA_CTX, *PMDMA_CTX;

//-------------------------------------------------------------------------------------------
/** @brief This function is used to set an MDMA descriptor to perform a COPY operation
 *
 *  @param pMdma          Pointer to the MDMA descriptor
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
INLINE void MdmaSetCopy (PMDMA_FDESC pMdma)
{
    MDMA_HW * pMdmaHw = MDMA_GET_HW(pMdma);

    // COPY and BOffset = 1

    pMdmaHw->FControl = MDMA_OP_COPY | (1 << 4);
    pMdmaHw->FStatus0 = 0;
    pMdmaHw->FStatus1 = 0;
}

//-------------------------------------------------------------------------------------------
/** @brief This function is used to set an MDMA descriptor to perform a CRC calculation operation
 *
 *  @param pMdma          Pointer to the MDMA descriptor
 *  @param CrcInit           CRC Init Value
 *  @param PolyValue       CRC Polynomial Value
 *  @param PolyOrder      CRC Polynomial Order
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
INLINE void MdmaSetCRC (PMDMA_FDESC pMdma, UINT32 CrcInit, UINT32 PolyValue, UINT32 PolyOrder)
{
    MDMA_HW * pMdmaHw = MDMA_GET_HW(pMdma);

    // 4G SCR and BOffset = 1

    pMdmaHw->FControl = MDMA_OP_CRC | (1 << 4) | (((PolyValue & 0xFFFF)) << 16);

    pMdmaHw->FStatus0 = CrcInit;
    pMdmaHw->FStatus1 = ((PolyValue >> 16) & 0xFFFF) | ((PolyOrder & 0x7) << 16);
}



//-------------------------------------------------------------------------------------------
/** @brief This function is used to set an MDMA descriptor to perform a 4G SCRAMBLER operation
 *
 *  @param pMdma          Pointer to the MDMA descriptor
 *  @param cInit0             cInit0 Seed value
 *  @param cInit1             cInit1 Seed value
 *  @param PackMode      Packing Mode
 *  @param nScrOpt         Scrambling Operation Mode
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------

INLINE void MdmaSet4GSCR (PMDMA_FDESC pMdma, UINT32 cInit0, UINT32 cInit1, UINT32 PackMode, UINT32 nScrOpt)
{
    MDMA_HW * pMdmaHw = MDMA_GET_HW(pMdma);

    // 4G SCR and BOffset = 1

    pMdmaHw->FControl = MDMA_OP_4SCR | (1 << 4);
    pMdmaHw->FStatus0 = cInit0 & 0x7FFFFFFF;
    pMdmaHw->FStatus1 = cInit1 & 0x7FFFFFFF;
    pMdmaHw->FControl |= ((nScrOpt  & 0x3) << 17) | ((PackMode & 0x1) << 16);
}


//-------------------------------------------------------------------------------------------
/** @brief This function is used to set an MDMA descriptor to perform a 4G DE-SCRAMBLER operation
 *
 *  @param pMdma          Pointer to the MDMA descriptor
 *  @param cInit0             cInit0 Seed value
 *  @param cInit1             cInit1 Seed value
 *  @param nScrOpt         De-Scrambling Operation Mode
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
INLINE void MdmaSet4GDSCR (PMDMA_FDESC pMdma, UINT32 cInit0, UINT32 cInit1, UINT32 nScrOpt)
{
    MDMA_HW * pMdmaHw = MDMA_GET_HW(pMdma);

    // 4G DESCR and BOffset = 1

    pMdmaHw->FControl = MDMA_OP_4DSC | (1 << 4);
    pMdmaHw->FStatus0 = cInit0 & 0x7FFFFFFF;
    pMdmaHw->FStatus1 = cInit1 & 0x7FFFFFFF;
    pMdmaHw->FControl |= ((nScrOpt  & 0x3) << 17);
}


//-------------------------------------------------------------------------------------------
/** @brief This function is used to set an MDMA descriptor to perform a 4G DE-SCRAMBLER operation when there is more than
 *               1 I/O pointer set to the descriptor (Done when I/O buffer size > 64K Bytes)
 *
 *  @param pMdma          Pointer to the MDMA descriptor
 *  @param cInit0             cInit0 Seed value
 *  @param cInit1             cInit1 Seed value
 *  @param nScrOpt         De-Scrambling Operation Mode
 *  @param numDesc       Number of descriptor (returned by the Mdma<B>XYZ</B>SetDataMultiple functions)
 *                                     where XYZ could be SYS, SPU or RAD
 *
 *  @return none
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------
INLINE void MdmaSetMultiple4GDSCR (PMDMA_FDESC pMdma, UINT32 cInit0, UINT32 cInit1, UINT32 nScrOpt, UINT32 numDesc)
{
    MDMA_HW * pMdmaHw = MDMA_GET_HW(pMdma);

    // 4G DESCR and BOffset = 1

    pMdmaHw->FControl = MDMA_OP_4DSC | (numDesc << 4);
    pMdmaHw->FStatus0 = cInit0 & 0x7FFFFFFF;
    pMdmaHw->FStatus1 = cInit1 & 0x7FFFFFFF;
    pMdmaHw->FControl |= ((nScrOpt  & 0x3) << 17);
}


//-------------------------------------------------------------------------------------------
/** @brief This function is used to set I/O pointers to the SYS MDMA Descriptor if the Input / Output size is <= 64K bytes.
 *
 *  @param pFDesc          Pointer to the MDMA descriptor
 *  @param pInData         Pointer to the Input array
 *  @param InDataSize     Size of the Input array
 *  @param pOutData       Pointer to the Output array
 *  @param OutDataSize   Size of the Output array
 *
 *  @return returns MX_OK
 *
 *  \ingroup group_lte_hal_mdma
 *
**/
//-------------------------------------------------------------------------------------------

INLINE void MdmaSetData(PMDMA_FDESC pFDesc, LPVOID pInData, UINT32 InDataSize, LPVOID pOutData, UINT32 OutDataSize)
{
    _ASSERT_PTR(pFDesc);

    pFDesc->IoBdesc[1].BPtr = (UINT32)pInData;
    pFDesc->IoBdesc[1].BCtrl = BCTRL_BLAST_MASK | InDataSize;

    if (SysCoreIsDCached((UINT32)pInData))
    {
        MxCacheClean(pInData, InDataSize);
    }

    pFDesc->IoBdesc[0].BPtr = (UINT32)pOutData;
    pFDesc->IoBdesc[0].BCtrl = BCTRL_BLAST_MASK | OutDataSize;

    if (SysCoreIsDCached((UINT32)pOutData))
    {
        // if buffer is not cache line aligned,
        // the non-aligned part should be cleaned before invalidation
        // to prevent data loss
        if (((UINT32)pOutData & 31) != 0 )
        {
            MxCacheClean(pOutData, 32);
        }
        MxCacheInvalidate(pOutData, OutDataSize);
    }
}

/** @brief This function sets two input and two output buffer pointers
           to one frame descriptiot.

    @param pFDesc   [in] - the pointer to frame descriptor allocated by MdmaAllocDesc
    @param pIn0     [in] - the pointer to buffer#0
    @param InSize0  [in] - the size of buffer#0 in bytes
    @param pIn1     [in] - the pointer to buffer#1
    @param InSize1  [in] - the size of buffer#1 in bytes
    @param pOut0    [out]- the pointer to output buffer#0
    @param OutSize0 [in] - the size of output buffer#0 in bytes
    @param pOut1    [out]- the pointer to output buffer#1
    @param OutSize1 [in] - the size of output buffer#1 in bytes
    */

INLINE void MdmaSetData2(PMDMA_FDESC pFDesc,
                           LPVOID pIn0, UINT32 InSize0,
                           LPVOID pIn1, UINT32 InSize1,
                           LPVOID pOut0, UINT32 OutSize0,
                           LPVOID pOut1, UINT32 OutSize1,
                           UINT32 nCacheCtrl)
{
    MDMA_HW * pMdmaHw = MDMA_GET_HW(pFDesc);

    pMdmaHw->FControl &= ~(0xFF << 4);
    pMdmaHw->FControl |= (2 << 4);

    //pMdmaHw->FControl = MDMA_OP_COPY | (2 << 4);

    _ASSERT_PTR(pFDesc);

    pFDesc->IoBdesc[2].BPtr = (UINT32)pIn0;
    pFDesc->IoBdesc[2].BCtrl = InSize0 & BCTRL_LEN_MASK;
    pFDesc->IoBdesc[3].BPtr = (UINT32)pIn1;
    pFDesc->IoBdesc[3].BCtrl = BCTRL_BLAST_MASK | (InSize1 & BCTRL_LEN_MASK);

    if (nCacheCtrl & MDMA_CACHE_PROC_IN)
    {
        if (SysCoreIsDCached((UINT32)pIn0))
        {
            MxCacheClean(pIn0, InSize0);
        }

        if (SysCoreIsDCached((UINT32)pIn1))
        {
            MxCacheClean(pIn1, InSize1);
        }
    }

    pFDesc->IoBdesc[0].BPtr = (UINT32)pOut0;
    pFDesc->IoBdesc[0].BCtrl = OutSize0 & BCTRL_LEN_MASK;
    pFDesc->IoBdesc[1].BPtr = (UINT32)pOut1;
    pFDesc->IoBdesc[1].BCtrl = BCTRL_BLAST_MASK | (OutSize1 & BCTRL_LEN_MASK);

    if (nCacheCtrl & MDMA_CACHE_PROC_OUT)
    {
        if (SysCoreIsDCached((UINT32)pOut0))
        {
            MxCacheInvalidate(pOut0, OutSize0);
        }

        if (SysCoreIsDCached((UINT32)pOut1))
        {
            MxCacheInvalidate(pOut1, OutSize1);
        }
    }
}

INLINE void MdmaSetDataEx(PMDMA_FDESC pFDesc,
                             LPVOID pInData,
                             UINT32 InDataSize,
                             LPVOID pOutData,
                             UINT32 OutDataSize,
                             UINT32 CacheCtrl)
{
    _ASSERT_PTR(pFDesc);

    pFDesc->IoBdesc[1].BPtr = (UINT32)pInData;
    pFDesc->IoBdesc[1].BCtrl = BCTRL_BLAST_MASK | InDataSize;
    if (CacheCtrl & MDMA_CACHE_PROC_IN)
    {
        if (SysCoreIsDCached((UINT32)pInData))
        {
            MxCacheClean(pInData, InDataSize);
        }
    }

    pFDesc->IoBdesc[0].BPtr = (UINT32)pOutData;
    pFDesc->IoBdesc[0].BCtrl = BCTRL_BLAST_MASK | OutDataSize;

    if (CacheCtrl & MDMA_CACHE_PROC_OUT)
    {
        if (SysCoreIsDCached((UINT32)pOutData))
        {
            // if buffer is not cache line aligned,
            // the non-aligned part should be cleaned before invalidation
            // to prevent data loss
            if (((UINT32)pOutData & 31) != 0 )
            {
                MxCacheClean(pOutData, 32);
            }
            MxCacheInvalidate(pOutData, OutDataSize);
        }
    }
}


MXRC MdmaIsInited(MDMA_HW_ID mdma_id);
MXRC MdmaReset(void);
MXRC MdmaInit(void);
MXRC MdmaShutdown(void);
MXRC MdmaSysInit(void);
MXRC MdmaSpuInit(void);
MXRC MdmaRadInit(void);
MXRC MdmaSysShutdown(void);
MXRC MdmaSpuShutdown(void);
MXRC MdmaRadShutdown(void);
MXRC MdmaGenShedding(void);
void MdmaAckIrq (UINT32 irq);
BOOL MdmaIsBusy(UINT32 nMdmaID);

PMDMA_FDESC MdmaAllocDesc(MDMA_HW_ID mdma_id, UINT32 NumDesc);
PMDMA_FDESC MdmaGetSingleDesc (MDMA_HW_ID mdma_id);
MXRC MdmaSetHwCtx(PMDMA_FDESC pFDesc, MDMAHWCTX HwContext);
void MdmaSetData(PMDMA_FDESC pFDesc, LPVOID pInData, UINT32 InDataSize, LPVOID pOutData, UINT32 OutDataSize);
UINT32 MdmaSetDataMultiple(PMDMA_FDESC pFDesc, LPVOID pInData, LPVOID pOutData, UINT32 DataSize);
MXRC MdmaSetLastDesc(PMDMA_FDESC pFDesc, void (*CbDone)(LPVOID), LPVOID CbContext);
MXRC MdmaRunEx(MDMA_HW_ID mdma_id, PMDMA_FDESC pFDesc, void (*CbDone)(LPVOID), LPVOID CbContext);
MXRC MdmaRunExQ(MDMA_HW_ID mdma_id, PMDMA_FDESC pFDesc, void (*CbDone)(LPVOID), LPVOID CbContext, UINT32 nNumUs);
void MdmaEmptyCallback(LPVOID p);
MXRC MdmaStart(MDMA_HW_ID mdma_id, PMDMA_FDESC pFDesc);
MXRC MdmaEnd(MDMA_HW_ID mdma_id, PMDMA_FDESC pFDesc);
void MdmaFreeDesc (MDMA_HW_ID mdma_id, PMDMA_FDESC pFDesc);
UINT32 MdmaGetExecTicks(MDMA_HW_ID mdma_id);
MXRC MdmaShedding(MDMA_HW_ID mdma_id);
MXRC MdmaCopy (MDMA_HW_ID mdma_id, LPVOID pDst, LPVOID pSrc, UINT32 nSize);

////////////////////////////////////////////
//Old
////////////////////////////////////////////

MXRC MdmaSysSetLog(UINT32 opcode, UINT8 logctrl, LPVOID pDst, UINT32 datasize);
MXRC MdmaSysCloseLog(void);
MXRC MdmaSpuSetLog(UINT32 opcode, UINT8 logctrl, LPVOID pDst, UINT32 datasize);
MXRC MdmaSpuCloseLog(void);
MXRC MdmaRadArraysCopy (LPVOID pDst, LPVOID* ppSrc, UINT32* pSize, UINT32 numberOfItems);
UINT32 MdmaSysGetMasterCpu (void);
UINT32 MdmaSpuGetMasterCpu (void);
UINT32 MdmaRadGetMasterCpu (void);

/** @brief This function calculates the CRC for the given buffer

    @param pRemainder [in/out]
    @param polynomial [in]
    @param width      [in] - the CRC width: 32/24/16/12/8 bits
    @param message    [in] - the source message for the CRC calculation
    @param nBytes     [in] - the size of message in bytes

    @return [UINT32] the CRC */

UINT32 MdmaProcCRC(UINT32 * pRemainder, UINT32 polynomial, UINT32 width, UINT8 const message[], UINT32 nBytes);
UINT32 MdmaProcCRC_b(UINT32 * pRemainder, UINT32 polynomial, UINT32 width, UINT8 const message[], UINT32 nBytes, UINT32 nBits);

void MdmaPrintInfo(PMDMA_CTX pMdmaCtx);
PUINT8 MdmaGetHWName(PMDMA_CTX pMdmaCtx);
PUINT8 MdmaGetOpName(PMDMA_FDESC pFDesc);
void MdmaSysMmuCfg(UINT32 nCpuID, UINT32 nParam);
void MdmaSpuMmuCfg(UINT32 nCpuID, UINT32 nParam);
void MdmaRadMmuCfg(UINT32 nCpuID, UINT32 nParam);
UINT32 MdmaIsMemUsed(UINT32 nAddr, UINT32 nSize);

MXRC MdmaSysRobustInit(void);
MXRC MdmaSpuRobustInit(void);
MXRC MdmaRadRobustInit(void);

MXRC MdmaSysRobustShutdown(void);
MXRC MdmaSpuRobustShutdown(void);
MXRC MdmaRadRobustShutdown(void);

UINT32 MdmaSysRobustGetMasterCpu(void);
UINT32 MdmaSpuRobustGetMasterCpu(void);
UINT32 MdmaRadRobustGetMasterCpu(void);

UINT32 MdmaGetResetNum(MDMA_HW_ID mdma_id, UINT32 nResetCounter);

#endif /* __MDMADRV_H__ */

#ifdef __cplusplus
}
#endif

