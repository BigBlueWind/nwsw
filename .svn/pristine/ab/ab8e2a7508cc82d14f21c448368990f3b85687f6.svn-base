/**
 * @file    prm_msg_trace.c
 * @brief This file contains handling of Message trace
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * @author        : Shweta Polepally
 * Date          : Nov 2010
 * Change History:
 *
 */

#include "prm.h"

void prmProcSetMsgTraceInAllStates(
  IN Msg           *msg,
  IO PrmContext    *prmContext
  )
{
  UNUSED(prmContext);
  MsgBsmPrmSetMsgTrace  *msgTraceMsg    = (MsgTrace*)msg->data;
  MsgDesc   *msgDesc    = getMsgDesc(msgTraceMsg->msgId);

  /* Set the values in the table */
  msgDesc->msgTraceLevel     = msgTraceMsg->msgTraceLevel;
  msgDesc->msgTraceDirection = msgTraceMsg->msgTraceDirection;
}
