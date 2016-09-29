//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Ncnbdata.c
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.10 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "4gmx.h"
#include "config.h"
#include "gemdrv.h"
#include "sysshutdown.h"
#include "icdrv.h"

__align(32) ABORT_FRAME gAbortCtx0;
__align(32) ABORT_FRAME gAbortCtx1;
__align(32) ABORT_FRAME gAbortCtx2;
__align(32) ABORT_FRAME gAbortCtx3;
__align(32) GicCpuRegs gDumpGicCpu0;
__align(32) GicCpuRegs gDumpGicCpu1;
__align(32) GicCpuRegs gDumpGicCpu2;
__align(32) GicCpuRegs gDumpGicCpu3;
__align(32) GicDistRegs gDumpGicDist;
__align(32) VUINT32 SysShtDwnLock[CPU_NUM];
__align(32) VUINT32 SysShtDwnProcAbortLock;

VUINT32 SysShtDwnAlarmState;

#ifdef PHY_GEM_ENABLED
__align(128) GEMRxBDescr gem_rx_frm [ETH_NUM_RX_BUFS];
__align(128) GEMTxBDescr gem_tx_frm [ETH_NUM_TX_BUFS];

__align(128) UINT8 gem_rx_buffers [ETH_NUM_RX_BUFS][2048];
#endif

