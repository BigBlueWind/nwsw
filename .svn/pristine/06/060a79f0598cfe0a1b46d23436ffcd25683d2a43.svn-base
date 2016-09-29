//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Ncnbdata.h
 *
 * @brief
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

#ifndef _NCNB_DATA_H_
#define _NCNB_DATA_H_

#include "4gmx.h"
#include "config.h"
#include "gemdrv.h"
#include "sysshutdown.h"
#include "icdrv.h"

extern __align(32) ABORT_FRAME gAbortCtx0;
extern __align(32) ABORT_FRAME gAbortCtx1;
extern __align(32) ABORT_FRAME gAbortCtx2;
extern __align(32) ABORT_FRAME gAbortCtx3;

extern __align(32) GicCpuRegs gDumpGicCpu0;
extern __align(32) GicCpuRegs gDumpGicCpu1;
extern __align(32) GicCpuRegs gDumpGicCpu2;
extern __align(32) GicCpuRegs gDumpGicCpu3;
extern __align(32) GicDistRegs gDumpGicDist;

extern __align(32) VUINT32 SysShtDwnLock[];
extern __align(32) VUINT32 SysShtDwnProcAbortLock;

extern VUINT32 SysShtDwnAlarmState;

#ifdef PHY_GEM_ENABLED
extern __align(128) HGRxFrm gem_rx_frm [];
extern __align(128) HGTxFrm gem_tx_frm [];

extern __align(128) UINT8 gem_rx_buffers [][2048];
#endif


#endif // _NCNB_DATA_H_

#ifdef __cplusplus
}
#endif

