//-------------------------------------------------------------------------------------------
/** @file DsPdrv.h
 *
 * @brief DSP (Ceva) HAL functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.32 $
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

#ifndef _DSP_DRV_H_
#define _DSP_DRV_H_
#include "4gmx.h"
#include "tcb.h"
#include "appids.h"
#include "hal.h"
#include "logmgr.h"
#include "devinfo.h"
#include "cevadrvcfg.h"

/******************************************************************************
*                The module error codes                                       *
*******************************************************************************/
#define RC_DSP_DRV_NO_HW_FOUND       DEF_USRRC(APP_DRIVER, DSP_DRV, 1)
#define RC_DSP_DRV_ALLOC_ERROR       DEF_USRRC(APP_DRIVER, DSP_DRV, 2)
#define RC_DSP_DRV_SPU_TIMEOUT       DEF_USRRC(APP_DRIVER, DSP_DRV, 3)
#define RC_DSP_DRV_DUMP_BUF_SO_SMALL DEF_USRRC(APP_DRIVER, DSP_DRV, 4)
#define RC_DSP_DRV_TIMEOUT           DEF_USRRC(APP_DRIVER, DSP_DRV, 5)
#define RC_DSP_DRV_BISR_FAILED       DEF_USRRC(APP_DRIVER, DSP_DRV, 6)
#define RC_DSP_DRV_BOOT_FAILED       DEF_USRRC(APP_DRIVER, DSP_DRV, 7)
#define RC_DSP_DRV_DUMP_PTR_ERROR    DEF_USRRC(APP_DRIVER, DSP_DRV, 8)
#define RC_DSP_DRV_DUMP_TIMEOUT      DEF_USRRC(APP_DRIVER, DSP_DRV, 9)
#define RC_DSP_DRV_NO_REX_SERVER     DEF_USRRC(APP_DRIVER, DSP_DRV, 10)
/******************************************************************************
*                The module log settings                                      *
*******************************************************************************/
#define DSP_LOG_TCB         (1 << 0)
#define DSP_LOG_IOCTRL      (1 << 1)
#define DSP_LOG_CTX         (1 << 2)
#define DSP_LOG_INP         (1 << 3)
#define DSP_LOG_OUT         (1 << 4)

#define DSP_DUMP_ENABLED
#define DSP_DUMP_HEAP       (NcnbHeap)
#define DSP_DUMP_SIZE       (128*1024)

#define DSP_DRV_EXCLUDE_LIST    (0<<7)   // The list of DSPs that are needed to be excluded alwas (by default all are included)

#ifdef REX_S_FORCED_ENABLED
#define DSP_DRV_DEF_REX_SERV    0
#else
#define DSP_DRV_DEF_REX_SERV    (0xff)
#endif

typedef struct tDSP_CTRL_REGS
{
    UINT32     Version;
    UINT32     Pad0;
    UINT32     DspBootAddrReg;
    UINT32     Pad1;
    UINT32     CtrlReg;
    UINT32     Pad2;
    UINT32     StatusReg;
    UINT32     Pad3;
    UINT32     JTagStatusReg;
    UINT32     Pad4;
} DSP_CTRL_REGS, *PDSP_CTRL_REGS;

typedef struct tDSP_CTX
{
    TCB *       pExecTcb;
    enum        {DSP_IN_RESET, DSP_READY, DSP_RUN} state;
    BOOL        bAsyncMode;
    UINT32      DspID;
    UINT32      StartTick;
    UINT32      ExecTicks;
    void        (*cbReqComp)(void *);
    void *      hReqCompCtx;
    LOGHANDLE   hLog;
    UINT32      LogMask;    // See: DSP_LOG_xxx

    void*       pDumpPtr;   // The pointer to the dump storage for this CEVA
    
} DSP_CTX, *PDSP_CTX;

typedef struct _DSP_INTERFACE_
{
    UINT32       Dead;    // reserved words to monitor the DSP code start-up (also DSP code version when code is stared)
    UINT32       Beef;    // reserved words to monitor the DSP code start-up (Reserved for future)

    TCB *        CevaTcb [DEVID_MAX_DSP_CORE];    // The pointer to personal TCB used for each CEVA
    LPVOID       CevaDump[DEVID_MAX_DSP_CORE];    // The pointer to the personal storage for CEVA dumper 
    CEVA_DRV_CFG CevaDrvCfg;                      // The information provided by ARM processor for CEVAs to use personal drivers (or NULL)
    
}DSP_INTERFACE;

typedef struct tDSPDRV_CTX
{
    HANDLE          hDspHeap;
    UINT32          numDSP;
    BOOL            bInitialized;
    UINT32          DevEnaMask;
    UINT32          DevUpMask;
    DSP_CTX         DspCtx[DEVID_MAX_DSP_CORE];
    VUINT32         CpuID;                  // The CPU ID where driver was initialized
    DSP_INTERFACE*  pHeadTcb;
    UINT32          RexServID;              // The CEVA processor ID where REX-SERVER part was initialized 
    UINT32          RexServLoadErr;
} DSPDRV_CTX, *PDSPDRV_CTX;

typedef struct tDSPBOOTDESC {
    UINT32 next;	/* next pointer in external memory */
    UINT32 code;	/* use program or data memory DMA */
    UINT32 length;	/* section length in bytes */
    UINT32 internal;	/* internal load address for DMA */
    UINT32 external;	/* external load address for DMA */
    UINT32 pad[3];
} DSPBOOTDESC, *PDSPBOOTDESC;

/******************************************************************************
*                The DSP objects addresses                                    *
*******************************************************************************/

#define     TCB_BASE_ADDR               (0xFC000000)
#define     TCB_IN_ADDR                 ((TCB *)TCB_BASE_ADDR)
#define     DSP_CODE_ADDR               ((PUINT8)0xFC001000)
#define     DSP_CTRL_REGS               ((PDSP_CTRL_REGS)0xC0000000)

#define     DSP_CTRL_RUN_PROC           (0x1300)
#define     DSP_RUN_TIMEOUT             (150*1000*10)      // 10 ms
#define     DSP_BOOT_TIMEOUT            (150*1000*10)      // 10 ms
#define     DSP_ASYNC_PERIOD            (1)
#define     DSP_WAIT_PRIOR              (1)

#define     CEVA_IRQ                    HAL_IRQ_CEVA_0

#define     DSP_BOOT_DESC_ADDR         (CRAM_BASEADDR + 0x800)
#define     DSP_EXT_SEGMENTS           (CRAM_BASEADDR + 0x1000)
#define     DSP_INT_SEGMENTS           (CRAM_BASEADDR + 0x2000)

//CEVAs are using the memory 0xFC000000 to 0xFC000008 when they are starting.
//So the offset is 8


#define DSP_STARTID    1     //starting CEVA ID for Diagnostics used to bypass ceva_0 when running @ 500Mhz
/******************************************************************************
*                The API declaration                                          *
*******************************************************************************/
INLINE TCB* GetDspTCB(UINT32 dspID)
{
    return ((DSP_INTERFACE*)TCB_BASE_ADDR)->CevaTcb[dspID];
}

INLINE void SetDspTCB(UINT32 dspID, TCB * pTcb)
{
    ((DSP_INTERFACE*)TCB_BASE_ADDR)->CevaTcb[dspID] = pTcb;
}

INLINE void SetDspDump(UINT32 dspID, LPVOID ptr)
{
    ((DSP_INTERFACE*)TCB_BASE_ADDR)->CevaDump[dspID] = ptr;
}

INLINE void* GetDspDump(UINT32 dspID)
{
    return ((DSP_INTERFACE*)TCB_BASE_ADDR)->CevaDump[dspID];
}

void DspDrvIrqHandler(volatile UINT32 * pClear, UINT32 nIrqID, LPVOID pParam);
MXRC DspDrvInit(void);
MXRC DspDrvGetMasterCpu (void);
MXRC DspDrvShutdown (void);
MXRC DspDrvBist(UINT32 dspID);
MXRC DspDrvBistDmaUtil(UINT32 dspID, UINT32 ext_addr, UINT32 int_addr, UINT32 size);

void DspDrvSetAsynMode(UINT32 dspID);

void DSPDrvSetCallBack(UINT32 dspID, void (*cbReqComp)(void *p), void *pReqCompCtx);
MXRC DspDrvRun(UINT32 devID, TCB *pTCB);
MXRC DspDrvRunEx(UINT32 dspID, TCB *pTCB, UINT32 nMlog);
MXRC DspDrvRunBlocking(UINT32 dspID, TCB *pTCB);
MXRC DspDrvRunInPullMode(UINT32 dspID, TCB *pTCB);
UINT32 DspDrvBoot(PUINT8 pDspImage, UINT32 DspImageSize, UINT32 timeout);
//-------------------------------------------------------------------------------------------
/** @brief This function loads DSP program memory with a Ceva Binary Image
 *
 *  @param   nCevaID       the specific CEVA ID
 *  @param   pDspImage     Pointer to program to load
 *  @param   DspImageSize  Size of program to load
 *  @param   timeout       System Timeout interval (after which it will display load error)
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------
UINT32 DspDrvBootCeva(UINT32 nCevaID, PUINT8 pDspImage, UINT32 DspImageSize, UINT32 timeout);
UINT32 DspDrvGetDspHeapAddr(void);
UINT32 DspDrvGetDspHeapSize(UINT32 DspHeapAddr);

//-------------------------------------------------------------------------------------------
/** @brief This function dumps status of Ceva registers to specified memroy
 *
 *
 *  @param   nCevaID  CEVA ID
 *  @param   pOutBuf  Pointer to Output
 *  @param   pnSize   Pointer to size of Ouptut
 *
 *  @return  returns MX_OK
 *
 *  \ingroup group_lte_hal_dsp
 *
**/
//-------------------------------------------------------------------------------------------

MXRC DspDrvDump (UINT32 nCevaID, PUINT8 pOutBuf, PUINT32 pnSize);

/** @brief This function generates NMI to all cevas.

    @param nExcMask [in] - is used to exclude some CEVA from the dump pool, by default = 0
    @param pDoneMask[out]- the mask of cevas completed dump operation

    @return [MXRC] an error code 

    \ingroup group_lte_hal_dsp

    */

MXRC DspDrvGetDump (UINT32 nExcMask, PUINT32 pDoneMask);

/** @brief This function returns address of dump storage.

    @param nDspID [in] - ceva code id:{0...max_num}

    @return [LPVOID]  the dump storage

    \ingroup group_lte_hal_dsp

    */
LPVOID DspDrvGetDumpAddr(UINT32 nDspID);

/** @brief The function sets a log of CEVA

    @param nCevaID  [in] - CEVA ID
    @param pStorage [in] - Pointer to storage of log
    @param nSize    [in] - Size of storage

    @return [MXRC] an error code
    \ingroup group_lte_hal_dsp

    */
MXRC DspDrvSetLog(UINT32 nCevaID,  LPVOID pStorage, UINT32 nSize);

/** @brief This function closes the CEVA log

    @param nCevaID  [in] - CEVA ID

    @return [MXRC] an error code
    \ingroup group_lte_hal_dsp
    */
MXRC DspDrvCloseLog(UINT32 nCevaID);

/** @brief The function sets the mask of fields which add to the log

    @param nCevaID  [in] - CEVA ID
    @param mask     [in] - see defines DSP_LOG_XXX
    \ingroup group_lte_hal_dsp
    */
void  DspDrvSetLogMask(UINT32 nCevaID, UINT32 mask);

/** @brief The function adds the data to the log

    @param pDspCtx  [in] - Context of CEVA which is logging
    @param pTcb     [in] - Pointer to TCB which need to add to the log

    @return [MXRC] an error code
    \ingroup group_lte_hal_dsp
    */

MXRC DspDrvAddToLog(PDSP_CTX pDspCtx, PTCB pTcb);

/** @brief This function returns the mask of available CEVAs 
           if drivers is inited, the mask contains the devices with passed BISR test

    @return [UINT32] the mask of CEVAs */

UINT32 DspDrvGetDevMask(void);

MXRC DspDrvResetDSPAll(void);

/** @brief This function is designed to run BIST test for all detected CEVAs
           and planned to be used just for checking ceva pool

    @return [MXRC] an error code */

MXRC DspDrvCevaPoolBist (void);

/** @brief This function delivers TCB to the REX-SERVER side 

    @param pTcb [in] - the pointer to the REX-SERVER TCB
    @param bWait[in] - to wait completeness of the TCB

    @return [MXRC] an error code */

MXRC DspDrvSendToRexServ(TCB * pTcb, UINT32 bWait);

/** @brief This function sets CEVA index where REX-SERVER code will be located
          this function has to be called before DspDrvInit

    @param dsp_id [in] - the CEVA devive id:[0...9]

    @return [MXRC] an error code */

MXRC DspDrvSetRexServCeva(UINT32 dsp_id);

/** @brief This function sends interrupt to the CEVA processor 
           to start processing some task prepared by the REX-client

    @return [MXRC] an error code

*/
           
MXRC DspDrvNotifyRex(void);

UINT32 DspDrvGetRexCeva (void);

void DspDrvRegRexCb (void (*Proc)(void*), void * ptr);

CEVA_DRV_CFG * DspDrvGetCevaDrvCfgParams(void);

/** @brief This code tries to restore CEVA processors by reloading 
           CEVA code. 

    @param  [nBadMask] - the mask of processors to be reloaded
    @return [UINT32] - the mask of restored processors */
           
UINT32 DspDrvReinitCeva(UINT32 nBadMask);

void DspMmuCfg(UINT32 nCpuID, UINT32 nParam);

#endif /*_DSP_DRV_H_*/

#ifdef __cplusplus
}
#endif

