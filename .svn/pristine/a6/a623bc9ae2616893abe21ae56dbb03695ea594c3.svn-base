/** @file ctrlmsg.h
 *
 * @brief Control messages definitions and interfaces
 * @author Mindspeed Technologies
 * @version $Revision: 1.9 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef CTRLMSG_H_
#define CTRLMSG_H_

#include "osal.h"
#include "basetypes.h"
#include "resultcodes.h"
#include "apidefs.h"

//
// Used in command construction
//
#define PAR_TYPE_U8             0
#define PAR_TYPE_U16            1
#define PAR_TYPE_U32            2
#define PAR_TYPE_STRING         3
#define PAR_TYPE_BINARY         4
#define PAR_TYPE_BYPASS         5
#define PAR_TYPE_TLV            6

#if defined (C4KEVM)

typedef struct _Msg_ICpu_Stat_
{
    U32         RxMsgNum;   // The number of received messages from L-ARM
    U32         TxMsgNum;   // The number of sent messages to L-ARM
    MSGHEADER   RxApi;      // The latest API command received from L-ARM
    MSGHEADER   TxApi;      // The latest API command sent to L-ARM

}TMsgICpuStat;

#endif

// Message construction
RESULTCODE MsgSimpleReply(PMSGHEADER srcMsg, U32 resultCode);
RESULTCODE MsgCommand(U16 srcID, U16 dstID, U16 msgID, U32 paramCount, ...);
RESULTCODE MsgIndication(U16 srcID, U16 dstID, U16 msgID, U32 paramCount, ...);

// Init new message fields
void MsgInitMessage(PMSGHEADER msg);
PMSGHEADER MsgInitReply(PMSGHEADER pSrc);

// Encapsulation support
U32 MsgGetHdrSize(void);
PTR MsgGetDataOffset(PTR msg);
PTR MsgGetDataOffsetList(PTR msg);

// Parameter manipulation
RESULTCODE MsgAppendParam(PMSGHEADER msg, U16 paramID, U16 paramLen, PTR paramData);
RESULTCODE MsgAppendU8(PMSGHEADER msg, U16 paramID, U8 paramData);
RESULTCODE MsgAppendU16(PMSGHEADER msg, U16 paramID, U16 paramData);
RESULTCODE MsgAppendU32(PMSGHEADER msg, U16 paramID, U32 paramData);

// Fast way to report result of process
RESULTCODE MsgResult(PMSGHEADER msg, U32 result);
RESULTCODE MsgReply(PMSGHEADER srcMsg, U32 result, U32 paramCount, ...);

// Get next parameter from message
PPARAMHEADER MsgGetNextParam(PMSGHEADER msg, PPARAMHEADER param);
// Get parameter by ID
PPARAMHEADER MsgGetParam(PMSGHEADER msg, U16 paramID);
RESULTCODE MsgGetU8(PMSGHEADER msg, U16 paramID, U8 *value);
RESULTCODE MsgGetU16(PMSGHEADER msg, U16 paramID, U16 *value);
RESULTCODE MsgGetU32(PMSGHEADER msg, U16 paramID, U32 *value);
RESULTCODE MsgGetParamVal(PMSGHEADER msg, U16 paramID, PTR *value, U32 *len);
// Get parameter by ID with default value if missed
U8 MsgGetU8Dflt(PMSGHEADER msg, U16 paramID, U8 dflt);
U16 MsgGetU16Dflt(PMSGHEADER msg, U16 paramID, U16 dflt);
U32 MsgGetU32Dflt(PMSGHEADER msg, U16 paramID, U32 dflt);
PTR MsgGetParamDflt(PMSGHEADER msg, U16 paramID, PTR dflt, U32 *len);

typedef EXPORT void       (*MsgFreeBufferCb)(PMSGHEADER pMsg);
typedef EXPORT PMSGHEADER (*MsgAllocBufferCb)(int flags);
extern MsgFreeBufferCb      MsgFreeBuffer;
extern MsgAllocBufferCb     MsgAllocBuffer;

EXPORT void MsgInitFreeBuffer(PMSGHEADER pMsg);
EXPORT void Msg1FreeBuffer(PMSGHEADER pMsg);
EXPORT void Msg2FreeBuffer(PMSGHEADER pMsg);

EXPORT PMSGHEADER MsgInitAllocBuffer(int flags);
EXPORT PMSGHEADER Msg1AllocBuffer(int flags);
EXPORT PMSGHEADER Msg2AllocBuffer(int flags);

#endif /* CTRLMSG_H_ */
