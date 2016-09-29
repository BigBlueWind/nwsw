//-------------------------------------------------------------------------------------------
/** @file mlog.c
 *
 * @brief Mindspeed Transaction Executive (TrEx) Profiler and System Debug Trace Logger
 * @author Mindspeed Technologies
 * @version $Revision: 1.110 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <stdio.h>
#include <time.h>
#include "tcb.h"
#include <string.h>

#if defined(__KERNEL__)
	#include <linux/jiffies.h>
	#include <linux/smp.h>
	#include <linux/vmalloc.h>
	#include <linux/device.h>
	#include <linux/fs.h>
	#include <linux/errno.h>
	#include <mach/transcede-2200.h>
	#include <mach/sysheap.h>
	#include <asm/uaccess.h>

	static int mlog_major;      /* Major number assigned to our device driver */
	static struct class *mlog_class;
	static struct device *mlog_device;

	#define UINT32 		uint32_t
	#define UINT8 		uint8_t
	typedef void*		LPVOID;
	#define MxGetCpuID()	smp_processor_id()
	#define MxGetARMID()	(1)

	#define uart_printf 	printk
	#define _ASSERT_PTR(x)	BUG_ON(x)
	#define MLOG_BUF_SIZE	(1024*1024)  //bytes             Moved this to Config.h for EVM
	#define MLOG_LOCK_DATA
	#define MLOG_LOCK	raw_spin_lock_irqsave(&mlog_spin, irq_flags[MxGetCpuID()])
	#define MLOG_RELEASE	raw_spin_unlock_irqrestore(&mlog_spin, irq_flags[MxGetCpuID()])

	#define MLOG_GETTICK()  *((unsigned int*)TIMER0_CURR_COUNT)

	#define MLogAlloc(size)	ddr_heap_alloc(size) //vmalloc(size)
	#define MLogFree(ptr)	ddr_heap_free(ptr) //vfree(ptr)

	static unsigned long irq_flags[CPU_NUM];
	static DEFINE_RAW_SPINLOCK(mlog_spin);
#elif defined(__linux__)
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define UINT32 uint32_t
#define uart_printf printf

#define MLOG_BUF_SIZE   (4096*60)  //bytes             Moved this to Config.h for EVM
#define MLOG_LOCK_DATA  int i_never_used
#define MLOG_LOCK   pthread_mutex_lock( &mutex1 )
#define MLOG_RELEASE    pthread_mutex_unlock( &mutex1 )

#define TIMER_APB_BASEADDR 0xFE050000   // taken from T4KDev/SYSCORE/DRIVERS/hal.h, to avoid including into userspace linux tools
#define TIMER0_CURR_COUNT 0xFE050004
#define MMAP_SIZE 4096
uint32_t *timer0_mmap_ptr = NULL;
uint32_t *timers_base = NULL;
#define MLOG_GETTICK()  (*timer0_mmap_ptr)

#define MxGetCpuID()  (1)
#define MxGetARMID()  (1)

static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
#elif !defined(_WIN32)
#include "4gmx.h"
#include "config.h"
#include "heaps.h"
#include "4gmx_heap.h"
#include "hal.h"
#include "globalstoragearrays.h"
#include "appprintf.h"
#include "pm.h"

#define ZIVAR  __attribute__((zero_init))
#define MLOG_LOCK_DATA   IRQTYPE mlog_irq_var
#define MLOG_LOCK        mlog_irq_var = ARM_INT_disable()
#define MLOG_RELEASE     ARM_INT_restore(mlog_irq_var)
#define MLOG_GETTICK()   Get24MHzTick()  // GetTIMETICK()  //GTMDrvGetTimeLower() //GTMDrvGetTimeLower() //MxGetTime() //the tick here depends on how TMDrvInit() was setup, typically = 1ms

#define MLogAlloc(size)		MxHeapAlloc(AppHeap,size);
#define MLogFree(ptr)		MxHeapFree(AppHeap,ptr);
#else
#include "windows.h"
#include <assert.h>

#define MLOG_BUF_SIZE   (4096*60)  //bytes             Moved this to Config.h for EVM
#define MLOG_GETTICK()   (_mlogtime++)
#define MLOG_LOCK_DATA int i=0
#define MLOG_LOCK     EnterCriticalSection(&_criticalsection)
#define MLOG_RELEASE  LeaveCriticalSection(&_criticalsection)
#define MxGetCpuID()  (1)
#define MxGetARMID()  (1)

unsigned int _mlogtime=0;
CRITICAL_SECTION _criticalsection;

#define MLogAlloc(size)		malloc(size);
#define MLogFree(ptr)		free(ptr);

#endif

//#define MLOG_DEBUG

#ifndef MLOG_DEBUG
#define MLOG_DEBUG_PRINT(...)
#else
#if defined(__KERNEL__)
#define MLOG_DEBUG_PRINT(...) printk (__VA_ARGS__)
#elif defined(_WIN32) || defined(__linux__)
#define MLOG_DEBUG_PRINT(...) printf (__VA_ARGS__)
#else
#define MLOG_DEBUG_PRINT(...) uart_printf (__VA_ARGS__)
#endif
#endif

#include "mlog.h"


MLOGCTX mlog_ctx = {0};
MLOG_INLINE int MLogIsEnabled(void);
unsigned int MLogWriteRecord(unsigned char *rec, unsigned int size);
unsigned int MLogWriteRecordCpu(unsigned int cpu, unsigned char *rec, unsigned int size);
unsigned int MLogWriteStaticRecord(unsigned char *rec, unsigned int size);
unsigned int MLogCreateStorage(MLOG_STORAGE_CTX *ctx, LPVOID mem);
unsigned int MLogCreatePartition(unsigned int cpu);
unsigned int MLogWriteFirstTime(unsigned int cpu, unsigned char *rec, unsigned int size);
unsigned int MLogWriteRecordEx(unsigned int cpu, unsigned char *rec, unsigned int size);
unsigned int MLogWriteStaticRecordEx(unsigned int cpu, unsigned char *rec, unsigned int size);
unsigned int MLogWriteStaticRecordEx2(unsigned int cpu, unsigned char *rec, unsigned int size);
unsigned int MLogFinish(void);
void MLogOnTTIDev(void);


#if defined(_WIN32) || defined(__linux__) | defined (__KERNEL__)
void MxiDefSysPart(SYSFPART * pPart, LPVOID pStorage, UINT32 nBlkSize, UINT32 nBlkCnt);
LPVOID MxiAllocSysPart(SYSFPART * pPart);
int MxiGetBlockIndex (SYSFPART * pPart, LPVOID pBlock);
void MxiFreeSysPart(SYSFPART * pPart, LPVOID pBlk);
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Local Functions
//--------------


MLOG_INLINE int MLogIsEnabled()
{
    return (mlog_ctx.Started && mlog_ctx.StoragePtr);
}


int MLogCheckMask(UINT32 DevId)
{
    if (mlog_ctx.Started && (mlog_ctx.EnabledMask & (1<<DevId)))
        return MLOG_TRUE;
    else
        return MLOG_FALSE;
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL DOXYGEN_TO_DO
 *
 *
 *  @param  code
 *  @param  listid
 *
 *  @return none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
MLOG_INLINE void MLogTCBListPut(unsigned int code,unsigned int listid)
{
    MLOGLISTREC listrec;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    listrec.code = code;
    listrec.listid = listid;
	listrec.ticks = MLOG_GETTICK();

    if(code != MLOG_ID_LIST)
        MLogWriteRecord((unsigned char *)&listrec, sizeof(MLOGLISTREC));
    else
        MLogWriteStaticRecord((unsigned char *)&listrec, sizeof(MLOGLISTREC));

}

MLOG_INLINE void MLogTCBListPutEx(unsigned int code,unsigned int listid, unsigned int cpu)
{
    MLOGLISTREC listrec;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    listrec.code = code;
    listrec.listid = listid;
	listrec.ticks = MLOG_GETTICK();

    MLogWriteRecordCpu(cpu, (unsigned char *)&listrec, sizeof(MLOGLISTREC));
}


#ifdef MLOG_ENABLED

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// MLog APIs
//-----------


//-------------------------------------------------------------------------------------------
/** @brief This returns the Mlog Context Location
 *
 *  @param  none
 *
 *  @return MLOGCTX Pointer to MLOG Context
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
MLOGCTX * MLogGetCtx (void)
{
    return (MLOGCTX *)&mlog_ctx;
}

//-------------------------------------------------------------------------------------------
/** @brief This sets the MLOG Logging Mask
 *
 *  @param  Masks to enable
 *
 *  @return MX_OK
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
unsigned int MLogSetMask (MLOG_DWORD nMask)
{
    mlog_ctx.EnabledMask = nMask;

    if (nMask != 0)
        mlog_ctx.Started = MLOG_TRUE;
    else
        mlog_ctx.Started = MLOG_FALSE;

    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief This presets the MLOG Logging Mask
 *
 *  @param  Masks to enable
 *
 *  @return MX_OK
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
unsigned int MLogSetMaskProg (MLOG_DWORD nMask)
{
    mlog_ctx.EnabledMaskProg = nMask;
    return 0;
}

//-------------------------------------------------------------------------------------------
/** @brief This activates preset MLOG Logging Mask
 *
 *  @param  none
 *
 *  @return MX_OK
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
unsigned int MLogActivateMaskProg (void)
{
    MLogSetMask(mlog_ctx.EnabledMaskProg);
    return 0;
}


//-------------------------------------------------------------------------------------------
/** @brief This gets the MLOG Logging Mask
 *
 *  @param  none
 *
 *  @return MLOG Enable Mask
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
MLOG_DWORD MLogGetMask (void)
{
    return mlog_ctx.EnabledMask;
}

//-------------------------------------------------------------------------------------------
/** @brief This function is used to get the current size of the MLOG logged in. By default
 *         it gives the allocated size of the Log (done in MlogOpen()). Once the Log is
 *         finalized by calling MlogFinish(), it gives the correct size of the log
 *
 *
 *  @param   none
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
unsigned int MLogGetFileSize(void)
{
    if (mlog_ctx.Finalized == 0)
        return (unsigned int)-1;


    return mlog_ctx.StorageSize;
}

//-------------------------------------------------------------------------------------------
/** @brief This function is used to get the location in DDR to the start of the Logged output
 *         It is inialized in MlogOpen() function and deallocated in the MlogClose() function
 *
 *
 *  @param   none
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
unsigned int * MLogGetFileLocation(void)
{
#if defined(_WIN32) || defined(__linux__) || defined (__KERNEL__)
    return  (unsigned int *)mlog_ctx.StoragePtr->hdr->BasePointer;
#else
    return  (mlog_ctx.StoragePtr) ? (unsigned int *)mlog_ctx.StoragePtr->hdr->BasePointer : (unsigned int *)gMlogBuf;
#endif
}

//-------------------------------------------------------------------------------------------
/** @brief This allocates memory in Cacheable DDR space for the Logger and initializes the
 *         logger's internal variables. This is the first function that needs to be called to
 *         start the Mlogger. Thisfunction is automatically called by the TrEx during
 *         system Init
 *
 *  @param  none
 *
 *  @return none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
int  MLogOpen()
{
    unsigned int ret;

    MLOG_DEBUG_PRINT("MLOG Open. \r\n");

    if (mlog_ctx.Opened == MLOG_TRUE)
    {
#if defined(_WIN32) || defined(__linux__) || defined(__KERNEL__)
        uart_printf("MLOG already Opened. Please close it first!!!\r\nGoing to Infinite Loop\r\n");
#else
        MLOG_DEBUG_PRINT("MLOG already Opened. Please close it first!!!\r\nGoing to Infinite Loop\r\n");
        MxDelay(50000);
#endif
        while(1);
    }

    // Initializations for Context
    mlog_ctx.Opened = MLOG_TRUE;
    mlog_ctx.Finalized = MLOG_FALSE;
    mlog_ctx.Started = MLOG_FALSE;
    mlog_ctx.Mlogenablerc = 0;
    mlog_ctx.Mlogtaskkeycounter = 1;
    mlog_ctx.StorageSize = MLOG_FRAME_LIMIT * MLOG_FRAME_SIZE * (CPU_NUM + MLOG_DEVICE_COUNT);

#if defined (__KERNEL__)
#elif defined(__linux__)
    int fd;
    fd = open("/dev/mem", O_RDONLY);
    if (fd >= 0)
    {
        timers_base = mmap(0, MMAP_SIZE, PROT_READ, MAP_SHARED, fd, TIMER_APB_BASEADDR);
        if (timers_base == MAP_FAILED)
        {
            timer0_mmap_ptr = NULL;
            uart_printf("Error while mapping 0x%x\n", TIMER_APB_BASEADDR);
            return MLOG_FALSE;
        }
        timer0_mmap_ptr = timers_base + (TIMER0_CURR_COUNT - TIMER_APB_BASEADDR) / sizeof(timers_base[0]);
        close(fd);
    }
    else
    {
        uart_printf("Error while opening /dev/mem\n");
        return MLOG_FALSE;
    }
#elif defined(_WIN32)
    InitializeCriticalSection(&_criticalsection);
#endif

    if(!mlog_ctx.StoragePtr)
    {
        MLOG_DEBUG_PRINT("Mlog: alloc storage ptr\r\n");
        mlog_ctx.StoragePtr = MLogAlloc(sizeof(MLOG_STORAGE_CTX));

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
        _ASSERT_PTR(mlog_ctx.StoragePtr);
#endif
        memset(mlog_ctx.StoragePtr, 0, sizeof(MLOG_STORAGE_CTX));

#if defined(_WIN32) || defined(__linux__) || defined(__KERNEL__)
        mlog_ctx.StoragePtr->ptr = MLogAlloc(mlog_ctx.StorageSize);
#else
        mlog_ctx.StoragePtr->ptr = gMlogBuf;
        mlog_ctx.DevStoragePtr   = MLogDevPtr;
#endif
    }
    else
        MLOG_DEBUG_PRINT("\r\n MLOG DOUBLE OPEN!!! \r\n");

#if defined(__KERNEL__)
	uart_printf("Mlog: create storages @ 0x%x (%d)\r\n", ddr_heap_virt_to_phys((unsigned long)mlog_ctx.StoragePtr->ptr), mlog_ctx.StorageSize);
#else
    MLOG_DEBUG_PRINT("Mlog: create storages (%d)\r\n", MLOG_BUF_SIZE);
#endif

    ret = MLogCreateStorage(mlog_ctx.StoragePtr, (LPVOID)mlog_ctx.StoragePtr->ptr);

    if(ret != MX_OK)
    {
        MLOG_DEBUG_PRINT("Crating error %d \r\n", ret);
        return MLOG_FALSE;
    }

    MLOG_DEBUG_PRINT("Mlog: create operation done\r\n");
    return MLOG_TRUE;
}


//-------------------------------------------------------------------------------------------
/** @brief This function un-initializes the allocated memory for the logger and clearly all
 *         other internally used variables. This function is called automatically by the
 *         TrEx during System sutdown
 *
 *  @param  none
 *
 *  @return none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
int MLogClose()
{
    int j;
    MLOG_LOCK_DATA;

    MLOG_DEBUG_PRINT("\r\n MLOG Close. \r\n");

    MLOG_LOCK;

    if (mlog_ctx.Opened == MLOG_FALSE)
    {
#if defined(_WIN32)
        uart_printf("MLOG not Opened. Please open it first before closing!!!\r\nGoing to Infinite Loop\r\n");
#else
        MLOG_DEBUG_PRINT("MLOG not Opened. Please open it first before closing!!!\r\nGoing to Infinite Loop\r\n");
#endif
        while(1);
    }

    mlog_ctx.Opened = MLOG_FALSE;
    mlog_ctx.Started = MLOG_FALSE;
    mlog_ctx.EnabledMask = 0;

    if (mlog_ctx.StoragePtr == 0)
    {
        MLOG_RELEASE;
        return MLOG_FALSE;
    }

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
    MLogFlush();
#endif

    for(j = 0; j < (CPU_NUM + MLOG_DEVICE_COUNT); j++)
    {
        if (mlog_ctx.StoragePtr->storage[j]!= 0)
        {
            MLogFree(mlog_ctx.StoragePtr->storage[j]);
            mlog_ctx.StoragePtr->storage[j] = NULL;
        }
    }
    MLogFree(mlog_ctx.StoragePtr);
    mlog_ctx.StoragePtr = NULL;

#if defined(_WIN32)
    DeleteCriticalSection(&_criticalsection);
#endif

#if defined(__linux__) && !defined(__KERNEL__)
    if(timers_base)
        munmap(timers_base, MMAP_SIZE);
#endif

    MLOG_RELEASE;
    return MLOG_TRUE;
}


//-------------------------------------------------------------------------------------------
/** @brief This function logs a TCB into its memory.Variables that are stored are TCB ID, Resource ID, Task ID,
 *         This is usually called during the creation of the TCB. It is automatically called when the TrEx
 *         when a new TCB is created using its API
 *
 *  @param  ptcb  This is the pointer to the TCB that needs to be logged
 *
 *  @return none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogTCB(void* ptcb)
{
    MLOGTCBREC tcbrec;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    if (MLogCheckMask(((PTCB)ptcb)->ResourceID) == MLOG_FALSE)
        return;

    tcbrec.code = MLOG_ID_NEWTASK;
    tcbrec.ptcb = (MLOG_DWORD)ptcb;
    tcbrec.taskID = ((PTCB)ptcb)->TaskID;
    tcbrec.resID = ((PTCB)ptcb)->ResourceID;

    MLogWriteStaticRecord((unsigned char *)&tcbrec, sizeof(MLOGTCBREC));
}

//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL DOXYGEN_TO_DO
 *
 *
 *  @param  ptcb
 *  @param  resourcenum
 *  @param  isstart
 *  @param  ticks
 *
 *  @return none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogEXE(void* ptcb,int resourcenum,int isstart,unsigned int ticks)
{
    MLOGEXEREC exerec;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    if (MLogCheckMask(((PTCB)ptcb)->ResourceID)== MLOG_FALSE)
        return;

    exerec.code     = isstart ? MLOG_ID_EXESTART : MLOG_ID_EXEFINISH;
    exerec.ptcb     = (MLOG_DWORD)ptcb;
    exerec.ticks    = (ticks != 0) ? (ticks) : (MLOG_GETTICK());
    exerec.resID    = resourcenum;
    exerec.errcode  = 0;

    if (isstart == MLOG_FALSE)
    {
        exerec.execticks = ((PTCB)ptcb)->ExecTicks;
        MLogWriteRecord((unsigned char *)&exerec, sizeof(MLOGEXEREC));
    }
    else
    {
        MLogWriteRecord((unsigned char *)&exerec, sizeof(MLOGEXEREC) - (2*MLOG_UINTSIZE));
    }
}

//-------------------------------------------------------------------------------------------
/** @brief This function logs the start of execution of the TCB. It is automatically called by
 *         the TrEx when it dispatches a TCB
 *
 *
 *  @param  ptcb          Pointer to the TCB that needs to be logged
 *  @param  resourcenum   Resource Index of the Resource being used. If Ceva 5 is used, then 5
 *  @param  thisTicks     Clock State at this point of time. This is found by reading the internal
 *                        clock register
 *
 *  @return none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogEXEStart(void* ptcb,int resourcenum,unsigned int thisTicks)
{
    MLogEXE(ptcb,resourcenum,MLOG_TRUE,thisTicks);
}



//-------------------------------------------------------------------------------------------
/** @brief This function logs the end of execution of the TCB. It is automatically called by
 *         the TrEx when it get interrupted by the resource when TCB completed
 *
 *
 *  @param   ptcb Pointer to the TCB that needs to be logged
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogEXEFinish(void* ptcb)
{
    MLogEXE(ptcb,(((PTCB) ptcb)->ResourceIndex),MLOG_FALSE,0); //pass 0 for ticks
}

//-------------------------------------------------------------------------------------------
/** @brief This function logs the end of execution of the TCB. It is automatically called by
 *         the scheduler when it dispatches a TCB. This is used to log in the end of a TCB that
 *         is part of a SuperTCB. This is a special kind of TCB. It is automitically called by the
 *         TrEx when it get interrupted by the resource when TCB completed
 *
 *  @param   ptcb          Pointer to the TCB that needs to be logged
 *  @param   resourcenum   Resource Index of the Resource being used. If Ceva 5 is used, then 5
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogEXEFinish2(void* ptcb,int resourcenum)
{
    MLogEXE(ptcb,resourcenum,MLOG_FALSE,0); //pass 0 for ticks
}

//-------------------------------------------------------------------------------------------
/** @brief This functions helps in logging tasks that are within a TCB inside a resource.
 *         One can profile subtasks within a TCB using this approach to see how many cycles that
 *         a smaller task within a TCB took and optimize this function if needed. It is automatically
 *         called by the TrEx if the TCB has subtasks
 *
 *
 *  @param   ptcb_parent    Pointer to the TCB that needs to be logged
 *  @param   subtaskid      ID for the Subtask that needs to be profiled
 *  @param   ticks          Time of the task logged within the resource
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogSubTask(void* ptcb_parent, unsigned int subtaskid, unsigned int ticks)
{
    MLOGSUBTASKREC strec;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    if (MLogCheckMask(((PTCB)ptcb_parent)->ResourceID)== MLOG_FALSE)
        return;

    strec.code = MLOG_ID_SUBTASK;
    strec.ptcb = (MLOG_DWORD)ptcb_parent;
    strec.subID = subtaskid;
    strec.ticks = ticks;
    MLogWriteRecord((unsigned char *)&strec, sizeof(MLOGSUBTASKREC));
}



//-------------------------------------------------------------------------------------------
/** @brief This function helps in logging tasks that are not dispatched by the TrEx in one's
 *         solution. This might include things like FEC / MDMA / other hardware accelrator paths
 *         that may be out of the path of the TrEx
 *
 *
 *  @param   taskid     ID for the Subtask that needs to be profiled
 *  @param   resourceid Can be a prefined ID for a Resource that the TTBox Tool recognizes.
 *                      A list of available values are shown in table below
 *  @param   ticksstart Start time of the task logged within the resource
 *  @param   ticksstop  End time of the task logged within the resoruce
 *
 *  @return  unsigned int   This is the unique ID assigned by the MLogger to identify this task.
 *                          This ID can be used at a later stage to add dependencies to other tasks
 *                          so that the TTBox tool can display the dependecies to this task
 *
 * \anchor MLOG_RESOURCE_TABLE
 * <BR/>
 *  TABLE_START
 *  ---------------------------------------
 *  |Resource ID   |Resource Type         |
 *  ---------------------------------------
 *  |0             |RESOURCE_RSRV         |
 *  ---------------------------------------
 *  |1             |RESOURCE_LARM0        |
 *  ---------------------------------------
 *  |2             |RESOURCE_MAP          |
 *  ---------------------------------------
 *  |3             |RESOURCE_CEVA         |
 *  ---------------------------------------
 *  |4             |RESOURCE_FEC          |
 *  ---------------------------------------
 *  |5             |RESOURCE_HOST         |
 *  ---------------------------------------
 *  |6             |RESOURCE_MDMA         |
 *  ---------------------------------------
 *  |7             |RESOURCE_FECUL        |
 *  ---------------------------------------
 *  |8             |RESOURCE_FECDL        |
 *  ---------------------------------------
 *  |9             |RESOURCE_XP_AXI       |
 *  ---------------------------------------
 *  |10            |RESOURCE_SYS_AXI      |
 *  ---------------------------------------
 *  |11            |RESOURCE_SPU_AXI      |
 *  ---------------------------------------
 *  |12            |RESOURCE_RAD_AXI      |
 *  ---------------------------------------
 *  |13            |RESOURCE_LARM1        |
 *  ---------------------------------------
 *  |14            |RESOURCE_UARM0        |
 *  ---------------------------------------
 *  |15            |RESOURCE_UARM1        |
 *  ---------------------------------------
 *  |16            |RESOURCE_UARM2        |
 *  ---------------------------------------
 *  |17            |RESOURCE_UARM3        |
 *  ---------------------------------------
 *  TABLE_END
 * <BR/><BR/>
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
unsigned int MLogTask(unsigned int taskid, unsigned int resourceid , unsigned int ticksstart,unsigned int ticksstop)
{
    unsigned int key=0;
    MLOGTASKREC taskrec;

    if (MLogIsEnabled() == MLOG_FALSE)
        return 0;

    if (MLogCheckMask(resourceid)== MLOG_FALSE)
        return 0;


    if (resourceid == RESOURCE_LARM || resourceid == RESOURCE_LARM1)
    {
        if (MxGetARMID() == 0)
        { /* Quad core */
            switch (MxGetCpuID())
            {
                case 0:
                    resourceid = RESOURCE_UARM0;
                break;
                case 1:
                    resourceid = RESOURCE_UARM1;
                break;
                case 2:
                    resourceid = RESOURCE_UARM2;
                break;
                case 3:
                    resourceid = RESOURCE_UARM3;
                break;
                default:
                {
                    UINT32 tmp = 0;
                    MLOG_DEBUG_PRINT("Mlog ARM Core# is wrong\n");
                    while(tmp == 0);
                    break;
                }
            }
        }
        else
        { /* Dual Core */
            switch (MxGetCpuID())
            {
                case 0:
                    resourceid = RESOURCE_LARM;
                break;
                case 1:
                    resourceid = RESOURCE_LARM1;
                break;
                default:
                {
                    UINT32 tmp = 0;
                    MLOG_DEBUG_PRINT("Mlog Core# is wrong\n");
                    while(tmp == 0);
                    break;
                }
            }
        }
    }

    taskrec.code = MLOG_ID_TASK;
    taskrec.taskID = taskid;
    taskrec.resID = resourceid;
    taskrec.tickstart = ticksstart;
    taskrec.tickstop = ticksstop;
    taskrec.key= mlog_ctx.Mlogtaskkeycounter++;

    MLogWriteRecord((unsigned char *)&taskrec, sizeof(MLOGTASKREC));

    return key;
}

//-------------------------------------------------------------------------------------------
/** @brief This Function helps in logging the currently set frequncies of each of the different
 *         modules within the device. This is used to calculate the exact resource cycle count
 *         for the task as all logged times are based on a global clock within the system. This
 *         function is called for all resources by TrEx automatically during System Init
 *
 *
 *  @param   resourceid  This is the resource ID shown in the \ref MLOG_RESOURCE_TABLE
 *  @param   freqvalue   The frequency value of the PLL register that has been attached
 *                       to this resource
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogFREQ(unsigned int resourceid, unsigned long freqvalue)
{
    MLOGFREQREC freqreg;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    freqreg.code = MLOG_ID_FREQ;
    freqreg.resID = resourceid;
    freqreg.freqID = *((MLOG_DWORD*)&freqvalue);
    freqreg.freqval = *((MLOG_DWORD*)(((char*)&freqvalue)+MLOG_UINTSIZE));

    MLogWriteStaticRecord((unsigned char *)&freqreg, sizeof(MLOGFREQREC));
}



//-------------------------------------------------------------------------------------------
/** @brief This function is used to mark important instances in exucution path. This is currently
 *         used in instances like the completion of a subframe of processing. These markers are
 *         read by the offline TTBox tool to mark lines / delimeters in the graph when plotting
 *         the output of the MLogger
 *
 *
 *  @param   markid  The options avilable are mentioned in table below
 *  @param   ticks   State of the global clock at this instance
 *
 *  @return  none
 *
 * \anchor MLOG_MARKID_TABLE
 * <BR/>
 *  TABLE_START
 *  ---------------------------------------
 *  |MARK ID       |Type of MARK in TTBox |
 *  ---------------------------------------
 *  |0             |MLOG_MARK_NOTHING     |
 *  ---------------------------------------
 *  |1             |MLOG_MARK_FRAMEBORDER |
 *  ---------------------------------------
 *  TABLE_END
 * <BR/><BR/>
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogMark(unsigned int markid,  unsigned int ticks)
{
    MLOGMARKREC markrec;

    //MLOG_DEBUG_PRINT("MLog mark! \r\n");

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    if (markid == MLOG_MARK_FRAMEBORDER)
    {
        mlog_ctx.StoragePtr->tti_counter++;

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
        if(MLOG_DEVICE_COUNT)
            MLogOnTTIDev();
#endif
    }

    markrec.code = MLOG_ID_MARK;
    markrec.markID = markid;
    markrec.ticks = ticks;
    MLogWriteRecord((unsigned char *)&markrec, sizeof(MLOGMARKREC));
}


#ifdef MLOG_ADD_DEPENDENCY
//-------------------------------------------------------------------------------------------
/** @brief This function attaches dependencies to Tasks / TCBs / Lists that were created. These are
 *         are displayed in the TTBox tool as lines and are used to anylze where the critical path
 *         is to recommend better way to scehdule the tasks. It is automatically called by TrEx when
 *         Dependencies are added to TCBs. It can be called by the user for Tasks / Lists
 *         that were logged in using the Mlogger APIs
 *
 *
 *  @param   mode    The Mode value is defined in table below
 *  @param   param1  Pointer to TCB or Task value returned by the MlogTask function
 *  @param   param2  Pointer to TCB or Task value returned by the MlogTask function
 *
 *  @return  none
 *
 * \anchor MLOG_DEPENDENCY_MODE_TABLE
 * <BR/>
 *  TABLE_START
 *  -----------------------------------------------------------------
 *  |MODE ID                  |Dependency Type                      |
 *  -----------------------------------------------------------------
 *  |MLOG_DEPENDS_TCB_TCB     |Dependency between 2 TCBs            |
 *  -----------------------------------------------------------------
 *  |MLOG_DEPENDS_KEY_KEY     |Dependency between 2 Tasks           |
 *  -----------------------------------------------------------------
 *  |MLOG_DEPENDS_LISTID_KEY  |Dependency between a Task and a List |
 *  -----------------------------------------------------------------
 *  |MLOG_DEPENDS_KEY_LISTID  |Dependency between a List and a Task |
 *  -----------------------------------------------------------------
 *  |MLOG_DEPENDS_LISTID_TCB  |Dependency between a List and a TCB  |
 *  -----------------------------------------------------------------
 *  |MLOG_DEPENDS_TCB_LISTID  |Dependency between a TCB and a List  |
 *  -----------------------------------------------------------------
 *  TABLE_END
 * <BR/><BR/>
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogDepends(int mode,unsigned int param1,unsigned int param2)
{
    MLOGDEPREC deprec;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    deprec.mode = mode;
    deprec.param1 = param1;
    deprec.param2 = param2;

    MLogWriteRecord((unsigned char *)&deprec, sizeof(MLOGDEPREC));
}
#endif

//-------------------------------------------------------------------------------------------
/** @brief This function marks a TCB as BYPASSED. The bypassed TCB is not going to be processed
 *
 *  @param  ptcb  This is the pointer to the TCB that needs to be logged
 *  @param  markid  The ID of the mark.
 *
 *  @return none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogMarkTcbControl(void* ptcb, unsigned int markid)
{
    MLOGTCBCNTRREC tcbctrlrec;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    if (MLogCheckMask(((PTCB)ptcb)->ResourceID) == MLOG_FALSE) return;

    tcbctrlrec.code = MLOG_ID_MARK_TCB_CTRL;
    tcbctrlrec.ptcb = (MLOG_DWORD)ptcb;
    tcbctrlrec.markid = markid;

    MLogWriteRecord((unsigned char *)&tcbctrlrec, sizeof(MLOGTCBCNTRREC));
}


#ifdef MLOG_IRQ_SUP_ENABLE


//-------------------------------------------------------------------------------------------
/** @brief This function is automatically called by the 4GMX OS when the ARM switches states.
 *         It is used later on to analyze what portion of the ARM was handling interrupts and
 *         what portion of the ARM was handling dispatching / other house cleaning work
 *
 *
 *  @param  code      ID for the state of ARM. For all defined states of the ARM, please look at table below
 *  @param  resid     ARM ID.
 *  @param  resindex  This is a reserved files for future. Currently always set to 0
 *
 *  @return none
 *
 * \anchor MLOG_ARM_STATE_TABLE
 * <BR/>
 *  TABLE_START
 *  ----------------------------------------------
 *  |CODE                   |ARM State           |
 *  ----------------------------------------------
 *  |MLOG_MODE_UNKNOWN      |Unknown state       |
 *  ----------------------------------------------
 *  |MLOG_MODE_IRQ          |Servicing Interrups |
 *  ----------------------------------------------
 *  |MLOG_MODE_SUPERVISOR   |Normal Master State |
 *  ----------------------------------------------
 *  |MLOG_MODE_IDLE         |Idle State          |
 *  ----------------------------------------------
 *  TABLE_END
 * <BR/><BR/>
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogResourceCondition(unsigned int code,unsigned int resid,unsigned int resindex)
{
    MLOGRESCONDREC rescondrec;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    if (mlog_ctx.Mlogenablerc == 0)
        return;

    if (MLogCheckMask(resid)== MLOG_FALSE) return;

    rescondrec.id = MLOG_ID_RESCONDITION;
    rescondrec.code = code;
    rescondrec.resid_index = ( resid << 16 ) | resindex;
    rescondrec.ticks = MLOG_GETTICK();
    MLogWriteRecord((unsigned char *)&rescondrec, sizeof(MLOGRESCONDREC));
}
#endif



//-------------------------------------------------------------------------------------------
/** @brief This function is used to enable the logging of the state of the ARM. There are a few
 *         modes that the 4GMX OS recognizes the ARM to be under. This is automatically called
 *         by the OS during bootup to enable this feature
 *
 *
 *  @param   bVal  0 = Disable, 1 = Enable
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogEnableRC(unsigned int bVal)
{
    mlog_ctx.Mlogenablerc = bVal;

}



//-------------------------------------------------------------------------------------------
/** @brief This function is used to register the creation of a TaskList. It is automatically
 *         by the TrEx when a new list is created
 *
 *
 *  @param   listid  ID of the List being created
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogTCBList(unsigned int listid)
{
    MLogTCBListPut(MLOG_ID_LIST, listid);
}




//-------------------------------------------------------------------------------------------
/** @brief This function is used to log in the time when a task list was started to execute.
 *         It is automatically called by the TrEx when a List start execution
 *
 *
 *  @param   listid  ID of the List being Executed
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogTCBListStart(unsigned int listid)
{
    MLogTCBListPut(MLOG_ID_LISTSTART, listid);
}

void MLogTCBListStartEx(unsigned int listid, unsigned int cpu)
{
    MLogTCBListPutEx(MLOG_ID_LISTSTART, listid, cpu);
}


//-------------------------------------------------------------------------------------------
/** @brief This function is used to log in the time when a task list was started to execute.
 *         It is automatically called by the TrEx when a List finishes execution
 *
 *
 *  @param   listid  ID of the List that finished Execution
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogTCBListStop(unsigned int listid)
{
    MLogTCBListPut(MLOG_ID_LISTSTOP, listid);
}






//-------------------------------------------------------------------------------------------
/** @brief This function is to register a TCB with a certain Task List. It is automatically
 *         called by the TrEx when a TCB is added to a List
 *
 *
 *  @param   ptcb    Pointer to the TCB that needs to be logged
 *  @param   listid  ID of the List that the TCB is being added to
 *
 *  @return  none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
void MLogTCBAddToList(void* ptcb,unsigned int listid)
{
    MLOGADDLISTREC addlistrec;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    if (MLogCheckMask(((PTCB)ptcb)->ResourceID)== MLOG_FALSE)
        return;

    addlistrec.code = MLOG_ID_ADDTOLIST;
    addlistrec.ptcb = (MLOG_DWORD)ptcb;
    addlistrec.listid = listid;

    MLogWriteRecord((unsigned char *)&addlistrec, sizeof(MLOGADDLISTREC));
}



#if !defined(_WIN32)
void MLogDevInfo(unsigned devInfo)
{
    MOGDEVINFOREC devinforec;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    devinforec.code = MLOG_ID_DEVICE_INFO;
    devinforec.devinfo = devInfo;

    MLogWriteStaticRecord((unsigned char *)&devinforec, sizeof(MOGDEVINFOREC));
}

void MLogRegisterFrameSubframe(unsigned int frameNum, unsigned int subFrameNum)
{
    MLOGFRAMESFREC farmesfrec;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    farmesfrec.code = MLOG_ID_FR_SUBFR;
    farmesfrec.framenum = frameNum;
    farmesfrec.subframenum = subFrameNum;

    MLogWriteRecord((unsigned char *)&farmesfrec, sizeof(MLOGFRAMESFREC));
}

void MLogMarkAndRegisterFrameSubframe(unsigned int markid, unsigned int ticks, unsigned int frameNum, unsigned int subFrameNum)
{
    MLOGFRAMESFREC farmesfrec;
    MLOGMARKREC markrec;
    MLOGCTX *pMlogCtx = MLogGetCtx ();

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    if (MxGetCpuID() == 0)
    {
        // Store this information for other cores to use
        pMlogCtx->MLogMarkId                    = markid;
        pMlogCtx->MLogMarkTicks                 = ticks;
        pMlogCtx->MLogMarkFrameNumber           = frameNum;
        pMlogCtx->MLogMarkSubframeFrameNumber   = subFrameNum;

        mlog_ctx.StoragePtr->tti_counter++;

#if !defined(_WIN32) && !defined(__linux__)
        if(MLOG_DEVICE_COUNT)
            MLogOnTTIDev();
#endif

        markrec.code                = MLOG_ID_MARK;
        markrec.markID              = markid;
        markrec.ticks               = ticks;
        MLogWriteRecord((unsigned char *)&markrec, sizeof(MLOGMARKREC));

        farmesfrec.code             = MLOG_ID_FR_SUBFR;
        farmesfrec.framenum         = frameNum;
        farmesfrec.subframenum      = subFrameNum;

        MLogWriteRecord((unsigned char *)&farmesfrec, sizeof(MLOGFRAMESFREC));
    }
    else
    {
        markrec.code                = MLOG_ID_MARK;
        markrec.markID              = pMlogCtx->MLogMarkId;
        markrec.ticks               = pMlogCtx->MLogMarkTicks;
        MLogWriteRecord((unsigned char *)&markrec, sizeof(MLOGMARKREC));

        farmesfrec.code             = MLOG_ID_FR_SUBFR;
        farmesfrec.framenum         = pMlogCtx->MLogMarkFrameNumber;
        farmesfrec.subframenum      = pMlogCtx->MLogMarkSubframeFrameNumber;

        MLogWriteRecord((unsigned char *)&farmesfrec, sizeof(MLOGFRAMESFREC));
    }
}


void MLogAddVariables(unsigned int numVar, unsigned int *variables, unsigned int ticks)
{
    unsigned int buff[16];
    unsigned int numBytes = (numVar + 1) * sizeof(unsigned int);

    if (MxGetCpuID() != 0)
        return;

    if((numBytes + sizeof(unsigned int)) > (sizeof(buff) * sizeof(unsigned int)))
        return;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    buff[0] = MLOG_ID_ADD_VARIABLES | numBytes;
    buff[1] = ticks;

    _memcpy(buff + 2, variables, numVar * sizeof(unsigned int));

    MLogWriteRecord((unsigned char *)&buff, numBytes + sizeof(unsigned int));
}


void MLogCacheMipsStats(unsigned int ticks)
{
#ifdef PM_ENABLED
    MLOGCACHEMIPSHEADREC headrec;
    MLOGCACHEMIPSREC cmrec;
    unsigned int i;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    headrec.code = MLOG_ID_CACHE_MIPS_STATS;
    headrec.ticks = ticks;
    headrec.buffcount = CPU_NUM;

    MLogWriteStaticRecord((unsigned char *)&headrec, sizeof(MLOGCACHEMIPSHEADREC));

    for (i = 0; i < CPU_NUM; i++)
    {
        PMStopCalcDataCacheMissEx(i);
        PMStopCalcDataCacheHitEx(i);
        PMStopCalcCycleEx(i);

        cmrec.buffnum = i;
        cmrec.cachemiss = PMGetDataCacheMissNumberEx(i);    // Cache Miss
        cmrec.cachehit = PMGetDataCacheHitNumberEx(i);     // Cache Hit
        cmrec.irqcycles = PMGetCyclesForArmModeEx(i, PM_ARM_MODE_IRQ);  // Arm IRQ Cycles
        cmrec.idlecycles = PMGetCyclesForArmModeEx(i, PM_ARM_MODE_IDLE); // Arm IDLE Cycles
        cmrec.supervisiorcycles = PMGetCyclesForArmModeEx(i, PM_ARM_MODE_SVSR); // Arm SUPERVISOR Cycles
        cmrec.irqcount = PMGetARMProcIrqNumEx(i);                      // Number of IRQs

        //MLogWriteStaticRecord((unsigned char *)&cmrec, sizeof(MLOGCACHEMIPSREC));
        MLogWriteRecord((unsigned char *)&cmrec, sizeof(MLOGCACHEMIPSREC));

        PMStartCalcDataCacheMissEx(i);
        PMStartCalcDataCacheHitEx(i);
        PMStartCalcCycleEx(i);
    }
#else
    if (MLogIsEnabled() == MLOG_FALSE)
        return;
#endif

}


void MLogCacheMipsStatsSingle(unsigned int ticks)
{
#ifdef PM_ENABLED

    MLOGCACHEMIPSHEADREC headrec;
    MLOGCACHEMIPSSREC cmrec;
    unsigned int armId = MxGetCpuID();

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    headrec.code = MLOG_ID_CACHE_MIPS_STATS_SINGLE;
    headrec.ticks = ticks;
    headrec.buffcount = 8;

    cmrec.buffnum = armId;
    cmrec.cachemiss = PMGetCacheMissStat();                             // Cache Miss
    cmrec.cachehit = PMGetCacheHitStat();                              // Cache Hit
    cmrec.cachemissinst = PMGetInstrCacheMissStat();                        // Instruction Cache Miss
    cmrec.irqcycles = PMGetCyclesForArmModeEx(armId, PM_ARM_MODE_IRQ);  // Arm IRQ Cycles
    cmrec.idlecycles = PMGetCyclesForArmModeEx(armId, PM_ARM_MODE_IDLE); // Arm IDLE Cycles
    cmrec.supervisiorcycles = PMGetCyclesForArmModeEx(armId, PM_ARM_MODE_SVSR); // Arm SUPERVISOR Cycles
    cmrec.irqcount = PMGetARMProcIrqNumEx(armId);                      // Number of IRQs

    //MLogWriteStaticRecord((unsigned char *)&headrec, sizeof(MLOGCACHEMIPSHEADREC));
    //MLogWriteStaticRecord((unsigned char *)&cmrec, sizeof(MLOGCACHEMIPSSREC));

    MLogWriteRecord((unsigned char *)&headrec, sizeof(MLOGCACHEMIPSHEADREC));
    MLogWriteRecord((unsigned char *)&cmrec, sizeof(MLOGCACHEMIPSSREC));


#else
    if (MLogIsEnabled() == MLOG_FALSE)
        return;
#endif
}

void MLogCacheMipsStatsSingleEx(unsigned int ticks)
{
#ifdef PM_ENABLED
    unsigned int armId = MxGetCpuID();
    MLOGCACHEMIPSHEADREC headrec;
    MLOGCACHEMIPSREC cmrec;

    if (MLogIsEnabled() == MLOG_FALSE)
        return;

    headrec.code = MLOG_ID_CACHE_MIPS_STATS_SINGLE;
    headrec.ticks = ticks;
    headrec.buffcount = 1;

    PMStopCalcDataCacheMissEx(armId);
    PMStopCalcDataCacheHitEx(armId);
    PMStopCalcCycleEx(armId);

    cmrec.buffnum = armId;
    cmrec.cachemiss = PMGetCacheMissStat();                             // Cache Miss
    cmrec.cachehit = PMGetCacheHitStat();                              // Cache Hit
    cmrec.irqcycles = PMGetCyclesForArmModeEx(armId, PM_ARM_MODE_IRQ);  // Arm IRQ Cycles
    cmrec.idlecycles = PMGetCyclesForArmModeEx(armId, PM_ARM_MODE_IDLE); // Arm IDLE Cycles
    cmrec.supervisiorcycles = PMGetCyclesForArmModeEx(armId, PM_ARM_MODE_SVSR); // Arm SUPERVISOR Cycles
    cmrec.irqcount = PMGetARMProcIrqNumEx(armId);                      // Number of IRQs

    MLogWriteStaticRecord((unsigned char *)&headrec, sizeof(MLOGCACHEMIPSHEADREC));
    MLogWriteStaticRecord((unsigned char *)&cmrec, sizeof(MLOGCACHEMIPSREC));

    PMStartCalcDataCacheMissEx(armId);
    PMStartCalcDataCacheHitEx(armId);
    PMStartCalcCycleEx(armId);
#else
    if (MLogIsEnabled() == MLOG_FALSE)
       return;
#endif

}


#endif



//-------------------------------------------------------------------------------------------
/** @brief DOX_INTERNAL DOXYGEN_TO_DO
 *
 *  @param  none
 *
 *
 *  @return none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
unsigned int MLogFinish()
{
    unsigned int j, size, shift;
    unsigned char *copy_to;
    MLOG_LOCK_DATA;

#if defined(_WIN32)
    printf("\nCleaning up MLOG!!!\n");
#else
    MLOG_DEBUG_PRINT("\nCleaning up MLOG!!!\n");
#endif

    MLOG_LOCK;

    if (mlog_ctx.Finalized == MLOG_TRUE)
    {
        MLOG_RELEASE;
        return MLOG_TRUE;
    }

    mlog_ctx.StorageSize = sizeof(MLOGFILEHEADER) + sizeof(MLOGFHPART) * mlog_ctx.StoragePtr->hdr->PartNum;
    copy_to = mlog_ctx.StoragePtr->ptr + mlog_ctx.StorageSize;

#ifdef MLOG_DEVICE_COUNT
    for(j = CPU_NUM; j < (CPU_NUM + MLOG_DEVICE_COUNT); j++)
    {
#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
        MLOG_STORAGE_HEADER *storage = mlog_ctx.StoragePtr->storage[j];
		if(storage->mem_part.storage)
	        MxCacheInvalidate(storage->ptr, storage->mempart_size);
#endif
    }

#endif

    for(j = 0; j < mlog_ctx.StoragePtr->hdr->PartNum ; j++)
    {
        MLOG_STORAGE_HEADER *storage = mlog_ctx.StoragePtr->storage[j];
        MLOG_FRAME_BLOCK_HEADER *block;

		if(!storage->mem_part.storage)
        {
			continue;
        }

        size = storage->block_num * MLOG_FRAME_SIZE;
        mlog_ctx.StorageSize += size;
        storage->hdr->Size = size;
        if(!j)
        {
            copy_to += size;
        }
        else  if (j && storage->ptr)
        {
            memcpy(copy_to, storage->ptr, size);
            shift = storage->ptr - copy_to;

            copy_to += size;

            storage->current_block -= shift;

            if (storage->static_block != NULL)
            {
                storage->static_block -= shift;
                storage->hdr->StaticData -= shift;
                block = (MLOG_FRAME_BLOCK_HEADER *)(storage->hdr->StaticData);
            }
            else
            {
                block = NULL;
            }

            if (storage->first_block != NULL)
            {
                storage->first_block -= shift;
            }

            storage->hdr->DynamicData -= shift;

            while(block)
            {
                if(block->next)
                    block->next = (MLOG_FRAME_BLOCK_HEADER *)((UINT32)(block->next) - shift);
                block = block->next;
            }

            block = (MLOG_FRAME_BLOCK_HEADER *)(storage->hdr->DynamicData);
            while(block)
            {
                if(block->next)
                    block->next = (MLOG_FRAME_BLOCK_HEADER *)((UINT32)(block->next) - shift);
                block = block->next;
            }
        }
    }

    mlog_ctx.Finalized = MLOG_TRUE;

    MLOG_RELEASE;
    return MLOG_TRUE;
}


//-------------------------------------------------------------------------------------------
/** @brief This function is used to finalize the Mlog Logged in and print out the location
 *         and length of the Mlog stored in DDR. It is automatically called when the PHY_STOP
 *         or PHY_SHUTDOWN API are initiated from the MAC side
 *
 *
 *  @param  none
 *
 *  @return none
 *
 *  \ingroup group_lte_mlog
 *
**/
//-------------------------------------------------------------------------------------------
#if defined(__KERNEL__)
unsigned int MLogPrint()
{
	MLogFinish();

	uart_printf("Location of MLog: 0x%x %d size\n", ddr_heap_virt_to_phys((int)MLogGetFileLocation()), MLogGetFileSize());

	return MLOG_TRUE;
}
#else
unsigned int MLogPrint()
{
#if defined(_WIN32) || defined(__linux__)
    FILE* file;
#endif

    if (mlog_ctx.Opened == MLOG_FALSE)
        return MLOG_FALSE;

    if (mlog_ctx.Finalized == MLOG_TRUE)
    {
#if defined(_WIN32)
        printf("MLOG already Finalized!!!\r\n");
#else
        MLOG_DEBUG_PRINT("MLOG already Finalized!!!\r\n");
#endif
    return MLOG_TRUE;
    }

#if !defined(_WIN32) && !defined(__linux__)
    MxDelayTicks(150000);
#endif

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
    if (MLogFlush() == MLOG_FALSE)
        return MLOG_FALSE;
#endif

#if defined(_WIN32) || defined(__linux__) && !defined(__KERNEL__)
    file = fopen (MLOG_FILENAME, "wb");

    if (file == NULL)
        return MLOG_FALSE;

    fwrite(MLogGetFileLocation(), 1, MLogGetFileSize(), file);
    fclose(file);

#endif

    return MLOG_TRUE;

}
#endif

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)

unsigned int MLogFlush (void)
{
    unsigned int *pMLogArray;
    unsigned int MLogLength;

    MLogFinish();

    MLogLength = MLogGetFileSize();
    pMLogArray = MLogGetFileLocation();

    if (pMLogArray != NULL)
    {
#ifdef __KERNEL__
		uart_printf("Location of MLog: 0x%x %d size\n", ddr_heap_virt_to_phys((int)pMLogArray), MLogLength);
#endif
        uart_printf("MLog(ver %02d) location: 0x%x\r\n", MLOG_VERSION, pMLogArray);
        uart_printf("MLog storage length: %d (bytes)\r\n", MLogLength);
        uart_printf("[rdmem 0x%x %d -o mlog.bin]\r\n", pMLogArray, MLogLength);

        MxCacheClean(pMLogArray, MLogLength);
    }
    else
    {
        uart_printf("The storage is NULL!\r\n");
        return MLOG_FALSE;
    }

    mlog_ctx.Started = MLOG_FALSE;

    return MLOG_TRUE;
}
#endif

INLINE unsigned int MLogWriteRecord(unsigned char *rec, unsigned int size)
{
    return mlog_ctx.StoragePtr->wr_dyn[MxGetCpuID()](MxGetCpuID(), rec, size);
}

INLINE unsigned int MLogWriteRecordCpu(unsigned int cpu, unsigned char *rec, unsigned int size)
{
    return mlog_ctx.StoragePtr->wr_dyn[cpu](cpu, rec, size);
}


INLINE unsigned int MLogWriteStaticRecord(unsigned char *rec, unsigned int size)
{
    return mlog_ctx.StoragePtr->wr_stat[MxGetCpuID()](MxGetCpuID(), rec, size);
}


unsigned int MLogCreateStorage(MLOG_STORAGE_CTX *ctx, LPVOID mem)
{
    int i;
    MLOGFILEHEADER  *hdr;

    if(!ctx)
        return RC_MLOG_CTX_ERROR;

    hdr = (MLOGFILEHEADER *)mem;
    hdr->Magic = MLOG_MAGIC;
    hdr->Version = MLOG_VERSION;
    hdr->BasePointer = (MLOG_DWORD)mem;
    hdr->PartNum = (CPU_NUM + MLOG_DEVICE_COUNT);
    ctx->hdr = hdr;

    for(i = 0; i < CPU_NUM + MLOG_DEVICE_COUNT; i++)
    {
        ctx->storage[i] = MLogAlloc(sizeof(MLOG_STORAGE_HEADER));

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
        _ASSERT_PTR(ctx->storage[i]);
#endif

        memset(ctx->storage[i], 0, sizeof(MLOG_STORAGE_HEADER));

        ctx->storage[i]->hdr = (MLOGFHPART *)((unsigned char *)ctx->hdr + sizeof(MLOGFILEHEADER) + i * sizeof(MLOGFHPART));
        ctx->storage[i]->hdr->Index = i;
        ctx->storage[i]->hdr->Size = MLOG_FRAME_LIMIT * MLOG_FRAME_SIZE;
        ctx->storage[i]->hdr->DynamicData = ctx->storage[i]->hdr->StaticData = 0;

        if(i < CPU_NUM)
        {
            ctx->wr_dyn[i] = ctx->wr_stat[i] = MLogWriteFirstTime;
        }
        else
        {
            // ???
        }
    }

    return MX_OK;
}

unsigned int MLogWriteFirstTime(unsigned int cpu, unsigned char *rec, unsigned int size)
{
	UINT32 rc;
	MLOG_STORAGE_HEADER * storage;
	MLOG_LOCK_DATA;

	if((rc = MLogCreatePartition(cpu)) != MX_OK)
		return rc;

	storage = mlog_ctx.StoragePtr->storage[cpu];

	MLOG_LOCK;

    _memcpy(storage->static_block->cur_data, rec, size);
    storage->static_block->cur_data += size;
    storage->static_block->payload += size;
    mlog_ctx.StoragePtr->wr_dyn[cpu] = mlog_ctx.StoragePtr->wr_stat[cpu] = MLogWriteStaticRecordEx;

    MLOG_RELEASE;

    return MX_OK;
}

unsigned int MLogCreatePartition(unsigned int cpu)
{
    unsigned char * membgn = NULL;
    MLOG_STORAGE_HEADER * storage = mlog_ctx.StoragePtr->storage[cpu];
    MLOG_LOCK_DATA;

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
        _ASSERT_PTR(storage);
#endif

    MLOG_LOCK;

    membgn = ((unsigned char *)mlog_ctx.StoragePtr->hdr + sizeof(MLOGFILEHEADER) + (CPU_NUM + MLOG_DEVICE_COUNT) * sizeof(MLOGFHPART));

    if (cpu < CPU_NUM)
    {
        storage->ptr = membgn+ cpu*(MLOG_FRAME_LIMIT * MLOG_FRAME_SIZE);
    }
    else
    {
        storage->ptr = mlog_ctx.DevStoragePtr + (cpu-CPU_NUM) * (MLOG_FRAME_LIMIT * MLOG_FRAME_SIZE);
    }

    MxiDefSysPart(&(storage->mem_part),(unsigned char *)storage->ptr, MLOG_FRAME_SIZE, MLOG_FRAME_LIMIT);

    storage->mempart_size = MLOG_FRAME_LIMIT * MLOG_FRAME_SIZE;
    storage->static_block = (MLOG_FRAME_BLOCK_HEADER *) MxiAllocSysPart(&storage->mem_part);
    storage->first_block = storage->current_block = (MLOG_FRAME_BLOCK_HEADER *) MxiAllocSysPart(&storage->mem_part);

    if(!storage->current_block || !storage->static_block)
    {
        MLOG_RELEASE;
        return RC_MLOG_ALLOC_ERROR;
    }

    storage->block_num += 2;

    memset(storage->static_block, 0, sizeof(MLOG_FRAME_BLOCK_HEADER));

    storage->static_block->cur_data = ((unsigned char *)storage->static_block) + sizeof(MLOG_FRAME_BLOCK_HEADER);
    storage->static_block->size = MLOG_FRAME_SIZE - sizeof(MLOG_FRAME_BLOCK_HEADER);

    storage->hdr->StaticData = (MLOG_DWORD)storage->static_block;

    //MLOG_DEBUG_PRINT("Create storage (%d) mem ptr %X, static block ptr %X, cur block ptr %X\r\n", cpu, membgn, storage->static_block, storage->current_block);

    memset(storage->current_block, 0, sizeof(MLOG_FRAME_BLOCK_HEADER));

    storage->current_block->cur_data = ((unsigned char *)storage->current_block) + sizeof(MLOG_FRAME_BLOCK_HEADER);
    storage->current_block->size = MLOG_FRAME_SIZE - sizeof(MLOG_FRAME_BLOCK_HEADER);
    storage->hdr->DynamicData = (MLOG_DWORD)storage->current_block;

	MLOG_RELEASE;
	return MX_OK;
}

unsigned int MLogWriteRecordEx(unsigned int cpu, unsigned char *rec, unsigned int size)
{
    MLOG_STORAGE_HEADER * storage;
    MLOG_FRAME_BLOCK_HEADER * curr;
    MLOG_LOCK_DATA;

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
    _ASSERT_PTR(mlog_ctx.StoragePtr);
#endif

    storage = mlog_ctx.StoragePtr->storage[cpu];
#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
    _ASSERT_PTR(storage);
#endif

    //MLOG_DEBUG_PRINT("dynamic: storage (%d): DYN tti %d, global tti %d\r\n", cpu, storage->tti_counter, mlog_ctx.StoragePtr->tti_counter);

    MLOG_LOCK;

    curr = storage->current_block;

    if(storage->tti_counter < mlog_ctx.StoragePtr->tti_counter)
    {
        //MLOG_DEBUG_PRINT("storage (%d): DYN block num %d\r\n", cpu, storage->block_num);

        while(storage->block_num >= MLOG_FRAME_LIMIT)
        {
            if(storage->first_block)
            {
                MLOG_FRAME_BLOCK_HEADER * first = storage->first_block->next;

                MxiFreeSysPart(&storage->mem_part,storage->first_block);

                storage->first_block = first;
                storage->block_num--;
                storage->hdr->DynamicData = (MLOG_DWORD)first;
            }
            else
            {
                MLOG_DEBUG_PRINT("MLOG: First block not present!!!\r\n");
                MLOG_RELEASE;
                return RC_MLOG_ALLOC_ERROR;
            }
        }

        //MLOG_DEBUG_PRINT("dynamic storage (%d): add next tti block\r\n");
        curr->next = (MLOG_FRAME_BLOCK_HEADER *) MxiAllocSysPart(&storage->mem_part);

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
        _ASSERT_PTR(curr->next);
#endif

        if(curr->next)
        {
            curr = curr->next;
            storage->current_block = curr;

            curr->next = NULL;
            curr->payload = 0;
            curr->cur_data = ((unsigned char *)curr) + sizeof(MLOG_FRAME_BLOCK_HEADER);
            curr->size = MLOG_FRAME_SIZE - sizeof(MLOG_FRAME_BLOCK_HEADER);
            storage->block_num++;
        }
        else
        {
            MLOG_DEBUG_PRINT("MLogWriteRecordEx: no memory\r\n");
            MLOG_RELEASE;
            return 1;
        }

        storage->tti_counter = mlog_ctx.StoragePtr->tti_counter;

        if (MxGetCpuID() != 0)
            MLogMarkAndRegisterFrameSubframe(0,0,0,0);
    }
    //MLOG_DEBUG_PRINT("dynamic storage (%d): payload %d, size %d, block size %d\r\n", cpu, storage->current_block->payload, size,  storage->current_block->size);

    if(curr->payload + size > curr->size)
    {
        //MLOG_DEBUG_PRINT(" DYN Add block \r\n");

        while(storage->block_num >= MLOG_FRAME_LIMIT)
        {
            if(storage->first_block)
            {
                MLOG_FRAME_BLOCK_HEADER * first = storage->first_block->next;

                MxiFreeSysPart(&storage->mem_part,storage->first_block);

                storage->first_block = first;
                storage->block_num--;
                storage->hdr->DynamicData = (MLOG_DWORD)first;
            }
            else
            {
                MLOG_DEBUG_PRINT("MLOG: First block not present 2!!!\r\n");
                MLOG_RELEASE;
                return RC_MLOG_ALLOC_ERROR;
            }
        }

        curr->next = (MLOG_FRAME_BLOCK_HEADER *) MxiAllocSysPart(&storage->mem_part);

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
        _ASSERT_PTR(curr->next);
#endif

        if(curr->next)
        {
            curr = curr->next;
            storage->current_block = curr;

            curr->next = NULL;
            curr->payload = 0;
            curr->cur_data = ((unsigned char *)curr) + sizeof(MLOG_FRAME_BLOCK_HEADER);
            curr->size = MLOG_FRAME_SIZE - sizeof(MLOG_FRAME_BLOCK_HEADER);
            storage->block_num++;
        }
        else
        {
            MLOG_DEBUG_PRINT("MLogWriteRecordEx: no memory2\r\n");
            MLOG_RELEASE;
            return 1;
        }
    }

    _memcpy(curr->cur_data, rec, size);
    curr->cur_data += size;
    curr->payload += size;

    MLOG_RELEASE;

    return MX_OK;
}



unsigned int MLogWriteStaticRecordEx(unsigned int cpu, unsigned char *rec, unsigned int size)
{
    MLOG_STORAGE_HEADER * storage = mlog_ctx.StoragePtr->storage[cpu];
    MLOG_FRAME_BLOCK_HEADER * curr;
    MLOG_LOCK_DATA;

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
    _ASSERT_PTR(storage);
#endif

    //MLOG_DEBUG_PRINT("stat storage (%d): payload %d, size %d, block size %d\r\n", cpu, storage->static_block->payload, size,  storage->static_block->size);

    if(mlog_ctx.StoragePtr->tti_counter)
    {
        mlog_ctx.StoragePtr->wr_dyn[cpu] = MLogWriteRecordEx;
        mlog_ctx.StoragePtr->wr_stat[cpu] = MLogWriteStaticRecordEx2;
        MLogWriteRecordEx(cpu, rec, size);
        return MX_OK;
    }

    MLOG_LOCK;

    curr = storage->static_block;

    if(curr->payload + size > curr->size)
    {
        //MLOG_DEBUG_PRINT("STAT Add block \r\n");
        curr->next = (MLOG_FRAME_BLOCK_HEADER *) MxiAllocSysPart(&storage->mem_part);
#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
        _ASSERT_PTR(storage->static_block->next);
#endif

        if(curr->next)
        {
            curr = curr->next;
            storage->static_block = curr;

            curr->next = NULL;
            curr->payload = 0;
            curr->cur_data = ((unsigned char *)curr) + sizeof(MLOG_FRAME_BLOCK_HEADER);
            curr->size = MLOG_FRAME_SIZE - sizeof(MLOG_FRAME_BLOCK_HEADER);
            storage->block_num++;
        }
        else
        {
            MLOG_DEBUG_PRINT("MLogWriteStaticRecordEx: no memory\r\n");
            MLOG_RELEASE;
            return 1;
        }
    }

    _memcpy(curr->cur_data, rec, size);
    curr->cur_data += size;
    curr->payload += size;

    MLOG_RELEASE;

    return MX_OK;
}

unsigned int MLogWriteStaticRecordEx2(unsigned int cpu, unsigned char *rec, unsigned int size)
{
    MLOG_STORAGE_HEADER * storage = mlog_ctx.StoragePtr->storage[cpu];
    MLOG_FRAME_BLOCK_HEADER * curr;
    MLOG_LOCK_DATA;

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
    _ASSERT_PTR(storage);
#endif

    MLOG_LOCK;

    curr = storage->static_block;

    if(curr->payload + size > curr->size)
    {
        //MLOG_DEBUG_PRINT("Stat 2 Add block \r\n");
        curr->next = (MLOG_FRAME_BLOCK_HEADER *) MxiAllocSysPart(&storage->mem_part);
#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
        _ASSERT_PTR(curr->next);
#endif

        if(curr->next)
        {
            curr = curr->next;
            storage->static_block = curr;

            curr->next = NULL;
            curr->payload = 0;
            curr->cur_data = ((unsigned char *)curr) + sizeof(MLOG_FRAME_BLOCK_HEADER);
            curr->size = MLOG_FRAME_SIZE - sizeof(MLOG_FRAME_BLOCK_HEADER);
            storage->block_num++;
        }
        else
        {
            MLOG_DEBUG_PRINT("MLogWriteStaticRecordEx2: no memory\r\n");
            MLOG_RELEASE;
            return 1;
        }
    }

    _memcpy(curr->cur_data, rec, size);
    curr->cur_data += size;
    curr->payload += size;

    MLOG_RELEASE;

    return MX_OK;
}

#if defined(_WIN32) || defined(__linux__) || defined(__KERNEL__)

void MxiDefSysPart(SYSFPART * pPart, LPVOID pStorage, UINT32 nBlkSize,
                   UINT32 nBlkCnt)
{
    UINT32 *p;
#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
    _ASSERT(pPart != NULL);
    _ASSERT(pStorage != NULL);
#endif
    pPart->storage = (UINT32 *) pStorage;
    pPart->freeblk = (UINT32 *) pStorage;
    pPart->blkcnt = nBlkCnt;
    pPart->blksize = nBlkSize;

    p = pPart->storage;

    while (--nBlkCnt)
    {
        *p = (UINT32)(((UINT8 *) p) + nBlkSize);
        p = (UINT32 *) * p;
    }

    *p = (UINT32) NULL;
}


LPVOID MxiAllocSysPart(SYSFPART * pPart)
{
    UINT32 *p;

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
    _ASSERT(pPart != NULL);
#endif

    p = pPart->freeblk;

    if (p)
    {
        pPart->freeblk = (UINT32 *) * p;
        pPart->AllocCnt++;
    }

    return p;
}

int MxiGetBlockIndex (SYSFPART * pPart, LPVOID pBlock)
{
#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
    _ASSERT_PTR(pPart);
    _ASSERT_PTR(pBlock);
#endif
    return ((UINT32)pBlock - (UINT32)pPart->storage) / pPart->blksize;
}

void MxiFreeSysPart(SYSFPART * pPart, LPVOID pBlk)
{
#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
    _ASSERT(pPart != NULL);
    _ASSERT(pBlk != NULL);
#endif
    *(UINT32 *) pBlk = (UINT32) pPart->freeblk;
    pPart->freeblk = (UINT32 *) pBlk;

    pPart->AllocCnt--;
}


#endif

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)

MLOG_FRAME_BLOCK_HEADER * MLogDevGetCurBlock(unsigned int devid)
{
	MLOG_STORAGE_HEADER * storage = NULL;

    if(devid < (CPU_NUM + MLOG_DEVICE_COUNT))
        storage = mlog_ctx.StoragePtr->storage[devid];
    else
        return NULL;

    _ASSERT_PTR(storage);

	if(!storage->mem_part.storage)
    {
		if(MLogCreatePartition(devid) != MX_OK)
			return NULL;
    }

    // we need to put data to the DDR (CEVA needs to read correct data)
    // and also to invalidate the same memory to allow ARM read data from DDR
    // the data prepared by CEVA

    if (storage->current_block != NULL)
    {
        MxCacheFlush(storage->current_block, sizeof (MLOG_FRAME_BLOCK_HEADER));
    }

    return storage->current_block;
}

MLOG_STORAGE_HEADER * MlogDevGetCurStorage(unsigned int devid)
{
    if(devid < (CPU_NUM + MLOG_DEVICE_COUNT))
        return mlog_ctx.StoragePtr->storage[devid];
    else
        return NULL;
}

#if 0
unsigned int MLogDevWriteRecord(MLOG_STORAGE_HEADER *storage, MLOG_FRAME_BLOCK_HEADER * current_block, unsigned char *rec, unsigned int size)
{
    if(storage->current_block->payload + size <= storage->current_block->size)
    {
        _memcpy(current_block->cur_data, rec, size);
        current_block->cur_data += size;
        current_block->payload += size;
    }
    else
        return (unsigned int)-1;

    return MX_OK;
}
#endif

void MLogOnTTIDev(void)
{
    unsigned int j;
    MLOG_STORAGE_HEADER *storage;
    MLOG_STORAGE_CTX *ctx = mlog_ctx.StoragePtr;
    MLOG_LOCK_DATA;

#if 0
    unsigned char * membgn = NULL;
#endif

    MLOG_LOCK;

    for(j = CPU_NUM; j < (CPU_NUM + MLOG_DEVICE_COUNT); j++)
    {
        storage = ctx->storage[j];

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
        _ASSERT_PTR(storage);
#endif

        if(! storage->block_num)
        {
#if 1
            continue;
#else
            membgn = ((unsigned char *)mlog_ctx.StoragePtr->hdr + sizeof(MLOGFILEHEADER) + (CPU_NUM + MLOG_DEVICE_COUNT) * sizeof(MLOGFHPART));
            storage->ptr = membgn + j*(MLOG_FRAME_LIMIT * MLOG_FRAME_SIZE);
            MxiDefSysPart(&(storage->mem_part),(unsigned char *)storage->ptr, MLOG_FRAME_SIZE, MLOG_FRAME_LIMIT);

            storage->mempart_size = MLOG_FRAME_LIMIT * MLOG_FRAME_SIZE;
            storage->first_block = storage->current_block = (MLOG_FRAME_BLOCK_HEADER *) MxiAllocSysPart(&storage->mem_part);



            if(!storage->current_block)
            {
                MLOG_RELEASE;
                return;
            }

            storage->block_num += 1;

            //MLOG_DEBUG_PRINT("Create storage (%d) mem ptr %X, static block ptr %X, cur block ptr %X\r\n", cpu, membgn, storage->static_block, storage->current_block);

            memset(storage->current_block, 0, sizeof(MLOG_FRAME_BLOCK_HEADER));

            storage->current_block->cur_data = ((unsigned char *)storage->current_block) + sizeof(MLOG_FRAME_BLOCK_HEADER);
            storage->current_block->size = MLOG_FRAME_SIZE - sizeof(MLOG_FRAME_BLOCK_HEADER);
            storage->hdr->DynamicData = (MLOG_DWORD)storage->current_block;
#endif
        }

        if(storage->block_num == MLOG_FRAME_LIMIT)
        {
            MLOG_FRAME_BLOCK_HEADER * first;
            if(storage->first_block)
            {
                first = storage->first_block->next;
                MxiFreeSysPart(&storage->mem_part,storage->first_block);

                storage->first_block = first;
                storage->block_num--;
                storage->hdr->DynamicData = (MLOG_DWORD)first;
            }
            else
            {
                MLOG_DEBUG_PRINT("MLOG: First block not present!!!\r\n");
                MLOG_RELEASE;
                return;
            }
        }


        //MLOG_DEBUG_PRINT("dynamic storage (%d): add next tti block\r\n");
        storage->current_block->next = (MLOG_FRAME_BLOCK_HEADER *) MxiAllocSysPart(&storage->mem_part);

#if !defined(_WIN32) && !defined(__linux__) && !defined(__KERNEL__)
        _ASSERT_PTR(storage->current_block->next);
#endif
        storage->current_block = storage->current_block->next;

        storage->current_block->next = NULL;
        storage->current_block->payload = 0;
        storage->current_block->cur_data = ((unsigned char *)storage->current_block) + sizeof(MLOG_FRAME_BLOCK_HEADER);
        storage->current_block->size = MLOG_FRAME_SIZE - sizeof(MLOG_FRAME_BLOCK_HEADER);
        storage->block_num++;

    }

    MLOG_RELEASE;
}

#endif

#endif
