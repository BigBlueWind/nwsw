//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Hbidrv.h
 *
 * @brief Host Bus Interface
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
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

#ifndef _HBI_H_
#define _HBI_H_

#include "4gmx.h"
#include "4gmx_serv.h"
#include "hal.h"

#define HBI_STAT_ENABLED

#define HBI_RC_OK    MX_OK
#define HBI_RC_EMPTY 1
#define HBI_RC_TXERROR 1

/*
 * Host Interface Controller
 */

#define HBI_INT_ENABLE_REG          (HBI_APB_BASEADDR + 0x0000)
#define HBI_INT_ACKNOWLEDGE_REG     (HBI_APB_BASEADDR + 0x0004)
#define HBI_TX_MAIL0_REG		    (HBI_APB_BASEADDR + 0x0030)
#define HBI_TX_MAIL1_REG		    (HBI_APB_BASEADDR + 0x0034)
#define HBI_TX_MAIL2_REG		    (HBI_APB_BASEADDR + 0x0038)
#define HBI_TX_MAIL3_REG		    (HBI_APB_BASEADDR + 0x003C)
#define HBI_RX_MAIL0_REG		    (HBI_APB_BASEADDR + 0x0040)
#define HBI_RX_MAIL1_REG		    (HBI_APB_BASEADDR + 0x0044)
#define HBI_RX_MAIL2_REG		    (HBI_APB_BASEADDR + 0x0048)
#define HBI_RX_MAIL3_REG		    (HBI_APB_BASEADDR + 0x004C)
#define HBI_INT_STATUS_REG		    (HBI_APB_BASEADDR + 0x0004)
#define HBI_TX_FIFO_LEVEL_REG       (HBI_APB_BASEADDR + 0x0014)
#define HBI_RX_FIFO_LEVEL_REG       (HBI_APB_BASEADDR + 0x0024)
#define HBI_INT_STATUS(mask)        (*(volatile unsigned short *)HBI_INT_STATUS_REG & mask)
#define HBI_TX_FIFO_DATA_REG_8      (HBI_APB_BASEADDR + 0x4000)
#define HBI_TX_FIFO_DATA_REG_16     (HBI_APB_BASEADDR + 0x8000)
#define HBI_TX_FIFO_DATA_REG_32     (HBI_APB_BASEADDR + 0xC000)
#define HBI_TX_FIFO_SIZE            (HBI_APB_BASEADDR + 0x0014)
#define HBI_TX_FIFO_THRESHOLD_HIGH  (HBI_APB_BASEADDR + 0x0018)
#define HBI_TX_FIFO_THRESHOLD_LOW   (HBI_APB_BASEADDR + 0x001C)
#define HBI_RX_FIFO_DATA_REG_8      (HBI_APB_BASEADDR + 0x4000)
#define HBI_RX_FIFO_DATA_REG_16     (HBI_APB_BASEADDR + 0x8000)
#define HBI_RX_FIFO_DATA_REG_32     (HBI_APB_BASEADDR + 0xC000)
#define HBI_RX_FIFO_SIZE            (HBI_APB_BASEADDR + 0x0024)
#define HBI_RX_FIFO_THRESHOLD_HIGH  (HBI_APB_BASEADDR + 0x0028)
#define HBI_RX_FIFO_THRESHOLD_LOW   (HBI_APB_BASEADDR + 0x002C)


#define RXEIE       0x0100
#define TXM3IE      0x0400
#define RXM3IE      0x0800
#define TXM3IAK     0x04
#define RXEIAK      0x10
#define RXM3IAK     0x40
#define FLUSH_RX    0x80

#define TXF         0x01
#define TXTH        0x02
#define RXE         0x04
#define RXTH        0x08
#define RXM3I       0x10
#define TXM3I       0x20

#define HBI_CMD_NUM	22

typedef struct tHBIDRVCTX
{
	// This interface is implied to be used with supervisor module
        // to prevent lots of changes in system modules

        HANDLE hThr;

        HANDLE RxQ;
        HANDLE TxQ;
        HANDLE RxP;
        HANDLE TxP;

        UINT32 RxQStor [HBI_CMD_NUM + 1];
        UINT32 TxQStor [HBI_CMD_NUM + 1];

        UINT8  RxPStor [HBI_CMD_NUM][16*1024];
        UINT8  TxPStor [HBI_CMD_NUM][16*1024];

        UINT32 cpuID;
} HBIDRVCTX, *PHBIDRVCTX;

HBIDRVCTX * HBIDrvGetCtx (void);
MXRC HBIDrvSetRxQueueHandler(HANDLE RxQueueHandler);
LPVOID HBIDrvAllocTxPart(void);
MXRC HBIDrvFreeRxPart(LPVOID pData);

MXRC HBIDrv_init(UINT32 nInit, LPVOID * pDrvCtx);
MXRC HBIDrvShutdown(void);

// **************************************************
//   These functions just to be compatible with UDI
// **************************************************

INLINE MXRC HBIDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData,
                    UINT32 nCfgDataSize) {return 0;}

INLINE MXRC HBIDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                   UINT32 nCmdDataSize)  {return 0;}

INLINE MXRC HBIDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                  UINT32 * pnCmdDataSize) {return 0;}

// ***************************************************

MXRC HBIDrvRecvMsg(PUINT32 pU32, PUINT32 DataLen);
MXRC HBIDrvXmitMsg(LPVOID pData, UINT32 DataLen);
MXRC HBIShareDataObjects (void);
MXRC HBIUnShareDataObjects (void);
void HBIMmuCfg(UINT32 nCpuID, UINT32 nParam);
#define HBI_TIMEOUT             (1000 * 1000)
#define HBI_FIFO_MAX_SIZE_32W   64

#endif

#ifdef __cplusplus
}
#endif

