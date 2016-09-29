//-------------------------------------------------------------------------------------------
/** @file systimers.h
 *
 * @brief System Timers implementation
 * @author Mindspeed Technologies
 * @version $Revision: 1.12 $
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

#ifndef __SYSTIMERS_H__
#define __SYSTIMERS_H__

#include "4gmx.h"
#include "hal.h"

#define SYSTICK_1_MS    	150000
#define WD_HW_PERIOD_TICK 	(ClkRstGetFreq (ARMx4_CORE0)*2)
#define WD_TH_PERIOD_MS		500

typedef struct
{
    UINT32  nInitVal;
    UINT32  nInited;
    VUINT32 nProcTimer;
	HANDLE hWDevnt;
	HANDLE hWDtimer;
	HANDLE hWDThread[CPU_NUM];
	UINT32 WDPeriod[CPU_NUM];

    void (*T1CallBack) (void * pData);
    void * T1CallBackParam;

    void (*T2CallBack) (void * pData);
    void * T2CallBackParam;

}SysTimerCtx;

#define XP_TIMER_IRQ_HANDLER_ENABLED

#define SYSTIMER0_MASK (1 << 0)
#define SYSTIMER1_MASK (1 << 1)
#define SYSTIMER2_MASK (1 << 2)
#define SYSTIMER3_MASK (1 << 3)

#define SysTimerGetTick() REG32(TIMER0_CURR_COUNT)

#define SYSTIMER_RC_OK MX_OK

SysTimerCtx *SysTimerGetCtx(void);

MXRC SysTimerDrv_init(UINT32 nInit, LPVOID * pCtx);

INLINE MXRC SysTimerDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData,
                     UINT32 nCfgDataSize) {return MX_OK;}

INLINE MXRC SysTimerDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                    UINT32 nCmdDataSize) {return MX_OK;}

INLINE MXRC SysTimerDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                   UINT32 * pnCmdDataSize) {return MX_OK;}

void SysTimer1Set(UINT32 TimerValue);
void SysTimer1SetCallBack (void (* pCB) (void *) ,  void * pParam );
void SysTimer1EnableIRQ(void);
void SysTimer1DisableIRQ(void);
void SysTimer2Set(UINT32 TimerValue);
void SysTimer2SetCallBack (void (* pCB) (void *) ,  void * pParam );
void SysTimer2EnableIRQ(void);
void SysTimer2DisableIRQ(void);
MXRC SysTimersStart(void);
MXRC SysTimerStartWithDiag (void);
MXRC SysTimersShutdown (void);
MXRC SysTimerInitWD(void);
void SysTimerMmuCfg(UINT32 nCpuID, UINT32 nParam);

#endif /* __SYSTIMERS_H__ */

#ifdef __cplusplus
}
#endif

