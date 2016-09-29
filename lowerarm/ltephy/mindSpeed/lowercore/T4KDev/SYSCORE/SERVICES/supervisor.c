//-------------------------------------------------------------------------------------------
/** @file supervisor.c
 *
 * @brief The implementation of supervisor's APIs
 * @author Mindspeed Technologies
 * @version $Revision: 1.103 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "supervisor.h"
#include "intercpu.h"
#include "apimgr.h"
#include "appprintf.h"
#include <string.h>
#include "heaps.h"
#include "systimers.h"
#include "initcpu.h"
#include "version.h"
#include "svsrlogger.h"
#include "mdmadrv.h"
#include "globalstoragearrays.h"
#include "sctp_client.h"
#include "tracer.h"
#ifdef UNIDIAG_ENABLED
#include "unified_diag.h"
#endif
#include "lcorecfg.h"
#include "mlog.h"

/** @brief The set of return codes of extended API commands building
    \ingroup group_lte_services
    */

typedef enum _BUILD_STATUS_
{
    BUILD_ALLOC_ERROR   = 0,
    BUILD_PAPAM_ERROR   = 1,
    BUILD_OK            = 2,
    BUILD_READY         = 3
}BuildStatus;


typedef struct tagITID
{
    UINT32 InstID;
    UINT32 MTypeID;
}ITID;

#define SvsrIcpuApiLogEna 0

#if SvsrIcpuApiLogEna
typedef struct tagSvsrAPIloggingItem
{
    UINT32 time;
    UINT32 lr;
    ApiHeader Hdr;
    UINT32 W1;
    UINT32 W2;
}SvsrAPIloggingItem;

#define SvsrIcpuApiLogStorage 100
SvsrAPIloggingItem SvsrTxApiLogBuffer[SvsrIcpuApiLogStorage][CPU_NUM];
UINT32 SvsrTxApiLogBufferCntr[CPU_NUM] = {0};
UINT32 SvsrTxApiLogBufferCntrTot[CPU_NUM] = {0};

SvsrAPIloggingItem SvsrRxApiLogBuffer[SvsrIcpuApiLogStorage][CPU_NUM];
UINT32 SvsrRxApiLogBufferCntr[CPU_NUM] = {0};
UINT32 SvsrRxApiLogBufferCntrTot[CPU_NUM] = {0};
#endif

// Debug Only
UINT32 NumTxBuffFull = 0;
UINT32 TxBuffFullTimeStamp[1024] = {0};


typedef struct _SVSR_DIAG_API_
{
    UINT32      nTTI;
    UINT32      nNum;
    UINT32      nListSize;
    UINT16      HostID;
    UINT8       Pattern;
    HANDLE      hThread;
    HANDLE      hTimer;
    HANDLE      hEvent;
    UINT32      NumSent;
	UINT32		NotifyType;

}SVSR_DIAG_API_CTX, *PSVSR_DIAG_API_CTX;


typedef struct tagSvsrSysObjInfo
{
    UINT32 ThreadsNum;        // The number of created threads

#ifdef MX_EVENT_ENABLED
    UINT32 EventsNum;        // The number of created events
#endif

#ifdef MX_LIST_ENABLED
    UINT32 ListsNum;        // The number of created lists
#endif

#ifdef MX_TIMER_ENABLED
    UINT32 TimersNum;        // The number of created timers
#endif

#ifdef MX_QUEUE_ENABLED
    UINT32 QueueNum;
    UINT32 QueueSize [MX_MAX_QUEUES];
#endif

#ifdef MX_HEAP_ENABLED
    UINT32 HeapNum;
    UINT32 HeapSize [MX_MAX_HEAPS];
#endif

#ifdef MX_PART_ENABLED
    UINT32 PartNum;
    UINT32 PartFreeBlk [MX_MAX_PARTITIONS];
#endif

#ifdef MX_RHEAP_ENABLED
    UINT32 RHeapNum;
    UINT32 RHeapSize [MX_MAX_RHEAPS];
#endif

} SvsrSysObjInfo;

SvsrSysObjInfo SysObjInfoOrg;
SvsrSysObjInfo SysObjInfoTmp;

SVSR_DIAG_API_CTX svsr_diag_ctx;
/******************************************************************************
*                The list of module global data                               *
*******************************************************************************/

SupervisorCtx SvsrCtx [CPU_NUM];  // The pointer to the supervisor context
                                    // this pointer should be initialized
                                    // by the Init function
static COMM_TYPE    TransportType;
static BOOL         ICReady;
static BOOL         SCTPReady;

#if 0

UINT32 SvsrDumpMask = (SVSR_DUMP_RX_API | SVSR_DUMP_TX_API);

#else

UINT32 SvsrDumpMask = 0;

#endif

static ITID inst_type_id [] =
{
    {IID_DSP_DIAG_0,        DSPDIAG},
    {IID_DSP_DIAG_1,        DSPDIAG},
    {IID_DSP_DIAG_2,        DSPDIAG},
    {IID_DSP_DIAG_3,        DSPDIAG},
    {IID_DSP_DIAG_4,        DSPDIAG},
    {IID_DSP_DIAG_5,        DSPDIAG},
    {IID_DSP_DIAG_6,        DSPDIAG},
    {IID_DSP_DIAG_7,        DSPDIAG},
    {IID_DSP_DIAG_8,        DSPDIAG},
    {IID_DSP_DIAG_9,        DSPDIAG},

    {IID_FP_DIAG_0,         FPDIAG},
    {IID_FP_DIAG_1,         FPDIAG},
    {IID_FP_DIAG_2,         FPDIAG},
    {IID_FP_DIAG_3,         FPDIAG},
    {IID_FP_DIAG_4,         FPDIAG},
    {IID_FP_DIAG_5,         FPDIAG},
    {IID_FP_DIAG_6,         FPDIAG},
    {IID_FP_DIAG_7,         FPDIAG},
    {IID_FP_DIAG_8,         FPDIAG},
    {IID_FP_DIAG_9,         FPDIAG},

    {IID_FEC_DIAG,          FECDIAG},

    {IID_SW_DIAG,           ARMDIAG},

    {IID_WIMAX,             WIMAXAPP},
    {IID_LTE,               LTEAPP},

    {IID_CPRI_DIAG,         CPRIDIAG},

    {IID_MDMA_DIAG,         MDMADIAG},

    {0,                     0}
};

ICORE_PART_CFG ICorePartListCfg[SVSR_ICORE_NUM] =
{
        // --------------- The table of I-CORE partitions ---------------

        {1,     SVSR_MSG_NUM,       SVSR_MSG_PART_SMALL_BLOCK_SIZE, {0}},
        {1,     SVSR_MSG_NUM,       SVSR_MSG_PART_MIDDLE_BLOCK_SIZE, {0}},
        {0,     SVSR_MSG_NUM,       SVSR_MSG_PART_BIG_BLOCK_SIZE, {0}},
        {0,     SVSR_MSG_NUM,       SVSR_MSG_PART_BLOCK_SIZE, {0}},
};

MXRC SvsrSctpSendReady (UINT32 ErrorCode);

/** @brief This function returns opposite type for type of API command

    @param type [in] - the API command type

    @return [UINT16] an opposite type

    \ingroup group_lte_services
    */

INLINE UINT16 SvsrGetRespType (UINT16 type)
{
    if (type == API_TYPE_CFG_PARAM)
        return API_TYPE_CFG_RESP;

    return API_TYPE_QRY_RESP;
}


/** @brief This function builds extended API command from API commands fragments

    @param pClient [in] - the client context.
    @param pApi    [in] - the fragment of API command

    @return [BuildStatus] status of building
    \ingroup group_lte_services
    */

BuildStatus SvsrBuildExtApi (ApiHandlerCtx * pClient, ExtApiHeader * pApi)
{
    PUINT8 pPayload;
    UINT32 nBufSize;  // The total size of a buffer for complete an API
    UINT32 nChunkSize;// The size of data in current pApi chunk

    nBufSize  = API_HDR_SIZE + pApi->TotalSize;
    nChunkSize= GetChunkPayloadSize(pApi);

    // In case of the first block of the API command
    // we need to allocate the space for API command data

    if (API_IS_FIRST_CHUNK(pApi))
    {
        pClient->AsmOffs = 0;

        if (pClient->AsmApiCmd != NULL)
        {
            MxHeapFree(AppHeap, pClient->AsmApiCmd);
        }

        pClient->AsmApiCmd = (ApiHeader *)MxHeapAlloc(AppHeap, nBufSize);

        if (pClient->AsmApiCmd == NULL)
            return BUILD_ALLOC_ERROR;

        // API header copy
        memcpy (pClient->AsmApiCmd, pApi, sizeof (ApiHeader));

        pClient->AsmApiCmd->Length =  pApi->TotalSize;
        pClient->AsmApiCmd->Control&= ~(API_EXTENDED_HEADER|API_FIRST_CHUNK);
    }

    // If the buffer was not allocated
    // (the FirstChunk bit was not set in the first packet)

    if (pClient->AsmApiCmd == NULL)
        return BUILD_PAPAM_ERROR;

    pPayload = GetApiPayload(pClient->AsmApiCmd) + pClient->AsmOffs;

    // Check buffer overflow

    if ((pClient->AsmOffs + nChunkSize) > pClient->AsmApiCmd->Length)
    {
        MxHeapFree(AppHeap, pClient->AsmApiCmd);

        pClient->AsmApiCmd = NULL;
        pClient->AsmOffs   = 0;

        return BUILD_PAPAM_ERROR;
    }

    memcpy (pPayload, GetExtApiPayload(pApi), nChunkSize);

    pClient->AsmOffs += nChunkSize;

    // to check the end of sending

    if (pClient->AsmOffs >= pClient->AsmApiCmd->Length)
        return BUILD_READY;

    return BUILD_OK;
}


/** @brief This function returns the pointer to the supervisor context

    @return [PSupervisorCtx] supervisor context
    \ingroup group_lte_services
    */

INLINE PSupervisorCtx SvsrGetCtx (void)
{
    return &SvsrCtx [MxGetCpuID ()];
}

/** @brief This function routs API command to the destination client. If
           API command had extended class of command, this function builds
           API command and sends it to the client as a solid block of data

    @param pChannel [in] - the context of communication channel
    @param pClient  [in] - the context of client
    @param pApi     [in] - the pointer to the received API command
    @param pResp    [in] - the pointer to buffer for response API command

    @return [MXRC] error code
    \ingroup group_lte_services
    */

APIHEC SvsrProcClientApi (ICChannelCtx * pChannel, ApiHandlerCtx * pClient,
                        ApiHeader * pApi, ApiHeader ** pResp)
{
    BOOL bExtClass = FALSE;
    MXRC rc = MX_OK;
    APIHEC hec = APIHEC_OK;
    BuildStatus stat = BUILD_READY;

    bExtClass = API_IS_EXTENDED(pApi);

    if (bExtClass != FALSE)
    {
        stat = SvsrBuildExtApi(pClient, (ExtApiHeader *) pApi);

        // To point the <pApi> to the buffer of assembled API command !!!
        pApi = pClient->AsmApiCmd;
    }

    switch (stat)
    {
        case BUILD_READY:
            {
                // ***********************************************************
                // In case of the last chunk of the extended API command, the
                // response will be sent to the assembled command only and
                // do not to the last chunk and assembled command
                // ***********************************************************
                // Just for simplifying the protocol
                // ***********************************************************

                if (pClient->RxApi != NULL)
                    rc = pClient->RxApi (pClient->ClientCtx, pApi);

                if (SUCCESSFUL(rc))
                {
                    hec = pClient->Handler(pClient->ClientCtx, pApi, *pResp);

                    if (pClient->TxApi != NULL && (hec == APIHEC_OK))
                        rc = pClient->TxApi (pClient->ClientCtx, *pResp);
                }

                // The API command was processed by the client, so
                // an allocated memory should be deallocated

                if (pClient->AsmApiCmd != NULL)
                {
                    MxHeapFree(AppHeap, pClient->AsmApiCmd);

                    pClient->AsmApiCmd = NULL;
                    pClient->AsmOffs   = 0;
                }

                return hec;
            }

        case BUILD_ALLOC_ERROR:
            {
                rc = RC_SVSR_ALLOC_ERROR;
                break;
            }

        case BUILD_PAPAM_ERROR:
            {
                rc = RC_SVSR_API_PROTO_ERROR;
                break;
            }

        default:
            {
                rc = RC_SVSR_OK;
                break;
            }
    }

    // Add one parameter (error code of operation) to the response
    // CHECK: Response code must be the 1st parameter
    if ((*pResp)->Type == API_TYPE_CFG_RESP)
        ApiAddErrorCode(*pResp, rc);

    return hec;
}

/** @brief This function sends responce to the host with an error code
           of unknown instance.

    @param pChannel [in] - communication channel
    @param pApi     [in] - the received API command
    @param pResp    [in] - the pointer to buffer for response API command

    @return [MXRC] error code
    \ingroup group_lte_services
    */

APIHEC SvsrProcUnknownInst (ICChannelCtx * pChannel, ApiHeader * pApi,
                           ApiHeader ** pResp)
{
    UINT32 nVal = RC_SVSR_UNKNOWN_INST;
    PSupervisorCtx pCtx = SvsrGetCtx ();

    _ASSERT_PTR(pCtx);

    pCtx->ICpuStat.RxErrNum ++;
    pCtx->ICpuStat.RxICpuApi = *pApi;
    pCtx->ICpuStat.RxTime    = MxGetTicks();

    printf ("\r\nSVSR: Incorrect instance ID (%d)\r\n", pApi->DstInstID);

    if (API_IS_EXTENDED(pApi))
    {
        printf ("[ext_rcvd]: Ctrl: 0x%08x Src: 0x%04x Dst: 0x%04x ID: 0x%04x Type: 0x%04x Len: %04d Total: %d\r\n",
            pApi->Control,
            pApi->SrcInstID,
            pApi->DstInstID,
            pApi->MessageID,
            pApi->Type,
            pApi->Length,
            ((ExtApiHeader*)pApi)->TotalSize);
    }
    else
    {
        printf ("[msg_rcvd]: Ctrl: 0x%08x Src: 0x%04x Dst: 0x%04x ID: 0x%04x Type: 0x%04x Len: %04d\r\n",
            pApi->Control,
            pApi->SrcInstID,
            pApi->DstInstID,
            pApi->MessageID,
            pApi->Type,
            pApi->Length);
    }

    printf ("------------------------------------\r\n\r\n");

    ApiAddErrorCode(*pResp, nVal);

    return APIHEC_OK;
}

/** @brief This function is designed to check I-CPU list blocks and to generate response to Linux

    @param pApi [in] - the API command (i-cpu list)
    @param pResp[out]- the lits of responces

*/

void SvsrCheckICpuList (ApiHeader * pApi, ApiHeader * pResp)
{
    UINT32 nListNum = 0, i;
    ApiParam * pParam;
    PUINT32 pCtrl, pPrev;
    static UINT32 my_icpu_list_id = 0;

    PSupervisorCtx pCtx;
    pCtx = SvsrGetCtx();
    _ASSERT_PTR(pCtx);

    if (pApi == NULL)
    {
        uart_printf ("pApi == NULL, wrong parameter\r\n");
        return;
    }

    if (pResp == NULL)
    {
        uart_printf ("pResp == NULL, wrong parameter\r\n");
        return;
    }

    pParam = ApiGetParamByID(pApi, 2);

    if (pParam == NULL)
    {
        uart_printf ("List is empty!!!");
        return;
    }

    pCtrl = (PUINT32)pParam->ParamData[0];

    if ((pCtrl == NULL) || !ICIsRxMemory(pCtx->CommChannel,  pCtrl))
    {
        uart_printf ("Pointer to the next block is wrong [%x]\r\n", pCtrl);
        return;
    }

    while (pCtrl != NULL)
    {
        MxCacheInvalidate (pCtrl, 128);

        // in this block we have 10 32bits words
        // to check the cache

        if (nListNum == 0)
        {
            for (i = 0; i < 10; i++)
            {
                if (pCtrl [i + 0] != pCtrl [i + 1])
                {
                    uart_printf ("Problem in chain [%x] != [%x], i=%d, blk_addr=0x%x, next_addr=0x%x, org_api=0x%x\r\n",
                        pCtrl [i + 0], pCtrl [i + 1], i,
                        pCtrl, pCtrl[0],
                        pApi
                    );

                    MxDelayTicks (150 * 1000);

                    return;
                }
            }
        }

        nListNum ++;

        // to get the pointer to the next list
        pPrev = pCtrl;

        pCtrl = (PUINT32)pCtrl [0];

        if ((pCtrl != NULL) && !ICIsRxMemory(pCtx->CommChannel,  pCtrl))
        {
            uart_printf ("Pointer to the next block is wrong [%x], index=%d\r\n", pCtrl, nListNum);
            return;
        }

        SvsrFreeApi((ApiHeader*)pPrev);
    }

    // To allocate and return the answer to the Linux

    pPrev = NULL;

    for (i = 0; i < nListNum; i++)
    {
        pCtrl = (PUINT32)SvsrAllocICApi ();

        if (pCtrl == NULL)
        {
            uart_printf ("Cannot allocate TX response block, i=%d", i);
            return;
        }

        memset (pCtrl, 0, sizeof (UINT32) * 11);

        if (pPrev != NULL)
        {
            pPrev [0] = (UINT32)pCtrl;
        }
        else
        {
            ApiAddParam(pResp, NULL, 2, &pCtrl, sizeof (pCtrl));
        }

        if (i == 1)
        {
            pPrev [1] = pPrev [0];
            pPrev [2] = pPrev [0];
            pPrev [3] = pPrev [0];
            pPrev [4] = pPrev [0];
            pPrev [5] = pPrev [0];
            pPrev [6] = pPrev [0];
            pPrev [7] = pPrev [0];
            pPrev [8] = pPrev [0];
            pPrev [9] = pPrev [0];
            pPrev [10]= pPrev [0];

            MxCacheClean (pPrev, 128);

            pCtrl[1] = MxGetTicks ();
            pCtrl[2] = my_icpu_list_id ++;
        }

        MxCacheClean (pCtrl, 128);

        pPrev = pCtrl;

    }

    // here the list is ready and will be sent to the linux
    // in this function caller

    //printf ("Recv #%d block\r\n", nListNum);

}


/** @brief This thread is designed to generate API list to the host.
           parameters of list is provided in SvsrEnableApiDiag function
    */

UINT32 SvsrApiDiagThread (void * p)
{
    ApiHeader * pApi;
    PSVSR_DIAG_API_CTX pDiagCtx = &svsr_diag_ctx;
    PUINT32 pPrevPacket = NULL, pPacket = NULL;
    UINT32 i = 0, j, rc, nTicks;
    UINT8 val, *pPlace;

    nTicks = MxGetTicks ();

    pApi = SvsrAllocICApi ();

    if (pApi == NULL)
    {
        printf ("SVSR-Error: Can't allocate first packet for API header\r\n");
        SvsrEnableApiDiag (0,0,0,0,0);
        return 0;
    }

    pApi = ApiCreate (pApi, 0, IID_SUPERVISOR, pDiagCtx->HostID, API_TYPE_IND, SVSR_TEST_GEN_API);

    if (pDiagCtx->nListSize == 0)
    {
        ApiAddParam(pApi, NULL, 3, &nTicks, sizeof (nTicks));
    }

    while (i < pDiagCtx->nListSize)
    {
        pPacket = (PUINT32)SvsrAllocICApi ();

        if (pPacket == NULL)
        {
            printf ("SVSR-Error: Can't allocate chain packet, i=%d\r\n", i);
            SvsrEnableApiDiag (0,0,0,0,0);
            return 0;
        }

        pPacket[0] = NULL;

        // to fill the packet

        pPlace = (PUINT8)(pPacket + 1);
        val = pDiagCtx->Pattern++;

        for (j=0; j < 40; j++)
        {
            pPlace[j] = val ++;
        }

        if (pPrevPacket == NULL)
        {
            ApiAddParam(pApi, NULL, 2, &pPacket, sizeof (pPacket));
        }
        else
        {
            pPrevPacket [0] = (UINT32)pPacket;
            MxCacheClean (pPrevPacket, 4);
        }

        pPrevPacket = pPacket;
        MxCacheClean (pPacket, 128);

        i++;
    }

    rc = SvsrSendICApi (pApi);

    if (FAILED(rc))
    {
        printf ("SVSR-Error: cannot send the list of API commands, %d\r\n", rc);
        SvsrEnableApiDiag (0,0,0,0,0);

        return rc;
    }

    pDiagCtx->NumSent ++;

    if (pDiagCtx->nNum != ~0 && pDiagCtx->nNum != 0)
    {
        pDiagCtx->nNum --;
    }

    if (pDiagCtx->nNum == 0)
    {
        SvsrEnableApiDiag (0,0,0,0,0);
        printf ("The test is stopped, num of sent list = %d\r\n", pDiagCtx->NumSent);
    }

    return 0;
}

UINT32 SvsrApiDiagThreadEx (void * p)
{
    ApiHeader * pApi;
    PSVSR_DIAG_API_CTX pDiagCtx = &svsr_diag_ctx;
    UINT32 i = 0, rc, nTicks;

    nTicks = MxGetTicks ();

    while (i < pDiagCtx->nListSize)
    {
		pApi = SvsrAllocICApi ();

	    if (pApi == NULL)
    	{
        	printf ("SVSR-Error: Can't allocate first packet for API header\r\n");
	        SvsrEnableApiDiag (0,0,0,0,0);
	        return 0;
	    }

		pApi = ApiCreate (pApi, 0, IID_SUPERVISOR, pDiagCtx->HostID, API_TYPE_IND, SVSR_TEST_GEN_API);

	    ApiAddParam(pApi, NULL, 3, &nTicks, sizeof (nTicks));

		if(i == pDiagCtx->nListSize - 1 || pDiagCtx->NotifyType)
			rc = SvsrSendICApiEx(pApi, true);
		else
			rc = SvsrSendICApiEx(pApi, false);

		if (FAILED(rc))
		{
			printf ("SVSR-Error: cannot send the list of API commands");
			SvsrEnableApiDiag (0,0,0,0,0);

			return rc;
		}

        i++;
    }


    pDiagCtx->NumSent ++;

    if (pDiagCtx->nNum != ~0 && pDiagCtx->nNum != 0)
    {
        pDiagCtx->nNum --;
    }

    if (pDiagCtx->nNum == 0)
    {
        SvsrEnableApiDiag (0,0,0,0,0);
        printf ("The test is stopped, num of sent list = %d\r\n", pDiagCtx->NumSent);
    }

    return 0;
}


/** @brief This function is designed to enable/disable API diagnostics,
           API diagnostics is designed to send API commands to the host
           in some period of time and to check API mechanism integrity

    @param nTTI         [in] - to enable/disable diagnostics, TTI>0 to enable|0-disable
    @param nNumPacket   [in] - the number of packets or (-1) till stop API
    @param nListSize    [in] - the number of API commands in the list send to the host
    @param DestID       [in] - Destination ID for outgoing packets

    @return [MXRC] an error code of operation*/

MXRC SvsrEnableApiDiag (UINT32 nTTI, UINT32 nNumPacket, UINT32 nListSize, UINT16 DestID, UINT32 TestType)
{
    MXRC rc;
	TENTRY proc;
    PSVSR_DIAG_API_CTX pDiagCtx = &svsr_diag_ctx;

    if (nTTI == 0)
    {
        if (HANDLE_NOT_NULL(pDiagCtx->hTimer))
        {
            MxStopTimer (pDiagCtx->hTimer);

            MxRemoveTimer (pDiagCtx->hTimer);
            MxRemoveEvent (pDiagCtx->hEvent);
            MxRemoveThread(pDiagCtx->hThread);

            pDiagCtx->hThread = HNULL;
            pDiagCtx->hEvent  = HNULL;
            pDiagCtx->hTimer  = HNULL;
        }
    }
    else
    {
        if (FAILED(rc = MxCreateEvent (0, NULL, &pDiagCtx->hEvent)))
        {
            return rc;
        }

        if (FAILED(rc = MxCreateTimer(TIMER_MODE_PERIOD, nTTI, NULL, pDiagCtx->hEvent, &pDiagCtx->hTimer)))
        {
            MxRemoveEvent (pDiagCtx->hEvent);
            return rc;
        }

		if(TestType)
		{
			proc = SvsrApiDiagThreadEx;
			pDiagCtx->NotifyType = TestType - 1;
		}
		else
			proc = SvsrApiDiagThread;

        if (FAILED(rc = MxCreateThread(proc, NULL, pDiagCtx, 0, &pDiagCtx->hThread)))
        {
            MxRemoveTimer (pDiagCtx->hTimer);
            MxRemoveEvent (pDiagCtx->hEvent);
            return rc;
        }

        rc = MxScheduleThreadByEvent(pDiagCtx->hThread, pDiagCtx->hEvent, THREAD_EVENT_AUTO_RESET);

        if (FAILED(rc))
        {
            MxRemoveThread(pDiagCtx->hThread);
            MxRemoveTimer (pDiagCtx->hTimer);
            MxRemoveEvent (pDiagCtx->hEvent);

            return rc;
        }

        pDiagCtx->nNum = nNumPacket;
        pDiagCtx->nListSize = nListSize;
        pDiagCtx->Pattern = (UINT8)1;
        pDiagCtx->HostID  = DestID;
        pDiagCtx->NumSent = 0;

        rc = MxStartTimer (pDiagCtx->hTimer);

        if (FAILED(rc))
        {
            MxRemoveThread(pDiagCtx->hThread);
            MxRemoveTimer (pDiagCtx->hTimer);
            MxRemoveEvent (pDiagCtx->hEvent);

            return rc;
        }
    }

    return MX_OK;
}

/** @brief This function is called to process API commands targeted to the
           lower ARM supervisor module, the InstanceID = 1 (the global: 0x8001)

    @param pData [in] - the pointer to the context [it set to NULL]
    @param pApi  [in] - the received API command
    @param pResp [out]- the pointer to the response

    @return [APIHEC]

    \ingroup group_lte_services
    */

APIHEC SvsrApiHandler (LPVOID pData, ApiHeader * pApi, ApiHeader * pResp)
{
    UINT32 i, typeID, index, elm;
    PSupervisorCtx pCtx;
    ApiParam * pParam;
    UINT32 nTicks = SysTimerGetTick ();
    UINT32 val = RC_SVSR_API_DONOT_SUPP;

    pCtx = SvsrGetCtx();
    _ASSERT_PTR(pCtx);

    pResp->Type = SvsrGetRespType(pApi->Type);

    if (pApi->DstInstID == SVSR_INVALID_INSTANCE)
    {
        val = RC_SVSR_UNKNOWN_INST;
        ApiAddErrorCode(pResp, val);
        return APIHEC_OK;
    }

    if (IS_CFG_API (pApi, SVSR_CHECK_API_LAT))
    {
        UINT32 * pTimeSrc = (UINT32 *)GetApiPayload (pApi);
        UINT32 * pTimeDst = (UINT32 *)GetApiPayload (pResp);
#if 0
        uart_printf ("Recv U-ARM test API, times:\r\n");
        uart_printf (" - %d\r\n", pTimeSrc [0]);
        uart_printf (" - %d\r\n", pTimeSrc [1]);
        uart_printf (" - %d\r\n", pTimeSrc [2]);
        uart_printf (" - %d\r\n", pTimeSrc [3]);
        uart_printf (" - %d\r\n", nTicks);
#endif
        memcpy (pTimeDst, pTimeSrc, 4 * sizeof (UINT32));
        pResp->Length = (4 * sizeof (UINT32) + 2 * sizeof (UINT32));
        pTimeDst [4] = nTicks;
        pTimeDst [5] = (SysTimerGetTick ());

        return APIHEC_OK;
    }
    else if (IS_CFG_API(pApi, SVSR_STORE_SYS_OBJ))
    {
        val = MX_OK;
        SvsrStoreSysObjs ();
    }
    else if (IS_QRY_API(pApi, SVSR_QUERY_INST_ID))
    {
        // to get the module type

        pParam = ApiGetParamByID(pApi, SVSR_PID_MOD_TYPE);

        if (pParam == NULL)
        {
            val = RC_SVSR_PARAM_ERROR;
            ApiAddErrorCode(pResp, val);

            return APIHEC_OK;
        }

        typeID = pParam->ParamData[0];

        // to get the module index, this parameter is optional
        pParam = ApiGetParamByID(pApi, SVSR_PID_MOD_INDEX);

        if (pParam == NULL)
        {
            index = 0;
        }
        else
        {
            index = pParam->ParamData [0];
        }

        val = RC_SVSR_UNREG_TYPE;
        elm = 0;

        for (i = 0; i < SVSR_CLIENT_NUM; i++)
        {
            if (pCtx->clients[i].Handler != NULL &&
                pCtx->clients[i].ModTypeID == typeID)
            {
                if (elm == index)
                {
                    ApiAddParam(pResp, NULL, SVSR_PID_MOD_INST_ID, &i, sizeof (UINT32));

                    return APIHEC_OK;
                }
                else
                {
                    elm ++;
                }
            }
        }
    }
    else if (IS_CFG_API(pApi, SVSR_CHECK_API_WAY))
    {
        ApiParam * pParam = ApiGetParamByID (pApi, 2);

        if (pParam != NULL)
        {
            pCtx->ApiWayTest = pParam->ParamData [0];
        }

        val = MX_OK;

        ApiAddParam (pResp, NULL, 2, &pCtx->ApiWayTest, sizeof (pCtx->ApiWayTest));

        pCtx->ApiWayTest ++;
    }
    else if (IS_API(pApi, SVSR_CHECK_API_WAY, API_TYPE_IND))
    {
        memcpy(GetApiPayload(pResp), GetApiPayload(pApi), pApi->Length);
        pResp->Length = pApi->Length;
        return APIHEC_OK;
    }
    else if (IS_QRY_API(pApi, SVSR_CHECK_SYS))
    {
        printf ("Check system resources ... ");

        val = SvsrCheckSysObjs (1, pResp);

        if (FAILED(val))
        {
            printf ("Problems are detected, rc=0x%08x\r\n", val);
        }
        else
        {
            printf ("OK\r\n");
        }
    }
    else if (IS_QRY_API (pApi, SVSR_GET_VERSION))
    {
        ApiAddParam (pResp, NULL, 2, (LPVOID)SysGetVersion (), strlen (SysGetVersion ())+1);
        PrintSysVersion ();
    }
    else if (IS_CFG_API(pApi, SVSR_WRITE_MEMORY))
    {
        ApiParam * pStatus, * pAddr, * pData, * pCRC;

        pStatus = ApiGetParamByID (pApi, 1);
        pAddr   = ApiGetParamByID (pApi, 2);
        pData   = ApiGetParamByID (pApi, 3);
        pCRC    = ApiGetParamByID (pApi, 4);

        if (pAddr == NULL || pData == NULL)
        {
            val = RC_SVSR_PARAM_ERROR;
            uart_printf ("Svsr: Write param error\r\n");
        }
        else
        {
            UINT32 CrcReal = 0;
            UINT32 CrcRef = 0;

            if (pCRC != NULL)
            {
                // Calc CRC for incoming data
                CrcReal = MdmaProcCRC (&CrcReal, 0x864CFB, 32, (PUINT8)(&(pData->ParamData[0])), pData->ParamLength);

                // Get Ref CRC
                CrcRef = pCRC->ParamData[0];
            }

            if (CrcReal == CrcRef)
            {
                val = MX_OK;
                memcpy ((LPVOID)(pAddr->ParamData[0]), &(pData->ParamData[0]), pData->ParamLength);
            }
            else
            {
                if ((pStatus != NULL) && (pStatus->ParamData[0] == RC_SVSR_SCTP_W_ERR))
                {
                    val = RC_SVSR_SCTP_W_ERR;
                    uart_printf ("Svsr: Write memory CRC error (SCTP)\r\n");
                }
                else
                {
                    val = RC_SVSR_ICPU_W_ERR;
                    uart_printf ("Svsr: Write memory CRC error (I-CPU)\r\n");
                }
            }
        }
    }
    else if (IS_QRY_API(pApi, SVSR_READ_MEMORY))
    {
        ApiParam * pAddr, * pSize;

        pAddr = ApiGetParamByID (pApi, 2);
        pSize = ApiGetParamByID (pApi, 3);

        if (pAddr == NULL || pSize == NULL)
        {
            val = RC_SVSR_PARAM_ERROR;
            uart_printf ("Svsr: Read param error\r\n");
        }
        else
        {
            //UINT32 Crc = 0;

            val = MX_OK;

            // Calculate CRC for outgoing data
            //Crc = MdmaProcCRC (&Crc, 0x864CFB, 32, (PUINT8)(pAddr->ParamData[0]), pSize->ParamData[0]);

            // Add response parameters
            ApiAddParam (pResp, NULL, 2, (LPVOID)(pAddr->ParamData[0]), pSize->ParamData[0]);
            //ApiAddParam (pResp, NULL, 3, &Crc, sizeof (Crc));

            return APIHEC_OK;
        }
    }
    else if (IS_CFG_API(pApi, DIAG_WRITE_DATA))
    {
        ApiParam * pAddr, * pData;

        pAddr = ApiGetParamByID (pApi, 0x08);
        pData = ApiGetParamByID (pApi, 0x53);

        if (pAddr == NULL || pData == NULL)
        {
            val = RC_SVSR_PARAM_ERROR;
            uart_printf ("Svsr:Write param error\r\n");
        }
        else
        {
            val = MX_OK;
            memcpy ((LPVOID)(pAddr->ParamData[0]), &(pData->ParamData[0]), pData->ParamLength);
        }
    }
    else if (IS_QRY_API(pApi, DIAG_READ_DATA))
    {
        ApiParam * pAddr, * pSize;

        pAddr = ApiGetParamByID (pApi, 8);
        pSize = ApiGetParamByID (pApi, 7);

        if (pAddr == NULL || pSize == NULL)
        {
            val = RC_SVSR_PARAM_ERROR;
            uart_printf ("Svsr:Read param error\r\n");
        }
        else
        {
            val = MX_OK;
            ApiAddParam (pResp, NULL, 0x53, (LPVOID)(pAddr->ParamData[0]), pSize->ParamData[0]);
            return APIHEC_OK;
        }
    }
    else if (IS_CFG_API(pApi, SVSR_API_DUMP_EN))
    {
         ApiParam * pDump;

        pDump= ApiGetParamByID (pApi, 3);

     if (pDump == NULL)
        {
            val = RC_SVSR_PARAM_ERROR;
            uart_printf ("Svsr:Read param error\r\n");
        }
        else
        {
            val = MX_OK;
            SvsrSetDumpMask(pDump->ParamData[0]);
        }
    }
    else if (IS_QRY_API(pApi, SVSR_API_CRC))
    {
        ApiParam * pInfo;
        UINT32 crc;

        pInfo= ApiGetParamByID (pApi, 2);

        if (pInfo != NULL)
        {
            UINT32 rem = 0;

            crc = MdmaProcCRC (&rem, 0x864CFB, 32, (PUINT8)pInfo->ParamData[0], pInfo->ParamData[1]);
            ApiAddParam (pResp, NULL, 2, &crc, sizeof (crc));
        }
        else
        {
            val = RC_SVSR_PARAM_ERROR;
            uart_printf ("Svsr:CRC param error\r\n");
        }

    }
    else if (IS_QRY_API(pApi, SVSR_GET_HEAP_INFO))
    {
        UINT32 nHeapID = ApiGetParamU32 (pApi, 2, 0);
        UINT32 nSize = 0;
        HANDLE h;
        char * pName = NULL;

        switch (nHeapID)
        {
            case 0:
                h = AppHeap;
                pName = "AppHeap";
                break;

            case 1:
                h = DspHeap;
                pName = "DspHeap";
                break;

            case 2:
                h = NcnbHeap;
                pName = "NcnbHeap";
                break;

            case 3:
                h = IramHeap;
                pName = "IramHeap";
                break;

            case 4:
                h = DDR1Heap;
                pName = "DDR1Heap";
                break;

            case 5:
                h = DDR1NcnbHeap;
                pName = "DDR1NcnbHeap";
                break;

            default:
                h = HNULL;
                pName = "HNULL heap";
                break;
        }

        #ifdef MX_HEAP_LEAKAGE_CONTROL
        nSize = MxHeapGetPacketSize(h);
        #else
        nSize = 0;
        #endif

        if (nSize > PACK_HEAP_INFO_BUF)
        {
            printf ("Due to memory size, it's impossible to save [%s] heap state, it's neded [%d] bytes\r\n", pName, nSize);
            val = RC_SVSR_NOT_MEMORY;
        }
        else
        {
            uart_printf ("Packing %s heap, pack size [%d] bytes ... ", pName, nSize);

            #ifdef MX_HEAP_LEAKAGE_CONTROL
            val = SvsrPackHeap (h, nHeapID, PackHeapPtr, PACK_HEAP_INFO_BUF);
            #else
            val = RC_SVSR_IS_NOT_IMP;
            #endif

            if (val == MX_OK)
            {
                uart_printf ("OK\r\nStorage:[0x%08x], packed size:[%d] bytes\r\n", PackHeapPtr, nSize);

                // if data can be stored in the response
                if (nSize < SvsrGetSysInfo().nMaxApiParamSize - 16)
                {
                    ApiAddParam (pResp, NULL, 2, PackHeapPtr, nSize);
                }
                // if not, just return the address where data is stored
                // by using READ commands, it will be possible to read that data
                else
                {
                    ApiAddParam (pResp, NULL, 3, &PackHeapPtr, sizeof (PackHeapPtr));
                    ApiAddParam (pResp, NULL, 4, &nSize, sizeof (nSize));
                }

            }
            else
                uart_printf ("Error, rc=0x%08x\r\n", val);
        }

    }
    else if (IS_CFG_API(pApi, SVSR_TEST_ICPU_LIST))
    {
        SvsrCheckICpuList (pApi, pResp);
    }
    else if (IS_CFG_API(pApi, SVSR_TEST_GEN_API))
    {
		UINT32 p1, p2, p3, p4, p5;

		p1 = ApiGetParamU32 (pApi, 2, 0);
		p2 = ApiGetParamU32 (pApi, 3, 0);
		p3 = ApiGetParamU32 (pApi, 4, 0);
		p4 = pApi->SrcInstID;
		p5 = ApiGetParamU32 (pApi, 5, 0);

		uart_printf(" ----- pass to SvsrEnableApiDiag %d, %d, %d, %d, %d \r\n", p1, p2, p3, p4, p5);

        val = SvsrEnableApiDiag (p1, p2, p3, p4, p5);

        if (ApiGetParamU32 (pApi, 2, 0) == 0)
        {
            ApiAddParam (pResp, NULL, 2, &svsr_diag_ctx.NumSent, sizeof (svsr_diag_ctx.NumSent));
        }
    }
    else if (IS_CFG_RESP_API(pApi, 7))
    {
        return APIHEC_DROP_RESP;
    }

    ApiAddErrorCode(pResp, val);

    return APIHEC_OK;
}


void SvsrDumpRecvApi (ApiHeader * pApi)
{
    if (( SvsrDumpMask & SVSR_DUMP_RX_API ) != 0)
    {
        if (API_IS_EXTENDED(pApi))
        {
            printf ("(cpu-%d)[ext_rcvd]: Ctrl: 0x%08x Src: 0x%04x Dst: 0x%04x ID: 0x%04x Type: 0x%04x Len: %04d Total: %d\r\n",
                MxGetCpuID (),
                pApi->Control,
                pApi->SrcInstID,
                pApi->DstInstID,
                pApi->MessageID,
                pApi->Type,
                pApi->Length,
                ((ExtApiHeader*)pApi)->TotalSize);
        }
        else
        {
            printf ("(cpu-%d)[msg_rcvd]: Ctrl: 0x%08x Src: 0x%04x Dst: 0x%04x ID: 0x%04x Type: 0x%04x Len: %04d\r\n",
                MxGetCpuID (),
                pApi->Control,
                pApi->SrcInstID,
                pApi->DstInstID,
                pApi->MessageID,
                pApi->Type,
                pApi->Length);
        }
    }

#ifdef UNIDIAG_ENABLED
    UniDiagStreamingAPI(PAR_API_MESSAGE_RX,  pApi);
#endif
}


void SvsrDumpSendApi (ApiHeader * pApi)
{
    if (( SvsrDumpMask & SVSR_DUMP_TX_API ) != 0)
    {
      if (API_IS_EXTENDED(pApi))
      {
            printf ("[ext_send]: Ctrl: 0x%08x Src: 0x%04x Dst: 0x%04x ID: 0x%04x Type: 0x%04x Len: %04d Total: %d\r\n",
            pApi->Control,
            pApi->SrcInstID,
            pApi->DstInstID,
            pApi->MessageID,
            pApi->Type,
            pApi->Length,
            ((ExtApiHeader*)pApi)->TotalSize);
      }
      else
      {
            printf ("[msg_send]: Ctrl: 0x%08x Src: 0x%04x Dst: 0x%04x ID: 0x%04x Type: 0x%04x Len: %04d\r\n",
            pApi->Control,
            pApi->SrcInstID,
            pApi->DstInstID,
            pApi->MessageID,
            pApi->Type,
            pApi->Length);
      }
    }

#ifdef UNIDIAG_ENABLED
    UniDiagStreamingAPI(PAR_API_MESSAGE_TX,  pApi);
#endif
}

/** @brief The function returns a pointer to client's context specified by the
           instance ID.

    @param nInstID [in] - the instance ID

    @return [ApiHandlerCtx*] the context or NULL
    \ingroup group_lte_services
    */

ApiHandlerCtx * SvsrGetRemClientCtx (UINT32 nInstID, PSupervisorCtx * pRemCtx)
{
    PSupervisorCtx pCtx;
    UINT32 nCpuNum = SysGetCpuNum ();
    UINT32 i;

    nInstID = GET_LARM_INST(nInstID);

    _ASSERT_PTR (pRemCtx);
    _ASSERT(nInstID < SVSR_CLIENT_NUM);

    if(nInstID >= SVSR_CLIENT_NUM)
        return NULL;

    // to find the API handler on remote core
    // because of ApiRxHandler is designed to be used on core-0
    // we start looking from core-1, because core-0 calls this function

    for (i = 0; i < nCpuNum; i++)
    {
        pCtx = &SvsrCtx [i];

        _ASSERT(pCtx != NULL);

        if (pCtx == NULL)
            continue;

        if (pCtx->clients [nInstID].Handler != NULL)
        {
            *pRemCtx = pCtx;
             return &pCtx->clients [nInstID];
        }
    }

    return NULL;
}

/** @brief The function returns a pointer to client's context specified by the
           instance ID.

    @param nInstID [in] - the instance ID

    @return [ApiHandlerCtx*] the context or NULL

    \ingroup group_lte_services
    */

INLINE ApiHandlerCtx * SvsrGetClientCtx (UINT32 nInstID)
{
    PSupervisorCtx pCtx = SvsrGetCtx ();

    nInstID = GET_LARM_INST(nInstID);

    _ASSERT(nInstID < SVSR_CLIENT_NUM);

    if(nInstID >= SVSR_CLIENT_NUM)
        return NULL;

    _ASSERT(pCtx != NULL);

    if (pCtx == NULL)
        return NULL;

    if (pCtx->clients [nInstID].Handler == NULL)
        return NULL;

    return &pCtx->clients [nInstID];
}


INLINE ApiHeader * SvsrCreateResp (ICChannelCtx * pChannel, ApiHeader * pApi)
{
    ApiHeader * pResp;

    pResp = (ApiHeader *)ICLockTxData(pChannel);

    _ASSERT_PTR(pResp);

    if(pResp == NULL)
        return NULL;

    // To create response (SrcInstID => DstInstID)
    ApiCreate(pResp, API_DEF_FLAGS, pApi->DstInstID, pApi->SrcInstID,
               SvsrGetRespType(pApi->Type),pApi->MessageID);

    // To convert the global numeration to the local lower ARM numeration
    pApi->DstInstID = GET_LARM_INST(pApi->DstInstID);

    return pResp;
}

MXRC SvsrAddSubApi (PSupervisorCtx pRemCtx, LPVOID pData, UINT32 * pnWasEmpty)
{
    IRQTYPE irq = ARM_INT_disable ();
    MxGetLock (&pRemCtx->IMsgSync);

    if (pRemCtx->IMsgPut == pRemCtx->IMsgGet)
    {
        *pnWasEmpty = 1;
    }
    else
    {
        *pnWasEmpty = 0;
    }

    pRemCtx->IMsgQStorage [pRemCtx->IMsgPut++] = (UINT32)pData;

    if (pRemCtx->IMsgPut >= pRemCtx->IMsgCount)
        pRemCtx->IMsgPut = 0;

    MxReleaseLock (&pRemCtx->IMsgSync);
    ARM_INT_restore (irq);

    return MX_OK;
}

INLINE ApiHeader * SvsrGetSubApi (PSupervisorCtx pCtx)
{
    ApiHeader * pApi;

    IRQTYPE irq = ARM_INT_disable ();
    MxGetLock (&pCtx->IMsgSync);

    if (pCtx->IMsgPut == pCtx->IMsgGet)
    {
        pApi = NULL;
    }
    else
    {
        pApi = (ApiHeader *) pCtx->IMsgQStorage [pCtx->IMsgGet++];

        if (pCtx->IMsgGet >= pCtx->IMsgCount)
            pCtx->IMsgGet = 0;
    }

    MxReleaseLock (&pCtx->IMsgSync);
    ARM_INT_restore (irq);

    return pApi;
}

/** @brief  This function parses RX API commands and routs its to the
            appropriate handler or process it by itself if supervisor is
            commands' destination.

    @param pChannel [in] - the context of communication channel

    @return [UINT32] error code of operation
    \ingroup group_lte_services
    */

UINT32 SvsrRxApiHandler (ICChannelCtx * pChannel)
{
    MXRC rc = MX_OK;

    ApiHeader * pApi, * pResp;
    ApiHandlerCtx * pClient;
    PSupervisorCtx pCtx = SvsrGetCtx ();
    APIHEC  hec;
    UINT32 tt = MxGetTicks(), ttt, count = 0;

    _ASSERT_PTR(pCtx);

    while ((pApi = ICLockRxData (pChannel)) != NULL)
    {
        // this code is needed for Linux
        // to be in sync with U-ARM (memory is cacheable)
        ttt = MxGetTicks();

        if (pChannel->ChanID == IC_CHAN_SVSR || pChannel->ChanID == IC_CHAN_SVSR_EX_ICPU)
        {
            MxCacheInvalidate(pApi, sizeof (*pApi));
            MxCacheInvalidate(pApi, MIN(ApiGetTotalSize (pApi), pCtx->CommBlockSize));
        }

        pCtx->ICpuStat.RxMsgNum ++;
        pCtx->ICpuStat.RxICpuApi = *pApi;
        pCtx->ICpuStat.RxTime    = MxGetTicks();

        //uart_printf("SvsrRxApiHandler: got ICPU msg core=%d\r\n", MxGetCpuID());

#if SvsrIcpuApiLogEna
       {
            UINT32 CpuID = MxGetCpuID();
            UINT32 Cntr = SvsrRxApiLogBufferCntr[CpuID];
            SvsrAPIloggingItem* pBuf = &SvsrRxApiLogBuffer[Cntr][CpuID];

            if (++Cntr >= SvsrIcpuApiLogStorage)
                Cntr = 0;

            SvsrRxApiLogBufferCntr[CpuID] = Cntr;
            SvsrRxApiLogBufferCntrTot[CpuID]++;

            pBuf->time = MxGetTicks();
            memcpy(&pBuf->Hdr, pApi, sizeof(ApiHeader));
            pBuf->W1 = *(UINT32*)(pApi + 1);
            pBuf->W2 = *((UINT32*)(pApi + 1) + 1);
       }
#endif

        SvsrDumpRecvApi (pApi);

        MLogTask(1201, RESOURCE_LARM, ttt, MxGetTicks());

        if (GetApiVersion(pApi) > API_SUPPORTED_VERSION)
        {
            if((pResp = SvsrCreateResp (pChannel, pApi)) == NULL)
            {
                ICFreeRxData(pChannel, pApi);
                return RC_SVSR_ALLOC_ERROR;
            }

            ApiAddErrorCode(pResp, RC_SVSR_UNKNOWN_API_VER);

            pCtx->ICpuStat.TxMsgNum++;
            pCtx->ICpuStat.TxTime = MxGetTicks ();
            pCtx->ICpuStat.TxICpuApi = *pApi;

            SvsrDumpSendApi (pResp);

            // In this case we specially ignore the suppression bit
            MxCacheClean(pResp, ApiGetTotalSize (pResp));
            ICSendTxData(pChannel, pResp, TRUE);
        }
        else
        {
            pClient = SvsrGetClientCtx (pApi->DstInstID);

            if (pClient != NULL)
            {
                if((pResp = SvsrCreateResp (pChannel, pApi)) == NULL)
                {
                    ICFreeRxData(pChannel, pApi);
                    return RC_SVSR_ALLOC_ERROR;
                }

                hec = SvsrProcClientApi (pChannel, pClient, pApi, &pResp);
            }
            else
            {
                PSupervisorCtx pRemCtx = NULL;
                UINT32 bWasEmpty;

                pClient = SvsrGetRemClientCtx (pApi->DstInstID, &pRemCtx);

                // just to route received API to the destination core
                if (pClient != NULL)
                {
                    _ASSERT_PTR (pRemCtx);

                    rc = SvsrAddSubApi (pRemCtx, pApi, &bWasEmpty);

                    _ASSERT_RC(rc);

                    //if (bWasEmpty)
                        MxScheduleThread (pRemCtx->IMsgH);
                    //continue;

                    count++;
                    if (count >= 3)
                    {
                        MxScheduleThread(MxGetCurrentThread());
                        break;
                    }
                    else
                    continue;
                }
                else
                {
                    if((pResp = SvsrCreateResp (pChannel, pApi)) == NULL)
                    {
                        ICFreeRxData(pChannel, pApi);
                        return RC_SVSR_ALLOC_ERROR;
                    }

                    hec = SvsrProcUnknownInst (pChannel, pApi, &pResp);
                }
            }

            if (hec == APIHEC_DROP_RESP || API_IS_ACK_SUPPRESS(pApi))
            {
                ICFreeTxData (pChannel, pResp);
            }
            else if (hec == APIHEC_KEEP_API_DROP_RESP)
            {
                // in this case API command should be saved in I-CPU space
                // and the response should be dropped

                ICFreeTxData (pChannel, pResp);
                continue;
            }
            else
            {
                SvsrDumpSendApi (pResp);

                pCtx->ICpuStat.TxMsgNum++;
                pCtx->ICpuStat.TxTime = MxGetTicks ();
                pCtx->ICpuStat.TxICpuApi = *pApi;

                MxCacheClean(pResp, ApiGetTotalSize (pResp));
                ICSendTxData(pChannel, pResp, TRUE);
            }
        }

        ICFreeRxData(pChannel, pApi);

        //if(MxGetCpuID() != 0)
        if (1)
        {// don't loop here if ICPU is handled by directly dedicated core
            MxScheduleThread(MxGetCurrentThread());
            break;
        }
    }

    MLogTask(1200, RESOURCE_LARM, tt, MxGetTicks());

    return rc;
}


UINT32 SvsrSctpRxApiHandler(void * msg)
{
    ApiHeader * pHdr = (ApiHeader *) msg;
    ApiHeader * pResp = NULL;
    MXRC ret = MX_OK;
    ApiHandlerCtx * pClient = NULL;
    APIHEC  hec = APIHEC_OK;

    SvsrDumpRecvApi(pHdr);

    if((pHdr->DstInstID & 0x8000) == 0)
    {
    	pResp = (ApiHeader *)SCTP_Alloc_part();

    	_ASSERT_PTR(pResp);

    	if(pResp == NULL)
    		return RC_SVSR_ALLOC_ERROR;

    	// To create response (SrcInstID => DstInstID)
    	ApiCreate(pResp, API_DEF_FLAGS, pHdr->DstInstID, pHdr->SrcInstID,
    	((pHdr->Type == API_TYPE_CFG_PARAM) ? API_TYPE_CFG_RESP : API_TYPE_QRY_RESP),pHdr->MessageID);

    	// To convert the global numeration to the local lower ARM numeration
    	pHdr->DstInstID = GET_LARM_INST(pHdr->DstInstID);
    	ApiAddErrorCode(pResp, RC_SVSR_OK);

    	SvsrSendApiSimpy(pResp);

    	return MX_OK;
    }

    if (GetApiVersion(pHdr) <= API_SUPPORTED_VERSION)
    {
    	pClient = SvsrGetClientCtx (pHdr->DstInstID);

        if (pClient != NULL)
        {
            pResp = (ApiHeader *)SCTP_Alloc_part();

            _ASSERT_PTR(pResp);

            if(pResp == NULL)
                return RC_SVSR_ALLOC_ERROR;

            // To create response (SrcInstID => DstInstID)
            ApiCreate(pResp, API_DEF_FLAGS, pHdr->DstInstID, pHdr->SrcInstID,
                       ((pHdr->Type == API_TYPE_CFG_PARAM) ? API_TYPE_CFG_RESP : API_TYPE_QRY_RESP),pHdr->MessageID);

            // To convert the global numeration to the local lower ARM numeration
            pHdr->DstInstID = GET_LARM_INST(pHdr->DstInstID);

            hec = SvsrProcClientApi (NULL, pClient, pHdr, &pResp);
        }
        else
        {
        	PSupervisorCtx pRemCtx = NULL;
            UINT32 bWasEmpty;

            pClient = SvsrGetRemClientCtx (pHdr->DstInstID, &pRemCtx);

            // just to route received API to the destination core
            if (pClient != NULL)
            {

            	_ASSERT_PTR (pRemCtx);

                ret = SvsrAddSubApi (pRemCtx, pHdr, &bWasEmpty);

                _ASSERT_RC(ret);

                if (bWasEmpty)
                	MxScheduleThread (pRemCtx->IMsgH);

                return 0;
            }
            else
            {

            	pResp = (ApiHeader *)SCTP_Alloc_part();

            	_ASSERT_PTR(pResp);

            	if(pResp == NULL)
            		return RC_SVSR_ALLOC_ERROR;

            	// To create response (SrcInstID => DstInstID)
            	ApiCreate(pResp, API_DEF_FLAGS, pHdr->DstInstID, pHdr->SrcInstID,
            	         ((pHdr->Type == API_TYPE_CFG_PARAM) ? API_TYPE_CFG_RESP : API_TYPE_QRY_RESP),pHdr->MessageID);

            	// To convert the global numeration to the local lower ARM numeration
            	pHdr->DstInstID = GET_LARM_INST(pHdr->DstInstID);
            	ApiAddErrorCode(pResp, RC_SVSR_UNKNOWN_INST);
            }
        }

        if (hec == APIHEC_DROP_RESP || API_IS_ACK_SUPPRESS(pHdr))
        {

        }
        else if (hec == APIHEC_KEEP_API_DROP_RESP)
        {

        }
        else
        {
            SvsrSendAPI(pResp);
        }

    }

    if(pResp)
    	SCTP_Free_part(pResp);

    return ret;
}


/** @brief  This function parses RX API commands received on core-0 and routed to this core

    @param pChannel [in] - the context of communication channel

    @return [UINT32] error code of operation
    \ingroup group_lte_services
    */

UINT32 SvsrSubRxApiHandler (LPVOID pSvsrCtx)
{
    MXRC rc = MX_OK;

    ApiHeader * pApi, * pResp;
    ApiHandlerCtx * pClient;
    ICChannelCtx * pChannel;
    APIHEC hec;

    PSupervisorCtx pCtx = (PSupervisorCtx)pSvsrCtx;

    _ASSERT_PTR (pCtx);

    pChannel = pCtx->CommChannel;

    _ASSERT_PTR (pChannel);

    while ((pApi = SvsrGetSubApi (pCtx)) != NULL)
    {
        pResp = (ApiHeader *)ICLockTxData(pChannel);
        _ASSERT_PTR(pResp);

        SvsrDumpRecvApi (pApi);

        if(pResp == NULL)
        {
            ICFreeRxData(pChannel, pApi);
            return RC_SVSR_ALLOC_ERROR;
        }

        // To create a response (SrcInstID => DstInstID)
        ApiCreate(pResp, API_DEF_FLAGS, pApi->DstInstID, pApi->SrcInstID,
                   SvsrGetRespType(pApi->Type),pApi->MessageID);

        // To convert the global numeration to the local lower ARM numeration
        pApi->DstInstID = GET_LARM_INST(pApi->DstInstID);

        if (GetApiVersion(pApi) > API_SUPPORTED_VERSION)
        {
            ApiAddErrorCode(pResp, RC_SVSR_UNKNOWN_API_VER);

            SvsrDumpSendApi (pResp);

            pCtx->ICpuStat.TxMsgNum++;
            pCtx->ICpuStat.TxTime = MxGetTicks ();
            pCtx->ICpuStat.TxICpuApi = *pApi;

            // In this case we specially ignore the suppression bit
            MxCacheClean(pResp, ApiGetTotalSize (pResp));
            ICSendTxData(pChannel, pResp, TRUE);
        }
        else
        {
            pClient = SvsrGetClientCtx (pApi->DstInstID);

            if (pClient != NULL)
            {
                hec = SvsrProcClientApi (pChannel, pClient, pApi, &pResp);
            }
            else
            {
                hec = SvsrProcUnknownInst (pChannel, pApi, &pResp);
            }

            if (hec == APIHEC_DROP_RESP || API_IS_ACK_SUPPRESS(pApi))
            {
                ICFreeTxData (pChannel, pResp);
            }
            else if (hec == APIHEC_KEEP_API_DROP_RESP)
            {
                // in this case API command should be saved in I-CPU space
                // and the response should be dropped

                ICFreeTxData (pChannel, pResp);
                MxScheduleThread(pCtx->IMsgH);
                return rc;

            }
            else
            {
                SvsrDumpSendApi (pResp);

                pCtx->ICpuStat.TxMsgNum++;
                pCtx->ICpuStat.TxTime = MxGetTicks ();
                pCtx->ICpuStat.TxICpuApi = *pApi;

                MxCacheClean(pResp, ApiGetTotalSize (pResp));
                ICSendTxData(pChannel, pResp, TRUE);
            }
        }

        ICFreeRxData(pChannel, pApi);
        MxScheduleThread(pCtx->IMsgH);
        return rc;
    }

    return rc;
}


SvsrSysInfo SvsrGetSysInfo (void)
{
    SvsrSysInfo info;
    PSupervisorCtx pCtx = SvsrGetCtx ();

    _ASSERT_PTR(pCtx);

    info.nMaxCommBlockSize = ICGetSendBlockSize (pCtx->CommChannel);
    info.nMaxApiParamSize  = info.nMaxCommBlockSize - (API_HDR_SIZE + PARAM_HDR_SIZE);

    return info;
}

/** @brief Find out I-CPU Communication block size

    @return I-CPU Communication block size
*/
UINT32 SvsrGetCommBlockSize (void)
{
    PSupervisorCtx pCtx = SvsrGetCtx ();

    return pCtx->CommBlockSize;
}

MsgHandlerData * SvsrGetICoreMsg (PSupervisorCtx pCtx)
{
    MsgHandlerData * pMgsData = NULL;

    IRQTYPE irq = ARM_INT_disable ();
    MxGetLock (&pCtx->MsgSync);

    if (pCtx->MsgFirst != NULL)
    {
        pMgsData = pCtx->MsgFirst;
        pCtx->MsgFirst = pMgsData->Next;

        if (pCtx->MsgFirst == NULL)
            pCtx->MsgLast = NULL;
    }

    MxReleaseLock (&pCtx->MsgSync);
    ARM_INT_restore (irq);

    return pMgsData;
}

MsgHandlerData * SvsrGetISigMsg (PSupervisorCtx pCtx)
{
    MsgHandlerData * pMgsData = NULL;

    IRQTYPE irq = ARM_INT_disable ();
    MxGetLock (&pCtx->SigSync);

    if (pCtx->SigFirst != NULL)
    {
        pMgsData = pCtx->SigFirst;
        pCtx->SigFirst = pMgsData->Next;

        if (pCtx->SigFirst == NULL)
            pCtx->SigLast = NULL;
    }

    MxReleaseLock (&pCtx->SigSync);
    ARM_INT_restore (irq);

    return pMgsData;
}


INLINE void SvsrFreeICoreMsg (PSupervisorCtx pCtx, MsgHandlerData * pMsg)
{
    IRQTYPE irq = ARM_INT_disable ();
    MxGetLock (&pCtx->MsgSync);

    _ASSERT_PTR (pCtx);
    _ASSERT_PTR (pMsg);

    MxiFreeSysPart (&pCtx->MsgPart, pMsg);

    MxReleaseLock (&pCtx->MsgSync);
    ARM_INT_restore (irq);
}

INLINE void SvsrFreeISigMsg (PSupervisorCtx pCtx, MsgHandlerData * pMsg)
{
    IRQTYPE irq = ARM_INT_disable ();
    MxGetLock (&pCtx->SigSync);

    _ASSERT_PTR (pCtx);
    _ASSERT_PTR (pMsg);

    MxiFreeSysPart (&pCtx->SigPart, pMsg);

    MxReleaseLock (&pCtx->SigSync);
    ARM_INT_restore (irq);
}


UINT32 SvsrMsgHandler (LPVOID p)
{
    PSupervisorCtx pCtx = SvsrGetCtx ();
    MsgHandlerCtx * pMsgCtx;
    UINT32 res;
    MXRC rc = MX_OK;
    MsgHandlerData * pMgsData;

    pCtx->MsgProcFlags = 1;

    while ((pMgsData = SvsrGetICoreMsg (pCtx)) != NULL)
    {
        _ASSERT (pMgsData->MsgID < SVSR_CLIENT_NUM);

        pMsgCtx = &(pCtx->msghdlr [pMgsData->MsgID]);

#if 0
        if (pMsgCtx->Handler == NULL)
        {
            if (pMgsData->MsgPtr != NULL)
            {
                uart_printf ("SVSR:CPU-%d, ID=%d, Hdl=NULL, opt=%x, %x %x, ctx=%x\r\n",
                    MxGetCpuID (),
                    pMgsData->MsgID,
                    pMgsData->MsgOpt,
                    ((PUINT8)pMgsData->MsgPtr)[0],
                    ((PUINT8)pMgsData->MsgPtr)[1],
                    pCtx
                );
            }
            else
            {
                uart_printf ("SVSR:CPU-%d, ID=%d, Hdl=NULL, opt=%x, %x %x, ctx=%x\r\n",
                    MxGetCpuID (),
                    pMgsData->MsgID,
                    pMgsData->MsgOpt, pCtx);
            }
        }
#endif

        _ASSERT_PTR (pMsgCtx->Handler);

        if (pMsgCtx->Handler == NULL)
        {
            SvsrFreeICoreMsg(pCtx, pMgsData);
            continue;
        }

        if (pMsgCtx->RxApi != NULL)
            rc = pMsgCtx->RxApi (pMsgCtx->ClientCtx, pMgsData->MsgPtr);

        _ASSERT_RC (rc);

        if (SUCCESSFUL(rc))
        {
            pCtx->ICoreStat.RxMsgNum++;        
            pCtx->CurMsgTs = pMgsData->TimeStamp;
            res = (UINT32)pMsgCtx->Handler (pMsgCtx->ClientCtx, pMgsData->MsgPtr);
        }
        else
        {
            res = 0;
        }

        // to deallocate the message automatically

        if ((res & SVSR_MSG_OPT_SAVE) == 0)
        {
            if ((pMgsData->MsgOpt & SVSR_MSG_OPT_DEALLOC) && (pMgsData->MsgPtr != NULL))
            {
                SvsrFreeMsgEx (pMgsData->MsgPtr);
            }
        }

        SvsrFreeICoreMsg(pCtx, pMgsData);

    }

    pCtx->MsgProcFlags = 0;

    return 0;
}


UINT32 SvsrSigHandler (LPVOID p)
{
    PSupervisorCtx pCtx = SvsrGetCtx ();
    MsgHandlerCtx * pMsgCtx;
    UINT32 res;
    MXRC rc = MX_OK;
    MsgHandlerData * pMgsData;
    UINT32 nCurMsgTs = pCtx->CurMsgTs;
    UINT32 MsgProcFlags = pCtx->MsgProcFlags;

    pCtx->MsgProcFlags = 1;

    while ((pMgsData = SvsrGetISigMsg (pCtx)) != NULL)
    {
        _ASSERT (pMgsData->MsgID < SVSR_CLIENT_NUM);

        pMsgCtx = &(pCtx->msghdlr [pMgsData->MsgID]);

        _ASSERT_PTR (pMsgCtx->Handler);

        if (pMsgCtx->Handler == NULL)
        {
            SvsrFreeISigMsg(pCtx, pMgsData);
            continue;
        }

        if (pMsgCtx->RxApi != NULL)
            rc = pMsgCtx->RxApi (pMsgCtx->ClientCtx, pMgsData->MsgPtr);

        _ASSERT_RC (rc);

        if (SUCCESSFUL(rc))
        {
            pCtx->CurMsgTs = pMgsData->TimeStamp;
            res = (UINT32)pMsgCtx->Handler (pMsgCtx->ClientCtx, pMgsData->MsgPtr);
        }
        else
        {
            res = 0;
        }

        // to deallocate the message automatically

        if ((res & SVSR_MSG_OPT_SAVE) == 0)
        {
            if ((pMgsData->MsgOpt & SVSR_MSG_OPT_DEALLOC) && (pMgsData->MsgPtr != NULL))
            {
                SvsrFreeMsgEx (pMgsData->MsgPtr);
            }
        }

        SvsrFreeISigMsg(pCtx, pMgsData);

    }

    pCtx->MsgProcFlags = MsgProcFlags;
    pCtx->CurMsgTs = nCurMsgTs;

    return 0;
}



/** @brief This function is designed to process I-CORE messages in polling mode
           and mainly designed to be used in blocking thread(s)

    @return [MXRC] an error code
    \ingroup group_lte_services
*/
MXRC SvsrProcICoreMsg (void)
{
    SupervisorCtx* pCtx = SvsrGetCtx();

    _ASSERT_PTR(pCtx);

    SvsrMsgHandler (&pCtx->hMsgHandler);

    return MX_OK;
}

MXRC SvsrInitSubApiHandler (SupervisorCtx* pCtx)
{
    MXRC rc;
    UINT32 nElmCount;

    _ASSERT_PTR (pCtx);

    _ASSERT_PTR (pCtx->CommChannel);

    nElmCount = (ICGetRecvBlockCount (pCtx->CommChannel) + 1);

    if (MxGetCpuID () == 0)
    {
        pCtx->IMsgQStorage = MxHeapAlloc (AppHeap, nElmCount * sizeof (UINT32) * CPU_NUM);
    }
    else
    {
        //_ASSERT_PTR (SvsrCtx [0]);
        pCtx->IMsgQStorage = (SvsrCtx [0].IMsgQStorage + nElmCount * MxGetCpuID ());
    }

    _ASSERT_PTR (pCtx->MsgStorage);

    if (pCtx->IMsgQStorage == NULL)
        return RC_SVSR_NOT_MEMORY;

    pCtx->IMsgCount = nElmCount;

    rc = MxCreateThread (SvsrSubRxApiHandler, NULL, pCtx, 0, &pCtx->IMsgH);

    _ASSERT_RC (rc);

    if (FAILED(rc))
    {
        MxHeapFree (AppHeap, pCtx->IMsgQStorage);
        return rc;
    }

    return MX_OK;
}

/**
 * TODO add doxygen description
 */

MXRC SvsrChangeTransport(COMM_TYPE type, char *sctp_host, unsigned int sctp_port)
{
	MXRC rc = 0;

    if(type == COMM_TYPE_DIRECT_SCTP)
    {
    	if(!SCTPReady)
    	{
    		rc = SCTP_client_init(sctp_host, sctp_port, SvsrSctpRxApiHandler);

    		_ASSERT_RC (rc);

    		if (FAILED(rc))
    		{
   		    		return rc;
    		}

    		SCTPReady = TRUE;
    	}
    	TransportType = type;

    	rc = SvsrSctpSendReady(rc);

    	_ASSERT_RC (rc);

    	if (FAILED(rc))
    	{
    		return rc;
    	}

    }
    else
    	TransportType = type;

    return MX_OK;
}

/** @brif The function initializes a supervisor manager and opens inter-cpu
          communication channel used for interaction with upper ARM.

    @param id      [in] - the ID of communication channel (see:ICChannelID type)

    @return [MXRC] error code
    \ingroup group_lte_services
    */

MXRC SvsrInit (void)
{
    MXRC rc;
    SupervisorCtx* pCtx;
    ICChannelID id;

    id = (ICChannelID)CoreCfg.HostIntID;

    //pCtx = (SupervisorCtx*)MxHeapAlloc(AppHeap, sizeof (SupervisorCtx));

    //SvsrCtx [MxGetCpuID ()] = pCtx;
    pCtx = &SvsrCtx [MxGetCpuID ()];

    if (pCtx == NULL)
        return RC_SVSR_NOT_MEMORY;

    memset (pCtx, 0, sizeof (SupervisorCtx));

    TransportType = COMM_TYPE_ICPU;

    if(TransportType == COMM_TYPE_ICPU)
    {
    	if (MxGetCpuID () == 0)
    	{
    		rc = ICOpenChannel(id, SvsrRxApiHandler, pCtx,
                            &pCtx->CommChannel);
    	}
    	else
    	{
    		rc = ICOpenChannel(id, NULL, NULL, &pCtx->CommChannel);

    		_ASSERT_RC (rc);
    	}

    	_ASSERT_RC (rc);

    	if (FAILED(rc))
    	{
    		//MxHeapFree(AppHeap, pCtx);
    		//SvsrCtx [MxGetCpuID ()] = NULL;

    		return rc;
    	}
    	ICReady = TRUE;
    }
    else if(TransportType == COMM_TYPE_DIRECT_SCTP)
    {
    	rc = SCTP_client_init(SVSR_SCTP_DEFHOST, SVSR_SCTP_DEFPORT, SvsrSctpRxApiHandler);

    	uart_printf("svsr_init: rc = %d", rc);

    	_ASSERT_RC (rc);

    	if (FAILED(rc))
    	{
  	 		//MxHeapFree(AppHeap, pCtx);
  	  		//SvsrCtx [MxGetCpuID ()] = NULL;
    		return rc;
    	}

    	SvsrSctpSendReady(rc);

    	SCTPReady = TRUE;
    }

    rc = SvsrInitSubApiHandler(pCtx);

    _ASSERT_RC (rc);

    if (FAILED(rc))
    {
        //MxHeapFree(AppHeap, pCtx);
        //SvsrCtx [MxGetCpuID ()] = NULL;

        return rc;
    }

    pCtx->CommChannelID = id;
    pCtx->CommBlockSize = ICGetRecvBlockSize (pCtx->CommChannel);

    // To register supervisor API handler (instance 0 - an invalid instance trap)
    if (FAILED(rc = SvsrRegClient(IID_RESERVED, SvsrApiHandler, pCtx)))
    {
        ICCloseChannel(pCtx->CommChannel);
        //MxHeapFree(AppHeap, pCtx);
        //SvsrCtx [MxGetCpuID ()] = NULL;

        return rc;
    }

    // To register supervisor API handler (instance 1 - the Supervisor instance )
    if (FAILED(rc = SvsrRegClient(IID_SUPERVISOR, SvsrApiHandler, pCtx)))
    {
        ICCloseChannel(pCtx->CommChannel);
        //MxHeapFree(AppHeap, pCtx);
        //SvsrCtx [MxGetCpuID ()] = NULL;
    }

    // To register supervisor API handler (instance 29 - the Supervisor instance )
    // it's used to be compatible with diagnostics and make possible to be used with TTBox
    if (FAILED(rc = SvsrRegClient(IID_SVSR_MEM_CTRL, SvsrApiHandler, pCtx)))
    {
        ICCloseChannel(pCtx->CommChannel);
        //MxHeapFree(AppHeap, pCtx);
        //SvsrCtx [MxGetCpuID ()] = NULL;
    }

    rc = MxCreateThread (SvsrMsgHandler, NULL, pCtx, 0, &pCtx->hMsgHandler);

    if (FAILED(rc))
    {
        //MxHeapFree(AppHeap, pCtx);
        //SvsrCtx [MxGetCpuID ()] = NULL;
        return rc;
    }

    rc = MxCreateThread (SvsrSigHandler, NULL, pCtx, 0, &pCtx->hSigHandler);

    if (FAILED(rc))
    {
        //MxHeapFree(AppHeap, pCtx);
        //SvsrCtx [MxGetCpuID ()] = NULL;
        return rc;
    }

    rc = SvsrInitMsg (SVSR_MSG_PART_BLOCK_NUM, SVSR_MSG_PART_BLOCK_SIZE);

    if (FAILED(rc))
    {
        printf ("SVSR: There is not free space for I-MSG partition\r\n");
        return rc;
    }

    rc = SvsrInitSig (SVSR_MSG_PART_BLOCK_NUM, SVSR_MSG_PART_BLOCK_SIZE);

    if (FAILED(rc))
    {
        printf ("SVSR: There is not free space for I-SIG partition\r\n");
        return rc;
    }

    return rc;
}

/** @brief  The function sends to the upper-arm the supervisor
            ready message. This message contains error code of system initialization

    @param ErrorCode [in] - error code of system initialization
    @return [MXRC] operation error code
    \ingroup group_lte_services
    */

MXRC SvsrSendReady (UINT32 ErrorCode)
{
    ApiHeader * pApi;
    PSupervisorCtx pCtx = SvsrGetCtx ();

    if (pCtx == NULL)
        return RC_SVSR_NOT_INIT;

    pApi = (ApiHeader * )ICLockTxData(pCtx->CommChannel);
    if (pApi == NULL)
        return RC_SVSR_ALLOC_ERROR;

    pApi = ApiCreate(pApi,
                     API_DEF_FLAGS,
                     MAKE_LARM_INST(SVSR_INSTANCE_ID),
                     UARM_INSTID_HOST,
                     API_TYPE_IND,
                     SVSR_READY);

    ApiAddErrorCode(pApi, ErrorCode);

    // to update the I-CPU statistics

    pCtx->ICpuStat.TxMsgNum++;
    pCtx->ICpuStat.TxTime = MxGetTicks ();
    pCtx->ICpuStat.TxICpuApi = *pApi;

    SvsrDumpSendApi(pApi);

    MxCacheClean(pApi, ApiGetTotalSize(pApi));

    return ICSendTxData(pCtx->CommChannel, pApi, TRUE);
}

/** @brief  The function sends to the upper-arm the supervisor
            ready message. This message contains error code of system initialization

    @param ErrorCode [in] - error code of system initialization
    @return [MXRC] operation error code
    \ingroup group_lte_services
    */

MXRC SvsrSctpSendReady (UINT32 ErrorCode)
{
    ApiHeader * pApi;
    PSupervisorCtx pCtx = SvsrGetCtx ();

    if (pCtx == NULL)
        return RC_SVSR_NOT_INIT;

    pApi = (ApiHeader * )SCTP_Alloc_part();

    if (pApi == NULL)
        return RC_SVSR_ALLOC_ERROR;

    pApi = ApiCreate(pApi,
                     API_DEF_FLAGS,
                     MAKE_LARM_INST(SVSR_INSTANCE_ID),
                     UARM_INSTID_HOST,
                     API_TYPE_IND,
                     SVSR_READY);

    ApiAddErrorCode(pApi, ErrorCode);
    SvsrDumpSendApi(pApi);

    MxCacheClean(pApi, ApiGetTotalSize(pApi));

    return SvsrSendAPI(pApi);
}

/** @brief This function returns module type ID

    @param nInstID [in] - the predefined instance,  see: tagInstID of appids.h

    @return [UINT32] DrvModID type of  appids.h file
    \ingroup group_lte_services
    */

UINT32 SvsrGetMType (UINT32 nInstID)
{
    UINT32 i;

    i = 0;

    while (inst_type_id [i].InstID != 0)
    {
        if (inst_type_id [i].InstID == nInstID)
            return inst_type_id [i].MTypeID;

        i++;
    }

    return 0;
}

/** @brief The function registers a new API handler at supervisor layer. The
           API commands with specified instance ID will be routed to the API handler.

    @param nInstID    [in] - the API handler instance ID
    @param hand       [in] - the pointer to the function of API commands handler
    @param pClientData[in] - the pointer to the handler context data


    @return [MXRC] error code
    \ingroup group_lte_services
    */

MXRC SvsrRegClient (UINT32 nInstID, APIHandler hand, LPVOID pClientData)
{
    PSupervisorCtx pCtx = SvsrGetCtx ();

    _ASSERT(hand != NULL);
    _ASSERT(nInstID < SVSR_CLIENT_NUM);

    if (pCtx == NULL)
        return RC_SVSR_NOT_INIT;

    if (pCtx->clients [nInstID].Handler != NULL)
        return RC_SVSR_ALREADY_USED;

    memset (&pCtx->clients [nInstID], 0, sizeof (ApiHandlerCtx));

    pCtx->clients [nInstID].Handler = hand;
    pCtx->clients [nInstID].ClientCtx = pClientData;
    pCtx->clients [nInstID].ModTypeID = SvsrGetMType (nInstID);

    return MX_OK;
}

/** @brief This function removes API handler specified by the instance ID. The
           API commands sent to this instance ID will be rejected and host be
           notified of response with supervisor error code.

    @return nInstID [in] - the Instance ID allocated by calling SvsrRegClient

    @return [MXRC] error code
    \ingroup group_lte_services
    */

MXRC SvsrRemClient (UINT32 nInstID)
{
    PSupervisorCtx pCtx = SvsrGetCtx ();

    if (pCtx == NULL)
        return RC_SVSR_NOT_INIT;

    _ASSERT(nInstID < sizeof (pCtx->clients)/sizeof (pCtx->clients [0]));

    if(nInstID >= sizeof (pCtx->clients)/sizeof (pCtx->clients [0]))
        return SVSR_INVALID_INSTANCE;

    if (pCtx->clients [nInstID].AsmApiCmd != NULL)
    {
        MxHeapFree (AppHeap, pCtx->clients [nInstID].AsmApiCmd);
        pCtx->clients [nInstID].AsmApiCmd = NULL;
    }

    memset (&pCtx->clients [nInstID], 0, sizeof (pCtx->clients [0]));

    return MX_OK;
}

/** @brief This function sets logger function for the specified message handler
           if logger function is not NULL pointer, this function will be called to
           log the TX or RX API command.

    @param nInstID    [in] - the instance handler ID
    @param nCtrl      [in] - contrl mask, see: SVSR_LOGGER_xxx
    @param logger     [in] - the pointer to the logger function, it can be null to stop logging

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SvsrSetClientLogger (UINT32 nInstID, UINT32 nCtrl, ApiLoggerProc logger)
{
    PSupervisorCtx pCtx;

    pCtx = &SvsrCtx [MxGetCpuID()];

    _ASSERT_PTR (pCtx);

    if (nInstID >= SVSR_CLIENT_NUM)
        return RC_SVSR_INDEX_ERR;

    if (nCtrl & SVSR_LOGGER_RX)
    {
        pCtx->clients[nInstID].RxApi = logger;
    }

    if (nCtrl & SVSR_LOGGER_TX)
    {
        pCtx->clients[nInstID].TxApi = logger;
    }

    return MX_OK;
}

#if SvsrIcpuApiLogEna
void SvsrPrintOutLogBuffers(void)
{
    UINT32 i, k, Cntr;

    for(i = 0; i<CPU_NUM; i++)
    {
        uart_printf("\r\n-------- CPU%d API log--------\r\n", i);

        if(SvsrTxApiLogBufferCntrTot[i])
        {
            if(SvsrTxApiLogBufferCntrTot[i] >= SvsrIcpuApiLogStorage)
                Cntr = SvsrIcpuApiLogStorage;
            else
                Cntr = SvsrTxApiLogBufferCntrTot[i];

            uart_printf("\r\n-------- TX total number = %d currentIdx = %d-------\r\n",
                SvsrTxApiLogBufferCntrTot[i], SvsrTxApiLogBufferCntr[i]);

            for(k = 0; k<Cntr; k++)
            {
                SvsrAPIloggingItem* pBuf = &SvsrTxApiLogBuffer[k][i];
                ApiHeader* pApi = &pBuf->Hdr;
                uart_printf ("%d-time=%d. Ctrl: 0x%X Src: 0x%X Dst: 0x%X ID: 0x%X Type: 0x%X Len: %d W1=%X W2=%X (lr=%X)\r\n",
                k, pBuf->time,
                pApi->Control,
                pApi->SrcInstID,
                pApi->DstInstID,
                pApi->MessageID,
                pApi->Type,
                pApi->Length, pBuf->W1, pBuf->W2, pBuf->lr);

                MxDelayTicks(45000000);
            }
        }
        if(SvsrRxApiLogBufferCntrTot[i])
        {
            if(SvsrRxApiLogBufferCntrTot[i] >= SvsrIcpuApiLogStorage)
                Cntr = SvsrIcpuApiLogStorage;
            else
                Cntr = SvsrRxApiLogBufferCntrTot[i];

            uart_printf("\r\n-------- RX total number = %d currentIdx = %d-------\r\n",
                SvsrRxApiLogBufferCntrTot[i], SvsrRxApiLogBufferCntr[i]);

            for(k = 0; k<Cntr; k++)
            {
                SvsrAPIloggingItem* pBuf = &SvsrRxApiLogBuffer[k][i];
                ApiHeader* pApi = &pBuf->Hdr;
                uart_printf ("%d-time=%d. Ctrl: 0x%X Src: 0x%X Dst: 0x%X ID: 0x%X Type: 0x%X Len: %d W1=%X W2=%X\r\n",
                k, pBuf->time,
                pApi->Control,
                pApi->SrcInstID,
                pApi->DstInstID,
                pApi->MessageID,
                pApi->Type,
                pApi->Length, pBuf->W1, pBuf->W2);

                MxDelayTicks(45000000);
            }
        }
    }
}
#endif
/** @brief This function sends API command to the upper ARM by using inter-cpu
           communication channel. If API command length is more than size of
           supported block, the command will be automatically divided to the
           smaller blocks and be sent to the host by using protocol of sending
           large commands.

    @param pApi     [in] - the pointer to the API commamnd data

    @return [MXRC] error code of operation
    \ingroup group_lte_services
    */

MXRC SvsrSendAPI (ApiHeader * pApi)
{
#if SvsrIcpuApiLogEna
    UINT32 lr = __return_address() - 4;
#endif
    LPVOID pBlock;
    UINT32 nSrcID;
    MXRC rc = MX_OK;

    PSupervisorCtx pCtx = SvsrGetCtx ();

    _ASSERT(pApi != NULL);
    _ASSERT(pCtx != NULL);

    if (pCtx == NULL)
        return RC_SVSR_NOT_INIT;

    nSrcID = GET_LARM_INST(pApi->SrcInstID);

    if (nSrcID < SVSR_CLIENT_NUM && pCtx->clients [nSrcID].TxApi != NULL)
    {
        rc = pCtx->clients [nSrcID].TxApi (pCtx->clients [nSrcID].ClientCtx, pApi);

        _ASSERT_RC (rc);

        if (FAILED(rc))
            return rc;
    }

    pApi->SrcInstID = MAKE_LARM_INST (pApi->SrcInstID);

    // to update the I-CPU statistics

    pCtx->ICpuStat.TxMsgNum++;
    pCtx->ICpuStat.TxTime = MxGetTicks ();
    pCtx->ICpuStat.TxICpuApi = *pApi;

#if SvsrIcpuApiLogEna
   {
        UINT32 CpuID = MxGetCpuID();
        UINT32 Cntr = SvsrTxApiLogBufferCntr[CpuID];
        SvsrAPIloggingItem* pBuf = &SvsrTxApiLogBuffer[Cntr][CpuID];

        if (++Cntr >= SvsrIcpuApiLogStorage)
            Cntr = 0;
        SvsrTxApiLogBufferCntr[CpuID] = Cntr;
        SvsrTxApiLogBufferCntrTot[CpuID]++;

        pBuf->time = MxGetTicks();
        memcpy(&pBuf->Hdr, pApi, sizeof(ApiHeader));
        pBuf->W1 = *(UINT32*)(pApi + 1);
        pBuf->W2 = *((UINT32*)(pApi + 1) + 1);
        pBuf->lr = lr;
   }
#endif


    if(TransportType == COMM_TYPE_ICPU)
    {
        UINT32 rc;

    	if(!ICReady)
    		return RC_SVSR_API_DONOT_SUPP;

    	if (pApi->Length > SvsrGetSysInfo().nMaxCommBlockSize)
    	{
    		return SvsrSendExtApi(pApi->DstInstID,
                                pApi->SrcInstID,
                                pApi->MessageID,
                                pApi->Type,
                                GetApiPayload(pApi),
                                pApi->Length);
    	}

        if (ICIsTxMemory(pCtx->CommChannel, pApi))
        {
            pBlock = pApi;
        }
        else
        {
        	pBlock = ICLockTxData(pCtx->CommChannel);

        	if (pBlock == NULL)
        		return RC_SVSR_ALLOC_ERROR;

        	memcpy (pBlock, pApi, ApiGetTotalSize(pApi));
        }

    	MxCacheClean(pBlock, ApiGetTotalSize (pApi));

    	SvsrDumpSendApi (pApi);

        rc = ICSendTxData(pCtx->CommChannel, pBlock, TRUE);

        if(rc != 0)
        {
            uart_printf("SvsrSendAPI: rc=%X  !!! (tick=%d)\r\n", rc, MxGetTicks());

            uart_printf("CPU%d: freeTx=%d freeRx=%d\r\n", MxGetCpuID(),
                SvsrGetFreeBlocksICTxPart(), SvsrGetFreeBlocksICRxPart());
#if SvsrIcpuApiLogEna
            uart_printf("Dumping API log for all cores:\r\n");
            SvsrPrintOutLogBuffers();
#endif
        }

    	return rc;
    }
    else if(TransportType == COMM_TYPE_DIRECT_SCTP)
    {
    	if(!SCTPReady)
    	    return RC_SVSR_API_DONOT_SUPP;

    	SvsrDumpSendApi (pApi);

    	return SCTP_client_send((unsigned char *)pApi, ApiGetTotalSize(pApi));
    }

    return RC_SVSR_API_DONOT_SUPP;
}

MXRC SvsrSendAPIEx (ApiHeader * pApi, BOOL notify)
{
#if SvsrIcpuApiLogEna
    UINT32 lr = __return_address() - 4;
#endif
    LPVOID pBlock;
    UINT32 nSrcID;
    MXRC rc = MX_OK;

    PSupervisorCtx pCtx = SvsrGetCtx ();

    _ASSERT(pApi != NULL);
    _ASSERT(pCtx != NULL);

    if (pCtx == NULL)
        return RC_SVSR_NOT_INIT;

    nSrcID = GET_LARM_INST(pApi->SrcInstID);

    if (nSrcID < SVSR_CLIENT_NUM && pCtx->clients [nSrcID].TxApi != NULL)
    {
        rc = pCtx->clients [nSrcID].TxApi (pCtx->clients [nSrcID].ClientCtx, pApi);

        _ASSERT_RC (rc);

        if (FAILED(rc))
            return rc;
    }

    pApi->SrcInstID = MAKE_LARM_INST (pApi->SrcInstID);

    // to update the I-CPU statistics

    pCtx->ICpuStat.TxMsgNum++;
    pCtx->ICpuStat.TxTime = MxGetTicks ();
    pCtx->ICpuStat.TxICpuApi = *pApi;

#if SvsrIcpuApiLogEna
   {
        UINT32 CpuID = MxGetCpuID();
        UINT32 Cntr = SvsrTxApiLogBufferCntr[CpuID];
        SvsrAPIloggingItem* pBuf = &SvsrTxApiLogBuffer[Cntr][CpuID];

        if (++Cntr >= SvsrIcpuApiLogStorage)
            Cntr = 0;
        SvsrTxApiLogBufferCntr[CpuID] = Cntr;
        SvsrTxApiLogBufferCntrTot[CpuID]++;

        pBuf->time = MxGetTicks();
        memcpy(&pBuf->Hdr, pApi, sizeof(ApiHeader));
        pBuf->W1 = *(UINT32*)(pApi + 1);
        pBuf->W2 = *((UINT32*)(pApi + 1) + 1);
        pBuf->lr = lr;
   }
#endif


    if(TransportType == COMM_TYPE_ICPU)
    {
        UINT32 rc;

    	if(!ICReady)
    		return RC_SVSR_API_DONOT_SUPP;

    	if (pApi->Length > SvsrGetSysInfo().nMaxCommBlockSize)
    	{
    		return SvsrSendExtApi(pApi->DstInstID,
                                pApi->SrcInstID,
                                pApi->MessageID,
                                pApi->Type,
                                GetApiPayload(pApi),
                                pApi->Length);
    	}

        if (ICIsTxMemory(pCtx->CommChannel, pApi))
        {
            pBlock = pApi;
        }
        else
        {
        	pBlock = ICLockTxData(pCtx->CommChannel);

        	if (pBlock == NULL)
        		return RC_SVSR_ALLOC_ERROR;

        	memcpy (pBlock, pApi, ApiGetTotalSize(pApi));
        }

    	MxCacheClean(pBlock, ApiGetTotalSize (pApi));

    	SvsrDumpSendApi (pApi);

        rc = ICSendTxData(pCtx->CommChannel, pBlock, notify);

        if(rc != 0)
        {
            uart_printf("SvsrSendAPI: rc=%X  !!! (tick=%d)\r\n", rc, MxGetTicks());

            uart_printf("CPU%d: freeTx=%d freeRx=%d\r\n", MxGetCpuID(),
                SvsrGetFreeBlocksICTxPart(), SvsrGetFreeBlocksICRxPart());
#if SvsrIcpuApiLogEna
            uart_printf("Dumping API log for all cores:\r\n");
            SvsrPrintOutLogBuffers();
#endif
        }

    	return rc;
    }
    else if(TransportType == COMM_TYPE_DIRECT_SCTP)
    {
    	if(!SCTPReady)
    	    return RC_SVSR_API_DONOT_SUPP;

    	SvsrDumpSendApi (pApi);

    	return SCTP_client_send((unsigned char *)pApi, ApiGetTotalSize(pApi));
    }

    return RC_SVSR_API_DONOT_SUPP;
}



/** @brief This function sends API command to the upper ARM by using inter-cpu
           communication channel. The function is used to send linked list API only!

    @param pApi     [in] - the pointer to the API commamnd data

    @return [MXRC] error code of operation
    \ingroup group_lte_services
    */

MXRC SvsrSendAPILinkedList (ApiHeader * pApi)
{
    LPVOID pBlock;
    UINT32 nSrcID;
    MXRC rc = MX_OK;

    PSupervisorCtx pCtx = SvsrGetCtx ();

    _ASSERT(pApi != NULL);
    _ASSERT(pCtx != NULL);

    if (pCtx == NULL)
        return RC_SVSR_NOT_INIT;

    nSrcID = GET_LARM_INST(pApi->SrcInstID);

    if (nSrcID < SVSR_CLIENT_NUM && pCtx->clients [nSrcID].TxApi != NULL)
    {
        rc = pCtx->clients [nSrcID].TxApi (pCtx->clients [nSrcID].ClientCtx, pApi);

        _ASSERT_RC (rc);

        if (FAILED(rc))
            return rc;
    }

    pApi->SrcInstID = MAKE_LARM_INST (pApi->SrcInstID);

    // to update the I-CPU statistics

    pCtx->ICpuStat.TxMsgNum++;
    pCtx->ICpuStat.TxTime = MxGetTicks ();
    pCtx->ICpuStat.TxICpuApi = *pApi;

    if(TransportType == COMM_TYPE_ICPU)
    {
        UINT32 rc;

    	if(!ICReady)
    		return RC_SVSR_API_DONOT_SUPP;

        if (ICIsTxMemory(pCtx->CommChannel, pApi))
        {
            pBlock = pApi;
        }
        else
        {
        	pBlock = ICLockTxData(pCtx->CommChannel);

        	if (pBlock == NULL)
        		return RC_SVSR_ALLOC_ERROR;

        	memcpy (pBlock, pApi, ApiGetTotalSize(pApi));
        }

    	MxCacheClean(pBlock, ApiGetTotalSize (pApi));

        {// Clean cache for linked list API
            PLinkedListApiElem pElem;
            UINT32 guardCntr = 100;

            if(pApi->Length >= sizeof(ApiParam))
            {
                PApiParam pApiParam = (PApiParam)(pApi + 1);
                pElem = (PLinkedListApiElem)pApiParam->ParamData[0];

                while(pElem && guardCntr--)
                {
                    MxCacheClean(pElem, sizeof(*pElem));
                    MxCacheClean(pElem->MessagePtr, pElem->MessageLen);
                    pElem = pElem->Next;
                }
            }
            else if(pApi->Length >= sizeof(PUINT32))
            {
                UINT32 ApiData = *(PUINT32)(pApi + 1);
                pElem = (PLinkedListApiElem)ApiData;

                while(pElem && guardCntr--)
                {
                    MxCacheClean(pElem, sizeof(*pElem));
                    MxCacheClean(pElem->MessagePtr, pElem->MessageLen);
                    pElem = pElem->Next;
                }
            }
            if(guardCntr == 0)
            {
                uart_printf("SvsrSendAPILinkedList: broken chain !!!\r\n");
            }
        }

    	SvsrDumpSendApi (pApi);

        rc = ICSendTxData(pCtx->CommChannel, pBlock, TRUE);

        if(rc != 0)
        {
            uart_printf("SvsrSendAPILinkedList: rc=%X !!! (tick=%d)\r\n", rc, MxGetTicks());

            uart_printf("CPU%d: freeTx=%d freeRx=%d\r\n", MxGetCpuID(),
                SvsrGetFreeBlocksICTxPart(), SvsrGetFreeBlocksICRxPart());
        }

    	return rc;
    }
    else if(TransportType == COMM_TYPE_DIRECT_SCTP)
    {
    	if(!SCTPReady)
    	    return RC_SVSR_API_DONOT_SUPP;

    	SvsrDumpSendApi (pApi);

    	return SCTP_client_send((unsigned char *)pApi, ApiGetTotalSize(pApi));
    }

    return RC_SVSR_API_DONOT_SUPP;
}


/** @brief This function sends API command to the upper ARM with HI-priority
           by using inter-cpu communication channel.
           It's possible to use this API only in case of extended I-CPU

    @param pApi     [in] - the pointer to the API commamnd data

    @return [MXRC] error code of operation */

MXRC SvsrSendHiAPI (ApiHeader * pApi)
{
#if SvsrIcpuApiLogEna
    UINT32 lr = __return_address() - 4;
#endif
    LPVOID pBlock;
    UINT32 nSrcID;
    MXRC rc = MX_OK;

    PSupervisorCtx pCtx = SvsrGetCtx ();

    _ASSERT(pApi != NULL);
    _ASSERT(pCtx != NULL);

    if (pCtx == NULL)
        return RC_SVSR_NOT_INIT;

    if (pCtx->CommChannelID != IC_CHAN_SVSR_EX_ICPU)
        return RC_SVSR_IS_NOT_IMP;

    nSrcID = GET_LARM_INST(pApi->SrcInstID);

    if (nSrcID < SVSR_CLIENT_NUM && pCtx->clients [nSrcID].TxApi != NULL)
    {
        rc = pCtx->clients [nSrcID].TxApi (pCtx->clients [nSrcID].ClientCtx, pApi);

        _ASSERT_RC (rc);

        if (FAILED(rc))
            return rc;
    }

    pApi->SrcInstID = MAKE_LARM_INST (pApi->SrcInstID);

    // to update the I-CPU statistics

    pCtx->ICpuStat.TxMsgNum++;
    pCtx->ICpuStat.TxTime = MxGetTicks ();
    pCtx->ICpuStat.TxICpuApi = *pApi;

#if SvsrIcpuApiLogEna
   {
        UINT32 CpuID = MxGetCpuID();
        UINT32 Cntr = SvsrTxApiLogBufferCntr[CpuID];
        SvsrAPIloggingItem* pBuf = &SvsrTxApiLogBuffer[Cntr][CpuID];

        if (++Cntr >= SvsrIcpuApiLogStorage)
            Cntr = 0;
        SvsrTxApiLogBufferCntr[CpuID] = Cntr;
        SvsrTxApiLogBufferCntrTot[CpuID]++;

        pBuf->time = MxGetTicks();
        memcpy(&pBuf->Hdr, pApi, sizeof(ApiHeader));
        pBuf->W1 = *(UINT32*)(pApi + 1);
        pBuf->W2 = *((UINT32*)(pApi + 1) + 1);
        pBuf->lr = lr;
   }
#endif

    if(TransportType == COMM_TYPE_ICPU)
    {
        UINT32 rc;

    	if(!ICReady)
    		return RC_SVSR_API_DONOT_SUPP;

    	if (pApi->Length > SvsrGetSysInfo().nMaxCommBlockSize)
    	{
    		return RC_SVSR_SIZE_ERROR;
    	}

        if (ICIsTxMemory(pCtx->CommChannel, pApi))
        {
            pBlock = pApi;
        }
        else
        {
        	pBlock = ICLockTxData(pCtx->CommChannel);

        	if (pBlock == NULL)
        		return RC_SVSR_ALLOC_ERROR;

        	memcpy (pBlock, pApi, ApiGetTotalSize(pApi));
        }

    	MxCacheClean(pBlock, ApiGetTotalSize (pApi));

    	SvsrDumpSendApi (pApi);

        rc = ICSendHiTxData(pCtx->CommChannel, pBlock, TRUE);

        if(rc != 0)
        {
            uart_printf("SvsrSendAPI: rc=%X !!! (tick=%d)\r\n", rc, MxGetTicks());

            uart_printf("CPU%d: freeTx=%d freeRx=%d\r\n", MxGetCpuID(),
                SvsrGetFreeBlocksICTxPart(), SvsrGetFreeBlocksICRxPart());
#if SvsrIcpuApiLogEna
            uart_printf("Dumping API log for all cores:\r\n");
            SvsrPrintOutLogBuffers();
#endif
        }

    	return rc;
    }
    else if(TransportType == COMM_TYPE_DIRECT_SCTP)
    {
    	if(!SCTPReady)
    	    return RC_SVSR_API_DONOT_SUPP;

    	SvsrDumpSendApi (pApi);

    	return SCTP_client_send((unsigned char *)pApi, ApiGetTotalSize(pApi));
    }

    return RC_SVSR_API_DONOT_SUPP;
}

/** @brief This function sends a large data block to the host as extended API
           command with a protocol of sending extended API commands by dividing
           it to the set of fragments

    @param nInstID   [in] - the instance ID
    @param nMessID   [in] - the message ID
    @param nType     [in] - command type
    @param pData     [in] - the pointer to the API command data
    @param nDataSize [in] - the size of command payload in bytes

    @return [MXRC] error code of operation
    \ingroup group_lte_services
    */

MXRC SvsrSendExtApi (UINT16 nDstInstID, UINT16 nSrcInstID, UINT16 nMessID,
                    UINT16 nType, LPVOID pData, UINT32 nDataSize)
{
    UINT32 nChunkSize, AddHdrSize, nCopySize, nReqBlkNum;

    ExtApiHeader extapi;

    LPVOID pPacket;

    MXRC rc = MX_OK;

    PSupervisorCtx pCtx = SvsrGetCtx ();

    _ASSERT_PTR(pCtx);

    nChunkSize = ICGetSendBlockSize(pCtx->CommChannel);
    nChunkSize -= API_EXT_HDR_SIZE;

    _ASSERT(nChunkSize != 0);
    if(nChunkSize == 0)
        return RC_SVSR_PARAM_ERROR;

    // To check the accessible number of packets in communication channel
    nReqBlkNum = (nDataSize / nChunkSize) + ((nDataSize % nChunkSize) ? 1 : 0);

    if (nReqBlkNum > ICGetTxFreeBlocks(pCtx->CommChannel))
    {
        return RC_SVSR_NOT_MEMORY;
    }

    AddHdrSize = (API_EXT_HDR_SIZE - API_HDR_SIZE);

    extapi.Control    = API_VERSION(API_HEADER_VERSION_1)  |
                        API_EXTENDED_HEADER                |
                        API_FIRST_CHUNK;

    extapi.DstInstID  = nDstInstID;
    extapi.SrcInstID  = MAKE_LARM_INST(nSrcInstID);
    extapi.MessageID  = nMessID;
    extapi.Type       = nType;
    extapi.Length     = 0;
    extapi.TotalSize  = nDataSize;

    while (nDataSize != 0)
    {
        pPacket = ICLockTxData(pCtx->CommChannel);
        if (pPacket == NULL)
        {
            rc = RC_SVSR_ALLOC_ERROR;
            break;
        }

        nCopySize = MIN(nChunkSize, nDataSize);

        extapi.Length = (UINT16)(nCopySize + AddHdrSize);

        // Copy extended header of the API command
        memcpy (pPacket, &extapi, sizeof (extapi));

        // Copy command data
        memcpy (((PUINT8)pPacket + sizeof (extapi)), pData, nCopySize);

        MxCacheClean(pPacket, nCopySize + sizeof (extapi));

        // prepare data for the next loop operation
        nDataSize -= nCopySize;

        // Set offset in source buffer
        pData = (PUINT8)pData + nCopySize;

        SvsrDumpSendApi ((ApiHeader*)pPacket);

        // The allocated packet should be free by the upper ARM
        rc = ICSendTxData(pCtx->CommChannel, pPacket, (nDataSize == 0));
        if (FAILED(rc))
        {
            break;
        }

        extapi.Control &= ~API_FIRST_CHUNK;
    }

    return rc;
}

/** @brief This function is designed to load system information about
 *             threads, timers, heaps, partitions and so on ...
 *
 *  @param  pInfo [out] - the pointer to the storage
 *
    \ingroup group_lte_services
 * */

void SvsrLoadSysObjInfo (SvsrSysObjInfo * pInfo)
{
    UINT32 i = 0;
    HANDLE h;
    MXRC rc = MX_OK;

    _ASSERT_PTR (pInfo);

    pInfo->ThreadsNum = MxGetObjCount (OBJ_TYPE_THREAD);

#ifdef MX_EVENT_ENABLED
    pInfo->EventsNum = MxGetObjCount (OBJ_TYPE_EVENT);
#endif

#ifdef MX_LIST_ENABLED
    pInfo->ListsNum = MxGetObjCount (OBJ_TYPE_LIST);
#endif

#ifdef MX_TIMER_ENABLED
    pInfo->TimersNum = MxGetObjCount (OBJ_TYPE_TIMER);
#endif

#ifdef MX_QUEUE_ENABLED
    pInfo->QueueNum = MxGetObjCount (OBJ_TYPE_QUEUE);

    for (i = 0; i < pInfo->QueueNum; i++)
    {
        h = MxGetObjByIndex (OBJ_TYPE_QUEUE, i);

        rc = MxGetQueueDataCount (h, &pInfo->QueueSize [i]);

#ifdef _MX_DEBUG
        _ASSERT_RC (rc);
#else
        rc = rc;
#endif
    }
#endif

#ifdef MX_PART_ENABLED
    pInfo->PartNum = MxGetObjCount (OBJ_TYPE_PARTITION);

    for (i = 0; i < pInfo->PartNum; i++)
    {
        h = MxGetObjByIndex (OBJ_TYPE_PARTITION, i);

        pInfo->PartFreeBlk [i] = MxGetFreePartBlocks (h);
    }
#endif

#ifdef MX_HEAP_ENABLED
    pInfo->HeapNum = MxGetObjCount (OBJ_TYPE_HEAP);

    for (i = 0; i < pInfo->HeapNum; i++)
    {
        h = MxGetObjByIndex (OBJ_TYPE_HEAP, i);

        pInfo->HeapSize [i] = MxHeapGetFreeSpace(h);
    }
#endif

#ifdef MX_RHEAP_ENABLED
    pInfo->RHeapNum = MxGetObjCount (OBJ_TYPE_RHEAP);

    for (i = 0; i < pInfo->RHeapNum; i++)
    {
        h = MxGetObjByIndex (OBJ_TYPE_RHEAP, i);

        pInfo->RHeapSize [i] = MxRHeapGetFreeSpace(h);
    }
#endif
}

/** @brief This function is designed to store current size of data objects (heaps, partitions, queues ... )
 *         and should be called first before usage of SvsrCheckMemObjs
    \ingroup group_lte_services
 *  */

MXRC SvsrStoreSysObjs (void)
{
    SvsrLoadSysObjInfo (&SysObjInfoOrg);

    return MX_OK;
}

/** @brief This function is designed to check current size of data objects (heaps, partitions, queues ... )
 *         with prestored values (see: SvsrStoreMemObjs)
 *
 *  @param nSkipICpu [in] - if 1 - the function checks I-CPU space also,
 *                             0 - just ignores I-CPU and checks the rest system resources
 *  @param pResp [out] - the pointer to the response, formed with this function to be sent to the host with
 *                           detailed information about problem
 *
 *  @return MXRC
    \ingroup group_lte_services
 *
 * */

MXRC SvsrCheckSysObjs (UINT32 nSkipICpu, ApiHeader * pResp)
{
    MXRC rc = MX_OK;
    UINT32 i = 0, num;
    HANDLE h;

    PSupervisorCtx pCtx = SvsrGetCtx ();

    SvsrSysObjInfo * pOrg = &SysObjInfoOrg;
    SvsrSysObjInfo * pTst = &SysObjInfoTmp;

    _ASSERT_PTR (pCtx);

    SvsrLoadSysObjInfo (pTst);

    if (pOrg->ThreadsNum != pTst->ThreadsNum)
    {
        printf ("Threads num error (%d != %d)\r\n", pOrg->ThreadsNum, pTst->ThreadsNum);

        rc = RC_SVSR_THR_NUM_ERR;

        if (pResp != NULL)
        {
            ApiAddErrorCode (pResp, rc);

            ApiAddParam (pResp, NULL, 0x101, &rc, sizeof (rc));
            ApiAddParam (pResp, NULL, 0x102, &pOrg->ThreadsNum, sizeof (pOrg->ThreadsNum));
            ApiAddParam (pResp, NULL, 0x103, &pTst->ThreadsNum, sizeof (pTst->ThreadsNum));
        }
    }

#ifdef MX_EVENT_ENABLED
    if (pOrg->EventsNum != pTst->EventsNum)
    {
        printf ("Events num error (%d != %d)\r\n", pOrg->EventsNum, pTst->EventsNum);

        rc = RC_SVSR_EVN_NUM_ERR;

        if (pResp != NULL)
        {
            ApiAddErrorCode (pResp, rc);

            ApiAddParam (pResp, NULL, 0x201, &rc, sizeof (rc));
            ApiAddParam (pResp, NULL, 0x202, &pOrg->EventsNum, sizeof (pOrg->EventsNum));
            ApiAddParam (pResp, NULL, 0x203, &pTst->EventsNum, sizeof (pTst->EventsNum));
        }
    }
#endif

#ifdef MX_TIMER_ENABLED
    if (pOrg->TimersNum != pTst->TimersNum)
    {
        printf ("Timers num error (%d != %d)\r\n", pOrg->TimersNum, pTst->TimersNum);

        rc = RC_SVSR_TMR_NUM_ERR;

        if (pResp != NULL)
        {
            ApiAddErrorCode (pResp, rc);

            ApiAddParam (pResp, NULL, 0x301, &rc, sizeof (rc));
            ApiAddParam (pResp, NULL, 0x302, &pOrg->TimersNum, sizeof (pOrg->TimersNum));
            ApiAddParam (pResp, NULL, 0x303, &pTst->TimersNum, sizeof (pTst->TimersNum));
        }
    }
#endif

#ifdef MX_LIST_ENABLED
    if (pOrg->ListsNum != pTst->ListsNum)
    {
        printf ("Lists num error (%d != %d)\r\n", pOrg->ListsNum, pTst->ListsNum);

        rc = RC_SVSR_LST_NUM_ERR;

        if (pResp != NULL)
        {
            ApiAddErrorCode (pResp, rc);

            ApiAddParam (pResp, NULL, 0x401, &rc, sizeof (rc));
            ApiAddParam (pResp, NULL, 0x402, &pOrg->ListsNum, sizeof (pOrg->ListsNum));
            ApiAddParam (pResp, NULL, 0x403, &pTst->ListsNum, sizeof (pTst->ListsNum));
        }
    }
#endif

#ifdef MX_QUEUE_ENABLED
    if (pOrg->QueueNum != pTst->QueueNum)
    {
        printf ("Queues num error (%d != %d)\r\n", pOrg->QueueNum, pTst->QueueNum);

        rc = RC_SVSR_QUEUE_NUM_ERR;

        if (pResp != NULL)
        {
            ApiAddErrorCode (pResp, rc);

            ApiAddParam (pResp, NULL, 0x501, &rc, sizeof (rc));
            ApiAddParam (pResp, NULL, 0x502, &pOrg->QueueNum, sizeof (pOrg->QueueNum));
            ApiAddParam (pResp, NULL, 0x503, &pTst->QueueNum, sizeof (pTst->QueueNum));
        }
    }

    num = MIN (pTst->QueueNum, pOrg->QueueNum);

    for (i = 0; i < num; i++)
    {
        if (nSkipICpu != 0)
        {
            if (pOrg->QueueSize [i] != pTst->QueueSize [i])
            {
                h = MxGetObjByIndex (OBJ_TYPE_QUEUE, i);

                if (IS_HANDLE_EQUAL(h,pCtx->CommChannel->hRxQueue ))
                {
                    // to skip cheking I-CPU objects
                    continue;
                }

                if (IS_HANDLE_EQUAL(h,pCtx->CommChannel->hTxQueue))
                {
                    // to skip cheking I-CPU objects
                    continue;
                }
            }
        }

        if (pOrg->QueueSize [i] != pTst->QueueSize [i])
        {
            h = MxGetObjByIndex (OBJ_TYPE_QUEUE, i);

            rc = RC_SVSR_QUEUE_SIZE_ERR;

            printf ("Queue data error (h=%08x) (%d != %d)\r\n", *(UINT32*)&h, pOrg->QueueSize [i], pTst->QueueSize [i]);

            if (pResp != NULL)
            {
                ApiAddParam (pResp, NULL, 0x601, &h, sizeof (h));
                ApiAddParam (pResp, NULL, 0x602, &pOrg->QueueSize [i], sizeof (pOrg->QueueSize [i]));
                ApiAddParam (pResp, NULL, 0x603, &pTst->QueueSize [i], sizeof (pTst->QueueSize [i]));
            }
        }
    }

#endif

#ifdef MX_HEAP_ENABLED
    if (pOrg->HeapNum != pTst->HeapNum)
    {
        printf ("Heaps num error (%d != %d)\r\n", pOrg->HeapNum, pTst->HeapNum);

        rc = RC_SVSR_HEAP_NUM_ERR;

        if (pResp != NULL)
        {
            ApiAddErrorCode (pResp, rc);

            ApiAddParam (pResp, NULL, 0x701, &rc, sizeof (rc));
            ApiAddParam (pResp, NULL, 0x702, &pOrg->HeapNum, sizeof (pOrg->HeapNum));
            ApiAddParam (pResp, NULL, 0x703, &pTst->HeapNum, sizeof (pTst->HeapNum));
        }
    }

    num = MIN (pTst->HeapNum, pOrg->HeapNum);

    for (i = 0; i < num; i++)
    {
        if (pOrg->HeapSize [i] != pTst->HeapSize [i])
        {
            rc = RC_SVSR_HEAP_SIZE_ERR;

            h = MxGetObjByIndex (OBJ_TYPE_HEAP, i);

            printf ("Heap size error (h=%08x) (%d != %d)\r\n", *(UINT32*)&h, pOrg->HeapSize[i], pTst->HeapSize[i]);

            if (pResp != NULL)
            {
                ApiAddParam (pResp, NULL, 0x801, &h, sizeof (h));
                ApiAddParam (pResp, NULL, 0x802, &pOrg->HeapSize[i], sizeof (pOrg->HeapSize[i]));
                ApiAddParam (pResp, NULL, 0x803, &pTst->HeapSize[i], sizeof (pTst->HeapSize[i]));
            }
        }
    }

#endif

#ifdef MX_RHEAP_ENABLED
    if (pOrg->RHeapNum != pTst->RHeapNum)
    {
        printf ("R-Heaps error (%d != %d)\r\n", pOrg->RHeapNum, pTst->RHeapNum);

        rc = RC_SVSR_RHEAP_NUM_ERR;

        if (pResp != NULL)
        {
            ApiAddErrorCode (pResp, rc);

            ApiAddParam (pResp, NULL, 0x901, &rc, sizeof (rc));
            ApiAddParam (pResp, NULL, 0x902, &pOrg->RHeapNum, sizeof (pOrg->RHeapNum));
            ApiAddParam (pResp, NULL, 0x903, &pTst->RHeapNum, sizeof (pTst->RHeapNum));
        }
    }

    num = MIN (pTst->RHeapNum, pOrg->RHeapNum);

    for (i = 0; i < num; i++)
    {
        if (pOrg->RHeapSize [i] != pTst->RHeapSize [i])
        {
            rc = RC_SVSR_RHEAP_SIZE_ERR;

            h = MxGetObjByIndex (OBJ_TYPE_RHEAP, i);

            printf ("R-Heap size error (h=%08x) (%d != %d)\r\n", *(UINT32*)&h, pOrg->RHeapSize[i], pTst->RHeapSize[i]);

            if (pResp != NULL)
            {
                ApiAddParam (pResp, NULL, 0xA01, &h, sizeof (h));
                ApiAddParam (pResp, NULL, 0xA02, &pOrg->RHeapSize[i], sizeof (pOrg->RHeapSize[i]));
                ApiAddParam (pResp, NULL, 0xA03, &pTst->RHeapSize[i], sizeof (pTst->RHeapSize[i]));
            }
        }
    }

#endif

#ifdef MX_PART_ENABLED
    if (pOrg->PartNum != pTst->PartNum)
    {
        printf ("Parts num error (%d != %d)\r\n", pOrg->PartNum, pTst->PartNum);

        rc = RC_SVSR_PART_NUM_ERR;

        if (pResp != NULL)
        {
            ApiAddErrorCode (pResp, rc);

            ApiAddParam (pResp, NULL, 0xB01, &rc, sizeof (rc));
            ApiAddParam (pResp, NULL, 0xB02, &pOrg->PartNum, sizeof (pOrg->PartNum));
            ApiAddParam (pResp, NULL, 0xB03, &pTst->PartNum, sizeof (pTst->PartNum));
        }
    }

    num = MIN (pTst->PartNum, pOrg->PartNum);

    for (i = 0; i < num; i++)
    {
        if (nSkipICpu != 0)
        {
            if (pOrg->PartFreeBlk [i] != pTst->PartFreeBlk [i])
            {
                h = MxGetObjByIndex (OBJ_TYPE_PARTITION, i);

                if (IS_HANDLE_EQUAL(h,pCtx->CommChannel->hRxPart ))
                {
                    // to skip cheking I-CPU objects
                    continue;
                }

                if (IS_HANDLE_EQUAL(h,pCtx->CommChannel->hTxPart))
                {
                    // to skip cheking I-CPU objects
                    continue;
                }
            }
        }

        if (pOrg->PartFreeBlk [i] != pTst->PartFreeBlk [i])
        {
            rc = RC_SVSR_PART_SIZE_ERR;

            h = MxGetObjByIndex (OBJ_TYPE_PARTITION, i);

            printf ("Part free blocks error (h=%08x) (%d != %d)\r\n", *(UINT32*)&h, pOrg->PartFreeBlk[i], pTst->PartFreeBlk[i]);

            if (pResp != NULL)
            {
                ApiAddParam (pResp, NULL, 0xC01, &h, sizeof (h));
                ApiAddParam (pResp, NULL, 0xC02, &pOrg->PartFreeBlk[i], sizeof (pOrg->PartFreeBlk[i]));
                ApiAddParam (pResp, NULL, 0xC03, &pTst->PartFreeBlk[i], sizeof (pTst->PartFreeBlk[i]));
            }
        }
    }
#endif

    if (pResp != NULL)
    {
        ApiAddErrorCode (pResp, rc);
    }

    return rc;
}

/** @brief This function sets dump mask and enables/disables dumping of TX/RX API commands
    @param nMask [in] see SVSR_DUMP_xxx macro

    @return [MXRC]
    \ingroup group_lte_services
    */
MXRC SvsrSetDumpMask (UINT32 nMask)
{
    SvsrDumpMask = nMask;

    return MX_OK;
}

/** @brief This function is designed to free API command located in I-CPU storage
           and should be used for this API commands only

    @param pApi [in] - the pointer to the API command located in I-CPU storage

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

#ifdef SYS_TRACER_ENABLED
    MXRC SvsrFreeApi (ApiHeader * pApi)
    {
        UINT32 lr = MX_GET_RADDR;
        return SvsrFreeApiLR(pApi, lr);
    }

    MXRC SvsrFreeApiLR (ApiHeader * pApi, UINT32 lr)
#else
    MXRC SvsrFreeApi (ApiHeader * pApi)
#endif
{
    PSupervisorCtx pCtx = SvsrGetCtx ();

    _ASSERT_PTR (pApi);
    _ASSERT_PTR (pCtx);

    if(TransportType == COMM_TYPE_ICPU)
    {
        #ifdef SYS_TRACER_ENABLED
    	    return ICFreeRxDataLR (pCtx->CommChannel, pApi, lr);
        #else
            return ICFreeRxData (pCtx->CommChannel, pApi);
        #endif
    }
    else if(TransportType == COMM_TYPE_DIRECT_SCTP)
    	return SCTP_Free_part(pApi);
    else
    	return NULL;
}

/** @brief The function allocates new one block in I-CPU partition
           and returns a pointer to this block, this data can be used
           with functions: SvsrFreeTxApi or SvsrSendTxApi to free or to send data
           in case of sending, the destination point is responsible to free data

    @return  [ApiHeader *] the pointer to the API command or NULL
    \ingroup group_lte_services
    */

#ifdef SYS_TRACER_ENABLED
    ApiHeader * SvsrAllocICApi(void)
    {
        UINT32 lr = MX_GET_RADDR;
        return SvsrAllocICApiLR(lr);
    }

    ApiHeader * SvsrAllocICApiLR(UINT32 lr)
#else
    ApiHeader * SvsrAllocICApi(void)
#endif
{
    PSupervisorCtx pCtx = SvsrGetCtx ();
    _ASSERT_PTR (pCtx);

    if(TransportType == COMM_TYPE_ICPU)
    {
        #ifdef SYS_TRACER_ENABLED
    	    return ICLockTxDataLR(pCtx->CommChannel, lr);
        #else
            return ICLockTxData(pCtx->CommChannel);
        #endif
    }
    else if(TransportType == COMM_TYPE_DIRECT_SCTP)
    {
    	return SCTP_Alloc_part();
    }

    return NULL;
}

/** @brief This function allocates memory in the I-CPU partition specified
            with ID (RX I-CPU or TX I-CPU)

    @param  type[in] - the type of I-CPU memory, see: SVSR_I_CPU_RX or SVSR_I_CPU_TX

    @return [ApiHeader *] the pointer to the memory casted like API header
    \ingroup group_lte_services
*/
ApiHeader * SvsrAllocICApiEx(UINT32 type)
{
    PSupervisorCtx pCtx = SvsrGetCtx ();
    ApiHeader * pApi = NULL;

    _ASSERT_PTR (pCtx);


    if(TransportType == COMM_TYPE_ICPU)
    {
    	if (type == SVSR_I_CPU_TX)
   	        pApi = ICAllocTxData(pCtx->CommChannel);
   	    else if (type == SVSR_I_CPU_RX)
   	        pApi = ICAllocRxData(pCtx->CommChannel);
   	    else
   	        pApi = NULL;
    }
    else if(TransportType == COMM_TYPE_DIRECT_SCTP)
    {
      	pApi = SCTP_Alloc_part();
    }

    _ASSERT_PTR(pApi);

    return pApi;
}

/** @brief The function frees allocated in I-CPU partition block
           It can be used only for the data allocated with function SvsrAllocTxApi

    @return  [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SvsrFreeICApi (ApiHeader * pApi)
{
    #ifdef SYS_TRACER_ENABLED
    UINT32 lr = MX_GET_RADDR;
    #endif

    PSupervisorCtx pCtx = SvsrGetCtx ();

    _ASSERT_PTR (pApi);
    _ASSERT_PTR (pCtx);

    if(TransportType == COMM_TYPE_ICPU)
    {
        #ifdef SYS_TRACER_ENABLED
            return ICFreeTxDataLR (pCtx->CommChannel, pApi, lr);
        #else
            return ICFreeTxData (pCtx->CommChannel, pApi);
        #endif
    }
    else if(TransportType == COMM_TYPE_DIRECT_SCTP)
    	return SCTP_Free_part(pApi);
    else
    	return NULL;
}

/** @brief The function sends data, if used I-CPU, it can be used only for the data
           allocated with function SvsrAllocTxApi ONLY!!!

    @return  [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SvsrSendICApi (ApiHeader * pApi)
{
#if SvsrIcpuApiLogEna
    UINT32 lr = __return_address() - 4;
#endif
    PSupervisorCtx pCtx = SvsrGetCtx ();
    UINT32 nSrcID;
    MXRC rc = MX_OK;

    _ASSERT(pApi != NULL);
    _ASSERT(pCtx != NULL);

    if (pCtx == NULL)
    {
        return RC_SVSR_NOT_INIT;
    }

    nSrcID = GET_LARM_INST(pApi->SrcInstID);

    if (nSrcID < SVSR_CLIENT_NUM && pCtx->clients [nSrcID].TxApi != NULL)
    {
        rc = pCtx->clients [nSrcID].TxApi (pCtx->clients [nSrcID].ClientCtx, pApi);

        _ASSERT_RC (rc);

        if (FAILED(rc))
        {
            return rc;
        }
    }

    pApi->SrcInstID = MAKE_LARM_INST (pApi->SrcInstID);

    // to update the I-CPU statistics

    pCtx->ICpuStat.TxMsgNum++;
    pCtx->ICpuStat.TxTime = MxGetTicks ();
    pCtx->ICpuStat.TxICpuApi = *pApi;

#if SvsrIcpuApiLogEna
   {
        UINT32 CpuID = MxGetCpuID();
        UINT32 Cntr = SvsrTxApiLogBufferCntr[CpuID];
        SvsrAPIloggingItem* pBuf = &SvsrTxApiLogBuffer[Cntr][CpuID];

        if (++Cntr >= SvsrIcpuApiLogStorage)
            Cntr = 0;

        SvsrTxApiLogBufferCntr[CpuID] = Cntr;
        SvsrTxApiLogBufferCntrTot[CpuID]++;

        pBuf->time = MxGetTicks();
        memcpy(&pBuf->Hdr, pApi, sizeof(ApiHeader));
        pBuf->W1 = *(UINT32*)(pApi + 1);
        pBuf->W2 = *((UINT32*)(pApi + 1) + 1);
        pBuf->lr = lr;
   }
#endif

    MxCacheClean(pApi, ApiGetTotalSize (pApi));

    SvsrDumpSendApi (pApi);

    if(TransportType == COMM_TYPE_ICPU)
    {
#if SvsrIcpuApiLogEna
        UINT32 ticks = MxGetTicks();
#endif

        if(FAILED(rc = ICSendTxData(pCtx->CommChannel, pApi, TRUE)))
        {
#if SvsrIcpuApiLogEna

            UINT32 ticks2 = MxGetTicks();
            uart_printf("SvsrSendICAPI: rc=%X (t1=%d t2=%d ticksICSendTxData=%d)!!!\r\n CPU-%d: freeTx=%d freeRx=%d\r\n",
                rc, ticks, ticks2, ticks2 - ticks, MxGetCpuID(), SvsrGetFreeBlocksICTxPart(), SvsrGetFreeBlocksICRxPart());

            uart_printf("Dumping API log for all cores:\r\n");
            SvsrPrintOutLogBuffers();
#endif
        }

    	return rc;
    }
    else if(TransportType == COMM_TYPE_DIRECT_SCTP)
    {
    	if(!SCTPReady)
       		return RC_SVSR_API_DONOT_SUPP;

    	rc = SCTP_client_send((unsigned char *)pApi, ApiGetTotalSize(pApi));

    	SCTP_Free_part(pApi);

       	return rc;
    }
    else
    	return RC_SVSR_API_DONOT_SUPP;

}

MXRC SvsrSendICApiEx (ApiHeader * pApi, BOOL notify)
{
#if SvsrIcpuApiLogEna
    UINT32 lr = __return_address() - 4;
#endif
    PSupervisorCtx pCtx = SvsrGetCtx ();
    UINT32 nSrcID;
    MXRC rc = MX_OK;

    _ASSERT(pApi != NULL);
    _ASSERT(pCtx != NULL);

    if (pCtx == NULL)
    {
        return RC_SVSR_NOT_INIT;
    }

    nSrcID = GET_LARM_INST(pApi->SrcInstID);

    if (nSrcID < SVSR_CLIENT_NUM && pCtx->clients [nSrcID].TxApi != NULL)
    {
        rc = pCtx->clients [nSrcID].TxApi (pCtx->clients [nSrcID].ClientCtx, pApi);

        _ASSERT_RC (rc);

        if (FAILED(rc))
        {
            return rc;
        }
    }

    pApi->SrcInstID = MAKE_LARM_INST (pApi->SrcInstID);

    // to update the I-CPU statistics

    pCtx->ICpuStat.TxMsgNum++;
    pCtx->ICpuStat.TxTime = MxGetTicks ();
    pCtx->ICpuStat.TxICpuApi = *pApi;

#if SvsrIcpuApiLogEna
   {
        UINT32 CpuID = MxGetCpuID();
        UINT32 Cntr = SvsrTxApiLogBufferCntr[CpuID];
        SvsrAPIloggingItem* pBuf = &SvsrTxApiLogBuffer[Cntr][CpuID];

        if (++Cntr >= SvsrIcpuApiLogStorage)
            Cntr = 0;

        SvsrTxApiLogBufferCntr[CpuID] = Cntr;
        SvsrTxApiLogBufferCntrTot[CpuID]++;

        pBuf->time = MxGetTicks();
        memcpy(&pBuf->Hdr, pApi, sizeof(ApiHeader));
        pBuf->W1 = *(UINT32*)(pApi + 1);
        pBuf->W2 = *((UINT32*)(pApi + 1) + 1);
        pBuf->lr = lr;
   }
#endif

    MxCacheClean(pApi, ApiGetTotalSize (pApi));

    SvsrDumpSendApi (pApi);

    if(TransportType == COMM_TYPE_ICPU)
    {
#if SvsrIcpuApiLogEna
        UINT32 ticks = MxGetTicks();
#endif

        if(FAILED(rc = ICSendTxData(pCtx->CommChannel, pApi, notify)))
        {
#if SvsrIcpuApiLogEna

            UINT32 ticks2 = MxGetTicks();
            uart_printf("SvsrSendICAPI: rc=%X (t1=%d t2=%d ticksICSendTxData=%d)!!!\r\n CPU-%d: freeTx=%d freeRx=%d\r\n",
                rc, ticks, ticks2, ticks2 - ticks, MxGetCpuID(), SvsrGetFreeBlocksICTxPart(), SvsrGetFreeBlocksICRxPart());

            uart_printf("Dumping API log for all cores:\r\n");
            SvsrPrintOutLogBuffers();
#endif
        }

    	return rc;
    }
    else if(TransportType == COMM_TYPE_DIRECT_SCTP)
    {
    	if(!SCTPReady)
       		return RC_SVSR_API_DONOT_SUPP;

    	rc = SCTP_client_send((unsigned char *)pApi, ApiGetTotalSize(pApi));

    	SCTP_Free_part(pApi);

       	return rc;
    }
    else
    	return RC_SVSR_API_DONOT_SUPP;

}



/** @brief The function sends data, If used ICPU, it can be used only for the data
           allocated with function SvsrAllocTxApi ONLY!!!
           Don't modify sending API.

    @return  [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SvsrSendApiSimpy (ApiHeader * pApi)
{
    PSupervisorCtx pCtx = SvsrGetCtx ();
    MXRC rc = MX_OK;

    _ASSERT(pApi != NULL);
    _ASSERT(pCtx != NULL);

    if (pCtx == NULL)
    {
        return RC_SVSR_NOT_INIT;
    }

    MxCacheClean(pApi, ApiGetTotalSize (pApi));

    SvsrDumpSendApi (pApi);

    if(TransportType == COMM_TYPE_ICPU)
    {
    	return ICSendTxData(pCtx->CommChannel, pApi, TRUE);
    }
    else if(TransportType == COMM_TYPE_DIRECT_SCTP)
    {
    	if(!SCTPReady)
       		return RC_SVSR_API_DONOT_SUPP;

    	rc = SCTP_client_send((unsigned char *)pApi, ApiGetTotalSize(pApi));

    	SCTP_Free_part(pApi);

       	return rc;
    }
    else
    	return RC_SVSR_API_DONOT_SUPP;

}

/** @brief This function registers message handler

    @param nMsgID       [in] - message ID
    @param hdlr         [in] - handler
    @param pHandleData  [in] - handler data

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SvsrRegMsgHandler (UINT32 nMsgID, MsgHandler hdlr, LPVOID pHandleData)
{
    PSupervisorCtx ctx = &SvsrCtx [MxGetCpuID ()];

    _ASSERT_PTR (ctx);

    if (nMsgID >= SVSR_CLIENT_NUM)
        return RC_SVSR_INDEX_ERR;

    ctx->msghdlr [nMsgID].Handler   = hdlr;
    ctx->msghdlr [nMsgID].ClientCtx = pHandleData;

    return MX_OK;
}

/** @brief This function registers message handler on the remote core

    @param nCoreID      [in] - the core ID
    @param nMsgID       [in] - message ID
    @param hdlr         [in] - handler
    @param pHandleData  [in] - handler data

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SvsrRegMsgHandlerEx (UINT32 nCoreID, UINT32 nMsgID, MsgHandler hdlr, LPVOID pHandleData)
{
    PSupervisorCtx ctx;

    _ASSERT (nCoreID < SysGetCpuNum());

    ctx = &SvsrCtx [nCoreID];

    _ASSERT_PTR (ctx);

    if (nMsgID >= SVSR_CLIENT_NUM)
        return RC_SVSR_INDEX_ERR;

    ctx->msghdlr [nMsgID].Handler   = hdlr;
    ctx->msghdlr [nMsgID].ClientCtx = pHandleData;

    return MX_OK;
}

/** @brief This function return message handler
           registered for this message ID

    @param nMsgID [in] - the message ID
    @param pObj   [out]- the pointer to the message handler structure

    @return [MsgHandlerCtx] message handler
    \ingroup group_lte_services
    */

MXRC SvsrGetMsgHandler (UINT32 nMsgID, MsgHandlerCtx * pObj)
{
    PSupervisorCtx ctx = &SvsrCtx [MxGetCpuID ()];

    _ASSERT_PTR (ctx);

    if (nMsgID >= SVSR_CLIENT_NUM)
        return RC_SVSR_INDEX_ERR;

    *pObj = ctx->msghdlr [nMsgID];

    return MX_OK;
}

/** @brief This function installs CORE message handler

    @param nMsgID [in] - the message ID
    @param pObj [in] - the pointer to the handler context

    @return [MXRC] error code
    \ingroup group_lte_services
    */

MXRC SvsrSetMsgHandler (UINT32 nMsgID, MsgHandlerCtx * pObj)
{
    PSupervisorCtx ctx = &SvsrCtx [MxGetCpuID ()];

    _ASSERT_PTR (ctx);
    _ASSERT_PTR(pObj);

    if (nMsgID >= SVSR_CLIENT_NUM)
        return RC_SVSR_INDEX_ERR;

    ctx->msghdlr [nMsgID] = *pObj;

    return MX_OK;
}

/** @brief This function sends message to the
           remote core supervisor

    @param nCoreID [in] - the core ID [0 ... CPU_NUMS)
    @param nID     [in] - massage handler ID
    @param pMsg    [in] - the pointer to the message
    @param opt     [in] - message option (automatic deallocation, etc), see: SVSR_MSG_OPT_xxx

    @NOTE:
         Sender and receiver are responsible to allocate and free
         memory, used like a message storage, also it can be just a pointer
         to a global buffer

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SvsrSendMsg (UINT32 nCoreID, UINT32 nID, LPVOID pMsg, UINT32 opt)
{
    PSupervisorCtx rctx;
    MXRC rc = RC_SVSR_COREID_ERR;
    IRQTYPE irq;
    UINT32 bEmpty;
    MsgHandlerData * pMsgInfo;

    if (nCoreID >= CPU_NUM)
        return rc;

    rctx = &SvsrCtx [MxGetCpuID()];
    rctx->ICoreStat.TxMsgNum++;

    rctx = &SvsrCtx [nCoreID];

    _ASSERT_PTR (rctx);

    irq = ARM_INT_disable ();
    MxGetLock (&rctx->MsgSync);

    bEmpty = (rctx->MsgFirst == NULL);

    pMsgInfo = MxiAllocSysPart (&rctx->MsgPart);

    if (pMsgInfo == NULL)
    {
        MxReleaseLock (&rctx->MsgSync);
        ARM_INT_restore (irq);

        return RC_SVSR_ICORE_OVR;
    }

    pMsgInfo->MsgID  = (UINT16)nID;
    pMsgInfo->MsgOpt = (UINT16)opt;
    pMsgInfo->MsgPtr = pMsg;
    pMsgInfo->TimeStamp = GetTIMETICK();
    pMsgInfo->Next   = NULL;
    pMsgInfo->Prior  = 0;       // The default message priority

    if (rctx->MsgFirst == NULL)
    {
        rctx->MsgFirst = pMsgInfo;
        rctx->MsgLast  = pMsgInfo;
    }
    else
    {
        rctx->MsgLast->Next = pMsgInfo;
        rctx->MsgLast = pMsgInfo;
    }

    MxReleaseLock (&rctx->MsgSync);
    ARM_INT_restore (irq);

    if (bEmpty)
    {
        if (opt & SVSR_MSG_OPT_HIGH_PROC)
        {
            rc = MxScheduleThreadEx(rctx->hMsgHandler, 0, SCHED_AT_BEGIN);
        }
        else
        {
            rc = MxScheduleThread (rctx->hMsgHandler);
        }
    }
    else
        rc = MX_OK;

    return rc;
}

/** @brief This function sends signal to the
           remote core supervisor

    @param nCoreID [in] - the core ID [0 ... CPU_NUMS)
    @param nID     [in] - massage handler ID
    @param pMsg    [in] - the pointer to the message
    @param opt     [in] - message option (automatic deallocation, etc), see: SVSR_MSG_OPT_xxx

    @NOTE:
         Sender and receiver are responsible to allocate and free
         memory, used like a message storage, also it can be just a pointer
         to a global buffer

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SvsrSendSignal (UINT32 nCoreID, UINT32 nID, LPVOID pMsg, UINT32 opt)
{
    PSupervisorCtx rctx;
    MXRC rc = RC_SVSR_COREID_ERR;
    IRQTYPE irq;
    UINT32 bEmpty;
    MsgHandlerData * pMsgInfo;

    if (nCoreID >= CPU_NUM)
        return rc;

    rctx = &SvsrCtx [nCoreID];

    _ASSERT_PTR (rctx);

    irq = ARM_INT_disable ();
    MxGetLock (&rctx->SigSync);

    bEmpty = (rctx->SigFirst == NULL);

    pMsgInfo = MxiAllocSysPart (&rctx->SigPart);

    if (pMsgInfo == NULL)
    {
        MxReleaseLock (&rctx->SigSync);
        ARM_INT_restore (irq);

        return RC_SVSR_ICORE_OVR;
    }

    pMsgInfo->MsgID  = (UINT16)nID;
    pMsgInfo->MsgOpt = (UINT16)opt;
    pMsgInfo->MsgPtr = pMsg;
    pMsgInfo->TimeStamp = GetTIMETICK();
    pMsgInfo->Next   = NULL;
    pMsgInfo->Prior  = 0;       // The default message priority

    if (rctx->SigFirst == NULL)
    {
        rctx->SigFirst = pMsgInfo;
        rctx->SigLast  = pMsgInfo;
    }
    else
    {
        rctx->SigLast->Next = pMsgInfo;
        rctx->SigLast = pMsgInfo;
    }

    MxReleaseLock (&rctx->SigSync);
    ARM_INT_restore (irq);

    if (bEmpty)
        rc = MxInjectThread(nCoreID, rctx->hSigHandler);
    else
        rc = MX_OK;

    return rc;
}

/** @brief This function returns a number of accessible TX blocks in
           communication channel

    @return [UINT32]  number of block

    *  \ingroup group_lte_hal
    */
UINT32 SvsrGetFreeBlocksICTxPart(void)
{
     PSupervisorCtx pCtx;
     ICChannelCtx * pCommChannel;

     pCtx = SvsrGetCtx ();

     _ASSERT_PTR (pCtx);

     pCommChannel = pCtx->CommChannel;
     _ASSERT_PTR (pCommChannel);

     return ICGetTxFreeBlocks(pCommChannel);
}

/** @brief This function returns a number of accessible RX blocks in
           communication channel

    @return [UINT32]  number of block
    *  \ingroup group_lte_hal

    */
UINT32 SvsrGetFreeBlocksICRxPart(void)
{
     PSupervisorCtx pCtx;
     ICChannelCtx * pCommChannel;

     pCtx = SvsrGetCtx ();

     _ASSERT_PTR (pCtx);

     pCommChannel = pCtx->CommChannel;
     _ASSERT_PTR (pCommChannel);

     return ICGetRecvFreeBlocks(pCommChannel);
}

/** @brief This function sends message to the
           remote core supervisor

    @param nCoreID [in] - the core ID [0 ... CPU_NUMS)
    @param nID     [in] - massage handler ID
    @param pMsg    [in] - the pointer to the message
    @param MsgPrior[in] - the message priority, 0 - highest priority
    @param opt     [in] - message option (automatic deallocation, etc), see: SVSR_MSG_OPT_xxx

    @NOTE:
         Sender and receiver are responsible to allocate and free
         memory, used like a message storage, also it can be just a pointer
         to a global buffer

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SvsrSendMsgEx (UINT32 nCoreID, UINT32 nID, LPVOID pMsg, UINT32 MsgPrior, UINT32 opt)
{
    PSupervisorCtx rctx;
    MXRC rc = RC_SVSR_COREID_ERR;
    IRQTYPE irq;
    UINT32 bEmpty;
    MsgHandlerData * pMsgInfo, *SearchMsg, *PrevMsg = NULL;

    if (nCoreID >= CPU_NUM)
        return rc;

    rctx = &SvsrCtx [MxGetCpuID()];
    rctx->ICoreStat.TxMsgNum++;

    rctx = &SvsrCtx [nCoreID];

    _ASSERT_PTR (rctx);

    irq = ARM_INT_disable ();
    MxGetLock (&rctx->MsgSync);

    bEmpty = (rctx->MsgFirst == NULL);

    pMsgInfo = MxiAllocSysPart (&rctx->MsgPart);

    if (pMsgInfo == NULL)
    {
        MxReleaseLock (&rctx->MsgSync);
        ARM_INT_restore (irq);

        return RC_SVSR_ICORE_OVR;
    }

    pMsgInfo->MsgID  = (UINT16)nID;
    pMsgInfo->MsgOpt = (UINT16)opt;
    pMsgInfo->MsgPtr = pMsg;
    pMsgInfo->TimeStamp = GetTIMETICK();
    pMsgInfo->Next   = NULL;
    pMsgInfo->Prior  = MsgPrior;

    // to find the place in the list for the messages

    if (rctx->MsgFirst == NULL)
    {
        rctx->MsgFirst = pMsgInfo;
        rctx->MsgLast  = pMsgInfo;
    }
    else
    {
        // to find the proper message place according to the message priority

        SearchMsg = rctx->MsgFirst;

        while (SearchMsg != NULL)
        {
            if (MsgPrior < SearchMsg->Prior)
                break;

            PrevMsg   = SearchMsg;
            SearchMsg = SearchMsg->Next;
        }

        if (SearchMsg == NULL)
        {
            rctx->MsgLast->Next = pMsg;
            rctx->MsgLast = pMsg;
        }
        else if (PrevMsg != NULL)
        {
            pMsgInfo->Next = SearchMsg;
            PrevMsg->Next = pMsgInfo;
        }
        else
        {
            pMsgInfo->Next = rctx->MsgFirst;
            rctx->MsgFirst = pMsgInfo;
        }
    }

    MxReleaseLock (&rctx->MsgSync);
    ARM_INT_restore (irq);

    if (bEmpty)
        rc = MxScheduleThread (rctx->hMsgHandler);
    else
        rc = MX_OK;

    return rc;
}

/** @brief This function initializes partition used for I-COREs communication

    @param nElmNum    [in] - the number of blocks in partition
    @param nBlockSize [in] - the block size

    @NOTE:  This function should be called from CORE-0 ONLY!!!

    @return [MXRC ] an error code
    \ingroup group_lte_services
    */

MXRC SvsrInitMsg (UINT32 nElmNum, UINT32 nBlockSize)
{
    MXRC rc;

    HANDLE h = NcnbHeap;
    //HANDLE h = AppHeap;
    HANDLE hSM = AppHeap;
    UINT32 i;
    LPVOID pStor;

    SupervisorCtx* pCtx;
    pCtx = &SvsrCtx [MxGetCpuID ()];

    // The base initialization of I-CORE communication
    MxiDefSysPart (&pCtx->MsgPart, (LPVOID)(&pCtx->MsgStorage[0]), sizeof (pCtx->MsgStorage[0]), SVSR_MSG_NUM);

    pCtx->MsgFirst = NULL;
    pCtx->MsgLast  = NULL;
    pCtx->MsgSync  = 0;

    if (MxGetCpuID () != 0)
        return MX_OK;

    // Initialization of the I-CORE partitions
    // this initialization is done on CORE-0
    // and partitions are shared to be accessed from
    // other cores
    // the configuration is based on predefined configuration
    // please see ICorePartListCfg table

    i = 0;

    #ifdef SVSR_INIT_MSG_ENABLED
    uart_printf ("Supervisor I-CORE initialization:\r\n");
    #endif

    while (i < SVSR_ICORE_NUM)
    {
        if (ICorePartListCfg[i].Cached)
        {
            pStor = MxHeapAlloc (AppHeap, ICorePartListCfg[i].BlockNum * ICorePartListCfg[i].BlockSize);
        }
        else
        {
            pStor = MxHeapAlloc (NcnbHeap, ICorePartListCfg[i].BlockNum * ICorePartListCfg[i].BlockSize);
        }

        if (pStor == NULL)
        {
            _ASSERT_PTR(pStor);
            return RC_SVSR_ICORE_ALLOC_ERR;
        }

        ICorePartListCfg[i].StorPtr = pStor;

        rc = MxCreatePart(  pStor,
                            ICorePartListCfg[i].BlockNum,
                            ICorePartListCfg[i].BlockSize,
                            NULL,
                            HNULL,
                            HNULL,
                            &ICorePartListCfg[i].PartHandle);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return RC_SVSR_ICORE_MAKE_ERR;
        }

        rc = MxShareObject (ICorePartListCfg[i].PartHandle, OBJ_SHARE_ON);

        if (FAILED(rc))
        {
            _ASSERT_RC(rc);
            return RC_SVSR_ICORE_SHARE_ERR;
        }

        #ifdef SVSR_INIT_MSG_ENABLED
        uart_printf ("*(%d)x(% 6d Bytes), cache is %s - OK\r\n", ICorePartListCfg[i].BlockNum, ICorePartListCfg[i].BlockSize, ICorePartListCfg[i].Cached ? " ON":"OFF");
        #endif

        i++;
    }

    return rc;
}

/** @brief This function initializes partition used for I-SIGNALS communication

    @param nElmNum    [in] - the number of blocks in partition
    @param nBlockSize [in] - the block size

    @NOTE:  This function should be called from CORE-0 ONLY!!!

    @return [MXRC ] an error code
    \ingroup group_lte_services
    */

MXRC SvsrInitSig (UINT32 nElmNum, UINT32 nBlockSize)
{
    MXRC rc = MX_OK;

    HANDLE hSigHeap = AppHeap;

    SupervisorCtx* pCtx;
    pCtx = &SvsrCtx [MxGetCpuID ()];

    // The base initialization of I-SIGNAL communication
    MxiDefSysPart (&pCtx->SigPart, (LPVOID)(&pCtx->SigStorage[0]), sizeof (pCtx->SigStorage[0]), SVSR_MSG_NUM);

    pCtx->SigFirst = NULL;
    pCtx->SigLast  = NULL;
    pCtx->SigSync  = 0;

    return rc;
}

/** @brief This function allocates partition block in cacheable memory
           and returns pointers to this block or NULL.

    @NOTE: If block was used with SvsrSendMsg function, it should not be deallocated
           the system will deallocate it automatically

    @return [ApiHeader *] the pointer to an allocated block
    \ingroup group_lte_services
    */

#ifdef SYS_TRACER_ENABLED
    ApiHeader * SvsrAllocMsg (void)
    {
        UINT32 lr = MX_GET_RADDR;
        return SvsrAllocMsgLR(lr);
    }

    ApiHeader * SvsrAllocMsgLR (UINT32 lr)
#else
    ApiHeader * SvsrAllocMsg (void)
#endif
{
    ApiHeader * p;

    // To allocate the block by using the last partition
    // the last one has the biggest size of blocks

    #ifndef SYS_TRACER_ENABLED
        p = MxAllocBlock (ICorePartListCfg[SVSR_ICORE_NUM - 1].PartHandle);
    #else
        p = MxAllocBlockLR (ICorePartListCfg[SVSR_ICORE_NUM - 1].PartHandle, lr);
    #endif

    _ASSERT_PTR(p);

    return p;
}

#ifdef SYS_TRACER_ENABLED
    ApiHeader * SvsrAllocMsgEx (UINT32 size)
    {
        UINT32 lr = MX_GET_RADDR;
        return SvsrAllocMsgExLR(size, lr);
    }

    ApiHeader * SvsrAllocMsgExLR (UINT32 size, UINT32 lr)
#else
    ApiHeader * SvsrAllocMsgEx (UINT32 size)
#endif
{
    ApiHeader * p = NULL;
    UINT32 i = 0;

    if (MxGetCpuID() != 0)
        i = SVSR_ICORE_NUM-2;           // last 2 Blocks

    while (i < SVSR_ICORE_NUM)
    {
        if (size <= ICorePartListCfg[i].BlockSize)
        {
            #ifndef SYS_TRACER_ENABLED
                p = MxAllocBlock (ICorePartListCfg[i].PartHandle);
            #else
                p = MxAllocBlockLR (ICorePartListCfg[i].PartHandle, lr);
            #endif

            // if we cannot allocate memory in this partition
            // let's try to allocate data in the next partition
            // where a size of block is bigger but this is
            // OK for the application

            if ((p == NULL) && ((i + 1) < SVSR_ICORE_NUM))
            {
                i++;
                continue;
            }

            _ASSERT_PTR(p);
            return p;
        }

        i++;
    }

    _ASSERT_PTR(p);
    return NULL;
}

/** @brief This function frees block manually, it should be used
           only in case when block was not send to remote core

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SvsrFreeMsg (LPVOID pMsg)
{
    return SvsrFreeMsgEx(pMsg);
}

/** @brief This function check is memory from I-CORE partition
           The I-CORE partition is specified by <idx> parameter

    @param idx [in] - the partition index (< SVSR_ICORE_NUM)
    @param pMem[in] - the pointer to the block of I-CORE partition

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

INLINE BOOL SvsrIsICoreMem (UINT32 idx, LPVOID pMem)
{
    UINT32 nSize;

    if (idx >= SVSR_ICORE_NUM)
        return FALSE;

    nSize = ICorePartListCfg[idx].BlockNum * ICorePartListCfg[idx].BlockSize;

    return (((UINT32)pMem >= (UINT32)ICorePartListCfg[idx].StorPtr) &&
         (UINT32)pMem < ((UINT32)ICorePartListCfg[idx].StorPtr + nSize));
}

/** @brief This function frees block manually from any msg partition, it should be used
           only in case when block was not send to remote core

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

#ifdef SYS_TRACER_ENABLED
    MXRC SvsrFreeMsgEx (LPVOID pMsg)
    {
        UINT32 lr = MX_GET_RADDR;
        return SvsrFreeMsgExLR(pMsg, lr);
    }

    MXRC SvsrFreeMsgExLR (LPVOID pMsg, UINT32 lr)
#else
    MXRC SvsrFreeMsgEx (LPVOID pMsg)
#endif
{
    UINT32 i = 0;
    _ASSERT_PTR (pMsg);

    while (i < SVSR_ICORE_NUM)
    {
        if (SvsrIsICoreMem(i, pMsg))
        {
            #ifdef SYS_TRACER_ENABLED
                return MxFreeBlockLR (ICorePartListCfg[i].PartHandle, pMsg, lr);
            #else
                return MxFreeBlock (ICorePartListCfg[i].PartHandle, pMsg);
            #endif
        }

        i++;
    }

    return RC_SVSR_NOT_ICORE_MEM;
}

/** @brief This function sets logger function for the specified message handler
           if logger function is not NULL pointer, this function will be called to
           log the TX or RX API command.

    @param nMsgHdlrID [in] - the message handler ID
    @param nCtrl      [in] - contrl mask, see: SVSR_LOGGER_xxx
    @param logger     [in] - the pointer to the logger function, it can be null to stop logging

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SvsrSetMsgLogger (UINT32 nMsgHdlrID, UINT32 nCtrl, ApiLoggerProc logger)
{
    PSupervisorCtx pCtx;

    pCtx = &SvsrCtx [MxGetCpuID()];

    _ASSERT_PTR (pCtx);

    if (nMsgHdlrID >= SVSR_CLIENT_NUM)
        return RC_SVSR_INDEX_ERR;

    if (nCtrl & SVSR_LOGGER_RX)
    {
        pCtx->msghdlr[nMsgHdlrID].RxApi = logger;
    }

    if (nCtrl & SVSR_LOGGER_TX)
    {
        pCtx->msghdlr[nMsgHdlrID].TxApi = logger;
    }

    return MX_OK;
}

/** @brief This function return the timestamp for the current message,
            this function should be called from message handler, in
            other case timestamp is meaningless

    @return [UINT32] the message timestamp (when message was putted to the I-CORE queue)
    \ingroup group_lte_services
    */

UINT32 SvsrGetMsgTimestamp (void)
{
    PSupervisorCtx pCtx;

    pCtx = &SvsrCtx [MxGetCpuID()];

    _ASSERT_PTR (pCtx);

    if (pCtx->MsgProcFlags == 0)
        return 0;

    return pCtx->CurMsgTs;
}


/** @brief This function is designed to automatically find out what is
           the communication interface used (I-CORE or I-CPU), which partition was used and to call
           an interface function to free already allocated data

    @param pData [in] - the pointer to the block of memory

    @return [UINT32] an error code
    \ingroup group_lte_services
    */

#ifdef SYS_TRACER_ENABLED

    MXRC SvsrFreeIMem (LPVOID pData)
    {
        UINT32 lr = MX_GET_RADDR;
        return SvsrFreeIMemLR (pData, lr);
    }

    MXRC SvsrFreeIMemLR (LPVOID pData, UINT32 lr)
#else
    MXRC SvsrFreeIMem (LPVOID pData)
#endif
{
    PSupervisorCtx pCtx = SvsrGetCtx ();
    UINT32 rc;

    _ASSERT_PTR (pCtx);
    _ASSERT_PTR (pData);

    #ifndef SYS_TRACER_ENABLED
    if ((rc = SvsrFreeMsgEx(pData)) != RC_SVSR_NOT_ICORE_MEM)
    #else
    if ((rc = SvsrFreeMsgExLR(pData, lr)) != RC_SVSR_NOT_ICORE_MEM)
    #endif
        return rc;

    if (ICIsRxMemory(pCtx->CommChannel, pData))
    {
        #ifndef SYS_TRACER_ENABLED
            return SvsrFreeApi((ApiHeader*)pData);
        #else
            return SvsrFreeApiLR((ApiHeader*)pData, lr);
        #endif
    }

    if (ICIsTxMemory(pCtx->CommChannel, pData))
    {
        return SvsrFreeICApi((ApiHeader*)pData);
    }

    return MX_OK;
}

/** @brief This function returns a number of I-CORE blocks that
    can be allocated by the application at this moment

    @return [UINT32] the number of available I-CORE blocks
    \ingroup group_lte_services
    */

UINT32 SvsrGetICoreNum (void)
{
    return MxGetFreePartBlocks(ICorePartListCfg[SVSR_ICORE_NUM - 1].PartHandle);
}

/** @brief This function returns a number of I-CORE blocks that
    can be allocated by the application at this moment

    @param block_size[in] - the size of block in bytes

    @return [UINT32] the number of available I-CORE blocks
    \ingroup group_lte_services
    */

UINT32 SvsrGetICoreNumEx (UINT32 block_size)
{
    UINT32 i = 0;

    while (i < SVSR_ICORE_NUM)
    {
        if (ICorePartListCfg[i].BlockSize <= block_size)
        {
            return MxGetFreePartBlocks(ICorePartListCfg[i].PartHandle);
        }

        i++;
    }

    return 0;
}

/** @brief This function puts MSPD I-CPU commands to the RX partition
            just to emulate remote I-CPU sender and generates interrupt
            to start processing this command

    @param pApi [in] - the pointer to the API command allocated in RX I-CPU partition!!!

    @return [MXRC] an error code
    \ingroup group_lte_services
    */

MXRC SvsrPutICpuApiToRx (ApiHeader * pApi)
{
    PSupervisorCtx pCtx = SvsrGetCtx ();

    _ASSERT_PTR (pCtx);
    _ASSERT_PTR(pApi);

    MxCacheClean(pApi, ApiGetTotalSize (pApi));

    return ICAddRxMessage (pCtx->CommChannel, pApi, 1);
}

/** @brief This function return I-CPU statistics for the specified core

    @param CpuID [in] - the CPU ID
    @param pStat [out]- the pointer to the statistics object

    @return [MXRC] an error code

    \ingroup group_lte_services

    */

MXRC SvsrGetICpuStatistics (UINT32 CpuID, SvsrApiStat * pStat)
{
    if (CpuID >= SysGetCpuNum ())
        return RC_SVSR_CPUID_ERROR;

    if (pStat == NULL)
        return RC_SVSR_PARAM_ERROR;

    *pStat = SvsrCtx [CpuID].ICpuStat;

    return MX_OK;
}

/** @brief This function return I-CORE statistics for the specified core

    @param CpuID [in] - the CPU ID
    @param pStat [out]- the pointer to the statistics object

    @return [MXRC] an error code

    \ingroup group_lte_services

    */

MXRC SvsrGetICoreStatistics (UINT32 CpuID, SvsrMsgStat * pStat)
{
    if (CpuID >= SysGetCpuNum ())
        return RC_SVSR_CPUID_ERROR;

    if (pStat == NULL)
        return RC_SVSR_PARAM_ERROR;

    *pStat = SvsrCtx [CpuID].ICoreStat;

    return MX_OK;
}


/** @brief The function packs specified heap, the packet data represents current heap state
           and maybe used for further analyzing

    @param hHeap    [in] - the heap handle
    @param id       [in] - the heap id, just used to identify heap
    @param pStor    [out]- the storage, where heap info can be stored
    @param nStorSize[in] - the storage size in bytes

    @return [MXRC] an error code

    \ingroup group_lte_services

    */

#ifdef MX_HEAP_LEAKAGE_CONTROL
MXRC SvsrPackHeap (HANDLE hHeap, UINT32 id, LPVOID pStor, UINT32 nStorSize)
{
    return MxHeapPack (hHeap, id, pStor, nStorSize);
}
#endif

/** @brief This function selects I-CPU master CPU, master CPU is the CPU
           defined to process I-CPU IRQ signals and call API handlers or route
           API commands to the destination cores

    @param nMasterCpu [in] - the master CPU, limited with CPU_NUM

    @return [MXRC] an error code of operation */

MXRC SvsrSelectICpuMaster (UINT32 nMasterCpu)
{
    return ICSelectMasterCpu (nMasterCpu);
}

#ifdef SYS_TRACER_ENABLED

/** @brief This function attaches supervisor communication object to the OS tracer

    @param nTrID    [in] - the communication object ID see: [TID_I_CPU_TX  ... TID_I_CORE_30K]
    @param pTraceH  [out]- the trace handler

    @return [LPVOID] the trace storage or NULL if error */

LPVOID SvsrEnableTrace (UINT32 nTrID, HANDLE * pTraceH)
{
    PSupervisorCtx pCtx = SvsrGetCtx ();
    void * p = NULL;
    UINT32 nSize = 0;
    HANDLE hObj;
    MXRC rc;

    _ASSERT_PTR(pTraceH);
    _ASSERT_PTR(pCtx);

    switch (nTrID)
    {
        case TID_I_CPU_TX:
        {
            if (pCtx->CommChannel->ChanID != IC_CHAN_SVSR_EX_ICPU)
            {
                nSize = MxTraceCalcStorSize(OBJ_TYPE_PARTITION, MxGetPartBlockCount(pCtx->CommChannel->hTxPart));
                hObj  = pCtx->CommChannel->hTxPart;
            }
            break;
        }

        case TID_I_CPU_RX:
        {
            if (pCtx->CommChannel->ChanID != IC_CHAN_SVSR_EX_ICPU)
            {
                nSize = MxTraceCalcStorSize(OBJ_TYPE_PARTITION, MxGetPartBlockCount(pCtx->CommChannel->hRxPart));
                hObj  = pCtx->CommChannel->hRxPart;
            }
            break;
        }

        case TID_I_CORE_0:
        {
            hObj  = ICorePartListCfg[0].PartHandle;
            nSize = MxTraceCalcStorSize(OBJ_TYPE_PARTITION, MxGetPartBlockCount(hObj));

            break;
        }

        case TID_I_CORE_1:
        {
            hObj  = ICorePartListCfg[1].PartHandle;
            nSize = MxTraceCalcStorSize(OBJ_TYPE_PARTITION, MxGetPartBlockCount(hObj));

            break;
        }

        case TID_I_CORE_2:
        {
            hObj  = ICorePartListCfg[2].PartHandle;
            nSize = MxTraceCalcStorSize(OBJ_TYPE_PARTITION, MxGetPartBlockCount(hObj));

            break;
        }

        case TID_I_CORE_3:
        {
            hObj  = ICorePartListCfg[3].PartHandle;
            nSize = MxTraceCalcStorSize(OBJ_TYPE_PARTITION, MxGetPartBlockCount(hObj));

            break;
        }
    }

    if (nSize == 0)
    {
        _ASSERT(nSize != 0);
        return NULL;
    }

    p = MxHeapAlloc (AppHeap, nSize);

    if (p == NULL)
    {
        _ASSERT_PTR(p);
        return NULL;
    }

    rc = SysEnableTrace (nTrID, hObj, nSize, pTraceH);

    if (FAILED(rc))
    {
        _ASSERT_RC(rc);
        MxHeapFree (AppHeap, p);

        return p;
    }

    uart_printf ("Svsr(cpu=%d): TraceEnabled for Obj=%d, ptr=0x%08x\r\n", MxGetCpuID(), nTrID, p);
    return p;
}

/** @brief To detach the tracer and to stop tracing, the storage has to be manually
           deallocated by using a pointer returned by SvsrEnableTrace and SvsrFreeTraceStorage

    @param nTrID [in] - the tracer ID, see [TID_I_CPU_TX  ... TID_I_CORE_3]

    @return [MXRC] an error code */

MXRC SvsrDisableTrace(UINT32 nTrID)
{
    PSupervisorCtx pCtx = SvsrGetCtx ();
    HANDLE hObj = HNULL;

    _ASSERT_PTR(pCtx);

    switch (nTrID)
    {
        case TID_I_CPU_TX:
        {
            if (pCtx->CommChannel->ChanID != IC_CHAN_SVSR_EX_ICPU)
            {
                hObj  = pCtx->CommChannel->hTxPart;
            }
            break;
        }

        case TID_I_CPU_RX:
        {
            if (pCtx->CommChannel->ChanID != IC_CHAN_SVSR_EX_ICPU)
            {
                hObj  = pCtx->CommChannel->hRxPart;
            }
            break;
        }

        case TID_I_CORE_0:
        {
            hObj  = ICorePartListCfg[0].PartHandle;
            break;
        }

        case TID_I_CORE_1:
        {
            hObj  = ICorePartListCfg[1].PartHandle;
            break;
        }

        case TID_I_CORE_2:
        {
            hObj  = ICorePartListCfg[2].PartHandle;
            break;
        }

        case TID_I_CORE_3:
        {
            hObj  = ICorePartListCfg[3].PartHandle;
            break;
        }
    }

    if (HANDLE_NULL(hObj))
        return RC_SVSR_TRACE_ID_ERROR;

    return SysDisableTrace(hObj);
}

/** @brief To deallocate trace storage allocated by SvsrEnableTrace

    @NOTE!  It has t be used only after SvsrDisableTrace function

    @param p [in] - the pointer returned by SvsrEnableTrace
    */

void SvsrFreeTraceStorage (void * p)
{
    _ASSERT_PTR(p);
    MxHeapFree (AppHeap, p);
}

#endif

