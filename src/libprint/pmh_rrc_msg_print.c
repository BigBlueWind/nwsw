/**
 * @file    pmh_rrc_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between PMH and RRC
 * @author : Ankur Agrawal
 * Date   : 17-Mar-2011
 * Change History:
 *
 */
#include "pmh_rrc_msg_print.h"

void printMsgPmhSmRrcGenStatReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhSmRrcGenStatReq *msg = (MsgPmhSmRrcGenStatReq*)data;
  logMsgParams( "{\n transId=%d\n resetAllCounters=%d\n",msg->transId, msg->resetAllCounters);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgSmPmhRrcGenStatRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgSmPmhRrcGenStatRsp *msg = (MsgSmPmhRrcGenStatRsp*)data;
  logMsgParams( "{\n transId=%d\n numEstbCon=%d\n numFailCon=%d\n numReConfigSucc=%d\n numReConfigFail=%d\n numUnExptMsg=%d\n",\
              msg->transId, msg->rrcGenCounters.numEstbCon,msg->rrcGenCounters.numFailCon, msg->rrcGenCounters.numReConfigSucc, msg->rrcGenCounters.numReConfigFail, msg->rrcGenCounters.numUnExptMsg);


  logMsgParams( " numLwLyFail=%d\n numDecErr=%d\n numEncErr=%d\n numPdcpErr=%d\n numRlcErr=%d\n",\
              msg->rrcGenCounters.numLwLyFail, msg->rrcGenCounters.numDecErr, msg->rrcGenCounters.numEncErr, msg->rrcGenCounters.numPdcpErr, msg->rrcGenCounters.numRlcErr);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgPmhSmRrcCellStatReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhSmRrcCellStatReq  *req = (MsgPmhSmRrcCellStatReq*)data;
  logMsgParams( "{\n transId=%d\n resetAllCounters=%d\n cellId=%d\n", req->transId, req->resetAllCounters, req->cellId);
  logMsgParams( "} Total length = %d\n", len );
}

void printMsgRrcPmhCellStatRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgSmPmhRrcCellStatRsp *msg = (MsgSmPmhRrcCellStatRsp*)data;
  logMsgParams( "{\n transId=%d\n cellId=%d\n pag=%d\n connSetup=%d\n connReject=%d\n connReq=%d\n connReestabReq=%d\n",\
              msg->transId, msg->rrcCellCounters.cellId,msg->rrcCellCounters.pag, msg->rrcCellCounters.connSetup, msg->rrcCellCounters.connReject, msg->rrcCellCounters.connReq, msg->rrcCellCounters.connReestabReq);


  logMsgParams( " connReestab=%d\n connReestabRej=%d\n numUEs=%d\n hoToEUTRASucc=%d\n hoFromEUTRAFail=%d\n",\
              msg->rrcCellCounters.connReestab, msg->rrcCellCounters.connReestabRej, msg->rrcCellCounters.numUEs, msg->rrcCellCounters.hoToEUTRASucc, msg->rrcCellCounters.hoFromEUTRAFail);
  logMsgParams("} Total length = %d\n", len );
}
