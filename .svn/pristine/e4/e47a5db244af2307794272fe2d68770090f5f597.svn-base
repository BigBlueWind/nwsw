
#ifndef __TIMER_EXT_H__
#define __TIMER_EXT_H__

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef __timeval__
#define __timeval__

typedef struct _timeval_t
{
     long tv_sec;     /* seconds */
     long tv_usec;    /* and microseconds */
} timeval;

#endif

/* Type of the second argument to `getitimer' and
   the second and third arguments `setitimer'.  */
typedef struct _itimerval_t
{ 
  timeval it_interval;
   timeval it_value;
} itimerval;

/*
 * Names of the interval timers, and structure
 * defining a timer setting:
 */
#define	ITIMER_REAL		0
#define	ITIMER_VIRTUAL		1
#define	ITIMER_PROF		2

/* Set the timer WHICH to *NEW.  If OLD is not NULL,
   set *OLD to the old value of timer WHICH.
   Returns 0 on success, -1 on errors.  */
extern int setitimer (int _which,
		      const itimerval *_new,
		      itimerval *_old);

#ifdef  __cplusplus
}
#endif

#endif

