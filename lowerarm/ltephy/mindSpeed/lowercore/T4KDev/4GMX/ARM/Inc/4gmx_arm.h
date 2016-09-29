//-------------------------------------------------------------------------------------------
/** @file 4gmx_arm.h
 *
 * @brief ARM dependent API definition
 * @author Mindspeed Technologies
 * @version $Revision: 1.21 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_ARM_H_
#define _4GMX_ARM_H_

#include "4gmx_config.h"
#include "4gmx_types.h"
#include "4gmx_const.h"
#include "4gmx_arm_types.h"
#include "4gmx_kernel.h"
#include "4gmx_arm_cp15.h"

#ifdef MX_ARM_MMU_ENABLED
#include "4gmx_arm_mmu.h"
#endif

#include "4gmx_l2cache.h"

#ifdef MX_ARM_ENABLED

#ifdef __cplusplus
extern "C"
{
#endif

#define MX_ARM_ISR_ERROR         DEF_MXRC(MXM_ARM, 1)
#define MX_ARM_UNSUPPORTED_CPU   DEF_MXRC(MXM_ARM, 2)
#define MX_ARM_UNSUPP_OPR        DEF_MXRC(MXM_ARM, 3)
#define MX_ARM_ISR_NOT_SET       DEF_MXRC(MXM_ARM, 4)
#define MX_ARM_CPU_ID_ERROR      DEF_MXRC(MXM_ARM, 5)

/************************************************************************/
/*                 API definition                                       */
/************************************************************************/

/** @brief The function is called by MxInit function and initializes
   ARM sub-system API.

 @param pCtx [in] - the pointer to the system context
 @param nOpt [in] - the configuration parameters

 @return [MXRC] error code of initialization */

MXRC MxARMSubSystemInit(PMXCONTEXT pCtx, UINT32 nOpt);

MXRC MxARMConfigIntc(PMXCONTEXT pCtx, UINT32 nOpt);

/************************************************************************/
/*     Stacks and modes control API                                     */
/************************************************************************/

/** @brief  The function sets SP register for selected processor mode and
   switches back to the previous CPU mode.

   The CPU_MODE_USER is not supported.

 @param cpumode [in] - the id of CPU mode
 @param pStack  [in] - the pointer to the head of stack (the stack is descending thing) 
 @param nSize   [in] - the size of stack in bytes 
*/

ASMPROC void MxSetCpuStack(CPUMODE cpumode, LPVOID pStack, UINT32 nSize);

/** @brief The function returns ARM core ID: [0...3] for ARMx4 or [0,1] for ARMx2
 @return [UINT32] ARM core ID [0 ... N] */

INLINE UINT32 MxGetCpuID(void)
{
    // This code converts 
    // ARMx4core2 and ARMx4core3 to ARMx4core0 and ARMx4core1

    return (sys_read_coreid() & 3);
}

INLINE UINT32 MxGetARMCpuIDReg(void)
{
    return (sys_read_coreid());
}

/** @brief The function returns ARM ID: 0 for ARMx4 or 1 for ARMx2
 @return [UINT32] ARM ID [0 ... N] */

INLINE UINT32 MxGetARMID(void)
{
    return (sys_read_coreid() >> 8) & 0xF;
}

#ifdef MX_L1_HW_SYNC_ENABLED

    #ifdef MX_HWSEMA_ENABLED
    void MxiInitHwSema (PMXCONTEXT pCtx, UINT32 semaID);
    void MxiLockHwSema (UINT32 semaID);
    void MxiUnlockHwSema (UINT32 semaID);
    #else
    void INLINE MxiInitHwSema (PMXCONTEXT pCtx, UINT32 semaID){};
    void INLINE MxiLockHwSema (UINT32 semaID){};
    void INLINE MxiUnlockHwSema (UINT32 semaID){};
    #endif    

#endif //MX_L1_HW_SYNC_ENABLED

#ifdef __CC_ARM

/** @brief This function returns id of ARM mode,
            please see CPUMODE for more information 

    @return [CPUMODE] */
            
CPUMODE MxGetCpuMode(void);

/** @brief This function sets ARM mode,
            please see CPUMODE for more information 

    @param mode [in] - the ARM mode 

    @return [MXRC] an error code  */

MXRC MxSetCpuMode(CPUMODE mode);

/** @brief Data synchronization barrier
    @return none */
void MxDSB(void);

/** @brief Instruction synchronization barrier
    @return none */
void MxISB(void);

/** @brief Data memory barrier
    @return none */
void MxDMB(void);

/** @brief Returns stack pointer
    @return none */
UINT32 MxGetSp(void);

/** @brief Wait for interrupt
    @return none */
void MxWFI(void);


void MxiARMErrata(UINT32 tmp);

/** @brief This function sets ARM low vectors base address,
            and may be used to change default 0 base address

    @param LVectorBaseAddr [in] - low vector base address (32 bytes aligned)

    @return none */

void    MxSetVB(UINT32 LVectorBaseAddr);

/** @brief This function returns ARM low vectors base address,
            and may be used to get low vectors base address

    @return [UINT32] low vector base address (32 bytes aligned)*/

UINT32  MxGetVB (void);

#else /* __CC_ARM */
INLINE CPUMODE MxGetCpuMode(void)
{
    UINT32 ctrl;

    asm volatile ("mrs %0, cpsr" : "=r" (ctrl));

    return (CPUMODE)(ctrl & CPU_MODE_MASK);
}

INLINE MXRC MxSetCpuMode(CPUMODE mode)
{
    UINT32 ctrl;

	asm volatile ("mrs %0, cpsr" : "=r" (ctrl) : );
    ctrl &= ~0x1F;
    ctrl |= mode;
	asm volatile ("msr cpsr, %0" : : "r" (ctrl));

    return MX_OK;
}
#endif /* __CC_ARM */

/** @brief The function returns ID of ARM CPU mode
    @param mode [in] - the ARM mode (see: CPUMODE type)
    @return [CPUMODEID] an ID of ARM CPU mode */

CPUMODEID MxGetArmModeID (CPUMODE mode);

/** @brief The function returns value of ARM CPU mode
    @param modeid [in] - the ID of ARM mode (see: CPUMODEID type)
    @return [CPUMODE] the value of ARM CPU mode */
    
CPUMODE MxGetArmMode (CPUMODEID modeid);

/************************************************************************/
/*      Interrupt control API                                           */
/************************************************************************/

MXRC MxSetExceptionHandler(ETYPE type, EXPENTRY proc);
MXRC MxSetExceptionHandlerCpu(CPUID id, ETYPE type, EXPENTRY proc);

MXRC MxSetIntHandler(UINT32 nIntID, ITYPE type, TPRIORITY priority,
                     TISRENTRY isr, LPVOID pParam);
MXRC MxSetIntHandlerEx(UINT32 nCpuID, UINT32 nIntID, ITYPE opt, TPRIORITY priority,
                     TISRENTRY isr, LPVOID pParam);

MXRC MxSetIntPriority(UINT32 nIntID, TPRIORITY priority);
MXRC MxSetIntPriorityEx(UINT32 nCpuID,UINT32 nIntID, TPRIORITY priority);
MXRC MxSetIntHandlerParam(UINT32 nIntID, LPVOID pData);

MXRC MxEnableInt(UINT32 nIntID);
MXRC MxEnableIntEx(UINT32 nCpuID, UINT32 nIntID);
MXRC MxDisableInt(UINT32 nIntID);
MXRC MxDisableIntEx(UINT32 nCpuID, UINT32 nIntID);
BOOL MxIsIntEnabled(UINT32 nIntID);

MXRC MxSetARMIntCtrl(UINT32 flags);
UINT32 MxGetARMIntCtrl(void);

#ifdef __CC_ARM

IRQTYPE ARM_INT_disable(void);
void ARM_INT_restore(IRQTYPE flags);
void ARM_INT_enable(ITYPE irq);

#else /* __CC_ARM */
INLINE IRQTYPE ARM_INT_disable(void)
{
    UINT32 ret_val;

    asm volatile ("mrs %0, cpsr" : "=r" (ret_val));
    // Disable IRQ
    asm volatile ("cpsid i" : : );
    // Disable FIQ
    asm volatile ("cpsid f" : : );

    return ret_val;
}

INLINE void ARM_INT_restore(IRQTYPE flags)
{
    asm volatile ("msr cpsr, %0" : : "r" (flags));
}
#endif /* __CC_ARM */

BOOL INLINE MxIsDCached (UINT32 nAddr)
{
    extern BOOL MxMmuIsDCached (UINT32 nAddr);
    return MxMmuIsDCached (nAddr);
}

void INLINE MxCacheInvalidate (LPVOID pMem, UINT32 nSize) 
{
    IRQTYPE irq;

    extern void _MxiCacheInvalidate (LPVOID pMem, UINT32 nSize);

    if (!MxIsDCached ((UINT32)pMem))
        return;

    irq = ARM_INT_disable ();

#ifdef MX_L1_HW_SYNC_ENABLED
    MxiLockHwSema (MX_HW_SEMA_L1_SYNC);
#endif     

#ifdef MX_ARM_L2_CACHE_ENABLED
    MxL2_invalidate ((UINT32)pMem, nSize);
#endif

    _MxiCacheInvalidate (pMem, nSize);

#ifdef MX_L1_HW_SYNC_ENABLED
    MxiUnlockHwSema (MX_HW_SEMA_L1_SYNC);
#endif

    ARM_INT_restore (irq);
}


void INLINE MxCacheClean(LPVOID pMem, UINT32 nSize) 
{
    IRQTYPE irq;

    extern void _MxiCacheClean (LPVOID pMem, UINT32 nSize);
    
    if (!MxIsDCached ((UINT32)pMem))
        return;
    
    irq = ARM_INT_disable ();

#ifdef MX_L1_HW_SYNC_ENABLED
    MxiLockHwSema (MX_HW_SEMA_L1_SYNC);
#endif 

    _MxiCacheClean(pMem, nSize);

#ifdef MX_ARM_L2_CACHE_ENABLED
    MxL2_clean((UINT32)pMem, nSize);
#endif

#ifdef MX_L1_HW_SYNC_ENABLED
    MxiUnlockHwSema (MX_HW_SEMA_L1_SYNC);
#endif
   
    ARM_INT_restore (irq);
}

void INLINE MxCacheFlush(LPVOID pMem, UINT32 nSize) 
{
    IRQTYPE irq;

    extern void _MxiCacheFlush (LPVOID pMem, UINT32 nSize);

    if (!MxIsDCached ((UINT32)pMem))
        return;

    irq = ARM_INT_disable ();

#ifdef MX_L1_HW_SYNC_ENABLED
    MxiLockHwSema (MX_HW_SEMA_L1_SYNC);
#endif 

    _MxiCacheFlush(pMem, nSize);

#ifdef MX_ARM_L2_CACHE_ENABLED
    MxL2_flush((UINT32)pMem, nSize);
#endif

#ifdef MX_L1_HW_SYNC_ENABLED
    MxiUnlockHwSema (MX_HW_SEMA_L1_SYNC);
#endif

    ARM_INT_restore (irq);
}

UINT32 MxGetCPSR (void);
void   MxSetCPSR (UINT32 nVal);

UINT32 MxGetArmScuBaseAddr(void);
UINT32 MxGetArmGicDistBaseAddr(void);
UINT32 MxGetArmGicCpuBaseAddr(void);

/** @brief This function is designed to return ARM context pointer,
           ARM context is used to contain data of ARM registers 
           r0-r12, sp, lr, pc

    @retur [ARMCPUCONTEXT *] */

ARMCPUCONTEXT * MxGetArmCpuCtx (void);

/** @brief This function is designed to return ARM context pointer,
           ARM context is used to contain data of ARM registers 
           r0-r12, sp, lr, pc

    @param nCpuID [in] - cpu id

    @retur [ARMCPUCONTEXT *] */

ARMCPUCONTEXT * MxGetArmCpuCtxEx (UINT32 nCpuID);

#ifdef __cplusplus
}
#endif

#endif /*MX_ARM_ENABLED */
#endif /*_4GMX_ARM_H_*/
