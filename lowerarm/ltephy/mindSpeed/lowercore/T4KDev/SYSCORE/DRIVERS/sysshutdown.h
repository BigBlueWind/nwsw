//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL sysshutdown.h
 *
 * @brief Implementation of the system shutdown functionality
 * @author Mindspeed Technologies
 * @version $Revision: 1.11 $
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

#include "config.h"

#ifndef _SYS_SHDOWN_H_
#define _SYS_SHDOWN_H_

#include "4gmx.h"
#include "hal.h"

#define PAR_ALARM_IDX     0xE200
#define PAR_ALARM_SEVERITY     0xE201
#define PAR_ALARM_DATA     0xE202

#define PAR_ALARM_ABORT_FRM0   0xE208
#define PAR_ALARM_ABORT_FRM1   0xE209
#define PAR_ALARM_ABORT_FRM2   0xE20A
#define PAR_ALARM_ABORT_FRM3   0xE20B
#define PAR_ALARM_STACK   0xE20F
#define PAR_ALARM_GIC_DIST   0xE217
#define PAR_ALARM_GIC_CPU0   0xE218
#define PAR_ALARM_GIC_CPU1   0xE219
#define PAR_ALARM_GIC_CPU2   0xE21A
#define PAR_ALARM_GIC_CPU3   0xE21B


#define ALARM_IDX_DABT      0xA001
#define ALARM_IDX_PABT      0xA002
#define ALARM_IDX_UNDEF     0xA003

#define ALARM_SEVERITY_INFO         0
#define ALARM_SEVERITY_SERIOUS      0x80
#define ALARM_SEVERITY_FATAL        0x100


typedef struct {
        VUINT32 r0;
        VUINT32 r1;
        VUINT32 r2;
        VUINT32 r3;
        VUINT32 r4;
        VUINT32 r5;
        VUINT32 r6;
        VUINT32 r7;
        VUINT32 r8;
        VUINT32 r9;
        VUINT32 r10;
        VUINT32 r11;
        VUINT32 r12;
        VUINT32 sp;
        VUINT32 lr;
        VUINT32 pc;
        VUINT32 instr;
        VUINT32 spsr;
        VUINT32 cpu;
        VUINT32 cpsr;   // 20 * 4 = 80 - filled by asm
        VUINT32 AbortType;
        VUINT32 Severity;
} ABORT_FRAME, *PABORT_FRAME;

void SysShdwnProcAbort (UINT16 Type);

void SysShutdownIRQHandler (VUINT32 * pClear, UINT32 nIrq, LPVOID pData);

MXRC SysShdwnDrv_init(UINT32 nInit, LPVOID * pDrvCtx);

INLINE MXRC SysShdwnDrv_config(LPVOID pDrvCtx, UINT32 nCfg, LPVOID pCfgData,
                    UINT32 nCfgDataSize) {return MX_OK;}

INLINE MXRC SysShdwnDrv_write(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                   UINT32 nCmdDataSize) {return MX_OK;}

INLINE MXRC SysShdwnDrv_read(LPVOID pDrvCtx, UINT32 nCmd, LPVOID pCmdData,
                  UINT32 * pnCmdDataSize) {return MX_OK;}

#endif // _SYS_SHDOWN_H_

#ifdef __cplusplus
}
#endif

