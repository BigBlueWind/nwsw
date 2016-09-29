	
	AREA CodeVectors, CODE, READONLY


	EXPORT ResetHandler
	EXPORT UndefHandlerX
	EXPORT PrefHandlerX
	EXPORT DataHandlerX

        IMPORT __scatterload
        IMPORT __rt_entry

	IMPORT Core0_StackSetup
        IMPORT CoreX_StackSetup
        IMPORT main

	IMPORT UndefHandler
	IMPORT PrefHandler
	IMPORT DataHandler
	IMPORT DataHandlerCheckMemAccess
	IMPORT ProcWDService
	IMPORT DevBaseInit

	IMPORT gAbortCtx0
	IMPORT gAbortCtx1
	IMPORT gAbortCtx2
	IMPORT gAbortCtx3
	IMPORT stacks_cpu0
	IMPORT stacks_cpu1
	IMPORT stacks_cpu2
	IMPORT stacks_cpu3
	IMPORT CoreCfg


SPRV_STACK_SIZE		EQU 	(64*1024)
ABORT_STACK_SIZE	EQU 	1024
ABORT_STACK_SHIFT	EQU 	(SPRV_STACK_SIZE + ABORT_STACK_SIZE)
UNDEF_STACK_SIZE	EQU 	1024
UNDEF_STACK_SHIFT	EQU 	(SPRV_STACK_SIZE + ABORT_STACK_SIZE + UNDEF_STACK_SIZE)

        PRESERVE8

ResetHandler
	
	; to disable IRQs and FIQS
	MRS 	r1, CPSR
	ORR 	r1, r1, #(0x80 | 0x40)
	MSR 	CPSR_c, r1

	MRC p15, 0, r1, c0, c0, 5   ; sys_read_coreid & 1
	AND     r1, r1, #0xF

	CMP r1, #0
	LDREQ   SP, =stacks_cpu0 + 3072

	CMP r1, #1
	LDREQ   SP, =stacks_cpu1 + 3072
	 
	CMP r1, #2
	LDREQ   SP, =stacks_cpu2 + 3072

	CMP r1, #3
	LDREQ   SP, =stacks_cpu3 + 3072

	BL		DevBaseInit
	BL		ProcWDService
	
	; set flag of running
	;LDR r2, =CoreCfg
	;LDR r1, =0x80000000
	;STR r1, [r2,#0x10]

	; to read CPU core ID, have it in r0
	MRC p15, 0, r0, c0, c0, 5
	AND r1, r0, #0xF00
	CMP r1, #0

	; jump to the ARMx4 
	; if this is ARMx4 cluster

	BEQ DoCase4x

DoCase2x
      	AND r1, r0, #0xF
	CMP r1, #0

	; if we started on core#1, we need to skip any additional initialization
	; and just jump to the code start-up
	BNE ARM_CORE_ENTRY

	; ------------------------------
        ; set reset for ARM-core#1
	; ------------------------------
        LDR r2, =0xFE440014
        LDR r3, [r2,#0]
        ORR r3, r3, #0x20
 	STR r3, [r2]	;to set reset signal for ARM-CORE#1
	NOP
	STR r3, [r2]	;to prevent any HW problems and to be sure that reset signal is set
        NOP

	; we are ready to jump to the entry point
	; after invalidating L2 cache 
	B DoGenCore0Staff

DoCase4x
      	AND r1, r0, #0xF
	CMP r1, #0
	BNE ARM_CORE_ENTRY

        ; core #0 set reset for cores #1, #2, #3
        LDR r2, =0xFE440034
        LDR r3, [r2,#0]
        ORR r3, r3, #0xE0
 	STR r3, [r2]
        NOP

DoGenCore0Staff
        ; disable L2cache, MMU, ICache, DCache
	isb
	;dsb
	DMB
        LDR r2, =0xFFF10730
        MOV r3, #0
        STR r3, [r2]
        LDR r2, =0xFFF10100
        STR r3, [r2]

	mrc	p15, 0, r0, c1, c0, 0
	BIC	r0, r0, #0x1800
	BIC	r0, r0, #0x5
	mcr	p15, 0, r0, c1, c0, 0
	dsb	
	isb

        ; MxiInvalidIDCache
        ISB
	DMB
        mov     r0,  #0
	mcr     p15, 0, r0, c8, c7, 0           ; Invalidate entire Unified TLB
	mcr     p15, 0, r0, c7, c5, 0           ; Invalidate ICache
	ISB
	DSB
    
	MOV     r0, #0 << 5             ; start with first line

nextset_invalidate1rst
	MOV     r1, #0 << 30            ; start with way 0 

nextway_invalidate1rst
	ORR     r2, r0, r1              ; contruct Way/Index value
	MCR     p15, 0, r2, c7, c6, 2   ; Invalidate by Way/Index (section)
	ADD     r1, r1, #1 << 30        ; Increment Way
	CMP     r1, #0
	BNE     nextway_invalidate1rst
	ADD     r0, r0, #1 << 5         ; Increment Line
	CMP     r0, #1 << 13            ; Look for overflow out of Set field
	BNE     nextset_invalidate1rst

	DMB
	DSB
	ISB

DoGenContinueStaff
ARM_CORE_ENTRY

	; -------------------------------
	; to clean-up all I-CPU signals 
	; set on Linux side
 	; -------------------------------
	LDR 	r1, =0xFE430040
	MOV	r0, #0xFF
	STR	r0, [r1]
	NOP	
	STR	r0, [r1]
	NOP

	; to disable the timer 
	LDR 	r1, =0xFFF00600
	MOV	r0, #0
	STR	r0, [r1, #8]
		
	; to clear the timer status 
	LDR 	r1, =0xFFF00600
	MOV	r0, #1
	STR	r0, [r1, #0xC]

	; to read CPU core ID
	MRC p15, 0, r0, c0, c0, 5
	AND r0, r0, #0xF

	cmp r0, #0
	BNE ARM_CoreX

	; An entry point of ARM-core0

	B __scatterload
	B __rt_entry

	B STOP

ARM_CoreX
	; An entry point of ARM-core1, 2, 3

	BL CoreX_StackSetup
	BL main


STOP	B STOP


UndefHandlerX

	STMFD   sp!, {r0-r1}		; store them on stack for now
	MRC	p15, 0, r1, c0, c0, 5   ; sys_read_coreid & 1
	AND     r1, r1, #0xF

	CMP	r1, #0
	LDREQ   r0, =gAbortCtx0

	CMP	r1, #1
	LDREQ   r0, =gAbortCtx1

	CMP	r1, #2
	LDREQ   r0, =gAbortCtx2

	CMP	r1, #3
	LDREQ   r0, =gAbortCtx3
	
	STMIA 	r0, {r0-r12}		; Store 1-st dataset r0-r12 to gAbortCtx - r0, r1 are wrong for now

	; correct r0 and r1
	LDR     r1, [sp]		; Contents of r0 on stack
	STR	r1, [r0]		; Store saved r0 to gAbortCtx
	LDR     r1, [sp, #4]		; Contents of r1 on stack
	STR	r1, [r0, #4]		; Store saved r1 to gAbortCtx

	; place cpuID to r5
	MRC	p15, 0, r5, c0, c0, 5   ; sys_read_coreid & 1:			r5 = cpuid
	AND     r5, r5, #0xF

	
	SUB	r2, lr, #4              ; PC value of dabort instruction: 	r2 = PC (lr - 4)

	MRS	r7, CPSR                ; save current CPSR to r6 for mode switching
	MRS	r8, SPSR                ; spsr_abt = CPSR of dabt originating mode: save to r8 for mode switching

	ORR	r9, r7, #0x40           ; no FIQs
	MSR	CPSR_cxsf, r9

	MOV	r4, r8                  ; building second dataset: 		r4 = SPSR
	MOV	r6, r7                  ; building second dataset: 		r6 = CPSR

	TST	r8, #0xF                ; test for the mode which raised exception
	ORREQ	r8, r8, #0xF            ; if NE => change mode usr->sys; else do nothing
	BIC	r9, r8, #0x20           ; CPSR.T, go to forced ARM state via r8
	ORR	r9, r9, #0x40           ; no FIQs
	MSR	CPSR_cxsf, r9           ; switch out from mode 0x17 (ABT) to dabt generating mode and state

        ; need to have sys mode here to be able to switch back to mode 0x17
        ; sys mode and user mode share sp and lr, so we get them correct
	MOV	r0, sp                  ; building 2-nd dataset: 		r0 = SP
	MOV	r1, lr			; building 2-nd dataset: 		r1 = LR

	ORR	r9, r7, #0x40           ; no FIQs
	MSR	CPSR_cxsf, r9           ; switch back to mode 0x17 (ABT)

	MOV	r3, #0			; instr isn't used for pabt: 		r3 = instr opcode

	CMP	r5, #0
	LDREQ	sp, =(gAbortCtx0 + 13*4) ; move sp for already saved space

	CMP	r5, #1
	LDREQ	sp, =(gAbortCtx1 + 13*4) ; move sp for already saved space

	CMP	r5, #2
	LDREQ	sp, =(gAbortCtx2 + 13*4) ; move sp for already saved space

	CMP	r5, #3
	LDREQ	sp, =(gAbortCtx3 + 13*4) ; move sp for already saved space
	
	STMIA	sp, {r0-r6}             ; and save the 2-nd dataset from r0 to r6

	; clean up & restoration follows: restored full context, sp first
	CMP	r5, #0
	LDREQ	sp, =(stacks_cpu0 + UNDEF_STACK_SHIFT)           ; sp is on the top of ABT stack

	CMP	r5, #1
	LDREQ	sp, =(stacks_cpu1 + UNDEF_STACK_SHIFT)           ; sp is on the top of ABT stack

	CMP	r5, #2
	LDREQ	sp, =(stacks_cpu2 + UNDEF_STACK_SHIFT)           ; sp is on the top of ABT stack

	CMP	r5, #3
	LDREQ	sp, =(stacks_cpu3 + UNDEF_STACK_SHIFT)           ; sp is on the top of ABT stack

	LDR	r6, =UndefHandler
	MOV	lr, pc
	BX	r6                      ; call UndefHandler (ARM or Thumb)

LOOPUH
	B	LOOPUH			; loop forever


PrefHandlerX

	STMFD   sp!, {r0-r1}		; store them on stack for now
	MRC	p15, 0, r1, c0, c0, 5   ; sys_read_coreid & 1
	AND     r1, r1, #0xF

	CMP	r1, #0
	LDREQ   r0, =gAbortCtx0

	CMP	r1, #1
	LDREQ   r0, =gAbortCtx1

	CMP	r1, #2
	LDREQ   r0, =gAbortCtx2

	CMP	r1, #3
	LDREQ   r0, =gAbortCtx3
	
	STMIA 	r0, {r0-r12}		; Store 1-st dataset r0-r12 to gAbortCtx - r0, r1 are wrong for now

	; correct r0 and r1
	LDR     r1, [sp]		; Contents of r0 on stack
	STR	r1, [r0]		; Store saved r0 to gAbortCtx
	LDR     r1, [sp, #4]		; Contents of r1 on stack
	STR	r1, [r0, #4]		; Store saved r1 to gAbortCtx

	; place cpuID to r5
	MRC	p15, 0, r5, c0, c0, 5   ; sys_read_coreid & 1:			r5 = cpuid
	AND     r5, r5, #0xF

	
	SUB	r2, lr, #4              ; PC value of dabort instruction: 	r2 = PC (lr - 4)

	MRS	r7, CPSR                ; save current CPSR to r6 for mode switching
	MRS	r8, SPSR                ; spsr_abt = CPSR of dabt originating mode: save to r8 for mode switching

	ORR	r9, r7, #0x40           ; no FIQs
	MSR	CPSR_cxsf, r9

	MOV	r4, r8                  ; building second dataset: 		r4 = SPSR
	MOV	r6, r7                  ; building second dataset: 		r6 = CPSR

	TST	r8, #0xF                ; test for the mode which raised exception
	ORREQ	r8, r8, #0xF            ; if EQ => change mode usr->sys; else do nothing
	BIC	r9, r8, #0x20           ; CPSR.T, go to forced ARM state via r8
	ORR	r9, r9, #0x40           ; no FIQs
	MSR	CPSR_cxsf, r9           ; switch out from mode 0x17 (ABT) to dabt generating mode and state

        ; need to have sys mode here to be able to switch back to mode 0x17
        ; sys mode and user mode share sp and lr, so we get them correct
	MOV	r0, sp                  ; building 2-nd dataset: 		r0 = SP
	MOV	r1, lr			; building 2-nd dataset: 		r1 = LR

	ORR	r9, r7, #0x40           ; no FIQs
	MSR	CPSR_cxsf, r9           ; switch back to mode 0x17 (ABT)

	MOV	r3, #0			; instr isn't used for pabt: 		r3 = instr opcode

	CMP	r5, #0
	LDREQ	sp, =(gAbortCtx0 + 13*4) ; move sp for already saved space

	CMP	r5, #1
	LDREQ	sp, =(gAbortCtx1 + 13*4) ; move sp for already saved space

	CMP	r5, #2
	LDREQ	sp, =(gAbortCtx2 + 13*4) ; move sp for already saved space

	CMP	r5, #3
	LDREQ	sp, =(gAbortCtx3 + 13*4) ; move sp for already saved space
	
	STMIA	sp, {r0-r6}             ; and save the 2-nd dataset from r0 to r6

	; clean up & restoration follows: restored full context, sp first
	CMP	r5, #0
	LDREQ	sp, =(stacks_cpu0 + ABORT_STACK_SHIFT)           ; sp is on the top of ABT stack

	CMP	r5, #1
	LDREQ	sp, =(stacks_cpu1 + ABORT_STACK_SHIFT)           ; sp is on the top of ABT stack

	CMP	r5, #2
	LDREQ	sp, =(stacks_cpu2 + ABORT_STACK_SHIFT)           ; sp is on the top of ABT stack

	CMP	r5, #3
	LDREQ	sp, =(stacks_cpu3 + ABORT_STACK_SHIFT)           ; sp is on the top of ABT stack

	LDR	r6, =PrefHandler
	MOV	lr, pc
	BX	r6                      ; call PrefHandler (ARM or Thumb)

LOOPPH
	B	LOOPPH			; loop forever


DataHandlerX

	PUSH     {r0-r12,lr}

	BL DataHandlerCheckMemAccess

	CMP r0, #1
	BNE JUST_DATA_ABORT

	POP      {r0-r12,lr}
	SUBS     pc,lr,#4

JUST_DATA_ABORT

	POP     {r0-r12,lr}
	STMFD   sp!, {r0-r1}		; store them on stack for now
	MRC	p15, 0, r1, c0, c0, 5   ; sys_read_coreid & 1
	AND     r1, r1, #0xF

	CMP	r1, #0
	LDREQ   r0, =gAbortCtx0

	CMP	r1, #1
	LDREQ   r0, =gAbortCtx1

	CMP	r1, #2
	LDREQ   r0, =gAbortCtx2

	CMP	r1, #3
	LDREQ   r0, =gAbortCtx3
	
	STMIA 	r0, {r0-r12}		; Store 1-st dataset r0-r12 to gAbortCtx - r0, r1 are wrong for now

	; correct r0 and r1
	LDR     r1, [sp]		; Contents of r0 on stack
	STR		r1, [r0]		; Store saved r0 to gAbortCtx
	LDR     r1, [sp, #4]	; Contents of r1 on stack
	STR		r1, [r0, #4]	; Store saved r1 to gAbortCtx

	; place cpuID to r5
	MRC	p15, 0, r5, c0, c0, 5   ; sys_read_coreid & 1:			r5 = cpuid
	AND     r5, r5, #0xF


	SUB	r2, lr, #8              ; PC value of dabort instruction: 	r2 = PC (lr - 8)

	MRS	r7, CPSR                ; save current mode to r6 for mode switching
	MRS	r8, SPSR                ; spsr_abt = CPSR of dabt originating mode: save to r8 for mode switching

	ORR	r9, r7, #0x40           ; no FIQs
	MSR	CPSR_cxsf, r9

	MOV	r4, r8                  ; building second dataset: 		r4 = SPSR
	MOV	r6, r7                  ; building second dataset: 		r6 = CPSR

	TST	r8, #0xF                ; test for the mode which raised exception
	ORREQ	r8, r8, #0xF            ; if EQ => change mode usr->sys; else do nothing
	BIC	r9, r8, #0x20           ; CPSR.T, go to forced ARM state via r8
	ORR	r9, r9, #0x40           ; no FIQs
	MSR	CPSR_cxsf, r9           ; switch out from mode 0x17 (ABT) to dabt generating mode and state

        ; need to have sys mode here to be able to switch back to mode 0x17
        ; sys mode and user mode share sp and lr, so we get them correct
	MOV	r0, sp                  ; building 2-nd dataset: 		r0 = SP
	MOV	r1, lr			; building 2-nd dataset: 		r1 = LR

	ORR	r9, r7, #0x40           ; no FIQs
	MSR	CPSR_cxsf, r9           ; switch back to mode 0x17 (ABT)

	TST	r8, #0x20               ; CPSR.T
	LDRNEH	r3, [r2]                ; THUBM state was, load 16-bit instr.: 	r3 = [pc](dabt)
	LDREQ	r3, [r2]                ; ARM state was, load 32-bit instr.: 	r3 = [pc](dabt)

	CMP	r5, #0
	LDREQ	sp, =(gAbortCtx0 + 13*4) ; move sp for already saved space

	CMP	r5, #1
	LDREQ	sp, =(gAbortCtx1 + 13*4) ; move sp for already saved space

	CMP	r5, #2
	LDREQ	sp, =(gAbortCtx2 + 13*4) ; move sp for already saved space

	CMP	r5, #3
	LDREQ	sp, =(gAbortCtx3 + 13*4) ; move sp for already saved space
	
	STMIA	sp, {r0-r6}             ; and save the 2-nd dataset from r0 to r6

	; clean up & restoration follows: restored full context, sp first
	CMP	r5, #0
	LDREQ	sp, =(stacks_cpu0 + ABORT_STACK_SHIFT)           ; sp is on the top of ABT stack

	CMP	r5, #1
	LDREQ	sp, =(stacks_cpu1 + ABORT_STACK_SHIFT)           ; sp is on the top of ABT stack

	CMP	r5, #2
	LDREQ	sp, =(stacks_cpu2 + ABORT_STACK_SHIFT)           ; sp is on the top of ABT stack

	CMP	r5, #3
	LDREQ	sp, =(stacks_cpu3 + ABORT_STACK_SHIFT)           ; sp is on the top of ABT stack

	BL DataHandler

LOOPDH
	B	LOOPDH			; loop forever

	

	END
