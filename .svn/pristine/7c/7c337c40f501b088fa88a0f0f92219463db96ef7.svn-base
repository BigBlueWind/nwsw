//-------------------------------------------------------------------------------------------
/** @file timerdrv.c
 *
 * @brief Timer driver "C" code
 * @author Mindspeed Technologies
 * @version $Revision: 1.31 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "timerdrv.h"
#include "hal.h"
#include "interrupts.h"
#include "uartdrv.h"
#include <string.h>
#include "icdrv.h"
#include "config.h"
#include "clkrst.h"
#include "appprintf.h"

extern const char LTmrName[];

static TimerDrvCtx  gTmrCtx [CPU_NUM];

TimerDrvCtx * TimerDrvGetCtx (void)
{
    return &gTmrCtx[MxGetCpuID ()];
}

/** @brief The handler processes IRQ interrupts of timer

 @param pClear [in] - the pointer to interrupt clear register
 @param pParam [in] - the ISR parameter
 \ingroup group_lte_services
 */

UINT32 TmrTicks = 0;
UINT32 TmrCount = 0;
UINT32 TimerDrvCheck;
VUINT32 TmrWarning = 0;		// The number of delayed interrupts ( dt < 1 ms) due to system delay 


void TmrDrvShowIrqTrace(void)
{
    UINT32 nNum = ICDrvGetTraceSize (0);
    UINT32 i, tti = 0;
    GIT_IRQ_TRACE_TYPE * pInfo;
	UINT32 prev_ticks = 0;

	if (!nNum)
		return;
	
    uart_printf ("=================== IRQ trace (%d) =====================\r\n", nNum);
    MxDelayTicks(20000);
    
    for (i = 0; i < nNum; i++)
    {
        pInfo = ICDrvGetTraceElm (0, i);
        _ASSERT_PTR(pInfo);
    
        if (pInfo->IrqID == HAL_IRQ_LOCAL_TIMER)
        {
            uart_printf ("TTI(%d) prev:[%d], s:[0x%08x], e:[0x%08x], t:[%d]\r\n", 
                tti++, prev_ticks ? (pInfo->StartTicks - prev_ticks) : 0, pInfo->StartTicks, pInfo->StopTicks, pInfo->StopTicks - pInfo->StartTicks);

			prev_ticks = pInfo->StopTicks;
        }
        else
        {
            uart_printf ("    (%d), s:[0x%08x], e:[0x%08x], t:[%d]\r\n", 
                pInfo->IrqID, pInfo->StartTicks, pInfo->StopTicks, pInfo->StopTicks - pInfo->StartTicks);
        }
        
        MxDelayTicks(20000);
    }
    
    uart_printf ("========================================================\r\n" );
    MxDelayTicks(20000);
}

void TimerDrvIrqHandler(volatile UINT32 * pClear, UINT32 nIrqID, LPVOID pParam)
{
    PMXCONTEXT pCtx;
    TimerDrvCtx *pDrvCtx = TimerDrvGetCtx();
    IRQTYPE r = ARM_INT_disable();
	UINT32 t;

    pCtx = MxInqContext();
    _ASSERT_PTR(pCtx);

     _ASSERT_PTR(pDrvCtx);

    pDrvCtx->Regs->TMStatus = 1;
    *pClear = nIrqID;

    pDrvCtx->ProcFracVal ++;

    MxProcFractionTimers(pCtx);

    if (pDrvCtx->ProcFracVal == pDrvCtx->Fraction)
    {
        pDrvCtx->ProcFracVal = 0;
    
        t = MxGetTicks ();
    	TmrCount ++;

    	if (TimerDrvCheck)
        {
            if (TmrTicks != 0)
            {
    		if ((t - TmrTicks) < 149000)
    		{
    			TmrWarning ++;
    		}
    		
                if (t - TmrTicks < 140000)
                {
                    uart_printf ("TIMER-DRV#2: non-RT is detected!!!! (0x%x - 0x%x = %d ticks), irq-num=%d\r\n", t, TmrTicks, t - TmrTicks, TmrCount);
                    TmrDrvShowIrqTrace ();
                    while (1);
                }

                TmrTicks = t;
            }
            else
            {
                TmrTicks = t;  
            }
        }

        if (pDrvCtx->Proc != NULL)
        {
            if (pDrvCtx->SkipMs == 0)
            {
                pDrvCtx->CurrentVal++;

                if (pDrvCtx->CurrentVal == pDrvCtx->Period)
                {
                    pDrvCtx->Proc (pDrvCtx->lpProcData);
                }

                if (pDrvCtx->CurrentVal >= pDrvCtx->Period)
                    pDrvCtx->CurrentVal = 0;
            }
            else
            {
                pDrvCtx->SkipMs--;
            }
        }

        MxProcTimers(pCtx);
    }

    ARM_INT_restore(r);
}

MXRC TimerDrvWaitForTicks (UINT32 nNumIrqSkip)
{
    IRQTYPE r;
    //UINT32 wait = 0;

    if ((TimerDrvGetCtrl() & TIMER_CTRL_ENABLE) == 0)
    {
        return TIMER_RC_DRV_IS_DISABLED;
    }

    r = ARM_INT_disable();

    // just to wait the tick in the same 
    // loop where we are waiting and skipping
    // interrupts
    
    nNumIrqSkip += 1;

    while (nNumIrqSkip)
    {
        while(1)
        {
            if (TimerDrvGetStatus() & 1)
                break;
        }

        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();

        TimerDrvSetStatus(1);
        
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();

        ICDrvClearPendIrq(HAL_IRQ_LOCAL_TIMER);
        
        nNumIrqSkip--;
    }

    ARM_INT_restore(r);

    return MX_OK;
}

MXRC TimerDrvReinitTimer(void)
{
    IRQTYPE irq;
    UINT32 nArmFrq;
    TimerDrvCtx *pDrvCtx = TimerDrvGetCtx();

    irq = ARM_INT_disable();

    nArmFrq = ClkRstGetFreq (ARMx4_CORE0);

    _ASSERT_PTR(pDrvCtx);

    // to disable the timer
    //--------------------------------
    pDrvCtx->Regs->TMControl  = 0;
    pDrvCtx->Regs->TMLoadVal  = (UINT32)(nArmFrq-1)/(2000*MX_TIMER_FRACTION);
    pDrvCtx->Regs->TMCounter  = 0;
    pDrvCtx->Fraction         = MX_TIMER_FRACTION;
    pDrvCtx->ProcFracVal      = 0;

    // to ACK possible IRQ set before this function
    // --------------------------------------------
    
    pDrvCtx->Regs->TMStatus = 1;
    ICDrvClearPendIrq(HAL_IRQ_LOCAL_TIMER);

    //MxDelayTicks(US_TO_TICKS(10));

    // to reinitialize the timer
    // --------------------------
    pDrvCtx->Regs->TMControl = (TIMER_CTRL_ENABLE | TIMER_CTRL_AUTO | TIMER_CTRL_INT_ENABLE);

    ARM_INT_restore(irq);

    return MX_OK;
}

MXRC TimerDrvEnableCheck(BOOL b)
{
	TimerDrvCheck = b;

	if (b == 0)
		TmrTicks = 0;

    ICDrvEnableIrqTrace (b);

    return MX_OK;
}

UINT32 TimerDrvGetWarningNum (void)
{
	return TmrWarning;
}

MXRC TimerDrvShutdown(void)
{
    TimerDrvSetCtrl(0);
    TimerDrvWDShutdown();

    return TIMER_RC_OK;
}

MXRC TimerDrvSetProc (UINT32 nPeriod, UINT32 SkipMs, TIMERPROC proc, LPVOID lpData)
{
    TimerDrvCtx *pDrvCtx = TimerDrvGetCtx();
    IRQTYPE irq;

    _ASSERT_PTR(pDrvCtx);

    if ((nPeriod == 0 && proc != NULL) || pDrvCtx == NULL)
    {
        _ASSERT ((nPeriod != 0));
        return TIMER_RC_PARAM_ERROR;
    }

    irq = ARM_INT_disable();

    pDrvCtx->Proc       = proc;
    pDrvCtx->lpProcData = lpData;
    pDrvCtx->Period     = nPeriod;
    pDrvCtx->CurrentVal = 0;
    pDrvCtx->SkipMs     = SkipMs;

    ARM_INT_restore(irq);

    return TIMER_RC_OK;
}

/** @brief The handler processes IRQ interrupts of watchdog timer

 @param pClear [in] - the pointer to interrupt clear register
 @param pParam [in] - the ISR parameter
 \ingroup group_lte_services
 */

void TimerDrvWDIrqHandler(volatile UINT32 * pClear, UINT32 nIrqID, LPVOID pParam)
{
    TimerDrvCtx *pDrvCtx = (TimerDrvCtx *) pParam;
    
#ifdef _MX_DEBUG
    PMXCONTEXT pCtx;
    pCtx = MxInqContext();
#endif
    _ASSERT_PTR(pCtx);

    pDrvCtx->Regs->WDStatus = 1;

    if (pDrvCtx->WDProc != NULL)
    {
        pDrvCtx->WDCurrentVal++;

        if (pDrvCtx->WDCurrentVal == pDrvCtx->WDPeriod)
        {
            pDrvCtx->WDProc (pDrvCtx->WDlpProcData);
        }

        if (pDrvCtx->WDCurrentVal >= pDrvCtx->WDPeriod)
            pDrvCtx->WDCurrentVal = 0;
    }

    *pClear = nIrqID;
}

MXRC TimerDrvWDShutdown(void)
{
    TimerDrvWDSetCtrl(0);
    return MX_OK;
}

MXRC TimerDrvWDSetProc (UINT32 nPeriod, TIMERPROC proc, LPVOID lpData)
{
    TimerDrvCtx *pDrvCtx = TimerDrvGetCtx();
    IRQTYPE irq;

    _ASSERT_PTR(pDrvCtx);

    if ((nPeriod == 0 && proc != NULL) || pDrvCtx == NULL)
    {
        _ASSERT ((nPeriod != 0));
        return TIMER_RC_PARAM_ERROR;
    }

    irq = ARM_INT_disable();

    pDrvCtx->WDProc       = proc;
    pDrvCtx->WDlpProcData = lpData;
    pDrvCtx->WDPeriod     = nPeriod;
    pDrvCtx->WDCurrentVal = 0;

    ARM_INT_restore(irq);

    return TIMER_RC_OK;
}

MXRC TimerDrvWD_init(UINT32 nInit)
{
    TimerDrvCtx *pDrvCtx;

#ifdef _MX_DEBUG
    PMXCONTEXT pMx;

    pMx = MxInqContext();

    _ASSERT(pMx != NULL);
#endif

    pDrvCtx = TimerDrvGetCtx ();

    if (pDrvCtx == NULL)
    {
        _ASSERT_PTR(pDrvCtx);
        return TIMER_RC_DRV_ALLOC_ERROR;
    }

    pDrvCtx->Regs = (MpCoreLocTimerWDReg *) nInit;

    // To disable the watchdog H/W

    pDrvCtx->Regs->WDDisable = 0x12345678;
    pDrvCtx->Regs->WDDisable = 0x87654321;

    pDrvCtx->Regs->WDControl = 0x0;

    // Reset statuses if any
    pDrvCtx->Regs->WDStatus  = 1;
    pDrvCtx->Regs->WDReset  = 1;

    // To prevent usage of local timer interrupt on L-ARM
    if (MxGetARMID () == 0)
    {
	    MxSetIntHandler(HAL_IRQ_LOCAL_TIMER_WD, INT_TYPE_IRQ, PRI_TIMER, TimerDrvWDIrqHandler,
	                    pDrvCtx);
	    MxEnableInt(HAL_IRQ_LOCAL_TIMER_WD);
    }

    return TIMER_RC_OK;
}

/** @brief This function initializes ARM-local timer for 1ms/MX_TIMER_FRACTION
           period of time 

    @return [MXRC] an error code */

MXRC TmrDrvInitLocalTimer (void)
{
#if defined(LOCAL_TIMER_INIT)
    TimerDrvCtx *pDrvCtx;
    UINT32 nArmFrq;
    IRQTYPE r;

    #ifdef _MX_DEBUG
        PMXCONTEXT pMx;

        pMx = MxInqContext();
        _ASSERT(pMx != NULL);    

    #endif

    pDrvCtx = TimerDrvGetCtx ();

#if 0
    //if timer is not enabled on this core
    if (((1 << MxGetCpuID ()) & MX_TIMER_CORES)== 0)
        return MX_OK;
#endif

    r = ARM_INT_disable();

    nArmFrq = ClkRstGetFreq (ARMx4_CORE0);

    _ASSERT_PTR(pDrvCtx);

    //uart_printf("ARM-TIMER:0x%08x\r\n", pDrvCtx->Regs);

    pDrvCtx->Regs->TMControl  = 0;     //  0 - prescale value 
    pDrvCtx->Regs->TMLoadVal  = (UINT32)(nArmFrq - 1)/(2000 * MX_TIMER_FRACTION);
    pDrvCtx->Fraction         = MX_TIMER_FRACTION;
    pDrvCtx->ProcFracVal      = 0;

     //uart_printf ("LOCAL IRQ:%d, fraction=%d\r\n", HAL_IRQ_LOCAL_TIMER, MX_TIMER_FRACTION);
     pDrvCtx->Regs->TMControl = (TIMER_CTRL_ENABLE | TIMER_CTRL_AUTO | TIMER_CTRL_INT_ENABLE);

    pDrvCtx->bInited = TRUE;

    ARM_INT_restore(r);
#endif 
    return MX_OK;
}


MXRC TimerDrv_init(UINT32 nInit, LPVOID * pCtx)
{
    TimerDrvCtx *pDrvCtx;

#ifdef _MX_DEBUG
    PMXCONTEXT pMx;

    pMx = MxInqContext();

    _ASSERT(pMx != NULL);
#endif

    //pDrvCtx = (TimerDrvCtx *) MxHeapAlloc(HNULL, sizeof(TimerDrvCtx));
    pDrvCtx = TimerDrvGetCtx ();

    if (pDrvCtx == NULL)
    {
        _ASSERT_PTR(pDrvCtx);
        return TIMER_RC_DRV_ALLOC_ERROR;
    }

    memset (pDrvCtx, 0, sizeof (*pDrvCtx));

    *pCtx = pDrvCtx;

    pDrvCtx->Regs = (MpCoreLocTimerWDReg *) nInit;

    pDrvCtx->Regs->TMControl = 0x0;
    pDrvCtx->Regs->TMStatus  = 1;

    MxSetIntHandler(HAL_IRQ_LOCAL_TIMER, 
                    INT_TYPE_IRQ, 
                    PRI_TIMER, 
                    TimerDrvIrqHandler,
                    pDrvCtx);
    
    MxEnableInt(HAL_IRQ_LOCAL_TIMER);

    //TimerDrvWD_init(nInit);

    return TIMER_RC_OK;
}

MXRC TimerDrv_config(LPVOID pDrv, UINT32 nCfg, LPVOID pCfgData,
                     UINT32 nCfgDataSize)
{
    TimerDrvCtx *pCtx;
    MpCoreLocTimerWDReg *Regs;

    _ASSERT(pDrv != NULL);

    pCtx = (TimerDrvCtx *) pDrv;
    Regs = pCtx->Regs;
		
    switch (nCfg)
    {
        case TIMER_CMD_CONFIG:
            {
                TimerConfig *pCfg = (TimerConfig *) pCfgData;

                _ASSERT(pCfg != NULL);

                pCfg->SBZ1 = 0;
                pCfg->SBZ2 = 0;

                Regs->TMLoadVal = *((UINT32 *) pCfg + 1);
                Regs->TMControl = *(UINT32 *) pCfg;

                break;
            }

        case TIMER_CMD_SET_PERIOD:
            {
                Regs->TMLoadVal   = (UINT32) pCfgData;
                pCtx->Fraction    = nCfgDataSize;
                pCtx->ProcFracVal = 0;

                break;
            }
        case TIMER_CMD_RESTART:
            {
                Regs->TMLoadVal = Regs->TMLoadVal;

                break;
            }
        case TIMER_CMD_SET_CTRL:
            {
                Regs->TMControl =
                    (Regs->TMControl & ((UINT32) 0xFF << 8)) |
                     (((UINT32) pCfgData) & 0x7);

                break;
            }
        case TIMER_CMD_SET_PRESCALE:
            {
                Regs->TMControl =
                    (Regs->TMControl & ~(0xFF << 8)) |
                    (((UINT32) pCfgData) << 8);

                break;
            }
        case TIMER_CMD_ENABLE:
            {
                if ((UINT32) pCfgData)
                {
                    Regs->TMControl |= TIMER_CTRL_ENABLE;
                }
                else
                {
                    Regs->TMControl &= ~TIMER_CTRL_ENABLE;
                }

                break;
            }


        case TIMER_WD_CMD_CONFIG:
            {
                TimerWDConfig *pCfg = (TimerWDConfig *) pCfgData;

                _ASSERT(pCfg != NULL);

                pCfg->SBZ1 = 0;
                pCfg->SBZ2 = 0;

                Regs->WDLoad = *((UINT32 *) pCfg + 1);
                Regs->WDControl = *(UINT32 *) pCfg;

                break;
            }

        case TIMER_WD_CMD_SET_PERIOD:
            {
                Regs->WDLoad = (UINT32) pCfgData;

                break;
            }
        case TIMER_WD_CMD_RESTART:
            {
                Regs->WDLoad = Regs->WDLoad;

                break;
            }
        case TIMER_WD_CMD_SET_CTRL:
            {
                Regs->WDControl =
                    (Regs->WDControl & ((UINT32) 0xFF << 8)) |
                     (((UINT32) pCfgData) & 0xF);

                break;
            }
        case TIMER_WD_CMD_SET_PRESCALE:
            {
                Regs->WDControl =
                    (Regs->WDControl & ~(0xFF << 8)) |
                    (((UINT32) pCfgData) << 8);

                break;
            }
        case TIMER_WD_CMD_DIS_WDMODE:
            {
                Regs->WDDisable = 0x12345678;
                Regs->WDDisable = 0x87654321;
                break;
            }
        case TIMER_WD_CMD_CLEAR_WDRESET:
            {
                Regs->WDReset = 1;
                break;
            }
        case TIMER_WD_CMD_ENABLE:
            {
                if ((UINT32) pCfgData)
                {
                    Regs->WDControl |= TIMER_CTRL_ENABLE;
                }
                else
                {
                    Regs->WDControl &= ~TIMER_CTRL_ENABLE;
                }

                break;
            }

        case TIMER_CMD_INIT_LOCAL_TIMER:
            {
                TmrDrvInitLocalTimer();
                break;
            }
            
        default:
            return TIMER_RC_DRV_UNSUPP_CMD;
    }

    return TIMER_RC_OK;
}

MXRC TimerDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                    UINT32 nCmdDataSize)
{
    return TIMER_RC_DRV_UNSUPP_CMD;
}

MXRC TimerDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                   UINT32 * pnCmdDataSize)
{
    TimerDrvCtx *pCtx;
    MpCoreLocTimerWDReg *Regs;
    IRQTYPE irq;

    _ASSERT(pDrvCtx != NULL);

    pCtx = (TimerDrvCtx *) pDrvCtx;
    Regs = pCtx->Regs;

    switch (nCmd)
    {
        case TIMER_CMD_GET_PERIOD:
            {
                _ASSERT_PTR(pCmdData);

                *((UINT32*)pCmdData) = Regs->TMLoadVal;

                break;
            }

        case TIMER_CMD_GET_TICKS:
            {
                _ASSERT_PTR(pCmdData);

                irq = ARM_INT_disable();

                *((UINT32*)pCmdData) = Regs->TMCounter;

                ARM_INT_restore(irq);

                break;
            }

        case TIMER_CMD_GET_FREQ:
            {
                _ASSERT_PTR(pCmdData);

                // the timer is arranged for 1ms

                *((UINT32*)pCmdData) = Regs->TMLoadVal * 1000;

                break;
            }

        case TIMER_CMD_GET_CONTROL:
            {
                _ASSERT_PTR(pCmdData);

                *((UINT32*)pCmdData) = Regs->TMControl;

                break;
            }


        case TIMER_WD_CMD_GET_PERIOD:
            {
                _ASSERT_PTR(pCmdData);

                *((UINT32*)pCmdData) = Regs->WDLoad;

                break;
            }

        case TIMER_WD_CMD_GET_TICKS:
            {
                _ASSERT_PTR(pCmdData);

                irq = ARM_INT_disable();

                *((UINT32*)pCmdData) = Regs->WDCounter;

                ARM_INT_restore(irq);

                break;
            }

        case TIMER_WD_CMD_GET_CONTROL:
            {
                _ASSERT_PTR(pCmdData);

                *((UINT32*)pCmdData) = Regs->WDControl;

                break;
            }

        case TIMER_CMD_GET_STATUS:
            {
                _ASSERT_PTR(pCmdData);

                *((UINT32*)pCmdData) = pCtx->bInited;
                break;
            }

        default:
            return TIMER_RC_DRV_UNSUPP_CMD;
    }

    return TIMER_RC_OK;
}

