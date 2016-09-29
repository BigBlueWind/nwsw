/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#pragma once

#include "PW_PALDefinition.h"
#include "MessageQueue.h"

#define PW_PAL_MSG_Q_TIMER            "/pwPALTimerMessageQueue"
#define PW_PAL_MSG_Q_DLAPI            "/pwPALDLAPIMessageQueue"

typedef enum
{
    EPW_MSGQ_TYPE_TIMER,
    EPW_MSGQ_TYPE_DLAPI,
} PW_EMsgQType;

int MQInitialize();
int MQClose();
int MQGetQueueId(PW_EMsgQType eMsgQType);
int MQGetWriteAccess(PW_EMsgQType eMsgQType);
int MQAdd(int iQueueFd, unsigned char* pucMsg, int iMsgLen);
