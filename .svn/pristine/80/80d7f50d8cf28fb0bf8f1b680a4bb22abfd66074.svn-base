//-------------------------------------------------------------------------------------------
/** @file 4gmx_kernel.h
 *
 * @brief The file contains definition of 4GMX kernel API and kernel types
 * @author Mindspeed Technologies
 * @version $Revision: 1.30 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_KERNEL_H_
#define _4GMX_KERNEL_H_

#include "4gmx_config.h"
#include "4gmx_types.h"
#include "4gmx_const.h"
#include "4gmx_thread.h"
#include "4gmx_mthread.h"
#include "4gmx_event.h"
#include "4gmx_list.h"
#include "4gmx_timer.h"
#include "4gmx_heap.h"
#include "4gmx_rheap.h"
#include "4gmx_part.h"
#include "4gmx_queue.h"

#ifdef MX_ARM_ENABLED
#include "4gmx_arm_types.h"
#endif

#ifdef MX_INTERCPU_ENABLED
#include "4gmx_intercpu.h"
#endif

#ifdef MX_MTHREAD_ENABLED
#include "4gmx_mkernel.h"
#include "4gmx_sema.h"
#endif

#ifdef MX_TRACER_ENABLED
#include "4gmx_tracer.h"
#endif


#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************/
/*      Error codes of 4GMX kernel module                               */
/************************************************************************/
#define MX_KERNEL_ALLOC_NAME_ERROR  DEF_MXRC(MXM_KERNEL, 1)
#define MX_KERNEL_SEARCH_ERROR      DEF_MXRC(MXM_KERNEL, 2)
#define MX_KERNEL_SET_NAME_ERROR    DEF_MXRC(MXM_KERNEL, 3)
#define MX_KERNEL_UNSUPPORTED_OBJ   DEF_MXRC(MXM_KERNEL, 4)
#define MX_KERNEL_DRV_DONT_USED     DEF_MXRC(MXM_KERNEL, 5)
#define MX_KERNEL_NO_ICDRV          DEF_MXRC(MXM_KERNEL, 6)
#define MX_KERNEL_IC_NOT_INIT       DEF_MXRC(MXM_KERNEL, 7)
#define MX_KERNEL_NOT_SHARED_SPACE  DEF_MXRC(MXM_KERNEL, 8)
#define MX_KERNEL_OBJ_NOT_FOUND     DEF_MXRC(MXM_KERNEL, 9)
#define MX_KERNEL_SECT_ID_ERROR     DEF_MXRC(MXM_KERNEL,10)
#define MX_KERNEL_SECT_UNSYNC       DEF_MXRC(MXM_KERNEL,11)
#define MX_KERNEL_PARAM_ERROR       DEF_MXRC(MXM_KERNEL,12)
/************************************************************************/
/*                                                                      */
/*    The 4GMX system context declaration                               */
/*                                                                      */
/************************************************************************/

#ifdef MX_NAMING_ENABLED
typedef struct _OBJNAME_
{
    HANDLE hObj;
    char name[MX_MAX_OBJ_NAME + 1];
} OBJNAME, *POBJNAME;
#endif

typedef struct _MXCONTEXT_
{
#ifdef MX_ARM_ENABLED
    INTVECTOR vectors[MX_MAX_INTERRUPTS];
    EXPENTRY  excepts[EXCEPTION_TYPE_NUM];
    UINT32    lvecaddr;

    PUINT32  mmu_l1_table_ptr;   // The pointer to the MMU layer-1 table
    PUINT32  mmu_l2_table_ptr;   // The pointer to the MMU layer-2 table (it's used to coved 4/64 page mode)
    UINT32   mmu_l2_table_size;  // The size of table in bytes
    SYSFPART mmu_l2_part;        // this partition controls MMU L2 table storage

    ARMCPUCONTEXT   ArmCtx;      // The ARM CPU context, it's used to store ARM supervisor context

#endif        /*MX_ARM_ENABLED */

#ifdef MX_DRV_FRAMEWORK_ENABLED
    UINT32 ICDrvId;                /**< The ID of interrupt controller driver     */
    UINT32 TmrDrvId;                /**< The ID of timer driver     */
    MXDRV *Drivers;                /**< The pointer to the drivers table, it can be NULL */
    LPVOID DrvCtx[MX_MAX_DRIVERS]; /**< The array of pointers to drivers' contexts   */
    UINT32 DrvCount;               /**< The number of system drivers      */
    UINT32 DrvNames[(MX_MAX_DRIVERS / 32) + 1]; /**< One bit - one driver, the bit detects
                                                     status of named or unnamed driver */
#endif        /*MX_DRV_FRAMEWORK_ENABLED */

    volatile THREAD *FST;          /**< The pointer to the first scheduled thread   */
    volatile THREAD *LST;          /**< The pointer to the last scheduled thread   */
    volatile THREAD *CET;          /**< The pointer to the current executed thread   */
    volatile THREAD *CESMPT;       /**< The pointer to the current executed thread SMP thread  */

    VUINT32 Ticks;                 /**< The counter of milliseconds spent from the system startup */
    VUINT32 FracTicks;             /**< The counter of 1ms timer fractions spent from the system startup */
    UINT32  SysCfg;                /**< The system configuration parameters, see: MXINIT::ConfigSet*/

    SYSFPART parts[OBJ_TYPE_NUM];  /**< The descriptors of partitions used for system objects allocating*/

    THREAD  threads[MX_MAX_THREADS];

#ifdef MX_MTHREAD_ENABLED    

    UINT32            MThreadID;   /**< The unique ID assigned to created threads                       */

    MTHREAD *         MFST;        /**< The pointer to the first scheduled m-thread                     */
    MTHREAD *         MLST;        /**< The pointer to the last scheduled m-thread                      */
    MTHREAD *         MCET;        /**< The pointer to the current executed m-thread                    */
    MTHREAD *         CETTBI;      /**< The pointer to the current executed m-thread terminated by IRQ  */
    HANDLE            hSSThread;   /**< M-Thread that handles single-stack threads                      */
    MTHREAD *         SleepList;   /**< The list of the threads that called MxSleep and waited for event*/ 
    
    UINT32            SchedNum;     /**< The number of scheduled m-threads at this moment                */
    UINT32            ThreadSwNum;  /**< The global counter of threads switching                         */

    UINT32           IrqStartTime; /**< The timer ticks when interrupt was happened                     */
    void   (*AppIrqHandler)(void); /**< The pointer to the application IRQ handler                      */

    MTHREAD mthreads[MX_MAX_MTHREADS];
    MXSEMA  semas   [MX_MAX_SEMA];
    
#endif

#ifdef MX_EVENT_ENABLED
    EVENT events[MX_MAX_EVENTS];
#endif        /*MX_EVENT_ENABLED */

#ifdef MX_LIST_ENABLED
    LIST lists[MX_MAX_LISTS];
#endif        /*MX_LISTS_ENABLED */

#ifdef MX_TIMER_ENABLED
    PTIMER FirstTimer;        /**< The pointer to the first activated timer processed by TIMER ISR */
    PTIMER LastTimer;         /**< The pointer to the last activated timer processed by TIMER ISR  */

    PTIMER FirstFracTimer;    /**< The pointer to the first activated timer processed by TIMER ISR */
    PTIMER LastFracTimer;     /**< The pointer to the last activated timer processed by TIMER ISR  */

    TIMER timers[MX_MAX_TIMERS];
#endif        /*MX_TIMER_ENABLED */

#ifdef MX_HEAP_ENABLED

    LPVOID SysHeap;     /**< The pointer to the memory of system heap, it can be null   */
    UINT32 SysHeapSize; /**< The size of system heap in bytes         */

    HANDLE hDefHeap;    /**< The descriptor of default heap associated with HNULL    */

    HEAP heaps[MX_MAX_HEAPS];

#endif        /*MX_HEAP_ENABLED */

#ifdef MX_RHEAP_ENABLED
    RHEAP rheaps[MX_MAX_RHEAPS];
#endif        /*MX_RHEAP_ENABLED */

#ifdef MX_PART_ENABLED
    PART partitions[MX_MAX_PARTITIONS];
#endif        /*MX_PART_ENABLED */

#ifdef MX_QUEUE_ENABLED
    QUEUE queues[MX_MAX_QUEUES];
#endif        /*MX_QUEUE_ENABLED*/

#ifdef MX_TRACER_ENABLED
    MX_TRACER_CTX tracers[MX_MAX_TRACERS];
#endif        /*MX_TRACER_ENABLED*/

    volatile BOOL StopScheduler;

#ifdef MX_NAMING_ENABLED
    UINT32 NameCount;
    OBJNAME NameTable[MX_MAX_NAMETABLE_SIZE];
#endif        /*MX_NAMING_ENABLED */

#ifdef MX_INTERCPU_ENABLED
    MXICCTX **ICTable;      /**< The 4GMX IC contexts array    */
    HANDLE hInterCpuThr;    /**< The inter-cpu commands parser */
    MXSMPCTX* ICSMPCtx;     /**< The pointer to the SMP context*/
#endif        /*MX_INTERCPU_ENABLED */

#ifdef MX_EXT_THREAD_TRACER_ENABLED
    void (*ExtThrTracer) (ExtTracer nCtrl); /**< The pointer to the external threads tracer*/
#endif

#ifdef MX_HWSEMA_ENABLED
    UINT32  HwSemaBase;     /**< The base address of HW semaphores */
#endif

#ifdef _MX_DEBUG
    PUINT8 TraceBufBegin;   /**< The pointer to the debug trace ring buffer */
    PUINT8 TraceBufEnd;     /**< The pointer to the end of buffer           */
    PUINT8 TracePut;        /**< The pointer to the place of trace data storing*/
    PUINT8 TraceGet;        /**< The pointer to the place of trace data loading*/
    UINT32 TraceBlkCount;   /**< The total number of trace blocks placed to the debug trace */
#endif

#if (MX_SAFE_SECTION_NUM > 0)

    IRQTYPE SafeSectStack [MX_SAFE_SECTION_NUM + 1];
    UINT32  SafeSectPut; // the index of free cell

    IRQTYPE IRQStatStack [MX_INT_STATUS_ARRAY + MX_INT_STATUS_ARRAY + 1];
    UINT32  IRQStatPut; // the index of free cell
    
#endif

} MXCONTEXT, *PMXCONTEXT;


typedef struct _MX_INIT_
{
    UINT32 ConfigSet;

#ifdef MX_ARM_ENABLED
    UINT32  lvecaddr;

    PUINT32 mmu_l1_table_ptr;   // The pointer to the MMU layer-1 table
    PUINT32 mmu_l2_table_ptr;   // The pointer to the MMU layer-2 table (it's used to coved 4/64 page mode)
    UINT32  mmu_l2_table_size;  // The size of table in bytes

#endif

#ifdef MX_HEAP_ENABLED
    LPVOID SysHeap;     /**< The pointer to the memory of system heap, it can be null   */
    UINT32 SysHeapSize; /**< The size of system heap in bytes         */
#endif

#ifdef MX_DRV_FRAMEWORK_ENABLED
    UINT32 ICDrvId;     /**< The ID of Interrupt controller driver     */
    UINT32 TmrDrvId;     /**< The ID of timer driver     */
    MXDRV *Drivers;     /**< The pointer to the drivers table, it can be NULL */
#endif

#ifdef MX_INTERCPU_ENABLED
    MXICCTX **ICTable;  /**< The 4GMX IC contexts array                    */
    UINT16   ICIrq;     /**< The inter-cpu IRQ id (communication event)    */
    MXICCTX *ICCtx;     /**< The pointer to the inter-cpu context storage  */
    MXSMPCTX*ICSMPCtx;  /**< The pointer to the SMP context                */
#endif

#ifdef _MX_DEBUG
    LPVOID TraceRingBuf;    /**< The pointer to the debug trace ring buffer */
    UINT32 TraceBufSize;    /**< The size of ring buffer in bytes */
#endif

#ifdef MX_EXT_THREAD_TRACER_ENABLED
    void (*ExtThrTracer) (ExtTracer nCtrl); /**< The pointer to the external threads tracer*/
#endif

#ifdef MX_HWSEMA_ENABLED
    UINT32  HwSemaBase;     /**< The base address of HW semaphores */
#endif

} MXINIT, *PMXINIT;

/** @brief The function returns current CPU ID used for system execution
 @return [UINT32] CPU ID [0 ... N] */

#ifdef __GNUC__
static
#endif
UINT32 MxGetCpuID(void);

/** @brief The function returns pointer to the 4GMX context based on CPU id

 @return [PMXCONTEXT] a pointer to 4GMX context or NULL */

INLINE PMXCONTEXT MxInqContext(void)
{
    extern PMXCONTEXT MXCtx[];  
#ifdef _MX_DEBUG

    UINT32 nCpu = MxGetCpuID();;

    _ASSERT(MXCtx[nCpu] != NULL);

    return MXCtx[nCpu];

#else
    return MXCtx[MxGetCpuID()];
#endif

}

/** @brief The function returns a pointer to the 4GMX context for the specified CPU ID

 @return [PMXCONTEXT] a pointer to 4GMX context or NULL */

INLINE PMXCONTEXT MxInqRemContext(UINT32 nCpuID)
{
    extern PMXCONTEXT MXCtx[];  /**< The Array of 4GMX contexts  */

    _ASSERT (nCpuID < MX_MAX_INTERCPU);
    _ASSERT(MXCtx[nCpuID] != NULL);

    return MXCtx[nCpuID];
}


/** @brief The function initializes 4GMX system and should be called before MX API usage

 @param pInit[in] - the configuration parameters of system
 @param pCtx [out] - the context is used like a system storage where 4GMX contains flags, objects, and so on ..

 @return [MXRC] an initialization error code */

MXRC MxInit(PMXINIT pInit, MXCONTEXT * pCtx);

/** @brief The function starts 4GMX scheduler

 @param idle [in] - the pointer to the idle function  (it can be null)
 @param param[in] - the parameter of idle function

 @return [MXRC] error code */

MXRC MxStart(TENTRY idle, LPVOID pParam);

/** @brief The function should be called for stopping 4GMX scheduler,
   it can be called from ISR and from thread contexts of execution.

 @return [MXRC] error code */

MXRC MxStop(void);

/** @brief The function returns the version of 4GMX system
    @param pHVer [out] - the hight version number
    @param pLVer [out] - the low version number

    @return [MXRC] error code of operation */

MXRC MxGetVersion(UINT8 * pHVer, UINT8 * pLVer);

/** @brief The function returns an object handle specified by name
    @param cpu_id [in] - the CPU id where object will be searched
    @param name   [in] - the ASSCIZ name of object
    @param outHand[out]- the object handle

    @return [MXRC] error code of operation */

MXRC MxGetObjByName(CPUID cpu_id, const char *name, HANDLE * outHand);


/** @brief The function associates the object with the ASCIIZ name
    @param h   [in] - the object handle
    @param name[in] - the ASSCIZ name

    @return [MXRC] error code of operation */

MXRC MxSetObjName(HANDLE h, LPCSTR name);

/** @brief The function returns a pointer to the object's name or NULL
    @param h   [in] - the object handle

    @return [const char*] the pointer to the object name or NULL*/

LPCSTR MxGetObjName(HANDLE h);

/** @brief The function returns a type of object (see OBJTYPE)
    @param h [in] - the object handle

    @return [OBJTYPE] the type of object */

INLINE OBJTYPE MxGetObjType(HANDLE h)
{
    return HANDLE_GET_TYPE(h);
}

/** @brief The function returns currently executed thread handle or HNULL

    @return [HANDLE] the handle of current thread or HNULL */

HANDLE MxGetCurrentThread(void);

/** @brief The function returns currently executed thread function or NULL

    @return [UINT32] the handle of current thread or NULL */
UINT32 MxGetCurrentThreadProc(void);


/** @brief The function returns a number of ticks spent from the system startup

    @return [UINT32] the ticks number */

UINT32 MxGetTime(void);

/** @brief The function returns a number of ticks spent from the system startup

    @return [UINT32] the ticks number

    \ingroup group_lte_4gmx_kernel
*/
UINT32 MxGetFracTime(void);

/** @brief This function returns number of ticks, 150 ticks is equal to 1us
           it depends on the system initialization, and usage of this function is
           related to usage of device HW, it's strongly recomented to check device before usage of this function

    @return [UINT32] ticks in unit,  150 unit = 1us */

UINT32 MxGetTicks(void);

/** @brief The function sets/changes the number of ticks spent from the system startup
    @param nTime [in] - new value of ticks

    @return [MXRC] error code of operation */

MXRC MxSetTime(UINT32 nTime);

/** @brief The function waits specified number of ms

    @NOTE: The timer driver should be activated

    @param nTime [in] - the number of ms

    @return [MXRC] error code of operation */

MXRC MxDelay (UINT32 nTimeout);

/** @brief This function uses on-board timer to wait some number of ticks

    @param nTickCount [in] - the tick number to wait

    @return [MXRC] an error code*/

MXRC MxDelayTicks (UINT32 nTickCount);

/** @brief The function returns a total number of created objects of "type" type
    @param type [in] - the type of objects

    @return [UINT32] number of created objects*/

UINT32 MxGetObjCount(OBJTYPE type);

/** @brief The function returns a total number of created objects of "type" type

    @param nCpuID[in] - the CPU index
    @param type  [in] - the type of objects

    @return [UINT32] number of created objects

    \ingroup group_lte_4gmx_kernel
*/
UINT32 MxGetObjCountEx(UINT32 nCpuID, OBJTYPE type);

/** @brief The function returns object's handle, specified by the object
           type and object index.

    @param type   [in] - the type of objects
    @param nIndex [in] - the object index

    @return [UINT32] the object handle or HNULL*/

HANDLE MxGetObjByIndex(OBJTYPE type, UINT32 nIndex);

/** @brief The function returns object's handle, specified by the object
           type and object index.

    @param nCpuID [in] - the CPU id of remoute system
    @param type   [in] - the type of objects
    @param nIndex [in] - the object index

    @return [UINT32] the object handle or HNULL*/

HANDLE MxGetObjByIndexEx(UINT32 nCpuID, OBJTYPE type, UINT32 nIndex);

/** @brief The function gets exclusive access to the object

    @param pObj [in] - the address of the object

    @return [MXRC] error code */

#ifdef _MX_DEBUG

MXRC    MxGetLock (VUINT32 * pObj);
IRQTYPE MxGetLockEx (VUINT32 * pObj);

#else // for release mode we need to increase the speed of syncronization

extern void MxiGetLock (VUINT32 * p);
extern IRQTYPE MxiGetLockEx (VUINT32 * p);
#define MxGetLock(x)    MxiGetLock (x)
#define MxGetLockEx(x)  MxiGetLockEx (x)

#endif  //_MX_DEBUG

/** @brief The function releases(frees) exclusive access

    @param pObj [in] - the address of the object

    @return [MXRC] error code */

INLINE MXRC MxReleaseLock (VUINT32 * pObj)
{
#ifdef MX_ARM_ENABLED

#if 0
    extern void MxDMB(void);
    extern void MxDSB(void);
    extern UINT32 ARM_INT_disable(void);
    extern void ARM_INT_restore(UINT32 a);
    IRQTYPE r;

    _ASSERT_PTR(pObj);

    r = ARM_INT_disable();

    MxDMB();
    MxDSB();
    *pObj = 0;
    MxDMB();
    MxDSB();

    ARM_INT_restore (r);
#else 
    extern UINT32 MxiReleaseLock(VUINT32 * pObj);
    _ASSERT_PTR(pObj);
    MxiReleaseLock (pObj);
#endif    
    
#else
    #error "The MxLockObj should be implemented for the CEVA processor"
#endif
    return MX_OK;
}

/** @brief The function releases(frees) exclusive access

    @param pObj [in] - the address of the object
    @param irq  [in] - the IRQ/FIQ status

    @return [MXRC] error code */

INLINE MXRC MxReleaseLockEx (VUINT32 * pObj, IRQTYPE irq)
{
#ifdef MX_ARM_ENABLED
    extern void MxDMB(void);
    extern void MxiReleaseLockEx(VUINT32 * pLock, UINT32 irq);

    _ASSERT_PTR(pObj);
    MxiReleaseLockEx (pObj, irq);
#else
    #error "The MxLockObj should be implemented for the CEVA processor"
#endif
    return MX_OK;
}

#ifdef MX_INTERCPU_ENABLED

    MXRC MxShareObject(HANDLE h, SHAREMODE nMode);

    /** @brief  The function schedules inter-cpu handler, it can be called
            from thread/ISR execution context

     @return [MXRC] error code of operation */

    MXRC MxScheduleInterCpu(void);

    /** @brief This function changes default priority of inter-cpu thread
     @param pr [in] - the new thread priority
     @return [pr] error code */

    MXRC MxSetInterCpuPriority(TPRIORITY pr);

#endif

/** @brief This function sets a value of the variable in exclusive way (with usage of synhronization)

    @param pVar [in] - the pointer to the variable
    @param nVal [in] - the value of the variable

    @return [UINT32] the set value */

UINT32  MxExcSet (UINT32 * pVar, UINT32 nVal);

/** @brief This function increments the variable in exclusive way (with usage of synhronization)

    @param pVar [in] - the pointer to the variable

    @return [UINT32] the new value of variable */

UINT32  MxExcInc (VUINT32 * pVar);

/** @brief This function decrements the variable in exclusive way (with usage of synhronization)

    @param pVar [in] - the pointer to the variable

    @return [UINT32] the new value of variable */

UINT32  MxExcDec (VUINT32 * pVar);


#if (MX_SAFE_SECTION_NUM > 0)

    /** @brief This function starts 'safe' section of code, IRQs are disabled
               the lock is established 

        @param nSectID [in] - the section ID, ID < MX_SAFE_SECTION_NUM
        @return [MXRC] an error code 

        \ingroup group_lte_4gmx_kernel

        */
    MXRC MxOpenSafeSection (UINT32 nSectID);

    /** @brief This function closes 'safe' section of code, IRQs are restored
               the lock is released 

        @return [MXRC] an error code 

        \ingroup group_lte_4gmx_kernel

        */
    MXRC MxCloseSafeSection (void);

    /** @brief This function disables IRQ signals and pushes status to the stack

        @NOTE: Please use MxIntRestore() to restore the system
    
        @return [MXRC] an error code 
        \ingroup group_lte_4gmx_kernel
        */

    MXRC MxIntDisable (void);

    /** @brief This function disables IRQ signals and pushes status to the stack

        @NOTE: Please use MxIntRestore() to restore the system
    
        @return [MXRC] an error code 
        \ingroup group_lte_4gmx_kernel
    */

    MXRC MxIntRestore (void);
    
#endif // (MX_SAFE_SECTION_NUM > 0)

#ifdef MX_DRV_FRAMEWORK_ENABLED

    /** @brief The function returns driver handle based on the index of driver in drivers table.
     @param nDrvIndex [in] - the drivers index
     @return [HANDLE] driver handle (it is used in driver API)*/

    HANDLE MxGetDrv(UINT32 nDrvIndex);

    /** @brief The function configures driver's parameter.
     @param hDrv   [in] - the driver handle (see MxGetDrv function)
     @param nCfgParam [in] - the id of driver parameter
     @param pParamData [in] - the pointer to data
     @param nDataSize [in] - the data size (in bytes)

     @return [MXRC] error code of configuration */

    MXRC MxConfigDrv(HANDLE hDrv, UINT32 nCfgParam, LPVOID pParamData,
                     UINT32 nDataSize);

    /** @brief The function writes data to driver
     @param hDrv   [in] - the driver handle (see MxGetDrv function)
     @param nCmdID  [in] - the id of command
     @param pCmdData  [in] - the pointer to data
     @param nDataSize [in] - the data size (in bytes)

     @return [MXRC] error code of writing */

    MXRC MxWriteDrvData(HANDLE hDrv, UINT32 nCmdID, LPVOID pCmdData,
                        UINT32 nDataSize);

    /** @brief The function reads driver data
     @param hDrv   [in] - the driver handle (see MxGetDrv function)
     @param nCmdID  [in] - the id of command
     @param pCmdData  [out]- the pointer to data
     @param nDataSize [in/out] - in: the data size of buffer (in bytes)
             out:the size of placed data (in bytes)

     @return [MXRC] error code of reading */

    MXRC MxReadDrvData(HANDLE hDrv, UINT32 nCmdID, LPVOID pCmdData,
                       UINT32 * pnDataSize);

    /** @brief The function returns pointer to driver context.
     @param hDrv [in] - the driver handle
     @return [LPVOID] a pointer to driver context, it can be NULL */

    LPCVOID MxGetDrvCtx(HANDLE hDrv);

#endif /*MX_DRV_FRAMEWORK_ENABLED */

/** @brief This function processes 1ms timers 
    @param pCtx [in] - OS context
    @return [MXRC] an error code */

MXRC MxProcTimers(PMXCONTEXT pCtx);

/** @brief This function is responsible to process fraction timers 
           fraction means timers with less than 1ms resolution (1ms/n),
           where n is the number of fractions

    @param pCtx [in] - the OS context 

    @return [MXRC] an error code 
    */

MXRC MxProcFractionTimers(PMXCONTEXT pCtx);

/** @brief The function raises inter-cpu interrupt by using interrupt controller driver
 @param nCpuList [in] - the List of CPUs (bit mask)
 @param nIntId   [in] - the ID of interrupt :  0 - 15
 @return [MXRC] error code */

MXRC MxRaiseInterCpu(INTC_CPU_IDS nCpuList, UINT16 nIntId);

/** @brief The function cleares inter-cpu interrupt by using interrupt controller driver
 @param nCpuList [in] - the List of CPUs (bit mask)
 @param nIntId   [in] - the ID of interrupt :  0 - 15
 @return [MXRC] error code */

MXRC MxClearInterCpu(INTC_CPU_IDS nCpuList, UINT16 nIntId);

/** @brief This function is designed to read 32bits data
       in safety context (data exception is handled)

    @param pAddr [in]   -   an address
    @param pData [out]  -   the read data (if return code is SUCCESS)

    @return [MXRC] an error code of operation */

MXRC MxSafeRead32 (PUINT32 pAddr, UINT32 * pData);

/** @brief This function is designed to write 32bits data
       in safety context (data exception is handled)

    @param pAddr [in]  -   an address
    @param nData [in]  -   write data

    @return [MXRC] an error code of operation */

MXRC MxSafeWrite32 (PUINT32 pAddr, UINT32 nData);

/**@brief This function is designed to load CPU context
          the data is specific for the processor, 
          ARM processor is described with <ARMCPUCONTEXT> type 

    @param nCpuID   [in] - the CPU id
    @param pCpuCtx  [out]- the pointer to the structure to where context will be loaded

    @return [MXRC] an error code of operation*/

MXRC MxLoadCpuContext (UINT32 nCpuID, void * pCpuCtx);

#ifdef __cplusplus
}
#endif

#endif /*_4GMX_KERNEL_H_*/
