/******************************************************************************
* Copyright: (C) 2010 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef LINUX
#include <sys/time.h>
#include <sys/types.h>
#endif
#include "pthread.h"
#include "PW_PALInterface.h"
#include "PW_PALStateMachine.h"
#include "PW_PALMessageQueue.h"
#include "envdep.h"
#include "LtePhyL2Api.h"
#include "MemoryDebug.h"
#include "ys_ms.h"

#define __sModuleName__ "INTF"
#define PN_MAX_RESPONSE_TIMEOUT 30

void* _RunFSMInMainThread(void* pvArg);
static PW_BOOL s_bRunFSM = PN_FALSE;
static int s_iWrtMainQFd = -1;
pthread_mutex_t     g_condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t      g_event_completion_cond = PTHREAD_COND_INITIALIZER;
static pthread_t    s_hMainThread;

extern S16 msArgc;              /* argc */
extern S8 **msArgv;            /* argv */
int PW_PALInitLibrary()
{
    int iRetVal = PN_FAILURE;
#ifdef LINUX
    struct timeval sCurrentTime;
    struct timespec sExpiration;
#endif
    char         *rootDir = NULL;
    char         logFileName[PW_LOG_FILE_LENGTH];
    int          argc;
    char           **argv;

    argc = msArgc;
    argv = msArgv;

    rootDir = argv[2];
    if(NULL != rootDir)
    {
      sprintf((char *)logFileName, "%s/log/pal.log", rootDir );
    }
    else
    {
      fprintf(stderr,"Environment variable --rootDir (-r) is NOT SET");
      exit(EXIT_FAILURE);
    }

    LOG_INIT_MODULE(logFileName, ELOG_LEVEL_DEBUG,
        ELOG_TIME_FORMAT_DATE_TIME);

    iRetVal = FSMInitialize();

    if(iRetVal == PN_SUCCESS)
    {
        LOG_INFO(ELOG_MESSAGE, "Successfully initialized FSM");
        s_bRunFSM = PN_TRUE;
    }
    else
    {
        LOG_ERROR(ELOG_MESSAGE, "Failed to initialize FSM");
        return iRetVal;
    }

    s_iWrtMainQFd = MQGetWriteAccess(EPW_MSGQ_TYPE_DLAPI);
    if(s_iWrtMainQFd == -1)
    {
        LOG_ERROR(ELOG_MESSAGE, "Unable to retrieve Message Queue Write \
                                FD for Main Thread");
        return PN_FAILURE;
    }

    pthread_mutex_lock(&g_condition_mutex); /* will block if lock not available*/

    /*Start Main Thread*/
    if (pthread_create(&s_hMainThread, NULL, _RunFSMInMainThread, NULL) != 0)
    {
        LOG_ERROR(ELOG_MESSAGE, "Failed to create Main Thread");
        return PN_FAILURE;
    }

#ifdef LINUX
    gettimeofday(&sCurrentTime, NULL);
    sExpiration.tv_sec = sCurrentTime.tv_sec + PN_MAX_RESPONSE_TIMEOUT;
    sExpiration.tv_nsec = sCurrentTime.tv_usec * 1000;
    iRetVal = pthread_cond_timedwait(&g_event_completion_cond,
        &g_condition_mutex, &sExpiration); /* will block until time expired */
#endif

    pthread_mutex_unlock(&g_condition_mutex);

    if (iRetVal != PN_SUCCESS)
    {
        LOG_ERROR(ELOG_MESSAGE, "Failed to initialize Main Thread");
        return PN_FAILURE;
    }

    LOG_INFO(ELOG_MESSAGE, "Successfully Initialized Pseudo PHY");
    return iRetVal;
}

int PW_PALCloseLibrary()
{
    s_bRunFSM = PN_FALSE;

    if (pthread_join(s_hMainThread, NULL) != 0)
    {
        LOG_ERROR(ELOG_MESSAGE, "Unable to terminate Main thread");
        return PN_FAILURE;
    }

    pthread_mutex_destroy(&g_condition_mutex);
    pthread_cond_destroy(&g_event_completion_cond);

    FSMClose();

    return PN_SUCCESS;
}

void* _RunFSMInMainThread(void* pvArg)
{
    /*Signal the main thread which is waiting for event to complete*/
    {
        pthread_mutex_lock(&g_condition_mutex); /* will block if lock not available*/
        pthread_cond_signal(&g_event_completion_cond); 
        pthread_mutex_unlock(&g_condition_mutex);
    }
    while(s_bRunFSM)
    {
        FSMExecute();
    }

    return NULL;
}

int PW_PALSendPhyTxStartIndication(unsigned short usSFN, unsigned char ucSubframe)
{
    LOG_DEVEL(ELOG_MESSAGE, "Sent TX START INDICATION primitive");
    MSGIND Pdata;
    Pdata.subFrameNum = ucSubframe ;
    Pdata.frameNumber = usSFN;

    PsuedoToCLMsg(PHY_TXSTART_IND, 1, &Pdata, 0);
    return PN_SUCCESS;
}

int PW_PALSendPhyRxStartIndication(unsigned char ucSubframe)
{
    LOG_DEBUG(ELOG_MESSAGE, "Sent RX START INDICATION primitive");
    unsigned char tempSubframe;

    ((ucSubframe - 1) > 0) ? tempSubframe = (ucSubframe - 1) : (tempSubframe = 9);

    MSGIND Pdata;
    Pdata.subFrameNum = tempSubframe;
    Pdata.status = 0;

    PsuedoToCLMsg(PHY_RXSTART_IND, 1, &Pdata, 0);
    return PN_SUCCESS;
}

int PW_PALSendPhyRxSduIndicationForRACH(unsigned char ucSubframe, unsigned char ucPreambleId)
{
    unsigned char arrData[4];
    LOG_DEBUG(ELOG_MESSAGE, "Sent RX SDU INDICATION primitive for RACH");
    UTILMemset(arrData, '\0', 4);


    ((ucSubframe - 1) > 0) ? arrData[0] = (ucSubframe - 1) : (arrData[0] = 9);
    LOG_DEBUG(ES1AP_DEBUG_MSG_WITH_INT_VALUE, "Subframe no at RACH Indication = ", ucSubframe);

    PsuedoToCLMsg(PHY_RXSDU_IND, 0, arrData, 1);
    PsuedoToCLMsg(PHY_RXEND_IND, 1, arrData, 1);
    return PN_SUCCESS;
}

int PW_PALSendPhyRxSduIndicationForULSCH(unsigned short usSFN, unsigned char ucSubframe,
                                         unsigned int uiCrnti, unsigned int uiMacPduSize,
                                         unsigned char* pucMacPdu)
{
    unsigned char* pData = NULL;
    unsigned char tempSubframe;
    LOG_DEBUG(ELOG_MESSAGE, "Sent RX SDU INDICATION primitive for UL-SCH");

    pData = (unsigned char*)UTILMalloc(uiMacPduSize + 1);
    UTILMemset(pData, '\0', uiMacPduSize + 1);

    ((ucSubframe - 1) > 0) ? tempSubframe = (ucSubframe - 1) : (tempSubframe = 9);

    UTILMemcpy(pData, &tempSubframe, 1);
    UTILMemcpy(pData + 1, pucMacPdu, uiMacPduSize);

    LOG_DEBUG(ELOG_MESSAGE, "Sent RX SDU INDICATION primitive for UL-SCH");
    LOG_DEBUG(ES1AP_DEBUG_MSG_WITH_INT_VALUE, "Subframe no at UL-SCH Indication = ", ucSubframe);
    PsuedoToCLMsg(PHY_RXSDU_IND, 1, pData, uiMacPduSize);
    PsuedoToCLMsg(PHY_RXEND_IND, 1, &pData[0] , 1);
    ys_crnti=uiCrnti;
    UTILFree(pData);
    pData = NULL;
    return PN_SUCCESS;
}

int PW_PALReceivePhyTxStartRequest(unsigned char ucSubframe)
{
    LOG_DEVEL(ELOG_MESSAGE, "Received TX START REQUEST primitive");
    return PN_SUCCESS;
}

int PW_PALReceivePhyTxSduRequest(unsigned short usChannelId,
                                 unsigned char ucChannelType,
                                 unsigned short usPhySduSizeInBytes,
                                 unsigned char* pucPhySdu,
                                 unsigned short usRnti)
{
    TDLAPIEventInfo *psDLAPIEventInfo = NULL;
    int iRetVal;
    char logMsg[128];
    sprintf(logMsg, "Received TX SDU REQUEST primitive for RNTI %d", usRnti);
    LOG_DEBUG(ELOG_MESSAGE, logMsg);
    psDLAPIEventInfo = (TDLAPIEventInfo *)UTILMalloc(sizeof(TDLAPIEventInfo));
    if(psDLAPIEventInfo == NULL)
    {
        LOG_ERROR(ELOG_MESSAGE, "Unable to allocate memory");
        return PN_FAILURE;
    }
    psDLAPIEventInfo->m_ucVersion = PAL_VERSION;
    psDLAPIEventInfo->m_eMspdPhyPrimitive = EMSPD_PHY_TXSDU_request;
    psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usChannelId =
        usChannelId;
    psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_ucChannelType =
        ucChannelType;
    psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usPhySduSizeInBytes =
        usPhySduSizeInBytes;
    psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_pucPhySdu =
        (unsigned char*)UTILMalloc(usPhySduSizeInBytes);
    UTILMemcpy(psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_pucPhySdu,
        pucPhySdu, usPhySduSizeInBytes);
    psDLAPIEventInfo->m_sPhyPrimitiveUnion.m_sMspdPhyTxSduRequest.m_usRnti = usRnti;

    iRetVal = MQAdd(s_iWrtMainQFd, (unsigned char*)&psDLAPIEventInfo,
        sizeof(TDLAPIEventInfo *));
    if(iRetVal != PN_SUCCESS)
    {
        LOG_ERROR(ELOG_MESSAGE, "Unable to add MSPD PHY Primitive \
                                event to Message Queue");
        return PN_FAILURE;
    }

    return PN_SUCCESS;
}
