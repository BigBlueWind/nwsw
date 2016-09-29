/**
 * @file    s1ap_mgr.c
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @details
 * This file contains S1AP Manager functions, all external interfaces towards S1M terminate
 * in this file.
 *
 * @author Lakshmi Narayana Madala
 *
 */

#include "ccpu_cmn.h"

/* RRM related includes */
#include "lueh.h"
#include "ueh.h"
#include "lueh.x"
#include "ueh_eut_x.h"
#include "s1ap.h"
#include "s1ap_cm.h"
#include "s1ap_wrap.h"
#include "s1ap_unwrap.h"
#include "s1ap_cmn_proc.h"
#include "s1ap_mgr.h"
#include "s1ap_sm.h"
#include "s1ap.h"
#include "bs_err.h"
#include "ueh_crm_sm.h"
#include "pst_events.h"

#include "log.h"

//extern  SmCfgCb smCfgCb;

/* ***************** Global Variable *********************** */
S1apMgrCb  enbS1MgrCb;

S1apMgrCb *getS1MgrCb(
    UINT32 peerId
    )
{
  return &enbS1MgrCb;
}

/**
 * @param    event     event in pst structure
 * @param    len       Length of data to be sent
 * @param    data      Data to be sent
 * @details
 * This function send the data from S1 Manager to BSM.
 * returns ROK on success, and RFAILED if it is unable to send the message to BSM
 */
PUBLIC S16 sendS1mMsgToBsm(
    IN Event   event,
    IN MsgLen    len,
    IO U8     *data
    )
{
  Pst   pst;
  Buffer *mBuf = NULLP;

  if (SGetMsg(DFLT_REGION, DFLT_POOL, &mBuf) != ROK)
  {
    logError("%s", "Error in Getting Message buffer");
  }
  cmMemset((U8*)&pst, '\0', sizeof(Pst));

  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTUEH; /* Change this to S1M, when S1M registers with SSI */
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);
  pst.dstEnt    = ENT_BSM;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);
  pst.event     = event;

  /* Add data to mBuf here.
   */
  if( (len > 0) && (NULL != data) )
  {
    SAddPstMsgMult((Data *)data, len, mBuf);
  }

  if(SPstTsk(&pst, mBuf) !=ROK)
  {
    logError("Error in sending event:%d from S1M to BSM", pst.event );
    return RFAILED;
  }
  else
  {
    logInfo("Sending event:%d from S1M to BSM", pst.event );
    return ROK;
  }
}

/**
 * @param    status    S1 Link Status
 * @details
 * This function send the link status(EVT_S1M_LINK_UP_IND/EVT_S1M_LINK_DOWN_IND) to BSM
 */
void s1apSendLinkStatusToUeh(
    IN TBOOL status
    )
{
  Buffer *mBuf = NULLP;
  Pst    pst;

  if (SGetMsg(DFLT_REGION, DFLT_POOL, &mBuf) != ROK)
  {
    return;
  }

  cmMemset( (U8*)&pst, '\0', sizeof(Pst));

  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcProcId = SM_SM_PROC;
  pst.srcEnt    = ENTUEH;
  pst.srcInst   = 0;
  pst.dstEnt    = ENTUEH;
  pst.dstProcId = SM_SM_PROC;

  if( S1_LINK_UP == status )
  {
    pst.event     = EVT_S1M_LINK_UP_IND;
  }
  else
  {
    pst.event     = EVT_S1M_LINK_DOWN_IND;
  }

  if(SPstTsk(&pst, mBuf) !=ROK)
  {
    logError("UEH -S1AP Send Failure-(proc(%d),ent(%d),inst(%d))", pst.dstProcId, pst.dstEnt, pst.dstInst);
  }
  else
  {
    logInfo("UEH -S1AP Send success-(proc(%d),ent(%d),inst(%d))", pst.dstProcId, pst.dstEnt, pst.dstInst);
  }
}

/**
 * @param    s1LinkStatus    S1 Link Status message
 * @param    s1Cb            S1 Call Block
 * @details
 * This function fills the MME IP address from s1Cb in the linkstatus message
 */
void s1apGetMmeIpAddr(
    IO MsgS1apBsmS1LinkStatus      *s1LinkStatus,
    IN S1apMgrCb                   *s1Cb
    )
{
  copyBytes( s1Cb->mmeIpAddr, (MAX_SIZE_MMECONFIG_MMEIPADDR + 1), &(s1LinkStatus->mmeIpAddr) );
}

/**
 * @param    linkStatus      Link Status message
 * @param    s1Cb            S1 Call Block
 * @details
 * When we receive Link UP, send this to BSM Immediately
 * When Link Down is received, If Link is locally closed Don't Report this DOWN to  bsm
 * If Link down is reported already then also Don't Report this twice.
 */
void s1apLinkStatusToBsm(
     IN UINT16        linkStatus,
     IO S1apMgrCb     *s1Cb
  )
{
  MsgS1apBsmS1LinkStatus s1LinkStatus;

  logInfo("S1 Link status is:%d", linkStatus);

  /*
   */
  s1apGetMmeIpAddr( &s1LinkStatus, s1Cb );
  if( S1_LINK_UP == linkStatus )
  {
    s1Cb->isS1LinkDownReported = FALSE;
    sendS1mMsgToBsm( EVT_S1M_LINK_UP_IND, sizeof(MsgS1apBsmS1LinkStatus), (U8*)&s1LinkStatus );
  }
  else
  {
    /* CCPU SCTP Stack sends DOWN Indication multiple TIMES,
     * If sent once then ignore other indications
     */
    if( TRUE == s1Cb->isS1LinkClosed )
      return;

    if( TRUE == s1Cb->isS1LinkDownReported)
      return;
    sendS1mMsgToBsm( EVT_S1M_LINK_DOWN_IND, sizeof(MsgS1apBsmS1LinkStatus), (U8*)&s1LinkStatus );
    s1Cb->isS1LinkDownReported = TRUE;
  }
}

/**
 * @param    mbuf       Received Message S1M_CONFIG from BSM
 * @details
 * Get the data from mBuf into a flat buffer
 * S1AP config Request, Save the configuration
 * Save the enodeB and MME IP Address received
 */
void s1mProcS1mCfgReq(
    Buffer *mBuf
    )
{
  MsgLen                cCnt;
  MsgBsmS1apConfigReq   s1apConfigReq;
  MsgS1apBsmConfigSucc  s1apCfgSucc;

  SCpyMsgFix( mBuf, 0, sizeof(MsgBsmS1apConfigReq), (Data*)&s1apConfigReq , &cCnt );

  /* Save the configuration */
  smCb.cfgCb.s1apCfgInfo = s1apConfigReq;
  cmInetAddr( (S8*)(s1apConfigReq.mmeConfig.mmeIpAddr), &(smCb.cfgCb.mmeIpAddr));
  smCb.cfgCb.mmeIpAddr = CM_INET_HTON_U32 (smCb.cfgCb.mmeIpAddr);

  s1apCfgSucc.transId = s1apConfigReq.transId;
  sendS1mMsgToBsm(EVT_S1M_BSM_CFG_SUCC, sizeof(s1apCfgSucc), (U8*)&s1apCfgSucc);
}

/**
 * @param    errorNum        BS Error number
 * @param    failReason      S1 Setup failure reason
 * @param    s1Cb            S1 Call Block
 * @details
 * S1 Setup procedure failed, Send OpStart fail to BSM,
 * Close the SCTP connection established towards MME
 */
void s1apSendOpstartFailClearSctp(
    IN BsErrorValue errorNum,
    IN SINT8        *failReason,
    IO S1apMgrCb    *s1Cb
    )
{
  OpStartFail opStartFail;
  clearBytes( &opStartFail, sizeof(OpStartFail) );

  opStartFail.errorNum = errorNum;
  /* warning ignored: conversion from unsigned to signed conversion
  copyBytes( failReason, strlen(failReason),  opStartFail.failReason); */
  snprintf((S8*)(opStartFail.failReason), sizeof(opStartFail.failReason), "%s", failReason );

  sendS1mMsgToBsm( EVT_S1M_BSM_OPSTART_FAIL, sizeof(OpStartFail), (U8*)&opStartFail );

}

/* When we receive Link UP, send this to BSM Immediately
 * When Link Down is received, If Link is locally closed Don't Report this DOWN to  bsm
 * If Link down is reported already then also Don't Report this twice.
 */
/* ***************** Delete the Peer ******************************* */
/**
 * @param    peerId     Peer ID configured in S1AP stack
 * @param    s1Cb       S1 Manager Call Block
 * @details
 * This function send the peer delete request to S1AP, this ensures that SCTP connection
 * is terminated by S1AP stack
 * Send Link Closed Indication to BSM
 */
S16 s1apBuildSzPeerDel(
    IN U32           peerId,
    IO S1apMgrCb    *s1Cb
  )
{
  SzMngmt cntrl;
  Pst      pst;
  MsgS1apBsmLinkClosedInd s1Linkclosed;

  SM_SET_ZERO(&cntrl, sizeof(SzMngmt));
  SM_SET_ZERO(&pst, sizeof(Pst));

  cntrl.u.cntrl.action        =  ADEL;
  cntrl.u.cntrl.subAction        =  SAELMNT;
  cntrl.u.cntrl.u.peer.nmbPeers = 1;
  UEHSM_ALLOC( &(cntrl.u.cntrl.u.peer.peerId), sizeof(TknU32) );
  cntrl.u.cntrl.u.peer.peerId->pres = TRUE;
  cntrl.u.cntrl.u.peer.peerId->val = peerId;

  /* Fill Header */
  cntrl.hdr.msgType             = TCNTRL;
  cntrl.hdr.entId.ent           = ENTSZ;
  cntrl.hdr.entId.inst          = SM_INST_ZERO;
  cntrl.hdr.elmId.elmnt         = STSZPEER;
  cntrl.hdr.transId             = SM_S1AP_PEERDEL_TRANS_ID;

  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTSZ;
  pst.dstProcId = SM_SZ_PROC;
  pst.srcProcId = SM_SM_PROC;

  logProc("Start|S1AP-PEER-Del|transId=%d", cntrl.hdr.transId);

  /* Send the request to the S1AP */
  SmMiLszCntrlReq(&pst, &cntrl);

  logInfo("%s","Sending S1 Linkclosed indication");
  s1apGetMmeIpAddr( &s1Linkclosed, s1Cb );
  sendS1mMsgToBsm( EVT_S1M_LINK_CLOSED_IND, sizeof(MsgS1apBsmLinkClosedInd), (U8*)&s1Linkclosed );
  s1Cb->isS1LinkClosed = TRUE;

  RETVALUE(ROK);
}

/**
 * @param    s1apMgrCb       S1 Manager Call block
 * @param    s1apCfgInfo     Configuration from OAM
 * @details
 * This function is initialises the S1 Manager, using the OAM values which are saved when
 * configuration request is received from BSM
 */
void s1apInitS1MgrCb(
    IO S1apMgrCb       *s1apMgrCb,
    IN MsgBsmS1apConfigReq     *s1apCfgInfo
    )
{
  UINT16 tac;
  UINT32 enbId;

  s1apMgrCb->procGuardTimerDuration     = ( smCb.cfgCb.s1apConfig.s1apGuardTimer * TMR_UINTS_PER_SEC );
  s1apMgrCb->maxS1SetupAllowed          =  smCb.cfgCb.s1apConfig.maxS1SetupAllowed;
  s1apMgrCb->pagingDrx                  = s1apCfgInfo->defaultPagCycle;

  /* We support only Macro EnodeB Id*/
  enbId = s1apCfgInfo->enodebId;
  s1apMgrCb->globalEnodeBId.enodeBType = EnbType_MacroEnodeB;
  s1apFillEnbIdFromUint32( EnbType_MacroEnodeB,
                           s1apMgrCb->globalEnodeBId.enbId,
                           enbId);

  s1apGetPlmnIdFromMccMnc( s1apMgrCb->globalEnodeBId.plmnIdentity,
                           s1apCfgInfo->plmnMCC,
                           s1apCfgInfo->plmnMNC);
  s1apMgrCb->numOfSupportedTac  = 1;
  tac = s1apCfgInfo->tac;
  s1apGetTacFromUint16( s1apMgrCb->tacs[0].tac,
                        s1apCfgInfo->tac );
  s1apMgrCb->tacs[0].servedBroadcastPlmns.numPlmns=1;
  s1apGetPlmnIdFromMccMnc( s1apMgrCb->tacs[0].servedBroadcastPlmns.plmnId[0],
                           s1apCfgInfo->plmnMCC,
                           s1apCfgInfo->plmnMNC);

  /* warning ignored: conversion from unsigned to signed */
  strncpy( (S8*)s1apMgrCb->enbName, (const S8 *)s1apCfgInfo->enodebName, MAX_SIZE_ENBNAME  );

  /* When adding multiple MME support use mmeID index sent by BSM as the PeerID
   * */
  s1apMgrCb->peerId = s1apCfgInfo->mmeConfig.index;
  copyBytes( s1apCfgInfo->mmeConfig.mmeIpAddr, MAX_SIZE_MMECONFIG_MMEIPADDR, s1apMgrCb->mmeIpAddr );

  cmInitTimers(&(s1apMgrCb->setupTimer.cmTimer) , 1);
  cmInitTimers(&(s1apMgrCb->guardTimer.cmTimer) , 1);
  s1apMgrCb->isS1LinkDownReported = FALSE;
  s1apMgrCb->isS1LinkClosed = FALSE;
  s1apMgrCb->overloadState = OverloadAction_PermitAllCalls;
  s1apSetState( s1apMgrCb, S1_STATE_INIT );
}

/**
 * @param    mbuf       Received Message OPStart from BSM
 * @details
 * On receiving opstart request Get S1Cb
 * Invoke the state machine with event S1_MSG_OPSTART
 */
void s1apProcOpStartReq(
    IN Buffer *mbuf
    )
{
  S1apMgrCb     *s1Cb =  getS1MgrCb( UEHSM_LSZ_MME_PRC1 ); /* TODO Add support for multiple MME */

  /*Configure the S1AP Stack regarding the PEER */
  smBuildSzPeerCfg();

  /* We are starting for the first time so Initialize the S1 Structures */
  clearBytes( s1Cb, sizeof(S1apMgrCb) );
  s1apInitS1MgrCb( s1Cb, &(smCb.cfgCb.s1apCfgInfo));

  s1apFsm( s1Cb, S1_MSG_OPSTART, NULL);
}

/**
 * @details
 * On receiving opstop request S1AP Manager, sends the Reset request to MME
 * currently We have only one peer, get S1 Call block for that peer and
 * invoke the SM with event S1_MSG_SND_RESET
 */
void s1apSndResetToPeerOamInt( void )
{
  S1apMgrCb     *s1Cb =  getS1MgrCb( UEHSM_LSZ_MME_PRC1 ); /* TODO Add support for multiple MME */

  /* Request S1 Manager to Send Global Reset */
  s1apFsm( s1Cb, S1_MSG_SND_RESET, NULL);
}

/**
 * @param    s1Cb       S1 Call Block
 * @details  On reception of timer expiry
 * We invoke the state Machine with the event S1_MSG_SETUP_TMR_EXPIRY,
 * To retry setup procedure
 */
void s1apProcS1setupTmrExpiry(
    IO S1apMgrCb     *s1Cb
    )
{
  s1apFsm( s1Cb, S1_MSG_SETUP_TMR_EXPIRY, NULL);
}

/**
 * @param    s1Cb       S1 Call Block
 * @details
 * On reception of Guard timer expiry
 * We invoke the state Machine with the event S1_MSG_PROC_GUARD_TMR_EXPIRY
 */
void s1apProcS1GuardTmrExpiry(
    IO S1apMgrCb     *s1Cb
    )
{
  s1apFsm( s1Cb, S1_MSG_PROC_GUARD_TMR_EXPIRY, NULL);
}

/**
 * @param    mbuf       Received Message OpStop from BSM
 * @details
 * On receiving opstop request S1AP Manager, sends the Reset request to MME
 * After Reset Response is received from MME then Reset Procedure is considered complete
 * currently We have only one peer,
 */
void s1apProcOpStopReq(
    IN Buffer *mbuf
    )
{
  S1apMgrCb     *s1Cb =  getS1MgrCb( UEHSM_LSZ_MME_PRC1 ); /* TODO Add support for multiple MME */

  s1apFsm( s1Cb, S1_MSG_OPSTOP, NULL);
}

/**
 * @param   uDatEvnt    Setup Response Message
 * @details
 * This function is invoked when there is a Setup Response message from S1AP
 * stack. Extract the S1 Call block pertaining to the peer Id, and invoke the state machine
 */
void s1apProcS1SetupRsp(
    IN SztUDatEvnt *uDatEvnt
    )
{
  S1apMgrCb     *s1Cb =  getS1MgrCb( uDatEvnt->peerId.val );

  s1apFsm( s1Cb, S1_MSG_ID_SETUP_RSP, uDatEvnt);
}

/**
 * @param   uDatEvnt    Setup Fail Message
 * @details
 * This function is invoked when there is a Setup Fail message from S1AP
 * stack. Extract the S1 Call block pertaining to the peer Id, and invoke the state machine
 */
void  s1apProcS1SetupFail(
    IN SztUDatEvnt *uDatEvnt
    )
{
  S1apMgrCb     *s1Cb =  getS1MgrCb( uDatEvnt->peerId.val );

  s1apFsm( s1Cb, S1_MSG_ID_SETUP_FAIL, uDatEvnt );
}

/**
 * @param   uDatEvnt    Reset Ack Message
 * @details
 * This function is invoked when there is a Reset Ack message from S1AP
 * stack. Extract the S1 Call block pertaining to the peer Id, and invoke the state machine
 */
void  s1apProcS1ResetAck(
    IN SztUDatEvnt *uDatEvnt
    )
{
  S1apMgrCb     *s1Cb =  getS1MgrCb(uDatEvnt->peerId.val);

  s1apFsm( s1Cb, S1_MSG_ID_RESET_ACK, uDatEvnt);
}

/**
 * @param   uDatEvnt    Reset Message
 * @details
 * This function is invoked when there is a Reset message from S1AP
 * stack. Extract the S1 Call block pertaining to the peer Id, and invoke the state machine
 */
void  s1apProcS1Reset(
    IN SztUDatEvnt *uDatEvnt
    )
{
  S1apMgrCb             *s1Cb =  getS1MgrCb( uDatEvnt->peerId.val );

  s1apFsm( s1Cb, S1_MSG_ID_RESET, uDatEvnt);
}

/**
 * @param   uDatEvnt    MME Config Update
 * @details
 * This function is invoked when there is a MME Configuration Update message from S1AP
 * stack. Extract the S1 Call block pertaining to the peer Id, and invoke the state machine
 */
void  s1apProcMmeCfgUpdate(
    IN SztUDatEvnt *uDatEvnt
    )
{
  S1apMgrCb             *s1Cb =  getS1MgrCb( uDatEvnt->peerId.val );

  s1apFsm( s1Cb, S1_MSG_MME_CFG_UPDATE, uDatEvnt);
}

/**
 * @param   uDatEvnt    S1 Overload Start event from MME
 * @details
 * This function is invoked when there is a OverLoad Start message from S1AP
 * stack. Extract the S1 Call block pertaining to the peer Id, and invoke the state machine
 */
void  s1apProcOverloadStart(
    IN SztUDatEvnt *uDatEvnt
    )
{
  S1apMgrCb             *s1Cb =  getS1MgrCb( uDatEvnt->peerId.val );

  s1apFsm( s1Cb, S1_MSG_OVERLOAD_START, uDatEvnt);
}

/**
 * @param   uDatEvnt    S1 Overload Stop event from MME
 * @details
 * This function is invoked when there is a OverLoad Stop message from S1AP
 * stack. Extract the S1 Call block pertaining to the peer Id, and invoke the state machine
 */
void  s1apProcOverloadStop(
    IN SztUDatEvnt *uDatEvnt
    )
{
  S1apMgrCb             *s1Cb =  getS1MgrCb( uDatEvnt->peerId.val );

  s1apFsm( s1Cb, S1_MSG_OVERLOAD_STOP, uDatEvnt  );
}

/**
 * @param   uDatEvnt    S1 Paging request from MME
 * @details
 * This function is invoked when there is a Paging message from S1AP
 * stack. Extract the S1 Call block pertaining to the peer Id, and invoke the state machine
 */
void  s1apProcPagingMsg(
    IN SztUDatEvnt *uDatEvnt
    )
{
  S1apMgrCb             *s1Cb =  getS1MgrCb( uDatEvnt->peerId.val );

  s1apFsm( s1Cb, S1_MSG_ID_PAGING, uDatEvnt  );
}

/**
 * @param   post        pst structure
 * @param   suId        Service User ID
 * @param   sztSta      SCTP connection status
 * @details
 * This function is invoked when there is a S1 Connection status message from S1AP
 * stack. We support LinkDown and LinkUp status. Extract the S1 Call block pertaining
 * to the peer Id, and invoke the state machine
 */
S16 s1apProcStatInd(
    IN Pst            *post,
    IN SuId           suId,
    IN SztStaInd      *sztSta
  )
{
  S1apMgrCb     *s1Cb =  getS1MgrCb( sztSta->peerId.val );

  logInfo("Received SCTP connection Indication Status:%d", sztSta->status );

  /* Received status of SCTP connection */
  if( SZT_STA_TYPE_ASSOC == sztSta->type )
  {
    if( SZT_STATUS_ASSOC_DOWN == sztSta->status)
    {
      s1apFsm( s1Cb, S1_MSG_LINK_DOWN, NULL  );
    }
    else if( SZT_STATUS_ASSOC_UP == sztSta->status )
    {
      s1apFsm( s1Cb, S1_MSG_LINK_UP, NULL  );
    }
  }

  return ROK;
}

/**
 * @param   uDatEvnt    S1 Write-Replace Warning Request MME
 * @details
 * This function is invoked when there is a  Write-Replace Warning Request from S1AP
 * stack. Extract the S1 Call block pertaining to the peer Id, and invoke the state machine
 */
void  s1apProcWarningReq(
    IN SztUDatEvnt *uDatEvnt
    )
{
  S1apMgrCb   *s1Cb =  getS1MgrCb( uDatEvnt->peerId.val );

  s1apFsm( s1Cb, S1_MSG_WARNING_REQ, uDatEvnt  );
}


/**
 * @param   pst         pst structure
 * @param   suId        Service User ID
 * @param   uDatEvnt    S1 PDU received from Peer
 * @details
 * All the common procedure data(NON-UE) will land in this function
 * Handle the received message based on the message type
 * The S1 PDU is freed at the end of this function after processing the received event
 */
PUBLIC S16 UehLiSztUDatInd(
    Pst         *pst,
    SuId         suId,
    SztUDatEvnt *uDatEvnt
  )
{
   U8          evnt;

   uehSztUtilsGetMsgIdx ( &evnt, uDatEvnt->pdu);

   logInfo("UehLiSztUDatInd: SZT UData Ind event:%d", evnt);

   /* Decode the received message,
    * And call respective handlers
    */
   switch(evnt)
   {
     case SZT_MSG_ID_SETUP_RSP:
       s1apProcS1SetupRsp( uDatEvnt );
       break;
     case SZT_MSG_ID_SETUP_FAIL:
       s1apProcS1SetupFail( uDatEvnt );
       break;
     case SZT_MSG_ID_RESET_ACK:
       s1apProcS1ResetAck( uDatEvnt );
       break;
     case SZT_MSG_ID_RESET:
       s1apProcS1Reset( uDatEvnt );
       break;
     case SZT_MSG_ID_PAGING:
       s1apProcPagingMsg( uDatEvnt );
       break;
     case SZT_MSG_ID_MME_CFG_UPD:
       s1apProcMmeCfgUpdate( uDatEvnt );
       break;
     case SZT_MSG_ID_OVLD_START:
       s1apProcOverloadStart( uDatEvnt );
       break;
     case SZT_MSG_ID_OVLD_STOP:
       s1apProcOverloadStop( uDatEvnt );
       break;
     case SZT_MSG_ID_WARNING_REQ:
       s1apProcWarningReq( uDatEvnt );
       break;
     case SZT_MSG_ID_ERR_IND:
       logError("SZT_MSG_ID_ERR_IND received | Not yet handled", evnt );
       break;
     default:
       logError("Event=%d not yet handled in S1AP Manager",evnt );
   }
   RETVALUE(ROK);
}
