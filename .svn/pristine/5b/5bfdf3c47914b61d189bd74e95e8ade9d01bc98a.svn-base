//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL appinit.h
 *
 * @brief This file contains declaration of the 4GMX contexts, drivers table for every
*       one CPU, and table of application initialization handlers
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

#ifndef _APP_INIT_H_
#define _APP_INIT_H_

#include "config.h"
#include "mlog.h"

typedef struct APPINITCTX
{
    const MXDRV *MxCpuDrv;         /**< The pointer to the table of drivers this current processor     */
    UINT32 ICDrvId;          /**< The index of interrupt controller driver          */
    UINT32(*Idle)(LPVOID);   /**< The pointer to the IDLE function used by the 4GMX system, it can be NULL */
    LPVOID IdleParam;        /**< The pointer to the IDLE function parameter         */
    MXRC(*SysInit)(UINT32 nCpuId, MXRC (*ExtAppInit) (void));
    void (*ThrTracer) (ExtTracer id);
} AppInitCtx;

typedef struct _CpuTraceType_
{
    UINT32 nCurMode;        // The current CPU mode, see: MLOG_MODE_xxx
}CpuTraceType;

typedef struct _CpuMIPS_
{
    UINT32 RefTicks;
    UINT32 Ticks;
    UINT32 BeginTime;
}CpuMIPSInfo;

typedef struct _SysStayInfo_
{
    BOOL   bIrqMode;
    BOOL   bFiqMode;
    BOOL   bDataMode;
    BOOL   bUndefMode;
    BOOL   bPrefMode;
    BOOL   bSWIMode;

    UINT32 hThread;     // Currently executed thread function
    UINT32 nIrqID;      // The interrupt ID
    UINT32 nExceptionLR;
    UINT32 isr;         //ISR of latest IRQ  

    /* context of core before  last IRQ */ 
    UINT32      r[16];
    UINT32      cpsr;
    UINT32      spsr;
    /* storage for problem specific debug information */
//    UINT32  debugVar[8];
    UINT32  mBuf;
    UINT32  event;
    UINT32  dstEnt;
    UINT32  dstInst;
    UINT32  srcEnt;
    UINT32  timestamp;
    UINT32  caller[4];
    UINT8   res[8];
}SysStayInfo;

extern MXCONTEXT MxCtx[];
extern AppInitCtx appinit[];

#ifdef MX_INTERCPU_ENABLED
extern MXICCTX   MxICCtx [];    // The inter-cpu contextes of the 4GMX systems
extern MXICCTX * MxICTable [];  // The table of the pointers to the IC contextes
#endif //MX_INTERCPU_ENABLED

#ifdef SYS_INFO_ENABLED
extern SysStayInfo syscoreinfo [];
#endif

MXRC AppInitCpu0(UINT32 cpuid, MXRC (*ExtAppInit) (void));
UINT32 IdleCpu0(LPVOID p);
void AppThrTracer (ExtTracer id);

#ifdef CPU1_ENABLED
MXRC AppInitCpu1(UINT32 cpuid, MXRC (*ExtAppInit) (void));
UINT32 IdleCpu1(LPVOID p);
#endif /*CPU1_ENABLED */

#ifdef CPU2_ENABLED
MXRC AppInitCpu2(UINT32 cpuid, MXRC (*ExtAppInit) (void));
UINT32 IdleCpu2(LPVOID p);
#endif /*CPU2_ENABLED */

#ifdef CPU3_ENABLED
MXRC AppInitCpu3(UINT32 cpuid, MXRC (*ExtAppInit) (void));
UINT32 IdleCpu3(LPVOID p);
#endif /*CPU3_ENABLED */

#ifdef MLOG_IRQ_SUP_ENABLE
#define GetCpuTraceMode()       _GetCpuTraceMode()
#define SetCpuTraceMode(nMode)  _SetCpuTraceMode((nMode))
#else
#define GetCpuTraceMode()       0
#define SetCpuTraceMode(nMode)
#endif

#define EXT_THR_TRACER_PROC AppThrTracer

#endif /*_APP_INIT_H_*/

#ifdef __cplusplus
}
#endif

