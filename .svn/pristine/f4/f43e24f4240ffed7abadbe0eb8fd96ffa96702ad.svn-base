/**
 * @file    pmh_pdcp_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between PMH and PDCP
 * @author : Ankur Agrawal
 * Date   : 10-Mar-2011
 * Change History:
 *
 */
#include "pmh_pdcp_msg_print.h"

void printMsgPmhSmPdcpGenStatReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhSmPdcpGenStatReq *msg = (MsgPmhSmPdcpGenStatReq*)data;
  logMsgParams( "{\n transId=%d\n resetAllCounters=%d\n",msg->transId, msg->resetAllCounters);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgPmhSmPdcpUplaneStatReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhSmPdcpUplaneStatReq *msg = (MsgPmhSmPdcpUplaneStatReq*)data;
  logMsgParams( "{\n transId=%d\n resetAllCounters=%d\n",msg->transId, msg->resetAllCounters);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgSmPmhPdcpGenStatRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgSmPmhPdcpGenStatRsp *msg = (MsgSmPmhPdcpGenStatRsp*)data;
  logMsgParams( "{\n transId=%d\n rxPdus=%d\n txPdus=%d\n errorPdusRecv=%d\n numSdusDisc=%d\n numCmpFails=%d\n",\
              msg->transId, msg->pdcpGenCounters.rxPdus, msg->pdcpGenCounters.txPdus, msg->pdcpGenCounters.errorPdusRecv, msg->pdcpGenCounters.numSdusDisc,  msg->pdcpGenCounters.numCmpFails);
  logMsgParams( " numDecmpFails=%d\n numIntgPrtFails=%d\n numIntgVrfFails=%d\n numCiphFails=%d\n numDeciphFails=%d\n",\
              msg->pdcpGenCounters.numDecmpFails, msg->pdcpGenCounters.numIntgPrtFails, msg->pdcpGenCounters.numIntgVrfFails, msg->pdcpGenCounters.numCiphFails,  msg->pdcpGenCounters.numDeciphFails);
  logMsgParams( " numPdusDiscObdTmrExp=%d\n numSdusDiscObdTmrExp=%d\n numPktsFrwd=%d\n numPktsRcvd=%d\n",\
              msg->pdcpGenCounters.numPdusDiscObdTmrExp, msg->pdcpGenCounters.numSdusDiscObdTmrExp, msg->pdcpGenCounters.numPktsFrwd, msg->pdcpGenCounters.numPktsRcvd);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgSmPmhPdcpUplaneStatRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgSmPmhPdcpUplaneStatRsp *msg = (MsgSmPmhPdcpUplaneStatRsp*)data;
  logMsgParams( "{\n transId=%d\n rxSdus=%d\n txSdus=%d\n ",\
              msg->transId, msg->pdcpUplaneCounters.rxSdus, msg->pdcpUplaneCounters.txSdus );
  logMsgParams("} Total length = %d\n", len );
}
