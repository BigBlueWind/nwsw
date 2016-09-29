/** @file isr.h
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

#ifndef ISR_H_
#define ISR_H_

#include "basetypes.h"
#include "resultcodes.h"

//
// Exception handler offsets
//
#define EXC_RESET               0x00
#define EXC_UNDEF               0x04
#define EXC_SWI                 0x08
#define EXC_PABT                0x0C
#define EXC_DABT                0x10
#define EXC_RESERVED            0x14
#define EXC_IRQ                 0x18
#define EXC_FIQ                 0x1C

//
// IRQ Priorities
//
#define EMPTY_VECTOR            0x16
#define EMPTY_PRIORITY          0x16161616

#ifdef __GNUC__
#ifndef __CC_ARM
#define __irq                   __attribute__((__interrupt__))
#endif /* __CC_ARM */
#endif /* __GNUC__ */

// Exception handler callback
#ifdef __GNUC__
#ifdef __CC_ARM
typedef __irq void (*EXC_HANDLER)(void);
#else /* __CC_ARM */
typedef void (*EXC_HANDLER)(void);
#endif /* __CC_ARM */
#else /* __GNUC__ */
#define __irq
typedef __irq void (*EXC_HANDLER)(void);
#endif /* __GNUC__ */

// IRQ handler callback
typedef void (*IRQ_HANDLER)(void);

//
// Exception handler prototypes
//
__irq void ResetHandler(void);
__irq void UndefHandler(void);
__irq void SvcHandler(void);
__irq void PrefetchHandler(void);
__irq void AbortHandler(void);
__irq void IrqHandler(void);
__irq void FiqHandler(void);

__irq void ResetHandlerL(void);
__irq void UndefHandlerL(void);
__irq void SvcHandlerL(void);
__irq void PrefetchHandlerL(void);
__irq void AbortHandlerL(void);
__irq void IrqHandlerL(void);
__irq void FiqHandlerL(void);

RESULTCODE IsrInit(U32 cpuid);

RESULTCODE IsrInstallExceptionHandler(U32 cpuid, U32 vector,
                                      EXC_HANDLER irqHandler);
RESULTCODE IsrAddIrqHandler(U32 vector, IRQ_HANDLER irqHandler);

#endif /* ISR_H_ */
