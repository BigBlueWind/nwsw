//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL sriodrv.h
 *
 * @brief SRIO driver (driver types/API declaration)
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
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

#ifndef _SRIO_DRV_H_
#define _SRIO_DRV_H_

#include "config.h"
#include "4gmx.h"
#include "appids.h"
#include "mdmadrv.h"

#ifdef SRIO_DRV_ENABLED

#define RC_SRIODRV_OK                   MX_OK
#define RC_SRIODRV_PARAM_ERROR          DEF_USRRC(APP_DRIVER, SRIO_DRV, 1)
#define RC_SRIODRV_SMALL_TX_BUF         DEF_USRRC(APP_DRIVER, SRIO_DRV, 2)
#define RC_SRIODRV_SMALL_RX_BUF         DEF_USRRC(APP_DRIVER, SRIO_DRV, 3)
#define RC_SRIODRV_RX_BUF_ERROR         DEF_USRRC(APP_DRIVER, SRIO_DRV, 4)
#define RC_SRIODRV_SUB_RX_BUF_ERROR     DEF_USRRC(APP_DRIVER, SRIO_DRV, 5)
#define RC_SRIODRV_RX_ERROR             DEF_USRRC(APP_DRIVER, SRIO_DRV, 6)
#define RC_SRIODRV_SEND_ERROR           DEF_USRRC(APP_DRIVER, SRIO_DRV, 7)
#define RC_SRIODRV_SEND_TIMEOUT         DEF_USRRC(APP_DRIVER, SRIO_DRV, 8)
#define RC_SRIODRV_AXI_ERROR            DEF_USRRC(APP_DRIVER, SRIO_DRV, 9)
#define RC_SRIODRV_RIO_ERROR            DEF_USRRC(APP_DRIVER, SRIO_DRV, 9)
#define RC_SRIODRV_FETCH_ERROR          DEF_USRRC(APP_DRIVER, SRIO_DRV, 9)
#define RC_SRIODRV_UPDATE_ERROR         DEF_USRRC(APP_DRIVER, SRIO_DRV, 9)
#define RC_SRIODRV_TX_BUSY              DEF_USRRC(APP_DRIVER, SRIO_DRV, 10)
#define RC_SRIODRV_TX_EMPTY             DEF_USRRC(APP_DRIVER, SRIO_DRV, 11)
#define RC_SRIODRV_CTX_ERROR            DEF_USRRC(APP_DRIVER, SRIO_DRV, 12)
#define RC_SRIODRV_ALLOC_ERROR          DEF_USRRC(APP_DRIVER, SRIO_DRV, 14)
#define RC_SRIODRV_SERDES_TIMEOUT       DEF_USRRC(APP_DRIVER, SRIO_DRV, 15)
#define RC_SRIODRV_NO_LINK              DEF_USRRC(APP_DRIVER, SRIO_DRV, 16)
#define RC_SRIODRV_RECV_TIMEOUT         DEF_USRRC(APP_DRIVER, SRIO_DRV, 17)

/*************************************************************/
/** These constants are used in DI to control LOG operations */
/*************************************************************/
#define SRIO_CTRL_TX_LOG                (1 << 0)    // To enable TX log
#define SRIO_CTRL_RX_LOG                (1 << 1)    // To enable RX log
#define SRIO_CTRL_TX_LOG_RR             (1 << 2)    // TX log is in round-robin mode
#define SRIO_CTRL_RX_LOG_RR             (1 << 3)    // RX log is in round-robin mode


#define SRIO_INT_STAT_AXI_PIO           (1 << 0)
#define SRIO_INT_STAT_RIO_PIO           (1 << 1)
#define SRIO_INT_STAT_WDMA              (1 << 2)
#define SRIO_INT_STAT_RDMA              (1 << 3)
#define SRIO_INT_STAT_IDME              (1 << 4)
#define SRIO_INT_STAT_ODME              (1 << 5)
#define SRIO_INT_STAT_MISC              (1 << 6)

// ==============================================================
// There are control and status bits of SRIO DMA descriptor
// ==============================================================
#define SRIO_DMA_CTRL_VALID             (1 << 0)
#define SRIO_DMA_CTRL_NEXT_OK           (1 << 1)
#define SRIO_DMA_STAT_DONE              (1 << 24)
#define SRIO_DMA_STAT_AXI_ERR           (1 << 25)
#define SRIO_DMA_STAT_RIO_ERR           (1 << 26)
#define SRIO_DMA_STAT_FETCH_ERR         (1 << 27)
#define SRIO_DMA_STAT_UPDATE_ERR        (1 << 28)

#define SRIO_DMA_STAT_ERR_MASK      (                               \
                                    SRIO_DMA_STAT_AXI_ERR   |       \
                                    SRIO_DMA_STAT_RIO_ERR   |       \
                                    SRIO_DMA_STAT_FETCH_ERR |       \
                                    SRIO_DMA_STAT_UPDATE_ERR        \
                                    )

#define SRIO_DMA_DEF_LEN(x)         (((x) >> 3) << 5)
#define SRIO_DMA_GET_LEN(x)         (((x) >> 5) & 0x1FFFF)
#define SRIO_DMA_SET_SRC_ADDR(x)    ((UINT32)(x) >> 3)
#define SRIO_DMA_GET_SRC_ADDR(dma)  ((dma)->SrcAddr << 3)
#define SRIO_DMA_SET_DST_ADDR(x)    ((UINT32)(x) >> 3)
#define SRIO_DMA_GET_DST_ADDR(dma)  ((dma)->DstAddr << 3)
#define SRIO_DMA_SET_NEXT_ADDR(x)   ((UINT32)(x) >> 3)
#define SRIO_DMA_GET_NEXT_ADDR(dma) ((dma)->Next << 3)

#define SRIO_DESC_SIZE(x)           (sizeof (SDMAD) * (x))
#define SRIO_BUF_SIZE(x)            ( ROUND((x), 8) )


#define SRIO_LB_OFF                 (0) // no loopback, the data is forwarded directly to the SerDes HW block
#define SRIO_LB_INTERNAL            (1) // The LB is enabled SRIO HW inside
#define SRIO_LB_PCS                 (2) // The LB is enabled in Serdes HW inside
#define SRIO_LB_PREDRIVER           (3) // Serdes pre-driver loopback

#define SRIO_LANES_X4               0   // default x4 mode
#define SRIO_LANES_X1               1   // Serdes is configured to use x1 lane
#define SRIO_LANES_X2               2   // Serdes is configured to use x2 lane

#define SRIO_CTRL_INITED            (1 << 0)

/** @brief SRIO DMA descriptor definition */

typedef struct tagSRIODescr
{
    VUINT32 CtrlStat;   // See:  SRIO_DMA_CTRL_xxx and SRIO_DMA_STAT_xxx
    VUINT32 SrcAddr;
    VUINT32 DstAddr;
    VUINT32 Next;

    VUINT32 Res[4];     // Alignment on 32 bytes

}SDMAD, *PSDMAD;

typedef enum
{
    SRIO_CMD_WRITE      = 1,
    SRIO_CMD_READ       = 2,
    SRIO_CMD_CFG        = 3,

}SrioDrvCmd;

typedef void (*SrioRxCb) (LPVOID pUsrData, PUINT8 pData, UINT32 nDataSize);

typedef struct
{
    UINT32      Ctrl;
    UINT32      nID;      // The device id assigned to this context
    VUINT32     Lock;
    IRQTYPE     Irq;

    UINT32      LogCtrl;    // The log control bits: enable/disable ... etc
    UINT32      LBMode;     // The loopback mode: see: SRIO_LB_xxx of this file
    UINT32      Speed;      // see: SRIO_SPEED_xxx

    UINT32      RemoteTxAddr;   // The destination address for WDMA operations, used if loopback is OFF
    UINT32      RemoteRxAddr;   // The destination address for RDMA operations, used if loopback is OFF

    PUINT8      DriverBuf;  // The pointer to the buffer allocated for TX/RX descriptors and TX/RX buffers

    PUINT8      TxBufPtr;
    UINT32      TxBufSize;  // The size of TX buffer in bytes used to send data
    UINT32      TxBufNum;   // The number of TX buffers (it's used for ping-pong scheme)

    PUINT8      RxBufPtr;
    UINT32      RxBufSize;  // The size of RX buffer in bytes used to receive data
    UINT32      RxBufNum;   // The number of RX buffers (chain of RX buffers, common value is 2)

    UINT32      TxLogPtr;   // The pointer to the tx log storage, it can be null
    UINT32      TxLogPos;   //
    UINT32      TxLogSize;  // The size of TX log storage in bytes
    UINT32      RxLogPtr;   // The pointer to the rx log storage, it can be null
    UINT32      RxLogPos;   //
    UINT32      RxLogSize;  // The size of RX log storage in bytes

    SrioRxCb    RxCb;       // The pointer to the "RX data handler"
    LPVOID      RxCbPtr;    // The "RX data handler" parameter

    SrioRxCb    TxCb;       // The pointer to the "TX data handler"
    LPVOID      TxCbPtr;    // The "TX data handler" parameter

    // DMA specific parameters

    PSDMAD      TxDmaHead;  // The pointer to the list of Tx DMA descriptors
    PSDMAD      RxDmaHead;  // The pointer to the list of Rx DMA descriptors

    UINT32      TxDmaCurIdx;
    UINT32      RxDmaCurIdx;

    UINT32      TxDmaNum;
    UINT32      RxDmaNum;


    // Statistics

    UINT32      SrioTotalIrqNum;
    UINT32      Srio_err_axi;
    UINT32      Srio_err_rio;
    UINT32      Srio_err_wdma;
    UINT32      Srio_err_rdma;
    UINT32      Srio_wdma_irq;
    UINT32      Srio_rdma_irq;

}SrioDrvCtx;

/** @brief The driver configuration parameters, it's used in
           SrioDrvEnabledDevice function */

typedef struct _SRIODRVCFG_
{
    UINT32      nID;        // The device ID: 0 or 1

    UINT32      LogCtrl;    // The log control bits: enable/disable ... etc
    UINT32      LBMode;     // The loopback mode: see: SRIO_LB_xxx of this file
    UINT32      Speed;      // see: SRIO_SPEED_xxx
    UINT32      Lanes;      // x1 or x4, see: SRIO_LANES_X1 or SRIO_LANES_X4

    UINT32      RemoteTxAddr;   // The destination address for WDMA operations, used if loopback is OFF
    UINT32      RemoteRxAddr;   // The destination address for RDMA operations, used if loopback is OFF

    UINT32      TxBufSize;  // The size of TX buffer in bytes used to send data
    UINT32      TxBufNum;   // The number of TX buffers (it's used for ping-pong scheme)

    UINT32      RxBufSize;  // The size of RX buffer in bytes used to receive data
    UINT32      RxBufNum;   // The number of RX buffers (chain of RX buffers, common value is 2)

    UINT32      TxLogPtr;   // The pointer to the tx log storage, it can be null
    UINT32      TxLogSize;  // The size of TX log storage in bytes
    UINT32      RxLogPtr;   // The pointer to the rx log storage, it can be null
    UINT32      RxLogSize;  // The size of RX log storage in bytes

    SrioRxCb    RxCb;       // The pointer to the "RX data handler"
    LPVOID      RxCbPtr;    // The "RX data handler" parameter

    SrioRxCb    TxCb;       // The pointer to the "TX data handler"
    LPVOID      TxCbPtr;    // The "TX data handler" parameter

}SRIODRVCFG;

//#define SRIO_SYNC_ENABLED

#ifdef SRIO_SYNC_ENABLED

INLINE void SrioDrvLock (VUINT32 * pObj) {MxGetLock(pObj);}
INLINE void SrioDrvUnlock (VUINT32 * pObj) {MxReleaseLock(pObj);}

#else

#define SrioDrvLock(x)
#define SrioDrvUnlock(x)

#endif // SRIO_SYNC_ENABLED

SrioDrvCtx * SrioDrvGetCtx (UINT32 nDevID);

/** @brief This function is used to send already prepared buffer, the
            address of buffer can be taken by using of SrioDrvGetTxBuffer function

    @param pCtx [in] - the pointer to the driver context
    @return [MXRC] an error code */

MXRC SrioDrvSendData (SrioDrvCtx * pCtx);

/** @brief This function is used to receive data in free RX buffer, the
            address of buffer can be taken by using of SrioDrvGetRxBuffer function

    @param pCtx [in] - the pointer to the driver context
    @return [MXRC] an error code */

MXRC SrioDrvRecvData (SrioDrvCtx * pCtx);

/** @brief This function is designed to enable SRIO device
           with specified parameters

    @param pCfg [in] - the pointer to the configuration parameters
    @return [MXRC] an error code */

MXRC SrioDrvEnabledDevice (SRIODRVCFG * pCfg);

/** @brief This function returns a pointer to the current buffer that can be used
            by the application to put data and it is used by SrioDrvSendData () function,
            the pointer is automatically changed by the system after callin of SrioDrvSendData ()

    @param pCtx  [in] - the device driver context
    @param nRes  [in] - reserved, SBZ

    @return [LPVOID] the pointer to the TX buffer */

LPVOID SrioDrvGetTxBuffer (SrioDrvCtx * pCtx, UINT32 nRes);

/** @brief This function returns a pointer to the current rx buffer that can be used
            by the system in operation of data reading and it is used by SrioDrvRecvData() function,
            the pointer is automatically changed by the system after callin of SrioDrvRecvData()

    @param pCtx  [in] - the device driver context
    @param nRes  [in] - reserved, SBZ

    @return [LPVOID] the pointer to the RX buffer */

LPVOID SrioDrvGetRxBuffer (SrioDrvCtx * pCtx, UINT32 nRes);

/** @brief This function shutdowns SRIO devices
    @return [MXRC] an error code */

MXRC SrioDrvShutdown (void);

///////////////////////////////////////////////////////////////////////////////
///                          4GMX UDI                                       ///
///////////////////////////////////////////////////////////////////////////////
MXRC SrioDrv_init(UINT32 nInit, LPVOID * pCtxStorage);

MXRC SrioDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData, UINT32 nCfgDataSize);

MXRC SrioDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData, UINT32 nCmdDataSize);

MXRC SrioDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData, UINT32 * pnCmdDataSize);

#endif //SRIO_DRV_ENABLED

#endif // _SRIO_DRV_H_

#ifdef __cplusplus
}
#endif

