#include "timer_ext.h"

/* Set the timer WHICH to *NEW.  If OLD is not NULL,
   set *OLD to the old value of timer WHICH.
   Returns 0 on success, -1 on errors.  */
int setitimer (int _which,
		      const itimerval *_new,
		      itimerval *_old)
{
  return 0;
}

