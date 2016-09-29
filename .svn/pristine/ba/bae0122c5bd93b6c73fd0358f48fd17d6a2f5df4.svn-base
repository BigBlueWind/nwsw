//-------------------------------------------------------------------------------------------
/** @file 4gmx_arm.c
 *
 * @brief The ARM specific API implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.14 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "4gmx_config.h"

#ifdef MX_ARM_ENABLED

#include "4gmx_arm.h"
#include "4gmx_serv.h"
#include "4gmx_const.h"


/** @brief The function is called by the sub-layer exception handlers
           and returns address of real (set by the application) exception handler.

    @param nVectID [in] - the ID of exception vection [0 .. 6]

    @return [EXPENTRY] an address of exception handler
    \ingroup group_lte_4gmx_kernel

    */

EXPENTRY MxARMGetExceptHandler (UINT32 nVectID)
{
    PMXCONTEXT pCtx;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    _ASSERT(nVectID < EXCEPTION_TYPE_NUM);

    _ASSERT(pCtx->excepts [nVectID] != NULL);

    return pCtx->excepts [nVectID];
}

/** @brief  This function is used like a sub-layer of
            software stack checking in privilege (exception) modes.

    @note  The RVDS uses SL(r10) register like a stack limit,
           so, we need to set r10 in privilege modes according to the
           size of stack in specified mode.

           The stack size is stored in [SP-4], see MxSetArmCpuStack function

           \ingroup group_lte_4gmx_kernel

           */

ASMPROC  IRQPROC  void SWSTReset (void)
{
    IMPORT   MxARMGetExceptHandler

    PRESERVE8

    SUB      lr, lr, #4
    STMDB    sp!,{lr}
    STMDB    sp!,{r0, r1, r2, r3, sl}

    // To get the limit of the stack

    LDR      SL, [SP, #+24]

    // To get the address of the EXCEPTION_TYPE_RESET
    MOV      R0, #0
    BL       MxARMGetExceptHandler

    BLX      R0

    LDMIA    sp!,{r0, r1, r2, r3, sl}
    LDMIA    sp!, {pc}^
}

/** @brief  This function is used like a sub-layer of
            software stack checking in privilege (exception) modes.

    @note  The RVDS uses SL(r10) register like a stack limit,
           so, we need to set r10 in privilege modes according to the
           size of stack in specified mode.

           The stack size is stored in [SP-4], see MxSetArmCpuStack function

           \ingroup group_lte_4gmx_kernel

           */

ASMPROC  IRQPROC  void SWSTUndef (void)
{
    IMPORT   MxARMGetExceptHandler

    PRESERVE8

    SUB      lr, lr, #4
    STMDB    sp!,{lr}
    STMDB    sp!,{r0, r1, r2, r3, sl}

    // To get the limit of the stack

    LDR      SL, [SP, #+24]

    // To get the address of the EXCEPTION_TYPE_UNDEF
    MOV      R0, #1
    BL       MxARMGetExceptHandler

    BLX      R0

    LDMIA    sp!,{r0, r1, r2, r3, sl}
    LDMIA    sp!, {pc}^
}

/** @brief  This function is used like a sub-layer of
            software stack checking in privilege (exception) modes.

    @note  The RVDS uses SL(r10) register like a stack limit,
           so, we need to set r10 in privilege modes according to the
           size of stack in specified mode.

           The stack size is stored in [SP-4], see MxSetArmCpuStack function

           \ingroup group_lte_4gmx_kernel

           */

ASMPROC  IRQPROC  void SWSTSWI (void)
{
    IMPORT   MxARMGetExceptHandler

    PRESERVE8

    SUB      lr, lr, #4
    STMDB    sp!,{lr}
    STMDB    sp!,{r0, r1, r2, r3, sl}

    // To get the limit of the stack

    LDR      SL, [SP, #+24]

    // To get the address of the EXCEPTION_TYPE_SWI
    MOV      R0, #2
    BL       MxARMGetExceptHandler

    BLX      R0

    LDMIA    sp!,{r0, r1, r2, r3, sl}
    LDMIA    sp!, {pc}^
}

/** @brief  This function is used like a sub-layer of
            software stack checking in privilege (exception) modes.

    @note  The RVDS uses SL(r10) register like a stack limit,
           so, we need to set r10 in privilege modes according to the
           size of stack in specified mode.

           The stack size is stored in [SP-4], see MxSetArmCpuStack function

           \ingroup group_lte_4gmx_kernel

           */

ASMPROC  IRQPROC  void SWSTPREFETCH (void)
{
    IMPORT   MxARMGetExceptHandler

    PRESERVE8

    SUB      lr, lr, #4
    STMDB    sp!,{lr}
    STMDB    sp!,{r0, r1, r2, r3, sl}

    // To get the limit of the stack

    LDR      SL, [SP, #+24]

    // To get the address of the EXCEPTION_TYPE_PREFETCH
    MOV      R0, #3
    BL       MxARMGetExceptHandler

    BLX      R0

    LDMIA    sp!,{r0, r1, r2, r3, sl}
    LDMIA    sp!, {pc}^
}

/** @brief  This function is used like a sub-layer of
            software stack checking in privilege (exception) modes.

    @note  The RVDS uses SL(r10) register like a stack limit,
           so, we need to set r10 in privilege modes according to the
           size of stack in specified mode.

           The stack size is stored in [SP-4], see MxSetArmCpuStack function

           \ingroup group_lte_4gmx_kernel

           */

ASMPROC  IRQPROC  void SWSTData (void)
{
    IMPORT   MxARMGetExceptHandler

    PRESERVE8

    SUB      lr, lr, #4
    STMDB    sp!,{lr}
    STMDB    sp!,{r0, r1, r2, r3, sl}

    // To get the limit of the stack

    LDR      SL, [SP, #+24]

    // To get the address of the EXCEPTION_TYPE_DATA
    MOV      R0, #4
    BL       MxARMGetExceptHandler

    BLX      R0

    LDMIA    sp!,{r0, r1, r2, r3, sl}
    LDMIA    sp!, {pc}^
}

/** @brief  This function is used like a sub-layer of
            software stack checking in privilege (exception) modes.

    @note  The RVDS uses SL(r10) register like a stack limit,
           so, we need to set r10 in privilege modes according to the
           size of stack in specified mode.

           The stack size is stored in [SP-4], see MxSetArmCpuStack function

           \ingroup group_lte_4gmx_kernel

           */

ASMPROC  IRQPROC  void SWSTIrq (void)
{
    IMPORT   MxARMGetExceptHandler

    PRESERVE8

    SUB      lr, lr, #4
    STMDB    sp!,{lr}
    STMDB    sp!,{r0, r1, r2, r3, sl}

    // To get the limit of the stack

    LDR      SL, [SP, #+24]

    // To get the address of the EXCEPTION_TYPE_IRQ
    MOV      R0, #6
    BL       MxARMGetExceptHandler

    BLX      R0

    LDMIA    sp!,{r0, r1, r2, r3, sl}
    LDMIA    sp!, {pc}^
}

/** @brief  This function is used like a sub-layer of
            software stack checking in privilege (exception) modes.

    @note  The RVDS uses SL(r10) register like a stack limit,
           so, we need to set r10 in privilege modes according to the
           size of stack in specified mode.

           The stack size is stored in [SP-4], see MxSetArmCpuStack function

           \ingroup group_lte_4gmx_kernel

           */

ASMPROC  IRQPROC  void SWSTFiq (void)
{
    IMPORT   MxARMGetExceptHandler

    PRESERVE8

    SUB      lr, lr, #4
    STMDB    sp!,{lr}
    STMDB    sp!,{r0, r1, r2, r3, sl}

    // To get the limit of the stack

    LDR      SL, [SP, #+24]

    // To get the address of the EXCEPTION_TYPE_FIQ
    MOV      R0, #7
    BL       MxARMGetExceptHandler

    BLX      R0

    LDMIA    sp!,{r0, r1, r2, r3, sl}
    LDMIA    sp!, {pc}^
}

void MxARMInitHWSemas (PMXCONTEXT pCtx)
{
    if (MxGetCpuID () == 0)
    {
        #ifdef MX_L1_HW_SYNC_ENABLED
        MxiInitHwSema (pCtx, MX_HW_SEMA_L1_SYNC);
        #endif

        #ifdef MX_L2_HW_SYNC_ENABLED
        MxiInitHwSema (pCtx, MX_HW_SEMA_L2_SYNC);
        #endif
    }

}

/** @brief The function is called by MxInit function and initializes
   ARM sub-system API.

 @param pCtx [in] - the pointer to the system context
 @param nOpt [in] - the configuration parameters

 @return [MXRC] error code of initialization

 \ingroup group_lte_4gmx_kernel

 */

MXRC MxARMSubSystemInit(PMXCONTEXT pCtx, UINT32 nOpt)
{
    MXRC rc = MX_OK;
    UINT32 nCp15;

    _ASSERT(pCtx != NULL);

    MxARMInitHWSemas(pCtx);

#ifdef MX_ARM_ERRATA_FIX_ENABLED
    MxiARMErrata (0);
#endif

#ifdef MX_ARM_MMU_ENABLED

    if (FAILED(rc = MxMmuSubSysInit(pCtx, nOpt)))
        return rc;

#endif

    nCp15 = sys_read_control();

    if ((nOpt & SYS_INIT_LOW_VECTORS))
    {
        nCp15 &= ~CR_V;
    }
    else
    {
        nCp15 |= CR_V;
    }

    nCp15 |= CR_AFE;

    sys_write_control(nCp15);

    if ((nOpt & SYS_INIT_LOW_VECTORS )!= 0)
    {
        MxSetVB (pCtx->lvecaddr);
    }

    return rc;
}

MXRC MxARMConfigIntc(PMXCONTEXT pCtx, UINT32 nOpt)
{
#ifdef MX_DRV_FRAMEWORK_ENABLED

    HANDLE hDrv;
    INTC_INIT_PARAM param;

    _ASSERT(pCtx != NULL);

    if (pCtx->ICDrvId == 0xFFFFFFFF || pCtx->Drivers == NULL)
        return MX_OK;

    hDrv = MxGetDrv(pCtx->ICDrvId);

    _ASSERT(HANDLE_NOT_NULL(hDrv));

    param.n4GMXInitOpt = nOpt;

    return MxConfigDrv(hDrv, INTC_INIT, &param, sizeof(param));

#else

    return MX_OK;

#endif /*MX_DRV_FRAMEWORK_ENABLED */
}

/** @brief The function returns ID of ARM CPU mode

    @param mode [in] - the ARM mode (see: CPUMODE type)

    @return [CPUMODEID] an ID of ARM CPU mode

    \ingroup group_lte_4gmx_kernel

    */

CPUMODEID MxGetArmModeID (CPUMODE mode)
{
    switch (mode)
    {
        case CPU_MODE_USER:
            return CPU_MODE_ID_USER;

        case CPU_MODE_FIQ:
            return CPU_MODE_ID_FIQ;

        case CPU_MODE_IRQ:
            return CPU_MODE_ID_IRQ;

        case CPU_MODE_SUPERVISOR:
            return CPU_MODE_ID_SUPERVISOR;

        case CPU_MODE_ABOART:
            return CPU_MODE_ID_ABOART;

        case CPU_MODE_UNDEF:
            return CPU_MODE_ID_UNDEF;

        case CPU_MODE_SYSTEM:
            return CPU_MODE_ID_SYSTEM;

        default:
            break;
    }

#ifdef _MX_DEBUG
    _ASSERT(0);
#else
    return CPU_MODE_ID_USER;
#endif
}

/** @brief The function returns value of ARM CPU mode

    @param modeid [in] - the ID of ARM mode (see: CPUMODEID type)

    @return [CPUMODE] the value of ARM CPU mode

    \ingroup group_lte_4gmx_kernel

    */

CPUMODE MxGetArmMode (CPUMODEID modeid)
{
    switch (modeid)
    {
        case CPU_MODE_ID_USER:
            return CPU_MODE_USER;

        case CPU_MODE_ID_FIQ:
            return CPU_MODE_FIQ;

        case CPU_MODE_ID_IRQ:
            return CPU_MODE_IRQ;

        case CPU_MODE_ID_SUPERVISOR:
            return CPU_MODE_SUPERVISOR;

        case CPU_MODE_ID_ABOART:
            return CPU_MODE_ABOART;

        case CPU_MODE_ID_UNDEF:
            return CPU_MODE_UNDEF;

        case CPU_MODE_ID_SYSTEM:
            return CPU_MODE_SYSTEM;

        default:
            break;
    }

#ifdef _MX_DEBUG
        _ASSERT(0);
#else
        return CPU_MODE_USER;
#endif
}

/** @brief The function set STACK POINTER for specified ARM CPU mode.

    @note  The function should be used from the privilege modes ONLY.
           Also function restores current processor mode before return.

    @param cpumode [in] - the value of CPU mode
    @param pStack  [in] - the pointer to the stack for specified mode
    @param nSize   [in] - the size of stack in bytes

    @return [none]

    \ingroup group_lte_4gmx_kernel

    */

ASMPROC void MxSetArmCpuStack(CPUMODE cpumode, LPVOID pStack, UINT32 nSize)
{
    // r0 = cpumode
    // r1 = pStack
    // r2 = nSize

    STMDB sp!, {r1, r3, r4, lr}

    /* getting current processor mode */
    mrs r3, cpsr
    mov r4, r3

    /* changing mode */
    bic r3, r3,#0x1F
    orr r3, r3, r0
    msr cpsr_c, r3

    /* setting stack pointer for selected mode */
    mov sp, r1

#ifdef __APCS_SWST

    /* The stack limit will be stored in the stack of
       specified CPU mode and be restored by the exception handler
       every time when processor changes mode  */

    SUB r1, r1, r2
    ADD r1, r1, #284    // 256 bytes required for RVDS and + 4 bytes of SL
                        // + 24 bytes of SWSTxxx functions

    STMDB sp!, {r1}
#endif

    /* switching back to the prev. mode */
    msr cpsr_c, r4

    /* restoring registers' value */
    LDMIA sp!, {r1, r3, r4}
    LDMIA sp !,{pc}
}


/** @brief  The function sets SP register for selected processor mode and
            switches back to the previous CPU mode.

   @note The CPU_MODE_USER mode should be not used.

 @param cpumode [in] - the id of CPU mode
 @param pStack  [in] - the pointer to the head of stack (the stack is descending thing)
 @param nSize   [in] - the size of stack in bytes


\ingroup group_lte_4gmx_kernel

*/

void MxSetCpuStack(CPUMODE cpumode, LPVOID pStack, UINT32 nSize)
{
    _ASSERT(cpumode != CPU_MODE_USER);
    _ASSERT(pStack  != NULL);

    /* In case of stack checking, the RVDS requires the size of stack
       should be more that 256 bytes */

#ifdef __APCS_SWST

     // 256 - for RVDS framework
     // 4   - for the SL (for the exception mode)
     // 16  - for the lr, r0, r1, r2, r3, sl

    _ASSERT(nSize >= 256 + 4 + (6 * 4) );
#endif

    // Setting the stack pointer for specified ARM mode

    MxSetArmCpuStack (cpumode, pStack, nSize);

}

/************************************************************************/
/*      Interrupt control API                                           */
/************************************************************************/

MXRC MxSetExceptionHandler(ETYPE type, EXPENTRY proc)
{
    return MxSetExceptionHandlerCpu (MxGetCpuID(), type, proc);
}

MXRC MxSetExceptionHandlerCpu(CPUID id, ETYPE type, EXPENTRY proc)
{
    UINT32 Mem, ProcAddr;

    PMXCONTEXT pCtx;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    if ((sys_read_control() & CR_V) != 0)
    {
        Mem = 0xFFFF0000;
    }
    else
    {
        Mem = MxGetVB ();
    }

    _ASSERT(((UINT32)type >> 2) < EXCEPTION_TYPE_NUM);

    pCtx->excepts [(UINT32)type >> 2] = proc;

    ProcAddr = (UINT32)proc;

#if defined(__CC_ARM) && defined (__APCS_SWST)

    // If software stack checking is used, the
    // sub-layer exception handlers should be used
    // for setting/restoring <SL> register

    switch (type)
    {
        case EXCEPTION_TYPE_RESET:
            {
                ProcAddr = (UINT32)SWSTReset;
                break;
            }

        case EXCEPTION_TYPE_UNDEF:
            {
                ProcAddr = (UINT32)SWSTUndef;
                break;
            }

        case EXCEPTION_TYPE_SWI:
            {
                ProcAddr = (UINT32)SWSTSWI;
                break;
            }

        case EXCEPTION_TYPE_PREFETCH:
            {
                ProcAddr = (UINT32)SWSTPREFETCH;
                break;
            }

        case EXCEPTION_TYPE_DATA:
            {
                ProcAddr = (UINT32)SWSTData;
                break;
            }
        case EXCEPTION_TYPE_IRQ:
            {
                ProcAddr = (UINT32)SWSTIrq;
                break;
            }

        case EXCEPTION_TYPE_FIQ:
            {
                ProcAddr = (UINT32)SWSTFiq;
                break;
            }
    }

#endif /*defined(__CC_ARM) && defined (__APCS_SWST)*/

    *(PUINT32)(Mem + (UINT32) type + 0x20) = ProcAddr;
    *(PUINT32)(Mem + (UINT32) type) = 0xE59FF018;

    return MX_OK;
}

MXRC MxSetIntHandler(UINT32 nIntID, ITYPE opt, TPRIORITY priority,
                     TISRENTRY isr, LPVOID pParam)
{
    IRQTYPE irq;
    PMXCONTEXT pCtx;

    _ASSERT(nIntID < MX_MAX_INTERRUPTS);

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    MxiDisInt(&irq);

    pCtx->vectors[nIntID].Isr = isr;
    pCtx->vectors[nIntID].Param = pParam;
    pCtx->vectors[nIntID].Opt = opt;

    if (isr == NULL)
    {
        MxDisableInt(nIntID);
        MxSetIntPriority(nIntID, priority);
    }
    else
    {
        MxSetIntPriority(nIntID, priority);
    }

    MxiRestInt(irq);

    return MX_OK;
}

MXRC MxSetIntHandlerEx(UINT32 nCpuID, UINT32 nIntID, ITYPE opt, TPRIORITY priority,
                     TISRENTRY isr, LPVOID pParam)
{
    IRQTYPE irq;
    PMXCONTEXT pCtx;

    _ASSERT(nIntID < MX_MAX_INTERRUPTS);

    pCtx = MxInqRemContext(nCpuID);

    _ASSERT(pCtx != NULL);

    if (pCtx == NULL)
        return MX_ARM_CPU_ID_ERROR;

    MxiDisInt(&irq);

    pCtx->vectors[nIntID].Isr = isr;
    pCtx->vectors[nIntID].Param = pParam;
    pCtx->vectors[nIntID].Opt = opt;

    if (isr == NULL)
    {
        MxDisableIntEx(nCpuID, nIntID);
        MxSetIntPriorityEx(nCpuID, nIntID, priority);
    }
    else
    {
        MxSetIntPriorityEx(nCpuID, nIntID, priority);
    }

    MxiRestInt(irq);

    return MX_OK;
}

MXRC MxSetIntPriority(UINT32 nIntID, TPRIORITY priority)
{
#ifdef MX_DRV_FRAMEWORK_ENABLED

    PMXCONTEXT pCtx;

    HANDLE hDrv;

    INTC_PRIOR prior;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    if (pCtx->vectors[nIntID].Isr == NULL)
        return MX_ARM_ISR_NOT_SET;

    if (pCtx->ICDrvId == 0xFFFFFFFF)
        return MX_ARM_UNSUPP_OPR;

    hDrv = MxGetDrv(pCtx->ICDrvId);

    _ASSERT(HANDLE_NOT_NULL(hDrv));

    prior.ID = nIntID;
    prior.Prior = priority;

    return MxWriteDrvData(hDrv,
                          (pCtx->vectors[nIntID].Opt & INT_TYPE_IRQ) ?
                          (INTC_SET_IRQ_PRIORITY):
                          (INTC_SET_FIQ_PRIORITY),
                          &prior,
                          sizeof(prior));

#else /*#ifdef MX_DRV_FRAMEWORK_ENABLED */

    return MX_ARM_UNSUPP_OPR;

#endif
}

MXRC MxSetIntPriorityEx(UINT32 nCpuID,UINT32 nIntID, TPRIORITY priority)
{
#ifdef MX_DRV_FRAMEWORK_ENABLED

    PMXCONTEXT pCtx;

    HANDLE hDrv;

    INTC_PRIOR prior;

    pCtx = MxInqRemContext(nCpuID);

    _ASSERT(pCtx != NULL);

    if (pCtx->vectors[nIntID].Isr == NULL)
        return MX_ARM_ISR_NOT_SET;

    if (pCtx->ICDrvId == 0xFFFFFFFF)
        return MX_ARM_UNSUPP_OPR;

    hDrv = MxGetDrv(pCtx->ICDrvId);

    _ASSERT(HANDLE_NOT_NULL(hDrv));

    prior.ID     = nIntID;
    prior.Prior  = priority;
    prior.nCpuID = nCpuID;

    return MxWriteDrvData(hDrv,
                          (pCtx->vectors[nIntID].Opt & INT_TYPE_IRQ) ?
                          (INTC_SET_IRQ_PRIORITY_EX):
                          (INTC_SET_FIQ_PRIORITY_EX),
                          &prior,
                          sizeof(prior));

#else /*#ifdef MX_DRV_FRAMEWORK_ENABLED */

    return MX_ARM_UNSUPP_OPR;

#endif
}


MXRC MxSetIntHandlerParam(UINT32 nIntID, LPVOID pData)
{
    PMXCONTEXT pCtx;
    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    if (pCtx->vectors[nIntID].Isr == NULL)
        return MX_ARM_ISR_NOT_SET;

    pCtx->vectors[nIntID].Param = pData;

    return MX_OK;
}

MXRC MxEnableInt(UINT32 nIntID)
{
#ifdef MX_DRV_FRAMEWORK_ENABLED

    PMXCONTEXT pCtx;
    IRQTYPE irq;
    HANDLE hDrv;
    INTC_INT_CTRL ctrl;
    MXRC rc;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    if (pCtx->ICDrvId == 0xFFFFFFFF)
        return MX_ARM_UNSUPP_OPR;

    hDrv = MxGetDrv(pCtx->ICDrvId);

    _ASSERT(HANDLE_NOT_NULL(hDrv));

    MxiDisInt(&irq);

    if (pCtx->vectors[nIntID].Isr == NULL)
    {
        MxiRestInt(irq);
        return MX_ARM_ISR_ERROR;
    }

    ctrl.ID    = nIntID;
    ctrl.Opt   = pCtx->vectors[nIntID].Opt;
    ctrl.CpuID = MxGetCpuID ();

    if (pCtx->vectors[nIntID].Opt & INT_TYPE_FIQ)
    {
        rc = MxWriteDrvData(hDrv, INTC_ENABLE_FIQ, &ctrl, sizeof(ctrl));
    }
    else
    {
        rc = MxWriteDrvData(hDrv, INTC_ENABLE_IRQ, &ctrl, sizeof(ctrl));
    }

    MxiRestInt(irq);

    return rc;

#else

    return MX_ARM_UNSUPP_OPR;

#endif /*MX_DRV_FRAMEWORK_ENABLED */
}

MXRC MxEnableIntEx(UINT32 nCpuID, UINT32 nIntID)
{
#ifdef MX_DRV_FRAMEWORK_ENABLED

    PMXCONTEXT pCtx;
    IRQTYPE irq;
    HANDLE hDrv;
    INTC_INT_CTRL ctrl;
    MXRC rc;

    pCtx = MxInqRemContext(nCpuID);

    _ASSERT(pCtx != NULL);

    if (pCtx->ICDrvId == 0xFFFFFFFF)
        return MX_ARM_UNSUPP_OPR;

    hDrv = MxGetDrv(pCtx->ICDrvId);

    _ASSERT(HANDLE_NOT_NULL(hDrv));

    MxiDisInt(&irq);

    if (pCtx->vectors[nIntID].Isr == NULL)
    {
        MxiRestInt(irq);
        return MX_ARM_ISR_ERROR;
    }

    ctrl.ID    = nIntID;
    ctrl.Opt   = pCtx->vectors[nIntID].Opt;
    ctrl.CpuID = nCpuID;

    if (pCtx->vectors[nIntID].Opt & INT_TYPE_FIQ)
    {
        rc = MxWriteDrvData(hDrv, INTC_ENABLE_FIQ_EX, &ctrl, sizeof(ctrl));
    }
    else
    {
        rc = MxWriteDrvData(hDrv, INTC_ENABLE_IRQ_EX, &ctrl, sizeof(ctrl));
    }

    MxiRestInt(irq);

    return rc;

#else

    return MX_ARM_UNSUPP_OPR;

#endif /*MX_DRV_FRAMEWORK_ENABLED */
}


MXRC MxDisableInt(UINT32 nIntID)
{
#ifdef MX_DRV_FRAMEWORK_ENABLED

    PMXCONTEXT pCtx;
    IRQTYPE irq;
    HANDLE hDrv;
    INTC_INT_CTRL ctrl;
    MXRC rc;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    if (pCtx->ICDrvId == 0xFFFFFFFF)
        return MX_ARM_UNSUPP_OPR;

    hDrv = MxGetDrv(pCtx->ICDrvId);

    _ASSERT(HANDLE_NOT_NULL(hDrv));

    MxiDisInt(&irq);

    ctrl.ID    = nIntID;
    ctrl.Opt   = pCtx->vectors[nIntID].Opt;
    ctrl.CpuID = MxGetCpuID ();

    if (pCtx->vectors[nIntID].Opt & INT_TYPE_IRQ)
    {
        rc = MxWriteDrvData(hDrv, INTC_DISABLE_IRQ, &ctrl, sizeof(ctrl));
    }
    else
    {
        rc = MxWriteDrvData(hDrv, INTC_DISABLE_FIQ, &ctrl, sizeof(ctrl));
    }

    MxiRestInt(irq);

    return rc;

#else

    return MX_ARM_UNSUPP_OPR;

#endif /*MX_DRV_FRAMEWORK_ENABLED */
}

MXRC MxDisableIntEx(UINT32 nCpuID,UINT32 nIntID)
{
#ifdef MX_DRV_FRAMEWORK_ENABLED

    PMXCONTEXT pCtx;
    IRQTYPE irq;
    HANDLE hDrv;
    INTC_INT_CTRL ctrl;
    MXRC rc;

    pCtx = MxInqRemContext(nCpuID);

    _ASSERT(pCtx != NULL);

    if (pCtx->ICDrvId == 0xFFFFFFFF)
        return MX_ARM_UNSUPP_OPR;

    hDrv = MxGetDrv(pCtx->ICDrvId);

    _ASSERT(HANDLE_NOT_NULL(hDrv));

    MxiDisInt(&irq);

    ctrl.ID    = nIntID;
    ctrl.Opt   = pCtx->vectors[nIntID].Opt;
    ctrl.CpuID = nCpuID;

    if (pCtx->vectors[nIntID].Opt & INT_TYPE_IRQ)
    {
        rc = MxWriteDrvData(hDrv, INTC_DISABLE_IRQ_EX, &ctrl, sizeof(ctrl));
    }
    else
    {
        rc = MxWriteDrvData(hDrv, INTC_DISABLE_FIQ_EX, &ctrl, sizeof(ctrl));
    }

    MxiRestInt(irq);

    return rc;

#else

    return MX_ARM_UNSUPP_OPR;

#endif /*MX_DRV_FRAMEWORK_ENABLED */
}


BOOL MxIsIntEnabled(UINT32 nIntID)
{
#ifdef MX_DRV_FRAMEWORK_ENABLED

    PMXCONTEXT pCtx;
    IRQTYPE irq;
    HANDLE hDrv;
    INTC_IS_ENABLED ctrl;
    UINT32 nSize;
    MXRC rc;

    pCtx = MxInqContext();

    _ASSERT(pCtx != NULL);

    if (pCtx->ICDrvId == 0xFFFFFFFF)
        return MX_ARM_UNSUPP_OPR;

    hDrv = MxGetDrv(pCtx->ICDrvId);

    _ASSERT(HANDLE_NOT_NULL(hDrv));

    MxiDisInt(&irq);

    ctrl.ID_Status = nIntID;
    nSize = sizeof(ctrl);

    if (pCtx->vectors[nIntID].Opt & INT_TYPE_IRQ)
    {
        rc = MxReadDrvData(hDrv, INTC_IS_IRQ_ENABLED, &ctrl, &nSize);
    }
    else
    {
        rc = MxReadDrvData(hDrv, INTC_IS_FIQ_ENABLED, &ctrl, &nSize);
    }

    MxiRestInt(irq);

    return (FAILED(rc)) ? (FALSE) : (ctrl.ID_Status);

#else
    return FALSE;
#endif /*MX_DRV_FRAMEWORK_ENABLED */
}

MXRC MxSetARMIntCtrl(UINT32 flags)
{
    UINT32 nIntCtrl;

    nIntCtrl = MxGetCPSR ();

    if (flags & INT_CTRL_IRQ_ENABLED)
    {
        nIntCtrl &= ~0x80;
    }
    else
    {
        nIntCtrl |= 0x80;
    }

    if (flags & INT_CTRL_FIQ_ENABLED)
    {
        nIntCtrl &= ~0x40;
    }
    else
    {
        nIntCtrl |= 0x40;
    }

    MxSetCPSR(nIntCtrl);

    return MX_OK;
}

UINT32 MxGetARMIntCtrl(void)
{
    UINT32 t_cpsr, nIntCtrl = 0;

    t_cpsr = MxGetCPSR();

    if (t_cpsr & 0x80)
    {
        nIntCtrl |= INT_CTRL_IRQ_DISABLED;
    }
    else
    {
        nIntCtrl |= INT_CTRL_IRQ_ENABLED;
    }

    if (t_cpsr & 0x40)
    {
        nIntCtrl |= INT_CTRL_FIQ_DISABLED;
    }
    else
    {
        nIntCtrl |= INT_CTRL_FIQ_ENABLED;
    }

    return nIntCtrl;
}

/** @brief This function is designed to return ARM context pointer,
           ARM context is used to contain data of ARM registers 
           r0-r12, sp, lr, pc

    @retur [ARMCPUCONTEXT *] */

ARMCPUCONTEXT * MxGetArmCpuCtx (void)
{
    MXCONTEXT * p = MxInqContext();
    _ASSERT_PTR(p);

#ifdef MX_MTHREAD_ENABLED
    p->IrqStartTime = MxGetTicks();
#endif

    return &p->ArmCtx;
}

/** @brief This function is designed to return ARM context pointer,
           ARM context is used to contain data of ARM registers 
           r0-r12, sp, lr, pc

    @param nCpuID [in] - cpu id

    @retur [ARMCPUCONTEXT *] */

ARMCPUCONTEXT * MxGetArmCpuCtxEx (UINT32 nCpuID)
{
    MXCONTEXT * p = MxInqRemContext(nCpuID);

    if (p == NULL)
        return NULL;

    return &p->ArmCtx;
}

#endif /*MX_ARM_ENABLED */
