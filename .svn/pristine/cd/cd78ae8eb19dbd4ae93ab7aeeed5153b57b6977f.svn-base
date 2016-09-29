//-------------------------------------------------------------------------------------------
/** @file svsrlogger.c
 *
 * @brief The definition of supervisor logger API
 * @author Mindspeed Technologies
 * @version $Revision: 1.55 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include <string.h>
#include "svsrlogger.h"
#include "heaps.h"
#include "hal.h"
#include "supervisor.h"
#include "appids.h"
#include "appprintf.h"
#include "mlog.h"
#include "globalstoragearrays.h"
#include "svsrloggerstorage.h"

APILOGGERCTX  logger_ctx;
SVSRLOG_TRACE_BUF_ELEM* gSvsrLogTraceBuffer = NULL;
VUINT32 gSvsrLogTraceBufferIdx;
VUINT32 gSvsrLogTraceBufferNum;

INLINE APILOGGERCTX * SvsrLoggerGetCtx (void)
{
    return &logger_ctx;
}

INLINE STORAGE_HEADER * SvsrLogHeader (void)
{
    return (STORAGE_HEADER *)logger_ctx.StoragePtr->hdr;
}

INLINE PUINT8 SvsrLogStorage (void)
{
    if (logger_ctx.StoragePtr == NULL)
        return NULL;

    return ((PUINT8)logger_ctx.StoragePtr->hdr) + sizeof(STORAGE_HEADER);
}

extern volatile UINT32 LoggerMDMAStartCount;
extern volatile UINT32 LoggerMDMAFinishCount;

BOOL SvsrLogIsCopyCompleted (void)
{
    return (LoggerMDMAStartCount == LoggerMDMAFinishCount);
}

void SvsrLoggerLock (APILOGGERCTX * pCtx)
{
    pCtx->Irq[MxGetCpuID ()] = ARM_INT_disable ();
    MxGetLock (&pCtx->Lock);
}

void SvsrLoggerUnlock (APILOGGERCTX * pCtx)
{
    MxReleaseLock (&pCtx->Lock);
    ARM_INT_restore (pCtx->Irq[MxGetCpuID ()]);
}

APIHEC SvsrLoggerApiHandler (LPVOID pCtx, ApiHeader * pApi, ApiHeader * pResp)
{
    UINT32 rc = MX_OK;

    if (IS_CFG_API (pApi, SVSR_LOGGER_CFG))
    {
        ApiParam * pParam     = ApiGetParamByID (pApi, 2);
        ApiParam * pParamMLog = ApiGetParamByID (pApi, 3);

        //uart_printf("SvsrLoggerApiHandler: SVSR_LOGGER_CFG \r\n");

        if (pParam != NULL)
            SvsrLoggerSetMask (pParam->ParamData [0]);

        if (pParamMLog != NULL)
            MLogSetMaskProg(pParamMLog->ParamData [0]);

        if (pParam == NULL && pParamMLog == NULL)
        {
            //uart_printf("SvsrLoggerApiHandler: Error! \r\n");
            rc = RC_LOGGER_API_ERROR;
        }
    }
    else if (IS_CFG_API (pApi, SVSR_LOGGER_RESET))
    {
		uart_printf("\r\nSvsrLoggerApiHandler - SVSR_LOGGER_RESET \r\n");
		SvsrLoggerResetStorage(1);
    }
    else if (IS_QRY_API(pApi, SVSR_LOGGER_GET_INFO))
    {
        STORAGE_HEADER * pHead = SvsrLogHeader ();

        if (pHead != NULL)
        {
            UINT32 DataSize = pHead->PayloadSize + sizeof(STORAGE_HEADER);
            // the pointer to the storage
            ApiAddParam (pResp, NULL, 2, (LPVOID)&logger_ctx.StoragePtr->hdr, sizeof (logger_ctx.StoragePtr->hdr));
            // the size of data located in the storage
            ApiAddParam (pResp, NULL, 3, (LPVOID)&DataSize, sizeof (DataSize));
            // flags
            ApiAddParam (pResp, NULL, 4, (LPVOID)&pHead->Flag, sizeof (pHead->Flag));
            // number of messages located in the storage
            ApiAddParam (pResp, NULL, 5, (LPVOID)&pHead->MsgNum, sizeof (pHead->MsgNum));
        }
        else
            rc = RC_LOGGER_HEADER_NULL;
    }
    else
    {
        rc = RC_LOGGER_UNKNOWN_API;
    }

    ApiAddErrorCode (pResp, rc);

    return APIHEC_OK;
}

MXRC SvsrLoggerEntry (void)
{
    MXRC rc;

    if (FAILED(rc = SvsrLoggerInit (SVSR_API_LOGGER_DEF_MASK, SVSR_API_LOGGER_BUF_SIZE)))
    {
        uart_printf ("SVSR-LOGGER: Initialization failed, rc=0x%08x\r\n", rc);
        return rc;
    }

    if (FAILED(rc = SvsrLoggerInitTraceBuf(SVSRLOG_TRACE_BUF_ELEM_NUM)))
    {
        uart_printf ("SVSR-LOGGER: Trace LogBuf initialization failed, rc=0x%08x\r\n", rc);
        return rc;
    }

    return rc;
}

MXRC SvsrLoggerInit (UINT32 nMask, UINT32 nLogSize)
{
    MXRC rc;
    APILOGGERCTX  * pCtx;

    pCtx = SvsrLoggerGetCtx();
    _ASSERT_PTR(pCtx);

    memset (pCtx, 0, sizeof (*pCtx));

    pCtx->EnabledMask = nMask;

    if ((pCtx->StoragePtr = (LoggerStorageCreate(&gSvsrLoggerBuf, nLogSize))) == NULL)//MxHeapAlloc (AppHeap, nLogSize+sizeof(LOG_HEADER))) == NULL)
    {
        _ASSERT_PTR(pCtx->StoragePtr);
        return RC_LOGGER_ALLOC_ERROR;
    }

    // to create the log header

    pCtx->StorageSize    = nLogSize;
    pCtx->Currentresetcounter = 0;
    pCtx->Resetcounter = LOGGER_MAX_LOGGING_SPACE;
    pCtx->ResetMarker = 0;
    pCtx->ResetMsgNum = 0;
    pCtx->DisableLogging = 0;   // Do not Disable

    rc = SvsrRegClient(IID_API_LOGGER, SvsrLoggerApiHandler, NULL);
    _ASSERT_RC (rc);

    //uart_printf ("SVSR-LOGGER: 0x%08x, size=%d\r\n", logger_ctx.StoragePtr->hdr, nLogSize);

    return rc;
}

MXRC SvsrLoggerFlush (void)
{
    int addr = __return_address() - 4;
    APILOGGERCTX  * pCtx = SvsrLoggerGetCtx();
    STORAGE_HEADER * pHdr = SvsrLogHeader();

    _ASSERT_PTR(pCtx);

    if (pCtx->StoragePtr->hdr != NULL)
    {
    	UINT32 psize = pHdr->PayloadSize + sizeof(STORAGE_HEADER) + ((pCtx->StoragePtr->LastOffset < pHdr->StartOffset) ? (pHdr->StartOffset - pCtx->StoragePtr->LastOffset) : 0);
        UINT32 size = MIN(pCtx->StorageSize,psize);
        UINT32 t;

        if(SvsrLoggerGetMask() != 0)
        {
            MxDelayTicks(150000); // to complete logging by other threads of other cores
        }

        uart_printf ("\r\n(cpu=%d lr=%x) Svsrlog flushing (0x%08x / %d)... ", MxGetCpuID(), addr, pHdr, size);
        t = MxGetTicks ();
        MxCacheClean (pHdr, size);
        t = MxGetTicks () - t;

        uart_printf ("OK (t=%d)\r\n", t);

        uart_printf("\r\nSvsrLogger: Location: 0x%08x, Storage Size: %d, Payload Size: %d\r\n",
            pCtx->StoragePtr->hdr, pCtx->StorageSize, size);

        uart_printf ("[rdmem 0x%08x %d -o api.bin]\r\n", pCtx->StoragePtr->hdr, size);
    }
    else
    {
        uart_printf ("The storage is NULL!\r\n");
    }

    return MX_OK;
}

MXRC SvsrLoggerRestore(UINT32 bInvCache)
{
    APILOGGERCTX * pCtx = SvsrLoggerGetCtx ();

    _ASSERT_PTR (pCtx);

    if (bInvCache)
    {
        SvsrLoggerLock(pCtx);
        MxCacheInvalidate(pCtx->StoragePtr->hdr, pCtx->StorageSize);
        SvsrLoggerUnlock(pCtx);
    }

    return MX_OK;
}

MXRC SvsrLoggerResetStorage(unsigned int lockState)     // 0  = Dont Lock. 1 = Lock
{
    MXRC rc = RC_LOGGER_ALLOC_ERROR;
    APILOGGERCTX * pCtx = SvsrLoggerGetCtx ();
    //STORAGE_HEADER* pHeader = SvsrLogHeader();

    _ASSERT_PTR (pCtx);

    if ((pCtx->StoragePtr == NULL) || (pCtx->StorageSize == 0))
    {
        uart_printf ("RESET: LOG storage is NULL or size is 0\r\n");
        _ASSERT_RC(rc);

        return rc;
    }

    // to write zeros to the memory

    if (lockState)
        SvsrLoggerLock(pCtx);

    //LoggerStorageReset((STORAGE_HEADER *)pCtx->StoragePtr, pCtx->StorageSize);

	LoggerStorageReset(pCtx->StoragePtr, 0);
	
	//pHeader->PayloadSize = pCtx->ResetMarker;
    //pHeader->MsgNum = pCtx->ResetMsgNum;

    //MxCacheClean(pCtx->StoragePtr, pCtx->StorageSize);

    if (lockState)
        SvsrLoggerUnlock(pCtx);

    return MX_OK;
}

extern UINT32 ApiPlayerGetCurrentSFN(API_LOG_INFO * pHdr);
    
MXRC SvsrLoggerGetInfo (LOG_INFO * pInfo)
{
    UINT32 nIndex = 0;
    API_LOG_INFO * pHdr;
    API_LOG_INFO * pFirstSF = NULL;
    API_LOG_INFO * pLastSF  = NULL;

    APILOGGERCTX * pCtx = SvsrLoggerGetCtx ();
    STORAGE_HEADER* pHeader = SvsrLogHeader();

    _ASSERT_PTR (pCtx);
    _ASSERT_PTR (pHeader);
    _ASSERT_PTR(pInfo);

    memset (pInfo, 0, sizeof(*pInfo));

    //pInfo->StorageSize  = pCtx->StorageSize - sizeof (LOG_HEADER);
    pInfo->Storage      = pCtx->StoragePtr->hdr;
    pInfo->Mask         = pCtx->EnabledMask;
    pInfo->StorageSize  = pHeader->LogStorSize + sizeof(STORAGE_HEADER);
    pInfo->DataSize     = pHeader->PayloadSize + ((pCtx->StoragePtr->LastOffset < pHeader->StartOffset) ? (pHeader->StartOffset - pCtx->StoragePtr->LastOffset) : 0 );
    pInfo->Flags        = pHeader->Flag;
    pInfo->ResetMarker  = pCtx->ResetMarker;
    pInfo->ResetCouter  = pCtx->Resetcounter;
    pInfo->CurrentResetCounter = pCtx->Currentresetcounter;

    while ((pHdr = LoggerStorageGetNextMsg (pCtx->StoragePtr, &nIndex)) != NULL)
    {
        // Message ID used like the index,
        // the array does contain enought space

        pInfo->ApiStat[pHdr->Message]++;
        pInfo->ApiNum ++;

        // Receive pointers for First and Last TxVectors
        if (pHdr->Message == PHY_TXSTART_REQ)
        {
            if (pFirstSF == NULL)
                pFirstSF = pHdr;
            pLastSF = pHdr;
            
            pInfo->TotalSF++;
        }

#ifdef LOGGER_TIME_MEASUREMENT_ENABLED
        pInfo->MaxTime [pHdr->Message] = pCtx->MaxTime[pHdr->Message];
        pInfo->MinTime [pHdr->Message] = pCtx->MinTime[pHdr->Message];
        pInfo->AvgTime [pHdr->Message] = pCtx->AvgTime[pHdr->Message];
        pInfo->MsgSize [pHdr->Message] = pCtx->MsgSize[pHdr->Message];
#endif
    }

    if (pFirstSF != NULL)
        pInfo->FirstSF = ApiPlayerGetCurrentSFN(pFirstSF);
    if (pLastSF != NULL)
        pInfo->LastSF  = ApiPlayerGetCurrentSFN(pLastSF);

    return MX_OK;
}

MXRC SvsrLoggerPrintInfo (void)
{
    LOG_INFO info;
    MXRC rc;
    UINT32 i;

    uart_printf ("Getting logger info ...");

    memset (&info, 0, sizeof(info));

    rc = SvsrLoggerGetInfo(&info);

    if (FAILED(rc))
    {
        uart_printf ("Failed, rc=%x\r\n", rc);
        return 0;
    }

    uart_printf ("OK\r\n");

    uart_printf ("      LOG storage: 0x%08x\r\n", info.Storage);
    uart_printf ("    LOG stor size: %d\r\n",     info.StorageSize);
    uart_printf (" LOG payload size: %d (total: %d)\r\n",     info.DataSize, info.DataSize + sizeof(STORAGE_HEADER));
    uart_printf ("First subframe id: %d\r\n", info.FirstSF);
    uart_printf (" Last subframe id: %d\r\n", info.LastSF);
    uart_printf ("  Total subframes: %d\r\n", info.TotalSF);
    uart_printf ("  Num of messages: %d\r\n\r\n", info.ApiNum);
    uart_printf ("     Status flags: 0x%08x\r\n",    info.Flags);
    uart_printf ("    LOG ResetMark: 0x%08x\r\n", info.ResetMarker);
    uart_printf ("     ResetCounter: 0x%08x\r\n", info.ResetCouter);
    uart_printf ("Curr ResetCounter: 0x%08x\r\n", info.CurrentResetCounter);
    uart_printf ("         LOG mask: 0x%08x\r\n\r\n", info.Mask);

    uart_printf ("Statistics:\r\n");

    for (i = 0; i < 32; i++)
    {
        if (info.ApiStat [i] != 0)
        {
#ifdef LOGGER_TIME_MEASUREMENT_ENABLED
            uart_printf (" *[cmd id = %02d]=>[%03d]{max:%d, avg:%d, min:%d, size:%d}\r\n", i, info.ApiStat [i], info.MaxTime[i], info.AvgTime[i], info.MinTime[i], info.MsgSize[i]);
#else
            uart_printf (" *[cmd id = %02d]=>[%03d]\r\n", i, info.ApiStat [i]);
#endif
        }
    }

    return MX_OK;
}

MXRC SvsrLoggerDisable (void)
{
    logger_ctx.DisableLogging = 1;

    return MX_OK;
}

MXRC SvsrLoggerEnable (void)
{
    logger_ctx.DisableLogging = 0;

    return MX_OK;
}



MXRC SvsrLoggerSetMask (UINT32 nMask)
{
    logger_ctx.EnabledMask = nMask;

    return MX_OK;
}

MXRC SvsrLoggerSetMaskNmm (UINT32 nMask)
{
    logger_ctx.EnabledMaskNmm = nMask;

    return MX_OK;
}


UINT32 SvsrLoggerGetMask (void)
{
    return logger_ctx.EnabledMask;
}

UINT32 SvsrLoggerGetMaskNmm (void)
{
    return logger_ctx.EnabledMaskNmm;
}


UINT32 SvsrLoggerGetFlag (void)
{
    STORAGE_HEADER * pHead = SvsrLogHeader();

    if (pHead == NULL)
    {
        uart_printf ("LOG: header is null\r\n");
        return 0;
    }

    return pHead->Flag;
}

/** @brief The function just to add PHY init message, this functionis the same to other
            code and keeped here for backward compatibility

    @return [MXRC] error code
    \ingroup group_lte_logging
    */

MXRC SvsrLoggerAddInit  (UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize)
{
#ifdef LOGGER_PRINT
    uart_printf ("LOG-INIT: %d, %d\r\n", nMsgID, nMsgSize);
#endif
    return SvsrLoggerAddMsgForce(nMsgID, nMacPhySrcDstID, pMsg, nMsgSize);
}

MXRC SvsrLoggerAddMsgForce (UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize)
{
    APILOGGERCTX * pCtx = SvsrLoggerGetCtx();
    STORAGE_HEADER * pLog = SvsrLogHeader();

#ifdef LOGGER_TIME_MEASUREMENT_ENABLED
    UINT32 tick;
    tick = MxGetTicks();
#endif

    _ASSERT_PTR (pCtx);

    if (pLog == NULL)
    {
        uart_printf ("LOG: Storage is NULL\r\n");
        return RC_LOGGER_HEADER_NULL;
    }

#ifdef LOGGER_PRINT
    uart_printf ("LOG-ADD: %d, %d\r\n", nMsgID, nMsgSize);
#endif

    if (pCtx->DisableLogging)
        return MX_OK;

    SvsrLoggerLock (pCtx);

    LoggerStorageAddMsg(pCtx->StoragePtr, nMsgID, nMacPhySrcDstID, pMsg, nMsgSize);

#ifdef LOGGER_TIME_MEASUREMENT_ENABLED

    tick = MxGetTicks () - tick;

    // MIN time
    if (pCtx->MinTime [nMsgID] != 0)
        pCtx->MinTime [nMsgID] = MIN(pCtx->MinTime [nMsgID], tick);
    else
        pCtx->MinTime [nMsgID] = tick;

    // MAX time
    if (pCtx->MaxTime [nMsgID] != 0)
        pCtx->MaxTime [nMsgID] = MAX(pCtx->MaxTime [nMsgID], tick);
    else
        pCtx->MaxTime [nMsgID] = tick;

    // AVERAGE time
    if (pCtx->AvgTime [nMsgID] != 0)
        pCtx->AvgTime [nMsgID] = (pCtx->AvgTime [nMsgID] + tick) >> 1;  // div 2
    else
        pCtx->AvgTime [nMsgID] = tick;

    // To save message size
    pCtx->MsgSize [nMsgID] = MAX(pCtx->MsgSize [nMsgID], nMsgSize);

#endif

    SvsrLoggerUnlock(pCtx);

    return MX_OK;
}


MXRC SvsrLoggerAddMsg (UINT32 nMsgID, UINT32 nMacPhySrcDstID, LPVOID pMsg, UINT32 nMsgSize)
{
    APILOGGERCTX * pCtx = SvsrLoggerGetCtx();

    _ASSERT_PTR (pCtx);

    if (pCtx->DisableLogging)
        return MX_OK;

    if(nMsgID >= 50)
    {
        if ((pCtx->EnabledMaskNmm & (1 << (nMsgID-50))) == 0)
            return MX_OK;
    }
    else
    {
        if ((pCtx->EnabledMask & (1 << nMsgID)) == 0)
            return MX_OK;
    }

    return SvsrLoggerAddMsgForce(nMsgID, nMacPhySrcDstID, pMsg, nMsgSize);
}

UINT32 SvsrLoggerGetMsgNum (void)
{
    STORAGE_HEADER * pHdr = SvsrLogHeader();

    _ASSERT_PTR (pHdr);

    return pHdr->MsgNum;
}

API_LOG_INFO * SvsrLoggerGetMsg (UINT32 nMsgIndex)
{
    return LoggerStorageGetMsg(SvsrLoggerGetCtx()->StoragePtr, nMsgIndex);
}

API_LOG_INFO * SvsrLoggerGetNextMsg (UINT32 * pnMsg)
{
    APILOGGERCTX * pCtx = SvsrLoggerGetCtx ();
    STORAGE_HEADER * pLog = SvsrLogHeader();
    PUINT8 pStor = (PUINT8)SvsrLogStorage();

    if (pCtx == NULL)
    {
        uart_printf ("LOG: Context is NULL (error)\r\n");
        return NULL;
    }

    if (pLog == NULL)
    {
        uart_printf ("LOG: Storage is NULL (error)\r\n");
        return NULL;
    }

    if (pStor == NULL)
    {
        uart_printf ("LOG: Storage is NULL (error)\r\n");
        return NULL;
    }

    return  LoggerStorageGetNextMsg(pCtx->StoragePtr, pnMsg);
}

PUINT8 SvsrGetLocation (void)
{
    return SvsrLogStorage();
}

UINT32 SvsrGetSize (void)
{
	APILOGGERCTX * pCtx = SvsrLoggerGetCtx ();
    STORAGE_HEADER * pHdr = SvsrLogHeader();

    if (pHdr == NULL)
    {
        uart_printf ("LOG: STORAGE is NULL\r\n");
        return 0;
    }

    return pHdr->PayloadSize + (pCtx->StoragePtr->LastOffset < pHdr->StartOffset) ? (pHdr->StartOffset - pCtx->StoragePtr->LastOffset) : 0 ;
}

LPVOID SvsrLoggerGetLocation (void)
{
    return (SvsrGetLocation() - sizeof(STORAGE_HEADER));
}

LPVOID SvsrLoggerGetStorageCtx (void)
{
    return SvsrLoggerGetCtx()->StoragePtr;
}


UINT32 SvsrLoggerGetSize (void)
{
    return (SvsrGetSize() + sizeof(STORAGE_HEADER));
}

UINT32 SvsrLoggerGetStorageSize (void)
{
    APILOGGERCTX  * pCtx;

    pCtx = SvsrLoggerGetCtx();

    if( pCtx == NULL)
    {
        uart_printf ("SvsrLoggerGetStorageSize: Ctx is NULL\r\n");
        return 0;
    }

    return pCtx->StorageSize;
}

UINT32 SvsrLoggerSetTTICycleLimit(unsigned int Ctr)
{
    APILOGGERCTX  * pCtx;

    pCtx = SvsrLoggerGetCtx();

    if( pCtx == NULL)
    {
        uart_printf ("SvsrLoggerSetResetCounter: Ctx is NULL\r\n");
        return 0;
    }

    SvsrLoggerLock(pCtx);
    pCtx->Resetcounter = Ctr;
    pCtx->Currentresetcounter = 0;
	pCtx->StoragePtr->TTIreset = Ctr;
    SvsrLoggerUnlock(pCtx);

    return 0;
}

UINT32 SvsrLoggerInitTraceBuf(UINT32 ElementsNum)
{// store debug data in circular buffer and printing it out to serial on some event

    if(gSvsrLogTraceBuffer != NULL)
    {
        uart_printf("Reusage of Trace buffer (ptr=0x%x)\r\n", gSvsrLogTraceBuffer);
    
        gSvsrLogTraceBufferIdx = 0;
        gSvsrLogTraceBufferNum = 0;
        
        return MX_OK;
    }

    gSvsrLogTraceBuffer = (SVSRLOG_TRACE_BUF_ELEM*)MxHeapAlloc(
        DDR1NcnbHeap, 
        ElementsNum * sizeof(SVSRLOG_TRACE_BUF_ELEM));

    if(gSvsrLogTraceBuffer == NULL)
    {
        uart_printf("SvsrLoggerInitTraceBuf pointer is NULL\r\n");
        return 1;
    }

    gSvsrLogTraceBufferIdx = 0;
    gSvsrLogTraceBufferNum = 0;

    //uart_printf("SvsrLoggerInitTraceBuf: OK\r\n");

    return MX_OK;
}

UINT32 SvsrLoggerAddTraceBuf(UINT32 data1, UINT32 data2, UINT32 data3, UINT32 data4, UINT32 data5,
    UINT32 data6, UINT32 data7, UINT32 data8, UINT32 data9, UINT32 data10)
{
    if(gSvsrLogTraceBuffer != NULL)
    {
        SVSRLOG_TRACE_BUF_ELEM* pBuf;

        pBuf = &gSvsrLogTraceBuffer[gSvsrLogTraceBufferIdx];
        
        pBuf->data[0] = data1;
        pBuf->data[1] = data2;
        pBuf->data[2] = data3;
        pBuf->data[3] = data4;
        pBuf->data[4] = data5;
        pBuf->data[5] = data6;
        pBuf->data[6] = data7;
        pBuf->data[7] = data8;
        pBuf->data[8] = data9;
        pBuf->data[9] = data10;

        gSvsrLogTraceBufferIdx ++;

        if(gSvsrLogTraceBufferIdx >= (SVSRLOG_TRACE_BUF_ELEM_NUM))
            gSvsrLogTraceBufferIdx = 0;

        gSvsrLogTraceBufferNum++;
    }

    return MX_OK;
}

void SvsrLoggerPrintTraceBuf(void)
{
    if(gSvsrLogTraceBufferNum && (gSvsrLogTraceBuffer != NULL))
    {
        UINT32 CurrIdx;
        SVSRLOG_TRACE_BUF_ELEM* pBuf;
        int ii;

        uart_printf("\r\nStart SvsrLogTraceBuffer dumping (%d totally processed entries)...\r\n", gSvsrLogTraceBufferNum);

        CurrIdx = 0;

        if(gSvsrLogTraceBufferNum > SVSRLOG_TRACE_BUF_ELEM_PRINT)
        {
            if(gSvsrLogTraceBufferIdx >= SVSRLOG_TRACE_BUF_ELEM_PRINT)
                CurrIdx = gSvsrLogTraceBufferIdx - SVSRLOG_TRACE_BUF_ELEM_PRINT;
            else
                CurrIdx = SVSRLOG_TRACE_BUF_ELEM_NUM - (SVSRLOG_TRACE_BUF_ELEM_PRINT - gSvsrLogTraceBufferIdx);
        }

        do
        {
            pBuf = &gSvsrLogTraceBuffer[CurrIdx];

            uart_printf("%d. ", CurrIdx);

            for (ii = 0; ii < sizeof(pBuf->data) / sizeof(pBuf->data[0]); ii++)
            {
                uart_printf("%6d ", pBuf->data[ii]);
            }

            MxDelayTicks(10000000);
            uart_printf("\r\n");

            if(++CurrIdx >= SVSRLOG_TRACE_BUF_ELEM_NUM)
                CurrIdx = 0;
        }while(CurrIdx != gSvsrLogTraceBufferIdx);

        uart_printf("\r\nDone\r\n");
    }
}



void SvsrLoggerSetResetMarker(void)
{
    APILOGGERCTX * pCtx = SvsrLoggerGetCtx ();
    STORAGE_HEADER* pHeader = SvsrLogHeader();

    if( pCtx == NULL)
    {
        uart_printf ("SvsrLoggerSetResetMarker: Ctx is NULL\r\n");
        return;
    }

    SvsrLoggerLock (pCtx);

    pCtx->ResetMarker = pHeader->PayloadSize  + (pCtx->StoragePtr->LastOffset < pHeader->StartOffset) ? (pHeader->StartOffset - pCtx->StoragePtr->LastOffset) : 0 ;
    pCtx->ResetMsgNum = pHeader->MsgNum;
	

    SvsrLoggerUnlock (pCtx);
}

