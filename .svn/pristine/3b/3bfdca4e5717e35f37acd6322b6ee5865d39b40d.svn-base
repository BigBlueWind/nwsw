/**
 * @file    bsm_lth_msg_print.c
 * @details Copyright Powerwave Technologies, Inc., 2010
 * @brief This file contains the print functions for the
 *          messages that are exchanged between BSM and LTH
 * @author :  Ramprasad Reddy Chinta
 * Date   :
 * Change History:
 *
 */

#include "bsm_lth_msg_print.h"

void printMsgBsmLthStartTraceReq(
    void    *data,
    UINT16  len
    )
{
  MsgBsmLthStartTraceReq *msg = (MsgBsmLthStartTraceReq*) data;

  logMsgParams("%s","{\n");
  logMsgParams( "filename=%s\n", msg->fileName );
  logMsgParams("} Total length = %d\n", len );
}

void printMsgLthBsmStartTraceFail(
    void    *data,
    UINT16  len
    )
{
  MsgLthBsmStartTraceFail *msg = (MsgLthBsmStartTraceFail*) data;

  logMsgParams("%s","{\n");
  logMsgParams( "bsErrorValue=%s\n", errorCodeStr(msg->bsErrorValue) );
  logMsgParams("} Total length = %d\n", len );
}

