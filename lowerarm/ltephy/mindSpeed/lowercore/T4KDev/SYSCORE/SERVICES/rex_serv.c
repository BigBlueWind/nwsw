//-------------------------------------------------------------------------------------------
/** @file rex_serv.c
 *
 * @brief The Rapid eXecutive (REX) server implementation, the server part is used
 *        by original REX service to process requests of REX and to free processor MIPS
 *        where original REX is located
 *
 * @author Mindspeed Technologies
 * @version $Revision: 1.45 $
 *
 * COPYRIGHT&copy; 2008-2012 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "rex_serv.h"
#include "mlogceva.h"
#include "cevamdmadrv.h"
#include "cevafpdrv.h"

#ifdef CEVA_PROJ
#include "cevaprintf.h"
#endif

#define REXS_KERNEL_ENTRY               (1<<0)
#define REXS_KERNEL_RECV_CMD            (1<<1)
#define REXS_KERNEL_FREE_CMD            (1<<2)
#define REXS_PROC_LIST                  (1<<3)
#define REXS_RUN_ON_CEVA                (1<<4)
#define REXS_PUSH_LIST                  (1<<5)
#define REXS_INIT_CMD                   (1<<6)
#define REXS_EXEC_LIST_CMD              (1<<7)
#define REXS_TCB_IS_DONE_CMD            (1<<8)
#define REXS_PROC_DEP                   (1<<9)
#define REXS_ADD_TCB_TO_SCHEDULING      (1<<10)
#define REXS_TCB_CTRL                   (1<<11)
#define REXS_SET_CTRL_CMD               (1<<12)
#define REXS_RESET_CTRL_CMD             (1<<14)
#define REXS_UNKNOWN_CMD                (1<<15)
#define REXS_LIST_IS_DONE               (1<<16)
#define REXS_PROC_LIST_TCB              (1<<17)
#define REXS_PROC_RES                   (1<<18)
#define REXS_RUN_TCB_CB                 (1<<19)
#define REXS_PROC_RES_BEGIN             (1<<20)
#define REXS_PROC_RES_END               (1<<21)
#define REXS_SHUTDOWN                   (1<<22)
#define REXS_IRQ_HANDLER_ENTRY          (1<<23)
#define REXS_IRQ_HANDLER_CMD            (1<<24)
#define REXS_IRQ_HANDLER_EXIT           (1<<25)
#define REXS_PROC_LIST_TCB_ON_HW        (1<<26)
#define REXS_REINIT_LIST_CMD            (1<<27)

const UINT32 rexs_dbg_msk = 0                           |
                            //REXS_KERNEL_ENTRY           |
                            //REXS_KERNEL_RECV_CMD        |
                            //REXS_KERNEL_FREE_CMD        |
                            //REXS_RUN_ON_CEVA            |
                            //REXS_PUSH_LIST              |
                            //REXS_INIT_CMD               |
                            //REXS_EXEC_LIST_CMD          |
                            //REXS_TCB_IS_DONE_CMD        |
                            //REXS_PROC_DEP               |
                            //REXS_ADD_TCB_TO_SCHEDULING  |
                            //REXS_TCB_CTRL               |
                            //REXS_SET_CTRL_CMD           |
                            //REXS_RESET_CTRL_CMD         |
                            //REXS_UNKNOWN_CMD            |
                            //REXS_PROC_LIST_TCB          |
                            //REXS_PROC_RES               |
                            //REXS_RUN_TCB_CB             |
                            //REXS_PROC_RES_BEGIN         |
                            //REXS_PROC_RES_END           |
                            //REXS_SHUTDOWN               |
                            //REXS_LIST_IS_DONE           |
                            //REXS_IRQ_HANDLER_ENTRY      |
                            //REXS_IRQ_HANDLER_CMD        |
                            //REXS_IRQ_HANDLER_EXIT       |
                            //REXS_PROC_LIST_TCB_ON_HW    |
                            //REXS_REINIT_LIST_CMD        |
                            0
                            ;


#define REXS_TRACE_SET_INF      		0x001
#define REXS_TRACE_KERNEL_ENTRY			0x002
#define REXS_TRACE_RECV_CMD    		 	0x003
#define REXS_TRACE_PROC_DONE   		 	0x004
#define REXS_TRACE_PROC_LIST            0x005
#define REXS_TRACE_ROUTE_TO_CLIENT      0x006
#define REXS_TRACE_ROUTE_IS_DONE        0x007
#define REXS_TRACE_ROUNTED_TO_FREE_CEVA 0x008
#define REXS_TRACE_PUSHED_TO_WAIT_CEVA  0x009
#define REXS_TRACE_NOTIFY_SLAVE_CEVA    0x00A
#define REXS_TRACE_NOTIFY_IS_DONE       0x00B
#define REXS_TRACE_SLAVE_CEVA_DONE      0x00C
#define REXS_TRACE_ROUTE_TCB_CB         0x00D
#define REXS_TRACE_ROUTE_TCB_CB_DONE    0x00E
#define REXS_TRACE_LIST_IS_PUSHED       0x00F
#define REXS_TRACE_RUN_TCB				0x010
#define REXS_TRACE_LIST_IS_DONE			0x011
#define REXS_TRACE_LIST_IS_DONE_SENT    0x012

#ifdef CEVA_PROJ

	void dbg_printf(){}
	void rexs_printf(UINT32 a,...) { a = a; }

    void RexsTrace(unsigned int id);
    void RexsTraceData(unsigned int id, unsigned int data);
    void RexsTraceData2(unsigned int id, unsigned int data, unsigned int data2);
    void RexsTraceData3(unsigned int id, unsigned int data, unsigned int data2, unsigned int data3);

	#if 0
		#define REXS_TRACE(x)			        RexsTrace((UINT32)(x));
		#define REXS_TRACE_DATA(id,x)	        RexsTraceData((UINT32)(id), (UINT32)(x));
		#define REXS_TRACE_DATA2(id,x,x2)	    RexsTraceData2((UINT32)(id), (UINT32)(x), (UINT32)(x2));
		#define REXS_TRACE_DATA3(id,x,x2,x3)	RexsTraceData3((UINT32)(id), (UINT32)(x), (UINT32)(x2), (UINT32)(x3));
	#else
		#define REXS_TRACE(x)
		#define REXS_TRACE_DATA(x,y)
		#define REXS_TRACE_DATA2(x,y,z)
		#define REXS_TRACE_DATA3(x,y,z,a)
	#endif
#else

	#define dbg_printf(...)
	//#define rexs_printf(mask,...) {if (rexs_dbg_msk & (mask)) {uart_printf (__VA_ARGS__); MxDelayTicks (150000);}}
    #define rexs_printf(mask,...)

    #define REXS_TRACE(x)
    #define REXS_TRACE_DATA(x,y)
    #define REXS_TRACE_DATA2(x,y,z)
    #define REXS_TRACE_DATA3(x,y,z,a)

    //#define MLogTask(...) MLogTask(__VA_ARGS__)
    //#define MLogTask(...)

    // for the ARM just to use
    #define MemCpyFromExt   memcpy
    #define MemCpyToExt     memcpy

#endif

#ifdef REXS_TASKS_MLOG_ENABLED
#define _MLogDevTask(a,b,c,d)   MLogDevTask(a,b,c,d)
#else
#define _MLogDevTask(a,b,c,d)
#endif


/******************************************************************************/
/*                         Local service variables                            */
/******************************************************************************/
static unsigned int CEVAPOOLTABLE[10] = {1, 4, 7, 2, 5, 8, 3, 6, 9, 0};
REX_SERV_CTX rex_serv_ctx;
TASKLIST     rex_list_stor [REX_MAX_LIST_NUM];
REXTCBD      rex_tcbd_stor [REX_MAX_TCB_NUM + REX_MAX_PRIVATE_TCB_NUM];

#ifndef CEVA_PROJ

typedef struct _REX_SERV_ERR_DESCR_
{
    UINT32  Code;
    char *  Descr;
}REX_SERV_ERR_DESCR;

REX_SERV_ERR_DESCR RexServErrorTable [] =
{
        {REX_SERV_RC_UNKNOWN_CMD,           "Unknown command is received"},
        {REX_SERV_RC_SKIP,                  "Dependency is skipped"},
        {REX_SERV_RC_DEP_ERROR,             "Dependency conflict is detected"},
        {REX_SERV_RC_DEP_OVERLOADED,        "Number of dependency is more than allowed"},
        {REX_SERV_RC_LIST_IS_SCHED,         "List is under processing"},
        {REX_SERV_RC_DOUBLE_ADD_TO_LST,     "TCB is already added to the list"},
        {REX_SERV_RC_LIST_IS_SCHEDULED,     "RemoveListCmd but list is under processing"},
        {REX_SERV_RC_LIST_IS_UNDER_PROC,    "ExecCmd but a list is already under processing(was not completed in time)"},
        {REX_SERV_RC_MLOG_OVERFLOW,         "CEVA-MLOG is overflowed for this TTI"},
        {REX_SERV_RC_LIST_CREATE_ERROR,     "Cannot create list, system does not have memory"},
        {REX_SERV_RC_TCBD_CREATE_ERROR,     "Cannot ñreate TCB, system does not have memory"},
        {REX_SERV_RC_LIST_ID_ERROR,         "List ID is specified incorrectly"},
        {REX_SERV_RC_NOT_FREE_CELLS,        "System does not have free MEM-CELL descriptors"},
        {REX_SERV_RC_NOT_FREE_CELLS_TCB,    "MEM-CELL does not have space for new TCB"},
        {REX_SERV_RC_TCBD_ERROR,            "TCB is not assigned to TCB-descriptor"},
        {REX_SERV_RC_TCBD_LIST_ERROR,       "TCB is not assigned to the list"},

        {0,                                 "none"}
};

#endif // CEVA_PROJ

void RexServCutFromSchedList(TASKLIST * pList, REXTCBD * pTcbD, UINT32 nTcbDCtrl);
void RexServAddToScheduled (PTASKLIST pList, REXTCBD * pTcbD);
void RexServCutFromList (REXTCBD * pTcbD);

/** @brief This function returns a pointer to REX-server context
    @return [REX_SERV_CTX*] the pointer */

REX_SERV_CTX* RexServGetCtx (void)
{
    return &rex_serv_ctx;
}

REX_SERV_COMM_INF * RexServGetInf (void)
{
    return rex_serv_ctx.pInf;
}

void RexsTrace(unsigned int id)
{
	REX_SERV_CTX * pCtx;
	pCtx = RexServGetCtx();

	if (pCtx != NULL)
	{
		if (pCtx->TracePtr != NULL)
		{
			if (pCtx->TraceOff * 4 <= pCtx->TraceSize)
			{
				pCtx->TracePtr [pCtx->TraceOff] = id;
				pCtx->TraceOff ++;
			}
		}
	}
}

void RexsTraceData(unsigned id, unsigned int data)
{
	REX_SERV_CTX * pCtx;
	pCtx = RexServGetCtx();

	if (pCtx != NULL)
	{
		if (pCtx->TracePtr != NULL)
		{
			if ((pCtx->TraceOff * 4 + 8) <= pCtx->TraceSize)
			{
			    id |= (1 << 28);

				pCtx->TracePtr [pCtx->TraceOff+0] = id;
				pCtx->TracePtr [pCtx->TraceOff+1] = data;
				pCtx->TraceOff += 2;
			}
		}
	}
}

void RexsTraceData2(unsigned id, unsigned int data, unsigned int data2)
{
	REX_SERV_CTX * pCtx;
	pCtx = RexServGetCtx();

	if (pCtx != NULL)
	{
		if (pCtx->TracePtr != NULL)
		{
			if ((pCtx->TraceOff * 4 + 12) <= pCtx->TraceSize)
			{
			    id |= (2 << 28);

				pCtx->TracePtr [pCtx->TraceOff+0] = id;
				pCtx->TracePtr [pCtx->TraceOff+1] = data;
                pCtx->TracePtr [pCtx->TraceOff+2] = data2;
				pCtx->TraceOff += 3;
			}
		}
	}
}

void RexsTraceData3(unsigned id, unsigned int data, unsigned int data2, unsigned int data3)
{
	REX_SERV_CTX * pCtx;
	pCtx = RexServGetCtx();

	if (pCtx != NULL)
	{
		if (pCtx->TracePtr != NULL)
		{
			if ((pCtx->TraceOff * 4 + 16) <= pCtx->TraceSize)
			{
			    id |= (3 << 28);

				pCtx->TracePtr [pCtx->TraceOff+0] = id;
				pCtx->TracePtr [pCtx->TraceOff+1] = data;
                pCtx->TracePtr [pCtx->TraceOff+2] = data2;
                pCtx->TracePtr [pCtx->TraceOff+3] = data3;
				pCtx->TraceOff += 4;
			}
		}
	}
}

void RexServDefSysPart(REXSYSFPART * pPart, LPVOID pStorage, UINT32 nBlkSize,
                   UINT32 nBlkCnt)
{
    UINT32 *p;

    _ASSERT(pPart != NULL);
    _ASSERT(pStorage != NULL);

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

LPVOID RexServAllocSysPart(REXSYSFPART * pPart)
{
    UINT32 *p;

    _ASSERT(pPart != NULL);

    p = pPart->freeblk;

    if (p)
    {
        pPart->freeblk = (UINT32 *) * p;
        pPart->AllocCnt++;
    }

    return p;
}

UINT32 RexServGetBlockIndex (REXSYSFPART * pPart, LPVOID pBlock)
{
    _ASSERT_PTR(pPart);
    _ASSERT_PTR(pBlock);

    return ((UINT32)pBlock - (UINT32)pPart->storage) / pPart->blksize;
}

void RexServFreeSysPart(REXSYSFPART * pPart, LPVOID pBlk)
{
    _ASSERT(pPart != NULL);
    _ASSERT(pBlk != NULL);

    *(UINT32 *) pBlk = (UINT32) pPart->freeblk;
    pPart->freeblk = (UINT32 *) pBlk;

    pPart->AllocCnt--;
}

/******************************************************************************
*                   CEVA/4GMX common API                                      *
******************************************************************************/

void RexPoolFree(REX_CMD_POOL * pPool, void * pBlock);
void RexServProcDependency (REX_SERV_CTX * pCtx, REXTCBD * pProcTcbD);

/** @brief This function is designed to initialize REX pools used to allocate the commands
           (client<->server interface)

    @param pPool    [in] - the pool handler
    @param pStorage [in] - the pool storage

*/
void RexInitPool (REX_CMD_POOL * pPool, RexCMD * pStorage)
{
    unsigned int i;

    memset (pPool, 0, sizeof (*pPool));

    pPool->Storage    = (unsigned char*)pStorage;
    pPool->BlockSize  = sizeof (RexCMD);

    // to put the indexes to the free storage

    i = 0;

    while (i < REX_SERVER_CMD_NUM)
    {
        RexPoolFree (pPool, pPool->Storage + (pPool->BlockSize * i));
        i++;
    }
}

/** @brief This function allocates (returns) a pointer to the free block

    @param pPool [in] - the pool descriptor

    @return [void*] the pointer to the allocated block*/

RexCMD* RexPoolAlloc (REX_CMD_POOL * pPool)
{
    unsigned int nIndex;
    unsigned int get = pPool->FreeGet;

    if (get == pPool->FreePut)
        return 0;

    nIndex = pPool->free_pool[get++];

    if (get >= (REX_SERVER_CMD_NUM + 1))
    {
        pPool->FreeGet = 0;
    }
    else
    {
        pPool->FreeGet = get;
    }

    return (RexCMD*)(pPool->Storage + (pPool->BlockSize * nIndex));
}

/** @brief This function frees block (puts block index to the 'free storage')

    @param pPool [in]  - the pool descriptor
    @param pBlock[in]  - the pointer to the block allocated by MxiPoolAlloc(...)

*/

void RexPoolFree(REX_CMD_POOL * pPool, void * pBlock)
{
    unsigned int index;
    unsigned int put;

    index = ((unsigned int)pBlock - (unsigned int)pPool->Storage) / /*pPool->BlockSize*/ sizeof (RexCMD);

    put = pPool->FreePut;

    pPool->free_pool [put++] = index;

    if (put >= (REX_SERVER_CMD_NUM + 1))
    {
        pPool->FreePut = 0;
    }
    else
    {
        pPool->FreePut = put;
    }
}

/** @brief This function sends data block (puts a block index to the 'used storage')

    @param pPool [in] - the pool descriptor
    @param pBlock[in] - the pointer to the block, allocated by MxiPoolAlloc(...)

    @return [BOOL] TRUE - sending is done successfully */

void RexPoolSend(REX_CMD_POOL * pPool, void * pBlock)
{
    unsigned int index;
    unsigned int put = pPool->ProcPut;

    index = ((unsigned int)pBlock - (unsigned int)pPool->Storage) / /*pPool->BlockSize*/sizeof (RexCMD);
    pPool->proc_pool [put++] = index;

    if (put >= (REX_SERVER_CMD_NUM+1))
    {
        pPool->ProcPut = 0;
    }
    else
    {
        pPool->ProcPut = put;
    }
}

/** @brief This function returns the pointer to the sent block
           (reads index from the 'used storage' and converts it to the pointer)

    @param pPool [in] - the pool descriptor

    @return [RexCMD*] the pointer to the received block or NULL*/

RexCMD* RexPoolRecv(REX_CMD_POOL * pPool)
{
    unsigned int index;
    unsigned int get = pPool->ProcGet;

    if (get == pPool->ProcPut)
        return NULL;

    index = pPool->proc_pool[get++];

    if (get >= (REX_SERVER_CMD_NUM + 1))
    {
        pPool->ProcGet = 0;
    }
    else
    {
        pPool->ProcGet = get;
    }

    return (RexCMD*)(pPool->Storage + (index * pPool->BlockSize));
}

#ifndef CEVA_PROJ

#include "rex.h"
#include "heaps.h"
#include "icdrv.h"
#include "rex.h"
#include "DSPdrv.h"
#include "mlog.h"
#include "rexcomp.h"
#include "config.h"
#include "mmumgr.h"
#include "fpdrv.h"

#define DEF_REX_SERV_CTX() REX_SERV_CTX * pCtx = RexServGetCtx()

#ifdef REX_SERV_ON_ARM
static HANDLE  hServThread;        // The handle of the server part thread on 4gmx
#endif

static VUINT32 ServThreadRun;

void RexServProcCevaDoneCmd(RexCMD* pCmd);

/** @brief This function allocates (returns) a pointer to the free block

    @param pPool [in] - the pool descriptor

    @return [void*] the pointer to the allocated block*/

INLINE RexCMD* RexPoolAllocServCmd (REX_SERV_CTX * pCtx)
{
    return RexPoolAlloc(pCtx->serv_inf[MxGetCpuID()]);
}

INLINE void RexPoolSendToServ(REX_SERV_CTX * pCtx, RexCMD * pCmd)
{
    RexPoolSend(pCtx->serv_inf[MxGetCpuID()], pCmd);
}

INLINE void RexServLock(REX_SERV_CTX* pCtx, VUINT32 * irq)
{
    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(irq);

    *irq = ARM_INT_disable();
    //MxGetLock (&pCtx->InfLock);
}

INLINE void RexServUnlock(REX_SERV_CTX* pCtx, VUINT32 * irq)
{
    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(irq);

    //MxReleaseLock (&pCtx->InfLock);
    ARM_INT_restore(*irq);
}

/** @brief The platform specific function to generate signal
           to the server part of REX
*/
void INLINE RexServNotifyServ (void)
{
    #ifdef REX_SERV_ON_ARM
        MXRC rc;
        REX_SERV_CTX* pCtx = RexServGetCtx();
        _ASSERT_PTR(pCtx);

        _ASSERT_HANDLE(hServThread);
        rc = MxScheduleThread(hServThread);
        _ASSERT_RC(rc);
        rc = rc;

    #else
        //DspDrvNotifyRex();
    #endif
}

/** @brief The platform specific function to generate signal
           to the client part of REX
*/
void RexServNotifyClient (void)
{
    ICDrvRaiseInterCpu (RexGetMasterCpu(), REX_SERV_CLIENT_IRQ_ID);
}

UINT32 RexServSendAndWaitForSyncCmd(REX_SERV_CTX * pCtx, RexCMD * pCmd, UINT32 timeout)
{
    UINT32 t = MxGetTicks();

    _ASSERT_PTR(pCmd);

    if (timeout == 0)
        timeout = REX_SERV_TIMEOUT_SYNC_CMD;

    pCmd->Status = 0;

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();

    while (pCmd->Status == 0)
    {
        _NOP();
        _NOP();
        _NOP();
        _NOP();

        if (MxGetTicks () - t > timeout)
            return 0;
    }

    return 1;
}

UINT32 RexServSendAndWaitForSyncCmdUnlock(REX_SERV_CTX * pCtx, RexCMD * pCmd, UINT32 timeout, VUINT32* irq)
{
    UINT32 t = MxGetTicks();

    _ASSERT_PTR(pCmd);

    if (timeout == 0)
        timeout = REX_SERV_TIMEOUT_SYNC_CMD;

    pCmd->Status = 0;

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();
    RexServUnlock(pCtx, irq);

    while (pCmd->Status == 0)
    {
        _NOP();
        _NOP();
        _NOP();
        _NOP();

        if (MxGetTicks () - t > timeout)
            return 0;
    }

    return 1;
}

MXRC RexProcServCmd (RexCMD * pCmd);

void ASMPROC RexServLoadCmd(RexCMD * pDst, RexCMD * pSrc)
{
    PUSH {r4, r5, r6, r7, r8, r9, r10}

    LDM r1, {r2, r3, r4, r5, r6, r7, r8, r9, r10}
    STM r0, {r2, r3, r4, r5, r6, r7, r8, r9, r10}

    POP  {r4, r5, r6, r7, r8, r9, r10}

    BX LR
}

void RexServProcRexServCmds(REX_SERV_CTX * pCtx)
{
    RexCMD * pCmd;
    RexCMD localCmd;
    _ASSERT_PTR(pCtx);

    rexs_printf(REXS_IRQ_HANDLER_ENTRY, "REX-SERV: IRQ handler entry, cpu-%d, mode=0x%x\r\n",
            MxGetCpuID(),
            MxGetCpuMode ()
    );

    while ((pCmd = RexPoolRecv(&pCtx->pInf->client)) != NULL)
    {
        _ASSERT_PTR(pCmd);

        rexs_printf(REXS_IRQ_HANDLER_CMD, "REX-SERV: IRQ handler cmd(%d), cpu-%d, mode=0x%x\r\n",
            pCmd->CmdID,
            MxGetCpuID(),
            MxGetCpuMode ()
        );

        RexServLoadCmd(&localCmd, pCmd);

        if (localCmd.CmdID == REX_CMD_CEVA_PROC_DONE)
        {
            RexServProcCevaDoneCmd(&localCmd);
        }
        else
        {
            RexProcServCmd (&localCmd);
        }
        RexPoolFree (&pCtx->pInf->client, pCmd);
    }

    rexs_printf(REXS_IRQ_HANDLER_EXIT, "REX-SERV: IRQ handler exit, cpu-%d, mode=0x%x\r\n",
            MxGetCpuID(),
            MxGetCpuMode ()
    );
}

void RexServCevaHandler(void * Ptr)
{
    REX_SERV_CTX * pCtx = (REX_SERV_CTX *)Ptr;

    _ASSERT_PTR(pCtx);

    RexServProcRexServCmds(pCtx);
}

void RexServIrqHandler(volatile UINT32 * pAckReg, UINT32 nIrqID, LPVOID pParam)
{
    REX_SERV_CTX * pCtx = (REX_SERV_CTX *)pParam;

    _ASSERT_PTR(pCtx);

    ICDrvCleanInterCpu(nIrqID);
    *pAckReg = nIrqID;

    RexServProcRexServCmds(pCtx);
}

/** @brief This is REX server ARM thread, the debug entry point

    @param p [in] - the pointer to the REX-Serv context

    @return [UINT32] */

UINT32 RexServArmThread (LPVOID p)
{
    MLogDevInit();
    RexServKernel(1);
    return 0;
}

#ifdef REX_SERV_ON_ARM

/** @brief This function initializes REX-server part on ARM processor,
           this is completelly debug mode and used in case of development / debugging only

    @param pCtx [in] - the pointer to the REX server context

    @return [MXRC] an error code */

MXRC RexServInitOnArm(REX_SERV_CTX * pCtx)
{
    MXRC rc;

    _ASSERT_PTR(pCtx);

    ServThreadRun = 1;
    rc = MxCreateThreadEx(REX_SERV_ARM_CORE_ID, RexServArmThread, NULL, pCtx, 0, &hServThread);
    _ASSERT_RC(rc);

    return rc;
}

MXRC RexServDeInitOnArm (REX_SERV_CTX * pCtx)
{
    MXRC rc = MX_OK;

    _ASSERT_PTR(pCtx);

    rexs_printf(REXS_SHUTDOWN, "REX-SERV shutdown on cpu-%d\r\n", MxGetCpuID());

    ServThreadRun = 0;

    _NOP();
    _NOP();
    _NOP();

    if (HANDLE_NOT_NULL(hServThread))
    {
        rc = MxRemoveThread(hServThread);
    }

    _ASSERT_RC(rc);
    return rc;
}

#endif

/** @brief This function sends command to the CEVA part REX to stop any processing
           and just to go to infinine loop

    @param pCtx [in] - the pointer to the REX-S context

    @return [MXRC] an error code*/

MXRC RexServStopCmd(REX_SERV_CTX * pCtx)
{
    RexCMD * pCmd;
    IRQTYPE irq;
    UINT32 nTicks;

    _ASSERT_PTR(pCtx);

    RexServLock (pCtx, &irq);

    pCmd = RexPoolAllocServCmd (pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        return REX_SERV_RC_SHD_CMD_ERR;
    }

    pCmd->CmdID = REX_CMD_SHUTDOWN | REX_CMD_OPT_BLOCKING;
    pCmd->Status= 0;

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();

    nTicks = MxGetTicks ();

    while (pCmd->Status == 0)
    {
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();
        _NOP();

        // if time > 10 ms
        if (MxGetTicks () - nTicks > 1500000)
        {
            RexServUnlock (pCtx, &irq);
            return REX_SERV_RC_SHD_TIMEOUT;
        }
    }

    RexServUnlock (pCtx, &irq);

    return MX_OK;
}

#if REX_MAX_PRIVATE_TCB_NUM > 0

MXRC RexServInitPrvTcb(REX_SERV_CTX * pCtx)
{
    UINT32 i;
    TCB * pTcb;
    RexCMD* pCmd;
    IRQTYPE irq;

    for (i = 0; i < REX_MAX_PRIVATE_TCB_NUM; i++)
    {
        pTcb = RexCreateTCB(0, RESOURCE_CEVA, NULL, 0, NULL, NULL);
        _ASSERT_PTR(pTcb);

        if (pTcb == NULL)
            return REX_SERV_RC_PRV_INIT_ERR;

        RexServLock (pCtx, &irq);
        pCmd = RexPoolAllocServCmd(pCtx);

        if (pCmd == NULL)
        {
            RexServUnlock (pCtx, &irq);
            _ASSERT_PTR(pCmd);
            return REX_SERV_RC_ALLOC_CMD_ERROR;
        }

        pCmd->CmdID = REX_CMD_PRV_TCB | REX_CMD_OPT_BLOCKING;
        pCmd->Param.PrvTcb.nIndex = i;
        pCmd->Param.PrvTcb.pTcb   = pTcb;

        if (RexServSendAndWaitForSyncCmd(pCtx, pCmd, MS_TO_TICKS(20)) == 0)
        {
            RexServUnlock (pCtx, &irq);
            return REX_SERV_RC_SCMD_TIMEOUT;
        }

        if (pCmd->RC != MX_OK)
        {
            RexServUnlock (pCtx, &irq);
            return pCmd->RC;
        }

        RexServUnlock (pCtx, &irq);
    }

    return MX_OK;
}

#endif

/** @brief To provide REX server side with configuration parameters
    @param pCtx [in]    - the context
*/

MXRC RexServSendInit (REX_SERV_CTX * pCtx, REXCFG* pCfg)
{
    RexCMD * pCmd;
    TCB * pTcb = NULL;

    #if REX_MAX_PRIVATE_TCB_NUM > 0 || !defined(REX_SERV_ON_ARM)
    MXRC rc;
    #endif

#ifndef REX_SERV_ON_ARM

    pTcb = MxHeapAlloc (DspHeap, sizeof (TCB));

    if (pTcb == NULL)
        return REX_SERV_RC_ALLOC_TCB_ERROR;

    memset (pTcb, 0, sizeof (*pTcb));

    pTcb->TaskID = 1;
    pTcb->InputDataPtr = (void*)pCtx->pInf;

    rc = DspDrvSendToRexServ(pTcb, 1);

    if (FAILED(rc))
        return REX_SERV_RC_SEND_TCB_ERROR;

#endif

    // till block is allocated
    pCmd = RexPoolAllocServCmd(pCtx);
    _ASSERT_PTR(pCmd);

    if (pCmd == NULL)
    {
        if (pTcb != NULL)
            MxHeapFree (DspHeap, pTcb);

        return REX_SERV_RC_ALLOC_CMD_ERROR;
    }
    memset (pCmd, 0, sizeof (*pCmd));

    pCmd->CmdID = REX_CMD_INIT;

    pCmd->Param.Init.ctrl           = pCfg->Ctrl;
    pCmd->Param.Init.CommInfPtr     = pCtx->pInf;
    pCmd->Param.Init.MLogPtr        = NULL;
    pCmd->Param.Init.hw_mask        = pCfg->Res[RESOURCE_CEVA];

    #ifdef REXS_POOLS_ENABLED
    pCmd->Param.Init.hw_mask_pool_a = pCfg->Res[REX_RESOURCE_CEVA_POOL_A];
    pCmd->Param.Init.hw_mask_pool_b = pCfg->Res[REX_RESOURCE_CEVA_POOL_B];
    #endif

    pCmd->Param.Init.SubTaskNum  = pCtx->SubTaskNumPerCeva;
    pCmd->Param.Init.SubTaskInfo = pCtx->SubTaskInfo;

    #if REXS_TRACE_SIZE > 0

        pCmd->Param.Init.TracePtr       = MxHeapAlloc (DspHeap, REXS_TRACE_SIZE);
        pCmd->Param.Init.TraceSize      = (pCmd->Param.Init.TracePtr != NULL)?REXS_TRACE_SIZE:0;

        pCtx->TracePtr = pCmd->Param.Init.TracePtr;
        pCtx->TraceSize= REXS_TRACE_SIZE;

        if (pCtx->TracePtr != NULL)
            memset (pCtx->TracePtr, 0x0, REXS_TRACE_SIZE);

        uart_printf (" *  TRACE-ADDR = 0x%x\r\n", pCmd->Param.Init.TracePtr);
        uart_printf (" *  TRACE-SIZE = %d\r\n", REXS_TRACE_SIZE);

    #endif

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();

    if (pTcb != NULL)
        MxHeapFree (DspHeap, pTcb);

    #if REX_MAX_PRIVATE_TCB_NUM > 0

        if (FAILED(rc = RexServInitPrvTcb(pCtx)))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        uart_printf (" * PRV-TCB-NUM = %d\r\n", REX_MAX_PRIVATE_TCB_NUM);
    #endif

    return MX_OK;
}

/** @brief To initialize sub-server part, it has to be called on 4GMX
           to initialize communication with CEVA server part

    @param plpCfg [in] - the pointer to the REX configuration parameters

    @return [MXRC] an error code of operation */

MXRC RexServInit (void * plpCfg)
{
    MXRC rc = MX_OK;

    rex_serv_ctx.pInf = (REX_SERV_COMM_INF *)MxHeapAlloc (REX_SERV_INF_HEAP, sizeof (*rex_serv_ctx.pInf));

    if (rex_serv_ctx.pInf == NULL)
        return REX_SERV_RC_ALLOC_ERROR;

    if (REXS_SUB_TASK_NUM != 0)
    {
        rex_serv_ctx.SubTaskInfo = (TCB_SUB_TASK_INFO*)MxHeapAlloc (REX_SERV_INF_HEAP, sizeof(TCB_SUB_TASK_INFO)*REXS_SUB_TASK_NUM*10);

        if (rex_serv_ctx.SubTaskInfo == NULL)
        {
            MxHeapFree (REX_SERV_INF_HEAP, rex_serv_ctx.pInf);
            return REX_SERV_RC_ALLOC_ERROR;;
        }

        rex_serv_ctx.SubTaskNumPerCeva = REXS_SUB_TASK_NUM;

        uart_printf (" *   SUB-TASKS = (%d) sub-tasks per CEVA\r\n", REXS_SUB_TASK_NUM);
        uart_printf (" *    SUB-BASE = 0x%x\r\n", rex_serv_ctx.SubTaskInfo);
    }
    else
    {
        rex_serv_ctx.SubTaskNumPerCeva = 0;
        rex_serv_ctx.SubTaskInfo = NULL;
    }

    #ifdef REX_SERV_ON_ARM
        uart_printf (" *   SERV-CEVA = ARM-EMULATOR(core-%d) (CMD size %d)\r\n", REX_SERV_ARM_CORE_ID, sizeof (RexCMD));
    #else
        uart_printf (" *   SERV-CEVA = CEVA-%d (CMD size %d)\r\n", DspDrvGetRexCeva(), sizeof (RexCMD));
    #endif

    uart_printf (" *    SERV-INF = 0x%x (size:%d)\r\n", rex_serv_ctx.pInf, sizeof (*rex_serv_ctx.pInf));

    #ifdef REX_QUANTUM_ENABLED
    uart_printf (" *     QUANTUM = enabled\r\n");
    #endif

    if (REX_IS_IDFT_ON_CEVA((REXCFG*)plpCfg))
        uart_printf (" *     FP IDFT = CEVA-%d\r\n", DspDrvGetRexCeva());
    else
        uart_printf (" *     FP IDFT = ARM core-%d\r\n", MapDrvGetMasterCpu());

    RexInitPool (&rex_serv_ctx.pInf->client,    rex_serv_ctx.pInf->client_pool);
    RexInitPool (&rex_serv_ctx.pInf->server[0], rex_serv_ctx.pInf->server_pool_0);
    RexInitPool (&rex_serv_ctx.pInf->server[1], rex_serv_ctx.pInf->server_pool_1);

    rex_serv_ctx.serv_inf[0] = &rex_serv_ctx.pInf->server[0];
    rex_serv_ctx.serv_inf[1] = &rex_serv_ctx.pInf->server[1];
    rex_serv_ctx.client_inf  = &rex_serv_ctx.pInf->client;

    rc = MxSetIntHandler(REX_SERV_CLIENT_IRQ_ID, INT_TYPE_IRQ, PRI_REX_SERV, RexServIrqHandler, &rex_serv_ctx);

    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    rc = MxEnableInt(REX_SERV_CLIENT_IRQ_ID);

    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    #ifdef REX_SERV_ON_ARM
        rc = RexServInitOnArm (&rex_serv_ctx);
        DspDrvRegRexCb(NULL, NULL);
    #else
        // to load the CEVA server part
        DspDrvRegRexCb(RexServCevaHandler, &rex_serv_ctx);
    #endif

    // To initialize the Server part with REX configuration parameters
    rc = RexServSendInit (&rex_serv_ctx, (REXCFG*)plpCfg);
    _ASSERT_RC(rc);

    return rc;
}

/** @brief This function turns off the REX-server part and called automatically from the REX

    @return [MXRC] an error code */

MXRC RexServShutdown(void)
{
    MXRC rc = MX_OK;
    IRQTYPE r;

    if (FAILED(rc = MxDisableInt(REX_SERV_CLIENT_IRQ_ID)))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    DspDrvRegRexCb(NULL,NULL);

    #ifdef REX_SERV_ON_ARM
        rc = RexServDeInitOnArm (&rex_serv_ctx);
    #else
        rc = RexServStopCmd(&rex_serv_ctx);
    #endif

    r = ARM_INT_disable();

#if REXS_TRACE_SIZE > 0
    if (rex_serv_ctx.TracePtr != NULL)
    {
        MxHeapFree (DspHeap, rex_serv_ctx.TracePtr);
        rex_serv_ctx.TracePtr = NULL;
    }
#endif

    if ((REXS_SUB_TASK_NUM != 0) && (rex_serv_ctx.SubTaskInfo))
    {
        MxHeapFree(REX_SERV_INF_HEAP, rex_serv_ctx.SubTaskInfo);
        rex_serv_ctx.SubTaskInfo = NULL;
    }

    if (rex_serv_ctx.pInf != NULL)
    {
        MxHeapFree (REX_SERV_INF_HEAP, rex_serv_ctx.pInf);
        rex_serv_ctx.pInf = NULL;
    }

    ARM_INT_restore(r);

    return rc;
}

/** @brief This function sends command to server part of REX to execute the
               task list defined by function parameter

    @param pList [in] - the pointer to the task list

    @return [MXRC] an error code of operation*/

MXRC RexServExecListCmd (TASKLIST * pList)
{
    MXRC rc = MX_OK;
    RexCMD * pCmd;
    IRQTYPE irq;
    DEF_REX_SERV_CTX();

    _ASSERT_PTR(pList);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_ERROR;
    }

    pCmd->CmdID = REX_CMD_EXEC_LIST;
    pCmd->Param.ExecList.ListPtr = pList;
    pCmd->Param.ExecList.StartTicks = MxGetTicks ();

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();
    RexServUnlock (pCtx, &irq);

    return rc;
}

/** @brief This function sends command to server part of REX to execute the
               task list defined by function parameter

    @param pListX [in] - the pointer to the task list (or NULL)

    @return [MXRC] an error code of operation*/

MXRC RexServExecList4Cmd (TASKLIST * pList0, TASKLIST * pList1, TASKLIST * pList2, TASKLIST * pList3)
{
    MXRC rc = MX_OK;
    RexCMD * pCmd;
    IRQTYPE irq;
    DEF_REX_SERV_CTX();

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_ERROR;
    }

    pCmd->CmdID = REX_CMD_EXEC_LIST_SET;
    
    pCmd->Param.ExecListSet.ListPtr[0] = pList0;
    pCmd->Param.ExecListSet.ListPtr[1] = pList1;
    pCmd->Param.ExecListSet.ListPtr[2] = pList2;
    pCmd->Param.ExecListSet.ListPtr[3] = pList3;

    pCmd->Param.ExecListSet.StartTicks = MxGetTicks ();

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();
    RexServUnlock (pCtx, &irq);

    return rc;
}

/** @brief This function sends confirmation to server part that TCB task was
           completed on client side (client is only one side where this TCB can be done)

    @param pTCB [in] - the pointer to the completed TCB

    @return [MXRC] an error code of operation*/

MXRC RexServTcbIsDoneCmd (TCB * pTCB)
{
    MXRC rc = MX_OK;
    RexCMD * pCmd;
    IRQTYPE irq;
    DEF_REX_SERV_CTX();

    _ASSERT_PTR(pTCB);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_ERROR;
    }

    pCmd->CmdID = REX_CMD_TCB_IS_DONE;
    pCmd->Param.TcbIsDone.TcbPtr = pTCB;

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();
    RexServUnlock (pCtx, &irq);

    return rc;
}

/** @brief This function sets/resets TCB block control parameters like
                - BYPASS
                - LOCKED

    @param nSet [in] - 1 - to set, 0 - to reset
    @param pTCB [in] - the pointer to the completed TCB
    @param mask [in] - the bit mask, see: REX_BYPASS_TCB | REX_LOCKED_TCB

    @return [MXRC] an error code of operation*/

MXRC RexServTcbCtrlCmd (UINT32 nSet, TCB * pTCB, UINT32 mask)
{
    MXRC rc = MX_OK;
    RexCMD * pCmd;
    IRQTYPE irq;
    DEF_REX_SERV_CTX();

    _ASSERT_PTR(pTCB);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_ERROR;
    }

    pCmd->CmdID = nSet ? REX_CMD_SET_TCB_CONTROL : REX_CMD_RESET_TCB_CONTROL;
    pCmd->Param.TcbCtrl.TcbPtr = pTCB;
    pCmd->Param.TcbCtrl.TcbCtrl= mask;

    if (mask & REX_BLOCKING_CALL)
    {
        pCmd->CmdID |= REX_CMD_OPT_BLOCKING;
        pCmd->Status = 0;
    }

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();

    if (mask & REX_BLOCKING_CALL)
    {
        while (pCmd->Status == 0)
        {
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
        }
    }

    RexServUnlock (pCtx, &irq);

    return rc;
}

/** @brief This function sets/resets the list of TCB blocks control parameters like
                - BYPASS
                - LOCKED

    @param nSet     [in] - 1 - to set, 0 - to reset
    @param ppTCB    [in] - the pointer to the completed TCBs
    @param nTcbNum  [in] - the number of TCBs in the array
    @param mask     [in] - the bit mask, see: REX_BYPASS_TCB | REX_LOCKED_TCB

    @return [MXRC] an error code of operation*/

MXRC RexServTcbCtrlExCmd (UINT32 nSet, TCB ** ppTCB, UINT32 nTcbNum, UINT32 mask)
{
    MXRC rc = MX_OK;
    RexCMD * pCmd;
    IRQTYPE irq;
    DEF_REX_SERV_CTX();

    _ASSERT_PTR(ppTCB);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_ERROR;
    }

    MxCacheClean (ppTCB, sizeof (TCB*) * nTcbNum);

    pCmd->CmdID = nSet ? REX_CMD_SET_TCB_CONTROL_EX : REX_CMD_RESET_TCB_CONTROL_EX;
    pCmd->Param.TcbCtrlEx.TcbList = ppTCB;
    pCmd->Param.TcbCtrlEx.TcbNum  = nTcbNum;
    pCmd->Param.TcbCtrlEx.TcbCtrl = (mask & ~REX_BLOCKING_CALL);

    if (mask & REX_BLOCKING_CALL)
    {
        pCmd->CmdID |= REX_CMD_OPT_BLOCKING;
        pCmd->Status = 0;
    }

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();

    if (mask & REX_BLOCKING_CALL)
    {
        while (pCmd->Status == 0)
        {
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
            _NOP();
        }
    }

    RexServUnlock (pCtx, &irq);

    return rc;
}


/** @brief This function sends command to server part of REX to reinitialize
               task list, it saves time on client part

        @param pList [in] - the pointer to the task list

        @return [MXRC] an error code */

MXRC RexServReinitListCmd(TASKLIST * pList)
{
    MXRC rc = MX_OK;
    RexCMD * pCmd;
    IRQTYPE irq;
    DEF_REX_SERV_CTX();

    _ASSERT_PTR(pList);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_ERROR;
    }

    pCmd->CmdID = REX_CMD_REINIT_LIST;
    pCmd->Param.ReinitList.List    = pList;

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();
    RexServUnlock (pCtx, &irq);

    return rc;
}

MXRC RexServShowTrace(UINT32 u32num)
{
    UINT32 i = 0;
    UINT32 data_num;
    REX_SERV_CTX * pCtx = RexServGetCtx();
    _ASSERT_PTR(pCtx);

    if (pCtx->TracePtr == NULL)
       return MX_OK;

    uart_printf ("-----------[CEVA TRACE %d]----------\r\n", sizeof (RexCMD));

    while (u32num --)
    {
        if (pCtx->TracePtr[i] == 0)
            break;

        data_num = (pCtx->TracePtr[i] >> 28) & 0xF;

        if (data_num)
            uart_printf ("[%03d]: 0x%08x [", i, pCtx->TracePtr[i]);
        else
            uart_printf ("[%03d]: 0x%08x\r\n", i, pCtx->TracePtr[i]);

        while (data_num --)
        {
            i++;

            if (data_num)
                uart_printf("0x%x, ", pCtx->TracePtr[i]);
            else
                uart_printf("0x%x]\r\n", pCtx->TracePtr[i]);
        }

        i++;
    }

    uart_printf ("-----------[**********]----------\r\n");

    return MX_OK;
}

/** @brief This function sends to the server message about new TTI with pointers
       to the MLog storages and other information

    @return [MXRC] an error code */

MXRC RexServNewTTICmd(void)
{
#ifdef MLOG_ENABLED

    IRQTYPE irq;
    RexCMD* pCmd;
    void * pMlog;
    REX_SERV_CTX * pCtx = RexServGetCtx();
    _ASSERT_PTR(pCtx);

    pMlog = MLogDevGetCurBlock(CPU_NUM + 0);

    if (pMlog == NULL)
    {
        uart_printf ("No MLog storage for the master CEVA device\r\n");
        _ASSERT_PTR(pMlog);

        return REX_SERV_RC_MLOG_STOR_NULL;
    }

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_ERROR;
    }

    //uart_printf ("MLOG: %x\r\n", pMlog);

    pCmd->CmdID = REX_CMD_NEW_TTI;
    pCmd->Param.NewTTI.MLogPtr = pMlog;

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();
    RexServUnlock (pCtx, &irq);

#endif

    return MX_OK;
}

/** @brief This function sends message to the REX-S component about uploading
           created Mlog information

    @return [MXRC] an error code */

MXRC RexServUploadMLogCmd(void)
{
#ifdef MLOG_ENABLED
    IRQTYPE irq;
    RexCMD* pCmd;
    REX_SERV_CTX * pCtx = RexServGetCtx();
    _ASSERT_PTR(pCtx);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_ERROR;
    }

    pCmd->CmdID = REX_CMD_UPLOAD_MLOG;

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();
    RexServUnlock (pCtx, &irq);
#endif

    return MX_OK;
}

/** @brief This function sends the request to the server part
           and returns list 'descriptors' (the pointer to some location)
           this location cannot be accessed directly

    @param ListID [in] - the list ID

    @return [TASKLIST*] the pointer to the list descriptor*/

PTASKLIST RexServCreateListCmd(UINT32 ListID)
{
    IRQTYPE irq;
    RexCMD* pCmd;
    TASKLIST * pList;
    REX_SERV_CTX * pCtx = RexServGetCtx();
    _ASSERT_PTR(pCtx);


    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return NULL;
    }

    pCmd->CmdID = REX_CMD_CREATE_LIST | REX_CMD_OPT_BLOCKING;
    pCmd->Param.CreateList.ListID = ListID;

    if (RexServSendAndWaitForSyncCmd(pCtx, pCmd, 0) == 0)
    {
        RexServUnlock (pCtx, &irq);
        return 0;
    }

    pList = (TASKLIST*)pCmd->Param.CreateList.ListPtr;
    RexServUnlock (pCtx, &irq);

    _ASSERT_PTR(pList);

    return pList;
}

/** @brief This function sends the request to the server part
           locally created object will be destroyed

    @param pList [in] - The pointer to the list object
    @param pArr  [in] - the pointer to the array of TCBs that should be free on ARM side
    @param pnNum [in] - in: max number of elements in the array, 
                       out: number of elements placed by the REX-SERCER

    @return [MXRC] the error code of operation */

MXRC RexServDestroyListCmd(PTASKLIST pList, TCB ** pArr, UINT32*pnNum)
{
    IRQTYPE irq;
    RexCMD* pCmd;
    REX_SERV_CTX * pCtx = RexServGetCtx();
    
    _ASSERT_PTR(pCtx);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_ERROR;
    }

    pCmd->CmdID = REX_CMD_DESTROY_LIST|REX_CMD_OPT_BLOCKING;
    pCmd->Param.DestroyList.ListPtr  = pList;
    pCmd->Param.DestroyList.TcbArray = pArr;
    pCmd->Param.DestroyList.ArrNum   = *pnNum;

    if (RexServSendAndWaitForSyncCmd(pCtx, pCmd, 0) == 0)
    {
    RexServUnlock (pCtx, &irq);
        return 0;
    }

    *pnNum = pCmd->Param.DestroyList.ArrNum;

    RexServUnlock (pCtx, &irq);
    return MX_OK;
}

/** @brief This function sets pointer to the callback function that will be
           called on ARM side when task list is done

    @param pList [in] - the pointer to the list object (see: RexServCreateList)
    @param cbDone[in] - the pointer to ARM procedure
    @param lpData[in] - the procedure parameter

    @return [MXRC] an error code */

MXRC RexServRegCBDoneCmd(PTASKLIST pList, REXCBDONE cbDone, LPVOID lpData)
{
    IRQTYPE irq;
    RexCMD* pCmd;
    REX_SERV_CTX * pCtx = RexServGetCtx();
    _ASSERT_PTR(pCtx);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_ERROR;
    }

    pCmd->CmdID = REX_CMD_REG_CB_DONE_LIST;
    pCmd->Param.RegCbDoneList.ListPtr = pList;
    pCmd->Param.RegCbDoneList.ListCbDone = (void*)cbDone;
    pCmd->Param.RegCbDoneList.ListCbDoneParam = lpData;

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();
    RexServUnlock (pCtx, &irq);

    return MX_OK;
}

MXRC RexServCreateTCBDCmd(UINT32 TaskID, UINT32 ResID, REXTCBCBDONE cbDone, LPVOID pPtr, TCB * pHwTcb, UINT32 quantum)
{
    IRQTYPE irq;
    RexCMD* pCmd;
    REX_SERV_CTX * pCtx = RexServGetCtx();
    _ASSERT_PTR(pCtx);

    _ASSERT_PTR(pHwTcb);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_ERROR;
    }

    pCmd->CmdID = REX_CMD_CREATE_TCBD | REX_CMD_OPT_BLOCKING;
    pCmd->Param.CreateTcbD.TaskID    = TaskID;
    pCmd->Param.CreateTcbD.ResID     = ResID;
    pCmd->Param.CreateTcbD.pHwTcb    = pHwTcb;
    pCmd->Param.CreateTcbD.CbDone    = (UINT32)cbDone;
    pCmd->Param.CreateTcbD.CbDonePtr = (UINT32)pPtr;
    pCmd->Param.CreateTcbD.Quantum   = quantum;

    if (RexServSendAndWaitForSyncCmd(pCtx, pCmd, 0) == 0)
    {
        RexServUnlock (pCtx, &irq);
        return REX_SERV_RC_SHD_TIMEOUT;
    }

    RexServUnlock (pCtx, &irq);
    return pCmd->RC;
}

MXRC RexServDestroyTCBDCmd(REXTCBD* pTcbD)
{
    IRQTYPE irq;
    RexCMD* pCmd;
    REX_SERV_CTX * pCtx = RexServGetCtx();
    _ASSERT_PTR(pCtx);

    _ASSERT_PTR(pTcbD);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_ERROR;
    }

    pCmd->CmdID = REX_CMD_DESTROY_TCBD | REX_CMD_OPT_BLOCKING;
    pCmd->Param.DestroyTcbD.pTcbD = pTcbD;

    if (RexServSendAndWaitForSyncCmd(pCtx, pCmd, 0) == 0)
    {
        RexServUnlock (pCtx, &irq);
        return REX_SERV_RC_SHD_TIMEOUT;
    }

    RexServUnlock (pCtx, &irq);
    return MX_OK;
}

MXRC RexServAddTcbDepCmd(TCB * pTcb, TCB * pDepOnThis)
{
    IRQTYPE irq;
    RexCMD* pCmd;
    REX_SERV_CTX * pCtx = RexServGetCtx();
    _ASSERT_PTR(pCtx);

    _ASSERT_PTR(pTcb);
    _ASSERT_PTR(pDepOnThis);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_ERROR;
    }

    pCmd->CmdID = REX_CMD_ADD_DEP | REX_CMD_OPT_BLOCKING;
    pCmd->Param.AddDep.pTcb = pTcb;
    pCmd->Param.AddDep.pDepOnThis = pDepOnThis;

    if (RexServSendAndWaitForSyncCmd(pCtx, pCmd, 0) == 0)
    {
        RexServUnlock (pCtx, &irq);
        return REX_SERV_RC_SHD_TIMEOUT;
    }

    RexServUnlock (pCtx, &irq);
    return MX_OK;
}

MXRC RexServAddToListCmd(TASKLIST * pList, TCB * pTcb, UINT32 nTcbPrior, UINT32 * plist_id)
{
    IRQTYPE irq;
    RexCMD* pCmd;
    REX_SERV_CTX * pCtx = RexServGetCtx();
    _ASSERT_PTR(pCtx);

    _ASSERT_PTR(pList);
    _ASSERT_PTR(pTcb);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_ERROR;
    }

    pCmd->CmdID = REX_CMD_ADD_TO_LIST | REX_CMD_OPT_BLOCKING;
    pCmd->Param.AddToList.pList = pList;
    pCmd->Param.AddToList.pTcb  = pTcb;
    pCmd->Param.AddToList.Prior = nTcbPrior;

    if (RexServSendAndWaitForSyncCmd(pCtx, pCmd, 0) == 0)
    {
        RexServUnlock (pCtx, &irq);
        return REX_SERV_RC_SHD_TIMEOUT;
    }

    if (plist_id != NULL)
        *plist_id = pCmd->Param.AddToList.ListID;

    RexServUnlock (pCtx, &irq);
    return MX_OK;
}

MXRC RexServGetListInfoCmd (TASKLIST * pList, RexCmdGetListInfo* pInfo)
{
    IRQTYPE irq;
    RexCMD* pCmd;
    REX_SERV_CTX * pCtx = RexServGetCtx();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pInfo);
    _ASSERT_PTR(pList);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_CMD_ERROR;
    }

    pCmd->CmdID = REX_CMD_GET_LIST_INFO | REX_CMD_OPT_BLOCKING;
    pCmd->Param.ListInfo.pList = pList;

    if (RexServSendAndWaitForSyncCmd(pCtx, pCmd, 0) == 0)
    {
        RexServUnlock (pCtx, &irq);
        return REX_SERV_RC_SCMD_TIMEOUT;
    }

    *pInfo = pCmd->Param.ListInfo;
     RexServUnlock (pCtx, &irq);

    return MX_OK;
}

UINT32 RexServGetListIDCmd(TASKLIST * pList)
{
    IRQTYPE irq;
    RexCMD* pCmd;
    UINT32 ID;
    REX_SERV_CTX * pCtx = RexServGetCtx();
    _ASSERT_PTR(pCtx);

    _ASSERT_PTR(pList);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_CMD_ERROR;
    }

    pCmd->CmdID = REX_CMD_GET_LIST_INFO | REX_CMD_OPT_BLOCKING;
    pCmd->Param.ListInfo.pList = pList;

    if (RexServSendAndWaitForSyncCmd(pCtx, pCmd, 0) == 0)
    {
        RexServUnlock (pCtx, &irq);
        return REX_SERV_RC_SCMD_TIMEOUT;
    }

    ID = pCmd->Param.ListInfo.ListID;
    RexServUnlock (pCtx, &irq);
    return ID;
}

/** @brief This function is used to associate TCB with memory cell,
           this is specific dependencies that is used to handle HW accelerators
           when CELL value is 1, REX will treat this dependencies like 'done' and
           TCB can be scheduled if no other dependencies

    @NOTE!
            This functionality is applicable for REX-SERVER mode only (CEVA or ARM-emulator)

    @param  pTcb [in] - the pointer to the TCB block accosiated with memory CELL
    @param  nAddr[in] - the CELL address (it cannot be NULL)
    @param  nInit[in] - the initial value for this memory cell

    @return [MXRC] an error code of operation */

MXRC RexServAddCellDepCmd(TCB * pTcb, UINT32 nAddr, UINT32 nInit)
{
    IRQTYPE irq;
    RexCMD* pCmd;
    REX_SERV_CTX * pCtx = RexServGetCtx();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTcb);
    _ASSERT_PTR(nAddr);

    if (nAddr == NULL)
        return REX_SERV_RC_PARAM_ERROR;

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_CMD_ERROR;
    }

    pCmd->CmdID = REX_CMD_ADD_CELL_DEP | REX_CMD_OPT_BLOCKING;
    pCmd->Param.AddCellDep.pTcb         = pTcb;
    pCmd->Param.AddCellDep.CellAddr     = nAddr;
    pCmd->Param.AddCellDep.CellInitVal  = nInit;

    if (RexServSendAndWaitForSyncCmd(pCtx, pCmd, 0) == 0)
    {
        RexServUnlock (pCtx, &irq);
        return REX_SERV_RC_SCMD_TIMEOUT;
    }

    if (pCmd->RC != MX_OK)
    {
        RexServUnlock (pCtx, &irq);
        return REX_SERV_RC_ADD_DEP_ERROR;
    }

    RexServUnlock (pCtx, &irq);
    return MX_OK;
}


/** @brief This function is used to reinitialize cells assigned to TCB blocks

    @NOTE!
            This functionality is applicable for REX-SERVER mode only (CEVA or ARM-emulator)

    @param  pCellList   [in] - the array of cell addresses 
    @param  pNewCellList[in] - the array of new cell addresses (it can be NULL)
    @param  nNum        [in] - the array element number

    @return [MXRC] an error code of operation */

MXRC RexServReinitCellsCmd(UINT32* pCellList, UINT32* pNewCellList, UINT32 nNum)
{
    IRQTYPE irq;
    RexCMD* pCmd;
    REX_SERV_CTX * pCtx = RexServGetCtx();

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCellList);

    if (pCellList == NULL)
        return REX_SERV_RC_PARAM_ERROR;

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_CMD_ERROR;
    }

    if (nNum != 0)
    {
        MxCacheClean(pCellList, nNum*sizeof (UINT32));

        if (pNewCellList != NULL)
            MxCacheClean(pNewCellList, nNum*sizeof (UINT32));
    }
    

    pCmd->CmdID = REX_CMD_REINIT_CELLS;
    pCmd->Param.ReinitCells.CellList = pCellList;
    pCmd->Param.ReinitCells.NewCellList = pNewCellList;
    pCmd->Param.ReinitCells.CellNum  = nNum;

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();
    RexServUnlock (pCtx, &irq);

    return MX_OK;
}


/** @brief This diagnostics API is designed to run some diagnostics code on ARM/CEVA
               REX-SERVER, and used to check some functionality, not designed to be used
               in general application

        @param nCmdID   [in]   - the diagnostics API ID
        @param pInPtr   [in]   - the input buffer for diag API
        @param InSize   [in]   - the size in bytes of input buffer
        @param pOutPtr  [out]  - the output buffer for diag API
        @param OutSize  [in]   - the size in bytes of output buffer

        @return [MXRC] an error code of operation */

MXRC RexServDiagCmd (UINT32 nCmdID, void*pInPtr, UINT32 nInSize, void*pOutPtr, UINT32 nOutSize)
{
    IRQTYPE irq;
    RexCMD* pCmd;
    REX_SERV_CTX * pCtx = RexServGetCtx();

    _ASSERT_PTR(pCtx);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_CMD_ERROR;
    }

    pCmd->CmdID = REX_CMD_DIAGNOSTICS | REX_CMD_OPT_BLOCKING;
    pCmd->Param.Diag.CmdID      = nCmdID;
    pCmd->Param.Diag.InputPtr   = pInPtr;
    pCmd->Param.Diag.OutputPtr  = pOutPtr;
    pCmd->Param.Diag.InputSize  = nInSize;
    pCmd->Param.Diag.OutputSize = nOutSize;

    if (RexServSendAndWaitForSyncCmd(pCtx, pCmd, 0) == 0)
    {
        RexServUnlock (pCtx, &irq);
        return REX_SERV_RC_SCMD_TIMEOUT;
    }

    if (pCmd->RC != MX_OK)
    {
        RexServUnlock (pCtx, &irq);
        return pCmd->RC;
    }

    RexServUnlock (pCtx, &irq);
    return MX_OK;
}

void RexServRunTcb(UINT32 nDevID, TCB * pTcb, UINT32 mlog_start_task)
{
    // to disable INT on CEVA by setting bit:16 in TCB::taskID

    TCB * nextTCB = pTcb;

    if (mlog_start_task)
    {
        while (nextTCB)
        {
            MLogDevEXEStart(nextTCB,nDevID,0);
            nextTCB = nextTCB->NextSupTcb;//nextTCB = nextTCB->NextTcb;
        }
    }

    pTcb->Status = TCB_CFG_STAT_DIS_INT;

    rexs_printf (REXS_RUN_ON_CEVA, "REX-SERV: CEVA-%d Run TCB(0x%x) on CEVA, task-id=%d, status=%d\r\n", nDevID, pTcb, pTcb->TaskID, pTcb->Status);
    DspDrvRunEx (nDevID, pTcb, 0);
}

void RexServNewTTIProc (RexCmdNewTTI * pCmd)
{
    _ASSERT_PTR(pCmd);
	MLogDevSetFrame(pCmd->MLogPtr);
}

/** @brief This function delivers current MLOG frame pointer to the
           CEVA code, and CEVA will be able to upload already
           prepared MLOG (previous TTI) to DDR memory.

    @param pMLogFrame [in] - the pointer to the MLog frame

    @return [MXRC] an error code */

MXRC RexServStartTTI(void * pMLogFrame)
{
    return MX_OK;
}

char * RexServGetErrorDescr(UINT32 code)
{
    UINT32 i = 0;

    while (RexServErrorTable [i].Code != 0)
    {
        if (RexServErrorTable [i].Code == code)
            return RexServErrorTable [i].Descr;

        i++;
    }

    return RexServErrorTable [i].Descr;
}

/** @brief This function sends to REX-SERVER information about restored
               CEVAs that can be used again to process TCBs

        @param nGoodMask[in] - the mask of good processors restored on ARM

        @return [MXRC] an error code */

MXRC RexServNotifyRestCevaCmd(UINT32 nGoodMask)
{
    IRQTYPE irq;
    RexCMD* pCmd;
    REX_SERV_CTX * pCtx = RexServGetCtx();

    _ASSERT_PTR(pCtx);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);
        return REX_SERV_RC_ALLOC_CMD_ERROR;
    }

    pCmd->CmdID = REX_CMD_RESTORED_CEVA;
    pCmd->Param.BadCevaRest.RestMask = nGoodMask;

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();
    RexServUnlock (pCtx, &irq);

    return MX_OK;
}

void RexServMmuCfg(UINT32 nCpuID, UINT32 nParam)
{
    #ifdef REX_SERV_ON_ARM
        if (nCpuID == REX_SERV_ARM_CORE_ID)
        {
            MmuMgrCfgMemAP(CRAM_BASEADDR, CRAM_SIZE, AP_MGR_READ_WRITE, AP_MGR_READ_WRITE);
        }

        DspMmuCfg(0, 0);

    #endif
}

void RexServProcCevaDoneCmd(RexCMD* pCmd)
{
#if REX_MAX_PRIVATE_TCB_NUM > 0
    REX_SERV_CTX * pCtx = RexServGetCtx();
    UINT32 nCpu;
    UINT32 nIdx;

    _ASSERT_PTR(pCtx);

    nCpu = REX_CC_HANDLE_GET_CPU(pCmd->Param.CevaProcDone.Handle);
    nIdx = REX_CC_HANDLE_GET_IDX(pCmd->Param.CevaProcDone.Handle);

    _ASSERT(nCpu < 4);
    _ASSERT(nIdx < REXS_CEVA_PROC_CALL_NUM);

    nCpu = MIN(nCpu, 3);
    nIdx = MIN(nIdx, REXS_CEVA_PROC_CALL_NUM-1);

    pCtx->CevaCallRetVal[nCpu][nIdx] = pCmd->Param.CevaProcDone.ProcRet;
    pCtx->CevaCallStatus[nCpu][nIdx] = REX_CC_HANDLE_DONE_VAL;

    if (pCtx->CevaCallCbDone[nCpu][nIdx] != NULL)
    {
        // to call the ARM application callback for the CAVA call
        //-------------------------------------------------------
        pCtx->CevaCallCbDone[nCpu][nIdx](pCtx->CevaCallCbDonePtr[nCpu][nIdx], 
                                         pCtx->CevaCallRetVal[nCpu][nIdx]);
    }
    
#endif
}

UINT32 RexServGetCevaCallHandle (REX_SERV_CTX * pCtx,CevaCallCbDoneType CbDone, void * Ptr)
{
#if REX_MAX_PRIVATE_TCB_NUM > 0
    UINT32 nCpuID = MxGetCpuID ();
    UINT32 nIdx;

    _ASSERT_PTR(pCtx);

    nIdx = pCtx->CevaCallHandleIdx[nCpuID]++;

    if (pCtx->CevaCallHandleIdx[nCpuID] >= REXS_CEVA_PROC_CALL_NUM)
    {
        pCtx->CevaCallHandleIdx[nCpuID] = 0;
    }

    pCtx->CevaCallStatus[nCpuID][nIdx]      = REX_CC_HANDLE_INIT_VAL;
    pCtx->CevaCallCbDone[nCpuID][nIdx]      = CbDone;
    pCtx->CevaCallCbDonePtr[nCpuID][nIdx]   = Ptr;
    
    return REX_MAKE_CC_HANDLE(nCpuID, nIdx);
#else
    return 0;
#endif
}

/** @brief This function sends request to CEVA code to call some CEVA code function,
       defined by nProcAddr.

    @param CbDone    [in]       - the pointer to the ARM CB-DONE function
    @param Ptr       [in]       - the pointer to the cbDone argument
    @param nProcAddr [in]       - the address of CEVA function
    @param quantum   [in]       - the quantum of time CEVA function has to be completed (in microseconds) 0 - quantum is turned off
    @param nParamNum [in]       - the number of parameters
    @param pParamArr [in]       - the CEVA function parameters, optional and depends on CEVA function

    @return [UINT32] the handle of CEVA call or NULL */

UINT32 RexServCallCevaProc(CevaCallCbDoneType CbDone, void * Ptr, UINT32 nProcAddr, UINT32 quantum, UINT32 nParamNum, UINT32 * pParamArr)
{
    IRQTYPE irq;
    RexCMD* pCmd;
    REX_SERV_CTX * pCtx = RexServGetCtx();
    UINT32 handle;

    _ASSERT_PTR(pCtx);

    RexServLock (pCtx, &irq);
    pCmd = RexPoolAllocServCmd(pCtx);

    if (pCmd == NULL)
    {
        RexServUnlock (pCtx, &irq);
        _ASSERT_PTR(pCmd);

        return NULL;
    }

    handle = RexServGetCevaCallHandle(pCtx, CbDone, Ptr);

    pCmd->CmdID = REX_CMD_CALL_CEVA_PROC;
    
    pCmd->Param.CallCevaProc.CevaProc = nProcAddr;
    pCmd->Param.CallCevaProc.ParamNum = nParamNum;
    pCmd->Param.CallCevaProc.Handle   = handle;
    pCmd->Param.CallCevaProc.CevaProcQ= US_TO_TICKS(quantum);

    if (nParamNum != 0)
    {
        memcpy (pCmd->Param.CallCevaProc.Param, pParamArr, nParamNum<<2);
    }

    RexPoolSendToServ(pCtx, pCmd);
    RexServNotifyServ();
    RexServUnlock (pCtx, &irq);

    return handle;
}

/** @brief This function waits for CEVA call completion

    @param handle   [in]  - the handle of CEVA call returned by RexServCallCevaProc function
    @param nTimeInUs[in]  - the time in microseconds (to wait completion), 0 - to check and return
    @param pRet     [in]  - the pointer to the variable to store CEVA procedure return value

    @return [MXRC] an error code of operation*/

MXRC RexServWaitCallCevaDone(UINT32 handle, UINT32 nTimeInUs, UINT32 * pRet)
{
#if REX_MAX_PRIVATE_TCB_NUM > 0
    REX_SERV_CTX * pCtx = RexServGetCtx();
    UINT32 nTime = 0;
    UINT32 nCpu, nIdx, ticks;

    _ASSERT_PTR(pCtx);

    if (handle == NULL)
        return REX_SERV_RC_HANDLE_ERROR;

    nCpu = REX_CC_HANDLE_GET_CPU(handle);
    nIdx = REX_CC_HANDLE_GET_IDX(handle);

    // to save time (MIPS) of ARM core
    // XP timers registers are VERY-VERY slow
    // ---------------------------------------
    if(nTimeInUs != 0)
    {
        nTime = MxGetTicks();
    }

    ticks = US_TO_TICKS(nTimeInUs);

    do
    {
        if (pCtx->CevaCallStatus[nCpu][nIdx] == REX_CC_HANDLE_DONE_VAL)
        {
            if (pRet != NULL)
            {
                *pRet = pCtx->CevaCallRetVal[nCpu][nIdx];
            }

            return MX_OK;
        }

    }
    while ((MxGetTicks() - nTime) < ticks);

    return REX_SERV_RC_NOT_DONE;
#else
    return REX_SERV_RC_NOT_SUPPORTED;
#endif
}

#else //#ifdef _4GMX_CONFIG_H_

#include "global.h"
#include "mlogceva.h"

void RexServNotifyClient()
{
	GenArmInt();
}

void RexServRunTcb(UINT32 dev_id, TCB * pTcb, UINT32 mlog_start_task)
{
    REXS_TRACE_DATA(REXS_TRACE_NOTIFY_SLAVE_CEVA, dev_id);

    if (mlog_start_task)
	    MLogDevEXEStart(pTcb, dev_id, 0);

    pTcb->Status = TCB_CFG_STAT_DIS_INT;

    REG32(0xFC000000 + (dev_id + 2) * 4) = (UINT32)pTcb;
    REG32(0xFD040000) = (1 << dev_id);

    REXS_TRACE_DATA(REXS_TRACE_NOTIFY_IS_DONE, dev_id);
}

/** @brief This function sets pointer to the communication interface
			the interface is specified by the host

	@param pInf [in] - the pointer to the interface */

void RexServSetInterface (void * pInf)
{
	REX_SERV_CTX * pCtx;
	pCtx = RexServGetCtx();

	if (pCtx != NULL)
	{
		pCtx->pInf = (REX_SERV_COMM_INF*)pInf;

        pCtx->serv_inf[0] = &pCtx->pInf->server[0];
        pCtx->serv_inf[1] = &pCtx->pInf->server[1];
        pCtx->client_inf  = &pCtx->pInf->client;

        REXS_TRACE_DATA(REXS_TRACE_SET_INF, (UINT32)pCtx->pInf);
    }
}

void RexServNewTTIProc (RexCmdNewTTI * pCmd)
{
	_ASSERT_PTR(pCmd);
	MLogDevSetFrame(pCmd->MLogPtr);
}

#endif // #ifdef _4GMX_CONFIG_H_

/******************************************************************************
*                      PLATFORM INDEPENDENT INTERFACE                         *
******************************************************************************/

/** @brief This function is designed to find and return IO buffer specified by the index and by the type

    @param pTCB     [in    ] - the pointer to the TCB task
    @param pLen     [out   ] - the pointer to the buffer length
    @param IOType   [in    ] - the type of data
    @param ioNum    [in/out] - the buffer index

    @return [LPVOID] the pointer to the buffer

    *  \ingroup group_lte_rexscheduler

    */

LPVOID RexServGetIOBufByType(PTCB pTCB, UINT32* pLen, UINT32 IOType, UINT32 *ioNum)
{
    TIOBufDesc * pDesc;
    UINT32 nNum, i;

    _ASSERT_PTR(pLen);
    _ASSERT_PTR(ioNum);

    if (pTCB == NULL)
    {
        _ASSERT_PTR(pTCB);
        return NULL;
    }

    nNum = TCB_CONTROL_LEN(pTCB) / sizeof (TIOBufDesc);
    pDesc = (TIOBufDesc *)((UINT32)TCB_CONTROL(pTCB) + ((*ioNum) * sizeof(TIOBufDesc)));

    i = *ioNum;

    while (i < nNum)
    {
        if ((pDesc->IOBufControl & IOType) != 0)
        {
            *ioNum = i + 1;
            *pLen  = pDesc->IOBufControl & BUF_SIZE_MASK;

            return pDesc->IOBufPtr;
        }

        i++;
        pDesc++;
    }

    *pLen = 0;
    *ioNum= 0;

    return NULL;
}

BOOL RexServIsTcbInList (PTASKLIST pList, REXTCBD * pTcbD)
{
    volatile REXTCBD * pBlock;
    _ASSERT_PTR(pTcbD);

    if (pList == NULL)
        return FALSE;

    pBlock = pList->TcbHead;

    while (pBlock)
    {
        if (pBlock == pTcbD)
            return TRUE;

        pBlock = pBlock->RexTcbNext;
    }

    return FALSE;
}


/** @brief This function checks the possibility to add new dependency

    @param pTcb         [in] - the TCB task that depends on pDepOnThis
    @param pDepOnThis   [in] - the dependency

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexServIsDepConflict (REXTCBD * pTcbD, REXTCBD * pDepOnThisD)
{
    UINT32 i;

    _ASSERT_PTR(pTcbD);
    _ASSERT_PTR(pDepOnThisD);

    if (pTcbD == pDepOnThisD)
        return REX_SERV_RC_DEP_ERROR;

    // to check if dependency is already added

    for (i = 0; i < pDepOnThisD->RefNum; i++)
    {
        if (pDepOnThisD->RefList [i] == pTcbD)
            return REX_SERV_RC_SKIP;
    }

    // to check the cyclic dependency

    for (i = 0; i < pTcbD->RefNum; i++)
    {
        if (pTcbD->RefList [i] == pDepOnThisD)
            return REX_SERV_RC_DEP_ERROR;
    }

    // if this TCB block is not in task list,
    // this dependencies is meaningless

    if (pDepOnThisD->RexList != NULL)
    {
        if (!RexServIsTcbInList ((PTASKLIST)pDepOnThisD->RexList, pDepOnThisD))
        {
            return REX_SERV_RC_SKIP;
        }
    }
    else
    {
        return REX_SERV_RC_SKIP;
    }

    // the dependency can de added

    return MX_OK;
}

BOOL RexServIsBuffersIntersects (UINT32 p1, UINT32 p2, UINT32 len1, UINT32 len2)
{
    if ((void*)p1 != NULL && (void*)p1 != NULL)
    {
        if (p2 >= p1 && p2 <= (p1 + len1) )
            return TRUE;

        if (p1 >= p2 && p1 <= (p2 + len2) )
            return TRUE;
    }

    return FALSE;
}

BOOL RexServIsIntersected(TCB * pTcb, TCB * pElm)
{
    UINT32 inp_scan, out_scan;
    UINT32 p1, p2, len1 = 0, len2 = 0;

    _ASSERT_PTR(pTcb);
    _ASSERT_PTR(pElm);

    // to check the <INPUT> and <OUTPUT> buffers

    if (RexServIsBuffersIntersects ((UINT32)pTcb->InputDataPtr, (UINT32)pElm->OutputDataPtr, pTcb->InputDataLen, pElm->OutputDataLen))
        return TRUE;

    // to scan the IO buffers

    inp_scan = 0;

    while ((void*)(p1 = (UINT32)RexServGetIOBufByType(pTcb, &len1, IN_BUF, &inp_scan)) != NULL)
    {
        if (RexServIsBuffersIntersects (p1, (UINT32)pElm->OutputDataPtr, len1, pElm->OutputDataLen))
            return TRUE;

        out_scan = 0;

        while ((void*)(p2 = (UINT32)RexServGetIOBufByType(pElm, &len2, OUT_BUF, &out_scan)) != NULL)
        {
            if (RexServIsBuffersIntersects (p1, p2, len1, len2))
                return TRUE;
        }
    }

    return FALSE;
}

/** @brief The function adds TCB dependency, the dependency means that original TCB block depends on
            the specified one and cannot be executed prior dependent.

    @param pTcb         [in] - the pointer to the TCB that will be dependent on specified TCB task
    @param pDepOnThis   [in] - the dependence

    @return [MXRC] an error code

    \ingroup group_lte_rexscheduler

    */

MXRC RexServAddDep (TCB* pTcb, TCB * pDepOnThis)
{
    MXRC rc = MX_OK;
    REXTCBD * pTcbD, *pDepOnThisD;

    _ASSERT_PTR(pTcb);
    _ASSERT_PTR(pDepOnThis);

    pTcbD = (REXTCBD *)pTcb->RexDescr;
    pDepOnThisD = (REXTCBD *)pDepOnThis->RexDescr;

    _ASSERT_PTR(pTcbD);
    _ASSERT_PTR(pDepOnThisD);

    if ((pDepOnThisD->RefNum) >= REX_DEP_SIZE)
    {
        rc = REX_SERV_RC_DEP_OVERLOADED;
        _ASSERT_RC(rc);
        return rc;
    }

    // to check the dependency conflicts
    rc = RexServIsDepConflict (pTcbD, pDepOnThisD);

    if (rc == REX_SERV_RC_SKIP)
    {
        return MX_OK;
    }

    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        return rc;
    }

    // to add a reference
    pDepOnThisD->RefList [pDepOnThisD->RefNum++] = pTcbD;

    // to increase the number of dependencies
    pTcbD->DepNum ++;

    // the sched list should contain only elements with 0 dependencies

    if (pTcbD->Ctrl & REX_CTRL_TCB_SCHEDULED)
    {
        // the element is already in the list
        // we need to reorganize the list according to the new priorities

        RexServCutFromSchedList (pTcbD->RexList, pTcbD, pTcbD->Ctrl);
    }

    return rc;
}

/** @brief The function discovers the additional dependencies by scanning the list,
           the dependencies are detected like input/output buffers intersection

    @param pList [in] - the list of tasks
    @param pTcb  [in] - the new one added task

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexServDiscoverDep (PTASKLIST pList, REXTCBD * pTcbD)
{
    REXTCBD * pElmD;
    MXRC rc = MX_OK;
    TCB* pElm, *pTcb;

    _ASSERT_PTR(pList);

    pElmD = (REXTCBD*)pList->TcbHead;

    pTcb = pTcbD->pHwTCB;
    _ASSERT_PTR(pTcb);

    while (pElmD)
    {
        pElm = pElmD->pHwTCB;
        _ASSERT_PTR(pElm);

        if (pTcb->ContextPtr == pElm->ContextPtr)
        {
            rc = RexServAddDep (pTcb, pElm);
        }
        else if (RexServIsIntersected(pTcb, pElm))
        {
            rc = RexServAddDep (pTcb, pElm);
        }
        else if (RexServIsIntersected(pElm, pTcb))
        {
            rc = RexServAddDep (pElm, pTcb);
        }

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }

        pElmD = pElmD->RexTcbNext;
    }

    return MX_OK;
}



/** @brief The function adds TCB task to the specified task list according to the dependence weight priority

    @param pTaskList    [in] - the task list
    @param pTcb         [in] - the pointer to the TCB task that should be added to the list
    @param nTcbPrior    [in] - the TCB priority, 0 - highest priority, it's valid only if REX_TCB_PRIORITY_ENABLED is defined

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexServAddTCBToList(PTASKLIST pTaskList, PTCB pTCB, UINT32 nTcbPrior)
{
    MXRC rc = MX_OK;
    REXTCBD * pTcbD;

    _ASSERT_PTR (pTCB);
    _ASSERT_PTR (pTaskList);

    pTcbD = (REXTCBD *)pTCB->RexDescr;
    _ASSERT_PTR(pTcbD);

    if (pTaskList->Ctrl & (REX_CTRL_LIST_IS_SCHEDULED | REX_CTRL_LIST_UNDER_PROC | REX_CTRL_LIST_STARTED))
    {
        rc = REX_SERV_RC_LIST_IS_SCHED;
        _ASSERT_RC (rc);
        return rc;
    }

    if ((pTcbD->RexList != NULL) || (pTcbD->Ctrl & REX_CTRL_TCB_IN_LIST))
    {
        rc = REX_SERV_RC_DOUBLE_ADD_TO_LST;
        _ASSERT_RC(rc);
        return rc;
    }

    if (RexServIsTcbInList (pTaskList, pTcbD))
    {
        rc = REX_SERV_RC_DOUBLE_ADD_TO_LST;
        _ASSERT_RC(rc);
        return rc;
    }

    if (/*pCtx->DiscoverDep*/ 0)
    {
        rc = RexServDiscoverDep (pTaskList, pTcbD);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }
    }

    // ListTcb points to the element prior what new TCB should be added
    // to add the new one block to the list

    if (pTaskList->TcbHead == NULL)
    {
        pTaskList->TcbHead = pTcbD;
        pTaskList->TcbTail = pTcbD;

        pTcbD->RexTcbNext = NULL;
        pTcbD->RexTcbPrev = NULL;
    }
    else
    {
        pTaskList->TcbTail->RexTcbNext = pTcbD;
        pTcbD->RexTcbPrev = (REXTCBD*)pTaskList->TcbTail;

        pTaskList->TcbTail = pTcbD;
        pTcbD->RexTcbNext = NULL;
    }

    pTaskList->TcbNum ++;

    pTcbD->Priority = nTcbPrior;
    pTcbD->RexList  = pTaskList;
    pTcbD->Ctrl    |= REX_CTRL_TCB_IN_LIST;

    // If the number of dependency is 0 and this element is allowed to be scheduled
    // this element can be added to the list of already scheduled elements

    if ((pTcbD->DepNum == 0) && !(pTcbD->Ctrl & REX_CTRL_TCB_LOCKED))
    {
        RexServAddToScheduled (pTaskList, pTcbD);
    }

    return rc;
}



/** @brief The function deallocates memory reserved for the TCB block

    @param pTcbD [in] - the removed TCB-Descriptor block

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexServDestroyTCB (REXTCBD * pTcbD)
{
    REX_SERV_CTX * pCtx = RexServGetCtx ();
    MXRC rc = MX_OK;
    UINT32 i;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTcbD);

    if (pTcbD->RexList != NULL)
    {
        RexServCutFromSchedList ((PTASKLIST)pTcbD->RexList, pTcbD, 1);
        RexServCutFromList (pTcbD);
    }

    for (i = 0; i < pTcbD->RefNum; i++)
    {
        _ASSERT_PTR (pTcbD->RefList [i]);
        pTcbD->RefList [i]->DepNum --;
    }

        RexServFreeSysPart(&pCtx->TcbDPart, pTcbD);
    return rc;
}


/** @brief This function destroys a task list object and a list of TCBs of this task list

    @param pList [in] - the pointer to the task list

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

MXRC RexServDestroyTaskList (PTASKLIST pList, TCB ** pArr, VUINT32 *pnNum)
{
    MXRC rc = MX_OK;
    REX_SERV_CTX * pCtx = RexServGetCtx ();
    UINT32 num = 0, nmax = 0;

    _ASSERT_PTR (pList);
    _ASSERT_PTR (pCtx);

    if (pList->Ctrl & REX_CTRL_LIST_IS_SCHEDULED)
    {
        rc = REX_SERV_RC_LIST_IS_SCHEDULED;
        _ASSERT_RC(rc);
        return rc;
    }

    if (pnNum != NULL)
    {
        nmax = *pnNum;
    }

    while (pList->TcbHead)
    {
        if ((pArr != NULL) && (num < nmax))
        {
            pArr [num] = pList->TcbHead->pHwTCB;
            num++;
        }
    
        rc = RexServDestroyTCB((REXTCBD*)pList->TcbHead);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return rc;
        }
    }
    
    RexServFreeSysPart(&pCtx->ListPart, pList);

    if (pnNum != NULL)
    {
        *pnNum = num;
    }
    
    return rc;
}

/** @brief To notify client side about error happened in a process of system work

    @param pCtx [in]    - the context
    @param code [in]    - the error code
*/

void RexServSendError (REX_SERV_CTX * pCtx, unsigned int code, unsigned int data)
{
    RexCMD * pCmd;

    // till block is allocated
    while ((pCmd = RexPoolAlloc (&pCtx->pInf->client)) == 0);

    pCmd->CmdID = REX_CMD_ERROR_CODE;
    pCmd->Param.TcbError.ErrorCode = code;
    pCmd->Param.TcbError.ErrorData = data;

    RexPoolSend(&pCtx->pInf->client, pCmd);
    RexServNotifyClient();
}

void RexServSendErrorEx(unsigned int code, unsigned int data)
{
    RexServSendError(RexServGetCtx(), code, data);
}

void RexServCutFromSchedList(TASKLIST * pList, REXTCBD * pTcbD, UINT32 nTcbDCtrl)
{
    _ASSERT_PTR(pTcbD);
    _ASSERT_PTR(pList);

    if (nTcbDCtrl & REX_CTRL_TCB_SCHEDULED)
    {
        if (pTcbD->RexSchedPrev != NULL)
        {
            pTcbD->RexSchedPrev->RexSchedNext = pTcbD->RexSchedNext;
        }

        if (pTcbD->RexSchedNext != NULL)
        {
            pTcbD->RexSchedNext->RexSchedPrev = pTcbD->RexSchedPrev;
        }

        if (pList->SchedHead == pTcbD)
        {
            pList->SchedHead = pTcbD->RexSchedNext;
        }

        if (pList->SchedTail == pTcbD)
        {
            pList->SchedTail = pTcbD->RexSchedPrev;
        }

        pTcbD->RexSchedNext = NULL;
        pTcbD->RexSchedPrev = NULL;

        nTcbDCtrl &= ~REX_CTRL_TCB_SCHEDULED;
    }

    pTcbD->Ctrl = nTcbDCtrl;
}

unsigned int RexServGetFreeCeva (unsigned int mask)
{
#if 0
    unsigned int val = RES_SERV_HW_CEVA_NUM;

    while (!(mask & (1 << val)))
    {
        val --;
    }

    return val;
#else
    unsigned int i = 0;

    while (!(mask & (1 << CEVAPOOLTABLE[i])))
    {
        i++;
    }
	return (CEVAPOOLTABLE[i]);

#endif
}

unsigned int RexServGetFreePrvTcb (unsigned int mask)
{
    unsigned int val = REX_MAX_PRIVATE_TCB_NUM;

    while (!(mask & (1 << val)))
    {
        val --;
    }

    return val;
}

void RexServPushTcb (REX_SERV_CTX * pCtx, REXTCBD * pTcbD)
{
    if (pTcbD->TaskID == TASKID_CALL_CEVA_PROC && (pCtx->tcbd_high_proc_queue_num + 1) < REXS_CEVA_HIGH_PROC_QUEUE)
    {
        pCtx->tcbd_high_proc_queue_num++;
        pCtx->tcbd_high_proc_queue [pCtx->tcbd_high_proc_put++] = pTcbD;

        if (pCtx->tcbd_high_proc_put >= REXS_CEVA_HIGH_PROC_QUEUE)
            pCtx->tcbd_high_proc_put = 0;

        _ASSERT(pCtx->tcbd_high_proc_put != pCtx->tcbd_high_proc_get);
    }
    else
    {
        pCtx->tcbd_proc_queue [pCtx->tcbd_proc_put++] = pTcbD;

        if (pCtx->tcbd_proc_put >= REX_SERVER_PROC_QUEUE)
            pCtx->tcbd_proc_put = 0;

        _ASSERT(pCtx->tcbd_proc_put != pCtx->tcbd_proc_get);
    }
}

REXTCBD * RexServPopTcb (REX_SERV_CTX * pCtx)
{
    REXTCBD * pTcbD;

    if (pCtx->tcbd_high_proc_queue_num != 0)
    {
        pCtx->tcbd_high_proc_queue_num--;
        pTcbD = pCtx->tcbd_high_proc_queue [pCtx->tcbd_high_proc_get++];

        if (pCtx->tcbd_high_proc_get >= REXS_CEVA_HIGH_PROC_QUEUE)
            pCtx->tcbd_high_proc_get = 0;

        return pTcbD;    
    }

    if (pCtx->tcbd_proc_put == pCtx->tcbd_proc_get)
        return NULL;

    pTcbD = pCtx->tcbd_proc_queue [pCtx->tcbd_proc_get++];

    if (pCtx->tcbd_proc_get >= REX_SERVER_PROC_QUEUE)
        pCtx->tcbd_proc_get = 0;

    return pTcbD;
}

void CevaRun (REX_SERV_CTX * pCtx, REXTCBD * pTcbD, TCB * pHwTcb)
{
    unsigned int dev_id;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTcbD);
    _ASSERT((pTcbD->Ctrl & REX_CTRL_TCB_DONE) == 0);

    if (pCtx->hw_mask == 0)
    {
        // Just to put TCB block to the queue
        // later this TCB will be de-queued and routed to CEVA

        REXS_TRACE_DATA(REXS_TRACE_PUSHED_TO_WAIT_CEVA, pTcbD->TaskID);

        RexServPushTcb(pCtx, pTcbD);
    }
    else
    {
        dev_id = RexServGetFreeCeva(pCtx->hw_mask);

        if (pCtx->SubTaskNumPerCeva != 0)
        {
            pHwTcb->SubTaskNum = pCtx->SubTaskNumPerCeva;
            pHwTcb->SubTaskinfo= pCtx->SubTaskInfo + (pCtx->SubTaskNumPerCeva * dev_id);
        }

        REXS_TRACE_DATA(REXS_TRACE_ROUNTED_TO_FREE_CEVA, dev_id);

        pCtx->hw_mask             ^= (1 << dev_id);
        pCtx->hw_res [dev_id]      = pHwTcb;
        pCtx->hw_res_tcbd [dev_id] = pTcbD;
        pCtx->hw_res_num ++;

        #ifdef REX_QUANTUM_ENABLED
        if (pTcbD->Quantum != 0)
        {
            pCtx->hw_res_ticks[dev_id] = MxGetTicks ();
        }
        #endif

        if (pTcbD->TaskID == TASKID_RUN_CEVA_FUNC)
        {
            pCtx->PrvTimeStart[dev_id] = MxGetTicks ();

            //printf ("REX:CEVA direct call(id=%d)\r\n", dev_id);

            // the ceva calls are tracked by using MLog task not start-finish records
            RexServRunTcb(dev_id, pHwTcb, 0);
        }
        else
        {
            RexServRunTcb(dev_id, pHwTcb, 1);
        }
        
    }
}

void CevaRunA (REX_SERV_CTX * pCtx, REXTCBD * pTcbD, TCB * pHwTcb)
{
	pHwTcb = pHwTcb;
	pCtx   = pCtx;
	pTcbD  = pTcbD;

    _ASSERT(0);
}

void CevaRunB (REX_SERV_CTX * pCtx, REXTCBD * pTcbD, TCB * pHwTcb)
{
	pHwTcb = pHwTcb;
	pCtx   = pCtx;
	pTcbD  = pTcbD;

    _ASSERT(0);
}

void FPRun (REX_SERV_CTX * pCtx, REXTCBD * pTcbD, TCB * pHwTcb)
{
    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTcbD);
    _ASSERT((pTcbD->Ctrl & REX_CTRL_TCB_DONE) == 0);

    if (pTcbD->TaskID == TASKID_RX_IDFT)
    {
        if (pCtx->IdftTcbdPtr != NULL)
            return;
    
        pCtx->IdftTcbdPtr = pTcbD;
        pCtx->IdftTcbStart= MxGetTicks();
    
        FpDrvRunTcb(pHwTcb, pCtx->IdftTcbStart);
    }
}

void ClientRunCmd (REX_SERV_CTX * pCtx, REXTCBD * pTcbD, TCB * pHwTcb)
{
    RexCMD * pCmd;
	#ifdef REXS_TASKS_MLOG_ENABLED
    UINT32 t = MxGetTicks ();
	#endif

    REXS_TRACE_DATA(REXS_TRACE_ROUTE_TO_CLIENT, pTcbD->TaskID);

    // till block is allocated
    while ((pCmd = RexPoolAlloc (&pCtx->pInf->client)) == 0);

    pCmd->CmdID = REX_CMD_EXEC_TCB;
    pCmd->Param.ExecTcb.pTcb = pHwTcb;

    RexPoolSend(&pCtx->pInf->client, pCmd);
    RexServNotifyClient();

    REXS_TRACE_DATA(REXS_TRACE_ROUTE_IS_DONE, pTcbD->TaskID);

    _MLogDevTask (REXS_MLOG_CLIENT_RUN_TCB, RESOURCE_REXS, t, MxGetTicks ());

	pTcbD = pTcbD;
}

void ClientRunTcbCbCmd (REX_SERV_CTX * pCtx, REXTCBD * pTcbD, UINT32 nErrCode)
{
    RexCMD * pCmd;
	#ifdef REXS_TASKS_MLOG_ENABLED
    UINT32 t = MxGetTicks ();
	#endif

    REXS_TRACE_DATA(REXS_TRACE_ROUTE_TCB_CB, pTcbD->TaskID);

    //rexs_printf(REXS_RUN_TCB_CB, "REX-SERV: Run#1 TCB-CB on client, tcbd(0x%x), task-id=%d, cb=0x%x\r\n",
    //            pTcbD,
    //            pTcbD->TaskID,
    //            pTcbD->CbDone
    //    );

    // till block is allocated
    while ((pCmd = RexPoolAlloc (&pCtx->pInf->client)) == 0);

    pCmd->CmdID = REX_CMD_EXEC_TCB_CB;
    pCmd->Param.ExecTcbCb.pTcb      = pTcbD->pHwTCB;
    pCmd->Param.ExecTcbCb.CbDone    = pTcbD->CbDone;
    pCmd->Param.ExecTcbCb.CbDonePtr = pTcbD->CbDonePtr;
    pCmd->Param.ExecTcbCb.nErrCode  = nErrCode;

    RexPoolSend(&pCtx->pInf->client, pCmd);
    RexServNotifyClient();

    //rexs_printf(REXS_RUN_TCB_CB, "REX-SERV: Run#2 TCB-CB on client, tcbd(0x%x), task-id=%d, cb=0x%x\r\n",
    //            pTcbD,
    //            pTcbD->TaskID,
    //            pTcbD->CbDone
    //    );

   _MLogDevTask (REXS_MLOG_CLIENT_RUN_TCB_CB, RESOURCE_REXS, t, MxGetTicks ());

   REXS_TRACE_DATA(REXS_TRACE_ROUTE_TCB_CB_DONE, pTcbD->TaskID);

}

void ClientCavaProcIsDoneCmd (REX_SERV_CTX * pCtx, REXTCBD * pTcbD)
{
#if REX_MAX_PRIVATE_TCB_NUM > 0
    RexCMD * pCmd;
    TCBCEVACALL * pTcbCall;

    // to restore private TCB-D/TCB resource
    pCtx->PrvFreeMask |= (1 << pTcbD->ResIndex);

    // till block is allocated
    while ((pCmd = RexPoolAlloc (&pCtx->pInf->client)) == 0);

    pTcbCall = (TCBCEVACALL *)pTcbD->pHwTCB;

    pCmd->CmdID = REX_CMD_CEVA_PROC_DONE;
    pCmd->Param.CevaProcDone.CevaProc = pTcbCall->ProcAddr;
    pCmd->Param.CevaProcDone.ProcRet  = pTcbCall->Params[0];
    pCmd->Param.CevaProcDone.Handle   = pTcbD->ResourceID;

    RexPoolSend(&pCtx->pInf->client, pCmd);
    RexServNotifyClient();
#endif
}

void RexServNotifyBadCevaCmd(REX_SERV_CTX * pCtx, UINT32 nBadMask)
{
    RexCMD * pCmd;

    // till block is allocated
    while ((pCmd = RexPoolAlloc (&pCtx->pInf->client)) == 0);

    pCmd->CmdID = REX_CMD_BAD_CEVA_DETECTED;
    pCmd->Param.BadCevaDet.BadMask = nBadMask;

    RexPoolSend(&pCtx->pInf->client, pCmd);
    RexServNotifyClient();
}

void RexServPushList (REX_SERV_CTX * pCtx, TASKLIST * pList)
{
    UINT32 ctrl = pList->Ctrl;

    if (ctrl & REX_CTRL_LIST_IS_SCHEDULED)
        return;

    if ((ctrl & REX_CTRL_LIST_UNDER_PROC) == 0)
        return;

    REXS_TRACE_DATA(REXS_TRACE_LIST_IS_PUSHED, pList->ID);

    pCtx->proc_list [pCtx->list_put++] = pList;

    if (pCtx->list_put >= REX_SERVER_LIST_NUM)
        pCtx->list_put = 0;

    _ASSERT(pCtx->list_put != pCtx->list_get);

    pList->Ctrl = ctrl | REX_CTRL_LIST_IS_SCHEDULED;

    //rexs_printf(REXS_PUSH_LIST, "REX-SERV PushList(id=%d, ctrl=0x%x) is scheduled\r\n", pList->ID, pList->Ctrl);
}

/** @brief This function adds new one element to the list of scheduled elements,
            the scheduled elements are ready to be routed to the device (CEVA, ARM, FP, etc)

    @param pList [in] - the pointer to the list
    @param pTCB  [in] - the pointer to the scheduled task

    @return [MXRC] an error code

    *  \ingroup group_lte_rexscheduler

    */

void RexServAddToScheduled (PTASKLIST pList, REXTCBD * pTcbD)
{
    UINT32 ctrl;

    _ASSERT_PTR(pList);
    _ASSERT_PTR(pTcbD);

    //rexs_printf (REXS_ADD_TCB_TO_SCHEDULING, "REX-SERV: +sched, list:(%x)(head=%x, tail=%x), tcb-d:(%x) tcb:(%x)\r\n", pList, pList->SchedHead, pList->SchedTail, pTcbD, pTcbD->pHwTCB);

    // just to prevent scheduling of this element twice
    // the case of cross-connected lists (one list is scheduled, the next one is not)

    ctrl = pTcbD->Ctrl;

    if (0 == (ctrl & (REX_CTRL_TCB_SCHEDULED | REX_CTRL_TCB_DONE | REX_CTRL_TCB_LOCKED)))
    {
        if (pList->SchedHead == NULL)
        {
            pList->SchedHead = pTcbD;
            pList->SchedTail = pTcbD;

            pTcbD->RexSchedNext = NULL;
            pTcbD->RexSchedPrev = NULL;
        }
        else
        {
            pList->SchedTail->RexSchedNext = pTcbD;
            pTcbD->RexSchedPrev = (REXTCBD*)pList->SchedTail;

            pList->SchedTail = (volatile REXTCBD*)pTcbD;
            pTcbD->RexSchedNext = NULL;
        }

        pTcbD->Ctrl = ctrl | REX_CTRL_TCB_SCHEDULED;
    }

    //rexs_printf (REXS_ADD_TCB_TO_SCHEDULING, "REX-SERV: >> REX:Scheduling, tcb-d:(%x) tcb(%x), Id=%d, ResID=%d\r\n", pTcbD, pTcbD->pHwTCB, pTcbD->pHwTCB->TaskID, pTcbD->ResourceID);
}

/** @brief This function removes the specified TCB block from the list of references

    @param pTcb     [in] - the pointer to the TCB to be processed
    @param pRefTcb  [in] - the reference

    *  \ingroup group_lte_rexscheduler

    */

void RexServRemoveRef (REXTCBD * pTcbD, REXTCBD * pRefTcbD)
{
    UINT32 i;

    _ASSERT_PTR(pTcbD);
    _ASSERT_PTR(pRefTcbD);

    i = 0;

    while (i < pTcbD->RefNum)
    {
        if (pTcbD->RefList [i] == pRefTcbD)
        {
            if ((i + 1) < pTcbD->RefNum)
            {
                //memcpy (&(pTcb->RefList [i]), &(pTcb->RefList [i+1]), (pTcb->RefNum - i - 1) * sizeof (pTcb->RefList[0]));
                pTcbD->RefList [i] = pTcbD->RefList [pTcbD->RefNum - 1];
            }

            pTcbD->RefList [--pTcbD->RefNum] = NULL;

            // to decrease the number of elements this element depends on
            pRefTcbD->DepNum --;
            continue;
        }

        i++;
    }
}

/** @brief This code scans the list of the TCB tasks and removes references
           to the bypassd TCBs.

    @param pList [in] - the task list
    @param pTcb  [in] - the TCB task

    @return [NONE]

    *  \ingroup group_lte_rexscheduler

    */

void RexServRemoveRefInList (PTASKLIST pList, REXTCBD * pTcbD)
{
    REXTCBD * pBlock;

    _ASSERT_PTR(pTcbD);

    if (pList == NULL)
        return;

    pBlock = (REXTCBD*)pList->TcbHead;

    while (pBlock != NULL)
    {
        if (pBlock != pTcbD)
        {
            RexServRemoveRef (pBlock, pTcbD);
        }

        pBlock = pBlock->RexTcbNext;
    }
}


/** @brief The function is designed to cut the TCB task from the task list
            mainly it's used to reorganize the list according to the new TCB task d-weight

    @param pTcb [in] - the pointer to the TCB task (this task contain pointer to
                       the list in which this TCB is included)

                       *  \ingroup group_lte_rexscheduler

                       */

void RexServCutFromList (REXTCBD * pTcbD)
{
    TASKLIST * pList;

    _ASSERT_PTR (pTcbD);

    pList = (TASKLIST*)pTcbD->RexList;

    // if TCB block is not in the list

    if (pList == NULL)
        return;

    if (pTcbD->RexTcbPrev != NULL)
    {
        pTcbD->RexTcbPrev->RexTcbNext = pTcbD->RexTcbNext;
    }

    if (pTcbD->RexTcbNext != NULL)
    {
        pTcbD->RexTcbNext->RexTcbPrev = pTcbD->RexTcbPrev;
    }

    if (pList->TcbHead == pTcbD)
    {
        pList->TcbHead = pTcbD->RexTcbNext;
    }

    if (pList->TcbTail == pTcbD)
    {
        pList->TcbTail = pTcbD->RexTcbPrev;
    }

    pTcbD->RexTcbNext = NULL;
    pTcbD->RexTcbPrev = NULL;
    pList->TcbNum --;

    // to process the list and removes links to this block
    RexServRemoveRefInList (pList, pTcbD);

    pTcbD->RexList = NULL;
    pTcbD->Ctrl &= ~REX_CTRL_TCB_IN_LIST;
}


void RexServSetTcbCtrl (REX_SERV_CTX * pCtx, TASKLIST * pList, REXTCBD * pTcbD, unsigned int mask)
{
	#ifdef REXS_TASKS_MLOG_ENABLED
    UINT32 t = MxGetTicks ();
	#endif
    UINT32 ctrl;

	pCtx = pCtx;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pList);
    _ASSERT_PTR(pTcbD);

    ctrl = pTcbD->Ctrl;

    if ((mask & REX_BYPASS_TCB) != 0)
    {
        ctrl |= REX_CTRL_TCB_BYPASSED;
        pTcbD->Ctrl = ctrl;
    }

    if ((mask & REX_LOCKED_TCB) != 0)
    {
        ctrl |= REX_CTRL_TCB_LOCKED;
        RexServCutFromSchedList (pList, pTcbD, ctrl);
    }

    _MLogDevTask(pTcbD->TaskID, RESOURCE_REXS, t, MxGetTicks ());
    _MLogDevTask(7777, RESOURCE_REXS, t, MxGetTicks ());
}

void RexServResetTcbCtrl (REX_SERV_CTX * pCtx, TASKLIST * pList, REXTCBD * pTcbD, unsigned int mask)
{
    UINT32 ctrl;
	#ifdef REXS_TASKS_MLOG_ENABLED
    UINT32 t = MxGetTicks ();
	#endif

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTcbD);
    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pList);

    #if 0
    rexs_printf(REXS_TCB_CTRL, "REX-SERV: TCB-RESET-CTRL:(tcb-d:%x) (tcb:%x), ctrl:(%x), task-id:(%d), list-id:(%d) list-tcb-num:(%d), list-tcb-done:(%d)\r\n",
        pTcbD,
        pTcbD->pHwTCB,
        mask,
        pTcbD->TaskID,
        pList->ID,
        pList->TcbNum,
        pList->TcbDone
        );
    #endif

    ctrl = pTcbD->Ctrl;

    if ((mask & REX_BYPASS_TCB) != 0)
    {
        ctrl &= ~REX_CTRL_TCB_BYPASSED;
        pTcbD->Ctrl = ctrl;
    }

    // TO UNLOCK TCB processing if TCB was really locked

    if ((mask & REX_LOCKED_TCB) != 0 && (ctrl & REX_CTRL_TCB_LOCKED))
    {
        ctrl &= ~REX_CTRL_TCB_LOCKED;
        pTcbD->Ctrl = ctrl;

        if ((pList != NULL) && (pTcbD->DepDone == pTcbD->DepNum) && !(ctrl & (REX_CTRL_TCB_DONE | REX_CTRL_TCB_SCHEDULED)))
        {
            RexServAddToScheduled(pList, pTcbD);
            RexServPushList(pCtx, pList);
        }
    }

    _MLogDevTask(pTcbD->TaskID, RESOURCE_REXS, t, MxGetTicks ());
    _MLogDevTask(8888, RESOURCE_REXS, t, MxGetTicks ());
}

void RexServProcDependency (REX_SERV_CTX * pCtx, REXTCBD * pTcbD)
{
    REXTCBD * pProcTcbD;
    unsigned int i = 0, RefNum;
    PTASKLIST pList;
    DEF_ASSERT_LR();

    _ASSERT_PTR_LR(pCtx);
    _ASSERT_PTR_LR(pTcbD);

    RefNum = pTcbD->RefNum;

    while (i < RefNum)
    {
        pProcTcbD = pTcbD->RefList [i];

        _ASSERT_PTR_LR(pProcTcbD);

        pProcTcbD->DepDone++;

        _ASSERT_LR(pProcTcbD->DepDone <= pProcTcbD->DepNum);

        // if dependency of the list TCB is completed and this TCB is not locked
        // and TCB was not processed, we need to add TCB to the list of ready elements
        // and to schedule this list to be processed by kernel

        if ((pProcTcbD->DepDone == pProcTcbD->DepNum) && !(pProcTcbD->Ctrl & (REX_CTRL_TCB_LOCKED | REX_CTRL_TCB_DONE)))
        {
            pList = (PTASKLIST)pTcbD->RexList;

            _ASSERT_PTR_LR(pList);

            RexServAddToScheduled (pList, pProcTcbD);
            RexServPushList(pCtx, pList);
        }

        i++;
    }
}

MXRC RexServReinitList(TASKLIST * pList)
{
    REXTCBD * pTcbD;
    MXRC rc = MX_OK;
    UINT32 i;
    PTASKLIST pRemList;

    //rexs_printf (REXS_REINIT_LIST_CMD, "REX-SERV: Reinit list(0x%x), list-id=%d\r\n", pList, pList->ID);

    pList->TcbDone = 0;
    pList->TcbError= 0;
    pList->Ctrl = 0;
    pList->SchedHead = NULL;
    pList->SchedTail = NULL;

    pTcbD = (REXTCBD *)pList->TcbHead;

    while (pTcbD)
    {
        // Clean up MLog
        //if (pTcbD->Ctrl & REX_CTRL_TCB_BYPASSED)
        //    MLogMarkTcbControl(pTcbD->pHwTCB, 0);

        pTcbD->DepDone      = 0;
        pTcbD->Ctrl         &= (REX_CTRL_TCB_IN_LIST);

        pTcbD->RexSchedNext = NULL;
        pTcbD->RexSchedPrev = NULL;

        pTcbD->RexDevNext   = NULL;
        pTcbD->RexRoutedNext= NULL;

        _ASSERT_PTR(pTcbD->pHwTCB);
        pTcbD->pHwTCB->Status = 0;

        // Check the cross-connections

        for (i = 0; i < pTcbD->RefNum; i++)
        {
            _ASSERT_PTR(pTcbD->RefList [i]);

            pRemList = (PTASKLIST)pTcbD->RefList [i]->RexList;

            // if TCB element that belongs to other list depends on this task
            // we need to decrease the DoneNum to show that this element
            // is not ready (because this list is reinitialized)

            if ((UINT32)pRemList != (UINT32)pTcbD->RexList && !(pRemList->Ctrl & REX_CTRL_LIST_UNDER_PROC))
            {
                //MxGetLock(&pRemList->Lock);

                if (pTcbD->RefList [i]->DepDone != 0)
                    pTcbD->RefList [i]->DepDone --;

                //MxReleaseLock(&pRemList->Lock);
            }
        }

        if ((pTcbD->DepNum == 0) && !(pTcbD->Ctrl & REX_CTRL_TCB_LOCKED))
        {
            RexServAddToScheduled (pList, pTcbD);
        }

        pTcbD = pTcbD->RexTcbNext;
    }

    return rc;
}

TCB * LocalArray [60];

void RexServProcTcbCtrlEx (REX_SERV_CTX * pCtx, UINT32 nSet, RexCMD *pCmd)
{
    UINT32 nNum, loadedNum, i, nCtrl;
    TCB ** p;
    TCB * pTcb;
    REXTCBD *pTcbD;
    PTASKLIST pList = NULL;
	#ifdef REXS_TASKS_MLOG_ENABLED
	UINT32 t = MxGetTicks ();
	#endif

    _ASSERT_PTR(pCmd);

    p    = pCmd->Param.TcbCtrlEx.TcbList;
    nNum = pCmd->Param.TcbCtrlEx.TcbNum;
    nCtrl= pCmd->Param.TcbCtrlEx.TcbCtrl;

    while (nNum != 0)
    {
        loadedNum = MIN(60, nNum);
        i = 0;

        MemCpyFromExt(LocalArray, p, loadedNum * sizeof (TCB*));

        while (i < loadedNum)
        {
            pTcb = LocalArray[i];
            _ASSERT_PTR(pTcb);

            pTcbD = (REXTCBD*)pTcb->RexDescr;
            _ASSERT_PTR(pTcbD);

            // if the task list is already started
            // and local copy is in local CEVA / ARM cache memory

            pList = (TASKLIST*)pTcbD->RexList;

            _ASSERT_PTR(pList);

            if (nSet)
            {
                RexServSetTcbCtrl (pCtx, pList, pTcbD, nCtrl);
            }
            else
            {
                RexServResetTcbCtrl (pCtx, pList, pTcbD, nCtrl);
            }

            i++;
        }

        p += loadedNum;
        nNum -= loadedNum;
    }

	_MLogDevTask (4444, RESOURCE_LARM, t, MxGetTicks ());
	_MLogDevTask (pList->ID * 1000 + pList->Ctrl, RESOURCE_LARM, t, MxGetTicks ());
}

void RexServInitLocalPart (REX_SERV_CTX * pCtx)
{
#if 1
    memset (rex_list_stor, 0, sizeof (rex_list_stor));
    RexServDefSysPart(&pCtx->ListPart, rex_list_stor, sizeof (rex_list_stor[0]), REX_MAX_LIST_NUM);
#endif

#if 1
	memset (rex_tcbd_stor, 0, sizeof (rex_tcbd_stor));
    RexServDefSysPart(&pCtx->TcbDPart, rex_tcbd_stor, sizeof (rex_tcbd_stor[0]), sizeof (rex_tcbd_stor) / sizeof (rex_tcbd_stor[0]));
#endif
}

/** @brief This function adds memory cell dependency to the TCB block
           the TCB will be scheduled to be processed only if all dependencies are done
           CELL dependency is done when CELL value != 0

    @param pTcb [in] - the TCB block
    @param nAddr[in] - the CELL address

    @return [UINT32] rex-server error code */

UINT32 RexServAddCellDep(REX_SERV_CTX * pCtx, TCB * pTcb, UINT32 nAddr, UINT32 nInitVal)
{
    UINT32 i;
    REX_SERV_CELL_DEP_CTX * pCellCtx;
    REX_SERV_CELL * pCell;
    REXTCBD * pTcbD;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pTcb);
    _ASSERT_PTR(nAddr);

    pTcbD = (REXTCBD *)pTcb->RexDescr;

    if (pTcbD == NULL)
        return REX_SERV_RC_TCBD_ERROR;

    if (pTcbD->RexList == NULL)
        return REX_SERV_RC_TCBD_LIST_ERROR;

    pCellCtx = &pCtx->CellCtx;

    // The first step is to find out cell that is already assigned
    // for this memory region

    pCell = pCellCtx->First;

    while (pCell != NULL)
    {
        if (pCell->Addr == nAddr)
            break;

        pCell = pCell->Next;
    }

    if (pCell == NULL)
    {
        // to allocate new space for the cell
        // if this address was not registered

        if ((pCellCtx->Num + 1) >= REXS_CELL_NUM)
            return REX_SERV_RC_NOT_FREE_CELLS;

        i = 0;

        while (i < REXS_CELL_NUM)
        {
            if ((pCellCtx->Cells[i].Status & REX_SERV_CELL_ALLOC) == 0)
            {
                pCell = &(pCellCtx->Cells[i]);
                break;
            }

            i++;
        }

        if (pCell == NULL)
            return REX_SERV_RC_NOT_FREE_CELLS;

        // Just allocated cell has to be initialized
        memset (pCell, 0, sizeof (*pCell));

        // to register new cell in the list of allocated cells
        pCellCtx->Num ++;

        pCell->Next = NULL;
        pCell->Prev = NULL;

        if (pCellCtx->First == NULL)
        {
            pCellCtx->First = pCell;
            pCellCtx->Last  = pCell;
        }
        else
        {
            pCellCtx->Last->Next = pCell;
            pCell->Prev = pCellCtx->Last;

            pCellCtx->Last = pCell;
        }
    }

    if (pCell->TcbDNum + 1 >= REXS_CELL_DEP_NUM)
        return REX_SERV_RC_NOT_FREE_CELLS_TCB;

    pCell->Status       = REX_SERV_CELL_ALLOC | REX_SERV_CELL_ENABLED;
    pCell->Addr         = nAddr;
    pCell->InitVal      = nInitVal;
    REG32(pCell->Addr)  = nInitVal;

    i = 0;
    while (i < REXS_CELL_DEP_NUM)
    {
        if (pCell->TcbD[i] == NULL)
        {
            pCell->TcbDNum ++;
            pCell->TcbD[i] = pTcbD;
            break;
        }

        i++;
    }

    // to register new depencencies

    pTcbD->DepNum++;

    // the sched list should contain only elements with 0 dependencies

    if (pTcbD->Ctrl & REX_CTRL_TCB_SCHEDULED)
    {
        // the element is already in the list
        // we need to reorganize the list according to the new priorities

        RexServCutFromSchedList (pTcbD->RexList, pTcbD, pTcbD->Ctrl);
    }

    return MX_OK;
}

void RexServReinitCells(REX_SERV_CTX * pCtx, UINT32 * pCellList, UINT32 * pNewCellList, UINT32 nCellNum)
{
    UINT32 nAddr, i;
    REX_SERV_CELL* pCell;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCellList);

    for (i = 0; i < nCellNum; i++)
    {
        nAddr = pCellList[i];

        pCell = pCtx->CellCtx.First;

        while (pCell != NULL)
        {
            if (pCell->Addr == nAddr)
            {
                pCell->Status |= REX_SERV_CELL_ENABLED;

                if (pNewCellList != NULL)
                {
                    pCell->Addr = pNewCellList[i];
                }
                
                REG32(pCell->Addr) = pCell->InitVal;

                break;
            }

            pCell = pCell->Next;
        }
    }
}

void RexServCevaMdmaCbDone (LPVOID ctx)
{
    (*(VUINT32*)ctx) = 1;
}

void RexServProcDiagCmd(REX_SERV_CTX * pCtx, RexCMD * pCmd)
{
    RexCmdDiag * pDiag = (RexCmdDiag *)&pCmd->Param.Diag;
    UINT16 rc;
    PMDMA_FDESC pFDesc;
    VUINT32 done = 0;

	pCtx = pCtx;

    switch (pDiag->CmdID)
    {
        case REX_DIAG_CMD_CHECK_SYS_MDMA:
        {
            rc = MdmaDrvOpen(MDMA_HW_SYS);

            if (rc != 0)
                break;

            pFDesc = MdmaDrvAllocDesc(MDMA_HW_SYS, 1);

            if (pFDesc == NULL)
            {
                rc = 0xF0;
                break;
            }

            done = 0;

            MdmaDrvSetCopy(pFDesc);
            MdmaDrvSetData(pFDesc, pDiag->InputPtr, pDiag->InputSize, pDiag->OutputPtr, pDiag->OutputSize);
            MdmaDrvSetLastDesc(pFDesc, RexServCevaMdmaCbDone, (void*)&done);

            rc = MdmaDrvRun(MDMA_HW_SYS, pFDesc);
            if (rc != 0)
                break;

            rc = MdmaDrvWaitCompletion(MDMA_HW_SYS, pFDesc);
            if (rc != 0)
                break;

            if (done == 0)
                rc = 0xF1;

            MdmaDrvClose(MDMA_HW_SYS);
            break;
        }

        case REX_DIAG_CMD_GET_STAT_INFO:
        {
            REX_SERV_STAT_INFO * pInfo = (REX_SERV_STAT_INFO *)pDiag->OutputPtr;

            if (pInfo != NULL)
            {
                pInfo->NumListRun       = pCtx->NumListRun;
                pInfo->NumBadList       = pCtx->NumBadList;
                pInfo->CevaBadDevMask   = pCtx->hw_bad_mask;
                pInfo->NumCevaRest      = pCtx->hw_restored_num;

                rc = 0;
            }
            else
            {
                rc = 0x1;
            }

            break;
        }
		
		case REX_DIAG_CMD_CHECK_PRINT:
		
			uart_printf ("CEVA-CODE:(0x%x/%d 0x%x/%d)\r\n", 
                        pDiag->InputPtr,
                        pDiag->InputSize,
                        pDiag->OutputPtr,
                        pDiag->OutputSize);

            rc = 0;
			break;

        default:
            rc = 0xFFFF;
            break;
    }


    pCmd->RC = rc;
}

MXRC RexServRunCevaProc(REX_SERV_CTX * pCtx, RexCmdCallCevaProc* pCmd)
{
#if REX_MAX_PRIVATE_TCB_NUM>0

    UINT32 put, get;

    // To add CEVA-CALL to the queue of calls

    put = pCtx->CevaCallPut;
    get = pCtx->CevaCallGet;

    memcpy(&pCtx->CevaCall[put], pCmd, sizeof (RexCmdCallCevaProc));

#if 0
    uart_printf ("CEVA PROC: addr:%x num:%x handle:%x {%x %x}\r\n",
        pCmd->CevaProc,
        pCmd->ParamNum,
        pCmd->Handle,
        pCmd->Param[0],
        pCmd->Param[1]);
#endif

    put++;

    if (put >= REXS_CEVA_PROC_CALL_NUM)
    {
        put = 0;
    }

    if (put == get)
        return REX_SERV_RC_CALL_QUEUE_OVERFLOW;

    pCtx->CevaCallPut = put;

    //printf ("\r\nput ceva proc!!\r\n");

    return MX_OK;
#else
    return REX_SERV_RC_FEATURE_TURNED_OFF;
#endif
}

MXRC RexServAssignPrvTcb(REX_SERV_CTX * pCtx, RexCmdPrvTcb * pCmd)
{
    #if REX_MAX_PRIVATE_TCB_NUM>0

        REXTCBD *   pTcbD;
        TCB*        pTcb;

        if (pCmd->nIndex >= REX_MAX_PRIVATE_TCB_NUM)
            return REX_SERV_RC_INDEX_ERROR;

        pTcb = pCmd->pTcb;
        pTcbD= pTcb->RexDescr;

        memset (pTcb,  0, sizeof (*pTcb));
        memset (pTcbD, 0, sizeof (*pTcbD));

        pTcb->RexDescr = pTcbD;
        pTcbD->pHwTCB  = pTcb;

        pCtx->PrvTcb  [pCmd->nIndex] = pTcb;
        pCtx->PrvTcbD [pCmd->nIndex] = pTcbD;

        pCtx->PrvFreeMask |= (1<<pCmd->nIndex);

        return MX_OK;

    #else
        return REX_SERV_RC_FEATURE_TURNED_OFF;
    #endif
}

/** @brief This function processes REX server commands sent from client side,
           please see: REX_SERV_CMD enum

    @param pCmd [in] - the client command */

void RexServProcCmd (REX_SERV_CTX * pCtx, RexCMD *pCmd)
{
    REXTCBD * pTcbD;
    TASKLIST * pList;
    UINT32 inf, blocking, rc;

	#ifdef REXS_TASKS_MLOG_ENABLED
	UINT32 t = MxGetTicks ();
	#endif

    UINT32 time, i;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pCmd);

    blocking = (pCmd->CmdID & REX_CMD_OPT_BLOCKING);
    pCmd->CmdID &= ~REX_CMD_OPT_BLOCKING;

    switch (pCmd->CmdID)
    {
        case REX_CMD_INIT:

            //rexs_printf (REXS_INIT_CMD, "REXS-CMD: Init command processing:\r\n");
            //rexs_printf (REXS_INIT_CMD, "   hw_mask: 0x%x\r\n", pCmd->Param.Init.hw_mask);
            //rexs_printf (REXS_INIT_CMD, " hw_mask-a: 0x%x\r\n", pCmd->Param.Init.hw_mask_pool_a);
            //rexs_printf (REXS_INIT_CMD, " hw_mask-b: 0x%x\r\n", pCmd->Param.Init.hw_mask_pool_b);

            inf = (UINT32)pCtx->pInf;
            memset (pCtx, 0, sizeof (*pCtx));
            pCtx->pInf = (REX_SERV_COMM_INF*)inf;

            pCtx->serv_inf[0] = &pCtx->pInf->server[0];
            pCtx->serv_inf[1] = &pCtx->pInf->server[1];
            pCtx->client_inf  = &pCtx->pInf->client;

            pCtx->hw_mask           = pCmd->Param.Init.hw_mask;

            #ifdef REXS_POOLS_ENABLED
            pCtx->hw_mask_pool_a    = pCmd->Param.Init.hw_mask_pool_a;
            pCtx->hw_mask_pool_b    = pCmd->Param.Init.hw_mask_pool_b;
            #endif

            pCtx->MLogPtr           = pCmd->Param.Init.MLogPtr;
            pCtx->TracePtr          = pCmd->Param.Init.TracePtr;
            pCtx->TraceSize         = pCmd->Param.Init.TraceSize;

            pCtx->SubTaskNumPerCeva = pCmd->Param.Init.SubTaskNum;
            pCtx->SubTaskInfo       = pCmd->Param.Init.SubTaskInfo;

            pCtx->Res[RESOURCE_CEVA]            = (RexServRunType)CevaRun;
            pCtx->Res[REX_RESOURCE_CEVA_POOL_A] = (RexServRunType)CevaRunA;
            pCtx->Res[REX_RESOURCE_CEVA_POOL_B] = (RexServRunType)CevaRunB;

            if (pCmd->Param.Init.ctrl & REX_CFG_IDFT_ON_CEVA)
            {
                pCtx->Res[RESOURCE_MAP] = (RexServRunType)FPRun;
            }
            else
            {
                pCtx->Res[RESOURCE_MAP] = (RexServRunType)ClientRunCmd;
            }
            
            pCtx->Res[RESOURCE_LARM]            = (RexServRunType)ClientRunCmd;

            RexServInitLocalPart (pCtx);

            break;

        case REX_CMD_EXEC_LIST:

            pList = (TASKLIST*)pCmd->Param.ExecList.ListPtr;
            _ASSERT_PTR(pList);

            pList->StartTick = MxGetTicks ();//pCmd->Param.ExecList.StartTicks;

            // to prevent scheduling list twice
            // if a list is already scheduled

            if((pList->Ctrl & REX_CTRL_LIST_UNDER_PROC) == 0)
            {
                //MLogDevTCBListStart(pList->ID);

                pList->TcbDone = 0;
                pList->Ctrl |= REX_CTRL_LIST_UNDER_PROC;

                RexServPushList (pCtx, pList);

                pCtx->NumListRun++;
            }
            else
            {
                RexServSendError (pCtx, REX_SERV_RC_LIST_IS_UNDER_PROC, pList->ID);
            }

            break;

        case REX_CMD_TCB_IS_DONE:

            pTcbD = (REXTCBD*)pCmd->Param.TcbIsDone.TcbPtr->RexDescr;
            _ASSERT_PTR(pTcbD);

            //MLogEXEFinish2(pTcbD->pHwTCB);
            //MLogTask();

            //rexs_printf(REXS_TCB_IS_DONE_CMD, "REXS-CMD:TCB IS DONE, tcb-d=0x%x, tcb=0x%x, task-id=%d, list-id\r\n", pTcbD, pTcbD->pHwTCB, pTcbD->TaskID, ((TASKLIST*)pTcbD->RexList)->ID);

            pList = (PTASKLIST)pTcbD->RexList;

            _ASSERT_PTR(pList);

            pTcbD->Ctrl |= REX_CTRL_TCB_DONE;
            pList->TcbDone ++;

            _ASSERT(pList->TcbDone <= pList->TcbNum);

            if (pList->TcbDone == pList->TcbNum)
            {
                RexServPushList(pCtx, pList);
            }

            RexServProcDependency (pCtx, pTcbD);
            break;

        case REX_CMD_SET_TCB_CONTROL:

            pTcbD = (REXTCBD*)pCmd->Param.TcbCtrl.TcbPtr->RexDescr;
            _ASSERT_PTR(pTcbD);

            pList = (PTASKLIST)pTcbD->RexList;

            _ASSERT_PTR(pList);

            //rexs_printf(REXS_SET_CTRL_CMD, "REX-SERV:Set tcb control: tcb(0x%x), task-id=%d, list-id=%d, mask=0x%x\r\n",
            //            pTcbD,
            //            pTcbD->TaskID,
            //            pList->ID,
            //            pCmd->Param.TcbCtrl.TcbCtrl);

            RexServSetTcbCtrl (pCtx, pList, pTcbD, pCmd->Param.TcbCtrl.TcbCtrl);
            break;

        case REX_CMD_RESET_TCB_CONTROL:

            pTcbD = (REXTCBD*)pCmd->Param.TcbCtrl.TcbPtr->RexDescr;
            _ASSERT_PTR(pTcbD);

            pList = (PTASKLIST)pTcbD->RexList;

            _ASSERT_PTR(pList);

            //rexs_printf(REXS_RESET_CTRL_CMD, "REX-SERV:Set tcb control: tcb(0x%x), task-id=%d, list-id=%d, mask=0x%x\r\n",
            //            pTcbD,
            //            pTcbD->TaskID,
            //            pList->ID,
            //            pCmd->Param.TcbCtrl.TcbCtrl);

            RexServResetTcbCtrl (pCtx, pList, pTcbD, pCmd->Param.TcbCtrl.TcbCtrl);
            break;

        case REX_CMD_REINIT_LIST:

            _ASSERT_PTR(pCmd->Param.ReinitList.List);
            RexServReinitList(pCmd->Param.ReinitList.List);

            break;

        case REX_CMD_NEW_TTI:

            _ASSERT_PTR(pCmd->Param.NewTTI.MLogPtr);
            RexServNewTTIProc (&pCmd->Param.NewTTI);

            break;

        case REX_CMD_SET_TCB_CONTROL_EX:

            RexServProcTcbCtrlEx(pCtx, 1, pCmd);
            break;

        case REX_CMD_RESET_TCB_CONTROL_EX:

            RexServProcTcbCtrlEx(pCtx, 0, pCmd);
            break;

        case REX_CMD_SHUTDOWN:

            pCmd->Status = 1;

            // just go to infinite loop
            // because pCmd is not NULL

            while (pCmd != NULL)
            {
            }

            break;

        case REX_CMD_UPLOAD_MLOG:

            MlogDevUpload ();
            break;

        case REX_CMD_CREATE_LIST:

            pList = RexServAllocSysPart(&pCtx->ListPart);

            if (pList != NULL)
            {
                memset (pList, 0, sizeof (*pList));
                pList->ID = pCmd->Param.CreateList.ListID;
            }

            pCmd->Param.CreateList.ListPtr = (UINT32)pList;
            pCmd->RC = (pList != NULL) ? MX_OK : REX_SERV_RC_LIST_CREATE_ERROR;

            break;

        case REX_CMD_REG_CB_DONE_LIST:

            pList = (PTASKLIST)pCmd->Param.RegCbDoneList.ListPtr;

            _ASSERT_PTR(pList);

            if (pList != NULL)
            {
                pList->CbDone = (REXCBDONE)pCmd->Param.RegCbDoneList.ListCbDone;
                pList->CbDoneParam = pCmd->Param.RegCbDoneList.ListCbDoneParam;
            }

            break;

        case REX_CMD_DESTROY_LIST:

            rc = RexServDestroyTaskList(pCmd->Param.DestroyList.ListPtr, pCmd->Param.DestroyList.TcbArray, &pCmd->Param.DestroyList.ArrNum);
            pCmd->Status = (rc == MX_OK);
            break;

        case REX_CMD_CREATE_TCBD:

            pTcbD = RexServAllocSysPart(&pCtx->TcbDPart);

            if (pTcbD != NULL)
            {
                memset (pTcbD, 0, sizeof (*pTcbD));
                pTcbD->TaskID       = pCmd->Param.CreateTcbD.TaskID;
                pTcbD->ResourceID   = pCmd->Param.CreateTcbD.ResID;
                pTcbD->CbDone       = (REXTCBCBDONE)pCmd->Param.CreateTcbD.CbDone;
                pTcbD->CbDonePtr    = (LPVOID)pCmd->Param.CreateTcbD.CbDonePtr;
                pTcbD->pHwTCB       = (PTCB)pCmd->Param.CreateTcbD.pHwTcb;

                #ifdef REX_QUANTUM_ENABLED
                pTcbD->Quantum      = pCmd->Param.CreateTcbD.Quantum;
                #endif

                if (pTcbD->pHwTCB != NULL)
                    pTcbD->pHwTCB->RexDescr = pTcbD;

                pCmd->RC = MX_OK;
            }
            else
            {
                pCmd->RC = REX_SERV_RC_TCBD_CREATE_ERROR;
            }

            break;

        case REX_CMD_DESTROY_TCBD:
            rc = RexServDestroyTCB((REXTCBD*)pCmd->Param.DestroyTcbD.pTcbD);
            pCmd->RC = rc;
            break;

        case REX_CMD_ADD_DEP:

            rc = RexServAddDep(pCmd->Param.AddDep.pTcb, pCmd->Param.AddDep.pDepOnThis);
            pCmd->RC = rc;

            break;

        case REX_CMD_ADD_TO_LIST:

            pList = (PTASKLIST)pCmd->Param.AddToList.pList;

            rc = RexServAddTCBToList(pList, pCmd->Param.AddToList.pTcb, pCmd->Param.AddToList.Prior);

            if (rc == MX_OK)
            {
                pCmd->Param.AddToList.ListID = pList->ID;
            }

            pCmd->RC = rc;

            break;

        case REX_CMD_GET_LIST_INFO:

            pList = pCmd->Param.ListInfo.pList;

            if (pList != NULL)
            {
                pCmd->Param.ListInfo.Ctrl   = pList->Ctrl;
                pCmd->Param.ListInfo.TcbNum = pList->TcbNum;
                pCmd->Param.ListInfo.TcbDone= pList->TcbDone;
                pCmd->Param.ListInfo.ListID = pList->ID;
                pCmd->Param.ListInfo.TcbError=pList->TcbError;
            }

            pCmd->RC = (pList != NULL) ? MX_OK : REX_SERV_RC_LIST_ID_ERROR;

            break;

        case REX_CMD_ADD_CELL_DEP:

            pCmd->RC = RexServAddCellDep (pCtx, pCmd->Param.AddCellDep.pTcb, pCmd->Param.AddCellDep.CellAddr, pCmd->Param.AddCellDep.CellInitVal);
            break;

        case REX_CMD_REINIT_CELLS:
            RexServReinitCells (pCtx, 
                                pCmd->Param.ReinitCells.CellList, 
                                pCmd->Param.ReinitCells.NewCellList,
                                pCmd->Param.ReinitCells.CellNum);
            break;

        case REX_CMD_DIAGNOSTICS:
            RexServProcDiagCmd (pCtx, pCmd);
            break;

        case REX_CMD_RESTORED_CEVA:

            time = MxGetTicks ();
            pCtx->hw_rst_mask |= pCmd->Param.BadCevaRest.RestMask;

            for(i = 0; i < RES_SERV_HW_CEVA_NUM; i++)
            {
                if ((pCtx->hw_rst_mask & (1<<i)) != 0)
                {
                    pCtx->hw_rst_dev_time[i] = time;
                }
            }

            break;

        case REX_CMD_CALL_CEVA_PROC:
            pCmd->RC = RexServRunCevaProc(pCtx, &pCmd->Param.CallCevaProc);
            break;

        case REX_CMD_PRV_TCB:
            pCmd->RC = RexServAssignPrvTcb(pCtx, &pCmd->Param.PrvTcb);
            break;

        case REX_CMD_EXEC_LIST_SET:

            time = MxGetTicks ();//pCmd->Param.ExecList.StartTicks;

            for (i = 0; i < 4; i++)
            {
                pList = (TASKLIST*)pCmd->Param.ExecListSet.ListPtr[i];

                if (pList == NULL)
                    continue;

                pList->StartTick = time;

                // to prevent scheduling list twice
                // if a list is already scheduled

                if((pList->Ctrl & REX_CTRL_LIST_UNDER_PROC) == 0)
                {
                    //MLogDevTCBListStart(pList->ID);

                    pList->TcbDone = 0;
                    pList->Ctrl |= REX_CTRL_LIST_UNDER_PROC;

                    RexServPushList (pCtx, pList);

                    pCtx->NumListRun++;
                }
                else
                {
                    RexServSendError (pCtx, REX_SERV_RC_LIST_IS_UNDER_PROC, pList->ID);
                }
            }

            break;

        default:

            //rexs_printf(REXS_UNKNOWN_CMD, "REX-SERV: Unknown command is received, id=%d\r\n", pCmd->CmdID);
            RexServSendError (pCtx, REX_SERV_RC_UNKNOWN_CMD, pCmd->CmdID);
            break;
    }

    _MLogDevTask (REXS_MLOG_PROC_CMD + pCmd->CmdID, RESOURCE_REXS, t, MxGetTicks ());

    if (blocking)
    {
        pCmd->Status = 1;
    }
}

void RexServCompleteList (REX_SERV_CTX * pCtx, TASKLIST *pLocalList)
{
    RexCMD * pCmd;

    #ifdef REXS_TASKS_MLOG_ENABLED
    UINT32 t;
    #endif

    if (pLocalList->Ctrl & REX_CTRL_LIST_IS_DONE)
        return;

    #ifdef REXS_TASKS_MLOG_ENABLED
    t = MxGetTicks ();
    #endif

    if (pLocalList->TcbError != 0)
    {
        pCtx->NumBadList++;
    }

    pLocalList->Ctrl |= REX_CTRL_LIST_IS_DONE;

    // *************************************************
    // REX_CTRL_LIST_IS_DONE will be set on client side
    // in RexCompleteList() function
    // *************************************************

    //rexs_printf (REXS_LIST_IS_DONE, "REX-SERV:List is done#1(sending info to client), list-id=%d\r\n", pList->ID);

    // till block is allocated
    while ((pCmd = RexPoolAlloc (&pCtx->pInf->client)) == NULL);

    pCmd->CmdID = REX_CMD_LIST_IS_DONE;

    pCmd->Param.ListIsDone.ListPtr    = pLocalList;
    pCmd->Param.ListIsDone.ListID     = pLocalList->ID;
    pCmd->Param.ListIsDone.CbDone     = (UINT32)pLocalList->CbDone;
    pCmd->Param.ListIsDone.CbDonePtr  = (UINT32)pLocalList->CbDoneParam;
    pCmd->Param.ListIsDone.StartTicks = pLocalList->StartTick;
    pCmd->Param.ListIsDone.NumBadTcb  = pLocalList->TcbError;

    RexPoolSend(&pCtx->pInf->client, pCmd);
    RexServNotifyClient();

    _MLogDevTask(REXS_MLOG_COMPLETE_LIST + pLocalList->ID, RESOURCE_REXS, t, MxGetTicks());
    REXS_TRACE_DATA(REXS_TRACE_LIST_IS_DONE_SENT, pLocalList->ID);
}

void RexServProcList(REX_SERV_CTX * pCtx, TASKLIST *pList)
{
    REXTCBD* pProcTcbD;
    UINT32 ctrl;
	//UINT32 id;

	#ifdef REXS_TASKS_MLOG_ENABLED
    UINT32 t = MxGetTicks (), t1;
	#endif

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pList);

    //rexs_printf(REXS_PROC_LIST_TCB, "REX-SERV: List TCB processing, list-id=%d\r\n", pList->ID);

    //id = pList->ID;

    while (pList->SchedHead)
    {
        pProcTcbD = (REXTCBD*)pList->SchedHead;
        pList->SchedHead = (volatile REXTCBD*)pProcTcbD->RexSchedNext;

        ctrl = pProcTcbD->Ctrl;

        if (ctrl & REX_CTRL_TCB_DONE)
            continue;

        ctrl &= ~REX_CTRL_TCB_SCHEDULED;
        if (ctrl & REX_CTRL_TCB_BYPASSED)
        {
            //t1 = MxGetTicks();

            pProcTcbD->Ctrl = ctrl | REX_CTRL_TCB_DONE;
            pList->TcbDone ++;
            RexServProcDependency (pCtx, pProcTcbD);

            //if (id == 0)
            //    _MLogDevTask (10000 * pProcTcbD->RefNum + pProcTcbD->TaskID, RESOURCE_REXS, t1, MxGetTicks());
        }
        else
        {
            pProcTcbD->Ctrl = ctrl;
            _ASSERT (pProcTcbD->ResourceID < REX_SUPPORTED_RES);
            _ASSERT_PTR (pCtx->Res[pProcTcbD->ResourceID]);

            //rexs_printf (REXS_PROC_LIST_TCB_ON_HW, "REX-SERV: Route TCB-D(0x%x) task-id=%d to HW(%d)\r\n",
            //            pProcTcbD,
            //            pProcTcbD->TaskID,
            //            pProcTcbD->ResourceID
            //    );

            REXS_TRACE_DATA2(REXS_TRACE_RUN_TCB, pProcTcbD->TaskID, pProcTcbD->ResourceID);
            pCtx->Res[pProcTcbD->ResourceID](pCtx, pProcTcbD, pProcTcbD->pHwTCB);
        }
    }

    pList->SchedTail = NULL;
    pList->Ctrl &= ~REX_CTRL_LIST_IS_SCHEDULED;

    //rexs_printf(REXS_PROC_LIST_TCB, "REX-SERV: List TCB processing is done, list-id=%d\r\n", pList->ID);

    _MLogDevTask (REXS_MLOG_PROC_LIST, RESOURCE_REXS, t, MxGetTicks());

    // if the list is done
    if (pList->TcbNum == pList->TcbDone)
    {
        REXS_TRACE_DATA(REXS_TRACE_LIST_IS_DONE, pList->ID);
        RexServCompleteList (pCtx, pList);
    }
}

void RexServProcLists (REX_SERV_CTX * pCtx)
{
    TASKLIST * pList;

    while (pCtx->list_get != pCtx->list_put)
    {
        pList = pCtx->proc_list [pCtx->list_get ++];

        //rexs_printf (REXS_PROC_LIST_TCB, "REX-SERV: ProcLists proc: List=0x%x, id=%d, tcb-num=%d/done=%d\r\n", pList, pList->ID, pList->TcbNum, pList->TcbDone);

        if (pCtx->list_get >= REX_SERVER_LIST_NUM)
        {
            pCtx->list_get = 0;
        }

        REXS_TRACE_DATA(REXS_TRACE_PROC_LIST, pList->ID);

        RexServProcList(pCtx, pList);
    }
}

/** @brief This function goes through cevas and checks completion status
           and routes new ready TCBs to free CEVAs

    @param pCtx[in] - the pointer to the REX server context */

void RexServProcRes (REX_SERV_CTX * pCtx)
{
    unsigned int dev, subnum, i, status;
    REXTCBD * pTcbD, *pTcbDToRunCb;
    TCB * pTcb;
    TASKLIST * pList;
    UINT32 nHangMask = 0, overtime;
    UINT32 t = MxGetTicks ();

	#ifdef REXS_TASKS_MLOG_ENABLED
	UINT32 mlog = 0;
	#endif

    //rexs_printf(REXS_PROC_RES_BEGIN, "REX-SERV: ProcRes begin\r\n");

    for (dev = 0; (dev < RES_SERV_HW_CEVA_NUM) && (pCtx->hw_res_num != 0); dev ++)
    {
        pTcb = pCtx->hw_res [dev];

        if (pTcb != NULL)
        {
            pTcbD = pCtx->hw_res_tcbd[dev];
            _ASSERT_PTR(pTcbD);

            overtime = 0;

            status = (pTcb->Status & TCB_CFG_STAT_MARK_DONE);

            #ifdef REX_QUANTUM_ENABLED
            if ((pTcbD->Quantum != 0) && ((t - pCtx->hw_res_ticks[dev]) > pTcbD->Quantum) && (status == 0))
            {
                // The situation when TCB processing is overtimed
                // this is a situation when CEVA does hang, we need to
                // inform the host about such situation
                // and to reload CEVA code

                nHangMask |= (1 << dev);
                overtime   = 1;
                
//                uart_printf ("\r\n\r\nREX-CEVA: overtime(%d), taskid-%d, q=%d, ceva=%d\r\n\r\n", t - pCtx->hw_res_ticks[dev], pTcbD->TaskID, pTcbD->Quantum, dev);
            }
            #endif

            // if routed CEVA task is completed or processing is overtimed
            if ((status != 0) || overtime)
            {
                pCtx->hw_res_num --;

                REXS_TRACE_DATA(REXS_TRACE_SLAVE_CEVA_DONE, dev);

				#ifdef REXS_TASKS_MLOG_ENABLED
                mlog = 1;
				#endif

                if (pCtx->SubTaskNumPerCeva != 0 && !overtime)
                {
                    subnum = pTcb->SubTaskNum;

                    _ASSERT_PTR(pTcb->SubTaskinfo);
                    _ASSERT(subnum < pCtx->SubTaskNumPerCeva);

                    for (i = 0; i < subnum; i++)
                    {
                        MLogDevSubTask(pTcb, pTcb->SubTaskinfo[i].TaskID, pTcb->SubTaskinfo[i].TaskMips);
                    }
                }

                if (pTcbD->TaskID == TASKID_RUN_CEVA_FUNC)
                {
                    MLogDevTaskEx(((PTCBCEVACALL)pTcb)->ProcAddr, 
                                  RESOURCE_CEVA, 
                                  dev, 
                                  //0, // till TTBox supports this
                                  pCtx->PrvTimeStart[dev], 
                                  t,
                                  0xFFFFFFFF);
                }
                else
                {
                    MLogDevEXEFinishError(pTcb, dev, overtime ? 1 : 0);
                    //MLogDevEXEFinish2(pTcb, dev);
                }

                pList = (TASKLIST*)pTcbD->RexList;

                #if REX_MAX_PRIVATE_TCB_NUM == 0
                _ASSERT_PTR(pList);
                #endif

                if (!overtime)
                {
                    // to add the CEVA to free pool

                    pCtx->hw_mask |= (1 << dev);
                }
                else
                {
                    // the CEVA is not reverted to free pool
                    // and marked like bad device
                    nHangMask |= (1 << dev);
                }

                pTcbD->Ctrl |= REX_CTRL_TCB_DONE;
                pTcbDToRunCb = pTcbD;

                // if we have something to do,
                // we immediately route new data to free CEVA

                if (!overtime)
                {
                    if ((pTcbD = RexServPopTcb (pCtx)) != NULL)
                    {
                        CevaRun (pCtx, pTcbD, pTcbD->pHwTCB);
                    }
                    else
                    {
                        pCtx->hw_res     [dev] = NULL;
                        pCtx->hw_res_tcbd[dev] = NULL;
                    }
                }
                else
                {
                    pCtx->hw_res     [dev] = NULL;
                    pCtx->hw_res_tcbd[dev] = NULL;
                }

                if (pTcbDToRunCb->CbDone != NULL)
                    ClientRunTcbCbCmd(pCtx, pTcbDToRunCb, overtime);

                // pList can be NULL for private TCBs
                // ----------------------------------
                if (pList != NULL)
                {
                    // to increase the number of processed TCBs
                    pList->TcbDone ++;

                    if (overtime)
                    {
                        pList->TcbError ++;
                    }

                    // if all elements of the list are processed
                    // we need to put this list to the queue
                    // to call the list callback

                    if (pList->TcbDone == pList->TcbNum)
                    {
                        RexServPushList (pCtx, pList);
                    }

                    RexServProcDependency (pCtx, pTcbDToRunCb);
                }
                else
                {
                    ClientCavaProcIsDoneCmd(pCtx, pTcbDToRunCb);
                }
            }
        }
    }

    if (nHangMask != 0)
    {
        RexServNotifyBadCevaCmd(pCtx, nHangMask);
    }

	#ifdef REXS_TASKS_MLOG_ENABLED
    if (mlog)
    {
        _MLogDevTask (REXS_MLOG_PROC_RES, RESOURCE_REXS, t, MxGetTicks ());
    }
	#endif
}

/** @brief This function goes through cevas and checks completion status
           and routes new ready TCBs to free CEVAs

    @param pCtx[in] - the pointer to the REX server context */

void RexServProcCells (REX_SERV_CTX * pCtx)
{
    UINT32 i, num;
    REX_SERV_CELL* pCell;
    REXTCBD * pTcbD;
    TASKLIST * pList,*pTcbList;

    _ASSERT_PTR(pCtx);

    pCell = pCtx->CellCtx.First;

    while (pCell != NULL)
    {
        if (pCell->Status & REX_SERV_CELL_ENABLED)
        {
            if (REG32(pCell->Addr) != pCell->InitVal)
            {
                // if the cell is processed, we can mark it like done
                // to optimize the time of data processing
                // external memory is so slow related to the internal or cacheable

                pCell->Status &= ~REX_SERV_CELL_ENABLED;

                i = 0;
                num = 0;

                while (num < pCell->TcbDNum && i < REXS_CELL_DEP_NUM)
                {
                    pTcbD = pCell->TcbD[i];

                    if (pTcbD != NULL)
                    {
                        pTcbList = (TASKLIST*)pTcbD->RexList;

                        if (pTcbList != NULL && (((pTcbList->Ctrl & REX_CTRL_LIST_UNDER_PROC) == 0) || ((pTcbList->Ctrl & REX_CTRL_LIST_IS_DONE))))
                        {
                            num++;
                            i ++;
                            continue;
                        }

                        pTcbD->DepDone ++;

                        _ASSERT(pTcbD->DepDone <= pTcbD->DepNum);

                        // if dependency of the list TCB is completed and this TCB is not locked
                        // and TCB was not processed, we need to add TCB to the list of ready elements
                        // and to schedule this list to be processed by kernel

                        if ((pTcbD->DepDone == pTcbD->DepNum) && !(pTcbD->Ctrl & (REX_CTRL_TCB_LOCKED | REX_CTRL_TCB_DONE)))
                        {
                            pList = (PTASKLIST)pTcbD->RexList;

                            _ASSERT_PTR(pList);

                            RexServAddToScheduled (pList, pTcbD);
                            RexServPushList(pCtx, pList);
                        }

                        num ++;
                    }

                    i++;
                }
            }
        }

        pCell = pCell->Next;
    }
}

/** @brief This code checks if CEVA are up,
           and to return such devices to good device pool

    @param pCtx[in] - the pointer to the context*/

void RexServProcBadCevas(REX_SERV_CTX * pCtx)
{
    UINT32 t, i;
    TCB * pTcb;
    REXTCBD *pTcbD;

    _ASSERT_PTR(pCtx);

    if (pCtx->hw_rst_mask == 0)
        return;

    t = MxGetTicks ();

    for (i = 0; i < RES_SERV_HW_CEVA_NUM; i++)
    {
        if ((pCtx->hw_rst_mask & (1 << i)) == 0)
            continue;

        pTcb = (TCB*)(REG32(0xFC000000 + (i + 2) * 4));

        if (pTcb != NULL)
        {
            if (pTcb->Status == STATUS_COMPLETE)
            {
                pCtx->hw_rst_mask &= ~(1 << i);
                pCtx->hw_mask |= (1 << i);
                pCtx->hw_restored_num++;

                //printf ("REX:recovered(id:%x), hwmask:0x%x\r\n", i, pCtx->hw_mask);

                continue;
            }
        }

        if ((t - pCtx->hw_rst_dev_time[i]) > REXS_CEVA_RST_TIMEOUT)
        {
            //printf ("REX:bad ceva(id:%x)\r\n", i);
            pCtx->hw_rst_mask &= ~(1 << i);
            pCtx->hw_bad_mask |= (1 << i);
        }
    }

    // New CEVAs are ready to be used 
    // in this case let's route the tasks stored 
    // in the CEVA queue
    //******************************************
    while (pCtx->hw_mask != 0)
    {
        if ((pTcbD = RexServPopTcb (pCtx)) != NULL)
        {
            //printf ("route to recovered(task:%d)(mask:%x)\r\n", pTcbD->TaskID, pCtx->hw_mask);
        
            pCtx->Res[RESOURCE_CEVA](pCtx, pTcbD, pTcbD->pHwTCB);
        }
        else
        {
            break;
        }
    }
}

void RexServProcCevaCalls (REX_SERV_CTX * pCtx)
{
#if REX_MAX_PRIVATE_TCB_NUM > 0

    UINT32 free_id;
    RexCmdCallCevaProc * pProc;
    UINT32 get;
    TCB * pTcb;
    REXTCBD * pTcbD;
    PTCBCEVACALL pTcbCall;

    get = pCtx->CevaCallGet;

    while (1)
    {
        if (pCtx->CevaCallPut == get)
            break;

        if (pCtx->PrvFreeMask == 0)
            break;

        free_id = RexServGetFreePrvTcb(pCtx->PrvFreeMask);

        pCtx->PrvFreeMask &= ~(1<<free_id);

        pProc = &pCtx->CevaCall[get];
        pTcb  = pCtx->PrvTcb[free_id];
        pTcbD = pCtx->PrvTcbD[free_id];

        pTcbCall = (PTCBCEVACALL)pTcb;

        pTcbCall->TaskID    = TASKID_CALL_CEVA_PROC;
        pTcbCall->ProcAddr  = pProc->CevaProc;
        pTcbCall->ParamNum  = pProc->ParamNum;
        pTcbD->ResIndex     = free_id;          // To restore occupied resource after call
        pTcbD->ResourceID   = pProc->Handle;    // The handle provided by the ARM client for this CEVA call (it will be returned to ARM)
        pTcbD->TaskID       = TASKID_CALL_CEVA_PROC;

        #ifdef REX_QUANTUM_ENABLED
        pTcbD->Quantum = pProc->CevaProcQ;
        #endif

        if (pProc->ParamNum != 0)
            memcpy (pTcbCall->Params, pProc->Param, pProc->ParamNum * sizeof (UINT32));

        //printf ("\r\nrun ceva proc!!\r\n");

        // to forward request to CEVA
        pCtx->Res[RESOURCE_CEVA](pCtx, pTcbD, pTcb);

        if ((++get) >= REXS_CEVA_PROC_CALL_NUM)
        {
            get = 0;
        }
    }

    pCtx->CevaCallGet = get;

#endif
}

void RexServProcFP(REX_SERV_CTX * pCtx)
{
    REXTCBD * pTcbD;
    TCB* pTcb;
    TASKLIST *pTcbList;
    UINT32 overtime=0, time=0;

    _ASSERT_PTR(pCtx);

    if (pCtx->IdftTcbdPtr == NULL)
        return;

    #ifdef REX_QUANTUM_ENABLED
    if (pCtx->IdftTcbdPtr->Quantum != 0)
    {
        time = MxGetTicks ();
        overtime = ((time - pCtx->IdftTcbStart) > pCtx->IdftTcbdPtr->Quantum) ? 1 : 0;
    }
    #endif

    if (!FpDrvIsIdftDone () || overtime)
        return;

    pTcbD = pCtx->IdftTcbdPtr;
    pTcb  = pTcbD->pHwTCB;

    #ifdef REX_QUANTUM_ENABLED
    if (pCtx->IdftTcbdPtr->Quantum == 0)
    {
        time = MxGetTicks ();
    }
    #endif

    pTcb->ExecTicks = time - pCtx->IdftTcbStart;
    MLogDevEXEFinish(pTcb);

    pTcbList = (TASKLIST*)pTcbD->RexList;

    if (pTcbList != NULL)
    {
        // to increase the number of processed TCBs
        pTcbList->TcbDone ++;

        if (overtime)
        {
            pTcbList->TcbError ++;
        }

        // if all elements of the list are processed
        // we need to put this list to the queue
        // to call the list callback

        if (pTcbList->TcbDone == pTcbList->TcbNum)
        {
            RexServPushList (pCtx, pTcbList);
        }

        RexServProcDependency (pCtx, pTcbD);
    }

    pCtx->IdftTcbdPtr = NULL;
}

void RexServPreInit(void)
{
	REX_SERV_CTX * pCtx = RexServGetCtx();

	if (pCtx != NULL)
	{
		memset (pCtx, 0, sizeof (REX_SERV_CTX));
	}
}

/** @brief The REX server kernel, this code processes TCB/TCBD blocks
           and makes possible to complete the list(s), also it communicates with
           client part of REX located on other platform

*/
void RexServKernel (UINT32 nCheckComm)
{
    REX_SERV_CTX * pCtx = RexServGetCtx();
    REX_SERV_COMM_INF * pInf = RexServGetInf ();
    RexCMD * pCmd;

    _ASSERT_PTR(pCtx);
    _ASSERT_PTR(pInf);

	nCheckComm = nCheckComm;

    //rexs_printf (REXS_KERNEL_ENTRY, "REXS: Kernel Entry, cpu=%d\r\n", MxGetCpuID());

#ifdef REX_SERV_ON_ARM
    while (ServThreadRun)
#else

    if (pInf != NULL)
#endif
    {
		//REXS_TRACE(REXS_TRACE_KERNEL_ENTRY);
		//REXS_TRACE_DATA(pInf);

        // if we have nothing to do
        // let's check for the new commands

		if (pCtx->list_put == pCtx->list_get)
		{
			while ((pCmd = RexPoolRecv (&pInf->server[0])) != NULL)
			{
				REXS_TRACE_DATA(REXS_TRACE_RECV_CMD, (UINT32)pCmd->CmdID);

				//rexs_printf (REXS_KERNEL_RECV_CMD, "REXS: recv CMD(%d), ptr=0x%x\r\n", pCmd->CmdID, pCmd);

				RexServProcCmd (pCtx, pCmd);

				REXS_TRACE_DATA(REXS_TRACE_PROC_DONE, pCmd->CmdID);

				//rexs_printf (REXS_KERNEL_FREE_CMD, "REXS: free CMD(%d), ptr=0x%x\r\n", pCmd->CmdID, pCmd);
				RexPoolFree(&pInf->server[0], pCmd);
			}

			while ((pCmd = RexPoolRecv (&pInf->server[1])) != NULL)
			{
				REXS_TRACE_DATA(REXS_TRACE_RECV_CMD, (UINT32)pCmd->CmdID);

				//rexs_printf (REXS_KERNEL_RECV_CMD, "REXS: recv CMD(%d), ptr=0x%x\r\n", pCmd->CmdID, pCmd);

				RexServProcCmd (pCtx, pCmd);

				REXS_TRACE_DATA(REXS_TRACE_PROC_DONE, pCmd->CmdID);

				//rexs_printf (REXS_KERNEL_FREE_CMD, "REXS: free CMD(%d), ptr=0x%x\r\n", pCmd->CmdID, pCmd);
				RexPoolFree(&pInf->server[1], pCmd);
			}
		}

        // --------------------------------------------
        // to handle ready lists and to put ready
        // to be processed elements to the queues
        // --------------------------------------------
        RexServProcLists (pCtx);

        // --------------------------------------------
        // to route TCB blocks to HW resources
        // --------------------------------------------
        RexServProcRes (pCtx);

        // --------------------------------------------
        // to process memory cells associated with TCB
        // tasks like additional dependencies
        // --------------------------------------------
        RexServProcCells(pCtx);

        // --------------------------------------------
        // to handle bad CEVAs by checking status of this devices
        // ARM code was informed about bad devices
        // and restarted it, REX-S is responcible to check
        // if CEVA(s) are OK, then return such device(s)
        // to the pool of good devices
        // --------------------------------------------
        RexServProcBadCevas(pCtx);

        // --------------------------------------------------
        // to process CEVA direct calls from ARM application
        // --------------------------------------------------
        RexServProcCevaCalls(pCtx);

        // --------------------------------------------------
        // to process FP tasks (routed to be processed on FP)
        // --------------------------------------------------
        RexServProcFP(pCtx);
    }

    //rexs_printf (REXS_KERNEL_ENTRY, "REXS: Kernel EXIT, cpu=%d\r\n", MxGetCpuID());
}


