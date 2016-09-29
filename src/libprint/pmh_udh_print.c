/**
 * @file    pmh_udh_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between PMH and UDH
 * @author : 
 * Date   : 
 * Change History:
 *
 */

#include "pmh_udh_print.h"
#include "ctr_id.h"

void printMsgUdhPmhDStatInd(
  IO  void    *data,
  IO  UINT16  len
  )
{
  UNUSED(data);
  UNUSED(len);
  logMsgParams("Received MSG_UDH_PMH_DSTATS_IND message");
}


void printMsgPmhUdhDStatReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgPmhUdhDStatsCfgReq *msg = (MsgPmhUdhDStatsCfgReq *)data;
  logMsgParams( "{DstatReq-timerValue=%d Cmd=%d TotalLen=%d}",msg->interval_ms, msg->cmd, len);
}
