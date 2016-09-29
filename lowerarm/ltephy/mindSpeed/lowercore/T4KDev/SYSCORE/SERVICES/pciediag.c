//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL pciediag.c
 *
 * @brief The PCIe diagnostics module
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "appprintf.h"
#include "pciedrv.h"
#include "heaps.h"

#include "pciediag.h"

MXRC PCIEDiagInit(UINT32* pReportData, UINT32* pReportSize)
{
	MXRC rc = MX_OK;
//	PCIEDRVCTX * pCtx;
	LPVOID addr;
	
	PCIECFG cfg;
	UINT32 val;

	PCIEREGS    cfg_regs;
	PCIELOCREGS lm_regs;
	PCIERCREGS rc_regs;

	// checking the status to prevent reinitialization PCIe device
//	pCtx = PCIeDrvGetCtx ();
//	if (pCtx->Inited == 1)
	if (REG32(RAD_PCIE_RESET) != 0x1)
	{
		uart_printf ("PCIe diagnostics: PCIe device has been already captured by LINUX.\n");
		return rc;
	}
	//////////////////////////////////////////////////////////////
	
	addr = MxHeapAlloc(DspHeap, 68*1024);

    memset (&cfg, 0, sizeof(cfg));

	cfg.BaseMem = ((UINT32)addr & 0xFFFFF000) + 4*1024;
	cfg.IrqCB   = NULL;
	cfg.Mode    = PCIE_DRV_OPT_RC; // RC
	cfg.Lanes   = 0;
	cfg.pIrqData= NULL;

//	if (b->ParamCount > 0)
//	    cfg.Mode = StrToNum (b->ParamList[0]);

	if (cfg.Mode != 0)
	    uart_printf ("PCIe(RC) init ... ");
	else
	    uart_printf ("PCIe(EP) init ... ");

	rc = PCIeDrvInit (&cfg);

	if (FAILED(rc))
	{
	    uart_printf ("PCIe diagnostics: PCIe initialization failed, rc=%x\r\n", rc);
	    MxHeapFree(DspHeap, addr);
	    return rc;
	}
	else
	{
	    uart_printf ("PCIe diagnostics: PCIe initialization OK\r\n");
	}

//	PCIeDrvShutdown();
//	MxHeapFree(DspHeap, addr);
//	return rc;

	uart_printf ("Connecting ... ");

	// to wait 3 seconds
	rc = PCIeDrvConnect (PCIE_DRV_CONNECT_TIMEOUT);

    if (FAILED(rc))
    {
        uart_printf ("PCIe diagnostics: PCIe connecting failed, rc=%x\r\n", rc);
        PCIeDrvShutdown();
        MxHeapFree(DspHeap, addr);
        return rc;
    }
    else
    {
        uart_printf ("PCIe diagnostics: PCIe connecting OK\r\n");
    }

    rc = PCIeDrvReadLocMgtRegs (&lm_regs);
    rc = PCIeDrvReadRCConfRegs (&rc_regs);
    
    if (FAILED(rc))
    {
    	uart_printf ("PCIe diagnostics: reading the registers FAILED.\n");
    	PCIeDrvShutdown();
    	MxHeapFree(DspHeap, addr);
    	return rc;
    }

    // Just for debug purpose
    val = REG32(RAD_CFG_PCIE_STATUS);

    if ((val & 3) == 0)
        uart_printf ("PCIe diagnostics: RootComplex: Link status: No receivers detected\r\n");
    if ((val & 3) == 1)
        uart_printf ("PCIe diagnostics: RootComplex: Link status: Link training in progress\r\n");
    if ((val & 3) == 2)
        uart_printf ("PCIe diagnostics: RootComplex: Link status: Link UP, DL initialization ... \r\n");
    if ((val & 3) == 3)
        uart_printf ("PCIe diagnostics: RootComplex: Link status: Link UP, DL initialization is OK \r\n");

    if (val & (1 << 2))
        uart_printf ("PCIe diagnostics: RootComplex: Speed 5.0 GB\r\n");
    else
        uart_printf ("PCIe diagnostics: RootComplex: Speed 2.5 GB\r\n");

    if (val & (1 << 3))
        uart_printf ("PCIe diagnostics: RootComplex: IO space Enabled\r\n");

    if (val & (1 << 4))
        uart_printf ("PCIe diagnostics: RootComplex: MemSpace Enabled\r\n");

    if (val & (1 << 5))
        uart_printf ("PCIe diagnostics: RootComplex: Bus Master Enabled\r\n");

    if (val & (1 << 6))
        uart_printf ("PCIe diagnostics: RootComplex: INTx Enabled\r\n");
	
    PCIeDrvShutdown();
    MxHeapFree(DspHeap, addr);
	
	return rc;
}


