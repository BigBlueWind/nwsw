/** @file hostiface.h
 *
 * @brief Host interface messages exchange
 * @author Mindspeed Technologies
 * @version $Revision: 1.11 $
 *
 * COPYRIGHT(c) 2008,2009,2010 Mindspeed Technologies.
 * ALL RIGHTS RESERVED
 *
 * This is Unpublished Proprietary Source Code of Mindspeed Technologies
 */

#ifndef TRANSPORT_H_
#define TRANSPORT_H_

#include "resultcodes.h"

#define PROTO_TCP                   6
#define PROTO_UDP                   22
#define PROTO_SCTP                  132

RESULTCODE ProtoInit(U32 protoid);
RESULTCODE ProtoDone(void);

RESULTCODE SctpSend(U32 size, PTR data);
RESULTCODE TcpSend(U32 size, PTR data);
RESULTCODE UdpSend(U32 size, PTR data);

extern RESULTCODE (*ProtoSend)(U32 size, PTR data);

RESULTCODE UdpProtoInit(void);
RESULTCODE UdpProtoDone(void);
void UdpRxRaiseSema(void);
void UdpFrameFree(PTR Packet);
typedef RESULTCODE (*UdpSendMsgCB)(U32 msgSize, PTR data);
#endif /* TRANSPORT_H_ */
