/**
  * @file    bsm_udh_msg_print.c
  * @details Copyright Powerwave Technologies, Inc., 2010
  * @brief This file contains the print functions for the messages that are exchanged
  *          between BSM and UDH
  * @author :  Ankur Agrawal
  * Date   :24-May-2011
  * Change History:
  *
  */

#include "bsm_udh_msg_print.h"

void printMsgBsmUdhModStateReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmUdhModStateReq *msg = (MsgBsmUdhModStateReq*)data;

  logMsgParams("%s","{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&msg->cmdRef);
  printTlvTagInfo(&msg->tagInfo);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgUdhBsmModStateRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgUdhBsmModStateRsp *msg = (MsgUdhBsmModStateRsp*)data;

  logMsgParams("%s","{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&msg->cmdRef);
  printTlvTagInfo(&msg->tagInfo);
  logMsgParams(" numErabs=%d\n aggregateDLThroughputKbps=%d\n aggregateULThrouhputKbps=%d\n",
                 msg->numErabs, msg->aggregateDLThroughputKbps, msg->aggregateULThroughputKbps );
  logMsgParams("} Total length = %d\n", len );
}
void printMsgBsmUdhCfgReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmUdhCfgReq *msg = (MsgBsmUdhCfgReq*)data;

  logMsgParams( "{\n transId=%d \n ueInactivityCheckIntervalSecs=%d \n startRnti=%d \n maxConnectedUes=%d\n ulInactivityIntervalInSecs=%d\n ueThroughtputTimerInSecs=%d",
                 msg->transId, msg->ueInactivityCheckIntervalSecs,msg->startRnti,msg->maxConnectedUes, msg->ulInactivityIntervalInSecs, msg->ueThroughtputTimerInSecs);
  logMsgParams( "\n} Total length :%d \n", len );
}

void printMsgActiveRabStatusReq(
  IO void    *data,
  IO UINT16  len
  )
{
  MsgBsmUdhActiveRabStatusReq *msg = (MsgBsmUdhActiveRabStatusReq*)data;
  logMsgParams("%s","{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&msg->cmdRef);
  logMsgParams(" msg->internalUeIdentity=%d\n", msg->internalUeIdentity );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgActiveRabStatusRsp(
  IO void    *data,
  IO UINT16  len
  )
{
  UINT16    numElem = 0;
  MsgUdhBsmActiveRabStatusRsp *msg = (MsgUdhBsmActiveRabStatusRsp*)data;
  logMsgParams("%s","{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&msg->cmdRef);
  logMsgParams(" numElem=%d\n", msg->numElem );
  for ( numElem=0; numElem < msg->numElem; numElem++ )
  {
    logMsgParams("Rab No: %d\n", numElem);
    logMsgParams(" index=%d\n internalUeId=%d\n rabIdentity=%d\n drbIdentity=%d\n rabQci=%d\n"
    " rabSetupTime=%s\n dlGtpTunnelId=%d\n ulGtpTunnelId=%d\n gtpKiloBytesDL=%d\n gtpKiloBytesUL=%d\n",
    msg->activeRabStatus[numElem].index, msg->activeRabStatus[numElem].internalUeId,
    msg->activeRabStatus[numElem].rabIdentity, msg->activeRabStatus[numElem].drbIdentity,
    msg->activeRabStatus[numElem].rabQci, msg->activeRabStatus[numElem].rabSetupTime,
    msg->activeRabStatus[numElem].dlGtpTunnelId, msg->activeRabStatus[numElem].ulGtpTunnelId,
    msg->activeRabStatus[numElem].gtpKiloBytesDL, msg->activeRabStatus[numElem].gtpKiloBytesUL);
  }
  logMsgParams("} Total length = %d\n", len );
}


void printMsgBsmUdhRabHistoryReq(
  IO void    *data,
  IO UINT16  len
  )
{
  MsgBsmUdhRabHistoryReq *msg = (MsgBsmUdhRabHistoryReq*)data;
  logMsgParams("%s","{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&msg->cmdRef);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgUdhBsmRabHistoryRsp(
  IO void    *data,
  IO UINT16  len
  )
{
  UINT16    i = 0;
  MsgUdhBsmRabHistoryRsp *msg = (MsgUdhBsmRabHistoryRsp*)data;
  logMsgParams("%s","{\n");
  /* Logs Command Reference */
  printTlvCmdRef(&msg->cmdRef);
  logMsgParams(" numElem=%d\n", msg->numElem );
  for ( i=0; i < msg->numElem; i++ )
  {
    logMsgParams("\nRow No: %d\n", i+1);

    logMsgParams(" index=%d\n internalUeId=%d\n rabIdentity=%d\n drbIdentity=%d\n rabQci=%d\n"
    " rabSetupTime=%s\n rabReleaseTime=%s\n rabReleaseCause=%d\n dlGtpTunnelId=%d\n ulGtpTunnelId=%d\n gtpKiloBytesDL=%d\n gtpKiloBytesUL=%d\n",
    msg->rabHistory[i].index, msg->rabHistory[i].internalUeId,
    msg->rabHistory[i].rabIdentity, msg->rabHistory[i].drbIdentity,
    msg->rabHistory[i].rabQci, msg->rabHistory[i].rabSetupTime,
    msg->rabHistory[i].rabReleaseTime, msg->rabHistory[i].rabReleaseCause,
    msg->rabHistory[i].dlGtpTunnelId, msg->rabHistory[i].ulGtpTunnelId,
    msg->rabHistory[i].gtpKiloBytesDL, msg->rabHistory[i].gtpKiloBytesUL);
  }
  logMsgParams("} Total length = %d\n", len );
}
