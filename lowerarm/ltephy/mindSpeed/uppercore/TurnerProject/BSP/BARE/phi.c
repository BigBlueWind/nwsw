/** @file phi.c
 *
 * @brief PCI/PCIe Host Interface for bare metal
 * @author Mindspeed Technologies
 * @version $Revision: 1.4 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_BARE

#include "phi.h"
#include "apb.h"
#include "portio.h"
#include "isr.h"
#include "intc.h"
#include "osal.h"

OSAL_RINGBUF PhiRxQueue;        // From host (be aware of register names!)
OSAL_RINGBUF PhiTxQueue;        // To host
OSAL_MEMARRAY PhiRxArray;       // From host
OSAL_MEMARRAY PhiTxArray;       // To host
OSAL_SEMAPHORE PhiRxSema;       // From host
OSAL_PADDRESS PhiRxQData[PHI_RXQ_SIZE];
OSAL_PADDRESS PhiTxQData[PHI_TXQ_SIZE];
U32 PhiRxArrayData[RoundUp(sizeof(PHI_MESSAGE), OSAL_CACHELINE_SIZE)*PHI_RXQ_SIZE/sizeof(U32)];
U32 PhiTxArrayData[RoundUp(sizeof(PHI_MESSAGE), OSAL_CACHELINE_SIZE)*PHI_TXQ_SIZE/sizeof(U32)];

//
// Tasks
//
OSAL_TASK PhiRxTaskID;

/** Initialize PCI Host Interface
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
RESULTCODE PhiInit(U32 cpuid)
{
    U32 blkSize = RoundUp(sizeof(PHI_MESSAGE), OSAL_CACHELINE_SIZE);

    if (OsalCreateRing(&PhiRxQueue, PhiRxQData,
                       PHI_RXQ_SIZE * sizeof(OSAL_PADDRESS)) != SUCCESS)
        return FAILURE;
    if (OsalCreateRing(&PhiTxQueue, PhiTxQData,
                       PHI_TXQ_SIZE * sizeof(OSAL_PADDRESS)) != SUCCESS)
        return FAILURE;
    if (OsalCreateMemArray(&PhiRxArray, PhiRxArrayData, blkSize * PHI_RXQ_SIZE,
                           blkSize) != SUCCESS)
        return FAILURE;
    if (OsalCreateMemArray(&PhiTxArray, PhiTxArrayData, blkSize * PHI_TXQ_SIZE,
                           blkSize) != SUCCESS)
        return FAILURE;
    if (OsalCreateSema(&PhiRxSema) != SUCCESS)
        return FAILURE;

    // Enable Tx & Rx
    IOWriteOr32A(PHI_INT_CTRL, PHI_TXM3IE | PHI_RXM3IE);

    IsrAddIrqHandler(INTC_HIMBOX, PhiIntHandler);
    IntcClearInt(INTC_HIMBOX);
    IntcEnableIrq(cpuid, INTC_HIMBOX);

    if (OsalCreateTask(&PhiRxTaskID, PhiRxTask, NULL, "PhiRx",
                       OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
    {
        return FAILURE;
    }
    OsalActivateTask(&PhiRxTaskID);

    return SUCCESS;
}

/*! \brief PHI driver low level interface
*/
RESULTCODE PhiSend(PHI_MESSAGE *msg)
{
    U32 size = msg->size;           // Already in U32
    U32 *ptr = msg->fifo.fifo32;

    // Write FIFO (note about TX/RX direction)
    while (size--)
        IOWrite32A(PHI_RX_FIFO_DATA32, *ptr++);

    // Write Mailbox registers (mail3 will trigger IRQ)
    IOWrite16A(PHI_RX_MAIL0, msg->mail0);
    IOWrite16A(PHI_RX_MAIL1, msg->mail1);
    IOWrite16A(PHI_RX_MAIL2, msg->mail2);
    IOWrite16A(PHI_RX_MAIL3, msg->mail3);

    return SUCCESS;
}

/** Write data to the Host interface
 *
 * @param size
 * @param data
 * @return
 */
RESULTCODE PhiWrite(U32 size, PTR data)
{
    PHI_MESSAGE *msg;
    U32 *dataPtr = (U32*) data;
    int i;

    size = RoundUp32(size);
    size /= sizeof(U32);
    if (size == 0 || size > PHI_FIFO_SIZE32)
        return FAILURE;

    if (OsalAllocMemArray(&PhiTxArray, (OSAL_PADDRESS*)&msg) != SUCCESS)
        return FAILURE;

    msg->size = size;
#ifdef M823XX
    msg->mail0 = 0;                     // Reserved: set to zero
    msg->mail1 = 0xFFFF;                // Channel number (supervisor)
    msg->mail2 = size * sizeof(U32);    // Number of bytes in FIFO (+ padding)
    msg->mail3 = 0x000B;                // BMR: Command, Miro, EOF
#endif /*M823XX*/

    // Write possible padding
    msg->fifo.fifo32[size-1] = 0;

    // TODO: This is big performance drop. Invent a way to reuse existing data
    for (i = 0; i < size; i++)
        msg->fifo.fifo32[i] = *dataPtr++;

    if (OsalPutRing(&PhiTxQueue, msg) != SUCCESS)
    {
        OsalFreeMemArray(&PhiTxArray, msg);
        return FAILURE;
    }

    // Enable RX Empty INT -- send pending message
    IOWriteOr32A(PHI_INT_CTRL, PHI_RXEEI);

    return SUCCESS;
}

/*! \brief PHI Task
*/
void PhiRxTask(void *param)
{
    while (1)
    {
        // TODO: Implement garbage collector
        OsalWaitForSema(&PhiRxSema, OSAL_WAIT_INFINITE);
    }
}

/** Handler for PHI interrupts
 */
void PhiIntHandler(void)
{
    PHI_MESSAGE *msg;
    U32 length, i;

    // Received message from host?
    if (/*(IORead32A(PHI_INT_CTRL) & PHI_TXM3IE) &&*/
        (IORead32A(PHI_INT_STATUS) & PHI_TXM3I))
    {
        if (OsalAllocMemArrayIsr(&PhiRxArray, (OSAL_PADDRESS*)&msg) == SUCCESS)
        {
            msg->size = RoundUp32(IORead32A(PHI_TX_FIFO_SIZE));
            msg->mail0 = IORead16A(PHI_TX_MAIL0);
            msg->mail1 = IORead16A(PHI_TX_MAIL1);
            msg->mail2 = IORead16A(PHI_TX_MAIL2);
            msg->mail3 = IORead16A(PHI_TX_MAIL3);
            // Convert length in bytes into length in words
            length = msg->size / sizeof(U32);
            // TODO: Do we really need this padding here ?
            msg->fifo.fifo32[length-1] = 0;
            for (i = 0; i < length; i++)
                msg->fifo.fifo32[i] = IORead32A(PHI_TX_FIFO_DATA32);
        }
        // Inform Host we've done
        IOWrite32A(PHI_INT_ACK, PHI_TXM3IAK);
    }

    // RX Empty?
    if (/*(IORead32A(PHI_INT_CTRL) & PHI_RXEEI) &&*/
        (IORead32A(PHI_INT_STATUS) & PHI_RXE))
    {
        if (OsalGetRing(&PhiTxQueue, (OSAL_PADDRESS*) &msg) == SUCCESS)
        {
            PhiSend(msg);
            // TODO: Make it IRQ-safe
//          OsalFreeMemArray(&PhiTxArray, msg);
        }
        else
        {
            // Disable RX Empty INT
            IOWriteAnd32A(PHI_INT_CTRL, ~(PHI_RXEEI));
        }
    }

    // Check if Host is ready for the next msg
    if (/*(IORead32A(PHI_INT_CTRL) & PHI_RXM3IE) &&*/
        (IORead32A(PHI_INT_STATUS) & PHI_RXM3I))
    {
        // Inform Host we've done
        IOWrite32A(PHI_INT_ACK, PHI_RXM3IAK);
        if (IORead32A(PHI_RX_FIFO_LEVEL) == 0)
        {
            // TODO: Send next chunk of data ?
        }
        else
        {
            // Inform Host we've done
            IOWrite32A(PHI_INT_ACK, PHI_RXEIAK);
            // Enable RX Empty INT
            IOWriteOr32A(PHI_INT_CTRL, PHI_RXEEI);
        }
    }
}

#endif /* USE_BARE */
