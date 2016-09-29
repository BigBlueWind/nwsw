/** @file phi.c
 *
 * @brief PCI/PCIe Host Interface for Linux
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_LINUX

#include "phi.h"
#include "osal.h"
#include <fcntl.h>
#include <unistd.h>

#ifdef SIMUPC
#define PHI_DEVNAME                 "/dev/null"
#else /* SIMUPC */
#define PHI_DEVNAME                 "/dev/pci"
#endif /* PHI_DEVNAME */

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
// Device descriptors
//
int PhiHandle = -1;

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
#if 0
    U32 blkSize = RoundUp(sizeof(PHI_MESSAGE), OSAL_CACHELINE_SIZE);

    if (OsalCreateMemArray(&PhiRxArray, PhiRxArrayData, blkSize * PHI_RXQ_SIZE,
                           blkSize) != SUCCESS)
    {
        return FAILURE;
    }
    if (OsalCreateMemArray(&PhiTxArray, PhiTxArrayData, blkSize * PHI_TXQ_SIZE,
                           blkSize) != SUCCESS)
    {
        return FAILURE;
    }

    PhiHandle = open(PHI_DEVNAME, O_RDWR);
    if (PhiHandle < 0)
    {
        // TODO: Handle error
        return FAILURE;
    }

    if (OsalCreateTask(&PhiRxTaskID, PhiRxTask, NULL, "PhiRx",
                       OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
    {
        return FAILURE;
    }
    OsalActivateTask(&PhiRxTaskID);
#endif /* 0 */
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
    ssize_t count;

    while (size)
    {
        count = write(PhiHandle, data, (size_t) size);
        if (count <= 0)
            return FAILURE;
        size -= count;
    }
    return SUCCESS;
}

/*! \brief PHI Task
*/
void PhiRxTask(void *param)
{
    OSAL_PADDRESS pBlock;
    ssize_t count;

    while (1)
    {
        if (OsalAllocMemArray(&PhiRxArray, &pBlock) == FAILURE)
        {
            // TODO: Handle error
            break;
        }
        // Blocking read
        if ((count = read(PhiHandle, pBlock, sizeof(PHI_MESSAGE))) > 0)
        {
            // TODO: dispatch
        }
        // TODO: garbage collector?
        OsalFreeMemArray(&PhiRxArray, pBlock);
    }
}

#endif /* USE_LINUX */
