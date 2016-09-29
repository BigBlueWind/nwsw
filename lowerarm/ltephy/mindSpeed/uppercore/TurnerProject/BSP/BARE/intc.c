/** @file intc.c
 *
 * @brief Interrupt controller
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT(c) 2008,2009 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef SIMUPC

#include "intc.h"
#include "apb.h"
#include "sysctrl.h"
#include "portio.h"

/** Disable IRQs on interrupt controller
 *
 * @param cpuid CPU ID on which disable interrupts handling
 * @param vector Interrupt vector number
 */
void IntcDisableIrq(U32 cpuid, U32 vector)
{
    if (cpuid == UARM)
    {
        if (vector < 32)
            IOWriteAnd32A(INTC_ARM0_IRQMASK0, ~(1UL << vector));
        else
            IOWriteAnd32A(INTC_ARM0_IRQMASK1, ~(1UL << (vector-32)));
    }
    else
    {
        if (vector < 32)
            IOWriteAnd32A(INTC_ARM1_IRQMASK0, ~(1UL << vector));
        else
            IOWriteAnd32A(INTC_ARM1_IRQMASK1, ~(1UL << (vector-32)));
    }
}

/** Enable IRQs on interrupt controller
 *
 * @param cpuid CPU ID on which enable interrupts handling
 * @param vector Interrupt vector number
 */
void IntcEnableIrq(U32 cpuid, U32 vector)
{
    if (cpuid == UARM)
    {
        if (vector < 32)
            IOWriteOr32A(INTC_ARM0_IRQMASK0, 1UL << vector);
        else
            IOWriteOr32A(INTC_ARM0_IRQMASK1, 1UL << (vector-32));
    }
    else
    {
        if (vector < 32)
            IOWriteOr32A(INTC_ARM1_IRQMASK0, 1UL << vector);
        else
            IOWriteOr32A(INTC_ARM1_IRQMASK1, 1UL << (vector-32));
    }
}

/** Check for IRQs status (enabled/disabled) on interrupt controller
 *
 * @param cpuid CPU ID on which check interrupt enable status
 * @param vector Interrupt vector number
 */
BOOLEAN IntcIsIrqEnabled(U32 cpuid, U32 vector)
{
    U32 irq_on = 0;

    if (cpuid == UARM)
    {
        if (vector < 32)
            irq_on = IORead32A(INTC_ARM0_IRQMASK0) & (1UL << vector);
        else
            irq_on = IORead32A(INTC_ARM0_IRQMASK1) & (1UL << (vector-32));
    }
    else
    {
        if (vector < 32)
            irq_on = IORead32A(INTC_ARM1_IRQMASK0) & (1UL << vector);
        else
            irq_on = IORead32A(INTC_ARM1_IRQMASK1) & (1UL << (vector-32));
    }
    return (irq_on != 0);
}

/** Clear pending interrupt flag on interrupt controller
 *
 * @param vector Interrupt vector number
 */
void IntcClearInt(U32 vector)
{
    // TODO: Replace with IOWriteOr32A ?
    if (vector < 32)
        IOWrite32A(INTC_STATUS_REG0, 1UL << vector);
    else
        IOWrite32A(INTC_STATUS_REG1, 1UL << (vector-32));
}

/** Add interrupt vector to self-clear list
 *
 * @param vector Interrupt vector number
 */
void IntcSetSelfclear(U32 vector)
{
    if (vector < 32)
        IOWriteOr32A(INTC_IRQ_SELFCLR, 1UL << vector);
}

/** Set interrupt vector priority
 *
 * @param vector Interrupt vector number
 * @param priority Priority [0..31]
 * @return Standard result code
 */
RESULTCODE IntcSetPriority(U32 vector, U32 priority)
{
    U32 flags;
    U32 baseReg;
    U32 shift;

    if (vector >= INTC_MAX_VEC)
        return FAILURE;
    if (priority >= 32)
        return FAILURE;

    if (vector >= 32)
    {
        baseReg = INTC_IRQ0_PRI0;
    }
    else
    {
        baseReg = INTC_IRQ1_PRI0;
    }
    // Pri [0..3] at offset 0; [4..7] at offset 4 and so on
    baseReg += priority & 0x1C;
    // Bits [0..7] for priority b+0; [8..16] for priority b+1 and so on
    shift = (priority & 0x03) << 3;

    // TODO: replace with __disable_irq() ?
    flags = SysDisableIrq();

    // Clear old priority value
    IOWriteAnd32A(baseReg, ~(0x000000FF << shift));
    // Write new priority value
    IOWriteOr32A(baseReg, priority << shift);

    SysRestoreIrq(flags);

    return SUCCESS;
}

/** Enable ARM1 operation
 */
void IntcEnableLowArm(void)
{
    // MSB enables ARM1 operation
    IOWriteOr32A(INTC_ARM1_CTRL_REG, 0x80000000);
}

/** Disable ARM1 operation
 */
void IntcDisableLowArm(void)
{
    // MSB disables ARM1 operation
    IOWriteAnd32A(INTC_ARM1_CTRL_REG, 0x7FFFFFFF);
}

#endif /* SIMUPC */
