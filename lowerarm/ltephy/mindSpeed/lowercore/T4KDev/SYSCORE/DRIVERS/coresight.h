//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL coresight.h
 *
 * @brief CoreSight components' registers
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
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


#ifndef CORESIGHT_H_
#define CORESIGHT_H_

#include "syscore.h"

#define ERRCODE_OK      0
#define ERRCODE_INIT    0xfffe
#define ERRCODE_NOMEM   0xfffd
#define ERRCODE_TIMEOUT 0xfffc
#define ERRCODE_PTM     0xfffb


#define TRACER_TIMEOUT      10000
#define TRACER_RUNNING(ID)	(1 << (ID))
#define TRACER_MASK         0x3F
#define TRACER_INIT         (1 << 12)

#define MAX_PTM                 6
#define BASEADDR_PTM_REG0       0xFE71C000
#define BASEADDR_PTM_REG1       0xFE71D000
#define BASEADDR_PTM_REG2       0xFE71E000
#define BASEADDR_PTM_REG3       0xFE71F000
#define BASEADDR_PTM_REG4       0xFE73C000
#define BASEADDR_PTM_REG5       0xFE73D000

#define BASEADDR_ETB_REG        0xFE601000
#define BASEADDR_FUNNEL_REG     0xFE604000


typedef struct stTraceCtx
{
    UINT32   ptm_addr[MAX_PTM];
	UINT32   etb_addr;
	UINT32   funnel_addr;

	UINT32   etb_bufsz;
	UINT32   flags;
}TraceCtx;

typedef struct stPtmConfig
{
    UINT8      PowerDown;
	UINT8      BranchOutput;
	UINT8      CycleAccurate;
	UINT8      ContextIDSize;
	UINT8      TimestampEnable;
	UINT8      ReturnStackEnable;
	UINT8      ForceSyncPacket;
	UINT8      SeqState;
	UINT32     SeqTranEvt1To2;
	UINT32     SeqTranEvt1To3;
	UINT32     SeqTranEvt2To1;
	UINT32     SeqTranEvt2To3;
	UINT32     SeqTranEvt3To1;
	UINT32     SeqTranEvt3To2;
	UINT32     TimestampEvent;
	UINT32     SyncFreq;
	UINT32     AddrCompVal[8];
	UINT32     ContextIDCompVal;
	UINT32     ContextIDCompMask;
	UINT32     CounterVal[2];
	UINT32     CounterEnable[2];
	UINT32     CounterReloadVal[2];
	UINT32     CounterReloadEvt[2];
	UINT16     TraceEnableStart;
	UINT16     TraceEnableStop;
	UINT8      TraceEnableActive;
	UINT8      TraceRangeExclude;
	UINT16     TraceRange;
	UINT32     TraceEnableEvt;
} PtmConfig, *PPtmConfig;


/* CoreSight Management Registers */
#define CSMR_LOCKACCESS 0xfb0
#define CSMR_LOCKSTATUS 0xfb4

#define UNLOCK_MAGIC	0xc5acce55


/* PTM control register */
/* 1. PTM configuration */
#define ETMCR       0x000
#define ETMCR_POWERDOWN          1
#define ETMCR_BRANCHOUTPUT		 (1 << 8)
#define ETMCR_DRC                (1 << 9)
#define ETMCR_PROGRAM		     (1 << 10)
#define ETMCR_CYCLEACCURATE      (1 << 12)
#define ETMCR_CONTEXTIDSIZE	     (3 << 14)
#define ETMCR_TIMESTAMPENABLE    (1 << 28)
#define ETMCR_ReturnStackEnable  (1 << 29)

#define ETMCR_OPTS		(ETMCR_BRANCHOUTPUT | \
				ETMCR_CONTEXTIDSIZE)

#define ETMCCR      0x004
#define ETMTRRIGER  0x008
#define ETMSR       0x010
#define ETMSCR      0x014

/* 2. TraceEnable control */
#define ETMTSSCR    0x018
#define ETMTEEVR    0x020
#define ETMTECR1    0x024

#define ETMTRACEIDR 0x200

/*3. Address comparators*/
#define ETMACVRn    0x040     /* 0x040 ~ 0x05C, address comparator value 1-8 */
#define ETMACTRn    0x080     /* 0x080 ~ 0x09C, address comparator access type 1-8 */

/*4. Counters */
#define ETMCNTRLDVRn     0x140   /* 0x140~0x144 counter reload value 1-2*/
#define ETMCNTENRn       0x150   /* 0x150~0x154 counter enable 1-2*/
#define ETMCNTRLDEVRn    0x160   /* 0x160~0x164 counter reload event 1-2*/
#define ETMCNTVRn        0x170   /* 0x170~0x174 counter value 1-2*/

/*5. Sequencer */
#define ETMSQabEVR       0x180   /* sequencer state transition event 1-6 */
#define ETMSQR           0x19C

/*6. Context ID comparators */
#define ETMCIDCVR       0x1B0   /* context ID comparator value 1*/
#define ETMCIDCMR       0x1BC   /* context ID comparator mask */

/*7. General control */
#define ETMSYNCFR      0x1E0    /* synchronization frequency */
#define ETMTSEVR       0x1F8    /* timestamp event */
#define ETMAUXCR       0x1FC    /* auxiliary control register */

/* ETM management registers, "ETM Architecture", 3.5.24 */
#define ETMMR_OSLAR	0x300
#define ETMMR_OSLSR	0x304
#define ETMMR_OSSRR	0x308
#define ETMMR_PDSR	0x314


#define ptm_writel(t, x, v, id) \
	(REG32(((t)->ptm_addr[id])+(x)) = (v))
#define ptm_readl(t, x, id) (REG32(((t)->ptm_addr[id])+(x)))

#define ptm_lock(t, id) do { ptm_writel((t), CSMR_LOCKACCESS, 0, (id)); } while (0)
#define ptm_unlock(t, id) \
	do { ptm_writel((t), CSMR_LOCKACCESS, UNLOCK_MAGIC, (id)); } while (0)

/* ETB registers, "CoreSight Components TRM", 9.3 */
#define ETBR_DEPTH		0x04
#define ETBR_STATUS		0x0c
#define ETBR_READMEM		0x10
#define ETBR_READADDR		0x14
#define ETBR_WRITEADDR		0x18
#define ETBR_TRIGGERCOUNT	0x1c
#define ETBR_CTRL		    0x20
#define ETBR_FORMATTERCTRL	0x304
#define ETBFF_ENFTC		1
#define ETBFF_ENFCONT		(1 << 1)
#define ETBFF_FONFLIN		(1 << 4)
#define ETBFF_MANUAL_FLUSH	(1 << 6)
#define ETBFF_TRIGIN		(1 << 8)
#define ETBFF_TRIGEVT		(1 << 9)
#define ETBFF_TRIGFL		(1 << 10)

#define etb_writel(t, x, v) \
	(REG32(((t)->etb_addr)+(x)) = (v))
#define etb_readl(t, x) (REG32(((t)->etb_addr)+(x)))

#define etb_lock(t) do { etb_writel((t), CSMR_LOCKACCESS, 0); } while (0)
#define etb_unlock(t) \
	do { etb_writel((t), CSMR_LOCKACCESS, UNLOCK_MAGIC); } while (0)

/*funnel registers*/
#define FUNNEL_CTRL   0x000
#define FUNNEL_PRI    0x004

#define funnel_writel(t, x, v) \
	(REG32(((t)->funnel_addr)+(x)) = (v))
#define funnel_readl(t, x) (REG32(((t)->funnel_addr)+(x)))

#define funnel_lock(t) do { funnel_writel((t), CSMR_LOCKACCESS, 0); } while (0)
#define funnel_unlock(t) \
	do { funnel_writel((t), CSMR_LOCKACCESS, UNLOCK_MAGIC); } while (0)


#ifndef min
#define min(x,y)	(((x) < (y) ) ? (x) : (y))
#endif

PPtmConfig trace_getptmcfg(UINT8 PtmID);
UINT32 trace_init(void);
UINT32 trace_configure(UINT8 PtmID, PtmConfig *pPtmCfg);
UINT32 trace_enable(UINT8 PtmID, UINT8 enable);
UINT32 trace_access(char *data, UINT32 len);
UINT32 trace_debug(UINT8 Func, UINT8 PtmID, void *Para);
UINT32 trace_getbufsz(void);


/* something for debug */
#define DEBUG_FUNC_INIT   1
#define DEBUG_FUNC_CFG    2
#define DEBUG_FUNC_TRACE  3
#define DEBUG_FUNC_ACCESS 4
#define DEBUG_FUNC_DUMP   5
#define DEBUG_FUNC_REGRD  6
#define DEBUG_FUNC_REGWR  7

#define DEBUG_REGTYPE_PTM    0
#define DEBUG_REGTYPE_FUNNEL 1
#define DEBUG_REGTYPE_ETB    2

typedef struct stRegInfo
{
    UINT8  ucRegType;
	UINT8  ucResv[1];
	UINT16 ucOffset;
	UINT32 ulValue;
}RegInfo;

typedef struct stDataInfo
{
    void   *addr;
	UINT32 len;
}DataInfo;


#endif /*CORESIGHT_H_*/

#ifdef __cplusplus
}
#endif

