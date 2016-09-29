/**
 * @file    pmh_ueh_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between PMH and UEH
 * @author : Ankur Agrawal
 * Date   : 02-Aug-2011
 * Change History:
 *
 */
#include "pmh_ueh_msg_print.h"

void printMsgPmhUehRrmStatReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhUehRrmStatReq *msg = (MsgPmhUehRrmStatReq*)data;
  logMsgParams( "{\n transId=%d\n resetAllCounters=%d\n",msg->transId, msg->resetAllCounters );
  logMsgParams("} Total length = %d\n", len );
}


void printMsgUehPmhRrmStatRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgUehPmhRrmStatRsp *msg = (MsgUehPmhRrmStatRsp*)data;
  logMsgParams( "{\n transId=%d\n numRlFailures=%d\n numS1setupReq=%d\n numIncomingHoSuccessful=%d\n",\
			  msg->transId, msg->rrmCounters.numRlFailures, msg->rrmCounters.nS1SetupReq, msg->rrmCounters.numIncomingHoSuccessful);
  logMsgParams( " numIncomingHoFailed=%d\n numIncomingHoRejected=%d\n numS1apRejected=%d\n",\
			  msg->rrmCounters.numIncomingHoFailed, msg->rrmCounters.numIncomingHoRejected, msg->rrmCounters.numS1apRejected);
  logMsgParams("} Total length = %d\n", len );

}

void printMsgPmhUehCounterStartReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  UINT32 i;

  MsgPmhUehCounterStartReq *msg = (MsgPmhUehCounterStartReq*)data;
  logMsgParams( "{\n transId=%u\n profileId=%u\n numOfSamples=%u\n samplingInterval=%u\n numOfCounters=%u\n",
      msg->transId, msg->profileId ,msg->numOfSamples ,msg->samplingInterval ,msg->numOfCounters);

  for (i=0;i<msg->numOfCounters;i++)
    logMsgParams(" %u ", msg->counterId[i] );

  logMsgParams("\n } Total length = %u\n", len );
}

void printMsgUehPmhCounterStartSucc(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgUehPmhCounterStartSucc *msg = (MsgUehPmhCounterStartSucc*)data;
  logMsgParams( "{\n transId=%u\n profileId=%u\n",
      msg->transId, msg->profileId);

  logMsgParams("\n } Total length = %u\n", len );
}

void printMsgUehPmhCounterStartFail(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgUehPmhCounterStartFail *msg = (MsgUehPmhCounterStartFail*)data;
  logMsgParams( "{\n transId=%u\n profileId=%u\n  errorNum=%s\n  failReason=%s\n",
      msg->transId, msg->profileId, errorCodeStr(msg->errorDesc.errorNum),msg->errorDesc.failReason);

  logMsgParams("\n } Total length = %u\n", len );
}

void printMsgPmhUehCounterStopReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhUehCounterStopReq *msg = (MsgPmhUehCounterStopReq*)data;
  logMsgParams( "{\n transId=%u\n profileId=%u\n cause=%d",
      msg->transId, msg->profileId, msg->cause);

  logMsgParams("\n } Total length = %u\n", len );
}

void printMsgUehPmhCounterStopSucc(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgUehPmhCounterStopSucc *msg = (MsgUehPmhCounterStopSucc*)data;
  logMsgParams( "{\n transId=%u\n profileId=%u\n cause=%d",
      msg->transId, msg->profileId, msg->cause);

  logMsgParams("\n } Total length = %u\n", len );
}

void printMsgUehPmhCounterStopInd(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgUehPmhCounterStopInd *msg = (MsgUehPmhCounterStopInd*)data;
  logMsgParams( "{\n measurementStopCause=%d\n profileId=%u\n",
      msg->measurementStopCause, msg->profileId);

  logMsgParams("\n } Total length = %u\n", len );
}

void printMsgUehPmhCounterReportInd(
  IO  void    *data,
  IO  UINT16  len
  )
{
  UINT32 i =0;
  MsgUehPmhCounterReportInd *msg = (MsgUehPmhCounterReportInd*)data;
  logMsgParams( "{\n sampleSeqNum=%u\n profileId=%u\n numOfCounters=%u\n collectedAt=%s\n",
      msg->sampleSeqNum, msg->profileId, msg->numOfCounters, msg->collectedAt);

  logMsgParams("\n%s\n","CounterId    Value");
  for (i=0; i< msg->numOfCounters; i++)
  {
    logMsgParams("    %u         %u\n",msg->counterVal[i].counterId, msg->counterVal[i].val);
  }

  logMsgParams("\n } Total length = %u\n", len );
}
