/********************************************************************16**

                         (c) COPYRIGHT 2009 by 
                         Continuous Computing Corporation.
                         All rights reserved.

     This software is confidential and proprietary to Continuous Computing 
     Corporation (CCPU).  No part of this software may be reproduced,
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written Software License 
     Agreement between CCPU and its licensee.

     CCPU warrants that for a period, as provided by the written
     Software License Agreement between CCPU and its licensee, this
     software will perform substantially to CCPU specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  CCPU also warrants 
     that has the corporate authority to enter into and perform under the   
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     CCPU MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL CCPU BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between CCPU and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact CCPU at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    Continuous Computing Corporation
                    9450, Carroll Park Drive
                    San Diego, CA-92121, USA

                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388

                    Email: support@trillium.com
                    Web: http://www.ccpu.com

*********************************************************************17*/

/************************************************************************
 
     Name:     LTE MAC Convergence Layer
  
     Type:     C source file
  
     Desc:     C source code for Entry point fucntions
  
     File:     ys_pal_li.c
  
     Sid:      
  
     Prg:      hsingh 
  
**********************************************************************/

/** @file ys_pal_li.c
@brief This module acts as an interface handler for upper interface and 
manages Pst and Sap related information for upper interface APIs.
*/

/* header include files -- defines (.h) */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#ifndef USE_UDP
#include <netinet/sctp.h>
#endif
#include <arpa/inet.h>
#include <sys/time.h>
#include <pthread.h>

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_lte.h"
#include "ctf.h"           /* CTF defines */
#include "lys.h"           /* layer management typedefs for CL */
#include "tfu.h"
#include "ys_ms_err.h"        /* YS error defines */
#include "ys_ms.h"

/* Silicon Includes */
#include "LtePhyL2Api.h"
#include "apidefs.h"
#include "ctrlmsg.h"
#include "maciface.h"

/* header/extern include files (.x) */
#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_lte.x"
#include "ctf.x"           /* CTF types */
#include "lys.x"           /* layer management typedefs for CL */
#include "tfu.x"
#include "ys_ms.x" 
#include "ys_ms_pal.x"

/* header/extern include files (.x) */
/* header include files (.h) */

/*ys004.102 :  Merged MSPD code with phy 1.7 */
#define YS_MAX_SLOTS     32

#define YS_TTI_COUNT_VAL 1

PUBLIC  UtilCircQ      circQueue;
PUBLIC  YsStCb         ysStCb;

/* OSAL Definitions */
extern OSAL_HEAP OsalGlobalHeap;
OSAL_MEMARRAY MsgArray;
OSAL_PADDRESS MsgArrayMem;
/* End of OSAL Definitions */


#ifndef USE_UDP
PRIVATE struct sockaddr_in RemAddr;
#endif
PRIVATE Void *ysPalRcvrActvTsk ARGS ((Void *tskPtr));


#ifdef ANSI
PUBLIC S16 ysMsgDispatchInit
(
Void
)
#else
PUBLIC S16 ysMsgDispatchInit()
#endif
{
   if (OsalAllocHeap(&OsalGlobalHeap, &MsgArrayMem, MSG_MAXSIZE * MSG_MAXMSG) != SUCCESS)
   {
      YS_DBG_ERR((_ysp, "Failed to OSAL alloc heap\n"));
      RETVALUE(RFAILED);
   }

   OsalCreateMemArray(&MsgArray, MsgArrayMem, MSG_MAXSIZE * MSG_MAXMSG, MSG_MAXSIZE);

   RETVALUE(ROK);
}

#ifdef USE_SSI_TMR

#ifdef ANSI
PUBLIC Void ysStartTmr
(
S16       tmrEvnt,
U32       tmrVal,
Ptr       cb
)
#else
PUBLIC Void ysStartTmr()
S16       tmrEvnt;
U32       tmrVal;
Ptr       cb;
#endif
{
   CmTmrArg    arg;

   TRC2(ysStartTmr)

   arg.tqCp   = &(ysStCb.tmrTqCp);
   arg.tq     = ysStCb.tmrTq;
   arg.cb     = (PTR)cb;
   arg.evnt   = tmrEvnt;
   arg.wait   = tmrVal;
   arg.max    = 1;
   arg.timers = &(ysStCb.tmr);
   arg.tNum   = NOTUSED;
   cmPlcCbTq(&arg);

   RETVOID;

}

#ifdef ANSI
PUBLIC Void ysPrcTmrExpiry
(
Ptr      cb,
S16      tmrEvnt
)
#else
PUBLIC Void ysPrcTmrExpiry (cb, tmrEvnt)
Ptr      cb;
S16      tmrEvnt;
#endif
{
   TRC2(ysPrcTmrExpiry)

   switch (tmrEvnt)
   {
      case YS_TTI_TMR_EVNT:
         PhySendToMacMsg(PHY_TXSTART_IND, 1, NULL, 0);
         (Void) ysStartTmr (YS_TTI_TMR_EVNT, YS_TTI_TMR_VAL, (PTR) NULLP);
         break;
   }

   RETVOID;
}



#ifdef ANSI
PUBLIC S16 ysActvTmr
(
Void
)
#else
PUBLIC S16 ysActvTmr()
#endif
{
   TRC2(ysActvTmr)

   cmPrcTmr(&ysStCb.tmrTqCp, ysStCb.tmrTq, (PFV) ysPrcTmrExpiry);

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 ysPalInitTti
(
Void
)
#else
PUBLIC S16 ysPalInitTti()
#endif
{
   U16    idx;

   TRC2(ysPalInitTti);

   for (idx=0; idx < YS_TQ_SIZE; idx++)
   {
      ysStCb.tmrTq[idx].first = NULLP;
      ysStCb.tmrTq[idx].tail  = NULLP;
   }
   ysStCb.tmrTqCp.nxtEnt = 0;
   ysStCb.tmrTqCp.tmrLen = YS_TQ_SIZE;
   
   cmInitTimers(&(ysStCb.tmr), 1);

   if ((SRegTmr(ysCb.ysInit.ent, ysCb.ysInit.inst,
			(S16)YS_TIME_RES, ysActvTmr)) != ROK)
   {
      YS_DBG_ERR((_ysp, "Unable to Register TTI timer\n"));
   }

   ysStartTmr (YS_TTI_TMR_EVNT, YS_TTI_TMR_VAL, (PTR) NULLP);

   RETVALUE (ROK);
}

#else /* if USE_SSI_TMR is not defined */


#ifdef ANSI
PRIVATE Void * ysPalTtiTmrTsk
(
Void
)
#else
PRIVATE Void * ysPalTtiTmrTsk()
#endif
{
   PRIVATE struct timespec interval, remInterval;
   PRIVATE struct timespec startTm, wakeup;

   Buffer *tempBuf;
   Pst    pst;
   S16    ret;
   PRIVATE U16 ttiCnt = 0, ttiEvnt;

   PRIVATE unsigned long diff_time;
   PRIVATE struct timespec entryRx, exitRx;

   clock_gettime(CLOCK_REALTIME, &entryRx); 
   clock_gettime(CLOCK_REALTIME, &wakeup);
   clock_gettime(CLOCK_REALTIME, &startTm);
   while(1)
   {
       remInterval.tv_sec = 0;
       remInterval.tv_nsec = 0;
       interval.tv_sec = 0;
       startTm.tv_nsec += YS_TTI_TMR_VAL;
       if (startTm.tv_nsec > 1000000000)
       {
          startTm.tv_nsec -= 1000000000;
          startTm.tv_sec += 1;
       }
       interval.tv_nsec = ((startTm.tv_sec * 1000000000 + startTm.tv_nsec) -
                           (wakeup.tv_sec *1000000000 + wakeup.tv_nsec));
       while((nanosleep(&interval, &remInterval))!=0)
       {
          interval = remInterval;
          remInterval.tv_sec = 0;
          remInterval.tv_nsec = 0; 
       }
       clock_gettime(CLOCK_REALTIME, &wakeup); 
       ttiCnt+=YS_TTI_COUNT_VAL;

      if(ttiCnt>=1000)
      {
         clock_gettime(CLOCK_REALTIME, &exitRx);
         if (exitRx.tv_sec - entryRx.tv_sec == 0)
         {
            diff_time = exitRx.tv_nsec - entryRx.tv_nsec;
         }
         else
         {
            diff_time = (exitRx.tv_sec * 1000000000 + exitRx.tv_nsec) - (entryRx.tv_sec *1000000000 + entryRx.tv_nsec);
         }
         /* printf("TTI Task: %d TTIs generated in %luns, %lums\n", ttiCnt, diff_time, ((float)diff_time/(float)(1000000))); */
         printf("TTI Task: %d TTIs generated in %luns, %lums\n", ttiCnt, diff_time, (diff_time/(1000000)));
         diff_time = 0;
         ttiCnt = 0;
         clock_gettime(CLOCK_REALTIME, &entryRx); 
         memset(&exitRx, 0, sizeof(struct timespec)); 
      }

      ttiEvnt = YS_TTI_COUNT_VAL; 
      while(ttiEvnt)
      {
         ret = SGetMsg(ysCb.ysInit.region, ysCb.ysInit.pool, &tempBuf);
         if (ret != ROK)
         {
            YS_DBG_ERR((_ysp, "SGetMsg failed\n"));
            continue;
         } 
         /* SPstTsk(&pst, tempBuf); */
         PhySendToMacMsg(PHY_TXSTART_IND, 1, NULL, 0);
         ttiEvnt--;
      }
   }
   pthread_exit(NULLP);
}

#ifdef ANSI
PUBLIC S16 ysPalInitTtiTmrTsk
(
Void
)
#else
PUBLIC S16 ysPalInitTtiTmrTsk()
#endif
{
   pthread_t tId;
   pthread_attr_t attr;
   cpu_set_t cpuSet; /* Enable if required for setting the affinity */

   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr, (size_t)NULLD);
   pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

   if ((pthread_create(&tId, &attr, ysPalTtiTmrTsk, (Ptr)NULL)) != 0)
   {
      pthread_attr_destroy(&attr);

      YS_DBG_ERR((_ysp, "Failed to create TTI generator thread\n"));

      RETVALUE(RFAILED);
   }

   /* Following is applicable only for Linux machine 
      from Intel. Core number used here can be changed
      accordingly, as expected on the machine of interest.
      This portion of code can be disabled if not required. */

   RETVALUE(ROK);
}


#endif /* USE_SSI_TMR */


#ifdef ANSI
PUBLIC S16 ysPalInitRcvrTsk
(
Void
)
#else
PUBLIC S16 ysPalInitRcvrTsk()
#endif
{
   pthread_t tId;
   pthread_attr_t attr;

   pthread_attr_init(&attr);
   pthread_attr_setstacksize(&attr, (size_t)NULLD);
   pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

   if ((pthread_create(&tId, &attr, ysPalRcvrActvTsk, (Ptr)NULL)) != 0)
   {
      pthread_attr_destroy(&attr);
 
      YS_DBG_ERR((_ysp, "Failed to create thered\n"));

      RETVALUE(RFAILED);
   }

   /* initialize the queue */
   utilCircQMakeNull(&circQueue);

   pthread_attr_destroy(&attr);

   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE Void *ysPalRcvrActvTsk
(
Void *tskPtr
)
#else
PRIVATE Void *ysPalRcvrActvTsk(tskPtr)
Void *tskPtr;
#endif
{
#ifdef USE_UDP
   struct sockaddr_in srcAddr;  /* local Internet address/port */
   struct sockaddr_in remSockAddr;
   U32    remAddrLen = 0;
   S32    recvLen;
   U8     buf[YS_MAX_SLOTS][YS_MAX_RCVD_BUF_LEN];
   Buffer *dumBuf = NULLP;
   S16    ret;
   Pst    pst;
#if 0
   S32    slot;
   U8     temp[10] ={0};
#else
   U8     *data;
#endif

   if((ysCb.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
   {
      YS_DBG_ERR((_ysp, "Server-socket() sockfd error\n"));
      exit(1);
   }

   cmMemset((U8*)&srcAddr, 0, sizeof(srcAddr));
   srcAddr.sin_family      = AF_INET;
   srcAddr.sin_port        = htons(YS_PORT_ENB);
   srcAddr.sin_addr.s_addr = inet_addr(YS_IP_ADDR_ENB);

   if(bind(ysCb.sockfd, (struct sockaddr *)&srcAddr,
                              sizeof(struct sockaddr)) == -1)
   {
      YS_DBG_ERR((_ysp, "Server-bind() error\n"));
      exit(1);
   }

   /*loop forever*/
   while(1)
   {
      /* clear the structure */
      cmMemset((U8*)&remSockAddr, 0, sizeof(remSockAddr));

#if 0
      slot = utilCircQEnqueue(&circQueue);
#else
      data = MsgAllocBuffer(YS_MS_MSG_COMMON);
#endif

      {
#if 0
         if((recvLen = recvfrom(ysCb.sockfd, circQueue.elements[slot].data, YS_MAX_RCVD_BUF_LEN, 0, 
                            (struct sockaddr *)&remSockAddr, 
                            (socklen_t *)&remAddrLen)) == -1)
#else
         if((recvLen = recvfrom(ysCb.sockfd, data, YS_MAX_RCVD_BUF_LEN, 0, 
                            (struct sockaddr *)&remSockAddr, 
                            (socklen_t *)&remAddrLen)) == -1)
#endif

         {
            YS_DBG_ERR((_ysp, "Server-recvfrom() error\n"));
            continue;
         }
      }

      /* copy data to a message structure */
      ret = SGetMsg(ysCb.ysInit.region, ysCb.ysInit.pool, &dumBuf);
      if (ret != ROK)
      {
         YS_DBG_ERR((_ysp, "SGetMsg failed\n"));
         continue;
      }

#if 0
      ret = SAddPstMsgMult(temp, 1, dumBuf);
      if (ret != ROK)
      {
         YS_DBG_ERR((_ysp, "SAddPstMsgMult failed\n"));
         continue;
      }
#else
      SAddPstMsgMult((Data*)&recvLen, 4, dumBuf);
      SAddPstMsgMult((Data*)&data, 4, dumBuf);
#endif

      pst.dstProcId = SFndProcId();
      pst.srcProcId = SFndProcId();
      pst.dstEnt = ENTTF;
      pst.srcEnt = ENTTF;
      pst.srcInst = 0;
      pst.dstInst = 0;
      pst.route = RTESPEC;
      pst.prior = PRIOR0;
      pst.region = ysCb.ysInit.region;
      pst.pool = ysCb.ysInit.region;
      pst.selector = 0;

      SPstTsk(&pst, dumBuf);
   }

#else /* USE_UDP */

   struct sockaddr_in srcAddr;  /* local Internet address/port */
   struct sockaddr_in remAddr;
   struct sctp_event_subscribe evnts;
   struct msghdr msg[1];
   struct cmsghdr *cmsg;
   char cbuf[sizeof(*cmsg) + sizeof(struct sctp_sndrcvinfo)];
   struct iovec iov[0];
   size_t cmsglen = sizeof(*cmsg) + sizeof(struct sctp_sndrcvinfo);
   U32    str_inc = 1;
   U32    msg_flags;
   U32    remAddrLen = 0;

   S32    recvLen;
   U8     *buf;
   Buffer *dumBuf = NULLP;
   S16    ret;
   U32    opt = 0;
   Pst    pst;
#if 0
   U8     temp[10] ={0};
#else
   U8     *data;
#endif
   U32    indx;
   ssize_t nr;
  
   ysCb.accRemSock = 0;

   if((ysCb.sockfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) == -1)
   {
      YS_DBG_ERR((_ysp, "Server-socket() sockfd error\n"));
      exit(1);
   }

   if (setsockopt(ysCb.sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
   {
      YS_DBG_ERR((_ysp, "Server-setsockoption() error SOL_SOCKET\n"));
      exit(1);
   }

   cmMemset((U8*)&srcAddr, 0, sizeof(srcAddr));
   srcAddr.sin_family      = AF_INET;
   srcAddr.sin_port        = htons(YS_PORT_ENB);
   srcAddr.sin_addr.s_addr = inet_addr(YS_IP_ADDR_ENB);

   cmMemset((U8*)&RemAddr, 0, sizeof(RemAddr));
   RemAddr.sin_family = AF_INET;
   RemAddr.sin_port = htons(YS_PORT_UE);
   RemAddr.sin_addr.s_addr = inet_addr(YS_IP_ADDR_UE);

   if(bind(ysCb.sockfd, (struct sockaddr *)&srcAddr, 
                              sizeof(struct sockaddr)) == -1)
   {
      YS_DBG_ERR((_ysp, "Server-bind() error\n"));
      exit(1);
   }

   /* Set socket option to reuse the address */
   bzero(&evnts, sizeof(evnts));
   evnts.sctp_data_io_event = 1;
   /* evnts.sctp_association_event = 1; */
   if (setsockopt(ysCb.sockfd, IPPROTO_SCTP, SCTP_EVENTS, &evnts, sizeof(evnts)) < 0)
   {
      YS_DBG_ERR((_ysp, "Server-setsockoption() error for IPPROTO_SCTP\n"));
      exit(1);
   }

   if(listen(ysCb.sockfd, 2) != 0)
   {
      YS_DBG_ERR((_ysp, "Server-listen() error\n"));
      exit(1);
   }

   /*loop forever*/
   while(1)
   {
#if 0
      /* SLock(&circ_queue_lock); */
      slot = utilCircQEnqueue(&circQueue);
#else
      data = MsgAllocBuffer(YS_MS_MSG_COMMON);
#endif

      {
#if 0
         /* memset(circQueue.elements[slot].data, 0, YS_MAX_RCVD_BUF_LEN); */
	 /* SUnlock(&circ_queue_lock); */
         if((recvLen = sctp_recvmsg(ysCb.sockfd, circQueue.elements[slot].data,
                     YS_MAX_RCVD_BUF_LEN,
                     (struct sockaddr*)&remAddr, &remAddrLen,
                     &ysCb.sri, &msg_flags)) == -1)
         {
            YS_DBG_ERR((_ysp, "Server-recvfrom() error\n"));
            continue;
         }
#else
         if((recvLen = sctp_recvmsg(ysCb.sockfd, data,
                     YS_MAX_RCVD_BUF_LEN,
                     (struct sockaddr*)&remAddr, &remAddrLen,
                     &ysCb.sri, &msg_flags)) == -1)
         {
            YS_DBG_ERR((_ysp, "Server-recvfrom() error\n"));
            continue;
         }
#endif
         if (msg_flags & MSG_NOTIFICATION)
         {
            continue;
         }
      }

      /* copy data to a message structure */
      ret = SGetMsg(ysCb.ysInit.region, ysCb.ysInit.pool, &dumBuf);
      if (ret != ROK)
      {
         YS_DBG_ERR((_ysp, "SGetMsg failed\n"));
         continue;
      }

#if 0
      ret = SAddPstMsgMult(temp, 1, dumBuf);
      if (ret != ROK)
      {
         YS_DBG_ERR((_ysp, "SAddPstMsgMult failed\n"));
         continue;
      }
#else
      SAddPstMsgMult((Data*)&recvLen, 4, dumBuf);
      SAddPstMsgMult((Data*)&data, 4, dumBuf);
#endif

      pst.dstProcId = SFndProcId();
      pst.srcProcId = SFndProcId();
      pst.dstEnt = ENTTF;
      pst.srcEnt = ENTTF;
      pst.srcInst = 0;
      pst.dstInst = 0;
      pst.route = RTESPEC;
      pst.prior = PRIOR0;
      pst.region = ysCb.ysInit.region;
      pst.pool = ysCb.ysInit.region;
      pst.selector = 0;

      SPstTsk(&pst, dumBuf);
   }
#endif /* USE_UDP */ 

   pthread_exit(NULLP);
} /* end of the function */

#ifdef ANSI
PUBLIC S16 ysPalSendSelf
(
U8        *sendBuf,
U32        bufLen
)
#else
PUBLIC S16 ysPalSendSelf(sendBuf, bufLen)
U8        *sendBuf;
U32        bufLen;
#endif
{
   S32     ret;                 /* temporary return value */
   struct sockaddr_in dstAddr;  /* local Internet address/port */


   cmMemset((U8*)&dstAddr, 0, sizeof(dstAddr));
   dstAddr.sin_family      = AF_INET;
   dstAddr.sin_port        = htons(YS_PORT_ENB);
   dstAddr.sin_addr.s_addr = inet_addr(YS_IP_ADDR_ENB);

   ret = sendto(ysCb.sockfd, sendBuf, bufLen, 0, 
                (struct sockaddr *)&dstAddr, sizeof(dstAddr)); 
   if(ret == -1)
   {
      YS_DBG_ERR((_ysp, "sendto failed\n"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC Void ysPalSend2Peer
(
U8        *sendBuf,
U32        bufLen
)
#else
PUBLIC Void ysPalSend2Peer(sendBuf, bufLen)
U8        *sendBuf;
U32        bufLen;
#endif
{
   S32     ret;                 /* temporary return value */
#ifdef USE_UDP
   struct sockaddr_in remAddr;

   bzero(&remAddr, sizeof(remAddr));
   remAddr.sin_family = AF_INET;
   remAddr.sin_port = htons(YS_PORT_UE);
   remAddr.sin_addr.s_addr = inet_addr(YS_IP_ADDR_UE);

   ret = sendto(ysCb.sockfd, sendBuf, bufLen, 0,
                (struct sockaddr *)&remAddr, sizeof(remAddr));

#else
   ret = sctp_sendmsg(ysCb.sockfd, sendBuf, bufLen, (struct sockaddr*)&RemAddr,
                      sizeof(struct sockaddr_in),
                      ysCb.sri.sinfo_ppid,
                      ysCb.sri.sinfo_flags,
                      ysCb.sri.sinfo_stream, 0, 0);
#endif
   if(ret == -1)
   {
      YS_DBG_ERR((_ysp, "sendto failed\n"));
      MsgFreeBuffer((PMSGHEADER)sendBuf);
   }

}

#define utilCircQIsEmpty(q) (utilCircQAddOne(q->rear) == q->front)?TRUE:FALSE


/*******************************************************************
*
*       Fun:  utilCircQDequeue
*
*       Desc: removes an element from the front of the queue
*
*       Ret:  void
*
*       Notes:
*
*       File:  ys_ms_pal.c
*
*******************************************************************/
Void utilCircQDequeue(UtilCircQ *queue)
{
   if(utilCircQIsEmpty(queue))
   {
      printf("CL: Queue is empty\n");
      return;
   }

   queue->front = utilCircQAddOne(queue->front);
}

/*******************************************************************
*
*       Fun:  utilCircQFront
*
*       Desc: returns the rear index on which data should be queued
*
*       Ret:  pointer to the front of the queue,NULL if the queue
*             is empty
*
*       Notes:
*
*       File:  ys_ms_pal.c
*
*******************************************************************/

UtilCircQData* utilCircQFront(UtilCircQ *queue)
{
   if(utilCircQIsEmpty(queue))
   {
      printf("CL: Queue is empty\n");
      return NULL;
   }

   return(&(queue->elements[queue->front]));
}

/*******************************************************************
*
*       Fun:  utilCircQEnqueue
*
*       Desc: returns the rear index on which data should be queued
*             next. data should be copied into
*             queue->elements[retvalue from this function]
*             A gap of 3 elements is always maintained so that there
*             is no problem when the sender and receiver are on two
*             seperate threads.
*
*       Ret:  index on which to queue data, -1 the the
*             queue if already full.
*
*       Notes:
*
*       File:  op_utl_circq.c
*
*******************************************************************/

S32 utilCircQEnqueue(UtilCircQ *queue)
{
   if(utilCircQAddOne(utilCircQAddOne(utilCircQAddOne(utilCircQAddOne(queue->rear)))) == queue->front)
   {
      printf("CL: Queue is full \n");
      return -1;
   }

   queue->rear = utilCircQAddOne(queue->rear);
   return queue->rear;
}

/*******************************************************************
*
*       Fun:  utilCircQAddOne
*
*       Desc: adds one to the passed index taking into account the
*             bounds
*
*       Ret:  void
*
*       Notes:
*
*       File:  ys_ms_pal.c
*
*******************************************************************/

U32 utilCircQAddOne(U32 idx)
{
   /* 0x1F below is UTIL_CIRC_Q_MAXSIZE(currently 32) - 1 */
   return((idx & 0x01F) + 1);
}

/*******************************************************************
*
*       Fun:  utilCircQMakeNull
*
*       Desc: initializes the queue to initial state
*
*       Ret:  void
*
*       Notes:
*
*       File:  
*
*******************************************************************/

Void utilCircQMakeNull(UtilCircQ *queue)
{
   queue->front = 1;
   queue->rear = UTIL_CIRC_Q_MAXSIZE;
}


/********************************************************************30**
  
         End of file: 
  
*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/
  
/********************************************************************90**

     ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
$SID$        ---     hsingh   1. initial release.
/main/1    ys004.102  vr   1. Merged MSPD code with phy 1.7
*********************************************************************91*/
