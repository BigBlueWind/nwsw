/**
 * @file     sm_lthsm_exms.c
 * @details  Copyright Powerwave Technologies, Inc., 2010
 * @brief    This file contains Handlers for Messages coming from LTH to SM
 * @author   Muktheshwar Madishetty
 * @date     22-Oct-2012
 * @details  1. sm_ex_ms.c is divided into sm_bsmsm_exms.c sm _lthsm_exms.c and sm_pmhsm_exms.c @n
 */

#include "sm.h"
/**
 * @param     pst   post structure contains source and destinations information
 * @param     mBuf  Buffer contains message coming from BSM
 * @details   Entry point to the messages received from BSM to SM
 */
PUBLIC S16 lthSmActvTsk(
  Pst         *pst,
  Buffer      *mBuf
  )
{
   S16 ret;
   ret = ROK;
   switch(pst->event)
   {

      case EVT_LTH_SM_TRC_START_REQ:
      case EVT_LTH_SM_TRC_STOP_REQ:
      {
        smProcTrcStartOrStopReq(mBuf);   
        SPutMsg(mBuf);
      }break;

      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         logError("%s","Received invalid event\n");
         break;
   }
   RETVALUE(ret);
}
/**
 * @param     mBuf contains RRC trace request
 * @details   This function process RRC Trace request received from BSM
 */
void smProcTrcStartOrStopReq(
  Buffer *mBuf
  )
{
  MsgLen                      cCnt;
  MsgLthSmTraceCntrlReq     traceCntrlReq;
  SCpyMsgFix( mBuf, 0, sizeof(MsgLthSmTraceCntrlReq), (Data*)&traceCntrlReq , &cCnt );
  U8 trcFlag;
  
  trcFlag = smGetCcpuTrcFlagFromPwavFlag( traceCntrlReq.traceCtrlCmd );

           logError("in TrcstartorstopReq :%d", traceCntrlReq.protocolName);
  switch(traceCntrlReq.protocolName)
  {
      case TRACECONFIGMOD_RRC: {smProcRrcTrcReq(mBuf,trcFlag);}break;
      case TRACECONFIGMOD_S1AP: {smProcS1apTrcReq(mBuf,trcFlag);}break;
      case TRACECONFIGMOD_PDCP: {smProcPdcpTrcReq(mBuf,trcFlag);}break;
      case TRACECONFIGMOD_RLC: {smProcRlcTrcReq(mBuf,trcFlag);}break;
      default:
           logError("smProcTrcStartReq: Unsupported Protocol to trace :%d", traceCntrlReq.protocolName);
           break;
  }

}
//##############################################

/**
 * @param     trcStatus  Trace status indicates start trace or stop trace
 * @details   This function returns CCPU Trace Flags, Enable or Disable from PWAV Trace Flag TRUE/FALSE
 */
U32 smGetCcpuTrcFlagFromPwavFlag(
  TraceCommand     trcStatus
  )
{
  if(TRACECOMMAND_STARTTRACE == trcStatus )
  {
    return AENA;
  }
  else /* ( TRACECOMMAND_STOPTRACE == trcStatus ) */
  {
    return ADISIMM;
  }
}

/* ********************* TRACE REQUEST HAQNDLERS ***************************** */
/**
 * @param    rgCntrlReq MAC control req message structure
 * @param    element element of control request
 * @param    action  what action to take on MAC stack
 * @param    subaction what the sub action to take on MAC stack
 * @details  This function fills the MAC control request message structure
 */
void smFillMacTrcCntrlReq(
  RgMngmt     *rgCntrlReq,
  S16         element,
  S16         action,
  S16         subAction
  )
{
  rgCntrlReq->t.cntrl.action = action;
  rgCntrlReq->t.cntrl.subAction = subAction;

  if( ADISIMM == action )
  {
    rgCntrlReq->t.cntrl.s.trcLen = LRG_NO_TRACE;
  }
  else
  {
    rgCntrlReq->t.cntrl.s.trcLen = LRG_FULL_TRACE;
  }

  rgCntrlReq->t.cntrl.s.rgSapCntrl.spId = STRGUSAP;

  rgCntrlReq->hdr.elmId.elmnt = element;
  rgCntrlReq->hdr.response.mem.region = DFLT_REGION;
  rgCntrlReq->hdr.response.mem.pool = RG_POOL;
  rgCntrlReq->hdr.response.selector = SM_SELECTOR_LC;
  rgCntrlReq->hdr.response.prior = PRIOR0;
  rgCntrlReq->hdr.response.route = RTESPEC;

  rgCntrlReq->hdr.msgType = TCNTRL;
  rgCntrlReq->hdr.entId.ent = ENTRG;
  rgCntrlReq->hdr.entId.inst = SM_INST_ZERO;
}
/**
 * @param     mBuf  Buffer contains MAC Trace request
 * @details   This function process MAC Trace request received from BSM
 */
void smProcMacTrcReq(
  Buffer *mBuf
  )
{

  RgMngmt                     macCntrlReq;
  Pst                         pst;
  U8                          trcFlag = 0;


  cmMemset((U8*)&pst, '\0', sizeof(Pst));
  cmMemset((U8*)&macCntrlReq, '\0', sizeof(RgMngmt));

  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTRG;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);

  /* Fill the control Request */
  smFillMacTrcCntrlReq(&macCntrlReq, STGEN, trcFlag, SATRC);

  cmPkLrgCntrlReq( &pst, &macCntrlReq);
}

/**
 * @param   kwCntrlReq RLC control req message structure
 * @param   element element of control request
 * @param   action  what action to take on RLC stack
 * @param   subaction what the sub action to take on RLC stack
 * @details This function fills the RLC control request message structure
 */
void smFillRlcTrcCntrlReq(
  KwMngmt *kwCntrlReq,
  S16     element,
  S16     action,
  S16     subAction
  )
{
  kwCntrlReq->t.cntrl.action = action;
  kwCntrlReq->t.cntrl.subAction = subAction;
  kwCntrlReq->t.cntrl.s.trcCntrl.trcLen = MAX_SIZE_OF_TRC_MSG;
  /* TODO Study the usage of Trace Mask and update this in future */
  kwCntrlReq->t.cntrl.s.trcCntrl.trcMask = 1;

  kwCntrlReq->hdr.msgType = TCNTRL;
  kwCntrlReq->hdr.entId.ent = ENTKW;
  kwCntrlReq->hdr.entId.inst = SM_INST_ZERO;
  kwCntrlReq->hdr.elmId.elmnt = element;
  if( AENA == action )
  {
    kwCntrlReq->hdr.transId = SM_RLC_START_TRACE_TRANS_ID;
  }
  else
  {
    kwCntrlReq->hdr.transId = SM_RLC_STOP_TRACE_TRANS_ID;
  }
}
/**
 * @param     mBuf  Buffer contains RLC Trace request
 * @details   This function process RLC Trace request received from BSM
 */
void smProcRlcTrcReq(
  Buffer *mBuf,
  U8 trcFlag
  )
{


  Pst                         pst;
  KwMngmt                     rlcCntrlReq;

         logError("Trc Flag=%d", trcFlag); 

  /* Enable/Disable traces on RLC */
  cmMemset((U8*)&pst, '\0', sizeof(Pst));
  cmMemset((U8*)&rlcCntrlReq, '\0', sizeof(KwMngmt));

  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTKW;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);

  /* Fill the control Request */
  smFillRlcTrcCntrlReq(&rlcCntrlReq, STGEN, trcFlag, SATRC);

  cmPkLkwCntrlReq( &pst, &rlcCntrlReq);
}
/**
 * @param   egCntrlReq GTP-U control req message structure
 * @param   element element of control request
 * @param   action  what action to take on GTP stack
 * @param   subaction what the sub action to take on GTP stack
 * @details This function fills the GTP control request message structure
 */
void smFillEgtpTrcCntrlReq(
  EgMngmt *egCntrlReq,
  S16     element,
  S16     action,
  S16     subAction,
  SpId    tSapId
  )
{
  egCntrlReq->u.cntrl.action = action;
  egCntrlReq->u.cntrl.subAction = subAction;
  egCntrlReq->u.cntrl.s.trcCntrl.trcLen = MAX_SIZE_OF_TRC_MSG;
  /* TODO Study the usage of Trace Mask and update this in future */
  egCntrlReq->u.cntrl.s.trcCntrl.trcMask = (LEG_TRC_EVENT_TX | LEG_TRC_EVENT_RX);
  egCntrlReq->u.cntrl.s.trcCntrl.t.tSapId = tSapId;

  egCntrlReq->hdr.msgType = TCNTRL;
  egCntrlReq->hdr.entId.ent = ENTEG;
  egCntrlReq->hdr.entId.inst = SM_INST_ZERO;
  egCntrlReq->hdr.elmId.elmnt = element;
  if( AENA == action )
  {
    egCntrlReq->hdr.transId = SM_EGTP_START_TRACE_TRANS_ID;
  }
  else
  {
    egCntrlReq->hdr.transId = SM_EGTP_STOP_TRACE_TRANS_ID;
  }
}
/**
 * @param     mBuf  Buffer contains GTP Trace request
 * @details   This function process GTP Trace request received from BSM
 */
void sfiProcEgtpTrcReq(
  Buffer *mBuf
  )
{

  Pst                         pst;
  U8                          trcFlag = 0;
  EgMngmt                     egCntrlReq;


  /* Enable/Disable traces on RLC */
  cmMemset((U8*)&pst, '\0', sizeof(Pst));

  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTEG;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);

  /* Fill the control Request */
  cmMemset((U8*)&egCntrlReq, '\0', sizeof(EgMngmt));
  smFillEgtpTrcCntrlReq(&egCntrlReq, STEGGEN, trcFlag, SATRC, UDH_EG_SPID);
  cmPkLegCntrlReq( &pst, &egCntrlReq);

  /* Fill the control Request */
  cmMemset((U8*)&egCntrlReq, '\0', sizeof(EgMngmt));
  smFillEgtpTrcCntrlReq(&egCntrlReq, STEGGEN, trcFlag, SATRC, EG_HI_SPID);
  cmPkLegCntrlReq( &pst, &egCntrlReq);
}

/**
 * @param   pjCntrlReq PDCP control req message structure
 * @param   element element of control request
 * @param   action  what action to take on PDCP stack
 * @param   subaction what the sub action to take on PDCP stack
 * @details This function fills the PDCP control request message structure
 */
void smFillPdcpTrcCntrlReq(
  PjMngmt *pjCntrlReq,
  S16     element,
  S16     action,
  S16     subAction
  )
{
  pjCntrlReq->t.cntrl.action = action;
  pjCntrlReq->t.cntrl.subAction = subAction;
  pjCntrlReq->t.cntrl.s.trcCntrl.trcLen = MAX_SIZE_OF_TRC_MSG;
  /* TODO Study the usage of Trace Mask and update this in future */
  pjCntrlReq->t.cntrl.s.trcCntrl.trcMask = 1;

  pjCntrlReq->hdr.msgType = TCNTRL;
  pjCntrlReq->hdr.entId.ent = ENTPJ;
  pjCntrlReq->hdr.entId.inst = SM_INST_ZERO;
  pjCntrlReq->hdr.elmId.elmnt = element;
  if( AENA == action )
  {
    pjCntrlReq->hdr.transId = SM_PDCP_START_TRACE_TRANS_ID;
  }
  else
  {
    pjCntrlReq->hdr.transId = SM_PDCP_STOP_TRACE_TRANS_ID;
  }
}
/**
 * @param     mBuf  Buffer contains PDCP Trace request
 * @details   This function process PDCP Trace request received from BSM
 */
void smProcPdcpTrcReq(
  Buffer *mBuf,
  U8 trcFlag
  )
{


  Pst                         pst;

  PjMngmt                     pdcpCntrlReq;

         logError("Trc Flag=%d", trcFlag); 

  /* Enable/Disable traces on RLC */
  cmMemset((U8*)&pst, '\0', sizeof(Pst));
  cmMemset((U8*)&pdcpCntrlReq, '\0', sizeof(PjMngmt));

  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTPJ;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);;
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);;

  /* Fill the control Data */
  smFillPdcpTrcCntrlReq(&pdcpCntrlReq, STGEN, trcFlag, SATRC);

  cmPkLpjCntrlReq( &pst, &pdcpCntrlReq);
}

/**
 * @param    s1apCntrlReq S1AP control req message structure
 * @param    element element of control request
 * @param    action  what action to take on S1AP stack
 * @param    subaction what the sub action to take on S1AP stack
 * @details  This function fills the S1AP control request message structure
 */
void smFillS1apTrcCntrlReq(
  SzMngmt *s1apCntrlReq,
  S16     element,
  S16     action,
  S16     subAction
  )
{
  s1apCntrlReq->hdr.elmId.elmnt = element;

  if(element == STSZSCTSAP)
  {
    if( ADISIMM == action )
    {
      s1apCntrlReq->u.cntrl.u.lSapTrc.trcLen = 0;
      s1apCntrlReq->hdr.transId = SM_S1AP_SCTSAP_STOP_TRACE_TRANS_ID;
    }
    else
    {
      s1apCntrlReq->u.cntrl.u.lSapTrc.trcLen = MAX_SIZE_OF_TRC_MSG;
      s1apCntrlReq->hdr.transId = SM_S1AP_SCTSAP_START_TRACE_TRANS_ID;
    }
    s1apCntrlReq->u.cntrl.u.lSapTrc.lSapId = 0;
  }
  else if(element == STSZPEER)
  {
    s1apCntrlReq->u.cntrl.u.pNodeTrc.peerId.pres = PRSNT_DEF;
    s1apCntrlReq->u.cntrl.u.pNodeTrc.peerId.val = 1; /* TODO Get Peer Id from BSM */
    if( ADISIMM == action )
    {
      s1apCntrlReq->u.cntrl.u.pNodeTrc.trcLen = 0;
      s1apCntrlReq->hdr.transId = SM_S1AP_PEER_STOP_TRACE_TRANS_ID;
    }
    else
    {
      s1apCntrlReq->u.cntrl.u.pNodeTrc.trcLen = MAX_SIZE_OF_TRC_MSG;
      s1apCntrlReq->hdr.transId = SM_S1AP_PEER_START_TRACE_TRANS_ID;
    }
  }

  s1apCntrlReq->hdr.response.mem.region = DFLT_REGION;
  s1apCntrlReq->hdr.response.mem.pool = SZ_POOL;
  s1apCntrlReq->hdr.response.selector = SM_SELECTOR_LC;
  s1apCntrlReq->hdr.response.prior = PRIOR0;
  s1apCntrlReq->hdr.response.route = RTESPEC;

  s1apCntrlReq->hdr.msgType = TCNTRL;
  s1apCntrlReq->hdr.entId.ent = ENTSZ;
  s1apCntrlReq->hdr.entId.inst = SM_INST_ZERO;

  s1apCntrlReq->u.cntrl.action = action;
  s1apCntrlReq->u.cntrl.subAction = subAction;
}
/**
 * @param     mBuf  Buffer contains S1AP Trace request
 * @details   This function process S1AP Trace request received from BSM
 */
void smProcS1apTrcReq(
  Buffer *mBuf,
  U8 trcFlag
  )
{


  Pst                         pst;

  SzMngmt                     s1apCntrlReq;

         logError("Trc Flag=%d", trcFlag); 

  cmMemset((U8*)&pst, '\0', sizeof(Pst));

  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTSZ;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);;
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);;

  /* Enable Trace for  SCT SAP*/
  cmMemset((U8*)&s1apCntrlReq, '\0', sizeof(SzMngmt));
  smFillS1apTrcCntrlReq(&s1apCntrlReq, STSZSCTSAP, trcFlag, SATRC);
  cmPkLszCntrlReq( &pst, &s1apCntrlReq);

  /* Enable Trace for  Peer Events */
  cmMemset((U8*)&s1apCntrlReq, '\0', sizeof(SzMngmt));
  smFillS1apTrcCntrlReq(&s1apCntrlReq, STSZPEER, trcFlag, SATRC);
  cmPkLszCntrlReq( &pst, &s1apCntrlReq);
}

/**
 * @param   rrcCntrlReq RRC control req message structure
 * @param   element element of control request
 * @param   action  what action to take on RRC stack
 * @param   subaction what the sub action to take on RRC stack
 * @details This function fills the RRC control request message structure
 */
void smFillRrcTrcCntrlReq(
  NhMngmt *rrcCntrlReq,
  S16     element,
  S16     action,
  S16     subAction
  )
{
  rrcCntrlReq->hdr.elmId.elmnt = element;
  rrcCntrlReq->u.cntrl.u.sap.sapId = 0;

  rrcCntrlReq->u.cntrl.action = action;
  rrcCntrlReq->u.cntrl.subAction = subAction;
  rrcCntrlReq->hdr.response.mem.region = DFLT_REGION;
  rrcCntrlReq->hdr.response.mem.pool = UEH_NH_POOL;
  rrcCntrlReq->hdr.response.selector = SM_SELECTOR_LC;
  rrcCntrlReq->hdr.response.prior = PRIOR0;
  rrcCntrlReq->hdr.response.route = RTESPEC;

  rrcCntrlReq->hdr.msgType = TCNTRL;
  rrcCntrlReq->hdr.entId.ent = ENTNH;
  rrcCntrlReq->hdr.entId.inst = SM_INST_ZERO;
}
/**
 * @param     rrcCntrlReq RRC - RLC  Data Sap Trace control request
 * @param     action  what action to take on RLC stack
 * @details   This function process RRC - RLC SAP Trace request
 */
void smFillNhRlcDataSapTrcCntrlReq(
  NhMngmt *rrcCntrlReq,
  S16      action
  )
{
  smFillRrcTrcCntrlReq( rrcCntrlReq, STNHKWUSAP, action, SATRC);
  if( ADISIMM == action )
  {
    rrcCntrlReq->hdr.transId = SM_RRC_KWUSAP_STOP_TRACE_TRANS_ID;
  }
  else
  {
    rrcCntrlReq->hdr.transId = SM_RRC_KWUSAP_START_TRACE_TRANS_ID;
  }
}
/**
 * @param     rrcCntrlReq RRC - RLC  Control Sap Trace control request
 * @param     action  what action to take on RLC stack
 * @details   This function process RRC - RLC SAP Trace request
 */
void smFillNhRlcControlSapTrcCntrlReq(
  NhMngmt *rrcCntrlReq,
  S16   action
  )
{
  smFillRrcTrcCntrlReq( rrcCntrlReq, STNHCKWSAP, action, SATRC);
  if( ADISIMM == action )
  {
    rrcCntrlReq->hdr.transId = SM_RRC_CKWSAP_STOP_TRACE_TRANS_ID;
  }
  else
  {
    rrcCntrlReq->hdr.transId = SM_RRC_CKWSAP_START_TRACE_TRANS_ID;
  }
}
/**
 * @param     rrcCntrlReq RRC - MAC  Control Sap Trace control request
 * @param     action  what action to take on MAC stack
 * @details   This function process RRC - MAC SAP Trace request
 */
void smFillNhMacSapTrcCntrlReq(
  NhMngmt *rrcCntrlReq,
  S16      action
  )
{
  smFillRrcTrcCntrlReq( rrcCntrlReq, STNHCRGSAP, action, SATRC);
  if( ADISIMM == action )
  {
    rrcCntrlReq->hdr.transId = SM_RRC_CRGSAP_STOP_TRACE_TRANS_ID;
  }
  else
  {
    rrcCntrlReq->hdr.transId = SM_RRC_CRGSAP_START_TRACE_TRANS_ID;
  }
}
/**
 * @param     rrcCntrlReq RRC - PDCP  Data Sap Trace control request
 * @param     action  what action to take on PDCP stack
 * @details   This function process RRC - PDCP SAP Trace request
 */
void smFillNhPdcpDataSapTrcCntrlReq(
  NhMngmt *rrcCntrlReq,
  S16     action
  )
{
  smFillRrcTrcCntrlReq( rrcCntrlReq, STNHPJUSAP, action, SATRC);
  if( ADISIMM == action )
  {
    rrcCntrlReq->hdr.transId = SM_RRC_PJUSAP_STOP_TRACE_TRANS_ID;
  }
  else
  {
    rrcCntrlReq->hdr.transId = SM_RRC_PJUSAP_START_TRACE_TRANS_ID;
  }
}
/**
 * @param     rrcCntrlReq RRC - PDCP  Control Sap Trace control request
 * @param     action  what action to take on PDCP stack
 * @details   This function process RRC - PDCP SAP Trace request
 */
void smFillNhPdcpControlSapTrcCntrlReq(
  NhMngmt *rrcCntrlReq,
  S16      action
  )
{
  smFillRrcTrcCntrlReq( rrcCntrlReq, STNHCPJSAP, action, SATRC);
  if( ADISIMM == action )
  {
    rrcCntrlReq->hdr.transId = SM_RRC_CPJSAP_STOP_TRACE_TRANS_ID;
  }
  else
  {
    rrcCntrlReq->hdr.transId = SM_RRC_CPJSAP_START_TRACE_TRANS_ID;
  }
}

/**
 * @param     mBuf contains RRC trace request
 * @details   This function process RRC Trace request received from BSM
 */
void smProcRrcTrcReq(
  Buffer *mBuf,
  U8 trcFlag
  )
{

  Pst                         pst;
  NhMngmt                     rrcCntrlReq;

         logError("Trc Flag=%d", trcFlag); 
  cmMemset((U8*)&pst, '\0', sizeof(Pst));
  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTNH;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);;
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);;

  /* Enable Trace for  RLC Data SAP*/
  cmMemset((U8*)&rrcCntrlReq, '\0', sizeof(NhMngmt));
  smFillNhRlcDataSapTrcCntrlReq(&rrcCntrlReq, trcFlag );
  cmPkLnhCntrlReq( &pst, &rrcCntrlReq);

  /* Enable Trace for  RLC Control SAP*/
  cmMemset((U8*)&rrcCntrlReq, '\0', sizeof(NhMngmt));
  smFillNhRlcControlSapTrcCntrlReq(&rrcCntrlReq, trcFlag );
  cmPkLnhCntrlReq( &pst, &rrcCntrlReq);

  /* Enable Trace for  MAC SAP*/
  cmMemset((U8*)&rrcCntrlReq, '\0', sizeof(NhMngmt));
  smFillNhMacSapTrcCntrlReq(&rrcCntrlReq, trcFlag);
  cmPkLnhCntrlReq( &pst, &rrcCntrlReq);

  /* Enable Trace for  PDCP Control SAP*/
  cmMemset((U8*)&rrcCntrlReq, '\0', sizeof(NhMngmt));
  smFillNhPdcpControlSapTrcCntrlReq(&rrcCntrlReq, trcFlag);
  cmPkLnhCntrlReq( &pst, &rrcCntrlReq);

  /* Enable Trace for  PDCP Data SAP*/
  cmMemset((U8*)&rrcCntrlReq, '\0', sizeof(NhMngmt));
  smFillNhPdcpDataSapTrcCntrlReq(&rrcCntrlReq, trcFlag);
  cmPkLnhCntrlReq( &pst, &rrcCntrlReq);
}
/**
 * @param     event  Event to be sent to LTH
 * @param     len    Length of the message to be sent to LTH
 * @param     *data  Data sent to LTH
 * @details   Process function is used to send events from SM to LTH
 */
PUBLIC S16 sendSmMsgToLth(
  Event   event,
  MsgLen    len,
  U8     *data
  )
{
  if( sendEventToOam( ENTSM, ENT_LTH, event, len, data ) !=ROK)
  {
    logError("Error in sending event:%d from SM to LTH",event );
    return RFAILED;
  }
  else
  {
    logInfo("Sending event:%d from SM to LTH", event );
    return ROK;
  }
}
