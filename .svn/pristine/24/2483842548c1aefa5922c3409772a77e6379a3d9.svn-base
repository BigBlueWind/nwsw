//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL ddrmondrv.c
 *
 * @brief T4K + EVM DDR monitor driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "4gmx.h"
#include "config.h"
#include "ddrmondrv.h"
#include "appprintf.h"
#include "sysddrctrl.h"
#include "sysshutdown.h"
#include "DSPdrv.h"
#include "svsrlogger.h"
#include "mlog.h"

DDRMonCtx  ddr_mon_ctx;

DDRMonCtx * DDRMonDrvGetCtx (void)
{
    return &ddr_mon_ctx;
}

void DDRMonIrqHandler(volatile UINT32 * pAckReg, UINT32 nIrqID, LPVOID pParam)
{
    UINT32 status, id, done = 0;
    DDRMonCtx * pDrv;
    PUINT8 pDDRController; 

    pDrv = DDRMonDrvGetCtx ();
    _ASSERT_PTR(pDrv);

    _ASSERT ((UINT32)pParam < 2);

    id = (UINT32)pParam;
    pDDRController = pDrv->DDRBase[id];
    _ASSERT_PTR(pDDRController);
    
    status = pDDRController[0x163];
    status = (status << 8) | pDDRController[0x162];
    status = (status << 8) | pDDRController[0x161];

    if(status & (1 << 9))
    {
        VUINT32 a = 0, b = 0, c = 0;
    
        // write protect access
        pDrv->WrOperAddr[id] = (UINT32)pDDRController[0x1b7];         
        pDrv->WrOperAddr[id] = (pDrv->WrOperAddr[id] << 8) | ((UINT32)pDDRController[0x1b6]); 
        pDrv->WrOperAddr[id] = (pDrv->WrOperAddr[id] << 8) | ((UINT32)pDDRController[0x1b5]); 
        pDrv->WrOperAddr[id] = (pDrv->WrOperAddr[id] << 8) | ((UINT32)pDDRController[0x1b4]); 
        pDrv->WrOperLen[id] = pDDRController[0x98];
        
        pDrv->WrProtectErrCount[id]++;

        DspDrvGetDump (0, &done);

        uart_printf ("\r\nDDR-MONITOR[id=%d]: add[%x], len[%d], ceva-dump-mask:0x%x (ptr=0x%x)\r\n", id, pDrv->WrOperAddr[id], pDrv->WrOperLen[id], done, DspDrvGetDumpAddr (0));

        if(SvsrLoggerGetMask())
            SvsrLoggerFlush();

        if (MLogGetMask() != 0)
            MLogPrint();

        // to completely stop the system and to get CEVAs coredump 
        SysShutdownIRQHandler (&a, b, (LPVOID)&c);
        
        while (1);
    }

    pDDRController[0x15b] = 0xff;
    pDDRController[0x15c] = 0xff;
    pDDRController[0x15d] = 0xff;

    *pAckReg = nIrqID;
}

/** @brief This function enables/disables DDR monitor driver 

    @param ctrl [in] - see DDR_MON_CTRL_xxx
    @param pList[in] - the pointer to the list of protected areas, or NULL

    @return [MXRC] an error code */

MXRC DDRMonDrvCtrl(UINT32 ctrl, DDRMonRegion * pList)
{
    DDRMonCtx * pDrv = DDRMonDrvGetCtx ();
    PUINT8 pDDRController;
    UINT32 i;
    
    _ASSERT_PTR (pDrv);

    if (ctrl & DDR_MON_CTRL_ENABLED)
    {
        if (pList != NULL)
        {
            i = 0;
            while (pList[i].Len != 0)
            {
                SysDDREnableWriteProtect (pList[i].DddrID, pList[i].RegNum, pList[i].Addr, pList[i].Addr + pList[i].Len - 1);
                i++;
            }
        }
    
        pDrv->Ctrl |= DDR_MON_CTRL_ENABLED;

        pDDRController = (PUINT8)DDR0_CONTROLLER_BASE;
        pDDRController[0x15e] = 0;
        pDDRController[0x15f] = 0;
        pDDRController[0x160] = 0;   
        MxEnableInt (HAL_IRQ_DDR0_CTRL);

        pDDRController = (PUINT8)DDR1_CONTROLLER_BASE;
        pDDRController[0x15e] = 0;
        pDDRController[0x15f] = 0;
        pDDRController[0x160] = 0;
        MxEnableInt (HAL_IRQ_DDR1_CTRL);
    }
    else
    {
        pDrv->Ctrl &= ~DDR_MON_CTRL_ENABLED;

        if (pList != NULL)
        {
            i = 0;
            
            while (pList[i].Len != 0)
            {
                SysDDRDisableWriteProtect(pList[i].DddrID);
                i++;
            }
        }
        
        MxDisableInt (HAL_IRQ_DDR0_CTRL);
        MxDisableInt (HAL_IRQ_DDR1_CTRL);
    }

    return MX_OK;
}
    
MXRC DDRMonDrv_init(UINT32 nInit, LPVOID * pDrvCtx)
{
    DDRMonCtx * pDrv = DDRMonDrvGetCtx ();
    MXRC rc;
    _ASSERT_PTR (pDrv);

    memset (pDrv, 0, sizeof (*pDrv));

    pDrv->DDRBase [0] = (PUINT8)DDR0_CONTROLLER_BASE;
    pDrv->DDRBase [1] = (PUINT8)DDR1_CONTROLLER_BASE;

    if (MxGetARMID() == 0)
        rc = MxSetIntHandler (HAL_IRQ_DDR0_CTRL, INT_TYPE_FIQ, PRI_DDR_MON, DDRMonIrqHandler, (LPVOID)0);
    else
        rc = MxSetIntHandler (HAL_IRQ_DDR0_CTRL, INT_TYPE_IRQ, PRI_DDR_MON, DDRMonIrqHandler, (LPVOID)0);

    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    rc = MxSetIntHandler (HAL_IRQ_DDR1_CTRL, INT_TYPE_FIQ, PRI_DDR_MON, DDRMonIrqHandler, (LPVOID)1);

    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    return MX_OK;
}
