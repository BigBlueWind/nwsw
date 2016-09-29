#include "4gmx.h"
#include <string.h>
#include "appids.h"
#include "hal.h"
#include "appprintf.h"
#include "timer_ext.h"

#ifdef __cplusplus
extern "C" {
#endif

int _gettimeofday(timeval *_time, void* timezone)
{
	unsigned int ms_time;	
	
	ms_time = MxGetTime();
	
	_time->tv_sec = ms_time / 1000;
	_time->tv_usec = (ms_time % 1000) * 1000;

	return 0; 
}


void _delay_loop(unsigned long usec)
{
	MxDelayTicks(usec * 150);
}

void _delay(unsigned long msec)
{
	_delay_loop(msec * 1000);
}

#ifdef __cplusplus
} /* end extern "C" */
#endif
