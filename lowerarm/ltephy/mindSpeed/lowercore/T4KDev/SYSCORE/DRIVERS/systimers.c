//-------------------------------------------------------------------------------------------
/** @file systimers.c
 *
 * @brief System Timers implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.18 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "config.h"
#include "systimers.h"
#include "appprintf.h"
#include "4gmx.h"
#include "clkrst.h"
#include "timerdrv.h"
#include "lcorecfg.h"
#include "sirqassign.h"
#include "icdrv.h"
#include "mmumgr.h"

SysTimerCtx SysTmrCtx;
extern const char LTmrName[];

SysTimerCtx *SysTimerGetCtx(void)
{
	return &SysTmrCtx;
}

#ifdef XP_TIMER_IRQ_HANDLER_ENABLED

/** @brief The handler processes IRQ interrupts of timer

 @param pClear [in] - the pointer to interrupt clear register
 @param pParam [in] - the ISR parameter
 \ingroup group_lte_services
 */

UINT32 SysTimerWDThread(LPVOID data);

void SysTimersIrqHandlerCpu0(volatile UINT32 * pClear, UINT32 nIrqID, LPVOID pParam)
{
    PMXCONTEXT pCtx;
    UINT32 nStatus;

    SysTmrCtx.nProcTimer = 1;

    pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);

    nStatus = REG32(TIMER_IRQ_STATUS);

    if (nStatus & SYSTIMER3_MASK)
    {
        MxProcTimers(pCtx);

        REG32(TIMER_IRQ_STATUS) |= SYSTIMER3_MASK;
    }

    if (nStatus & SYSTIMER2_MASK)
    {
    	if (SysTmrCtx.T2CallBack != NULL)
    		SysTmrCtx.T2CallBack (SysTmrCtx.T2CallBackParam);

    	REG32(TIMER_IRQ_STATUS) |= SYSTIMER2_MASK;
    }

    if (nStatus & SYSTIMER1_MASK)
    {
    	if (SysTmrCtx.T1CallBack != NULL)
    		SysTmrCtx.T1CallBack (SysTmrCtx.T1CallBackParam);

    	REG32(TIMER_IRQ_STATUS) |= SYSTIMER1_MASK;
    }

    *pClear = nIrqID;
}

/** @brief The handler processes IRQ interrupts of timer

 @param pClear [in] - the pointer to interrupt clear register
 @param pParam [in] - the ISR parameter

 \ingroup group_lte_services
 */

void SysTimersIrqHandlerCpu1(volatile UINT32 * pClear, UINT32 nIrqID, LPVOID pParam)
{
    PMXCONTEXT pCtx;

    if (SysTmrCtx.nProcTimer == 0)
    	return;

    pCtx = MxInqContext();

    _ASSERT_PTR(pCtx);

    MxProcTimers(pCtx);

    SysTmrCtx.nProcTimer = 0;
}

#endif

void SysTimersInit_x2(UINT32 TimerValue)
{
    IRQTYPE irq = ARM_INT_disable ();

    REG32(TIMER_IRQ_STATUS) = 0xF;

    REG32(TIMER0_CNTR_REG) = (UINT32)0xFFFFFFFE;

    REG32(TIMER3_LBOUND_REG) = 0;
    REG32(TIMER3_HBOUND_REG) = (UINT32)TimerValue;
    REG32(TIMER3_CNTR_REG) = TimerValue & (~1);

    REG32(TIMER_IRQ_MASK) = SYSTIMER3_MASK;

    ARM_INT_restore (irq);
}

void SysTimersInit_x4(UINT32 TimerValue)
{
    IRQTYPE irq = ARM_INT_disable ();

    REG32(TIMER_IRQ_STATUS) = 0xF;

    REG32(TIMER0_CNTR_REG) = (UINT32)0xFFFFFFFE;

    ARM_INT_restore (irq);
}

void SysTimer1Set(UINT32 TimerValue)
{
    IRQTYPE irq;

    irq = ARM_INT_disable ();

    REG32(TIMER1_CNTR_REG) = TimerValue & (~1);

    ARM_INT_restore (irq);
}

void SysTimer1SetCallBack (void (* pCB) (void *) ,  void * pParam )
{
    IRQTYPE irq;

    irq = ARM_INT_disable ();

    SysTmrCtx.T1CallBack = pCB;
    SysTmrCtx.T1CallBackParam = pParam;

    ARM_INT_restore (irq);
}

void SysTimer1EnableIRQ(void)
{
    REG32(TIMER_IRQ_MASK) |= SYSTIMER1_MASK;	// to enable the IRQ for timer1
}

void SysTimer1DisableIRQ(void)
{
    IRQTYPE irq;

    irq = ARM_INT_disable ();

    REG32(TIMER1_CNTR_REG) = 0xFFFFFFFF & (~1);

    REG32(TIMER_IRQ_MASK) &= ~SYSTIMER1_MASK;	// to disable IRQ for timer1

    ARM_INT_restore (irq);
}

void SysTimer2Set(UINT32 TimerValue)
{
    IRQTYPE irq;

    irq = ARM_INT_disable ();

    REG32(TIMER2_LBOUND_REG) = 0;
    REG32(TIMER2_HBOUND_REG) = (UINT32)TimerValue;
    REG32(TIMER2_CNTR_REG) = TimerValue & (~1);

    ARM_INT_restore (irq);
}

void SysTimer2SetCallBack (void (* pCB) (void *) ,  void * pParam )
{
    IRQTYPE irq;

    irq = ARM_INT_disable ();

    SysTmrCtx.T2CallBack = pCB;
    SysTmrCtx.T2CallBackParam = pParam;

    ARM_INT_restore (irq);
}

void SysTimer2EnableIRQ(void)
{
    REG32(TIMER_IRQ_MASK) |= SYSTIMER2_MASK;	// to enable the IRQ for timer2
}

void SysTimer2DisableIRQ(void)
{
    IRQTYPE irq;

    irq = ARM_INT_disable ();

    REG32(TIMER2_LBOUND_REG) = 0;
    REG32(TIMER2_HBOUND_REG) = (UINT32)0xFFFFFFFF;
    REG32(TIMER2_CNTR_REG) = 0xFFFFFFFF & (~1);

    REG32(TIMER_IRQ_MASK) &= ~SYSTIMER2_MASK;	// to disable IRQ for timer2

    ARM_INT_restore (irq);
}

MXRC SysTimerDrv_init(UINT32 nInit, LPVOID * pCtx)
{
    MXRC rc = SYSTIMER_RC_OK;

    if (SysTmrCtx.nInited != 0x12345678)
    {
    	// to use 1ms timestamp
        SysTmrCtx.nInitVal = ClkRstGetFreq(XP_AXI) / 1000;
    }

#ifdef XP_TIMER_IRQ_HANDLER_ENABLED
    // the ARMx2 should be cloked with XP timer
    if (MxGetARMID () != 0)
    {
        if (MxGetCpuID () == 0)
    	{
    	    #if 0
	        rc = MxSetIntHandler(HAL_IRQ_TIMER, (ITYPE)(INT_TYPE_IRQ | INT_TYPE_1N), PRI_SYS_TIMERS, SysTimersIrqHandlerCpu0,
	    		(LPVOID)SysTmrCtx.nInitVal);

	        if (FAILED (rc))
	    	    return rc;

	    	rc = MxEnableInt(HAL_IRQ_TIMER);

	        if (FAILED (rc))
	            return rc;
            #endif

            // It's done some later in SysTimersInit_x2()
            //REG32(TIMER0_CNTR_REG) = (UINT32)0xFFFFFFFE;
      	}

#if 0
    	// timer usage on Core-1 is disabled to save MIPS
    	// time tasks should be scheduled on Core-0

    	else
    	{
    		rc = MxSetIntHandler(HAL_IRQ_TIMER, (ITYPE)(INT_TYPE_IRQ | INT_TYPE_1N), PRI_SYS_TIMERS, Sys_TimerIrqHandlerCpu1,
    			    		(LPVOID)SysTmrCtx.nInitVal);

    		if (FAILED (rc))
	    	    return rc;

	    	rc = MxEnableInt(HAL_IRQ_TIMER);

	        if (FAILED (rc))
	            return rc;
    	}
#endif
    }
#endif

    return rc;
}

MXRC SysTimersStart(void)
{
    MXRC rc = SYSTIMER_RC_OK;

    if (SysTmrCtx.nInited != 0x12345678)
    {
    	// to use 1ms timestamp
        SysTmrCtx.nInitVal = ClkRstGetFreq(XP_AXI) / 1000;
        SysTimersInit_x4 (SysTmrCtx.nInitVal);
        SysTmrCtx.nInited = 0x12345678;
    }

    return rc;
}

MXRC SysTimerStartWithDiag (void)
{
    MXRC rc = MX_OK;

    UINT32 t1, t2;

    //uart_printf ("XP timer initialization ... ");
    SysTimersStart();
    //uart_printf ("OK\r\n");

    t1 = GetTIMETICK ();
    //uart_printf (" * XP tick:0x%08x\r\n", t1);

    t2 = GetTIMETICK ();
    //uart_printf (" * XP tick:0x%08x\r\n", t2);

    if (t1 != t2)
    {
        //uart_printf ("XP timer is OK\r\n");
    }
    else
    {
        uart_printf ("XP timer is not INITIALIZED\r\n");

        rc = 1;

        _ASSERT_RC (rc);

        while (rc);
    }

    return MX_OK;
}

//UINT32 wd_count[CPU_NUM] = {0,0,0,0};

UINT32 SysTimerWDThread(LPVOID data)
{
	HANDLE hTimer;
	MXRC rc = MX_OK;
	UINT32 ResetCpuId, i;
	
	ResetCpuId = MxGetCpuID();

	//if(ResetCpuId == 0)
	//	wd_count[ResetCpuId]++;

	//if((wd_count[ResetCpuId] > 50))
	//	return 0;

	if((MX_WD_CORES & 1 << ResetCpuId))
	{
		rc = MxGetObjByName(ResetCpuId, LTmrName, &hTimer);

	    if (FAILED(rc))
	    {
	        uart_printf ("MxGetObjByName is failed, rc=%x\r\n", rc);
			return 0;
	    }

		SysTmrCtx.WDPeriod[ResetCpuId] = WD_HW_PERIOD_TICK;
		MxConfigDrv(hTimer, TIMER_WD_CMD_CLEAR_WDRESET, (LPVOID) 0, 0);
	    MxConfigDrv(hTimer, TIMER_WD_CMD_SET_PRESCALE, (LPVOID) 0, 0);
		MxConfigDrv(hTimer, TIMER_WD_CMD_SET_PERIOD, (LPVOID) (SysTmrCtx.WDPeriod[ResetCpuId])  ,0);	
	    MxConfigDrv(hTimer, TIMER_WD_CMD_SET_CTRL, (LPVOID)(TIMER_CTRL_ENABLE | TIMER_CTRL_AUTO | TIMER_WD_CTRL_WDMODE), 0);	

	}
	if(ResetCpuId == 0)
		for(i = 0; i < CPU_NUM; i++)	
			if( HANDLE_NOT_NULL(SysTmrCtx.hWDThread[i]) && (MX_WD_CORES & 1 << i))
				MxScheduleThread(SysTmrCtx.hWDThread[i]);

	return 0;
}

MXRC SysTimerInitWD(void)
{
	UINT32 wdmask = MX_WD_CORES;
    MXRC rc = MX_OK;
    UINT32 ResetCpuId;

	ResetCpuId = MxGetCpuID();

	if(wdmask == 0)
		return rc;

	if(ResetCpuId == 0)
	{
		rc = MxCreateThreadEx(ResetCpuId, SysTimerWDThread, NULL, NULL, 0, &SysTmrCtx.hWDThread[ResetCpuId]);  
		if(FAILED(rc))
			return rc;
	    rc = MxCreateEventEx(ResetCpuId, 0, NULL, &SysTmrCtx.hWDevnt);    
		if(FAILED(rc))
			return rc;
	    rc = MxCreateTimerEx(ResetCpuId, TIMER_MODE_PERIOD , WD_TH_PERIOD_MS, NULL, SysTmrCtx.hWDevnt, &SysTmrCtx.hWDtimer);    
		if(FAILED(rc))
			return rc;
	    rc = MxScheduleThreadByEvent(SysTmrCtx.hWDThread[ResetCpuId], SysTmrCtx.hWDevnt, THREAD_EVENT_AUTO_RESET);
		if(FAILED(rc))
			return rc;

		SysTimerWDThread(NULL);

		rc = MxStartTimer(SysTmrCtx.hWDtimer);
		if(FAILED(rc))
			return rc;
	}else
	{		
		if(!(MX_WD_CORES & 1 << ResetCpuId))
			return MX_OK;
		
		rc = MxCreateThreadEx(ResetCpuId, SysTimerWDThread, NULL, NULL, 0, &SysTmrCtx.hWDThread[ResetCpuId]);  
		if(FAILED(rc))
			return rc;
	}			

	return rc;
}

MXRC SysTimersShutdown (void)
{
	HANDLE hTimer;
	UINT32 ResetCpuId;

	ResetCpuId = MxGetCpuID();

	if(MX_WD_CORES & 1 << ResetCpuId )
	{			
		MxGetObjByName(ResetCpuId, LTmrName, &hTimer);
		
		MxConfigDrv(hTimer, TIMER_WD_CMD_CLEAR_WDRESET, 0, 0);
		MxConfigDrv(hTimer, TIMER_WD_CMD_DIS_WDMODE, 0, 0);
		MxConfigDrv(hTimer, TIMER_WD_CMD_ENABLE, 0, 0);
	}
	
    if(ResetCpuId == 0)
    {
		REG32 (TIMER_IRQ_MASK) = 0;		
		if(HANDLE_NOT_NULL(SysTmrCtx.hWDtimer))
		{
			MxStopTimer(SysTmrCtx.hWDtimer);
			SysTmrCtx.hWDtimer = HNULL;
		}

		MxDisableInt (HAL_IRQ_TIMER);
		memset (&SysTmrCtx, 0, sizeof (SysTmrCtx));
    }
	
    return SYSTIMER_RC_OK;
}

void ProcWDService (void)
{
	LPVOID pcfg = (LPVOID)&CoreCfg;
	UINT32 cur_cpu = MxGetCpuID();
	if(TimerDrvWDGetWDReset() || CoreCfg.RunStatus & 1 << (31 - cur_cpu))
	{
		UINT32 cur_cpu = MxGetCpuID();
		uart_printf("=========================================================================================\r\n");
		uart_printf("          !!!WatchDog has stopped the system on core %d due to %dms timout!!!          \r\n", cur_cpu, WD_HW_PERIOD_TICK / ClkRstGetFreq (ARMx4_CORE0));
		uart_printf("=========================================================================================\r\n");
		TimerDrvWDSetWDReset(1);
		TimerDrvWDDisWDMode();
		TimerDrvWDSetCtrl(0);
		ICDrvRaiseInterCpu (0, SOFT_ICPU_SYS_SHDWN);
		while(1);
	}
	else
    {   
        if (cur_cpu == 0)
        {
            
        }
    
		((LCoreCfg *)pcfg)->RunStatus |= (1 << (31 - cur_cpu));
    }
}

void SysTimerMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    MmuMgrCfgMemAP(TIMER_APB_BASEADDR, 4*1024, AP_MGR_READ_WRITE, AP_MGR_READ_ONLY);
}


