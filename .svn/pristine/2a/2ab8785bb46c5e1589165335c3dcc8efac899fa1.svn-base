/* vectorsl.S
 *
 */

#ifndef USE_LINUX

    .section ".lvectors.table"

    /* Vector 0x00 is for RESET handler */
    ldr     pc, ResetAddrL

    /* Vector 0x04 is for UNDEF handler */
    ldr     pc, UndefinedAddrL

    /* Vector 0x08 is for SVC handler */
    ldr     pc, SvcAddrL

    /* Vector 0x0C is for PABT handler */
    ldr     pc, PrefetchAddrL

    /* Vector 0x10 is for DABT handler */
    ldr     pc, AbortAddrL

    /* Vector 0x14 is reserved */
    nop

    /* Vector 0x18 is for IRQ handler */
    ldr     pc, IrqAddrL

    /* Vector 0x1C is for FIQ handler */
    ldr     pc, FiqAddrL

ResetAddrL:
.long ResetHandlerL

UndefinedAddrL:
.long UndefHandlerL

SvcAddrL:
.long SvcHandlerL

PrefetchAddrL:
.long PrefetchHandlerL

AbortAddrL:
.long AbortHandlerL

IrqAddrL:
.long IrqHandlerL

FiqAddrL:
.long FiqHandlerL

#endif /* USE_LINUX */
