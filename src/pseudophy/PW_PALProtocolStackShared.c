/*******************************************************************************
* Copyright: (C) 2009 Polaris Networks. All Rights Reserved.                   *
*    No part of this source code may be reproduced, transmitted, transcribed   *
*    or translated into any language without the written permission of         *
*    Polaris Networks or its suppliers or affiliate companies. To obtain this  *
*    permission write to the attention of Legal counsel, Polaris Networks,     *
*    75 Robbins Road, MA 02421 United States.                                  *
*******************************************************************************/
#include "PW_PALProtocolStackShared.h"

#if defined(LINUX)

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

int UTILGetCurrentTimeOfDay(size_t* plSeconds, size_t* plUseconds)
{
    struct timeval sTimeVal;
    gettimeofday(&sTimeVal, NULL);
    if (plSeconds != NULL)
    {
        *plSeconds = sTimeVal.tv_sec;
    }
    if (plUseconds != NULL)
    {
        *plUseconds = sTimeVal.tv_usec;
    }
    return 0;
}
#endif
