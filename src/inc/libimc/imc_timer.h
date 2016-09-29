/**
 * @file    imc_timer.h
 * @brief Description
 *          Timer Service, also called TIMEKEEPER
 * @details Copyright Powerwave Technologies, Inc., 2009
 * @author : Manu Sharma
 * Date   :
 * Description :
 */

#ifndef IMC_TIMER_H
#define IMC_TIMER_H

#include "bs_config.h"
#include "bs_tables.h"
#include "basic_types.h"
#include "msg_ids.h"
#include <sys/unistd.h> /* usleep */
#include <time.h>

/* VALUES *********************************************************************/
#define NULL_TIMER_ID def(TimerId, 0)
#define MIN_TIMER_ID  def(TimerId, 1)
#define MAX_TIMER_ID  def(TimerId, (MIN_TIMER_ID + MAX_TIMER_INST))
#define TIMESPEC_NO_TIMER { timeUnitNone, 0 }
#define MAX_TIMERS_EACH_MODULE  12
#define MAX_MINUTES_ALLOWED_FOR_USEC_GRAN           70
#define MAX_MINUTES_ALLOWED_FOR_SEC_GRAN            70000000
/* TYPES **********************************************************************/

/** Unique identifier of a timer.
 *  Allocated by timer service.
 *  Not to be changed by users.
 */
typedef UINT32 TimerId;

/** TimerType as defined by user.
 *  Not interpreted by timer.
 *  See createTimer() for details.
 */
typedef UINT32 Duratn;

typedef struct s_TimeSpec
{
  TimeUnit timeUnit;
  Duratn duration;
} __attribute__((__packed__))TimeSpec;

typedef UINT32 TimeoutUserInfo;

typedef void (*TimerExpiryCallBackFunc) (TimerId  timerId,
                                         void *paramPassedToTimerExpiryCallbackFunc);

/** Information given by user when timer is created.
 */
typedef struct s_TimerDesc
{
  ModuleId        moduleId;
  TimerName       timerType;
  TimeSpec        timeSpec;
  MsgId           timeoutMsgId;
  /* userData is returned to user at TIMEOUT */
  TimeoutUserInfo userInfo;
  
} __attribute__((__packed__))TimerDesc;

/** Msg sent to user at TIMEOUT */
typedef struct s_MsgTimeoutInd
{
  TimerId         timerId;
  TimerDesc       timerDesc;
} __attribute__((__packed__))MsgTimeoutInd;

typedef MsgTimeoutInd  TimerInfo;

typedef struct s_ModTimersInfo
{
  UINT8          totalTimersInMod;
  TimerInfo      modTimerInfo[MAX_TIMERS_EACH_MODULE];
} ModTimersInfo;

/** Information associated with each timer instance */
typedef struct s_TimerCtx
{
  /* Timer's private data */
  TimerId         timerId;         /* Identifies this timer instance */
  TBOOL           isAllocated;     /* TRUE => timer is allocated to a user */
  Duratn          ticksRemaining;  /* in terms of clockIntervals of timer service */
  TimerDesc       timerDesc;         /* Data given by user, will be returned at timer expiry */
  TimerExpiryCallBackFunc timerExpiryCallBackFunc; /* Callback function when Timer Expiry Happens */
  void           *paramPassedToTimerExpiryCallbackFunc;
} __attribute__((__packed__))TimerCtx;

typedef struct s_TimerServiceCtx
{
  TimerCtx timerTab[MAX_TIMER_ID];    /* Table of all timer Instances of this service */
  TimeSpec clock;
  /* The clock run by timer service.
   * This clock is used to update the elapsed time.
   */
} __attribute__((__packed__))TimerServiceCtx;

/* USAGE **********************************************************************/

/* Steps on how to use timer service:
 *
 * Initialise:
 *   Initialise the service: timerServiceInit();
 *   This needs to be called only once.
 *
 * How to allocate a timer?
 *   This is done by calling createTimer();
 * Example:
 *   TimeSpec timerDesc = { T_WAIT_HO_ACK, {TU_MSEC, 10 }, toutMsgId, msId };
 *   TimerId  timerId = createTimer(&timeDesc);
 *
 *   Now a timer is allocated to you.
 *   But it is not ticking yet.
 *
 * How to start a timer?
 *   To set the timer ticking, call startTimer().
 *   The timerId allocated using createTimer() is to be used here.
 *  Example:
 *   starTimer(timerId);
 *
 *   Now the timer is ticking.
 *
 * Timeout:
 *   At timeout, message with msgId=toutMsgId will be sent from
 *   TIMEKEEPER, i.e. this service.
 *   This message would contain userInfo.
 *
 * Stopping a timer:
 *   If user needs to stop a timer, stopTimer() should be called.
 *   The words stopTimer() and cancelTimer() are same.
 *
 *   Example:
 *     stopTimer(&timerId);
 *
 *   Note that stopTimer() takes address of TimerId.
 *   stopTimer() also deallocates the timerId.
 *   Thus, once stopped, the timer is gone.
 *   No operation on the timerId is permitted after stopTimer().
 *
 * TimerRestart:
 *   This feature is not supported.
 */

/* API ************************************************************************/

TimerDesc  *getBsTimer( TimerName timertype);

/* Initialises the timer service */
void timerServiceInit(
    IN TimeSpec tick
    );

/** This function creates a timer instance.
 *  The created/allocated instance is idenfied by timerId.
 *  Tihs TimerId is to be used in all subsequent actions on the timer.
 *
 *  The timer created is NOT ticking yet.
 *  Timer starts ticking only when startTimer() is called.
 *
 *  User MUST not change timerId.
 */

TimerId createTimer(
  IN TimerDesc *timerDesc
);

/* This function sets the timer identified by timerId ticking. */
TBOOL startTimer(
  IN TimerId timerId
);

/** This function stops and deallocates timer identified by timerId */
void stopTimerInst( TimerId timerId);

#define stopTimer( timerId)    stopTimerInst( timerId); \
                               timerId = NULL_TIMER_ID;

/* This function gives Timer Running Status TRUE --> RUNNING  FALSE --> NOT-RUNNING */
TBOOL checkTimerStatus(
  IN TimerId id
  );
/** This function creates and starts a timer.
 *  It is same as createTimer() + startTimer()
 */
TimerId createAndStartTimer(
  IN TimerDesc *timerDesc,
  TimerExpiryCallBackFunc timerExpiryCallBackFunc,
  void *paramPassedToTimerExpiryCallbackFunc
);

#define sleep_sec(secs)    usleep(secs * 1000 * 1000)
#define sleep_msec(msecs)  usleep(msecs * 1000)

time_t getCurrentTime(void);
UINT32 getUpTime( IN time_t  firstTime );
void stopallTimers(void);
#endif /* IMC_TIMER_H */
