/** @file icpu.c
 *
 * @brief I-CPU communication for Linux
 * @author Mindspeed Technologies
 * @version $Revision: 1.56 $
 *
 * COPYRIGHT(c) 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_LINUX

#include "bsp.h"
#include "basetypes.h"
#include "osal.h"
#include "icpu.h"
#include "icpu2_lnx.h"
#include "msgroute.h"
#include "apidefs.h"
#include "settings.h"
#include "diags.h"
#include "sysctrl.h"
#include <sys/mman.h>
#include "timers.h"
#include <sys/ioctl.h>

//
// Device IO Control
//
#define IOCTL_CREATE_MBOX           1
#define IOCTL_ALLOC_BLOCK           2
#define IOCTL_FREE_BLOCK            3

#define ICPU_DEVNAME                "/dev/lowarm"
#define MEM_DEVNAME                 "/dev/mem"

// Used in DEMO mode
#define ICPU_TXQUEUE                "/fromupper"
#define ICPU_RXQUEUE                "/toupper"

// Used in SHM mode
#define ICPU_SHMKEY                 "/c4kfwshm"
#define ICPU_SHMSIZE                (MSG_MAXSIZE*12)

#if defined (BSMAC) || defined (ENODEB)
#define ICPU_SHMFLAGS               (IPC_CREAT)
#define ICPU_SHMTXOFFSET            0
#define ICPU_SHMRXOFFSET            (ICPU_SHMSIZE/2)
#define ICPU_SHMTXEND               (ICPU_SHMSIZE/2)
#define ICPU_SHMRXEND               ICPU_SHMSIZE
#define ICPU_RXSEMA                 "/c4kfwrxsem"
#define ICPU_TXSEMA                 "/c4kfwtxsem"
#else
#define ICPU_SHMFLAGS               0
#define ICPU_SHMTXOFFSET            (ICPU_SHMSIZE/2)
#define ICPU_SHMRXOFFSET            0
#define ICPU_SHMTXEND               ICPU_SHMSIZE
#define ICPU_SHMRXEND               (ICPU_SHMSIZE/2)
#define ICPU_RXSEMA                 "/c4kfwtxsem"
#define ICPU_TXSEMA                 "/c4kfwrxsem"
#endif /* BSMAC || ENODEB */

#ifdef C4KEVM
// Direct (memory-mapped) mode
//#define ICPU_DESC_BASE              0x09800000  // Physical address of descriptor
#define ICPU_DESC_BASE              0x1FA00000    // Physical address of descriptor
#define ICPU_DESC_BASE_HI              0x3FA00000    // Physical address of descriptor
#define ICPU_DESC_BASE_HI2              0x3F000000    // Physical address of descriptor
#define ICPU_QENTRY_SIZE            sizeof(U32)
#define ICPU_RXPART_SIZE            (IcpuRxEntries * MSG_MAXSIZE)
#define ICPU_TXPART_SIZE            (IcpuTxEntries * MSG_MAXSIZE)
#define ICPU_RXQUEUE_SIZE           (IcpuRxEntries * ICPU_QENTRY_SIZE)
#define ICPU_TXQUEUE_SIZE           (IcpuTxEntries * ICPU_QENTRY_SIZE)
#define ICPU_DESC_SIZE              0x00100000  // Descriptors segment size
#define ICPU_RXPART_BASE            ((uintptr_t)IcpuDescBase+ICPU_DESC_SIZE)
#define ICPU_TXPART_BASE            (ICPU_RXPART_BASE+ICPU_RXPART_SIZE)
#define ICPU_QUEUE_BASE             (ICPU_TXPART_BASE+ICPU_TXPART_SIZE)
#define ICPU_RX_DESC                0           // Index of RX descriptor in array
#define ICPU_TX_DESC                1           // Index of TX descriptor in array
#define ICPU_RDMEM                  (32+5)      // Interrupt number to L-ARM from RdMem
#define ICPU_TX_INT                 (32+6)      // Interrupt number to L-ARM
#define ICPU_RX_INT                 (32+7)      // Interrupt number from L-ARM
#define UIO_MAX                     8           // Max number of User I/O drivers in system
#ifdef CACHED_ICPU
#define UIO_CACHED_MAX              2           // Max number of Cached segments
#endif

#define ICPU_GET_INDEX  _IOWR('I', 2, unsigned long)
#define ICPU_GET_BASE	_IOR('I', 3, unsigned long)
#define ICPU_LARM_CRASH _IOWR('I', 4, unsigned long)
#define ICPU_SET_IRQ    _IOWR('I', 5, unsigned long)
#define ICPU_GET_IRQ    _IOWR('I', 6, unsigned long)

#ifdef CACHED_ICPU
/* uarm.h */
#define UARM_IOCTL_SHM_CREATE        _IOWR('U', 1, unsigned int*)
#define UARM_IOCTL_CACHE_CLEAN            _IOR ('U', 2, unsigned int*)
#define UARM_IOCTL_CACHE_INVALIDATE        _IOR ('U', 3, unsigned int*)
#define UARM_IOCTL_CACHE_FLUSH         _IOR ('U', 4, unsigned int*)
#endif

ICPU_STATS IcpuStats;

#endif /* C4KEVM */

//
// Task IDs
//
OSAL_TASK IcpuRxTaskID;

//
// Device descriptors
//
int IcpuHandle = -1;
int DevMemMap = -1;

sem_t *IcpuRxSema = SEM_FAILED;
sem_t *IcpuTxSema = SEM_FAILED;
OSAL_MAILBOX IcpuTxMbox = 0;
OSAL_MAILBOX IcpuRxMbox = 0;
PTR IcpuShmAddr = NULL;
PTR IcpuShmTxAddr = NULL;
PTR IcpuShmRxAddr = NULL;
extern OSAL_MEMARRAY MsgArray;

//
// Mode pointers
//
void (*IcpuRxTask)(PTR data) = NULL;
RESULTCODE (*IcpuSendMsg)(U32 size, PTR data) = NULL;

//
// Mode of operation
//
int IcpuMode = IMODE_RELEASE;

#ifdef C4KEVM
//
// Direct mode pointers
//
volatile U32 *IcpuIntc = NULL;                  // Interrupt controller
volatile U32 *IcpuSema = NULL;                  // Hardware semaphores
volatile ICPU_COMM *IcpuDesc[2] = {NULL};       // Array of pointers to descriptors
PTR IcpuTxPartStorage = NULL;                   // TX partition storage
PTR IcpuRxPartStorage = NULL;                   // RX partition storage
PTR IcpuQueueStorage = NULL;                    // Queues storage
volatile int IcpuDirectTask = 0;                // Flag the non-standard task is active
void *IcpuDescBase = (void *)ICPU_DESC_BASE;    // Stores actual physical address of main descriptor
U32 IcpuRxEntries = (U32)ICPU_RXENTRIES;
U32 IcpuTxEntries = (U32)ICPU_TXENTRIES;
volatile U32 *IcpuGic = NULL;

OSAL_MUTEX GicMutex;

#ifdef ICPU_DEBUG_PART
ICPU_BLOCK_DBG AllocIcpuBuffDbg[512] = {0};
U32 AllocIcpuBuffDbgIdx = 0;
U32 AllocIcpuBuffDbgNum = 0;

ICPU_BLOCK_DBG TxIcpuBuffDbg[512] = {0};
U32 TxIcpuBuffDbgIdx = 0;
U32 TxIcpuBuffDbgNum = 0;
#endif

//
// User I/O driver mapping
// Total 8 drivers from /dev/uio0 to /dev/uio7
// Each is responsible for own memory area
//
typedef struct IcpuDrvDesc
{
    int file;
    int refCount;                               // To track munmap call
    PTR mapped;                                 // Contains pointer returned by mmap
    char name[16];                        // Device name
    PTR address;                          // Physical address
    U32 size;                             // Size of covered memory area
} ICPU_DRV_DESC;

ICPU_DRV_DESC UioDrvLo[UIO_MAX] =
{
 {0, 0, NULL, "/dev/uio0", (PTR) DDR0_BASEADDR, DDR_SIZE},
 {0, 0, NULL, "/dev/uio1", (PTR) 0x20000000, DDR_SIZE},   // TODO: Should be DDR1_BASEADDR
 {0, 0, NULL, "/dev/uio2", (PTR) CRAM_BASEADDR, CRAM_SIZE},
 {0, 0, NULL, "/dev/uio3", (PTR) IRAM_BASEADDR, IRAM_SIZE},
 {0, 0, NULL, "/dev/uio4", (PTR) JRAM_BASEADDR, JRAM_SIZE},
 {0, 0, NULL, "/dev/uio5", (PTR) TIMER_APB_BASEADDR, 0x00010000},
 {0, 0, NULL, "/dev/uio6", (PTR) SYS_SEMAPHORE_BASE, 0x00010000},
 {0, 0, NULL, "/dev/uio7", (PTR) CFG_SYS_BASEADDR, 0x00010000},
};

ICPU_DRV_DESC UioDrvHi[UIO_MAX] =
{
 {0, 0, NULL, "/dev/uio0", (PTR) DDR0_BASEADDR, 0x20000000},
 {0, 0, NULL, "/dev/uio1", (PTR) 0x20000000, 0x20000000},
 {0, 0, NULL, "/dev/uio2", (PTR) CRAM_BASEADDR, CRAM_SIZE},
 {0, 0, NULL, "/dev/uio3", (PTR) IRAM_BASEADDR, IRAM_SIZE},
 {0, 0, NULL, "/dev/uio4", (PTR) JRAM_BASEADDR, JRAM_SIZE},
 {0, 0, NULL, "/dev/uio5", (PTR) TIMER_APB_BASEADDR, 0x00010000},
 {0, 0, NULL, "/dev/uio6", (PTR) SYS_SEMAPHORE_BASE, 0x00010000},
 {0, 0, NULL, "/dev/uio7", (PTR) CFG_SYS_BASEADDR, 0x00010000},
};

#ifdef CACHED_ICPU
ICPU_DRV_DESC UioDrvCached[UIO_CACHED_MAX] =
{
 {0, 0, NULL, "/dev/u-arm", (PTR) 0, 0},
 {0, 0, NULL, "/dev/u-arm", (PTR) 0, 0},
};

ICPU_DRV_DESC* CachedUioDrv = UioDrvCached;
#endif

ICPU_DRV_DESC* UioDrv = UioDrvLo;

#endif /* C4KEVM */

//
// Forward declarations
//
RESULTCODE IcpuSendDirect(U32 msgSize, PTR data);
RESULTCODE IcpuSendDevice(U32 msgSize, PTR data);
RESULTCODE IcpuSendMailBox(U32 msgSize, PTR data);
RESULTCODE IcpuSendShm(U32 msgSize, PTR data);
void IcpuRxTaskDirect(PTR data);
void IcpuRxTaskDirectIoctl(PTR data);
void IcpuRxTaskDevice(PTR data);
void IcpuRxTaskMailBox(PTR data);
void IcpuRxTaskShm(PTR data);
#ifdef C4KEVM
PTR IcpuMapIo(PTR addr, U32 len);
void IcpuUnmapIo(PTR addr);
void IcpuLock(V32 *lock);
void IcpuUnlock(V32 *lock);
PTR IcpuAllocPart(ICPU_PART *pPart);
RESULTCODE IcpuEnqueue(volatile ICPU_QUEUE* pQueue, U32 index);
#ifdef CACHED_ICPU
PTR IcpuMapCached(PTR addr, U32 len);
#endif
#endif /* C4KEVM */

IcpuGetPhysCb IcpuGetPhys = NULL;
IcpuGetVirtCb IcpuGetVirt = NULL;
IcpuRxDirectCb IcpuRxDirect = NULL;


/** Initialize I-CPU
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
RESULTCODE IcpuInit(U32 cpuid)
{
    int temp;

    // "useshm" is compatibility setting
    ReadSettingInt("useshm", &IcpuMode, IMODE_DIRECT);
    // "icpu" is a newer spelling
    ReadSettingInt("icpu", &temp, IcpuMode);
    // New spelling takes precedence over old one
    if (temp != IcpuMode)
        IcpuMode = temp;

    MsgAllocBuffer = Msg1AllocBuffer;
    MsgFreeBuffer = Msg1FreeBuffer;

    //
    // First, check if we want to use direct I/O
    //
    if (IcpuMode == IMODE_DIRECT)
    {
#ifdef C4KEVM
        unsigned long icpubaseaddr = ICPU_DESC_BASE_HI2;

        DevMemMap = open(MEM_DEVNAME, O_RDWR | O_SYNC);
        if (DevMemMap < 0) 
        {
            DiagPrint(DIAG_LEVEL1, "ICPU", "DirectMode: Can't open device %s [%i]\n", MEM_DEVNAME, errno);
        }

        if (Icpu2Init() == SUCCESS)
        {
            printf("ICPU2: init ok\n");
            return SUCCESS;
        }

        IcpuGetPhys = Icpu1GetPhys;
        IcpuGetVirt = Icpu1GetVirt;
        IcpuRxDirect = Icpu1RxDirect;

        if (DevMemMap >= 0)
        {
            void *map = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED | MAP_LOCKED, DevMemMap, 0xFFF00000);
            if (map != MAP_FAILED)
            {
                IcpuGic = (volatile unsigned int*)((unsigned long)map + 0x100);
                DiagPrint(DIAG_LEVEL1, "ICPU", "DirectMode: IcpuGic %X\n", IcpuGic);
             }
        }

        IcpuHandle = open(ICPU_DEVNAME, O_RDWR);
        if (IcpuHandle >= 0)
        {
            DiagPrint(DIAG_LEVEL1, "ICPU", "DirectMode: %s is opened\n", ICPU_DEVNAME);
            if (ioctl(IcpuHandle, ICPU_GET_BASE, &icpubaseaddr) == 0)
            {
                DiagPrint(DIAG_LEVEL1, "ICPU", "DirectMode: %s proposed %X base ICPU address\n", ICPU_DEVNAME, icpubaseaddr);
            }
        }
        else
        {
            DiagPrint(DIAG_LEVEL1, "ICPU", "DirectMode: Can't open device %s [%i]\n", ICPU_DEVNAME, errno);
        }

        OsalCreateMutex(&GicMutex);

        // Check descriptor address of fast I-CPU communication
        ReadSettingInt("icpuaddr", (int *) &IcpuDescBase, icpubaseaddr);

        UioDrv = ((U32)IcpuDescBase == ICPU_DESC_BASE)? UioDrvLo : UioDrvHi;

        // Map memory for all descriptors
        IcpuDesc[ICPU_RX_DESC] = IcpuMapIo((PTR) IcpuDescBase, ICPU_DESC_SIZE);

        if (IcpuDesc[ICPU_RX_DESC] == NULL)
        {
            DiagPrint(DIAG_LEVEL0, "ICPU", "Unable to map direct I/O\n");
            if(IcpuHandle >= 0)
            {
                close(IcpuHandle);
                IcpuHandle = -1;
            }
            // Try other modes
            IcpuMode = IMODE_RELEASE;
        }
        else
        {
            // TX descriptor is following RX
            IcpuDesc[ICPU_TX_DESC] = IcpuDesc[ICPU_RX_DESC] + 1;

            IcpuRxEntries = IcpuDesc[ICPU_RX_DESC]->part.count;
            IcpuTxEntries = IcpuDesc[ICPU_TX_DESC]->part.count;

            DiagPrint(DIAG_LEVEL1, "ICPU", "DirectMode: found totally %d Rx blocks num, %d Tx blocks num\n", IcpuRxEntries, IcpuTxEntries);

            if(IcpuRxEntries == 0 || IcpuTxEntries == 0)
            {
                if((U32)IcpuDescBase == ICPU_DESC_BASE_HI)
                {
                    IcpuRxEntries = ICPU_RXENTRIES_HI;
                    IcpuTxEntries = ICPU_TXENTRIES_HI;
                }
                else if((U32)IcpuDescBase == ICPU_DESC_BASE_HI2)
                {
                    IcpuRxEntries = ICPU_RXENTRIES_HI2;
                    IcpuTxEntries = ICPU_TXENTRIES_HI2;
                }
            }

            // Map memory for partitions: Rx-1st, Tx-2nd
#ifdef CACHED_ICPU
            CachedUioDrv[ICPU_RX_DESC].address = (PTR)ICPU_RXPART_BASE;
            CachedUioDrv[ICPU_RX_DESC].size = ICPU_RXPART_SIZE;

            CachedUioDrv[ICPU_TX_DESC].address = (PTR)ICPU_TXPART_BASE;
            CachedUioDrv[ICPU_TX_DESC].size = ICPU_TXPART_SIZE;


            IcpuRxPartStorage = IcpuMapCached((PTR)ICPU_RXPART_BASE, ICPU_RXPART_SIZE);
//            IcpuRxPartStorage = IcpuMapIo((PTR)ICPU_RXPART_BASE, ICPU_RXPART_SIZE);

            IcpuTxPartStorage = IcpuMapCached((PTR)ICPU_TXPART_BASE, ICPU_TXPART_SIZE);
//            IcpuTxPartStorage = IcpuMapIo((PTR)ICPU_TXPART_BASE, ICPU_TXPART_SIZE);

            DiagPrint(DIAG_LEVEL1, "ICPU", "DirectMode: maped storages: Rx=%X Tx=%X\n", IcpuRxPartStorage, IcpuTxPartStorage);
#else
            IcpuRxPartStorage = IcpuMapIo((PTR)ICPU_RXPART_BASE, ICPU_RXPART_SIZE);
            IcpuTxPartStorage = IcpuMapIo((PTR)ICPU_TXPART_BASE, ICPU_TXPART_SIZE);
#endif
            // Map memory for queues
            IcpuQueueStorage = IcpuMapIo((PTR)ICPU_QUEUE_BASE, ICPU_RXQUEUE_SIZE + ICPU_TXQUEUE_SIZE);

            // Semaphores
            IcpuSema = (V32 *)IcpuMapIo((PTR) SYS_SEMAPHORE_BASE, 0x1000);
            // Interrupt controller
            IcpuIntc = (V32 *)IcpuMapIo((PTR) CFG_SYS_BASEADDR, 0x1000);

            memset(&IcpuStats, 0, sizeof(IcpuStats));

#ifdef ICPU_DEBUG_PART
            memset(AllocIcpuBuffDbg, 0, sizeof(AllocIcpuBuffDbg));
            memset(TxIcpuBuffDbg, 0, sizeof(TxIcpuBuffDbg));
#endif
            // Check if L-ARM is loaded
            if (IcpuDesc[ICPU_RX_DESC]->part.storage
                && IcpuDesc[ICPU_TX_DESC]->part.storage
                && IcpuDesc[ICPU_RX_DESC]->queue.storage
                && IcpuDesc[ICPU_TX_DESC]->queue.storage)
            {
                // Unconditionally change when this mode specified
                IcpuRxTask = (IcpuHandle >= 0)? IcpuRxTaskDirectIoctl : IcpuRxTaskDirect;
                IcpuSendMsg = IcpuSendDirect;

                // Reset TX partition & queue
              //IcpuDesc[ICPU_TX_DESC]->part.free = IcpuDesc[ICPU_TX_DESC]->part.storage;
              //IcpuDesc[ICPU_TX_DESC]->part.allocated = 0;

                // Unlock any remaining locks
                IcpuUnlock(IcpuDesc[ICPU_RX_DESC]->part.lock);
                IcpuUnlock(IcpuDesc[ICPU_RX_DESC]->queue.lock);
                IcpuUnlock(IcpuDesc[ICPU_TX_DESC]->part.lock);
                IcpuUnlock(IcpuDesc[ICPU_TX_DESC]->queue.lock);

                DiagPrint(DIAG_LEVEL2, "ICPU", "Using direct I/O%s[Phys=%p,Virt=%p]\n",
                    (IcpuHandle >= 0)? " (ioctl) " : " ", IcpuDescBase, IcpuDesc[ICPU_RX_DESC]);
            }
            else
            {
                DiagPrint(DIAG_LEVEL0, "ICPU", "L-ARM is not loaded, I-CPU is disabled\n");
            }
        }
#endif /* C4KEVM */
    }
    //
    // Then try shared memory
    // Fall back to device mode if unsuccessful
    //
    else if (IcpuMode == IMODE_SHM)
    {
        if (IcpuSendMsg == NULL)
        {
            IcpuHandle = shmget(ftok(ICPU_SHMKEY, '4'), ICPU_SHMSIZE, ICPU_SHMFLAGS);
            IcpuRxSema = sem_open(ICPU_RXSEMA, O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, 0);
            IcpuTxSema = sem_open(ICPU_TXSEMA, O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO, 0);
            if (IcpuHandle == -1 || IcpuRxSema == SEM_FAILED || IcpuTxSema == SEM_FAILED)
            {
                DiagPrint(DIAG_LEVEL1, "ICPU", "SHM is not present[%i]\n", errno);
                // Try device mode
                IcpuMode = IMODE_RELEASE;
            }
            else
            {
                IcpuShmAddr = shmat(IcpuHandle, NULL, 0);
                IcpuShmTxAddr = IcpuShmAddr + ICPU_SHMTXOFFSET;
                IcpuShmRxAddr = IcpuShmAddr + ICPU_SHMRXOFFSET;
                IcpuRxTask = IcpuRxTaskShm;
                IcpuSendMsg = IcpuSendShm;
                DiagPrint(DIAG_LEVEL2, "ICPU", "Using SHM mode\n");
            }
        }
    }

    //
    // Next, check if we can run in MBOX mode
    // This is compatibility mode: we try to open mqueue even if default mode is "device"
    // Then fall back to device mode if mqueues not exist
    //
    if (IcpuMode == IMODE_RELEASE || IcpuMode == IMODE_DEMO)
    {
        if (IcpuSendMsg == NULL)
        {
            // Check if Message Queue is present in the system
            if (OsalCreateMailBox(&IcpuRxMbox, ICPU_RXQUEUE, 0, 0, NULL, FALSE)
                == SUCCESS)
            {
                // RX mailbox present, try TX mailbox too
                if (OsalCreateMailBox(&IcpuTxMbox, ICPU_TXQUEUE, 0, 0,
                                        NULL, FALSE) == SUCCESS)
                {
                    // Both TX and RX present, use them
                    IcpuMode = IMODE_DEMO;
                    IcpuRxTask = IcpuRxTaskMailBox;
                    IcpuSendMsg = IcpuSendMailBox;
                    DiagPrint(DIAG_LEVEL2, "ICPU", "Using MBOX mode\n");
                }
                else
                {
                    DiagPrint(DIAG_LEVEL1, "ICPU", "Unable to open MBOX TX [%i]\n", errno);
                    OsalDestroyMailBox(&IcpuRxMbox, ICPU_RXQUEUE);
                }
            }
            else
            {
                DiagPrint(DIAG_LEVEL1, "ICPU", "MBOX queue is not present\n");
            }
        }
    }

    //
    // At last, check if we run in release mode
    //
    if (IcpuMode == IMODE_RELEASE || IcpuMode == IMODE_FORCE_DEVICE)
    {
        // Reached here if all other possibilities declined
        if (IcpuSendMsg == NULL)
        {
            IcpuHandle = open(ICPU_DEVNAME, O_RDWR);
            if (IcpuHandle >= 0)
            {
                IcpuRxTask = IcpuRxTaskDevice;
                IcpuSendMsg = IcpuSendDevice;
                DiagPrint(DIAG_LEVEL2, "ICPU", "Using device mode\n");
            }
            else
            {
                DiagPrint(DIAG_LEVEL1, "ICPU", "Unable to open device [%i]\n", errno);
            }
        }
        else
        {
            DiagPrint(DIAG_LEVEL1, "ICPU", "Already assigned\n");
        }
    }

    if (IcpuRxTask)
    {
        OsalCreateTask(&IcpuRxTaskID, IcpuRxTask, NULL, "IcpuRxTask",
                       OSAL_PRIORITY_NORMAL, 0, 0);

        OsalSetTaskPriority(&IcpuRxTaskID, 0);
        OsalActivateTask(&IcpuRxTaskID);
    }

    return SUCCESS;
}

#ifdef C4KEVM
volatile U32 *ApbTimerRegs = NULL;

/** This task dispatches data from L-ARM
 *  Use direct User I/O mode
 *
 * @param data Not used
 */
volatile U32 IOtick;
volatile U32 IOavg = 0;

void IcpuRxTaskDirect(PTR data)
{
    volatile ICPU_COMM *pIcpuComm = IcpuDesc[ICPU_RX_DESC];
    volatile ICPU_PART *pPart  = &pIcpuComm->part;
    volatile ICPU_QUEUE *pQueue = &pIcpuComm->queue;
    PMSGHEADER pHdr;
    int index;
    // Calculate virtual addresses
    U32 *qaddr = (U32*)((uintptr_t)IcpuQueueStorage + (uintptr_t)pQueue->storage - ICPU_QUEUE_BASE);
    ICPU_PART_BLOCK *paddr = (ICPU_PART_BLOCK*)((uintptr_t)IcpuRxPartStorage +
                             (uintptr_t)pPart->storage -
                             ICPU_RXPART_BASE);
    volatile U16 get;
    volatile U16 count = pQueue->count;

    IcpuDirectTask = 1;                         // Signal we're started

#if 1
    ApbTimerRegs = (V32 *)mmap(NULL, 4096, PROT_READ, MAP_SHARED | MAP_LOCKED, DevMemMap, TIMER_APB_BASEADDR);
    if (ApbTimerRegs == MAP_FAILED)
        DiagPrint(DIAG_LEVEL0, "ICPU", "Unable to map direct Timer\n");
#endif

    while (IcpuDirectTask != 2)                 // Run until asked to close
    {
        get = pQueue->get;
        if (pQueue->put != get)
    {
            IcpuLock(pQueue->lock);
            index = qaddr[get];
            if (index < IcpuRxEntries)
            {
                pHdr = &paddr[index].header;
                if (++get >= count)
                    get = 0;
                pQueue->get = get;
                IcpuUnlock(pQueue->lock);
#ifdef CACHED_ICPU
                IcpuCacheInvalidate((U32*)pHdr, sizeof(MSGHEADER));
                IcpuCacheInvalidate((U32*)(pHdr+1), pHdr->length);
#endif
                IcpuStats.RxTotal++;
                DiagPrintApi(DIAG_LEVEL30, "ICPURX", DIAG_DECODETLV, pHdr);
                if (pHdr->control & CTRL_BYPASS)
                    DiagDumpPhyApi(DIAG_LEVEL29, MsgGetDataOffset(pHdr));
                {
                    //volatile int t = (*(ApbTimerRegs  + 1));
                    //float timediff = 0;
                    MsgDispatch(MSGS_LARM, pHdr->length + sizeof(MSGHEADER), pHdr);
                    //timediff = ((*(ApbTimerRegs  + 1)) - t ) / 150000.0;
                    //if (timediff > 1.5)
                    //{
                    //    printf("MsgDispatch time %f/n", timediff);
                    //    DiagPrint(DIAG_LEVEL0, "ICPU", "MsgDispatch time %f/n", timediff);
                    //}
                }
            }
            else
                IcpuUnlock(pQueue->lock);
        }
    }
    IcpuDirectTask = 3;                         // Confirm we're closing
}

/** Return a pointer to a next message in I-CPU partition
 *
 * @return Pointer to message if received or NULL if queue is empty
 */
int Icpu1RxDirect(PTR data)
{
    volatile ICPU_COMM *pIcpuComm = IcpuDesc[ICPU_RX_DESC];
    volatile ICPU_PART *pPart  = &pIcpuComm->part;
    volatile ICPU_QUEUE *pQueue = &pIcpuComm->queue;
    int rc = 0;

    if(pQueue->get != pQueue->put)
    {
        int index;
        rc = ioctl(IcpuHandle, ICPU_GET_INDEX, &index);

        if(rc == 0)
        {
            if(index >= 0)
            {
                ICPU_PART_BLOCK *paddr = (ICPU_PART_BLOCK*)((uintptr_t)IcpuRxPartStorage +
                             (uintptr_t)pPart->storage -
                             ICPU_RXPART_BASE);
 
                PMSGHEADER pHdr = &paddr[index].header;
#ifdef CACHED_ICPU
                IcpuCacheInvalidate((U32*)pHdr, sizeof(MSGHEADER));
                IcpuCacheInvalidate((U32*)(pHdr+1), pHdr->length);
#endif
                rc = (pHdr->length + sizeof(MSGHEADER));
                memcpy((void *)data, (void *)pHdr, rc);
                MsgFreeBuffer(pHdr);
                IcpuStats.RxTotal++;
            }
            else if(index < 0)
            {
                printf("IcpuRxTaskDirect: index %i\r\n", index);
            }
        }
        else
        {
            printf("IcpuRxTaskDirect: ioctl return 0x%X\r\n", rc);
        }
    }
    return rc;
}

#if 0
int Icpu1RxDirect(PTR data)
{
    volatile ICPU_COMM *pIcpuComm = IcpuDesc[ICPU_RX_DESC];
    volatile ICPU_PART *pPart  = &pIcpuComm->part;
    volatile ICPU_QUEUE *pQueue = &pIcpuComm->queue;
    PMSGHEADER pHdr = NULL;
    U16 index;
    U32 ret_val = 0;

    // Calculate virtual addresses
    U32 *qaddr = (U32*) ((uintptr_t) IcpuQueueStorage + (uintptr_t) pQueue->storage -
        ICPU_QUEUE_BASE);
    ICPU_PART_BLOCK *paddr = (ICPU_PART_BLOCK*)((uintptr_t)IcpuRxPartStorage +
        (uintptr_t) pPart->storage - ICPU_RXPART_BASE);

    if (pQueue->put != pQueue->get)
    {
        index = qaddr[pQueue->get];
        if (index < IcpuRxEntries)
        {
            pHdr = &paddr[index].header;

            IcpuLock(pQueue->lock);
            if (++pQueue->get >= pQueue->count)
                pQueue->get = 0;
            IcpuUnlock(pQueue->lock);

            IcpuStats.RxTotal++;
#ifdef CACHED_ICPU
            IcpuCacheInvalidate((U32*)pHdr, sizeof(MSGHEADER));
            IcpuCacheInvalidate((U32*)(pHdr+1), pHdr->length);
#endif
            ret_val = pHdr->length + sizeof(MSGHEADER);
            memcpy((void *)data, (void *)pHdr, ret_val);
            MsgFreeBuffer(pHdr);
        }
    }
    return ret_val;
}
#endif


U32 IcpuRxDirectIoctl(PTR data)
{
    volatile ICPU_COMM *pIcpuComm = IcpuDesc[ICPU_RX_DESC];
    volatile ICPU_PART *pPart  = &pIcpuComm->part;
    volatile ICPU_QUEUE *pQueue = &pIcpuComm->queue;
    int rc = 0;

    if(pQueue->get != pQueue->put)
    {
        int index;
        rc = ioctl(IcpuHandle, ICPU_GET_INDEX, &index);

        if(rc == 0)
        {
            if(index >= 0)
            {
                ICPU_PART_BLOCK *paddr = (ICPU_PART_BLOCK*)((uintptr_t)IcpuRxPartStorage +
                             (uintptr_t)pPart->storage -
                             ICPU_RXPART_BASE);
 
                PMSGHEADER pHdr = &paddr[index].header;
#ifdef CACHED_ICPU
                IcpuCacheInvalidate((U32*)pHdr, sizeof(MSGHEADER));
                IcpuCacheInvalidate((U32*)(pHdr+1), pHdr->length);
#endif
                rc = (pHdr->length + sizeof(MSGHEADER));
                memcpy((void *)data, (void *)pHdr, rc);
                MsgFreeBuffer(pHdr);
                IcpuStats.RxTotal++;
            }
            else if(index < 0)
            {
                printf("IcpuRxTaskDirect: index %i\r\n", index);
            }
        }
        else
        {
            printf("IcpuRxTaskDirect: ioctl return 0x%X\r\n", rc);
        }
    }
    return rc;
}

void IcpuRxTaskDirectIoctl(PTR data)
{
    volatile ICPU_COMM *pIcpuComm = IcpuDesc[ICPU_RX_DESC];
    volatile ICPU_PART *pPart  = &pIcpuComm->part;
    //volatile ICPU_QUEUE *pQueue = &pIcpuComm->queue;
    PMSGHEADER pHdr;
    int index;
    ICPU_PART_BLOCK *paddr = (ICPU_PART_BLOCK*)((uintptr_t)IcpuRxPartStorage +
                             (uintptr_t)pPart->storage -
                             ICPU_RXPART_BASE);
    S32 rc;

    IcpuDirectTask = 1;                         // Signal we're started
#if 0
    unsigned long mask = 1; /* processor 0 */
    /* bind process to processor 0 */
    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) <0)
    {
        perror("IcpuRxTaskDirect: pthread_setaffinity_np");
    }
#endif

#if 1
    ApbTimerRegs = (V32 *)mmap(NULL, 4096, PROT_READ, MAP_SHARED | MAP_LOCKED, DevMemMap, TIMER_APB_BASEADDR);
    if (ApbTimerRegs == MAP_FAILED)
        DiagPrint(DIAG_LEVEL0, "ICPU", "Unable to map direct Timer\n");
#endif

    while (IcpuDirectTask != 2)                 // Run until asked to close
    {
        rc = ioctl(IcpuHandle, ICPU_GET_INDEX, &index);

        if(rc == 0)
        {
            if(index >= 0)
            {
                pHdr = &paddr[index].header;
                IcpuStats.RxTotal++;
#ifdef CACHED_ICPU
                IcpuCacheInvalidate((U32*)pHdr, sizeof(MSGHEADER));
                IcpuCacheInvalidate((U32*)(pHdr+1), pHdr->length);
#endif
                DiagPrintApi(DIAG_LEVEL30, "ICPURX", DIAG_DECODETLV, pHdr);
                if (pHdr->control & CTRL_BYPASS)
                    DiagDumpPhyApi(DIAG_LEVEL29, MsgGetDataOffset(pHdr));
                MsgDispatch(MSGS_LARM, pHdr->length + sizeof(MSGHEADER), pHdr);
            }
            else if(index < 0)
            {
                printf("IcpuRxTaskDirect: index %i\r\n", index);
            }
        }
        else
        {
            printf("IcpuRxTaskDirect: ioctl return 0x%X\r\n", rc);
        }
    }
    IcpuDirectTask = 3;                         // Confirm we're closing
}


#endif /* C4KEVM */

/** This task dispatches data from L-ARM
 *  Use release mode (direct access to the device)
 *
 * @param data Not used
 */
void IcpuRxTaskDevice(PTR data)
{
    PTR buffer;
    ssize_t ret;

    while (1)
    {
        if (OsalAllocMemArray(&MsgArray, &buffer) == SUCCESS)
        {
            // Simple copying from kernel space right now
            if ((ret = read(IcpuHandle, buffer, MSG_MAXSIZE)) > 0)
            {
                DiagPrintApi(DIAG_LEVEL30, "ICPURX", DIAG_DECODETLV, buffer);
                if (((PMSGHEADER) buffer)->control & CTRL_BYPASS)
                    DiagDumpPhyApi(DIAG_LEVEL29, MsgGetDataOffset(buffer));
                MsgDispatch(MSGS_LARM, sizeof(MSGHEADER) + ((PMSGHEADER) buffer)->length, buffer);
            }
            else if (ret < 0)
            {
                // Error has been occured, exit task
                OsalFreeMemArray(&MsgArray, buffer);
                DiagPrint(DIAG_LEVEL4, "ICPURX", "ERROR: Read from ICPU [%i]\n", ret);
                break;
            }
            else
            {
                MsgFreeBuffer(buffer);
            }
        }
        else
        {
            // No memory, wait
            OsalWaitDelay(MEM_RECOVERYTIME);
        }
    }
    // Close handle
    if (IcpuHandle >= 0)
    {
        close(IcpuHandle);
        IcpuHandle = -1;
    }
}

/** This task dispatches data from L-ARM
 *  Use DEMO mode (mailboxes)
 *
 * @param data Not used
 */
void IcpuRxTaskMailBox(PTR data)
{
    PTR buffer;
    OSAL_SIZE msgSize;

    while (1)
    {
        if (OsalAllocMemArray(&MsgArray, &buffer) == SUCCESS)
        {
            msgSize = MSG_MAXSIZE;
            if (OsalGetMsg(&IcpuRxMbox, buffer, &msgSize) == SUCCESS)
            {
                PMSGHEADER pHdr = (PMSGHEADER) buffer;
                DiagPrintApi(DIAG_LEVEL30, "ICPURX", DIAG_DECODETLV, buffer);
                MsgDispatch(MSGS_LARM, sizeof(MSGHEADER) + pHdr->length, buffer);
            }
            else
            {
                // Error occured, exit task
                DiagPrint(DIAG_LEVEL4, "ICPURX", "ERROR: Read from MBOX [%i]\n", errno);
                OsalFreeMemArray(&MsgArray, buffer);
                break;
            }
        }
        else
        {
            // No memory, wait
            OsalWaitDelay(MEM_RECOVERYTIME);
        }
    }
    OsalDestroyMailBox(&IcpuRxMbox, ICPU_RXQUEUE);
}

/** This task dispatches data from L-ARM using shared memory
 *
 * @param data Not used
 */
void IcpuRxTaskShm(PTR data)
{
    PTR buffer;
    PMSGHEADER pHdr;

    while (1)
    {
        if (OsalAllocMemArray(&MsgArray, &buffer) == SUCCESS)
        {
            if (sem_wait(IcpuRxSema) == 0)
            {
                pHdr = (PMSGHEADER) IcpuShmRxAddr;
                memcpy(buffer, pHdr, pHdr->length + sizeof(MSGHEADER));
                DiagPrintApi(DIAG_LEVEL30, "ICPURX", DIAG_DECODETLV, buffer);
                MsgDispatch(MSGS_LARM, pHdr->length + sizeof(MSGHEADER), buffer);
                IcpuShmRxAddr += MSG_MAXSIZE;
                if (IcpuShmRxAddr >= IcpuShmAddr + ICPU_SHMRXEND)
                {
                    IcpuShmRxAddr = IcpuShmAddr + ICPU_SHMRXOFFSET;
                }
            }
            else
            {
                // Error occured, exit task
                DiagPrint(DIAG_LEVEL4, "ICPURX", "ERROR: Read from SHM [%i]\n", errno);
                OsalFreeMemArray(&MsgArray, buffer);
                break;
            }
        }
        else
        {
            // No memory, wait
            OsalWaitDelay(MEM_RECOVERYTIME);
        }
    }
}

#ifdef C4KEVM
/** Enqueue message index in I-CPU direct access partition
 *
 * @param pQueue Pointer to I-CPU queue structure
 * @param index Message index to enqueue
 * @return Standard result code
 */
RESULTCODE IcpuEnqueue(volatile ICPU_QUEUE* pQueue, U32 index)
{
    U32 *addr;
    U16 put;

    IcpuLock(pQueue->lock);
    put = pQueue->put + 1;
    if (put >= pQueue->count)
        put = 0;

    if (put != pQueue->get)
    {
        // Convert physical to virtual address
        addr = (U32*)((uintptr_t)IcpuQueueStorage + (uintptr_t)pQueue->storage - ICPU_QUEUE_BASE);
        addr[pQueue->put] = index;
        pQueue->put = put;
        IcpuUnlock(pQueue->lock);
        return SUCCESS;
    }
    IcpuUnlock(pQueue->lock);
    return FAILURE;
    }

/** Send data to L-ARM
 *  Use direct mode (memory mapped I/O)
 *
 * @param size Size of data chunk
 * @param data Pointer to the data
 * @return Standard result code
 */
#ifdef ICPU_DEBUG_PART
void IcpuBuffDbgDump(void)
{
    if(AllocIcpuBuffDbgNum != 0)
    {
        ICPU_BLOCK_DBG* pBuf;
        U32 CurrIdx = 0;

        printf("Dumping Allocated buffer:\n");
        if(AllocIcpuBuffDbgNum > 511)
        {
            if(AllocIcpuBuffDbgIdx >= 511)
                CurrIdx = AllocIcpuBuffDbgIdx - 511;
            else
                CurrIdx = 512 - (511 - AllocIcpuBuffDbgIdx);
        }
        do
        {
            pBuf = &AllocIcpuBuffDbg[CurrIdx];

            printf("%d. ", CurrIdx);
            printf("pPhysAddr %X tAlloc %d\n",pBuf->pBlockPhys, pBuf->TicksAlloc);

            if(++CurrIdx >= 512)
                CurrIdx = 0;
        }while(CurrIdx != AllocIcpuBuffDbgIdx);
    }

    if(TxIcpuBuffDbgNum != 0)
    {
        ICPU_BLOCK_DBG* pBuf;
        U32 CurrIdx = 0;

        printf("Dumping Sending buffer:\n");
        if(TxIcpuBuffDbgNum > 511)
        {
            if(TxIcpuBuffDbgIdx >= 511)
                CurrIdx = TxIcpuBuffDbgIdx - 511;
            else
                CurrIdx = 512 - (511 - TxIcpuBuffDbgIdx);
        }
        do
        {
            pBuf = &TxIcpuBuffDbg[CurrIdx];

            printf("%d. ", CurrIdx);
            printf("pPhysAddr %X tSent %d\n",pBuf->pBlockPhys, pBuf->TicksSent);

            if(++CurrIdx >= 512)
                CurrIdx = 0;
        }while(CurrIdx != TxIcpuBuffDbgIdx);
    }
}

inline void IcpuTxBuffDbgAdd(PTR address)
{
    ICPU_BLOCK_DBG* pBuf = &TxIcpuBuffDbg[TxIcpuBuffDbgIdx];

    pBuf->pBlockPhys = address;
    pBuf->TicksSent = (*(ApbTimerRegs  + 1));

    if(++TxIcpuBuffDbgIdx >= 512)
        TxIcpuBuffDbgIdx = 0;

    TxIcpuBuffDbgNum++;
}
#endif

RESULTCODE IcpuSendDirect(U32 size, PTR data)
{
    volatile ICPU_COMM *pIcpuComm = IcpuDesc[ICPU_TX_DESC];
    volatile ICPU_PART *pPart = &pIcpuComm->part;
    volatile ICPU_QUEUE* pQueue = &pIcpuComm->queue;
    PTR address;
    int index;

    DiagPrintApi(DIAG_LEVEL30, "ICPUTX", DIAG_DECODETLV, data);
    if (((PMSGHEADER)data)->control & CTRL_BYPASS)
        DiagDumpPhyApi(DIAG_LEVEL29, MsgGetDataOffset(data));

    // Check if we are sending non-native block
    if ((data < IcpuTxPartStorage) || ((uintptr_t)data >= (uintptr_t)IcpuTxPartStorage + ICPU_TXPART_SIZE))
    {
        PTR pNew = IcpuAllocBlock();
        if (pNew == NULL)
        {
          #ifdef ICPU_MEMORY_DEBUGGING
           // No memory, cleanup
            ioctl(IcpuHandle, ICPU_LARM_CRASH, &index);
            printf("IcpuSendDirect: ICPU TX partition ends (peer stuck?) (tick=%i) (RxNum=%d TxNum=%d TxNumRalc=%d)!!!!\n", (*(ApbTimerRegs  + 1)),
                IcpuStats.RxTotal, IcpuStats.TxTotal, IcpuStats.TxRealloc);
           #ifdef ICPU_DEBUG_PART
            printf("IcpuSendDirect: TxLastTicks = %i TxLastTicks1 = %i TxLastTicks2 = %i\n", IcpuStats.TxLastTicks,
                IcpuStats.TxLastTicks1, IcpuStats.TxLastTicks2);
            printf("IcpuSendDirect: AllocTotalNum = %i AllocLastTicks = %i\n", IcpuStats.AllocTotalNum ,
                IcpuStats.AllocLastTicks);
            IcpuBuffDbgDump();
           #endif
           MsgFreeBuffer(data);  // don't free data for usual way, upper layer should decide what to do with it
           exit(-1);
           #endif
            return FAILURE;
        }
        IcpuStats.TxRealloc++;
        // memcpy is evil
        memcpy(pNew, data, size);
        // Free original block
        MsgFreeBuffer(data);
        // Substitute block
        data = pNew;
    }

#if 0
    {
        PMSGHEADER pHdr = (PMSGHEADER) data;
        if(pHdr->msgID == DIAG_CHECK_API_LAT)
        {
            *((U32*)pHdr + 7) = Timer0GetCntr();
        }
    }
#endif

    IcpuStats.TxTotal++;
    // Convert virtual address to physical
    address = data - IcpuTxPartStorage + (PTR)ICPU_TXPART_BASE;
    // Get the array index
    // TODO: This is performance hit
    index = ((uintptr_t)address - (uintptr_t)pPart->storage) / sizeof(ICPU_PART_BLOCK);
    // Enqueue index to peer CPU

#ifdef ICPU_DEBUG_PART
    IcpuStats.TxLastTicks = (*(ApbTimerRegs  + 1));
#endif

#ifdef CACHED_ICPU
    // clean the cache
    IcpuCacheClean(data, size);
#endif
    // Enqueue index to peer CPU
    IcpuEnqueue(pQueue, index);
#ifdef ICPU_DEBUG_PART
    IcpuStats.TxLastTicks1 = (*(ApbTimerRegs  + 1));
    IcpuTxBuffDbgAdd(address);

    IcpuStats.TxLastTicks2 = (*(ApbTimerRegs  + 1));
#endif
    // Signal peer CPU (direct access to INTC)
    *(volatile U32 *)(IcpuIntc + 0xF) = 1 << (ICPU_TX_INT - 32);

    return SUCCESS;
}
#endif /* C4KEVM */

/** Send data to L-ARM
 *  Use release mode (access to device)
 *
 * @param size Size of data chunk
 * @param data Pointer to the data
 * @return Standard result code
 */
RESULTCODE IcpuSendDevice(U32 size, PTR data)
{
    RESULTCODE ret = SUCCESS;
    ssize_t count;

    if (IcpuHandle >= 0)
    {
        DiagPrintApi(DIAG_LEVEL30, "ICPUTX", DIAG_DECODETLV, data);
        if (((PMSGHEADER) data)->control & CTRL_BYPASS)
            DiagDumpPhyApi(DIAG_LEVEL29, MsgGetDataOffset(data));
        // Simple copying to kernel space right now
        count = write(IcpuHandle, data, (size_t) size);
        if (count <= 0)
        {
            ret = FAILURE;
        }
    }
    MsgFreeBuffer(data);
    return SUCCESS;
}

/** Send data to L-ARM
 *  Use DEMO mode (mailbox)
 *
 * @param size Size of data chunk
 * @param data Pointer to the data
 * @return Standard result code
 */
RESULTCODE IcpuSendMailBox(U32 size, PTR data)
{
    RESULTCODE ret = FAILURE;

    if ((IcpuTxMbox != 0) && (size <= MSG_MAXSIZE))
    {
        DiagPrintApi(DIAG_LEVEL30, "ICPUTX", DIAG_DECODETLV, data);
        ret = OsalSendMsg(&IcpuTxMbox, data, size);
    }
    MsgFreeBuffer(data);

    return ret;
}

/** Send data to L-ARM through shared memory (debug)
 *
 * @param size Size of data chunk
 * @param data Pointer to the data
 * @return Standard result code
 */
RESULTCODE IcpuSendShm(U32 size, PTR data)
{
    RESULTCODE ret = SUCCESS;

    if (IcpuShmTxAddr != NULL)
    {
        DiagPrintApi(DIAG_LEVEL30, "ICPUTX", DIAG_DECODETLV, data);
        memcpy(IcpuShmTxAddr, data, size);
        IcpuShmTxAddr += MSG_MAXSIZE;
        if (IcpuShmTxAddr >= IcpuShmAddr + ICPU_SHMTXEND)
        {
            IcpuShmTxAddr = IcpuShmAddr + ICPU_SHMTXOFFSET;
        }
        sem_post(IcpuTxSema);
    }
    else
    {
        ret = FAILURE;
    }
    MsgFreeBuffer(data);

    return ret;
}

/** Stop I-CPU
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
RESULTCODE IcpuDone(void)
{
    IcpuSendMsg = NULL;
    if (IcpuRxTask)
    {
#ifdef C4KEVM
        // Ensure Direct RX task is closed before we go
        // Direct RX task is not using signals for synchronization
        // TODO: Update when another sync mechanism available
        if (IcpuDirectTask == 1)            // Task is running
        {
            IcpuDirectTask = 2;             // Ask it to stop
            while (IcpuDirectTask != 3)     // Wait for confirmation
                __nop();
            IcpuDirectTask = 0;             // Cleanup
        }
#endif /* C4KEVM */
        OsalDeactivateTask(&IcpuRxTaskID);
        OsalDestroyTask(&IcpuRxTaskID);
        IcpuRxTask = NULL;
    }
    if (IcpuShmAddr != NULL)
    {
        shmdt(IcpuShmAddr);
        IcpuShmAddr = NULL;
    }
    if (IcpuRxSema != SEM_FAILED)
    {
        sem_close(IcpuRxSema);
        sem_unlink(ICPU_RXSEMA);
        IcpuRxSema = SEM_FAILED;
    }
    if (IcpuTxSema != SEM_FAILED)
    {
        sem_close(IcpuTxSema);
        sem_unlink(ICPU_TXSEMA);
        IcpuTxSema = SEM_FAILED;
    }
    if (IcpuRxMbox)
    {
        OsalDestroyMailBox(&IcpuRxMbox, ICPU_RXQUEUE);
        IcpuRxMbox = 0;
    }
    if (IcpuTxMbox)
    {
        OsalDestroyMailBox(&IcpuTxMbox, ICPU_TXQUEUE);
        IcpuTxMbox = 0;
    }
    if (IcpuHandle >= 0)
    {
        close(IcpuHandle);
        IcpuHandle = -1;
    }
#ifdef C4KEVM
    if (IcpuDesc[ICPU_RX_DESC])
    {
        IcpuUnmapIo((PTR)(IcpuDesc[ICPU_RX_DESC]));
        IcpuDesc[ICPU_RX_DESC] = NULL;
        IcpuDesc[ICPU_TX_DESC] = NULL;
    }
    if (IcpuTxPartStorage)
    {
        IcpuUnmapIo(IcpuTxPartStorage);
        IcpuTxPartStorage = NULL;
    }
    if (IcpuRxPartStorage)
    {
        IcpuUnmapIo(IcpuRxPartStorage);
        IcpuRxPartStorage = NULL;
    }
    if (IcpuQueueStorage)
    {
        IcpuUnmapIo(IcpuQueueStorage);
        IcpuQueueStorage = NULL;
    }
    if (IcpuIntc)
    {
        IcpuUnmapIo((PTR)IcpuIntc);
        IcpuIntc = NULL;
    }
    if (IcpuSema)
    {
        IcpuUnmapIo((PTR)IcpuSema);
        IcpuSema = NULL;
    }
#endif /* C4KEVM */

    return SUCCESS;
}

#ifdef C4KEVM
/**
 *
 * @param addr Physical address
 * @param len Region size
 * @return NULL on failure, virtual address on success
 */
PTR IcpuMapIo(PTR addr, U32 len)
{
    ICPU_DRV_DESC *pDrv = UioDrv;
    int i;

    // Find UIO driver which is responsible for this address
    for (i = 0; i < UIO_MAX; i++)
    {
        if (addr >= pDrv->address && addr < pDrv->address + pDrv->size)
            break;
        pDrv++;
    }
    if (i == UIO_MAX)
    {
        // Did not find memory map
        printf("IcpuMapIo: max UIO number\n");
        return NULL;
    }
    if (pDrv->file == 0)
    {
        pDrv->file = open(pDrv->name, O_RDWR);
        if (pDrv->file <= 0)
        {
            printf("IcpuMapIo: file open failed\n");
            return NULL;
        }
        pDrv->mapped = mmap(0, pDrv->size, PROT_READ | PROT_WRITE, MAP_SHARED, pDrv->file, 0);
        if (pDrv->mapped == MAP_FAILED)
        {
            printf("IcpuMapIo: mmap failed\n");
            close(pDrv->file);
            pDrv->file = 0;
            return NULL;
        }
    }
    pDrv->refCount++;

    return (PTR) (pDrv->mapped + (addr - pDrv->address));
}

#ifdef CACHED_ICPU
PTR IcpuMapCached(PTR addr, U32 len)
{
    ICPU_DRV_DESC *pDrv = CachedUioDrv;
    int i;

    // Find UIO driver which is responsible for this address
    for (i = 0; i < UIO_CACHED_MAX; i++)
    {
        if (addr >= pDrv->address && addr < pDrv->address + pDrv->size)
            break;
        pDrv++;
    }

    if (i == UIO_CACHED_MAX)
    {
        // Did not find memory map
        printf("IcpuMapCached: max UIO number\n");
        return NULL;
    }

    pDrv->file = open(pDrv->name, O_RDWR);
    if (pDrv->file <= 0)
    {
        printf("IcpuMapCached: file open failed\n");
        return NULL;
    }

    printf("IcpuMapCached: id %d name %s size %d file %d address %x\n",
        i,
        pDrv->name,
        pDrv->size,
        pDrv->file,
        (U32)pDrv->address);

    pDrv->mapped = mmap(0, pDrv->size, PROT_READ | PROT_WRITE, MAP_SHARED, pDrv->file, (U32)pDrv->address);
    if (pDrv->mapped == MAP_FAILED)
    {
        printf("IcpuMapCached: mmap failed\n");
        close(pDrv->file);
        pDrv->file = 0;
        return NULL;
    }

    pDrv->refCount++;

    if ((U32)addr != (U32) pDrv->address || len !=  pDrv->size )
    {
        printf("IcpuMapCached: incorrect mapping\n");
        return 0;
    }

    printf("IcpuMapCached: mapped address %x\n",
        pDrv->mapped);

    return (PTR) (pDrv->mapped + (addr - pDrv->address));
}

#endif

/** Removes physical to virtual memory mapping for the given region
 *
 * @param addr Physical address
 */
void IcpuUnmapIo(PTR addr)
{
    ICPU_DRV_DESC *pDrv = UioDrv;
    int i;

    // Find UIO driver which is responsible for this address
    for (i = 0; i < UIO_MAX; i++)
    {
        if (addr >= pDrv->address && addr < pDrv->address + pDrv->size)
            break;
        pDrv++;
    }
    if (i == UIO_MAX)
    {
        // Did not find appropriate memory map
        return;
    }

    if (pDrv->refCount)
    {
        if (--pDrv->refCount == 0)
        {
            munmap(pDrv->mapped, pDrv->size);
            pDrv->mapped = NULL;
            if (pDrv->file)
            {
                close(pDrv->file);
                pDrv->file = 0;
            }
        }
    }
    else
    {
        // TODO: Indicate error
        return;
    }
}


S32 IcpuSetIrqIoctl(V32 irq)
{
    S32 rc;

    rc = ioctl(IcpuHandle, ICPU_SET_IRQ, &irq);
    return rc;
}

V32 IcpuGetIrqIoctl(V32 *irq)
{
    S32 rc;

    rc = ioctl(IcpuHandle, ICPU_GET_IRQ, irq);
    return rc;
}

/** Lock hardware semaphore (mutex)
 *
 * @param lock Physical address of hardware semaphore
 */
void IcpuLock(V32 *lock)
{
    // Get offset from the base segment and then add it to a virtual address
    V32 *address = (V32 *) ((uintptr_t)lock - SYS_SEMAPHORE_BASE + (uintptr_t)IcpuSema);

    OsalCaptureMutex(&GicMutex, 0);
#if 1
    if (IcpuGic)
    {
        /*if (*IcpuGic != 1)
        {
            printf("Lock: IRQ was changed!!!\n");
        }*/

        do
        {
            *IcpuGic = 0;
            __nop();
        }while (*IcpuGic != 0);
    }
#else
    {
        V32 irq = 0;

        IcpuGetIrqIoctl(&irq);
        if (irq != 1)
        {
            printf("Lock: IRQ was changed!!!\n");
        }
    }
    IcpuSetIrqIoctl(0);
#endif

    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();

    // Wait for HW semaphore if locked
    while (*(volatile U32 *)address != 0)
        __nop();
}

/** Unlock hardware semaphore (mutex)
 *
 * @param lock Physical address of hardware semaphore
 */
void IcpuUnlock(V32 *lock)
{
    // Get offset from the base segment and then add it to a virtual address
    V32 *address = (V32*) ((uintptr_t)lock - SYS_SEMAPHORE_BASE + (uintptr_t)IcpuSema);

    // Reset HW semaphore/counter
    *address = 0;
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();

#if 1
    if (IcpuGic)
    {
        /*if (*IcpuGic != 0)
        {
            printf("Unlock: IRQ was changed!!!\n");
        }*/

        do
        {
           *IcpuGic = 1;
           __nop();
        }while (*IcpuGic != 1);
    }
#else
    {
        V32 irq = 1;
        IcpuGetIrqIoctl(&irq);
        if (irq != 0)
        {
            printf("Unlock: IRQ was changed!!!\n");
        }
    }
    IcpuSetIrqIoctl(1);
#endif
    OsalFreeMutex(&GicMutex);
}

/** Allocate memory block in direct access I-CPU TX partition
 *
 * @return Virtual address of a newly allocated block or NULL on failure
 */
#ifdef ICPU_DEBUG_PART
inline void IcpuAllocBuffDbgAdd(PTR address)
{
    ICPU_BLOCK_DBG* pBuf = &AllocIcpuBuffDbg[AllocIcpuBuffDbgIdx];

    pBuf->pBlockPhys = address;
    pBuf->TicksAlloc = (*(ApbTimerRegs  + 1));

    if(++AllocIcpuBuffDbgIdx >= 512)
        AllocIcpuBuffDbgIdx = 0;

    AllocIcpuBuffDbgNum++;
}
#endif

PTR IcpuAllocBlock(void)
{
    PTR block;
    PTR address;
    ICPU_PART *pPart = (ICPU_PART *)&IcpuDesc[ICPU_TX_DESC]->part;

    IcpuLock(pPart->lock);

    block = pPart->free;
    if (block != NULL)
    {
        // Convert physical address to a virtual address
        address = block - ((PTR)pPart->storage) + IcpuTxPartStorage;
        // This is physical address
#ifdef CACHED_ICPU
        // clean the cache
        IcpuCacheInvalidateTx(address, sizeof(PTR));
#endif
        pPart->free = *(PTR *) address;
        pPart->allocated++;
#ifdef ICPU_DEBUG_PART
        IcpuStats.AllocTotalNum++;
        IcpuStats.AllocLastTicks = (*(ApbTimerRegs  + 1));
        IcpuAllocBuffDbgAdd(block);
#endif
    }
    else
        address = NULL;

    IcpuUnlock(pPart->lock);

    return address;
}

/** Releases block in I-CPU RX partition
 *
 * @param block Virtual address of block to release
 * @return SUCCESS if block was freed
 */
RESULTCODE IcpuFreeBlock(PTR block)
{
    PTR address;
    // Only RX partition handled
    ICPU_PART *pPart = (ICPU_PART *)&IcpuDesc[ICPU_RX_DESC]->part;

    if (IcpuRxPartStorage == NULL)
        return FAILURE;

    if ((block >= IcpuRxPartStorage) && (block < IcpuRxPartStorage + ICPU_RXPART_SIZE))
    {
        // Get physical address
        address = (block - IcpuRxPartStorage) + // Offset from segment
                  (PTR)(ICPU_RXPART_BASE);      // Physical address of partition storage

        IcpuLock(pPart->lock);
        *(PTR *) block = pPart->free;           // Physical address
#ifdef CACHED_ICPU
        // clean the cache
        IcpuCacheCleanRx(block, sizeof(PTR));
#endif
        pPart->free = address;                  // Physical address
        pPart->allocated--;
        IcpuUnlock(pPart->lock);

        return SUCCESS;
    }

    // This is not our block
    return FAILURE;
}

void IcpuRaiseInter(unsigned int numInter)
{
    // Signal peer CPU (direct access to INTC)
    *(volatile U32 *)(IcpuIntc + 0xF) |= 1 << (numInter - 32);
}

/** Translates virtual address to a physical space. This region must be opened by UIO driver
 *
 * @param virtaddr
 * @return Physical address on success, NULL on failure
 */
PTR Icpu1GetPhys(PTR virtaddr)
{
    int i;

#ifdef CACHED_ICPU
    for (i = 0; i < UIO_CACHED_MAX; i++)
    {
        // This driver is opened
        if (CachedUioDrv[i].refCount > 0)
        {
            // Required address falls into covered memory region
            if ((virtaddr >= CachedUioDrv[i].mapped) && (virtaddr < CachedUioDrv[i].mapped + CachedUioDrv[i].size))
            {
                // Return offset from the base physical address
                return CachedUioDrv[i].address + (virtaddr - CachedUioDrv[i].mapped);
            }
        }
    }
#endif
    for (i = 0; i < UIO_MAX; i++)
    {
        // This driver is opened
        if (UioDrv[i].refCount > 0)
        {
            // Required address falls into covered memory region
            if ((virtaddr >= UioDrv[i].mapped) && (virtaddr < UioDrv[i].mapped + UioDrv[i].size))
            {
                // Return offset from the base physical address
                return UioDrv[i].address + (virtaddr - UioDrv[i].mapped);
            }
        }
    }
    return NULL;
}

/** Translates physical address to a virtual space. This region must be opened by UIO driver
 *
 * @param phyaddr
 * @return Virtual address on success, NULL on failure
 */
PTR Icpu1GetVirt(PTR physaddr)
{
    int i;

#ifdef CACHED_ICPU
    for (i = 0; i < UIO_CACHED_MAX; i++)
    {
        // This driver is opened
        if (CachedUioDrv[i].refCount > 0)
        {
            // Required address falls into covered memory region
            if ((physaddr >= CachedUioDrv[i].address) && (physaddr < CachedUioDrv[i].address+ CachedUioDrv[i].size))
            {
                // Return offset from the base physical address
                return CachedUioDrv[i].mapped + (physaddr - CachedUioDrv[i].address); // Need to add delta to the mapped address
            }
        }
    }
#endif
    for (i = 0; i < UIO_MAX; i++)
    {
        // This driver is opened
        if (UioDrv[i].refCount > 0)
        {
            // Required address falls into covered memory region
            if ((physaddr >= UioDrv[i].address) && (physaddr < UioDrv[i].address+ UioDrv[i].size))
            {
                // Return offset from the base physical address
                return UioDrv[i].mapped + (physaddr - UioDrv[i].address); // Need to add delta to the mapped address
            }
        }
    }
    return NULL;
}

#ifdef CACHED_ICPU
void IcpuCacheClean(U32 * pMem,  U32 nSize)
{
    int rc = 0;
    ICPU_DRV_DESC *pDrv = &CachedUioDrv[ICPU_TX_DESC];

    unsigned int params[3] =
    {
        (unsigned int)pMem,
        (unsigned int)((U32)pMem + nSize),
        (unsigned int)pDrv->address
    };

   if ((rc = ioctl(pDrv->file, UARM_IOCTL_CACHE_CLEAN, params)) != 0)
   {
       printf("IcpuCacheClean rc = %d\n", rc);
   }

    //printf("[0x%X] IcpuCacheClean pDrv->file = %d\n", pMem, pDrv->file);

    return;
}

void IcpuCacheCleanTx(U32 * pMem,  U32 nSize)
{
    int rc = 0;
    ICPU_DRV_DESC *pDrv = &CachedUioDrv[ICPU_TX_DESC];

    unsigned int params[3] =
    {
        (unsigned int)pMem,
        (unsigned int)((U32)pMem + nSize),
        (unsigned int)pDrv->address
    };

   if ((rc = ioctl(pDrv->file, UARM_IOCTL_CACHE_CLEAN, params)) != 0)
   {
       printf("IcpuCacheClean rc = %d\n", rc);
   }

    //printf("[0x%X] IcpuCacheCleanTx pDrv->file = %d\n", pMem, pDrv->file);

    return;
}

void IcpuCacheCleanRx(U32 * pMem,  U32 nSize)
{
    int rc = 0;
    ICPU_DRV_DESC *pDrv = &CachedUioDrv[ICPU_RX_DESC];

    unsigned int params[3] =
    {
        (unsigned int)pMem,
        (unsigned int)((U32)pMem + nSize),
        (unsigned int)pDrv->address
    };

   if ((rc = ioctl(pDrv->file, UARM_IOCTL_CACHE_CLEAN, params)) != 0)
   {
       printf("IcpuCacheClean rc = %d\n", rc);
   }

    //printf("[0x%X] IcpuCacheCleanRx pDrv->file = %d\n", pMem, pDrv->file);

    return;
}

void IcpuCacheInvalidate(U32 * pMem,  U32 nSize)
{
    int rc = 0;
    ICPU_DRV_DESC *pDrv = &CachedUioDrv[ICPU_RX_DESC];

    unsigned int params[3] =
    {
        (unsigned int)pMem,
        (unsigned int)((U32)pMem + nSize),
        (unsigned int)pDrv->address
    };

    if ((rc = ioctl(pDrv->file, UARM_IOCTL_CACHE_INVALIDATE, params)) != 0)
    {
        printf("IcpuCacheInvalidate rc = %d\n", rc);
    }

   //printf("[0x%X] IcpuCacheInvalidate pDrv->file = %d\n", pMem,  pDrv->file);

    return;
}

void IcpuCacheInvalidateRx(U32 * pMem,  U32 nSize)
{
    int rc = 0;
    ICPU_DRV_DESC *pDrv = &CachedUioDrv[ICPU_RX_DESC];

    unsigned int params[3] =
    {
        (unsigned int)pMem,
        (unsigned int)((U32)pMem + nSize),
        (unsigned int)pDrv->address
    };

    if ((rc = ioctl(pDrv->file, UARM_IOCTL_CACHE_INVALIDATE, params)) != 0)
    {
        printf("IcpuCacheInvalidate rc = %d\n", rc);
    }

   //printf("[0x%X] IcpuCacheInvalidateRx pDrv->file = %d\n", pMem,  pDrv->file);

    return;
}

void IcpuCacheInvalidateTx(U32 * pMem,  U32 nSize)
{
    int rc = 0;
    ICPU_DRV_DESC *pDrv = &CachedUioDrv[ICPU_TX_DESC];

    unsigned int params[3] =
    {
        (unsigned int)pMem,
        (unsigned int)((U32)pMem + nSize),
        (unsigned int)pDrv->address
    };

    if ((rc = ioctl(pDrv->file, UARM_IOCTL_CACHE_INVALIDATE, params)) != 0)
    {
        printf("IcpuCacheInvalidate rc = %d\n", rc);
    }

   //printf("[0x%X] IcpuCacheInvalidateTx pDrv->file = %d\n", pMem,  pDrv->file);

    return;
}

void IcpuCacheFlushTx(U32 * pMem,  U32 nSize)
{
    int rc = 0;
    ICPU_DRV_DESC *pDrv = &CachedUioDrv[ICPU_TX_DESC];

    unsigned int params[3] =
    {
        (unsigned int)pMem,
        (unsigned int)((U32)pMem + nSize),
        (unsigned int)pDrv->address
    };

   if ((rc = ioctl(pDrv->file, UARM_IOCTL_CACHE_FLUSH, params)) != 0)
   {
       printf("IcpuCacheClean rc = %d\n", rc);
   }

    //printf("[0x%X] IcpuCacheCleanTx pDrv->file = %d\n", pMem, pDrv->file);

    return;
}

void IcpuCacheFlushRx(U32 * pMem,  U32 nSize)
{
    int rc = 0;
    ICPU_DRV_DESC *pDrv = &CachedUioDrv[ICPU_RX_DESC];

    unsigned int params[3] =
    {
        (unsigned int)pMem,
        (unsigned int)((U32)pMem + nSize),
        (unsigned int)pDrv->address
    };

   if ((rc = ioctl(pDrv->file, UARM_IOCTL_CACHE_FLUSH, params)) != 0)
   {
       printf("IcpuCacheClean rc = %d\n", rc);
   }

    //printf("[0x%X] IcpuCacheCleanRx pDrv->file = %d\n", pMem, pDrv->file);

    return;
}

#endif /* CACHED_ICPU */

#endif /* C4KEVM */

#endif /* USE_LINUX */

