; vectorsl.s
;
; Copyright 2008 by Mindspeed Technologies
; All rights reserved
;
; Interrupt vector table for Lower ARM

 [ :LNOT: :DEF: USE_LINUX

    PRESERVE8

    AREA VectLow, CODE, READONLY

    ENTRY

    ; Vector 0x00 is for RESET handler
    LDR     PC, ResetAddrL

    ; Vector 0x04 is for UNDEF handler
    LDR     PC, UndefinedAddrL

    ; Vector 0x08 is for SVC handler
    LDR     PC, SvcAddrL

    ; Vector 0x0C is for PABT handler
    LDR     PC, PrefetchAddrL

    ; Vector 0x10 is for DABT handler
    LDR     PC, AbortAddrL

    ; Vector 0x14 is reserved
    NOP

    ; Vector 0x18 is for IRQ handler
    LDR     PC, IrqAddrL

    ; Vector 0x1C is for FIQ handler
    LDR     PC, FiqAddrL

    IMPORT  ResetHandlerL           ; In init.s
    IMPORT  UndefHandlerL           ; In isr.c
    IMPORT  SvcHandlerL             ; In isr.c
    IMPORT  PrefetchHandlerL        ; In isr.c
    IMPORT  AbortHandlerL           ; In isr.c
    IMPORT  IrqHandlerL             ; In isr.c
    IMPORT  FiqHandlerL             ; In isr.c

ResetAddrL      DCD     ResetHandlerL
UndefinedAddrL  DCD     UndefHandlerL
SvcAddrL        DCD     SvcHandlerL
PrefetchAddrL   DCD     PrefetchHandlerL
AbortAddrL      DCD     AbortHandlerL
IrqAddrL        DCD     IrqHandlerL
FiqAddrL        DCD     FiqHandlerL

 ]                                  ; !USE_LINUX

    END
