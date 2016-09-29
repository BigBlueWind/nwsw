/**
 * @file     sm_pmhsm_exms.c
 * @details  Copyright Powerwave Technologies, Inc., 2010
 * @brief    This file contains Handlers for Messages coming from PMH to SM
 * @author   Madhusudana Rao Kata
 * @date     07-Jun-2011
 * @details  1. sm_sfi_exms.c is divided into sm_bsmsm_exms.c and sm_pmhsm_exms.c @n
 */

#include "sm.h"


/* ********************* STATS REQUEST HANDLERS ***************************** */

/**
 * @param     event  Event to be sent to PMH
 * @param     len    Length of the message to be sent to PMH
 * @param     *data  Data sent to PMH
 * @details   Process function is used to send events from SM to PMH
 */

PUBLIC S16 sendSmMsgToPmh(
  Event   event,
  MsgLen    len,
  U8     *data
  )
{
  if( sendEventToOam( ENTSM, ENT_PMH, event, len, data ) !=ROK)
  {
    logError("Error in sending event:%d from SM to PMH",event );
    return RFAILED;
  }
  else
  {
    logInfo("Sending event:%d from SM to PMH", event );
    return ROK;
  }
}



/**
 * @param   kwCntrlReq PDCP control req message structure
 * @param   element element of control request
 * @details This function fills the PDCP control request message structure
 */
void smFillPdcpStsReq(
  PjMngmt *pjCntrlReq,
  S16      element
  )
{
  /* Fill the header of control message */
  pjCntrlReq->hdr.msgType = TSTS;
  pjCntrlReq->hdr.entId.ent = ENTPJ;
  pjCntrlReq->hdr.entId.inst = SM_INST_ZERO;
  pjCntrlReq->hdr.elmId.elmnt = element;
}
/**
 * @param   rcvdPst Post structure used to send Statistics Request for PDCP
 * @param   element element of control request
 * @details This function process the PDCP Statistics control request message
 */
void smProcPdcpStsReq(
  Pst         *rcvdPst,
  Buffer      *mBuf
  )
{
  MsgLen        cCnt;
  PjMngmt       pdcpCntrlReq;
  S16           element;
  Pst           pst;
  TBOOL         resetCounterFlag = LPJ_NOZEROSTS;

  MsgPmhSmPdcpGenStatReq         pdcpGenStatReq;
  MsgPmhSmPdcpUplaneStatReq      pdcpUplaneStatReq;

  logInfo("Sending Statistics Request for PDCP for element:%d",rcvdPst->event);

  cmMemset((U8*)&pdcpCntrlReq, '\0', sizeof(PjMngmt));

  if( EVT_PMH_SM_PDCP_UPLANE_STAT_REQ == rcvdPst->event)
  {
    SCpyMsgFix( mBuf, 0, sizeof(MsgPmhSmPdcpUplaneStatReq), (Data*)&pdcpUplaneStatReq , &cCnt );
    pdcpCntrlReq.hdr.transId = pdcpUplaneStatReq.transId;
    pdcpCntrlReq.t.sts.s.pjuSap.spId = 1; /* User Plane Counters need to be retrieved using PJ_DRB_SAP */
    if ( TRUE EQ pdcpUplaneStatReq.resetAllCounters )
    {
      resetCounterFlag = LPJ_ZEROSTS; /* Reset statistics counters */
    }
    element = STPJUSAP;
  }
  else
  {
    SCpyMsgFix( mBuf, 0, sizeof(MsgPmhSmPdcpGenStatReq), (Data*)&pdcpGenStatReq , &cCnt );
    pdcpCntrlReq.hdr.transId = pdcpGenStatReq.transId;
    if ( TRUE EQ pdcpGenStatReq.resetAllCounters )
    {
      resetCounterFlag = LPJ_ZEROSTS; /* Reset statistics counters */
    }
    element = STGEN;
  }

  cmMemset((U8*)&pst, '\0', sizeof(Pst));
  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTPJ;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);;
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);;

  smFillPdcpStsReq(&pdcpCntrlReq, element );
  cmPkLpjStsReq( &pst, resetCounterFlag, &pdcpCntrlReq );
}

/**
 * @param    sbCntrlReq SCTP control req message structure
 * @param    element element of control request
 * @details  This function fills the SCTP control request message structure
 */
void smFillSctpStsReq(
  SbMgmt      *sbCntrlReq,
  S16         element
  )
{
  /* Fill the header of control message */
  sbCntrlReq->hdr.msgType = TSTS;
  sbCntrlReq->hdr.entId.ent = ENTSB;
  sbCntrlReq->hdr.entId.inst = SM_INST_ZERO;
  sbCntrlReq->hdr.elmId.elmnt = element;
}
/**
 * @param   rcvdPst Post structure used to send Statistics Request for SCTP
 * @param   element element of control request
 * @details This function process the SCTP Statistics control request message
 */
void smProcSctpStsReq(
  Pst         *rcvdPst,
  Buffer      *mBuf
  )
{
  MsgPmhSmSctpGenStatReq        sctpGenStatReq;
  MsgLen                        cCnt;
  SbMgmt                        sctpCntrlReq;
  Pst                           pst;
  TBOOL                         resetCounterFlag = NOZEROSTS;

  SCpyMsgFix( mBuf, 0, sizeof(MsgPmhSmSctpGenStatReq), (Data*)&sctpGenStatReq , &cCnt );
  logInfo("Sending Statistics Request for SCTP General Counters (resetAllCounters:%d)", sctpGenStatReq.resetAllCounters);

  cmMemset((U8*)&pst, '\0', sizeof(Pst));
  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTSB;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);;
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);;

  cmMemset((U8*)&sctpCntrlReq, '\0', sizeof(SbMgmt));
  sctpCntrlReq.hdr.transId = sctpGenStatReq.transId;
  if ( TRUE EQ sctpGenStatReq.resetAllCounters )
  {
    resetCounterFlag = ZEROSTS; /* Reset statistics counters */
  }
  smFillSctpStsReq(&sctpCntrlReq, STSBGEN ); /* We are interested only in SCTP General Counters */
  cmPkLsbStsReq( &pst, resetCounterFlag, &sctpCntrlReq );
}

/**
 * @param   egCntrlReq GTP control req message structure
 * @param   element element of control request
 * @details This function fills the GTP control request message structure
 */
void smFillEgtpStsReq(
  EgMngmt  *egCntrlReq,
  S16      element
  )
{
  /* Fill the header of control message */
  egCntrlReq->hdr.msgType = TSTS;
  egCntrlReq->hdr.entId.ent = ENTEG;
  egCntrlReq->hdr.entId.inst = SM_INST_ZERO;
  egCntrlReq->hdr.elmId.elmnt = element;
}
/**
 * @param   rcvdPst Post structure used to send Statistics Request for GTP
 * @param   element element of control request
 * @details This function process the GTP Statistics control request message
 */
void smProcEgtpStsReq(
  Pst           *rcvdPst,
  Buffer        *mBuf
  )
{
  MsgPmhSmGtpGenStatReq         gtpGenStatReq;
  MsgLen                        cCnt;
  EgMngmt                       gtpStsReq;
  Pst                           pst;
  TBOOL                         resetCounterFlag = NOZEROSTS;

  SCpyMsgFix( mBuf, 0, sizeof(MsgPmhSmGtpGenStatReq), (Data*)&gtpGenStatReq , &cCnt );
  logInfo("Sending Statistics Request for EGTP General Counters resetAllCounters:%d", gtpGenStatReq.resetAllCounters);

  cmMemset((U8*)&pst, '\0', sizeof(Pst));
  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTEG;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);;
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);;

  cmMemset((U8*)&gtpStsReq, '\0', sizeof(EgMngmt));
  gtpStsReq.hdr.transId = gtpGenStatReq.transId;
  if ( TRUE EQ gtpGenStatReq.resetAllCounters )
  {
    resetCounterFlag = ZEROSTS;
  }
  smFillEgtpStsReq(&gtpStsReq, STEGGEN ); /* We are interested only in EGTP General Counters */
  cmPkLegStsReq( &pst, resetCounterFlag, &gtpStsReq );
}
/**
 * @param   kwCntrlReq RLC control req message structure
 * @param   element element of control request
 * @details This function fills the RLC control request message structure
 */
void smFillRlcStsReq(
  KwMngmt  *kwCntrlReq,
  S16      element
  )
{
  kwCntrlReq->hdr.msgType = TSTS;
  kwCntrlReq->hdr.entId.ent = ENTKW;
  kwCntrlReq->hdr.entId.inst = SM_INST_ZERO;
  kwCntrlReq->hdr.elmId.elmnt = element;
}
/**
 * @param   rcvdPst Post structure used to send Statistics Request for RLC
 * @param   element element of control request
 * @details This function process the RLC Statistics control request message
 */
void smProcRlcStsReq(
  Pst           *rcvdPst,
  Buffer        *mBuf
)
{
  MsgPmhSmRlcGenStatReq         rlcGenStatReq;
  MsgLen                        cCnt;
  KwMngmt                       rlcCntrlReq;
  Pst                           pst;
  TBOOL                         resetCounterFlag = LKW_NOZEROSTS;

  SCpyMsgFix( mBuf, 0, sizeof(MsgPmhSmRlcGenStatReq), (Data*)&rlcGenStatReq , &cCnt );
  logInfo("%s", "Sending Statistics Request for RLC General Counters");

  cmMemset((U8*)&pst, '\0', sizeof(Pst));
  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTKW;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);;
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);;

  cmMemset((U8*)&rlcCntrlReq, '\0', sizeof(KwMngmt));
  rlcCntrlReq.hdr.transId = rlcGenStatReq.transId;
  if ( TRUE EQ rlcGenStatReq.resetAllCounters )
  {
    resetCounterFlag = LKW_ZEROSTS;
  }
  smFillRlcStsReq(&rlcCntrlReq, STGEN );
  cmPkLkwStsReq( &pst, resetCounterFlag, &rlcCntrlReq );
}
/**
 * @param   szCntrlReq S1AP control req message structure
 * @param   element element of control request
 * @details This function fills the S1AP control request message structure
 */
void smFillS1apStsReq(
  SzMngmt  *szCntrlReq,
  S16      element
  )
{
  /* Fill the header of control message */
  szCntrlReq->hdr.msgType = TSTS;
  szCntrlReq->hdr.entId.ent = ENTSZ;
  szCntrlReq->hdr.entId.inst = SM_INST_ZERO;
  szCntrlReq->hdr.elmId.elmnt = element;
}
/**
 * @param   rcvdPst Post structure used to send Statistics Request for S1AP
 * @param   element element of control request
 * @details This function process the S1AP Statistics control request message
 */
void smProcS1apStsReq(
  Pst           *rcvdPst,
  Buffer        *mBuf
  )
{
  MsgPmhSmS1apGenStatReq        s1GenStatReq;
  MsgPmhSmS1apPeerNodeStatReq   s1PeerStatReq;
  MsgLen                        cCnt;
  SzMngmt                       s1apStsReq;
  S16                           element;
  Pst                           pst;
  TBOOL                         resetCounterFlag = NOZEROSTS;

  logInfo("Sending Statistics Request for S1AP for element:%d",rcvdPst->event );

  cmMemset((U8*)&s1apStsReq, '\0', sizeof(SzMngmt));
  if( EVT_PMH_SM_S1AP_PEER_NODE_STAT_REQ == rcvdPst->event)
  {
    SCpyMsgFix( mBuf, 0, sizeof(MsgPmhSmS1apPeerNodeStatReq), (Data*)&s1PeerStatReq , &cCnt );
    s1apStsReq.u.sts.u.node.peerId.val  = s1PeerStatReq.peerId;
    s1apStsReq.u.sts.u.node.peerId.pres = TRUE;
    s1apStsReq.hdr.transId = s1PeerStatReq.transId;
    if ( TRUE EQ s1PeerStatReq.resetAllCounters )
    {
      resetCounterFlag = ZEROSTS;
    }
    element = STSZPEER;
  }
  else
  {
    SCpyMsgFix( mBuf, 0, sizeof(MsgPmhSmS1apGenStatReq), (Data*)&s1GenStatReq , &cCnt );
    s1apStsReq.hdr.transId = s1GenStatReq.transId;
    if ( TRUE EQ s1GenStatReq.resetAllCounters )
    {
      resetCounterFlag = ZEROSTS;
    }
    element = STSZGEN;
  }

  cmMemset((U8*)&pst, '\0', sizeof(Pst));
  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTSZ;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);;
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);;

  smFillS1apStsReq(&s1apStsReq, element);
  cmPkLszStsReq( &pst, resetCounterFlag, &s1apStsReq );
}
/**
 * @param   nhCntrlReq RRC control req message structure
 * @param   element element of control request
 * @details This function fills the RRC control request message structure
 */
void smFillRrcStsReq(
  NhMngmt *nhCntrlReq,
  S16      element
  )
{
  nhCntrlReq->hdr.msgType = TSTS;
  nhCntrlReq->hdr.entId.ent = ENTNH;
  nhCntrlReq->hdr.entId.inst = SM_INST_ZERO;
  nhCntrlReq->hdr.elmId.elmnt = element;
}
/**
 * @param   rcvdPst Post structure used to send Statistics Request for RRC
 * @param   element element of control request
 * @details This function process the RRC Statistics control request message
 */
void smProcRrcStsReq(
  Pst           *rcvdPst,
  Buffer        *mBuf
  )
{
  MsgPmhSmRrcCellStatReq        rrcPeerStatReq;
  MsgPmhSmRrcGenStatReq         rrcGenStatReq;
  MsgLen                        cCnt;
  NhMngmt                       rrcCntrlReq;
  S16                           element;
  Pst                           pst;
  TBOOL                         resetCounterFlag = NOZEROSTS;

  logInfo("Sending Statistics Request for RRC for element:%d",rcvdPst->event );

  cmMemset((U8*)&rrcCntrlReq, '\0', sizeof(NhMngmt));

  if( EVT_PMH_SM_RRC_CELL_STAT_REQ == rcvdPst->event)
  {
    SCpyMsgFix( mBuf, 0, sizeof(MsgPmhSmRrcCellStatReq), (Data*)&rrcPeerStatReq , &cCnt );
    rrcCntrlReq.hdr.transId = rrcPeerStatReq.transId;
    if ( TRUE EQ rrcPeerStatReq.resetAllCounters )
    {
      resetCounterFlag = ZEROSTS;
    }
    rrcCntrlReq.u.sts.u.cellSts.cellId = rrcPeerStatReq.cellId;
    element = STNHCELL;
  }
  else
  {
    SCpyMsgFix( mBuf, 0, sizeof(MsgPmhSmRrcGenStatReq), (Data*)&rrcGenStatReq , &cCnt );
    rrcCntrlReq.hdr.transId = rrcGenStatReq.transId;
    if ( TRUE EQ rrcGenStatReq.resetAllCounters )
    {
      resetCounterFlag = ZEROSTS;
    }
    element = STNHGEN;
  }

  cmMemset((U8*)&pst, '\0', sizeof(Pst));
  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTNH;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);;
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);;
  smFillRrcStsReq(&rrcCntrlReq, element );
  cmPkLnhStsReq( &pst, resetCounterFlag, &rrcCntrlReq );
}
/**
 * @param   rgCntrlReq MAC control req message structure
 * @param   element element of control request
 * @details This function fills the MAC control request message structure
 */
void smFillMacStsReq(
  RgMngmt  *rgCntrlReq,
  S16      element
  )
{
  rgCntrlReq->hdr.msgType = TSTS;
  rgCntrlReq->hdr.entId.ent = ENTRG;
  rgCntrlReq->hdr.entId.inst = SM_INST_ZERO;
  rgCntrlReq->hdr.elmId.elmnt = element;
}
/**
 * @param   rcvdPst Post structure used to send Statistics Request for MAC
 * @param   element element of control request
 * @details This function process the MAC Statistics control request message
 */
void smProcMacStsReq(
  Pst           *rcvdPst,
  Buffer        *mBuf
  )
{
  MsgLen                        cCnt;
  RgMngmt                       macStsReq;
  S16                           element;
  Pst                           pst;

  logInfo("Sending Statistics Request for MAC for element:%d",rcvdPst->event );

  cmMemset((U8*)&macStsReq, '\0', sizeof(RgMngmt));
  if( EVT_PMH_SM_MAC_RLCSAP_STAT_REQ == rcvdPst->event)
  {
    MsgPmhSmMacRlcsapStatReq        macRlcsapStatReq;
    SCpyMsgFix( mBuf, 0, sizeof(MsgPmhSmMacRlcsapStatReq), (Data*)&macRlcsapStatReq , &cCnt );
    macStsReq.hdr.transId = macRlcsapStatReq.transId;
    if ( TRUE EQ macRlcsapStatReq.resetAllCounters )
    {
      macStsReq.t.sts.action = ARST;
    }
    element = STRGUSAP;
  }
  else if( EVT_PMH_SM_MAC_PHYSAP_STAT_REQ == rcvdPst->event)
  {
    MsgPmhSmMacPhysapStatReq         macPhysapStatReq;
    SCpyMsgFix( mBuf, 0, sizeof(MsgPmhSmMacPhysapStatReq), (Data*)&macPhysapStatReq , &cCnt );
    macStsReq.hdr.transId = macPhysapStatReq.transId;
    if ( TRUE EQ macPhysapStatReq.resetAllCounters )
    {
      macStsReq.t.sts.action = ARST;
    }
    element = STTFUSAP;
  }
  else
  {
    MsgPmhSmMacGenStatReq        macGenStatReq;
    SCpyMsgFix( mBuf, 0, sizeof(MsgPmhSmMacGenStatReq), (Data*)&macGenStatReq , &cCnt );
    macStsReq.hdr.transId = macGenStatReq.transId;
    if ( TRUE EQ macGenStatReq.resetAllCounters )
    {
      macStsReq.t.sts.action = ARST;
    }
    element = STGEN;
  }

  cmMemset((U8*)&pst, '\0', sizeof(Pst));
  /* Fill Pst */
  pst.selector  = SM_SELECTOR_LC;
  pst.srcEnt    = ENTSM;
  pst.dstEnt    = ENTRG;
  pst.dstProcId = getProcIdByEntityId(pst.dstEnt);;
  pst.srcProcId = getProcIdByEntityId(pst.srcEnt);;

  smFillMacStsReq(&macStsReq, element );
  cmPkLrgStsReq( &pst, &macStsReq );
}

/**
 * @param     pst   post structure contains source and destinations information
 * @param     mBuf  Buffer contains message coming from PMH
 * @details   Entry point to the messages received from PMH to SM
 */
PUBLIC S16 pmhSmActvTsk(
  Pst         *pst,
  Buffer      *mBuf
  )
{
   S16 ret;
   ret = ROK;

   switch(pst->event)
   {
      case EVT_PMH_SM_PDCP_GEN_STAT_REQ:
      case EVT_PMH_SM_PDCP_UPLANE_STAT_REQ:
        smProcPdcpStsReq(pst, mBuf);
        SPutMsg(mBuf);
        break;

      case EVT_PMH_SM_SCTP_GEN_STAT_REQ:
        smProcSctpStsReq(pst, mBuf);
        SPutMsg(mBuf);
        break;

      case EVT_PMH_SM_GTP_GEN_STAT_REQ:
        smProcEgtpStsReq(pst, mBuf);
        SPutMsg(mBuf);
        break;

      case EVT_PMH_SM_RLC_GEN_STAT_REQ:
        smProcRlcStsReq( pst, mBuf);
        SPutMsg(mBuf);
        break;

      case EVT_PMH_SM_S1AP_GEN_STAT_REQ:
      case EVT_PMH_SM_S1AP_PEER_NODE_STAT_REQ:
        smProcS1apStsReq(pst, mBuf);
        SPutMsg(mBuf);
        break;

      case EVT_PMH_SM_RRC_GEN_STAT_REQ:
      case EVT_PMH_SM_RRC_CELL_STAT_REQ:
        smProcRrcStsReq(pst, mBuf);
        SPutMsg(mBuf);
        break;

      case EVT_PMH_SM_MAC_GEN_STAT_REQ:
      case EVT_PMH_SM_MAC_RLCSAP_STAT_REQ:
      case EVT_PMH_SM_MAC_PHYSAP_STAT_REQ:
        smProcMacStsReq(pst, mBuf);
        SPutMsg(mBuf);
        break;

      default:
         SPutMsg(mBuf);
         ret = RFAILED;
         logError("%s","Received invalid event\n");
         break;
   }
   RETVALUE(ret);
}
