/** @file spusched.c
 *
 * @brief SPU scheduler
 * @author Mindspeed Technologies
 * @version $Revision: 1.8 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "spusched.h"
#include "osal.h"

//
// Queues
//
OSAL_QUEUE SpuTxQueue;
OSAL_QUEUE SpuRxQueue;

//
// Tasks
//
OSAL_TASK SpuRxTaskID;
OSAL_TASK SpuTxTaskID;

//
// Events
//
OSAL_SEMAPHORE SpuRxSema;
OSAL_SEMAPHORE SpuTxSema;

//
// Queue memory placeholders
//
U32 SpuRxQMem[SPU_RXQSIZE];
U32 SpuTxQMem[SPU_TXQSIZE];

/**
 *
 * @param
 * @return Standard result code
 */
RESULTCODE SpuInit(void)
{
    if (OsalCreateSema(&SpuRxSema) != SUCCESS)
    {
        return FAILURE;
    }

    if (OsalCreateSema(&SpuTxSema) != SUCCESS)
    {
        return FAILURE;
    }

    if (OsalCreateQueue(&SpuRxQueue, SpuRxQMem, SPU_RXQSIZE * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        return FAILURE;
    }

    if (OsalCreateQueue(&SpuTxQueue, SpuTxQMem, SPU_TXQSIZE * sizeof(OSAL_PADDRESS)) != SUCCESS)
    {
        return FAILURE;
    }

    if (OsalCreateTask(&SpuRxTaskID, SpuRxTask, NULL, "SpuRxTask",
                       OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
    {
        return FAILURE;
    }
    OsalActivateTask(&SpuRxTaskID);

    if (OsalCreateTask(&SpuTxTaskID, SpuTxTask, NULL, "SpuTxTask",
                       OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
    {
        return FAILURE;
    }
    OsalActivateTask(&SpuTxTaskID);

    return SUCCESS;
}

/**
 *
 * @param data
 */
void SpuRxTask(PTR data)
{
    while (1)
    {
        if (OsalWaitForSema(&SpuRxSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            // TODO: Do something
        }
    }
}

/**
 *
 * @param data
 */
void SpuTxTask(PTR data)
{
    while (1)
    {
        if (OsalWaitForSema(&SpuTxSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            // TODO: Do something
        }
    }
}
