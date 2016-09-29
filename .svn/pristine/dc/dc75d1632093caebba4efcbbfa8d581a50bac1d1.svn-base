; irqhdlr.s - IRQ Handler stubs for RTXC Quadros
;/*
; *   RTXC Quadros
; *   Copyright (c) 1999-2008
; *   Quadros Systems, Inc.
; *   ALL RIGHTS RESERVED
; *
; *   THE INFORMATION HEREIN IS CONFIDENTIAL AND PROPRIETARY.
; *   UNAUTHORIZED DISCLOSURE OR DISTRIBUTION IS PROHIBITED.
;*/

;*
;* modification history
;*
;* 09-feb-2006  wld   added marker + CPAR to frame for XSCALE MMX support
;* 06-mar-2006  wld   forced stack alignment x8 for IRQ interrupts
;*

;
; Note: This module contains the Assembly macros for the IRQ Handler
;       Prolog and Epilog.
;
    PRESERVE8

    CODE32      ; force ARM - not THUMB mode

EABI_STACK_ALIGNMENT EQU 8 ; only 8 or 16; 8=Default and Recommended

; PSR definitions
SVC32_Mode EQU &13 ; SWI_BASED

    IMPORT   |pKWS|
    IMPORT   |IS_Dispatch|

;
; member offsets must match KWS in kws.h
;
                        ^ 0 ; start of KWS struct

KWS_csf_OFFSET          # 4
KWS_prtxcstk_OFFSET     # 4
KWS_lnest_OFFSET        # 4

;
; member offsets must match FRAME in rtxstruc.h
;
                  ^ 0 ; start of FRAME struct
PAD_OFFSET        # (EABI_STACK_ALIGNMENT - 4)
CSF_OFFSET        # 4
CPSR_OFFSET       # 4
R0_OFFSET         # 4
R1_OFFSET         # 4
R2_OFFSET         # 4
R3_OFFSET         # 4
R4_OFFSET         # 4
R5_OFFSET         # 4
R6_OFFSET         # 4
R7_OFFSET         # 4
R8_OFFSET         # 4
R9_OFFSET         # 4
R10_OFFSET        # 4
R11_OFFSET        # 4
R12_OFFSET        # 4
R14_OFFSET        # 4
LR_OFFSET  EQU    R14_OFFSET
R15_OFFSET        # 4
PC_OFFSET  EQU    R15_OFFSET

SIZEOF_FRAME      # 0 ; defined for documentation purposes only

RTXC_Mode EQU SVC32_Mode

I_Bit  EQU  &80     ; (1 :SHL: 7) processor IRQ enable/disable bit

;
; IRQ Stack Scratch Area
;
; NOTE: only 12 bytes of IRQ stack area are ever used by RTXC.
;       Interrupts are switched to run on RTXC Kernel Stack (kws.prtxcstk).
;
                 ^ -(3*4)
IRQ_PC_OFFSET    # 4 ; return PC
IRQ_CPSR_OFFSET  # 4 ; CPSR at time of interrupt
IRQ_R0_OFFSET    # 4 ; R0 at time of interrupt


    MACRO
    RTXCISRms $AsmCode,$CCode

    IMPORT  $CCode ; generic IRQ handler

   EXPORT $AsmCode
   AREA |.text|, CODE, READONLY     ; intentional non-CodeArea

$AsmCode

    SUB lr, lr, #4                  ; adjust pc for pipeline effect

    STR lr, [sp, #IRQ_PC_OFFSET]    ; save interrupted "PC" on IRQ stack
    STR r0, [sp, #IRQ_R0_OFFSET]    ; save R0 on IRQ stack

    MRS r0, SPSR
    STR r0, [sp, #IRQ_CPSR_OFFSET]  ; save interrupted "CPSR" on IRQ stack

    MOV r0, sp                      ; preserve IRQ sp across mode switch

    ; assumes FIQ bit is always set coming in
    MSR CPSR_c, #RTXC_Mode+I_Bit    ; force to SVC mode + IRQ disabled

    SUB sp, sp, #4                  ; leave hole for r15 (PC) at bottom of frame

    STMFD sp!, {r1-r12, lr}         ; store pre-interrupt LR + r1-r12 registers
                                    ; on SVC stack

    LDR r1, [r0, #IRQ_CPSR_OFFSET]  ; copy CPSR from IRQ to SVC stack
    LDR r2, [r0, #IRQ_R0_OFFSET]    ; copy r0 from IRQ to SVC stack

    STMFD sp!, {r1-r2}              ; save r0 and CPSR to SVC stack

    LDR  lr, [r0, #IRQ_PC_OFFSET]   ; copy pc from IRQ to SVC stack

    LDR  v1, =|pKWS|                ; get &pKWS
    LDR  v1, [v1, #0]               ; v1 = pKWS
    LDR  v2, [v1, #KWS_csf_OFFSET]  ; v2 = pkws->csf

    STMFD sp!, {v2}                 ; pframe->csf = pkws->csf

    SUB  sp, sp, #(EABI_STACK_ALIGNMENT-4) ; pad for alignment

    STR  lr, [sp, #PC_OFFSET]       ; put return address on stack

    ADD  v2, v2, #1
    STR  v2, [v1, #KWS_csf_OFFSET]  ; pkws->csf++

    MOV v3, sp                      ; save sp across irqc

    TEQ v2, #1                      ; if not already nested (csf+1 in v2)
    LDREQ sp, [v1, #KWS_prtxcstk_OFFSET] ; then switch to kernel stack

    ; force stack alignment to meet EABI 2.0 - assumes kernel stack is already aligned
    BICNE sp, sp, #4                ; align if necessary
                                    ;  eg 0x14->0x10, 0x20->0x20

    LDR r12, =$CCode                ; insert manual veneer
    MOV lr, pc
    BX  r12                         ; void IrqHandler(FRAME * | void)
    MOV a1, v3                      ; restore sp

    LDR r12, =|IS_Dispatch|         ; insert manual veneer
    MOV lr, pc
    BX  r12                         ; FRAME *IS_Dispatch(FRAME *)

    ; switch to next tasks' stack via a1 (aka r0)

    ADD   a1, a1, #(EABI_STACK_ALIGNMENT-4) ; pop pad(s) from stack

    ADD   sp, a1, #4                ; skip over csf on stack

;;
;; note: &kws (=pkws) and csf are preserved across call in v1 and v2
;;

    SUB  v2, v2, #1
    STR  v2, [v1, #KWS_csf_OFFSET]  ; pkws->csf = csf--

    LDMFD sp!, {r0}                 ; restore CPSR from SVC stack
    MSR spsr_cf, r0                 ; prepare spsr for restoration of cpsr

    LDMFD sp!, {r0-r12, lr, pc}^    ; restore all regs from SVC stack

    MEND                            ; End of macro

;;
;; Actual macro expansion
;;
    RTXCISRms IrqHandler,IrqHandlerCb
    RTXCISRms IrqHandlerL,IrqHandlerLCb

    END
