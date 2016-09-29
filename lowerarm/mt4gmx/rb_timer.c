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
/********************************************************************20**

     Name:

     Type:     C File.

     Desc:    Supervisor is used to communicate with upper arm
              Using shared memory

     File:     rb_timer.c

     Sid:      ju_timer.c@@/main/1 - Mon Aug 29 22:31:34 2011

     Prg:

************************************************************************/

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "rb_include.h"

#ifdef SS_4GMX_LCORE
volatile U16 tmrStarted[MAX_NUM_CORES] = {FALSE, FALSE, FALSE, FALSE};


/*
 *
 *       Fun:    rbTmrInitStart
 *
 *       Desc:   This Function calls Base Timer Initialization And  Base Timer
 *               Start functions.
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 */
#ifndef SS_TTI_TMR
#ifdef ANSI
PUBLIC S16 rbTmrInitStart
(
void *pThreadParam
)
#else
PUBLIC S16 rbTmrInitStart(pThreadParam)
void *pThreadParam;
#endif
{
   RB_PRINTF("\n[%s:%d]Initializing Timer coreId=%d\n", __func__, __LINE__, MxGetCpuID());
   rbTmrInit();
   rbStartTmr();
   return MX_OK;
}


/*
 *
 *       Fun:    rbInitTmr
 *
 *       Desc:   Initializes Base Timer
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbTmrInit
(
void
)
#else
PUBLIC S16 rbTmrInit()
#endif
{
   U32 coreID =0;
#ifdef RB_TMR_ALLCORE
   int i =0;
#endif

   TRC0(rbTmrInit);

   coreID = MxGetCpuID();

   if(MX_OK != MxCreateThreadEx (coreID, (TENTRY)rbTmrHdlr, NULL, NULL, (TPRIORITY)RB_BASETMR_PRIORITY, &osCp.dep.tmrThrdID[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB063, ERRZERO, "MxCreateThreadEx Failed");
      RETVALUE(RFAILED);
   }

   if(MX_OK != MxCreateEventEx (coreID, EVENT_MODE_RESET, NULL, &osCp.dep.tmrEvntID[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB064, ERRZERO, "MxCreateEvent Failed");
      RETVALUE(RFAILED);
   }

   /*create Timer*/
   if(MX_OK != MxCreateTimerEx (coreID, TIMER_MODE_PERIOD, RB_BASE_TMR_RESOLUTION, "rbBaseTmr", osCp.dep.tmrEvntID[coreID],  &osCp.dep.tmrID[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB065, ERRZERO, "MxCreateTimerEx Failed");
      RETVALUE(RFAILED);
   }

   /* Schedule the thread by event*/
   if(MX_OK != MxScheduleThreadByEvent (osCp.dep.tmrThrdID[coreID], osCp.dep.tmrEvntID[coreID], THREAD_EVENT_AUTO_RESET))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB066, ERRZERO, "MxSheduleThreadByEvent Failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    rbTmrDeInit
 *
 *       Desc:   DeInitializes Base Timer
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbTmrDeInit
(
S16 coreID
)
#else
PUBLIC S16 rbTmrDeInit(coreID)
S16 coreID;
#endif
{
   TRC0(rbTmrDeInit);

   if(coreID < 0 || coreID >= MAX_NUM_CORES)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB067, ERRZERO, "coreID out of range");
      RETVALUE(RFAILED);
   }

   /*Remove Timer*/
   if(MX_OK != MxRemoveTimer(osCp.dep.tmrThrdID[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB068, ERRZERO, "MxRemoveTimer Failed");
      RETVALUE(RFAILED);
   }

   /*Remove Timer Event*/
   if(MX_OK != MxRemoveEvent(osCp.dep.tmrThrdID[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB069, ERRZERO, "MxRemoveEvent Failed");
      RETVALUE(RFAILED);
   }

   /*Remove Timer Thread*/
   if(MX_OK != MxRemoveThread(osCp.dep.tmrThrdID[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB070, ERRZERO, "MxRemoveThread Failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    rbStartTmr()
 *
 *       Desc:   Starts Base Timer
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbStartTmr
(
void
)
#else
PUBLIC S16 rbStartTmr()
#endif
{
   U32 coreID =0;

   TRC0(rbStartTmr);

   coreID = MxGetCpuID();

   /*starting Base Timer*/
   RB_PRINTF("%s:%d coreID:%d\n",__func__,__LINE__,coreID);

   if(MX_OK != MxStartTimer (osCp.dep.tmrID[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB071, ERRZERO, "MxStartTimer Failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    rbStopTmr
 *
 *       Desc:   Stops Base Timer
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbStopTmr
(
S16 coreID
)
#else
PUBLIC S16 rbStopTmr(coreID)
S16 coreID;
#endif
{
   TRC0(rbStopTmr);

   RB_PRINTF("%s: %d coreID=%d",__func__,__LINE__,coreID);

   if(coreID < 0 || coreID >= MAX_NUM_CORES)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB072, ERRZERO, "coreID out of range");
      RETVALUE(RFAILED);
   }

   /*Stop Timer*/
   if(MX_OK != MxStopTimer(osCp.dep.tmrID[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB073, ERRZERO, "MxRemoveTimer Failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
#endif /*SS_TTI_TMR*/

#ifndef RB_TMR_ALLCORE
#ifdef RB_CHANNEL_SVSR

/*
 *
 *       Fun:    rbInitTmrSvsr
 *
 *       Desc:   Initializes Timer supervisor channel
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 rbInitTmrSvsr
(
void
)
#else
PUBLIC S16 rbInitTmrSvsr()
#endif
{
   U32 CoreID = MxGetCpuID();
   U32 InstId = IID_CORE + CoreID;
   U32 *pCtx = &osCp.dep.pTmrCtx[CoreID];

   TRC0(rbInitTmrSvsr);

   if(MX_OK!= SvsrRegMsgHandler(InstId + (2 * MAX_NUM_CORES), rbGetTmrSvsrMsg, pCtx))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB074, ERRZERO, "SvsrRegMsgHandler Failed");
      RETVALUE(RFAILED);
   }

   tmrStarted[CoreID] = TRUE;

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    rbDeInitTmrSvsr
 *
 *       Desc:   DeInitializes Timer Supervisor channel
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 rbDeInitTmrSvsr
(
void
)
#else
PUBLIC S16 rbDeInitTmrSvsr()
#endif
{
   U32 CoreID=0;

   TRC0(rbDeInitTmrSvsr);

   CoreID = MxGetCpuID();

   tmrStarted[CoreID] = FALSE;

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    rbPutTmrSvsrMsg
 *
 *       Desc:   This Function puts timer-message using supervisor channel
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 rbPutTmrSvsrMsg
(
S16 coreID,
U16 indeX
)
#else
PUBLIC S16 rbPutTmrSvsrMsg(coreID, indeX)
S16 coreID;
U16 indeX;
#endif
{
   S16 ret;
   U32 instId = 0;
   PRIVATE U32 tmrDrop = 0;

   TRC0(rbPutTmrSvsrMsg);

   if(coreID < 0 || coreID >= MAX_NUM_CORES)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB075, ERRZERO, "coreID out of range");
      RETVALUE(RFAILED);
   }

   if(tmrStarted[coreID] != TRUE)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB076, ERRZERO, "Svsr is not initialized");
      RETVALUE(RFAILED);
   }

   instId = IID_CORE + coreID + (2 * MAX_NUM_CORES);

   ret = SvsrSendMsg(coreID, instId, (LPVOID)&osCp.tmrTbl[indeX], SVSR_MSG_OPT_DEFAULT);
   if(ret != 0)
   {
      //RB_PRINTF("\n[%s:%d] SvsrSendMsg Failed cnt=%d ret=%d idx=%p\n", __func__, __LINE__, ++tmrDrop, ret, &osCp.tmrTbl[indeX]);
      //RBLOGERROR(ERRCLS_DEBUG, ERB077, ERRZERO,"Call to SvsrSendMsg failed\n");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    rbGetTmrSvsrMsg
 *
 *       Desc:   This Function Gets Timer Message through supervisor channel
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 *
 */
#ifdef ANSI
PUBLIC MXRC rbGetTmrSvsrMsg
(
LPVOID pClientCtx,
LPVOID lMsg
)
#else
PUBLIC MXRC rbGetTmrSvsrMsg(pClientCtx, lMsg)
LPVOID pClientCtx;
LPVOID lMsg;
#endif
{

   PRIVATE S16 firstMsg = 1;

   TRC0(rbGetTmrSvsrMsg);

   if (pClientCtx == NULL)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB078, ERRZERO,"pClientCtx is NULL\n");
      RETVALUE(MX_OK);
   }

   if(lMsg == NULLP)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB079, ERRZERO,"lMsg is NULL\n");
      RETVALUE(MX_OK);
   }

   if(firstMsg)
   {
      MxSetThreadPriority(MxGetCurrentThread(),RB_SVSRICORE_PRIORITY);
      firstMsg = 0;
   }

   if(ROK != rbGetTmrMsg((void *)lMsg))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB080, ERRZERO, "rbGetTmrMsg Failed");
      RETVALUE(MX_OK);
   }

   RETVALUE(MX_OK);
}
#else


/*
 *
 *       Fun:    rbInitTmrQueue()
 *
 *       Desc:   Initializes Timer Queue
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbInitTmrQueue
(
void
)
#else
PUBLIC S16 rbInitTmrQueue()
#endif
{
   U32 coreID=0;

   TRC0(rbInitTmrQueue);

   coreID = MxGetCpuID();

   /*queue handler thread*/
   if(MX_OK != MxCreateThread((TENTRY)rbGetTmrQueueMsg, NULL, 0, (TPRIORITY)RB_TMRQ_PRIORITY, &osCp.dep.TmrQThrdID[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB081, ERRZERO, "MxCreateThreadEx Failed");
      RETVALUE(RFAILED);
   }

   /*queue Event*/
   if(MX_OK != MxCreateEvent (EVENT_MODE_RESET , NULL, &osCp.dep.TmrQEvntID[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB082, ERRZERO, "MxCreateEventEx Failed");
      RETVALUE(RFAILED);
   }

   osCp.dep.pMemTmr[coreID] = MxHeapAlloc(SsiHeap, MAX_TMRQ_LENGTH * sizeof(int *));
   if (osCp.dep.pMemTmr[coreID] == NULL)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB083, ERRZERO, "MxHeapAlloc Failed");
      RETVALUE(RFAILED);
   }

   /*Creating queue */
   if(MX_OK != MxCreateQueue (osCp.dep.pMemTmr[coreID], MAX_TMRQ_LENGTH, sizeof(int *), NULL, osCp.dep.TmrQEvntID[coreID], &osCp.dep.TmrQHndlID[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB084, ERRZERO, "MxCreateQueue Failed");
      RETVALUE(RFAILED);
   }

   if(MX_OK != MxScheduleThreadByEvent (osCp.dep.TmrQThrdID[coreID],osCp.dep.TmrQEvntID[coreID],THREAD_EVENT_AUTO_RESET))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB085, ERRZERO, "MxScheduleThreadByEvent Failed");
      RETVALUE(RFAILED);
   }

   if(MX_OK != MxShareObject (osCp.dep.TmrQHndlID[coreID], OBJ_SHARE_ON))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB086, ERRZERO, "MxShareObject Failed");
      RETVALUE(RFAILED);
   }

   tmrStarted[coreID] = TRUE;

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    rbDeInitTmrQueue
 *
 *       Desc:   DeInitializes Timer Queue
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbDeInitTmrQueue
(
void
)
#else
PUBLIC S16 rbDeInitTmrQueue()
#endif
{
   U32 coreID=0;

   TRC0(rbDeInitTmrQueue);

   coreID = MxGetCpuID();
   /*remove Queue*/
   if(MX_OK != MxRemoveQueue (osCp.dep.TmrQHndlID[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB087, ERRZERO, "MxRemoveQueue Failed");
      RETVALUE(RFAILED);
   }

   /*remove event */
   if(MX_OK != MxRemoveEvent (osCp.dep.TmrQEvntID[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB088, ERRZERO, "MxRemoveEvent Failed");
      RETVALUE(RFAILED);
   }

   /*remove thread */
   if(MX_OK != MxRemoveThread(osCp.dep.TmrQThrdID[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB089, ERRZERO, "MxRemoveThread Failed");
      RETVALUE(RFAILED);
   }

   /*deallocate memory */
   if(MX_OK != MxHeapFree(SsiHeap, osCp.dep.pMemTmr[coreID]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB090, ERRZERO, "MxHeapFree Failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    rbPutTmrQueueMsg
 *
 *       Desc:   Handles Timer Message to be put into queue
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 */

#ifdef ANSI
PUBLIC S16 rbPutTmrQueueMsg
(
S16 coreID,
U16 indeX
)
#else
PUBLIC S16 rbPutTmrQueueMsg(coreID, indeX)
S16 coreID;
U16 indeX;
#endif
{
   U32 noOfElem =0;

   TRC0(rbPutTmrQueueMsg);

   if(coreID < 0 || coreID >= MAX_NUM_CORES)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB091, ERRZERO, "coreID out of range");
      RETVALUE(RFAILED);
   }

   if(tmrStarted[coreID] != TRUE)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB092, ERRZERO, "Timer Channel is not initialized");
      RETVALUE(RFAILED);
   }

   if(MX_OK != MxGetQueueDataCount(osCp.dep.TmrQHndlID[coreID],&noOfElem))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB093, ERRZERO, "MxGetQueueDataCount Failed");
      RETVALUE(RFAILED);
   }

   if(noOfElem>=(MAX_TMRQ_LENGTH-1))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB094, ERRZERO, "Queue Full");
      RETVALUE(RFAILED);
   }

   if(MX_OK != MxPutQueueData(osCp.dep.TmrQHndlID[coreID], (LPVOID)&osCp.tmrTbl[indeX]))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB095, ERRZERO, "MxPutQueueData Failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    rbGetTmrQueueMsg
 *
 *       Desc:   s function hits when there timerqueue Event is set
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 */

#ifdef ANSI
PUBLIC S16 rbGetTmrQueueMsg
(
void
)
#else
PUBLIC S16 rbGetTmrQueueMsg()
#endif
{
   LPVOID pData = NULL;
   U32    noOfElem = 0, elem = 0;
   U32 coreID;

   TRC0(rbGetTmrQueueMsg);

   coreID = MxGetCpuID();

   if(MX_OK != MxGetQueueDataCount(osCp.dep.TmrQHndlID[coreID],&noOfElem))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB096, ERRZERO, "MxGetQueueDataCount Failed");
      RETVALUE(RFAILED);
   }

   if(noOfElem > MAX_TMRQELEM_TO_READ)
   {
      noOfElem = MAX_TMRQELEM_TO_READ;
   }

   for(elem = 0;elem < noOfElem; elem++)
   {
      if(MX_OK != MxGetQueueData(osCp.dep.TmrQHndlID[coreID], &pData))
      {
         RBLOGERROR(ERRCLS_DEBUG, ERB097, ERRZERO, "MxGetQueueData Failed");
         RETVALUE(RFAILED);
      }

      if (pData == NULL)
      {
         RBLOGERROR(ERRCLS_DEBUG, ERB098, ERRZERO, "MxGetQueueData Fetched NULL pointer");
         RETVALUE(RFAILED);
      }

      if(ROK != rbGetTmrMsg((void *)pData))
      {
         RBLOGERROR(ERRCLS_DEBUG, ERB099, ERRZERO, "rbGetTmrMsg Failed");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(MX_OK);
}
#endif


/*
 *
 *       Fun:    rbPutTmrMsg
 *
 *       Desc:   Put message into Timer-queue
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbPutTmrMsg
(
S16 coreID,
U16 indeX
)
#else
PUBLIC S16 rbPutTmrMsg(coreID, indeX)
S16 coreID;
U16 indeX;
#endif
{
   TRC0(rbPutTmrMsg);

   if(coreID < 0 || coreID >= MAX_NUM_CORES)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB100, ERRZERO, "coreID out of range");
      RETVALUE(RFAILED);
   }

   if(tmrStarted[coreID] != TRUE)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB101, ERRZERO, "Timer Channel is not initialized");
      RETVALUE(RFAILED);
   }

#ifndef RB_CHANNEL_SVSR
   if(ROK != rbPutTmrQueueMsg(coreID, indeX))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB102, ERRZERO, "rbPutTmrQueueMsg Failed");
      RETVALUE(RFAILED);
   }
#else
   if(ROK != rbPutTmrSvsrMsg(coreID, indeX))
   {
      //RBLOGERROR(ERRCLS_DEBUG, ERB103, ERRZERO, "rbPutTmrSvsrMsg Failed");
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    rbGetTmrMsg
 *
 *       Desc:   Get message from Timer-queue
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbGetTmrMsg
(
void *tTmr
)
#else
PUBLIC S16 rbGetTmrMsg(tTmr)
void *tTmr;
#endif
{
   SsTmrEntry *tmr = (SsTmrEntry *)tTmr;

   TRC0(rbGetTmrMsg);

   if (tmr == NULL)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB104, ERRZERO, "tmr is NULL pointer");
      RETVALUE(RFAILED);
   }

#ifdef SS_MT_TMR
   if(tmr->mtFlag)
   {
      tmr->tmrActvFnMt(tmr->ownerEnt, tmr->ownerInst);
   }
   else
#endif
   {
      tmr->tmrActvFn();
   }

   RETVALUE(MX_OK);
}
#endif

#ifndef RB_TICK_TIMER

/*
 *
 *       Fun:   rbTimeout
 *
 *       Desc:  Process timer event. Called from the common timer
 *              code when a timeout occurs.
 *
 *       Ret:   Void
 *
 *       Notes:
 *
 *       File:  rb_timer.c
 *
 */
#ifdef ANSI
PUBLIC Void rbTimeout
(
PTR tCb,                        /* control block */
S16 evnt                        /* event */
)
#else
PUBLIC Void rbTimeout(tCb, evnt)
PTR tCb;                        /* control block */
S16 evnt;                       /* event */
#endif
{
   CmTmrArg     arg;
   SsTmrEntry  *tEnt;
   SsIdx        idx;
   S16          coreID;
   S16          regCoreID;

   TRC0(rbTimeout);

   coreID = MxGetCpuID();

   /* get the timer entry */
   tEnt = (SsTmrEntry *) tCb;

   /* if the timer was deleted, This will be NULL, so drop it */
   if ((tEnt == NULL) || (tEnt->used == FALSE))
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      RBLOGERROR(ERRCLS_DEBUG, ERB105, ERRZERO, "Timer entry is no more in use");
#endif
      RETVOID;
   }

   /* Verify that This timer entry is still around and belongs to our task */
   if ((osCp.tmrTbl[tEnt->tmrId].used == FALSE) ||
         (osCp.tmrTbl[tEnt->tmrId].ownerEnt != tEnt->ownerEnt) ||
         (osCp.tmrTbl[tEnt->tmrId].ownerInst != tEnt->ownerInst))
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      RBLOGERROR(ERRCLS_DEBUG, ERB106, ERRZERO, "Timer entry is invalid");
#endif
      RETVOID;
   }

   /* ensure that the TAPA task is still registered */
   idx = osCp.tTskIds[tEnt->ownerEnt][tEnt->ownerInst];
   if ((idx == SS_TSKNC) || (osCp.tTskTbl[idx].used != TRUE))
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      RBLOGERROR(ERRCLS_DEBUG, ERB107, ERRZERO, "TAPA task is no more registered");
#endif
      RETVOID;
   }

   regCoreID = rbGetCoreId(osCp.tmrTbl[tEnt->tmrId].ownerEnt, osCp.tmrTbl[tEnt->tmrId].ownerInst);

   /* call tmr handler registered*/
   if (regCoreID != coreID)
   {
#ifndef RB_TMR_ALLCORE
      /*call Activation function*/
      rbPutTmrMsg(regCoreID, tEnt->tmrId);
#endif
   }
   else
   {
#ifdef SS_MT_TMR
      if(osCp.tmrTbl[tEnt->tmrId].mtFlag == TRUE)
      {
         osCp.tmrTbl[tEnt->tmrId].tmrActvFnMt(osCp.tmrTbl[tEnt->tmrId].ownerEnt,
               osCp.tmrTbl[tEnt->tmrId].ownerInst);
      }
      else
#endif
         /* invoke the timer activation function */
      {
         osCp.tmrTbl[tEnt->tmrId].tmrActvFn();
      }
   }

   /* restart the timer */
   arg.tq     = osCp.dep.tmrTq;
   arg.tqCp   = &osCp.dep.tmrTqCp;
   arg.timers = tEnt->dep.timers;
   arg.cb     = (PTR) tEnt;
   arg.evnt   = TMR_DEF;
   arg.wait   = 0;
   arg.tNum   = NOTUSED;
   arg.max    = TMR_DEF_MAX;
   arg.wait   = tEnt->interval;
   cmPlcCbTq(&arg);

   RETVOID;
}
#endif


/*
 *
 *       Fun:    rbTmrHdlr
 *
 *       Desc:   The timer handler  function. Counts time
 *               if time expires for perticular registered timer
 *               1) if core 0 call activation timer directly
 *               2) if Not core 0 Put timer message into timer message queue
 *
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_timer.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbTmrHdlr
(
void
)
#else
PUBLIC S16 rbTmrHdlr()
#endif
{
#ifndef RB_TICK_TIMER
   static int tickCount = 0;
#else
   SsTmrEntry *tmr;            /* pointer to timer entry */
   U32        timer;
   S16        coreID;
   S16        regCoreID;
#endif

   TRC0(rbTmrHdlr);

   //RB_PRINTF("%s: %d Timer HIT ticks=%d\n",__func__,__LINE__, MxGetTicks());
#ifndef RB_TICK_TIMER
   /* Always run the base timer for 1msec interval */
   cmPrcTmr(&osCp.dep.tmrTqCp, osCp.dep.tmrTq, rbTimeout);
#else
   coreID = MxGetCpuID();

   for (timer = 0; timer < SS_MAX_TMRS; timer++)
   {
      if (osCp.tmrTbl[timer].used == TRUE)
      {
         tmr = &osCp.tmrTbl[timer];
         if(--(tmr->dep.ticksToExpire) == 0)
         {
            regCoreID = rbGetCoreId(tmr->ownerEnt,tmr->ownerInst);
            /* call tmr handler registered*/
            //regCoreID = 3;
            if (regCoreID != coreID)
            {
#ifndef RB_TMR_ALLCORE
               /*call Activation function*/
               rbPutTmrMsg(regCoreID,timer);
#endif
            }
            else
            {
#ifdef SS_MT_TMR
               if(tmr->mtFlag)
               {
                  tmr->tmrActvFnMt(tmr->ownerEnt, tmr->ownerInst);
               }
               else
#endif
               {
                  tmr->tmrActvFn();
               }
            }
            tmr->dep.ticksToExpire = tmr->interval;
         }
      }
   }
#endif
   RETVALUE(ROK);
}/*rbTmrHdlr*/
#endif


/********************************************************************30**

         End of file:     ju_timer.c@@/main/1 - Mon Aug 29 22:31:34 2011

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

   ver        pat        init                  description
---------  ---------  ----------  -------------------------------------
/main/1      --------     ------    1. Initial release
*********************************************************************91*/
