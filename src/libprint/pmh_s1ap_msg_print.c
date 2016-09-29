/**
 * @file    pmh_s1ap_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between PMH and S1AP
 * @author : Ankur Agrawal
 * Date   : 08-Jul-2011
 * Change History:
 *
 */

#include "pmh_s1ap_msg_print.h"

void printMsgPmhSmS1apGenStatReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhSmS1apGenStatReq *msg = (MsgPmhSmS1apGenStatReq*)data;
  logMsgParams( "{\n transId=%d\n resetAllCounters=%d\n",msg->transId, msg->resetAllCounters );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgPmhSmS1apPeerNodeStatReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhSmS1apPeerNodeStatReq *msg = (MsgPmhSmS1apPeerNodeStatReq*)data;
  logMsgParams( "{\n transId=%d\n resetAllCounters=%d\n",msg->transId, msg->resetAllCounters );
  logMsgParams("} Total length = %d\n", len );
}


void printMsgSmPmhS1apGenStatRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgSmPmhS1apGenStatRsp *msg = (MsgSmPmhS1apGenStatRsp*)data;
  logMsgParams( "{\n transId=%d\n nmbConn=%d\n nmbFailConn=%d\n maxMemAlloc=%d\n"
             "nmbDecErr=%d\n nmbEncErr=%d\n nmbAudReq=%d\n"
             "nmbAudConn=%d\n nmbAudPeer=%d\n nmbAudFlc=%d\n lastCollectedAt=%s\n",
              msg->transId, msg->s1apGenCounters.nmbConn, msg->s1apGenCounters.nmbFailConn, msg->s1apGenCounters.maxMemAlloc, msg->s1apGenCounters.nmbDecErr, msg->s1apGenCounters.nmbEncErr,
              msg->s1apGenCounters.nmbAudReq, msg->s1apGenCounters.nmbAudConn, msg->s1apGenCounters.nmbAudPeer, msg->s1apGenCounters.nmbAudFlc, msg->s1apGenCounters.lastCollectedAt );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgSmPmhS1apPeerNodeStatRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgSmPmhS1apPeerNodeStatRsp *msg = (MsgSmPmhS1apPeerNodeStatRsp*)data;
  logMsgParams( "%s", "{\n" );
  logMsgParams( " transId=%d\n index=%d\n peerId=%d\n nmbUnexpEvt=%d\n", msg->transId, msg->s1apPeerNodeCounters.index, msg->s1apPeerNodeCounters.peerId, msg->s1apPeerNodeCounters.nmbUnexpEvt );
  logMsgParams( " nmbTRafExp=%d\n eRABSetupReq=%d\n eRABSetupResp=%d\n", msg->s1apPeerNodeCounters.nmbTRafExp, msg->s1apPeerNodeCounters.eRABSetupReq, msg->s1apPeerNodeCounters.eRABSetupResp );
  logMsgParams( " eRABModifyRequest=%d\n eRABModResp=%d\n eRABRelCmd=%d\n", msg->s1apPeerNodeCounters.eRABModifyRequest, msg->s1apPeerNodeCounters.eRABModResp, msg->s1apPeerNodeCounters.eRABRelCmd );

  logMsgParams( " eRABRelComp=%d\n eRABRelReq=%d\n initCxtSetup=%d\n", msg->s1apPeerNodeCounters.eRABRelComp, msg->s1apPeerNodeCounters.eRABRelReq, msg->s1apPeerNodeCounters.initCxtSetup );
  logMsgParams( " initCxtSetupResp=%d\n initCxtSetupFail=%d\n ueCxtRelReq=%d\n", msg->s1apPeerNodeCounters.initCxtSetupResp, msg->s1apPeerNodeCounters.initCxtSetupFail, msg->s1apPeerNodeCounters.ueCxtRelReq );
  logMsgParams( " ueCxtRelCmd=%d\n ueCxtRelComp=%d\n ueCxtModReq=%d\n", msg->s1apPeerNodeCounters.ueCxtRelCmd, msg->s1apPeerNodeCounters.ueCxtRelComp, msg->s1apPeerNodeCounters.ueCxtModReq );

  logMsgParams( " ueCxtModResp=%d\n ueCxtModFail=%d\n initUEMsg=%d\n", msg->s1apPeerNodeCounters.ueCxtModResp, msg->s1apPeerNodeCounters.ueCxtModFail, msg->s1apPeerNodeCounters.initUEMsg );
  logMsgParams( " dwnNASTpt=%d\n upNASTpt=%d\n nonDelIndNAS=%d\n", msg->s1apPeerNodeCounters.dwnNASTpt, msg->s1apPeerNodeCounters.upNASTpt, msg->s1apPeerNodeCounters.nonDelIndNAS );
  logMsgParams( " errIndtx=%d\n errIndrx=%d\n noDatatx=%d\n", msg->s1apPeerNodeCounters.errIndtx, msg->s1apPeerNodeCounters.errIndrx, msg->s1apPeerNodeCounters.noDatatx );

  logMsgParams( " noDatarx=%d\n resettx=%d\n resetrx=%d\n", msg->s1apPeerNodeCounters.noDatarx, msg->s1apPeerNodeCounters.resettx, msg->s1apPeerNodeCounters.resetrx );
  logMsgParams( " resetAcktx=%d\n resetAckrx=%d\n setupReq=%d\n", msg->s1apPeerNodeCounters.resetAcktx, msg->s1apPeerNodeCounters.resetAckrx, msg->s1apPeerNodeCounters.setupReq );
  logMsgParams( " setupResp=%d\n setupFail=%d\n paging=%d\n", msg->s1apPeerNodeCounters.setupResp, msg->s1apPeerNodeCounters.setupFail, msg->s1apPeerNodeCounters.paging );

  logMsgParams( " handReqd=%d\n handCmd=%d\n handPrepFail=%d\n", msg->s1apPeerNodeCounters.handReqd, msg->s1apPeerNodeCounters.handCmd, msg->s1apPeerNodeCounters.handPrepFail );
  logMsgParams( " handReq=%d\n handReqAck=%d\n handFail=%d\n", msg->s1apPeerNodeCounters.handReq, msg->s1apPeerNodeCounters.handReqAck, msg->s1apPeerNodeCounters.handFail );
  logMsgParams( " handNtfy=%d\n pathSwchReq=%d\n pathSwchReqAck=%d\n", msg->s1apPeerNodeCounters.handNtfy, msg->s1apPeerNodeCounters.pathSwchReq, msg->s1apPeerNodeCounters.pathSwchReqAck );

  logMsgParams( " handCan=%d\n handCanAck=%d\n eNBStaTrans=%d\n", msg->s1apPeerNodeCounters.handCan, msg->s1apPeerNodeCounters.handCanAck, msg->s1apPeerNodeCounters.eNBStaTrans );
  logMsgParams( " mmeStaTrans=%d\n deactTrace=%d\n eNBStaTrans=%d\n", msg->s1apPeerNodeCounters.mmeStaTrans, msg->s1apPeerNodeCounters.deactTrace, msg->s1apPeerNodeCounters.eNBStaTrans );
  logMsgParams( " traceFailInd=%d\n locReportCntrl=%d\n locReportFailInd=%d\n", msg->s1apPeerNodeCounters.traceFailInd, msg->s1apPeerNodeCounters.locReportCntrl, msg->s1apPeerNodeCounters.locReportFailInd );

  logMsgParams( " locReport=%d\n enbCfgUpd=%d\n enbCfgUpdAck=%d\n", msg->s1apPeerNodeCounters.locReport, msg->s1apPeerNodeCounters.enbCfgUpd, msg->s1apPeerNodeCounters.enbCfgUpdAck );
  logMsgParams( " enbCfgUpdFail=%d\n mmeCfgUpd=%d\n mmeCfgUpdAck=%d\n", msg->s1apPeerNodeCounters.enbCfgUpdFail, msg->s1apPeerNodeCounters.mmeCfgUpd, msg->s1apPeerNodeCounters.mmeCfgUpdAck );
  logMsgParams( " mmeCfgUpdFail=%d\n upS1CDMA2000Tunn=%d\n dnS1CDMA2000Tunn=%d\n", msg->s1apPeerNodeCounters.mmeCfgUpdFail, msg->s1apPeerNodeCounters.upS1CDMA2000Tunn, msg->s1apPeerNodeCounters.dnS1CDMA2000Tunn );

  logMsgParams( " ueCapInfoInd=%d\n overloadStart=%d\n overloadStop=%d\n", msg->s1apPeerNodeCounters.ueCapInfoInd, msg->s1apPeerNodeCounters.overloadStart, msg->s1apPeerNodeCounters.overloadStop );
  logMsgParams( " writeRepWarnReq=%d\n writeRepWarnRsp=%d\n enbDirInfoTrans=%d\n", msg->s1apPeerNodeCounters.writeRepWarnReq, msg->s1apPeerNodeCounters.writeRepWarnRsp, msg->s1apPeerNodeCounters.enbDirInfoTrans );
  logMsgParams( " mmeDirInfoTrans=%d\n privMsgtx=%d\n privMsgrx=%d\n", msg->s1apPeerNodeCounters.mmeDirInfoTrans, msg->s1apPeerNodeCounters.privMsgtx, msg->s1apPeerNodeCounters.privMsgrx );

  logMsgParams( " eNBConfigTfr=%d\n mmeConfigTfr=%d\n cellTrafficTrace=%d\n", msg->s1apPeerNodeCounters.eNBConfigTfr, msg->s1apPeerNodeCounters.mmeConfigTfr, msg->s1apPeerNodeCounters.cellTrafficTrace );
  logMsgParams( " szDedProcStstx=%d\n szDedProcStsrx=%d\n szComProcStstx=%d\n szComProcStsrx=%d\n", msg->s1apPeerNodeCounters.szDedProcStstx, msg->s1apPeerNodeCounters.szDedProcStsrx, msg->s1apPeerNodeCounters.szComProcStstx, msg->s1apPeerNodeCounters.szComProcStsrx );
  logMsgParams("} Total length = %d\n", len );
}
