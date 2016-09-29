/**
 * @file    imc_msg_desc.c
 * @brief   This file contains description of messages in BS.
 *          This description is mandatory; otherwise IMC would not work.
 *          So, whenever a new message is defined, this file should be updated.
 *
 * @details Copyright PowerWave Technologies, Inc., 2009
 *
 * Author, Date, Description:
 *
 */

#include "imc_msg_desc.h"

MsgDesc c_msgDescTable[MSG_ID_MAX];

void initElementInMsgDescTable(
    MsgId         msgId,
    ModuleId      src,   /* sender */
    ModuleId      dest,  /* receiver */
    UINT16        len,
    UINT16        maxInstances,
    PrintFunc     printFunc,
    MsgTraceLevel msgTraceLevel,
    MsgTraceDirection msgTraceDirection
    )
{
  MsgDesc  msgDesc;

  msgDesc.src          = src;
  msgDesc.dest         = dest;
  msgDesc.len          = len;
  msgDesc.maxInstances = maxInstances;
  msgDesc.printFunc    = printFunc;
  msgDesc.msgTraceLevel= msgTraceLevel;
  msgDesc.msgTraceDirection = msgTraceDirection;

  if( msgId > MSG_ID_MAX )
  {
    BS_ERROR_UNKNOWN_MSG(msgId);
    return;
  }

  c_msgDescTable[msgId] = msgDesc;
}

/**
* @param msgId  The message identifier
* @details Description
* This function returns the message description of a given message identifier
*/
MsgDesc* getMsgDesc(
  IN MsgId     msgId
)
{
  /* One of the purposes of this function is to
     hide c_msgDescTable.
     Do not use c_msgDescTable in other places,
     unless there is no other way
   */
  if( ( msgId EQ MSG_UNDEFINED ) || ( msgId > MSG_ID_MAX ) )
  {
    /* Reaching here means msgId is not described in msgDescTable */
    BS_ERROR_UNKNOWN_MSG(msgId);
    return NULL;
  }

  return &c_msgDescTable[msgId];
}

/**
* @param msgId  The message to which these trace levels are applied
* @param msgTraceLevel  What level of tracing we want for this message
* @param msgTraceDirection  Location of trace, whether incoming or outgoing or both
* @details Description
* This function Sets the trace level of the message with the given parameters
* Validity checks for these values are done at BSM, no need to check again
*/
void setMsgTraceConfig(
  IN MsgId         msgId,
  IN MsgTraceLevel msgTraceLevel,
  IN MsgTraceDirection msgTraceDirection
  )
{
  c_msgDescTable[msgId].msgTraceLevel     = msgTraceLevel;
  c_msgDescTable[msgId].msgTraceDirection = msgTraceDirection;
}
