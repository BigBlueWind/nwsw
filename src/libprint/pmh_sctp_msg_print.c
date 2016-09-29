/**
 * @file    pmh_sctp_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between PMH and SCTP
 * @author : Ankur Agrawal
 * Date   : 17-Mar-2011
 * Change History:
 *
 */

#include "pmh_sctp_msg_print.h"

void printMsgPmhSmSctpGenStatReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhSmSctpGenStatReq *msg = (MsgPmhSmSctpGenStatReq*)data;
  logMsgParams( "{\n transId=%d\n resetAllCounters=%d\n",msg->transId, msg->resetAllCounters);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgSmPmhSctpGenStatRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgSmPmhSctpGenStatRsp *msg = (MsgSmPmhSctpGenStatRsp*)data;
  logMsgParams( "{\n transId=%d\n noInitTx=%d\n noInitReTx=%d\n noInitRx=%d\n noIAckTx=%d\n noIAckRx=%d\n noShDwnTx=%d\n noShDwnReTx=%d\n",\
              msg->transId, msg->sctpGenCounters.noInitTx,msg->sctpGenCounters.noInitReTx, msg->sctpGenCounters.noInitRx, msg->sctpGenCounters.noIAckTx, msg->sctpGenCounters.noIAckRx, msg->sctpGenCounters.noShDwnTx, msg->sctpGenCounters.noShDwnReTx);
  logMsgParams( " noShDwnRx=%d\n noShDwnAckTx=%d\n noShDwnAckReTx=%d\n noShDwnAckRx=%d\n noCookieTx=%d\n noCookieReTx=%d\n noCookieRx=%d\n",\
              msg->sctpGenCounters.noShDwnRx, msg->sctpGenCounters.noShDwnAckTx, msg->sctpGenCounters.noShDwnAckReTx, msg->sctpGenCounters.noShDwnAckRx, msg->sctpGenCounters.noCookieTx, msg->sctpGenCounters.noCookieReTx, msg->sctpGenCounters.noCookieRx);
  logMsgParams( " noCkAckTx=%d\n noCkAckRx=%d\n noDataTx=%d\n noDataReTx=%d\n noDataRx=%d\n noDAckTx=%d\n",\
              msg->sctpGenCounters.noCkAckTx, msg->sctpGenCounters.noCkAckRx, msg->sctpGenCounters.noDataTx, msg->sctpGenCounters.noDataReTx, msg->sctpGenCounters.noDataRx, msg->sctpGenCounters.noDAckTx);
  logMsgParams( " noDAckRx=%d\n noShDwnCmpltTx=%d\n noShDwnCmpltRx=%d\n noHBeatTx=%d\n noHBeatRx=%d\n noHBAckTx=%d\n noHBAckRx=%d\n",\
              msg->sctpGenCounters.noDAckRx, msg->sctpGenCounters.noShDwnCmpltTx, msg->sctpGenCounters.noShDwnCmpltRx, msg->sctpGenCounters.noHBeatTx, msg->sctpGenCounters.noHBeatRx, msg->sctpGenCounters.noHBAckTx, msg->sctpGenCounters.noHBAckRx);

  logMsgParams( " noAbortTx=%d\n noAbortRx=%d\n noErrorTx=%d\n noErrorRx=%d\n noEcneTx=%d\n noEcneReTx=%d\n",\
              msg->sctpGenCounters.noAbortTx, msg->sctpGenCounters.noAbortRx, msg->sctpGenCounters.noErrorTx, msg->sctpGenCounters.noErrorRx, msg->sctpGenCounters.noEcneTx, msg->sctpGenCounters.noEcneReTx);

  logMsgParams( " noEcneRx=%d\n noCwrTx=%d\n noCwrRx=%d\n noQueryTx=%d\n noQueryReTx=%d\n noQueryRspRx=%d\n",\
              msg->sctpGenCounters.noEcneRx, msg->sctpGenCounters.noCwrTx, msg->sctpGenCounters.noCwrRx, msg->sctpGenCounters.noQueryTx, msg->sctpGenCounters.noQueryReTx, msg->sctpGenCounters.noQueryRspRx);

  logMsgParams("} Total length = %d\n", len );
}
