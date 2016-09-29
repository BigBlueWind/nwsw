//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL icdrv.c
 *
 * @brief The implementation of GIC driver
 * @author Mindspeed Technologies
 * @version $Revision: 1.59 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "icdrv.h"
#include "appprintf.h"
#include "hal.h"
#include "sirqassign.h"
#include "sysshutdown.h"
#include "ncnbdata.h"
#include "appinit.h"
#include "larmmsgid.h"
#include "mdmadrv.h"
#include "gemdrv.h"
#include "FECdrv.h"
#include "fpdrv.h"
#include "uartdrv.h"
#include "pm.h"

VUINT32 NCNBVAR GicLowerARM;
INTCCTX GicCtx [CPU_NUM];

#ifdef GIC_TRACE_ENABLED

GIT_IRQ_TRACE_TYPE GicIrqTrace [GIC_TRACE_SIZE];
UINT32 GicIrqTraceIndex = 0;
VUINT32 GicIrqTraceEnabled = 0;

#endif // GIC_TRACE_ENABLED

extern MXEHT void UndefHandlerX(void);
extern MXEHT void PrefHandlerX(void);
extern MXEHT void DataHandlerX(void);

MXEHT_ void UndefHandler(void)
{
    int addr = __return_address() - 4;
    VUINT32 reg = 0;

#ifdef PROC_ABORTS_ENABLED
    switch (MxGetCpuID ())
    {
        case 0:
            addr = gAbortCtx0.lr;
            break;

        case 1:
            addr = gAbortCtx1.lr;
            break;

        case 2:
            addr = gAbortCtx2.lr;
            break;

        case 3:
            addr = gAbortCtx3.lr;
            break;

        default:
            addr = gAbortCtx0.lr;
            break;
    }
#endif

#ifdef SYS_INFO_ENABLED
    syscoreinfo [MxGetCpuID ()].bUndefMode = 1;
    syscoreinfo [MxGetCpuID ()].nExceptionLR = addr;
#endif

#ifdef PROC_ABORTS_ENABLED
    SysShdwnProcAbort (ALARM_IDX_UNDEF);
#endif

    SysShutdownIRQHandler(&reg, 0, NULL);

    while (1) ;
}

ASMPROC void SWIHandlerAsm (void)
{
    mrc p15,0,r0,c1,c1,0  // to read SecureControlMonitor
    BIC r0, #0x1          // reset NS: val &= ~(1<<0)
    mcr p15,0,r0,c1,c1,0  // change to secure world

    SUBS pc, lr, #0
}
    

MXEHT void SWIHandler(void)
{
#if 0
    int addr = __return_address() - 4;
    VUINT32 reg = 0;

#ifdef SYS_INFO_ENABLED
    syscoreinfo [MxGetCpuID ()].bSWIMode = 1;
    syscoreinfo [MxGetCpuID ()].nExceptionLR = addr;
#endif

    uart_printf ("(cpu=%d)SWI Handler: 0x%08x\r\n", MxGetCpuID (), addr);

    SysShutdownIRQHandler(&reg, 0, NULL);

    while (1) ;
#endif    
}

MXEHT_ void PrefHandler(void)
{
    int addr = __return_address() - 4;
    VUINT32 reg = 0;

    uart_printf_print_buf();
    uart_printf_select_dev(PRINT_DEV_UART, 0, 0);

#ifdef PROC_ABORTS_ENABLED

    switch (MxGetCpuID ())
    {
        case 0:
            addr = gAbortCtx0.lr;
            break;

        case 1:
            addr = gAbortCtx1.lr;
            break;

        case 2:
            addr = gAbortCtx2.lr;
            break;

        case 3:
            addr = gAbortCtx3.lr;
            break;

        default:
            addr = gAbortCtx0.lr;
            break;
    }
    
#endif

#ifdef SYS_INFO_ENABLED
    syscoreinfo [MxGetCpuID ()].bPrefMode = 1;
    syscoreinfo [MxGetCpuID ()].nExceptionLR = addr;
#endif

#ifdef PROC_ABORTS_ENABLED
    SysShdwnProcAbort (ALARM_IDX_PABT);
#endif

    SysShutdownIRQHandler(&reg, 0, NULL);

    while (1) ;
}

UINT32 DataHandlerCheckMemAccess (void)
{
    INTCCTX *pDrvCtx = &GicCtx [MxGetCpuID ()];

    if (pDrvCtx->CtrlMask & IC_CTRL_MASK_CHECK_ADDR)
    {
        // to set a flag operation is failed
        pDrvCtx->CtrlMask |= IC_CTRL_MASK_CHECK_ADDR_FAILED;
        return 1;
    }

    return 0;
}

MXEHT_ void DataHandler(void)
{
    int addr = __return_address() - 8;
    PMXCONTEXT pMx = NULL;
    INTCCTX *pDrvCtx;

    extern VUINT32 just_show;

    uart_printf_print_buf();
    uart_printf_select_dev(PRINT_DEV_UART, 0, 0);
    
    //uart_printf ("DATA abort: addr=%x\r\n", gAbortCtx0.pc);
    //SysShdwnAbortFrmToCLI ();
    //while (pMx == NULL);

#ifdef PROC_ABORTS_ENABLED

    switch (MxGetCpuID ())
    {
        case 0:
            addr = gAbortCtx0.pc;
            break;

        case 1:
            addr = gAbortCtx1.pc;
            break;

        case 2:
            addr = gAbortCtx2.pc;
            break;

        case 3:
            addr = gAbortCtx3.pc;
            break;

        default:
            addr = gAbortCtx0.pc;
            break;
    }

#endif

#ifdef SYS_INFO_ENABLED
    syscoreinfo [MxGetCpuID ()].bDataMode = 1;
    syscoreinfo [MxGetCpuID ()].nExceptionLR = addr;
#endif

    pMx = MxInqContext();

    // This asserts are present in debug compilation ONLY!

    _ASSERT(pMx != NULL);

    // This asserts are present in debug compilation ONLY!

    _ASSERT(HANDLE_NOT_NULL(MxGetDrv(pMx->ICDrvId)));

    pDrvCtx = (INTCCTX *) MxGetDrvCtx(MxGetDrv(pMx->ICDrvId));

    // This asserts are present in debug compilation ONLY!
    _ASSERT(pDrvCtx != NULL);

    if (pDrvCtx->CtrlMask & IC_CTRL_MASK_CHECK_ADDR)
    {
        // to set a flag operation is failed

        pDrvCtx->CtrlMask |= IC_CTRL_MASK_CHECK_ADDR_FAILED;
    }
    else
    {
    	VUINT32 reg = 0;

#ifdef PROC_ABORTS_ENABLED
        SysShdwnProcAbort (ALARM_IDX_DABT);
#endif

        SysShutdownIRQHandler(&reg, 0, NULL);

        while (1) ;
    }
}

MXEHT void ResHandler(void)
{
    UINT32 addr = __return_address() - 8;
    uart_printf ("Res handler:%x\r\n", addr);
    while (1);
}

MXEHT void FiqHandler(void)
{
    PMXCONTEXT pMx;
    UINT32 nIrqID, nIrqMasked, nIrqMasked2, nIrqMasked3;
	UINT32 nCpuID = MxGetCpuID();

    GicCpuRegs * pCpu = (GicCpuRegs *) (GIC_CPU_BASE_ADDR);

    pMx = MxInqContext();

    // This asserts are present in debug compilation ONLY!
    _ASSERT(pMx != NULL);

    while (TRUE)
    {
        /* Try to read stable interrupt ID */
        #ifdef GIC_IRQ_PROTECTION_ENABLED

        	nIrqMasked = ((pCpu->HighPend) & INTC_FAKE_IRQ_MASK);

            if (nIrqMasked == INTC_FAKE_IRQ_ID)
                break;

            nIrqMasked2 = ((pCpu->HighPend) & INTC_FAKE_IRQ_MASK);

            if (nIrqMasked2 != nIrqMasked)
               continue;

            nIrqMasked3 = ((pCpu->HighPend) & INTC_FAKE_IRQ_MASK);

            if (nIrqMasked2 != nIrqMasked || nIrqMasked3 != nIrqMasked)
               continue;

        #endif

        /* Reading the interrupt ID */
        nIrqMasked = ((nIrqID = pCpu->IntAck) & INTC_FAKE_IRQ_MASK);

        if (nIrqMasked == INTC_FAKE_IRQ_ID)
            break;

        _ASSERT(nIrqMasked < MX_MAX_INTERRUPTS);
        _ASSERT_PTR (pMx->vectors[nIrqMasked].Isr);

        #ifdef GIC_IRQ_PROTECTION_ENABLED
            if (nIrqMasked >= MX_MAX_INTERRUPTS)
            {
                if (nIrqMasked < 1024)
                    pCpu->IntEnd = nIrqID;

                continue;
            }

            if (pMx->vectors[nIrqMasked].Isr == NULL)
            {
                pCpu->IntEnd = nIrqID;
                continue;
            }
        #endif

		pMx->vectors[nIrqMasked].Isr(&(pCpu->IntEnd), nIrqID, pMx->vectors[nIrqMasked].Param);

        #ifdef ICDRV_INT_STAT_ENABLED
        pMx->vectors[nIrqMasked].Stat ++;
        #endif
    }
}

ASMPROC void IrqHandlerAsm (void)
{
    PRESERVE8

    IMPORT   IrqHandler
    IMPORT   MxGetArmCpuCtx

    PUSH    {r0-r7,r12,lr}

    PUSH    {r0}
    PUSH    {r1-r3,lr}		    // store them on stack for now
    BL      MxGetArmCpuCtx
    POP     {r1-r3,lr}
    
	STMIA 	r0, {r0-r12}		// Store 1-st dataset r0-r12 to ctx - r0, r1 are wrong for now

	// correct r0 register
	LDR     r1, [sp]		    // Contents of r0 on stack
	STR	    r1, [r0]		    // Store saved r0 to ctx

    SUB     r1, lr, #4
    STR	    r1, [r0, #(4*15)]	// To save PC of terminater mode 

    // the next code is to save 
    // SP, LR and PC of terminated mode 
    // for this purpose we need 
    // to switch ARM mode 

	MRS	    r1, CPSR                // save current CPSR to r1 for mode switching
	MRS	    r2, SPSR                // spsr_irq = CPSR of irq mode: save to r2 for mode switching

    // to switch to the terminated mode
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ORR	    r3, r2, #(0x40|0x80)    // no FIQs/no IRQ
	MSR	    CPSR_c, r3

    // We are in terminated mode 
    // to save SP and LR
    // ~~~~~~~~~~~~~~~~~~~~~~~~~
    STR     SP, [r0, #(4*13)]
    STR     LR, [r0, #(4*14)]

    MRS	    r3, CPSR                // save current CPSR
    STR     r3, [r0, #(4*16)]
	MRS	    r3, SPSR                // save current SPSR
	STR     r3, [r0, #(4*17)]

    // to switch back to the IRQ mode 
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	MSR	    CPSR_c, r1

    POP     {r0}
    BL       IrqHandler

    POP      {r0-r7,r12,lr}
    SUBS     pc,lr,#4
}

/** @brief This handler is designed to be used on U-ARM processor
 * 			to make possible process SPI interrupts
 *
 */

MXEHT_ void IrqHandler(void)
{
    PMXCONTEXT pMx;
    UINT32 nIrqID, nIrqMasked, nIrqMasked2, nIrqMasked3;
	UINT32 nCpuID = MxGetCpuID();

    GicCpuRegs * pCpu = (GicCpuRegs *) (GIC_CPU_BASE_ADDR);

#ifdef PM_ENABLED
    PMStartIrqProc();
#endif

#ifdef SYS_INFO_ENABLED
{
    PARMCPUCONTEXT pArmCtx = MxGetArmCpuCtx();
    int i;

    syscoreinfo [MxGetCpuID ()].bIrqMode = 1;
    syscoreinfo [MxGetCpuID ()].nExceptionLR = __return_address () - 4;

    for (i = 0; i < 16; i++)
       syscoreinfo [MxGetCpuID ()].r[i] = pArmCtx->r[i];

    syscoreinfo [MxGetCpuID ()].cpsr = pArmCtx->cpsr;
    syscoreinfo [MxGetCpuID ()].spsr = pArmCtx->spsr;
}

#endif

    pMx = MxInqContext();

    // This asserts are present in debug compilation ONLY!

    _ASSERT(pMx != NULL);

    SetCpuTraceMode (MLOG_MODE_IRQ);

    while (TRUE)
    {
        /* Try to read stable interrupt ID */

#ifdef GIC_IRQ_PROTECTION_ENABLED

    	nIrqMasked = ((pCpu->HighPend) & INTC_FAKE_IRQ_MASK);

        if (nIrqMasked == INTC_FAKE_IRQ_ID)
            break;

        nIrqMasked2 = ((pCpu->HighPend) & INTC_FAKE_IRQ_MASK);

        if (nIrqMasked2 != nIrqMasked)
           continue;

        nIrqMasked3 = ((pCpu->HighPend) & INTC_FAKE_IRQ_MASK);

        if (nIrqMasked2 != nIrqMasked || nIrqMasked3 != nIrqMasked)
           continue;

#endif

        /* Reading the interrupt ID */

        nIrqMasked = ((nIrqID = pCpu->IntAck) & INTC_FAKE_IRQ_MASK);

#ifdef SYS_INFO_ENABLED
        syscoreinfo [MxGetCpuID ()].nIrqID = nIrqID;
#endif
        if (nIrqMasked == INTC_FAKE_IRQ_ID)
            break;

        _ASSERT(nIrqMasked < MX_MAX_INTERRUPTS);
        _ASSERT_PTR (pMx->vectors[nIrqMasked].Isr);

#ifdef GIC_IRQ_PROTECTION_ENABLED
        if (nIrqMasked >= MX_MAX_INTERRUPTS)
        {
            if (nIrqMasked < 1024)
                pCpu->IntEnd = nIrqID;

            continue;
        }

        if (pMx->vectors[nIrqMasked].Isr == NULL)
        {
            pCpu->IntEnd = nIrqID;
            continue;
        }
#endif

        #ifdef GIC_TRACE_ENABLED
            if (GicIrqTraceEnabled && nCpuID == 0 && GicIrqTraceIndex < GIC_TRACE_SIZE)
            {
                GicIrqTrace [GicIrqTraceIndex].IrqID = nIrqMasked;
                GicIrqTrace [GicIrqTraceIndex].StartTicks = MxGetTicks();
            }
        #endif
        
#ifdef PM_ENABLED
        PMIncIrqProc();
#endif        
#ifdef SYS_INFO_ENABLED
        syscoreinfo [MxGetCpuID ()].isr = (UINT32)pMx->vectors[nIrqMasked].Isr;
#endif
		pMx->vectors[nIrqMasked].Isr(&(pCpu->IntEnd), nIrqID, pMx->vectors[nIrqMasked].Param);

        #ifdef ICDRV_INT_STAT_ENABLED
        pMx->vectors[nIrqMasked].Stat ++;
        #endif

        #ifdef GIC_TRACE_ENABLED
            if (GicIrqTraceEnabled && nCpuID == 0 && GicIrqTraceIndex < GIC_TRACE_SIZE)
            {
                GicIrqTrace [GicIrqTraceIndex++].StopTicks = MxGetTicks();
            }
        #endif
    }

#ifdef SYS_INFO_ENABLED
    syscoreinfo [MxGetCpuID ()].nIrqID   = ~0;
    syscoreinfo [MxGetCpuID ()].bIrqMode = 0;
#endif

    SetCpuTraceMode (MLOG_MODE_SUPERVISOR);

#ifdef PM_ENABLED
    PMEndIrqProc();
#endif
}

MXEHT_ void IrqHandlerARMx2(void)
{
    PMXCONTEXT pMx;
    INTVECTOR * pVect;
    UINT32 nIrqMasked, nIrqMasked2, nIrqMasked3;

#ifdef MLOG_IRQ_SUP_ENABLE
    UINT32 nTrCpuMode = GetCpuTraceMode ();
#endif

    GicCpuRegs * pCpu = (GicCpuRegs *) (GIC_CPU_BASE_ADDR);
    GicDistRegs* pDistr =(GicDistRegs*)(GIC_DISTR_BASE_ADDR);

#ifdef PM_ENABLED
    PMStartIrqProc();
#endif

#ifdef SYS_INFO_ENABLED
    syscoreinfo [MxGetCpuID ()].bIrqMode = 1;
    syscoreinfo [MxGetCpuID ()].nExceptionLR = __return_address () - 4;
#endif

    pMx = MxInqContext();

    // This asserts are present in debug compilation ONLY!
    _ASSERT(pMx != NULL);

    SetCpuTraceMode (MLOG_MODE_IRQ);

    while (TRUE)
    {
        /* Reading the interrupt ID */

    	nIrqMasked = ((pCpu->HighPend) & INTC_FAKE_IRQ_MASK);

#ifdef SYS_INFO_ENABLED
        syscoreinfo [MxGetCpuID ()].nIrqID = nIrqMasked;
#endif

        if (nIrqMasked == INTC_FAKE_IRQ_ID)
            break;

#ifdef GIC_IRQ_PROTECTION_ENABLED
        nIrqMasked2 = ((pCpu->HighPend) & INTC_FAKE_IRQ_MASK);

        if (nIrqMasked2 != nIrqMasked)
           continue;

        nIrqMasked3 = ((pCpu->HighPend) & INTC_FAKE_IRQ_MASK);

        if (nIrqMasked2 != nIrqMasked || nIrqMasked3 != nIrqMasked)
           continue;
#endif

        // to get the IRQ handler
        pVect = &pMx->vectors[nIrqMasked];

        // this code is needed here to process incorrect IRQ IDs returned by GIC/CPU
#ifdef GIC_IRQ_PROTECTION_ENABLED
        if ((pVect->Isr == NULL) || (((UINT32)pVect->Isr & 0x3) != 0) || (nIrqMasked >= MX_MAX_INTERRUPTS))
        {
        	//uart_printf ("GIC/CPU WR %d %d\r\n", nIrqMasked, MX_MAX_INTERRUPTS);
        	//uart_printf ("GIC/CPU ID = %d\r\n", pCpu->HighPend);

        	// to check the range of GIC registers

        	if ((nIrqMasked >> 5) < 32)
        	{
        		pDistr->ClrPend[(nIrqMasked >> 5)] = 1 << (nIrqMasked & 0x1F);
        	}

        	continue;
        }

#else
        _ASSERT(nIrqMasked < MX_MAX_INTERRUPTS);
        _ASSERT_PTR (pVect->Isr);
#endif

        #ifdef PM_ENABLED
        PMIncIrqProc();
        #endif
        
        pVect->Isr(&(pDistr->ClrPend[(nIrqMasked >> 5)]), 1 << (nIrqMasked & 0x1F),	pVect->Param);

        #ifdef ICDRV_INT_STAT_ENABLED
        pMx->vectors[nIrqMasked].Stat ++;
        #endif
    }

    // to restore the previous CPU mode in the log
    SetCpuTraceMode (nTrCpuMode);

#ifdef SYS_INFO_ENABLED
    syscoreinfo [MxGetCpuID ()].nIrqID   = ~0;
    syscoreinfo [MxGetCpuID ()].bIrqMode = 0;
#endif

#ifdef PM_ENABLED
    PMEndIrqProc();
#endif
}

/** @brief This function returns Local CPU ID [0,1,2,3]

    @return [UINT32] */

INLINE UINT32 GicGetLocalCpuID ()
{
    return MxGetARMCpuIDReg() & 0xF;
}

INLINE void GicGetLock (void)
{
    MxGetLock (&GicLowerARM);
}

INLINE void GicReleaseLock (void)
{
    MxReleaseLock (&GicLowerARM);
}


/** @brief This function returns the number of elements in the IRQ trace

    @param nCpu [in] - the CPU id (0..3)

    @return [UINT32] the number of elements */

UINT32 ICDrvGetTraceSize (UINT32 nCpu)
{
    #ifdef GIC_TRACE_ENABLED
        return GicIrqTraceIndex;
    #else
        return 0;
    #endif
}

/** @brief This function returns a pointer to the trace elements for some processor

    @param nCpu  [in] - the CPU id (0..3)
    @param nIndex[in] - the element index, see ICDrvGetTraceSize function

    @return [GIT_IRQ_TRACE_TYPE *] the pointer to the trace element or NULL*/
    
GIT_IRQ_TRACE_TYPE * ICDrvGetTraceElm(UINT32 nCpu, UINT32 nIndex)
{
    #ifdef GIC_TRACE_ENABLED

        if (nIndex >= GicIrqTraceIndex)
            return NULL;
    
        return &(GicIrqTrace[nIndex]);
    #else
        return NULL;
    #endif
}

/** @brief This function enabled IRQ trace, please also see GIC_TRACE_ENABLED macro

    @param bEnb [in] - the flag to enable/disable IRQ trace 

    @return [MXRC] an error code */
    
MXRC ICDrvEnableIrqTrace (UINT32 bEnb)
{
    #ifdef GIC_TRACE_ENABLED
        GicIrqTraceEnabled = bEnb;
    #else
        return RC_INTS_TRACE_IS_DISABLED;
    #endif
}

void ICDrvAckHWIrq (UINT32 irq)
{
    switch (irq)
    {
        case HAL_IRQ_MDMA_RAD:
        case HAL_IRQ_MDMA_SPU:
        case HAL_IRQ_MDMA:
            MdmaAckIrq(irq);
            break;

        case HAL_IRQ_FEC_DL:
        case HAL_IRQ_FEC_UL:
            FecAckIrq(irq);
            break;

        case HAL_IRQ_GEM_0:
        case HAL_IRQ_GEM_1:
            GemDrvAckIrq(irq);
            break;

        case HAL_IRQ_UART_0:
        case HAL_IRQ_UART_1:
        case HAL_IRQ_UART_2:
            UartDrvAckIrq (irq);
            break;
    }

}

void ICDrvShutdown (void)
{
	UINT32 i = 0;
	GicDistRegs* pDistr = (GicDistRegs*)GIC_DISTR_BASE_ADDR;
	GicCpuRegs * pCpu   = (GicCpuRegs *)GIC_CPU_BASE_ADDR;

    if (pDistr->Ctrl != 0)
    {
    	for (i = 0; i < 32; i++)
    	{
    		pDistr->ClrEnable [i] = 0xFFFFFFFF;
    		pDistr->ClrPend [i] = 0xFFFFFFFF;
    	}
    }

	pCpu->Ctrl = 0;
	pDistr->Ctrl = 0;
}

/** @brief This function switches to NS ARM mode and enables GIC:DISRT
           for NS mode, it's needed to enable processing of NS IRQs 
           in Secure mode */
           
ASMPROC void ICDrvEnableNSIrq(void)
{
    push    {r0, r1, r2, r3, lr}

    mrc p15, 0, r3, c12, c0, 0      // to read VBAR register set by 4GMX
    mcr p15, 0, r3, c12, c0, 1      // set MVBAR register to 0

    cps #0x16                       // change processor mode to monitor mode

    // to set NS mode 
    mrc p15, 0, r1, c12, c0,1
    orr r1, r1, #1                  // to set bit#0: NS bit
    mcr p15, 0, r1, c1, c1, 0       // change to non-secure world
    
    cps #0x13                       // change processor mode to supervisor mode in non-secure world

    // to read GIC:DISTR base address
    mrc p15, 4, r0, c15, c0, 0  
    add r0, r0, #0x1000
    mov r1, #1
    str r1, [r0]                    // enable non-secure interrupts

    // to read GIC:DISTR base address
    mrc p15, 4, r0, c15, c0, 0  
    add r0, r0, #0x100
    mov r1, #1
    str r1, [r0]                   // enable non-secure interrupts

    mcr p15, 0, r3, c12, c0, 0      // set non-secure  VBAR to R3 (VBAR of secure mode)
    
    smc #0                          // call monitor exception to switch (to secure) world

    pop {r0, r1, r2, r3, lr}
    
    bx lr

}


/** @brief  This function enables IC distributor and interrupt CPU interfaces and
   also unmasks the IRQ and FIQ interrupts.

 @note The IC_ENABLE_INT_PROCESSING macro should be enabled

 @param pCtx [in] - the pointer to the driver context
 @return [BOOL] */

BOOL ICDrvInits(PINTCCTX pCtx)
{
    MxSetARMIntCtrl(INT_CTRL_IRQ_DISABLED | INT_CTRL_FIQ_DISABLED);

#ifdef IC_ENABLE_INT_PROCESSING

    if (MxGetCpuID () == 0)
    {
        pCtx->Distr->Ctrl = 0;

        #ifdef ICDRV_FIQ_ENABLED
        if (MxGetARMID () == 0)
        {
            ICDrvEnableNSIrq ();
        }
        #endif

        /* Enable Distributor interface */

        #ifdef ICDRV_FIQ_ENABLED
            pCtx->Distr->Ctrl = (MxGetARMID () == 0) ? 3 : 1;
        #else
            pCtx->Distr->Ctrl = 1;
        #endif
    }

    pCtx->Cpu->Ctrl = 0;

    // ==========================================
    
    // BIT[0]: <EnableS> Secure enable for the Cortex-A9 processor interface:
    // 0 = disables the Cortex-A9 processor interface from sending 
    // secure interrupts to the Cortex-A9 processor
    // 1 = enables the Cortex-A9 processor interface to send secure interrupts to the Cortex-A9
    // processor.

    // BIT[1]: <EnableNSb> Non-secure enable for the Cortex-A9 processor interface:
    // 0 = disables the Cortex-A9 processor interface from sending non-secure interrupts to the
    // Cortex-A9 processor
    // 1 = enables the Cortex-A9 processor interface to send non-secure interrupts to the Cortex-A9
    // processor.

    // BIT[2]: <AckCtla> When a Cortex-A9 processor performs a secure read of the int_ack Register and the highest
    // priority interrupt is non-secure, this bit controls the acknowledge response as follows:
    // 0 = The Cortex-A9 processor interface returns an INTID value of 1022 and the interrupt remains
    // Pending.
    // 1 = The Cortex-A9 processor interface returns the INTID value of the non-secure interrupt and
    //acknowledges the interrupt. The interrupt changes state to Active, or Active-and-pending.

    // BIT[3]: <FIQEn> Enables the Cortex-A9 processor interface to send secure interrupts using the nFIQ signal.
    // 0 = the Cortex-A9 processor interface uses nIRQ when signaling secure or non-secure interrupts.
    // 1 = the Cortex-A9 processor interface uses nFIQ when signaling secure interrupts, and nIRQ
    // when signaling non-secure interrupts.

    #ifdef ICDRV_FIQ_ENABLED
        if (MxGetARMID () == 0)
        {
            pCtx->Cpu->Ctrl = ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (0 << 4));
            pCtx->Cpu->Mask = 0xFC;
        }
        else
        {
            pCtx->Cpu->Ctrl = 1;
            pCtx->Cpu->Mask = 0xF0;
        }
    #else
        pCtx->Cpu->Ctrl = 1;
        pCtx->Cpu->Mask = 0xF0;
    #endif

    // ==========================================

    MxSetARMIntCtrl(INT_CTRL_IRQ_ENABLED | INT_CTRL_FIQ_ENABLED);

#endif /*IC_ENABLE_INT_PROCESSING */

    return TRUE;

}

/** @brief The function checks is INT enabled / disabled
    @param nID [in] - the INT id 
    @return [BOOL] enable/disable status */

BOOL ICDrvIsIntEnabled(UINT32 nID)
{
    GicDistRegs * pRegs = (GicDistRegs *)MxGetArmGicDistBaseAddr();
    _ASSERT_PTR(pRegs);
    
    return pRegs->SetEnable [ nID / 32 ] & (1 << (nID % 32));
}

/** @brief This function enumerates enable in the system interrupts 
    @param pEnumId [in/out] - the pointer to enumerator, by initially 0
    @param pInfo   [out]    - the pointer to information structure
    @return [MXRC] an error code */
    
MXRC ICDrvEnumInt (UINT32 * pEnumId, ICDRV_INT_INFO * pInfo)
{
    UINT32 nId, i, cpu_id, target, nIdx, Offs;
    PMXCONTEXT pMx [CPU_NUM];
    GicDistRegs * pRegs = (GicDistRegs *)MxGetArmGicDistBaseAddr();
    UINT32 nCpuNum = SysGetCpuNum ();

    i = 0;
    while (i < nCpuNum)
    {
        pMx[i] = MxInqRemContext(i);
        _ASSERT_PTR (pMx[i]);
        i++;
    }
    
    _ASSERT_PTR (pRegs);
    _ASSERT_PTR (pEnumId);
    _ASSERT_PTR (pInfo);

    nId    = (*pEnumId) & 0xffff;
    cpu_id = (*pEnumId) >> 16;

    // to find the next enabled INT

    while (nId < 1023)
    {
        if (!ICDrvIsIntEnabled (nId))
        {
            nId ++;
            cpu_id = 0;
            continue;
        }

        if (pMx[cpu_id]->vectors[nId].Isr != NULL)
        {
            if (nId > 32)
            {
                target = (pRegs->Target[nId / 4] >> ((nId % 4)*8)) & 0xFF;
                
                if (target & (1 << cpu_id))
                    break;
            }
            else
            {
                break;
            }
        }

        if ((cpu_id + 1) >= nCpuNum)
        {
            nId ++;
            cpu_id = 0;
        }
        else
        {
            cpu_id ++;
        }
    }

    if (!ICDrvIsIntEnabled (nId))
        return RC_INTS_ENUM_STOPPED;

    // to write needed information to the pInfo object 

    pInfo->nIntID     = nId;
    #ifdef ICDRV_FIQ_ENABLED
    pInfo->IntType    = (pRegs->Secur[(nId >> 5)] & (1 << (nId % 32))) ? INT_TYPE_IRQ : INT_TYPE_FIQ;
    #else
    pInfo->IntType    = INT_TYPE_IRQ;
    #endif

    nIdx = (nId / 4);
    Offs = (nId % 4) * 8;

    //uart_printf ("\r\n%d -> %08x (%d %d %x)\r\n", nId, pRegs->Prior [nId/4], nIdx, Offs, (pRegs->Prior [nIdx]>>Offs)&0xFF);
 
    pInfo->nIntPr = (pRegs->Prior [nIdx] >> Offs) & 0xFF;
    pInfo->nIntPr >>= 3;

    if (nId < 32)
    {
        pInfo->IntMode    = INT_TYPE_LEVEL;
        pInfo->CpuMask    = 1 << cpu_id;
    }
    else
    {
        pInfo->IntMode    = (((pRegs->Config [(nId >> 4)] >> ((nId % 16) * 2)) & 3) == 3) ? INT_TYPE_EDGE : INT_TYPE_LEVEL;
        pInfo->CpuMask    = (pRegs->Target[nId / 4] >> ((nId % 4)*8)) & 0xFF;
    }
    pInfo->Handler    = (UINT32)pMx[cpu_id]->vectors[nId].Isr;
    pInfo->HandlerPtr = (UINT32)pMx[cpu_id]->vectors[nId].Param;
    pInfo->Statistics = (UINT32)pMx[cpu_id]->vectors[nId].Stat;

    if ((cpu_id + 1) >= nCpuNum)
    {
        nId ++;
        cpu_id = 0;
    }
    else
    {
        cpu_id ++;
    }

    *pEnumId = nId | (cpu_id << 16);

    return MX_OK;
}

/************************************************************************************************/
/*                       The driver's interface functions                                       */
/************************************************************************************************/

MXRC ICDrvClearIrq (UINT32 nID)
{
    GicDistRegs * pDistr = (GicDistRegs *) GIC_DISTR_BASE_ADDR;

    UINT32 nIdx = (nID >> 5);
    pDistr->ClrEnable[nIdx] = (1 << (nID % 32));

    return MX_OK;
}

MXRC ICDrvClearPendIrq (UINT32 nID)
{
    GicDistRegs * pDistr = (GicDistRegs *) GIC_DISTR_BASE_ADDR;

    UINT32 nIdx = (nID >> 5);
    pDistr->ClrPend[nIdx] = (1 << (nID % 32));

    return MX_OK;
}

BOOL ICDrvIsIrq (UINT32 nID)
{
    GicDistRegs * pDistr = (GicDistRegs *) GIC_DISTR_BASE_ADDR;

    UINT32 nIdx = (nID >> 5);
    return pDistr->SetPend[nIdx] & (1 << (nID % 32));
}

/** @brief This function initializes GIC device

    @param nInit    [in] - the CPU id
    @param pDrvCtx  [out]- the context of driver

    @return [MXRC] the error code */

MXRC ICDrv_init(UINT32 nInit, LPVOID * pDrvCtx)
{
    PINTCCTX pCtx;
    UINT32 i, nIrqID, nIdx;

    ARM_INT_disable();

    //pCtx = (PINTCCTX) MxHeapAlloc(HNULL, sizeof(INTCCTX));
    pCtx = &GicCtx [MxGetCpuID ()];

    if (pCtx == NULL)
        return RC_INTS_DRV_ALLOC_ERROR;

    *pDrvCtx = pCtx;

    if (MxGetCpuID () == 0 && MxGetARMID () == 0)
    {
        REG32(TIMER_IRQ_MASK) = 0;
    }

    pCtx->Distr = (GicDistRegs *) GIC_DISTR_BASE_ADDR;
    pCtx->Cpu = (GicCpuRegs *) GIC_CPU_BASE_ADDR;

    pCtx->nInitOpt = 0;
    pCtx->pOSCtx = MxInqContext();

    // The GIC device should be initialized by the CPU-0
    // the other CPUs will use only CPU interface of the device
    // In case of FPGA architecture, the GIC is initialized by ARMx4core0

    if (nInit == 0)
    {
        GicLowerARM = 0;
    }

    // to clean GIC/CPU queue (if it's ARMx4)

    	if (MxGetARMID () == 0)
    	{
	        while (TRUE)
	        {
	            nIrqID = pCtx->Cpu->IntAck;

	            if ((nIrqID & INTC_FAKE_IRQ_MASK) == INTC_FAKE_IRQ_ID)
	                break;

                ICDrvAckHWIrq (nIrqID);

	            pCtx->Cpu->IntEnd = nIrqID;
	        }
    	}

    // to enable the GIC only for the ARMx2 or for ARMx4 if GIC is disabled
    // Only CORE-0 is responsible to resore GIC/DISTR registers

    //if ((nInit == 0) && (pCtx->Distr->Ctrl == 0) && (MxGetCpuID () == 0))
    if (nInit == 0)
    {
        /* Set level depend for all H/W interrupts */

        for (i = 1; i < sizeof(pCtx->Distr->Config) / sizeof(VUINT32); i++)
        {
            pCtx->Distr->Config[i] = 0x55555555;
        }

        for (i = 0; i < sizeof(pCtx->Distr->ClrEnable) / sizeof(VUINT32); i++)
        {
            pCtx->Distr->ClrEnable[i] = 0xFFFFFFFF;
        }

        for (i = 0; i < 1024; i++)
        {
            nIdx = (i / 4);  /**< 32 bits word contains bits for 4 interrupt signals */
            pCtx->Distr->Target[nIdx] = 0;
        }
     }

#ifdef IC_ENABLE_INT_PROCESSING

    #ifdef MX_MTHREAD_ENABLED
       MxSetAppIrqHandler((MxGetARMID() == 0) ? IrqHandler : IrqHandlerARMx2);
    #endif

    if (MxGetCpuID () == 0)
    {
    	extern MXEHT void ResetHandler (void);

    	MxSetExceptionHandler(EXCEPTION_TYPE_RESET, ResetHandler);
    	MxSetExceptionHandler(EXCEPTION_TYPE_UNDEF, (EXPENTRY)UndefHandler);
        MxSetExceptionHandler(EXCEPTION_TYPE_SWI, (EXPENTRY)SWIHandlerAsm);

#ifdef PROC_ABORTS_ENABLED
        MxSetExceptionHandler(EXCEPTION_TYPE_PREFETCH, PrefHandlerX);
        MxSetExceptionHandler(EXCEPTION_TYPE_DATA, DataHandlerX);
        MxSetExceptionHandler(EXCEPTION_TYPE_UNDEF, UndefHandlerX);
#else
        MxSetExceptionHandler(EXCEPTION_TYPE_PREFETCH, PrefHandler);
        MxSetExceptionHandler(EXCEPTION_TYPE_DATA, DataHandler);
        MxSetExceptionHandler(EXCEPTION_TYPE_UNDEF, UndefHandler);
#endif

        #ifdef MX_MTHREAD_ENABLED
            MxSetExceptionHandler(EXCEPTION_TYPE_IRQ, (EXPENTRY)MxIrqHandler);
        #else
            MxSetExceptionHandler(EXCEPTION_TYPE_IRQ, (MxGetARMID() == 0) ? (EXPENTRY)IrqHandlerAsm : IrqHandlerARMx2);
        #endif
        
        MxSetExceptionHandler(EXCEPTION_TYPE_FIQ, FiqHandler);

        MxSetExceptionHandler(EXCEPTION_TYPE_RESERVED, ResHandler);
        
    }

#endif /*IC_ENABLE_INT_PROCESSING */

    return MX_OK;
}

MXRC ICDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData,
                  UINT32 nCfgDataSize)
{
    PINTCCTX pDrv;

    _ASSERT(pDrvCtx != NULL);

    pDrv = (PINTCCTX) pDrvCtx;

    if (nCfg != INTC_INIT)
       return RC_INTS_DRV_UNSUPP_CMD;

    pDrv->nInitOpt = ((INTC_INIT_PARAM *) pCfgData)->n4GMXInitOpt;

    if (!ICDrvInits(pDrv))
       return RC_INTS_DRV_INIT_ERR;

    return MX_OK;
}

MXRC ICDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                 UINT32 nCmdDataSize)
{
    UINT32 nIdx;
    INTCCTX *pCtx;
    MXRC rc = MX_OK;

    _ASSERT(pDrvCtx != NULL);

    pCtx = (INTCCTX *) pDrvCtx;

    switch (nCmd)
    {
        case INTC_ENABLE_IRQ:
        case INTC_ENABLE_FIQ:
            {
                INTC_INT_CTRL *pCmd = (INTC_INT_CTRL *) pCmdData;

                extern UINT32 set_scr (UINT32 a);

                _ASSERT(pCmd != NULL);

                GIC_DRV_GET_LOCK();

                #ifdef ICDRV_FIQ_ENABLED
                if (MxGetARMID () == 0)
                {
                    if (nCmd == INTC_ENABLE_FIQ)
                    {   
                        // to reset N-Secure bit
                        pCtx->Distr->Secur[(pCmd->ID >> 5)] &= ~(1 << (pCmd->ID % 32));
                    }
                    else
                    {
                        // to set N-Secure bit
                        pCtx->Distr->Secur[(pCmd->ID >> 5)] |= (1 << (pCmd->ID % 32));
                    }
                }
                #endif

                // Interrupt configuration

                nIdx = (pCmd->ID >> 4); // div 16

                if (pCmd->Opt & INT_TYPE_EDGE)
                {
               //evm      pCtx->Distr->Config [nIdx] |= (2 << ((pCmd->ID % 16) * 2));
                    pCtx->Distr->Config [nIdx] |= (3 << ((pCmd->ID % 16) * 2));
                }
                else
                {
                    pCtx->Distr->Config [nIdx] &= ~(2 << ((pCmd->ID % 16) * 2));
                }

                if (pCmd->Opt & INT_TYPE_1N)
                {
                    pCtx->Distr->Config [nIdx] |= (1 << ((pCmd->ID % 16) * 2));
                }
                else
                {
                    pCtx->Distr->Config [nIdx] &= ~(1 << ((pCmd->ID % 16) * 2));
                }

                // Interrupt enabling

                nIdx = (pCmd->ID >> 5);

                pCtx->Distr->SetEnable[nIdx] |= (1 << (pCmd->ID % 32));

                /*Setup target processor */

                nIdx = (pCmd->ID / 4);  /**< 32 bits word contains bits for 4 interrupt signals */

                if (MxGetARMID () == 0)
                {
                	//pCtx->Distr->Target[nIdx] &= ~((0xFF) << (8 * (pCmd->ID % 4)));
                }

                pCtx->Distr->Target[nIdx] |= (1 << CUR_CPU_ID) << (8 * (pCmd->ID % 4));

                GIC_DRV_RELEASE_LOCK ();

                break;
            }

        case INTC_ENABLE_IRQ_EX:
        case INTC_ENABLE_FIQ_EX:
            {
                INTC_INT_CTRL *pCmd = (INTC_INT_CTRL *) pCmdData;

                _ASSERT(pCmd != NULL);

                GIC_DRV_GET_LOCK();

                #ifdef ICDRV_FIQ_ENABLED
                if (MxGetARMID () == 0)
                {
                    if (nCmd == INTC_ENABLE_FIQ_EX)
                    {   
                        // to reset N-Secure bit
                        pCtx->Distr->Secur[(pCmd->ID >> 5)] &= ~(1 << (pCmd->ID % 32));
                    }
                    else
                    {
                        // to set N-Secure bit
                        pCtx->Distr->Secur[(pCmd->ID >> 5)] |= (1 << (pCmd->ID % 32));
                    }
                }
                #endif

                // Interrupt configuration

                nIdx = (pCmd->ID >> 4); // div 16

                if (pCmd->Opt & INT_TYPE_EDGE)
                {
               //evm      pCtx->Distr->Config [nIdx] |= (2 << ((pCmd->ID % 16) * 2));
                    pCtx->Distr->Config [nIdx] |= (3 << ((pCmd->ID % 16) * 2));
                }
                else
                {
                    pCtx->Distr->Config [nIdx] &= ~(2 << ((pCmd->ID % 16) * 2));
                }

                if (pCmd->Opt & INT_TYPE_1N)
                {
                    pCtx->Distr->Config [nIdx] |= (1 << ((pCmd->ID % 16) * 2));
                }
                else
                {
                    pCtx->Distr->Config [nIdx] &= ~(1 << ((pCmd->ID % 16) * 2));
                }

                // Interrupt enabling

                nIdx = (pCmd->ID >> 5);

                pCtx->Distr->SetEnable[nIdx] |= (1 << (pCmd->ID % 32));

                /*Setup target processor */

                nIdx = (pCmd->ID / 4);  /**< 32 bits word contains bits for 4 interrupt signals */

                if (MxGetARMID () == 0)
                {
                	//pCtx->Distr->Target[nIdx] &= ~((0xFF) << (8 * (pCmd->ID % 4)));
                }

                pCtx->Distr->Target[nIdx] |= (1 << pCmd->CpuID) << (8 * (pCmd->ID % 4));

                GIC_DRV_RELEASE_LOCK ();

                break;
            }

        case INTC_DISABLE_IRQ:
        case INTC_DISABLE_FIQ:
            {
                INTC_INT_CTRL *pCmd = (INTC_INT_CTRL *) pCmdData;

                _ASSERT(pCmd != NULL);

                GIC_DRV_GET_LOCK ();

                nIdx = (pCmd->ID >> 5);
                
                // to disable IRQ in GIC for this core
                pCtx->Distr->ClrEnable[nIdx] = (1 << (pCmd->ID % 32));

                // to clear IRQ signal if any
                pCtx->Distr->ClrPend[nIdx] = (1 << (pCmd->ID % 32));

                // to exclude this core from the

                nIdx = (pCmd->ID / 4);  /**< 32 bits word contains bits for 4 interrupt signals */
                pCtx->Distr->Target[nIdx] &= ~((1 << CUR_CPU_ID) << (8 * (pCmd->ID % 4)));

                GIC_DRV_RELEASE_LOCK ();

                break;
            }

        case INTC_DISABLE_IRQ_EX:
        case INTC_DISABLE_FIQ_EX:
            {
                INTC_INT_CTRL *pCmd = (INTC_INT_CTRL *) pCmdData;

                _ASSERT(pCmd != NULL);

                GIC_DRV_GET_LOCK ();

                nIdx = (pCmd->ID >> 5);

                pCtx->Distr->ClrEnable[nIdx] = (1 << (pCmd->ID % 32));

                // to exclude this core from the

                nIdx = (pCmd->ID / 4);  /**< 32 bits word contains bits for 4 interrupt signals */
                pCtx->Distr->Target[nIdx] &= ~((1 << pCmd->CpuID) << (8 * (pCmd->ID % 4)));

                GIC_DRV_RELEASE_LOCK ();

                break;
            }

        case INTC_SET_IRQ_PRIORITY:
        case INTC_SET_IRQ_PRIORITY_EX:
        case INTC_SET_FIQ_PRIORITY:
        case INTC_SET_FIQ_PRIORITY_EX:
        	{
                INTC_PRIOR *pCmd = (INTC_PRIOR *) pCmdData;
                UINT32 Offs;

        	    nIdx = (pCmd->ID / 4);
                Offs = (pCmd->ID % 4) * 8;

                pCmd->Prior <<= 3;

                pCtx->Distr->Prior [nIdx] &= ~(0xFF << Offs);
                pCtx->Distr->Prior [nIdx] |= (pCmd->Prior << Offs);
                
        	    break;
        	}

        case INTC_RESET_IRQ_PRIORITY:
        case INTC_RESET_FIQ_PRIORITY:
            {
#ifdef _MX_DEBUG
                INTC_PRIOR *pParam = (INTC_PRIOR *) pCmdData;

                _ASSERT(pParam != NULL);
#endif
                GIC_DRV_GET_LOCK ();

                GIC_DRV_RELEASE_LOCK ();

                break;
            }

        case INTC_RAISE_SOFT_INT:
            {
                if (((INTC_RAISE_SOFTINT *) pCmdData)->IntId >= HAL_IRQ_INTER_ARM_0)
                {
                    REG32 (CFG_SYS_ARM_IRQ_SET) = 1 << (((INTC_RAISE_SOFTINT *) pCmdData)->IntId - HAL_IRQ_INTER_ARM_0);
                }
                else
                {
                    INTC_RAISE_SOFTINT * pCmd = (INTC_RAISE_SOFTINT *)pCmdData;
                    UINT32 secure_flag;

                    #ifdef ICDRV_FIQ_ENABLED
                    if (MxGetARMID () == 0)
                    {
                        secure_flag = (pCmd->CpuList & GIC_FIQ_TYPE) ? (0) : (1 << 15);
                    }
                    else
                    {
                        secure_flag = 0;
                    }
                    #else
                    secure_flag = 0;
                    #endif
                    pCmd->CpuList &= 0xFF;

                    if (pCmd->IntId != SOFT_ICPU_TO_UARM)
                    {
                        // to convert logical core id to physical core id
						rc = MxGetRemoteCoreID (pCmd->CpuList, &pCmd->CpuList);

                        // to keep core id only, not cluster ID
                        pCmd->CpuList &= 0xF;

                        _ASSERT_RC(rc);
                    }

                    // Send interrupt to the CPUs specified in the target list
                    pCtx->Distr->SoftGenInt = ((((1 << pCmd->CpuList) & 0xFF) << 16) | (pCmd->IntId)) | secure_flag;

                    //GIC_DRV_RELEASE_LOCK ();
                }

                break;
            }

        case INTC_CLEAR_SOFT_INT:
            {
            	UINT32 nVal = ((INTC_RAISE_SOFTINT *) pCmdData)->IntId;

                if (nVal >= HAL_IRQ_INTER_ARM_0)
                {
                    REG32 (CFG_SYS_ARM_IRQ_CLR) = (1 << (nVal - HAL_IRQ_INTER_ARM_0));
                }

                break;
            }

        case INTC_SAFE_WRITE_32BITS:
            {
                INTC_SAFE_RW * pParam = (INTC_SAFE_RW *)pCmdData;

                _ASSERT_PTR(pParam);

                pCtx->CtrlMask &= ~IC_CTRL_MASK_CHECK_ADDR_FAILED;
                pCtx->CtrlMask |= IC_CTRL_MASK_CHECK_ADDR;

                // To try write 32 bits value

                *(UINT32*)(pParam->Address & (~0x3)) = pParam->Value;

                if (pCtx->CtrlMask & IC_CTRL_MASK_CHECK_ADDR_FAILED)
                {
                    rc = RC_INTS_SAFE_WRITE_FAILED;
                }
                else
                {
                    rc = MX_OK;
                }

                // To clean control bits

                pCtx->CtrlMask &= ~(IC_CTRL_MASK_CHECK_ADDR_FAILED | IC_CTRL_MASK_CHECK_ADDR);

                break;
            }


        default:
            return RC_INTS_DRV_UNSUPP_CMD;
    }

    return rc;
}

MXRC ICDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                UINT32 * pnCmdDataSize)
{
    UINT32 nIdx;
    INTCCTX *pCtx;
    MXRC rc = MX_OK;

    _ASSERT(pDrvCtx != NULL);

    pCtx = (INTCCTX *) pDrvCtx;

    switch (nCmd)
    {
        case INTC_IS_FIQ_ENABLED:
        case INTC_IS_IRQ_ENABLED:
            {
                INTC_IS_ENABLED *pParam = (INTC_IS_ENABLED *) pCmdData;

                _ASSERT(pParam != NULL);

                nIdx = (pParam->ID_Status >> 5);

                pParam->ID_Status =
                    (pCtx->Distr->SetEnable[nIdx] >> (pParam->ID_Status % 32)) & 1;

                break;
            }

        case INTC_SAFE_READ_32BITS:
            {
                INTC_SAFE_RW * pParam = (INTC_SAFE_RW *)pCmdData;

                _ASSERT_PTR(pParam);

                pCtx->CtrlMask &= ~IC_CTRL_MASK_CHECK_ADDR_FAILED;
                pCtx->CtrlMask |= IC_CTRL_MASK_CHECK_ADDR;

                // To try read 32 bits value

                pParam->Value = *(UINT32*)(pParam->Address & (~0x3));

                if (pCtx->CtrlMask & IC_CTRL_MASK_CHECK_ADDR_FAILED)
                {
                    rc = RC_INTS_SAFE_READ_FAILED;
                }
                else
                {
                    rc = MX_OK;
                }

                // To clean control bits

                pCtx->CtrlMask &= ~(IC_CTRL_MASK_CHECK_ADDR_FAILED | IC_CTRL_MASK_CHECK_ADDR);

                break;
            }

        default:
            return RC_INTS_DRV_UNSUPP_CMD;
    }

    return rc;
}
