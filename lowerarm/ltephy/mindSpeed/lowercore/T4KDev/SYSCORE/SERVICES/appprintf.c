//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL appprintf.c
 *
 * @brief This file implements printf and sprintf functions
 * @author Mindspeed Technologies
 * @version $Revision: 1.21 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdarg.h>
#include <string.h>
#include "lcorecfg.h"
#include "appprintf.h"
#include "uartdrv.h"
#include "4gmx.h"
#include "cline.h"
#include "heaps.h"
#include "dbgfunctab.h"
#include "conicpu.h"


typedef struct _UART_BUF_CTX_
{
    PUINT8 ptr;
    UINT32 put;
    UINT32 get;
    UINT32 len;
    UINT32 SelfAlloc;

}UART_BUF_CTX;

extern UARTCtx UartCtx[3];
extern const char* UARTName[];
static UINT32 uart_printf_align = 0;
static VUINT32 uart_printf_num = 0;
static VUINT32 uart_fprintf_num = 0;

static char * uart_printf_header = NULL;
static VUINT32 uart_print_dev = PRINT_DEV_UART;

UART_BUF_CTX    uart_buf;

VUINT32 uart_prn_lock = 0;
char uart_buffer [2*1024];

VUINT32 uart_fprn_lock = 0;
char uart_fbuffer [2*1024];


void uart_printf_set_align(UINT32 nNum)
{
    uart_printf_align = nNum;
}

void uart_printf_reset_num(void)
{
    uart_printf_num = 0;
}

UINT32 uart_printf_get_num(void)
{
    return (UINT32)uart_printf_num;
}

void uart_printf_set_header(char * pHeader)
{
    uart_printf_header = pHeader;
}

UINT32 uart_printf_get_dev(void)
{
	return uart_print_dev;
}


UINT32 uart_printf_select_dev(UINT32 nDevID, UINT32 param1, UINT32 param2)
{
    if (uart_buf.ptr != NULL && uart_buf.SelfAlloc != 0)
    {
        MxHeapFree(AppHeap, uart_buf.ptr);
    }

    memset (&uart_buf, 0, sizeof(uart_buf));

    if (nDevID == PRINT_DEV_BUF)
    {
        uart_buf.ptr = (PUINT8)param1;
        uart_buf.len = param2;
    }
    else if (nDevID == PRINT_DEV_TRACE)
    {
        UINT32 len = param1 ? param1 : 20*1024;

        uart_buf.ptr = (PUINT8)MxHeapAlloc (AppHeap, len);

        _ASSERT_PTR(uart_buf.ptr);

        if(uart_buf.ptr == NULL)
            return 0;

        uart_buf.len = len;
        uart_buf.SelfAlloc = 1;

        nDevID = PRINT_DEV_BUF;
    }

    uart_print_dev = nDevID;

    return 1;
}

void uart_push_char_to_buf(char ch)
{
    UINT32 get = uart_buf.get;

    if (uart_buf.ptr == 0)
        return;

    uart_buf.ptr[uart_buf.put++] = ch;

    if (uart_buf.put >= uart_buf.len)
        uart_buf.put = 0;

    if (uart_buf.put == get)
    {
        uart_buf.get ++;

        if (uart_buf.get >= uart_buf.len)
            uart_buf.get = 0;
    }
}

void uart_push_str_to_buf(char * pStr)
{
    if (pStr == NULL)
        return;

    while (*pStr != 0)
    {
        uart_push_char_to_buf(*pStr++);
    }
}

void uart_printf_print_buf(void)
{
    IRQTYPE irq;
    UINT32 nNum = 0;
    UINT8 ch;

    PUARTPARAMS pUartParams;
    UINT32 cmgrUART;

    cmgrUART = ((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrUART;

    if (cmgrUART == CMGR_TURNED_OFF)
    	return;

    pUartParams = (PUARTPARAMS)&CoreCfg.UARTParams[cmgrUART];

    if (pUartParams->EN == 0)
    	return;

    irq = ARM_INT_disable();
    MxGetLock (&uart_prn_lock);

    if (uart_print_dev == PRINT_DEV_BUF && uart_buf.ptr != 0)
    {
        while (uart_buf.get != uart_buf.put)
        {
            ch = uart_buf.ptr[uart_buf.get++];

            if (uart_buf.get >= uart_buf.len)
                uart_buf.get = 0;

            if (ch == 0)
                continue;

            UartDrvWriteChar(UARTGetCtx (cmgrUART), ch, 1);

            nNum++;
        }
    }

    if (nNum)
        CLineNeedUpdateLogo();

    uart_printf_num += nNum;

    MxReleaseLock (&uart_prn_lock);
    ARM_INT_restore (irq);
}

/** @brief This function sends data to the UART device in non-interrutp mode,
           it's designed to be used in IRQ/FIQ/EXC modes

    @param fmt [in] - the printf format template
    @return [int] the number of outputted bytes */

int uart_printf(const char *fmt, ...)
{
    va_list args;
    IRQTYPE irq;
    int length = 0;
    UINT32 nAlign = 0;

    PUARTPARAMS pUartParams;
    UINT32 cmgrUART;
    UINT32 nUpdateLogo = 0;

	cmgrUART = ((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrUART;

        if(!((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrCICPU)
        {
            if (cmgrUART == CMGR_TURNED_OFF)
                return 0;

            pUartParams = (PUARTPARAMS)&CoreCfg.UARTParams[cmgrUART];

            if (pUartParams->EN == 0)
                return 0;
        }
    // synchronization is needed here because we use
    // the same UART device, so in this case we can use one global buffer
    // instead of local one located on the stack, also the size of local buffer
    // can be much more bigger related to the local one

    irq = ARM_INT_disable();
    MxGetLock (&uart_prn_lock);

    if (uart_printf_header != NULL)
    {
        if (uart_print_dev == PRINT_DEV_UART)
        {
            nUpdateLogo = 1;

            // to send data to UART(RS-232)
            UartDrvWriteStr (UARTGetCtx (cmgrUART), uart_printf_header);
        }
        else if (uart_print_dev == PRINT_DEV_BUF)
        {
            uart_push_str_to_buf(uart_printf_header);
        }

        uart_printf_header = NULL;
    }

    va_start(args, fmt);     /* Initialize variable arguments. */

    // ------------------------------------------------------------------
    // Safe function is used to prevent buffer overwriting
    // ------------------------------------------------------------------
    length = _vsnprintf(uart_buffer, sizeof (uart_buffer) - 1, fmt, args);

    nAlign = 0;
    while (nAlign < uart_printf_align)
    {
        if (uart_print_dev == PRINT_DEV_UART)
        {
            nUpdateLogo = 1;
            UartDrvWriteChar(UARTGetCtx (cmgrUART), ' ', 1);
        }
        else if (uart_print_dev == PRINT_DEV_BUF)
        {
            uart_push_char_to_buf(' ');
        }
        else if (uart_print_dev == PRINT_DEV_CICPU)
        {
            ConICPUPutCh(' ', TRUE);
        }

        nAlign++;
    }

    if (uart_print_dev == PRINT_DEV_UART)
    {
        uart_printf_num += uart_printf_align;
        uart_printf_num += length;
        nUpdateLogo = 1;

        // to send data to UART(RS-232)
        UartDrvWriteStr (UARTGetCtx (cmgrUART), uart_buffer);
    }
    else if (uart_print_dev == PRINT_DEV_BUF)
    {
        uart_push_str_to_buf(uart_buffer);
    }
	
    else if (uart_print_dev == PRINT_DEV_CICPU)
    {
        ConICPUPutData((PUINT8)uart_buffer, length, TRUE);				
    }

    if (nUpdateLogo)
        CLineNeedUpdateLogo();

    MxReleaseLock (&uart_prn_lock);
    ARM_INT_restore (irq);

    va_end(args);

    return length;
}


int uart_force_printf(const char *fmt, ...)
{
    va_list args;
    IRQTYPE irq;
    int length = 0;

    PUARTPARAMS pUartParams;
    UINT32 cmgrUART;    

	cmgrUART = ((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrUART;


	if (cmgrUART == CMGR_TURNED_OFF)
    	return 0;

	pUartParams = (PUARTPARAMS)&CoreCfg.UARTParams[cmgrUART];

	if (pUartParams->EN == 0)
    	return 0;
	
    // synchronization is needed here because we use
    // the same UART device, so in this case we can use one global buffer
    // instead of local one located on the stack, also the size of local buffer
    // can be much more bigger related to the local one

    irq = ARM_INT_disable();
    MxGetLock (&uart_fprn_lock); 

    va_start(args, fmt);     /* Initialize variable arguments. */

    // ------------------------------------------------------------------
    // Safe function is used to prevent buffer overwriting
    // ------------------------------------------------------------------
    length = _vsnprintf(uart_fbuffer, sizeof (uart_fbuffer) - 1, fmt, args);
    
    // to send data to UART(RS-232)
    UartDrvWriteStr (UARTGetCtx (cmgrUART), uart_fbuffer);
 
    MxReleaseLock (&uart_fprn_lock);
    ARM_INT_restore (irq);

    va_end(args);

    return length;
}


void DisplayCallStack(void)
{
    PVUINT32 pc = (PVUINT32)__current_pc();
    PVUINT32 sp = (PVUINT32)__current_sp();
    PVUINT32 lr = (PVUINT32)__return_address();

    rootinfo_print_stack(pc, sp, lr, NULL);
}

void set_term_opt (UINT32 nOpt)
{
    UINT32 nVal = 1, i = 0;
    char msg [] = {0x1b, '[', 'x','x','m', 0};

    while (i < 32)
    {
        switch (nOpt & (nVal << i))
        {
            case TERM_OPT_RESET_ALL:
                msg [2] = '0';
                msg [3] = 'm';
                msg [4] = 0;
                break;

            case TERM_OPT_BRIGHT:
                msg [2] = '1';
                msg [3] = 'm';
                msg [4] = 0;
                break;

            case TERM_OPT_DIM:
                msg [2] = '2';
                msg [3] = 'm';
                msg [4] = 0;
                break;

            case TERM_OPT_UNDERSCORE:
                msg [2] = '4';
                msg [3] = 'm';
                msg [4] = 0;
                break;

            case TERM_OPT_BLINK:
                msg [2] = '5';
                msg [3] = 'm';
                msg [4] = 0;
                break;

            case TERM_OPT_REVERSE:
                msg [2] = '7';
                msg [3] = 'm';
                msg [4] = 0;
                break;

            case TERM_OPT_HIDDEN:
                msg [2] = '8';
                msg [3] = 'm';
                msg [4] = 0;
                break;

            case TERM_OPT_TEXT_BLACK:
                msg [2] = '3';
                msg [3] = '0';
                msg [4] = 'm';
                break;

            case TERM_OPT_TEXT_RED:
                msg [2] = '3';
                msg [3] = '1';
                msg [4] = 'm';
                break;

            case TERM_OPT_TEXT_GREEN:
                msg [2] = '3';
                msg [3] = '2';
                msg [4] = 'm';
                break;

            case TERM_OPT_TEXT_YELLOW:
                msg [2] = '3';
                msg [3] = '3';
                msg [4] = 'm';
                break;

            case TERM_OPT_TEXT_BLUE:
                msg [2] = '3';
                msg [3] = '4';
                msg [4] = 'm';
                break;

            case TERM_OPT_TEXT_MAGENTA:
                msg [2] = '3';
                msg [3] = '5';
                msg [4] = 'm';
                break;

            case TERM_OPT_TEXT_CYAN:
                msg [2] = '3';
                msg [3] = '6';
                msg [4] = 'm';
                break;

            case TERM_OPT_TEXT_WHITE:
                msg [2] = '3';
                msg [3] = '7';
                msg [4] = 'm';
                break;


            //////////////////////////////////////


            case TERM_OPT_TEXT_BG_BLACK:
                msg [2] = '4';
                msg [3] = '0';
                msg [4] = 'm';
                break;

            case TERM_OPT_TEXT_BG_RED:
                msg [2] = '4';
                msg [3] = '1';
                msg [4] = 'm';
                break;

            case TERM_OPT_TEXT_BG_GREEN:
                msg [2] = '4';
                msg [3] = '2';
                msg [4] = 'm';
                break;

            case TERM_OPT_TEXT_BG_YELLOW:
                msg [2] = '4';
                msg [3] = '3';
                msg [4] = 'm';
                break;

            case TERM_OPT_TEXT_BG_BLUE:
                msg [2] = '4';
                msg [3] = '4';
                msg [4] = 'm';
                break;

            case TERM_OPT_TEXT_BG_MAGENTA:
                msg [2] = '4';
                msg [3] = '5';
                msg [4] = 'm';
                break;

            case TERM_OPT_TEXT_BG_CYAN:
                msg [2] = '4';
                msg [3] = '6';
                msg [4] = 'm';
                break;

            case TERM_OPT_TEXT_BG_WHITE:
                msg [2] = '4';
                msg [3] = '7';
                msg [4] = 'm';
                break;

            default:
                i++;
                continue;
        }

        i++;
        uart_printf (msg);
    }
}

/* End of printf.c */

