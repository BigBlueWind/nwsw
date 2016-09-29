//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL pm.h
 *
 * @brief Performance monitor
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

#ifndef _PM_H_
#define _PM_H_

#include "4gmx.h"

#define PMU_COUNTERS_NUMBER     ((pm_get_PMCR() >> 11) & 0x1F) 
#define PMU_COUNTERS_MASK       ((1 << PMU_COUNTERS_NUMBER) - 1)

#define PMCR_ENABLE                 (1 << 0)
#define PMCR_RESET_EVENT_CNTR       (1 << 1)
#define PMCR_RESET_CLOCK_CNTR       (1 << 2)
#define PMCR_ENABLE_CLOCK_DIVIDER   (1 << 3)
#define PMCR_DISABLE_CLOCK_IN_PROH  (1 << 5)

#define PMU_CLK_COUNTER_INDEX   	31

#define PMSELR_COUNTER_0            0x00        // counter of data cache missing number
#define PMSELR_COUNTER_1            0x01        // counter of data cache hitting number
#define PMSELR_COUNTER_2            0x02        // counter of insturction cache missing number
#define PMSELR_COUNTER_3            0x03        // counter of all instruction number

#define PM_ARM_MODE_RSV             0
#define PM_ARM_MODE_SVSR            1
#define PM_ARM_MODE_IDLE            2
#define PM_ARM_MODE_IRQ             3

#define PMXEVTYPER_EVENT_DATA_CACHE_MISS    0x03
#define PMXEVTYPER_EVENT_DATA_CACHE_HIT     0x04
#define PMXEVTYPER_EVENT_INSTR_CACHE_MISS   0x01
#define PMXEVTYPER_EVENT_INSTR_EXECUTED     0x08

#define PMU_CLK_COUNTER_COUNT_EVERY_CLOCK     0x00
#define PMU_CLK_COUNTER_COUNT_EVERY_64_CLOCK  0x01

// context command IDs
#define PM_CMD_START_CALC_DATA_CACHE_MISS   0x01
#define PM_CMD_STOP_CALC_DATA_CACHE_MISS    0x02
#define PM_CMD_GET_DATA_CACHE_MISS_NUMBER   0x03
#define PM_CMD_START_CALC_DATA_CACHE_HIT    0x04
#define PM_CMD_STOP_CALC_DATA_CACHE_HIT     0x05
#define PM_CMD_GET_DATA_CACHE_HIT_NUMBER    0x06
#define PM_CMD_START_CALC_CYCLE             0x07
#define PM_CMD_STOP_CALC_CYCLE              0x08
#define PM_CMD_GET_CYCLE_NUMBER             0x09
#define PM_CMD_GET_ARM_SVSR_CYCLES          0x0A
#define PM_CMD_GET_ARM_IRQ_CYCLES           0x0B
#define PM_CMD_GET_ARM_IDLE_CYCLES          0x0C

#define PM_CTRL_MODE_TURNED_OFF             0
#define PM_CTRL_MODE_ENABLE_CLOCK           1
#define PM_CTRL_MODE_DISABLE_CLOCK          2
#define PM_CTRL_MODE_TURNED_ON              3

typedef struct tPMDRV_CORE_CTX
{
    HANDLE      hThread;
    VUINT32     cmd;
    VUINT32     result;

    VUINT32     PMMode;             // See: PM_CTRL_MODE_xxx

    VUINT32     ARMMode;            // see: PM_ARM_MODE_xxx, by default it's supervisor mode
    VUINT32     Cycles [4];
    VUINT32     IrqTermMode;
    VUINT32     IrqTicks;
    VUINT32     CurIrqProcNum;

    // The counters curently used by 
    // PM to calculate statistics 
    
    VUINT32     ARMSvsrStat;        // The number of clocks in ARM supervisor mode 
    VUINT32     ARMIdleStat;        // The number of clocks in ARM idle mode 
    VUINT32     ARMIrqSvsrStat;     // The number of clocks in IRQ mode but ARM in svsr mode (ARM was terminated)
    VUINT32     ARMIrqIdleStat;     // The number of clocks in IRQ mode but ARM in idle mode

    // The counters with "MIPS"
    // for the previous tick

    VUINT32     SvsrModeTicks;
    VUINT32     IdleModeTicks;
    VUINT32     IrqModeTicks;
    VUINT32     IrqProcNum;

    VUINT32     CacheMissNum;       // Data cache miss
    VUINT32     CacheHitNum;        // Data cache hit

    VUINT32     InstrCacheMissNum;  // Instruction cache miss
    UINT32      InstrNum;           // Executed instruction number
    
} PM_DRV_CORE_CTX, *PPM_DRV_CORE_CTX;

typedef struct tPMDRV_CTX
{
    UINT32           StartedMask;   // The mask of CPUs where PMs are started 
    UINT32           ProfCpuMask;   // The mask of cores that are allowed to put data to MLog 

    PM_DRV_CORE_CTX  ctx [CPU_NUM];
    
}PM_DRV_CTX;

extern UINT32 pm_get_PMCR(void);
extern void   pm_set_PMCR(UINT32);
extern UINT32 pm_get_PMCNTENSET(void);
extern void   pm_set_PMCNTENSET(UINT32);
extern UINT32 pm_get_PMCNTENCLR(void);
extern void   pm_set_PMCNTENCLR(UINT32);
extern UINT32 pm_get_PMSELR(void);
extern void   pm_set_PMSELR(UINT32);
extern UINT32 pm_get_PMCCNTR(void);
extern void   pm_set_PMCCNTR(UINT32);
extern UINT32 pm_get_PMXEVTYPER(void);
extern void   pm_set_PMXEVTYPER(UINT32);
extern UINT32 pm_get_PMXEVCNTR(void);
extern void   pm_set_PMXEVCNTR(UINT32);
extern UINT32 pm_get_PMUSERENR(void);
extern void   pm_set_PMUSERENR(UINT32);
extern UINT32 pm_get_PMOVSR(void);
extern void   pm_set_PMOVSR(UINT32);
extern UINT32 pm_get_PMINTENCLR(void);
extern void   pm_set_PMINTENCLR(UINT32);
    
/**
    Initialize PMU
*/
MXRC PMInit(void);
UINT32 PMProcessCmd (void * pThreadParam);

//------------------------------------------------------------------------------

/** @brief The function configures and starts to count number of data 
           cache miss
*/ 
void PMStartCalcDataCacheMiss(void);

/** @brief The function stops countig number of data 
           cache miss
*/ 
void PMStopCalcDataCacheMiss(void);

/** @brief The function gets numbers of data cache miss
           
    @return number of data cache miss  
*/ 
UINT32 PMGetDataCacheMissNumber(void);

//------------------------------------------------------------------------------

/**
    @brief The function configures and starts to count number of data 
           cache hit
*/ 
void PMStartCalcDataCacheHit(void);

/**
    @brief The function stops counting number of data cache hit
*/ 
void PMStopCalcDataCacheHit(void);

/**
    @brief The function gets numbers of data cache hit
    
    @return number of data cache hit  
*/  
UINT32 PMGetDataCacheHitNumber(void);


//------------------------------------------------------------------------------

/**
    @brief  The function configures and starts counter to count cycles 
*/ 
void PMStartCalcCycle(void);

/**
    @brief  The function stops counting number of cycles 
*/ 
void PMStopCalcCycle(void);

/**
    @brief The function gets number of cycles 
    
    @return number of cyclies
*/  
UINT32 PMGetCycleNumber(void);

/** @brief This function is designed to return the number of ARM clocks 
           spent by ARM in some specific mode 

    @param nMode[in] - mode id, see: PM_ARM_MODE_xxx

    @return [UINT32] clocks */

UINT32 PMGetCyclesForArmMode (UINT32 nMode);

//------------------------------------------------------------------------------

/** @brief The function configures and starts to count number of instruction 
           cache miss
*/ 
void PMStartCalcInstrCacheMiss(void);

/** @brief The function stops countig number of instruction 
           cache miss
*/ 
void PMStopCalcInstrCacheMiss(void);

/** @brief The function gets numbers of instruction cache miss
           
    @return number of instruction cache miss  
*/ 
UINT32 PMGetInstrCacheMissNumber(void);

/** @brief This function is designed to return the number of instruction cache miss situations 
            detected in the previous measurement period

    @return [UINT32] number of instruction cache miss
*/
UINT32 PMGetInstrCacheMissStat(void);

//------------------------------------------------------------------------------

/** @brief The function configures and starts to count number of executed instruction 

*/ 
void PMStartCalcInstr(void);

/** @brief The function stops countig number of executed instruction 

*/ 
void PMStopCalcInstr(void);

/** @brief The function gets numbers of executed instruction
           
    @return number of executed instruction
*/ 
UINT32 PMGetInstrCache(void);

/** @brief This function is designed to return the number of executed instruction situations 
            detected in the previous measurement period

    @return [UINT32] number of executed instruction
*/
UINT32 PMGetInstrStat(void);

//------------------------------------------------------------------------------

/** @brief The function configures and starts to count number of data 
           cache miss on remote or local 4GMX system. 
           
    @param cpu  [in] - The CPU id, the range is [0 … CPU_NUM), for a local 
                       processor this value can be set to 0xFFFFFFFF 
                      (CUR_CPU macro)
*/
void PMStartCalcDataCacheMissEx(CPUID cpu);


/** @brief The function stops counting number of data cache miss on 
           remote or local 4GMX system. 
           
    @param cpu  [in] - The CPU id, the range is [0 … CPU_NUM), for a local 
                       processor this value can be set to 0xFFFFFFFF 
                      (CUR_CPU macro)
*/
void PMStopCalcDataCacheMissEx(CPUID cpu);

/** @brief The function gets numbers of data cache miss on 
           remote or local 4GMX system. 
           
    @param cpu  [in] - The CPU id, the range is [0 … CPU_NUM), for a local 
                       processor this value can be set to 0xFFFFFFFF 
                      (CUR_CPU macro)
    @return number of data cache miss                      
*/
UINT32 PMGetDataCacheMissNumberEx(CPUID cpu);


//------------------------------------------------------------------------------

/** @brief The function configures and starts to count number of data 
           cache hit on remote or local 4GMX system. 
           
    @param cpu  [in] - The CPU id, the range is [0 … CPU_NUM), for a local 
                       processor this value can be set to 0xFFFFFFFF 
                      (CUR_CPU macro)
*/
void PMStartCalcDataCacheHitEx(CPUID cpu);

/** @brief The function stops counting number of data cache hit on 
           remote or local 4GMX system. 
           
    @param cpu  [in] - The CPU id, the range is [0 … CPU_NUM), for a local 
                       processor this value can be set to 0xFFFFFFFF 
                      (CUR_CPU macro)
*/
void PMStopCalcDataCacheHitEx(CPUID cpu);

/** @brief The function gets numbers of data cache hit on 
           remote or local 4GMX system. 
           
    @param cpu  [in] - The CPU id, the range is [0 … CPU_NUM), for a local 
                       processor this value can be set to 0xFFFFFFFF 
                      (CUR_CPU macro)
    @return number of data cache hit                      
*/
UINT32 PMGetDataCacheHitNumberEx(CPUID cpu);

//------------------------------------------------------------------------------

/** @brief The function configures and starts to count cycles 
           on remote or local 4GMX system. 
           
    @param cpu  [in] - The CPU id, the range is [0 … CPU_NUM), for a local 
                       processor this value can be set to 0xFFFFFFFF 
                      (CUR_CPU macro)
*/
void PMStartCalcCycleEx(CPUID cpu);

/** @brief The function stops counting cycles 
           on remote or local 4GMX system. 
           
    @param cpu  [in] - The CPU id, the range is [0 … CPU_NUM), for a local 
                       processor this value can be set to 0xFFFFFFFF 
                      (CUR_CPU macro)
*/
void PMStopCalcCycleEx(CPUID cpu);

/** @brief The function gets number cycles 
           on remote or local 4GMX system. 
           
    @param cpu  [in] - The CPU id, the range is [0 … CPU_NUM), for a local 
                       processor this value can be set to 0xFFFFFFFF 
                      (CUR_CPU macro)
    @return number of cyclies
*/
UINT32 PMGetCycleNumberEx(CPUID cpu);

/** @brief This function is designed to return the number of ARM clocks 
           spent by ARM in some specific mode 

    @param cpu  [in] - The CPU id, the range is [0 … CPU_NUM), for a local 
                       processor this value can be set to 0xFFFFFFFF 
                      (CUR_CPU macro)
    @param nMode[in] - mode id, see: PM_ARM_MODE_xxx

    @return [UINT32] clocks */

UINT32 PMGetCyclesForArmModeEx (CPUID cpu, UINT32 nMode);


/** @brief This function is designed to be called by the drivers 
           to collect ARM modes statictics 

    @param nMode   [in] - PM_ARM_MODE_xxx
    @param nCycles [in] - the number of cycles calculated like PMGetCycleNumber()

    */

void PMAddARMModeCycles (UINT32 nMode, UINT32 nCycles);

/** @brief This function is used to notify PM about starting of IRQ processing
           this function is used in a pair with PMEndIrqProc() 

    */           

void PMStartIrqProc (void);

/** @brief This function is used to add IRQ statistics
           this function is used in a pair with PMEndIrqProc() 

    */
void PMEndIrqProc (void);

/** @brief This function is used to add MIPS statistics
           and called from IRQ handler 

    */

void PMAddMIPSStatistics(void);

/** @brief This function sets current ARM mode 

            @param nMode[in] - ARM mode:  PM_ARM_MODE_SVSR or PM_ARM_MODE_IDLE

*/
void PMSetARMMode(UINT32 nMode);

/** @brief This function enables profiling (putting data to MLog) for the 
           specific cores 

    @param CpuMask [in] - CPU mask

    @return [MXRC] an error code */

MXRC PMEnableProf (UINT32 CpuMask);

/** @brief This function generates synchronization signal to all cores, 
           where PM is enabled 

    @param exc_mask [in] the cpu mask that must be excluded for some reason 
                         by default this value should be equal to 0

    @return [MXRC] an error code 

    */           

MXRC PMGenSyncSignal (UINT32 exc_mask);

void PMIncIrqProc (void);

/** @brief This function is designed to return the number of processed IRQ signals 

    @param cpu  [in] - The CPU id, the range is [0 … CPU_NUM), for a local 
                       processor this value can be set to 0xFFFFFFFF 
                      (CUR_CPU macro)

    @return [UINT32] number of IRQ*/
    
UINT32 PMGetARMProcIrqNumEx (CPUID cpu);

/** @brief This function is designed to return the number of cache miss situations 
            detected in the previous measurement period

    @return [UINT32] number of cache miss*/
    
UINT32 PMGetCacheMissStat(void);

/** @brief This function is designed to return the number of cache hit situations 
            detected in the previous measurement period

    @return [UINT32] number of cache hit*/
    
UINT32 PMGetCacheHitStat(void);

//------------------------------------------------------------------------------


#endif //_PM_H_

#ifdef __cplusplus
}
#endif

