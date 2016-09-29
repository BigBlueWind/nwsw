/** @file icpu.h
 *
 * @brief Inter-CPU communication definitions and interfaces
 * @author Mindspeed Technologies
 * @version $Revision: 1.31 $
 *
 * COPYRIGHT(c) 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef ICPU_H_
#define ICPU_H_

#include "resultcodes.h"
#include "apidefs.h"

//
// Operation modes
//
#define IMODE_RELEASE               0       // Access to I-CPU device
#define IMODE_DEMO                  1       // Use mailboxes
#define IMODE_SHM                   2       // Use shared memory
#define IMODE_FORCE_DEVICE          3       // Force device mode even if mailboxes present
#define IMODE_DIRECT                4       // Direct memory mapping
#define IMODE_HBI                   5       // communication via HBI

//
// Direct (memory-mapped) mode

// BSP before 1.82
#define ICPU_RXENTRIES              64
#define ICPU_TXENTRIES              64

#define ICPU_RXENTRIES_HI              64
#define ICPU_TXENTRIES_HI              64

#define ICPU_RXENTRIES_HI2              256
#define ICPU_TXENTRIES_HI2              256

//
// Initialization
//
RESULTCODE IcpuInit(U32 cpuid);
RESULTCODE IcpuDone(void);

//
// Pointer to actual function
//
extern int IcpuMode;
extern RESULTCODE (*IcpuSendMsg)(U32 size, PTR data);
extern void (*IcpuRxTask)(PTR data);
RESULTCODE IcpuSendShm(U32 size, PTR data);

//
// Tasks
//
#ifdef USE_WINDOWS
void IcpuRxGet(PTR data);
RESULTCODE IcpuTxPut(U32 size, PTR data);
EXPORT PTR IcpuTxGet(void);
#endif

//
// Direct memory-mapped I/O
//
#if defined (USE_LINUX) && defined (C4KEVM)

typedef struct IcpuPartBlock
{
    union
    {
        volatile PTR next;                      // Pointer to a next free block (physical address)
        U32 block[MSG_MAXSIZE / sizeof(U32)];   // Block filler
        MSGHEADER header;                       // Easier access
    };
} ICPU_PART_BLOCK;

typedef struct IcpuQueue
{
    U32 *storage;       // Physical address of array of blocks
    volatile U16 get;   // Read index
    volatile U16 put;   // Write index
    U16 count;          // Element count
    U16 size;           // Currently it is sizeof(int)
    volatile U32 *lock; // Physical address of I-CPU hardware semaphore
} ICPU_QUEUE;

typedef struct IcpuPart
{
    ICPU_PART_BLOCK *storage; // Physical address
    volatile PTR free;  // Free block (physical address)
    U32 size;           // Block size
    U32 count;          // Element count
    volatile U32 *lock; // Physical address of I-CPU hardware semaphore
    U32 allocated;      // Number of allocated blocks
} ICPU_PART;

typedef struct IcpuComm
{
    ICPU_QUEUE queue;
    ICPU_PART part;
} ICPU_COMM;

typedef struct _IcpuStats
{
    U32 TxRealloc;
    U32 TxTotal;
    U32 RxTotal;
}ICPU_STATS;

extern ICPU_STATS IcpuStats;

PTR IcpuAllocBlock(void);
RESULTCODE IcpuFreeBlock(PTR block);
typedef int (*IcpuRxDirectCb)(PTR data);
extern IcpuRxDirectCb IcpuRxDirect;
int Icpu1RxDirect(PTR data);

extern int IcpuHandle;
extern int DevMemMap;

PTR Icpu1GetPhys(PTR virtaddr);
PTR Icpu1GetVirt(PTR physaddr);

typedef PTR               (*IcpuGetPhysCb)(PTR virtaddr);
typedef PTR               (*IcpuGetVirtCb)(PTR physaddr);
extern IcpuGetPhysCb        IcpuGetPhys;
extern IcpuGetVirtCb        IcpuGetVirt;
#endif /* USE_LINUX && C4KEVM */

#endif /* ICPU_H_ */
