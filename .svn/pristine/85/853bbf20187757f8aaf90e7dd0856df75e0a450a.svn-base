; vectors.s
;
; Copyright 2008,2009 by Mindspeed Technologies
; All rights reserved
;
; Interrupt vector table for Upper ARM

 [ :LNOT: :DEF: USE_LINUX

    PRESERVE8

    AREA VectUp, CODE, READONLY

    ENTRY

; A vector table and dummy exception handlers

    ; Vector 0x00 is for RESET handler
    LDR     PC, ResetAddr

    ; Vector 0x04 is for UNDEF handler
    LDR     PC, UndefinedAddr

    ; Vector 0x08 is for SVC handler
    LDR     PC, SvcAddr

    ; Vector 0x0C is for PABT handler
    LDR     PC, PrefetchAddr

    ; Vector 0x10 is for DABT handler
    LDR     PC, AbortAddr

    ; Vector 0x14 is reserved
    NOP

    ; Vector 0x18 is for IRQ handler
    LDR     PC, IrqAddr

    ; Vector 0x1C is for FIQ handler
    LDR     PC, FiqAddr

    IMPORT  ResetHandler            ; In init.s
    IMPORT  UndefHandler            ; In isr.c
    IMPORT  SvcHandler              ; In isr.c
    IMPORT  PrefetchHandler         ; In isr.c
    IMPORT  AbortHandler            ; In isr.c
    IMPORT  IrqHandler              ; In isr.c
    IMPORT  FiqHandler              ; In isr.c

ResetAddr      DCD     ResetHandler
UndefinedAddr  DCD     UndefHandler
SvcAddr        DCD     SvcHandler
PrefetchAddr   DCD     PrefetchHandler
AbortAddr      DCD     AbortHandler
IrqAddr        DCD     IrqHandler
FiqAddr        DCD     FiqHandler

 ]                                  ; !USE_LINUX

    END
