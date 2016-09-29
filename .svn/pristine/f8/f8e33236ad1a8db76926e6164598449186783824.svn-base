//-------------------------------------------------------------------------------------------
/** @file 4gmx_types.h
 *
 * @brief The file contains 4GMX common types definition
 * @author Mindspeed Technologies
 * @version $Revision: 1.24 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/
//-------------------------------------------------------------------------------------------

#ifndef _4GMX_TYPES_H_
#define _4GMX_TYPES_H_

#include "4gmx_compiler.h"
  
#ifdef __cplusplus
extern "C"
{
#endif

/************************************************************************/
/*     UINT64, UINT32, UINT16 and UINT8 definition                      */
/************************************************************************/
#ifndef _UINT64_
#define _UINT64_
#if defined (__CC_ARM)
typedef unsigned __int64 UINT64, *PUINT64;
#elif defined (__GNUC__)
typedef long long unsigned int UINT64, *PUINT64;
#endif
#endif /*_UINT64_*/

#ifndef _UINT32_
#define _UINT32_
typedef unsigned int UINT32, *PUINT32;
#endif /*_UINT32_*/

#ifndef _UINT16_
#define _UINT16_
typedef unsigned short UINT16, *PUINT16;
#endif /*_UINT16_*/

#ifndef _UINT8_
#define _UINT8_
typedef unsigned char UINT8, *PUINT8;
#endif /*_UINT8_*/

#ifndef _VUINT64_
#define _VUINT64_
#if defined (__CC_ARM)
typedef volatile unsigned __int64 VUINT64, *PVUINT64;
#elif defined (__GNUC__)
typedef volatile long long unsigned int VUINT64, *PVUINT64;
#endif
#endif /*_UINT64_*/

#ifndef _VUINT32_
#define _VUINT32_
typedef volatile unsigned int VUINT32, *PVUINT32;
#endif /*_UINT32_*/

#ifndef _VUINT16_
#define _VUINT16_
typedef volatile unsigned short VUINT16, *PVUINT16;
#endif /*_UINT16_*/

#ifndef _VUINT8_
#define _VUINT8_
typedef volatile unsigned char VUINT8, *PVUINT8;
#endif /*_UINT8_*/

/************************************************************************/
/*     SINT64, SINT32, SINT16 and SINT8 definition                      */
/************************************************************************/
#ifndef _SINT64_
#define _SINT64_
#if defined (__CC_ARM)
typedef __int64 SINT64, *PSINT64;
#elif defined (__GNUC__)
typedef signed long long SINT64, *PSINT64;
#endif
#endif /*_SINT64_*/

#ifndef _SINT32_
#define _SINT32_
typedef int SINT32, *PSINT32;
#endif /*_SINT32_*/

#ifndef _SINT16_
#define _SINT16_
typedef signed short SINT16, *PSINT16;
#endif /*_SINT16_*/

#ifndef _SINT8_
#define _SINT8_
typedef signed char SINT8, *PSINT8;
#endif /*_SINT8_*/

/************************************************************************/
/*     MXRC, LPVOID, MXCPUID, BOOL definition                           */
/************************************************************************/

#ifndef _MXRC_
#define _MXRC_
typedef UINT32 MXRC;
typedef VUINT32 VMXRC;
#endif /*_MXRC_*/

#ifndef _LPVOID_
#define _LPVOID_
typedef void *LPVOID;
typedef const void *LPCVOID;
#endif /*_LPVOID_*/

#ifndef _MXCPUID_
#define _MXCPUID_
typedef UINT32 MXCPUID;
#endif /*_MXCPUID_*/

#ifndef _BOOL_
#define _BOOL_
typedef UINT32 BOOL;
#endif /*_BOOL_*/

#ifndef _MXSYNC_
#define _MXSYNC_
typedef PVUINT32 MXSYNC;
typedef PVUINT32 VMXSYNC;
#endif /*_MXSYNC_*/

#ifndef _MXLPCSTR_
#define _MXLPCSTR_
typedef const char * LPCSTR;
#endif /*_MXLPCSTR_*/

#ifndef _MXLPSTR_
#define _MXLPSTR_
typedef char * LPSTR;
#endif /*_MXLPSTR_*/

/************************************************************************/
/*       The constants of system initialization options                 */
/************************************************************************/
typedef enum _SYS_INIT_OPT_
{
    SYS_INIT_DEF     = 0
#ifdef MX_ARM_ENABLED
  , SYS_INIT_USE_VIC     = (1 << 0)
  , SYS_INIT_LOW_VECTORS = (1 << 1)
  , SYS_INIT_AMP_MODE    = (1 << 2)
  , SYS_SMP_MASTER       = (1 << 3)
#endif

#ifdef MX_INTERCPU_ENABLED
  , SYS_INIT_NO_PROC_SMP_THREAD = (1 << 4)
#endif

} SYS_INIT_OPT;

/************************************************************************/
/*       The system objects IDs                                         */
/************************************************************************/
typedef enum _OBJTYPE_
{
    OBJ_TYPE_THREAD,
    OBJ_TYPE_MTHREAD,
    
#ifdef MX_MTHREAD_ENABLED    
    OBJ_TYPE_SEMA,
#endif    

#ifdef MX_LIST_ENABLED
    OBJ_TYPE_LIST,
#endif

#ifdef MX_QUEUE_ENABLED
    OBJ_TYPE_QUEUE,
#endif

#ifdef MX_PART_ENABLED
    OBJ_TYPE_PARTITION,
#endif

#ifdef MX_HEAP_ENABLED
    OBJ_TYPE_HEAP,
#endif

#ifdef MX_RHEAP_ENABLED
    OBJ_TYPE_RHEAP,
#endif

#ifdef MX_TIMER_ENABLED
    OBJ_TYPE_TIMER,
#endif

#ifdef MX_EVENT_ENABLED
    OBJ_TYPE_EVENT,
#endif

#ifdef MX_TRACER_ENABLED
    OBJ_TYPE_TRACER,
#endif

    OBJ_TYPE_NUM,

#ifdef MX_DRV_FRAMEWORK_ENABLED
    OBJ_TYPE_DRIVER   /**< The drivers is a special type of objects and have not partitions */
#endif
} OBJTYPE;

#ifdef MX_INTERCPU_ENABLED

typedef enum _SHAREMODE_
{
    OBJ_SHARE_OFF = (0 << 0),
    OBJ_SHARE_ON  = (1 << 0)
} SHAREMODE;

#endif   /*MX_INTERCPU_ENABLED */

/************************************************************************/
/*       The HANDLE definition                                          */
/************************************************************************/
#ifndef _HANDLE_DEF
#define _HANDLE_DEF 1
typedef struct _HANDLE_
{
    unsigned int Index :8;
    unsigned int aws_1 :1;
    unsigned int Res   :11;
    unsigned int Type  :4;   /**< The type of object, see: OBJTYPE typedef */
    unsigned int CPU   :8;   /**< The CPU ID, which object belongs         */
} HANDLE;
#endif

extern const HANDLE HNULL;

/************************************************************************/
/*      The type of fast partition used in 4GMX context                 */
/************************************************************************/
typedef struct _SYSFPART_
{
    UINT32 *storage;
    UINT32 *freeblk;
    UINT32 blksize;
    UINT32 blkcnt;
    UINT32 AllocCnt;
} SYSFPART, *PSYSFPART;


/************************************************************************/
/*      The system queue it's used like internal type of the 4GMX       */
/************************************************************************/
typedef struct _SYSQUEUE_
{
    UINT32 *storage;
    UINT16  get;
    VUINT16 put;
	UINT16  size;		// The element size should be equal to 4 bytes
	UINT16  count;		// The number of queue elements
} SYSQUEUE, *PSYSQUEUE;

#ifndef _TPRIORITY_
#define _TPRIORITY_
    typedef UINT32 TPRIORITY;
#endif /*_TPRIORITY_*/

#ifndef _DEF_TSCHED_CONTROL_
#define _DEF_TSCHED_CONTROL_
typedef enum _TSCHEDCTRL_
{
     SCHED_AT_END       = (0 << 0)
    ,SCHED_AT_BEGIN     = (1 << 0)
    ,SCHED_USE_PRIORITY = (1 << 1)

#ifdef MX_INTERCPU_ENABLED
    ,SCHED_SMP          = (1 << 2)
#endif

    ,SCHED_ONLY_PUT_MSG = (1 << 3)
    ,SCHED_BY_CONDITION = (1 << 4)

    // The system bits, it cannot be used by Application

#ifdef MX_INTERCPU_ENABLED
    ,SCHED_R_SMP_PROC   = (1 << 30)
#endif

}TSCHEDCTRL;
#endif /*_DEF_TSCHED_CONTROL_*/

#ifndef _IRQTYPE_
#define _IRQTYPE_
    typedef UINT32 IRQTYPE;
#endif /*_IRQTYPE_*/

#ifndef _CPUID_
#define _CPUID_
    typedef UINT32 CPUID;
#endif /*_CPUID_*/

#ifndef MAX
#define MAX(x, y) ((x) >= (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y) ((x) <= (y) ? (x) : (y))
#endif

#ifndef _SHAREDPROC_
typedef MXRC (*ShareProc) (HANDLE h, PVUINT32 lock);
#endif

#ifndef _UNSHAREDPROC_
typedef MXRC (*UnshareProc) (HANDLE h);
#endif

#ifndef _EXT_TRACER_DEF_TYPE_
#define _EXT_TRACER_DEF_TYPE_
typedef enum _EXT_TRACER_
{
    EXT_TRACER_START_THREAD = 0,  // To start of some specific thread
    EXT_TRACER_STOP_THREAD  = 1,  // To end of some specific thread
    EXT_TRACER_START_THREADS= 2,  // Start the loop of threads processing 
    EXT_TRACER_START_IDLE   = 4,  // The kernel starts IDLE call back 

}ExtTracer;
#endif /* _EXT_TRACER_DEF_TYPE_ */

/************************************************************************/
/*    The driver interface definition                                   */
/************************************************************************/

typedef MXRC(*DrvInit)(UINT32 nInit, LPVOID * pDrvCtx);
typedef MXRC(*DrvConfig)(LPVOID pDrvCtx, UINT32 nCfgID, LPVOID pCfgData,
                         UINT32 nDataSize);
typedef MXRC(*DrvRead)(LPVOID pDrvCtx, UINT32 nCmdID, LPVOID pCmdData,
                       UINT32 * pnDataSize);
typedef MXRC(*DrvWrite)(LPVOID pDrvCtx, UINT32 nCmdID, LPVOID pCmdData,
                        UINT32 nDataSize);

typedef struct _MXDRV_
{
    UINT32  InitParam;

    DrvInit   Init;
    DrvConfig Config;
    DrvRead   Read;
    DrvWrite  Write;

    const char *DrvName;

} MXDRV, *PMXDRV;

/************************************************************************/
/*      The commands of interrupt controller driver                     */
/************************************************************************/

typedef enum _INTC_CMDS_
{
    INTC_INIT,    /**< The driver of interrupt controller should make initialization  */
    INTC_ENABLE_IRQ,
    INTC_ENABLE_FIQ,
    INTC_ENABLE_IRQ_EX,
    INTC_ENABLE_FIQ_EX,
    INTC_DISABLE_IRQ,
    INTC_DISABLE_FIQ,
    INTC_DISABLE_IRQ_EX,
    INTC_DISABLE_FIQ_EX,
    INTC_IS_FIQ_ENABLED,
    INTC_IS_IRQ_ENABLED,
    INTC_SET_IRQ_PRIORITY,
    INTC_SET_FIQ_PRIORITY,
    INTC_RESET_IRQ_PRIORITY,
    INTC_RESET_FIQ_PRIORITY,
    INTC_SET_IRQ_PRIORITY_EX,
    INTC_SET_FIQ_PRIORITY_EX,

    INTC_RAISE_SOFT_INT,
    INTC_CLEAR_SOFT_INT,
    INTC_SAFE_WRITE_32BITS, // It checks a possibility of writing 32 bits value to the spec. address
    INTC_SAFE_READ_32BITS   // It checks a possibility of reading 32 bits value from the spec. address
} INTC_CMDS;

typedef enum _TIMER_DRV_CMD_
{
    TIMER_CMD_CONFIG,
    TIMER_CMD_ENABLE,
    TIMER_CMD_SET_CTRL,
    TIMER_CMD_SET_PERIOD,
    TIMER_CMD_SET_PRESCALE,
    TIMER_CMD_GET_PERIOD,
    TIMER_CMD_GET_TICKS,
    TIMER_CMD_GET_FREQ,
    TIMER_CMD_RESTART,
    TIMER_CMD_INIT_LOCAL_TIMER,
    TIMER_WD_CMD_CONFIG,
    TIMER_WD_CMD_ENABLE,
    TIMER_WD_CMD_SET_CTRL,
    TIMER_WD_CMD_SET_PERIOD,
    TIMER_WD_CMD_RESTART,
    TIMER_WD_CMD_SET_PRESCALE,
    TIMER_WD_CMD_DIS_WDMODE,
    TIMER_WD_CMD_CLEAR_WDRESET,
    TIMER_WD_CMD_GET_PERIOD,
    TIMER_WD_CMD_GET_TICKS,
    TIMER_CMD_GET_CONTROL,
    TIMER_WD_CMD_GET_CONTROL,
    TIMER_CMD_GET_STATUS,
} TIMER_DRV_CMD;


typedef UINT32 INTC_CPU_IDS;

/*******************************/
/*      INTC_INIT command      */
/*******************************/
typedef struct _INTC_INIT_PARAM_
{
    UINT32 n4GMXInitOpt;
} INTC_INIT_PARAM;

/*******************************************************/
/*   INTC_ENABLE_IRQ, INTC_ENABLE_FIQ,                 */
/*   INTC_DISABLE_IRQ, INTC_DISABLE_FIQ  commands      */
/*******************************************************/
typedef struct _INTC_INT_CTRL_
{
    UINT32 ID;  /**< The id of interrupt */
    UINT32 Opt;
    UINT32 CpuID;
} INTC_INT_CTRL;

/*******************************************************/
/*  INTC_IS_FIQ_ENABLED,                               */
/* INTC_IS_IRQ_ENABLED  commands                      */
/*******************************************************/

typedef struct _INTC_IS_ENABLED_
{
    UINT32 ID_Status;   /* On Input the interrupt ID, on output: TRUE or FALSE */
} INTC_IS_ENABLED;

/*******************************************************/
/*  INTC_SET_IRQ_PRIORITY,                             */
/* INTC_SET_FIQ_PRIORITY,                             */
/*  INTC_RESET_IRQ_PRIORITY,                           */
/* INTC_RESET_FIQ_PRIORITY commands                   */
/*******************************************************/

typedef struct _INTC_PRIOR_
{
    UINT32 ID;       /**< The id of interrupt  */
    TPRIORITY Prior; /**< The interrupt priority */
    UINT32 nCpuID;
} INTC_PRIOR;

/*******************************************************/
/*  INTC_RAISE_SOFT_INT,                               */
/*******************************************************/
typedef struct _INTC_RAISE_SOFTINT_
{
    UINT32 CpuList; /**< See: INTC_CPU_IDS */
    UINT32 IntId;
} INTC_RAISE_SOFTINT;


/*******************************************************/
/*   INTC_SAFE_WRITE_32BITS/INTC_SAFE_READ_32BITS      */
/*******************************************************/
typedef struct _INTC_SAFE_RW_
{
    UINT32  Address;    // The address of data
    UINT32  Value;      // For Write: the writing value, For Read: the read value

} INTC_SAFE_RW;

/** @brief This type defines 4GMX POOL object, the pool is an object 
           that is based on storage and two arrays:

    - storage     - just a storage where data blocks are located
    - first array - contains the indexes of accessible blocks and can be used to get next free index 
    - second array- contains the indexes of already used blocks and used to find elements to process it

    The algorithm is:
    ~~~~~~~~~~~~~~~~~
       1. to get an index of free element from 'FreePtr' storage 
       2. to set some data in that data block
       3. to put the index of this block to the 'UsedPtr' storage which from these elements can be taken and processed
       4. to put the index of processed block to the 'FreePtr' storage 
       
*/
typedef struct _MX_POOL_
{
    PUINT8      StoragePtr;     // The pointer to the storage where blocks are located
    UINT32      BlockNum;       // The number of blocks in storage 
    UINT32      BlockSize;      // The size of block in bytes

    PUINT32     FreePtr;        // The pointer to the storage with free object indexes  
    VUINT32     FreePut;        // PUT index used to put the new item to 'free' storage
    VUINT32     FreeGet;        // GET index used to get the new free item from 'free' storage

    PUINT32     UsedPtr;        // The pointer to the storage with 'Used' object indexes  
    VUINT32     UsedPut;        // PUT index used to put the new item to 'already used' storage
    VUINT32     UsedGet;        // GET index used to get the item from 'already used' storage

}MXPOOL, *PMXPOOL;

#ifdef __cplusplus
}
#endif

#endif /*_4GMX_TYPES_H_*/
