//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL gemdrv.h
 *
 * @brief The Cadence GEM driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.11 $
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

#ifndef _GEM_DRV_H_
#define _GEM_DRV_H_
#include "config.h"
#include "4gmx.h"
#include <string.h>
#include "appids.h"
#include "hal.h"
#include "gemdrvtypes.h"

/***********************************************************************
**               GEM driver configuration                              *
************************************************************************/
#define GEM_DRV_STATISTICS_ENABLE
#define GEM_IRQ_PUSH_ENABLE
#undef  GEM_LOCKS_ENABLE

#define GEM_DEFAULT_TX_OPT              0x0     // See: GEM_DMA_TX_xxx macros

#define GEM_TYPES_NUM                   8
#define GEM_MACS_NUM                    4
#define GMAC_MDC_DIVIDER 				3


/***********************************************************************
**  The error codes of module, it's used by module interface functions *
************************************************************************/
#define GEM_RC_OK                       MX_OK
#define GEM_RC_PARAM_ERROR              DEF_USRRC(APP_DRIVER, GEM_DRV, 1)
#define GEM_RC_NOT_MEMORY               DEF_USRRC(APP_DRIVER, GEM_DRV, 2)
#define GEM_RC_UNKNOWN_SPEED            DEF_USRRC(APP_DRIVER, GEM_DRV, 3)
#define GEM_RC_UNKNOWN_PHY              DEF_USRRC(APP_DRIVER, GEM_DRV, 4)
#define GEM_RC_DMA_IS_NOT_CONFIG        DEF_USRRC(APP_DRIVER, GEM_DRV, 5)
#define GEM_RC_RX_EMPTY                 DEF_USRRC(APP_DRIVER, GEM_DRV, 6)
#define GEM_RC_FRM_IS_NOT_USED          DEF_USRRC(APP_DRIVER, GEM_DRV, 7)
#define GEM_RC_FRM_IS_NOT_SOF           DEF_USRRC(APP_DRIVER, GEM_DRV, 8)
#define GEM_RC_DMA_TX_IS_NOT_INIT       DEF_USRRC(APP_DRIVER, GEM_DRV, 9)
#define GEM_RC_TX_MISTIMING             DEF_USRRC(APP_DRIVER, GEM_DRV, 10)
#define GEM_RC_TX_OVERLOAD              DEF_USRRC(APP_DRIVER, GEM_DRV, 11)
#define GEM_RC_TX_ERROR                 DEF_USRRC(APP_DRIVER, GEM_DRV, 12)
#define GEM_RC_RX_ERROR                 DEF_USRRC(APP_DRIVER, GEM_DRV, 14)
#define GEM_RC_TX_DISABLED              DEF_USRRC(APP_DRIVER, GEM_DRV, 15)
#define GEM_RC_COLLISION                DEF_USRRC(APP_DRIVER, GEM_DRV, 16)
#define GEM_RC_RETRY_EXCEEDED           DEF_USRRC(APP_DRIVER, GEM_DRV, 17)
#define GEM_RC_AHB_ERR                  DEF_USRRC(APP_DRIVER, GEM_DRV, 18)
#define GEM_RC_UNDERRUN                 DEF_USRRC(APP_DRIVER, GEM_DRV, 19)
#define GEM_RC_LATE_COLLISION           DEF_USRRC(APP_DRIVER, GEM_DRV, 20)
#define GEM_RC_HRESP_ERR                DEF_USRRC(APP_DRIVER, GEM_DRV, 21)
#define GEM_RC_MAC_ID_ERROR             DEF_USRRC(APP_DRIVER, GEM_DRV, 22)
#define GEM_RC_PHY_RW_TO             	DEF_USRRC(APP_DRIVER, GEM_DRV, 23)


/******************************************************************************
*       Definition of the GEM context type                                    *
*******************************************************************************/

#define GEMCTX(x)       ((GemCtx*)(x))

typedef void (*GemDevIrqCB)(LPVOID pCBCtx, LPVOID pGemDrvCtx, UINT32 nIrqStatus);

/** @brief GEM statistics */

#ifdef GEM_DRV_STATISTICS_ENABLE

typedef struct tagGemDrvStat
{
    // -----------------------------------------------
    // TX statistics  (TxS)
    // -----------------------------------------------
    VUINT32     TxSOctetsLow;       // Octets transmitted [31:0] (in frames without error)
    VUINT32     TxSOctetsHigh;      // Octets transmitted [47:32] (in frames without error)
    VUINT32     TxSFrmNum;          // Frames transmitted without error, excluding pause frames
    VUINT32     TxSBrdcastNum;      // Broadcast frames transmitted without error, excluding pause frames
    VUINT32     TxSMulcastNum;      // Multicast frames transmitted without error,excluding pause frames
    VUINT32     TxSPauseNum;        // Transmitted pause frames (unless sent through external FIFO interface)
    VUINT32     TxSFrm_64Num;       // 64 byte frames transmitted without error, excluding pause frames
    VUINT32     TxSFrm_65_127Num;   // 65 to 127 byte frames transmitted without error
    VUINT32     TxSFrm_128_255Num;  // 128 to 255 byte frames transmitted without error
    VUINT32     TxSFrm_256_511Num;  // 256 to 511 byte frames transmitted without error
    VUINT32     TxSFrm_512_1023Num; // 512 to 1023 byte frames transmitted without error
    VUINT32     TxSFrm_1024_1518Num;// 1024 to 1518 byte frames transmitted without error
    VUINT32     TxSFrm_over1518Num; // Greater than 1518 byte frames transmitted without error
    VUINT32     TxSUndrNum;         // Transmit under run errors
    VUINT32     TxSSingleCollNum;   // Single collision frames
    VUINT32     TxSMulCollNum;      // Multiple collision frames
    VUINT32     TxSExcCollNum;      // Excessive collisions
    VUINT32     TxSLateCollNum;     // Late collisions
    VUINT32     TxSDeferredNum;     // Deferred transmission frames
    VUINT32     TxSCrSenseNum;      // Carrier sense errors

    // -----------------------------------------------
    // RX statistics  (RxS)
    // -----------------------------------------------
    VUINT32     RxSOctetsLow;       // Octets received [31:0] (in frames without error)
    VUINT32     RxSOctetsHigh;      // Octets received [47:32] (in frames without error)
    VUINT32     RxSFrmNum;          // Frames received without error, excluding pause frames
    VUINT32     RxSBrdcastNum;      // Broadcast frames received without error, excluding pause frames
    VUINT32     RxSMulcastNum;      // Multicast frames received without error,excluding pause frames
    VUINT32     RxSPauseNum;        // received pause frames (unless sent through external FIFO interface)
    VUINT32     RxSFrm_64Num;       // 64 byte frames transmitted without error, excluding pause frames
    VUINT32     RxSFrm_65_127Num;   // 65 to 127 byte frames received without error
    VUINT32     RxSFrm_128_255Num;  // 128 to 255 byte frames received without error
    VUINT32     RxSFrm_256_511Num;  // 256 to 511 byte frames received without error
    VUINT32     RxSFrm_512_1023Num; // 512 to 1023 byte frames received without error
    VUINT32     RxSFrm_1024_1518Num;// 1024 to 1518 byte frames received without error
    VUINT32     RxSFrm_over1518Num; // Greater than 1518 byte frames received without error
    VUINT32     RxSUndersizeNum;    // Undersize frames received
    VUINT32     RxSOversizeNum;     // Oversize frames received
    VUINT32     RxSJabbersNum;      // Jabbers received
    VUINT32     RxSFrmChkSeqErr;    // Frame check sequence errors
    VUINT32     RxSLenErr;          // Length field frame errors
    VUINT32     RxSRecvSymErr;      // Receive symbol errors
    VUINT32     RxSAlignErr;        // Alignment errors
    VUINT32     RxSRecvResErr;      // Receive resource errors
    VUINT32     RxSRecvOvrErr;      // Receive overrun errors
    VUINT32     RxSIPChkSumErr;     // IP header checksum errors
    VUINT32     RxSTCPChkSumErr;    // TCP checksum errors
    VUINT32     RxSUDPChkSumErr;    // UDP checksum errors

}GemDrvStat;

#endif // GEM_DRV_STATISTICS_ENABLE


/** @brief The GEM context definition,
            it contains the driver's needed parameters */

typedef struct tagGemCtx
{
    UINT32          MasterCpu;
    UINT32          Inited;
    UINT32          nDevID;

    GEMRegs     *   IPRegs;
    GEMCoreRegs *   CoreRegs;

#ifdef GEM_LOCKS_ENABLE
    MXSYNC          Sync;       // The pointer to the sync. object, it can be null
#endif

    UINT32          RxD;        // RX buffers descriptors list
    UINT32          TxD;        // TX buffers descriptors list

    UINT32          CurRxDP;    // current value of the RX buffers descriptors list pointer
    UINT32          CurTxDP;    // current value of the TX buffers descriptors list pointer
    UINT32          FreeTxDP;   // the possition of the next free TX buffer descriptor

    GemDevIrqCB     IrqProc;
    LPVOID          ProcCtx;
    UINT32          IrqMask;

#ifdef GEM_IRQ_PUSH_ENABLE
    GemDevIrqCB     SavedIrqProc;
    LPVOID          SavedProcCtx;
    UINT32          SavedIrqMask;
#endif

#ifdef GEM_DRV_STATISTICS_ENABLE
    GemDrvStat      Stat;       // GEM statistics
#endif

}GemCtx;

/** @brief The list of supported by the Cadence GEM PHY interfaces */

typedef enum
{
    RGMII       =   0x0,
    SGMII       =   0x4,    // 100:SGMII

}MACPhyInt;

/** @brief The list of supported by the Cadence GEM PHY speeds */

typedef enum
{
    MAC_SPEED_100Mbps   =   1,
    MAC_SPEED_1000Mbps  =   2

}MACSpeed;

typedef struct tagGemCfg
{
    MACPhyInt       Phy;            // PHY interface
    MACSpeed        Speed;          // MAC Speed

    UINT8           MacAddr [6];    // MAC address

}GemCfg;

typedef struct tagGemDMACfg
{
    HGTxFrm     TxDescr;        // The pointer to the array of TX buffer descriptors
    HGRxFrm     RxDescr;        // The pointer to the array of RX buffer descriptors

    UINT32      TxDescrCount;   // Number of TX buffer descriptors
    UINT32      RxDescrCount;   // Number of RX buffer descriptors

    UINT32      DMAConfig;      // See: GEM_DMA_xxx macros

    UINT32      RxBufOpt;       // The additional bits for the
    LPVOID      RxBuffers;      // The pointer to the global RX, this buffer is used by the RX buffer descripts

}GemDMACfg;

/******************************************************************************
*                     The extended driver interface                           *
*******************************************************************************/

#ifdef GEM_LOCKS_ENABLE

    INLINE void GemDrvLock (LPVOID pCtx)
    {
        if (GEMCTX(pCtx)->Sync != NULL)
            MxGetLock (GEMCTX(pCtx)->Sync);
    }

    INLINE void GemDrvUnlock (LPVOID pCtx)
    {
        if (GEMCTX(pCtx)->Sync != NULL)
            MxReleaseLock (GEMCTX(pCtx)->Sync);
    }

#else //#ifdef GEM_LOCKS_ENABLE

    INLINE void GemDrvLock (LPVOID pCtx){}

    INLINE void GemDrvUnlock (LPVOID pCtx){}

#endif //#ifdef GEM_LOCKS_ENABLE


INLINE UINT32 GemDrvGetNetCtrl(LPVOID pCtx)
{
    _ASSERT_PTR(pCtx);

    return GEMCTX(pCtx)->IPRegs->NetCtrl;
}

INLINE UINT32 GemDrvGetNetCfg(LPVOID pCtx)
{
    _ASSERT_PTR(pCtx);

    return GEMCTX(pCtx)->IPRegs->NetCfg;
}


INLINE MXRC GemDrvSetNetCtrl(LPVOID pCtx, UINT32 nVal)
{
    _ASSERT_PTR(pCtx);

    GEMCTX(pCtx)->IPRegs->NetCtrl = nVal;

    return GEM_RC_OK;
}

INLINE MXRC GemDrvSetNetCfg(LPVOID pCtx, UINT32 nVal)
{
    _ASSERT_PTR(pCtx);

    GEMCTX(pCtx)->IPRegs->NetCfg = nVal;

    return GEM_RC_OK;
}

INLINE MXRC GemDrvEnableRX (LPVOID pCtx)
{
	_ASSERT_PTR(pCtx);

    GEMCTX(pCtx)->IPRegs->NetCtrl |= GEM_NETCTRL_RX_ENB;

    return GEM_RC_OK;
}

INLINE MXRC GemDrvDisableRX (LPVOID pCtx)
{
    _ASSERT_PTR(pCtx);

    GEMCTX(pCtx)->IPRegs->NetCtrl &= ~GEM_NETCTRL_RX_ENB;

    return GEM_RC_OK;
}

INLINE MXRC GemDrvEnableTX (LPVOID pCtx)
{
    _ASSERT_PTR(pCtx);

    GEMCTX(pCtx)->IPRegs->NetCtrl |= GEM_NETCTRL_TX_ENB;

    return GEM_RC_OK;
}

INLINE MXRC GemDrvDisableTX (LPVOID pCtx)
{
    HGTxFrm pFrame;

    _ASSERT_PTR(pCtx);

    GemDrvLock (pCtx);

    GEMCTX(pCtx)->IPRegs->NetCtrl &= ~GEM_NETCTRL_TX_ENB;

    // to wait of TX ending

    while (GEMCTX(pCtx)->IPRegs->TxStatus & GEM_TX_STAT_GO)
    {
        CPU_IDLE();
    }

    GEMCTX(pCtx)->CurTxDP  = GEMCTX(pCtx)->TxD;
    GEMCTX(pCtx)->FreeTxDP = GEMCTX(pCtx)->TxD;

    pFrame = (HGTxFrm)GEMCTX(pCtx)->TxD;

    // to reset all TX buffer descriptors

    while (pFrame != NULL)
    {
        GEM_DMA_TX_SET_USED(pFrame);

        pFrame = GEM_DMA_TX_IS_WRAP(pFrame) ? NULL : (pFrame + 1);
    }

    GemDrvUnlock (pCtx);

    return GEM_RC_OK;
}

INLINE MXRC GemDrvSetLB (LPVOID pCtx, BOOL bLoopBack)
{
    _ASSERT_PTR(pCtx);

    if (bLoopBack)
        GEMCTX(pCtx)->IPRegs->NetCtrl |= GEM_NETCTRL_RX_LB;
    else
        GEMCTX(pCtx)->IPRegs->NetCtrl &= ~GEM_NETCTRL_RX_LB;

    return GEM_RC_OK;
}

INLINE MXRC GemDrvCopyAll (LPVOID pCtx, BOOL bEnable)
{
    _ASSERT_PTR(pCtx);

    if (bEnable)
        GEMCTX(pCtx)->IPRegs->NetCfg |= GEM_NETCFG_COPY_ALL;
    else
        GEMCTX(pCtx)->IPRegs->NetCfg &= ~GEM_NETCFG_COPY_ALL;

    return GEM_RC_OK;
}


INLINE MXRC GemDrvEnBroadcast (LPVOID pCtx, BOOL bEnable)
{
    _ASSERT_PTR(pCtx);

    if (bEnable)
        GEMCTX(pCtx)->IPRegs->NetCfg &= ~GEM_NETCFG_NO_BROADCAST;
    else
        GEMCTX(pCtx)->IPRegs->NetCfg |= GEM_NETCFG_NO_BROADCAST;

    return GEM_RC_OK;
}

INLINE MXRC GemDrvEnMulticast (LPVOID pCtx, BOOL bEnable)
{
    _ASSERT_PTR(pCtx);

    if (bEnable)
        GEMCTX(pCtx)->IPRegs->NetCfg |= GEM_NETCFG_MULTICAST_ENB;
    else
        GEMCTX(pCtx)->IPRegs->NetCfg &= ~GEM_NETCFG_MULTICAST_ENB;

    return GEM_RC_OK;
}

INLINE MXRC GemDrvEnUnicast (LPVOID pCtx, BOOL bEnable)
{
    _ASSERT_PTR(pCtx);

    if (bEnable)
        GEMCTX(pCtx)->IPRegs->NetCfg |= GEM_NETCFG_UNICAST_ENB;
    else
        GEMCTX(pCtx)->IPRegs->NetCfg &= ~GEM_NETCFG_UNICAST_ENB;

    return GEM_RC_OK;
}

INLINE UINT32 GemDrvGetTxStatus (LPVOID pCtx)
{
    _ASSERT_PTR(pCtx);

    return GEMCTX(pCtx)->IPRegs->TxStatus;
}

INLINE void GemDrvSetTxStatus (LPVOID pCtx, UINT32 val)
{
    _ASSERT_PTR(pCtx);

    GEMCTX(pCtx)->IPRegs->TxStatus = val;
}


INLINE UINT32 GemDrvGetRxStatus (LPVOID pCtx)
{
    _ASSERT_PTR(pCtx);

    return GEMCTX(pCtx)->IPRegs->RxStatus;
}

INLINE void GemDrvSetRxStatus (LPVOID pCtx, UINT32 val)
{
    _ASSERT_PTR(pCtx);

    GEMCTX(pCtx)->IPRegs->RxStatus = val;
}

#ifdef GEM_IRQ_PUSH_ENABLE

    INLINE void GemDrvPushIrqProc (LPVOID pCtx)
    {
        IRQTYPE irq;

        _ASSERT_PTR(pCtx);

        irq = ARM_INT_disable();

        GemDrvLock (pCtx);

        GEMCTX(pCtx)->SavedIrqProc = GEMCTX(pCtx)->IrqProc;
        GEMCTX(pCtx)->SavedProcCtx = GEMCTX(pCtx)->ProcCtx;
        GEMCTX(pCtx)->SavedIrqMask = GEMCTX(pCtx)->IrqMask;

        GemDrvUnlock (pCtx);

        ARM_INT_restore(irq);
    }

    INLINE void GemDrvPopIrqProc (LPVOID pCtx)
    {
        IRQTYPE irq;

        _ASSERT_PTR(pCtx);

        irq = ARM_INT_disable();

        GemDrvLock (pCtx);

        GEMCTX(pCtx)->IrqProc = GEMCTX(pCtx)->SavedIrqProc;
        GEMCTX(pCtx)->ProcCtx = GEMCTX(pCtx)->SavedProcCtx;
        GEMCTX(pCtx)->IrqMask = GEMCTX(pCtx)->SavedIrqMask;

        GEMCTX(pCtx)->IPRegs->IntDisable = 0xFFFFFFFF;
        GEMCTX(pCtx)->IPRegs->IntEnable  = (GEMCTX(pCtx)->IrqMask);

        GemDrvUnlock (pCtx);

        ARM_INT_restore(irq);
    }

#endif // GEM_IRQ_PUSH_ENABLE

INLINE void GemDrvSetIrqProc (LPVOID pCtx, GemDevIrqCB proc, LPVOID procCtx, UINT32 nIrqMask)
{
    IRQTYPE irq;

    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable();

    GemDrvLock (pCtx);

    //GEMCTX(pCtx)->IPRegs->IntDisable = 0xFFFFFFFF;
    GEMCTX(pCtx)->IPRegs->IntDisable = 0x0;
    GEMCTX(pCtx)->IPRegs->IntEnable  = (nIrqMask);

    GEMCTX(pCtx)->IrqProc = proc;
    GEMCTX(pCtx)->ProcCtx = procCtx;
    GEMCTX(pCtx)->IrqMask = nIrqMask;

    GemDrvUnlock (pCtx);

    ARM_INT_restore(irq);
}

#ifdef GEM_DRV_STATISTICS_ENABLE

    INLINE void GemDrvClrStat (LPVOID pCtx)
    {
        _ASSERT_PTR(pCtx);

        memset (&GEMCTX(pCtx)->Stat, 0, sizeof (GEMCTX(pCtx)->Stat));

        GEMCTX(pCtx)->IPRegs->NetCtrl |= GEM_NETCTRL_CLR_STAT;
    }

    /** @brief This function returns GEM statistics

        @param pCtx [in]    -   the driver context
        @param pStat[out]   -   a storage for statictics

        @return [MXRC] an error code */

    MXRC GemDrvGetStat (LPVOID pCtx, GemDrvStat * pStat);

#endif //GEM_DRV_STATISTICS_ENABLE


/** @brief The function returns the size of DMA RX buffer

    @param pDrvCtx [in] - the driver context

    @return [UINT32] the size of DMA RX buffer */

INLINE UINT32 GemDrvGetDMARxSize (LPVOID pDrvCtx)
{
    _ASSERT_PTR(pDrvCtx);

    return GEM_DMA_GET_RX_SIZE(GEMCTX(pDrvCtx)->IPRegs->DMACfg);
}

INLINE LPVOID GemDrvGetRxBuf (HGRxFrm frm)
{
    return (LPVOID)(frm->AddrOpt);
}


/** @brief This function initializes GEM and should be called first before
           usage of drivers functions

    @param pDrvCtx [in] - the driver context
    @param pCfg    [in] - parameters

    @return [MXRC] an error code */

MXRC GemDrvCfg (LPVOID pDrvCtx, GemCfg * pCfg);

/** @brief This function sets source MAC address of GEM, it's used
           like a pattern for comparation

    @param pDrvCtx [in] - the driver context
    @param pMAC    [in] - the array of MAC address

    @return [MXRC] an error code */

MXRC GemDrvSetMAC (LPVOID pDrvCtx, UINT8 * pMAC);

/** @brief This function sets a source MAC address of GEM, it's used
           like a pattern for comparation

    @param pDrvCtx [in] - the driver context
    @param pMAC    [in] - the array of MAC address

    @return [MXRC] an error code */

MXRC GemDrvSetMACEx (LPVOID pDrvCtx, UINT32 ID, UINT8 * pMAC);

/** @brief This function enables/disable the unicast/multicast hash bit

    @param pDrvCtx [in] - the driver context
    @param nHashID [in] - the bit index [0...63]
    @param nEnable [in] - enable/disable bit

    @return [MXRC] error code */

MXRC GemDrvHashCtrl (LPVOID pDrvCtx, UINT32 nHashID, UINT32 nEnable);

/** @brief This function returns an index of the hash bit based on the provided MAC

    @param pMac [in] - the MAC address

    @return [UINT32] an index */

UINT32 GemDrvGetMacHash (PUINT8 pMac);

/** @brief This function sets TypeID, it's used
           as a pattern to detect is it "possible" or "not possible" to copy this frame

    @param pDrvCtx [in] - the driver context
    @param nTypeID [in] - the TypeID
    @param nTypeVal[in] - the Type value

    @return [MXRC] an error code */

MXRC GemDrvSetType (LPVOID pDrvCtx, UINT32 nTypeID, UINT32 nTypeVal);

/** @brief This function resets TypeID, this filter will be disabled

    @param pDrvCtx [in] - the driver context
    @param nTypeID [in] - the TypeID
    @param nTypeVal[in] - the Type value

    @return [MXRC] an error code */

MXRC GemDrvResetType (LPVOID pDrvCtx, UINT32 nTypeID);

/** @brief This function configures Cadence GEM DMA engine

    @param pDrvCtx [in] - the pointer to the driver context
    @param pCfg    [in] - the pointer to DMA configuration parameters

    @return [MXRC] an error code */

MXRC GemDrvConfigDMA (LPVOID pDrvCtx, GemDMACfg * pCfg);

/** @brief This function resets TX and RX pointers of GEM DMA and
           re-initializes TX and RX buffer descriptors.
           In this way all possible received frames will be discarded

    @param pDrvCtx [in] - the driver context

    @return [MXRC] error code of operation */

MXRC GemDrvResetDMA (LPVOID pDrvCtx);

/** @brief The function schedules and sends a frame.

    @NOTE: This function may be used conjointly with GemDrvSchTxFrm
           function for packets sending in a burst mode

    @param pDrvCtx  [in] - the driver context
    @param nTxFrmOpt[in] - see: GEM_DMA_TX_xxx macros
    @param pBuf     [in] - the pointer to the buffer (it can be NULL)
    @param nSize    [in] - the size of buffer (should be > 0 if pBuf != NULL )

    @return [MXRC] the error code of sending */

MXRC GemDrvTxFrm (LPVOID pDrvCtx, UINT32 nTxFrmOpt, LPVOID pHBuf, UINT32 nHSize, LPVOID pPBuf, UINT32 nPSize);



/** @brief It returns the pointer to the current TX frame (to the first buffer)

    @Note: If TX is active, the function returns a pointer to the first frame that
           was used for TX operation, in other case it will return a pointer to the
           first frame that will be used for TX

    @param pDrvCtx [in] - the driver context

    @return [HGTxFrm] */

HGTxFrm GemDrvGetFirstTxFrm (LPVOID pDrvCtx);

/** @brief This function removes a frame from the TX buffer descriptors list

    @NOTE: It can be useful to discard TX frame with error of sending

    @Warning: The TX operation will be halted!!!

    @param pDrvCtx [in] - the driver context
    @param pTxFrame[in] - the TX frame descriptor

    @return [MXRC] an error code */

MXRC GemDrvRemTxFrm (LPVOID pDrvCtx, HGTxFrm pTxFrame);

/** @breif This function schedules a packet for sending but does not send packet
           immediately, TX operation should be started by GemDrvTxFrm function.

    @param pDrvCtx [in] - the driver context
    @param nOpt    [in] - additional options
    @param pBuf    [in] - the pointer to buffer
    @param nSize   [in] - the size of buffer

    @return [MXRC] the error code of packet scheduling */

MXRC GemDrvSchTxFrm (LPVOID pDrvCtx, UINT32 nOpt, LPVOID pHBuf, UINT32 nHSize, LPVOID pPBuf, UINT32 nPSize);



/** @brief This function allocates a TX frame and sets frame parameters.
           The function can be used conjunctly with GemDrvAddTxBuf function
           to have a possibility to specify more than one buffer per frame.

    @param pDrvCtx [in] - the driver context
    @param opt     [in] - the additional TX options (see: GEM_DMA_TX_xxx bits)
    @param pBuf    [in] - the first buffer of frame
    @param nSize   [in] - the size of buffer

    @return [HGTxFrm] the allocated frame or NULL */

HGTxFrm GemDrvAllocTxFrm (LPVOID pDrvCtx, UINT32 opt, LPVOID pBuf, UINT32 nSize);

/** @brief This functon adds an additional buffer to the allocated frame,
           it should be used conjunctly with the GemDrvAllocTxFrm function

    @param pDrvCtx [in] - the driver context
    @param pTxFrame[in] - the allocated frame
    @param pBuf    [in] - the additional buffer for frame
    @param nSize   [in] - the size of buffer

    @return [MXRC] an error code */

MXRC GemDrvAddTxBuf (LPVOID pDrvCtx, HGTxFrm pTxFrame, LPVOID pBuf, UINT32 nSize);

/** @brief The function returns a descriptor of the received frame, this
           descriptor can be used for getting frame buffer(s),
           various status and for free frame operation

    @param pDrvCtx  [in] - the driver context
    @param pRxFrame [out]- the frame descriptor or NULL
    @param pnBufNum [out]- the number of buffers per received frame (the size of one buffer is DMA configured)
    @param pnFrmSize[out]- the total size of received frame in bytes

    @return [MXRC] an error code */

MXRC GemDrvRxFrm (LPVOID pDrvCtx, HGRxFrm * pRxFrame, UINT32 * pnBufNum, UINT32 * pnFrmSize);

/** @brief The function returns a pointer to the frame's buffer (one of)

    @param pDrvCtx [in] - the driver context
    @param pFrame  [in] - the frame descriptor
    @param nIdx    [in] - the index of buffer (see: GemDrvRxFrm function)

    @return [LPVOID] a pointer to the frame buffer */

LPVOID GemDrvGetRxFrmBuf (LPVOID pDrvCtx, HGRxFrm pFrame, UINT32 nIdx);

/** @brief This function frees a received frame (it marks buffer descriptors are not used)

    @param pDrvCtx [in] - the driver context
    @param pFrame  [in] - the frame descriptor (the first buffer descriptor of the frame)

    @return [MXRC] the error code of operation */

MXRC GemDrvFreeRxFrm (LPVOID pDrvCtx, HGRxFrm pFrame);

/** @brief This function returns a pointer to the driver context and provides
           a possibility to use extended driver interface

    @param nDevID [in] - the device ID [0, 1]

    @return [LPVOID] a pointer to the driver interface */

LPVOID GemDrvGetCtx (UINT32 nDevID);

UINT16 GemDrvReadPhy (LPVOID pDrvCtx, UINT32 nPhyID, UINT32 nRegID);

MXRC GemDrvWritePhy (LPVOID pDrvCtx, UINT32 nPhyID, UINT32 nRegID, UINT16 nData);

/** @brief This function is called to stop GEM driver and
           disable GEM HW block to send and receive packets

    @param nDevID [in] - the device ID

    @return [MXRC] an error code */

MXRC GemDrvShutdown (UINT32 nDevID);

/** @brief This function initializes GEM driver and should be called first
           before usage of any driver API, the next functions are:

             * GemDrvCfg        - to configure GEM driver
             * GemDrvConfigDMA  - to configure GEM HW DMA block

    @param nBaseAddr[in] - the GEM HW base address (see: GEMAC0_BASEADDR and GEMAC1_BASEADDR)

    @return [MXRC] an error code */

MXRC GemDrvInit (UINT32 nBaseAddr);

void GemDrvAckIrq (UINT32 irq);

/******************************************************************************
*                     The standard driver interface                           *
*******************************************************************************/

MXRC GemDrv_init(UINT32 nInit, LPVOID * pCtx);

MXRC GemDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData, UINT32 nCfgDataSize);

MXRC GemDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData, UINT32 nCmdDataSize);

MXRC GemDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData, UINT32 * pnCmdDataSize);

BOOL GemDrvIsLinkUp(GemCtx * pCtx);

void GemDrvMmuCfg(UINT32 nCpuID, UINT32 nParam);

#endif /*_GEM_DRV_H_*/


#ifdef __cplusplus
}
#endif

