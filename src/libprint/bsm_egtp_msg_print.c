/**
  * @file    bsm_egtp_msg_print.c
  * @details Copyright Powerwave Technologies, Inc., 2010
  * @brief This file contains the print functions for the
  *          messages that are exchanged between BSM and EGTP
  * @author :  Ramprasad Reddy Chinta
  * Date   :
  * Change History:
  *
  */

 #include "bsm_egtp_msg_print.h"

void printMsgBsmEgtpInitConfigReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmEgtpInitConfigReq *msg =  (MsgBsmEgtpInitConfigReq*)data;
  logMsgParams("%s","{\n");
  logMsgParams( "enodebIpAddr=%s\ntransId=%d\n",msg->enodebIpAddr, msg->transId );
  logMsgParams( "} Total length :%d \n", len );
}
