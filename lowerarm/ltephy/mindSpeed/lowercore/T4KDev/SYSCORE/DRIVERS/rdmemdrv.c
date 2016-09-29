//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Rdmemdrv.c
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "rdmemdrv.h"
#include "appprintf.h"
#include "4gmx.h"
#include "config.h"
#include "sirqassign.h"


#ifdef RDMEM_CACHE_CONTROL_ENABLED

void Sys_RdMemIrqHandlerCpu0(volatile UINT32 * pAckReg, UINT32 nIrqID, LPVOID pParam);

MXRC RdMemDrv_init(UINT32 nInit, LPVOID * pCtx)
{
    MXRC rc = MX_OK;

    rc = MxSetIntHandler(SOFT_ICPU_RDMEM, INT_TYPE_IRQ, PRI_RDMEM, Sys_RdMemIrqHandlerCpu0,NULL);

    if (FAILED (rc))
	    return rc;

    rc = MxEnableInt(SOFT_ICPU_RDMEM);

	if (FAILED (rc))
	    return rc;

    return rc;
}

void Sys_RdMemIrqHandlerCpu0(volatile UINT32 * pAckReg, UINT32 nIrqID, LPVOID pParam)
{
   UINT32 nCpuID = MxGetCpuID();
    static UINT32 stopCounter = 0;
    if (stopCounter++ == 0)
        stop_printf("Stopping PHY\r\n");

    uart_printf("Stopphy \r\n");

    MxFlushDCache();

    uart_printf("The DCache has flushed \r\n");

    // to clear the inter-CPU interrupt generated to this processor
    // from other processor(s)
    MxClearInterCpu(nCpuID, nIrqID);

    *pAckReg = nIrqID;
}

#endif /* RDMEM_CACHE_CONTROL_ENABLED */
