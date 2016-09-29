/** @file uart.c
 *
 * @brief UART module
 * @author Mindspeed Technologies
 * @version $Revision: 1.3 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_RTXC

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
OSAL_QUEUE ConOutQueue;
OSAL_QUEUE ConInQueue;
OSAL_MUTEX ConMutex;

U32 Uart0RxQData[UART_RXQ_SIZE];
U32 Uart0TxQData[UART_TXQ_SIZE];
U32 ConOutQData[UART_TXQ_SIZE];
U32 ConInQData[UART_RXQ_SIZE];

//
// Tasks
//
OSAL_TASK Uart0TaskID;
OSAL_TASK Uart1TaskID;
OSAL_TASK Uart0TxTaskID;
//OSAL_TASK Uart1TxTaskID;

/*! \brief Initialize UART
 *
 *  \param cpuid CPU ID on which this function called
 *  \return Standard result code
*/
RESULTCODE UartInit(U32 cpuid)
{
    U32 latch = ClkGetAmbaClock() / (16 * UART_BAUD_RATE);

    if (OsalCreateRingBuf(&Uart0RxQueue, Uart0RxQData, UART_RXQ_SIZE) != SUCCESS)
        return FAILURE;

    if (OsalCreateRingBuf(&Uart0TxQueue, Uart0TxQData, UART_TXQ_SIZE) != SUCCESS)
        return FAILURE;

    if (OsalCreateSema(&Uart0RxSema) != SUCCESS)
        return FAILURE;

    if (OsalCreateQueueEx(&ConOutQueue, ConOutQData, UART_TXQ_SIZE, 1) != SUCCESS)
        return FAILURE;

    if (OsalCreateQueueEx(&ConInQueue, ConInQData, UART_RXQ_SIZE, 1) != SUCCESS)
        return FAILURE;

    if (OsalCreateMutex(&ConMutex) != SUCCESS)
        return FAILURE;

    rtxc_stdio_init(ConMutex, ConInQueue, ConOutQueue);

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
        return FAILURE;

    if (OsalActivateTask(&Uart0TaskID) != SUCCESS)
        return FAILURE;

    // RTXC uses semaphore for UART TX
    if (OsalCreateTask(&Uart0TxTaskID, Uart0TxTask, NULL, "Uart0Tx",
                   OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
        return FAILURE;

    if (OsalActivateTask(&Uart0TxTaskID) != SUCCESS)
        return FAILURE;

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
            OsalPutRingBuf(&Uart0RxQueue, (PUNKNOWN)ch);
            rxcntr++;
        }
        else if (status == IIR_TX_EMPTY)
        {
            while (txcntr < UART_FIFO_SIZE)
            {
                if (OsalGetRingBuf(&Uart0TxQueue, (PUNKNOWN) &ch) != SUCCESS)
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

/** This task handles UART0 Tx requests
 */
void Uart0TxTask(void *param)
{
    U32 data;

    while (1)
    {
        // Wait for console data available
        KS_GetQueueDataW(ConOutQueue, (void *)&data);
        // Put it into UART0 TX queue
        OsalPutRingBuf(&Uart0TxQueue, (PUNKNOWN)data);
//        KS_PutQueueDataW(Uart0TxQueue, (void *)&data);
        // Enable TX Empty interrupt
        IOWriteOr32A(UART_IER, IER_ETBEI);
    }
}

/*! \brief This task handles UART0 Rx requests
*/
void Uart0RxTask(void *param)
{
    while (1)
    {
        OsalWaitForSema(&Uart0RxSema, OSAL_WAIT_INFINITE);
    }
}

RESULTCODE Uart0Write(U32 size, PUNKNOWN data)
{
    return SUCCESS;
}

RESULTCODE Uart1Write(U32 size, PUNKNOWN data)
{
    return SUCCESS;
}

#endif /* USE_RTXC */
