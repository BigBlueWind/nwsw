/**
 * @file    pmh_rlc_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between PMH and RLC
 * @author : Ankur Agrawal
 * Date   : 07-Mar-2011
 * Change History:
 *
 */

#include "pmh_rlc_msg_print.h"

void printMsgPmhSmRlcGenStatReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhSmRlcGenStatReq *msg = (MsgPmhSmRlcGenStatReq*)data;
  logMsgParams( "{\n transId=%d\n resetAllCounters=%d\n",msg->transId, msg->resetAllCounters);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgSmPmhRlcGenStatRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgSmPmhRlcGenStatRsp *msg = (MsgSmPmhRlcGenStatRsp*)data;
  logMsgParams( "{\n transId=%d\n numUeCfg=%d\n pdusRecv=%d\n pdusSent=%d\n pdusRetx=%d\n bytesRecv=%d\n bytesSent=%d\n unexpPdusRecv=%d\n errorPdusRecv=%d\n protTimeOut=%d\n numOfRb=%d\n numSduDisc=%d\n", \
             msg->transId, msg->rlcGenCounters.numUeCfg, msg->rlcGenCounters.pdusRecv, msg->rlcGenCounters.pdusSent, msg->rlcGenCounters.pdusRetx, msg->rlcGenCounters.bytesRecv,\
             msg->rlcGenCounters.bytesSent, msg->rlcGenCounters.unexpPdusRecv, msg->rlcGenCounters.errorPdusRecv, msg->rlcGenCounters.protTimeOut, msg->rlcGenCounters.numOfRb, msg->rlcGenCounters.numSduDisc);
  logMsgParams("} Total length = %d\n", len );
}
