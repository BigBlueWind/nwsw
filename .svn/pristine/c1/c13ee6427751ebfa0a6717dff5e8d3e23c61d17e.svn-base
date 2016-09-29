/*******************************************************************************
* Copyright: (C) 2009 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "Constants.h"
#include "Logger.h"
#include "MessageQueue.h"

/* Module Name needs to be set for Logging */
#define __sModuleName__ "SLIB"

#define PN_MAX_QMSG_SIZE 512

#if defined (PN_USE_MSGQ) || defined (PN_USE_FIFO)
static int UTILOpenMessageQueue(const char* kpcFileName, int iFlags);
#elif (defined PN_USE_PIPE)
static int UTILOpenMessageQueue(const unsigned int kuiMsgQIndex, int iFlags);
#elif (defined PN_USE_SOCKET)
static int UTILOpenMessageQueue(const unsigned int kuiServerPort, int iFlags);
#else
#error "Define IPC to be used for inter-thread messaging"
#endif

#ifdef PN_USE_MSGQ
int UTILCreateMessageQueue(const char* kpcFileName)
{
#ifdef LINUX
    mqd_t mqd = 0;
    int iError = 0;
    struct mq_attr mqAttributes;
    mqAttributes.mq_maxmsg = 100;
    mqAttributes.mq_msgsize = PN_MAX_QMSG_SIZE;
    mqd = mq_open(kpcFileName, O_RDONLY | O_CREAT | O_NONBLOCK | O_EXCL, S_IRWXU | S_IRWXG, &mqAttributes);
    iError = errno;

    if (mqd == -1)
    {
        LOG_DEVEL( EMQ_CREATE_FAILED, kpcFileName, strerror(iError));
        return PN_FAILURE;
    } 
    else
    {
        LOG_DEBUG( EMQ_CREATE_SUCCESSFUL, kpcFileName);
    }
    return mqd;
#else
    return PN_FAILURE;
#endif
}

#elif (defined PN_USE_FIFO)

int UTILCreateMessageQueue(const char* kpcFifoName)
{
#ifdef LINUX
    int iRet = mkfifo(kpcFifoName, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (iRet != 0)
    {
        LOG_DEVEL( EMQ_CREATE_FAILED, kpcFifoName, strerror(errno));
        return PN_FAILURE;
    } 
    else
    {
        LOG_DEBUG( EMQ_CREATE_SUCCESSFUL, kpcFifoName);
    }

    /* Open the FIFO */
    return UTILOpenMessageQueue(kpcFifoName, O_RDONLY);
#else
    return PN_FAILURE;
#endif
}

#elif (defined PN_USE_PIPE)

int UTILCreateMessageQueue(const unsigned int kuiMsgQIndex)
{
#ifdef LINUX
    return mqd;
#else
    return PN_FAILURE;
#endif
}

#elif (defined PN_USE_SOCKET)

int UTILCreateMessageQueue(const unsigned int kuiServerPort)
{
#ifdef LINUX
    return mqd;
#else
    return PN_FAILURE;
#endif
}

#endif

#if defined (PN_USE_MSGQ) || defined (PN_USE_FIFO)
int UTILOpenMessageQueueWriter(const char* kpcFileName)
{
    int iFlags;
#ifdef WIN32
    iFlags = _O_WRONLY;
#else
    iFlags = O_WRONLY;
#endif

    return UTILOpenMessageQueue(kpcFileName, iFlags);
}
#else
#error "UTILOpenMessageQueueWriter is not supported for IPCS other than FIFO and MSGQ"
#endif

#if (defined PN_USE_MSGQ)
int UTILOpenMessageQueue(const char* kpcFileName, int iFlags)
{
#ifdef LINUX  
    mqd_t mqd = 0;
    int iError = 0;

    struct mq_attr mqAttributes;
    mqAttributes.mq_maxmsg = 100;
    mqAttributes.mq_msgsize = PN_MAX_QMSG_SIZE;
    
    mqd = mq_open(kpcFileName, iFlags, &mqAttributes);
    iError = errno;

    if (mqd == -1)
    {
        LOG_ERROR(EMQ_OPEN_FAILED, kpcFileName, strerror(errno));
        return PN_FAILURE;
    }
    return mqd;
#else
    return PN_FAILURE;
#endif
}
#elif (defined PN_USE_FIFO)
int UTILOpenMessageQueue(const char* kpcFifoName, int iFlags)
{
#ifdef LINUX
    int iRet = open(kpcFifoName, O_NONBLOCK | iFlags);
    if (iRet == -1)
    {
        LOG_ERROR(EMQ_OPEN_FAILED, kpcFifoName, strerror(errno));
        return PN_FAILURE;
    } 
    else
    {
        LOG_DEBUG( EMQ_OPEN_SUCCESSFUL, kpcFifoName, iRet);
    }

    return iRet;
#else
    return PN_FAILURE;
#endif
}
#elif defined (PN_USE_PIPE)
#elif defined (PN_USE_SOCKET)
#endif

#if defined (PN_USE_MSGQ) || defined (PN_USE_FIFO)
int UTILDeleteMessageQueue(const char* kpcFileName)
{
#ifdef LINUX
    int iRetVal;
    int iError = 0;

#if defined (PN_USE_MSGQ)
    iRetVal = mq_unlink(kpcFileName);
#elif defined (PN_USE_FIFO)
    iRetVal = unlink(kpcFileName);
#endif

    iError = errno;

    if (iRetVal == -1)
    {
        LOG_ERROR( EMQ_DELETE_FAILED, kpcFileName);
        return PN_FAILURE;
    }
    else
    {
    }
    return iRetVal;
#else
    return PN_FAILURE;
#endif
}
#else
#define UTILDeleteMessageQueue
#endif

int UTILCloseMessageQueue(int iMessageQueueId)
{
#ifdef LINUX
    int iRetVal;
    int iError = 0;

#if (defined PN_USE_MSGQ)
    iRetVal = mq_close(iMessageQueueId);
#elif defined (PN_USE_FIFO) || defined (PN_USE_PIPE)
    iRetVal = close(iMessageQueueId);
#elif (defined PN_USE_SOCKET)
#else
#endif

    iError = errno;

    if (iRetVal == -1)
    {
        LOG_DEBUG(EMQ_CLOSE_FAILED);
        return PN_FAILURE;
    }

    return iRetVal;
#else
    return PN_FAILURE;
#endif
}

int UTILAddMessageInMessageQueue(int    iMessageQueueId,
                                 ubyte*  kpucMessage, 
                                 int    iMessageLen,
                                 uint   uiMessagePriority)
{
#ifdef LINUX
    int iRetVal = PN_FAILURE;

#if (defined PN_USE_MSGQ)
    iRetVal = mq_send(iMessageQueueId, (const char*)kpucMessage, iMessageLen, uiMessagePriority);
#elif defined (PN_USE_FIFO) || defined (PN_USE_PIPE)
    iRetVal = write(iMessageQueueId, (const char*)kpucMessage, iMessageLen);
#elif defined (PN_USE_SOCKET)
#else
#endif

    if (iRetVal == -1)
    {
        LOG_ERROR(EMQ_SEND_FAILED, strerror(errno));
        return PN_FAILURE;
    }
    else
    {
        LOG_DEVEL(EMQ_SEND_SUCCESS, iMessageLen, iMessageQueueId);
    }

    return iRetVal;
#else
    return PN_FAILURE;
#endif
}

int UTILRetrieveMessageFromMessageQueue(int     iMessageQueueId,
                                        ubyte*  pucMessage, 
                                        int     iMessageLen,
                                        uint*   puiMessagePriority)
{
#ifdef LINUX
    int iRetVal;          

#if (defined PN_USE_MSGQ)
    iRetVal = mq_receive(iMessageQueueId, (char*)pucMessage, iMessageLen, puiMessagePriority);
#elif defined (PN_USE_FIFO) || defined (PN_USE_PIPE)
    iRetVal = read(iMessageQueueId, pucMessage, iMessageLen);
#elif defined (PN_USE_SOCKET)
#else
#endif

    if (iRetVal == -1)
    {
        if ((errno != EAGAIN) && (errno != EINTR))
        {
            LOG_ERROR(EMQ_RECEIVE_FAILED, iMessageQueueId, strerror(errno), errno);
        }
        else
        {
            LOG_DEVEL(ELOG_MESSAGE, strerror(errno));
        }
        return PN_FAILURE;
    }
    else
    {
        LOG_DEVEL(EMQ_RECV_SUCCESS, iRetVal, iMessageQueueId);
    }

    return iRetVal;
#else
    return PN_FAILURE;
#endif
}

int UTILRetrieveMessageFromMessageQueueTimeOut(int     iMessageQueueId,
                                               ubyte*  pucMessage, 
                                               int     iMessageLen,
                                               uint*   puiMessagePriority,
                                               int     iTimeoutVal)
{
    int iRetVal    = 0;
#ifdef LINUX
#if defined (PN_USE_MSGQ)

    fd_set fds;
    struct timeval tval;
    int iReadyFDs  = 0;
        
    tval.tv_sec    = 0;
    tval.tv_usec   = iTimeoutVal;
    
    FD_ZERO ( &fds );
    
    FD_SET ( iMessageQueueId, &fds );
    
    iReadyFDs      = select ( iMessageQueueId + 1, &fds, (fd_set *) 0, (fd_set *) 0, &tval );
    
    if ( iReadyFDs == -1 )
    {
        return PN_FALSE;
    }
    
    if ( !FD_ISSET ( iMessageQueueId, &fds ) )
    {
        return PN_FALSE;
    }

    iRetVal = mq_receive ( iMessageQueueId, (char*)pucMessage, iMessageLen, puiMessagePriority );

    if ( iRetVal == -1 )
    {
        if ( (errno != EAGAIN) && (errno != EINTR) )
        {
            LOG_ERROR(EMQ_RECEIVE_FAILED, iMessageQueueId, strerror(errno), errno);
        }
        else
        {
            LOG_DEVEL(ELOG_MESSAGE, strerror(errno));
        }
        return PN_FAILURE;
    }
    else
    {
        LOG_DEVEL(EMQ_RECV_SUCCESS, iRetVal, iMessageQueueId);
    }
#endif
#endif
    return iRetVal;
    

    
}

