;*********************************************************************************
;                                                                                *
;       Copyright 2010 by Mindspeed Technologies, Inc.                           *
;       All Rights Reserved                                                      *
;                                                                                *
;       Mindspeed Technologies, Inc.                                             *
;       4000 MACARTHUR BLVD - EAST TOWER                                         *
;       NEWPORT BEACH, CA 92660                                                  *
;                                                                                *
;*********************************************************************************
;       4gmx_arm.s                                                               *
;                                                                                *
;       ARM API implementation                                                   *
;                                                                                *
;*********************************************************************************/

	PRESERVE8

	EXPORT 	sys_write_control
	EXPORT  sys_read_control
	EXPORT  sys_read_coreid
	EXPORT  mmu_write_ttb0
	EXPORT  mmu_read_ttb0
	EXPORT  mmu_write_domain_access
	EXPORT  mmu_read_domain_access
	EXPORT	mmu_read_fault_address

	EXPORT  MxGetCpuMode
	EXPORT  MxSetCpuMode
	EXPORT  ARM_INT_disable
	EXPORT  ARM_INT_restore
	EXPORT  ARM_INT_enable
	EXPORT  MxGetCPSR
	EXPORT  MxSetCPSR

	EXPORT  MxGetAuxCtrl
	EXPORT  MxSetAuxCtrl

	EXPORT 	MxiGetLock
	EXPORT 	MxiGetLockEx
	EXPORT 	MxiReleaseLockEx
	EXPORT  MxiReleaseLock
	EXPORT 	MxDSB
	EXPORT 	MxISB
	EXPORT 	MxDMB
	
	EXPORT 	MxSetVB
	EXPORT 	MxGetVB

	EXPORT  MxWFI

	EXPORT  MxiARMErrata
	EXPORT  MxSycnProc

	IMPORT  MxiFlushDCache
	IMPORT  MxiInvalidDCache

	EXPORT  MxGetArmScuBaseAddr
	EXPORT  MxGetArmGicDistBaseAddr
	EXPORT  MxGetArmGicCPUBaseAddr
	EXPORT  MxGetSp

	AREA MX_ARM_AREA, CODE, READONLY

MxDSB
	 DSB
	 BX LR

MxDMB
	DMB
	BX LR

MxISB
	 ISB
	 BX LR

; ------------------------------------------------------
;            MxiGetLock 
;               r0 - address of Lock variable 
;               r1 - storage to read data 
;               r2 - storage to read data 
;               r3 - IRQ/FIQ status
; ------------------------------------------------------
MxiGetLock

		MRS 	r3, CPSR				; Save IRQ Status
		ORR 	r1, r3, #(0x80 | 0x40)	; Disable IRQ/FIQ
		MSR 	CPSR_c, r1				; Disable IRQ/FIQ
		MOV		r2, #0xFF

		ISB
		
TRYAGAIN

		DMB
		DSB
		
        LDREX   r1, [r0]      ;/** load the lock value    */
        CMP     r1, #0        ;/** is the lock free?      */
        STREXEQ r1, r2, [r0]  ;/** try and claim the lock */
        CMPEQ   r1, #0        ;/** did this succeed?      */
        BNE     TRYAGAIN      ;/** no - try again. . .    */
        
        DMB
        DSB

		MSR 	CPSR_c, r3    ; Disable IRQ/FIQ        ; return the IRQ/FIQ status
		BX		LR	

; ----------------------------------

		MOV	r2, #0xFF

TRYAGAIN2
		DMB
        LDREX   r1, [r0]         ;/** load the lock value */
        CMP     r1, #0           ;/** is the lock free? */
        STREXEQ r1, r2, [r0]  ;/** try and claim the lock */
        CMPEQ   r1, #0           ;/** did this succeed? */
        BNE     TRYAGAIN2         ;/** no - try again. . . */
        DMB
	BX	LR

; ========================================
;   Input:  r0 - address of lock variable 
;  Return:  IRQ status 
; ========================================
MxiGetLockEx
		MRS 	r3, CPSR				; Save IRQ Status
		ORR 	r1, r3, #(0x80 | 0x40)	; Disable IRQ/FIQ
		MSR 	CPSR_c, r1				; Disable IRQ/FIQ
		
		MOV		r2, #0xFF
TRYAGAIN_EX

		DMB
		
        LDREX   r1, [r0]      ;/** load the lock value    */
        CMP     r1, #0        ;/** is the lock free?      */
        STREXEQ r1, r2, [r0]  ;/** try and claim the lock */
        CMPEQ   r1, #0        ;/** did this succeed?      */
        BNE     TRYAGAIN_EX   ;/** no - try again. . .    */
        
        DMB

		MOV		R0, R3	; return the IRQ/FIQ status
		BX	LR	

; ========================================
;   Input:  r0 - address of lock variable 
;   Input:  r1 - IRQ status
;  Return:  none 
; ========================================
MxiReleaseLockEx
		DMB
		MOV R2, #0
		STR R2, [R0]
		DMB
		MSR 	CPSR_c, r1				; Restore IRQ/FIQ
		BX	LR		


; ========================================
;   Input:  r0 - address of lock variable 
;  Return:  none 
; ========================================
MxiReleaseLock
		MRS 	r3, CPSR				; Save IRQ Status
		ORR 	r1, r3, #(0x80 | 0x40)	; Disable IRQ/FIQ
		MSR 	CPSR_c, r1				; Disable IRQ/FIQ

		MOV		r1, #0x00
		
		DMB
		DSB
		STR		r1, [r0]
		DMB
		DSB

		MSR 	CPSR_c, r3				; Restore IRQ/FIQ
		
		BX	LR


sys_write_control
	MCR	p15, 0, r0, c1, c0, 0
	BX	LR

sys_read_control
	MRC	p15, 0, r0, c1, c0, 0
	BX	LR

sys_read_coreid
	MRC	p15, 0, r0, c0, c0, 5
	BX	LR	

mmu_write_ttb0
	MCR	p15, 0, r0, c2, c0, 0
	BX	LR

mmu_read_ttb0
	MRC	p15, 0, r0, c2, c0, 0
	BX	LR

mmu_write_domain_access
	MCR	p15, 0, r0, c3, c0, 0
	BX	LR

mmu_read_domain_access
	MRC	p15, 0, r0, c3, c0, 0
	BX	LR

mmu_read_fault_address
	MRC	p15, 0, r0, c6, c0, 0
	BX	LR

MxGetCpuMode
	MRS	r0, CPSR
	AND	r0, r0, #0x1F
	BX	LR


MxSetCpuMode
	MRS 	r1, CPSR
	BIC 	r1, r1, #0x1F
	ORR 	r1, r1, r0
	MSR 	CPSR_c, r1
	MOV	r0, #0
	BX	LR

ARM_INT_disable
	MRS 	r0, CPSR
	ORR 	r1, r0, #(0x80 | 0x40)
	MSR 	CPSR_c, r1
	BX 	LR

ARM_INT_enable
	MRS 	r1, CPSR
	BIC 	r1, r1, r0
	MSR 	CPSR_c, r1
	BX 	LR	

ARM_INT_restore
	MSR 	CPSR_c, r0
	BX	LR


MxGetCPSR
	MRS	r0, CPSR
	BX	LR


MxSetCPSR
	MSR	CPSR_c,	r0
	BX	LR
	

MxGetAuxCtrl
	MRC 	p15, 0, r0, c1, c0, 1 ; Read ACTLR
	BX	LR


MxSetAuxCtrl	

	ISB
	DMB
	DSB

	MCR 	p15, 0, r0, c1, c0, 1 ; Write ACTLR
	
	ISB
	DMB
	DSB

	;B MxiFlushDCache
	;B MxiInvalidDCache
	
	BX	LR


MxSetVB
	MCR p15, 0, r0, c12, c0, 0
	BX	LR

MxGetVB
	MRC p15, 0, r0, c12, c0, 0
	BX	LR	


; ------------------------------------------
;  Wait for interrupt function
; ------------------------------------------
MxWFI
	WFI
	BX LR


; ------------------------------------------
;  RO is defined like function parameter and 
;  can be used with this function
; ------------------------------------------
MxiARMErrata

	; CP15, 0, C15, C0, 1 is not documented 
	; some description of this register can be found in ARM errata doc

	MRC     p15, 0, r0, c15, c0, 1         ;
	ORR     r0, r0, #(1 << 12)            ; disable outstanding access
	ORR     r0, r0, #(1 << 20)            ; disable PLD, PLD (preload data cache) will be treated like NOP
	ORR     r0, r0, #(1 << 24)            ; disable PLI PLI (preload instruction) will be treated like NOP
	MCR     p15, 0, r0, c15, c0, 1         ;
	DSB		
	MRC     p15, 0, r0, c1, c0, 1          ; errata #571620
	BIC     r0, r0, #(1 << 2)             ; disable L1 automatic data prefecher
	BIC     r0, r0, #(1 << 1)             ; disable L2 automatic data prefecher
	MCR     p15, 0, r0, c1, c0, 1          ;
	DSB
	MOV	pc, lr

MxSycnProc

	ISB
	DMB
	DSB
	BX LR

; ------------------------------------------------------------
;     UINT32 MxGetArmScuBaseAddr (void)
;
;     It returns the base address of ARM SCU block
; ------------------------------------------------------------

MxGetArmScuBaseAddr
  MRC     p15, 4, r0, c15, c0, 0  ; Read periph base address
  BX LR

; ------------------------------------------------------------
;     UINT32 MxGetArmGicBaseAddr (void)
;
;     It returns the base address of ARM SCU block
; ------------------------------------------------------------

MxGetArmGicDistBaseAddr
  MRC     p15, 4, r0, c15, c0, 0  ; Read periph base address
  ADD     r0, r0, #0x1000
  BX LR

MxGetSp
  MOV		r0, sp
  BX LR

MxGetArmGicCPUBaseAddr
  MRC     p15, 4, r0, c15, c0, 0  ; Read periph base address
  ADD     r0, r0, #0x100
  BX LR

  END



