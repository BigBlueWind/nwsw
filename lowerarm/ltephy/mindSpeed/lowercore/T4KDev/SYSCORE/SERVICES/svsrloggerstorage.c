#include "svsrloggerstorage.h"
#include "supervisor.h"
#include "heaps.h"
#include <string.h>
#include "appprintf.h"
#include "mdmadrv.h"
#include "svsrloggerstorage.h"
#include "svsrlogger.h"

// Counters for MDMA copy processing
volatile UINT32 LoggerMDMAStartCount  = 0;
volatile UINT32 LoggerMDMAFinishCount = 0;

volatile STORAGE_CTX LoggerSavedCtx;
volatile STORAGE_HEADER LoggerSavedHeader;
//#define LOGDEBUG		1

UINT32 LoggerStorageGetLogVersion(pSTORAGE_CTX ctx)
{
    return (UINT32)ctx->hdr->Version;
}

pSTORAGE_CTX LoggerStorageCreate(PUINT8 *ptr, UINT32 size)
{

	pSTORAGE_CTX   	ctx;	
    STORAGE_HEADER	*pHeader;

    if(!size || size <= sizeof(STORAGE_HEADER))
        return NULL;

    if(!*ptr)
    {
        *ptr = MxHeapAlloc(DDR1Heap, size);

        if(ptr == NULL)
            return NULL;
    }

	ctx = (pSTORAGE_CTX)MxHeapAlloc(DDR1Heap, sizeof(STORAGE_CTX));
	memset(ctx, 0, sizeof(STORAGE_CTX));
    memset (*ptr, 0, size);

    // to create the log header
    pHeader = (STORAGE_HEADER*)(*ptr);

    pHeader->Version     = LOGGER_VERSION;
    pHeader->Flag        = 0;
    pHeader->MsgNum      = 0;
    pHeader->PayloadSize = 0;
    pHeader->LogStorSize = size - sizeof(STORAGE_HEADER);

	ctx->hdr = pHeader;
    return ctx;
}

MXRC LoggerStorageReset(pSTORAGE_CTX ctx, UINT32 size)
{
	if(!ctx || !ctx->hdr)
		return RC_STORAGE_ERROR;
        
    ctx->hdr->Version     = LOGGER_VERSION;
    ctx->hdr->Flag        = 0;
    ctx->hdr->MsgNum      = 0;
    ctx->hdr->PayloadSize = 0;
	ctx->hdr->StartOffset = 0;
	ctx->hdr->StartPhySize= 0;
	
	if(size)
	    ctx->hdr->LogStorSize = size - sizeof(STORAGE_HEADER);

	return RC_STORAGE_OK;
}

MXRC LoggerStorageRestoreCtx(pSTORAGE_CTX ctx)
{
    if(!ctx || !ctx->hdr)
        return RC_STORAGE_ERROR;

    memcpy((void*)ctx, (void*)&LoggerSavedCtx, sizeof(STORAGE_CTX));
    memcpy((void*)ctx->hdr, (void*)&LoggerSavedHeader, sizeof(STORAGE_HEADER));

    return RC_STORAGE_OK;
}

MXRC LoggerStorageSaveCtx(pSTORAGE_CTX ctx)
{
    if(!ctx || !ctx->hdr)
        return RC_STORAGE_ERROR;

    memcpy((void*)&LoggerSavedCtx, (void*)ctx, sizeof(STORAGE_CTX));
    memcpy((void*)&LoggerSavedHeader, (void*)ctx->hdr, sizeof(STORAGE_HEADER));

    return RC_STORAGE_OK;
}

extern UINT32 PhyDiGetCurrentTTI(void);

INLINE void LoggerStorageBuildHeader (UINT32 nMsgID, UINT32 nMacPhySrcDstID, UINT32 nLen, API_LOG_INFO * pHeader)
{
	_ASSERT_PTR (pHeader);
	
    pHeader->PayloadLen = nLen;
    pHeader->Message    = nMsgID;
    pHeader->Source     = LOGGER_GET_SRC_ID(nMacPhySrcDstID);
    pHeader->Destination= LOGGER_GET_DST_ID(nMacPhySrcDstID);
    pHeader->PHY_ID     = LOGGER_GET_PHY_ID(nMacPhySrcDstID);
    pHeader->MAC_ID     = LOGGER_GET_MAC_ID(nMacPhySrcDstID);	
    pHeader->Timestamp  = SvsrGetMsgTimestamp();
    pHeader->CurrentTTI = PhyDiGetCurrentTTI();
}

#ifdef LOGGER_RADMDMA_COPY_ENABLED
void LoggerStorageMsgCopyComplete(LPVOID p)
{
    LoggerMDMAFinishCount++;
    return;
}
#endif

MXRC LoggerStorageAddMsg(pSTORAGE_CTX ctx, UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize)
{
    PUINT8 pData;
    API_LOG_INFO * pApi;
	API_LOG_INFO * next_tti = NULL;
	API_LOG_INFO * cur_tti  = NULL;
    PUINT8 pStor;
    UINT32 msg_all_size;
	UINT32 free_space = 0;
	UINT32 need_roundup = 0;
	UINT32 need_more_space = 0;
	UINT32 tti_size = 0;
	UINT32 new_padding = 0;
	UINT32 new_tti = 0;
	UINT32 max_tti = 0;

    if (!ctx || !ctx->hdr)
    {
        uart_printf ("Log Storage: Header is NULL\r\n");
        return RC_LOGGER_HEADER_NULL;
    }	

	if(!nMsgSize)
		return RC_STORAGE_ERROR;

	pStor = (PUINT8)(((UINT32)ctx->hdr) + sizeof(STORAGE_HEADER));
	msg_all_size = sizeof (API_LOG_INFO) + ROUND(nMsgSize,4);

	if(nMsgID == PHY_INIT_REQ)
	{
		LoggerStorageReset(ctx, 0);
		ctx->hdr->StartOffset = ctx->hdr->StartPhySize = msg_all_size;
	}

	if(nMsgID == PHY_START_REQ || nMsgID == PHY_INIT_IND)
	{		
                LoggerStorageRestoreCtx(ctx);
		ctx->hdr->StartOffset += msg_all_size;
		ctx->hdr->StartPhySize += msg_all_size;
	}
		
	//if(nMsgID != PHY_RXSDU_IND && nMsgID != PHY_RXSTATUS_IND && nMsgID != PHY_TXSDU_REQ && nMsgID != PHY_RXEND_IND && ctx->SubframeNum !=  LOGGER_GET_PHY_ID(nMacPhySrcDstID))
	if(ctx->SubframeNum !=  PhyDiGetCurrentTTI())
	{
		new_tti++;
		ctx->hdr->MsgNum++;
	}

	if(ctx->TTIreset && ctx->hdr->MsgNum == ctx->TTIreset)
		max_tti++;

	if ((((msg_all_size + (ctx->hdr->PayloadSize + ctx->PaddingSize)) > ctx->hdr->LogStorSize) && (ctx->hdr->StartOffset > ctx->hdr->StartPhySize)) || max_tti)
		need_more_space++;				

	if(((ctx->LastOffset + msg_all_size) > ctx->hdr->LogStorSize) || (max_tti && (ctx->hdr->StartOffset < ctx->LastOffset)) )
	{
		need_roundup++;
		new_padding = ctx->hdr->LogStorSize - ctx->LastOffset;
		ctx->curApi->roundup++; 
		if((!need_more_space && (ctx->hdr->StartOffset - ctx->hdr->StartPhySize) < msg_all_size) )
			need_more_space++;				
	}

	if(need_more_space)
	{	
		cur_tti = (API_LOG_INFO *)(pStor + ctx->hdr->StartOffset);
		next_tti = ((API_LOG_INFO *)cur_tti)->nextTTI;
		if(need_roundup)						
			ctx->LastOffset = ctx->hdr->StartPhySize;		
		else if(!max_tti)				
			free_space =  ctx->hdr->StartOffset - ctx->LastOffset;						
				
		while(free_space < msg_all_size)
		{
			if(!next_tti)
			{
				ctx->hdr->PayloadSize = ctx->hdr->StartOffset = ctx->LastOffset = ctx->hdr->StartPhySize;
				ctx->PaddingSize = 0;
				ctx->hdr->MsgNum--;
				break;			
			}

			if((PUINT8)next_tti > (PUINT8)cur_tti)
			{
				tti_size = (PUINT8)next_tti - (PUINT8)cur_tti;
#ifdef LOGDEBUG
				if(!tti_size)
					abort();
#endif
				free_space += tti_size;
				ctx->hdr->StartOffset += tti_size;
				cur_tti = next_tti;		
#ifdef LOGDEBUG
				if(ctx->hdr->StartOffset >= ctx->hdr->LogStorSize)
					abort();
				if((PUINT8)(((API_LOG_INFO *)(pStor + ctx->hdr->StartOffset))->nextTTI) > pStor + ctx->hdr->LogStorSize)
					abort();
#endif
				if(!cur_tti && free_space < msg_all_size)
					return RC_STORAGE_OVERFLOW;
				next_tti = cur_tti->nextTTI;				
				ctx->hdr->PayloadSize -= tti_size;
				ctx->hdr->MsgNum--;
			}
			else
			{						
				tti_size = (PUINT8)next_tti - (pStor + ctx->hdr->StartPhySize) + (ctx->hdr->LogStorSize - ((PUINT8)cur_tti - pStor)) - ctx->PaddingSize;	
#ifdef LOGDEBUG
				if(!tti_size)
					abort();
#endif
				free_space += tti_size;
				if(need_roundup)											
					if(free_space >= new_padding)
						free_space -= new_padding;
					else
						free_space = 0;
								
				ctx->hdr->StartOffset = (PUINT8)next_tti - pStor;				
				cur_tti = next_tti;
#ifdef LOGDEBUG
				if(ctx->hdr->StartOffset >= ctx->hdr->LogStorSize)
					abort();
				if((PUINT8)(((API_LOG_INFO *)(pStor + ctx->hdr->StartOffset))->nextTTI) > pStor + ctx->hdr->LogStorSize)
					abort();			
#endif
				if(!cur_tti && free_space < msg_all_size)
					return RC_STORAGE_OVERFLOW;
				next_tti = cur_tti->nextTTI;
				ctx->hdr->PayloadSize -= tti_size;
				ctx->hdr->MsgNum--;				
			}
		}		
	}
	else if(need_roundup)	
		ctx->LastOffset = ctx->hdr->StartPhySize; // skip PHY START, it always first;		

	if(ctx->curApi)
		ctx->curApi->end = 0;
    pApi    = (API_LOG_INFO *)(pStor + ctx->LastOffset);
	ctx->curApi = pApi;	
	pApi->end = 1;
	pApi->nextTTI = 0;
	pApi->roundup = 0;
    pData   = (PUINT8)(((UINT32)pApi) + sizeof(API_LOG_INFO));
	
	if(need_roundup)
		ctx->PaddingSize = new_padding;

    LoggerStorageBuildHeader(nMsgID, nMacPhySrcDstID, nMsgSize, pApi);

	if(!ctx->curTTIstart)
	{
			ctx->curTTIstart = pApi;					
			ctx->SubframeNum = PhyDiGetCurrentTTI();
	}
	else if(new_tti)
	{	
			ctx->curTTIstart->nextTTI = pApi;
			ctx->curTTIstart = pApi;		
			ctx->SubframeNum = PhyDiGetCurrentTTI();
	}
	
#ifdef LOGGER_RADMDMA_COPY_ENABLED
    if ((MdmaIsInited(MDMA_HW_RAD)) && (nMsgSize > LOGGER_MEMCOPY_VS_MDMA_LIMIT_SIZE))
    {
        MDMA_FDESC *pMdma = MdmaAllocDesc(MDMA_HW_RAD, 1);

        _ASSERT_PTR(pMdma);
        MdmaSetCopy(pMdma);
        MdmaSetData(pMdma, pMsg, nMsgSize, pData, nMsgSize);
        MdmaRunEx(MDMA_HW_RAD, pMdma, LoggerStorageMsgCopyComplete, NULL);
        LoggerMDMAStartCount++;
    }
    else
#endif
    {
        memcpy (pData, pMsg, nMsgSize);
    }
#ifdef LOGDEBUG
	if(ctx->LastOffset < ctx->hdr->StartOffset && ctx->LastOffset + msg_all_size > ctx->hdr->StartOffset)
		abort();
#endif
	ctx->LastOffset += msg_all_size;
    ctx->hdr->PayloadSize += msg_all_size;
#ifdef LOGDEBUG
	if(ctx->hdr->PayloadSize > ctx->hdr->LogStorSize)
		abort();
	if(ctx->hdr->StartOffset > ctx->hdr->StartPhySize && (API_LOG_INFO *)(pStor + ctx->hdr->StartOffset) == ctx->curApi)
		abort();
#endif

    if(nMsgID == PHY_INIT_REQ || nMsgID == PHY_INIT_IND)
        LoggerStorageSaveCtx(ctx);

    return RC_STORAGE_OK ;
}

API_LOG_INFO * LoggerStorageGetMsg (pSTORAGE_CTX ctx, UINT32 nMsgIndex)
{
    UINT32 i = 0;
    UINT32 nMsgPos = 0;
    API_LOG_INFO * pApi = NULL;

	if(!ctx || !ctx->hdr)
		return NULL;

    while ((pApi = LoggerStorageGetNextMsg(ctx, &nMsgPos)) != NULL)
    {
		if(!pApi)
			return NULL;

        if (i == nMsgIndex)
            return pApi;
        i++;
    }

    return pApi;
}

API_LOG_INFO * LoggerStorageGetNextMsgNew (pSTORAGE_CTX ctx, UINT32 * pnMsg)
{
     API_LOG_INFO * pHdr;
	PUINT8 pStor = NULL;
	static int fr = 1;
		
	if(!ctx || !ctx->hdr || *pnMsg == 0xFFFFFFFF || !ctx->hdr->PayloadSize)
		return NULL;

    pStor = ((PUINT8)(ctx->hdr)) + sizeof(STORAGE_HEADER);

    if (pStor == NULL)
    {
        uart_printf ("LOG: Storage is NULL (error)\r\n");
        return NULL;
    }
	if(fr && !ctx->hdr->StartPhySize)
	{
		*pnMsg = ctx->hdr->StartOffset;
		fr = 0;
	}
	
	pHdr = (API_LOG_INFO*)(((UINT32)pStor) + *pnMsg);

	if(!pHdr->Message || !pHdr->PayloadLen) 
		return NULL;
	

	if(!pHdr->roundup)
	{
		(*pnMsg) += (sizeof (API_LOG_INFO) + ROUND(pHdr->PayloadLen, 4));
		if(*pnMsg == ctx->hdr->StartPhySize)
			*pnMsg = ctx->hdr->StartOffset;
	}	
	else
		(*pnMsg) = ctx->hdr->StartPhySize;

	if(pHdr->end)
		(*pnMsg) = 0xFFFFFFFF;
    
	return pHdr;
}


API_LOG_INFO * LoggerStorageGetNextMsgOld (pSTORAGE_CTX ctx, UINT32 * pnMsg)
{
    API_LOG_INFO * pHdr;
	PUINT8 pStor = NULL;

    pStor = ((PUINT8)(ctx->hdr)) + sizeof(STORAGE_HEADER);

    if (pStor == NULL)
    {
        uart_printf ("LOG: Storage is NULL (error)\r\n");
        return NULL;
    }

    pHdr = (API_LOG_INFO*)(((UINT32)pStor) + *pnMsg);

    if ((UINT32)pHdr >= (UINT32)(pStor + ctx->hdr->PayloadSize))
        return NULL;

    (*pnMsg) += (sizeof (API_LOG_INFO) + ROUND(pHdr->PayloadLen, 4));

     return pHdr;
}


API_LOG_INFO * LoggerStorageGetNextMsg(pSTORAGE_CTX ctx, UINT32 * pnMsg)
{
	if(!ctx || !ctx->hdr)
			return NULL;

	if(ctx->hdr->Version == LOGGER_VERSION)
		return LoggerStorageGetNextMsgNew(ctx, pnMsg);
	else if(ctx->hdr->Version < LOGGER_VERSION)
		return LoggerStorageGetNextMsgOld(ctx, pnMsg);

	return NULL;
}


