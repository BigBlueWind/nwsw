/******************************************************************************
 *                                                                             *
 *       Copyright 2010 by Mindspeed Technologies, Inc.                        *
 *       All Rights Reserved                                                   *
 *                                                                             *
 *       Mindspeed Technologies, Inc.                                          *
 *       4000 MACARTHUR BLVD - EAST TOWER                                      *
 *       NEWPORT BEACH, CA 92660                                               *
 *                                                                             *
 *******************************************************************************
 *       pmdrv.c                                                               *
 *                                                                             *
 *       Perfomance Monitor Unit interface functions                           *
 *                                                                             *
 *****************************************************************************/


#include "config.h" 

#ifdef PM_ENABLED

#include "pm.h"
#include "appprintf.h"
#include "sirqassign.h"
#include "icdrv.h"
#include "mlog.h"

PM_DRV_CTX PMCtx;

INLINE PM_DRV_CORE_CTX * PMGetCoreCtx (void)
{
    return &PMCtx.ctx[MxGetCpuID()];
}

INLINE PM_DRV_CORE_CTX * PMGetCoreCtxEx (UINT32 nCpuID)
{
    return &PMCtx.ctx[nCpuID];
}

void PMIrqSyncHandler (volatile UINT32 * pAckReg, UINT32 nIrqID, LPVOID pParam)
{
    *pAckReg = nIrqID;

    PMAddMIPSStatistics ();
}

MXRC PMInit(void)
{
    MXRC rc;
    UINT32 cpuId;
    PPM_DRV_CORE_CTX pPMCtx; 

    cpuId = MxGetCpuID ();
    pPMCtx = PMGetCoreCtxEx(cpuId);

    memset(pPMCtx, 0x00, sizeof(PM_DRV_CORE_CTX));

    uart_printf("\r\nPerformance monitor CPU-%d: init...", cpuId);

    rc = MxCreateThread(PMProcessCmd, NULL, NULL, (TPRIORITY)0, &pPMCtx->hThread);
    
    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        uart_printf ("Create thread error, rc=%x\r\n", rc);
        return rc;
    }

    pm_set_PMUSERENR(1);
    pm_set_PMOVSR(0xffffffff);
    pm_set_PMINTENCLR(0xffffffff);
    pm_set_PMCR((pm_get_PMCR() | PMCR_RESET_CLOCK_CNTR | PMCR_RESET_EVENT_CNTR | PMCR_ENABLE) & (~PMCR_ENABLE_CLOCK_DIVIDER) & (~PMCR_DISABLE_CLOCK_IN_PROH));  

    rc = MxSetIntHandler(SOFT_ICPU_PM_SYNC, INT_TYPE_IRQ, PRI_PMON, PMIrqSyncHandler, pPMCtx);

    if (FAILED(rc))
    {
        uart_printf ("Error sync IRQ setup, rc=%x\r\n", rc);
        return rc;
    }

    rc = MxEnableInt (SOFT_ICPU_PM_SYNC);

    if (FAILED(rc))
    {
        uart_printf ("Error sync IRQ enable, rc=%x\r\n", rc);
        return rc;
    }

    uart_printf("OK\n");

    PMCtx.StartedMask |= (1 << MxGetCpuID ());

    return MX_OK;
}

/** @brief This function generates synchronization signal to all cores, 
           where PM is enabled 

    @param exc_mask [in] the cpu mask that must be excluded for some reason 
                         by default this value should be equal to 0

    @return [MXRC] an error code 

    */           

MXRC PMGenSyncSignal (UINT32 exc_mask)
{
    UINT32 core_mask = (PMCtx.StartedMask & PMCtx.ProfCpuMask);

    core_mask &= ~exc_mask;

    if (core_mask != 0)
        return ICDrvRaiseInterCpuByMask (core_mask, SOFT_ICPU_PM_SYNC);

    return MX_OK;
}

/** @brief This function enables profiling (putting data to MLog) for the 
           specific cores 

    @param CpuMask [in] - CPU mask

    @return [MXRC] an error code */

MXRC PMEnableProf (UINT32 CpuMask)
{
    IRQTYPE irq = ARM_INT_disable();
    UINT32 i;

    PMCtx.ProfCpuMask = CpuMask;

    i = 0;

    while (i < sizeof (PMCtx.ctx) / sizeof (PMCtx.ctx[0]))
    {
        if (CpuMask & (1 << i))
        {
            if (PMCtx.ctx [i].PMMode != PM_CTRL_MODE_TURNED_ON)
                PMCtx.ctx [i].PMMode = PM_CTRL_MODE_ENABLE_CLOCK;
        }
        else
        {
            if (PMCtx.ctx [i].PMMode != PM_CTRL_MODE_TURNED_OFF)
                PMCtx.ctx [i].PMMode = PM_CTRL_MODE_DISABLE_CLOCK;
        }

        i++;
    }

    ARM_INT_restore(irq);

    return MX_OK;
}

/**
    @brief  Configure counter to count specified event.
    @param index    [IN] Counter index can be 0, 1, 2, .. 5 - general event counters or 31 - clock counter
    @param event_id [IN] According to ARM Perfomance Monitor Unit documentation. For the clock counter 
                    (index = 31) use event_id = 0 to count every clock or event_id != 0 to count 
                    every 64 clock
    @return 0 on success
*/    
UINT32 PM_CfgCounter(UINT32 index, UINT32 event_id)
{
    if(index < PMU_COUNTERS_NUMBER)
    {
        pm_set_PMSELR(index);            
        pm_set_PMXEVCNTR(0);
        pm_set_PMXEVTYPER(event_id);
        pm_set_PMOVSR(1 << index);
        return 0;
    }
    else if(index == PMU_CLK_COUNTER_INDEX)
    {
        if(event_id)
            pm_set_PMCR(pm_get_PMCR() | PMCR_ENABLE_CLOCK_DIVIDER);
        else
            pm_set_PMCR(pm_get_PMCR() & (~PMCR_ENABLE_CLOCK_DIVIDER));
        pm_set_PMOVSR(1 << index);
        pm_set_PMCR(pm_get_PMCR() | PMCR_RESET_CLOCK_CNTR);
        return 0;
    }
    return 1;
}

/**
    @brief Get counter state
    @brief  Configure counter to count specified event.
    @param index    [IN] Index of the counter to get value (can be 0, 1, 2, .. 5 - general event counters or 31 - clock counter)
    @param event_id [OUT] Event ID which corresponds to the counter 
    @param value    [OUT] Counter value
    @param overfl   [OUT] non-zero if counter overflow detected
    @return 0 on success    
*/                    
UINT32 PM_GetCounter(UINT32 index, PUINT32 event_id, PUINT32 value, PUINT32 overfl)
{
    UINT32 ovflow;

    ovflow = pm_get_PMOVSR();
    if(index < PMU_COUNTERS_NUMBER)
    {        
        pm_set_PMSELR(index);    
        if(value) *value = pm_get_PMXEVCNTR();
        if(event_id) *event_id = pm_get_PMXEVTYPER();
        if(overfl) *overfl = ovflow & (1 << index) ? 1 : 0;  
        return 0;
    }   
    else if(index == PMU_CLK_COUNTER_INDEX)
    {
        if(value) *value = pm_get_PMCCNTR();
        if(overfl) *overfl = (ovflow & ((UINT32)1 << PMU_CLK_COUNTER_INDEX)) ? 1 : 0;  
        if(event_id) *event_id = (pm_get_PMCR() & PMCR_ENABLE_CLOCK_DIVIDER) ? 1 : 0;
        return 0;        
    }
    return 1;
}

void PMStartCalcDataCacheMiss()
{
    // configure counter
    if(PM_CfgCounter (PMSELR_COUNTER_0, PMXEVTYPER_EVENT_DATA_CACHE_MISS))
    {
        uart_printf("PMStartCalcDataCacheMiss: configuration of counter (0x%x) event_id (0x%x) - failed \n",PMSELR_COUNTER_0, PMXEVTYPER_EVENT_DATA_CACHE_MISS);
        return;
    }

    // enebale counter
    pm_set_PMCNTENSET(1 << PMSELR_COUNTER_0);    
    
}

void PMStopCalcDataCacheMiss()
{   
    pm_set_PMCNTENCLR(1 << PMSELR_COUNTER_0);
}

UINT32 PMGetDataCacheMissNumber()
{
    UINT32 event_id;
    UINT32 value;
    UINT32 overfl;

    if (PM_GetCounter(PMSELR_COUNTER_0,&event_id, &value, &overfl))
    {
        uart_printf("PMGetDataCacheMissNumber: getting counter (0x%x) value - failed \n",PMSELR_COUNTER_0);
        return 1;
    }

    if (event_id != PMXEVTYPER_EVENT_DATA_CACHE_MISS)
    {
        uart_printf("PMGetDataCacheMissNumber:  event_id (0x%x) - failed \n",event_id);
        return 1;
    }

    return value;
}

void PMStartCalcDataCacheHit()
{
    // configure counter
    if(PM_CfgCounter (PMSELR_COUNTER_1, PMXEVTYPER_EVENT_DATA_CACHE_HIT))
    {
        uart_printf("PMStartCalcDataCacheMiss: configuration of counter (0x%x) event_id (0x%x) - failed \n",PMSELR_COUNTER_1, PMXEVTYPER_EVENT_DATA_CACHE_HIT);
        return;
    }

    // enebale counter
    pm_set_PMCNTENSET(1 << PMSELR_COUNTER_1);    
    
}

void PMStopCalcDataCacheHit()
{
    pm_set_PMCNTENCLR(1 << PMSELR_COUNTER_1);
}


UINT32 PMGetDataCacheHitNumber()
{
    UINT32 event_id;
    UINT32 value;
    UINT32 overfl;

    if (PM_GetCounter(PMSELR_COUNTER_1,&event_id, &value, &overfl))
        uart_printf("PMGetDataCacheMissNumber: getting counter (0x%x) value - failed \n",PMSELR_COUNTER_1);

    if (event_id != PMXEVTYPER_EVENT_DATA_CACHE_HIT)
        uart_printf("PMGetDataCacheMissNumber:  event_id (0x%x) - failed \n",event_id);

    return value;
}

void PMStartCalcInstrCacheMiss()
{
    // configure counter
    if(PM_CfgCounter (PMSELR_COUNTER_2, PMXEVTYPER_EVENT_INSTR_CACHE_MISS))
    {
        uart_printf("PMStartCalcDataCacheMiss: configuration of counter (0x%x) event_id (0x%x) - failed \n",PMSELR_COUNTER_2, PMXEVTYPER_EVENT_INSTR_CACHE_MISS);
        return;
    }

    // enebale counter
    pm_set_PMCNTENSET(1 << PMSELR_COUNTER_2);    
    
}

void PMStopCalcInstrCacheMiss()
{   
    pm_set_PMCNTENCLR(1 << PMSELR_COUNTER_2);
}

UINT32 PMGetInstrCacheMissNumber()
{
    UINT32 event_id;
    UINT32 value;
    UINT32 overfl;

    if (PM_GetCounter(PMSELR_COUNTER_2,&event_id, &value, &overfl))
    {
        uart_printf("PMGetDataCacheMissNumber: getting counter (0x%x) value - failed \n",PMSELR_COUNTER_2);
        return 1;
    }

    if (event_id != PMXEVTYPER_EVENT_INSTR_CACHE_MISS)
    {
        uart_printf("PMGetDataCacheMissNumber:  event_id (0x%x) - failed \n",event_id);
        return 1;
    }

    return value;
}

void PMStartCalcInstr()
{
    // configure counter
    if(PM_CfgCounter (PMSELR_COUNTER_3, PMXEVTYPER_EVENT_INSTR_EXECUTED))
    {
        uart_printf("PMStartCalcDataCacheMiss: configuration of counter (0x%x) event_id (0x%x) - failed \n",PMSELR_COUNTER_3, PMXEVTYPER_EVENT_INSTR_EXECUTED);
        return;
    }

    // enebale counter
    pm_set_PMCNTENSET(1 << PMSELR_COUNTER_3);    
    
}

void PMStopCalcInstr()
{
    pm_set_PMCNTENCLR(1 << PMSELR_COUNTER_3);
}

UINT32 PMGetInstrNumber()
{
    UINT32 event_id;
    UINT32 value;
    UINT32 overfl;

    if (PM_GetCounter(PMSELR_COUNTER_3,&event_id, &value, &overfl))
        uart_printf("PMGetDataCacheMissNumber: getting counter (0x%x) value - failed \n",PMSELR_COUNTER_3);

    if (event_id != PMXEVTYPER_EVENT_INSTR_EXECUTED)
        uart_printf("PMGetDataCacheMissNumber:  event_id (0x%x) - failed \n",event_id);

    return value;
}

void PMStartCalcCycle(void)
{
    IRQTYPE irq = ARM_INT_disable();
    PM_DRV_CORE_CTX * pCtx = PMGetCoreCtx();

    _ASSERT_PTR(pCtx);

    // configure counter
    if(PM_CfgCounter (PMU_CLK_COUNTER_INDEX, PMU_CLK_COUNTER_COUNT_EVERY_CLOCK))
    {
        ARM_INT_restore(irq);
        uart_printf("PMStartCycleCounter: configuration of counter (0x%x) event_id (0x%x) - failed \n",PMU_CLK_COUNTER_INDEX, PMU_CLK_COUNTER_COUNT_EVERY_CLOCK);
        return;
    }

    // enebale counter
    pm_set_PMCNTENSET((UINT32)1 << PMU_CLK_COUNTER_INDEX);    
    ARM_INT_restore(irq);
}

void PMStopCalcCycle(void)
{
    pm_set_PMCNTENCLR((UINT32)1 << PMU_CLK_COUNTER_INDEX);
    _NOP();
    _NOP();
    _NOP();
}

UINT32 PMGetCycleNumber(void)
{
#ifdef PM_ENABLED
    UINT32 event_id;
    UINT32 value;
    UINT32 overfl;

    if (PM_GetCounter(PMU_CLK_COUNTER_INDEX,&event_id, &value, &overfl))
    {
        uart_printf("PMGetCycleCounter: getting counter (0x%x) value - failed \n",PMU_CLK_COUNTER_INDEX);
        return 1;
    }

    return value;

#else 
    return 0;
#endif
}

/** @brief This function is designed to return the number of ARM clocks 
           spent by ARM in some specific mode 

    @param nMode[in] - mode id, see: PM_ARM_MODE_xxx

    @return [UINT32] clocks */

UINT32 PMGetCyclesForArmMode (UINT32 nMode)
{
    return PMGetCyclesForArmModeEx (MxGetCpuID(), nMode);
}

UINT32 PMProcessCmd (void * pThreadParam)
{
    UINT32 cpuId;
    PPM_DRV_CORE_CTX pPMCtx;

    cpuId = MxGetCpuID ();
    pPMCtx = PMGetCoreCtxEx(cpuId);
    
    switch(pPMCtx->cmd)
    {
        case PM_CMD_START_CALC_DATA_CACHE_MISS:
            PMStartCalcDataCacheMiss();
            break;

        case PM_CMD_STOP_CALC_DATA_CACHE_MISS:
            PMStopCalcDataCacheMiss();
            break;

        case PM_CMD_GET_DATA_CACHE_MISS_NUMBER:
            pPMCtx->result = PMGetDataCacheMissNumber();
            break;

        case PM_CMD_START_CALC_DATA_CACHE_HIT:
            PMStartCalcDataCacheHit();
            break;

        case PM_CMD_STOP_CALC_DATA_CACHE_HIT:
            PMStopCalcDataCacheHit();
            break;

        case PM_CMD_GET_DATA_CACHE_HIT_NUMBER:
            pPMCtx->result = PMGetDataCacheHitNumber();
            break;            

        case PM_CMD_START_CALC_CYCLE:
            //uart_printf("PMProcessCmd: PM_CMD_START_CALC_CYCLE\n");
            PMStartCalcCycle();
            break;

        case PM_CMD_STOP_CALC_CYCLE:
            PMStopCalcCycle();
            break;

        case PM_CMD_GET_CYCLE_NUMBER:
            pPMCtx->result = PMGetCycleNumber();
            break;            

        case PM_CMD_GET_ARM_SVSR_CYCLES:
            pPMCtx->result = PMGetCyclesForArmMode (PM_ARM_MODE_SVSR);
            break;

        case PM_CMD_GET_ARM_IRQ_CYCLES:
            pPMCtx->result = PMGetCyclesForArmMode (PM_ARM_MODE_IRQ);
            break;
            
        case PM_CMD_GET_ARM_IDLE_CYCLES:
            pPMCtx->result = PMGetCyclesForArmMode (PM_ARM_MODE_IDLE);
            break;

            default:
            break;
    }

    // cmd was done
    pPMCtx->cmd = 0x00;

    return 1;
}

void PMStartCalcDataCacheMissEx(CPUID cpu)
{
    UINT32 cpuId;
    PPM_DRV_CORE_CTX pPMCtx;
    MXRC rc;

    cpuId = MxGetCpuID ();

    if (cpu == cpuId || cpu == CUR_CPU)
    {
        PMStartCalcDataCacheMiss();
        return;
    }
    
    pPMCtx = PMGetCoreCtxEx(cpu); 
    pPMCtx->cmd = PM_CMD_START_CALC_DATA_CACHE_MISS;
    
    rc = MxInjectThread(cpu, pPMCtx->hThread);
    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        uart_printf ("Inject thread error, rc=%x\r\n", rc);
        return;
    }

    // waiting command status: done
    while(pPMCtx->cmd);

}
void PMStopCalcDataCacheMissEx(CPUID cpu)
{
    UINT32 cpuId;
    PPM_DRV_CORE_CTX pPMCtx;
    MXRC rc;

    cpuId = MxGetCpuID ();
    
    if (cpu == cpuId || cpu == CUR_CPU)
    {
        PMStopCalcDataCacheMiss();
        return; 
    }

    pPMCtx = PMGetCoreCtxEx(cpu);
    pPMCtx->cmd = PM_CMD_STOP_CALC_DATA_CACHE_MISS;
    
    rc = MxInjectThread(cpu, pPMCtx->hThread);
    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        uart_printf ("Inject thread error, rc=%x\r\n", rc);
        return;
    }

    // waiting command status: done
    while(pPMCtx->cmd);
}

UINT32 PMGetDataCacheMissNumberEx(CPUID cpu)
{
    UINT32 cpuId;
    PPM_DRV_CORE_CTX pPMCtx;
    MXRC rc;

    cpuId = MxGetCpuID ();

    if (cpu == cpuId || cpu == CUR_CPU)
        return PMGetDataCacheMissNumber();

    pPMCtx = PMGetCoreCtxEx(cpu);
    pPMCtx->cmd = PM_CMD_GET_DATA_CACHE_MISS_NUMBER;
    
    rc = MxInjectThread(cpu, pPMCtx->hThread);
    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        uart_printf ("Inject thread error, rc=%x\r\n", rc);
        return rc;
    }    

    // waiting command status: done
    while(pPMCtx->cmd);

    return pPMCtx->result;
}

void PMStartCalcDataCacheHitEx(CPUID cpu)
{
    UINT32 cpuId;
    PPM_DRV_CORE_CTX pPMCtx;
    MXRC rc;

    cpuId = MxGetCpuID ();

    if (cpu == cpuId || cpu == CUR_CPU)
    {
        PMStartCalcDataCacheHit();
        return;
     }

    pPMCtx = PMGetCoreCtxEx(cpu);
    pPMCtx->cmd = PM_CMD_START_CALC_DATA_CACHE_HIT;
    
    rc = MxInjectThread(cpu, pPMCtx->hThread);
    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        uart_printf ("Inject thread error, rc=%x\r\n", rc);
        return;
    }

    // waiting command status: done
    while(pPMCtx->cmd);
}

void PMStopCalcDataCacheHitEx(CPUID cpu)
{
    UINT32 cpuId;
    PPM_DRV_CORE_CTX pPMCtx;
    MXRC rc;

    cpuId = MxGetCpuID ();

    if (cpu == cpuId || cpu == CUR_CPU)
    {
        PMStopCalcDataCacheHit();
        return;
    }

    pPMCtx = PMGetCoreCtxEx(cpu);
    pPMCtx->cmd = PM_CMD_STOP_CALC_DATA_CACHE_HIT;
    
    rc = MxInjectThread(cpu, pPMCtx->hThread);
    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        uart_printf ("Inject thread error, rc=%x\r\n", rc);
        return;
    }

    // waiting command status: done
    while(pPMCtx->cmd);
}

UINT32 PMGetDataCacheHitNumberEx(CPUID cpu)
{
    UINT32 cpuId;
    PPM_DRV_CORE_CTX pPMCtx;
    MXRC rc;

    cpuId = MxGetCpuID ();

    if (cpu == cpuId || cpu == CUR_CPU)
        return PMGetDataCacheHitNumber();

    pPMCtx = PMGetCoreCtxEx(cpu);
    pPMCtx->cmd = PM_CMD_GET_DATA_CACHE_HIT_NUMBER;

    rc = MxInjectThread(cpu, pPMCtx->hThread);
    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        uart_printf ("Inject thread error, rc=%x\r\n", rc);
        return rc;
    }

    // waiting command status: done
    while(pPMCtx->cmd);

    return pPMCtx->result;
}

void PMStartCalcCycleEx(CPUID cpu)
{
    UINT32 cpuId;
    PPM_DRV_CORE_CTX pPMCtx;
    MXRC rc;

    cpuId = MxGetCpuID ();

    if (cpu == cpuId || cpu == CUR_CPU)
    {
        PMStartCalcCycle();
        return;
    }

    pPMCtx = PMGetCoreCtxEx(cpu);
    pPMCtx->cmd = PM_CMD_START_CALC_CYCLE;

    rc = MxInjectThread(cpu, pPMCtx->hThread);
    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        uart_printf ("Inject thread error, rc=%x\r\n", rc);
        return;
    }

    // waiting command status: done
    while(pPMCtx->cmd);    

}

void PMStopCalcCycleEx(CPUID cpu)
{
    UINT32 cpuId;
    PPM_DRV_CORE_CTX pPMCtx;
    MXRC rc;

    cpuId = MxGetCpuID ();

    if (cpu == cpuId || cpu == CUR_CPU)
    {
        PMStopCalcCycle();
        return;
    }

    pPMCtx = PMGetCoreCtxEx(cpu);
    pPMCtx->cmd = PM_CMD_STOP_CALC_CYCLE;
    
    rc = MxInjectThread(cpu, pPMCtx->hThread);
    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        uart_printf ("Inject thread error, rc=%x\r\n", rc);
        return;
    }

    // waiting command status: done
    while(pPMCtx->cmd);       
}
UINT32 PMGetCycleNumberEx(CPUID cpu)
{
    UINT32 cpuId;
    PPM_DRV_CORE_CTX pPMCtx;
    MXRC rc;

    cpuId = MxGetCpuID ();

    if (cpu == cpuId || cpu == CUR_CPU)
        return PMGetCycleNumber();

    pPMCtx = PMGetCoreCtxEx(cpu);
    pPMCtx->cmd = PM_CMD_GET_CYCLE_NUMBER;

    rc = MxInjectThread(cpu, pPMCtx->hThread);
    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        uart_printf ("Inject thread error, rc=%x\r\n", rc);
        return rc;
    }

    // waiting command status: done
    while(pPMCtx->cmd);  

    return pPMCtx->result;
}

/** @brief This function is designed to return the number of processed IRQ signals 

    @param cpu  [in] - The CPU id, the range is [0 … CPU_NUM), for a local 
                       processor this value can be set to 0xFFFFFFFF 
                      (CUR_CPU macro)

    @return [UINT32] number of IRQ*/

UINT32 PMGetARMProcIrqNumEx (CPUID cpu)
{
    PM_DRV_CORE_CTX * pCtx;

    if (cpu == CUR_CPU)
        cpu = MxGetCpuID ();

    pCtx = PMGetCoreCtxEx(cpu);

    _ASSERT_PTR(pCtx);

    return pCtx->IrqProcNum;
}

/** @brief This function is designed to return the number of cache miss situations 
            detected in the previous measurement period

    @return [UINT32] number of cache miss*/
    
UINT32 PMGetCacheMissStat(void)
{
    PM_DRV_CORE_CTX * pCtx;

    pCtx = PMGetCoreCtx();

    _ASSERT_PTR(pCtx);

    return pCtx->CacheMissNum;
}

/** @brief This function is designed to return the number of cache hit situations 
            detected in the previous measurement period

    @return [UINT32] number of cache hit*/
    
UINT32 PMGetCacheHitStat(void)
{
    PM_DRV_CORE_CTX * pCtx;

    pCtx = PMGetCoreCtx();

    _ASSERT_PTR(pCtx);

    return pCtx->CacheHitNum;
}

UINT32 PMGetInstrCacheMissStat(void)
{
    PM_DRV_CORE_CTX * pCtx;

    pCtx = PMGetCoreCtx();

    _ASSERT_PTR(pCtx);

    return pCtx->InstrCacheMissNum;
}

UINT32 PMGetInstrStat(void)
{
    PM_DRV_CORE_CTX * pCtx;

    pCtx = PMGetCoreCtx();

    _ASSERT_PTR(pCtx);

    return pCtx->InstrNum;
}

/** @brief This function is designed to return the number of ARM clocks 
           spent by ARM in some specific mode 

    @param cpu  [in] - The CPU id, the range is [0 … CPU_NUM), for a local 
                       processor this value can be set to 0xFFFFFFFF 
                      (CUR_CPU macro)
    @param nMode[in] - mode id, see: PM_ARM_MODE_xxx

    @return [UINT32] clocks */

UINT32 PMGetCyclesForArmModeEx (CPUID cpu, UINT32 nMode)
{
    IRQTYPE irq = ARM_INT_disable();
    UINT32 nVal = 0;
    PM_DRV_CORE_CTX * pCtx;

    if (cpu == CUR_CPU)
        cpu = MxGetCpuID ();

    pCtx = PMGetCoreCtxEx(cpu);

    _ASSERT_PTR(pCtx);

    switch (nMode)
    {
        case PM_ARM_MODE_SVSR:

            // Supervisor mode is a mode when ARM was not in 
            //   IDLE (IDLE is not supervisor and not IRQ)
            //   IRQ  
            
            nVal = pCtx->SvsrModeTicks;
            break;

        case PM_ARM_MODE_IDLE:

            // IDLE mode when ARM was not in supervisor and not in IRQ modes
            
            nVal = pCtx->IdleModeTicks;
            break;

        case PM_ARM_MODE_IRQ:

            // Just supervisor IRQ + IRQ when ARM was in IDLE mode
            nVal = pCtx->IrqModeTicks;
            break;
    }

    ARM_INT_restore(irq);

    return nVal;
}


/** @brief This function is designed to be called by the drivers 
           to collect ARM modes statictics 

    @param nMode   [in] - PM_ARM_MODE_xxx
    @param nCycles [in] - the number of cycles calculated like PMGetCycleNumber()

    */

void PMAddARMModeCycles (UINT32 nMode, UINT32 nCycles)
{
#ifdef PM_ENABLED

    IRQTYPE irq = ARM_INT_disable();
    PM_DRV_CORE_CTX * pCtx = PMGetCoreCtx();

    _ASSERT_PTR(pCtx);

    switch (nMode)
    {
        case PM_ARM_MODE_SVSR:
            pCtx->ARMSvsrStat += nCycles;
            break;

        case PM_ARM_MODE_IDLE:
            pCtx->ARMIdleStat += nCycles;
            break;

        case PM_ARM_MODE_IRQ:

            if (pCtx->ARMMode == PM_ARM_MODE_SVSR)
                pCtx->ARMIrqSvsrStat += nCycles;
            else if (pCtx->ARMMode == PM_ARM_MODE_IDLE)
                pCtx->ARMIrqIdleStat += nCycles;
            
            break;
    }

    ARM_INT_restore(irq);

#else 

#endif
}

/** @brief This function is used to notify PM about starting of IRQ processing
           this function is used in a pair with PMEndIrqProc() 

    */           

void PMStartIrqProc (void)
{
    PM_DRV_CORE_CTX * pCtx = PMGetCoreCtx();
    _ASSERT_PTR(pCtx);

    if (pCtx->PMMode == PM_CTRL_MODE_TURNED_OFF)
        return;

    pCtx->IrqTicks = PMGetCycleNumber ();
    pCtx->IrqTermMode = pCtx->ARMMode;
}

/** @brief This function is used to add IRQ statistics
           this function is used in a pair with PMEndIrqProc() 

    */
void PMEndIrqProc (void)
{
    PM_DRV_CORE_CTX * pCtx = PMGetCoreCtx();

    _ASSERT_PTR(pCtx);

    if (pCtx->PMMode == PM_CTRL_MODE_TURNED_OFF)
        return;

    if (PMGetCycleNumber() < pCtx->IrqTicks)
    {
        uart_printf ("PM: cycles problem detected, cpu=%d (%d < %d)\r\n", MxGetCpuID (), PMGetCycleNumber(), pCtx->IrqTicks);
        return;
    }

    if (pCtx->IrqTermMode == PM_ARM_MODE_SVSR)
        pCtx->ARMIrqSvsrStat += (PMGetCycleNumber() - pCtx->IrqTicks);
    else if (pCtx->IrqTermMode == PM_ARM_MODE_IDLE)
        pCtx->ARMIrqIdleStat += (PMGetCycleNumber() - pCtx->IrqTicks);
}

/** @brief This function is used to add MIPS statistics
           and called from IRQ handler 

    */

void PMAddMIPSStatistics(void)
{
    PM_DRV_CORE_CTX * pCtx = PMGetCoreCtx();
    UINT32 nVal;
    
    _ASSERT_PTR(pCtx);

    if (pCtx->PMMode == PM_CTRL_MODE_TURNED_OFF)
        return;
    
    PMStopCalcCycle ();
    PMStopCalcDataCacheMiss();
    PMStopCalcDataCacheHit();
    PMStopCalcInstrCacheMiss();

    // At this moment we need to complete previous measurements 
    // and start new one 

    nVal = PMGetCycleNumber ();

    // to add the cycles to ARM terminated mode 
    PMAddARMModeCycles (pCtx->ARMMode, nVal - pCtx->Cycles [pCtx->ARMMode]);
    pCtx->Cycles [pCtx->ARMMode] = 0;

    // to add the cycles to IRQ mode for the terminated mode

    if (pCtx->ARMMode == PM_ARM_MODE_SVSR)
    {
        pCtx->ARMIrqSvsrStat += (nVal - pCtx->IrqTicks);
    }
    else if (pCtx->ARMMode == PM_ARM_MODE_IDLE)
    {
        pCtx->ARMIrqIdleStat += (nVal - pCtx->IrqTicks);
    }

    pCtx->SvsrModeTicks = (pCtx->ARMSvsrStat - pCtx->ARMIrqSvsrStat);
    pCtx->IdleModeTicks = (pCtx->ARMIdleStat - pCtx->ARMIrqIdleStat);
    pCtx->IrqModeTicks  = (pCtx->ARMIrqSvsrStat + pCtx->ARMIrqIdleStat);
    pCtx->IrqProcNum    = (pCtx->CurIrqProcNum);

    pCtx->CacheMissNum  = PMGetDataCacheMissNumber();
    pCtx->CacheHitNum   = PMGetDataCacheHitNumber();
    pCtx->InstrCacheMissNum = PMGetInstrCacheMissNumber();

    pCtx->ARMSvsrStat    = 0;
    pCtx->ARMIdleStat    = 0;
    pCtx->ARMIrqSvsrStat = 0;
    pCtx->ARMIrqIdleStat = 0;
    pCtx->CurIrqProcNum  = 0;

    pCtx->IrqTicks = 0;

    PMStartCalcCycle ();
    PMStartCalcDataCacheMiss();
    PMStartCalcDataCacheHit();
    PMStartCalcInstrCacheMiss();

    //MxDelayTicks (MxGetCpuID () * (150 * 2));

    // To add data to MLog 
    MLogCacheMipsStatsSingle (MxGetTicks());

}


void PMIncIrqProc (void)
{
#ifdef PM_ENABLED
    PM_DRV_CORE_CTX * pCtx = PMGetCoreCtx();

    _ASSERT_PTR(pCtx);

    if (pCtx->PMMode == PM_CTRL_MODE_TURNED_ON)
        pCtx->CurIrqProcNum ++;
    
#endif    
}

void PMAddIrqCycles (UINT32 nCycles)
{
#ifdef PM_ENABLED

    
#else 

#endif
}

/** @brief This function sets current ARM mode 

            @param nMode[in] - ARM mode:  PM_ARM_MODE_SVSR or PM_ARM_MODE_IDLE

*/
void PMSetARMMode(UINT32 nMode)
{
#ifdef PM_ENABLED
     // it's called in IRQ protected mode 

     PM_DRV_CORE_CTX * pCtx = PMGetCoreCtx();
     UINT32 nCycl;

    _ASSERT_PTR(pCtx);

    if (pCtx->PMMode == PM_CTRL_MODE_ENABLE_CLOCK)
    {
        PMStartCalcCycle ();
        PMStartCalcDataCacheMiss();
        PMStartCalcDataCacheHit();
        
        pCtx->PMMode = PM_CTRL_MODE_TURNED_ON;
    }
    else if (pCtx->PMMode == PM_CTRL_MODE_DISABLE_CLOCK)
    {
        PMStopCalcCycle ();

        PMStopCalcDataCacheMiss();
        PMStopCalcDataCacheHit();
        
        pCtx->ARMMode = PM_ARM_MODE_RSV;
        pCtx->PMMode = PM_CTRL_MODE_TURNED_ON;
        
        return;
    }
    else if (pCtx->PMMode == PM_CTRL_MODE_TURNED_OFF)
    {
        return;
    }

    if (pCtx->ARMMode == nMode)
        return;

    nCycl = PMGetCycleNumber ();

    PMAddARMModeCycles (pCtx->ARMMode, nCycl - pCtx->Cycles[pCtx->ARMMode]);

    pCtx->ARMMode = nMode;
    pCtx->Cycles [nMode] = nCycl;

#endif
}

void PM_StartEventCount(UINT32 reg_to_count, UINT32 evt_to_count)
{
    UINT32 r;

    pm_set_PMUSERENR(1);

    // disable PM
    r = pm_get_PMCR();
    pm_set_PMCR( r & (~1));

    // select register
    pm_set_PMSELR(reg_to_count);
    // clear counter
    pm_set_PMXEVCNTR(0);
    // configure event to count
    pm_set_PMXEVTYPER(evt_to_count);

    // enable counter
    pm_set_PMCNTENSET(1 << reg_to_count);

    // enabble PM
    r = pm_get_PMCR();
    pm_set_PMCR( r | 1);
}

void PM_StartClockCount()
{
    pm_set_PMUSERENR(1);

    // enable PMCCNTR
    pm_set_PMCNTENSET((UINT32)1 << 31);

    // reset clock counter & enable PM
    pm_set_PMCR(( pm_get_PMCR() | PMCR_DISABLE_CLOCK_IN_PROH | PMCR_RESET_CLOCK_CNTR | PMCR_ENABLE) & (~PMCR_ENABLE_CLOCK_DIVIDER));
}

UINT32 PM_GetEventCount(UINT32 reg_to_count)
{
    // select register
    pm_set_PMSELR(reg_to_count);
    // get counter
    return pm_get_PMXEVCNTR();
}

UINT32 PM_GetClockCount()
{
    return pm_get_PMCCNTR();
}

#endif // #ifdef PM_ENABLED

