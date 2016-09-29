/**
  * @file    bsm_sctp_msg_print.c
  * @details Copyright Powerwave Technologies, Inc., 2010
  * @brief This file contains the print functions for the
  *          messages that are exchanged between BSM and SCTP
  * @author :  Ramprasad Reddy Chinta
  * Date   :
  * Change History:
  *
  */

#include "bsm_sctp_msg_print.h"

void printMsgBsmSctpInitConfigReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmSctpInitConfigReq *msg =  (MsgBsmSctpInitConfigReq*)data;
  logMsgParams("%s","{\n");
  logMsgParams( "enodebIpAddr=%s\ntransId=%d\n",msg->enodebIpAddr, msg->transId );
  logMsgParams("} Total length = %d\n", len );
}
