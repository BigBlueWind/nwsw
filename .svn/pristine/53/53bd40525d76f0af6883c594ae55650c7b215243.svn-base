/* init-gnuc.s
*/

#ifndef USE_LINUX

#define USR_MODE    0x00000010
#define FIQ_MODE    0x00000011
#define IRQ_MODE    0x00000012
#define SVC_MODE    0x00000013
#define ABT_MODE    0x00000017
#define UND_MODE    0x0000001b
#define PSR_F_BIT   0x00000040
#define PSR_I_BIT   0x00000080

    .section ".cs3.reset"
    .type  _ResetHandler, #function
    .align 5;
    .globl ResetHandler
ResetHandler:

    /* Enter Data Abort mode, set up stack for it */
    mov     r0, #0x00000017 | 0x00000080 | 0x00000040
    msr     cpsr_cxsf, r0
    ldr     r13, =AbortStack
    add     r13, r13, #1024

    /* Enter Undefined Instruction mode, set up stack for it */
    mov     r0, #0x0000001b | 0x00000080 | 0x00000040
    msr     cpsr_cxsf, r0
    ldr     r13, =UndefStack
    add     r13, r13, #1024

    /* Enter FIQ mode, set up stack for it */
    mov     r0, #0x00000011 | 0x00000080 | 0x00000040
    msr     cpsr_cxsf, r0
    ldr     r13, =FiqStack
    add     r13, r13, #1024

    /* Enter IRQ mode, set up stack for it */
    mov     r0, #0x00000012 | 0x00000080 | 0x00000040
    msr     cpsr_cxsf, r0
    ldr     r13, =IrqStack
    add     r13, r13, #1024

    /* Enter Supervisor mode, set up stack for it */
    mov     r0, #0x00000013 | 0x00000080 | 0x00000040
    msr     cpsr_cxsf, r0

    /* Code from CS3 startup sequence */
    ldr     r2, =__cs3_heap_start
    ldr     sp, =__cs3_stack
    ldr     r0, [r2, #8]
    cmp     r0, #0
    movne   sp, r0
    ldr     r1, =__cs3_heap_limit
    ldr     r0, [r2, #4]
    cmp     r0, #0
    strne   r0, [r1]

    /* We're still in SVC mode, go to RTL Init */
    ldr     r12, =__cs3_start_c         @ Save this in register for possible long jump
    bx      r12                         @ C Library startup code

InitFailure:
    b       InitFailure                 @ Should not reach this point but in case we did

    .section ".cs3.reset"
    .type  _ResetHandlerL, #function
    .align 5;
    .globl ResetHandlerL
ResetHandlerL:

    /* TODO:  Do we need this? */
    mov     r0,#0
    mcr     p15, 0, r0, c1, c0, 0

    /* Enter Data Abort mode, set up stack for it */
    mov     r0, #0x00000017 | 0x00000080 | 0x00000040
    msr     cpsr_cxsf, r0
    ldr     r13, =AbortStackL
    add     r13, r13, #1024

    /* Enter Undefined Instruction mode, set up stack for it */
    mov     r0, #0x0000001b | 0x00000080 | 0x00000040
    msr     cpsr_cxsf, r0
    ldr     r13, =UndefStackL
    add     r13, r13, #1024

    /* Enter FIQ mode, set up stack for it */
    mov     r0, #0x00000011 | 0x00000080 | 0x00000040
    msr     cpsr_cxsf, r0
    ldr     r13, =FiqStackL
    add     r13, r13, #1024

    /* Enter IRQ mode, set up stack for it */
    mov     r0, #0x00000012 | 0x00000080 | 0x00000040
    msr     cpsr_cxsf, r0
    ldr     r13, =IrqStackL
    add     r13, r13, #1024

    /* Enter Supervisor mode, set up stack for it */
    mov     r0, #0x00000013 | 0x00000080 | 0x00000040
    msr     cpsr_cxsf, r0
    ldr     r13, =SvcStackL
    add     r13, r13, #1024

    /* LARM entry point: this function should never return */
    ldr     r12, =mainl                 @ Save this in register for possible long jump
    bx      r12

InitFailureL:
    b       InitFailureL                @ Should not reach this point but in case we did

#endif /* USE_LINUX */
