/**
 * @file    pmh_mac_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between PMH and MAC
 * @author : Ankur Agrawal
 * Date   : 09-Mar-2011
 * Change History:
 *
 */
#include "pmh_mac_msg_print.h"

void printMsgPmhSmMacGenStatReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhSmMacGenStatReq *msg = (MsgPmhSmMacGenStatReq*)data;
  logMsgParams( "{\n transId=%d\n resetAllCounters=%d\n",msg->transId, msg->resetAllCounters );

  logMsgParams("} Total length = %d\n", len );
}

void printMsgPmhSmMacRlcsapStatReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhSmMacRlcsapStatReq *msg = (MsgPmhSmMacRlcsapStatReq*)data;
  logMsgParams( "{\n transId=%d\n resetAllCounters=%d\n",msg->transId, msg->resetAllCounters );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgPmhSmMacPhysapStatReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhSmMacPhysapStatReq *msg = (MsgPmhSmMacPhysapStatReq*)data;
  logMsgParams( "{\n transId=%d\n resetAllCounters=%d\n",msg->transId, msg->resetAllCounters );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgSmPmhMacGenStatRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgSmPmhMacGenStatRsp *msg = (MsgSmPmhMacGenStatRsp*)data;
  logMsgParams( "{\n transId=%d\n numHarqFail=%d\n numUeCfg=%d\n numCellCfg=%d\n",\
              msg->transId, msg->macGenCounters.numHarqFail,msg->macGenCounters.numUeCfg, msg->macGenCounters.numCellCfg);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgSmPmhMacRlcsapStatRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgSmPmhMacRlcsapStatRsp *msg = (MsgSmPmhMacRlcsapStatRsp*)data;
  logMsgParams( "{\n transId=%d\n numOfSDUTxRlcMacIf=%d\n numOfSDURxRlcMacIf=%d\n numSDUDropRlcMacIf=%d\n",\
              msg->transId, msg->macRlcsapCounters.numOfSDUTxRlcMacIf,msg->macRlcsapCounters.numOfSDURxRlcMacIf, msg->macRlcsapCounters.numSDUDropRlcMacIf);
  logMsgParams("} Total length = %d\n", len );
}

void printMsgSmPmhMacPhysapStatRsp(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgSmPmhMacPhysapStatRsp *msg = (MsgSmPmhMacPhysapStatRsp*)data;
  logMsgParams( "{\n transId=%d\n numOfPduTxL2L1If=%d\n numOfPduRxL2L1If=%d\n numPduDropL2L1If=%d\n",\
              msg->transId, msg->macPhysapCounters.numOfPduTxL2L1If, msg->macPhysapCounters.numOfPduRxL2L1If, msg->macPhysapCounters.numPduDropL2L1If);
  logMsgParams("} Total length = %d\n", len );
}
