//-------------------------------------------------------------------------------------------
/** @file DOX_INTERNAL lte_entry.c
 *
 * @brief The entry point of the LTE application
 * @author Mindspeed Technologies
 * @version $Revision: 1.158 $
 *
 * COPYRIGHT&copy; 2008-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 **/
//-------------------------------------------------------------------------------------------

#include "ltephy.h"
#include <string.h>
#include "lte_entry.h"
#include "ext_phy.h"
#include "appprintf.h"
#include "supervisor.h"
#include "heaps.h"
#include "apimgr.h"
#include "larmmsgid.h"
#include "diagnostics.h"
#include "LtePhyL2Api.h"
#include "phyprofiler.h"
#include "phydatacomm.h"
#include "timerdrv.h"
#include "stream.h"
#include "cmgr.h"
#include "prototypes.h"
#include "LteBsTxThread.h"
#include "LteBsRxThread.h"
#include "LteBsRxPrachThread.h"
#include "LteBsTxDataSdu.h"
#include "LteBsRxDataSdu.h"
#include "LteBsApiThread.h"
#include "LteBsRxConfig.h"
#include "cpridrv.h"
#include "globalstoragearrays.h"
#include "evmfpga.h"
#include "ltertconfig.h"
#include "svsrlogger.h"
#include "phydi.h"
#include "lte_bs_phy_nmm_thread.h"

#include "dataint.h"
#include "mlog.h"

#include "UMTSNwlMsgPro.h"

extern UINT32 SvsrGetSize (void);


// Each application module must define this variable
MXRC (*AppInitCallback)(void) = LTEInitHandler;

short MacCpuID;

extern UINT32 LteRxDataInit(UINT32* ptr, UINT32 MsgSize);
extern PUINT8 LteGetMlogInformation(U32* MsgSize);
extern PUINT8 LteGetSvsrInformation(U32* MsgSize);
extern PUINT8 LteGetPhyStatsInformation(U32* MsgSize);
static MXRC LteGetRxSduBuf(LPVOID pParam, ApiHeader *pApi, ApiHeader * pResp);
static MXRC LteGetTxLogBufIq(LPVOID pParam, ApiHeader *pApi, ApiHeader * pResp);
static MXRC LteSetRxLogBufIq(LPVOID pParam, ApiHeader *pApi, ApiHeader * pResp);
static MXRC LteMlogOut(ApiHeader * pApi, ApiHeader * pResp);
static MXRC LteMlogSize(ApiHeader * pApi, ApiHeader * pResp);
static MXRC LteSvsrLogOut(ApiHeader * pApi, ApiHeader * pResp);
static MXRC LteSvsrLogSize(ApiHeader * pApi, ApiHeader * pResp);
static MXRC LtePhyStatsOut(ApiHeader * pApi, ApiHeader * pResp);
static MXRC LtePhyStatsSize(ApiHeader * pApi, ApiHeader * pResp);
LTEAppCtx *pLteCtx = NULL;


__align(4) const char PhyCreateCommand[]    = "createphy";
__align(4) const char PhyStartCommand[]     = "phystart";
__align(4) const char PhyStopCommand[]      = "phystop";
__align(4) const char PhyShutdownCommand[]  = "physhutdown";
__align(4) const char PhyShutdownCommand2[] = "phyend";
__align(4) const char PhyStatsCommand[]     = "phystats";
__align(4) const char PhyMemStatCommand[]   = "phymemstat";
__align(4) const char PhyPauseCommand[]     = "phypause";
__align(4) const char PhyResumeCommand[]    = "phyresume";
__align(4) const char PhyLogsCommand[]      = "phylogs";
__align(4) const char PhyReconfigCommand[]  = "phyreconfig";
__align(4) const char nmm_start_command[]   = "nmmstart";
__align(4) const char nmm_stop_command[]    = "nmmstop";
__align(4) const char RadioSaveCommand[]     = "radiosave";



#define MAC_PROCESSING_THREAD_ARM_ID ((MxGetARMID () == 0) ? 3 : 1)

/** @brief LteSendMsgToMac:function to send messages to the MAC layer
 *
 *
 *  <1> Hardware Resource:
 *  ARM
 *
 *
 *  @param outMsg     Pointer to the LTE API output Message to be written
 *                    an encapsulated under the Mindspeed generic message
 *                    format described in MSA-81479 Turner MAC PHY API
 *                    with MessageID 0x7000 Foreign API Message.
 *
 *  @param outMsgSize Size in bytes of the output message
 *
 *  @return rc   	  Status of the execution of this function
 *
 *
 *  <2> Parameter Properties:
 *
 *  TABLE_START
 *  --------------------------------------------------------------------------------------
 *  |Name       |Name In  |Direction  |Length |Type          |Format  |Valid Range|Units |
 *  |           |Standard |           |       |              |        |           |      |
 *  --------------------------------------------------------------------------------------
 *  |outMsg     |         |pinput     |  1    | pUnsigned int|(1:31:0)|(0,2^32)   |      |
 *  --------------------------------------------------------------------------------------
 *  |outMsgSize |		  |input      |	 1    |	Unsigned int |(1:31:0)|[4,16368]  |      |
 *  --------------------------------------------------------------------------------------
 *  |pApi       |         |poutput    |  1    |	pUnsigned int|(1:31:0)|(0,2^32)   |      |
 *  --------------------------------------------------------------------------------------
 *
 *  TABLE_END
 *
 *
 *  <3> Block Diagram:
 *
 *  BLOCK_START
 *  ----------------------------------------
 *  |Block Name          |LteSendMsgToMac  |
 *  ----------------------------------------
 *  |Input Stream(s)     |outMsg,outMsgSize|
 *  ----------------------------------------
 *  |Output Stream(s)    |pApi             |
 *  ----------------------------------------
 *  |Config Parameter(s) |                 |
 *  -------------------------------------- -
 *  |Status Parameter(s) |rc               |
 *  ----------------------------------------
 *  BLOCK_END
 *
 *
 * <4> Detailed Description:
 * This function encapusalates a Mindspeed PHY SAP Interface message into
 * a Mindspeed Turner MAC & PHY API generic message using an encapsulated
 * foreign api message type
 *
 *
 * <5> Cycle Count Formula:
 * TBD
 *
 *
 * <6> References:
 * Mindspeed "Turner MAC & PHY API", Mindspeed "PHY SAP Interface Specification
 * for LTE Broadband Wireless Access Base Stations"
 *
 *
 * \ingroup group_lte_api
 *
 **/
U32 LteSendMsgToMac(U32* outMsg, U32 outMsgSize)
{
    MXRC rc = MX_OK;
    ApiHeader * pApi;
    U32 tick = GetTick();

    _ASSERT_PTR(outMsg);

    if (GetMacInstanceId() == UARM_INSTID_DEFMAC)
    {// if remote peer is real MAC use real ICPU
        pApi = (ApiHeader *) ((U8*)outMsg - sizeof(ApiHeader));
        // The command size was increased to the total size of parameter
        pApi->Length = outMsgSize;
        SvsrSendICApi(pApi);
        MLogTask(PID_API_SEND_TO_LINUX_MAC, RESOURCE_LARM, tick, GetTick());

        return rc;
    }
    else if (GetMacInstanceId())
    {// it's for neighbour MAC, use fake ICPU
        SvsrSendMsg(MAC_PROCESSING_THREAD_ARM_ID, GetMacInstanceId(), outMsg, SVSR_MSG_OPT_DEALLOC);
        MLogTask(PID_API_SEND_TO_4GMX_MAC, RESOURCE_LARM, tick, GetTick());
    }

    return rc;
}

U32 LteSendMsgToUpperNmm(U32* outMsg, U32 outMsgSize)
{
    MXRC rc = MX_OK;
    ApiHeader * pApi;
    U32 tick = GetTick();

    _ASSERT_PTR(outMsg);

    if (GetUNmmInstanceId() == UARM_INSTID_DEFMAC
        || GetUNmmInstanceId() == UARM_INSTID_DEF_UNMM)
    {// if remote peer uses ICPU
        pApi = (ApiHeader *) ((U8*)outMsg - sizeof(ApiHeader));
        // The command size was increased to the total size of parameter
        pApi->Length = outMsgSize;
        SvsrSendICApi(pApi);
        MLogTask(PID_NMMAPI_SEND_TO_LINUX_MAC, RESOURCE_LARM, tick, GetTick());

        return rc;
    }
    else if (GetUNmmInstanceId())
    {// it's for neighbour upper NMM, use fake ICORE
        SvsrSendMsg(MAC_PROCESSING_THREAD_ARM_ID, GetUNmmInstanceId(), outMsg, SVSR_MSG_OPT_DEALLOC);
        MLogTask(PID_NMMAPI_SEND_TO_4GMX_MAC, RESOURCE_LARM, tick, GetTick());
    }

    return rc;
}

void LteSendListToRmtMac(UINT32 localHead)
{
    LTEAppCtx   *pCtx = pLteCtx;
    ApiHeader   *pApi;

    if (localHead)
    {
        pApi = (ApiHeader *) pCtx->msgBuf;
        _ASSERT_PTR(pApi);
        if (pApi == NULL)
            return;

        ApiCreate(pApi, API_ENCAP_FM, pCtx->nInstanceID, GetMacInstanceId(), API_TYPE_IND,
                  PHY_LTE_MSG_LIST);

        ApiAddParam(pApi, NULL, PHY_LTE_MSG_LIST, &localHead, sizeof(UINT32));

        SvsrSendAPILinkedList(pApi);
    }
    return;
}

//#define STOP_MEMORY_LEAK

#ifdef STOP_MEMORY_LEAK

extern ICORE_PART_CFG ICorePartListCfg[SVSR_ICORE_NUM];

void stopNoBuffer(U8* ptr, U32 ret_add)
{
    if (ptr == NULL)
    {
        uart_printf("stopNoBuffer - Null vector at 0x%08x\r\n", ret_add);

        uart_printf("I-Core Info\r\n");
        CLineCmdTestICore(NULL, NULL);

        uart_printf("I-Cpu Info\r\n");
        CLineCmdGetInfo(NULL, NULL);


        stop_printf("NULL\r\n");
    }
}
#else
#define stopNoBuffer(ptr, ret_add)
#endif



U8* Phy2MacIfAllocBuffer(U32 size)
{
    U8 *ptr = NULL;
    int addr = __return_address() - 4;

    if(GetMacInstanceId() == UARM_INSTID_DEFMAC)
    {
        ptr = (U8*)SvsrAllocICApi();
        stopNoBuffer(ptr, addr);

        return ptr;
    }

    ptr = (U8*)SvsrAllocMsgEx(size);
    stopNoBuffer(ptr, addr);

    return ptr;
}

U8* Phy2MacIfAllocNcNbBuffer(U32 size)
{
    U8 *ptr = NULL;
    int addr = __return_address() - 4;

    if(GetMacInstanceId() == UARM_INSTID_DEFMAC)
    {
        ptr = (U8*)SvsrAllocICApi();
        stopNoBuffer(ptr, addr);

        return ptr;
    }

    ptr = (U8*)SvsrAllocMsg();
    stopNoBuffer(ptr, addr);
    return ptr;
}

U8* Phy2MacIfApiBuffer(U32 size)
{
    U8 *ptr = NULL;
    int addr = __return_address() - 4;

    if(GetMacInstanceId() == UARM_INSTID_DEFMAC)
    {
        ApiHeader * pApi = SvsrAllocICApi();
        if(pApi)
        {
            ApiCreate(pApi, API_ENCAP_FM, pLteCtx->nInstanceID,
                GetMacInstanceId(), API_TYPE_IND, PHY_LTE_MSG);
            ptr = ((U8*)pApi + sizeof(ApiHeader));
        }
        stopNoBuffer(ptr, addr);
        return ptr;
    }

    ptr = (U8*)SvsrAllocMsgEx(size);
    stopNoBuffer(ptr, addr);
    return (U8*)ptr;
}

U8* PhyNMM2MacIfApiBuffer(U32 size)
{
    U8 *ptr = NULL;
    int addr = __return_address() - 4;

    if(GetUNmmInstanceId() == UARM_INSTID_DEFMAC
        || GetUNmmInstanceId() == UARM_INSTID_DEF_UNMM)
    {
        ApiHeader * pApi = SvsrAllocICApi();
        if(pApi)
        {
            ApiCreate(pApi, API_ENCAP_FM, pLteCtx->nNMMInstanceID,
                GetUNmmInstanceId(), API_TYPE_IND, PHY_LTE_NMM_MSG);
            ptr = ((U8*)pApi + sizeof(ApiHeader));
        }
        stopNoBuffer(ptr, addr);
        return ptr;
    }

    ptr = (U8*)SvsrAllocMsgEx(size);
    stopNoBuffer(ptr, addr);
    return (U8*)ptr;
}

MXRC LteMsgHandlerEx(LPVOID pClientCtx, LPVOID pMsg) // The response for API command
{
    LtePhyMessagePush(pMsg, FIFO_MAC_TO_PHY_API, 0, 0);

    return SVSR_MSG_OPT_SAVE;
}

/** @brief DOX_INTERNAL This function will be called by the supervisor in case of receiving
 API command with the LTE instance ID

 @param pClientCtx [in] -is not used
 @param pApi       [in] - pointer to the received API command
 @param pResp      [out]- pointer to the response for the API command

 @NOTE: 1. The response is automatically allocated by the supervisor
 2. If function does not want to send a response, it should return APIHEC_DROP_RESP value


 @return [APIHEC] the control code of response control
 **/
MXRC LteApiHandlerEx(LPVOID pClientCtx, ApiHeader *pApi, ApiHeader *pResp) // The response for API command
{
    PLTE_PHY_API_THREAD_CTX pThCtx = LtePhyApiGetCtx();
    ApiParam * pParam;
    PMAC2PHY_QUEUE_EL pElm;
    //PGENMSGDESC pPhyMsg;

    _ASSERT_PTR(pApi);
    pParam = ApiGetParamByID(pApi, PHY_QUE_HDR_PARAM_ID);
    _ASSERT_PTR(pParam);

    if (pParam != NULL)
    {
        pThCtx->pMsg = (PMAC2PHY_QUEUE_EL)pParam->ParamData[0];
        _ASSERT_PTR(pThCtx->pMsg);

        // we need to invalidate the list of blocks

        pElm = pThCtx->pMsg;

        while (pElm != NULL)
        {
            // to invalidate the list element
            MxCacheInvalidate(pElm, sizeof (MAC2PHY_QUEUE_EL));

            _ASSERT_PTR(pElm->MessagePtr);

            if (pElm->MessagePtr != NULL)
            {
                MxCacheInvalidate(pElm->MessagePtr, pElm->MessageLen);

                if(pElm->MessageType == PHY_TXSDU_REQ)
                {
                    PTXSDUREQ pTxSduReq = (PTXSDUREQ) pElm->MessagePtr;
                    if(gUseTxSduPointer)
                    {// means it carries pointer inside
                        MxCacheInvalidate(pTxSduReq->pTxSdu, pTxSduReq->msgLen);
                    }
                }
            }

            pElm = pElm->Next;
        }

        if (GetMacInstanceId() == 0)
            SetMacInstanceId(pApi->SrcInstID);

        LtePhyMessagePush((PMAC2PHY_QUEUE_EL)pParam->ParamData[0], FIFO_MAC_TO_PHY_API, 0, 0);

        return APIHEC_DROP_RESP;
    }
    else
    {
        ApiAddErrorCode(pApi, RC_LTE_PARAM_ERROR);
    }

    return APIHEC_OK;
}


// Not Supported anymore
MXRC LteMsgHandler(LPVOID pClientCtx, LPVOID pMsg) // The response for API command
{
    uart_printf("LteMsgHandler: Not Supported Anymore\r\n");
    return MX_OK;
}


/** @brief DOX_INTERNAL This function will be called by the supervisor in case of receiving
 API command with the LTE instance ID

 @param pClientCtx [in] - pointer to the LTE context (LTEAppCtx *, please see LTEInit)
 @param pApi       [in] - pointer to the received API command
 @param pResp      [out]- pointer to the response for the API command

 @NOTE: 1. The response is automatically allocated by the supervisor
 2. If function does not want to send a response, it should return APIHEC_DROP_RESP value


 @return [APIHEC] the control code of response control
 **/
APIHEC LTEAPIHandler(LPVOID pClientCtx, ApiHeader *pApi, ApiHeader *pResp)
{
    MXRC rc = MX_OK;
    LTEAppCtx * pCtx = (LTEAppCtx *) pClientCtx;
    ApiParam * pParam;

    if (pCtx == NULL)
    {
        rc = RC_PHY_CONTEXT_ALLOC_ERROR;
        ApiAddErrorCode(pResp, rc);
        return APIHEC_OK;
    }

    // The API command can be parsed/created
    // by using API functions of <apimgr.h> file
    if (pApi->Control & API_ENCAP_FM)
    {
        uart_printf("LTEAPIHandler: API_ENCAP_FM Not Supported Anymore\r\n");
        uart_printf("Message MsgID=%X Type=%X was not processed\r\n", pApi->MessageID, pApi->Type);
        return APIHEC_OK;
    }
    else if (pApi->Type == API_TYPE_CFG_PARAM)
    {
        pResp->Type = API_TYPE_CFG_RESP;

        switch (pApi->MessageID)
        {
            case PHY_LTE_START_SECTOR:
                break;
            case PHY_LTE_STOP_SECTOR:
                break;
            case PHY_LTE_PHY_CONTEXT:
                break;
            case PHY_LTE_RX_IQ:
                break;
            case PHY_LTE_TX_IQ:
                break;
            case PHY_LTE_TASK_LIST:
                break;

            case PHY_LTE_TIMER_RESUME:
                rc = PhyDiResume();
                break;

            case PHY_LTE_TIMER_PAUSED:
                {
                    UINT32 pause_thr = ApiGetParamU32 (pApi, 2, 0);
                    UINT32 skip_thr  = ApiGetParamU32 (pApi, 3, 0);
                    UINT32 host_id   = ApiGetParamU32 (pApi, 4, 0);

                    rc = PhyDiPauseSetRunCtrl (pause_thr, skip_thr, host_id);
                }

                break;

            case PHY_SETRXBUF:
                rc = LteSetRxLogBufIq(pCtx, pApi, pResp);
                ApiAddErrorCode(pResp, rc);
                break;

            case DIAG_INPUT:
                uart_printf("LTEAPIHandler: DIAG_INPUT message not supported anymore\r\n");
                break;

            case PHY_LTE_DEBUG_MSG:
                uart_printf("LTEAPIHandler: PHY_LTE_DEBUG_MSG message not supported anymore\r\n");
                break;

            case PHY_LTE_CPTIBILITY_MSG:
                uart_printf("LTEAPIHandler: PHY_LTE_CPTIBILITY_MSG message not supported anymore\r\n");
                break;

                // Emulation of Antenna interface event for FDX test
                // This message triggers the start of the PHY TX and RX processing
            case PHY_LTE_PHYTICK_MSG:
                uart_printf("LTEAPIHandler: PHY_LTE_PHYTICK_MSG message not supported anymore\r\n");
                break;

            case PHY_LTE_TIMER_STOP:
                uart_printf("LTEAPIHandler: PHY_LTE_TIMER_STOP message not supported anymore\r\n");
                break;
            case PHY_LTE_RX_BUF_IQ:
                uart_printf("LTEAPIHandler: PHY_LTE_RX_BUF_IQ message not supported anymore\r\n");
                break;
            case PHY_CPRI_GPIO_CTRL:
                rc = CPRIDrvCfgApi(pApi, pResp);
                break;
            case PHY_CPRI_CONFIG:
            case PHY_CPRI_SFP_CTRL:
                rc = CPRIDrvCfgApi(pApi, pResp);
		        break;
            case PHY_EVMFPGA_GPIO_CTRL:
                rc = EvmFpgaCfgApi(pApi, pResp);
                break;
            case APP_DIAG_PROFILE_ENA:
            case APP_DIAG_PROFILE_DIS:
                break;
            case PHY_LTE_SEND_IQ_ENA:
            case PHY_LTE_SEND_IQ_DIS:
            case PHY_LTE_RCV_IQ_ENA:
            case PHY_LTE_RCV_IQ_DIS:
                break;
            default:
                rc = RC_LTE_UNKNOWN_COMMAND;
                break;
        }
    }
    else if (pApi->Type == API_TYPE_QRY_PARAM)
    {
        pResp->Type = API_TYPE_QRY_RESP;

        switch (pApi->MessageID)
        {
            case PHY_LTE_TX_IQ:
                break;
            case PHY_GETTXBUF:
                rc = LteGetTxLogBufIq(pCtx, pApi, pResp);
                ApiAddErrorCode(pResp, rc);
                break;
            case PHY_LTE_TX_BUF_IQ:
                uart_printf("LTEAPIHandler: PHY_LTE_TX_BUF_IQ is not supported anymore\r\n");
                ApiAddErrorCode(pResp, rc);
                break;
            case PHY_GETRXSDUBUF:
                rc = LteGetRxSduBuf(pCtx, pApi, pResp);
                ApiAddErrorCode(pResp, rc);
                break;
            case DIAG_OUTPUT:
                // Check for the Diag Data Type to be reported
                pParam = ApiGetParamByID(pApi, DIAG_DATA_TYPE);
                switch (pParam->ParamData[0])
                {
                    case IQ_SAMPLES:
                        uart_printf("LTEAPIHandler: IQ_SAMPLES is not supported anymore\r\n");
                        break;

                    case UNCODED_OUT:
                        uart_printf("LTEAPIHandler: UNCODED_OUT is not supported anymore\r\n");
                        break;

                    case FEC_DEC_OUT:
                        uart_printf("LTEAPIHandler: FEC_DEC_OUT not supported anymore\n");
                        break;

                    case SRS_CHEST_OUT:
                        uart_printf("LTEAPIHandler: SRS_CHEST_OUT not supported anymore\n");
                        break;

                    case PRACH_OUT:
                        uart_printf("LTEAPIHandler: PRACH_OUT not supported anymore\n");
                        break;

                    case MLOG_OUT:
                        rc = LteMlogOut(pApi, pResp);
                        break;

                    case MLOG_SIZE:
                        rc = LteMlogSize(pApi, pResp);
                        break;

                    case PHYSTATS_OUT:
                        rc = LtePhyStatsOut(pApi, pResp);
                        break;

                    case PHYSTATS_SIZE:
                        rc = LtePhyStatsSize(pApi, pResp);
                        break;

                    case SVSR_OUT:
                        rc = LteSvsrLogOut(pApi, pResp);
                        break;

                    case SVSR_SIZE:
                        rc = LteSvsrLogSize(pApi, pResp);
                        break;

                    case PROFILE_INFO:
                        // Not supported anymore
                        break;
                }
                break;
            case PHY_LTE_TASK_LIST:
                break;
            case PHY_LTE_DEBUG_MSG:
                break;
            case APP_DIAG_PROFILE_STAT:

                break;
            case AUTO_RUN_STAT:
                uart_printf("LTEAPIHandler: AUTO_RUN_STAT not supported anymore\r\n");
                break;
            case PHY_CPRI_GPIO_CTRL:
                break;
            default:
                rc = RC_LTE_UNKNOWN_COMMAND;
                break;
        }
        if (SUCCESSFUL(rc))
            return APIHEC_OK;
    }
    else if (pApi->Type == API_TYPE_IND)
    {
        switch (pApi->MessageID)
        {
            case SVSR_CHECK_API_WAY:
                memcpy(GetApiPayload(pResp), GetApiPayload(pApi), pApi->Length);
                pResp->Length = pApi->Length;
                return APIHEC_OK;
            default:
                rc = RC_LTE_UNKNOWN_COMMAND;
                break;
        }
    }
    else
    {
        rc = RC_LTE_UNKNOWN_COMMAND;
    }

    ApiAddErrorCode(pResp, rc);
    return APIHEC_OK;
}

/** @brief DOX_INTERNAL This function is called by the function of the L-ARM application init.
 (the function AppInitCpu0 of appinit.c file).

 @return [MXRC] the error code of initialization (0 (or MX_OK) - OK)
 */
MXRC LTEInitHandler(void)
{
    MXRC rc = MX_OK;

    if (pLteCtx != NULL)
    {
        phy_printf("ERROR: LTE application already started [ID=%d]\r\n", pLteCtx->nInstanceID);
        return RC_PHY_CONTEXT_ALLOC_ERROR;
    }

    pLteCtx = (LTEAppCtx *) MemAlloc(sizeof(LTEAppCtx), DDR_MEM, 1);
    if (pLteCtx == NULL)
    {
        phy_printf("ERROR: Unable to allocate APP heap\r\n");
        return RC_LTE_ALLOC_ERROR;
    }

    memset(&gGenProfileCtx, 0, sizeof(gGenProfileCtx));

    // Register LTE API handler on the supervisor layer
    rc = SvsrRegClient(IID_LTE, LTEAPIHandler, pLteCtx);
    if (SUCCESSFUL(rc))
    {
        pLteCtx->nInstanceID = IID_LTE;
        phy_printf("LTE application started [ID=%d]\r\n", pLteCtx->nInstanceID);
    }
    else
    {
        phy_printf("ERROR: Unable to register LTE application [%d]\r\n", rc);
    }

    if (SUCCESSFUL(rc = SvsrRegClient(IID_LTE_EX, (APIHandler)LteApiHandlerEx, pLteCtx)))
    {
        phy_printf("LTE application extended interface is activated [ID=%d]\r\n", IID_LTE_EX);
    }
    else
    {
        phy_printf("ERROR: Unable to register LTE application with extended interface [%d]\r\n", rc);
    }

    // Register NMM LTE API handler on the supervisor layer
    rc = SvsrRegClient(IID_LTE_NMM, (APIHandler)lte_bs_phy_nmm_api_handler, pLteCtx);
    if (SUCCESSFUL(rc))
    {
        pLteCtx->nNMMInstanceID = IID_LTE_NMM;
        phy_printf("Register NMM LTE API handler [ID=%d]\r\n", pLteCtx->nNMMInstanceID);
    }
    else
    {
        phy_printf("ERROR: Unable to register NMM LTE API handler [%d]\r\n", rc);
    }


    // Register 3G NWL  API handler on the supervisor layer
    rc = SvsrRegClient(IDD_UMTS_NWL, (APIHandler)UmtsNWLHandler, pLteCtx);
    if (SUCCESSFUL(rc))
    {
        pLteCtx->n3GNWLInstanceID = IDD_UMTS_NWL;
        phy_printf("Register UMTS NMM  API handler [ID=%d]\r\n", pLteCtx->n3GNWLInstanceID);
    }
    else
    {
        phy_printf("ERROR: Unable to register UMTS NMM API handler [%d]\r\n", rc);
    }

    /* register MAC - PHY handler*/
    SvsrRegMsgHandler(IID_LTE, LteMsgHandler, pLteCtx);
    SvsrRegMsgHandler(IID_LTE_EX, LteMsgHandlerEx, pLteCtx);
    rc |= LtePhyInitApiThread();

    SvsrRegMsgHandler(IID_LTE_NMM, lte_bs_phy_nmm_msg_handler, pLteCtx);
    rc |= lte_bs_phy_nmm_init_api_thread();

    if (SUCCESSFUL(rc))
    {
        pLteCtx->nMsgHandlerID = IID_LTE;
        phy_printf("LTE Msg Handler Registered [ID=%d]\r\n", pLteCtx->nMsgHandlerID);
    }
    else
    {
        phy_printf("ERROR: Unable to register LTE application [%d]\r\n", rc);
    }

    CMgrRegisterHandler(PhyCreateCommand,     LteCreatePhy);
    CMgrRegisterHandler(PhyStartCommand,      LtePhyStart);
    CMgrRegisterHandler(PhyStopCommand,       LtePhyStop);
    CMgrRegisterHandler(PhyShutdownCommand,   LtePhyShutdown);
    CMgrRegisterHandler(PhyStatsCommand,      LtePhyStats);
    CMgrRegisterHandler(PhyMemStatCommand,    LtePhyMemStats);
    CMgrRegisterHandler(PhyShutdownCommand2,  LtePhyShutdown);
    CMgrRegisterHandler(PhyPauseCommand,      LtePhyPause);
    CMgrRegisterHandler(PhyResumeCommand,     LtePhyResume);
    CMgrRegisterHandler(PhyLogsCommand,       LtePhyLogs);
    CMgrRegisterHandler(nmm_start_command,    LteNmmStart);
    CMgrRegisterHandler(nmm_stop_command,     LteNmmStop);
	CMgrRegisterHandler(RadioSaveCommand,     WCDMARadioSave);

    return rc;
}

MXRC LTERegisterPhyConsoleFunctions(TCMgrCommand handler, UINT32 CommandType)
{
    switch(CommandType)
    {
        case PHY_INIT_REQ:
            CMgrRegisterHandler(PhyCreateCommand, handler);
        break;

        case PHY_START_REQ:
            CMgrRegisterHandler(PhyStartCommand, handler);
        break;

        case PHY_STOP_REQ:
            CMgrRegisterHandler(PhyStopCommand, handler);
        break;

        case PHY_SHUTDOWN_REQ:
            CMgrRegisterHandler(PhyShutdownCommand, handler);
            CMgrRegisterHandler(PhyShutdownCommand2, handler);
        break;

        case PHY_PAUSE_REQ:
            CMgrRegisterHandler(PhyPauseCommand, handler);
        break;

        case PHY_RESUME_REQ:
            CMgrRegisterHandler(PhyResumeCommand, handler);
        break;

        case PHY_RECONFIG_REQ:
            CMgrRegisterHandler(PhyReconfigCommand, handler);
        break;

        case NMM_START:
            CMgrRegisterHandler(nmm_start_command, handler);
        break;

        case NMM_STOP:
            CMgrRegisterHandler(nmm_stop_command, handler);
        break;

        default:
        break;
    }

    return MX_OK;
}


UINT32 LteNmmStart(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    uart_printf("Error: There is no testmac compiled\r\n");
    return 0;
}
UINT32 LteNmmStop(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    uart_printf("Error: There is no testmac compiled\r\n");
    return 0;
}

UINT32 LteCreatePhy(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    uart_printf("Error: There is no testmac compiled\r\n");
    return 0;
}
UINT32 LtePhyStart(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    uart_printf("Error: There is no testmac compiled\r\n");
    return 0;
}

UINT32 LtePhyStop(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    PhyDiStop(NULL);
    return 0;
}
UINT32 LtePhyShutdown(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    PhyDiSetShutdown();
    return 0;
}

UINT32 LtePhyStats(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    uart_printf("PhyStats Function has not been registered yet as PhyInit has not happened yet!!\r\n");
    return 0;
}

UINT32 LtePhyMemStats(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 PhyDiState = PhyDiQueryState();

    if (PhyDiState >= PHY_STATE_INIT)
    {
        if (PhyDiState !=  PHY_STATE_START)
        {
            LteBsPhyDisplayFreeSpace("Current Memory Status", 1);
        }
    }
    return 0;
}

extern PHYDICTX PhyDiCtx;
extern DIHANDLE PhyDiHandle;

PPHYDICTX pPhyDiCtx = &PhyDiCtx;


UINT32 LtePhyPause(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    if (pPhyDiCtx->PhyStartMode == AUTO_TIMER)
    {
        DiPause(PhyDiHandle);
    }
    else if (pPhyDiCtx->PhyStartMode == AUTO_PCIE)
    {
        pPhyDiCtx->PhyPauseResume = 1;
        pPhyDiCtx->PhyPauseResumeSkipped = 0;
    }
    else
    {
        uart_printf("PHYDI: This mode doesn't support PAUSE/RESUME\n");
    }

    uart_printf("PHYDI-PAUSE: Last TTI was %d\n", gTTIPauseResumeCount);

    return 0;
}

UINT32 LtePhyResume(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    uart_printf("PHYDI-RESUME: Current TTI is %d\n", gTTIPauseResumeCount);

    if (pPhyDiCtx->PhyStartMode == AUTO_TIMER)
    {
        DiResume(PhyDiHandle);
    }
    else if (pPhyDiCtx->PhyStartMode == AUTO_PCIE)
    {
        pPhyDiCtx->PhyPauseResume = 0;
        pPhyDiCtx->PhyPauseResumeSkipped = 5;
    }
    else
    {
        uart_printf("PHYDI: This mode doesn't support PAUSE/RESUME\n");
    }

    return 0;
}

UINT32 LtePhyLogs(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 PhyDiState = PhyDiQueryState();

    if (PhyDiState == PHY_STATE_START)
    {
        if (pParams->ParamCount > 0)
        {
            gPhyLogsEnableMask = StrToNum(pParams->ParamList[0]);
            LteBsApiThreadEnableLogs();

            if (gDefaultIqEnableMask)
            {
                DiLogControl(NULL, 0, gDefaultIqEnableMask);
            }
        }
    }
    else
    {
        if (pParams->ParamCount == 0)
        {
            uart_printf("\r\n");
            uart_printf("USAGE: phylogs <logmask> where logmask: \r\n");
            uart_printf("LTE_BS_PHY_ENABLE_MLOG               1<<0\r\n");
            uart_printf("LTE_BS_PHY_ENABLE_APILOG             1<<1\r\n");
            uart_printf("LTE_BS_PHY_ENABLE_IQLOGS             1<<2\r\n");
            uart_printf("LTE_BS_PHY_ENABLE_PHYSTATS           1<<3\r\n");
            uart_printf("LTE_BS_PHY_ENABLE_APIERRORCHECK      1<<4\r\n");
            uart_printf("If Bit is 1 then log enabled else log disabled\r\n");
            uart_printf("\r\n");
        }
        else
        {
            gPhyLogsEnableMask = StrToNum(pParams->ParamList[0]);
        }

        uart_printf("Currently Enabled Flags (Mask: 0x%08x):\r\n", gPhyLogsEnableMask);
        if (gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_MLOG)
            uart_printf("LTE_BS_PHY_ENABLE_MLOG\r\n");
        if (gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_APILOG)
            uart_printf("LTE_BS_PHY_ENABLE_APILOG\r\n");
        if (gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_IQLOGS)
            uart_printf("LTE_BS_PHY_ENABLE_IQLOGS\r\n");
        if (gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_PHYSTATS)
            uart_printf("LTE_BS_PHY_ENABLE_PHYSTATS\r\n");
        if (gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_APIERRORCHECK)
            uart_printf("LTE_BS_PHY_ENABLE_APIERRORCHECK\r\n");

        if((gPhyLogsEnableMask & LTE_BS_PHY_ENABLE_APILOG) == 0)
            SvsrLoggerSetMask(0);

        uart_printf("\r\n");
    }

    return 0;
}

UINT32 LtePhyLogsAddMask(UINT32 mask)
{
    gPhyLogsEnableMask |= mask;

    return 0;
}

UINT32 LtePhyLogsRemoveMask(UINT32 mask)
{
    gPhyLogsEnableMask &= ~mask;

    return 0;
}

UINT32 LtePhySetApiLogMask(UINT32 mask)
{
    gDefaultApiLoggingMask = mask;

    return 0;
}


/** @brief DOX_INTERNAL This function will be called by the supervisor in case of receiving
 API command with the LTE instance ID

 @param pClientCtx [in] - pointer to the LTE context (LTEAppCtx *, please see LTEInit)
 @param pApi       [in] - pointer to the received API command
 @param pResp      [out]- pointer to the response for the API command

 @NOTE: 1. The response is automatically allocated by the supervisor
 2. If function does not want to send a response, it should return APIHEC_DROP_RESP value


 @return [APIHEC] the control code of response control
 **/
APIHEC LTEAPIHandlerStub(LPVOID pClientCtx, ApiHeader *pApi, ApiHeader *pResp)
{
    uart_printf("LTEAPIHandlerStub: %X\r\n", pClientCtx);

    printf ("(cpu-%d)rx: Ctrl: 0x%08x Src: 0x%04x Dst: 0x%04x ID: 0x%04x Type: 0x%04x Len: %04d\r\n",
        MxGetCpuID (),
        pApi->Control,
        pApi->SrcInstID,
        pApi->DstInstID,
        pApi->MessageID,
        pApi->Type,
        pApi->Length);

    if(pClientCtx == NULL)
    {// Ex case
        ApiParam * pParam;
        PMAC2PHY_QUEUE_EL pElm, curr, tmp;

        _ASSERT_PTR(pApi);

        pParam = ApiGetParamByID(pApi, PHY_QUE_HDR_PARAM_ID);
        _ASSERT_PTR(pParam);

        if (pParam != NULL)
        {
            pElm = (PMAC2PHY_QUEUE_EL)pParam->ParamData[0];
            _ASSERT_PTR(pElm);

            uart_printf("Start p=%X\r\n", pElm);

            // we need to invalidate the list of blocks
            curr = pElm;
            while (curr != NULL)
            {
                // to invalidate the list element
                MxCacheInvalidate(curr, sizeof (MAC2PHY_QUEUE_EL));

                _ASSERT_PTR(curr->MessagePtr);
                if (curr->MessagePtr != NULL)
                    MxCacheInvalidate(curr->MessagePtr, curr->MessageLen);

                curr = curr->Next;
            }

            // free all
            curr = pElm;
            while (curr)
            {
                if(curr->MessagePtr)
                {
                    uart_printf("Free1 p=%X\r\n", curr->MessagePtr);
                    SvsrFreeIMem(curr->MessagePtr);
                }

                tmp = curr;
                curr = curr->Next;
                tmp->Next = NULL;
                uart_printf("Free2 p=%X\r\n", tmp);
                SvsrFreeIMem(tmp);
            }
        }
    }

    return APIHEC_DROP_RESP;
}

static MXRC LteSetRxLogBufIq(LPVOID pParam, ApiHeader *pApi, ApiHeader * pResp)
{
    MXRC rc = MX_OK;
    ApiParam *pApiParam;
    UINT32 nSize, nOffs;
    UINT32 phySectorId, antennId = 0;
    UINT32 length_adc_bytes_ant;
    UINT32 length_adc_bytes_sf;
    UINT32 sf_num;
    U8*pInIq, *ToBuf = NULL;
    UINT32 maxIqBufferSize;
    PLTEPHYDESC pPhy;
    PBS pBs;
    PBS_RX pBsRx;
    UINT16 RxAntTotalNum;
    UINT16 Nfft;

    _ASSERT_PTR(pParam);

    ToBuf = (UINT8*) PhyDiGetRxBufPos(0, 0);
    if (ToBuf == NULL)
    {
        return RC_LTE_ALLOC_ERROR;
    }

    // Sector number
    pApiParam = ApiGetParamByID(pApi, PHY_SECTOR_PARAM_ID);
    phySectorId = (pApiParam != NULL) ? pApiParam->ParamData[0] : 0;

    // Sizeof each samples per Antenna and per SF
    pApiParam = ApiGetParamByID(pApi, PHY_PAR_FFT_NUM);
    if (pApiParam == NULL)
    {
        pPhy = getPhyCtxApiProc(phySectorId);
        pBs = pPhy->pBs;
        if (pBs == NULL)
        {
            uart_printf(
                        "(1): Please send this msg after PHY creation or provide FFTnum and nRxAnt\r\n");
            return RC_PHY_NO_VALID_DATA_ERROR;
        }
        Nfft = pBs->Nfft;

        pBsRx = pPhy->pBsRx;
        if (pBsRx == NULL)
        {
            uart_printf(
                        "(2): Please send this msg after PHY creation or provide FFTnum and nRxAnt\r\n");
            return RC_PHY_NO_VALID_DATA_ERROR;
        }
        RxAntTotalNum = pBsRx->Nrx_antennas;
    }
    else
    {// it's the case when IQ samples are programmed before INIT_REQ coming to PHY
        Nfft = (U16) pApiParam->ParamData[0];
        pApiParam = ApiGetParamByID(pApi, PHY_PAR_ANT_TOTALNUM);
        if (pApiParam == NULL)
        {
            uart_printf("nRxAnt should be provided as well\r\n");
            return RC_PHY_NO_VALID_DATA_ERROR;
        }
        RxAntTotalNum = (U16) pApiParam->ParamData[0];
    }

    length_adc_bytes_ant = sizeof(S16) * 2 * ComputeUpperBound(Nfft);
    length_adc_bytes_sf = length_adc_bytes_ant * RxAntTotalNum;

    // get SF number
    pApiParam = ApiGetParamByID(pApi, PHY_SF_NUMBER);
    sf_num = (pApiParam != NULL) ? pApiParam->ParamData[0] : 0;
    ToBuf += sf_num * length_adc_bytes_sf;

    // get sector Antenna
    pApiParam = ApiGetParamByID(pApi, PHY_ANTENNA_NUM_PARAM_ID);
    antennId = (pApiParam != NULL) ? pApiParam->ParamData[0] : 0;
    if (antennId >= RxAntTotalNum)
    {
        uart_printf("No Antenna ID param - rejected\r\n");
        return RC_LTE_PARAM_ERROR;
    }
    ToBuf += antennId * length_adc_bytes_ant;

    // Get Size of data to be set
    pApiParam = ApiGetParamByID(pApi, PHY_BUF_SIZE_PARAM_ID);
    if (pApiParam == NULL)
    {
        uart_printf("No Size param - rejected\r\n");
        return RC_LTE_PARAM_ERROR;
    }
    nSize = pApiParam->ParamData[0];

    // Get Offset
    pApiParam = ApiGetParamByID(pApi, PHY_BUF_OFFS_PARAM_ID);
    if (pApiParam == NULL)
    {
        uart_printf("No Offset param - rejected\r\n");
        return RC_LTE_PARAM_ERROR;
    }
    nOffs = pApiParam->ParamData[0];

    maxIqBufferSize = PhyDiGetRxBufSize(0, 0);

    if ((UINT32) ToBuf + nSize + nOffs > (UINT32) PhyDiGetRxBufPos(0, 0) + maxIqBufferSize)
    {
        uart_printf("Offset overflow - rejected\r\n");
        return RC_LTE_PARAM_ERROR;
    }

    pApiParam = ApiGetParamByID(pApi, PHY_DATA_PARAM_ID);
    if (pApiParam == NULL)
    {
        uart_printf("No data param - rejected\r\n");
        return RC_LTE_PARAM_ERROR;
    }
    pInIq = (U8*) &pApiParam->ParamData[0];

    ToBuf += nOffs;
    memcpy(ToBuf, pInIq, nSize);

    return rc;
}

static MXRC LteGetTxLogBufIq(LPVOID pParam, ApiHeader *pApi, ApiHeader * pResp)
{
    MXRC rc = MX_OK;
    ApiParam *pApiParam;
    UINT32 nSize, nOffs;
    UINT32 phySectorId = 0;
    UINT32 antennId = 0;
    S16 *pOutIq;
    UINT32 maxIqBufferSize;
    UINT8 *pOutputSignalAx = NULL;

    _ASSERT_PTR(pParam);

    // get sector #
    pApiParam = ApiGetParamByID(pApi, PHY_SECTOR_PARAM_ID);
    if (pApiParam == NULL)
        return RC_LTE_PARAM_ERROR;
    phySectorId = pApiParam->ParamData[0];
    if (phySectorId != 0) // only one sector is supported
        return RC_PHY_INCORRECT_PARAM;

    // get sector antenna
    pApiParam = ApiGetParamByID(pApi, PHY_ANTENNA_NUM_PARAM_ID);
    if (pApiParam == NULL)
        return RC_LTE_PARAM_ERROR;
    antennId = pApiParam->ParamData[0];

    // Get Size of requested data
    pApiParam = ApiGetParamByID(pApi, PHY_BUF_SIZE_PARAM_ID);
    if (pApiParam == NULL)
        return RC_LTE_PARAM_ERROR;
    nSize = pApiParam->ParamData[0];

    // Get Offset
    pApiParam = ApiGetParamByID(pApi, PHY_BUF_OFFS_PARAM_ID);
    if (pApiParam == NULL)
        return RC_LTE_PARAM_ERROR;
    nOffs = pApiParam->ParamData[0];

    pOutIq = PhyDiGetTxBufPos(0, antennId);
    pOutputSignalAx = (UINT8*) pOutIq;
    if (pOutIq == NULL)
    {
        return RC_LTE_ALLOC_ERROR;
    }

    maxIqBufferSize = PhyDiGetTxBufSize(0, antennId);
    if (nSize + nOffs > maxIqBufferSize)
    {
        if (nOffs >= maxIqBufferSize)
            return RC_LTE_PARAM_ERROR;

        nSize = maxIqBufferSize - nOffs;
    }


    // put chunk of TX IQ samples buffer to response
    ApiAddParam(pResp, NULL, PHY_DATA_PARAM_ID, pOutputSignalAx + nOffs, nSize);

    // put sector number to Response
    ApiAddParam(pResp, NULL, PHY_SECTOR_PARAM_ID, &phySectorId, sizeof(phySectorId));

    // put Antenna number to Response
    ApiAddParam(pResp, NULL, PHY_ANTENNA_NUM_PARAM_ID, &antennId, sizeof(phySectorId));

    return rc;
}

static MXRC LteGetRxSduBuf(LPVOID pParam, ApiHeader *pApi, ApiHeader * pResp)
{
    MXRC rc = MX_OK;
    ApiParam *pApiParam;
    UINT32 nSize, nOffs;

    _ASSERT_PTR(pParam);

    if (gUlSduBuf == NULL)
        return RC_LTE_ALLOC_ERROR;

    // Get Size of requested data
    pApiParam = ApiGetParamByID(pApi, PHY_BUF_SIZE_PARAM_ID);
    if (pApiParam == NULL)
        return RC_LTE_PARAM_ERROR;
    nSize = pApiParam->ParamData[0];

    // Get Offset
    pApiParam = ApiGetParamByID(pApi, PHY_BUF_OFFS_PARAM_ID);
    if (pApiParam == NULL)
        return RC_LTE_PARAM_ERROR;

    nOffs = pApiParam->ParamData[0];
    if (nSize + nOffs > UL_SDU_BUF_SIZE)
    {
        if (nOffs >= UL_SDU_BUF_SIZE)
            return RC_LTE_PARAM_ERROR;

        nSize = UL_SDU_BUF_SIZE - nOffs;
    }

    // put chunk of RxSdu buffer to response
    ApiAddParam(pResp, NULL, PHY_DATA_PARAM_ID, gUlSduBuf + nOffs, nSize);

    return rc;
}

/** PHY_START_REQ handler
 *
 * @param phyInstance PHY instance ID
 * @return Success = 0, error code otherwise
 */
U32 LteStart(U32 phyInstance, PSTARTREQ pParam, U32 logIqEnable)
{
    U32 ret = 0;
    ret |= PhyDiStart(phyInstance, pParam, logIqEnable);
    return ret;
}

static MXRC LteMlogOut(ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * pParam;

    UINT32 nSize, nOffs;
    U32 MsgSize;
    U8* ptr = 0;

    MXRC rc = MX_OK;

    // Size

    pParam = ApiGetParamByID(pApi, DIAG_BUF_SIZE_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    nSize = pParam->ParamData[0];

    // Offset

    pParam = ApiGetParamByID(pApi, DIAG_BUF_OFFS_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    nOffs = pParam->ParamData[0];

    ptr = LteGetMlogInformation(&MsgSize);

    if ((nSize + nOffs > MsgSize))
    {
        if (nOffs >= MsgSize)
        {
            printf("LTEDIAG: LteMlogOut Error!\r\n");
            return RC_DIAG_PARAM_ERROR;
        }

        nSize = MsgSize - nOffs;
    }

    ApiAddParam(pResp, NULL, DIAG_DATA_PARAM_ID, (PUINT8) ptr + nOffs, nSize);

    return rc;
}

static MXRC LteMlogSize(ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * pParam;

    UINT32 nSize, *MlogSize; //, nOffs;
    PUINT8 ptr = 0;

    MXRC rc = MX_OK;

    // Size

    pParam = ApiGetParamByID(pApi, DIAG_BUF_SIZE_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    nSize = pParam->ParamData[0];

    // Offset

    pParam = ApiGetParamByID(pApi, DIAG_BUF_OFFS_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    //nOffs = pParam->ParamData[0];

    MlogSize = MemAlloc(4, CRAM_MEM, 0);
    ptr = (PUINT8) MlogSize;

    *MlogSize = MLogGetFileSize();

    //uart_printf("MlogSize = %d, nSize = %d, off = %d\n", *MlogSize, nSize, nOffs);

    ApiAddParam(pResp, NULL, DIAG_DATA_PARAM_ID, (PUINT8) ptr, nSize);

    MemFree(MlogSize, CRAM_MEM);

    return rc;
}

static MXRC LteSvsrLogOut(ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * pParam;

    UINT32 nSize, nOffs;
    U32 MsgSize;
    U8* ptr = 0;

    MXRC rc = MX_OK;

    // Size

    pParam = ApiGetParamByID(pApi, DIAG_BUF_SIZE_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    nSize = pParam->ParamData[0];

    // Offset

    pParam = ApiGetParamByID(pApi, DIAG_BUF_OFFS_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    nOffs = pParam->ParamData[0];

    ptr = LteGetSvsrInformation(&MsgSize);

    if ((nSize + nOffs > MsgSize))
    {
        if (nOffs >= MsgSize)
        {
            printf("LTEDIAG: LteSvsrLogOut Error!\r\n");
            return RC_DIAG_PARAM_ERROR;
        }

        nSize = MsgSize - nOffs;
    }

    ApiAddParam(pResp, NULL, DIAG_DATA_PARAM_ID, (PUINT8) ptr + nOffs, nSize);

    return rc;
}

static MXRC LteSvsrLogSize(ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * pParam;

    UINT32 nSize, *SvsrLogSize; //, nOffs;
    PUINT8 ptr = 0;

    MXRC rc = MX_OK;

    // Size

    pParam = ApiGetParamByID(pApi, DIAG_BUF_SIZE_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    nSize = pParam->ParamData[0];

    // Offset

    pParam = ApiGetParamByID(pApi, DIAG_BUF_OFFS_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    //nOffs = pParam->ParamData[0];

    SvsrLogSize = MemAlloc(4, CRAM_MEM, 0);
    ptr = (PUINT8) SvsrLogSize;

    *SvsrLogSize = SvsrGetSize();

    //uart_printf("SvsrLogSize = %d, nSize = %d, off = %d\n", *SvsrLogSize, nSize, nOffs);

    ApiAddParam(pResp, NULL, DIAG_DATA_PARAM_ID, (PUINT8) ptr, nSize);

    MemFree(SvsrLogSize, CRAM_MEM);

    return rc;
}


static MXRC LtePhyStatsOut(ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * pParam;

    UINT32 nSize, nOffs;
    U32 MsgSize;
    U8* ptr = 0;

    MXRC rc = MX_OK;

    // Size

    pParam = ApiGetParamByID(pApi, DIAG_BUF_SIZE_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    nSize = pParam->ParamData[0];

    // Offset

    pParam = ApiGetParamByID(pApi, DIAG_BUF_OFFS_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    nOffs = pParam->ParamData[0];

    ptr = LteGetPhyStatsInformation(&MsgSize);

    if ((nSize + nOffs > MsgSize))
    {
        if (nOffs >= MsgSize)
        {
            printf("LTEDIAG: LtePhyStatsOut Error!\r\n");
            return RC_DIAG_PARAM_ERROR;
        }

        nSize = MsgSize - nOffs;
    }

    ApiAddParam(pResp, NULL, DIAG_DATA_PARAM_ID, (PUINT8) ptr + nOffs, nSize);

    return rc;
}

static MXRC LtePhyStatsSize(ApiHeader * pApi, ApiHeader * pResp)
{
    ApiParam * pParam;

    UINT32 nSize, *PhyStatsSize; //, nOffs;
    PUINT8 ptr = 0;

    MXRC rc = MX_OK;

    // Size

    pParam = ApiGetParamByID(pApi, DIAG_BUF_SIZE_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    nSize = pParam->ParamData[0];

    // Offset

    pParam = ApiGetParamByID(pApi, DIAG_BUF_OFFS_PARAM_ID);
    if (pParam == NULL)
        return RC_DIAG_NOT_PARAM;

    //nOffs = pParam->ParamData[0];

    PhyStatsSize = MemAlloc(4, CRAM_MEM, 0);
    ptr = (PUINT8) PhyStatsSize;

    *PhyStatsSize = LteBsPhyStatsGetSize();

    //uart_printf("PhyStatsSize = %d, nSize = %d, off = %d\n", *PhyStatsSize, nSize, nOffs);

    ApiAddParam(pResp, NULL, DIAG_DATA_PARAM_ID, (PUINT8) ptr, nSize);

    MemFree(PhyStatsSize, CRAM_MEM);

    return rc;
}

