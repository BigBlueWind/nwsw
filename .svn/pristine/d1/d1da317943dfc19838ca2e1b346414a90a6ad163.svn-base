;*********************************************************************************
;                                                                                *
;       Copyright 2008 by Mindspeed Technologies, Inc.                           *
;       All Rights Reserved                                                      *
;                                                                                *
;       Mindspeed Technologies, Inc.                                             *
;       4000 MACARTHUR BLVD - EAST TOWER                                         *
;       NEWPORT BEACH, CA 92660                                                  *
;                                                                                *
;*********************************************************************************
;       4gmx_mmu.s                                                               *
;                                                                                *
;       ARM MMU API implementation                                               *
;                                                                                *
;*********************************************************************************/

	PRESERVE8

	EXPORT MxiInvalidICache
	EXPORT MxiInvalidDCache
	EXPORT MxiInvalidIDCache
	EXPORT MxiFlushDCache
	EXPORT MxiFlushIDCache

    EXPORT _MxiCacheInvalidate
	EXPORT _MxiCacheClean
	EXPORT _MxiCacheFlush
	
	EXPORT MxiEnableICache
	EXPORT MxiDisableICache

	EXPORT MxiEnableMmuMaster
	EXPORT MxiEnableMmuSlave
	EXPORT MxiDisableMMU

	EXPORT MxiEnableDCache
	EXPORT MxiDisableDCache
	EXPORT MxiInvalidateDataTLB

	AREA MXMMUAREA, CODE, READONLY

MxiEnableICache

	ISB
	DMB
	DSB
	mrc	p15, 0, r0, c1, c0, 0
	ORR	r0, r0, #0x1800
	mcr	p15, 0, r0, c1, c0, 0
	DMB
	DSB	
	ISB

	BX LR

MxiDisableICache

	DSB
	DMB
	mrc	p15, 0, r0, c1, c0, 0
	BIC	r0, r0, #0x1800
	mcr	p15, 0, r0, c1, c0, 0
	dsb	
	isb

	BX LR


MxiEnableDCache

	MOV R3, LR
	BL  MxiInvalidDCache
	MOV LR, R3
	
	ISB
	DMB
	DSB
	
	mrc	p15, 0, r0, c1, c0, 0
	ORR	r0, r0, #0x4	;
	mcr	p15, 0, r0, c1, c0, 0
	
	ISB
	DMB
	DSB	

	; TO ENABLE SMP 
	
	;MRC p15, 0, r0, c1, c0, 1; Read ACTLRs
	;ORR r0, r0, #(1<<6)|1
	;MCR p15, 0, r0, c1, c0, 1; Write ACTLR

	BX LR

MxiDisableDCache
	
	DMB
	DSB
	mrc	p15, 0, r0, c1, c0, 0
	BIC	r0, r0, #0x4
	mcr	p15, 0, r0, c1, c0, 0
	dsb	
	isb

	BX LR

MxiEnableMMU2

	ISB
    DMB
    DSB

	;MOV R3, LR
	;BL MxiFlushDCache
	;MOV LR, R3
    
	mrc	p15, 0, r0, c1, c0, 0
	ORR	r0, r0, #0x1	;
	ISB
    DMB
    DSB
	mcr	p15, 0, r0, c1, c0, 0
	ISB
    DMB
    DSB
	
	BX LR


; ------------------------------------------------------------
;  4GXM SCU controls
; ------------------------------------------------------------

enable_scu

  MRC     p15, 4, r0, c15, c0, 0  ; Read periph base address

  LDR     r1, [r0, #0x0]          ; Read the SCU Control Register
  ORR     r1, r1, #0x1            ; Set bit 0 (The Enable bit)
  STR     r1, [r0, #0x0]          ; Write back modifed value

  BX      lr

; ------------------------------------------------------------
;  4GXM enable SMP mode
; ------------------------------------------------------------
enable_smp

  ; SMP status is controlled by bit 6 of the CP15 Aux Ctrl Reg

  MRC     p15, 0, r0, c1, c0, 1   ; Read ACTLR
  ORR     r0, r0, #((1 << 6):OR:(1 << 2))          ; Set bit#6 / bit#2
  MCR     p15, 0, r0, c1, c0, 1   ; Write ACTLR

  BX      lr

; ------------------------------------------------------------
;  4GXM disable SMP mode
; ------------------------------------------------------------
disable_smp

  ; SMP status is controlled by bit 6 of the CP15 Aux Ctrl Reg

  MRC     p15, 0, r0, c1, c0, 1   ; Read ACTLR
  BIC     r0, r0, #0x040          ; Clear bit 6
  MCR     p15, 0, r0, c1, c0, 1   ; Write ACTLR

  BX      lr

; ------------------------------------------------------------
;  4GXM Get SMP mask
; ------------------------------------------------------------
get_smp_mask

  MRC     p15, 4, r0, c15, c0, 0  ; Read periph base address

  LDR     r0, [r0, #0x004]        ; Read SCU Configuration register
  MOV     r0, r0, LSR #4          ; Bits 7:4 gives the cores in SMP mode, shift then mask
  AND     r0, r0, #0x0F

  BX      lr


; --------------------------------------------------------------
  ; void TLB_maintenance_broadcast_onn(void)
  ; Enable the broadcasting of cache & TLB maintenance operations
  ; When enabled AND in SMP, broadcast all "inner sharable"
  ; cache and TLM maintenance operations to other SMP cores
; --------------------------------------------------------------
TLB_maintenance_broadcast_on PROC
  MRC     p15, 0, r0, c1, c0, 1   ; Read Aux Ctrl register
  ORR     r0, r0, #0x01           ; Set the FW bit (bit 0)
  MCR     p15, 0, r0, c1, c0, 1   ; Write Aux Ctrl register

  BX      lr
  ENDP

; --------------------------------------------------------------
  ; void TLB_maintenance_broadcast_off(void)
  ; Enable the broadcasting of cache & TLB maintenance operations
  ; When enabled AND in SMP, broadcast all "inner sharable"
  ; cache and TLM maintenance operations to other SMP cores
; --------------------------------------------------------------
TLB_maintenance_broadcast_off PROC
  MRC     p15, 0, r0, c1, c0, 1   ; Read Aux Ctrl register
  BIC     r0, r0, #0x01           ; Clear the FW bit (bit 0)
  MCR     p15, 0, r0, c1, c0, 1   ; Write Aux Ctrl register

  BX      lr
  ENDP

; ------------------------------------------------------------
  ; void scu_invalidate_secure_ways(unsigned int cpu, unsigned int ways)
  ; cpu: 0x0=CPU 0 0x1=CPU 1 etc...
  ; This function invalidates the SCU copy of the tag rams
  ; for the specified core.  Typically only done at start-up.
  ; Possible flow:
  ; - Invalidate L1 caches
  ; - Invalidate SCU copy of TAG RAMs
  ; - Join SMP
;-------------------------------------------------------------
  
scu_invalidate_secure_ways
  AND     r0, r0, #0x03           ; Mask off unused bits of CPU ID
  MOV     r0, r0, LSL #2          ; Convert into bit offset (four bits per core)
  
  AND     r1, r1, #0x0F           ; Mask off unused bits of ways
  MOV     r1, r1, LSL r0          ; Shift ways into the correct CPU field

  MRC     p15, 4, r2, c15, c0, 0  ; Read periph base address

  STR     r1, [r2, #0x0C]         ; Write to SCU Invalidate All in Secure State
  
  BX      lr


; -------------------------------------------------------------
;    void MxiEnableMmuMaster (UINT32 ctrl)
;         @param ctrl [in] - see MMU_CTRL_xxx of 4gmx_arm_mmu.h file
;
;    this function enables SMP mode for the master processor, in our case 
;    this is ARM:core#0
; -------------------------------------------------------------

MxiEnableMmuMaster	PROC

	PUSH {r0, lr}
	BL MxiInvalidIDCache
	POP  {r0, lr}

	;
	; Barrier
	; --------
	DSB
	DMB

	; Clear Branch Prediction Array
	; ------------------------------
	MOV     r1, #0x0
	MCR     p15, 0, r1, c7, c5, 6     ; BPIALL - Invalidate entire branch predictor array

	;
	; Invalidate TLBs
	;------------------
	MOV     r1, #0x0
	MCR     p15, 0, r1, c8, c7, 0     ; TLBIALL - Invalidate entire Unifed TLB

	;
	; Barrier
	; --------
	DSB
	DMB

	; -------------------------------------------
	; SAVE LR in R3
	; -------------------------------------------
	MOV R3, LR

	; Enable MMU
	;-------------
	MRC     p15, 0, r1, c1, c0, 0		; Read current control reg
	ORR     r1, r1, #1					; M-flag
	
	tst r0, #(1<<2)		;MMU_CTRL_ICACHE_ENABLED
	ORRNE   r1, r1, #0x1800

	tst r0, #(1<<3)		;MMU_CTRL_DCACHE_ENABLED
	ORRNE   r1, r1, #0x4
	
	MCR     p15, 0, r1, c1, c0, 0		; Write reg back

	tst r0, #(1 << 31)	; see MxMmuCtrl function, 31 bit is reserved to be used to enable SMP mode
	BEQ no_smp_master

	BL      enable_scu

	; ---------------------
	; Enable SMP mode 
	; ---------------------
	MOV     r0, #0x0                  ; Move CPU ID into r0
	MOV     r1, #0xF                  ; Move 0xF (represents all four ways) into r1
	BL      scu_invalidate_secure_ways
	BL      enable_smp
	BL      TLB_maintenance_broadcast_on

no_smp_master
	MOV 	LR, R3
	BX LR
	ENDP

; -------------------------------------------------------------
;    void MxiEnableMmuSlave (UINT32 ctrl)
;         @param ctrl [in] - see MMU_CTRL_xxx of 4gmx_arm_mmu.h file
;
;    this function joins SMP pool, for core#1, core#2, core#3
; -------------------------------------------------------------

MxiEnableMmuSlave	PROC

	PUSH {r0, lr}
	BL MxiInvalidIDCache
	POP  {r0, lr}

	;
	; Barrier
	; --------
	DSB
	DMB

	; Clear Branch Prediction Array
	; ------------------------------
	MOV     r1, #0x0
	MCR     p15, 0, r1, c7, c5, 6     ; BPIALL - Invalidate entire branch predictor array

	;
	; Invalidate TLBs
	;------------------
	MOV     r1, #0x0
	MCR     p15, 0, r1, c8, c7, 0     ; TLBIALL - Invalidate entire Unifed TLB

	;
	; Barrier
	; --------
	DSB
	DMB

	; -------------------------------------------
	; SAVE LR in R3
	; -------------------------------------------
	MOV R3, LR

	; Enable MMU
	;-------------
	MRC     p15, 0, r1, c1, c0, 0		; Read current control reg
	ORR     r1, r1, #1					; M-flag

	tst r0, #(1<<2)		;MMU_CTRL_ICACHE_ENABLED
	ORRNE   r1, r1, #0x1800

	tst r0, #(1<<3)		;MMU_CTRL_DCACHE_ENABLED
	ORRNE   r1, r1, #0x4
	
	MCR     p15, 0, r1, c1, c0, 0		; Write reg back

	tst r0, #(1 << 31)	; see MxMmuCtrl function, 31 bit is reserved to be used to enable SMP mode
	BEQ no_smp_slave

	;BL      enable_scu

	; ---------------------
	; Enable SMP mode 
	; ---------------------
	MOV     r0, #0x0                  ; Move CPU ID into r0
	MOV     r1, #0xF                  ; Move 0xF (represents all four ways) into r1
	BL      scu_invalidate_secure_ways
	BL      enable_smp
	BL      TLB_maintenance_broadcast_on

no_smp_slave
	MOV 	LR, R3
	BX LR
	ENDP


MxiDisableMMU

	isb
	;dsb
	DMB
	mrc	p15, 0, r0, c1, c0, 0
	BIC	r0,  r0, #0x1	;
	mcr	p15, 0, r0, c1, c0, 0
	dsb	
	isb

	BX LR

MxiFlushIDCache

	ISB
	DMB
	DSB
	
	mov     r0,  #0
	mcr     p15, 0, r0, c8, c7, 0           ; Invalidate entire Unified TLB
	mcr     p15, 0, r0, c7, c5, 0           ; Invalidate ICache
	
	ISB
	DMB
	DSB

	MOV     r0, #0 << 5             ; start with first line

nextset_flush
	MOV     r1, #0 << 30            ; start with way 0 

nextway_flush
	ORR     r2, r0, r1              ; contruct Way/Index value
	MCR     p15, 0, r2, c7, c14, 2  ; Clean and Invalidate by Way/Index (section)
	ADD     r1, r1, #1 << 30        ; Increment Way
	CMP     r1, #0
	BNE     nextway_flush
	ADD     r0, r0, #1 << 5         ; Increment Line
	CMP     r0, #1 << 13            ; Look for overflow out of Set field
	BNE     nextset_flush

	DMB
	DSB
	ISB
	BX	LR


MxiInvalidIDCache

    ISB
	DMB
	DSB
	
    mov     r0,  #0
	mcr     p15, 0, r0, c8, c7, 0           ; Invalidate entire Unified TLB
	mcr     p15, 0, r0, c7, c5, 0           ; Invalidate ICache
	
	ISB
	DMB
	DSB
    
	MOV     r0, #0 << 5             ; start with first line

nextset_invalidate1
	MOV     r1, #0 << 30            ; start with way 0 

nextway_invalidate1
	ORR     r2, r0, r1              ; contruct Way/Index value
	MCR     p15, 0, r2, c7, c6, 2   ; Invalidate by Way/Index (section)
	ADD     r1, r1, #1 << 30        ; Increment Way
	CMP     r1, #0
	BNE     nextway_invalidate1
	ADD     r0, r0, #1 << 5         ; Increment Line
	CMP     r0, #1 << 13            ; Look for overflow out of Set field
	BNE     nextset_invalidate1

	ISB
	DMB
	DSB

	BX	LR
	
;	MOV     r0, #0 << 5             ; start with first line
;
;nextset_clean1
;	MOV     r1, #0 << 30            ; start with way 0 
;
;nextway_clean1
;	ORR     r2, r0, r1              ; contruct Way/Index value
;	MCR     p15, 0, r2, c7, c14, 2  ; Clean and Invalidate by Way/Index (section)
;	ADD     r1, r1, #1 << 30        ; Increment Way
;	CMP     r1, #0
;	BNE     nextway_clean1
;	ADD     r0, r0, #1 << 5         ; Increment Line
;	CMP     r0, #1 << 13            ; Look for overflow out of Set field
;	BNE     nextset_clean1
;
;	DSB
;	BX	LR
	

; ============================================================
;   This function invalidates all ICache and should be called 
;   only one time at the step of system initialization
;   Parameters: void
;   Return:     void
;     
; ============================================================

MxiInvalidICache

	ISB
    	DMB
    	mov     r0,  #0
	mcr     p15, 0, r0, c8, c7, 0           ; Invalidate entire Unified TLB
	mcr     p15, 0, r0, c7, c5, 0           ; Invalidate ICache
	ISB
	DSB
	ISB

	BX LR

; ============================================================
;   This function invalidates all DCache and should be called 
;   only one time at the step of system initialization
;   Parameters: void
;   Return:     void
;     
; ============================================================

MxiInvalidDCache
; -----------------------------------------------------
; written for 32KB cache
; -----------------------------------------------------
	ISB
	DMB
	DSB

	MOV     r0, #0 << 5             ; start with first line

nextset_invalidate
	MOV     r1, #0 << 30            ; start with way 0 

nextway_invalidate
	ORR     r2, r0, r1              ; contruct Way/Index value
	MCR     p15, 0, r2, c7, c6, 2   ; Invalidate by Way/Index (section)
	ADD     r1, r1, #1 << 30        ; Increment Way
	CMP     r1, #0
	BNE     nextway_invalidate
	ADD     r0, r0, #1 << 5         ; Increment Line
	CMP     r0, #1 << 13            ; Look for overflow out of Set field
	BNE     nextset_invalidate

    DMB
    DSB
    ISB
    
    BX      r14
    
;	MOV     r0, #0 << 5             ; start with first line
;
;nextset_clean
;	MOV     r1, #0 << 30            ; start with way 0 

;nextway_clean
;	ORR     r2, r0, r1              ; contruct Way/Index value
;	MCR     p15, 0, r2, c7, c14, 2  ; Clean and Invalidate by Way/Index (section)
;	ADD     r1, r1, #1 << 30        ; Increment Way
;	CMP     r1, #0
;	BNE     nextway_clean
;	ADD     r0, r0, #1 << 5         ; Increment Line
;	CMP     r0, #1 << 13            ; Look for overflow out of Set field
;	BNE     nextset_clean

;	DSB
    
;	BX      r14

; ============================================================
;   This function cleans and invalidates DCache 
;   Parameters: void
;   Return:     void
;   Registers : r0, r1, r2 are used
; ============================================================

MxiFlushDCache
; -----------------------------------------------------
; written for 32KB cache
; -----------------------------------------------------
	ISB
	DMB
	DSB

	MOV     r0, #0 << 5             ; start with first line

nextset_id_flush
	MOV     r1, #0 << 30            ; start with way 0 

nextway_id_flush
	ORR     r2, r0, r1              ; contruct Way/Index value
	DSB
	MCR     p15, 0, r2, c7, c14, 2  ; Clean and Invalidate by Way/Index (section)
	ADD     r1, r1, #1 << 30        ; Increment Way
	CMP     r1, #0
	BNE     nextway_id_flush
	ADD     r0, r0, #1 << 5         ; Increment Line
	CMP     r0, #1 << 13            ; Look for overflow out of Set field
	BNE     nextset_id_flush

	ISB
	DMB
	DSB
    
	BX      r14


; ============================================================
;   This function invalidates DCache line and may be used in APP 
;   to invalidate the DCache (variables, arrays and so on...)
;   Parameters: 
;        R0 - the base address (not-alignment)
;        R1 - the size in bytes 
;   Return:     void
;     
; ============================================================

_MxiCacheInvalidate

	;MRS 	r2, CPSR
	;ORR 	r3, r2, #0xC0 
	;MSR 	CPSR_c, r3

	ISB
	DSB
	ADD		r1, r0, r1

	TST 	r0, #31 
	BIC 	r0, r0, #31
	MCRNE	p15, 0, r0, c7, c14, 1		; clean & invalidate D / U line

	ISB
	DSB
	TST		r1, #31
	BIC		r1, r1, #31
	MCRNE	p15, 0, r1, c7, c14, 1		; clean & invalidate D / U line
	
InvL
	DSB
	MCR		p15, 0, r0, c7, c6, 1		; invalidate D / U line
	ADD		r0, r0, #32

	CMP		r0, r1
	BLO		InvL
	
	ISB
	DSB
	;MSR 	CPSR_c, r2
	
	BX      LR
; ==========================================


; ============================================================
;   This function cleans DCache line and may be used in APP 
;   to clean the DCache (variables, arrays and so on...)
;   Parameters: 
;        R0 - the base address (not-alignment)
;        R1 - the size in bytes 
;   Return:     void
;     
; ============================================================

_MxiCacheClean

    ;MRS 	r2, CPSR
    ;ORR 	r3, r2, #0xC0 
    ;MSR 	CPSR_c, r3

	; to get the last address
	ADD	r1, r0, r1
	
	; round base address 
	BIC	r0, r0, #31

	ADD	r1, r1, #31
	BIC r1, r1, #31

	ISB

CleanL

	DSB

	MCR	p15, 0, r0, c7, c10, 1	; clean D / U line

	ADD	r0, r0, #32

	CMP	r0, r1
	BLO	CleanL

    ISB
	DSB
    ;MSR 	CPSR_c, r2

	BX      LR
; ==========================================

; ============================================================
;   This function flushes DCache line and may be used in APP 
;   to flush the DCache (variables, arrays and so on...)
;   Parameters: 
;        R0 - the base address (not-alignment)
;        R1 - the size in bytes 
;   Return:     void
;     
; ============================================================

_MxiCacheFlush

    ;MRS 	r2, CPSR
    ;ORR 	r3, r2, #0xC0 
    ;MSR 	CPSR_c, r3
	
	; to get the last address
	ADD	r1, r0, r1
	
	; rount base address 
	BIC	r0, r0, #31

	ADD	r1, r1, #31
	BIC 	r1, r1, #31

	ISB

FlushL
	DSB
	MCR	p15, 0, r0, c7, c14, 1		; clean & invalidate D / U line

	ADD	r0, r0, #32
	CMP	r0, r1

	BLO	FlushL

	ISB
	DSB
	;MSR 	CPSR_c, r2
	BX      LR


MxiAllCacheInv

invalidate_cache

		STMDB  sp!, {r0-r12}


	DMB
	mov     r0, #0
	mcr     p15, 0, r0, c8, c7, 0           ; Invalidate entire Unified TLB
	mcr     p15, 0, r0, c7, c5, 0           ; Invalidate ICache
        mrc     p15, 1, r0, c0, c0, 1           ; read clidr
        ands    r3, r0, #0x7000000              ; extract loc from clidr
        mov     r3, r3, lsr #23                 ; left align loc bit field
        beq     finished                        ; if loc is 0, then no need to clean
        mov     r10, #0                         ; start clean at cache level 0
loop1
        add     r2, r10, r10, lsr #1            ; work out 3x current cache level
        mov     r1, r0, lsr r2                  ; extract cache type bits from clidr
        and     r1, r1, #7                      ; mask of the bits for current cache only
        cmp     r1, #2                          ; see what cache we have at this level
        blt     skip                            ; skip if no cache, or just i-cache
        mcr     p15, 2, r10, c0, c0, 0          ; select current cache level in cssr
        isb                                     ; isb to sych the new cssr&csidr
        mrc     p15, 1, r1, c0, c0, 0           ; read the new csidr
        and     r2, r1, #7                      ; extract the length of the cache lines
        add     r2, r2, #4                      ; add 4 (line length offset)
        ldr     r4, =0x3ff
        ands    r4, r4, r1, lsr #3              ; find maximum number on the way size
        clz     r5, r4                          ; find bit position of way size increment
        ldr     r7, =0x7fff
        ands    r7, r7, r1, lsr #13             ; extract max number of the index size
loop2
        mov     r9, r4                          ; create working copy of max way size
loop3
        orr     r11, r10, r9, lsl r5            ; factor way and cache number into r11
        orr     r11, r11, r7, lsl r2            ; factor index number into r11
        mcr     p15, 0, r11, c7, c6, 2          ; invalidate by set/way
        subs    r9, r9, #1                      ; decrement the way
        bge     loop3
        subs    r7, r7, #1                      ; decrement the index
        bge     loop2
skip
        add     r10, r10, #2                    ; increment cache number
        cmp     r3, r10
        bgt     loop1

finished
        mov     r10, #0                         ; swith back to cache level 0
        mcr     p15, 2, r10, c0, c0, 0          ; select current cache level in cssr
        dsb
        isb
        mov     pc, lr

fixup_cpu
        mrc     p15, 0, r10, c15, c0, 1         ; 
        orr     r10, r10, #(1 << 12)            ; disable outstanding access
        orr     r10, r10, #(1 << 20)            ; disable PLD
        orr     r10, r10, #(1 << 24)            ; disable PLI	
        mcr     p15, 0, r10, c15, c0, 1         ;
	dsb		
        mrc     p15, 0, r10, c1, c0, 1          ; errata #571620
	bic     r10, r10, #(1 << 2)             ; disable L1 automatic data prefecher
	bic     r10, r10, #(1 << 1)             ; disable L2 automatic data prefecher
	mcr     p15, 0, r10, c1, c0, 1          ;
	dsb

	LDMIA  sp!, {r0-r12}

	mov	pc, lr

MxiInvalidateDataTLB

	DSB
	mov r0, #0
	MCR p15,0,r0,c8,c6,0		; Invalidate data TLBb entry by MVA
	DSB
	ISB
	BX LR

	END



