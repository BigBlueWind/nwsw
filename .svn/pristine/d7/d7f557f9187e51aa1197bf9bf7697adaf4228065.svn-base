/*******************************************************************************
* Copyright: (C) 2009 Polaris Networks. All Rights Reserved.                   *
*    No part of this source code may be reproduced, transmitted, transcribed   *
*    or translated into any language without the written permission of         *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this  *
*    permission write to the attention of Legal counsel, Polaris Networks,     *
*    75 Robbins Road, MA 02421 United States.                                  *
*******************************************************************************/
#ifndef __PW_PAL_PROTOCOLSTACK_SHARED_H__
#define __PW_PAL_PROTOCOLSTACK_SHARED_H__

typedef unsigned char           ubyte;
typedef unsigned short int      ubyte2;
typedef unsigned int            ubyte4;
typedef unsigned long long      ubyte8;

/*typedef char                  byte; */
typedef signed short int        byte2;
typedef signed int              byte4;
typedef signed long long        byte8;

typedef unsigned int            uint;

typedef unsigned char           PN_BOOL;
typedef unsigned char           u8;

#define REVERSE_4_BYTES_INT(val) \
                ( (val & 0xff000000) >> 24 ) | \
                ( (val & 0x00ff0000) >> 8 ) | \
                ( (val & 0x0000ff00) << 8 ) | \
                ( (val & 0x000000ff) << 24 )

#ifdef LINUX

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>

int UTILGetCurrentTimeOfDay(size_t* plSeconds, size_t* plUseconds);
#endif

#endif
