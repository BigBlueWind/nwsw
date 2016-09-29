/** @file icpu.c
 *
 * @brief I-CPU communication for Windows
 * @author Mindspeed Technologies
 * @version $Revision: 1.45 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_WINDOWS

#include "osal.h"
#include "bsp.h"
#include "icpu.h"
#include "msgroute.h"
#include "apidefs.h"
#include "settings.h"
#include "turnerio.h"
#include "t4kcom.h"
#include "diags.h"

// Used in SHM mode
#define ICPU_SHMKEY                 _T("Global\\c4kfwshm")
#define ICPU_SHMSIZE                (MSG_MAXSIZE*128)

#if defined (BSMAC) || defined (ENODEB)
#define ICPU_SHMFLAGS               (IPC_CREAT)
#define ICPU_SHMTXOFFSET            0
#define ICPU_SHMRXOFFSET            (ICPU_SHMSIZE/2)
#define ICPU_SHMTXEND               (ICPU_SHMSIZE/2)
#define ICPU_SHMRXEND               ICPU_SHMSIZE
#define ICPU_RXSEMA                 _T("Global\\c4kfwrxsem")
#define ICPU_TXSEMA                 _T("Global\\c4kfwtxsem")
#else
#define ICPU_SHMFLAGS               0
#define ICPU_SHMTXOFFSET            (ICPU_SHMSIZE/2)
#define ICPU_SHMRXOFFSET            0
#define ICPU_SHMTXEND               ICPU_SHMSIZE
#define ICPU_SHMRXEND               (ICPU_SHMSIZE/2)
#define ICPU_RXSEMA                 _T("Global\\c4kfwtxsem")
#define ICPU_TXSEMA                 _T("Global\\c4kfwrxsem")
#endif /* BSMAC || ENODEB */

//
// Task IDs
//
OSAL_TASK IcpuRxTaskID;
OSAL_TASK IcpuRxTaskIDAck;

//
// Device descriptors
//
HANDLE IcpuHandle = NULL;
HANDLE IcpuRxSema = NULL;
HANDLE IcpuTxSema = NULL;
OSAL_PADDRESS IcpuShmAddr = NULL;
OSAL_PADDRESS IcpuShmTxAddr = NULL;
OSAL_PADDRESS IcpuShmRxAddr = NULL;
extern OSAL_MEMARRAY MsgArray;

IcpuGetPhysCb IcpuGetPhys = NULL;
IcpuGetVirtCb IcpuGetVirt = NULL;

//
// Mode pointers
//
void (*IcpuRxTask)(PTR data) = NULL;
RESULTCODE (*IcpuSendMsg)(U32 size, PTR data) = NULL;
PTR (*IcpuGetTxMsg)(void) = NULL;

//
// Mode of operation
//
int IcpuMode = IMODE_RELEASE;
RESULTCODE IcpuT4KDeviceSend(U32 msgSize, PTR data);
void IcpuRxTaskT4KDevice(PTR data);
void IcpuRxTaskT4KDeviceAck(PTR data);
RESULTCODE IcpuSendDevice(U32 msgSize, PTR data);
RESULTCODE IcpuSendBypass(U32 msgSize, PTR data);
void IcpuRxTaskShm(PTR data);
void IcpuRxTaskDevice(PTR data);
void IcpuRxTaskBypass(PTR data);

/** Initialize I-CPU
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
OSAL_QUEUE TxQueue;

RESULTCODE IcpuInit(U32 cpuid)
{
    int temp;

    MsgFreeBuffer = Msg1FreeBuffer;
    MsgAllocBuffer = Msg1AllocBuffer;

    // "useshm" is compatibility setting
    ReadSettingInt("useshm", &IcpuMode, IMODE_SHM);
    // "icpu" is a newer spelling
    ReadSettingInt("icpu", &temp, IcpuMode);
    // New spelling takes precedence over old one
    if (temp != IcpuMode)
        IcpuMode = temp;

    //
    // First, check if we want to use shared memory (default for Windows)
    //
    if (IcpuMode == IMODE_SHM)
    {
        if (IcpuSendMsg == NULL)
        {
            IcpuHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, ICPU_SHMSIZE, ICPU_SHMKEY);
            IcpuRxSema = CreateSemaphore(NULL, 0, ICPU_SHMSIZE / MSG_MAXSIZE / 2, ICPU_RXSEMA);
            IcpuTxSema = CreateSemaphore(NULL, 0, ICPU_SHMSIZE / MSG_MAXSIZE / 2, ICPU_TXSEMA);
            if (IcpuHandle == NULL || IcpuRxSema == NULL || IcpuTxSema == NULL)
            {
                DiagPrint(DIAG_LEVEL1, "ICPU", "Unable to access shared memory [%i]\n", errno);
                IcpuMode = IMODE_RELEASE;
            }
            else
            {
                IcpuShmAddr = MapViewOfFile(IcpuHandle, FILE_MAP_ALL_ACCESS, 0, 0, ICPU_SHMSIZE);
                if (IcpuShmAddr == NULL)
                {
                    DiagPrint(DIAG_LEVEL1, "ICPU", "Unable to attach to the shared memory [%i]\n", GetLastError());
                    IcpuMode = IMODE_RELEASE;
                }
                else
                {
                    IcpuShmTxAddr = (OSAL_PADDRESS) ((uintptr_t)IcpuShmAddr + ICPU_SHMTXOFFSET);
                    IcpuShmRxAddr = (OSAL_PADDRESS) ((uintptr_t)IcpuShmAddr + ICPU_SHMRXOFFSET);
                    IcpuRxTask = IcpuRxTaskShm;
                    IcpuSendMsg = IcpuSendShm;
                    DiagPrint(DIAG_LEVEL2, "ICPU", "Shared memory mode\n");
                }
            }
        }
    }

    if (IcpuMode == IMODE_RELEASE)
    {
        if (IcpuSendMsg == NULL)
        {
            DiagPrint(DIAG_LEVEL2, "ICPU", "Interface disabled\n");
        }
    }
    else if(IcpuMode == IMODE_DIRECT)
    {
        OSAL_PADDRESS Head;
        if (OsalAllocHeap(&OsalGlobalHeap,
                          &Head,
                          6 * sizeof(PTR)) != SUCCESS)
        {
            DiagPrint(DIAG_LEVEL0, "ICPU", "No memory [%s,%i]\n", __FUNCTION__, __LINE__);
            return FAILURE;
        }
        if (OsalCreateQueue(&TxQueue,Head,6 * sizeof(PTR)) != SUCCESS)
        {
           return FAILURE;
        }
        if (IcpuSendMsg == NULL)
        {
            IcpuRxSema = CreateSemaphore(NULL, 0, ICPU_SHMSIZE / MSG_MAXSIZE / 2, ICPU_RXSEMA);
            IcpuTxSema = CreateSemaphore(NULL, 0, ICPU_SHMSIZE / MSG_MAXSIZE / 2, ICPU_TXSEMA);
            if (IcpuRxSema == NULL || IcpuTxSema == NULL)
            {
                return FAILURE;
            }
            else
            {
                IcpuSendMsg = IcpuTxPut;
                IcpuGetTxMsg = IcpuTxGet;
            }
        }
    }
    else if (IcpuMode == IMODE_HBI)
    {
        if (T4KComOpen(WDM_DRIVER) != FAILURE)
        {
            // FIXME: Do not use INI file
            char buf[256] = {0};
            static const char* confFileName = "c:\\t4kphy.ini";
            FILE *f = fopen(confFileName, "r");
            if(f == NULL) 
            {
                DiagPrint(DIAG_LEVEL0, "ICPU", "ERROR %d opening configuration file <%s>\n",
                          GetLastError(), confFileName);
                return FAILURE;
            }   
            fscanf(f, "%s", buf);
            fclose(f);

            ///!!!!!!!!
            ///!!!!!!!! note - supervisor ready are received inside T4KLoadImage
            ///!!!!!!!!
            if (T4KLoadLowArmImage(buf) != SUCCESS)
                return FAILURE;

            if (IcpuSendMsg == NULL)
            {
                IcpuRxTask = IcpuRxTaskT4KDevice;
                IcpuSendMsg = IcpuT4KDeviceSend;
                DiagPrint(DIAG_LEVEL2, "ICPU", "HBI memory mode\n");
                OsalCreateTask(&IcpuRxTaskID, IcpuRxTaskT4KDeviceAck, NULL, "IcpuRxTaskAck",
                               OSAL_PRIORITY_NORMAL, 0, 0);
                OsalActivateTask(&IcpuRxTaskID);
            }
        }
    }

    if (IcpuRxTask)
    {
        OsalCreateTask(&IcpuRxTaskID, IcpuRxTask, NULL, "IcpuRxTask", OSAL_PRIORITY_NORMAL, 0, 0);
        OsalActivateTask(&IcpuRxTaskID);
    }

    return SUCCESS;
}

/** This task dispatches data from L-ARM
 *  Use release mode (direct access to the device)
 *
 * @param data Not used
 */
void IcpuRxTaskDevice(PTR data)
{
}

/** This task dispatches data from L-ARM using shared memory
 *
 * @param data Not used
 */
void IcpuRxTaskShm(PTR data)
{
    PMSGHEADER pHdr;
    PTR rxBuffer;

    while (1)
    {
        if (OsalAllocMemArray(&MsgArray, &rxBuffer) == SUCCESS)
        {
            if (WaitForSingleObject(IcpuRxSema, INFINITE) == WAIT_OBJECT_0)
            {
                pHdr = (PMSGHEADER) IcpuShmRxAddr;
                memcpy(rxBuffer, pHdr, sizeof(MSGHEADER) + pHdr->length);

                MsgDispatch(MSGS_LARM, pHdr->length + sizeof(MSGHEADER), rxBuffer);

                IcpuShmRxAddr = (OSAL_PADDRESS) ((uintptr_t)IcpuShmRxAddr + MSG_MAXSIZE);
                if ((uintptr_t)IcpuShmRxAddr >= (uintptr_t)IcpuShmAddr + ICPU_SHMRXEND)
                {
                    IcpuShmRxAddr = (OSAL_PADDRESS)((uintptr_t)IcpuShmAddr + ICPU_SHMRXOFFSET);
                }
            }
            else
            {
                // Stop task
                OsalFreeMemArray(&MsgArray, rxBuffer);
                break;
            }
        }
    }
}

RESULTCODE IcpuT4KDeviceSend(U32 size, PTR data)
{
    RESULTCODE rc;

	rc = T4KSendApiCmd(0, data);
	OsalFreeMemArray(&MsgArray, data);
    return rc; 
}

/** This task dispatches data from L-ARM using shared memory
 *
 * @param data Not used
 */
void IcpuRxTaskT4KDevice(PTR data)
{
    PMSGHEADER pHdr;
    PTR rxBuffer;

    while (1)
    {
        if (OsalAllocMemArray(&MsgArray, &rxBuffer) == SUCCESS)
        {
			pHdr = rxBuffer;
			pHdr->type = MSGT_INDICATION;
            if (T4KRecvIndCmd(0, rxBuffer, 0) == SUCCESS)
            {
                  pHdr = (PMSGHEADER) rxBuffer;
                  MsgDispatch(MSGS_LARM,
                              pHdr->length + sizeof(MSGHEADER),
                              rxBuffer);
            }
            else
            {
                // Stop task
                DiagPrint(DIAG_LEVEL4, "HBI", "ERROR Reading from device\n");
                OsalFreeMemArray(&MsgArray, rxBuffer);
                break;
            }
        }
        else
        {
            DiagPrint(DIAG_LEVEL5, "HBI", "No memory [%s,%i]\n", __FILE__, __LINE__);
            // Try again
            OsalWaitDelay(MEM_RECOVERYTIME);
        }
    }
}


void IcpuRxTaskT4KDeviceAck(PTR data)
{
    PMSGHEADER pHdr;
    PTR rxBuffer;

    while (1)
    {
        if (OsalAllocMemArray(&MsgArray, &rxBuffer) == SUCCESS)
        {
			pHdr = rxBuffer;
			pHdr->type = MSGT_INDICATION;
            if (T4KRecvApiCmd(0, rxBuffer, 0) == SUCCESS)
            {
                  MsgDispatch(MSGS_LARM,
                              pHdr->length + sizeof(MSGHEADER),
                              rxBuffer);
            }
            else
            {
                // Stop task
                DiagPrint(DIAG_LEVEL4, "HBI", "ERROR Reading from device\n");
                OsalFreeMemArray(&MsgArray, rxBuffer);
                break;
            }
        }
        else
        {
            // Try again
            OsalWaitDelay(MEM_RECOVERYTIME);
        }
    }
}

/** Send data to L-ARM
 *  Use release mode (direct access to device)
 *
 * @param size Size of data chunk
 * @param data Pointer to the data
 * @return Standard result code
 */
RESULTCODE IcpuSendDevice(U32 size, PTR data)
{
    OsalFreeMemArray(&MsgArray, data);
    return SUCCESS;
}

RESULTCODE IcpuSendShm(U32 size, PTR data)
{
    if (IcpuShmTxAddr != NULL)
    {
        memcpy(IcpuShmTxAddr, data, size);
        IcpuShmTxAddr = (OSAL_PADDRESS) ((uintptr_t) IcpuShmTxAddr + MSG_MAXSIZE);
        if ((uintptr_t)IcpuShmTxAddr >= (uintptr_t)IcpuShmAddr + ICPU_SHMTXEND)
        {
            IcpuShmTxAddr = (OSAL_PADDRESS)((uintptr_t)IcpuShmAddr + ICPU_SHMTXOFFSET);
        }
        ReleaseSemaphore(IcpuTxSema, 1, NULL);
    }
    OsalFreeMemArray(&MsgArray, data);

    return SUCCESS;
}

RESULTCODE IcpuTxPut(U32 size, PTR data)
{
    OsalEnqueue(&TxQueue, data);
    ReleaseSemaphore(IcpuTxSema, 1, NULL);
    return SUCCESS;
}

EXPORT PTR IcpuTxGet(void)
{
    PTR data;
    OsalDequeue(&TxQueue,&data);
    return data;
}


/** Stop I-CPU
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
RESULTCODE IcpuDone(void)
{
    printf(C4KFW "Stopping I-CPU interface... ");
    if (IcpuRxTaskID)
    {
        OsalDeactivateTask(&IcpuRxTaskID);
        OsalDestroyTask(&IcpuRxTaskID);
        IcpuRxTaskID = 0;
    }
    if (IcpuShmAddr != NULL)
    {
        UnmapViewOfFile(IcpuShmAddr);
        IcpuShmAddr = NULL;
    }
    if (IcpuHandle)
    {
        CloseHandle(IcpuHandle);
        IcpuHandle = NULL;
    }
    if (IcpuRxSema)
    {
        CancelIo(IcpuRxSema);
        CloseHandle(IcpuRxSema);
        IcpuRxSema = NULL;
    }
    if (IcpuTxSema)
    {
        CancelIo(IcpuTxSema);
        CloseHandle(IcpuTxSema);
        IcpuTxSema = NULL;
    }

    T4KComClose();
    printf("Done\n");

    return SUCCESS;
}

#endif /* USE_WINDOWS */
