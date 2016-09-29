//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL ptmdrv.c
 *
 * @brief This file contains ptm driver "C" code
 * @author Mindspeed Technologies
 * @version $Revision: 1.1 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------
#ifdef PTM_ENABLED
#include "coresight.h"

static TraceCtx    stTracer;
static PtmConfig   astPtmCfg[MAX_PTM];


UINT32 Ptm_Init(TraceCtx  *t)
{
    int  i;
   
    memset(t, 0, sizeof(TraceCtx));
	
	t->etb_addr    = BASEADDR_ETB_REG;
	t->funnel_addr = BASEADDR_FUNNEL_REG;

	t->ptm_addr[0] = BASEADDR_PTM_REG0;
	t->ptm_addr[1] = BASEADDR_PTM_REG1;
	t->ptm_addr[2] = BASEADDR_PTM_REG2;
	t->ptm_addr[3] = BASEADDR_PTM_REG3;
	t->ptm_addr[4] = BASEADDR_PTM_REG4;
	t->ptm_addr[5] = BASEADDR_PTM_REG5;

	t->flags       = TRACER_INIT;

	etb_unlock(t);
	t->etb_bufsz = etb_readl(t, ETBR_DEPTH);
	
	/* make sure trace capture is disabled */
	etb_writel(t, ETBR_CTRL, 0);
	etb_writel(t, ETBR_FORMATTERCTRL, 0x1000);
	etb_lock(t);

    for(i=0; i < MAX_PTM; i++)
    {
	    ptm_unlock(t, i);
	    (void)ptm_readl(t, ETMMR_PDSR, i);
	  
	     /* dummy first read */
	    (void)ptm_readl(t, ETMMR_OSSRR, i);
	    ptm_writel(t, ETMCR, 0x400, i);

	    ptm_lock(t, i);
    }

	return ERRCODE_OK;

}

PPtmConfig Trace_GetPtmCfg(UINT8 PtmID)
{
    PtmConfig *p;
	
    if(PtmID >= MAX_PTM)
		return NULL;

	p = &astPtmCfg[PtmID];

	return p;
}
 
UINT32 PtmConfigDefaultValue(UINT8 PtmID)
{
    int  i;
	PtmConfig *p;
	
    if(PtmID >= MAX_PTM)
		return ERRCODE_PTM;

	p = &astPtmCfg[PtmID];
	
    p->PowerDown            = 0;
	p->BranchOutput         = 1;
	p->CycleAccurate        = 0;
	p->ContextIDSize        = 0;
	p->TimestampEnable      = 0;
	p->ReturnStackEnable    = 0;
	p->ForceSyncPacket      = 0;
	p->SeqState             = 0;
	p->SeqTranEvt1To2       = 0;
	p->SeqTranEvt1To3       = 0;
	p->SeqTranEvt2To1       = 0;
	p->SeqTranEvt2To3       = 0;
	p->SeqTranEvt3To1       = 0;
	p->SeqTranEvt3To2       = 0;
	p->TimestampEvent       = 0;
	p->SyncFreq             = 0;
	p->ContextIDCompVal     = 0;
	p->ContextIDCompMask    = 0;
	p->TraceEnableStart     = 0;
	p->TraceEnableStop      = 0;
	p->TraceEnableActive    = 0;
	p->TraceRangeExclude    = 1;
	p->TraceRange           = 0;
	p->TraceEnableEvt       = 0x6F;
	
	for(i=0; i<8; i++)
	{
		p->AddrCompVal[i] = 0;
	}

	for(i=0; i<2; i++)
	{
		p->CounterVal[i]        = 0;
		p->CounterEnable[i]     = 0;
		p->CounterReloadVal[i]  = 0;
		p->CounterReloadEvt[i]  = 0;
	}

	return ERRCODE_OK;
}

UINT32 PtmConfigValue(UINT8 PtmID,  PtmConfig *pPtmCfg)
{
    UINT32 i;
	PtmConfig *p;

	
    if(PtmID >= MAX_PTM)
		return ERRCODE_PTM;

	p = &astPtmCfg[PtmID];

	p->PowerDown            = pPtmCfg->PowerDown;
	p->BranchOutput         = pPtmCfg->BranchOutput;
	p->CycleAccurate        = pPtmCfg->CycleAccurate;
	p->ContextIDSize        = pPtmCfg->ContextIDSize;
	p->TimestampEnable      = pPtmCfg->TimestampEnable;
	p->ReturnStackEnable    = pPtmCfg->ReturnStackEnable;
	p->ForceSyncPacket      = pPtmCfg->ForceSyncPacket;
	p->SeqState             = pPtmCfg->SeqState;
	p->SeqTranEvt1To2       = pPtmCfg->SeqTranEvt1To2;
	p->SeqTranEvt1To3       = pPtmCfg->SeqTranEvt1To3;
	p->SeqTranEvt2To1       = pPtmCfg->SeqTranEvt2To1;
	p->SeqTranEvt2To3       = pPtmCfg->SeqTranEvt2To3;
	p->SeqTranEvt3To1       = pPtmCfg->SeqTranEvt3To1;
	p->SeqTranEvt3To2       = pPtmCfg->SeqTranEvt3To2;
	p->TimestampEvent       = pPtmCfg->TimestampEvent;
	p->SyncFreq             = pPtmCfg->SyncFreq;
	p->ContextIDCompVal     = pPtmCfg->ContextIDCompVal;
	p->ContextIDCompMask    = pPtmCfg->ContextIDCompMask;
	p->TraceEnableStart     = pPtmCfg->TraceEnableStart;
	p->TraceEnableStop      = pPtmCfg->TraceEnableStop;
	p->TraceEnableActive    = pPtmCfg->TraceEnableActive;
	p->TraceRangeExclude    = pPtmCfg->TraceRangeExclude;
	p->TraceRange           = pPtmCfg->TraceRange;
	p->TraceEnableEvt       = pPtmCfg->TraceEnableEvt;
	
	for(i=0; i<8; i++)
	{
		p->AddrCompVal[i] = pPtmCfg->AddrCompVal[i];
	}

	for(i=0; i<2; i++)
	{
		p->CounterVal[i]        = pPtmCfg->CounterVal[i];
		p->CounterEnable[i]     = pPtmCfg->CounterEnable[i];
		p->CounterReloadVal[i]  = pPtmCfg->CounterReloadVal[i];
		p->CounterReloadEvt[i]  = pPtmCfg->CounterReloadEvt[i];
	}

	return ERRCODE_OK;
}

static BOOL Trace_IsInit(TraceCtx *t)
{
	return !!(t->flags & TRACER_INIT);
}

static BOOL Trace_Isrunning(TraceCtx *t, UINT8 PtmID)
{
	return !!(t->flags & TRACER_RUNNING(PtmID));
}

static BOOL Running_Trace_Exist(TraceCtx *t)
{
	return !!(t->flags & TRACER_MASK);
}

static UINT32 Trace_Start(TraceCtx  *t, UINT8 PtmID)
{
	UINT32 timeout = TRACER_TIMEOUT;
	UINT32     v;
	PtmConfig  *p;
	UINT8      i;
	
	if(!Running_Trace_Exist(t))
	{
	    etb_unlock(t);
		
	    etb_writel(t,ETBR_FORMATTERCTRL, 0);
	    etb_writel(t,ETBR_CTRL, 1);
	
	    etb_lock(t);
	}


	/* enable slave port x */
	funnel_unlock(t);

	v = funnel_readl(t, FUNNEL_CTRL);
	v |= (1 << PtmID);
    funnel_writel(t, FUNNEL_CTRL, v);
		
    funnel_lock(t);

	if(PtmID < MAX_PTM)
	{
	    p = &astPtmCfg[PtmID];
	}

	/* configure ptm */
	v = ETMCR_PROGRAM |(p->PowerDown) | (p->BranchOutput << 8) | (p->CycleAccurate << 12) | \
	    (p->ContextIDSize << 14) | (p->TimestampEnable << 28) | (p->ReturnStackEnable << 29);
	ptm_unlock(t, PtmID);
	
	ptm_writel(t, ETMCR, v, PtmID);

	while (!(ptm_readl(t, ETMSR, PtmID) & 0x2) && --timeout)
		;
	if (!timeout) {
		uart_printf ("Waiting for progbit to assert timed out\r\n");
		ptm_lock(t, PtmID);
		return ERRCODE_TIMEOUT;
	}

	v = ptm_readl(t, ETMAUXCR, PtmID);
	v = v | (p->ForceSyncPacket << 3);
	ptm_writel(t, ETMAUXCR, v, PtmID);

	v = p->SeqState;
	ptm_writel(t, ETMSQR, v, PtmID);

	v = p->SeqTranEvt1To2;
	ptm_writel(t, ETMSQabEVR, v, PtmID);

	v = p->SeqTranEvt2To1;
    ptm_writel(t, (ETMSQabEVR + 0x4), v, PtmID);
	
    v = p->SeqTranEvt2To3;
    ptm_writel(t, (ETMSQabEVR + 0x8), v, PtmID);

    v = p->SeqTranEvt3To1;
    ptm_writel(t, (ETMSQabEVR + 0xc), v, PtmID);

	v = p->SeqTranEvt3To2;
    ptm_writel(t, (ETMSQabEVR + 0x10), v, PtmID);

	v = p->SeqTranEvt1To3;
    ptm_writel(t, (ETMSQabEVR + 0x14), v, PtmID);

	v = p->SyncFreq;
	ptm_writel(t, ETMSYNCFR, v, PtmID);

    for(i=0; i<8; i++)
    {
	    v = p->AddrCompVal[i];
		ptm_writel(t, (ETMACVRn+4*i), v, PtmID);
    }

	v = p->ContextIDCompVal;
	ptm_writel(t, ETMCIDCVR, v, PtmID);

	v = p->ContextIDCompMask;
	ptm_writel(t, ETMCIDCMR, v, PtmID);

	for(i=0; i<2; i++)
	{
	    v = p->CounterVal[i];
		ptm_writel(t, (ETMCNTVRn+4*i), v, PtmID);

		v = p->CounterEnable[i];
		ptm_writel(t, (ETMCNTENRn+4*i), v, PtmID);

		v = p->CounterReloadVal[i];
		ptm_writel(t, (ETMCNTRLDVRn+4*i), v, PtmID);

		v = p->CounterReloadEvt[i];
		ptm_writel(t, (ETMCNTRLDEVRn+4*i), v, PtmID);
	}

	v = p->TraceEnableStart | (p->TraceEnableStop << 16);
	ptm_writel(t, ETMTSSCR, v, PtmID);

	v = p->TraceRange | (p->TraceRangeExclude << 24) | (p->TraceEnableActive << 25);
	ptm_writel(t, ETMTECR1, v, PtmID);

	v = p->TraceEnableEvt;
	ptm_writel(t, ETMTEEVR, v, PtmID);

    v = 0x50 + PtmID;
	ptm_writel(t, ETMTRACEIDR, v, PtmID);

	v = ptm_readl(t, ETMCR, PtmID);
    v &= ~ETMCR_PROGRAM;
	ptm_writel(t, ETMCR, v, PtmID);

	timeout = TRACER_TIMEOUT;
	while ((ptm_readl(t, ETMSR, PtmID) & 0x2) && --timeout)
		;
	if (!timeout) {
		uart_printf("Waiting for progbit to deassert timed out\n");
		ptm_lock(t, PtmID);
		return ERRCODE_TIMEOUT;
	}

	ptm_lock(t, PtmID);

	t->flags |= TRACER_RUNNING(PtmID);

	return ERRCODE_OK;
}

static UINT32 Trace_Stop(TraceCtx  *t, UINT8 PtmID)
{
	UINT32 timeout = TRACER_TIMEOUT;
	UINT32 v;
	
	ptm_unlock(t, PtmID);
	ptm_writel(t, ETMCR, 0x400, PtmID);
	while (!(ptm_readl(t, ETMSR, PtmID) & 2) && --timeout)
		;
	if (!timeout) {
		uart_printf("Waiting for progbit to assert timed out\n");
		ptm_lock(t, PtmID);
		return ERRCODE_TIMEOUT;
	}
	ptm_lock(t, PtmID);

	funnel_unlock(t);
	v = funnel_readl(t, FUNNEL_CTRL);
	v &= ~(1 << PtmID);
    funnel_writel(t, FUNNEL_CTRL, v);	
    funnel_lock(t);

	t->flags &= ~(TRACER_RUNNING(PtmID));

    if(!Running_Trace_Exist(t))
    {
    	etb_unlock(t);
    	etb_writel(t, ETBR_FORMATTERCTRL, ETBFF_MANUAL_FLUSH);
    	timeout = TRACER_TIMEOUT;
    	while (etb_readl(t, ETBR_FORMATTERCTRL) &
    			ETBFF_MANUAL_FLUSH && --timeout)
    		;
    	if (!timeout) {
    		uart_printf("Waiting for formatter flush to commence "
    				"timed out\n");
    		etb_lock(t);
    		return ERRCODE_TIMEOUT;
    	}
        etb_writel(t, ETBR_CTRL, 0);
    	etb_lock(t);
    }

	
	return ERRCODE_OK;
}

static UINT32 Etb_GetDataLen(TraceCtx  *t)
{
	UINT32 v;
	UINT32 rp, wp;

	v = etb_readl(t, ETBR_STATUS);
	if(v & 1)
		return t->etb_bufsz;

	rp = etb_readl(t, ETBR_READADDR);
	wp = etb_readl(t, ETBR_WRITEADDR);

	if (rp > wp) {
		etb_writel(t, ETBR_READADDR, 0);
		etb_writel(t, ETBR_WRITEADDR, 0);

		return 0;
	}

	return wp - rp;
}

static UINT32 Etb_Dump(void)
{
	TraceCtx  *t = &stTracer;
	UINT32 first = 0;
	UINT32 length;
	UINT32 i=0;

	if (Running_Trace_Exist(t))
	{
	    for(i=0; i< MAX_PTM; i++)
	    {
	        if(Trace_Isrunning(t, i))
	        {
		        Trace_Stop(t, i);
	        }
	    }
	}

	etb_unlock(t);

	length = Etb_GetDataLen(t);

	if (length == t->etb_bufsz)
		first = etb_readl(t, ETBR_WRITEADDR);

	etb_writel(t, ETBR_READADDR, first);

	uart_printf("Trace buffer contents length: %d\n", length);
	uart_printf("--- ETB buffer begin ---\n");
	for (; length; length--)
		uart_printf("%d: %08x \r\n", i++, etb_readl(t, ETBR_READMEM));
	uart_printf("\n--- ETB buffer end ---\n");

	/* deassert the overflow bit */
	etb_writel(t, ETBR_CTRL, 1);
	etb_writel(t, ETBR_CTRL, 0);

	etb_writel(t, ETBR_TRIGGERCOUNT, 0);
	etb_writel(t, ETBR_READADDR, 0);
	etb_writel(t, ETBR_WRITEADDR, 0);

	etb_lock(t);

	return ERRCODE_OK;
}

static UINT32 Etb_Read(TraceCtx  *t, char *data, UINT32 len)
{
    UINT32  total, i;
	UINT32  length;
	UINT32  first = 0;
	UINT32  *buf = NULL;

	if (Running_Trace_Exist(t))
	{
	    for(i=0; i< MAX_PTM; i++)
	    {
	        if(Trace_Isrunning(t, i))
	        {
		        Trace_Stop(t, i);
	        }
	    }
	}

	
    etb_unlock(t);

	total = Etb_GetDataLen(t);
	if (total == t->etb_bufsz)
		first = etb_readl(t, ETBR_WRITEADDR);

	etb_writel(t, ETBR_READADDR, first);

	length = min(total * 4, (int)len);

	buf = MxHeapAlloc(NcnbHeap, length);
	if(buf == NULL)
		return ERRCODE_NOMEM;
		
	for (i = 0; i < length / 4; i++)
	{
		buf[i] = etb_readl(t, ETBR_READMEM);
		//uart_printf("%d: %08x \r\n", i++, buf[i]);
	}

	/* the only way to deassert overflow bit in ETB status is this */
	/* deassert the overflow bit */
	etb_writel(t, ETBR_CTRL, 1);
	etb_writel(t, ETBR_CTRL, 0);

	etb_writel(t, ETBR_TRIGGERCOUNT, 0);
	etb_writel(t, ETBR_READADDR, 0);
	etb_writel(t, ETBR_WRITEADDR, 0);

	etb_lock(t);

	memcpy(data, buf, length);
	MxHeapFree(NcnbHeap, buf);

	return ERRCODE_OK;
}

PPtmConfig trace_getptmcfg(UINT8 PtmID)
{
    PtmConfig *p;
	
    if(PtmID >= MAX_PTM)
		return NULL;

	p = &astPtmCfg[PtmID];

	return p;
}

UINT32 trace_init(void)
{
    TraceCtx  *t = &stTracer;
	PtmConfig *p = astPtmCfg;
	UINT8 i;

	
	Ptm_Init(t);
	
	memset(p, 0, MAX_PTM * sizeof(PtmConfig));
	for(i=0; i<MAX_PTM; i++)
	{
	    PtmConfigDefaultValue(i);
	}
	
	return ERRCODE_OK;
}

UINT32 trace_configure(UINT8 PtmID, PtmConfig *pPtmCfg)
{
    UINT32 result;

	result = PtmConfigValue(PtmID, pPtmCfg);

	return result;
}

UINT32 trace_enable(UINT8 PtmID, UINT8 enable)
{
    TraceCtx  *t= &stTracer;

    if(!Trace_IsInit(t))
		return ERRCODE_INIT;
	
	if(enable)
	{
	    Trace_Start(t, PtmID);
	}
	else
	{
	    Trace_Stop(t, PtmID);
	}

	return ERRCODE_OK;
}

UINT32 trace_access(char *data, UINT32 len)
{
    TraceCtx  *t= &stTracer;
	UINT32 result;
	
    result = Etb_Read(t, data, len);

	return result;
}

UINT32 trace_getbufsz(void)
{
    TraceCtx  *t= &stTracer;

	return (4 * t->etb_bufsz);
}


UINT32 Reg_Read(UINT8 Type, UINT8 PtmID, UINT16 Offset)
{
     TraceCtx  *t= &stTracer;
	 UINT32 v;

	 if(!Trace_IsInit(t))
		return ERRCODE_INIT;
	 
    if(Type == DEBUG_REGTYPE_PTM)
    {
		ptm_unlock(t, PtmID);
		v = ptm_readl(t, Offset, PtmID);
        uart_printf("read ptm %d register offset 0x%x = 0x%x \r\n",PtmID, Offset, v);
		ptm_lock(t, PtmID);
    }
    else if(Type == DEBUG_REGTYPE_FUNNEL)
	{
		funnel_unlock(t);
		v = funnel_readl(t, Offset);
        uart_printf("read funnel register offset 0x%x = 0x%x \r\n", Offset, v);
		funnel_lock(t);
	}
	else if(Type == DEBUG_REGTYPE_ETB)
	{
		etb_unlock(t);
		v = etb_readl(t, Offset);
        uart_printf("read etb register offset 0x%x = 0x%x \r\n", Offset, v);
		etb_lock(t);
	}

	return ERRCODE_OK;
}

UINT32 Reg_Write(UINT8 Type, UINT8 PtmID, UINT16 Offset, UINT32 Value)
{
     TraceCtx  *t= &stTracer;

	 if(!Trace_IsInit(t))
		return ERRCODE_INIT;
	 
    if(Type == DEBUG_REGTYPE_PTM)
    {
		ptm_unlock(t, PtmID);
		ptm_writel(t, Offset, Value, PtmID);
        uart_printf("write ptm %d register offset 0x%x = 0x%x \r\n",PtmID, Offset, Value);
		ptm_lock(t, PtmID);
    }
    else if(Type == DEBUG_REGTYPE_FUNNEL)
	{
		funnel_unlock(t);
		funnel_writel(t, Offset, Value);
        uart_printf("write funnel register offset 0x%x = 0x%x \r\n", Offset, Value);
		funnel_lock(t);
	}
	else if(Type == DEBUG_REGTYPE_ETB)
	{
		etb_unlock(t);
		etb_writel(t, Offset, Value);
        uart_printf("write etb register offset 0x%x = 0x%x \r\n", Offset, Value);
		etb_lock(t);
	}

	return ERRCODE_OK;
}

UINT32 trace_debug(UINT8 Func, UINT8 PtmID, void *Para)
{
    PtmConfig  *pCfg;
	RegInfo    *pRegInfo;
	DataInfo   *pDataInfo;
	UINT8      enable;
	UINT32     result;

	if(Func == DEBUG_FUNC_INIT)
	{
	    result = trace_init();
	}
	else if(Func == DEBUG_FUNC_CFG)
	{
	    pCfg = (PtmConfig*) Para;
	    result = trace_configure(PtmID, pCfg);
	}
	else if(Func == DEBUG_FUNC_TRACE)
	{
	    enable = *((UINT32 *) Para);
		result = trace_enable(PtmID, enable);
	}
	else if(Func == DEBUG_FUNC_ACCESS)
	{
	    pDataInfo = (DataInfo*)Para;

		result = trace_access((char *)pDataInfo->addr, pDataInfo->len);
	}
	else if(Func == DEBUG_FUNC_DUMP)
	{
	    result = Etb_Dump();
	}
	else if(Func == DEBUG_FUNC_REGRD)
	{
	    pRegInfo = (RegInfo *)Para;
		result = Reg_Read(pRegInfo->ucRegType, PtmID, pRegInfo->ucOffset);
		
	}
	else if(Func == DEBUG_FUNC_REGWR)
	{
	    pRegInfo = (RegInfo *)Para;
		result = Reg_Write(pRegInfo->ucRegType, PtmID, pRegInfo->ucOffset, pRegInfo->ulValue);
	}
		
	return result;
    
}
#endif
