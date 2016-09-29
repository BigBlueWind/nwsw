/** @file icpu.c
 *
 * @brief Inter-CPU communication
 * @author Mindspeed Technologies
 * @version $Revision: 1.7 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_BARE

#include "icpu.h"
#include "isr.h"
#include "intc.h"
#include "osal.h"

//
// Task IDs
//
OSAL_TASK IcpuRxTaskID;
OSAL_TASK IcpuTxTaskID;

//
// Semaphores
//
OSAL_SEMAPHORE IcpuRxSema;
OSAL_SEMAPHORE IcpuTxSema;

//
// Forward declarations
//
void Ptp0IntHandler(void);
void Ptp1IntHandler(void);
void Ptp2IntHandler(void);
void IcpuTxTask(PTR data);
void IcpuRxTaskDirect(PTR data);
RESULTCODE IcpuSendMsgDirect(U32 size, PTR data);

void (*IcpuRxTask)(PTR data) = IcpuRxTaskDirect;
RESULTCODE (*IcpuSendMsg)(U32 size, PTR data) = IcpuSendMsgDirect;

/** Initialize InterCPU
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
RESULTCODE IcpuInit(U32 cpuid)
{
    if (cpuid == UARM)
    {
        IsrAddIrqHandler(INTC_PTP1, Ptp1IntHandler);
        IntcClearInt(INTC_PTP1);
        IntcEnableIrq(UARM, INTC_PTP1);
    }
    else if (cpuid == LARM)
    {
        IsrAddIrqHandler(INTC_PTP0, Ptp0IntHandler);
        IntcClearInt(INTC_PTP0);
        IntcEnableIrq(LARM, INTC_PTP0);

        // DSP to Lower ARM
        IsrAddIrqHandler(INTC_PTP2, Ptp2IntHandler);
        IntcClearInt(INTC_PTP2);
        IntcEnableIrq(LARM, INTC_PTP2);
    }

    if (OsalCreateTask(&IcpuRxTaskID, IcpuRxTask, NULL, "IcpuRxTask",
                       OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
    {
        return FAILURE;
    }
    OsalActivateTask(&IcpuRxTaskID);

    if (OsalCreateTask(&IcpuTxTaskID, IcpuTxTask, NULL, "IcpuTxTask",
                       OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
    {
        return FAILURE;
    }
    OsalActivateTask(&IcpuTxTaskID);

    return SUCCESS;
}

/** Stop I-CPU processing
 *
 */
RESULTCODE IcpuDone(void)
{
    if (IcpuRxTaskID)
    {
        OsalDestroyTask(&IcpuRxTaskID);
        IcpuRxTaskID = NULL;
    }
    if (IcpuTxTaskID)
    {
        OsalDestroyTask(&IcpuTxTaskID);
        IcpuTxTaskID = NULL;
    }
    return SUCCESS;
}

/** @brief Upper ARM to Lower ARM interrupt handler
 *
 */
void Ptp0IntHandler(void)
{
}

/** @brief Lower ARM to Upper ARM interrupt handler
 *
 */
void Ptp1IntHandler(void)
{
}

/** @brief DSP to Lower ARM interrupt handler
 *
 */
void Ptp2IntHandler(void)
{
}

/** This task handles InterCPU Rx requests
 *
 * @param data Not used
 */
void IcpuRxTaskDirect(PTR data)
{
    while (1)
    {
        if (OsalWaitForSema(&IcpuRxSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            // TODO: Do something
        }
    }
}

/** This task handles InterCPU Tx requests
 *
 * @param data Not used
 */
void IcpuTxTask(PTR data)
{
    while (1)
    {
        if (OsalWaitForSema(&IcpuTxSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            // TODO: Do something
        }
    }
}

/** Send message to peer CPU
 *
 * @param size Size of message
 * @param data Pointer to data
 * @return Standard result code
 */
RESULTCODE IcpuSendMsgDirect(U32 size, PTR data)
{
    // TODO: Enqueue
    OsalRaiseSema(&IcpuTxSema);

    return SUCCESS;
}

#endif /* USE_BARE */
