/*--------------------------------------------------------------- 
 * Copyright (c) 1999,2000,2001,2002,2003                              
 * The Board of Trustees of the University of Illinois            
 * All Rights Reserved.                                           
 *--------------------------------------------------------------- 
 * Permission is hereby granted, free of charge, to any person    
 * obtaining a copy of this software (Iperf) and associated       
 * documentation files (the "Software"), to deal in the Software  
 * without restriction, including without limitation the          
 * rights to use, copy, modify, merge, publish, distribute,        
 * sublicense, and/or sell copies of the Software, and to permit     
 * persons to whom the Software is furnished to do
 * so, subject to the following conditions: 
 *
 *     
 * Redistributions of source code must retain the above 
 * copyright notice, this list of conditions and 
 * the following disclaimers. 
 *
 *     
 * Redistributions in binary form must reproduce the above 
 * copyright notice, this list of conditions and the following 
 * disclaimers in the documentation and/or other materials 
 * provided with the distribution. 
 * 
 *     
 * Neither the names of the University of Illinois, NCSA, 
 * nor the names of its contributors may be used to endorse 
 * or promote products derived from this Software without
 * specific prior written permission. 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE CONTIBUTORS OR COPYRIGHT 
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
 * ________________________________________________________________
 * National Laboratory for Applied Network Research 
 * National Center for Supercomputing Applications 
 * University of Illinois at Urbana-Champaign 
 * http://www.ncsa.uiuc.edu
 * ________________________________________________________________ 
 *
 * headers.h
 * by Mark Gates <mgates@nlanr.net>
 * -------------------------------------------------------------------
 * All system headers required by iperf.
 * This could be processed to form a single precompiled header,
 * to avoid overhead of compiling it multiple times.
 * This also verifies a few things are defined, for portability.
 * ------------------------------------------------------------------- */

#ifndef HEADERS_H
#define HEADERS_H

#define IPERF_SERVER    1
/* standard C headers */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#include "netdb.h"
#include "timer_ext.h"

//    #include <netinet/in.h>
//    #include <netinet/tcp.h>

//    #include <arpa/inet.h>   /* netinet/in.h must be before this on SunOS */


//#undef IPERF_REPORT
#define IPERF_REPORT 1

#undef IPERF_DELAY
//#define IPERF_DELAY 1

#undef IPERF_HAVE_SELECT
//#define IPERF_HAVE_SELECT 1



/*--------- Address Families --------*/
#define  AF_INET                    2 
#define  AF_INET6                   23 

#define SOCKET_ERROR   -1
#define INVALID_SOCKET -1
#define	EINTR           4	/* Interrupted system call */
#define LOG_DAEMON      (3<<3)  /* system daemons */

// Rationalize stdint definitions and sizeof, thanks to ac_create_stdint_h.m4
// from the gnu archive
typedef unsigned long long int max_size_t;

typedef int pid_t;

/* in case the OS doesn't have these, we provide our own implementations */
#include "utils.h"
#include "inet_aton.h"
#include "snprintf.h"

#ifndef SHUT_RD
    #define SHUT_RD   0
    #define SHUT_WR   1
    #define SHUT_RDWR 2
#endif // SHUT_RD

#endif /* HEADERS_H */






