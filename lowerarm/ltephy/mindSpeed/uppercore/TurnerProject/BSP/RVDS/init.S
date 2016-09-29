; init.s
;
; Copyright 2008,2009 by Mindspeed Technologies
; All rights reserved
;
; Application entry point (Upper ARM, Lower ARM)
;

 [ :LNOT: :DEF: USE_LINUX

    PRESERVE8

; Standard definitions of mode bits and interrupt (I & F) flags in PSRs
Mode_USR        EQU     0x10
Mode_FIQ        EQU     0x11
Mode_IRQ        EQU     0x12
Mode_SVC        EQU     0x13
Mode_ABT        EQU     0x17
Mode_UND        EQU     0x1B
Mode_SYS        EQU     0x1F ; available on ARM Arch 4 and later

I_Bit           EQU     0x80 ; when I bit is set, IRQ is disabled
F_Bit           EQU     0x40 ; when F bit is set, FIQ is disabled

    AREA    INIT, CODE, READONLY

;
; Entry point for Upper ARM
;
    EXPORT  ResetHandler
ResetHandler    FUNCTION

    ENTRY

    ; Enter Data Abort mode, set up stack for it
    IMPORT  AbortStack
	MOV		r0, #Mode_ABT:OR:I_Bit:OR:F_Bit
	MSR		CPSR_cxsf, r0
	LDR		r13, =AbortStack
	ADD		r13, r13, #1024

    ; Enter Undefined Instruction mode, set up stack for it
    IMPORT  UndefStack
	MOV		r0, #Mode_UND:OR:I_Bit:OR:F_Bit
	MSR		CPSR_cxsf, r0
	LDR		r13, =UndefStack
	ADD		r13, r13, #1024

    ; Enter FIQ mode, set up stack for it
    IMPORT  FiqStack
	MOV		r0, #Mode_FIQ:OR:I_Bit:OR:F_Bit
	MSR		CPSR_cxsf, r0
	LDR		r13, =FiqStack
	ADD		r13, r13, #1024

    ; Enter IRQ mode, set up stack for it
    IMPORT  IrqStack
	MOV		r0, #Mode_IRQ:OR:I_Bit:OR:F_Bit
	MSR		CPSR_cxsf, r0
	LDR		r13, =IrqStack
	ADD		r13, r13, #1024

    ; Enter Supervisor mode, set up stack for it
    IMPORT  SvcStack
	MOV		r0, #Mode_SVC:OR:I_Bit:OR:F_Bit
	MSR		CPSR_cxsf, r0
	LDR		r13, =SvcStack
	ADD		r13, r13, #1024

	; We're still in SVC mode, go to RTL Init
    IMPORT  __main
    LDR     r12,=__main                 ; Save this in register for possible long jump
    BX      r12                         ; C Library startup code

0   B       0                           ; In case we accidentally reached this point

    ENDFUNC

;
; Entry point for Lower ARM
;
    EXPORT  ResetHandlerL
ResetHandlerL   FUNCTION

; TODO:  Do we need this?
	MOV		R0,#0
	MCR		p15, 0, r0, c1, c0, 0

    ; Enter Data Abort mode, set up stack for it
    IMPORT  AbortStackL
	MOV		r0, #Mode_ABT:OR:I_Bit:OR:F_Bit
	MSR		CPSR_cxsf, r0
	LDR		r13, =AbortStackL
	ADD		r13, r13, #1024

    ; Enter Undefined Instruction mode, set up stack for it
    IMPORT  UndefStackL
	MOV		r0, #Mode_UND:OR:I_Bit:OR:F_Bit
	MSR		CPSR_cxsf, r0
	LDR		r13, =UndefStackL
	ADD		r13, r13, #1024

    ; Enter FIQ mode, set up stack for it
    IMPORT  FiqStackL
	MOV		r0, #Mode_FIQ:OR:I_Bit:OR:F_Bit
	MSR		CPSR_cxsf, r0
	LDR		r13, =FiqStackL
	ADD		r13, r13, #1024

    ; Enter IRQ mode, set up stack for it
    IMPORT  IrqStackL
	MOV		r0, #Mode_IRQ:OR:I_Bit:OR:F_Bit
	MSR		CPSR_cxsf, r0
	LDR		r13, =IrqStackL
	ADD		r13, r13, #1024

    ; Enter Supervisor mode, set up stack for it
    IMPORT  SvcStackL
	MOV     r0, #Mode_SVC:OR:I_Bit:OR:F_Bit
	MSR     CPSR_cxsf, r0
	LDR     r13, =SvcStackL
	ADD     r13, r13, #1024

    ; LARM entry point: this function should never return
    IMPORT  mainl
    LDR     r12, =mainl                 ; Save this in register for possible long jump
	BX      r12

0   B       0                           ; In case we accidentally reached this point

    ENDFUNC

 ]                                      ; !USE_LINUX

    END

