/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#include "PW_PALMessageQueue.h"

#define __sModuleName__ "MSGQ"

static int s_iTimerQueueFd, s_iDlApiQueueFd;

int MQInitialize()
{
    s_iTimerQueueFd = UTILCreateMessageQueue(PW_PAL_MSG_Q_TIMER);
    if (s_iTimerQueueFd == PN_FAILURE)
    {
#ifdef LINUX
        if (errno == EEXIST)
        {
            UTILDeleteMessageQueue(PW_PAL_MSG_Q_TIMER);
            s_iTimerQueueFd = UTILCreateMessageQueue(PW_PAL_MSG_Q_TIMER);
            if (s_iTimerQueueFd == PN_FAILURE)
            {
                LOG_ERROR(ELOG_MESSAGE, "Unable to create Timer Message Queue");
                return PN_FAILURE;
            }
        }
        else
        {
            LOG_ERROR(ELOG_MESSAGE, "Unable to create Timer Message Queue");
            return PN_FAILURE;
        }
#endif
    }

    s_iDlApiQueueFd = UTILCreateMessageQueue(PW_PAL_MSG_Q_DLAPI);
    if (s_iDlApiQueueFd == PN_FAILURE)
    {
#ifdef LINUX
        if (errno == EEXIST)
        {
            UTILDeleteMessageQueue(PW_PAL_MSG_Q_DLAPI);
            s_iDlApiQueueFd = UTILCreateMessageQueue(PW_PAL_MSG_Q_DLAPI);
            if (s_iDlApiQueueFd == PN_FAILURE)
            {
                LOG_ERROR(ELOG_MESSAGE, "Unable to create DL API Message Queue");
                return PN_FAILURE;
            }
        }
        else
        {
            LOG_ERROR(ELOG_MESSAGE, "Unable to create DL API Message Queue");
            return PN_FAILURE;
        }
#endif
    }

    return PN_SUCCESS;
}

int MQClose()
{
    int iRetVal = 0;

    iRetVal = UTILCloseMessageQueue(s_iTimerQueueFd);

    iRetVal = UTILDeleteMessageQueue(PW_PAL_MSG_Q_TIMER);

    iRetVal = UTILCloseMessageQueue(s_iDlApiQueueFd);

    iRetVal = UTILDeleteMessageQueue(PW_PAL_MSG_Q_DLAPI);

    return PN_SUCCESS;
}

int MQGetQueueId(PW_EMsgQType eMsgQType)
{
    if(eMsgQType == EPW_MSGQ_TYPE_TIMER)
    {
        return s_iTimerQueueFd;
    }
    else if(eMsgQType == EPW_MSGQ_TYPE_DLAPI)
    {
        return s_iDlApiQueueFd;
    }
    else
    {
        return PN_FAILURE;
    }
}

int MQGetWriteAccess(PW_EMsgQType eMsgQType)
{
    if(eMsgQType == EPW_MSGQ_TYPE_TIMER)
    {
        return UTILOpenMessageQueueWriter(PW_PAL_MSG_Q_TIMER);
    }
    else if(eMsgQType == EPW_MSGQ_TYPE_DLAPI)
    {
        return UTILOpenMessageQueueWriter(PW_PAL_MSG_Q_DLAPI);
    }
    else
    {
        return PN_FAILURE;
    }
    
}

int MQAdd(int iQueueFd, unsigned char* pucMsg, int iMsgLen)
{
    return UTILAddMessageInMessageQueue(iQueueFd, pucMsg, iMsgLen, 0); /*Message Priority 0*/
}
