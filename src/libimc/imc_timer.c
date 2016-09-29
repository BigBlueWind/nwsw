/**
 * @file    imc_timer.c
 * @brief Timer Service @n
 *        This file contains the functions for timer creation and stopping timer
 * @details Copyright Powerwave Technologies, Inc., 2009
 * 
 * @author Manu Sharma
 *
 * @date 17May2010 Added parameter tick to timerServiceInit() : Manu Sharma
 * 
 *
 */

#include "imc_timer.h"
#include "log.h"
#include "imc.h" /* for createAndSendMsg */
#include "imc_err.h"
#include  "enumstr_oamtables.h"
#include <signal.h>
#include <sys/time.h>
#include <errno.h>

/* GLOBALS ********************************************************************/
TimerServiceCtx g_TimerServiceCtx;

/* INTERNAL FUNCTIONS *********************************************************/
/**
 * @details This function returns the global timer service context
 */
inline TimerServiceCtx *getTimerServiceCtx(void)
{
  return &(g_TimerServiceCtx);
}

/**
 * @param timerId Timer ID identifies a timer uniquely in a given process
 * @details 
 * This function returns the context of the timerId
 */
inline TimerCtx *getTimerCtx(
  IN TimerId timerId
  )
{
  return (&(getTimerServiceCtx()->timerTab[timerId]));
}

/**
 * @param timeSpec The duration of timer in minutes, or seconds, or milliseconds or micro seconds
 * @details This function Converts a timeSpec to equivalent in microsecond
 * As OS Allows in terms of Seconds and Micro Seconds we Calculate TicksRemaining in terms of Seconds
 * or MicroSeconds only.
 */
Duratn getDurationInUsec(
  IN TimeSpec *timeSpec
  )
{
  if(TIMEUNIT_USEC EQ timeSpec->timeUnit)
   return timeSpec->duration;

  if(TIMEUNIT_MSEC EQ timeSpec->timeUnit)
   return (timeSpec->duration * 1000);

  if(TIMEUNIT_SEC EQ timeSpec->timeUnit)
   return (timeSpec->duration * 1000 * 1000);

  if(TIMEUNIT_MINUTE EQ timeSpec->timeUnit)
  {
    if( MAX_MINUTES_ALLOWED_FOR_USEC_GRAN < timeSpec->duration )
    {
      logError("%d (%s) < %d", MAX_MINUTES_ALLOWED_FOR_USEC_GRAN, "MAX_MINUTES_ALLOWED_FOR_USEC_GRAN", timeSpec->duration);
      return 0;
    }
    return (timeSpec->duration * 1000 * 1000 * 60);
  }

  BS_ERROR_UNKNOWN_TIMEER_UNIT(timeSpec->timeUnit );
  return 0;
}

Duratn getDurationInSec(
  IN TimeSpec *timeSpec
  )
{
  if(TIMEUNIT_SEC EQ timeSpec->timeUnit)
   return (timeSpec->duration);

  if(TIMEUNIT_MINUTE EQ timeSpec->timeUnit)
  {
    if( MAX_MINUTES_ALLOWED_FOR_SEC_GRAN < timeSpec->duration )
    {
      logError("%d (%s) < %d", MAX_MINUTES_ALLOWED_FOR_SEC_GRAN, "MAX_MINUTES_ALLOWED_FOR_SEC_GRAN", timeSpec->duration);
      return 0;
    }
    return (timeSpec->duration * 60);
  }

  BS_ERROR_UNKNOWN_TIMEER_UNIT(timeSpec->timeUnit );
  return 0;
}

/**
 * @param timerId Timer ID identifies a timer uniquely in a given process
 * @details This function identifies whether timerId is valid
 */
inline TBOOL isTimerIdValid(
  IN TimerId timerId
  )
{
  return (timerId >= MIN_TIMER_ID && timerId < MAX_TIMER_ID ? TRUE : FALSE);
}

/**
 * @param ctx The complete context of a timer
 * @details This function identifies whether timer with the timerId is allocated,
 * This avoids duplicate allocation of a timer
 */
inline TBOOL isTimerAllocated(
  IN TimerCtx *ctx
  )
{
  return (ctx->isAllocated);
}

/**
* @param ctx The complete context of a timer
* @details 
* This function identifies whether timer with the given ctx is running
*/
inline TBOOL isTimerRunning(
  IN TimerCtx *ctx
  )
{
  return (ctx->ticksRemaining >0);
}

/**
 * @param   desc TimerDesc fill the timer parameters
 * @details This function allocates a timer with the given timer description
 */
TimerId allocTimer(
  IN TimerDesc *desc
  )
{
  TimerId i;
  TimerCtx *ctx;

  for(i=MIN_TIMER_ID; i<MAX_TIMER_ID; i++)
  {
    ctx = getTimerCtx(i);
    if(FALSE EQ isTimerAllocated(ctx))
    {
      ctx->isAllocated = TRUE;
      ctx->timerDesc   = *desc;
      logTmr("Alloc timerId=%d type=%s du=%d %s, UserInfo=%x", ctx->timerId,\
          timerNameStr(desc->timerType), desc->timeSpec.duration, timeUnitStr(desc->timeSpec.timeUnit), desc->userInfo );
      return ctx->timerId;
    }
  }
  BS_ERROR_ALL_TIMERS_IN_USE(__FUNCTION__, MAX_TIMER_ID - MIN_TIMER_ID );
  return NULL_TIMER_ID;
}

/**
 * @param ctx TimerCtx of timer
 * @details This function deallocates a timer with the ctx TimerContext
 */
void deallocTimer(
  FR TimerCtx *ctx
  )
{
  if(!ctx) return;
  ctx->isAllocated    = FALSE;
  ctx->ticksRemaining = 0;
  logTmr("Dealloc timerId=%d, type=%s du=%d %s UserInfo=%x", ctx->timerId,
           timerNameStr(ctx->timerDesc.timerType), ctx->timerDesc.timeSpec.duration, timeUnitStr(ctx->timerDesc.timeSpec.timeUnit),
           ctx->timerDesc.userInfo );
  memset(&ctx->timerDesc, 0, sizeof(TimerDesc));
}

/** @addtogroup imc_api_fun
 * @param timerDesc TimerDesc fill the timer parameters
 * @details This function creates a timer with the given timer description,
 * This is a wrapper over allocTimer
 * @{
 */
TimerId createTimer(
  IN TimerDesc *timerDesc
  )
{
  return allocTimer(timerDesc);
}

/**
 * @param id Timer ID
 * @details This function starts the timer identified by TimerId
 */
TBOOL startTimer(
  IN TimerId id
)
{
  Duratn        clockInterval;
  Duratn        timerDuration;
  TimerServiceCtx *modCtx = getTimerServiceCtx();
  TimerCtx        *ctx    = getTimerCtx(id);

  if((ctx && isTimerAllocated(ctx)) EQ FALSE)
  {
    BS_ERROR_TIMER_NOT_ALLOCATED(__FUNCTION__, id);
    return FALSE;
  }

  if( TIMEUNIT_SEC <=  modCtx->clock.timeUnit )
  {
    clockInterval   = getDurationInSec(&modCtx->clock);
    timerDuration   = getDurationInSec(&ctx->timerDesc.timeSpec);
    ctx->ticksRemaining = timerDuration / clockInterval;
  }
  else
  {
    clockInterval   = getDurationInUsec(&modCtx->clock);
    timerDuration   = getDurationInUsec(&ctx->timerDesc.timeSpec);
    ctx->ticksRemaining = timerDuration / clockInterval;
  }
  if(0 EQ ctx->ticksRemaining)
  {
    logError("Trying to start timer of 0 duration, clockIntervalUsec=%d, timerDurationUsec=%d, Timer NOT started.",
      clockInterval,
      timerDuration);
    return FALSE;
  }
  /* Enable below logs if required */
/*  logTmr("started timerId=%d, clockIntervalUsec=%d, timerDurationUsec=%d, ticks=%d",
      ctx->timerId,
      clockInterval,
      timerDuration,
      ctx->ticksRemaining); */
  return TRUE;
}

/**
 * @param id Timer ID
 * @details This function stops the timer identified by TimerId
 */
void stopTimerInst(
     TimerId id
     )
{
  TimerCtx *ctx = getTimerCtx(id);
  if(isTimerAllocated(ctx) EQ FALSE)
  {
    logError("%s called but Timer Id=%d is not allocated ",__FUNCTION__, id );
    return;
  }
  if(isTimerRunning(ctx) EQ FALSE)
  {
    logTmr("%s called but TimerId=%d is not running", __FUNCTION__, id);
  }
  deallocTimer(ctx);
}
/** @} */

/**
 * @param id Timer ID
 * @details This function Checks Timer Running Status
 */
TBOOL checkTimerStatus(
  IN TimerId id
  )
{
  TimerCtx *ctx = getTimerCtx(id);
  if(isTimerAllocated(ctx) EQ FALSE)
  {
    logError("%s called but Timer Id=%d is not allocated ",__FUNCTION__, id );
    return FALSE;
  }
  if(isTimerRunning(ctx) EQ FALSE)  return FALSE;
  else return TRUE;

}

/**
 * @param   result The difference between the start time of timer and current time is placed here
 * @param   start  The time when the timer is started.
 * @param   curTime Time at the reception of SIGALRM
 * @details Taken from GNU, this is not used anymore
 * Subtract the `struct timeval' values X and Y, storing the result in RESULT.
 * Return 1 if the difference is negative, otherwise 0.
 * Store the time when the timer is started, get the time when the SIGALRM is received
 * compute the difference and place the difference in result. if the difference is >=0 the
 * timer with that timerId is considered expired
 */
int timeval_subtract (
  struct timeval *result,
  struct timeval *start,
  struct timeval *curTime
)
{
  /* Perform the carry for the later subtraction by updating y. */
  if (start->tv_usec < curTime->tv_usec)
  {
    int nsec = (curTime->tv_usec - start->tv_usec) / 1000000 + 1;
    curTime->tv_usec -= 1000000 * nsec;
    curTime->tv_sec += nsec;
  }
  if (start->tv_usec - curTime->tv_usec > 1000000) {
    int nsec = (start->tv_usec - curTime->tv_usec) / 1000000;
    curTime->tv_usec += 1000000 * nsec;
    curTime->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
  result->tv_sec = start->tv_sec - curTime->tv_sec;
  result->tv_usec = start->tv_usec - curTime->tv_usec;

  /* Return 1 if result is negative. */
  return start->tv_sec < curTime->tv_sec;
}

/**
 * @param   signum SIGALRM is expected to this function
 * @details This function is invoked when SIGALRM is received by a process from OS
 */
void timerHandler (
  int signum
  )
{
  /* Another clock interval elapsed */
  UNUSED(signum);
  TimerId          i;
  TimerServiceCtx *modCtx;
  TimerCtx        *ctx;
  MsgTimeoutInd    msgInd;

  modCtx = getTimerServiceCtx();
  for(i=MIN_TIMER_ID; i<MAX_TIMER_ID; i++)
  {
    ctx = getTimerCtx(i);
    if(isTimerRunning(ctx) EQ TRUE)
    {
      ctx->ticksRemaining--;
      if(ctx->ticksRemaining EQ 0)
      {
	/* Timer expiry: inform user */
        msgInd.timerDesc = ctx->timerDesc;
        msgInd.timerId   = ctx->timerId;
        logTimeout("Timeout timerId=%d", ctx->timerId);
        if(ctx->timerExpiryCallBackFunc EQ NULL)
        {
	    createAndSendMsg(ctx->timerDesc.timeoutMsgId,
                             (UINT8*)&msgInd,
                             sizeof(msgInd));
        }
        else
        {
          ctx->timerExpiryCallBackFunc(i, ctx->paramPassedToTimerExpiryCallbackFunc);
        }
        deallocTimer(ctx);
      }
    }
  }
}

/**
 * @details This function is the start function of timer routine, this is invoked at module start
 * When a module is registered with framework a check is done to verify whether to start timer service or not,
 * if timer service is required then only this function is invoked.
 * @param  tick
 *         It is the timer resolution i.e. duration at which timers will tick.
 *         All timers will be a multiple of this number.
 *         So, if the tick is 2 seconds, timers can be 2, 4, 6, 8, ... seconds.
 *         If an application runs timers in seconds, set it to 1 sec. 
 *         Setting this is important as it reduces the CPU usage of the process.
 */
void timerServiceInit(
  IN TimeSpec tick
  )
{
  TimerServiceCtx *modCtx = getTimerServiceCtx();
  TimerId i;
  TimerCtx *ctx;
  struct sigaction sa;
  struct itimerval timer;

  memset(&timer, 0, sizeof(struct itimerval));
  memset(modCtx, 0, sizeof(TimerServiceCtx));
  modCtx->clock = tick; /* Get ticks from OS i.e. SIGALRM at this rate */
  
  for(i=MIN_TIMER_ID; i<MAX_TIMER_ID; i++)
  {
    ctx = getTimerCtx(i);
    memset(ctx, 0, sizeof(TimerCtx));
    ctx->isAllocated = FALSE;
    ctx->timerId = i; /* Must never be changed again */
  }

  /* OS can give a basic clock to framework in form of SIGALRM. 
   * Register for alarm signal with OS
   */
  memset (&sa, 0, sizeof (sa));
  sa.sa_handler = &timerHandler;
  sigaction (SIGALRM, &sa, NULL);
  siginterrupt( SIGALRM, 0);

  /* usec value cannot be greater than 1 sec. see setitimer man page */

  if (TIMEUNIT_SEC <= tick.timeUnit)
  {
    timer.it_value.tv_sec = tick.duration;
    timer.it_interval.tv_sec = tick.duration;
    logInfo("clock will tick at %ld seconds", timer.it_value.tv_sec);
  }
  else
  {
    timer.it_value.tv_usec = getDurationInUsec(&modCtx->clock);
    timer.it_interval.tv_usec = getDurationInUsec(&modCtx->clock);
    logInfo("clock will tick at %ld usec", timer.it_value.tv_usec);
  }

  /* Start a virtual timer.  It counts down whenever this process is executing
   * MS:18May2010: TODO: comment does not match with actual usage. 
   *               What do we want: real or virtual?
   */
  if(setitimer (ITIMER_REAL, &timer, NULL) EQ -1)
  {
    logFatalErrorAndExit(errno, "could not start timer service: strerror=%s", strerror(errno));
  }
}

/**
 * @param desc TimerDesc fill the timer parameters
 * @details This function creates a timer with the given timer description and
 *          starts the timer
 */
TimerId createAndStartTimer(
    IN TimerDesc *desc,
    TimerExpiryCallBackFunc timerExpiryCallBackFunc,
    void *paramPassedToTimerExpiryCallbackFunc
    )
{
  TimerId id = createTimer(desc);
  TimerCtx        *ctx;

  /*Fill the Callback function and moduleCtx into TimerCtx of TimerId */
  ctx = getTimerCtx(id);
  ctx->timerExpiryCallBackFunc = timerExpiryCallBackFunc;
  ctx->paramPassedToTimerExpiryCallbackFunc = paramPassedToTimerExpiryCallbackFunc;

  if(id NOTEQ NULL_TIMER_ID) startTimer(id);
  return id;
}

/**
 * @details This function will return the current time.
 */
time_t getCurrentTime()
{
  return time(NULL);
}

/**
 * @param firstTime time when the process was started.
 * @details This function will return the uptime time.
 */
UINT32 getUpTime(
  IN time_t  firstTime
  )
{
  time_t  timeNow = time(NULL);
  return ((UINT32)(timeNow - firstTime));
}

/**
 * @param   none
 * @details This function will stop all the timers running this module
 */
void stopallTimers(void)
{
  /* Another clock interval elapsed */
  TimerId          i;
  TimerServiceCtx *modCtx;
  TimerCtx        *ctx;

  modCtx = getTimerServiceCtx();
  for(i=MIN_TIMER_ID; i<MAX_TIMER_ID; i++)
  {
    ctx = getTimerCtx(i);
    if(isTimerRunning(ctx) EQ TRUE)
    {
      stopTimerInst(ctx->timerId);
    }
  }
}
