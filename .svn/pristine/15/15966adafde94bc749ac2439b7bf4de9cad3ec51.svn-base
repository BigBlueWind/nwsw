/*******************************************************************************
 * Copyright: (C) 2009 Polaris Networks. All Rights Reserved.                  *
 *    No part of this source code may be reproduced, transmitted, transcribed  *
 *    or translated into any language without the written permission of        *
 *    Polaris Networks or its suppliers or affiliate companies. To obtain this *
 *    permission write to the attention of Legal counsel, Polaris Networks,    *
 *    75 Robbins Road, MA 02421 United States.                                 *
 ******************************************************************************/
#pragma once

#include <stdio.h>
#include "Constants.h"

#pragma pack( push, 1 )
/*========================================================================*/
/* Different Timer types                                                  */
/*========================================================================*/
typedef enum ETimerType
{
    ETM_TIMER_TYPE_SINGLE_SHOT,
    ETM_TIMER_TYPE_PERIODIC,
} ETimerType;

/*========================================================================*/
/* Event Handler user Callback function signature. in_pUserDataPtr is the */
/* pointer to the user data which was provided while registering the event*/
/*========================================================================*/

#define TIMER_MAX_REPETITION_REACHED 1
#define TIMER_MAX_REPETITION_NOT_REACHED 0

#define TIMER_INFINITE_REPEAT            0xff

typedef void ( *TMTimerHandler )( void* in_pUserData, unsigned char in_ucIsMaxRepetitionReached );
typedef void ( *TTimerParamsCleanupHandler) (void* in_pvParams);

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************/
/* Function Prototypes (Public Functions)                                         */
/**********************************************************************************/
int TMInitialize( void );
int TMCleanup( void );

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
 );
 
int TMDeregisterTimer( size_t in_uiTimerID );

int TMStartTimer
( 
	size_t 			  in_uiTimerID , 
	void*             in_pUserData
);

int TMStopTimer( size_t in_uiTimerID );
unsigned char TMIsTimerRunning( size_t in_uiTimerID );
int TMReconfigureTimer( size_t in_uiTimerID, unsigned int in_uiIntervalMs );
void *TMGetTimerUserData(size_t in_uiTimerID);

#ifdef __cplusplus
}
#endif

#pragma pack( pop )
