//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL Initcpu.c
 *
 * @brief
 * @author Mindspeed Technologies
 * @version $Revision: 1.79 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include <signal.h>
#include "config.h"
#include "4gmx.h"
#include "appinit.h"
#include "appprintf.h"
#include "hal.h"
#include "sirqassign.h"
#include "initcpu.h"
#include "lcorecfg.h"
#include "intercpu.h"
#include "version.h"
#include "lcorecfg.h"
#include "icdrv.h"
#include "devinit.h"
#include "devinfo.h"
#include "svsrlogger.h"
#include "mmumgr.h"
#include "dbgfunctab.h"


//******************************************************************************
// Just temporary variables to prevent warnings ("Empty Execution region description")
//******************************************************************************
UINT32 EXT_ICPU_VAR syscore_gr_a = 0; 
UINT32 EMUL_DDR1    syscore_emul_ddr1 = 0; 
UINT32 PRV_CORE0    core0_mem = 0;
UINT32 CRAMVAR      cram_mem = 0;
//******************************************************************************

#ifdef ARM_LVECTORS_PROTECTION
const __align(128) UINT8 low_vectors_table [256];
{}
#endif

NCNBVAR SYSCORE_MEM_AP syscore_mem_ap;

#ifdef __CC_ARM

// this pragma prevents usage of any ARM semihosting functions,
// to prevent usage of unexpected SWI interrupts

#pragma import (__use_no_semihosting_swi)
#endif

void MxAssertHandler(UINT32 nRC, const char *pFile, UINT32 nLine, UINT32 lr)
{
    UINT32 nLne;    // Specially added to have a possibility
		            // to find out the line number in debug mode
  	PVUINT32 pc = (PVUINT32)__current_pc();
	PVUINT32 sp = (PVUINT32)__current_sp();

    nLne = nLine;

    uart_printf("\r\n[--------ASSERT--------]\r\n");
    uart_printf("  CPU ID : %d\r\n", MxGetCpuID ());
    uart_printf("CPU Mode : 0x%x\r\n", MxGetCpuMode ());
    uart_printf("      RC : 0x%08x\r\n", nRC);
    uart_printf("    File : %s\r\n",     pFile);
    uart_printf("     Row : %d\r\n",     nLine);
    uart_printf("      LR : 0x%x\r\n----------------------\r\n",   lr);

	rootinfo_print_stack(pc, sp, NULL, NULL);

    nLne = nLne;
	

    if(SvsrLoggerGetMask())
        SvsrLoggerFlush();

    if (MLogGetMask() != 0)
        MLogPrint();

    if (MxGetCpuMode () == CPU_MODE_SUPERVISOR)
    {
        ARM_INT_enable ((ITYPE)(INT_TYPE_IRQ | INT_TYPE_FIQ));
        ICDrvRaiseInterCpu (0, SOFT_ICPU_SYS_SHDWN);
    }
    else
    {
        if (MxGetCpuID () == 0)
        {
            extern void SysShutdownIRQHandler (VUINT32 * pClear, UINT32 nIrq, LPVOID pData);
            VUINT32 a = 0, b = 0, c = 0;

            ICDrvRaiseInterCpu (0, SOFT_ICPU_SYS_SHDWN);
            SysShutdownIRQHandler (&a, b, (LPVOID)&c);
        }
        else
        {
            ARM_INT_enable ((ITYPE)(INT_TYPE_IRQ | INT_TYPE_FIQ));
            ICDrvRaiseInterCpu (0, SOFT_ICPU_SYS_SHDWN);
        }
    }

    while (1) ;
}

#ifdef MX_CREATE_OBJ_LOG_ENABLED

#define MAX_CREATE_OBJ_LOG 100

UINT32 CreateObjectIndex = 0;
UINT32 CreateObjectLogH [MAX_CREATE_OBJ_LOG] = {0};
UINT32 CreateObjectLogA [MAX_CREATE_OBJ_LOG] = {0};

/** @brief This function is designed to store the program address and object handlers
 * 			to provide the possibility to easily find out the source code and created object (partition, queue, heap...)
 *
 *  @param  procAddr [in] - the program address
 *  @param  h        [in] - the handler of created object
 *
 *  In this way we can find what is the function has created the object with this handler
 *
 * */
void MxCreateObjLog(UINT32 procAddr, HANDLE h)
{
	UINT32 nType;
	UINT32 i, nh;

	if (MxGetCpuID () != 0)
		return;

	nType = HANDLE_GET_TYPE (h);

	if (nType == OBJ_TYPE_HEAP || nType == OBJ_TYPE_QUEUE || nType == OBJ_TYPE_RHEAP || nType == OBJ_TYPE_PARTITION)
	{
		nh = *(PUINT32)&h;
		i = 0;

		while ((i < MAX_CREATE_OBJ_LOG) && (CreateObjectLogH[i] != 0))
		{
			if (CreateObjectLogH [i] == nh)
			{
				CreateObjectLogH [CreateObjectIndex] = nh;
				CreateObjectLogA [CreateObjectIndex] = procAddr;

				return;
			}

			i++;
		}

		if (CreateObjectIndex < MAX_CREATE_OBJ_LOG)
		{
			CreateObjectLogH [CreateObjectIndex] = *(PUINT32)&h;
			CreateObjectLogA [CreateObjectIndex] = procAddr;

			//uart_printf ("C-LOG: addr=0x%08x, h = 0x%08x\r\n", procAddr, CreateObjectLogH [CreateObjectIndex]);
		}
		else
		{
			//uart_printf ("C-LOG NO SPACE: addr=0x%08x, h = 0x%08x\r\n", procAddr, *(PUINT32*)&h);
		}

		CreateObjectIndex ++;
	}
}

#endif

#ifdef __APCS_SWST
/** @brief  This function is called by the RVDS in case of
            stack overflowing in general modes and exception modes as well*/

void SystemStackOverflow (int mode)
{
    while (1) ;
}
#endif /*__APCS_SWST*/

/** @brief This function checks MMU table and returns a status for the memory page
            is D-CACHE enabled or not

    @param nAddr [in] - the page address

    @return [BOOL] the status */

BOOL SysCoreIsDCached (UINT32 nAddr)
{
#ifdef MX_ARM_MMU_ENABLED
    return MxMmuIsDCached (nAddr);
#else
    return FALSE;
#endif
}

/** @brief This function returns number of CPUs controlled by the library

    @return [UINT32] the number of CPUs */

UINT32 SysGetCpuNum (void)
{
    // for the L-ARM not more than 2 cores
    if (MxGetARMID () != 0)
    {
        return MIN(CPU_NUM, 2);
    }

    // for U-ARM up to 4 CORES
    return MIN(CPU_NUM, 4);
}

/** @brief This function returns information about specific code section

    @param nID  [in] - the section ID
    @param pInfo[out]- information
    @return [MXRC] an error code */

MXRC SysCoreGetSectInfo (UINT32 nID, SECTINFO * pInfo)
{
    extern UINT8 Image$$ROOT_RO$$Base [];
    extern UINT8 Image$$ROOT_RWZI$$Base [];
    extern UINT8 Image$$EXT_ICPU$$Base [];
    extern UINT8 Image$$PRV_MEM_CORE0$$Base [];
    extern UINT8 Image$$STACK_HEAP$$Base [];
    extern UINT8 Image$$NCNB_DATA$$Base [];
    extern UINT8 Image$$EMUL_DDR1_SPACE$$Base [];

    _ASSERT_PTR (pInfo);

    if (pInfo == NULL)
        return RC_SC_PARAM_ERROR;

    switch (nID)
    {
        case SECTION_RO_CODE:
        {
            pInfo->Base = (UINT32)Image$$ROOT_RO$$Base;
            pInfo->Size = ROOT_RO_SIZE;
            break;
        }

        case SECTION_RW_CODE:
        {
            pInfo->Base = (UINT32)Image$$ROOT_RWZI$$Base;
            pInfo->Size = ROOT_RW_SIZE;
            break;
        }

        case SECTION_EXT_ICPU:
        {
            pInfo->Base = (UINT32)Image$$EXT_ICPU$$Base;
            pInfo->Size = ROOT_ICPUV2_SIZE;
            break;
        }

        case SECTION_PRV_CORE0:
        {
            pInfo->Base = (UINT32)Image$$PRV_MEM_CORE0$$Base;
            pInfo->Size = ROOT_PRV_CORE0_MEM_SIZE;
            break;
        }

        case SECTION_APP_HEAP:
        {
            pInfo->Base = (UINT32)Image$$STACK_HEAP$$Base;
            pInfo->Size = ROOT_HEAP_SIZE;
            break;
        }

        case SECTION_NCNB:
        {
            pInfo->Base = (UINT32)Image$$NCNB_DATA$$Base;
            pInfo->Size = ROOT_NCNB_SIZE;
            break;
        }

        default:
            memset (pInfo, 0, sizeof (*pInfo));
            return RC_SC_UNKNOWN_SECTION;

    }

    return MX_OK;
}

/** @brief This function is designed to be call on some 4GMX core to set
           access permission for selected region

    @param Base     [in] - the base address of the region
    @param nSize    [in] - the size of the region in bytes
    @param nAccess  [in] - the access permission

    @return [UINT32] an error code */

UINT32 SetMemmoryAccess (UINT32 Base, UINT32 nSize, UINT32 nAccess)
{
    UINT32 nRegionType;

    while (nSize != 0)
    {
        nRegionType = MxMmuGetRegionMapType (Base);

        if (nRegionType == MMU_SECTION_TYPE_FAULT)
            return RC_SC_MMU_TABLE_ERROR;

        if (nRegionType == MMU_SECTION_TYPE_1M_SECTION)
        {
            MxMmuSetMemAP (Base, nAccess);

            Base += 1024*1024; // +1MB
            nSize -= MIN (nSize, 1024*1024);
        }

        if (nRegionType == MMU_SECTION_TYPE_PAGED_64)
        {
            MxMmuSetMemAP (Base, nAccess);

            Base += 64*1024; // +1MB
            nSize -= MIN (nSize, 64*1024);
        }

        if (nRegionType == MMU_SECTION_TYPE_PAGED_4)
        {
            MxMmuSetMemAP (Base, nAccess);

            Base += 4*1024; // +1MB
            nSize -= MIN (nSize, 4*1024);
        }
    }

    return MX_OK;
}

UINT32 SysCoreRemoteMemAP (SYSCORE_MEM_AP * pParam)
{
    pParam->Rc = SetMemmoryAccess (pParam->Base, pParam->Size, pParam->nAccess);
    pParam->Status = 1;

    return 0;
}

/** @brief This function sets memory access to the memory region

    @param nCpuMask [in] - CPU mask
    @param Base     [in] - the base address
    @param nSize    [in] - the size in bytes
    @param nAccess  [in] - the access flags, see: 4GMX enum <MMU_AP>

    @return [MXRC] an error code */

MXRC SysCoreMemAccess (UINT32 nCpuMask, UINT32 Base, UINT32 nSize, UINT32 nAccess)
{
    UINT32 cpu = 0;
    UINT32 cpu_num = SysGetCpuNum ();
    UINT32 local_cpu = MxGetCpuID ();
    MXRC rc;
    UINT32 nTick;

    for (cpu = 0; cpu < cpu_num; cpu++)
    {
        if (nCpuMask & (1 << cpu))
        {
            if (local_cpu == cpu)
            {
                SetMemmoryAccess (Base, nSize, nAccess);
            }
            else
            {
                syscore_mem_ap.Base     = Base;
                syscore_mem_ap.Size     = nSize;
                syscore_mem_ap.nAccess  = nAccess;
                syscore_mem_ap.Status   = 0;
                syscore_mem_ap.Rc       = 0;

                rc = MxInjectCode (cpu, (TENTRY)SysCoreRemoteMemAP, &syscore_mem_ap);

                if (FAILED(rc))
                {
                    _ASSERT_RC(rc);
                    return rc;
                }

                nTick = GetTIMETICK();
                while ((syscore_mem_ap.Status == 0) && (GetTIMETICK () - nTick < TICKS_TO_MS(2000)))
                {
                    _NOP();
                }

                if (syscore_mem_ap.Status == 0)
                {
                    _ASSERT(syscore_mem_ap.Status != 0);
                    return RC_SC_MMU_TIMEOUT;
                }

                if (FAILED(syscore_mem_ap.Rc))
                {
                    _ASSERT_RC(syscore_mem_ap.Rc);
                    return syscore_mem_ap.Rc;
                }
            }
        }
    }

    return MX_OK;
}

/** @brief This function is needed to keep the references
 * 		to the 'unused code' of system library like vectors and
 * 		BRM PlugIN,  if we remove this function, the linker will not
 * 		add library code to the destination image, because those functions
 * 		are not called directly from the project
 * */

void  ForceCodeLinking (void)
{
	extern void ResetHandler (void);
	extern UINT32 BrmPlugin (void *);
	extern void LOWER_VECTORS (void);
	extern void HIGH_VECTORS (void);

	BrmPlugin (NULL);
	ResetHandler ();
	LOWER_VECTORS ();
	HIGH_VECTORS ();

    if (syscore_gr_a != 0)
        syscore_gr_a = 0;

    if (syscore_emul_ddr1 != 0)
        syscore_emul_ddr1 = 0;

    if (core0_mem != 0)
    {
        core0_mem = 0;
    }

    if (cram_mem != 0)
    {
        cram_mem = 0;
    }
    
    if (MxGetCpuID() == 0)
    {
        core0_mem = rootinfo[0].ObjType;
    }
}

void LoadUartCfg (UINT32 nCpuID)
{
    // this section inits default values for CEVA, FP, UART and other devices in configuration structure CoreCfg
    if (nCpuID == 0)
    {
        if (CoreCfg.UARTParams[0] == 0)
        {
            PUARTPARAMS pUart0 = (PUARTPARAMS)&CoreCfg.UARTParams[0];
        	pUart0->inited = 1;
        }

        if (CoreCfg.UARTParams[1] == 0)
        {
            PUARTPARAMS pUart1 = (PUARTPARAMS)&CoreCfg.UARTParams[1];

        	pUart1->size     = 3;    // 8 bits
        	pUart1->stop     = 0;    // 1 stop bit
        	pUart1->parity   = 0;    // no parity
        	pUart1->parsel   = 0;    // odd
        	pUart1->speed    = 115200;
        	if (SYSCORE_DEFAULT_UART_ID == 1)
        	        pUart1->EN         = 1;    // UART-1 enabled
        	pUart1->inited      = 1;       // UART-1 inited
#ifdef UART_DISABLE_FORCED
            pUart1->EN = 0; // UART-1 disabled
#endif
        }
        // UART-2
        if (CoreCfg.UARTParams[2] == 0)
        {
        	PUARTPARAMS pUart2 = (PUARTPARAMS)&CoreCfg.UARTParams[2];

            pUart2->size     = 3;    // 8 bits
        	pUart2->stop     = 0;    // 1 stop bit
        	pUart2->parity   = 0;    // no parity
        	pUart2->parsel   = 0;    // odd
        	pUart2->speed    = 115200;
        	if (SYSCORE_DEFAULT_UART_ID == 2)
        		pUart2->EN       = 1;    // UART-2 enabled
        	pUart2->inited = 1;          // UART-2 inited
#ifdef UART_DISABLE_FORCED
            pUart2->EN = 0; // UART-2 disabled
#endif
        }
        UartDrvLoadInitParams(&CoreCfg.UARTParams);

       // Console manager
       if (((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrinited == 0)
       {
          ((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrUART = SYSCORE_DEFAULT_UART_ID;
          ((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrinited = 1;

#ifdef UART_DISABLE_FORCED
          ((PENABLEDEVICE)&CoreCfg.EnableDevice)->cmgrUART = CMGR_TURNED_OFF;
#endif
       }

    }

}

/** @brief This function is an entry point of syscore library
            and should be called first before usage of any system API


        @NOTE  When this function calls cpu0 or cpu1 callbacks
               the system is initialized and application may use free
               any system API commands


    @param pParam [in] - the input parameters

    @return [MXRC] an error code */

MXRC SysCoreStart(SyscoreParam * pParam)
{
    UINT32 rc = MX_OK;
    STACKSHEAP *pStacksHeap;
    extern UINT32 RvdsSysHeapNotUsed;

    MXINIT init;
    IRQTYPE irq;

    UINT32 nCpuID = MxGetCpuID();

    ExtAppCallback cpu = NULL;
    MmuCfg mmu_cfg = NULL;

    RvdsSysHeapNotUsed = 1;

    if (pParam != NULL)
    {
        cpu = pParam->app_entry [MxGetCpuID ()];
        mmu_cfg = pParam->mmu_cfg [MxGetCpuID ()];
    }

    if (MxGetCpuID () == 0)
    {
        DevInfoInit ();
        LoadUartCfg(nCpuID);
    }

#ifdef __APCS_SWST
    signal (SIGSTAK, SystemStackOverflow);
#endif

    switch (nCpuID)
    {
        case 0:
            pStacksHeap = (STACKSHEAP *)&stacks_cpu0;
            break;

        case 1:
            pStacksHeap = (STACKSHEAP *)&stacks_cpu1;
            break;

        case 2:
            pStacksHeap = (STACKSHEAP *)&stacks_cpu2;
            break;

        case 3:
            pStacksHeap = (STACKSHEAP *)&stacks_cpu3;
            break;

        default:
            pStacksHeap = (STACKSHEAP *)&stacks_cpu0;
            // pStacksHeap == NULL just to prevent warning
           _ASSERT(pStacksHeap == NULL);
           break;
    }

    // to fill the stacks with pattern (for the debug purpose)

    memset (&init, 0, sizeof (init));
    memset (pStacksHeap->abort, ABORT_STACK_PATTER, ABORT_STACK_SIZE);
    memset (pStacksHeap->undef, UNDEF_STACK_PATTER, UNDEF_STACK_SIZE);
    memset (pStacksHeap->fiq,   FIQ_STACK_PATTER,   FIQ_STACK_SIZE);
    memset (pStacksHeap->irq,   IRQ_STACK_PATTER,   IRQ_STACK_SIZE);

    MxSetCpuStack(CPU_MODE_ABOART, pStacksHeap->abort + ABORT_STACK_SIZE,
                    ABORT_STACK_SIZE);

    MxSetCpuStack(CPU_MODE_UNDEF, pStacksHeap->undef + UNDEF_STACK_SIZE,
                    UNDEF_STACK_SIZE);

    MxSetCpuStack(CPU_MODE_FIQ, pStacksHeap->fiq + FIQ_STACK_SIZE,
                    FIQ_STACK_SIZE);

    MxSetCpuStack(CPU_MODE_IRQ, pStacksHeap->irq + IRQ_STACK_SIZE,
                    IRQ_STACK_SIZE);

    init.ConfigSet = SYS_INIT_DEF;

    if (MxGetARMID () == 0)
    {
        init.ConfigSet |= SYS_INIT_LOW_VECTORS;
#ifdef ARM_LVECTORS_PROTECTION
        init.lvecaddr   = (UINT32)low_vectors_table;
#endif
    }

    init.mmu_l1_table_ptr = (PUINT32)mmutable [MxGetCpuID ()];
    init.mmu_l2_table_ptr = (PUINT32)mmu_l2_table [MxGetCpuID ()];
    init.mmu_l2_table_size= (MX_MMU_L2_1M_NUMS * 256 * sizeof (UINT32));

    init.Drivers = (MXDRV*)appinit[nCpuID].MxCpuDrv;
    init.ICDrvId = appinit[nCpuID].ICDrvId;
#ifdef LOCAL_TIMER_INIT    
    init.TmrDrvId = 0x01;
#else
    init.TmrDrvId = 0xFFFFFFFF;
#endif

    init.SysHeap = DEFHEAP_BASE_ADDR(nCpuID);
    init.SysHeapSize = DEF_HEAP_SIZE;

#ifdef MX_INTERCPU_ENABLED
    init.ICTable = MxICTable;
    init.ICCtx   = &MxICCtx [nCpuID];
    init.ICSMPCtx= &MxSmpCtx;

    // to exclude SMP threads execution for core-1 (PHY) and core-3 (MAC)
    if (nCpuID == 2 || nCpuID == 3)
    {
        init.ConfigSet |= SYS_INIT_NO_PROC_SMP_THREAD;
    }

     // CPU-0 is master for SMP system
    if (MxGetCpuID () == 0)
    {
        init.ConfigSet |= SYS_SMP_MASTER;
    }

    if (MxGetARMID() == 0)
    {
        init.ICIrq   = SOFT_ICPU_4GMX_ICPU;
    }
    else
    {
   	    init.ICIrq   = SOFT_ICPU_4GMX_ICPU;
    }
#endif //MX_INTERCPU_ENABLED

#ifdef _MX_DEBUG
    init.TraceRingBuf = NULL;
    init.TraceBufSize = 0;
#endif

#ifdef MX_EXT_THREAD_TRACER_ENABLED
    init.ExtThrTracer = appinit[nCpuID].ThrTracer;
#endif

#ifdef MX_HWSEMA_ENABLED
    init.HwSemaBase = SEMAPHORES_BASE;
#endif

    if (FAILED(rc = DevInit()))
    {
        _ASSERT_RC(rc);
    }

    if (FAILED(rc = MxInit(&init, &MxCtx[nCpuID])))
    {
        _ASSERT(0);
    }

    if (MxGetCpuID () == 0)
    {
        PrintSysVersion ();

        //REG32(&CoreCfg.HostIntID) = 2;

        if (IS_EXT_ICPU())
        {
            extern UINT8 Image$$EXT_ICPU$$Base [];
            UINT32 icpu_base = (UINT32)Image$$EXT_ICPU$$Base;
        
            memset ((LPVOID)icpu_base, 0, 128);

            // to redefine I-CPU base address in case of new I-CPU
            REG32(&CoreCfg.ICPUBase) = icpu_base;

            if (IsMMUMsgEnabled())
                uart_printf (" - I-CPU extended: 0x%08x\r\n", icpu_base);
        }
    }

    #ifdef MX_ARM_MMU_ENABLED
        irq = ARM_INT_disable ();
        MMUInit(nCpuID, mmu_cfg);
        ARM_INT_restore (irq);
    #else
        uart_printf("MMU is disabled\r\n");
    #endif

    // Application initialization
    // --------------------------
    if (appinit[nCpuID].SysInit != NULL)
    {
        if (FAILED(rc = appinit[nCpuID].SysInit(nCpuID, cpu)))
        {
        	_ASSERT_RC(rc);
        	return rc;
        }
    }

    if (FAILED(rc = MxStart(appinit[nCpuID].Idle, appinit[nCpuID].IdleParam)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    // this function is added here just to keep references to the
    // library functions, this code will be not called at all
    // according to the logic implemented above
    if (FAILED(rc))
    	ForceCodeLinking ();

    return (int)rc;
}

#ifdef __CC_ARM

ASMPROC void Core0_StackSetup (void)
{
    import stacks_cpu0;

    LDR r0, =stacks_cpu0
    ADD r0, r0, #SPRV_STACK_SIZE
    MOV SP, r0

    LDR r0, =STACK_BASE_ADDR_CPU0
    LDR r1, =SVSR_STACK_PATTER
    LDR r2, =SPRV_STACK_SIZE

LOOPCORE0
    CMP r2, #0
    STRNE r1, [r0], #4
    SUBNE r2, r2, #4
    BNE LOOPCORE0

    BX LR
}


ASMPROC void CoreX_StackSetup (UINT32 cpuid)
{
    import stacks_cpu1;
    import stacks_cpu2;
    import stacks_cpu3;

    MOV r1,r0
    CMP r1, #1
    LDREQ r0, =stacks_cpu1

    CMP r1, #2
    LDREQ r0, =stacks_cpu2

    CMP r1, #3
    LDREQ r0, =stacks_cpu3

    MOV r1, r0
    ADD r0, r0, #SPRV_STACK_SIZE
    MOV SP, r0

    MOV r0, r1
    LDR r1, =SVSR_STACK_PATTER
    LDR r2, =SPRV_STACK_SIZE

LOOPCORE1
    CMP r2, #0
    STRNE r1, [r0], #4
    SUBNE r2, r2, #4
    BNE LOOPCORE1

    BX LR
}


ASMPROC void __user_initial_stackheap(void)
{
    import stacks_cpu0;

    /*the heap + the stack in the same buffer */

    // to fill the supervisor stack with pattern
    LDR r0, =STACK_BASE_ADDR_CPU0 /* no heap                   */
    LDR r1, =SVSR_STACK_PATTER
    LDR r2, =SPRV_STACK_SIZE

LOOP
    CMP r2, #0
    STRNE r1, [r0], #4
    SUBNE r2, r2, #4
    BNE LOOP

    LDR r0, = STACK_BASE_ADDR_CPU0
    ADD r1, r0, #SPRV_STACK_SIZE

    BX lr
}
#else /* __CC_ARM */
ASMPROC void __user_initial_stackheap(void)
{
}
ASMPROC void CoreX_StackSetup (void)
{
}

#endif /* __CC_ARM */
