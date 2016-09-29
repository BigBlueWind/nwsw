//-------------------------------------------------------------------------------------------
/** @file dspprintf.c
 *
 * @brief Implement printf function for CEVAs.
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "config.h"

#ifdef DSP_PRINTF_ENABLED

#include "dspprintf.h"
#include "appprintf.h"
#include "sirqassign.h"
#include "heaps.h"

// Descriptor of cyclic buffer in NCNB memory.
DSPPRINTFDESC NCNBVAR DspPrintfDesc;

MXRC DspPrintfInit(void)
{
    MXRC rc = MX_OK;
    PDSPPRINTFDESC pDPD = &DspPrintfDesc;

    uart_printf("CEVA printf service start... ");

    memset(pDPD, 0x00, sizeof(*pDPD));

    // Create the thread fot the hadler of cyclic buffer
    if( FAILED(rc = MxCreateThread(DspPrintfThread, NULL, NULL, NULL, &pDPD->h)) )
        return rc;

    // fill in field of the cyclic buffer descriptor
    pDPD->size = DSP_PRINTF_BUF_LEN;
    pDPD->get = 0;
    pDPD->put = 0;


    // Alloce the cyclic buffer in NCNB memory.
    if ( (pDPD->pStart = MxHeapAlloc(DDR1NcnbHeap, DSP_PRINTF_BUF_LEN)) == NULL )
    {
        uart_printf ("DDR1NcnbHeap alloc error\n");
        return rc;
    }

    memset(pDPD->pStart,0x00,DSP_PRINTF_BUF_LEN);

    // Put the pointer of the cyclic buffer descriptor to CRAM.
    CRAM_ARM_PRINTBUF_ADDR = (unsigned int)pDPD;

    if (SOFT_ICPU_DSP_PRINTF < 0)
    {
        uart_printf("Fail! \n");
        _ASSERT(0);
        return rc;
    }

    if ( FAILED(rc = MxSetIntHandler(SOFT_ICPU_DSP_PRINTF, INT_TYPE_IRQ, 0, DspPrintfIrqHandler,NULL)) )
	    return rc;

	if ( FAILED (rc = MxEnableInt(SOFT_ICPU_DSP_PRINTF)) )
	    return rc;

    uart_printf("OK! \n");

    return rc;
}

void DspPrintfIrqHandler(volatile UINT32 * pAckReg, UINT32 nIrqID, LPVOID pParam)
{
    UINT32 nCpuID = MxGetCpuID();
    PDSPPRINTFDESC pDPD = &DspPrintfDesc;

    // to clear the inter-CPU interrupt generated to this processor
    // from other processor(s)
    MxClearInterCpu(nCpuID, nIrqID);

    // Schedule the hadler of cyclic buffer
    if ( FAILED(MxScheduleThread(pDPD->h)) )
        uart_printf ("Schedule thread error\n");

    *pAckReg = nIrqID;
}

void DspPrintfThread (void)
{
    PDSPPRINTFDESC pDPD = &DspPrintfDesc;

    if (pDPD->get != pDPD->put)
    {
        UartDrvWriteChar(UARTGetCtx (DSP_UART_PORT), pDPD->pStart[pDPD->get++], 1);

        if (pDPD->get == pDPD->size)
            pDPD->get = 0;
    }

    if (pDPD->get != pDPD->put)
       if ( FAILED(MxScheduleThread(pDPD->h)) )
            uart_printf ("Schedule thread error\n");

}

#endif /* DSP_PRINTF_ENABLED */
