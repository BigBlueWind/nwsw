//-------------------------------------------------------------------------------------------
/** @file cevaprintf.c
 *
 * @brief Implement printf function for CEVAs. 
 * @author Mindspeed Technologies
 * @version 
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
**/
//-------------------------------------------------------------------------------------------

#include "cevaprintf.h"
#include "cevaprnlib.h"
#include "global.h"
#include "cevadrvcfg.h"

#define IER_RXTH            0x01    /* Enable Received Data Available Interrupt*/
#define IER_TXTH            0x02    /* Enable Transmitter Empty Interrupt        */

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

void UartHwLock (UINT32 Addr)
{
	while (REG32(Addr) != 0)
	{
		
	}
}

void UartHwUnlock (UINT32 Addr)
{
	REG32(Addr) = 0;
}

void UartWriteStr(UINT32 nBaseAddr, char* pStr)
{
	VUINT32 nOpCode;
    PUARTREGS pUart = NULL;
	UINT32 t;
	
	pUart = (PUARTREGS)nBaseAddr;

    nOpCode = (pUart->ier & IER_TXTH);
    pUart->ier &= ~IER_TXTH;

    while (*pStr)
    {
		t = MxGetTicks ();
		
		while((pUart->lsr & 0x20) == 0)
        {
			if (MxGetTicks () - t > 150*1000)
				break;
        }
		
        if (MxGetTicks () - t > 150*1000)
        {
            pUart->ier |= nOpCode;
            return;
        }
		
		pUart->data = *pStr;

        pStr++;
    }

    pUart->ier |= nOpCode;
}

static unsigned char tBuf[512];

void uart_printf(const char *fmt, ...)
{
	PCEVA_DRV_CFG pCfg = CEVA_GET_DRV_CFG();
    
    int length = 0;
    StringOutputFile sf;   /*, org; */
    va_list a;
    printf_display_rec dr;

	if (pCfg->UartCfg.UartAddr == 0)
		return;
	
    // Check the descriptor of cyclic buffer
    sf.ptr = (char *)tBuf;
    /*org.ptr = sf.ptr; */
    /* format the variable argument input string */
    va_start(a, fmt);
    dr.putc1 = _sputc;
    dr.ferror = _serror;
    dr.fn = _no_fp_display;
    length = __vfprintf((FILE *) & sf, fmt, a, &dr);
    _sputc(0, (FILE *) & sf);
    va_end(a);

    //tBuf[10] = 0;
	
	UartHwLock(pCfg->UartCfg.HwSema);
	
	UartWriteStr (pCfg->UartCfg.UartAddr, (char*)tBuf);
	
	UartHwUnlock(pCfg->UartCfg.HwSema);
}
