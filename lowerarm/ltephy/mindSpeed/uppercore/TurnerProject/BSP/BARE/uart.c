/** @file uart.c
 *
 * @brief UART module
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2008 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_BARE

#include "uart.h"
#include "apb.h"
#include "clkcore.h"
#include "intc.h"
#include "isr.h"
#include "sysctrl.h"
#include "portio.h"
#include "osal.h"

OSAL_SEMAPHORE Uart0RxSema;
OSAL_SEMAPHORE Uart1RxSema;
OSAL_RINGBUF Uart0RxQueue;
OSAL_RINGBUF Uart0TxQueue;

U32 Uart0RxQData[UART_RXQ_SIZE];
U32 Uart0TxQData[UART_TXQ_SIZE];
U32 ConOutQData[UART_TXQ_SIZE];
U32 ConInQData[UART_RXQ_SIZE];

//
// Tasks
//
OSAL_TASK Uart0TaskID;
OSAL_TASK Uart1TaskID;

/*! \brief Initialize UART
 *
 *  \param cpuid CPU ID on which this function called
 *  \return Standard result code
*/
RESULTCODE UartInit(U32 cpuid)
{
    U32 latch = ClkGetAmbaClock() / (16 * UART_BAUD_RATE);

    if (OsalCreateRing(&Uart0RxQueue, Uart0RxQData, UART_RXQ_SIZE) != SUCCESS)
    {
        return FAILURE;
    }

    if (OsalCreateRing(&Uart0TxQueue, Uart0TxQData, UART_TXQ_SIZE) != SUCCESS)
    {
        return FAILURE;
    }

    if (OsalCreateSema(&Uart0RxSema) != SUCCESS)
    {
        return FAILURE;
    }

    // Enable divisor latch access
    IOWrite32A(UART_LCR, LCR_DLAB);
    // 115200 at this time
    IOWrite32A(UART_DLL, latch & 0xFF);
    IOWrite32A(UART_DLH, (latch >> 8) & 0xFF);
    // 8-n-1, divisor latch access disabled
    IOWrite32A(UART_LCR, LCR_ONE_STOP | LCR_CHAR_LEN8);
    // Enable Receiver interrupt
    IOWrite32A(UART_IER, IER_ERBFI);
    // FIFO enable, FIFO reset, TX FIFO empty trigger, RX FIFO 14 trigger
    IOWrite32A(UART_FCR, FCR_FIFOE | FCR_RFIFOR | FCR_XFIFOR | FCR_RXTRG14);

    IsrAddIrqHandler(INTC_UART0, UartIntHandler);
    IntcEnableIrq(cpuid, INTC_UART0);

    if (OsalCreateTask(&Uart0TaskID, Uart0RxTask, NULL, "Uart0Rx",
                   OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
    {
        return FAILURE;
    }
    OsalActivateTask(&Uart0TaskID);

    return SUCCESS;
}

/** Handler for UART IRQs
 */
void UartIntHandler(void)
{
    U32 status;
    U32 rxcntr = 0;
    U32 txcntr = 0;
    U32 ch;

    while ((status = (IORead32A(UART_IIR) & 0xF)) != IIR_NOINT)
    {
        if (status == IIR_RX_DATA)
        {
            ch = IORead32A(UART_RBR);
            OsalPutRing(&Uart0RxQueue, (PTR)ch);
            rxcntr++;
        }
        else if (status == IIR_TX_EMPTY)
        {
            while (txcntr < UART_FIFO_SIZE)
            {
                if (OsalGetRing(&Uart0TxQueue, (PTR) &ch) != SUCCESS)
                {
                    txcntr = 0;
                    // Disable TX Empty interrupt
                    IOWriteAnd32A(UART_IER, ~IER_ETBEI);
                    break;
                }
                IOWrite32A(UART_RBR, ch);
                txcntr++;
            }
        }
    }
    if (rxcntr > 0)
    {
//        OsalRaiseSemaIsr(&Uart0RxSema);
    }
}

#ifdef __GNUC__

#ifdef __CC_ARM
/*! \brief Reimplement fputc to make printf work with UART
 *
 *  \param ch Char to put into file
 *  \param f file handle (not used)
 *  \return Last character
*/
int fputc(int ch, FILE *f)
{
    while (OsalEnqueue(&Uart0TxQueue, (OSAL_PADDRESS) ch) != SUCCESS)
    {
        // TODO: In case queue is full block here until possible to put data
        // TODO: Let something lost but don't block
        // TODO: Select the right algorithm
        __nop();
    }
    // Enable TX Empty interrupt
    IOWriteOr32A(UART_IER, IER_ETBEI);

    return ch;
}
#else /* __CC_ARM */

/** Reimplement "read" to use UART console
 *
 * @param fildes File descriptor
 * @param buf
 * @param nbyte
 * @return
 */
int _read(int fildes, void *buf, size_t nbyte)
{
    return 0;
}

/** Reimplement "write" to use UART console
 *
 * @param fildes File descriptor
 * @param buf
 * @param nbyte
 * @return
 */
int _write(int fildes, const void *buf, size_t nbyte)
{
    int res = 0;
    const char *ptr = (const char *) buf;
    char ch;

    while (nbyte--)
    {
        ch = *ptr++;
        if (OsalEnqueue(&Uart0TxQueue, (OSAL_PADDRESS) (int) ch) != SUCCESS)
            break;
        res++;
    }
    // Enable TX Empty interrupt
    IOWriteOr32A(UART_IER, IER_ETBEI);

    return res;
}
#endif /* __CC_ARM */
#endif /* __GNUC__ */

/*! \brief Reimplement ferror to make printf work with UART
 *
 *  \param f File handle (not used)
 *  \return EOF
*/
#ifndef __GNUC__
int ferror(FILE *f)
{
    return EOF;
}
#endif /* __GNUC__ */

/*! \brief This task handles UART0 Rx requests
*/
void Uart0RxTask(void *param)
{
    while (1)
    {
        OsalWaitForSema(&Uart0RxSema, OSAL_WAIT_INFINITE);
    }
}

RESULTCODE Uart0Write(U32 size, PTR data)
{
    return SUCCESS;
}

RESULTCODE Uart1Write(U32 size, PTR data)
{
    return SUCCESS;
}

#endif /* USE_BARE */
