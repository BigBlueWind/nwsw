//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL sysshutdown.c
 *
 * @brief Implementation of the system shutdown functionality
 * @author Mindspeed Technologies
 * @version $Revision: 1.71 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "sysshutdown.h"
#include "sirqassign.h"
#include "appprintf.h"

#include "timerdrv.h"
#include "icdrv.h"
#include "uartdrv.h"
#include "FECdrv.h"
#include "mdmadrv.h"
#include "cpridrv.h"
#include "fpdrv.h"

#include "ncnbdata.h"
#include "systimers.h"
#include "supervisor.h"
#include "pciedrv.h"
//#include "cache-l2x0.h"
#include "spaccdrv.h"
#include "sysddrctrl.h"
#include "rex.h"
#include "dbgfunctab.h"
#include "mlog.h"

extern void MxiFlushIDCache (void);

static UINT32 SysShdwnGetUsedStackLength (UINT32 Cpu, VUINT32 pSP)
{// it should return how many bytes of stack were used
    UINT32 Len = 0;
    STACKSHEAP* pStackHeap;

    switch (Cpu)
    {
        case 0:
            pStackHeap = &stacks_cpu0;
            break;

        case 1:
            pStackHeap = &stacks_cpu1;
            break;

        case 2:
            pStackHeap = &stacks_cpu2;
            break;

        case 3:
            pStackHeap = &stacks_cpu3;
            break;

        default:
            pStackHeap = &stacks_cpu0;
            break;
    }

    if(pSP <= (UINT32)pStackHeap->abort)
    {// supervisor
        Len = (UINT32)pStackHeap->abort - pSP;
        uart_printf("SVSR stack used SP=%X UsedLen=%d\r\n", pSP, Len);
    }
    else if(pSP <= (UINT32)pStackHeap->undef)
    {// abort
        Len = (UINT32)pStackHeap->undef - pSP;
        uart_printf("ABORT stack used SP=%X UsedLen=%d\r\n", pSP, Len);
    }
    else if(pSP <= (UINT32)pStackHeap->fiq)
    {// undef
        Len = (UINT32)pStackHeap->fiq - pSP;
        uart_printf("UNDEF stack used SP=%X UsedLen=%d\r\n", pSP, Len);
    }
    else if(pSP <= (UINT32)pStackHeap->irq)
    {// fiq
        Len = (UINT32)pStackHeap->irq - pSP;
        uart_printf("FIQ stack used SP=%X UsedLen=%d\r\n", pSP, Len);
    }
    else if(pSP <= ((UINT32)pStackHeap->irq + IRQ_STACK_SIZE))
    {// irq
        Len = (UINT32)pStackHeap->irq + IRQ_STACK_SIZE - pSP;
        uart_printf("IRQ stack used SP=%X UsedLen=%d\r\n", pSP, Len);
    }

    return Len;
}

static void SysShdwnSendAlarm(UINT32 DumpCpu)
{
    ApiHeader* pApi;
    PABORT_FRAME pAbortCtx;
    SINT32 DumpStackLen;

    switch (DumpCpu)
    {
        case 0:
            pAbortCtx = &gAbortCtx0;
            break;

        case 1:
            pAbortCtx = &gAbortCtx1;
            break;

        case 2:
            pAbortCtx = &gAbortCtx2;
            break;

        case 3:
            pAbortCtx = &gAbortCtx3;
            break;

        default:
            pAbortCtx = &gAbortCtx0;
            break;

    }

    if ((pApi = SvsrAllocICApi()) == NULL)
    {
        uart_printf("Alarm: can't alloc ICPU block\r\n");
        return;
    }

    ApiCreate(pApi,
            API_VERSION(API_SUPPORTED_VERSION),
            IID_SUPERVISOR,
            UARM_INSTID_HOST,
            API_TYPE_SYSINFO,
            DIAG_ALARM_IND);

    ApiAddParam(pApi,
                NULL,
                PAR_ALARM_IDX,
                (LPVOID)&pAbortCtx->AbortType,
                sizeof(pAbortCtx->AbortType));

    ApiAddParam(pApi,
                NULL,
                PAR_ALARM_SEVERITY,
                (LPVOID)&pAbortCtx->Severity,
                sizeof(pAbortCtx->Severity));

    ApiAddParam(pApi,
                NULL,
                PAR_ALARM_ABORT_FRM0 + DumpCpu,
                pAbortCtx,
                20 * sizeof(VUINT32));

    ApiAddParam(pApi,
                NULL,
                PAR_ALARM_GIC_DIST,
                &gDumpGicDist,
                sizeof(gDumpGicDist));

    if ( gDumpGicCpu0.Ctrl != 0xDEADC0DE )
    {
        ApiAddParam(pApi,
                    NULL,
                    PAR_ALARM_GIC_CPU0,
                    &gDumpGicCpu0,
                    sizeof(gDumpGicCpu0));
    }

    if ( gDumpGicCpu1.Ctrl != 0xDEADC0DE )
    {
        ApiAddParam(pApi,
                    NULL,
                    PAR_ALARM_GIC_CPU1,
                    &gDumpGicCpu1,
                    sizeof(gDumpGicCpu1));
    }

    if ( gDumpGicCpu2.Ctrl != 0xDEADC0DE )
    {
        ApiAddParam(pApi,
                    NULL,
                    PAR_ALARM_GIC_CPU2,
                    &gDumpGicCpu2,
                    sizeof(gDumpGicCpu2));
    }

    if ( gDumpGicCpu3.Ctrl != 0xDEADC0DE )
    {
        ApiAddParam(pApi,
                    NULL,
                    PAR_ALARM_GIC_CPU3,
                    &gDumpGicCpu3,
                    sizeof(gDumpGicCpu3));
    }

    DumpStackLen = SvsrGetSysInfo().nMaxApiParamSize - pApi->Length - PARAM_HDR_SIZE - 128;

    if(DumpStackLen > 0)
    {// if there is a space
        UINT32 UsedLen = SysShdwnGetUsedStackLength(DumpCpu, pAbortCtx->sp);

        DumpStackLen = MIN (DumpStackLen, UsedLen);
        ApiAddParam(pApi,
                NULL,
                PAR_ALARM_STACK,
                (PUINT8)pAbortCtx->sp,
                DumpStackLen);
    }

    uart_printf("Alarm message is sent to UARM\r\n");

    SvsrSendICApi(pApi);

}

void SysShdwnAbortFrmToCLI (void)
{
    int i, j;
    PVUINT32 pData;
    PVUINT32 pStack;
	PVUINT32 pc, lr;

    UINT32 ticks = MxGetTicks();

    uart_printf ("Registers Dump for cpu-%d:\r\n", MxGetCpuID());

    switch (MxGetCpuID ())
    {
        case 0:
            pData = (PVUINT32)&gAbortCtx0;
            break;

        case 1:
            pData = (PVUINT32)&gAbortCtx1;
            break;

        case 2:
            pData = (PVUINT32)&gAbortCtx2;
            break;

        case 3:
            pData = (PVUINT32)&gAbortCtx3;
            break;

        default:
            pData = (PVUINT32)&gAbortCtx0;
            break;
    }

	pc = (PVUINT32)((PABORT_FRAME)pData)->pc;
	lr = (PVUINT32)((PABORT_FRAME)pData)->lr;

    i = 0;
    do{
        j = 0;
        do{
            uart_printf ("R%02d: 0x%8.8X ", (i * 4 + j), *pData++);
        }while(++j < 4);
        uart_printf("\r\n");
    }while(++i < 3);
    uart_printf ("R12: 0x%8.8X ", *pData++);
    pStack = (PVUINT32)(*pData);
    uart_printf (" SP: 0x%8.8X ", *pData++);
    uart_printf (" LR: 0x%8.8X ", *pData++);
    uart_printf (" PC: 0x%8.8X\r\n", *pData++);

    uart_printf ("[PC]: 0x%8.8X ", *pData++);
    uart_printf ("SPSR: 0x%8.8X ", *pData++);
    pData++;
    uart_printf ("CPSR: 0x%8.8X\r\n", *pData++);

    uart_printf("Stack:");

    for(i = 0; i < 40; i++)
    {
        if ((i % 4) == 0)
            uart_printf("\r\n[0x%08X]:  ", (UINT32)(pStack+i));

        uart_printf("0x%08X ", pStack[i]);
    }

    uart_printf("\r\nticks=%d cpu=%d\r\n", ticks, MxGetCpuID ());

    RexUploadMLog();
    MxDelayTicks(150000);

    SvsrLoggerFlush();
    MLogPrint();

    rootinfo_print_stack(pc, pStack, lr, "Data Abort");
}

static void SysShdwnStoreRegs ( BOOL bAbortEntry )
{// this function should store all possible HW registers for future analysis (coredump)
// it should be useful to check that registers which are status/statistics storing
    GicCpuRegs*pGicCpuRegsDst;
    GicCpuRegs*pGicCpuRegsSrc;

    if(bAbortEntry)
    {// coming once
        PVUINT32 pSrc;
        PVUINT32 pDst;
        UINT32 NumWords = sizeof(gDumpGicDist) / sizeof(VUINT32);

        // Store GIC-Distributor registers
        pSrc = (PVUINT32) GIC_DISTR_BASE_ADDR;
        pDst = (PVUINT32) &gDumpGicDist;

        do
        {
            *pDst++ = *pSrc++;
            // PLEASE don't touch these 'nop's below
            __nop();
            __nop();
        }while(--NumWords);

        // write magic numbers to manage only once writing
        gDumpGicCpu0.Ctrl = 0xDEADC0DE;
        gDumpGicCpu1.Ctrl = 0xDEADC0DE;
        gDumpGicCpu2.Ctrl = 0xDEADC0DE;
        gDumpGicCpu3.Ctrl = 0xDEADC0DE;
        SysShtDwnAlarmState = 0xDEADC0DC + MxGetCpuID ();
    }

    // we're here 3 times during abort

    // Store GIC-CPU registers

    switch (MxGetCpuID ())
    {
        case 0:
            pGicCpuRegsDst = &gDumpGicCpu0;
            break;

        case 1:
            pGicCpuRegsDst = &gDumpGicCpu1;
            break;

        case 2:
            pGicCpuRegsDst = &gDumpGicCpu2;
            break;

        case 3:
            pGicCpuRegsDst = &gDumpGicCpu3;
            break;

        default:
            pGicCpuRegsDst = &gDumpGicCpu0;
            break;
    }

    if(pGicCpuRegsDst->Ctrl == 0xDEADC0DE)
    {
        pGicCpuRegsSrc = (GicCpuRegs*) GIC_CPU_BASE_ADDR;
        pGicCpuRegsDst->Ctrl = pGicCpuRegsSrc->Ctrl;
        pGicCpuRegsDst->Mask = pGicCpuRegsSrc->Mask;
        pGicCpuRegsDst->BinPoint = pGicCpuRegsSrc->BinPoint;
        pGicCpuRegsDst->IntEnd = pGicCpuRegsSrc->IntEnd;
        pGicCpuRegsDst->RunPrior = pGicCpuRegsSrc->RunPrior;
    }

#if 0
    if(bAbortEntry)
    {// we aren't protected against L-ARM ICPU death, send first alarm right now
        SysShdwnSendAlarm (SysShtDwnAlarmState & 1);
    }
#endif
}

void (*L2AbortHndlr)();

void SysShdwnProcAbort (UINT16 Type)
{
    PABORT_FRAME pAbortCtx;
    UINT32 i;

    // IRQs/FIQs are disabled

    // Store Type

    MLogSetMask(0);

    switch (MxGetCpuID ())
    {
        case 0:
            pAbortCtx = &gAbortCtx0;
            break;

        case 1:
            pAbortCtx = &gAbortCtx1;
            break;

        case 2:
            pAbortCtx = &gAbortCtx2;
            break;

        case 3:
            pAbortCtx = &gAbortCtx3;
            break;

        default:
            pAbortCtx = &gAbortCtx0;
            break;
    }

    pAbortCtx->AbortType = Type;
    pAbortCtx->Severity = ALARM_SEVERITY_FATAL;

    SysShdwnStoreRegs (TRUE);

    while(SysShtDwnProcAbortLock == 1)
        __nop();
    SysShtDwnProcAbortLock = 1;

    uart_printf ("(cpu=%d) %s ABORT: CODE ADDRESS:0x%08x\r\n", MxGetCpuID (),
        (Type == ALARM_IDX_DABT)? "DATA" : ((Type == ALARM_IDX_PABT)? "PREFETCH": "UNDEF"),
        (Type == ALARM_IDX_DABT)? pAbortCtx->pc : pAbortCtx->lr);

    SysShdwnAbortFrmToCLI ();

    if (L2AbortHndlr)
        L2AbortHndlr();

    if(SvsrLoggerGetMask())
        SvsrLoggerFlush();

    MLogPrint();

    SysShtDwnProcAbortLock = 0;

    // Safe clear - e.g. if DABT occurs during drivers shutting down

    for (i = 0; i < CPU_NUM; i++)
        SysShtDwnLock [i] = 0;

    ICDrvRaiseInterCpu (0, SOFT_ICPU_SYS_SHDWN);
}

static void SysShdwnStoreState (void )
{// this function should store all possible registers for future analysis (coredump)

    // IRQs are disabled

    MxiFlushIDCache ();

    SysShdwnStoreRegs (FALSE);
}

static void SysShdwnCheckAlarm (void )
{// this function sends alarm if any, all the info is ready

    if (MxGetCpuID() == 0)
    {
        if (SysShtDwnAlarmState >= 0xDEADC0DC)
        {
            SysShdwnSendAlarm (SysShtDwnAlarmState & 3);
        }
        SysShtDwnAlarmState = 0;
    }
}

void SysShutdownIRQHandler (VUINT32 * pClear, UINT32 nIrq, LPVOID pData)
{
    VUINT32 count = 0, nVal, cpunum = 2;
    UINT32 nCpuID = MxGetCpuID();
    UINT32 nGenIrq = 0;

#ifdef _MX_DEBUG
    //MXRC rc = MX_OK;
#endif

    if (MxGetARMID () == 0)
    {
        cpunum = CPU_NUM;

        // to generate the local irq to other cores

        if (nCpuID == 0)
        {
            nGenIrq = 1;
        }
    }
    else
        cpunum = MIN(CPU_NUM, 2);

    ARM_INT_disable ();

    SysShdwnStoreState ();

    if (nCpuID == 0)
    {
        if (cpunum >= 1)
        {
            if (nGenIrq != 0)
                ICDrvRaiseInterCpu (GIC_IRQ_TYPE | 1, SOFT_ICPU_SYS_SHDWN_EX);

            count = 0;
            while(SysShtDwnLock[1] == 0)
            {
                if (++count > 1000000)
                   break;
            }
        }

        if (cpunum >= 2)
        {
            if (nGenIrq != 0)
                ICDrvRaiseInterCpu (GIC_IRQ_TYPE | 2, SOFT_ICPU_SYS_SHDWN_EX);

            count = 0;
            while(SysShtDwnLock[2] == 0)
            {
                if (++count > 1000000)
                   break;
            }
        }

        if (cpunum >= 3)
        {
            if (nGenIrq != 0)
                ICDrvRaiseInterCpu (GIC_IRQ_TYPE | 3, SOFT_ICPU_SYS_SHDWN_EX);

            count = 0;
            while(SysShtDwnLock[3] == 0)
            {
                if (++count > 1000000)
                   break;
            }
        }
    }

    GemDrvShutdown(1);
    SysShdwnCheckAlarm ();
    MdmaGenShedding();
    TimerDrvShutdown ();

#ifdef SPACC_DRV_ENABLED
    SPAccDrvShutdown();
#endif

    SysTimersShutdown ();
    FecGenShedding();
    CPRIDrvGenShedding();
    MapDrvGenShedding();
    PCIeDrvGenShedding ();
    RexGenShedding();

    // this printf outputs information and
    // provides some delay before reset the HW per.
    // the delay is needed to let CEVAs finish all transactions

    uart_printf("Shutdown [CPU#%d]\n", nCpuID);

    if (nCpuID == 0)
    {
        UartDrvShutdownAll();
    }

    // to turn off SCU module
    //REG32(MxGetArmScuBaseAddr () + 0x0C) &= ~(0xF << (nCpuID * 4));
    //REG32(MxGetArmScuBaseAddr () + 0x00) = 0;

    if (nCpuID == 0)
    {
        MxFlushDCache ();

        //nVal = MxGetAuxCtrl ();
        //nVal &= ~((1 << 6) | 1);
        //MxSetAuxCtrl (nVal);
    }

    if (MxGetARMID () == 0 && nCpuID != 0)
    {
        *pClear = nIrq;
    }

    if(nCpuID == 0)
    {
        MxClearInterCpu (0, SOFT_ICPU_SYS_SHDWN);
        *pClear = nIrq;

#ifdef MX_ARM_L2_CACHE_ENABLED
        MxL2_disable ();
#endif

        SysDDRDisableWriteProtect(0);
        SysDDRDisableWriteProtect(1);
    }

    if (nCpuID == 0)
        ICDrvShutdown ();

    SysShtDwnLock[nCpuID] = 1;

    while (1)
    {
        _NOP();
    }
}

MXRC SysShdwnDrv_init(UINT32 nInit, LPVOID * pDrvCtx)
{
	MXRC rc;
    UINT32 nIrqID = SOFT_ICPU_SYS_SHDWN;

    if (MxGetCpuID () != 0)
    {
        nIrqID = SOFT_ICPU_SYS_SHDWN_EX;
    }

    // for the L-ARM
    if (MxGetARMID () != 0)
    {
        if (FAILED(rc = MxSetIntHandler (nIrqID, (ITYPE)(INT_TYPE_IRQ | INT_TYPE_1N), PRI_SYS_SHTDWN, SysShutdownIRQHandler, NULL)))
    	    return rc;
    }
    // for the U-ARM
    else
    {
        if (FAILED(rc = MxSetIntHandler (nIrqID, INT_TYPE_IRQ, PRI_SYS_SHTDWN, SysShutdownIRQHandler, NULL)))
    	    return rc;
    }

	if (FAILED(rc = MxEnableInt (nIrqID)))
		return rc;

	SysShtDwnLock[MxGetCpuID ()] = 0;

        SysShtDwnProcAbortLock = 0;

	return MX_OK;
}

