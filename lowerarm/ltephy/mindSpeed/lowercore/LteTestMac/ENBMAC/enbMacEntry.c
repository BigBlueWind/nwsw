/** @file enbMacEntry.c
 *
 * @brief The entry point of the LTE MAC application
 * @author Mindspeed Technologies
 *
 * COPYRIGHT&copy; 2009-2011 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
**/

#include <string.h>
#include "stream.h"
#include "enbMacEntry.h"
#include "appprintf.h"
#include "enbApiHdlr.h"
#include "supervisor.h"
#include "heaps.h"
#include "larmmsgid.h"
#include "diagnostics.h"
#include "enbMacIface.h"
#include "bsmac.h"
#include "mlog.h"
#include "svsrlogger.h"
#include "apiplayer.h"
#include "LteMacTestCfgMgr.h"
#include "enbMac2PhyIface.h"
#include "lte_entry.h"

#include "lte_mac_state.h"

/*
 * Local Limits
 */

#define PID_MAC_PROCESSING_THREAD   30000

/*
 * Private Types
 */

typedef struct
{
    UINT32 nInstanceID;
    UINT32 nMsgHandlerID;
    UINT32 RegComplete;
    HANDLE hRegThread;
    HANDLE inApiE;
    HANDLE inApiQ;
    UINT32 inApiQStor[19 + 1];
} LteMacAppCtx;

/*
 * External Function References
 */

extern void Mac2PhyIfFreeBuffer(PGENMSGDESC pMsg);
extern void SetMacInstanceId(UINT32 MacId);
extern UINT32 GetMacInstanceId(void);
extern RESULTCODE LteMacProcessFromPhyMessage(PGENMSGDESC pMsgHdr, PDLSUBFRDESC pDlSubFrame,
                                              PULSUBFRDESC pUlSubFrame, PTR ptrCtx);
extern short MacCpuID;

/*
 * Private Function Prototypes
 */

static UINT32 LteMacConsoleHandler(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
static UINT32 LteMac_CreateMac(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
static UINT32 LteMac_CreatePhy(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
static UINT32 LteMac_PhyStart(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
static UINT32 LteMac_PhyStop(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
static UINT32 LteMac_PhyShutdown(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
static UINT32 LteMac_PhyReconfig(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
static UINT32 LteMac_ApiPlay(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
static UINT32 LteMac_NMMStart(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
static UINT32 LteMac_NMMStop(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
static UINT32 LteMac_NMMCmd(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);
static UINT32 UMTS_NMMCmd(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams);

static MXRC PhyGetMsg(HANDLE h, PTR pMsg, U32 *pMsgSize);

static RESULTCODE SvsrCreateMacInst(ApiHeader * msg, ApiHeader *pResp);
static RESULTCODE SvsrCreatePhy(ApiHeader * msg, ApiHeader *pResp);
static RESULTCODE SvsrDestroyMac(ApiHeader * msg, ApiHeader *pResp);
static RESULTCODE SvsrDestroyPhy(ApiHeader * msg, ApiHeader *pResp);
static RESULTCODE MacNmmApi(ApiHeader * msg);

extern RESULTCODE MacNMMCmd(U32 mode);

/*UMTSNMM*/
extern RESULTCODE MacUmtsNMMCmd(U32 mode);

/*
 * Local string constants on aligned addresses
 * (strcmp on high optimization levels require alignment)
 */

__align(4) const char MacCommand[]          = "mac";
__align(4) const char MacHelpCommand[]      = "help";
__align(4) const char CreateMacCommand[]    = "createmac";
__align(4) const char ApiPlayCommand[]      = "logplay";
__align(4) const char nmmCommand[]          = "nmmcmd";
/*UMTSNMM*/
__align(4) const char UmtsNmmCommand[]   = "umtsnmm";

/*
 * External Variables Reference
 */

extern unsigned char api_log_1ms_data[];
extern unsigned int api_log_1ms_size;

/*
 * Global Variables
 */

MXRC (*LteMacAppInitCallback)(void) = LteMacInitHandler;
LteMacAppCtx *pLteMacAppCtx =  NULL;
volatile UINT32 PhyStartComplete = 0;
HANDLE hPhyStartThrReg;

/*
 * Private Module Variables
 */

static U32 start_flag = 0;

/*
 * Private Functions
 */

/** Send API message to the PHY (4GMX-specific)
 *
 *  @param h [IN] Unused
 *  @param pMsg [IN] Pointer to the message data
 *  @param msgSize [IN] Data size
 *  @return 4GMX result code
 */
static MXRC PhySendMsg(PTR h, PTR pMsg, U32 msgSize)
{
    return SvsrSendMsg(0, IID_LTE, pMsg, SVSR_MSG_OPT_DEALLOC);
}

/** Receive message from the PHY (4GMX-specific)
 *
 *  @param h [IN] Unused
 *  @param pMsg [IN] Pointer to the message data
 *  @param pMsgSize [IN] Unused
 *  @return 4GMX result code
 */
static MXRC PhyGetMsg(HANDLE h, PTR pMsg, U32 *pMsgSize)
{
    //static int ul_idx = 0;
    MXRC rc;
    U32 TickNum = globalTTITickCount;
    volatile int t = MacGetTick();

    PRINT_FUNCTION_NAME(__FUNCTION__);

    rc = LteMacProcessFromPhyMessage(pMsg, &DlSubFrame, &UlSubFrame, NULL);

    if (start_flag)
    {
        MLogTask(PID_MAC_PROCESSING_THREAD, RESOURCE_LARM, t, MacGetTick());
    }
    start_flag = 1;
    TickNum = globalTTITickCount - TickNum;
    //ul_idx = (ul_idx + (TickNum & 1)) & 7;

    return rc;
}

/*
 * Public Functions
 */

/** MAC message handler registered at Supervisor level for the MAC instance ID
 *
 * @param pClientCtx
 * @param pMsg Pointer to the message to process (MSA-E-81479)
 * @return 4GMX result code
 */
MXRC LteMacMsgHandler(LPVOID pClientCtx, LPVOID pMsg)
{
    U32 size = 0;
    _ASSERT_PTR(pClientCtx);

    return PhyGetMsg(HNULL, pMsg, &size);
}

/** @brief DOX_INTERNAL This function will be called by the supervisor in case of receiving
            API command with the LTE instance ID

    @param pClientCtx [in] - pointer to the LTE context (LTEAppCtx *, please see LTEInit)
    @param pApi       [in] - pointer to the received API command
    @param pResp      [out]- pointer to the response for the API command

    @NOTE: 1. The response is automatically allocated by the supervisor
           2. If function does not want to send a response, it should return APIHEC_DROP_RESP value

    @return [APIHEC] the control code of response control
*/
APIHEC LteMacApiHandler(LPVOID pClientCtx, ApiHeader *pApi, ApiHeader *pResp)
{
    RESULTCODE rc = SUCCESS;
    LteMacAppCtx *pCtx = (LteMacAppCtx *) pClientCtx;

    if (pCtx == NULL)
    {
        rc = RC_PHY_CONTEXT_ALLOC_ERROR;
        ApiAddErrorCode(pResp, rc);
        return APIHEC_OK;
    }

    if (pApi->Type == API_TYPE_CFG_PARAM)
    {
        switch (pApi->MessageID)
        {
            case SVSR_CREATEMAC:
                rc = SvsrCreateMacInst(pApi, pResp);
                break;

            case SVSR_DESTROYMAC:
                rc = SvsrDestroyMac(pApi, pResp);
                break;

            case SVSR_CREATEPHY:
                rc = SvsrCreatePhy(pApi, pResp);
                break;

            case SVSR_DESTROYPHY:
                rc = SvsrDestroyPhy(pApi, pResp);
                break;

            case MAC_TEST_CONFIG:
                rc = MacApiConfigTestConfig(pApi, pResp);
                break;

            case MAC_TEST_RXVECTOR_CFG:
                rc = MacApiConfigTestRxVecCfg(pApi, pResp);
                break;

            case MAC_TEST_TXVECTOR_CFG:
                rc = MacApiConfigTestTxVecCfg(pApi, pResp);
                break;

            case MAC_TEST_MAP_CONF:
                rc = MacApiSetAllocationMap(pApi, pResp);
                break;

            case PHY_LTE_START:
            case PHY_LTE_STOP:
            case PHY_LTE_SHUTDOWN:
                rc = MacApiConfig(pApi, pResp);
                break;

            case PHY_LTE_NMM_MSG:
                MacNmmApi(pApi);
                return APIHEC_DROP_RESP;

            case PHY_TEST_INVALID_DATA:
                rc = MacApiInvalidDataTest(pApi, pResp);
                break;

            default:
                rc = RC_LTE_UNKNOWN_COMMAND;
                break;
        }
    }
    else if (pApi->Type == API_TYPE_QRY_PARAM)
    {
        switch (pApi->MessageID)
        {
            default:
                rc = RC_LTE_UNKNOWN_COMMAND;
                break;
        }
    }
    else if (pApi->Type == API_TYPE_IND)
    {
        switch (pApi->MessageID)
        {
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

/** Performs MAC instance registration on Supervisor level
 *  Application callback
 *
 * @param p Unused
 * @return 4GMX result code
 */
MXRC MacRegMsgThread(void * p)
{
    MXRC rc = SUCCESS;

    SvsrRegMsgHandler(IID_LTE_MAC, LteMacMsgHandler, pLteMacAppCtx);
    if (SUCCESSFUL(rc))
    {
        pLteMacAppCtx->nMsgHandlerID = IID_LTE;
        //uart_printf("LTE MAC Msg Handler Registered [ID=%d]\r\n", pLteMacAppCtx->nMsgHandlerID);
    }
    else
    {
        uart_printf("ERROR: Unable to register LTE application [%d]\r\n", rc);
    }
    /* list-based RxSdu*/
    SvsrRegMsgHandler(IID_LTE_MAC_EX, LteMacRxSduHandler, pLteMacAppCtx);
    MacCpuID = MxGetCpuID();


    pLteMacAppCtx->RegComplete = 1;
    return rc;
}

/** @brief DOX_INTERNAL This function is called by the function of the L-ARM application init.
           (the function AppInitCpu0 of appinit.c file).

    @return [MXRC] the error code of initialization (0 (or MX_OK) - OK)
*/
MXRC LteMacInitHandler(void)
{
    MXRC rc = MX_OK;

#if 0
    if (pLteMacAppCtx != NULL)
    {
        uart_printf("ERROR: LTE application already started [ID=%d]\r\n",
                    pLteMacAppCtx->nInstanceID);
        return RC_PHY_CONTEXT_ALLOC_ERROR;
    }
#endif

    pLteMacAppCtx = (LteMacAppCtx *) MxHeapAlloc(AppHeap, sizeof(LteMacAppCtx));
    if (pLteMacAppCtx == NULL)
    {
        uart_printf("ERROR: Unable to allocate APP heap\r\n");
        return RC_LTE_ALLOC_ERROR;
    }
    memset(pLteMacAppCtx, 0, sizeof(LteMacAppCtx));

    // Register LTE API handler on the supervisor layer
    rc = SvsrRegClient(IID_LTE_MAC, LteMacApiHandler, pLteMacAppCtx);
    if (SUCCESSFUL(rc))
    {
        pLteMacAppCtx->nInstanceID = IID_LTE_MAC;
        uart_printf("LTE MAC application started [ID=%d]\r\n", pLteMacAppCtx->nInstanceID);

        // Register LTE console handler
        //CMgrRegisterHandler(MacCommand, LteMacConsoleHandler);
        CMgrRegisterHandler(CreateMacCommand, LteMac_CreateMac);

        LTERegisterPhyConsoleFunctions(LteMac_CreatePhy, PHY_INIT_REQ);
        LTERegisterPhyConsoleFunctions(LteMac_PhyStart,  PHY_START_REQ);
        LTERegisterPhyConsoleFunctions(LteMac_PhyStop,   PHY_STOP_REQ);
        LTERegisterPhyConsoleFunctions(LteMac_PhyShutdown,   PHY_SHUTDOWN_REQ);
        LTERegisterPhyConsoleFunctions(LteMac_PhyReconfig,   PHY_RECONFIG_REQ);
        LTERegisterPhyConsoleFunctions(LteMac_NMMStart,  NMM_START);
        LTERegisterPhyConsoleFunctions(LteMac_NMMStop,   NMM_STOP);

        CMgrRegisterHandler(nmmCommand,     LteMac_NMMCmd);
        CMgrRegisterHandler(ApiPlayCommand, LteMac_ApiPlay);
        /*UMTSNMM*/
        CMgrRegisterHandler(UmtsNmmCommand,     UMTS_NMMCmd);

        //MacRegMsgThread(NULL);
    }
    else
    {
        uart_printf("ERROR: Unable to register LTE MAC application [%d]\r\n", rc);
    }
    ApiPlayerInit();

    return rc;
}

/** Handles SVSR_CREATEMAC command
 *
 * @param msg Pointer to the message header (MSA-E-81479)
 * @param pResp Pointer to the message response buffer (preallocated)
 * @return Standard result code
 */
RESULTCODE SvsrCreateMacInst(ApiHeader * msg, ApiHeader *pResp)
{
    RESULTCODE ret = SUCCESS;
    U32 instID = 0;

    ret |= MacSetInitConfig(msg);
    ret |= MacInit(PhySendMsg, (PhyRxCb) PhyGetMsg, NULL, NULL);
    instID = UARM_INSTID_DEFMAC;

    ApiAddErrorCode(pResp, ret);
    ApiAddParam(pResp, NULL, PAR_INSTID, &instID, sizeof(U32));

    return ret;
}

/** Handles SVSR_CREATEPHY command
 *
 * @param msg Pointer to the message header (MSA-E-81479)
 * @param pResp Pointer to the message response buffer (preallocated)
 * @return Standard result code
 */
RESULTCODE SvsrCreatePhy(ApiHeader *msg, ApiHeader *pResp)
{
    RESULTCODE ret = SUCCESS;
    U32 instID = 0;

    ret = MacCreatePhy(NULL);

    ApiAddErrorCode(pResp, ret);
    ApiAddParam(pResp, NULL, PAR_INSTID, &instID, sizeof(U32));

    return ret;
}

/** Handle SVSR_DESTROYMAC message
 *
 * @param msg Pointer to the message header (MSA-E-81479)
 * @param pResp Pointer to the message response buffer (preallocated)
 * @return Standard result code
 */
RESULTCODE SvsrDestroyMac(ApiHeader *msg, ApiHeader *pResp)
{
    RESULTCODE ret;

    // Destroy MAC instance
    ret = MacDone();
    ApiAddErrorCode(pResp, ret);

    return ret;
}

/** Handle SVSR_DESTROYPHY message
 *
 * @param msg Pointer to the message header (MSA-E-81479)
 * @param pResp Pointer to the message response buffer (preallocated)
 * @return Standard result code
 */
RESULTCODE SvsrDestroyPhy(ApiHeader *msg, ApiHeader *pResp)
{
    RESULTCODE ret = SUCCESS;

    ret = MacApiConfig(msg, pResp);

    return ret;
}

/** Handle 'createmac' command from console input
 *
 * @param pCtx Pointer to console manager context
 * @param pParams Pointer to command parameters list
 * @return 4GMX result code
 */
UINT32 LteMac_CreateMac(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 configId, rc;
    UINT32 ConsoleValsNum = 0;
    UINT32 ConsoleVals[CM_MAX_PARAM_COUNT] = {0};

    if (pParams->ParamCount < 1)
    {
        int i;
        uart_printf("Current Configs supported: %d\r\n", TEST_MGR_CONFIG_MAX_ID);
        for (i = 0; i < TEST_MGR_CONFIG_MAX_ID; i++)
            uart_printf("%3d: %s\r\n", i, TEST_MGR_PREDEF_CONFIG_STR[i]);

        uart_printf("\nConfig Details:\r\n---------------\r\n\n");
        uart_printf("-----------------------------------------------------------------------------------\r\n");
        uart_printf("| Test | NumAnt | TrMode | PBCH | NumCtrl | PDCCH |     PDSCH     |     PDSCH     |\r\n");
        uart_printf("|      |        |        |      |   Sym   |       | (Num/ RB/Mod) | (Num/ RB/Mod) |\r\n");
        uart_printf("-----------------------------------------------------------------------------------\r\n");

        for (i = 1; i < TEST_MGR_CONFIG_MAX_ID; i++)
        {
            uart_printf("| %4d ", i);
            uart_printf("| %6d ", TEST_MGR_PREDEF_CONFIG_DETAILS[i][0]);
            uart_printf("| %6d ", TEST_MGR_PREDEF_CONFIG_DETAILS[i][1]);
            uart_printf("| %4d ", TEST_MGR_PREDEF_CONFIG_DETAILS[i][2]);
            uart_printf("| %7d ", TEST_MGR_PREDEF_CONFIG_DETAILS[i][3]);
            uart_printf("| %5d ", TEST_MGR_PREDEF_CONFIG_DETAILS[i][4]);
            uart_printf("| (%3d/%3d/%3d) ", TEST_MGR_PREDEF_CONFIG_DETAILS[i][5],
                        TEST_MGR_PREDEF_CONFIG_DETAILS[i][6], TEST_MGR_PREDEF_CONFIG_DETAILS[i][7]);
            uart_printf("| (%3d/%3d/%3d) ", TEST_MGR_PREDEF_CONFIG_DETAILS[i][8],
                        TEST_MGR_PREDEF_CONFIG_DETAILS[i][9], TEST_MGR_PREDEF_CONFIG_DETAILS[i][10]);
            uart_printf("|\r\n");
            uart_printf("-----------------------------------------------------------------------------------\r\n");
        }
        return RC_CM_PARAM_ERROR;
    }
    else if(pParams->ParamCount > CM_MAX_PARAM_COUNT)
    {
        uart_printf("Warning: maximum allowed parameters number is: %d\r\n", CM_MAX_PARAM_COUNT);
        uart_printf("last %d parameters are rejected now\r\n", pParams->ParamCount -CM_MAX_PARAM_COUNT);
        pParams->ParamCount = CM_MAX_PARAM_COUNT;
    }

    configId = StrToNum(pParams->ParamList[0]);

    for(ConsoleValsNum = 0; ConsoleValsNum < pParams->ParamCount; ConsoleValsNum++)
    {
        ConsoleVals[ConsoleValsNum] = StrToNum(pParams->ParamList[ConsoleValsNum]);
    }
    MacProcConsoleParams(ConsoleVals, ConsoleValsNum);
    rc = (MXRC) MacInit(PhySendMsg, (PhyRxCb) PhyGetMsg, NULL, NULL);

    printf("Done [rc = %d], [configId %d]\n", rc, configId);

    return 0;
}

/** Handle 'createphy' command from console input
 *
 * @param pCtx Pointer to console manager context
 * @param pParams Pointer to command parameters list
 * @return 4GMX result code
 */
UINT32 LteMac_CreatePhy(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 rc;
    UINT32 mode;
    UINT32 icpu_emul = 0; // Emulate I-CPU communication

    mode = PHYINIT_NEW_MAC_TO_PHY_INTERFACE 
        | PHYINIT_PHY_MAINTAINS_PBCH 
        | PHYINIT_ROBUST_CONTROL;  // default - all latest is enabled

    if (pParams->ParamCount > 0)
    {
        mode = StrToNum(pParams->ParamList[0]);
    }

    if (pParams->ParamCount > 1)
    {
        icpu_emul = StrToNum(pParams->ParamList[1]);
        Mac2PhyEnableICPUEmul(icpu_emul);
    }
    rc = MacCreatePhy(&mode);

    if (icpu_emul)
    {
        printf("CreatePHY is done [rc = %d] [I-CPU]\n", rc);
    }
    else
    {
        printf("CreatePHY is done [rc = %d]\n", rc);
    }

    return 0;
}

/** Handle 'phystart' command from console input
 *
 * @param pCtx Pointer to console manager context
 * @param pParams Pointer to command parameters list
 * @return 4GMX result code
 */
UINT32 LteMac_PhyStart(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    PHY_AUTORUN_CTX* pStartAutoRun = &pBsMacDesc->PhyAutoCtx;

    if (pParams->ParamCount < 3)
        return RC_CM_PARAM_ERROR;

    pStartAutoRun->AutoMode = StrToNum(pParams->ParamList[0]);
    pStartAutoRun->TimerResolution = StrToNum(pParams->ParamList[1]);
    pStartAutoRun->SFCount = StrToNum(pParams->ParamList[2]);

    if (pParams->ParamCount >= 4)
        pStartAutoRun->RxSduEna = StrToNum(pParams->ParamList[3]);
    else
        pStartAutoRun->RxSduEna = 0;

    uart_printf("\r\nStarting ... mode:[%d] resolution:[%d] SFN:[%d] RxSduLog[%d]\r\n",
                pStartAutoRun->AutoMode, pStartAutoRun->TimerResolution, pStartAutoRun->SFCount,
                pStartAutoRun->RxSduEna);

    uart_printf("Context SFN=%d SF=%d\r\n", getFrameNum(), getSubFrameNum());

    return eNbMacAutoRun(PhyAutoStart, IID_LTE);
}

/** Handle 'phystop' command from console input
 *
 * @param pCtx Pointer to console manager context
 * @param pParams Pointer to command parameters list
 * @return 4GMX result code
 */
UINT32 LteMac_PhyStop(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 rc = eNbMacAutoRun(PhyAutoStop, IID_LTE);
#if 0
    uart_printf("\nDone [rc = %d]: Context SFN=%d SF=%d\n", rc, getFrameNum(), getSubFrameNum());
#endif
    return 0;
}

/** Handle 'physhutdown' command from console input
 *
 * @param pCtx Pointer to console manager context
 * @param pParams Pointer to command parameters list
 * @return 4GMX result code
 */
UINT32 LteMac_PhyShutdown(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 rc = eNbMacAutoRun(PhyAutoShutdown, IID_LTE);
#if 0
    uart_printf("\nDone [rc = %d]: Context SFN=%d SF=%d\n", rc, getFrameNum(), getSubFrameNum());
#endif
    return 0;
}

/** Handle 'phyreconfig' command from console input
 *
 * @param pCtx Pointer to console manager context
 * @param pParams Pointer to command parameters list
 * @return 4GMX result code
 */
UINT32 LteMac_PhyReconfig(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 mode;
    UINT32 rc;

    mode = PHYINIT_NEW_MAC_TO_PHY_INTERFACE 
        | PHYINIT_PHY_MAINTAINS_PBCH 
        | PHYINIT_ROBUST_CONTROL;  // default - all latest is enabled

    if (pParams->ParamCount > 0)
    {
        mode = StrToNum(pParams->ParamList[0]);
    }

    rc = MacReconfigPhy(&mode);

    return rc;
}

/** Handle 'logplay' command from console input
 *
 * @param pCtx Pointer to console manager context
 * @param pParams Pointer to command parameters list
 * @return 4GMX result code
 */
UINT32 LteMac_ApiPlay(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 StartSFN = 0;
    UINT32 FinishSFN = 0;
    UINT32 nLogPrep = 0;
    UINT32 tti = 0, bLogSdu = 0, di = 0xFF;

    if (pParams->ParamCount < 2)
    {
        return RC_CM_PARAM_ERROR;
    }

    if (logger_ctx.StoragePtr == NULL)
    {
        uart_printf("\r\n");
        uart_printf("\r\n");
        uart_printf("Log storage is NULL!\r\n");
        return 1;
    }

    StartSFN  	= CMgrGetParam (pParams, 0, 0);
    FinishSFN 	= StartSFN + CMgrGetParam (pParams, 1, 0xFFFFFFFF) - 1;
    tti 		= CMgrGetParam (pParams, 2, 1);
    di         	= CMgrGetParam (pParams, 3, 0xFF);
    bLogSdu   	= CMgrGetParam (pParams, 4, APIPLAYER_DEF_LOG_SDU_CTRL);
    nLogPrep  	= CMgrGetParam (pParams, 5, 0);

    if (nLogPrep != 0)
    {
        uart_printf("\r\n");
        uart_printf("\r\n");
        uart_printf("  Restoring log from (0x%x) (%d) ... ", api_log_1ms_data, api_log_1ms_size);
        memcpy(logger_ctx.StoragePtr->hdr, api_log_1ms_data, api_log_1ms_size);
        uart_printf("OK\r\n");
    }
    return ApiPlayerStart((PUINT8)logger_ctx.StoragePtr, StartSFN, FinishSFN, tti, di, bLogSdu);
}

/** Process PHY_START.req (84xxx-SWG-009-A) command as a separate scheduled thread
 *
 * @param p Unused
 * @return 4GMX result code
 */
UINT32 PhyStartThread(void *p)
{
    eNbMacAutoRun(PhyAutoStart, IID_LTE);

    PhyStartComplete = 1;
    return 0;
}

/** Process PHY_STOP.req (84xxx-SWG-009-A) command as a separate scheduled thread
 *
 * @param p Unused
 * @return 4GMX result code
 */
UINT32 PhyStopThread(void *p)
{
    eNbMacAutoRun(PhyAutoStop, IID_LTE);
    PhyStartComplete = 1;

    return 0;
}

UINT32 LteMac_NMMStart(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 mode = 0;

    uart_printf("[%s]\n", __FUNCTION__);

    if (pParams->ParamCount > 0)
    {
        mode = StrToNum(pParams->ParamList[0]);
    }

    MacNMMStart(mode);

    return 0;
}

UINT32 LteMac_NMMStop(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 mode = 0;

    uart_printf("[%s]\n", __FUNCTION__);

    if (pParams->ParamCount > 0)
    {
        mode = StrToNum(pParams->ParamList[0]);
    }

    MacNMMStop(mode);

    return 0;
}

UINT32 LteMac_NMMCmd(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 mode = 0;
    UINT32 param2;

    if (pParams->ParamCount > 0)
    {
        mode = StrToNum(pParams->ParamList[0]);
        if (pParams->ParamCount > 1)
        {
            param2 = StrToNum(pParams->ParamList[1]);
            mode |= param2 << 16;
        }
    }
    else
    {
        uart_printf("nmmcmd needs one parameter:\n\t 0:NMM_START, 1:NMM_STOP, 2:NMM_RSSI_MEAS_REQ, 3:NMM_STOP_RSSI_MEAS_REQ \n \
            \t4:NMM_CELL_SEARCH_REQ, 5:NMM_STOP_CELL_SEARCH_REQ, 6:NMM_PBCH_CONFIG_REQ, 7:NMM_STOP_PBCH_REQ\n \
            \t8:NMM_SIB1_CONFIG_REQ, 9:NMM_BCCH_CONFIG_REQ, 10:NMM_STOP_BCCH_REQ");

        return 0;
    }

    MacNMMCmd(mode);

    return 0;
}

static RESULTCODE MacNmmApi(ApiHeader * msg)
{
    RESULTCODE ret = SUCCESS;
    PGENMSGDESC pPhyMsg = (PGENMSGDESC)(msg+1);

    LteMacSendToPhyNmmMessage(pPhyMsg->msgType, pPhyMsg, 1);

    return ret;

}


/*UMTSNMM*/
UINT32 UMTS_NMMCmd(CMgrCmdCtx *pCtx, CMgrCmdParamCtx *pParams)
{
    UINT32 rc;
    UINT32 mode = 0;
    UINT32 icpu_emul = 0; // Emulate I-CPU communication

    if (pParams->ParamCount > 0)
    {
        mode = StrToNum(pParams->ParamList[0]);
    }
    else
    {
        uart_printf("UMTSNMM needs one parameter or parameter err: \n\r \
            \t---0:UMTSNMM_INIT \n\r \
            \t---1:UMTSNMM_STOP \n\r \
            \t---2:UMTSNMM_CELL_SEARCH_START\n\r \
            \t---3:UMTSNMM_CELL_SEARCH_STOP \n\r \
            \t---4:UMTSNMM_BCH_DECODER_START\n\r \
            \t---5:UMTSNMM_BCH_DECODER_STOP \n\r");
        return 0;
    }

    rc = MacUmtsNMMCmd(mode);

    if (icpu_emul)
    {
        uart_printf("nmmcmd is done [rc = %d] [I-CPU]\n", rc);
    }
    else
    {
        uart_printf("nmmcmd is done [rc = %d]\n", rc);
    }

    return 0;
}
