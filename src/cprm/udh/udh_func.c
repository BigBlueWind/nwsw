
/**
 * @file    udh_func.c
 * @brief This file contains the functions for udh interfaces
 *          for UEH, eGTP, PDCP
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Bhogeswar M, Madhu K, 27Aprl2011:
 *     a) Created initial version
 */

#include "ludh.h"
#include "udh_includes.h"
#include "sendToOam.h"
#include "ctr_id.h"
#include "eg_util.h"

EXTERN Queue       *g_udh_prio0_q;
EXTERN Queue       *g_tucl_prio0_q;
EXTERN Queue       *g_gtpu_prio0_q;

/**************************************************************************************/
/***********************************  SM, BSM  -- FUNC  Start  ************************/
/**************************************************************************************/
/* BSM - UDH OPSTART Request
 * UDH will unpack the received request and Save it in UDH context
 * Sends SUCC/FAIL information to BSM
 * */
void udhProcOpStartReq(void)
{
  UdhCb  *udhCb = getUdhCb();

  /* TODO: needs to move when cell delete handling */
  udhDeleteAllContexts();

  if(udhCb->ueInactivityCheckIntervalSecs > 0)
  {
   /* UDH expects the timer values to be in seconds */
   logTmr("UE Inactivity Check Interval=%d secs", udhCb->ueInactivityCheckIntervalSecs);
   SRegSecResTmr( ENTUDH, 0, 1 /*udhCb->ueInactivityCheckIntervalSecs*/ , udhHandleInactivityTimeout);
  }
  /* Start UL inactivity timer */
  if(udhCb->ulInactivityChkIntvInSecs > 0)
  {
    logTmr("UL Inactivity Timer Started, interval=%d secs", udhCb->ulInactivityChkIntvInSecs);
    SRegSecResTmr( ENTUDH, 0, udhCb->ulInactivityChkIntvInSecs , udhHandleUlInactivityTimeout);
  }

  if(0 < udhCb->ueThrptTmrInvlSec)
  {
    SRegSecResTmr( ENTUDH, 0, udhCb->ueThrptTmrInvlSec, udhHandleThroughputTimeout);
  }
  sendUdhMsgToBsm( EVT_UDH_BSM_OPSTART_SUCC, 0, NULL );
  udhCb->totalActvRabs = 0;
  udhCb->numUesConnected = 0;
}

/* BSM - UDH OPSTOP Request
 * UDH will unpack the received request and Save it in UDH context
 * Sends SUCC/FAIL information to BSM
 * */
void udhProcOpStopReq(void)
{
  UdhCb  *udhCb = getUdhCb();

  if(udhCb->ueInactivityCheckIntervalSecs > 0)
  {
    SDeregSecResTmr(ENTUDH, 0, 1, udhHandleInactivityTimeout);
  }

  if(udhCb->ulInactivityChkIntvInSecs > 0)
  {
    SDeregSecResTmr(ENTUDH, 0, udhCb->ulInactivityChkIntvInSecs, udhHandleUlInactivityTimeout);
  }

  if(0 < udhCb->ueThrptTmrInvlSec)
  {
    SDeregSecResTmr(ENTUDH, 0, udhCb->ueThrptTmrInvlSec, udhHandleThroughputTimeout);
  }
//      udhDeleteAllContexts();
  sendUdhMsgToBsm( EVT_UDH_BSM_OPSTOP_CMPL, 0, NULL );
}

/**
 * @param    transId         transId received in request message
 * @param    errorNum        BS Error number
 * @param    failReason      UDH failure reason
 * @details
 * Udh configuratin failed, Send Config fail to BSM
 */
void udhSendBsmUdhConfigReqFail(
  IN UINT16       transId,
  IN BsErrorValue errorNum,
  IN UINT8        *failReason
  )
{
  MsgUdhBsmCfgFail bsmUdhCfgFail;
  clearBytes( &bsmUdhCfgFail, sizeof(MsgUdhBsmCfgFail) );

  bsmUdhCfgFail.transId            = transId;
  bsmUdhCfgFail.errorDesc.errorNum = errorNum;
#if 0 /* Manu: bug - size has to be the capacity of the buffer, not passed string */
  snprintf( (char*)bsmUdhCfgFail.errorDesc.failReason, strlen((const char*)failReason)+1, "%s", failReason );
#endif
  snprintf( (char*)&bsmUdhCfgFail.errorDesc.failReason[0], sizeof(bsmUdhCfgFail.errorDesc.failReason) -1, "%s", failReason );

  sendUdhMsgToBsm( EVT_UDH_BSM_CFG_FAIL, sizeof(MsgUdhBsmCfgFail), (U8*)&bsmUdhCfgFail);
}

/* BSM - UDH Config request
 * UDH will unpack the received request and Save it in UDH context
 * Sends SUCC/FAIL information to BSM
 * */
TBOOL udhProcBsmUdhCfgReq(
  Buffer   *mBuf
  )
{
  MsgBsmUdhCfgReq  bsmUdhCfg;
  MsgUdhBsmCfgSucc bsmUdhCfgSucc;
  MsgLen           cCnt;
  UdhCb           *udhCb = NULLP;
  S16              ret = ROK;

  udhCb = getUdhCb();
  ret = SCpyMsgFix( mBuf, 0, sizeof(MsgBsmUdhCfgReq), (Data*)&bsmUdhCfg , &cCnt );
  if(ret == ROK)
  {
    logInfo("start-rnti- %d max-conn-ues- %d ue-inactv-chk-interval %d ue-thpt-timer-intvl %d ul-inactv-chk-interval %d",
               bsmUdhCfg.startRnti, bsmUdhCfg.maxConnectedUes, bsmUdhCfg.ueInactivityCheckIntervalSecs,
               bsmUdhCfg.ueThroughtputTimerInSecs, bsmUdhCfg.ulInactivityIntervalInSecs); 
    udhCb->ueInactivityCheckIntervalSecs = bsmUdhCfg.ueInactivityCheckIntervalSecs;
    udhCb->startRnti                     = bsmUdhCfg.startRnti;
    udhCb->maxConnectedUes               = bsmUdhCfg.maxConnectedUes;
    udhCb->ueThrptTmrInvlSec             = bsmUdhCfg.ueThroughtputTimerInSecs;
    udhCb->ulInactivityChkIntvInSecs     = bsmUdhCfg.ulInactivityIntervalInSecs;
    udhCb->memoryDiagCfg                 = bsmUdhCfg.memoryDiagCfg;    
    /*
     * Update run-time memory leak control flag
     */
   configMemLeakDetection(udhCb->memoryDiagCfg);
  }

  else
  {
    udhSendBsmUdhConfigReqFail(bsmUdhCfg.transId, BS_ERR_LENGTH_MISMATCH_FOR_RCVD_MSG, (UINT8*)"bsmUdhCfg Message Copy failed");
    return ret;
  }

  ret = udhInitUeCb();
  if(ret != ROK )
  {
    udhSendBsmUdhConfigReqFail(bsmUdhCfg.transId, BS_ERR_OUT_OF_MEMORY, (UINT8*)"Mem Allocation failed for ueInfoPtrs");
    return ret;
  }
  clearBytes( &bsmUdhCfgSucc, sizeof(MsgUdhBsmCfgSucc) );
  bsmUdhCfgSucc.transId                = bsmUdhCfg.transId;
  sendUdhMsgToBsm( EVT_UDH_BSM_CFG_SUCC, sizeof(MsgUdhBsmCfgSucc), (U8*)&bsmUdhCfgSucc);
  return ret;
}

/* Save configuration, Send Succ/Unsucc Confirm Message to SM */
PUBLIC S16 udhProcSapCfgReq(
  Pst       *pst,
  CmSapCfg  *sapCfg
  )
{
  SapCb  *sapToBeCfg;
  SapCb  *udhSmSapCb;
  Pst    *smPst = NULLP;;
  UdhCb  *udhCb = getUdhCb();
  
  udhSmSapCb = &(udhCb->smSap);
  if(udhSmSapCb->sapState ==  SAP_NOT_CONFIGURED)
  {
    udhSmSapCb = &(udhCb->smSap);

    udhSmSapCb->sapCfg.pst.srcProcId = SM_UDH_PROC;
    udhSmSapCb->sapCfg.pst.dstProcId = SM_SM_PROC;

    udhSmSapCb->sapCfg.pst.dstEnt  = ENTSM;
    udhSmSapCb->sapCfg.pst.dstInst = (Inst)0;
    udhSmSapCb->sapCfg.pst.srcEnt  = ENTUDH;
    udhSmSapCb->sapCfg.pst.srcInst = (Inst)0;

    udhSmSapCb->sapCfg.pst.prior = (Prior)UEHSM_MSGPRIOR;
    udhSmSapCb->sapCfg.pst.route = (Route)RTESPEC;
    udhSmSapCb->sapCfg.pst.event = (Event)EVTNONE;
    udhSmSapCb->sapCfg.pst.region = DFLT_REGION;

    udhSmSapCb->sapCfg.pst.pool = UDH_POOL;
    udhSmSapCb->sapCfg.pst.selector  = SM_SELECTOR_LC;

    udhSmSapCb->sapState = SAP_CONFIGURED;
  }

  smPst = &(udhCb->smSap.sapCfg.pst);

  switch(sapCfg->sapId)
  {
    case(UDH_GTP_SAP) :  sapToBeCfg = &(udhCb->gtpSap);  break;
    case(UDH_PDCP_SAP):  sapToBeCfg = &(udhCb->pdcpSap); break;
    case(UDH_UEH_SAP) :  sapToBeCfg = &(udhCb->uehSap);  break;
    default:
      /* Pack and send unsuccessful configuration confirmation to SM */
      udhPkAndSendSmUdhSapCfgCfm(smPst, sapCfg->sapId, CM_CFG_NOK);
      return FALSE;
  }
  memcpy(&(sapToBeCfg->sapCfg), sapCfg, sizeof(CmSapCfg));
  sapToBeCfg->sapState = SAP_CONFIGURED;

  /* Pack and send successful configuration confirmation to SM */
  udhPkAndSendSmUdhSapCfgCfm(smPst, sapCfg->sapId, CM_CFG_OK);
  return TRUE;
}

/* Process Sm-Udh Bind Request */
PUBLIC S16 udhProcSmUdhBndReq
(
  Pst           *pst,
  CmSapBindReq  *cmSapBindReq
)
{
  S16           status = ROK ;
  CmSapBindCfm  cmSapBindCfm;
  SapCb         *sapToBeCfg = NULLP;
  UdhCb         *udhCb = getUdhCb();
  Pst           *smPst = NULLP;;

  cmSapBindCfm.sapId = cmSapBindReq->sapId;

  smPst = &(udhCb->smSap.sapCfg.pst);

  if((udhCb->gtpSap.sapState == SAP_NOT_CONFIGURED ) ||
     (udhCb->pdcpSap.sapState == SAP_NOT_CONFIGURED) ||
     (udhCb->uehSap.sapState == SAP_NOT_CONFIGURED) )
  {
    cmPkAndSendUdhSmBndCfm(smPst, &cmSapBindCfm , CM_BND_NOK);     /* Send Bind Confirm Fail to SM */
    logError("udhHandleSmEvents: Bind request received before SAP configuration\
         gtpSap.sapState = %d , pdcpSap.sapState = %d , uehSap.sapState = %d \n",\
         udhCb->gtpSap.sapState,udhCb->pdcpSap.sapState,udhCb->uehSap.sapState );
    RETVALUE(RFAILED);
  }

  switch(cmSapBindReq->sapId)
  {
   case(UDH_GTP_SAP) :
   {
     sapToBeCfg = &(udhCb->gtpSap);
     status = cmPkEgtBndReq(&sapToBeCfg->sapCfg.pst, sapToBeCfg->sapCfg.suId , sapToBeCfg->sapCfg.spId );
     if(status == RFAILED)
     {
       cmPkAndSendUdhSmBndCfm(smPst, &cmSapBindCfm, CM_BND_NOK);     /* Send Bind Confirm Fail to SM */
       RETVALUE(status);
     }
     break;
   }

   case(UDH_PDCP_SAP):
   {
     sapToBeCfg = &(udhCb->pdcpSap);
     status = cmPkPjuBndReq(&sapToBeCfg->sapCfg.pst, sapToBeCfg->sapCfg.suId , sapToBeCfg->sapCfg.spId);
     if(status == RFAILED)
     {
       cmPkAndSendUdhSmBndCfm(smPst, &cmSapBindCfm, CM_BND_NOK);     /* Send Bind Confirm Fail to SM */
       RETVALUE(status);
     }
     break;
   }
   default:
   {
     /* Pack and send unsuccessful configuration confirmation to SM */
     cmPkAndSendUdhSmBndCfm(smPst, &cmSapBindCfm, CM_BND_NOK);
     RETVALUE(RFAILED);
   }
  }
  RETVALUE(ROK);
  /* PDCP Bind Request Will be sent after receiving GTP Bind CFM */
}

/* Process Debug control information from SM i.e log level settings from SM  */
PUBLIC S16 udhProcSmUdhDbgCntrl(
  SmUdhLogConfig  *smUdhLogConfig
  )
{
  LogContext  *logContext  = getLogContext();

  logContext->logConfig.logMask1 = smUdhLogConfig->logMask1;

  RETVALUE(ROK);
}

/**
 * Fills Active Rabs Stats into Active Rab Status Confirm message
 */
PUBLIC S16 udhFillActvRabStats(
  UdhUeCb                     *udhUeCb,
  MsgUdhBsmActiveRabStatusRsp *actRabStsRsp
  )
{
  U16  rbIdx = 0;
  U16  noOfRabsStatusFilled = 0;

  for(rbIdx = 0; ((noOfRabsStatusFilled < udhUeCb->numRabs) && (rbIdx< UEH_MAX_RABS_IN_UE) ); rbIdx++)
  {
    if(UDH_RAB_ACTIVE == udhUeCb->rabCbTable[rbIdx].isActive)
    {
      actRabStsRsp->activeRabStatus[actRabStsRsp->numElem] = udhUeCb->rabCbTable[rbIdx].activeRabStatus;
      actRabStsRsp->activeRabStatus[actRabStsRsp->numElem].index = actRabStsRsp->numElem + 1;
      actRabStsRsp->activeRabStatus[actRabStsRsp->numElem].gtpKiloBytesDL = (udhUeCb->rabCbTable[rbIdx].rabCounters.totalKBytesReceivedFromGtp)/1024;
      actRabStsRsp->activeRabStatus[actRabStsRsp->numElem].gtpKiloBytesUL = (udhUeCb->rabCbTable[rbIdx].rabCounters.totalKBytesReceviedFromPdcp)/1024;
      actRabStsRsp->numElem++;
      noOfRabsStatusFilled ++;
    }
    /* Do not fill the array if limit has reched */
    if(actRabStsRsp->numElem >= MAX_ROWS_IN_ACTIVERABSTATUS_TABLE)
      break;
  }
  RETVALUE(ROK);
}

/**
 * Process Active Rabs Status recieved from BSM
 */
void udhProcActiveRabStatusReq(
    Buffer *mBuf
  )
{
  U16                          ueIdx = 0;
  UdhCb                        *udhCb = NULLP;
  UdhUeCb                      *ueCb = NULLP;
  MsgLen                       cCnt = 0;
  MsgBsmUdhActiveRabStatusReq  actRabStsReq;
  MsgUdhBsmActiveRabStatusRsp  actRabStsRsp;
  U8                           noOfUesStatusTaken = 0;
  S16                          ret = ROK;

  logFunc("%s",__func__);
  clearBytes(&actRabStsReq, sizeof(MsgBsmUdhActiveRabStatusReq));

  ret = SCpyMsgFix(mBuf, 0, sizeof(MsgBsmUdhActiveRabStatusReq), (Data*)&actRabStsReq, &cCnt);
  if(ret != ROK) /* TODO: Need to send alarm because not able to decode cmdRef */
  {
    logError("%s", "SCpyMsgFix Failed to copy actRabStsReq");
    return;
  }

  clearBytes(&actRabStsRsp, sizeof(MsgUdhBsmActiveRabStatusRsp));
  
  actRabStsRsp.cmdRef = actRabStsReq.cmdRef;


  udhCb = getUdhCb();

  if( 0 EQ udhCb->numUesConnected )
  {
    /* No UEs are connected.*/
    sendUdhMsgToBsm(EVT_UDH_BSM_ACTIVE_RAB_STATUS_RSP, sizeof(MsgUdhBsmActiveRabStatusRsp), (UINT8*)&actRabStsRsp);
    return;
  }

  /* To send all Active UEs RABs if internalUeIdentity is not specified */
  if(0 == actRabStsReq.internalUeIdentity)
  {
    for(ueIdx = 0; ((ueIdx < UDH_MAX_UE_POOL) && (noOfUesStatusTaken < (udhCb->numUesConnected) )) ; ueIdx++)
    {
      ueCb = udhCb->ueInfoPtrs[ueIdx];
      if(NULLP != ueCb)
      {
        udhFillActvRabStats(ueCb, &actRabStsRsp);
        noOfUesStatusTaken ++;
      }
    }
  }
  /* Send specified Active UE RABs Status */
  else
  {
    ueIdx = UDH_GET_UEIDX_BY_UEID(actRabStsReq.internalUeIdentity);
    ueCb = udhCb->ueInfoPtrs[ueIdx];
    if(NULLP != ueCb)
    {
      if(actRabStsReq.internalUeIdentity == ueCb->rabCbTable[0].pdcpId.ueId)
      {
        udhFillActvRabStats(ueCb, &actRabStsRsp);
      }
    }
  }
  sendUdhMsgToBsm(EVT_UDH_BSM_ACTIVE_RAB_STATUS_RSP, sizeof(MsgUdhBsmActiveRabStatusRsp), (UINT8*)&actRabStsRsp);
}

/***************************************************************
*  Func: udhProcModStateReq
*  @detail: This function is called when MOD_STATE_REQ is received
*  from BSM and sends MOD_STATE_RSP. 
*  
**************************************************************/
void udhProcModStateReq(
    Buffer *mBuf
  )
{
  UdhCb                 *udhCb = getUdhCb();
  MsgLen                cCnt = 0;
  MsgBsmUdhModStateReq  modStateReq;
  MsgUdhBsmModStateRsp  modStateRsp;
  S16                   ret = ROK;

  logFunc("%s","udhProcModStateReq");

#ifdef PWAV_MEM2
  printPmTable(&g_blockBufTable, 0);
  printPmTable(&g_msgBufTable, 0);
  printPmTable(&g_staticBufTable, 0);
#endif

  logCtr("numUesConnected=%d|totalActvRabs=%d|numUeInactivityTimeouts=%d", 
      udhCb->numUesConnected,
      udhCb->totalActvRabs,
      udhCb->numUeInactivityTimeouts);

  clearBytes(&modStateReq, sizeof(MsgBsmUdhModStateReq));
  ret = SCpyMsgFix(mBuf, 0, sizeof(MsgBsmUdhModStateReq), (Data*)&modStateReq, &cCnt);
  if(ret != ROK) /* TODO: Need to send alarm because not able to decode cmdRef */
  {
    logError("%s", "SCpyMsgFix Failed to copy modStateReq");
    return;
  }

  clearBytes(&modStateRsp, sizeof(MsgUdhBsmModStateRsp));

  modStateRsp.cmdRef  = modStateReq.cmdRef;
  modStateRsp.tagInfo = modStateReq.tagInfo;
  modStateRsp.numErabs = udhCb->totalActvRabs;
  /* Throughput per second */
  if (udhCb->ueThrptTmrInvlSec != 0)
  {
    
    AGGREGATE_THROUGHPUT_KBPS(udhCb->udhDatThrput.dlDataCntPrvIntvl,udhCb->ueThrptTmrInvlSec,modStateRsp.aggregateDLThroughputKbps);
    AGGREGATE_THROUGHPUT_KBPS(udhCb->udhDatThrput.ulDataCntPrvIntvl,udhCb->ueThrptTmrInvlSec,modStateRsp.aggregateULThroughputKbps);
    
  }
  sendUdhMsgToBsm(EVT_UDH_BSM_MOD_STATE_RSP, sizeof(MsgUdhBsmModStateRsp), (UINT8*)&modStateRsp);
}
/***********************************   SM -- FUNC  Ends    ****************************/

/**************************************************************************************/
/***********************************  UEH  -- FUNC  Start  ****************************/
/**************************************************************************************/

PRIVATE CONSTANT UdhAddRabCfm udhAddRabCfm[] = /* TODO : Need to add functions for tightly coupled and light weight loosely coupled */
{
  cmPkAndSendAddRabCfm,        /* 0 - loosely coupled */
  cmPkAndSendAddRabCfm,        /* 1 - tightly coupled, stub layer */
};

/**
 * Process Active Rabs Status recieved from BSM
 */
void udhProcRabHistoryReq(Buffer *mBuf)
{
  U16                     rabIdx = 0;
  UdhCb                   *udhCb = NULLP;
  MsgLen                  cCnt = 0;
  MsgBsmUdhRabHistoryReq  rabHisReq; 
  MsgUdhBsmRabHistoryRsp  rabHisRsp;
  S16                   ret = ROK;

  clearBytes(&rabHisReq, sizeof(MsgBsmUdhRabHistoryReq));
  ret = SCpyMsgFix(mBuf, 0, sizeof(MsgBsmUdhRabHistoryReq), (Data*)&rabHisReq, &cCnt);
  if(ret != ROK) /* TODO: Need to send alarm because not able to decode cmdRef */
  {
    logError("%s", "SCpyMsgFix Failed to copy rabHisReq");
    return;
  }
  clearBytes(&rabHisRsp, sizeof(MsgUdhBsmRabHistoryRsp));

  rabHisRsp.cmdRef = rabHisReq.cmdRef;

  logInfo("%s","udhProcRabHistoryReq");

  udhCb = getUdhCb();
  if(udhCb != NULLP)
  {
    /* RAB Index is updated based on total number of RABs
     * i.e RAB History is updated like circular buffer and
     * History is shown as from oldest to new */
    if (udhCb->totalNumOfRabs + 1 > MAX_ROWS_IN_RABHISTORY_TABLE)
    {
      rabIdx = udhCb->rabIdx;
    }
    else
    {
      rabIdx = 0;
    }

    for ( ; rabIdx < udhCb->totalNumOfRabs ; rabIdx++)
    {
      rabHisRsp.rabHistory[rabHisRsp.numElem] = udhCb->rabHistory[rabIdx];
      rabHisRsp.numElem++;
    }
    if (udhCb->totalNumOfRabs == MAX_ROWS_IN_RABHISTORY_TABLE)
    {
      for (rabIdx = 0; rabIdx < udhCb->rabIdx ;rabIdx++)
      {
        rabHisRsp.rabHistory[rabHisRsp.numElem] = udhCb->rabHistory[rabIdx];
        rabHisRsp.numElem++;
      }
    }
  }
  sendUdhMsgToBsm(EVT_UDH_BSM_RAB_HISTORY_RSP, sizeof(MsgUdhBsmRabHistoryRsp), (UINT8*)&rabHisRsp);
}


void createAndSendUdhUehAddRabCfm(
  UdhUehAddRabCfm  *udhUehAddRabCfm
  )
{
  Pst *pst = getUdhUehPst();
  (*udhAddRabCfm[pst->selector]) (pst , udhUehAddRabCfm);
}

/**************************************/
/*      Delete RAB Success to UEH     */
/**************************************/
PRIVATE CONSTANT UdhDelRabCfm udhDelRabCfm [] = /* TODO : Need to add functions for tightly coupled and light weight loosely coupled */
{
   cmPkAndSendDelRabCfm,        /* 0 - loosely coupled */
   cmPkAndSendDelRabCfm,        /* 1 - tightly coupled, stub layer */
};

void createAndSendUdhUehDelRabCfm(
  UdhUehDelRabCfm  *udhUehDelRabCfm
  )
{
  Pst *pst = getUdhUehPst();
  (*udhDelRabCfm[pst->selector]) (pst , udhUehDelRabCfm);
}

UdhUeCb* getUeInfoPtrByUeId(
  CmLteRnti   ueId
  )
{
  U8          ueIndex = 0;

  ueIndex = UDH_GET_UEIDX_BY_UEID(ueId);
  if(ueIndex > UDH_MAX_UE_POOL)
  {
    return (NULLP);
  }
  else
  {
    return (g_udhCb.ueInfoPtrs[ueIndex]);
  }
}

PRIVATE CONSTANT UdhDelUeCfm udhDelUeCfm [] = /* TODO : Need to add functions for tightly coupled and light weight loosely coupled */
{
   cmPkAndSendDelUeCfm,        /* 0 - loosely coupled */
   cmPkAndSendDelUeCfm,        /* 1 - tightly coupled, stub layer */
};

void createAndSendUdhUehDelUeCfm(
  UdhUehDelUeCfm  *udhUehDelUeCfm
  )
{
  Pst *pst = getUdhUehPst();
  (*udhDelUeCfm[pst->selector]) (pst , udhUehDelUeCfm);
}
/**
 * Process Delete UE Request received from UEH
 */
PUBLIC S16 udhProcUehUdhDelUeReq(
  Pst              *pst,
  SuId              suId,
  UehUdhDelUeReq   *uehUdhDelUeReq
  )
{
  U8  rabId = 0;
  UdhCb *udhCb = getUdhCb();
  UdhRabCb *udhRabCb = NULLP;
  UdhUehDelUeCfm  udhUehDelUeCfm;
  clearBytes(&udhUehDelUeCfm, sizeof(UdhUehDelUeCfm));
  udhUehDelUeCfm.ueId = uehUdhDelUeReq->ueId;

  UdhUeCb *udhUeCb = getUeInfoPtrByUeId(uehUdhDelUeReq->ueId);
  if(NULLP == udhUeCb )
  {
    logError("ueId=%d|proc=DelUe|cause=ContextNotFound", uehUdhDelUeReq->ueId);
    udhUehDelUeCfm.status = RFAILED;
    createAndSendUdhUehDelUeCfm(&udhUehDelUeCfm); /* pack and post */
    RETVALUE(RFAILED);
  }
  for(rabId = 0; rabId < UEH_MAX_RABS_IN_UE; rabId++ )
  {
    /* Delete from mapping table */
    if(UDH_RAB_ACTIVE == udhUeCb->rabCbTable[rabId].isActive)
    {
      udhCb->totalActvRabs--;
      udhRabCb = &(udhUeCb->rabCbTable[(rabId)]);
     /* Prsently ENB is supporting only one DRB thats the reason this fucntion is called here */
      udhRabCb->isActive = UDH_RAB_INACTIVE;
      udhFillRabHistoryTable(udhRabCb,RABRELEASECAUSE_UECONTEXTRELEASED);
      udhSetLocTeId2RabCb(udhCb, udhUeCb->rabCbTable[rabId].locTeId, NULLP);
      logInfo("ueId=%d|rabId=%d|rabDeleted|reason=DelUeCmd", uehUdhDelUeReq->ueId, udhRabCb->rabId);
      clearBytes(&udhUeCb->rabCbTable[rabId],sizeof(UdhRabCb));
    }
  }
  /* Free memory */
  UEH_FREE(udhUeCb,  sizeof(UdhUeCb));
  udhCb->ueInfoPtrs[UDH_GET_UEIDX_BY_UEID(uehUdhDelUeReq->ueId)] = NULLP;

  udhUehDelUeCfm.status = ROK;
  udhCb->numUesConnected--;

  logInfo("ueId=%d|Tx=DelUeCfm|OK|totalUes=%d", uehUdhDelUeReq->ueId, udhCb->numUesConnected);
  createAndSendUdhUehDelUeCfm(&udhUehDelUeCfm);

  RETVALUE(ROK);
}
/*
 *      Fun:   udhCpyCmTptAddr
 *      Desc:  Copy Addr.
 *      Ret:   ROK   - ok
 *      File:  udh_func.c
 */
PUBLIC S16 udhCpyCmTptAddr
(
CmTptAddr *dst,         /* pointer to the destination CmTptAddr. */
CmTptAddr *src          /* pointer to the source CmTptAddr. */
)
{
   if((src == NULLP) || (dst == NULLP))
   {
      RETVALUE(RFAILED);
   }

   dst->type = src->type;
   if(src->type == CM_INET_IPV4ADDR_TYPE)
   {
      dst->u.ipv4TptAddr.port = src->u.ipv4TptAddr.port;
      dst->u.ipv4TptAddr.address = src->u.ipv4TptAddr.address;
   }
   else if(src->type == CM_INET_IPV6ADDR_TYPE)
   {
      dst->u.ipv6TptAddr.port = src->u.ipv6TptAddr.port;
      cmMemcpy((U8*)dst->u.ipv6TptAddr.ipv6NetAddr, (U8*)src->u.ipv6TptAddr.ipv6NetAddr,
            sizeof(CmInetIpAddr6));
   }

   RETVALUE( ROK);
}

void udhUpdateRabStats(
  UdhRabCb         *rabCb
  )
{
  /* Active RAB Status Updation */
  rabCb->activeRabStatus.index = 0;
  rabCb->activeRabStatus.internalUeId = rabCb->pdcpId.ueId;
  rabCb->activeRabStatus.rabIdentity = rabCb->rabId;
  rabCb->activeRabStatus.drbIdentity = rabCb->pdcpId.rbId;
  rabCb->activeRabStatus.rabQci = rabCb->qci;
  fillDataAndTimeStampField(rabCb->activeRabStatus.rabSetupTime);
  rabCb->activeRabStatus.dlGtpTunnelId = rabCb->locTeId;
  rabCb->activeRabStatus.ulGtpTunnelId = rabCb->remTeid;
  /* these need to be updated from rabCounters when BSM asks for Rab Status */
  rabCb->activeRabStatus.gtpKiloBytesDL = 0;
  rabCb->activeRabStatus.gtpKiloBytesUL = 0;
//  rabCb->activeRabStatus.gtpPktsUL = 0;
//  rabCb->activeRabStatus.gtpPktsUL = 0;

  /* Fill the Establishmnet of Rab in Rab History table */
  udhFillRabHistoryTable(rabCb,RABRELEASECAUSE_NOTAPPLICABLE);
}

/**
 * Process Add Rab Request received from UEH
 */
PUBLIC S16 udhProcUehUdhAddRabReq(
  Pst              *pst,
  SuId              suId,
  UehUdhAddRabReq  *uehUdhAddRabReq
  )
{
  U16              ueId = 0;
  UdhCb            *udhCb = getUdhCb();
  UdhRabCb         *rabCb = NULLP;
  UdhUehAddRabCfm  udhUehAddRabCfm;

  clearBytes(&udhUehAddRabCfm, sizeof(UdhUehAddRabCfm));
  udhUehAddRabCfm.pdcpId = uehUdhAddRabReq->pdcpId;
  udhUehAddRabCfm.status = ROK;
  ueId = uehUdhAddRabReq->pdcpId.ueId;

  UdhUeCb *udhUeCb = getUeInfoPtrByUeId(ueId);
  if(NULLP == udhUeCb)
  {
    /* Allocate Memory for UE */
    UEH_ALLOC(&udhUeCb,  sizeof(UdhUeCb ));
    if(NULLP == udhUeCb)
    {
      logError("ueId=%d|rbId=%d|proc=AddRab|failed|cause=NoMem|req=%d bytes", ueId, uehUdhAddRabReq->pdcpId.rbId, sizeof(UdhUeCb));
      udhUehAddRabCfm.status = RFAILED;
      createAndSendUdhUehAddRabCfm(&udhUehAddRabCfm);
      RETVALUE(RFAILED);
    }
    udhCb->numUesConnected++;
    udhCb->ueInfoPtrs[UDH_GET_UEIDX_BY_UEID(ueId)] = udhUeCb;
  }

  /* Rab  - RabId
   * SRB1 -  1
   * SRB2 -  2
   * DRB starts from  -  3 to 10 */
  rabCb = &(udhUeCb->rabCbTable[UDH_GET_RBIDX_BY_RBID(uehUdhAddRabReq->pdcpId.rbId)]);
  if(UDH_RAB_ACTIVE == rabCb->isActive)
  {
    logError("ueId=%d|rbId=%d|proc=AddRab|failed|cause=context already present", ueId, uehUdhAddRabReq->pdcpId.rbId);
    udhUehAddRabCfm.status = RFAILED;
    createAndSendUdhUehAddRabCfm(&udhUehAddRabCfm);
    RETVALUE(RFAILED);
  }
  rabCb->isActive = UDH_RAB_ACTIVE;
  /* Mark this rab as active so that is does not get deleted in the first timeout */
  rabCb->isMontiored = FALSE ;

  udhUeCb->numRabs++;
  /* Update Active Rabs */
  udhCb->totalActvRabs++;

  udhCpyCmTptAddr(&(rabCb->dstIpAddr),&(uehUdhAddRabReq->dstIpAddr));
  udhCpyCmTptAddr(&(rabCb->srcIpAddr),&(uehUdhAddRabReq->srcIpAddr));
  udhCpyCmTptAddr(&(rabCb->frmDstIpAddr),&(uehUdhAddRabReq->frmDstIpAddr));

  rabCb->locTeId = uehUdhAddRabReq->locTeId;
  rabCb->remTeid = uehUdhAddRabReq->remTeid;
  rabCb->qci     = uehUdhAddRabReq->qci;
  rabCb->rabId   = uehUdhAddRabReq->rabId;
  rabCb->pdcpId  = uehUdhAddRabReq->pdcpId;

  /* Add this new rab to mapping table */
  udhSetLocTeId2RabCb(udhCb, rabCb->locTeId, rabCb);

  /* Initialise all counters:
   * No explicit initialisation is needed as rabCb has been set to 0 */
  logInfo("ueId=%d|rabId=%d|proc=AddRab|status=%d|totalUes=%d|totalRabs=%d", ueId, uehUdhAddRabReq->rabId, udhUehAddRabCfm.status, udhCb->numUesConnected, udhCb->totalActvRabs);
  createAndSendUdhUehAddRabCfm(&udhUehAddRabCfm); /* pack and post */

  /* Update Rab statistics */
  udhUpdateRabStats(rabCb);

  RETVALUE(ROK);
}


/**
 * Process Delete Rab Request received from UEH
 */
PUBLIC S16 udhProcUehUdhDelRabReq(
  Pst              *pst,
  SuId              suId,
  UehUdhDelRabReq  *uehUdhDelRabReq
  )
{
  U16              ueId = 0;
  U8               rbId = 0;
  UdhRabCb         *udhRabCb = NULLP;
  UdhCb            *udhCb = getUdhCb();
  UdhUeCb          *udhUeCb = NULLP;
  UdhUehDelRabCfm  udhUehDelRabCfm;

  ueId = uehUdhDelRabReq->pdcpId.ueId;
  rbId = uehUdhDelRabReq->pdcpId.rbId;

  clearBytes(&udhUehDelRabCfm, sizeof(UdhUehDelRabCfm));
  udhUehDelRabCfm.pdcpId = uehUdhDelRabReq->pdcpId;
  udhUehDelRabCfm.status = ROK;

  udhUeCb = getUeInfoPtrByUeId(ueId);
  if(NULLP == udhUeCb)
  {
    logError("ueId=%d|rbId=%d|proc=DelRab|failed|cause=Ctx not found", ueId, rbId);
    udhUehDelRabCfm.status = RFAILED;
    createAndSendUdhUehDelRabCfm(&udhUehDelRabCfm);
    RETVALUE(RFAILED);
  }
  if(0 == udhUeCb->numRabs)
  {
    logError("ueId=%d|rbId=%d|proc=DelRab|failed|cause=No Rabs found", ueId, rbId);
    udhUehDelRabCfm.status = RFAILED;
    createAndSendUdhUehDelRabCfm(&udhUehDelRabCfm);
    logError("Self Cleanup|deleting context for ueId=%d", ueId);
    UEH_FREE(udhUeCb,  sizeof(UdhUeCb));
    udhCb->ueInfoPtrs[UDH_GET_UEIDX_BY_UEID(ueId)] = NULLP;
    RETVALUE(RFAILED);
  }

  udhRabCb = &(udhUeCb->rabCbTable[UDH_GET_RBIDX_BY_RBID(rbId)]);
  if(UDH_RAB_INACTIVE == udhRabCb->isActive)
  {
    logError("ueId=%d|rbId=%d|proc=DelRab|failed|cause=ContextNotFound|rabs=%d", ueId, rbId, udhUeCb->numRabs);
    udhUehDelRabCfm.status = RFAILED;
    createAndSendUdhUehDelRabCfm(&udhUehDelRabCfm);
    RETVALUE(RFAILED);
  }
  udhSetLocTeId2RabCb(udhCb, udhRabCb->locTeId, NULLP);

  /* Fill the Release information of Rab in Rab History table */
  udhFillRabHistoryTable(udhRabCb,uehUdhDelRabReq->rabReleaseCause);

  udhUeCb->numRabs--;
  /* Update Active Rabs */
  udhCb->totalActvRabs--;
  udhRabCb->isActive = UDH_RAB_INACTIVE;

  if(udhUeCb->numRabs EQ 0)
  {
    /* Free memory */
    UEH_FREE(udhUeCb,  sizeof(UdhUeCb));
    udhCb->ueInfoPtrs[UDH_GET_UEIDX_BY_UEID(ueId)] = NULLP;
    udhCb->numUesConnected--;
    logInfo("ueId=%d|CtxDeleted|reason=allRabsDeleted", ueId);
  }

  createAndSendUdhUehDelRabCfm(&udhUehDelRabCfm); /* pack and post */
  logInfo("ueId=%d|Tx=DelRabCfm|status=%d|totalUes=%d|totalRabs=%d", ueId, udhUehDelRabCfm.status, udhCb->numUesConnected, udhCb->totalActvRabs);

  RETVALUE(ROK);
}

/**
 * Filling the Rab History Table 
 */
void udhFillRabHistoryTable(UdhRabCb *rabCb,RabReleaseCause rabReleaseCause)
{
  U16 idx = 0;
  UdhCb *udhCb = NULLP;

  logInfo("%s","udhFillRabHistoryTable: Filling the Rab History Table");

  udhCb = getUdhCb();
  if(udhCb != NULLP)
  {
    if(rabReleaseCause != RABRELEASECAUSE_NOTAPPLICABLE)
    {
      for(idx = 0;idx < udhCb->totalNumOfRabs; idx++)
      {
        if(udhCb->rabHistory[idx].internalUeId ==  rabCb->pdcpId.ueId)
        {
          if(udhCb->rabHistory[idx].rabIdentity == rabCb->rabId)
          {
            fillDataAndTimeStampField(udhCb->rabHistory[idx].rabReleaseTime);
            udhCb->rabHistory[idx].rabReleaseCause = rabReleaseCause;
          }
        }
      } 
    }
    else
    {
      if(MAX_ROWS_IN_RABHISTORY_TABLE == udhCb->rabIdx)
      {  
        udhCb->rabIdx = 0;
      }
      if(udhCb->totalNumOfRabs < MAX_ROWS_IN_RABHISTORY_TABLE)
      {
        udhCb->totalNumOfRabs++;
      }  
      /* Storing the elements in Rab History Table */
      udhCb->rabHistory[udhCb->rabIdx].index = udhCb->rabIdx + 1;
      udhCb->rabHistory[udhCb->rabIdx].internalUeId = rabCb->pdcpId.ueId;
      udhCb->rabHistory[udhCb->rabIdx].rabIdentity = rabCb->rabId;
      udhCb->rabHistory[udhCb->rabIdx].drbIdentity = rabCb->pdcpId.rbId;
      udhCb->rabHistory[udhCb->rabIdx].rabQci = rabCb->qci;
      fillDataAndTimeStampField(udhCb->rabHistory[udhCb->rabIdx].rabSetupTime);
      udhCb->rabHistory[udhCb->rabIdx].rabReleaseCause = RABRELEASECAUSE_NOTAPPLICABLE; 
      udhCb->rabHistory[udhCb->rabIdx].dlGtpTunnelId = rabCb->locTeId;
      udhCb->rabHistory[udhCb->rabIdx].ulGtpTunnelId = rabCb->remTeid;
      udhCb->rabIdx++;
    }
  }
}
/**
 * Filling the Down link data in kilo Bytes in Rab History Table 
 */
void udhFillDlKbInRabHisTable(UdhRabCb *udhRabCb)
{
  U16 idx = 0;
  UdhCb *udhCb = NULLP;

  udhCb = getUdhCb();
  if(udhCb != NULLP)
  {
    for(idx = 0;idx < udhCb->totalNumOfRabs; idx++)
    {
      if(udhCb->rabHistory[idx].internalUeId ==  udhRabCb->pdcpId.ueId)
      {
        if(udhCb->rabHistory[idx].rabIdentity == udhRabCb->rabId)
        {
          udhCb->rabHistory[idx].gtpKiloBytesDL = udhRabCb->rabCounters.totalKBytesReceivedFromGtp / 1024;
        }
      }
    }
  }
}

/**
 * Filling the Uplink data in kilo Bytes in Rab History Table 
 */
void udhFillUlKbInRabHisTable(UdhRabCb *udhRabCb)
{
  U16 idx = 0;
  UdhCb *udhCb = NULLP;

  udhCb = getUdhCb();
  if(udhCb != NULLP)
  {
    for(idx = 0;idx < udhCb->totalNumOfRabs; idx++)
    {
      if(udhCb->rabHistory[idx].internalUeId ==  udhRabCb->pdcpId.ueId)
      {
        if(udhCb->rabHistory[idx].rabIdentity == udhRabCb->rabId)
        {
          udhCb->rabHistory[idx].gtpKiloBytesUL = udhRabCb->rabCounters.totalKBytesReceviedFromPdcp / 1024 ;
        }
      }
    }
  }
}

PRIVATE CONSTANT UehBndCfm uehBndCfm [] = /* TODO : Need to add functions for tightly coupled and light weight loosely coupled */
{
  cmPkAndSendUdhUehBndCfm,        /* 0 - loosely coupled */
  cmPkAndSendUdhUehBndCfm,        /* 1 - tightly coupled, stub layer */
};


void createAndSendUdhUehBndCfm(
  Pst    *pst,
  SuId    suId,
  U8      status
  )
{
  (*uehBndCfm[pst->selector]) (pst , suId, status);
}

PUBLIC S16 udhProcUehUdhBndReq
(
  Pst       *pst,
  SuId      suId,
  SpId      spId
)
{
  S16      ret = ROK;           /* Return Value */
  UdhCb    *udhCb = getUdhCb();
  SapCb    *uehSap = &udhCb->uehSap;

  /* Verify UDH -UEH SAP State */
  switch(uehSap->sapState)
  {
    /* SAP is configured but not bound */
    case SAP_CONFIGURED:
    {
      /* copy bind configuration parameters  */
      uehSap->sapCfg.suId          = suId;
      uehSap->sapCfg.pst.dstProcId = pst->srcProcId;
      uehSap->sapCfg.pst.dstEnt    = pst->srcEnt;
      uehSap->sapCfg.pst.dstInst   = pst->srcInst;

      uehSap->sapCfg.pst.srcEnt    = pst->dstEnt;
      uehSap->sapCfg.pst.srcInst   = pst->dstInst;

       /* Update the State */
      uehSap->sapState = SAP_BOUND;
      break;
    }
    /* SAP is already bound */
    case SAP_BOUND:
    {
      /* Sap is already bound check source, destination Entity and Proc Id  */
      if ((uehSap->sapCfg.pst.dstProcId!= pst->srcProcId)
           || (uehSap->sapCfg.pst.dstEnt != pst->srcEnt)
           || (uehSap->sapCfg.pst.dstInst != pst->srcInst)
           || (uehSap->sapCfg.suId != suId))
      {
        logError("udhProcUehUdhBndReq: SAP is already bound Received Diff PST Original->\
            dstProcId = %d, dstEnt = %d,dstInst = %d, suId = %d : NEW -> dstProcId = %d, dstEnt = %d,dstInst = %d, suId = %d " ,\
                uehSap->sapCfg.pst.dstProcId, uehSap->sapCfg.pst.dstEnt,uehSap->sapCfg.pst.dstInst,uehSap->sapCfg.suId, \
                pst->srcProcId,pst->srcEnt,pst->srcInst,suId);
        ret = RFAILED;
      }
      break;
   }

   default: /* SAP_NOT_CONFIGURED */
   {
     logError("%s","udhProcUehUdhBndReq: SAP_NOT_CONFIGURED \n");
     ret = RFAILED;
     break;
   }
  }
  /* Pack and Post Bind Confirm to UEH */
  if (ret == ROK)
  {
    cmPkAndSendUdhUehBndCfm(&(uehSap->sapCfg.pst), uehSap->sapCfg.suId, CM_BND_OK);
  }
  else
  {
    cmPkAndSendUdhUehBndCfm(&(uehSap->sapCfg.pst), uehSap->sapCfg.suId, CM_BND_NOK);
  }
  RETVALUE(ROK);
}


/***********************************  UEH  -- FUNC  Ends   ****************************/


/**************************************************************************************/
/***********************************  GTP  -- FUNC  Start  ****************************/
/**************************************************************************************/

/*
 *       Fun :   udhProcEgtBndCfm
 *       Desc:   EGT Bind Confirm
 *       Ret :   ROK
 *       File:   udh_func.c
 */
PUBLIC S16 udhProcEgtBndCfm(
  Pst     *pst,
  SuId    suId,
  U8      status
  )
{
  UdhCb *udhCb = getUdhCb();
  CmSapBindCfm  cmSapBindCfm;

  if (CM_BND_OK == status)
  {
    udhCb->gtpSap.sapState = SAP_BOUND;
    cmSapBindCfm.sapId = UDH_GTP_SAP;
    cmPkAndSendUdhSmBndCfm(&udhCb->smSap.sapCfg.pst, &cmSapBindCfm, CM_BND_OK);
  }
  else
  {
    udhCb->gtpSap.sapState = SAP_NOT_CONFIGURED;
    logError("%s","udhProcEgtBndCfm: EGT SAP Configuration Failed");
    cmSapBindCfm.sapId = UDH_GTP_SAP;
    cmPkAndSendUdhSmBndCfm(&udhCb->smSap.sapCfg.pst, &cmSapBindCfm, CM_BND_NOK);
    RETVALUE(RFAILED);
  }
  RETVALUE(ROK);
}

/*
*
*       Fun:   EuLiEgtBndCfm - EGT Bind Cfm
*
*       Desc:  This function is used by to present Bind Cfm to EGT.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 EuLiEgtBndCfm(
    Pst     *pst,
    SuId    suId,
    U8      status
  )
{
   logFunc("%s", "EuLiEgtBndCfm");
   RETVALUE(udhProcEgtBndCfm(pst,suId,status));
}

/*
 *       Fun :   udhUpdateDlStats
 *       Desc:   Update Downlink data statistics
 *       Ret :   NONE
 *       File:   udh_func.c
 */
void udhUpdateDlStats(
  UdhRabCb      *udhRabCb,
  MsgLen        msgLen
  )
{
  UdhCb         *udhCb = getUdhCb();
  /* Increment the counters */
  udhRabCb->rabCounters.pktsReceivedFromGtpInUeInActvInterval++;
  udhRabCb->rabCounters.totalPktsReceivedFromGtp++;
  udhRabCb->rabCounters.bytesReceivedFromGtpInCurrentInterval += msgLen;
  udhRabCb->rabCounters.totalKBytesReceivedFromGtp += msgLen;

  /* Updating the Rab history table */
  udhFillDlKbInRabHisTable(udhRabCb);
  /* Throughput */
  udhCb->udhDatThrput.dlDataCntCrntIntvl += msgLen;
}

/*
 *       Fun :   udhProcEgtDatInd
 *       Desc:   EGT Data Indication
 *       Ret :   ROK
 *       File:   udh_func.c
 */
PUBLIC S16 udhProcEgtDatInd(
  Pst         *pst,
  SuId         suId,
  EgtUEvnt    *eguEvtMsg
  )
{
  U32            locTeid = 0;
  UdhRabCb      *udhRabCb = NULLP;
  UdhCb         *udhCb = getUdhCb();
  CmLtePdcpId   *pdcpId = NULLP;
  S16            ret = ROK;
  MsgLen         msgLen;

  locTeid = eguEvtMsg->u.egMsg->msgHdr.teId;

  if(locTeid EQ 0)
  {
    logError("Invalid TeId=%d", (int)locTeid);
    RETVALUE(RFAILED);
  }
  /* If Invalid TEID is received, drop the G-PDU */
  if(locTeid > UEH_MAX_TUNNEL_ID)
  {
    logError("Invalid TeId=%d", (int)locTeid);
    SPutMsg(eguEvtMsg->u.egMsg->u.mBuf);
    RETVALUE(RFAILED);
  }

  udhRabCb = udhGetRabCbByLocalTunnelId(udhCb, locTeid);
  if( NULL == udhRabCb )
  {
    logError("No RAB context found for TeId=%d", (int)locTeid);
    SPutMsg(eguEvtMsg->u.egMsg->u.mBuf);
    RETVALUE(RFAILED);
  }

  /* Allocate pdcpId as it goes in DataReq to PDCP.
   * This allocation is inefficient, a local variable can do.
   * It is done here as pack function expects it as allocated memory 
   */
  ret = SGetSBuf(udhCb->pdcpSap.sapCfg.pst.region, udhCb->pdcpSap.sapCfg.pst.pool, (Data **)&pdcpId, sizeof(CmLtePdcpId));
  if (ret != ROK)
  {
    logError("no mem for pdcpId.rbId=%d", udhRabCb->pdcpId.rbId);
    SPutMsg(eguEvtMsg->u.egMsg->u.mBuf);
    RETVALUE(RFAILED);
  }
  cmMemcpy ((U8 *)pdcpId, (U8 *)&(udhRabCb->pdcpId), sizeof(CmLtePdcpId));

  SFndLenMsg(eguEvtMsg->u.egMsg->u.mBuf, &msgLen); /* for statistics */


  /* Triggering Data Request towards PJU */
  UdhLiPjuDatReq(&(udhCb->pdcpSap.sapCfg.pst), /* pst->event will be set in Pack function */
                  udhCb->pdcpSap.sapCfg.spId,
                  pdcpId,
                  udhRabCb->rabCounters.totalPktsReceivedFromGtp,
                  eguEvtMsg->u.egMsg->u.mBuf);
  logMsgOut("UDH->PJU|Data|ueId=%d|rbId=%d|sduNum=%d", \
           udhRabCb->pdcpId.ueId, udhRabCb->pdcpId.rbId, udhRabCb->rabCounters.totalPktsReceivedFromGtp);
#if  0  /* This part of code added to findout whether multiple egMsg in Eg Event or not */
  if(eguEvtMsg->u.egMsg->next != NULLP)
  {
    logError("%s","udhProcEgtDatInd: **** Multiple EGTP Messages in one message **** ");
  }
#endif
  /* Update statistics */
  udhUpdateDlStats(udhRabCb , msgLen);

  RETVALUE(ROK);
}


/*
*
*       Fun:   EuLiEgtEguDatInd
*
*       Desc:  eGTP Data Ind.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_li.c
*
*/
PUBLIC S16 EuLiEgtEguDatInd(
    Pst         *pst,
    SuId        suId,
    EgtUEvnt    *eguEvtMsg
  )
{
  S16           ret = RFAILED;
  logFunc("%s", "EuLiEgtEguDatInd");

  ret = udhProcEgtDatInd(pst, suId, eguEvtMsg);
  
  /* Free the received GTP message */
  cmFreeMem(eguEvtMsg->u.egMsg);
  SPutSBuf(pst->region, pst->pool, (Data* )eguEvtMsg, sizeof(EgtUEvnt));

  RETVALUE(ret);
}

PRIVATE CONSTANT PjuDatReq UdhLiPjuDatReqMt[] = /* TODO : check Assignment of PDCP function for Tightly coupled */
{
  cmPkPjuDatReq,     /* 0 - loosely coupled */
  cmPkPjuDatReq,     /* 1 - tightly coupled, service user EU */
};

/*
 *       Fun:   UehLiPjuDatReq
 *       Desc:  Data Req
 *       Ret:   ROK
 *       File:  udh_func.c
 */
PUBLIC S16 UdhLiPjuDatReq
(
  Pst*         pst,
  SpId         spId,
  CmLtePdcpId *pdcpId,
  PjuSduId    sduId,
  Buffer      *mBuf
)
{
  (*UdhLiPjuDatReqMt[pst->selector])(pst, spId, pdcpId, sduId, mBuf);
  RETVALUE(ROK);
}

/***********************************  GTP  -- FUNC  Ends   ****************************/

/**************************************************************************************/
/***********************************  PDCP -- FUNC  Start  ****************************/
/**************************************************************************************/
/*
 *  Fun :   udhProcPjuBndCfm
 *  Desc:   PJU Bind Cfm
 *  Ret :   ROK
 *  File:   udh_func.c
 */
PUBLIC S16 udhProcPjuBndCfm(
  Pst         *pst,
  SuId        suId,
  U8          status
  )
{
  UdhCb *udhCb = getUdhCb();
  CmSapBindCfm  cmSapBindCfm;

  if (CM_BND_OK == status)
  {
    udhCb->pdcpSap.sapState = SAP_BOUND;
    cmSapBindCfm.sapId = UDH_PDCP_SAP;
    cmPkAndSendUdhSmBndCfm(&udhCb->smSap.sapCfg.pst, &cmSapBindCfm, CM_BND_OK);
  }
  else
  {
    udhCb->pdcpSap.sapState = SAP_NOT_CONFIGURED;
    logError("%s","udhProcPjuBndCfm: PDCP SAP Configuration Failed");
    cmSapBindCfm.sapId = UDH_PDCP_SAP;
    cmPkAndSendUdhSmBndCfm(&udhCb->smSap.sapCfg.pst, &cmSapBindCfm, CM_BND_NOK);
    RETVALUE(RFAILED);
  }
  RETVALUE(ROK);
}

/*
 *       Fun :   udhUpdateUlStats
 *       Desc:   Update Uplink data statistics
 *       Ret :   NONE
 *       File:   udh_func.c
 */
void udhUpdateUlStats(
  UdhRabCb      *udhRabCb,
  MsgLen        msgLen
  )
{
  UdhCb         *udhCb = getUdhCb();

  udhRabCb->rabCounters.pktsReceivedFromPdcpInUeInActvInterval++;
  udhRabCb->rabCounters.pktsReceivedFromPdcpInUlInActvInterval++;
  udhRabCb->rabCounters.bytesReceivedFromPdcpInCurrentInterval += msgLen;
  udhRabCb->rabCounters.totalKBytesReceviedFromPdcp += msgLen;
  udhRabCb->rabCounters.totalPktsReceviedFromPdcp++;
  /* updating the Rab history table  */
  udhFillUlKbInRabHisTable(udhRabCb);
  /* Throughput */
  udhCb->udhDatThrput.ulDataCntCrntIntvl += msgLen;
  
}


/*
 *  Fun :   udhProcPjuDatInd
 *  Desc:   PJU Data Indication
 *  Ret :   ROK
 *  File:   udh_func.c
 */
PUBLIC S16 udhProcPjuDatInd(
  Pst                 *pst,
  SuId                suId,
  CmLtePdcpId         *pdcpId,
  Buffer              *mBuf
  )
{
  UdhRabCb    *rabCb     = NULLP;
  UdhCb       *udhCb     = getUdhCb();
  EgtUEvnt    *eguEvtMsg = NULLP;
  MsgLen       msgLen    = 0;
  S16          ret       = ROK;
  Mem          sMem;
  EgUMsgHdr   *eguHdr    = NULLP;
  UdhUeCb     *udhUeCb   = NULLP;

  SFndLenMsg(mBuf, &msgLen);
  if (msgLen EQ 0)
  {
    logError("msgLen 0 ueId=%d, rbId=%d", pdcpId->ueId, pdcpId->rbId);
    goto error;
  }

  udhUeCb = getUeInfoPtrByUeId(pdcpId->ueId);
  if(NULLP == udhUeCb)
  {
    logError("UE context not found for ueId=%d, rbId=%d", pdcpId->ueId, pdcpId->rbId);
    goto error;
  }

  rabCb = &udhUeCb->rabCbTable[UDH_GET_RBIDX_BY_RBID(pdcpId->rbId)];
  if(NULLP == rabCb)
  {
   logError("no rab context found for ueId=%d, rbId=%d", pdcpId->ueId, pdcpId->rbId);
   goto error;
  }

  /* Fill EGTP Data Message */
  if ((ret = SGetSBuf(udhCb->gtpSap.sapCfg.pst.region, udhCb->gtpSap.sapCfg.pst.pool, (Data **)&eguEvtMsg, sizeof(EgtUEvnt))) != ROK)
  {
    logError("Memory allocation (%d bytes) fail for ueId=%d, rbId=%d, ret=%d", sizeof(EgtUEvnt), pdcpId->ueId, pdcpId->rbId, ret);
    goto error;
  }

  cmMemset((U8*)eguEvtMsg, 0, sizeof(EgtUEvnt));

  sMem.pool   = udhCb->gtpSap.sapCfg.pst.pool;
  sMem.region = udhCb->gtpSap.sapCfg.pst.region;

  if(cmAllocEvnt(sizeof(EgUMsg), EG_UTIL_MEM_SIZE, &sMem, (Ptr* )&((eguEvtMsg)->u.egMsg)) != ROK)
  {
    logError("No mem to send pkt to GTP for pdcpId.ueId=%d, pdcpId.rbId=%d, req(=%d)", pdcpId->ueId, pdcpId->rbId, EG_UTIL_MEM_SIZE);
    goto error;
  }

  SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));

  /* Populating Header Info */
  eguHdr = &(eguEvtMsg)->u.egMsg->msgHdr;
  eguHdr->msgType = EGT_GTPU_MSG_GPDU;
  eguHdr->nPduNmb.pres = FALSE;
  eguHdr->seqNumber.pres = FALSE;
  eguHdr->extHdr.udpPort.pres = FALSE;
  eguHdr->extHdr.pdcpNmb.pres = FALSE;
  eguHdr->teId = rabCb->remTeid;

  /* Fill egMsg */
  udhCpyCmTptAddr(&((eguEvtMsg)->u.egMsg->srcAddr),&rabCb->srcIpAddr);
  udhCpyCmTptAddr(&((eguEvtMsg)->u.egMsg->remAddr),&(rabCb->dstIpAddr));
  eguEvtMsg->u.egMsg->u.mBuf = mBuf;


  /* Trigger EGTP Data Req */
  UdhLiEgtEguDatReq(&(udhCb->gtpSap.sapCfg.pst), udhCb->gtpSap.sapCfg.spId, eguEvtMsg);

  /* Update counters */
  udhUpdateUlStats(rabCb , msgLen);

  RETVALUE(ROK);

  error:
  SPutSBuf(pst->region, pst->pool, (Data *)pdcpId, sizeof(CmLtePdcpId));
  SPutMsg(mBuf);
  if(eguEvtMsg != NULL)
  {
    egUFreeUiEvnt(eguEvtMsg);
  }
  /* TODO: Do we need packets dropped counter */
  RETVALUE(RFAILED);
}


PRIVATE CONSTANT EgtEguDatReq UdhLiEgtEguDatReqMt[] =
{
  EgUiEgtEguDatReq,          /* only make data as tightly coupled */
  cmPkEgtEguDatReq,          /* 1 - tightly coupled, service user EU */
};

/*
 *  Fun :  UdhLiEgtEguDatReq
 *  Desc:  Egt Data Request
 *  Ret :  ROK -      ok
 *  File:  udh_func.c
 */
PUBLIC S16 UdhLiEgtEguDatReq
(
  Pst          *pst,              /* post structure */
  SuId         suId,              /* service user id */
  EgtUEvnt     *egtUEvnt
)
{
  (*UdhLiEgtEguDatReqMt[pst->selector])(pst, suId, egtUEvnt);
  RETVALUE(ROK);
} /* end of UdhLiEgtEguDatReq() */


/*********************************** PDCP -- FUNC  Ends ********************************/

/*********************************** UE - INACTIVITY ********************************/
/** Checks if the given rab is inactive */
TBOOL isRabInactive(
  UdhRabCounters* rabCounters
  )
{
  if ((0 == rabCounters->pktsReceivedFromGtpInUeInActvInterval)
      &&
      (0 == rabCounters->pktsReceivedFromPdcpInUeInActvInterval))
   {
     return TRUE;
   }
  else
  {
    logCtr("Pkts from GTP %d, from PDCP %d", rabCounters->pktsReceivedFromGtpInUeInActvInterval,
           rabCounters->pktsReceivedFromPdcpInUeInActvInterval);
    rabCounters->pktsReceivedFromGtpInUeInActvInterval  = 0;
    rabCounters->pktsReceivedFromPdcpInUeInActvInterval = 0;
    return FALSE;
  }
}

TBOOL isRabInactiveInUlPath(
  UdhRabCounters* rabCounters
  )
{
  if(0 == rabCounters->pktsReceivedFromPdcpInUlInActvInterval)
  {
    return TRUE;
  }
  else
  {
    rabCounters->pktsReceivedFromPdcpInUlInActvInterval = 0;
    return FALSE;
  }
}

PRIVATE CONSTANT UdhUehInactiveUesIndFun UdhUehInactiveUesIndFunTab[] =
{
  cmPkAndSendInactiveUesInd,          /* 0 - loosely coupled */
  cmPkAndSendInactiveUesInd,          /* 1 - tightly coupled, service user EU */
};

/*
 *  Fun :  udhUehInactiveUesInd
 *  Desc:  Inactivity Indication to UEH
 *  Ret :  ROK -      ok
 *  File:  udh_func.c
 */
PUBLIC S16 udhUehInactiveUesInd
(
  Pst                 *pst,              /* post structure */
  SpId                *spId,             /* service Provider id */
  UdhUehUeInactiveInd *udhUehInactiveUeInd
)
{
  (*UdhUehInactiveUesIndFunTab[pst->selector])(pst, spId, udhUehInactiveUeInd);
  RETVALUE(ROK);
}

/* Create and send UDH-UEH Inactive UE Indicator */
void createAndSendUdhUehUeInactiveInd(
  UdhUehUeInactiveInd *udhUehUeInactiveInd
  )
{
  CmSapCfg *uehSapCfg = getUehSapCfg();
  udhUehInactiveUesInd(&uehSapCfg->pst , &uehSapCfg->spId , udhUehUeInactiveInd);
  /* Pack and send to UEH */
}

PRIVATE CONSTANT UdhUehUlInactiveUesIndFun UdhUehUlInactiveUesIndFunTab[] =
{
  cmPkAndSendUlInactiveUesInd,          /* 0 - loosely coupled */
  cmPkAndSendUlInactiveUesInd,          /* 1 - tightly coupled, service user EU */
};

PUBLIC S16 udhUehUlInactiveUesInd
(
  Pst                 *pst,              /* post structure */
  SpId                *spId,             /* service Provider id */
  UdhUehUlInactiveInd *udhUehUlInactiveUeInd
)
{
  (*UdhUehUlInactiveUesIndFunTab[pst->selector])(pst, spId, udhUehUlInactiveUeInd);
  RETVALUE(ROK);
}

/* Create and send UDH-UEH Inactive UL Indicator */
void createAndSendUdhUehUlInactiveInd(
  UdhUehUlInactiveInd *udhUehUlInactiveInd
  )
{
  CmSapCfg *uehSapCfg = getUehSapCfg();
  udhUehUlInactiveUesInd(&uehSapCfg->pst , &uehSapCfg->spId , udhUehUlInactiveInd);
  /* Pack and send to UEH */
}
/** TIMEOUT/ Throughput Timer **/
PUBLIC S16  udhHandleThroughputTimeout(void)
{
  UdhCb      *udhCb = getUdhCb();

  logFunc("%s","udhThroughputTimeout");

#if UDH_RAB_THROUGHPUT
  U32        ueIdx = 0, rabId = 0 , inactiveRabs = 0;
  UdhUeCb    *ueCb  = NULLP;
  UdhRabCb   *rabCb = NULLP;
  CmLteRnti  ueId = 0;

  for(ueIdx= 0; ueIdx < udhCb->maxConnectedUes; ueIdx++)
  {
    ueCb = udhCb->ueInfoPtrs[ueIdx];

    if(NULL != ueCb)
    {
      for(rabId = 0; rabId < UEH_MAX_RABS_IN_UE; rabId++ )
      {
        rabCb = &ueCb->rabCbTable[rabId];
        if(UDH_RAB_ACTIVE == rabCb->isActive)
        {
          /* clear the counters for next interval*/
          rabCb->rabCounters.bytesReceivedFromGtpInCurrentInterval = 0;
          rabCb->rabCounters.bytesReceivedFromPdcpInCurrentInterval = 0  ;
        }
      }
    }
  }
#endif
  
  udhCb->udhDatThrput.dlDataCntPrvIntvl  = udhCb->udhDatThrput.dlDataCntCrntIntvl;
  udhCb->udhDatThrput.ulDataCntPrvIntvl  = udhCb->udhDatThrput.ulDataCntCrntIntvl;
  udhCb->udhDatThrput.dlDataCntCrntIntvl = 0;
  udhCb->udhDatThrput.ulDataCntCrntIntvl = 0;

  /* No need to start the timer again , It will repeat*/
  RETVALUE(ROK);
}
/** TIMEOUT/Inactivity Check **/
PUBLIC S16  udhHandleInactivityTimeout(void)
{
  UdhCb                *udhCb = getUdhCb();
  U32                  ueIdx = 0, rabId = 0 , inactiveRabs = 0;
  UdhUeCb              *ueCb  = NULLP;
  UdhRabCb             *rabCb = NULLP;
  CmLteRnti            ueId = 0;
  UdhUehUeInactiveInd  udhUehUeInactiveInd;
  U16                  totalUes = 0;

  udhCb->numUeInactivityTimeouts++;

  clearBytes(&udhUehUeInactiveInd, sizeof(UdhUehUeInactiveInd));
  logInfo("udhHandleInactivityTimeout %d", udhCb->numUeInactivityTimeouts);

  for(ueIdx= 0; ueIdx < UDH_MAX_UE_POOL; ueIdx++)
  {
    ueCb = udhCb->ueInfoPtrs[ueIdx];
    if(NULLP == ueCb) continue;
    else totalUes++;

    inactiveRabs = 0;
    for(rabId = 0; rabId < UEH_MAX_RABS_IN_UE; rabId++)
    {
      rabCb = &ueCb->rabCbTable[rabId];
      if(FALSE == rabCb->isActive) continue; /* not allocated */

      if(FALSE == rabCb->isMontiored)
      {
        /* Don't check for inactivity this time, but check from next time onwards */
        rabCb->isMontiored = TRUE;
        rabCb->secsSinceInactivityCheck = 0;
        logState("ueId=%d|ueIdx=%d|rabId=%d|event=InactivityMonitorStarted", rabCb->pdcpId.ueId, ueIdx, rabCb->rabId);
        continue;
      }
      /* Handling of active rabs */
      rabCb->secsSinceInactivityCheck++;
      if(rabCb->secsSinceInactivityCheck < udhCb->ueInactivityCheckIntervalSecs) continue;
      else 
      {
        /* check inactivity */
        if (TRUE == isRabInactive(&rabCb->rabCounters))
        {
          logState("ueId=%d|rabId=%d|event=RabInactive", rabCb->pdcpId.ueId, rabCb->rabId);
          inactiveRabs++;
          ueId = rabCb->pdcpId.ueId;
        }
        else 
          logInfo("ueId=%d|rabId=%d|event=RabActive|totalUes=%d|totalRabs=%d", rabCb->pdcpId.ueId, rabCb->rabId, udhCb->numUesConnected, udhCb->totalActvRabs);

        if (inactiveRabs == ueCb->numRabs)
        {
          logState("ueId=%d|event=UeInactive", ueId);
          udhUehUeInactiveInd.ueId[udhUehUeInactiveInd.noofInactiveUes] = ueId;
          udhUehUeInactiveInd.noofInactiveUes++;
        }

        rabCb->secsSinceInactivityCheck = 0; /* reset since inactivy has bene checked */
      }
    }
  }

  /* If UE is found as inactive then send message to UEH  */
  if(udhUehUeInactiveInd.noofInactiveUes > 0)
  {
    createAndSendUdhUehUeInactiveInd(&udhUehUeInactiveInd);
  }
  //logInfo("totalUes=%d|inactiveUes=%d", totalUes, udhUehUeInactiveInd.noofInactiveUes);
  /* No need to start the timer again , It will repeat*/
  RETVALUE(ROK);
}

/*
 *  Fun :  udhHandleUlInactivityTimeout
 *  Desc:  This function sends UL Inactivity Indication to UEH for all existing UEs
 *  Ret :  ROK -      ok
 *  File:  udh_func.c
 */
PUBLIC S16  udhHandleUlInactivityTimeout(void)
{
  UdhCb                *udhCb = getUdhCb();
  U32                  ueIdx = 0, rabId = 0 ;// ulInactiveRabs = 0;
  UdhUeCb              *ueCb  = NULLP;
  UdhRabCb             *rabCb = NULLP;
  CmLteRnti            ueId = 0;
  UdhUehUlInactiveInd  udhUehUlInactiveInd;

  clearBytes(&udhUehUlInactiveInd, sizeof(UdhUehUlInactiveInd));
  logFunc("%s","udhHandleUlInactivityTimeout");

  for(ueIdx= 0; ueIdx < UDH_MAX_UE_POOL; ueIdx++)
  {
    ueCb = udhCb->ueInfoPtrs[ueIdx];
    if(NULLP == ueCb)
    {
      continue;
    }
    rabCb = &ueCb->rabCbTable[rabId];
    ueId  = rabCb->pdcpId.ueId;
    udhUehUlInactiveInd.ueId[udhUehUlInactiveInd.noofInactiveUes] = ueId;
    udhUehUlInactiveInd.noofInactiveUes++;
  }

  /* No need to send UL_INACTIVITY_IND if no UE is attached. */
  if(udhUehUlInactiveInd.noofInactiveUes > 0)
  {
    createAndSendUdhUehUlInactiveInd(&udhUehUlInactiveInd);
  }
  RETVALUE(ROK);
}

/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/


inline PUBLIC S16 sendUdhMsgToBsm(Event event, MsgLen len, U8 *data) { return sendEventToOam(ENTUDH, ENT_BSM, event, len, data); }
inline PUBLIC S16 sendUdhMsgToPrm(Event event, MsgLen len, U8 *data) { return sendEventToOam(ENTUDH, ENT_PRM, event, len, data); }
//inline PUBLIC void sendUdhHelloAckToPrm(void) { sendUdhMsgToPrm(EVT_HELLO_ACK, 0, NULL); }

void sendUdhStartIndToPrm(void)
{
  U32 version = getSvnVersion(MODID_UDH);
  logInfo ("UDH svn version : %d", version);
  /*send start indication to PRM indicating successful startup*/
  if( sendUdhMsgToPrm(EVT_START_IND, sizeof(U32), (U8*)&version ) == ROK)
  {
    logInfo("%s","sendUdhStartIndToPrm : Sent EVT_START_IND from UDH to PRM");
  }
}

void sendUdhHelloAckToPrm(
  Buffer   *mBuf
  )
{
  MsgLen              cCnt;
  MsgPrmModHello      hello;
  MsgModPrmHelloAck   helloAck;
  S16                 ret = ROK;

  clearBytes( &helloAck, sizeof(MsgModPrmHelloAck) );

  ret = SCpyMsgFix( mBuf, 0, sizeof(MsgPrmModHello), (Data*)&hello , &cCnt );
  if(ret != ROK) /* TODO: Need to send alarm because not able to decode cmdRef */
  {
    logError("%s", "SCpyMsgFix Failed to copy hello from PRM");
    /* NOTE: No return is used even fail happened in copying message. */
  }

  helloAck.helloAckSeqNum = hello.helloSeqNum;

  if( sendUdhMsgToPrm(EVT_HELLO_ACK, sizeof(MsgModPrmHelloAck), (U8*)&helloAck) == ROK)
  {
    logInfo("sendUdhHelloAckToPrm :Sent EVT_HELLO_ACK from UDH to PRM seqNo [%d]",helloAck.helloAckSeqNum);
  }
}

/**********************************************************************/

/*
 *  Fun :  udhInitUeCb
 *  Desc:  This function is used to initialise pointers used to store UE information(On OP-START).
 *  File:  udh_func.c
 */
PUBLIC S16 udhInitUeCb( void )
{
  UdhCb  *udhCb = getUdhCb();
  U16     ueIdx  = 0 , tnlIdx = 0 ;

  if( ROK != SGetSBuf(UDH_MEM_REGION, UDH_MEM_POOL, (Data **)(&(udhCb->ueInfoPtrs)), (UDH_MAX_UE_POOL * sizeof(UdhUeCb *))))
  {
    logError("%s"," Memory allocation failed for ueInfoPtrs ");
    RETVALUE(RFAILED);
  }

  for(ueIdx = 0; ueIdx < UDH_MAX_UE_POOL ; ueIdx++)
  {
    udhCb->ueInfoPtrs[ueIdx] = NULLP;
  }

  for(tnlIdx = 0; tnlIdx < UEH_MAX_TUNNEL_ID; tnlIdx++)
  {
    udhCb->lclTeId2RabCb[tnlIdx] = NULLP;
  }

  RETVALUE(ROK);
}

/*
 *  Fun :  udhDeleteAllContexts
 *  Desc:  This function is used to Clear Array of pointers on OP-STOP from BSM.
 *  File:  udh_func.c
 */
PUBLIC S16 udhDeleteAllContexts( void )
{
  U16       ueIdx  = 0 , tnlIdx = 0 ;
  UdhCb     *udhCb = getUdhCb();

  if(NULL != udhCb->ueInfoPtrs)
  {
    for(ueIdx = 0; ueIdx < UDH_MAX_UE_POOL ; ueIdx++)
    {
      UdhUeCb *udhUeCb = (udhCb->ueInfoPtrs[ueIdx]);
      if(NULL != udhUeCb)
      {
        UEH_FREE(udhUeCb, sizeof(UdhUeCb));
        udhCb->ueInfoPtrs[ueIdx] = NULLP;
      }
    }
  }

  for(tnlIdx = 0; tnlIdx < UEH_MAX_TUNNEL_ID; tnlIdx++)
  {
    udhCb->lclTeId2RabCb[tnlIdx] = NULLP;
  }
  RETVALUE(ROK);
}

/*
 *  Fun :  udhProcDstatReq
 *  Desc:  This function handles the DstatReq msg from PMH for sending Debug counters to it
 *  File:  udh_func.c
 */
void udhProcDstatReq (
  Buffer   *mBuf
)
{
  MsgLen                 cCnt;
  S16                    ret = ROK;
  MsgPmhUdhDStatsCfgReq  udhDStatReq;

  clearBytes( &udhDStatReq, sizeof( MsgPmhUdhDStatsCfgReq) );
  ret = SCpyMsgFix( mBuf, 0, sizeof(MsgPmhUdhDStatsCfgReq), (Data*)&udhDStatReq, &cCnt );
  if(ret != ROK) /* TODO: Need to send alarm because not able to decode cmdRef */
  {
    logError("%s", "SCpyMsgFix Failed to copy hello from PRM");
    /* NOTE: No return is used even fail happened in copying message. */
  }
  /* Start a timer, at the time of expiry it has to report Debug Counters to PMH */
  if( udhDStatReq.cmd == STCMD_START )
  {
    if( udhDStatReq.interval_ms > 0 )
    {
      g_gtpu_prio0_q = NULL;
      g_udh_prio0_q = NULL;
      g_tucl_prio0_q = NULL;
      /* UDH expects the timer values to be in seconds */
      logInfo("udhHandlePmhEvents: Debug Statistics Timer started with interval = %d", udhDStatReq.interval_ms);
      SRegSecResTmr( ENTUDH, 0, udhDStatReq.interval_ms/1000, udhReportDebugStats);
    }
  }
  else if( udhDStatReq.cmd == STCMD_STOP )
  {
    SDeregSecResTmr( ENTUDH, 0, udhDStatReq.interval_ms/1000, udhReportDebugStats );
    g_gtpu_prio0_q = NULL;
    g_udh_prio0_q = NULL;
    g_tucl_prio0_q = NULL;
  }
}

/*
 *  Fun :  udhReportDebugStats
 *  Desc:  This function is call back for reporting Debug statistics
 *  File:  udh_func.c
 */
S16 udhReportDebugStats(void)
{
  MsgUdhPmhDStatsInd   udhPmhDstats;
  logFunc( "%s", __func__);

  clearBytes( &udhPmhDstats, sizeof(MsgUdhPmhDStatsInd) );

  udhPmhDstats.n = 3;
  udhPmhDstats.counterList[0].id  = CTRID_GTPU_QUEUE_LEN;
  udhPmhDstats.counterList[1].id  = CTRID_UDH_QUEUE_LEN;
  udhPmhDstats.counterList[2].id  = CTRID_TUCL_QUEUE_LEN;
  if( NULL != g_gtpu_prio0_q )
  {
    udhPmhDstats.counterList[0].val = g_gtpu_prio0_q->crntSize;
  }

  if( NULL != g_udh_prio0_q )
  {
    udhPmhDstats.counterList[1].val = g_udh_prio0_q->crntSize;
  }

  if( NULL != g_tucl_prio0_q )
  {
    udhPmhDstats.counterList[2].val = g_tucl_prio0_q->crntSize;//osCp.tTskTbl[dstIdx].sTsk->dQ.queue[0].crntSize;
  }
  if(( NULL != g_gtpu_prio0_q ) && ( NULL != g_udh_prio0_q ) && ( NULL != g_tucl_prio0_q ))
  {
    logInfo( "%s", "Sending EVT_UDH_PMH_DSTAT_RSP" );
    sendEventToOam (ENTUDH, ENT_PMH, EVT_UDH_PMH_DSTAT_RSP, sizeof( MsgUdhPmhDStatsInd ), (UINT8 *)&udhPmhDstats);
  }
  RETVALUE(ROK);
}
