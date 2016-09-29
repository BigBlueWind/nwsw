/**
 * @file    imc_msg_desc.h
 * @brief Description of all messages in the system
 * @details Copyright Powerwave Technologies, Inc., 2009
 *
 * @author : Lakshmi Narayana Madala
 * Date   :
 * Description :
 */

#ifndef IMC_MSG_DESC_H
#define IMC_MSG_DESC_H

#include "all_cmn.h"

/** MsgDesc:
 *  This type is used to describe messages.
 *  Description of a message includes:
 *  a) msgId: this is a unique identifier for each message in the system.
 *  b) src  : the module the created this message.
 *  c) dest : the module to which this message was sent.
 *  d) len  : total number of "data" bytes in the message i.e. payload size.
 *  e) maxInstances: the maximum number of instances of this message that should be created at run time.
 *       This field is a threshold.
 *       if (createMsg(someMsgId) is called and
 *           instances for someMsgId >=maxInstances), then
 *       1. A new message will be created but
 *       2. A warning will be logged
 *       This feature is a way to:
 *       a) detect memory leaks and
 *       b) determine the actual number of message instances we need at run time.
 *  f) printFunc: A function to print the message.
*/
typedef void (*PrintFunc)(void* val, UINT16 len);

typedef struct s_MsgDesc
{
  ModuleId      src;   /* sender */
  ModuleId      dest;  /* receiver */
  UINT16        len;
  UINT16        maxInstances;
  PrintFunc     printFunc;
  MsgTraceLevel msgTraceLevel;
  MsgTraceDirection msgTraceDirection;
} MsgDesc;

MsgDesc* getMsgDesc(
  IN MsgId msgId
);

void initElementInMsgDescTable(
    MsgId         msgId,
    ModuleId      src,   /* sender */
    ModuleId      dest,  /* receiver */
    UINT16        len,
    UINT16        maxInstances,
    PrintFunc     printFunc,
    MsgTraceLevel msgTraceLevel,
    MsgTraceDirection msgTraceDirection
    );
void setMsgTraceConfig(
    IN MsgId         msgId,
    IN MsgTraceLevel msgTraceLevel,
    IN MsgTraceDirection msgTraceDirection
    );

#endif /* IMC_MSG_DESC_H */

