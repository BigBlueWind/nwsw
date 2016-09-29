/** @file gemac.c
 *
 * @brief GEMAC driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.6 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifdef USE_BARE

#include "gemac.h"
#include "gemac_hal.h"
#include "apb.h"
#include "intc.h"
#include "isr.h"
#include "portio.h"
#include "arbiter.h"
#include "idma.h"
#include "osal.h"
#include "sysctrl.h"
#include <stdarg.h>

// TODO: this is STUB
#include "packetif.h"

GEM_DEVICE Gemac0;
GEM_DEVICE Gemac1;

//
// Task IDs
//
OSAL_TASK Gemac0RxTaskID;
OSAL_TASK Gemac0TxTaskID;
OSAL_TASK Gemac1RxTaskID;
OSAL_TASK Gemac1TxTaskID;

OSAL_MEMARRAY Gemac0Rx;
OSAL_MEMARRAY Gemac0Tx;
OSAL_MEMARRAY Gemac1Rx;
OSAL_MEMARRAY Gemac1Tx;

OSAL_SEMAPHORE Gemac0RxSema;
OSAL_SEMAPHORE Gemac0TxSema;
OSAL_SEMAPHORE Gemac1RxSema;
OSAL_SEMAPHORE Gemac1TxSema;

U8 Gemac0Addr[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
U8 Gemac1Addr[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

//
// Placeholders for OSAL_MEMARRAY
//
U8 Gemac0TxMem[sizeof(GEMAC_FRAME)*GEM_MAX_TXQ] __attribute__((aligned(32)));
U8 Gemac1TxMem[sizeof(GEMAC_FRAME)*GEM_MAX_RXQ] __attribute__((aligned(32)));

//
// To be used by inbound EDMA
//
GEMAC_FRAME Gemac0RxMem[GEM_MAX_TXQ] __attribute__((aligned(32)));
GEMAC_FRAME Gemac1RxMem[GEM_MAX_TXQ] __attribute__((aligned(32)));

GEMAC_FRAME *Gemac0TxHead = NULL;
GEMAC_FRAME *Gemac1TxHead = NULL;

/** Prepare data structures for GEMAC Rx
 *
 *  \note GEMAC0 for now
 */
void GemacPrepareRx(void)
{
    GEMAC_FRAME *frm = Gemac0RxMem;
    U32 i;

    for (i = 0; i < GEM_MAX_RXQ-1; i++)
    {
        frm->control.next = (struct IdmaDesc*) (frm+1);
        frm->control.system = 0;
        frm->control.status = 0;
        frm->control.control = IDMA_FCONTROL_FREADY /*| IDMA_FCONTROL_IRQEN*/;

        // TODO: Rewrite when scattering enabled
        frm->control.descr[0].pointer = frm->data.data32;
        frm->control.descr[0].control = GEM_MAX_FRAME;

        frm++;
    }
    // Last frame in chain
    frm->control.next = NULL;
    frm->control.system = 0;
    frm->control.status = 0;
    frm->control.control = IDMA_FCONTROL_FLAST /*| IDMA_FCONTROL_IRQEN*/;
    // TODO: Rewrite when scattering enabled
    frm->control.descr[0].pointer = frm->data.data32;
    frm->control.descr[0].control = GEM_MAX_FRAME;

    CacheDCCleanInvalidateRange((U32)Gemac0RxMem, ((U32)Gemac0RxMem) + sizeof(Gemac0RxMem));
}

/** Initialize GEMAC module
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
RESULTCODE GemacInit(U32 cpuid)
{
    if (OsalCreateMemArray(&Gemac0Tx, Gemac0TxMem, sizeof(GEMAC_FRAME) * GEM_MAX_TXQ,
                           sizeof(GEMAC_FRAME)) != SUCCESS)
        return FAILURE;

    if (OsalCreateMemArray(&Gemac1Tx, Gemac1TxMem, sizeof(GEMAC_FRAME) * GEM_MAX_TXQ,
                           sizeof(GEMAC_FRAME)) != SUCCESS)
        return FAILURE;

    if (OsalCreateSema(&Gemac0RxSema) != SUCCESS)
        return FAILURE;

    if (OsalCreateSema(&Gemac0TxSema) != SUCCESS)
        return FAILURE;

    if (OsalCreateSema(&Gemac1RxSema) != SUCCESS)
        return FAILURE;

    if (OsalCreateSema(&Gemac1TxSema) != SUCCESS)
        return FAILURE;

    GemacPrepareRx();

    if (OsalCreateTask(&Gemac0RxTaskID, Gemac0RxTask, NULL, "Gemac0RxTask",
                       OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
    {
        return FAILURE;
    }
    OsalActivateTask(&Gemac0RxTaskID);

    if (OsalCreateTask(&Gemac0TxTaskID, Gemac0TxTask, NULL, "Gemac0TxTask",
                       OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
    {
        return FAILURE;
    }
    OsalActivateTask(&Gemac0TxTaskID);

    if (OsalCreateTask(&Gemac1RxTaskID, Gemac1RxTask, NULL, "Gemac1RxTask",
                       OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
    {
        return FAILURE;
    }
    OsalActivateTask(&Gemac1RxTaskID);

    if (OsalCreateTask(&Gemac1TxTaskID, Gemac1TxTask, NULL, "Gemac1TxTask",
                       OSAL_PRIORITY_NORMAL, 0, 0) != SUCCESS)
    {
        return FAILURE;
    }
    OsalActivateTask(&Gemac1TxTaskID);

    IdmaInit(cpuid, IDMA_EDMA0 | IDMA_EDMA1);

    // Init GEMAC0
    Gemac0.config = (V32*)GEMAC_GEM_CONFIG(0);
    Gemac0.registers = (GEM_REG*) APB_GEMIP_BASE(0);

    // Init GEMAC1
    Gemac1.config = (V32*)GEMAC_GEM_CONFIG(1);
    Gemac1.registers = (GEM_REG*) APB_GEMIP_BASE(1);

    GemStopTx(&Gemac0);
    GemDisableRx(&Gemac0);

    // GEMAC0
    IOWrite32A(GEMAC_GEM_CONFIG(0), GEM_CONF_MODE_GEM_GMII |
               GEM_CONF_MODE_SEL_GEM | GEM_CONF_DUPLEX_SEL_GEM |
               GEM_CONF_DUPLEX_GEM_FULL);
    IOWrite32A(GEMAC_GEM_TX_CTRL(0), GEM_TXCTRL_DMAIF_EN | GEM_TXCTRL_CRC_EN |
               GEM_TXCTRL_RETR_EN);
    IOWrite32A(GEMAC_GEM_RX_CTRL(0), GEM_RXCTRL_DMAIF_EN);
    IOWrite32A(GEMAC_GEM_STAT_PKT_SIZE(0), 0x100);

    // Init DMA TX/RX Data FIFO Config Registers
    IOWrite32A(GEMAC_GEM_FIFO_RXHIGH(0), 0x17);
    IOWrite32A(GEMAC_GEM_FIFO_RXLOW(0), 0x16);
    IOWrite32A(GEMAC_GEM_FIFO_CONTROL(0), GEM_FIFO_CTRL_TXFF_EN |
               GEM_FIFO_CTRL_HBTXRQ_EN | GEM_FIFO_CTRL_RXFF_EN |
               GEM_FIFO_CTRL_HBRXRQ_EN | GEM_FIFO_CTRL_TXCP_INH);

    // ARM side
    IOWrite32A(GEMAC_ARM_FIFO_RXHIGH(0), 0xE0);
    IOWrite32A(GEMAC_ARM_FIFO_RXLOW(0), 0x40);
    IOWrite32A(GEMAC_ARM_FIFO_CONTROL(0), ARM_FIFO_RXDREQWE | ARM_FIFO_TXDREQRE);

    GemDisableCopyAll(&Gemac0);
    GemAllowBroadcast(&Gemac0);
    GemEnableUnicast(&Gemac0);
    GemEnableMulticast(&Gemac0);
    GemDisableFcsRx(&Gemac0);

    GemEnetAddrByteMac(Gemac0Addr, &Gemac0.macAddr);
    GemSetLAddr1(&Gemac0, &Gemac0.macAddr);

    GemStartTx(&Gemac0);
    GemEnableRx(&Gemac0);

    IdmaStart(IDMA_EDMA0);

    return SUCCESS;
}

/** Prepare GEMAC for Tx process
 *
 * @return Standard result code
 */
RESULTCODE GemacStartTx(void)
{
    CacheDCCleanInvalidateRange((U32)Gemac0TxMem, (U32)Gemac0TxMem + sizeof(Gemac0TxMem));
    // Start DMA
    IOWrite32A(MMEM_HEAD(0), (U32)Gemac0TxHead);
    IOWriteOr32A(MMEM_START(0), 1);

    return SUCCESS;
}

// TODO: Create 2 versions: with memcpy() and with direct buffers

/** Send data block to GEMAC
 *
 * @param size Size of block in bytes
 * @param data Pointer to data
 * @return Standart result code
 */
RESULTCODE GemacWrite(U32 size, PTR data)
{
    GEMAC_FRAME *frm;
    U32 *src = (U32*) data;
    U32 *dest;
    U32 i;

    if (size > GEM_MAX_FRAME)
        return FAILURE;

    if (OsalAllocMemArray(&Gemac0Tx, (OSAL_PADDRESS*)&frm) != SUCCESS)
    {
        return FAILURE;
    }

    // Convert to size in words
    size += sizeof(U32)-1;
    size /= sizeof(U32);

    // TODO: use memcpy()
    dest = frm->data.data32;
    for (i = 0; i < size; i++)
        *dest++ = *src++;

    if (Gemac0TxHead == NULL)
    {
        frm->control.next = NULL;
        Gemac0TxHead = frm;
    }
    else
    {
        frm->control.next = (PTR)Gemac0TxHead;
    }
    frm->control.control |= IDMA_FCONTROL_FREADY;

    // Single data buffer
    frm->control.descr[0].pointer = frm->data.data32;
    frm->control.descr[0].control = size * sizeof(U32) | IDMA_BCONTROL_BLAST;

    GemacStartTx();

    return SUCCESS;
}

/** Add Buffer Descriptor to the chain
 *
 * @param frame Pointer to GEMAC structure
 * @param index Index of Buffer Descriptor
 * @param size Size of data in bytes
 * @param data Pointer to data buffer
 * @return Standard result code
 */
RESULTCODE GemacAddBDesc(GEMAC_FRAME *frame, U32 index, U32 size, PTR data)
{
    // FIXME: Other fields
    frame->control.descr[index].pointer = data;
    frame->control.descr[0].control = size | IDMA_BCONTROL_BLAST;

    return SUCCESS;
}

/** @brief Write up to IDMA_MAX_BDESC blocks
 *         Each block must be defined by pair of [sizeNN, dataNN]
 *
 * @param blkNum Block index
 * @param size Size of data in bytes
 * @param data Pointer to data
 * @return Standard result code
 */
RESULTCODE GemacWriteBlocks(U32 blkNum, U32 size, PTR data, ...)
{
    GEMAC_FRAME *frm;
    U32 i;
    U32 sizeVar;
    PTR dataVar;

    va_list paramList;

    if (blkNum > IDMA_MAX_BDESC)
        return FAILURE;

    if (size > GEM_MAX_FRAME)
        return FAILURE;

    if (OsalAllocMemArray(&Gemac0Tx, (OSAL_PADDRESS*)&frm) != SUCCESS)
    {
        return FAILURE;
    }

    GemacAddBDesc(frm, 0, size, data);

    va_start(paramList, data);
    for (i = 1; i < blkNum; i++)
    {
        sizeVar = va_arg(paramList, U32);
        dataVar = va_arg(paramList, PTR);
        if (sizeVar > GEM_MAX_FRAME)
        {
            OsalFreeMemArray(&Gemac0Tx, (OSAL_PADDRESS*)frm);
            va_end(paramList);          // Needed?
            return FAILURE;
        }
        GemacAddBDesc(frm, i, sizeVar, dataVar);
    }
    frm->control.descr[i-1].control |= IDMA_BCONTROL_BLAST;

    va_end(paramList);

    GemacStartTx();

    return SUCCESS;
}

/** This task handles Rx requests on GEMAC0
 *
 * @param taskParm Not used
 */
void Gemac0RxTask(PTR taskParm)
{
    while (1)
    {
        if (OsalWaitForSema(&Gemac0RxSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            OsalClearSema(&Gemac0RxSema);
            // TODO: Do something
        }
    }
}

/** This task handles Tx requests on GEMAC0
 *
 * @param taskParm Not used
 */
void Gemac0TxTask(PTR taskParm)
{
    GEMAC_FRAME *frmList, *frmTmp;

    while (1)
    {
        if (OsalWaitForSema(&Gemac0TxSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            OsalClearSema(&Gemac0TxSema);
            frmList = Gemac0TxHead;
            // Collect garbage
            while (frmList)
            {
                frmTmp = (GEMAC_FRAME*) frmList->control.next;
                if (OsalFreeMemArray(&Gemac0Tx, frmList) != SUCCESS)
                {
                    // TODO: Alert
                    break;
                }
                frmList = frmTmp;
            }
        }
    }
}

/** This task handles Rx requests on GEMAC1
 *
 * @param taskParm Not used
 */
void Gemac1RxTask(PTR taskParm)
{
    while (1)
    {
        if (OsalWaitForSema(&Gemac1RxSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            OsalClearSema(&Gemac1RxSema);
            // TODO: Do something
        }
    }
}

/** This task handles Tx requests on GEMAC1
 *
 * @param taskParm Not used
 */
void Gemac1TxTask(PTR taskParm)
{
    GEMAC_FRAME *frmList, *frmTmp;

    while (1)
    {
        if (OsalWaitForSema(&Gemac1TxSema, OSAL_WAIT_INFINITE) == SUCCESS)
        {
            // TODO: Move to another CPU?
            if (OsalCheckSema(&Gemac1TxSema) == SUCCESS)
            {
                OsalClearSema(&Gemac1TxSema);
                frmList = Gemac1TxHead;
                // Collect garbage
                while (frmList)
                {
                    frmTmp = (GEMAC_FRAME*) frmList->control.next;
                    if (OsalFreeMemArray(&Gemac1Tx, frmList) != SUCCESS)
                    {
                        // TODO: Alert
                        break;
                    }
                    frmList = frmTmp;
                }
            }
        }
    }
}

// TODO: this is stub
RESULTCODE NetEthInit(BOOLEAN bOpen, OSAL_HEAP *pHeap, MacCB cb)
{
    return SUCCESS;
}

#endif /* USE_BARE */
