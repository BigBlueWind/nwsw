//-------------------------------------------------------------------------------------------
/** @file timerdrv.h
 *
 * @brief This file contains local timer driver "H" code
 * @author Mindspeed Technologies
 * @version $Revision: 1.17 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _TIMER_DRV_H_
#define _TIMER_DRV_H_

#include "4gmx.h"
#include "appids.h"
#include "hal.h"
#include "clkrst.h"
#include "icdrv.h"

/********************************************************************
**  The error codes of module, used by interface functions of module*
*********************************************************************/
#define TIMER_RC_OK                     MX_OK
#define TIMER_RC_PARAM_ERROR            DEF_USRRC(APP_DRIVER, LOCAL_TIMER_DRV, 1)
#define TIMER_RC_DRV_ALLOC_ERROR        DEF_USRRC(APP_DRIVER, LOCAL_TIMER_DRV, 2)
#define TIMER_RC_DRV_UNSUPP_CMD         DEF_USRRC(APP_DRIVER, LOCAL_TIMER_DRV, 3)
#define TIMER_RC_DRV_IS_DISABLED        DEF_USRRC(APP_DRIVER, LOCAL_TIMER_DRV, 4)

#define LOCAL_TIMER_ADDR            (0xFFF00600)

#define LOCAL_TIMER_PERIOD          TimerDrvGetPeriod ()

typedef struct _MPCORE_LOC_TMWD_
{
    VUINT32 TMLoadVal;  /**< Timer Load Register    */
    VUINT32 TMCounter;  /**< Timer Counter Register    */
    VUINT32 TMControl;  /**< Timer Control Register    */
    VUINT32 TMStatus;   /**< Timer Interrupt Status Register */

    VUINT32 Res [4];

    VUINT32 WDLoad;     /**< Watchdog Load Register    */
    VUINT32 WDCounter;  /**< Watchdog Counter Register   */
    VUINT32 WDControl;  /**< Watchdog Control Register   */
    VUINT32 WDStatus;   /**< Watchdog Interrupt Status Register */
    VUINT32 WDReset;    /**< Watchdog Reset Sent Register  */
    VUINT32 WDDisable;  /**< Watchdog Disable Register   */

} MpCoreLocTimerWDReg;

typedef void (*TIMERPROC)(LPVOID lpData);

/** @brief  The context of timer driver used for
   storing internal parameters of driver parameters */

typedef struct _TIMER_DRV_CTX_
{
    MpCoreLocTimerWDReg *Regs;
    TIMERPROC            Proc;
    LPVOID               lpProcData;
    UINT32               Period;
    UINT32               CurrentVal;
    UINT32               SkipMs;

    TIMERPROC            WDProc;
    LPVOID               WDlpProcData;
    UINT32               WDPeriod;
    UINT32               WDCurrentVal;

    UINT32               Fraction;
    UINT32               ProcFracVal;

    BOOL                 bInited; 
} TimerDrvCtx;


typedef struct _TIMER_CONFIG_
{
    UINT32 Enable    :1;
    UINT32 AutoReload:1;
    UINT32 IntEnable :1;
    UINT32 SBZ1      :5;
    UINT32 Prescale  :8;
    UINT32 SBZ2      :16;
    UINT32 Counter   :32;
} TimerConfig;

typedef struct _TIMERWD_CONFIG_
{
    UINT32 Enable    :1;
    UINT32 AutoReload:1;
    UINT32 IntEnable :1;
    UINT32 WDMode :1;
    UINT32 SBZ1      :4;
    UINT32 Prescale  :8;
    UINT32 SBZ2      :16;
    UINT32 Counter   :32;
} TimerWDConfig;

/************************************************************/
/*      TIMER_CMD_SET_CTRL                                  */
/************************************************************/

#define TIMER_CTRL_ENABLE     (1 << 0)
#define TIMER_CTRL_AUTO       (1 << 1)
#define TIMER_CTRL_INT_ENABLE (1 << 2)
#define TIMER_WD_CTRL_WDMODE (1 << 3)

INLINE void TimerDrvSetCtrl (UINT32 nCtrl)
{
    ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->TMControl = nCtrl;
}

INLINE UINT32 TimerDrvGetCtrl (void)
{
    return ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->TMControl;
}

INLINE void TimerDrvSetStatus (UINT32 nStatus)
{
    ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->TMStatus = nStatus;
}

INLINE UINT32 TimerDrvGetStatus (void)
{
    return ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->TMStatus;
}

INLINE void TimerDrvRestart (void)
{
    MpCoreLocTimerWDReg * pReg = (MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR; 
    
    pReg->TMCounter = pReg->TMLoadVal;
    pReg->TMLoadVal = pReg->TMLoadVal;

    if (pReg->TMStatus & 1)
    {
        pReg->TMStatus  = 1;
        ICDrvClearPendIrq(HAL_IRQ_LOCAL_TIMER);
    }
}

INLINE void TimerDrvSetPeriod (UINT32 nVal)
{
    ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->TMLoadVal = nVal;
}

INLINE UINT32 TimerDrvGetTicks (void)
{
    return ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->TMCounter;
}

INLINE UINT32 TimerDrvGetPeriod (void)
{
    return ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->TMLoadVal;
}

INLINE UINT32 TimerDrvGetFreq (void)
{
    return ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->TMLoadVal * 1000;
}

INLINE void TimerDrvWDSetCtrl (UINT32 nCtrl)
{
    ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->WDControl = nCtrl;
}

INLINE void TimerDrvWDRestart (void)
{
    ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->WDLoad =
        ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->WDLoad;
}

INLINE void TimerDrvWDSetPeriod (UINT32 nVal)
{
    ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->WDLoad = nVal;
}

INLINE void TimerDrvWDEnaWDMode (void)
{
    ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->WDControl |= TIMER_WD_CTRL_WDMODE;
}

INLINE void TimerDrvWDDisWDMode (void)
{
    ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->WDDisable = 0x12345678;
    ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->WDDisable = 0x87654321;
}

INLINE void TimerDrvWDClearWDReset (void)
{
    ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->WDReset = 1;
}

INLINE UINT32 TimerDrvWDGetWDReset (void)
{
	return ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->WDReset;
}

INLINE void TimerDrvWDSetWDReset (UINT32 val)
{
	((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->WDReset = val;
}

INLINE UINT32 TimerDrvWDGetTicks (void)
{
    return ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->WDCounter;
}

INLINE UINT32 TimerDrvWDGetPeriod (void)
{
    return ((MpCoreLocTimerWDReg*)LOCAL_TIMER_ADDR)->WDLoad;
}

MXRC TimerDrvWaitForTicks (UINT32 nNumIrqSkip);
MXRC TimerDrvReinitTimer(void);
MXRC TimerDrvEnableCheck(BOOL b);
UINT32 TimerDrvGetWarningNum (void);

MXRC TimerDrvShutdown(void);
MXRC TimerDrvWDShutdown(void);

MXRC TimerDrvSetProc (UINT32 nPeriod, UINT32 SkipMs, TIMERPROC proc, LPVOID lpData);
MXRC TimerDrvWDSetProc (UINT32 nPeriod, TIMERPROC proc, LPVOID lpData);

/** @brief This function initializes ARM-local timer for 1ms/MX_TIMER_FRACTION
           period of time 

    @return [MXRC] an error code */

MXRC TmrDrvInitLocalTimer (void);

MXRC TimerDrv_init(UINT32 nInit, LPVOID * pCtx);

MXRC TimerDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData,
                     UINT32 nCfgDataSize);

MXRC TimerDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                    UINT32 nCmdDataSize);

MXRC TimerDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                   UINT32 * pnCmdDataSize);

#endif /* _TIMER_DRV_H_ */

#ifdef __cplusplus
}
#endif

