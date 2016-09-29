/*******************************************************************************
* Copyright: (C) 2009 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#ifndef __PN_MESSAGE_QUEUE_H__
#define __PN_MESSAGE_QUEUE_H__

#ifdef LINUX
#include <netinet/in.h>
#include <mqueue.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#else
#include <winsock2.h>
#include <windows.h>
#define __func__ __FUNCTION__
#endif

#include "PW_PALProtocolStackShared.h"

#ifdef WIN32
#define PN_MESSAGEQUEUE_API __declspec( dllexport )
#else
#define PN_MESSAGEQUEUE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined (PN_USE_MSGQ) || defined (PN_USE_FIFO)
    PN_MESSAGEQUEUE_API int UTILCreateMessageQueue(const char* kpcFileName);
    PN_MESSAGEQUEUE_API int UTILOpenMessageQueueWriter(const char* kpcFileName);
    PN_MESSAGEQUEUE_API int UTILDeleteMessageQueue(const char* kpcFileName);
#elif (defined PN_USE_PIPE)
    PN_MESSAGEQUEUE_API int UTILCreateMessageQueue(const unsigned int kuiMsgQIndex);
    PN_MESSAGEQUEUE_API int UTILOpenMessageQueueWriter(const unsigned int kuiMsgQIndex);
    PN_MESSAGEQUEUE_API int UTILDeleteMessageQueue(const unsigned int kuiMsQIndex);
#elif (defined PN_USE_SOCKET)
    PN_MESSAGEQUEUE_API int UTILCreateMessageQueue(const unsigned int kuiServerPort);
    PN_MESSAGEQUEUE_API int UTILOpenMessageQueueWriter(const unsigned int kuiServerPort);
    PN_MESSAGEQUEUE_API int UTILDeleteMessageQueue(const unsigned int kuiServerPort);
#else
#error "Define IPC to be used for inter-thread messaging"
#endif

    PN_MESSAGEQUEUE_API int UTILCloseMessageQueue(int iMessageQueueId);
    PN_MESSAGEQUEUE_API int UTILAddMessageInMessageQueue(int    iMessageQueueId,
        ubyte*  pucMessage, 
        int    iMessageLen,
        uint   uiMessagePriority);

    PN_MESSAGEQUEUE_API int UTILRetrieveMessageFromMessageQueue(int     iMessageQueueId,
        ubyte*  pucMessage, 
        int     iMessageLen,
        uint*   puiMessagePriority);
    PN_MESSAGEQUEUE_API int UTILRetrieveMessageFromMessageQueueTimeOut(int     iMessageQueueId,
                                               ubyte*  pucMessage,
                                               int     iMessageLen,
                                               uint*   puiMessagePriority,
                                               int     iTimeoutVal);

#ifdef __cplusplus
}
#endif

#endif /* __PN_MESSAGE_QUEUE_H__ */
