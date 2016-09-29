/*
 * ICPU2 support user space library
 *
 * Copyright (C) 2011 Mindspeed Technologies, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "resultcodes.h"
#include "icpu2_lnx.h"
#include "msgroute.h"
#include "apidefs.h"
#include "diags.h"
#include "icpu.h"

#define ICPU2_USE_KERNEL_DRV

#ifdef ICPU2_USE_KERNEL_DRV

#define ICPU2_DEVNAME 	"/dev/icpu2"
#define ICPU2_WAIT_TYPE 1

#else //ICPU2_USE_KERNEL_DRV

#define ICPU2_DEVNAME 	NULL
#define ICPU2_WAIT_TYPE 0

#endif //ICPU2_USE_KERNEL_DRV 

struct icpu2    *icpu2=NULL;
OSAL_TASK       Icpu2RxTaskID;

static volatile U32 *ApbTimerReg = NULL;

RESULTCODE Icpu2Init(void)
{

    icpu2 = icpu2_init(ICPU2_DEVNAME);
    if (icpu2 == NULL)
    {
        return FAILURE;
    }
    else
    {
#if 0
        U32     rc, rc1;
        U8      *buf_rx;
        U8      cnt=0;

        do 
        {
            rc = icpu2_wait(icpu2, 0);

            if (rc)
            {
                buf_rx = 0;
                rc1 = icpu2_rx(icpu2, (void **)&buf_rx);
                if (buf_rx && rc1)
                {
                    cnt++;
                }
            }
        } while (rc && (cnt<128));

        if (cnt>=128)
        {
            printf("RxQueue reading during init (overflow)\n");
        }
#endif

        if(DevMemMap > 0)
        {
           void* map = mmap(NULL, 4096, PROT_READ, MAP_SHARED, DevMemMap, 0xFE050000);
	    if (map != MAP_FAILED)
           {   
                ApbTimerReg = (volatile U32*)((unsigned long)map + 4);
                DiagPrint(DIAG_LEVEL1, "ICPU", "DirectMode: ApbTimerReg %X\n", ApbTimerReg);
           }
        }

        IcpuHandle = icpu2->driver_fd;

        IcpuGetPhys = Icpu2GetPhys;
        IcpuGetVirt = Icpu2GetVirt;
        IcpuRxDirect = Icpu2RxDirect;
        MsgFreeBuffer = Msg2FreeBuffer;
        MsgAllocBuffer = Msg2AllocBuffer;

        IcpuSendMsg = Icpu2SendDirect;

        OsalCreateTask(&Icpu2RxTaskID, Icpu2RxTaskDirect, NULL, "Icpu2RxTask",
                       OSAL_PRIORITY_NORMAL, 0, 0);
        OsalSetTaskPriority(&Icpu2RxTaskID, 0);
        OsalActivateTask(&Icpu2RxTaskID);
    }

    return SUCCESS;
}



/** Send data to L-ARM
 *  Use direct mode (memory mapped I/O)
 *
 * @param size Size of data chunk
 * @param data Pointer to the data
 * @return Standard result code
 */
RESULTCODE Icpu2SendDirect(U32 size, PTR data)
{
    int rc;

    rc = icpu2_tx_allocated(icpu2, data, 0);
    if(rc == -EINVAL)
    {
        rc = icpu2_tx(icpu2, data, size, 0);
    }
    return rc;

}

/** This task dispatches data from L-ARM
 *  Use direct User I/O mode
 *
 * @param data Not used
 */
void Icpu2RxTaskDirect(PTR data)
{
    static PMSGHEADER pHdr;
    static void* pBlock;
    U32 rc, rc1;

    while (1)
    {
        rc = icpu2_wait(icpu2, 0);
        if (rc)
        {
            pBlock = NULL;
            rc1 = icpu2_rx(icpu2, &pBlock);
            if (pBlock && rc1)
            {
                pHdr = (PMSGHEADER)pBlock;
                DiagPrintApi(DIAG_LEVEL30, "ICPURX", DIAG_DECODETLV, pHdr);
                if (pHdr->control & CTRL_BYPASS)
                    DiagDumpPhyApi(DIAG_LEVEL29, MsgGetDataOffset(pHdr));
                MsgDispatch(MSGS_LARM, pHdr->length + sizeof(MSGHEADER), pHdr);
            }
        }
        else
        {
            usleep(5);
        }
    }
}


int Icpu2RxDirect(PTR data)
{
    static PMSGHEADER pHdr;
    static void* pBlock;
    U32 rc1, rc2 = 0;

    pBlock = NULL;
        rc1 = icpu2_rx(icpu2, &pBlock);
        if (pBlock && rc1)
        {
            pHdr = (PMSGHEADER)pBlock;
            DiagPrintApi(DIAG_LEVEL30, "ICPURX", DIAG_DECODETLV, pHdr);
            if (pHdr->control & CTRL_BYPASS)
                DiagDumpPhyApi(DIAG_LEVEL29, MsgGetDataOffset(pHdr));
            rc2 = pHdr->length + sizeof(MSGHEADER);
            memcpy(data, pHdr, rc2);
            Icpu2FreeBlock(pHdr);
        }
    return rc2;
}


PTR Icpu2AllocBlock(void)
{
    PTR pBlock = NULL;
    icpu2_tx_alloc(icpu2, &pBlock, 0);
    return (PTR)pBlock;
}

int Icpu2FreeBlock(PTR pMsg)
{
    return icpu2_rx_free(icpu2, (void *) pMsg);
}


struct icpu2 *icpu2_init(const char *driver_node)
{
    int sysfs_fd, len;
    void *buf, *s;
//    struct icpu2 *icpu2;
    unsigned int context_phys;

    /* if ICPU2 support is enabled - this file contains details of control data */
    sysfs_fd = open(ICPU2_SYSFS_INFO_PATH, O_RDONLY);
    if (sysfs_fd < 0)
        goto err;

    /* read proc file into buffer */
    buf = malloc(4*1024);
    if (!buf)
        goto err0;

    len = read(sysfs_fd, buf, 4096);
    if (len < 16)
        goto err1;

    /* search for queues array */
    s = strstr(buf, "Context: 0x");
    if (!s)    
        goto err1;
    if (sscanf(s, "Context: 0x%X\n", &context_phys) != 1)
        goto err1;

    icpu2 = calloc(1, sizeof(*icpu2));
    if (!icpu2)
        goto err1;

    /* map context, size is rather big - we want to cover queues in one shot */
    icpu2->context_mm = icpu2_mmap(context_phys, 1024*1024);
    if (!icpu2->context_mm)
        goto err2;
    icpu2->context = icpu2->context_mm->virt;

    icpu2->tx_block_size = icpu2->context->tx_block_size;
    icpu2->rx_block_size = icpu2->context->rx_block_size;
    icpu2->irq_mask = 1 << ((icpu2->context->tx_irq - 32) & 7);

    /* open driver node, can be ignored */
    icpu2->driver_fd = -1;

    if (driver_node) {
        icpu2->driver_fd = open(driver_node, O_RDWR);
        if (icpu2->driver_fd < 0)
            goto err3;
    }

    /* init virtual pointers to queues, queue structures are in array */
    icpu2->rxq_free.queue = &icpu2->context->queues[0];
    icpu2->rxq_priority.queue = &icpu2->context->queues[1];
    icpu2->rxq.queue = &icpu2->context->queues[2];
    icpu2->txq_free.queue = &icpu2->context->queues[3];
    icpu2->txq_priority.queue = &icpu2->context->queues[4];
    icpu2->txq.queue = &icpu2->context->queues[5];

    /* map ::ptr arrays */
    icpu2->rxq_free.ptr = (void*)icpu2->context + icpu2->rxq_free.queue->ptr_phys - context_phys;
    icpu2->rxq_priority.ptr = (void*)icpu2->context + icpu2->rxq_priority.queue->ptr_phys - context_phys;
    icpu2->rxq.ptr = (void*)icpu2->context + icpu2->rxq.queue->ptr_phys - context_phys;
    icpu2->txq_free.ptr = (void*)icpu2->context + icpu2->txq_free.queue->ptr_phys - context_phys;
    icpu2->txq_priority.ptr = (void*)icpu2->context + icpu2->txq_priority.queue->ptr_phys - context_phys;
    icpu2->txq.ptr = (void*)icpu2->context + icpu2->txq.queue->ptr_phys - context_phys;

    /* map tx/rx storages */
    icpu2->tx_storage_mm = icpu2_mmap(icpu2->context->tx_storage, icpu2->txq_free.queue->num * icpu2->tx_block_size);
    if (!icpu2->tx_storage_mm)
        goto err4;

    icpu2->rx_storage_mm = icpu2_mmap(icpu2->context->rx_storage, icpu2->rxq_free.queue->num * icpu2->rx_block_size);
    if (!icpu2->rx_storage_mm)
        goto err5;

    /* map INTC regs, used to notify opposite CPU */
    icpu2->intc_mm = icpu2_mmap(0xFE430000, 4096);
    if (!icpu2->intc_mm)
        goto err6;
    icpu2->intc_arm_set = icpu2->intc_mm->virt + 0x3C;

    free(buf);
    close(sysfs_fd);

    return icpu2;

err6:
    icpu2_munmap(icpu2->rx_storage_mm);
err5:
    icpu2_munmap(icpu2->tx_storage_mm);
err4:
    if (icpu2->driver_fd >= 0)
        close(icpu2->driver_fd);
err3:
    icpu2_munmap(icpu2->context_mm);
err2:
    free(icpu2);
err1:
    free(buf);
err0:
    close(sysfs_fd);
err:
    return NULL;
}

int icpu2_exit(struct icpu2 *icpu2)
{
    if (!icpu2)
        return -ENODEV;

    if (icpu2->driver_fd >= 0)
        close(icpu2->driver_fd);

    icpu2_munmap(icpu2->context_mm);

    icpu2_munmap(icpu2->tx_storage_mm);
    icpu2_munmap(icpu2->rx_storage_mm);

    icpu2_munmap(icpu2->intc_mm);

    free(icpu2);

    return 0;
}

void *icpu2_mmap_helper(unsigned int phys_start, unsigned int length)
{
    int fd;
    void *map = NULL;

    fd = open("/dev/mem", O_RDWR);
    if (fd >= 0) {
        map = mmap(0, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, phys_start);
        if (map == MAP_FAILED)
            map = NULL;
        close(fd);
    }

    return map;
}

struct icpu2_mm *icpu2_mmap(unsigned int phys_addr, unsigned int length)
{
    struct icpu2_mm *mm = malloc(sizeof(*mm));

    if (mm) {
        mm->phys = phys_addr;
        mm->length = length;
        mm->virt = icpu2_mmap_helper(mm->phys, mm->length);
        if (!mm->virt) {
            free(mm);
            mm = NULL;
        }
    }

    return mm;
}

int icpu2_munmap(struct icpu2_mm *mm)
{
    if (!mm)
        return -ENOMEM;

    if (mm->virt)
        munmap(mm->virt, mm->length);
    free(mm);

    return 0;    
}

int icpu2_wait(struct icpu2 *icpu2, int wait_type)
{
    int status = 0;

    if (!icpu2)
        return -ENODEV;

again:
    /* check both queues to be non-empty */
    if (icpu2->rxq_priority.queue->get != icpu2->rxq_priority.queue->put)
        status |= 1;

    if (icpu2->rxq.queue->get != icpu2->rxq.queue->put)
        status |= 2;

    /* user asked just check and exit, we also return immediately if any queue is non-empty */
    if (wait_type == 0 || status)
        return status;

    /* negative here means busy wait in user space, retry queue check */
    if (wait_type < 0)
        goto again;

    /* positive - ask driver */
    if (icpu2->driver_fd < 0)
        return -ENXIO;

    status = ioctl(icpu2->driver_fd, ICPU2_IOCTL_WAIT, 0);
    if (status >= 0)
        goto again;    /* if ioctl fails, return status, otherwise check queues again */

    return status;
}

static int __icpu2_queue_is_empty(struct icpu2_local_queue *q)
{
    return q->queue->put == q->queue->get;
}

static unsigned int __icpu2_queue_get_update(struct icpu2_local_queue *q)
{
    unsigned int get = q->queue->get+1;

    if (get >= q->queue->num)
        get = 0;

    return get;
}

static unsigned int __icpu2_queue_get(struct icpu2_local_queue *q, unsigned int get)
{
    unsigned int index = q->ptr[q->queue->get];
    q->queue->get = get;
    q->queue->stat_get++;
    return index;
}

static unsigned int __icpu2_queue_put_update(struct icpu2_local_queue *q)
{
    unsigned int put = q->queue->put+1;

    if (put >= q->queue->num)
        put = 0;

    return put;
}

static void __icpu2_queue_put(struct icpu2_local_queue *q, unsigned int index, unsigned int put)
{
    q->ptr[q->queue->put] = index;
    q->queue->put = put;
    q->queue->stat_put++;
}

static void *__icpu2_rx(struct icpu2 *icpu2, struct icpu2_local_queue *q)
{
    unsigned int block_index;

    if (__icpu2_queue_is_empty(q))
        return NULL;

    block_index = __icpu2_queue_get(q, __icpu2_queue_get_update(q));
    if (block_index >= q->queue->num) {
        /* invalid block index - can't happen, abort */
        fprintf(stderr, "ICPU2: invalid index %u in queue (max is %u)\n", block_index, q->queue->num - 1);
        abort();
    }

    return icpu2->rx_storage_mm->virt + block_index*icpu2->rx_block_size;
}

int icpu2_rx(struct icpu2 *icpu2, void **block)
{
    void *block_virt;

    if (!icpu2)
        return -ENODEV;
    if (!block)
        return -EINVAL;

    block_virt = __icpu2_rx(icpu2, &icpu2->rxq_priority);
    if (block_virt) {
        *block = block_virt;
        return 1;
    }

    block_virt = __icpu2_rx(icpu2, &icpu2->rxq);
    if (block_virt) {
        *block = block_virt;
        return 2;
    }

    return 0;
}

int icpu2_rx_free(struct icpu2 *icpu2, void *block_virt)
{
    unsigned int put;
    struct icpu2_local_queue *q;
    struct icpu2_mm *s;
    unsigned int block_index;

    if (!icpu2)
        return -ENODEV;

    q = &icpu2->rxq_free;
    s = icpu2->rx_storage_mm;

    /* convert virtual address to block index */
    block_index = (block_virt - s->virt) / icpu2->rx_block_size;

    /* user may pass something weird into the call, don't abort, just signal error */
    if (block_index >= q->queue->num)
        return -EINVAL;

    put = __icpu2_queue_put_update(q);
    if (put == q->queue->get) {
        /* queue is full - can't happen, abort */
        fprintf(stderr, "ICPU2: no space for free blocks\n");
        abort();
    }

    __icpu2_queue_put(q, block_index, put);

    return 0;
}

int icpu2_tx(struct icpu2 *icpu2, void *buf, unsigned int length, int priority)
{
    void *block;

    if (!icpu2)
        return -ENODEV;

    if (length > icpu2->tx_block_size)
        return -EINVAL;

    if (icpu2_tx_alloc(icpu2, &block, 0))
        return -ENOSPC;

    memcpy(block, buf, length);

    return icpu2_tx_allocated(icpu2, block, priority);
}

int icpu2_tx_alloc(struct icpu2 *icpu2, void **block, int wait) {
    unsigned int get;
    struct icpu2_local_queue *q;
    struct icpu2_mm *s;
    unsigned int block_index;

    if (!icpu2)
        return -ENODEV;
    if (!block)
        return -EINVAL;

    q = &icpu2->txq_free;
    s = icpu2->tx_storage_mm;

    get = __icpu2_queue_get_update(q);
    if (get == q->queue->put) {
        /* no free blocks, check if wait is allowed, quit immediately if not */
        if (!wait)
            return -ENOSPC;

        while (get == q->queue->put)
            usleep(100);
    }

    /* got space, get free block index, perorm simple sanity check */
    block_index = __icpu2_queue_get(q, get);
    if (block_index >= q->queue->num) {
        /* invalid block index - can't happen, abort */
        fprintf(stderr, "ICPU2: invalid index %u in queue (max is %u)\n", block_index, q->queue->num - 1);
        abort();
    }

    *block = s->virt + block_index*icpu2->tx_block_size;

    return 0;
}

int icpu2_tx_allocated(struct icpu2 *icpu2, void *block_virt, int priority)
{
    unsigned int put;
    struct icpu2_local_queue *q;
    struct icpu2_mm *s;
    unsigned int block_index;

    if (!icpu2)
        return -ENODEV;

    q = priority ? &icpu2->txq_priority : &icpu2->txq;
    s = icpu2->tx_storage_mm;

    block_index = (block_virt - s->virt) / icpu2->tx_block_size;

    /* user may pass something weird into the call, don't abort, just signal error */
    if (block_index >= q->queue->num)
        return -EINVAL;

    put = __icpu2_queue_put_update(q);
    if (put == q->queue->get) {
        /* queue is full - can't happen, abort */
        fprintf(stderr, "ICPU2: no space for free blocks\n");
        abort();
    }

    __icpu2_queue_put(q, block_index, put);

    /* raise IRQ to notify the opposite CPU */
    *icpu2->intc_arm_set = icpu2->irq_mask;

    return 0;
}

PTR Icpu2GetPhys(PTR pAddr)
{

    U8* phys;
    phys = (U8*)((U32)pAddr - (U32)(icpu2->tx_storage_mm->virt) + (U32)(icpu2->tx_storage_mm->phys));
    return (PTR)phys;
}

PTR Icpu2GetVirt(PTR pAddr)
{
    U8* virt;
    virt = (U8*)((U32)pAddr - (U32)(icpu2->rx_storage_mm->phys) + (U32)(icpu2->rx_storage_mm->virt));
    return (PTR)virt;
}

