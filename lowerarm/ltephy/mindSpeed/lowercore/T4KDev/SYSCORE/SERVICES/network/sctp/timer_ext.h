
#ifndef __TIMER_EXT_H__
#define __TIMER_EXT_H__

#ifdef  __cplusplus
extern "C" {
#endif

typedef unsigned int time_t;


struct timeval
{
     long tv_sec;     /* seconds */
     long tv_usec;    /* and microseconds */
};

/* Type of the second argument to `getitimer' and
   the second and third arguments `setitimer'.  */
struct itimerval
{ 
  struct timeval it_interval;
   struct timeval it_value;
};

/*
 * Names of the interval timers, and structure
 * defining a timer setting:
 */

//struct tm * localtime(time_t *t);
int _gettimeofday(struct timeval *_time, void* timezone);


#define	ITIMER_REAL		0
#define	ITIMER_VIRTUAL		1
#define	ITIMER_PROF		2


# define timerisset(tvp)	((tvp)->tv_sec || (tvp)->tv_usec)
# define timerclear(tvp)	((tvp)->tv_sec = (tvp)->tv_usec = 0)
# define timercmp(a, b, CMP) 						      \
  (((a)->tv_sec == (b)->tv_sec) ? 					      \
   ((a)->tv_usec CMP (b)->tv_usec) : 					      \
   ((a)->tv_sec CMP (b)->tv_sec))
# define timeradd(a, b, result)						      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec + (b)->tv_sec;			      \
    (result)->tv_usec = (a)->tv_usec + (b)->tv_usec;			      \
    if ((result)->tv_usec >= 1000000)					      \
      {									      \
	++(result)->tv_sec;						      \
	(result)->tv_usec -= 1000000;					      \
      }									      \
  } while (0)
# define timersub(a, b, result)						      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;			      \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;			      \
    if ((result)->tv_usec < 0) {					      \
      --(result)->tv_sec;						      \
      (result)->tv_usec += 1000000;					      \
    }									      \
  } while (0)


#ifdef  __cplusplus
}
#endif

#endif

