/** @file idma.c
 *
 * @brief IDMA/MDMA/EDMA module
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#if defined (USE_BARE) || defined (USE_RTXC)

#include "idma.h"
#include "apb.h"
#include "gemac.h"
#include "isr.h"
#include "intc.h"

//
// Memory arrays to use with DMA
//
extern GEMAC_FRAME Gemac0RxMem[];
extern GEMAC_FRAME Gemac1RxMem[];

//
// Frame linked lists
//
extern GEMAC_FRAME *Gemac0TxHead;
extern GEMAC_FRAME *Gemac1TxHead;

/** Initialize IDMA
 *
 * @param cpuid CPU ID on which this function called
 * @param idmaMask Select what interfaces to initialize (bitmask)
 * @return Standard result code
 */
RESULTCODE IdmaInit(U32 cpuid, U32 idmaMask)
{
    if (idmaMask & IDMA_EDMA0)
    {
        IdmaStop(IDMA_EDMA0);

        // Memory to EMAC0 (Tx)
        IOWrite32A(MMEM_BURST(0), 255); // burst size 255
        IOWrite32A(MMEM_START(0), 0);

        // EMAC0 to Memory (Rx)
        IOWrite32A(EMMM_BURST(0), 255); // burst size 255
        IOWrite32A(EMMM_START(0), 0);

        // Enable EDMARX/TX from GEMAC0
        IsrAddIrqHandler(INTC_EDMA0RX, Edma0RxHandler);
        IntcEnableIrq(cpuid, INTC_EDMA0RX);

        IsrAddIrqHandler(INTC_EDMA0TX, Edma0TxHandler);
        IntcEnableIrq(cpuid, INTC_EDMA0TX);
    }

    if (idmaMask & IDMA_EDMA1)
    {
        IdmaStop(IDMA_EDMA1);

        // Memory to EMAC1 (Tx)
        IOWrite32A(MMEM_BURST(1), 255); // burst size 255
        IOWrite32A(MMEM_START(1), 0);

        // EMAC1 to Memory (Rx)
        IOWrite32A(EMMM_BURST(1), 255); // burst size 255
        IOWrite32A(EMMM_START(1), 0);

        // Enable EDMARX/TX from GEMAC0
        IsrAddIrqHandler(INTC_EDMA1RX, Edma1RxHandler);
        IntcEnableIrq(cpuid, INTC_EDMA1RX);

        IsrAddIrqHandler(INTC_EDMA1TX, Edma1TxHandler);
        IntcEnableIrq(cpuid, INTC_EDMA1TX);
    }
    return SUCCESS;
}

/** Start IDMA transaction
 *
 * @param idmaMask Which interface(s) to start (bitmask)
 * @return Standard result code
 */
RESULTCODE IdmaStart(U32 idmaMask)
{
    if (idmaMask & IDMA_EDMA0)
    {
        // Start EDMA0 RX
        IOWrite32A(EMMM_HEAD(0), (U32)&Gemac0RxMem);
        IOWriteOr32A(EMMM_START(0), IDMA_START);
    }

    if (idmaMask & IDMA_EDMA1)
    {
        // Start EDMA0 RX
        IOWrite32A(EMMM_HEAD(1), (U32)&Gemac1RxMem);
        IOWriteOr32A(EMMM_START(1), IDMA_START);
    }
    return SUCCESS;
}

/** Stop IDMA
 *
 * @param idmaMask Which interface(s) to stop (bitmask)
 * @return Standard result code
 */
RESULTCODE IdmaStop(U32 idmaMask)
{
    // GEMAC0
    if (idmaMask & IDMA_EDMA0)
    {
        IOWrite32A(MMEM_SOFT_RESET(0), 1);
        IOWrite32A(EMMM_SOFT_RESET(0), 1);
    }

    // GEMAC1
    if (idmaMask & IDMA_EDMA1)
    {
        IOWrite32A(MMEM_SOFT_RESET(1), 1);
        IOWrite32A(EMMM_SOFT_RESET(1), 1);
    }

    // Inbound MDMA
    if (idmaMask & IDMA_MDMAIN)
    {

    }

    // Outbound MDMA
    if (idmaMask & IDMA_MDMAOUT)
    {

    }
    return SUCCESS;
}

/** Interrupt handler for EDMA0 Rx
 */
void Edma0RxHandler(void)
{
}

/** Interrupt handler for EDMA0 Tx
 */
void Edma0TxHandler(void)
{
    Gemac0TxHead = NULL;
}

/** Interrupt handler for EDMA1 Rx
 */
void Edma1RxHandler(void)
{
}

/** Interrupt handler for EDMA1 Tx
 */
void Edma1TxHandler(void)
{
    Gemac1TxHead = NULL;
}

#endif /* USE_BARE || USE_RTXC */
