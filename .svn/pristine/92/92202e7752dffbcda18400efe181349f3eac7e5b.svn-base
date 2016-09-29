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

     File:     rb_icore.c

     Sid:      ju_icore.c@@/main/1 - Mon Aug 29 22:31:33 2011

     Prg:

************************************************************************/

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "rb_include.h"
#include "appinit.h"


#ifdef SS_4GMX_LCORE
#ifdef RB_CHANNEL_SVSR
#else
PUBLIC S16 rbGetICoreQueueForCore0 ARGS((void));
PUBLIC S16 rbGetICoreQueueForCore1 ARGS((void));
PUBLIC S16 rbGetICoreQueueForCore2 ARGS((void));
PUBLIC S16 rbGetICoreQueueForCore3 ARGS((void));
PUBLIC S16 rbGetICoreQueueForCore4 ARGS((void));
PUBLIC S16 rbGetICoreQueueForCore5 ARGS((void));
#endif
PUBLIC volatile U32 print = 0;
PUBLIC volatile U16 icoreStarted[MAX_NUM_CORES] = {FALSE, FALSE, FALSE, FALSE};

#ifndef RB_CHANNEL_SVSR


/*
 *
 *       Fun:    rbInitICoreQueue()
 *
 *       Desc:   Initializes (N-1) Queues From a core
 *               For all others Cores to core itself.
 *               where N is number of core.
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbInitICoreQueue
(
void
)
#else
PUBLIC S16 rbInitICoreQueue()
#endif
{
   U32 coreID   = 0;
   U32 qForCore = 0;
   UINT32 ret   = 0;
   char qThrdName[20],qEvntName[20],qName[20];

   TRC0(rbInitICoreQueue);

   coreID = MxGetCpuID();

   /*  Called on each core. Creates queues to receive from
    *  cores other than self.
    */
   /*rb001.101 Modified for Self Post related Fix*/
#ifdef RB_SINGLE_CORE
   qForCore=coreID;
#else
   for(qForCore = 0; qForCore < MAX_NUM_CORES; qForCore++)
#endif
   {
         sprintf(qThrdName,"qThrdName%d",qForCore);
         sprintf(qEvntName,"qEvntName%d",qForCore);
         sprintf(qName,"qName%d",qForCore);

         /*queue handler thread*/
         switch(qForCore)
         {
            case 0:
               ret = MxCreateThread( rbGetICoreQueueForCore0, qThrdName, 0,
                        (TPRIORITY)RB_COREQ_PRIORITY, &osCp.dep.QThrdID[coreID][qForCore]);
               break;

            case 1:
               ret = MxCreateThread( rbGetICoreQueueForCore1, qThrdName, 0,
                        (TPRIORITY)RB_COREQ_PRIORITY, &osCp.dep.QThrdID[coreID][qForCore]);
               break;

            case 2:
               ret = MxCreateThread( rbGetICoreQueueForCore2, qThrdName, 0,
                        (TPRIORITY)RB_COREQ_PRIORITY, &osCp.dep.QThrdID[coreID][qForCore]);
               break;

            case 3:
               ret = MxCreateThread( rbGetICoreQueueForCore3, qThrdName, 0,
                        (TPRIORITY)RB_COREQ_PRIORITY, &osCp.dep.QThrdID[coreID][qForCore]);
               break;

            case 4:
               ret = MxCreateThread( rbGetICoreQueueForCore4, qThrdName, 0,
                        (TPRIORITY)RB_COREQ_PRIORITY, &osCp.dep.QThrdID[coreID][qForCore]);
               break;

            case 5:
               ret = MxCreateThread( rbGetICoreQueueForCore5, qThrdName, 0,
                        (TPRIORITY)RB_COREQ_PRIORITY, &osCp.dep.QThrdID[coreID][qForCore]);
               break;

            default:
               ret = RFAILED;
               break;
         }

         if(MX_OK !=ret)
         {
            RBLOGERROR(ERRCLS_DEBUG, ERB003, ERRZERO, "MxCreateThreadEx Failed");
            RETVALUE(RFAILED);
         }

         /*queue Event*/
         if(MX_OK != MxCreateEvent (EVENT_MODE_RESET , qEvntName,
                  &osCp.dep.QEvntID[coreID][qForCore]))
         {
            RBLOGERROR(ERRCLS_DEBUG, ERB004, ERRZERO, "MxCreateEventEx Failed");
            RETVALUE(RFAILED);
         }

         osCp.dep.pMem[coreID][qForCore] = MxHeapAlloc(SsiHeap, 
                                              MAX_QUEUE_LENGTH * sizeof(Buffer *));
         if (osCp.dep.pMem[coreID][qForCore] == NULL)
         {
            RBLOGERROR(ERRCLS_DEBUG, ERB005, ERRZERO, "MxHeapAlloc Failed");
            RETVALUE(RFAILED);
         }

         /*Creating queue */
         if(MX_OK != MxCreateQueue (osCp.dep.pMem[coreID][qForCore], 
                  MAX_QUEUE_LENGTH, sizeof(Buffer *), qName, 
                  osCp.dep.QEvntID[coreID][qForCore], &osCp.dep.QHndlID[coreID][qForCore]))
         {
            RBLOGERROR(ERRCLS_DEBUG, ERB006, ERRZERO, "MxCreateQueue Failed");
            RETVALUE(RFAILED);
         }

         if(MX_OK != MxScheduleThreadByEvent (osCp.dep.QThrdID[coreID][qForCore],
                  osCp.dep.QEvntID[coreID][qForCore],THREAD_EVENT_AUTO_RESET))
         {
            RBLOGERROR(ERRCLS_DEBUG, ERB007, ERRZERO, "MxScheduleThreadByEvent Failed");
            RETVALUE(RFAILED);
         }
         if(MX_OK != MxShareObject (osCp.dep.QHndlID[coreID][qForCore], OBJ_SHARE_ON))
         {
            RBLOGERROR(ERRCLS_DEBUG, ERB008, ERRZERO, "MxShareObject Failed");
            RETVALUE(RFAILED);
         }
   }
   icoreStarted[coreID] = TRUE;

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    rbDeInitICoreQueue()
 *
 *       Desc:   DeInitializes (N-1) Queues From a core
 *               for all other cores.
 *               where N is number of core.
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbDeInitICoreQueue
(
void
)
#else
PUBLIC S16 rbDeInitICoreQueue()
#endif
{
   U32 coreID   = 0;
   U32 qForCore = 0;

   TRC0(rbDeInitICoreQueue);

   coreID = MxGetCpuID();

   /*rb001.101 Modified for Self Post related Fix*/
#ifdef RB_SINGLE_CORE
   qForCore=coreID;
#else
   for(qForCore = 0; qForCore < MAX_NUM_CORES; qForCore++)
#endif
   {

         /*remove Queue*/
         if(MX_OK != MxRemoveQueue (osCp.dep.QHndlID[coreID][qForCore]))
         {
            RBLOGERROR(ERRCLS_DEBUG, ERB009, ERRZERO, "MxRemoveQueue Failed");
            RETVALUE(RFAILED);
         }

         /*remove event */
         if(MX_OK != MxRemoveEvent (osCp.dep.QEvntID[coreID][qForCore]))
         {
            RBLOGERROR(ERRCLS_DEBUG, ERB010, ERRZERO, "MxRemoveEvent Failed");
            RETVALUE(RFAILED);
         }

         /*remove thread */
         if(MX_OK != MxRemoveThread(osCp.dep.QThrdID[coreID][qForCore]))
         {
            RBLOGERROR(ERRCLS_DEBUG, ERB011, ERRZERO, "MxRemoveThread Failed");
            RETVALUE(RFAILED);
         }

         /*deallocate memory */
         if(MX_OK != MxHeapFree(SsiHeap, osCp.dep.pMem[coreID][qForCore]))
         {
            RBLOGERROR(ERRCLS_DEBUG, ERB012, ERRZERO, "MxHeapFree Failed");
            RETVALUE(RFAILED);
         }
   }

   icoreStarted[coreID] = FALSE;

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    rbPutICoreQueueMsg()
 *
 *       Desc:   Insert Message Into queue.
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbPutICoreQueueMsg
(
S16 dstCoreID,
Buffer *mBuf
)
#else
PUBLIC S16 rbPutICoreQueueMsg(dstCoreID, mBuf)
S16 dstCoreID;
Buffer *mBuf;
#endif
{
   PRIVATE U32 pktDrop = 0;
   S16 coreID = MxGetCpuID(), ret = 0;

   TRC0(rbPutICoreQueueMsg);

   if(mBuf == NULL)
   {
      RB_PRINTF("\n[%s:%d] Null Buffer\n", __func__, __LINE__);
      RETVALUE(RFAILED);
   }

   if(dstCoreID < 0 || dstCoreID >= MAX_NUM_CORES)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB013, ERRZERO, "dstCoreID out of range");
      RETVALUE(RFAILED);
   }

   if(icoreStarted[dstCoreID] != TRUE)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB014, ERRZERO, "Queue is not initialized");
      RETVALUE(RFAILED);
   }

   if((ret = MxPutQueueData(osCp.dep.QHndlID[dstCoreID][coreID], (LPVOID)mBuf)) != MX_OK)
   {
      //RB_PRINTF("\n[%s:%d] MxPutQueueData Failed cnt=%d ret=%d mBuf=%p\n",
      //      __func__, __LINE__, ++pktDrop, ret, mBuf);
      if (ret == MX_IC_TIMEOUT)
      {
         RETVALUE(ROK);
      }
      RBLOGERROR(ERRCLS_DEBUG, ERB015, ERRZERO, "MxPutQueueData Failed");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}


/*
 *
 *       Fun:    rbGetICoreQueueForCore0()
 *
 *       Desc:   Queue handler for messages
 *               from core 0
 *
 *       Ret:    MX_OK
 *
 *       Notes:  None.
 *
 *       File:   rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbGetICoreQueueForCore0
(
void
)
#else
PUBLIC S16 rbGetICoreQueueForCore0()
#endif
{
   TRC0(rbGetICoreQueueForCore0);

   rbGetICoreQueueMsg(0);

   RETVALUE(MX_OK);
}


/*
 *
 *       Fun:    rbGetICoreQueueForCore1()
 *
 *       Desc:   Queue handler for messages
 *               from core 1
 *
 *       Ret:    MX_OK
 *
 *       Notes:  None.
 *
 *       File:   rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbGetICoreQueueForCore1
(
void
)
#else
PUBLIC S16 rbGetICoreQueueForCore1()
#endif
{
   TRC0(rbGetICoreQueueForCore1);

   rbGetICoreQueueMsg(1);

   RETVALUE(MX_OK);
}


/*
 *
 *       Fun:    rbGetICoreQueueForCore2()
 *
 *       Desc:   Queue handler for messages from
 *               core 2
 *
 *       Ret:    MX_OK
 *
 *       Notes:  None.
 *
 *       File:   rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbGetICoreQueueForCore2
(
void
)
#else
PUBLIC S16 rbGetICoreQueueForCore2()
#endif
{
   TRC0(rbGetICoreQueueForCore2);

   rbGetICoreQueueMsg(2);

   RETVALUE(MX_OK);
}


/*
 *
 *       Fun:    rbGetICoreQueueForCore3()
 *
 *       Desc:   Queue handler for messages
 *               from core 3
 *
 *       Ret:    MX_OK
 *
 *       Notes:  None.
 *
 *       File:   rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbGetICoreQueueForCore3
(
void
)
#else
PUBLIC S16 rbGetICoreQueueForCore3()
#endif
{
   volatile int t = GetTIMETICK();
   int t2;
   TRC0(rbGetICoreQueueForCore3);

   rbGetICoreQueueMsg(3);

#ifdef MSPD_MLOG
   MLogTask(PID_SELFPOST_CORE3, RESOURCE_LARM, t, t2 = GetTIMETICK());
   MLogTask(PID_L2_PROCESSING, RESOURCE_LARM, t, t2);
#endif

   RETVALUE(MX_OK);
}


/*
 *
 *       Fun:    rbGetICoreQueueForCore4()
 *
 *       Desc:   Queue handler for messages
 *               from core 4
 *
 *       Ret:    MX_OK
 *
 *       Notes:  None.
 *
 *       File:   rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbGetICoreQueueForCore4
(
void
)
#else
PUBLIC S16 rbGetICoreQueueForCore4()
#endif
{
   TRC0(rbGetICoreQueueForCore4);

   rbGetICoreQueueMsg(4);

   RETVALUE(MX_OK);
}


/*
 *
 *       Fun:    rbGetICoreQueueForCore5()
 *
 *       Desc:   Queue handler for messages
 *               from core 5
 *
 *       Ret:    MX_OK
 *
 *       Notes:  None.
 *
 *       File:   rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbGetICoreQueueForCore5
(
void
)
#else
PUBLIC S16 rbGetICoreQueueForCore5()
#endif
{
   TRC0(rbGetICoreQueueForCore5);

   rbGetICoreQueueMsg(5);

   RETVALUE(MX_OK);
}


/*
 *
 *       Fun:    rbGetICoreQueueMsg()
 *
 *       Desc:   Remove Message from queue .
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbGetICoreQueueMsg
(
S16 srcCoreID
)
#else
PUBLIC S16 rbGetICoreQueueMsg(srcCoreID)
S16 srcCoreID;
#endif
{
   LPVOID pData = NULL;
   U32    noOfElem = 0, elem = 0;
   U32    coreID = 0;
   U32 tick = *(volatile U32 *) 0xFE050004;
   U32 tick2;

   TRC0(rbGetICoreQueueMsg);

   coreID = MxGetCpuID();

   if(MX_OK != MxGetQueueDataCount(osCp.dep.QHndlID[coreID][srcCoreID],&noOfElem))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB016, ERRZERO, "MxGetQueueDataCount Failed");
      RETVALUE(RFAILED);
   }

   if(noOfElem > MAX_QELEM_TO_READ_FOR_SELFPOST)
   {
      noOfElem = MAX_QELEM_TO_READ_FOR_SELFPOST;
      MxScheduleThread(osCp.dep.QThrdID[coreID][coreID]);
   }

   for(elem = 0;elem < noOfElem; elem++)
   {
      if(MX_OK != MxGetQueueData(osCp.dep.QHndlID[coreID][srcCoreID], &pData))
      {
         RB_PRINTF("[%s:%d] MxGetQueueData Failed\n",__func__,__LINE__);
         RBLOGERROR(ERRCLS_DEBUG, ERB017, ERRZERO, "MxGetQueueData Failed");
         RETVALUE(RFAILED);
      }

      if (pData == NULL)
      {
         RBLOGERROR(ERRCLS_DEBUG, ERB018, ERRZERO, "MxGetQueueData Fetched NULL pointer");
         RETVALUE(RFAILED);
      }

      /*handle data*/
      if(ROK != rbGetICoreMsg((Buffer *)pData))
      {
         RB_PRINTF("[%s:%d] rbGetICoreMsg Failed\n",__func__,__LINE__);
         RETVALUE(RFAILED);
      }
   }
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_SSI_ICORE_MSG, RESOURCE_LARM, tick, tick2 = *(volatile U32 *) 0xFE050004);
#endif

   RETVALUE(MX_OK);
}
#else


/*
 *
 *     Fun  : rbInitICoreSvsr
 *
 *     Desc : Registers client with inter core supervisor channel
 *
 *     Ret:   ROK      - ok
 *            RFAILED  - failed, general (optional)
 *
 *     Notes: None
 *
 *     File : rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbInitICoreSvsr
(
void
)
#else
PUBLIC S16 rbInitICoreSvsr()
#endif
{
   U32 CoreID = MxGetCpuID();
   U32 InstId = IID_CORE + MAX_NUM_CORES + CoreID;
   U32 *pCtx  = &osCp.dep.pICoreCtx[CoreID];

   TRC0(rbInitICoreSvsr);

   RB_PRINTF("\n[%s:%d] core=%d\n", __func__, __LINE__, MxGetCpuID());
   if(MX_OK!= SvsrRegMsgHandler(InstId /*+ MAX_NUM_CORES*/, rbGetICoreSvsrMsg, pCtx))
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB019, ERRZERO, "SvsrRegMsgHandler Failed");
      RETVALUE(RFAILED);
   }

   icoreStarted[CoreID] = TRUE;

   RETVALUE(ROK);
}/*end of rbInitICoreSvsr*/


/*
 *
 *     Fun  : rbDeInitICoreSvsr
 *
 *     Desc : DeRegisters client with inter core supervisor channel
 *
 *     Ret:   ROK      - ok
 *            RFAILED  - failed, general (optional)
 *
 *     Notes: None
 *
 *     File : rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbDeInitICoreSvsr
(
void
)
#else
PUBLIC S16 rbDeInitICoreSvsr()
#endif
{
   U32 CoreID = MxGetCpuID();

   TRC0(rbDeInitICoreSvsr);

   icoreStarted[CoreID] = FALSE;

   RETVALUE(ROK);
}/*end of rbInitICoreSvsr*/


/*
 *
 *       Fun:   rbPutICoreSvsrMsg
 *
 *       Desc:  This function serves as a Tx for reaching the entities and instances on LARM.
 *
 *       Ret:   ROK      - ok
 *              RFAILED  - failed, general (optional)
 *
 *       Notes:
 *
 *       File:  rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbPutICoreSvsrMsg
(
S16 dstCoreID,
Buffer *mBuf
)
#else
PUBLIC S16 rbPutICoreSvsrMsg(dstCoreID, mBuf)
S16 dstCoreID;
Buffer *mBuf;
#endif
{
   S16 ret;
   U32 instId = 0;
   PRIVATE U32 pktDrop = 0;
   volatile int t;

   TRC0(rbPutICoreSvsrMsg);

   if (mBuf == NULL)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB020, ERRZERO,"Null Buffer pointer \n");
      RETVALUE(RFAILED);
   }

   if(dstCoreID < 0 || dstCoreID >= MAX_NUM_CORES)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB021, ERRZERO, "dstCoreID out of range");
      RETVALUE(RFAILED);
   }

   if(icoreStarted[dstCoreID] != TRUE)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB022, ERRZERO, "Queue is not initialized");
      RETVALUE(RFAILED);
   }

   instId =  IID_CORE + dstCoreID + MAX_NUM_CORES;

   t = GetTIMETICK();

 /*  if ((unsigned int)mBuf->b_rptr  < 0x11300000 || (unsigned int) mBuf->b_rptr  >  0x40000000 + 0x20000000)
   {
        uart_printf ("Incorrect b_rptr %08x\n", mBuf->b_rptr);
        *(volatile int*)0xdeaddead = 0xdeaddead;
        //stop_printf("Incorrect b_rptr %08x\n", mBuf->b_rptr);
   }*/

   ret = SvsrSendMsg(dstCoreID, instId, (LPVOID)mBuf, SVSR_MSG_OPT_DEFAULT);
#ifdef MSPD_MLOG
   MLogTask(PID_L2_ICORE_SvsrSendMsg, RESOURCE_LARM, t, GetTIMETICK());
#endif
   if(ret != 0)
   {
      RB_PRINTF("\n[%s:%d] SvsrSendMsg Failed cnt=%d ret=%0x mBuf=%p dstCoreId=%u instId=%u\n",
         __func__, __LINE__, ++pktDrop, ret, mBuf, dstCoreID, instId);
      RBLOGERROR(ERRCLS_DEBUG, ERB023, ERRZERO,"Call to SvsrSendMsg failed\n");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/*end of rbPutICoreSvsrMsg*/


/*
 *
 *       Fun:  rbGetICoreSvsrMsg
 *
 *       Desc:  This primitive is a callback from Syscore for handling messages from lower arm
 *
 *       Ret:   ROK      - ok
 *              RFAILED  - failed, general (optional)
 *
 *
 *       Notes: None
 *
 *       File:  rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC MXRC rbGetICoreSvsrMsg
(
LPVOID pClientCtx,
LPVOID lMsg
)
#else
PUBLIC MXRC rbGetICoreSvsrMsg(pClientCtx, lMsg)
LPVOID pClientCtx;
LPVOID lMsg;
#endif
{
   PRIVATE S16 firstMsg = 1;
   volatile int t = GetTIMETICK();

   TRC0(rbGetICoreSvsrMsg);

   if (pClientCtx == NULL)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB024, ERRZERO,"pClientCtx is NULL\n");
      RETVALUE(MX_OK);
   }

   if(lMsg == NULLP)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB025, ERRZERO,"lMsg is NULL\n");
      RETVALUE(MX_OK);
   }

   if(firstMsg)
   {
      MxSetThreadPriority(MxGetCurrentThread(),RB_SVSRICORE_PRIORITY);
      firstMsg = 0;
   }
   /*handle data*/
   if(ROK != rbGetICoreMsg((Buffer *)lMsg))
   {
      RB_PRINTF("[%s:%d] rbGetICoreMsg Failed\n",__func__,__LINE__);
      RETVALUE(MX_OK);
   }
#ifdef MSPD_MLOG_NEW
   MLogTask(PID_SSI_ICORE_MSG, RESOURCE_LARM, t, GetTIMETICK());
#endif

   return MX_OK;
} /* end of rbICoreSvrHndlr */
#endif


/*
 *
 *       Fun:    rbPutICoreMsg()
 *
 *       Desc:   Put Message into message-queue.
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbPutICoreMsg
(
S16 dstCoreID,
Pst *pst,
Buffer *mBuf
)
#else
PUBLIC S16 rbPutICoreMsg(dstCoreID, pst, mBuf)
S16 dstCoreID;
Pst *pst;
Buffer *mBuf;
#endif
{
   volatile U32 tick;
   TRC0(rbPutICoreMsg);

   tick = GetTIMETICK();
   if(mBuf == NULL)
   {
      RB_PRINTF("\n[%s:%d] Null Buffer\n", __func__, __LINE__);
      RETVALUE(RFAILED);
   }

   if(dstCoreID < 0 || dstCoreID >= MAX_NUM_CORES)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB026, ERRZERO, "dstCoreID out of range");
      RETVALUE(RFAILED);
   }

   {
      U32 mlogDebug[5];
	  
      mlogDebug[0] = (unsigned int)mBuf;
      mlogDebug[1] = (unsigned int)pst->event;
      mlogDebug[2] = (unsigned int)pst->dstEnt;
      mlogDebug[3] = (unsigned int)pst->dstInst;         
      mlogDebug[4] = (unsigned int)pst->srcEnt;         
//MLogAddVariables(5, mlogDebug, PID_SSI_SNDICORE_MSG);
      MLogAddVariables(5, mlogDebug, tick);
   }

   CMCHKPK(cmPkPst, pst, mBuf);

#ifndef RB_CHANNEL_SVSR
   if((rbPutICoreQueueMsg(dstCoreID, (LPVOID)mBuf)) != ROK)
   {
      RB_PRINTF("\n[%s:%d] rbPutICoreQueueMsg Failed\n", __func__, __LINE__);
      MxExcInc(&print);
      RBLOGERROR(ERRCLS_DEBUG, ERB027, ERRZERO, "rbPutICoreQueueMsg Failed");
      RETVALUE(RFAILED);
   }
#else
   if((rbPutICoreSvsrMsg(dstCoreID, (LPVOID)mBuf)) != ROK)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE(ROK);
}


/*
 *
 *       Fun:    rbGetICoreMsg()
 *
 *       Desc:   Handle Message removed from message-queue.
 *
 *       Ret:    ROK     : if Success
 *               RFAILED : If Failure
 *
 *       Notes:  None.
 *
 *       File:   rb_icore.c
 *
 */
#ifdef ANSI
PUBLIC S16 rbGetICoreMsg
(
Buffer *mBuf
)
#else
PUBLIC S16 rbGetICoreMsg(mBuf)
Buffer *mBuf;
#endif
{
   Pst nPst;
   U32 coreID , dstCoreID;
   SsTTskEntry *tTsk;
   SsMsgInfo   *mInfo;
   S16 ret ,idx;
   extern SysStayInfo syscoreinfo [4];

   TRC0(rbGetICoreMsg);

  /* if ((unsigned int)mBuf->b_rptr  < 0x11300000 || (unsigned int) mBuf->b_rptr  >  0x40000000 + 0x20000000)
   {
        uart_printf ("Incorrect b_rptr %08x\n", mBuf->b_rptr);
        *(volatile int*)0xdeaddead = 0xdeaddead;
        //stop_printf("Incorrect b_rptr %08x\n", mBuf->b_rptr);
   }*/
   coreID = MxGetCpuID();

   if (mBuf == NULL)
   {
      RBLOGERROR(ERRCLS_DEBUG, ERB029, ERRZERO, "mBuf is NULL pointer");
      RETVALUE(RFAILED);
   }

   if (print)
   {
      RB_PRINTF("\n[%s:%d] coreID=%d mBuf=%p\n",__func__,__LINE__, coreID, mBuf);
      MxExcDec(&print);
   }
   syscoreinfo[MxGetCpuID()].timestamp = *(volatile U32 *) 0xFE050004;
   syscoreinfo[MxGetCpuID()].res[0] = 1; /* 1 is for Icore msg */
   syscoreinfo[MxGetCpuID()].res[1] = 1; /* 1 is for start of the task */

   CMCHKUNPK(cmUnpkPst, &nPst, mBuf)
   {
	 U32 mlogDebug[5];
     volatile U32 tick;
	 tick = GetTIMETICK();
	 mlogDebug[0] = (unsigned int)mBuf;
	 mlogDebug[1] = (unsigned int)nPst.event;
	 mlogDebug[2] = (unsigned int)nPst.dstEnt;
	 mlogDebug[3] = (unsigned int)nPst.dstInst;         
	 mlogDebug[4] = (unsigned int)nPst.srcEnt;         
	// MLogAddVariables(5, mlogDebug, PID_SSI_RCVICORE_MSG);
	 MLogAddVariables(5, mlogDebug, tick);
   }

   /*handle data*/
   dstCoreID = rbGetCoreId(nPst.dstEnt, nPst.dstInst);
   if(dstCoreID != coreID)
   {
      SPutMsg(mBuf);
   }
   else
   {
      mInfo = (SsMsgInfo *)mBuf->b_rptr;

      if (mInfo->eventInfo.event == SS_EVNT_DATA)
      {
         idx = osCp.tTskIds[nPst.dstEnt][nPst.dstInst];

         /* verify that it hasn't been deregistered */
         if (idx == SS_TSKNC)
         {
            SPutMsg(mBuf);
            RETVALUE(APIHEC_DROP_RESP);
         }
         if (idx > 9)
         {
            uart_printf("Idx is %d\n", idx);
            *(volatile int*) 0xdeaddead = 0xdeadbeef;
         }
         tTsk = &osCp.tTskTbl[idx];
	     {
		    syscoreinfo[MxGetCpuID()].mBuf	= (unsigned int)mBuf;
		    syscoreinfo[MxGetCpuID()].event	= (unsigned int)nPst.event;
		    syscoreinfo[MxGetCpuID()].dstEnt	= (unsigned int)nPst.dstEnt;
		    syscoreinfo[MxGetCpuID()].dstInst = (unsigned int)nPst.dstInst;
		    syscoreinfo[MxGetCpuID()].srcEnt	= (unsigned int)nPst.srcEnt;
		    syscoreinfo[MxGetCpuID()].timestamp = *(volatile U32 *) 0xFE050004;
		    syscoreinfo[MxGetCpuID()].res[1] = 2; /* 2 is for unpack of Pst done */
		 }
         if (MxGetCpuID() == 2)
         {
            EXTERN S16 pjActvTsk ARGS ((Pst *pst, Buffer *mBuf));
            EXTERN S16 kwActvTsk ARGS ((Pst *pst, Buffer *mBuf));
            EXTERN S16 rgActvTsk ARGS ((Pst *pst, Buffer *mBuf));
            EXTERN S16 schActvTsk ARGS ((Pst *pst, Buffer *mBuf));
            EXTERN S16 ysActvTsk ARGS ((Pst *pst, Buffer *mBuf));
            EXTERN S16 ysActvTsk ARGS ((Pst *pst, Buffer *mBuf));
            if ((unsigned int) tTsk->actvTsk == (unsigned int)pjActvTsk ||
                (unsigned int) tTsk->actvTsk == (unsigned int)kwActvTsk  ||
                (unsigned int) tTsk->actvTsk == (unsigned int)rgActvTsk  ||
                (unsigned int) tTsk->actvTsk == (unsigned int)schActvTsk ||
                (unsigned int) tTsk->actvTsk == (unsigned int)ysActvTsk)
             {
                ret = tTsk->actvTsk(&nPst, mBuf);
				{
					extern SysStayInfo syscoreinfo [4];
					syscoreinfo[MxGetCpuID()].res[1] = 255; /* 255 denotes the end of this task */
                }
				if(ret != ROK)
                {
                   MSPD_DBG("actTsk:%d idx:%d mbuf:%x evt:%d dstEnt:%d dstInst:%d srcEnt:%d\n", 
                      tTsk, idx, (unsigned int)mBuf,(unsigned int)nPst.event,
                      (unsigned int)nPst.dstEnt, (unsigned int)nPst.dstInst,
                      (unsigned int)nPst.srcEnt);                  
                }				   
             }
             else
             {
                uart_printf("tTsk->actvTsk is %08x\n", (unsigned int)tTsk->actvTsk);
                *(volatile int*) 0xdeaddead = 0xdeadbeef;
             }
         }
          else
          	{
              ret = tTsk->actvTsk(&nPst, mBuf);
			  	{
					extern SysStayInfo syscoreinfo [4];
					syscoreinfo[MxGetCpuID()].res[1] = 255; /* 255 denotes the end of this task */
                }
                if(ret != ROK)
                {
                    MSPD_DBG("actTsk:%d idx:%d mbuf:%x evt:%d dstEnt:%d dstInst:%d srcEnt:%d\n", 
                       tTsk, idx, (unsigned int)mBuf,(unsigned int)nPst.event,
                       (unsigned int)nPst.dstEnt, (unsigned int)nPst.dstInst,
                       (unsigned int)nPst.srcEnt);                  
                }		
			  
          	}
      }
      if(ROK != ret)
      {
         RBLOGERROR(ERRCLS_DEBUG, ERB042, ERRZERO,"SPstTsk failed\n");
         SPutMsg(mBuf);
      }
   }
  
   RETVALUE(MX_OK);
}
#endif


/********************************************************************30**

         End of file:     ju_icore.c@@/main/1 - Mon Aug 29 22:31:33 2011

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
/main/1      rb001.101    sasahni   1. Modified for Self Post related Fix
                                    2. Scheduling queue thread when there 
                                       are messages still in the Queue
*********************************************************************91*/
