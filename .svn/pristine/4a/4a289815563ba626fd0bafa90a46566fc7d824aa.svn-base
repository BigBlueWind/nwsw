/**
 * @file    bsm_sih_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between BSM and SIH
 * @author :  Shweta Polepally
 * Date   :
 * Change History:
 *
 */

#include "bsm_sih_msg_print.h"

void printMsgSihConfigReq(
  IO  void    *data,
  IO  UINT16  len
  )
{
  MsgBsmSihInitConfigReq *msg =  (MsgBsmSihInitConfigReq*)data;
  logMsgParams("%s","{\n");
  logMsgParams( " emsAddr=%s\n snmpCommunityString=%s\n",msg->emsAddr,msg->snmpCommunityString );
  logMsgParams("} Total length = %d\n", len );
}
