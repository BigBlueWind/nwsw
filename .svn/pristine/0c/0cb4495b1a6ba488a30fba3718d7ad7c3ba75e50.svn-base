/*******************************************************************************
* Copyright: (C) 2009 Polaris Networks. All Rights Reserved.                  *
*    No part of this source code may be reproduced, transmitted, transcribed  *
*    or translated into any language without the written permission of        *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this *
*    permission write to the attention of Legal counsel, Polaris Networks,    *
*    75 Robbins Road, MA 02421 United States.                                 *
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <winsock2.h>
#include "pthread.h"
#include "semaphore.h"
#include <windows.h>
#include <time.h>
#else
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#endif

#include "Timer.h"
#include "Logger.h"
#include "MemoryDebug.h"
#include "PW_PALProtocolStackShared.h"

/* Module Name needs to be set for Logging */
#define __sModuleName__ "Timer"

/**********************************************************************************/
/* Enum declaration                                                               */
/**********************************************************************************/
#define TM_TIMER_INTERVAL_S 1

typedef enum ETMTimerState
{
    ETM_TIMER_MODULE_NOT_INITIALIZED,
    ETM_TIMER_MODULE_INITIALIZED,
    ETM_TIMER_MODULE_CLEANED,
} ETMTimerState;

/*========================================================================*/
/* Structure for storing information of each timer identified by timer ID.*/
/*========================================================================*/
struct STimerList;
typedef struct STimerContext
{
    unsigned char     bIsTimerRunning;      /* Running or not                     */
    unsigned char     bIsMarkedToBeDeleted; /* Marked to be deleted */
    ETimerType        eTimerType;           /* Periodic or single shot            */
    unsigned int      uiTimerInterval;      /* After this interval timer is fired */
    TMTimerHandler    ptTimerHandler;       /* Callback to call when expire       */
    TTimerParamsCleanupHandler pTimerParamsCleanupHandler;
    void*             pvUserData;           /* Pointer to pass in callback        */

    unsigned char	  ucMaxRepetition;
    unsigned char	  ucRepetitionDone;

    /* the time when it is to go off */
    struct timeval    sActionTime;

    struct STimerList *psRunningListNode;
    struct STimerList *psRegisteredListNode;
} TTimerContext;

/*========================================================================*/
/* Structure for storing information of each timer identified by timer ID.*/
/*========================================================================*/
typedef struct STimerList
{
    void*              pvData;
    struct STimerList* psNext;
    struct STimerList* psPrev;
} TTimerList;

typedef struct
{
    TTimerList*     psListHead;
    TTimerList*     psListTail;
    unsigned int    uiListLen;
} TTimerListInfo;

/**********************************************************************************/
/* static variables declaration                                                   */
/**********************************************************************************/
static TTimerListInfo  s_sRegisteredTimerListInfo;
static TTimerListInfo  s_sRunningTimerListInfo;
static pthread_t       s_tTimerThreadHandle;
static pthread_mutex_t s_tTimerInfoMutex; 
static unsigned char   s_bTimerThreadRunning = PN_FALSE;
static int             s_iInitialized;

/**********************************************************************************/
/* Private function prototypes                                                     */
/**********************************************************************************/
static void*    _TMTimerThreadProc( void* in_pArg );
static int      _StopTimer( TTimerList* psTimerListNode );
TTimerList*     _TMAddTimer( TTimerListInfo* psTimerListInfo, TTimerContext* psTimer );
void            _TMDeleteTimer( TTimerListInfo* psTimerListInfo, TTimerList* psTimer );
void            _TMGetTime( struct timeval* psNow );
void            _TMModifyActionTime( TTimerContext* psTimerContext );
unsigned char   _TMIsTimerExpired( TTimerContext* psTimerContext, struct timeval* psNow );
static void     _TMClenupTimerList(  TTimerListInfo* psTimerListInfo );
void            _FreeTimerContext (TTimerContext* psTimerContext);

/**********************************************************************************/
/* Description:  This function is used for initializing timer module. It is       */
/*               called from the APIs Exposed to Path Controller module.          */
/*                                                                                */
/* Return Value:  int                                                             */
/*                                                                                */
/* Parameter[in]: None (void)                                                     */
/*                                                                                */
/* History:                                                                       */
/*     25-AUG-2009      Tuhin Khanrah          Initial Creation                   */
/**********************************************************************************/
int TMInitialize( void )
{
    int iStatus = PN_FAILURE;

    LOG_API_IN();

    do
    {
        if( s_iInitialized == ETM_TIMER_MODULE_INITIALIZED )
        {
            iStatus = PN_SUCCESS;
            break;
        }
        iStatus = pthread_mutex_init( &s_tTimerInfoMutex, NULL );
        if( iStatus == PN_FAILURE )
        {
            break;
        }

        UTILMemset(&s_sRegisteredTimerListInfo, 0, sizeof(TTimerListInfo));
        UTILMemset(&s_sRunningTimerListInfo, 0, sizeof(TTimerListInfo));

        iStatus = pthread_create( &s_tTimerThreadHandle, NULL, _TMTimerThreadProc, NULL );
        if( iStatus == PN_FAILURE )
        {
            break;
        }	

        s_iInitialized = ETM_TIMER_MODULE_INITIALIZED;
    } while( 0 );

    LOG_API_OUT();

    return iStatus;
}

/**********************************************************************************/
/* Description:  This function is used for cleaning up timer module. It is        */
/*               called from the APIs Exposed to Path Controller module.          */
/*                                                                                */
/* Return Value:  int                                                             */
/*                                                                                */
/* Parameter[in]: None (void)                                                     */
/*                                                                                */
/* History:                                                                       */
/*     25-AUG-2009      Tuhin Khanrah          Initial Creation                   */
/**********************************************************************************/
int TMCleanup( void )
{
    int iStatus = PN_FAILURE;

    LOG_API_IN();

    do 
    {
        if( s_iInitialized != ETM_TIMER_MODULE_INITIALIZED )
        {
            break;
        }

        s_bTimerThreadRunning = PN_FALSE;

        iStatus = pthread_cancel( s_tTimerThreadHandle );
        pthread_join(s_tTimerThreadHandle, NULL);

        if( iStatus == PN_FAILURE )
        {
            break;
        }

        _TMClenupTimerList(&s_sRegisteredTimerListInfo);
        s_iInitialized = ETM_TIMER_MODULE_CLEANED;

        iStatus = pthread_mutex_destroy( &s_tTimerInfoMutex );
        if( iStatus == PN_FAILURE )
        {
            break;
        }
    } while( 0 );

    LOG_API_OUT();

    return iStatus;
}

/*========================================================================*/
/* Timer related functions.                                               */
/*========================================================================*/
int TMRegisterTimer
(
 ETimerType     in_eTimerType,    /* Periodic/Single Shot                            */
 unsigned int   in_uiIntervalMs,  /* in milli second. 0 means immediate.             */
 TMTimerHandler in_pUserCallback, /* Callback function to call when this timer fires */
 TTimerParamsCleanupHandler in_pTimerParamsCleanupHandler,
 unsigned char	in_ucMaxRepetition,
 size_t*  out_puiTimerID
 )
{
    int iRetVal = PN_FAILURE;
    TTimerContext* psTimerContext = NULL;
    TTimerList* psTimerNode    = NULL;

    LOG_API_IN();

    /*LOG_DEBUG(ELOG_MESSAGE, "Registering Timer");*/

    do 
    {
        if( s_iInitialized != ETM_TIMER_MODULE_INITIALIZED )
        {
            break;
        }

        psTimerContext = ( TTimerContext* )UTILMalloc( sizeof( TTimerContext ));
        if( psTimerContext == NULL )
        {
            break;
        }
        psTimerContext->bIsTimerRunning = PN_FALSE;
        psTimerContext->bIsMarkedToBeDeleted = PN_FALSE;
        psTimerContext->eTimerType      = in_eTimerType;
        psTimerContext->ptTimerHandler  = in_pUserCallback;
        psTimerContext->pTimerParamsCleanupHandler  = in_pTimerParamsCleanupHandler;

        psTimerContext->pvUserData      = NULL;
        psTimerContext->uiTimerInterval = in_uiIntervalMs;

        psTimerContext->ucMaxRepetition  =	in_ucMaxRepetition;

        pthread_mutex_lock( &s_tTimerInfoMutex );

        psTimerNode = _TMAddTimer( &s_sRegisteredTimerListInfo, psTimerContext );
        if( psTimerNode == NULL )
        {
            iRetVal = PN_FAILURE;
        }
        psTimerContext->psRegisteredListNode = psTimerNode;

        pthread_mutex_unlock( &s_tTimerInfoMutex );

        *out_puiTimerID = ( size_t )( psTimerNode );
        iRetVal = PN_SUCCESS;

        LOG_DEVEL(ETM_REGISTERED_TIMER, psTimerNode, in_uiIntervalMs, in_ucMaxRepetition);

    } while( 0 );

    LOG_API_OUT();

    return iRetVal;
}


int TMDeregisterTimer( size_t in_uiTimerID )
{
    int iRetVal                   = PN_FAILURE;
    TTimerList* psListNode    = NULL;
    TTimerContext* psTimerContext = NULL;

    LOG_API_IN();

    /*LOG_DEBUG(ELOG_MESSAGE, "Deregistering Timer");*/

    do 
    {
        if( s_iInitialized != ETM_TIMER_MODULE_INITIALIZED )
        {
            break;
        }

        if( in_uiTimerID == 0 )
        {
            break;
        }

        pthread_mutex_lock( &s_tTimerInfoMutex );
        psListNode = ( TTimerList* )( size_t* )( in_uiTimerID );
        psTimerContext = ( TTimerContext* )( psListNode->pvData );
        _TMDeleteTimer( &s_sRegisteredTimerListInfo, psListNode );
        psTimerContext->psRegisteredListNode = NULL;
        if (psTimerContext != NULL)
        {
            if (psTimerContext->bIsTimerRunning == PN_TRUE)
            {
                _StopTimer( psListNode );
                psTimerContext->bIsMarkedToBeDeleted = PN_TRUE;
            }
            else
            {
                if (psTimerContext->psRunningListNode == NULL)
                {
                    _FreeTimerContext( psTimerContext );
                }
                else
                {
                    psTimerContext->bIsMarkedToBeDeleted = PN_TRUE;
                }
            }
        }
        pthread_mutex_unlock( &s_tTimerInfoMutex );

        iRetVal = PN_SUCCESS;

        LOG_DEVEL(ETM_DEREGISTERED_TIMER, psListNode);

    } while( 0 );

    LOG_API_OUT();

    return iRetVal;
}

int TMStartTimer
( 
 size_t     in_uiTimerID, 
 void*      in_pUserData
 )
{
    int iRetVal                   = PN_SUCCESS;
    TTimerList* psTempListNode    = NULL;
    TTimerContext* psTimerContext = NULL;

    LOG_API_IN();

    /*LOG_DEBUG(ELOG_MESSAGE, "Starting Timer");*/

    do 
    {
        if( s_iInitialized != ETM_TIMER_MODULE_INITIALIZED )
        {
            break;
        }

        if( in_uiTimerID == 0 )
        {
            break;
        }

        pthread_mutex_lock( &s_tTimerInfoMutex );

        psTempListNode = ( TTimerList* )( size_t* )( in_uiTimerID );
        psTimerContext = ( TTimerContext* )( psTempListNode->pvData );

        if (psTimerContext->bIsTimerRunning == PN_FALSE)
        {
            psTimerContext->ucRepetitionDone =  1;
            psTimerContext->pvUserData       =  in_pUserData;

            _TMGetTime( &( psTimerContext->sActionTime ) );
            _TMModifyActionTime( psTimerContext );
            psTimerContext->bIsTimerRunning = PN_TRUE;

            if (psTimerContext->psRunningListNode == NULL)
            {
                psTempListNode = _TMAddTimer( &s_sRunningTimerListInfo, psTimerContext );
                if (psTempListNode == NULL)
                {
                    iRetVal = PN_FAILURE;
                }
                psTimerContext->psRunningListNode = psTempListNode;
            }
        }
        else
        {
            iRetVal = PN_FAILURE;
        }

        pthread_mutex_unlock( &s_tTimerInfoMutex );

        if (iRetVal == PN_SUCCESS)
        {
            LOG_DEVEL(ETM_STARTED_TIMER, in_uiTimerID, psTimerContext->psRunningListNode);
        }
    } while( 0 );

    LOG_API_OUT();

    return iRetVal;
}

int TMStopTimer( size_t in_uiTimerID )
{
    TTimerList* psListNode = NULL;
    int iRetVal = PN_FAILURE;

    LOG_API_IN();

    /*LOG_DEBUG(ELOG_MESSAGE, "Stopping Timer");*/

    if( in_uiTimerID == 0 )
    {
        return iRetVal;
    }

    pthread_mutex_lock( &s_tTimerInfoMutex );

    psListNode = ( TTimerList* )( size_t* )( in_uiTimerID );
    iRetVal = _StopTimer(psListNode);

    pthread_mutex_unlock( &s_tTimerInfoMutex );

    if (iRetVal == PN_SUCCESS)
    {
        LOG_DEVEL(ETM_STOPPED_TIMER, psListNode);
    }

    LOG_API_OUT();

    return iRetVal;
}

static int _StopTimer( TTimerList* psTimerListNode )
{
    int iRetVal                   = PN_FAILURE;
    TTimerContext* psTimerContext = NULL;

    LOG_API_IN();

    /*LOG_DEBUG(ELOG_MESSAGE, "Stopping _Timer"); */

    do 
    {
        if( s_iInitialized != ETM_TIMER_MODULE_INITIALIZED )
        {
            break;
        }

        if( psTimerListNode == NULL )
        {
            break;
        }

        psTimerContext = ( TTimerContext* )( psTimerListNode->pvData );
        if( psTimerContext == NULL )
        {
            break;
        }

        if (psTimerContext->bIsTimerRunning == PN_TRUE)
        {
            psTimerContext->bIsTimerRunning = PN_FALSE;
            iRetVal = PN_SUCCESS;
        }

        /*LOG_DEBUG(ETM_STOPPED_TIMER, psTimerListNode);*/

    } while( 0 );

    LOG_API_OUT();

    return iRetVal;
}

unsigned char TMIsTimerRunning( size_t in_uiTimerID )
{
    unsigned char bRetVal              = PN_FALSE;
    TTimerList* psTempListNode    = NULL;
    TTimerContext* psTimerContext = NULL;

    LOG_API_IN();

    do
    {
        if( s_iInitialized != ETM_TIMER_MODULE_INITIALIZED )
        {
            break;
        }

        if( in_uiTimerID <= 0 )
        {
            break;
        }

        pthread_mutex_lock( &s_tTimerInfoMutex );

        psTempListNode = ( TTimerList* )( size_t* )( in_uiTimerID );
        psTimerContext = ( TTimerContext* )( psTempListNode->pvData );
        if(psTimerContext)
        {
        if( psTimerContext->bIsTimerRunning == PN_TRUE )
        {
            bRetVal = PN_TRUE;
        }
        }
        pthread_mutex_unlock( &s_tTimerInfoMutex );

    } while( 0 );

    LOG_API_OUT();

    return bRetVal;
}


int TMReconfigureTimer( size_t in_uiTimerID, unsigned int in_uiIntervalMs )
{
    int iRetVal                   = PN_FAILURE;
    TTimerList* psTempListNode    = NULL;
    TTimerContext* psTimerContext = NULL;

    LOG_API_IN();

    do
    {
        if( s_iInitialized != ETM_TIMER_MODULE_INITIALIZED )
        {
            break;
        }

        if( in_uiTimerID <= 0 )
        {
            break;
        }

        pthread_mutex_lock( &s_tTimerInfoMutex );

        psTempListNode = ( TTimerList* )( size_t* )( in_uiTimerID );
        psTimerContext = ( TTimerContext* )( psTempListNode->pvData );

        if( psTimerContext )
        {
            psTimerContext->uiTimerInterval = in_uiIntervalMs;
            _TMGetTime( &( psTimerContext->sActionTime ) );
            _TMModifyActionTime( psTimerContext );
            iRetVal = PN_SUCCESS;
        }

        pthread_mutex_unlock( &s_tTimerInfoMutex );

    } while( 0 );

    LOG_API_OUT();

    return iRetVal;
}

void *TMGetTimerUserData(size_t in_uiTimerID)
{
    TTimerList* psTempListNode    = NULL;
    TTimerContext* psTimerContext = NULL;
    void *pvUserData = NULL;

    LOG_API_IN();

    do
    {
        if( s_iInitialized != ETM_TIMER_MODULE_INITIALIZED )
        {
            break;
        }

        if( in_uiTimerID <= 0 )
        {
            break;
        }

        pthread_mutex_lock( &s_tTimerInfoMutex );

        psTempListNode = ( TTimerList* )( size_t* )( in_uiTimerID );
        psTimerContext = ( TTimerContext* )( psTempListNode->pvData );

        if( psTimerContext )
        {
            pvUserData = psTimerContext->pvUserData;
        }

        pthread_mutex_unlock( &s_tTimerInfoMutex );

    } while( 0 );

    LOG_API_OUT();

    return pvUserData;
}

/**********************************************************************************/
/* Description:  This private function is used to call the callback function      */
/*               corresponding to the expire timer. It is called when the thread  */
/*               is created.                                                      */
/*                                                                                */
/* Return Value:  int                                                             */
/*                                                                                */
/* Parameter[in]:                                                                 */
/*     in_uiTimerID: Indicates timer ID that needs to be deregistered             */
/*                                                                                */
/* History:                                                                       */
/*     25-AUG-2009      Tuhin Khanrah          Initial Creation                   */
/**********************************************************************************/
static void* _TMTimerThreadProc( void* in_pArg )
{
    TTimerList     *psCurrListNode = NULL, *psNextListNode = NULL;
    TTimerContext  *psTimerContext = NULL;
    unsigned char   bMaxRptnReached = TIMER_MAX_REPETITION_REACHED;
    unsigned char   bTimerToBeFired = PN_FALSE;
    TMTimerHandler  pTimerHandler = NULL;
    void            *pvUserData = NULL;
    struct timeval  sNow;
    struct timeval  sSelectIntervaltime = { 0 };
    unsigned int    uiCurrIter = 0;

    LOG_API_IN(); 
    
    s_bTimerThreadRunning = PN_TRUE;

    while( s_bTimerThreadRunning )
    {
        /* Wait for 1 second */

#ifndef WIN32
        sSelectIntervaltime.tv_sec  = 0;
        sSelectIntervaltime.tv_usec = 10000; /* 10 ms timer tick */
        select( 0, NULL, NULL, NULL, &sSelectIntervaltime );
#else
        Sleep(TM_TIMER_INTERVAL_S << 10);
#endif

        _TMGetTime( &sNow );
        uiCurrIter = 0;
        psCurrListNode = NULL;
        while( PN_TRUE )
        {
            bMaxRptnReached = TIMER_MAX_REPETITION_REACHED;
            bTimerToBeFired = PN_FALSE;
            pTimerHandler = NULL;
            pvUserData = NULL;

            pthread_mutex_lock( &s_tTimerInfoMutex );
            if (s_sRunningTimerListInfo.psListHead == NULL)
            {
                pthread_mutex_unlock( &s_tTimerInfoMutex );
                break;
            }

            if (uiCurrIter == 0)
            {
                psCurrListNode = s_sRunningTimerListInfo.psListHead;
            }
            else
            {
                psCurrListNode = psNextListNode;
            }
            uiCurrIter++;

            if (psCurrListNode == NULL)
            {
                pthread_mutex_unlock( &s_tTimerInfoMutex );
                break;
            }
            psNextListNode = psCurrListNode->psNext;

            psTimerContext = ( TTimerContext* )( psCurrListNode->pvData );
            if (psTimerContext == NULL)
            {
                pthread_mutex_unlock( &s_tTimerInfoMutex );
                continue;
            }

            if( psTimerContext->bIsTimerRunning == PN_TRUE )
            {
                if( _TMIsTimerExpired( psTimerContext, &sNow ) == PN_TRUE )
                {
                    if (( psTimerContext->eTimerType == ETM_TIMER_TYPE_PERIODIC ) &&
                        (psTimerContext->ucRepetitionDone < psTimerContext->ucMaxRepetition))
                    {
                        bMaxRptnReached = TIMER_MAX_REPETITION_NOT_REACHED;
                        _TMModifyActionTime( psTimerContext );
                    }
                    else if (( psTimerContext->eTimerType == ETM_TIMER_TYPE_PERIODIC ) &&
                        (psTimerContext->ucRepetitionDone == psTimerContext->ucMaxRepetition)
                        && (psTimerContext->ucMaxRepetition == TIMER_INFINITE_REPEAT))
                    {
                        psTimerContext->ucRepetitionDone = 0;
                        bMaxRptnReached = TIMER_MAX_REPETITION_NOT_REACHED;
                        _TMModifyActionTime( psTimerContext );
                    }
                    else
                    {
                        _StopTimer(psCurrListNode);
                    }
                    psTimerContext->ucRepetitionDone += 1;
                    bTimerToBeFired = PN_TRUE;
                    pTimerHandler = psTimerContext->ptTimerHandler;
                    pvUserData = psTimerContext->pvUserData;
                }
            }
            else
            {
                /* Remove this node from "Running" list */
                _TMDeleteTimer( &s_sRunningTimerListInfo, psCurrListNode );
                psTimerContext->psRunningListNode = NULL;
                if( psTimerContext->bIsMarkedToBeDeleted == PN_TRUE )
                {
                    /* This node is marked to be Deregistered */
                    _FreeTimerContext( psTimerContext );
                }
            }

            pthread_mutex_unlock( &s_tTimerInfoMutex );

            if (bTimerToBeFired == PN_TRUE)
            {
                if (pTimerHandler != NULL)
                {
                    ( *( pTimerHandler ) )( pvUserData, bMaxRptnReached);
                    LOG_DEVEL(ETM_INVOKED_TIMER_CB, psCurrListNode);
                }
            }
        }
    }

    LOG_API_OUT();

    return NULL;
}

TTimerList* _TMAddTimer( TTimerListInfo* psTimerListInfo, TTimerContext* psTimerContext )
{
    TTimerList* psTimerListNode = NULL;

    LOG_API_IN();

    /*LOG_DEBUG(ELOG_MESSAGE, "Adding timer");*/

    psTimerListNode = UTILMalloc( sizeof( TTimerList ) );
    if( psTimerListNode == NULL )
    {
        return NULL;
    }

    psTimerListNode->pvData = ( void* )psTimerContext;
    psTimerListNode->psNext = NULL;
    psTimerListNode->psPrev = NULL;

    if( psTimerListInfo->psListHead == NULL )
    {
        psTimerListInfo->psListHead = psTimerListNode;
    }

    if( psTimerListInfo->psListTail != NULL )
    {
        psTimerListInfo->psListTail->psNext = psTimerListNode;
    }
    psTimerListNode->psPrev = psTimerListInfo->psListTail;
    psTimerListInfo->psListTail = psTimerListNode;

    psTimerListInfo->uiListLen++;

    LOG_DEVEL(ETM_ADDED_TIMER,
        psTimerListNode,
        psTimerListInfo, 
        psTimerListInfo->psListHead,
        psTimerListInfo->psListTail,
        psTimerListInfo->uiListLen);

    LOG_API_OUT();

    return psTimerListNode;
}

void _TMDeleteTimer( TTimerListInfo* psTimerListInfo, TTimerList* psTimer )
{
    TTimerList *pTmpListToPrint = psTimer;
    LOG_API_IN();

    if( psTimer == NULL)
    {
        return;
    }

    /*LOG_DEBUG(ELOG_MESSAGE, "Deleting timer");*/

    if ( psTimer == psTimerListInfo->psListHead )
    {
        psTimerListInfo->psListHead = psTimerListInfo->psListHead->psNext;
    }
    else
    {
        psTimer->psPrev->psNext = psTimer->psNext;
    }
    if (psTimer == psTimerListInfo->psListTail)
    {
        psTimerListInfo->psListTail = psTimerListInfo->psListTail->psPrev;
    }
    else
    {
        psTimer->psNext->psPrev = psTimer->psPrev;
    }
    psTimer->psNext = NULL;
    psTimer->psPrev = NULL;
    psTimer->pvData = NULL;
    UTILFree( psTimer );

    psTimerListInfo->uiListLen--;

    LOG_DEVEL(ETM_REMOVED_TIMER,
        pTmpListToPrint,
        psTimerListInfo, 
        psTimerListInfo->psListHead,
        psTimerListInfo->psListTail,
        psTimerListInfo->uiListLen);

    LOG_API_OUT();

    return;
}

void _TMGetTime( struct timeval* psNow )
{
    UTILGetCurrentTimeOfDay((size_t*)&psNow->tv_sec, (size_t*)&psNow->tv_usec);
}

void _TMModifyActionTime( TTimerContext* psTimerContext )
{
    psTimerContext->sActionTime.tv_sec  += ( psTimerContext->uiTimerInterval >> 10 );
    psTimerContext->sActionTime.tv_usec += ( psTimerContext->uiTimerInterval % 1000 ) << 10;

    if( psTimerContext->sActionTime.tv_usec >= 1000000 )
    {
        psTimerContext->sActionTime.tv_sec++;
        psTimerContext->sActionTime.tv_usec -= 1000000;
    }
}

unsigned char _TMIsTimerExpired( TTimerContext* psTimerContext, struct timeval* psNow )
{
    if( psTimerContext && psNow )
    {
/*
        byte4 lSecs  = 0;
        byte4 lUsecs = 0;

        lSecs  = psTimerContext->sActionTime.tv_sec - psNow->tv_sec;
        lUsecs = psTimerContext->sActionTime.tv_usec - psNow->tv_usec;

        if( lSecs < 0 )
        {
            return PN_TRUE;
        }

        if( lUsecs < 0 )
        {
            lSecs--;
            lUsecs += 1000000;
        }
        if( lSecs < 0 )
        {
            return PN_TRUE;
        }
*/
        if (psTimerContext->sActionTime.tv_sec < psNow->tv_sec)
        {
            return PN_TRUE;
        }
    }

    return PN_FALSE;
}

static void _TMClenupTimerList(  TTimerListInfo* psTimerListInfo )
{
/*
    TTimerList* psCurList = NULL;
    TTimerList* psNextList = NULL;
    psCurList = psTimerListInfo->psListHead;
    LOG_API_IN();

    pthread_mutex_lock( &s_tTimerInfoMutex );

    while( psCurList )
    {
        _StopTimer( psCurList );
        psNextList = psCurList->psNext;
        UTILFree(psCurList->pvData);
        UTILFree(psCurList);
        psCurList = psNextList;
    }

    pthread_mutex_unlock( &s_tTimerInfoMutex );

    LOG_API_OUT();
*/
}

void _FreeTimerContext (TTimerContext* psTimerContext)
{
    if (psTimerContext == NULL)
    {
        return;
    }
    psTimerContext->bIsTimerRunning = PN_FALSE;
    psTimerContext->bIsMarkedToBeDeleted = PN_FALSE;
    psTimerContext->uiTimerInterval = 0;
    psTimerContext->ptTimerHandler = NULL;
    psTimerContext->pTimerParamsCleanupHandler = NULL;
    psTimerContext->pvUserData = NULL;
    psTimerContext->ucMaxRepetition = 0;
    psTimerContext->ucRepetitionDone = 0;
    
    if( NULL != psTimerContext->psRunningListNode )
    {
    	psTimerContext->psRunningListNode->pvData = NULL;
    }
    psTimerContext->psRunningListNode = NULL;
    
    if( NULL != psTimerContext->psRegisteredListNode )
    {
    	psTimerContext->psRegisteredListNode->pvData = NULL;
    }
    psTimerContext->psRegisteredListNode = NULL;

    UTILFree(psTimerContext);
}
