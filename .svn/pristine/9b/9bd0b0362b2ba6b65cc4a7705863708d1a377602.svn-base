//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL UARTdrv.c
 *
 * @brief This file contains UART driver "C" code
 * @author Mindspeed Technologies
 * @version $Revision: 1.27 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "config.h"
#include <string.h>
#include "hal.h"
#include "uartdrv.h"
#include "lcorecfg.h"
#include "mmumgr.h"
#include "cevadrvcfg.h"

// These macros will be redefined to use real sync
// functions in case of the second core

#if 1
#define UART_DRV_GET_LOCK(x)	MxGetLock (x); UartHwLock(CEVA_DRV_HW_SEMA_LARM_UART);
#define UART_DRV_RELEASE_LOCK(x) MxReleaseLock (x); UartHwUnlock(CEVA_DRV_HW_SEMA_LARM_UART);
#define UART_DRV_INIT_LOCK()    REG32(CEVA_DRV_HW_SEMA_LARM_UART)=0
#else
#define UART_DRV_GET_LOCK(x)
#define UART_DRV_RELEASE_LOCK(x)
#define UART_DRV_INIT_LOCK()
#endif

UARTCtx UartCtx [3];

static UARTPARAMS UartInitParams[3]; // Initial parameters for UART

typedef struct
{
	PUARTCtx pUartCtx;
	unsigned char* pAccumulator; // the pointer to buffer with data
	UINT32 bufsize;
	BOOL redirect;
	UINT32 bufcounter;
	BOOL rollover;
}UARTBUFFER, *PUARTBUFFER;

static UARTBUFFER UARTBuf;

extern const LCoreCfg CoreCfg;

typedef struct tagUARTREGS {
    volatile UINT32 data;   /* Receive/Transmit data register*/
    volatile UINT32 ier;    /* Interrupt Enable register*/
    volatile UINT32 iir_fcr;/* Interrupt Identity register / FIFO Control register*/
    volatile UINT32 lcr;    /* Line Control register*/
    volatile UINT32 mcr;    /* Modem Control register*/
    volatile UINT32 lsr;    /* Line Status register*/
    volatile UINT32 msr;    /* Modem Status register*/
    volatile UINT32 sr;     /* Scratch register*/
} UARTREGS, *PUARTREGS;

typedef struct tagUARTDLREGS {
    volatile UINT32 dll;    /* Divisor Latch (Low)*/
    volatile UINT32 dlh;    /* Divisor Latch (High) */
} UARTDLREGS, *PUARTDLREGS;

void UartHwLock(UINT32 Addr)
{
    if (Addr == 0)
        return;

    while (REG32(Addr) != 0)
    {
        _NOP();
    }
}

void UartHwUnlock(UINT32 Addr)
{
    if (Addr == 0)
        return;

    REG32(Addr) = 0;
}

static void UartDrvAccumulate(UARTCtx* pUartCtx, char* pStr)
{
	//UINT32 strsize;

	UARTBuf.pUartCtx = pUartCtx;

	//strsize = strlen(pStr);

	while (*pStr)
	{
		*(UARTBuf.pAccumulator + UARTBuf.bufcounter)= *pStr;
		pStr += 1;
		UARTBuf.bufcounter += 1;
		if (UARTBuf.bufcounter == UARTBuf.bufsize)
		{
			UARTBuf.bufcounter = 0;
			UARTBuf.rollover = TRUE;
		}
	}
	*(UARTBuf.pAccumulator + UARTBuf.bufcounter) = '\0';
}

/**	@brief The function builds LCR value (line control register)
		   based on the UARTCfg structure

	@param pCfg [in] - a pointer to the init data

	@return [UINT32] LCR value */

UINT32 GetLCR (UARTCfg * pCfg)
{
    UINT32 nLCR = 0;

    nLCR = pCfg->ByteSize;

    if (pCfg->StopBits)
    {
       nLCR |= LCR_TWO_STOP;
    }

    /* Even parity */
    if (pCfg->Parity == 1)
    {
       nLCR |= (LCR_PEN | LCR_EPS);
    }
    /* Odd parity */
    else if (pCfg->Parity == 2)
    {
       nLCR |= (LCR_PEN);
    }

    return nLCR;
}

static void UartDrvInitDev(UARTCtx * pCtx)
{
    UINT32	latch_value = 0;
    PUARTDLREGS	pUartLatch  = NULL;
    PUARTREGS	pUart       = NULL;
    VUINT32 num=0;

    _ASSERT_PTR(pCtx);

    latch_value = ( UART_BUSCLOCK ) / (16 * pCtx->Config.Speed);

    pUartLatch  = (PUARTDLREGS)pCtx->BaseAddr;
    pUart       = (PUARTREGS)pCtx->BaseAddr;

    if (pUart->lcr != GetLCR (&pCtx->Config))
    {
        pUart->lcr  = LCR_DLAB;

        pUartLatch->dll = (latch_value & 0xFF);        /* Set Divisor Latch Low*/
        pUartLatch->dlh = (latch_value >> 8  ) & 0xFF; /* Set Divisor Latch high*/

        pUart->lcr = GetLCR (&pCtx->Config);
    }
    
    pUart->ier  = 0;		/**< RX - interrupt disabled / TX interrupt disabled */
    pUart->mcr  = 0xC0;

    pUart->iir_fcr = FCR_XMITRES | FCR_RCVRRES;

    while (num++ < 2000)
    {
        _NOP();
    }

    pUart->iir_fcr = FCR_XMITRES | FCR_RCVRRES | FCR_FIFOEN;/* Reset, FIFO Enable, 0 thresholds*/

    //pUart->ier = IER_RXTH | IER_TXTH;
}


static void UartDrvUpdateFreq(UARTCtx * pCtx)
{
    UINT32	latch_value = 0;
    PUARTDLREGS	pUartLatch  = NULL;
    PUARTREGS	pUart       = NULL;
    UINT32 ier = 0;

    _ASSERT_PTR(pCtx);

    MxDelayTicks (150*1000);

    latch_value = ( UART_BUSCLOCK ) / (16 * pCtx->Config.Speed);

    pUartLatch  = (PUARTDLREGS)pCtx->BaseAddr;
    pUart       = (PUARTREGS)pCtx->BaseAddr;

    ier = pUart->ier;

    pUart->ier = 0;
    MxDelayTicks (150*1000);

    pUart->lcr  = LCR_DLAB;

    pUartLatch->dll = (latch_value & 0xFF);        /* Set Divisor Latch Low*/
    pUartLatch->dlh = (latch_value >> 8  ) & 0xFF; /* Set Divisor Latch high*/

    MxDelayTicks (150*1000);

    pUart->lcr = GetLCR (&pCtx->Config);

    pUart->ier  = ier;		/**< RX - interrupt enable / TX interrupt enable */
    pUart->mcr  = 0xC0;

    pUart->iir_fcr = FCR_XMITRES | FCR_RCVRRES | FCR_FIFOEN;/* Reset, FIFO Enable, 0 thresholds*/

    MxDelayTicks (150*1000);
    
}

BOOL UartDrvWriteChar(UARTCtx * pCtx, char ch, BOOL bWait)
{
    volatile UARTREGS *pUart = NULL;

    int nWait = 0;

    if (pCtx->bInited == 0) // It is terminator for callers if UART forbidden.
		return TRUE;

    pUart = (UARTREGS *) pCtx->BaseAddr;

    pUart->data = ch;

    if (bWait)
    {
        while((pUart->lsr & 0x20) == 0)
        {
           // waiting, todo: setup wait timeout

           if (nWait >= UART_WAIT_CYCLE)
              return FALSE;

              nWait ++;
        }
    }

    return TRUE;
}

LPVOID UARTGetCtx (UINT32 nDev)
{
	_ASSERT (nDev < (sizeof (UartCtx) / sizeof (UartCtx[0])) );

    return &UartCtx[nDev];
}

MXRC UartDrvWriteStr (LPVOID pDrvCtx,  char * pStr)
{
    UARTCtx *pDrv = (UARTCtx *)pDrvCtx;

    IRQTYPE irq;
    UINT32 nOpCode;
    PUARTREGS pUart = NULL;

    if (pDrv->bInited == 0) // It is terminator for callers if UART forbidden.
		return MX_OK;

    if (UARTBuf.redirect == TRUE) // redirection of UART output in the buffer in memory
    {
        UartDrvAccumulate(pDrv, pStr);
		return MX_OK;
    }


    irq = ARM_INT_disable ();

    pUart = (PUARTREGS)pDrv->BaseAddr;

    UART_DRV_GET_LOCK(&pDrv->Sync);

    nOpCode = (pUart->ier & IER_TXTH);
    pUart->ier &= ~IER_TXTH;

    while (*pStr)
    {
        if (!UartDrvWriteChar((UARTCtx *) pDrvCtx, *pStr, 1))
        {
            pUart->ier |= nOpCode;

            UART_DRV_RELEASE_LOCK(&pDrv->Sync);
            ARM_INT_restore (irq);

            return RC_UART_TIMEOUT;
        }

        pStr++;
    }

    pUart->ier |= nOpCode;

    UART_DRV_RELEASE_LOCK(&pDrv->Sync);

    ARM_INT_restore (irq);

    return MX_OK;
}

BOOL UartDrvReadChar(LPVOID pDrvCtx , char *pCh)
{
    volatile PUARTREGS pUart = (PUARTREGS)((UARTCtx *)pDrvCtx)->BaseAddr;

    if (((UARTCtx*)pDrvCtx)->bInited == 0) // It is terminator for callers if UART forbidden.
		return FALSE;

    if ((pUart->lsr & 0x01) == 0)
       return FALSE;

    *pCh = (SINT8)pUart->data;

    return TRUE;
}

UINT32 UartDrvShutdown(UINT32 nDev)
{
    UARTCtx *pDrv = UARTGetCtx (nDev);
    volatile PUARTREGS pUart;
    VUINT32 tmp = 0;

    if (pDrv->bInited == 0) // It is terminator for callers if UART forbidden.
	    return 0;

	if (pDrv != NULL)
	{
		if (pDrv->BaseAddr != NULL)
		{
			pUart = (PUARTREGS)pDrv->BaseAddr;
			//pUart->ier &= ~(IER_RXTH | IER_TXTH);
			pUart->ier = 0;
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            tmp = pUart->iir_fcr;
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            tmp = pUart->iir_fcr;
		}
	}

    pDrv->bInited = 0;
    return tmp;
}

void UartDrvShutdownAll(void)
{
	UINT32 i;

	for (i = 0; i < (sizeof (UartCtx) / sizeof (UartCtx[0])); i++ )
	{
		UartDrvShutdown (i);
	}
}

MXRC UartDrvResetDevInt (UINT32 nDevID)
{
    PUARTREGS pUart = NULL;
    VUINT32 tmp;

    if (UartInitParams[nDevID].EN == 0) // It is terminator for callers if UART forbidden.
		return MX_OK;
    if (UartCtx[nDevID].bInited == 0)
		return RC_UART_ALLOC_ERR;

    if (nDevID == 0)
    {
        pUart = (PUARTREGS)UART0_BASEADDR;
    }
    else if (nDevID == 1)
    {
        pUart = (PUARTREGS)UART1_BASEADDR;
    }
    else if (nDevID == 2)
    {
        pUart = (PUARTREGS)UART2_BASEADDR;
    }
    else
        return RC_UART_UART_ID_ERR;

    pUart->ier &= ~IER_TXTH|IER_RXTH;
    tmp = pUart->iir_fcr;
    pUart->iir_fcr = 0x0;

    return MX_OK;
}

MXRC UartDrvReinitHW (UINT32 nDevID)
{
    PUARTREGS pUart = NULL;
    VUINT32 tmp;

    if (nDevID == 0)
    {
        pUart = (PUARTREGS)UART0_BASEADDR;
    }
    else if (nDevID == 1)
    {
        pUart = (PUARTREGS)UART1_BASEADDR;
    }
    else if (nDevID == 2)
    {
        pUart = (PUARTREGS)UART2_BASEADDR;
    }
    else
        return RC_UART_UART_ID_ERR;

    pUart->ier = 0;       // to disable all possible interrupts 
    tmp = pUart->iir_fcr; // to read the registers and resets HW signal inside UART 
    pUart->iir_fcr = 0x0;
    tmp = pUart->data;    // to make the fake read

    return MX_OK;
}


void UartDrvAckIrq(UINT32 irq)
{
    UINT32 r; 
    r = ARM_INT_disable();

    switch (irq)
    {
        case HAL_IRQ_UART_0:
        	UartDrvReinitHW(0);
            break;

        case HAL_IRQ_UART_1:
        	UartDrvReinitHW(1);
            break;

        case HAL_IRQ_UART_2:
        	UartDrvReinitHW(2);
            break;
    }
    
    ARM_INT_restore(r);
}

MXRC UartDrv_init(UINT32 nInit, LPVOID * pDrvCtx)
{
    UARTCtx *pDrv;
 //   BOOL bInit = UART0_BASEADDR;
    BOOL bInit = NULL;
    IRQTYPE irq;
    int uartnum = UartDrvGetNum(nInit);

    if (uartnum < 0)
		return RC_UART_ALLOC_ERR;

    UART_DRV_INIT_LOCK();

    if (UartInitParams[uartnum].EN)
    {
        irq = ARM_INT_disable();
    	UartDrvResetDevInt(uartnum);
        ARM_INT_restore(irq);
    }

    switch (nInit)
    {
        case UART0_BASEADDR:
            pDrv = (UARTCtx *)&UartCtx[0];
	    if (UartInitParams[0].EN)
                REG32(GPIO_PIN_SELECT_REG) &= ~((UINT32)3 << 19);
            break;

        case UART1_BASEADDR:
            pDrv = (UARTCtx *)&UartCtx[1];
	    if (UartInitParams[1].EN)
                REG32(GPIO_PIN_SELECT_REG) &= ~((UINT32)3 << 29);
            break;

        case UART2_BASEADDR:
            pDrv = (UARTCtx *)&UartCtx[2];
	    if (UartInitParams[2].EN)
	    {
                REG32(GPIO_PIN_SELECT_REG) &= ~((UINT32)1 << 31);
                REG32(GPIO_PIN_SELECT_REG) &= ~((UINT32)1 << 15);
	    }
            break;

        default:
            pDrv = NULL;
            break;
    }

    if (pDrv == NULL)
        return RC_UART_ALLOC_ERR;

    bInit = (pDrv->bInited == 0x12345678);

    if ((!bInit) &&  (UartInitParams[uartnum].EN == 1))
    {
        memset (pDrv, 0, sizeof (UARTCtx));

        pDrv->BaseAddr         = nInit;
        pDrv->Config.ByteSize  = UartInitParams[uartnum].size;
        pDrv->Config.Speed     = UartInitParams[uartnum].speed;
        pDrv->Config.StopBits  = UartInitParams[uartnum].stop;

        if (UartInitParams[uartnum].parity)
            pDrv->Config.Parity = ((UINT32)UartInitParams[uartnum].parsel + 1);
        else
            pDrv->Config.Parity = 0;

        UartDrvInitDev(pDrv);

        pDrv->bInited = 0x12345678;
    }

    *pDrvCtx = pDrv;

    return RC_UART_OK;
}

MXRC UartDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData,
                    UINT32 nCfgDataSize)
{
    UARTCtx *pDrv;
    //int uartnum;

    _ASSERT(pDrvCtx != NULL);

    pDrv = (UARTCtx *) pDrvCtx;

    if (pDrv->bInited == 0)
		return RC_UART_ALLOC_ERR;

    UART_DRV_GET_LOCK(&pDrv->Sync);

    switch (nCfg)
    {
        case UART_CMD_SET_SPEED:
            {
                pDrv->Config.Speed = (UINT32) pCfgData;
                break;
            }

        case UART_CMD_SET_PARITY:
            {
                pDrv->Config.Parity = (UINT16)(UINT32) pCfgData;
                break;
            }

        case UART_CMD_SET_STOPBITS:
            {
                pDrv->Config.StopBits = (UINT16)(UINT32) pCfgData;
                break;
            }

        case UART_CMD_SET_BYTESIZE:
            {
                pDrv->Config.ByteSize = (UINT16)(UINT32) pCfgData;
                break;
            }
        case UART_CMD_SET_LOOPBACK:
            {
                PUARTREGS pUart = NULL;

                pUart = (PUARTREGS)pDrv->BaseAddr;

                pUart->mcr |= MCR_LOOPBACK;

                UART_DRV_RELEASE_LOCK(&pDrv->Sync);

                return MX_OK;
            }

        case UART_CMD_RESET_LOOPBACK:
            {
                PUARTREGS pUart = NULL;

                pUart = (PUARTREGS)pDrv->BaseAddr;

                pUart->mcr &= ~MCR_LOOPBACK;

                UART_DRV_RELEASE_LOCK(&pDrv->Sync);

                return MX_OK;
            }
        case UART_CMD_ENABLE_TX_EMPTY:
            {
                PUARTREGS pUart = NULL;

                pUart = (PUARTREGS)pDrv->BaseAddr;

                pUart->ier |= IER_TXTH;

                UART_DRV_RELEASE_LOCK(&pDrv->Sync);

                return MX_OK;
            }

        case UART_CMD_ENABLE_RX_READY:
            {
                PUARTREGS pUart = NULL;

                pUart = (PUARTREGS)pDrv->BaseAddr;

                pUart->ier |= IER_RXTH;

                UART_DRV_RELEASE_LOCK(&pDrv->Sync);

                return MX_OK;
            }

        case UART_CMD_DISABLE_TX_EMPTY:
            {
                PUARTREGS pUart = NULL;

                pUart = (PUARTREGS)pDrv->BaseAddr;

                pUart->ier &= ~IER_TXTH;

                UART_DRV_RELEASE_LOCK(&pDrv->Sync);

                return MX_OK;
            }

        case UART_CMD_DISABLE_RX_READY:
            {
                PUARTREGS pUart = NULL;

                pUart = (PUARTREGS)pDrv->BaseAddr;

                pUart->ier &= ~IER_RXTH;

                UART_DRV_RELEASE_LOCK(&pDrv->Sync);

                return MX_OK;
            }
        case UART_CMD_SET_IER:
            {
                ((PUARTREGS)pDrv->BaseAddr)->ier = (UINT32)pCfgData;
                break;
            }

        default:
            UART_DRV_RELEASE_LOCK(&pDrv->Sync);
            return RC_UART_CMD_ERR;
    }

    UartDrvInitDev(pDrv);

    UART_DRV_RELEASE_LOCK(&pDrv->Sync);

    return MX_OK;
}

MXRC UartDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                   UINT32 nCmdDataSize)
{

    _ASSERT(pDrvCtx != NULL);

    if (((UARTCtx*)pDrvCtx)->bInited == 0) // It is terminator for callers if UART forbidden.
		return MX_OK;

    UART_DRV_GET_LOCK(&((UARTCtx *)pDrvCtx)->Sync);

    switch (nCmd)
    {
        case UART_CMD_PUT_CHAR:
            {
                if (!UartDrvWriteChar((UARTCtx *) pDrvCtx, (char)(UINT32) pCmdData, 1))
                    return RC_UART_TIMEOUT;

                break;
            }

        case UART_CMD_PUT_STR:
            {
                char *pStr = (char *)pCmdData;

                while (*pStr)
                {
                    if (!UartDrvWriteChar((UARTCtx *) pDrvCtx, *pStr, 1))
                        return RC_UART_TIMEOUT;

                    pStr++;
                }

                break;
            }

        default:
            UART_DRV_RELEASE_LOCK(&((UARTCtx *) pDrvCtx)->Sync);
            return RC_UART_CMD_ERR;
    }

    UART_DRV_RELEASE_LOCK(&((UARTCtx *) pDrvCtx)->Sync);

    return MX_OK;
}

MXRC UartDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                  UINT32 * pnCmdDataSize)
{
    UARTCtx * pCtx = (UARTCtx *)pDrvCtx;

    _ASSERT(pDrvCtx != NULL);
    _ASSERT(pCmdData != NULL);

    if (pCtx->bInited == 0) // It is terminator for callers if UART forbidden.
		return RC_UART_BUF_EMPTY;

    UART_DRV_GET_LOCK (&pCtx->Sync);

    switch (nCmd)
    {
        case UART_CMD_GET_CHAR:
            {
                if (!UartDrvReadChar(pCtx, (char *)pCmdData))
                {
                    UART_DRV_RELEASE_LOCK (&pCtx->Sync);

                    return RC_UART_TIMEOUT;
                }

                break;
            }

        case UART_CMD_GET_IRQ_ID:
            {
                switch (pCtx->BaseAddr)
                {
                    case UART0_BASEADDR:
                        {
                            *((UINT32*)pCmdData) = HAL_IRQ_UART_0;
                            break;
                        }
                    case UART1_BASEADDR:
                        {
                            *((UINT32*)pCmdData) = HAL_IRQ_UART_1;
                            break;
                        }
                    case UART2_BASEADDR:
                        {
                            *((UINT32*)pCmdData) = HAL_IRQ_UART_2;
                            break;
                        }
                    default:
                        UART_DRV_RELEASE_LOCK (&pCtx->Sync);
                        return RC_UART_UART_ID_ERR;
                }

                break;
            }
        case UART_CMD_GET_IIR:
            {
                *((UINT32*)pCmdData) = ((UARTREGS*)pCtx->BaseAddr)->iir_fcr;

                break;
            }

        case UART_CMD_GET_IER:
            {
                *((UINT32*)pCmdData) = ((UARTREGS*)pCtx->BaseAddr)->ier;
                break;
            }
        case UART_CMD_IS_TX_IRQ_ENABLED:
            {
                *((UINT32*)pCmdData) = (((UARTREGS*)pCtx->BaseAddr)->ier & IER_TXTH) ? 1 : 0;
                break;
            }

        default:
            UART_DRV_RELEASE_LOCK (&pCtx->Sync);
            return RC_UART_CMD_ERR;
    }

    UART_DRV_RELEASE_LOCK (&pCtx->Sync);

    return MX_OK;
}

void UartDrvLoadInitParams(LPVOID pUartInitParams)
{
	UINT32 i = 0;

	while (i < 3)
	{
		UartInitParams[i] = ((PUARTPARAMS)pUartInitParams)[i];
		i += 1;
	}
}

int UartDrvGetNum(UINT32 baseaddr)
{
	switch (baseaddr)
	{
		case UART0_BASEADDR:
			return 0;
		case UART1_BASEADDR:
			return 1;
		case UART2_BASEADDR:
			return 2;
		default:
			return -1;
	}
}

UINT32 UartDrvGetBaseAddr(UINT32 index)
{
    switch (index)
	{
		case 0:
            return UART0_BASEADDR;
		case 1:
            return UART1_BASEADDR;
		case 2:
            return UART2_BASEADDR;

		default:
			return UART0_BASEADDR;
	}
}

MXRC UartDrvFix(LPVOID pDrvCtx)
{   
    volatile UARTREGS *pUart;
    UARTCtx* pCtx = (UARTCtx*)pDrvCtx;
    VUINT32 tmp, num = 0;

    pUart = (UARTREGS *) pCtx->BaseAddr;

    tmp = pUart->iir_fcr;

    if (tmp & (1<<1))
    {
        pUart->ier &= ~IER_TXTH;
    }

    while (num < 1000 && (tmp & (1 << 2)))
    {
        if ((pUart->lsr & 0x01) == 0)
            break;
    
        tmp = pUart->data;

        num++;
    }

    tmp = pUart->iir_fcr;
    pUart->iir_fcr = FCR_XMITRES | FCR_RCVRRES;
    
    num = 0;
    while (num++ < 1000)
    {
        _NOP();
        _NOP();
        _NOP();
        _NOP();
    }
    tmp = pUart->iir_fcr;
    pUart->iir_fcr = FCR_XMITRES | FCR_RCVRRES | FCR_FIFOEN;

    num = 0;
    while (num++ < 1000)
    {
        _NOP();
        _NOP();
        _NOP();
        _NOP();
    }

    return MX_OK;
}

void UartDrvStartHoldPrinting(LPVOID pBuf, UINT32 bufsize)
{
	if (pBuf == NULL)
		return;
	if (bufsize == 0)
		return;
	memset (pBuf, 0, bufsize);
	UARTBuf.pAccumulator = (unsigned char*)pBuf;
	UARTBuf.bufsize = bufsize;
	UARTBuf.redirect = TRUE;
	UARTBuf.bufcounter = 0;
	UARTBuf.rollover = FALSE;
}

MXRC UartDrvPrintBuf(void)
{
	UARTCtx *pDrv;
	IRQTYPE irq;
    UINT32 nOpCode;
    PUARTREGS pUart = NULL;

	if (UARTBuf.pUartCtx == NULL)
		return RC_UART_BUF_EMPTY;

	pDrv = UARTBuf.pUartCtx;

   	if (pDrv->bInited == 0) // It is terminator for callers if UART forbidden.
		return RC_UART_ALLOC_ERR;


    irq = ARM_INT_disable ();

    pUart = (PUARTREGS)pDrv->BaseAddr;

   	 UART_DRV_GET_LOCK(&pDrv->Sync);

    nOpCode = (pUart->ier & IER_TXTH);
    pUart->ier &= ~IER_TXTH;

	UARTBuf.bufcounter += 1; // jumps over '\0' and moves the pointer to begining of the string
	if (UARTBuf.bufcounter == UARTBuf.bufsize)
		UARTBuf.bufcounter = 0;
	if (UARTBuf.rollover == FALSE) // if rollover condition doesn't happen - print from begining of buffer
		UARTBuf.bufcounter = 0;

    while (*(UARTBuf.pAccumulator + UARTBuf.bufcounter))
    {
       	if (!UartDrvWriteChar((UARTCtx *)UARTBuf.pUartCtx, *(UARTBuf.pAccumulator + UARTBuf.bufcounter), 1))
        {
           	pUart->ier |= nOpCode;

            UART_DRV_RELEASE_LOCK(&pDrv->Sync);
            ARM_INT_restore (irq);

            return RC_UART_TIMEOUT;
        }

        UARTBuf.bufcounter += 1;
		if (UARTBuf.bufcounter == UARTBuf.bufsize)
			UARTBuf.bufcounter = 0;
    }

    pUart->ier |= nOpCode;

    UART_DRV_RELEASE_LOCK(&pDrv->Sync);

    ARM_INT_restore (irq);

    memset (UARTBuf.pAccumulator, 0, UARTBuf.bufsize);
	UARTBuf.bufcounter = 0;
	UARTBuf.rollover = FALSE;

    return MX_OK;
}

void UartDrvStopHoldPrinting(void)
{
	UARTBuf.pUartCtx = NULL;
	UARTBuf.pAccumulator = NULL;
	UARTBuf.bufsize = 0;
	UARTBuf.bufcounter = 0;
	UARTBuf.redirect = FALSE;
	UARTBuf.rollover = FALSE;
}

void UartDrvReinitFreq(void)
{
    UINT32 nID;
    UARTCtx * pCtx;

    for (nID = 0; nID < 3; nID++)
    {
        pCtx = &UartCtx[nID];

        if (pCtx->bInited == 0x12345678)
            UartDrvUpdateFreq(pCtx);
    }
}

void UartMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    MmuMgrCfgMemAP(UART0_BASEADDR, 4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
    MmuMgrCfgMemAP(UART1_BASEADDR, 4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
    MmuMgrCfgMemAP(UART2_BASEADDR, 4*1024, AP_MGR_READ_WRITE, AP_NO_ACCESS);
}

