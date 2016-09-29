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
  
        Name:    PDCP - TMR module file
    
        Type:    C source file
  
        Desc:    Source code for timer functions such as, 

                 - pjStartTmr
                 - pjStopTmr
                 - pjTmrExpiry
                 - pjHdlDiscTmrExp
                 - pjHdlInitObdTmrExp 
                 - pjHdlUeDelWaitTmrExp
                  
        File:    gp_pj_tmr.c

        Sid:      gp_pj_tmr.c@@/main/1 - Fri Nov 13 14:13:48 2009
 
        Prg:     vp
  
*********************************************************************21*/


/* header (.h) include files */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "cpj.h"           /* RRC layer */
#include "pju.h"           /* PDCP service user */
#include "lpj.h"           /* LPJ defines */
#include "kw_env.h"        /* RLC environment options */
#include "kw.h"            /* RLC defines */
#include "kw_err.h"        /* Error defines */

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */
#include "cpj.x"           /* RRC layer */
#include "pju.x"           /* PDCP service user */
#include "lpj.x"           /* LPJ */
#include "kw.x"

/* local defines */
/* Refer nhu.x   */
#define PJ_MAX_DISC_TMR_INT                1500

/* local externs */

/* forward references */

/* public variable declarations */

/* This structure holds all the global structs we need. */

/* private variable declarations */

/* private function declarations */
/** @file gp_pj_tmr.c
@brief PDCP Timer Module
**/

/**
 *
 * @brief Handler to start timer
 *       
 *
 * @b Description
 *        This function is invoked by DLM modules to start the discard timer for SDUs.
 *
 *
 *  @param[in] cb      Transmission Buffer control block 
 *  @param[in] tmrEvnt Timer event to be started
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
*/
#ifdef MSPD
EXTERN U32 ysPdcpTimerArr[5];
#endif
#ifdef ANSI
PUBLIC S16 pjStartTmr
(
PTR       cb,           /* Parent control block */
S16       tmrEvnt       /* Timer event */
)
#else
PUBLIC S16 pjStartTmr (cb, tmrEvnt)
PTR       cb;           /* Parent control block */
S16       tmrEvnt;      /* Timer event */
#endif
{
   PjRbCb   *pjRbCb;
   PjTxEnt  *txEnt;
   U16      wait;
   S16      waitTime;
   CmTimer  *tmr;
   U8       maxTmrs;
   U32      count;
   Ticks    curTime;
   CmTmrArg arg;
   /* kw006.201, ccpu00120574, fixed the warning */
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   KwUeCb   *ueCb;
   PjRxEnt  *rxEnt;
#endif
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   PjL2MeasEvtCb *measEvtCb;
#endif
   TRC2(pjStartTmr)

   PJDBGP((KW_DBGMASK_DL | KW_DBGMASK_BRIEF | KW_PJ_DBGMASK_UTL),
      (_kwPBuf, "pjStartTmr(cb, tmrEvnt (%d)) \n", tmrEvnt));
   MSPD_DBG_RLC("pjStartTmr(cb, tmrEvnt (%d)) \n", tmrEvnt);

   pjRbCb  = NULLP;
   txEnt   = NULLP;
   wait    = 0;
   waitTime = 0;
   tmr     = NULLP;
   maxTmrs = 0;
   count   = 0;
   curTime = 0;
   /* kw006.201, ccpu00120574, fixed the warning */
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   ueCb    = NULLP;
   rxEnt   = NULLP;
#endif

   switch (tmrEvnt)
   {
      case PJ_EVT_DISC_TMR:
      {
         pjRbCb = (PjRbCb *)cb;
         tmr = &(pjRbCb->dlCb.discTmr);
         maxTmrs = PJ_MAX_DISC_TMR;
         /* MS_FIX for DISC TMR */
         if(pjRbCb->dlCb.discTmrInt > PJ_MAX_DISC_TMR_INT)
         {
            wait = 0;
            pjRbCb->dlCb.discTmrInt = 0;
         }
         else
         {
#ifdef MSPD
            ysPdcpTimerArr[0]++;
#endif
            count = pjRbCb->dlCb.txBuf.numEntries;
            if(count > 0)
            {
               if ( pjRbCb->mode == PJ_DRB_AM)
                   txEnt = pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), pjRbCb->dlCb.cfmExp);
               else 
                   txEnt = pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), pjRbCb->dlCb.nxtToSub);
               SGetSysTime(&curTime);
            }
            if ( txEnt != NULLP )
            {
               waitTime  =  pjRbCb->dlCb.discTmrInt - (( curTime - txEnt->arrTime )/ kwCb.genCfg.timeRes);
               if(waitTime > 1)
               {
                  wait = (U16) waitTime;
               }
               else
               {
                  wait  =  1;
               }
            }
         }
         break;
      }
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
      case PJ_EVT_DL_OBD_TMR:
      {
         pjRbCb = (PjRbCb *)cb;
         tmr=&(pjRbCb->dlCb.obdTmr);
         maxTmrs = PJ_MAX_OBD_TMR;
         txEnt = pjDbmGetTxEnt(&(pjRbCb->dlCb.txBuf), pjRbCb->dlCb.nxtToSub);
         if ( txEnt != NULLP )
         {
            wait = kwCb.pjGenCfg.obdTmrInt;
            pjRbCb->dlCb.obdCount = txEnt->count;
#ifdef MSPD
            ysPdcpTimerArr[1]++;
#endif
         }

         break;
      }
      case PJ_EVT_UL_OBD_TMR:
      {
         pjRbCb = (PjRbCb *)cb;
         tmr=&(pjRbCb->ulCb.obdTmr);
         maxTmrs = PJ_MAX_OBD_TMR;
        /* kw006.201 ccpu00118629, replaced macro with function */
         pjUlmGetSubCnt(pjRbCb, &(pjRbCb->ulCb.obdPdu));
         rxEnt = pjDbmGetRxEnt(&(pjRbCb->ulCb.recBuf), pjRbCb->ulCb.obdPdu);
         if ( rxEnt != NULLP )
         {
#ifdef MSPD
            ysPdcpTimerArr[2]++;
#endif
            wait = kwCb.pjGenCfg.obdTmrInt;
         }

         break;
      } 
      case PJ_EVT_OBD_WAIT_TMR:
      {
         ueCb = (KwUeCb *)cb;
         tmr=&(ueCb->libInfo.obdTmr);
         maxTmrs = PJ_MAX_OBD_TMR;
         wait = kwCb.pjGenCfg.obdWtTmrInt;
#ifdef MSPD
         ysPdcpTimerArr[3]++;
#endif
         break;
      }

#endif
      /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
      case PJ_EVT_L2_TMR:
      {
         measEvtCb = (PjL2MeasEvtCb *)cb;
         tmr=&(measEvtCb->l2Tmr);
         maxTmrs = PJ_L2_MAX_TIMERS;
#ifdef MSPD
         wait = (measEvtCb->l2TmrCfg.val)/(kwCb.genCfg.timeRes);
         ysPdcpTimerArr[4]++;
#else
         wait = (measEvtCb->l2TmrCfg.val * SS_TICKS_SEC)/(kwCb.genCfg.timeRes * 1000);
#endif
         if((measEvtCb->l2TmrCfg.val > 0) && wait == 0)
         {
            wait = 1;
         }
         break;
      }
#endif
      default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         KWLOGERROR(ERRCLS_INT_PAR, EKW226, (ErrVal) 0,
                    "pjStartTmr: Invalid tmrEvnt");
#endif
      }
   } 

   if(wait != 0)
   {
      arg.tqCp   = &kwCb.kwTqCp;
      arg.tq     = kwCb.kwTq;
      arg.cb     = cb;
      arg.timers = tmr;
      arg.evnt   = tmrEvnt;
      arg.wait   = wait;
      arg.tNum   = 0;
      arg.max    = maxTmrs;

      cmPlcCbTq(&arg);
   }

   RETVALUE(ROK);
}

/**
 *
 * @brief Handler to stop timer
 *       
 *
 * @b Description
 *        This function is used to stop protocol timer, based on the timer event.
 *
 *
 *  @param[in] cb      Transmission Buffer control block 
 *  @param[in] tmrEvnt Timer event to be started 
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
*/
#ifdef ANSI
PUBLIC S16 pjStopTmr
(
PTR    cb,              /* Parent control block */
U8     tmrEvnt          /* Timer */
)
#else
PUBLIC S16 pjStopTmr (cb, tmrEvnt)
PTR    cb;              /* Parent control block */
U8     tmrEvnt;         /* Timer */
#endif
{
   CmTmrArg     arg;
   CmTimer      *tmr;
   PjRbCb       *pjRbCb;
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   KwUeCb       *ueCb;
#endif
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   PjL2MeasEvtCb *measEvtCb;
#endif
   U8           maxTimers;

   TRC2(pjStopTmr)
   
   PJDBGP((KW_DBGMASK_DL | KW_DBGMASK_BRIEF | KW_PJ_DBGMASK_UTL),
      (_kwPBuf, "pjStopTmr(cb, tmrEvnt(%d)) \n", tmrEvnt));
   
   tmr = NULLP;
   maxTimers = 0;
   pjRbCb    = NULLP;
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   ueCb   = NULLP;
#endif

   switch (tmrEvnt)
   {
      case PJ_EVT_DISC_TMR:
      {
         pjRbCb = (PjRbCb *)cb;
         tmr = &(pjRbCb->dlCb.discTmr);
         maxTimers = PJ_MAX_DISC_TMR;
#ifdef MSPD
         ysPdcpTimerArr[0]--;
#endif
         break;
      }
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
      case PJ_EVT_DL_OBD_TMR:
      {
         pjRbCb = (PjRbCb *)cb;
         tmr = &(pjRbCb->dlCb.obdTmr);
         maxTimers = PJ_MAX_OBD_TMR;
#ifdef MSPD
         ysPdcpTimerArr[1]--;
#endif
         break;
      }
      case PJ_EVT_UL_OBD_TMR:
      {
         pjRbCb = (PjRbCb *)cb;
         tmr = &(pjRbCb->ulCb.obdTmr);
         maxTimers = PJ_MAX_OBD_TMR;
#ifdef MSPD
         ysPdcpTimerArr[2]--;
#endif
         break;
      }
      case PJ_EVT_OBD_WAIT_TMR:
      {
         ueCb = (KwUeCb *)cb;
         tmr  = &(ueCb->libInfo.obdTmr);
         maxTimers = PJ_MAX_OBD_TMR;
#ifdef MSPD
         ysPdcpTimerArr[3]--;
#endif
         break;
      }

#endif
     /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
      case PJ_EVT_L2_TMR:
      {
         measEvtCb = (PjL2MeasEvtCb *)cb;
         tmr  = &(measEvtCb->l2Tmr);
         maxTimers = PJ_L2_MAX_TIMERS;
#ifdef MSPD
         ysPdcpTimerArr[4]--;
#endif
         break;
      }
#endif
      default:
         break;
   }
   
   
   if (tmrEvnt != TMR0)
   {
      arg.tqCp    = &kwCb.kwTqCp;
      arg.tq      = kwCb.kwTq;
      arg.cb      = (PTR)cb;
      arg.timers  = tmr;
      arg.evnt    = tmrEvnt;
      arg.wait    = 0;
      arg.max     = maxTimers;
      arg.tNum    = 0;
      cmRmvCbTq(&arg);

   }
   
   RETVALUE(ROK);
}

/**
 *
 * @brief Handler to invoke events on expiry of timer.
 *       
 *
 * @b Description
 *        This function is used to handle expiry of timer,it invokes relevant functions.
 *
 *
 *  @param[in] cb        Transmission Buffer control block 
 *  @param[in] tmrType   Type of the timer to be stopped
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
*/
#ifdef ANSI
PUBLIC S16 pjTmrExpiry
(
PTR       cb,           /* Parent control block */
S16       tmrEvnt       /* Timer event */
)
#else
PUBLIC S16 pjTmrExpiry (cb, tmrEvnt)
PTR       cb;           /* Parent control block */
S16       tmrEvnt;      /* Timer event */
#endif
{
   PjRbCb      *pjRbCb        = NULLP;
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   KwUeCb      *ueCb          = NULLP;
#endif 
   /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   PjL2MeasEvtCb *measEvtCb;
#endif
   TRC2(pjTmrExpiry)

   PJDBGP((KW_DBGMASK_DL | KW_DBGMASK_BRIEF | KW_PJ_DBGMASK_UTL),
      (_kwPBuf, "pjTmrExpiry(cb, tmrEvnt(%d)) \n", tmrEvnt));
   switch (tmrEvnt)
   {
      case PJ_EVT_DISC_TMR:
      {
#ifdef MSPD
         stop_printf("DISC_TMR expired\n");
#endif
         pjRbCb = (PjRbCb *)cb;
         pjDlmDiscTmrExp(pjRbCb);
#ifdef MSPD
         ysPdcpTimerArr[0]--;
#endif
         break;
      }
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
      case PJ_EVT_DL_OBD_TMR:
      {
         pjRbCb = (PjRbCb *)cb;
         pjDlmObdTmrExp(pjRbCb);
#ifdef MSPD
         ysPdcpTimerArr[1]--;
#endif
         break;
      }
      case PJ_EVT_UL_OBD_TMR:
      {
         pjRbCb = (PjRbCb *)cb;
         pjUlmHdlObdTmrExpiry(pjRbCb);
#ifdef MSPD
         ysPdcpTimerArr[2]--;
#endif
         break;
      }
      case PJ_EVT_OBD_WAIT_TMR:
      {
         ueCb = (KwUeCb *)cb;
         pjUtlHdlInitObdTmrExp(ueCb);
#ifdef MSPD
         ysPdcpTimerArr[3]--;
#endif
         break;
      }

#endif
     /* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
      case PJ_EVT_L2_TMR:
      {
         measEvtCb = (PjL2MeasEvtCb *)cb;
         pjUtlHdlL2TmrExp(measEvtCb);
#ifdef MSPD
         ysPdcpTimerArr[4]--;
#endif
         break;
      }
#endif
      default:
         break;
   }

   RETVALUE(ROK);
}

/**
 *
 * @brief Handler to check if the timer is running
 *       
 *
 * @b Description
 *        This function is used to check if the timer is running at the given
 *        moment.
 *
 *
 *  @param[in] cb      Transmission Buffer control block 
 *  @param[in] tmrType   Type of the timer to be stopped 
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PUBLIC Bool pjChkTmr
(
PTR       cb,           /* Parent control block */
S16       tmrEvnt       /* Timer event */
)
#else
PUBLIC Bool pjChkTmr(cb, tmrEvnt)
PTR       cb;           /* Parent control block */
S16       tmrEvnt;      /* Timer event */
#endif
{
   PjRbCb  *pjCb         = NULLP;
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
   KwUeCb  *ueCb         = NULLP;
#endif
   Bool tmrRunning = FALSE;

   TRC2(pjChkTmr)

   PJDBGP((KW_DBGMASK_DL | KW_DBGMASK_BRIEF | KW_PJ_DBGMASK_UTL),
      (_kwPBuf, "pjChkTmr(cb, tmrEvnt(%d)) \n", tmrEvnt));

   switch (tmrEvnt)
   {
      case PJ_EVT_DISC_TMR:
      {
         pjCb = (PjRbCb *)cb;
         if (pjCb->dlCb.discTmr.tmrEvnt == PJ_EVT_DISC_TMR)
         {
            tmrRunning = TRUE;
         }
         break;
      }
#if (defined(PJ_SEC_ASYNC) || defined(PJ_CMP_ASYNC))
      case PJ_EVT_DL_OBD_TMR:
      {
         pjCb = (PjRbCb *)cb;
         if (pjCb->dlCb.obdTmr.tmrEvnt == PJ_EVT_DL_OBD_TMR)
         {
            tmrRunning = TRUE;
         }
         break;
      }
      case PJ_EVT_UL_OBD_TMR:
      {
         pjCb = (PjRbCb *)cb;
         if (pjCb->ulCb.obdTmr.tmrEvnt == PJ_EVT_UL_OBD_TMR)
         {
            tmrRunning = TRUE;
         }
         break;
      }
      case PJ_EVT_OBD_WAIT_TMR:
      {
         ueCb = (KwUeCb *)cb;
         if (ueCb->libInfo.obdTmr.tmrEvnt == PJ_EVT_OBD_WAIT_TMR)
         {
            tmrRunning = TRUE;
         }
         break;
      }
#endif
      default:
      {

#if (ERRCLASS & ERRCLS_INT_PAR)
         KWLOGERROR(ERRCLS_INT_PAR, EKW227, (ErrVal)0,
                    "pjChkTmr: Invalid tmrEvnt");
#endif
      }
   } 

   RETVALUE(tmrRunning);
}

  
/********************************************************************30**
  
         End of file:     gp_pj_tmr.c@@/main/1 - Fri Nov 13 14:13:48 2009
  
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
/main/1      ---      vp   1. LTERLC Release 2.1
/main/2     kw005.201 ap   1. Added support for L2 Measurement.
/main/3     kw006.201 ap   1. Replaced macro with function.                    
                           2. ccpu00120574, fixed the warning.
*********************************************************************91*/
