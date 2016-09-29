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

     Name:    LTE eNodeB - ve file

     Type:    C Source file

     Desc:    This file contains the enodeB application timer code

     File:    ve_data_tmr.c

     Sid:      ve_data_tmr.c@@/main/1 - Wed Dec  1 08:44:23 2010

     Prg:     an
*********************************************************************21*/

/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_inet.h"
#include "cm_tpt.h"

#include "nhu_asn.h"
#include "nhu.h"           /* NHU interface defines */
#include "pju.h"           /* PDCP data interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"           /* PHY RRM control Interface */
#include "szt.h"           /* S1AP RRM control Interface */
#ifdef EU_DAT_APP
#include "egt.h"           /* S1AP RRM control Interface */
#endif 
#include "lrg.h"           /* MAC RRM control Interface */
#include"ve_eut.h"


/* RRM related includes */
#include "ve.h" 
#include "lve.h"
#include "ve_perf.h" 
#include "ve_sm_init.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_lib.x"
#include "cm_inet.x"
#include "cm_tpt.x"

#include "nhu_asn.x"
#include "nhu.x"           /* NHU Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "cpj.x"           /* CPJ Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "ctf.x"           /* PHY RRM control Interface */
#include "szt.x"           /* S1AP RRM control Interface */
#include "czt.x"
#ifdef EU_DAT_APP
#include "egt.x"           /* S1AP RRM control Interface */
#endif
#include "lrg.x"           /* MAC RRM control Interface */
#include"ve_eut.x"
#include "ve_sm_init.x"

/* RRM related includes */
#include "lve.x" 
#include "ve.x" 




/*
 *
 *       Fun:   veRegInitDatTmr
 *
 *       Desc:  Register and initialize timer queue structures in 
 *              LTE eNodeB during general config.
 *              
 *       Ret:   ROK - ok; 
 *              RFAILED - failed;
 *
 *       Notes: none
 *
 *       File:  vb_tmr.c
 *
 */
#ifdef ANSI
PUBLIC S16 veRegInitDatTmr
(
Void
)
#else
PUBLIC S16 veRegInitDatTmr()
#endif
{
   S16 ret;
   U16 cnt;

   TRC2(veRegInitDatTmr);

   ret = SRegTmr((Ent)ENTVE, (Inst)0, VE_TMR_RES, veActvDatTmr);
   if (ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veRegInitDatTmr: SRegTmr Failed \n"));
#endif
      RETVALUE(RFAILED);
   }

     veEuCb.tqCp.tmrLen = VE_NUM_TQENTRY;
     for(cnt = 0; cnt < VE_NUM_TQENTRY; cnt++)
     {
        veEuCb.tq[cnt].first = NULLP;
        veEuCb.tq[cnt].tail = NULLP;
     }

   RETVALUE(ROK);

} /* veRegInitDatTmr */

/*
 *
 *       Fun:   Activate Task - timers
 *
 *       Desc:  Invoked by system services to activate a task with
 *              a timer tick.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  ve_tmr.c
 *
 */
#ifdef ANSI
PUBLIC S16 veActvDatTmr
(
void
)
#else
PUBLIC S16 veActvDatTmr()
#endif
{
   TRC3(veActvDatTmr);

   cmPrcTmr(&veEuCb.tqCp, veEuCb.tq, (PFV) veProcInactvTmr);

   RETVALUE(ROK);

} /* end of veActvDatTmr */


/*
*
*       Fun:    veInactvStartTmr
*
*       Desc:   start inactivity timer
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   ve_tmr.c
*
*/

#ifdef ANSI
PUBLIC Void veInactvStartTmr
(
PTR                 cb,
S16                 tmrEvnt,
U32                 delay
)
#else
PUBLIC Void veInactvStartTmr (cb, tmrEvnt, delay)
PTR                 cb;
S16                 tmrEvnt;
U32                 delay;
#endif /* ANSI */
{
   U16         wait;
   CmTmrArg    arg;
   CmTimer     *tmr;
   U8          maxTmrs;
   VeEuUeCb    *euUeCb;

   /* Added trace macro */
   TRC2(veInactvStartTmr)

   wait = 0;
   switch (tmrEvnt)
   {
      /* For Inactivity Timer */
      case VE_TMR_INACTIVITY:
         euUeCb   = ( VeEuUeCb*)cb;
         tmr      = &euUeCb->inactivityTmr;
         maxTmrs  = 1;
         break;

      default:
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veStartTmr: Invalid tmrEvnt\n"));
#endif
         RETVOID;

   } /* end of switch */

   wait = (U16)delay;/* klock warning fix */

   /* initialize argument for common timer function */
   if(wait != 0)
   {
      arg.tqCp   = &(veEuCb.tqCp);
      arg.tq     = veEuCb.tq;
      arg.cb     = cb;
      arg.timers = tmr;
      arg.evnt   = tmrEvnt;
      arg.wait   = wait;
      arg.tNum   = 0;
      arg.max    = maxTmrs;

      cmPlcCbTq(&arg);
   }

   RETVOID;
} /* end of veInactvStartTmr() */


/*
*
*       Fun:    veInactvStopTmr
*
*       Desc:   stop the Inactivity timer
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   ve_tmr.c
*
*/

#ifdef ANSI
PUBLIC Void veInactvStopTmr
(
PTR     cb,
S16     event
)
#else
PUBLIC Void veInactvStopTmr (cb, event)
PTR     cb;
S16     event;
#endif /* ANSI */
{

   CmTmrArg     arg;
   U8           idx;
   Bool         tmrRunning;
   CmTimer      *timers = NULLP;
   U8           max = 0;
   VeEuUeCb     *euUeCb;

   TRC3(veStopTmr)

   idx = 0;

   tmrRunning = FALSE;
   switch(event)
   {
      case VE_TMR_INACTIVITY:
      {
         euUeCb = (VeEuUeCb *)cb;
         max     =  1;
         if(euUeCb->inactivityTmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
            euUeCb->expiryCnt = 0;
         }
         timers = &euUeCb->inactivityTmr;
         break;
      }

      default:
         break;
   }
   if(tmrRunning == FALSE)
   {
      RETVOID;
   }

   /* initialize argument for common timer function */
   arg.tqCp    = &veEuCb.tqCp;
   arg.tq      = veEuCb.tq; 
   arg.timers  = timers;
   arg.cb      = cb;
   arg.evnt    = event;
   arg.wait    = 0;
   arg.max     = max;
   arg.tNum  = idx;
   cmRmvCbTq(&arg);

   RETVOID;
} /* end of veInactvStopTmr() */

/*
*
*       Fun:    veProcInactvTmr
*
*       Desc:   Handle the expiration of Inactivity timer
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   ve_tmr.c
*
*/

#ifdef ANSI
PUBLIC Void veProcInactvTmr
(
Ptr     cb,
S16     event
)
#else
PUBLIC Void veProcInactvTmr (cb, event)
Ptr     cb;
S16     event;
#endif /* ANSI */
{
   VeEuUeCb    *euUeCb;

   TRC3(veProcInactvTmr)

   euUeCb = (VeEuUeCb *)cb;
   switch(event)
   {
      case VE_TMR_INACTIVITY:
      {
         if(euUeCb)
         {
            if(euUeCb->datRcvd == FALSE)
            {
               euUeCb->expiryCnt += 1;
               if(euUeCb->expiryCnt == smCfgCb.maxExpires)
               {
                  /* We have identified the inactivity of a particular UE
                   * So we should initiate the Release Request towards
                   * the MME. This function invocation shall deallocate
                   * euUeCb control block*/
                  printf("\n ********************************\n");
                  printf("\n INACTIVITY DETECTED             \n");
                  printf("\n ********************************\n");
                  veInactivityDetected(euUeCb);
               }
               else
               { 
#ifndef MSPD
                  /* g++ compilation fix */
                  printf("\nMaximum Expiry Count %lu and Current Expiry Count %d", smCfgCb.maxExpires, euUeCb->expiryCnt);
#endif



                  /* restart the inactivity timer */
                  /* Fix for CID- 1657-01-01 DefectId- 115433 */
                  /* Added pointer type casting to remove warning */
                  veInactvStopTmr( (PTR)euUeCb, VE_TMR_INACTIVITY);
                  /* Added pointer type casting to remove warning */
                  veInactvStartTmr( (PTR)euUeCb, VE_TMR_INACTIVITY, smCfgCb.inactvTmrVal);
               } /* end of else part */
            } /* end of if statement of datRcvd */
            else
            {
               euUeCb->datRcvd = FALSE; 
               /* Reset the continuous expiry count to 0 and restart the timer*/
               euUeCb->expiryCnt = 0;

               /* restart the inactivity timer */
               /* Fix for CID- 1657-01-01 DefectId- 115433 */
               /* Added pointer type casting to remove warning */
               veInactvStopTmr( (PTR)euUeCb, VE_TMR_INACTIVITY);
               /* Added pointer type casting to remove warning */
               veInactvStartTmr( (PTR)euUeCb, VE_TMR_INACTIVITY, smCfgCb.inactvTmrVal);
            } /* end of else part */

         } /* end of if statement */
         else
         {
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veProcInactvTmr: Invalid euUeCb \n"));
#endif
         } /* end of else part */
           
         break;
      } /* end of INACTIVITY case */
      default:
         /* Invalid Timer */
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veProcInactvTmr: Invalid timer \n"));
#endif
         break;
   } /* end of switch statement */

   RETVOID;
} /* end of veProcInactvTmr() */





/********************************************************************30**

         End of file:     ve_data_tmr.c@@/main/1 - Wed Dec  1 08:44:23 2010

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
------------ -------- ---- ----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
