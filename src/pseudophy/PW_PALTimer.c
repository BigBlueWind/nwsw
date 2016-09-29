/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#include "Timer.h"
#include "MemoryDebug.h"
#include "PW_PALTimer.h"
#include "PW_PALDefinition.h"
#include "PW_PALMessageQueue.h"

#define __sModuleName__ "TMR"

static int s_iWrtTimerQFd = -1;
static void _FireTTIIndicationTimer(void* pvUserData, unsigned char ucData);

int TMInitTimer(unsigned char ucTimeToWait, unsigned int uiTimerId)
{
    int iRetVal;

    s_iWrtTimerQFd = MQGetWriteAccess(EPW_MSGQ_TYPE_TIMER);
    if(s_iWrtTimerQFd == -1)
    {
        LOG_ERROR(ELOG_MESSAGE, "Unable to retrieve Message Queue Write FD \
                                for Timer Thread");
        return PN_FAILURE;
    }

    if(TMInitialize() == PN_FAILURE)
    {
        LOG_ERROR(ELOG_MESSAGE, "Unable to create timer");
        return PN_FAILURE;
    }

    iRetVal = TMRegisterTimer(
        ETM_TIMER_TYPE_PERIODIC,
        ucTimeToWait*1000,
        _FireTTIIndicationTimer,
        NULL,
        TIMER_INFINITE_REPEAT,
        &uiTimerId);

    if(iRetVal == PN_FAILURE)
    {
        LOG_ERROR(ELOG_MESSAGE, "Unable to register timer for TTI Indication");
        return PN_FAILURE;
    }

    iRetVal = TMStartTimer(uiTimerId, NULL);

    return PN_SUCCESS;
}

int TMCloseTimer(void)
{
    TMCleanup();
    return PN_SUCCESS;
}

static void _FireTTIIndicationTimer(void* pvUserData, unsigned char ucData)
{
    TTimerEventInfo *psTimerEventInfo = NULL;
    int iRetVal;

    psTimerEventInfo = (TTimerEventInfo *)UTILMalloc(sizeof(TTimerEventInfo));
    if(psTimerEventInfo == NULL)
    {
        LOG_ERROR(ELOG_MESSAGE, "Unable to allocate memory");
        return;
    }
    psTimerEventInfo->m_ucVersion = PAL_VERSION;

    iRetVal =
        MQAdd(s_iWrtTimerQFd, (unsigned char*)&psTimerEventInfo,
        sizeof(TTimerEventInfo *));

    if(iRetVal != PN_SUCCESS)
    {
        LOG_ERROR(ELOG_MESSAGE, "Unable to add Timer event to Message Queue");
    }
    return;
}
