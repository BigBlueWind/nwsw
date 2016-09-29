/** @file uart.c
 *
 * @brief UART module for Linux
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_LINUX

#include "uart.h"
#include "osal.h"

#ifdef SIMUPC
#define UART0_DEVNAME               "/dev/ttyS0"
#define UART1_DEVNAME               "/dev/ttyS1"
#else /* SIMUPC */
#define UART0_DEVNAME               "/dev/ttyAMA0"
#define UART1_DEVNAME               "/dev/ttyAMA1"
#endif

OSAL_SEMAPHORE Uart0RxSema;
OSAL_SEMAPHORE Uart1RxSema;
OSAL_RINGBUF Uart0RxQueue;
OSAL_RINGBUF Uart0TxQueue;

U32 Uart0RxQData[UART_RXQ_SIZE];
U32 Uart0TxQData[UART_TXQ_SIZE];
U32 ConOutQData[UART_TXQ_SIZE];
U32 ConInQData[UART_RXQ_SIZE];

//
// Device descriptors
//
int Uart0Handle = -1;
int Uart1Handle = -1;

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
    Uart0Handle = open(UART0_DEVNAME, O_RDWR);
    if (Uart0Handle < 0)
    {
        // TODO: Ensure we do not print to the UART
        fprintf(stderr, "ERROR: Unable to open UART0 [%i]\n", errno);
        return FAILURE;
    }

    if (OsalCreateTask(&Uart0TaskID, Uart0RxTask, NULL, "Uart0Rx",
                   OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
    {
        return FAILURE;
    }
    OsalActivateTask(&Uart0TaskID);

    return SUCCESS;
}

/*! \brief This task handles UART0 Rx requests
*/
void Uart0RxTask(void *param)
{
    char buffer[256];
    ssize_t count, i;

    while (1)
    {
        count = read(Uart0Handle, buffer, sizeof(buffer));
        if (count > 0)
        {
            for (i = 0; i < count; i++)
            {
                // TODO: parse buffer[i]
            }
        }
    }
}

/*! \brief This task handles UART1 Rx requests
*/
void Uart1RxTask(void *param)
{
    char buffer[256];
    ssize_t count, i;

    while (1)
    {
        count = read(Uart1Handle, buffer, sizeof(buffer));
        if (count > 0)
        {
            for (i = 0; i < count; i++)
            {
                // TODO: parse buffer[i]
            }
        }
    }
}

RESULTCODE Uart0Write(U32 size, PTR data)
{
    ssize_t count;

    while (size)
    {
        count = write(Uart0Handle, data, (size_t) size);
        if (count <= 0)
            return FAILURE;
        size -= count;
    }
    return SUCCESS;
}

RESULTCODE Uart1Write(U32 size, PTR data)
{
    ssize_t count;

    while (size)
    {
        count = write(Uart1Handle, data, (size_t) size);
        if (count <= 0)
            return FAILURE;
        size -= count;
    }
    return SUCCESS;
}

#endif /* USE_LINUX */
