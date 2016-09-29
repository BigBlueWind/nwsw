//-------------------------------------------------------------------------------------------
/** @file cline.c
 *
 * @brief This file contains implementation of console command menu,
*       API of initialization and so on
 * @author Mindspeed Technologies
 * @version $Revision: 1.23 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "config.h"

#ifdef CLINE_INIT

#include "cline.h"
#include "cmgr.h"
#include "heaps.h"
#include <string.h>
#include "uartdrv.h"
#include "config.h"
#include "appprintf.h"
#include "stream.h"
#include "console.h"
#include "lcorecfg.h"
#include "DSPdrv.h"
#include "conicpu.h"

CLCtx *  ClineCtx = NULL;
CLCtx    ClineCtxData;
extern const char* UARTName[];

CMgrMenuElm * CLineGlobalMenu = NULL;

static UINT8 UartRead (LPVOID pParam)
{
    UINT8 ch;
    IRQTYPE irq;

    CLCtx * pCtx = (CLCtx *)pParam;

    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable ();

    if (pCtx->RxPut != pCtx->RxGet)
    {
        ch = pCtx->rxbuf [pCtx->RxGet];

        pCtx->RxGet ++;
        
        if (pCtx->RxGet >= sizeof (pCtx->rxbuf))
            pCtx->RxGet = 0;

        ARM_INT_restore(irq);

	    return ch;
    }

    ARM_INT_restore(irq);

    return 0;
}

static UINT8 UartWrite (LPVOID pParam, UINT8 ch)
{
#if 1
    IRQTYPE irq;

    if (ClineCtx == NULL)
    	return 0;

    irq = ARM_INT_disable();
    CLINE_GET_LOCK(&ClineCtx->Sync);

    ClineCtx->txbuf [ClineCtx->TxPut ++] = ch;

    if (ClineCtx->TxPut >= sizeof (ClineCtx->txbuf))
        ClineCtx->TxPut = 0;

    CLINE_RELEASE_LOCK(&ClineCtx->Sync);
    ARM_INT_restore(irq);
#else 

#ifdef _MX_DEBUG
    MXRC rc;
#endif

    IRQTYPE irq;

    CLCtx * pCtx = (CLCtx *)pParam;

    _ASSERT_PTR(pCtx);

    irq = ARM_INT_disable();

#ifdef _MX_DEBUG
    rc = MxWriteDrvData(pCtx->hUart, UART_CMD_PUT_CHAR, (LPVOID)(UINT32)ch, 1);
    _ASSERT_RC(rc);
#else
    MxWriteDrvData(pCtx->hUart, UART_CMD_PUT_CHAR, (LPVOID)(UINT32)ch, 1);
#endif

    ARM_INT_restore(irq);

#endif

    return 0;
}


static UINT8 CICPURead (LPVOID pParam)
{
    UINT8 ch;

	if(ConICPUGetCh(&ch) != ICPU_QUEUE_OK)
	{		
		return 0;
	}
	
    return ch;
}

static UINT8 CICPUWrite (LPVOID pParam, UINT8 ch)
{
	MXRC rc ;
	  
    rc = ConICPUPutCh(ch, FALSE);

	if (rc)
		uart_force_printf("CICPUWrite : %c[%d], %d\r\n", ch, ch, rc);
	

    return 0;
}


static UINT8 CICPUWriteComp(LPVOID pParam)
{
	ConICPUNotify();	
	return 0;
}


static UINT8 UartCompleteWrite (LPVOID pParam)
{
    MXRC rc = 0;
    IRQTYPE irq;

    if (ClineCtx == NULL)
        return 0;

    irq = ARM_INT_disable();

    rc = UartDrv_config (ClineCtx->lpUartCtx, UART_CMD_ENABLE_TX_EMPTY, 0, 0);

    ARM_INT_restore(irq);
    
    _ASSERT_RC(rc);
    
    return (UINT8)rc;
}

void UartTxRxIrq (volatile UINT32 * pClear, UINT32 nIrqID, LPVOID pParam)
{
    MXRC rc;

    CLCtx * pCtx = (CLCtx *)pParam;

    UINT32 nSize;
    UINT8  ch;
    UINT32 iir, tmp;
    int num = 0;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(ClineCtx);

	
    CLINE_GET_LOCK(&ClineCtx->Sync);

    rc = MxReadDrvData(pCtx->hUart, UART_CMD_GET_IIR, &iir, &nSize);

    tmp = iir & 0xF;

    if (tmp == 4)
    {
        //rc = MxReadDrvData(pCtx->hUart, UART_CMD_GET_CHAR, &ch, &nSize);

        rc = UartDrv_read (pCtx->lpUartCtx, UART_CMD_GET_CHAR, &ch, &nSize);

        if (rc != MX_OK)
        {
            CLINE_RELEASE_LOCK(&ClineCtx->Sync);
            return;
	}

        pCtx->rxbuf [pCtx->RxPut++] = ch;

        if (pCtx->RxPut >= sizeof (pCtx->rxbuf))
            pCtx->RxPut = 0;

        rc = MxScheduleThread(pCtx->hTask);

        _ASSERT_RC(rc);

    }
    else  if (tmp == 2)
    {
        // if we already have a symbol, we put this character to the UART device
        // if the list of tx characters is empty just to disable this interrupt,
        // this interrupt will be enabled in CLWriteStr function

        if (ClineCtx->TxGet == ClineCtx->TxPut)
        {
            MxConfigDrv(pCtx->hUart, UART_CMD_DISABLE_TX_EMPTY, 0, 0);		
        }
        else
        {
            while ((num < 1) && (ClineCtx->TxGet != ClineCtx->TxPut))
	        {
                UartDrvWriteChar(pCtx->lpUartCtx, ClineCtx->txbuf [ClineCtx->TxGet++], 0);

                if (ClineCtx->TxGet >= sizeof (ClineCtx->txbuf))
                    ClineCtx->TxGet = 0;

    	        num ++;
	        }
        }

    }
	else
	{		
	    uart_printf ("[FIX UART:0x%x]\r\n", tmp);
        rc = UartDrvFix(pCtx->lpUartCtx);
    
		if (rc != MX_OK)
		{
		    *pClear = nIrqID;
			CLINE_RELEASE_LOCK(&ClineCtx->Sync);
			return;
		}
	}

    CLINE_RELEASE_LOCK(&ClineCtx->Sync);

    *pClear = nIrqID;
}

UINT32 CICPUCallBack (CharICPUCtx * ctx)
{
    MXRC rc;

    CLCtx * pCtx = (CLCtx *)ClineCtx;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(ClineCtx);

	
    CLINE_GET_LOCK(&ClineCtx->Sync);

    rc = MxScheduleThread(pCtx->hTask);

    _ASSERT_RC(rc);
  
    CLINE_RELEASE_LOCK(&ClineCtx->Sync); 

	//uart_force_printf("CICPUCallBack !\r\n");

	return rc;
}


void CICPUWriteProc(void)
{
	int num = 0;


    while ((num < 1) && (ClineCtx->TxGet != ClineCtx->TxPut))
    {
				
		if(ConICPUPutCh(ClineCtx->txbuf [ClineCtx->TxGet++], FALSE) != ICPU_QUEUE_FULL)
		{
        	if (ClineCtx->TxGet >= sizeof (ClineCtx->txbuf))
            	ClineCtx->TxGet = 0;
		}
		else
		{
			ConICPUNotify();
			return;
		}

        num ++;
    }

	ConICPUNotify();


}

UINT32 CLTask (LPVOID pParam)
{
#ifdef _MX_DEBUG
    MXRC rc;
#endif

    CLCtx * pCtx = (CLCtx *)pParam;

    _ASSERT_PTR(pCtx);

#ifdef _MX_DEBUG
    rc = CMgrProcData(&pCtx->MgrCtx);

    _ASSERT_RC(rc);
#else
    CMgrProcData(&pCtx->MgrCtx);
#endif

    return 0;
}

/** @brief This function sets a pointer to the external application menu table
 *
 *  @param pMenu [in] - the menu table
 *
 *  @return [MXRC] an error code

 *  \ingroup group_lte_services

 */

MXRC CLSetMenu (CMgrMenuElm * pMenu)
{
	CLineGlobalMenu = pMenu;

	return MX_OK;
}

/** @brief The function initializes console command line module

    @eturn [MXRC] error code
    *  \ingroup group_lte_services

    */

MXRC CLInit (void)
{
    CMgrInitData init;
    MXRC rc;
    UINT32 nSize;
    UINT32 IrqId;
    char ch;
    UINT32 num = 0;
    HANDLE hUart;
	UINT32 cmgrUART;

    CEVA_DRV_CFG * pCevaCfg = DspDrvGetCevaDrvCfgParams();

	CLCtx * pCtx = ClineCtx = &ClineCtxData;

	// Direct print to UART1
	ClineCtx = pCtx;
	
	memset (pCtx, 0, sizeof (CLCtx));
	
	if(((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrCICPU)
	{		
		pCtx->device = CLINE_DEVICE_CICPU;	
	}
	else
	{	
		pCtx->device = CLINE_DEVICE_UART;
	    cmgrUART = ((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrUART;

		if (cmgrUART == CMGR_TURNED_OFF)
	        return MX_OK;

	    rc = MxGetObjByName(CUR_CPU,  UARTName[cmgrUART], &hUart);
		_ASSERT_RC(rc);

	    if (pCevaCfg != NULL)
	    {
	        pCevaCfg->UartCfg.HwSema    = CEVA_DRV_HW_SEMA_LARM_UART;
	        pCevaCfg->UartCfg.UartAddr  = UartDrvGetBaseAddr(cmgrUART);
	    }
	}
		    
	if(pCtx->device == CLINE_DEVICE_UART)
	{

	    pCtx->hUart      = hUart;
	    pCtx->lpUartCtx  = (LPVOID)MxGetDrvCtx(hUart);

	    init.cbRead      = UartRead;
	    init.cbWrite     = UartWrite;
	    init.cbWriteComp = UartCompleteWrite;
	    init.CommDevData = pCtx;
	    init.Hotkey      = NULL;
		init.Device		 = CLINE_DEVICE_UART;
	}
	else if(pCtx->device == CLINE_DEVICE_CICPU)
	{
		init.cbRead      = CICPURead;
	    init.cbWrite     = CICPUWrite;
	    init.cbWriteComp = CICPUWriteComp;
	    init.CommDevData = pCtx;
	    init.Hotkey      = NULL;
		init.Device		 = CLINE_DEVICE_CICPU;
	}
    if (CLineGlobalMenu == NULL)
    	init.MainMenu    = Menu;
    else
    	init.MainMenu    = CLineGlobalMenu;

    rc = CMgrInit ("L-ARM", &init, &pCtx->MgrCtx);

    _ASSERT_RC(rc);

    rc = MxCreateThread(CLTask, NULL, pCtx, (TPRIORITY)0, &pCtx->hTask);

    _ASSERT_RC(rc);

	if(pCtx->device == CLINE_DEVICE_UART)
	{
	    nSize = sizeof (IrqId);
	    IrqId = 0;

	    rc = MxReadDrvData(hUart, UART_CMD_GET_IRQ_ID, &IrqId, &nSize);

	    _ASSERT_RC(rc);

	    pCtx->nIrqID = IrqId;

	    rc = MxSetIntHandler(IrqId, (ITYPE)(INT_TYPE_IRQ), PRI_UART, UartTxRxIrq, (LPVOID)pCtx);

	    _ASSERT_RC(rc);

	    rc = MxConfigDrv(hUart, UART_CMD_ENABLE_RX_READY, NULL, NULL);

	    _ASSERT_RC(rc);
	
	    num = 0;

	    while (num < 1000)
	    {
	        nSize = sizeof (ch);

	        rc = MxReadDrvData(hUart, UART_CMD_GET_CHAR, &ch, &nSize);

	        if (FAILED(rc))
	        {
	            rc = MX_OK;
	            break;
	        }

	        num ++;
	    }

	    // if some HW keeps sending data to UART 
	    // just to disable the UART to prevent system lock-up
	    if (num < 1000)
	    {
	        rc = MxEnableInt(IrqId);
	        _ASSERT_RC(rc);
	    }
	    else
	    {
	        uart_printf ("UART-IRQ-RX is not enabled due to UART problems");
	        ClineCtx = NULL;    // the flag that CL is not inited
	        rc = RC_CL_GARBAGE_DETECTED;
	    }
	}
	else if(pCtx->device == CLINE_DEVICE_CICPU)
	{			
		ConICPURegHandler(CICPUCallBack);
	}
    return rc;
}

/** @brief The function shows console input
    @eturn [MXRC] error code

    *  \ingroup group_lte_services

    */

MXRC CLUpdate (void)
{
#ifdef _MX_DEBUG
    MXRC rc;
#endif

    if (ClineCtx == NULL)
        return RC_CL_NOT_INIT;

    _ASSERT_PTR(ClineCtx);

#ifdef _MX_DEBUG
    rc = CMgrUpdate(&ClineCtx->MgrCtx);
#else
    CMgrUpdate(&ClineCtx->MgrCtx);
#endif

    ClineCtx->Ctrl &= ~CLINE_NEED_UPDATE;

    _ASSERT_RC(rc);

    return MX_OK;
}

BOOL CLIsActive (void)
{
    return ClineCtx != NULL;
}

MXRC CLWriteStr (char * pStr)
{
    IRQTYPE irq;
    UINT32 bEmpty;

    if (pStr == NULL)
        return RC_CL_PARAM_ERROR;

    if (ClineCtx == NULL)
        return RC_CL_NOT_INIT;

    irq = ARM_INT_disable();
    CLINE_GET_LOCK(&ClineCtx->Sync);

    ClineCtx->Ctrl |= CLINE_NEED_UPDATE;

    bEmpty = (ClineCtx->TxPut == ClineCtx->TxGet);

    while (*pStr)
    {
        ClineCtx->txbuf [ClineCtx->TxPut++] = *pStr ++;

        if (ClineCtx->TxPut >= sizeof (ClineCtx->txbuf))
            ClineCtx->TxPut = 0;
    }
	if(ClineCtx->device == CLINE_DEVICE_UART)
	{
	    if (bEmpty)
    	    UartDrv_config (ClineCtx->lpUartCtx, UART_CMD_ENABLE_TX_EMPTY, 0, 0);
	}
	else if(ClineCtx->device == CLINE_DEVICE_CICPU)
		CICPUWriteProc();

    CLINE_RELEASE_LOCK(&ClineCtx->Sync);
    ARM_INT_restore(irq);

    return MX_OK;
}

MXRC CLWriteChar (char ch)
{
    IRQTYPE irq;
    MXRC rc = MX_OK;
    UINT32 bEmpty;

    if (ClineCtx == NULL)
    	return RC_CL_NOT_INIT;

    irq = ARM_INT_disable();
    CLINE_GET_LOCK(&ClineCtx->Sync);

    ClineCtx->Ctrl |= CLINE_NEED_UPDATE;

    bEmpty = (ClineCtx->TxPut == ClineCtx->TxGet);

    ClineCtx->txbuf [ClineCtx->TxPut ++] = ch;

    if (ClineCtx->TxPut >= sizeof (ClineCtx->txbuf))
        ClineCtx->TxPut = 0;

	if(ClineCtx->device == CLINE_DEVICE_UART)
	{
	    if (bEmpty)
    	    rc = UartDrv_config (ClineCtx->lpUartCtx, UART_CMD_ENABLE_TX_EMPTY, 0, 0);
	}
	else if(ClineCtx->device == CLINE_DEVICE_CICPU)
		CICPUWriteProc();

    _ASSERT_RC(rc);

    CLINE_RELEASE_LOCK(&ClineCtx->Sync);
    ARM_INT_restore(irq);	

    return rc;
}

BOOL IsPrintActive (void)
{
    if (ClineCtx == NULL)
        return FALSE;
    
    return (ClineCtx->TxPut != ClineCtx->TxGet);
}

BOOL HaveToBeUpdated (void)
{
    if (ClineCtx == NULL)
        return FALSE;
    
    return (ClineCtx->Ctrl & CLINE_NEED_UPDATE);
}

void CLineNeedUpdateLogo(void)
{
    if (ClineCtx == NULL)
        return;
    
    ClineCtx->Ctrl |= CLINE_NEED_UPDATE;
}

void WaitPrint (void)
{
    if (ClineCtx == NULL)
        return;
    
    while (ClineCtx->TxPut != ClineCtx->TxGet)
    {
        _NOP();
        _NOP();
    }
}

#endif // CLINE_INIT
