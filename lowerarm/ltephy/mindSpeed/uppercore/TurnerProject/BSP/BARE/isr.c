/** @file isr.c
 *
 * @brief Interrupts and exceptions handling
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#include "isr.h"
#include "apb.h"
#include "intc.h"
#include "gpio.h"
#include "sysctrl.h"
#include "uart.h"

#ifdef USE_RTXC
extern __irq void IrqHandler(void);
extern __irq void IrqHandlerL(void);
#endif /*USE_RTXC*/

#ifndef SIMUPC

//
// Exception modes stacks
//
U32 UndefStack[256];        // Upper ARM
U32 SvcStack[256];
U32 PrefetchStack[256];
U32 AbortStack[256];
U32 IrqStack[256];
U32 FiqStack[256];

U32 UndefStackL[256];       // Lower ARM
U32 SvcStackL[256];
U32 PrefetchStackL[256];
U32 AbortStackL[256];
U32 IrqStackL[256];
U32 FiqStackL[256];

//
// Interrupt vectors for FIQ/IRQ
//
IRQ_HANDLER IsrVectorTable[INTC_MAX_VEC] =
    {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    };

//
// Exception handlers
//

/** Undefined instruction exception handler on ARM0
 */
__irq void UndefHandler(void)
{
    CacheDCClean();
    while (1)
    {
    }
}

/** SWI (SVC) exception handler on ARM0
 */
__irq void SvcHandler(void)
{
    CacheDCClean();
    while (1)
    {
    }
}

/** Prefetch Abort exception handler on ARM0
 */
__irq void PrefetchHandler(void)
{
    CacheDCClean();
    while (1)
    {
    }
}

/** Data Abort exception handler on ARM0
 */
__irq void AbortHandler(void)
{
    static char* ptr = "Data abort\r\n";
    __disable_irq();
    while (*ptr)
    {
        IOWrite32A(UART_RBR, *ptr++);
        while ((IORead32A(UART_LSR) & LSR_THRE) == 0)
        {
            __nop();
        }
    }
    CacheDCClean();
    while (1)
    {
    }
}

/** Actual interrupt handler routine called from IRQ context (UARM)
 */
void IrqHandlerCb(void)
{
    U32 winner, srcMask, bitMask, idx;

    // Single Winner register (32 sources)
    while ((winner = IORead32A(INTC_IRQ0_WIN)) != INTC_NOWINNER)
    {
        if (IsrVectorTable[winner] != NULL)
            IsrVectorTable[winner]();
        // Clear out IRQ mask
        IOWrite32A(INTC_STATUS_REG0, 1UL << winner);
    }

    // Read next part of IRQ sources
    srcMask = IORead32A(INTC_STATUS_REG0) & IORead32A(INTC_ARM0_IRQMASK0);
    idx = INTC_PTP0;
    bitMask = 1 << INTC_PTP0;
    while (srcMask)
    {
        if (srcMask & bitMask)
        {
            if (IsrVectorTable[idx] != NULL)
                IsrVectorTable[idx]();
            // Clear out IRQ mask
            IOWrite32A(INTC_STATUS_REG0, bitMask);
        }
        idx++;
        srcMask &= ~bitMask;
        bitMask <<= 1;
    }

    // Read next part of IRQ sources
    srcMask = IORead32A(INTC_STATUS_REG1) & IORead32A(INTC_ARM0_IRQMASK1);
    idx = INTC_GPIO0;
    bitMask = 1 << (INTC_GPIO0 - 32);
    while (srcMask)
    {
        if (srcMask & bitMask)
        {
            if (IsrVectorTable[idx] != NULL)
                IsrVectorTable[idx]();
            // Clear out IRQ mask
            IOWrite32A(INTC_STATUS_REG1, bitMask);
        }
        idx++;
        srcMask &= ~bitMask;
        bitMask <<= 1;
    }
}

#ifdef USE_LINUX
/** Dummy function
 *
 */
__irq void IrqHandler(void)
{
}

/** Dummy function
 *
 */
__irq void IrqHandlerL(void)
{
}
#endif /* USE_LINUX */

#ifdef USE_BARE
/** IRQ handler on ARM0
 */
__irq void IrqHandler(void)
{
    IrqHandlerCb();
}
#endif

/** @brief FIQ handler for Upper ARM
 *
 *  @note Not used now
*/
__irq void FiqHandler(void)
{
}

/** Undefined Instruction exception handler on LARM
 */
__irq void UndefHandlerL(void)
{
    CacheDCClean();
    while (1)
    {
    }
}

/** SWI (SVC) exception handler on LARM
 */
__irq void SvcHandlerL(void)
{
    CacheDCClean();
    while (1)
    {
    }
}

/** Prefetch Abort exception handler on LARM
 */
__irq void PrefetchHandlerL(void)
{
    CacheDCClean();
    while (1)
    {
    }
}

/*! @brief Lower ARM Data Abort exception handler
 *
 *  Handles true Data Abort as well as SDRAM size detection (in UARM mode)
*/
__irq void AbortHandlerL(void)
{
    CacheDCClean();
    while (1)
    {
    }
}

/** Actual interrupt handler routine called from IRQ context (LARM)
 *
 */
void IrqHandlerLCb(void)
{
    U32 winner, srcMask, bitMask, idx;

    // Single Winner register (32 sources)
    while ((winner = IORead32A(INTC_IRQ1_WIN)) != INTC_NOWINNER)
    {
        if (IsrVectorTable[winner] != NULL)
            IsrVectorTable[winner]();
        // Clear out IRQ mask
        IOWrite32A(INTC_STATUS_REG0, 1UL << winner);
    }

    // Read next part of IRQ sources
    srcMask = IORead32A(INTC_STATUS_REG0) & IORead32A(INTC_ARM1_IRQMASK0);
    idx = INTC_PTP0;
    bitMask = 1 << INTC_PTP0;
    while (srcMask)
    {
        if (srcMask & bitMask)
        {
            if (IsrVectorTable[idx] != NULL)
                IsrVectorTable[idx]();
            // Clear out IRQ mask
            IOWrite32A(INTC_STATUS_REG0, bitMask);
        }
        idx++;
        srcMask &= ~bitMask;
        bitMask <<= 1;
    }

    // Read next part of IRQ sources
    srcMask = IORead32A(INTC_STATUS_REG1) & IORead32A(INTC_ARM1_IRQMASK1);
    idx = INTC_GPIO0;
    bitMask = 1 << (INTC_GPIO0 - 32);
    while (srcMask)
    {
        if (srcMask & bitMask)
        {
            if (IsrVectorTable[idx] != NULL)
                IsrVectorTable[idx]();
            // Clear out IRQ mask
            IOWrite32A(INTC_STATUS_REG1, bitMask);
        }
        idx++;
        srcMask &= ~bitMask;
        bitMask <<= 1;
    }
}

#ifdef USE_BARE
__irq void IrqHandlerL(void)
{
    IrqHandlerLCb();
}
#endif

/*! \brief FIQ handler for Lower ARM
 *         Not used now
*/
__irq void FiqHandlerL(void)
{
}

/*! @brief Add specified IRQ handler to the list
 *
 *  @param vector Vector number
 *  @param irqHandler Pointer to function to handle IRQ
 *  @return Standard result code
*/
RESULTCODE IsrAddIrqHandler(U32 vector, IRQ_HANDLER irqHandler)
{
    U32 flags;

    if (vector >= INTC_MAX_VEC)
        return FAILURE;

    // TODO: replace with __disable_irq() ?
    flags = SysDisableIrq();

    CacheDCInvalidateRange((U32)IsrVectorTable,
                           (U32)IsrVectorTable + sizeof(IsrVectorTable));
    // Add handler to the ISR table
    IsrVectorTable[vector] = irqHandler;
    CacheDCCleanInvalidateRange((U32)IsrVectorTable,
                                (U32)IsrVectorTable + sizeof(IsrVectorTable));

    SysRestoreIrq(flags);
    return SUCCESS;
}

/*! @brief Install Exception handler vector
 *
 * @param cpuid For which CPU
 * @param vector Vector number
 * @param irqHandler Pointer to exception handler function
 * @return Standard result code
 */
RESULTCODE IsrInstallExceptionHandler(U32 cpuid, U32 vector,
                                      EXC_HANDLER irqHandler)
{
    EXC_HANDLER *base = (EXC_HANDLER*)0x00000000;
    if (cpuid == UARM)
    {
        base = (EXC_HANDLER*) 0xFFFF0000;
    }
    base += vector / sizeof(EXC_HANDLER*);
    *base = (EXC_HANDLER)0xE59FF018;       // LDR PC, [PC,#18]
    *(base+8) = irqHandler;

    return SUCCESS;
}

#endif /* SIMUPC */

/** Initialize exception handlers
 *
 * @param cpuid CPU ID on which this function called
 * @return Standard result code
 */
RESULTCODE IsrInit(U32 cpuid)
{
#ifndef USE_LINUX
    U32 tmp;

    if (cpuid == UARM)
    {
        IsrInstallExceptionHandler(UARM, EXC_RESET, ResetHandler);
        IsrInstallExceptionHandler(UARM, EXC_UNDEF, UndefHandler);
        IsrInstallExceptionHandler(UARM, EXC_SWI, SvcHandler);
        IsrInstallExceptionHandler(UARM, EXC_PABT, PrefetchHandler);
        IsrInstallExceptionHandler(UARM, EXC_DABT, AbortHandler);
        IsrInstallExceptionHandler(UARM, EXC_IRQ, IrqHandler);
        IsrInstallExceptionHandler(UARM, EXC_FIQ, FiqHandler);

        // All disabled initially
        IOWrite32A(INTC_ARM0_IRQMASK0, 0);
        IOWrite32A(INTC_ARM0_FIQMASK0, 0);
        IOWrite32A(INTC_ARM0_IRQMASK1, 0);
        IOWrite32A(INTC_ARM0_FIQMASK1, 0);

        // Clear out any pending flags
        IOWrite32A(INTC_STATUS_REG0, 0xFFFFFFFF);
        IOWrite32A(INTC_STATUS_REG1, 0xFFFFFFFF);

        // Set interrupt priorities
        // Required to enable use of IRQ Winner register
        tmp = INTC_PTP0 | (INTC_PTP1 << 8) | (INTC_PTP2 << 16)
                | (INTC_XDONE0 << 24);
        IOWrite32A(INTC_IRQ0_PRI0, tmp);
        IOWrite32A(INTC_IRQ1_PRI0, tmp);

        tmp = INTC_SPU0 | (INTC_SPU1 << 8) | (INTC_SPU0 << 16)
                | (INTC_SPU1 << 24);
        IOWrite32A(INTC_IRQ0_PRI1, tmp);
        IOWrite32A(INTC_IRQ1_PRI1, tmp);

        tmp = INTC_HIMBOX | (INTC_TIMERA << 8) | (INTC_TIMERB << 16)
                | (INTC_EMAC0 << 24);
        IOWrite32A(INTC_IRQ0_PRI2, tmp);
        IOWrite32A(INTC_IRQ1_PRI2, tmp);

        tmp = INTC_PUI | (INTC_PUDMARX << 8) | (INTC_PUDMATX << 16)
                | (INTC_EDMA0RX << 24);
        IOWrite32A(INTC_IRQ0_PRI3, tmp);
        IOWrite32A(INTC_IRQ1_PRI3, tmp);

        tmp = INTC_MDMA_DONE | (INTC_TDMA << 8) | (INTC_TDM_TIMER << 16)
                | (INTC_XDONE1 << 24);
        IOWrite32A(INTC_IRQ0_PRI4, tmp);
        IOWrite32A(INTC_IRQ1_PRI4, tmp);

        tmp = INTC_EDMA0TX | (INTC_EDMA0RX << 8) | (INTC_EDMA1TX << 16)
                | (INTC_EDMA1RX << 24);
        IOWrite32A(INTC_IRQ0_PRI5, tmp);
        IOWrite32A(INTC_IRQ1_PRI5, tmp);

        IOWrite32A(INTC_IRQ0_PRI6, tmp);
        IOWrite32A(INTC_IRQ1_PRI6, tmp);

        IOWrite32A(INTC_IRQ0_PRI7, tmp);
        IOWrite32A(INTC_IRQ1_PRI7, tmp);

        // TODO: Refine
        IOWrite32A(INTC_IRQ_SELFCLR, (1<<INTC_PTP2) | (1<<INTC_PTP0) |
                   (1<<INTC_PTP1) | (1<<INTC_XDONE0) | (1UL<<INTC_TIMERA) |
                   (1UL<<INTC_XDONE1));

        // Should be done on UARM before LARM enabled
        IsrInstallExceptionHandler(LARM, EXC_RESET, ResetHandlerL);
        IsrInstallExceptionHandler(LARM, EXC_UNDEF, UndefHandlerL);
        IsrInstallExceptionHandler(LARM, EXC_SWI, SvcHandlerL);
        IsrInstallExceptionHandler(LARM, EXC_PABT, PrefetchHandlerL);
        IsrInstallExceptionHandler(LARM, EXC_DABT, AbortHandlerL);
        IsrInstallExceptionHandler(LARM, EXC_IRQ, IrqHandlerL);
        IsrInstallExceptionHandler(LARM, EXC_FIQ, FiqHandlerL);
    }
    else
    {
        // All disabled initially
        IOWrite32A(INTC_ARM1_IRQMASK0, 0);
        IOWrite32A(INTC_ARM1_FIQMASK0, 0);
        IOWrite32A(INTC_ARM1_IRQMASK1, 0);
        IOWrite32A(INTC_ARM1_FIQMASK1, 0);
    }
#endif /* USE_LINUX */

    return SUCCESS;
}
