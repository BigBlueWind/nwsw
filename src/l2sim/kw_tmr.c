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
  
        Name:    RLC - TMR module file
    
        Type:    C source file
  
        Desc:    Source code for timer functions such as, 

                 - kwStartTmr
                 - kwStopTmr
                 - kwTmrExpiry
                 - kwBndTmrExpiry  
                  
        File:    gp_tmr.c

        Sid:      gp_tmr.c@@/main/2 - Fri Nov 13 14:14:01 2009
 
        Prg:     nm
  
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
#include "kw_env.h"        /* RLC environment options */
#ifdef KW_PDCP
#include "lpj.h"           /* LPJ defines */
#include "cpj.h"           /* CPJ defines */
#include "pju.h"           /* PJU defines */
#endif

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
#ifdef KW_PDCP
#include "lpj.x"           /* LPJ defines */
#include "cpj.x"           /* CPJ defines */
#include "pju.x"           /* PJU defines */
#endif

#include "kw.x"

/* local defines */

/* local externs */

/* forward references */

/* public variable declarations */

/* This structure holds all the global structs we need. */

/* private variable declarations */

/* private function declarations */
PRIVATE Void kwBndTmrExpiry(PTR cb);

/** @file gp_tmr.c
@brief RLC Timer Module
**/

/**
 *
 * @brief Handler to start timer
 *       
 *
 * @b Description
 *        This function is invoked by AMM/UMM to start the relevant timers for reordering
 *
 *
 *  @param[in] cb      RB control block 
 *  @param[in] tmrEvnt Timer event to be started
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
*/
#ifdef ANSI
PUBLIC S16 kwStartTmr
(
PTR       cb,           /* Parent control block */
S16       tmrEvnt       /* Timer event */
)
#else
PUBLIC S16 kwStartTmr (cb, tmrEvnt)
PTR       cb;           /* Parent control block */
S16       tmrEvnt;      /* Timer event */
#endif
{
   KwRbCb *rbCb         = NULLP;
   KwAmDl *amDl         = NULLP;
   KwAmUl *amUl         = NULLP;
   KwRguSapCb *rguSap   = NULLP;
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   KwL2MeasEvtCb *measEvtCb = NULLP;
#endif

   U16         wait     = 0;
   CmTmrArg    arg;
   CmTimer     *tmr     = NULLP;
   U8          maxTmrs  = 0;

   TRC2(kwStartTmr)

   /* kw002.201 Adjusting the wait time as per timeRes configured by layer manager */
   switch (tmrEvnt)
   {
      case KW_EVT_UMUL_REORD_TMR:
      {
         rbCb = (KwRbCb *)cb;
#ifdef MSPD
	 /* MS_WORKAROUND : Reducing tmrVal, as with higher values, 
	  * RLC becomes a bottleneck */
	 UMUL.reOrdTmrInt = 30;
         wait = (UMUL.reOrdTmrInt)/(kwCb.genCfg.timeRes);
#else
         wait = (UMUL.reOrdTmrInt * SS_TICKS_SEC)/(kwCb.genCfg.timeRes * 1000);
#endif
         /* kw005.201 Changed wait calculation ccpu00117634*/ 
         wait = (UMUL.reOrdTmrInt * SS_TICKS_SEC)/(kwCb.genCfg.timeRes * 1000);
         if( (0 != UMUL.reOrdTmrInt) && (0 == wait) )
         {
            wait = 1;
         }
         tmr  = &UMUL.reOrdTmr;
         maxTmrs = KW_MAX_UM_TMR;
         break;
      }
      case KW_EVT_AMUL_REORD_TMR:
      {
         rbCb = (KwRbCb *)cb;
         amUl = &(rbCb->m.am.amUl);
         /* kw005.201 Changed wait calculation ccpu00117634*/ 
#ifdef MSPD
         wait = (amUl->reOrdTmrInt)/(kwCb.genCfg.timeRes);
#else
         wait = (amUl->reOrdTmrInt * SS_TICKS_SEC)/(kwCb.genCfg.timeRes * 1000);
#endif
         if( (0 != amUl->reOrdTmrInt) && (0 == wait) )
         {
            wait = 1;
         }
         tmr  = &amUl->reOrdTmr;
         maxTmrs = KW_MAX_AM_TMR;
         break;
      }
      case KW_EVT_AMUL_STA_PROH_TMR:
      {
         rbCb = (KwRbCb *)cb;
         amUl = &(rbCb->m.am.amUl);
         /* kw005.201 Changed wait calculation ccpu00117634*/ 
#ifdef MSPD
         wait = (amUl->staProhTmrInt)/(kwCb.genCfg.timeRes);
#else
         wait = (amUl->staProhTmrInt * SS_TICKS_SEC)/(kwCb.genCfg.timeRes * 1000);
#endif
         if( (0 != amUl->staProhTmrInt) && (0 == wait) )
         {
            wait = 1;
         }
         tmr  = &amUl->staProhTmr;
         maxTmrs = KW_MAX_AM_TMR;
         break;
      } 
      case KW_EVT_AMDL_POLL_RETX_TMR:
      {
         rbCb = (KwRbCb *)cb;
         amDl = &(rbCb->m.am.amDl);
         /* kw005.201 Changed wait calculation ccpu00117634*/ 
#ifdef MSPD
         wait = (amDl->pollRetxTmrInt)/(kwCb.genCfg.timeRes);
#else
         wait = (amDl->pollRetxTmrInt * SS_TICKS_SEC)/(kwCb.genCfg.timeRes * 1000);
#endif
         if( (0 != amDl->pollRetxTmrInt) && (0 == wait) )
         {
            wait = 1;
         }
         tmr  = &amDl->pollRetxTmr;
         maxTmrs = KW_MAX_AM_TMR;
         break;
      } 
      case KW_EVT_WAIT_BNDCFM:
      {
         rguSap = (KwRguSapCb *)cb;
         /* kw005.201 Changed wait calculation ccpu00117634*/ 
#ifdef MSPD
         wait   = (rguSap->bndTmrInt)/(kwCb.genCfg.timeRes);
#else
         wait   = (rguSap->bndTmrInt * SS_TICKS_SEC)/(kwCb.genCfg.timeRes * 1000);
#endif
         if( (0 != rguSap->bndTmrInt) && (0 == wait) )
         {
            wait = 1;
         }
         tmr    = &rguSap->bndTmr;
         maxTmrs = KW_MAX_RGUSAP_TMR;
         break;
      }
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
      case KW_EVT_L2_TMR:
      {
         measEvtCb = (KwL2MeasEvtCb *)cb;
         /* kw005.201 Changed wait calculation ccpu00117634*/ 
#ifdef MSPD
         wait = (measEvtCb->l2TmrCfg.val)/(kwCb.genCfg.timeRes);
#else
         wait = (measEvtCb->l2TmrCfg.val * SS_TICKS_SEC)/(kwCb.genCfg.timeRes * 1000);
#endif
         if( (0 != measEvtCb->l2TmrCfg.val) && (0 == wait) )
         {
            wait = 1;
         }
         tmr       = &measEvtCb->l2Tmr;
         maxTmrs   = KW_L2_MAX_TIMERS;
         break;
      }
#endif
      default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         KWLOGERROR(ERRCLS_INT_PAR, EKW204, (ErrVal) 0,
                    "kwStartTmr: Invalid tmrEvnt");
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
 *  @param[in] cb      RB control block 
 *  @param[in] tmrEvnt Timer event to be started 
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
*/
#ifdef ANSI
PUBLIC S16 kwStopTmr
(
PTR    cb,              /* Parent control block */
U8     tmrType          /* Timer */
)
#else
PUBLIC S16 kwStopTmr (cb, tmrType)
PTR    cb;              /* Parent control block */
U8     tmrType;         /* Timer */
#endif
{
   KwRbCb *rbCb         = NULLP;
   KwAmDl *amDl         = NULLP;
   KwAmUl *amUl         = NULLP;
   KwRguSapCb *rguSap   = NULLP;

   CmTmrArg   arg;
   CmTimer    *timers   = NULLP;
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   KwL2MeasEvtCb *measEvtCb = NULLP;
#endif
   TRC2(kwStopTmr)
  
   switch (tmrType)
   {
      case KW_EVT_UMUL_REORD_TMR:
      {
         rbCb = (KwRbCb *)cb;
         timers  = &UMUL.reOrdTmr;
         arg.max = KW_MAX_UM_TMR;
         break;
      }
      case KW_EVT_AMUL_REORD_TMR:
      {
         rbCb = (KwRbCb *)cb;
         amUl = &(rbCb->m.am.amUl);
         timers = &amUl->reOrdTmr;
         arg.max = KW_MAX_AM_TMR;
         break;
      }
      case KW_EVT_AMUL_STA_PROH_TMR:
      {
         rbCb = (KwRbCb *)cb;
         amUl = &(rbCb->m.am.amUl);
         timers = &amUl->staProhTmr;
         arg.max = KW_MAX_AM_TMR;
         break;
      } 
      case KW_EVT_AMDL_POLL_RETX_TMR:
      {
         rbCb = (KwRbCb *)cb;
         amDl = &(rbCb->m.am.amDl);
         timers = &amDl->pollRetxTmr;
         arg.max = KW_MAX_AM_TMR;
         break;
      } 
      case KW_EVT_WAIT_BNDCFM:
      {
         rguSap = (KwRguSapCb *)cb;
         timers = &rguSap->bndTmr;
         arg.max = KW_MAX_RGUSAP_TMR;
         break;
      }
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
      case KW_EVT_L2_TMR:
      {
         measEvtCb = (KwL2MeasEvtCb *)cb;
         timers   = &measEvtCb->l2Tmr;
         arg.max  = KW_L2_MAX_TIMERS;
         break;
      }
#endif
      default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         KWLOGERROR(ERRCLS_INT_PAR, EKW205, (ErrVal) 0,
                    "kwStopTmr : Invalid tmrEvnt");
#endif
      }
   } 
   if (tmrType != TMR0)
   {
      arg.tqCp    = &kwCb.kwTqCp;
      arg.tq      = kwCb.kwTq;
      arg.cb      = cb;
      arg.timers  = timers;
      arg.evnt    = tmrType;
      arg.wait    = 0;
      arg.tNum  = 0;
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
 *  @param[in] rbCb      RB control block 
 *  @param[in] tmrType   Type of the timer to be stopped
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
*/
#ifdef ANSI
PUBLIC S16 kwTmrExpiry
(
PTR       cb,           /* Parent control block */
S16       tmrEvnt       /* Timer event */
)
#else
PUBLIC S16 kwTmrExpiry (cb, tmrEvnt)
PTR       cb;           /* Parent control block */
S16       tmrEvnt;      /* Timer event */
#endif
{
   KwRbCb *rbCb         = NULLP;
   /* kw006.201, ccpu00120574, fixed the warning */
/* kw005.201 added support for L2 Measurement */
#ifdef LTE_L2_MEAS
   KwL2MeasEvtCb *measEvtCb = NULLP;
#endif

   TRC2(kwTmrExpiry)
   switch (tmrEvnt)
   {
#if 0     
      case KW_EVT_UMUL_REORD_TMR:
      {
         rbCb = (KwRbCb *)cb;
         kwUmmReOrdTmrExp(rbCb);

         break;
      }
      case KW_EVT_AMUL_REORD_TMR:
      {
         rbCb = (KwRbCb *)cb;
         /* kw006.201, ccpu00120574, fixed the warning */
         kwAmmReOrdTmrExp(rbCb);
         break;
      }
      case KW_EVT_AMUL_STA_PROH_TMR:
      {
         rbCb = (KwRbCb *)cb;
         /* kw006.201, ccpu00120574, fixed the warning */
         kwAmmStaProTmrExp(rbCb);

         break;
      }
      case KW_EVT_AMDL_POLL_RETX_TMR:
      {
         rbCb = (KwRbCb *)cb;
         /* kw006.201, ccpu00120574, fixed the warning */
         kwAmmPollRetxTmrExp(rbCb);
         /* kw005.201 ccpu00117318, updating the statistics */
         kwCb.genSts.protTimeOut++;
         break;
      }
#endif      
      case KW_EVT_WAIT_BNDCFM:
      {
         kwBndTmrExpiry(cb);
         /* kw005.201 added missing break statement */
         break;
      }
#if 0      
      /* kw005.201 L2 Measurement support */
#ifdef LTE_L2_MEAS
      case KW_EVT_L2_TMR:
      {
         measEvtCb = (KwL2MeasEvtCb *)cb;
         kwUtlHdlL2TmrExp(measEvtCb);
         break;
      }
#endif
#endif      
      default:
      {
        //logInfo("%s", "PDCP timer expired ");
#if 0        
#ifdef KW_PDCP
         pjTmrExpiry(cb, tmrEvnt);
#endif /* KW_PDCP */
#endif         
         break;
      }
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
 *  @param[in] rbCb      RB control block 
 *  @param[in] tmrType   Type of the timer to be stopped 
 *
 * 
 *  @return  S16
 *      -# ROK 
 *      -# RFAILED 
 */
#ifdef ANSI
PUBLIC Bool kwChkTmr
(
PTR       cb,           /* Parent control block */
S16       tmrEvnt       /* Timer event */
)
#else
PUBLIC Bool kwChkTmr(cb, tmrEvnt)
PTR       cb;           /* Parent control block */
S16       tmrEvnt;      /* Timer event */
#endif
{
   KwRbCb *rbCb         = NULLP;
   KwAmDl *amDl         = NULLP;
   KwAmUl *amUl         = NULLP;
   KwRguSapCb *rguSap   = NULLP;
   Bool tmrRunning = FALSE;

   TRC2(kwChkTmr)

   switch (tmrEvnt)
   {
      case KW_EVT_UMUL_REORD_TMR:
      {
         rbCb = (KwRbCb *)cb;
         if (UMUL.reOrdTmr.tmrEvnt == KW_EVT_UMUL_REORD_TMR)
         {
            tmrRunning = TRUE;
         }
         break;
      }
      case KW_EVT_AMUL_REORD_TMR:
      {
         rbCb = (KwRbCb *)cb;
         amUl = &(rbCb->m.am.amUl);
         if (amUl->reOrdTmr.tmrEvnt == KW_EVT_AMUL_REORD_TMR)
         {
            tmrRunning = TRUE;
         }
         break;
      }
      case KW_EVT_AMUL_STA_PROH_TMR:
      {
         rbCb = (KwRbCb *)cb;
         amUl = &(rbCb->m.am.amUl);
         if (amUl->staProhTmr.tmrEvnt == KW_EVT_AMUL_STA_PROH_TMR)
         {
            tmrRunning = TRUE;
         }
         break;
      } 
      case KW_EVT_AMDL_POLL_RETX_TMR:
      {
         rbCb = (KwRbCb *)cb;
         amDl = &(rbCb->m.am.amDl);
         if (amDl->pollRetxTmr.tmrEvnt == KW_EVT_AMDL_POLL_RETX_TMR)
         {
            tmrRunning = TRUE;
         }
         break;
      } 
      case KW_EVT_WAIT_BNDCFM:
      {
         rguSap = (KwRguSapCb *)cb;
         if (rguSap->bndTmr.tmrEvnt == KW_EVT_WAIT_BNDCFM)
         {
            tmrRunning = TRUE;
         }
         break;
      }
      default:
      {

#if (ERRCLASS & ERRCLS_INT_PAR)
         KWLOGERROR(ERRCLS_INT_PAR, EKW206, (ErrVal)0,
                    "kwChkTmr: Invalid tmrEvnt");
#endif
      }
   } 

   RETVALUE(tmrRunning);
}

/**
 *
 * @brief Private handler to invoke an event for bind timer expiry
 *       
 *
 * @b Description
 *        This function processes the RLC bind timer expiry. If the number of retry is
 *        less than the maximum retry counter, bind request is sent again, else an 
 *        alarm is raised to the layer manager.
 *
 *
 *  @param[in] cb      RB control block 
 *
 * 
 *   @return Void
 *      -# RETVOID
*/
#ifdef ANSI
PRIVATE Void kwBndTmrExpiry
(
PTR       cb            /* Parent control block */
)
#else
PRIVATE Void kwBndTmrExpiry(cb)
PTR       cb;           /* Parent control block */
#endif
{
   KwRguSapCb *rguSapCb; 

   TRC2(kwBndTmrExpiry)

   rguSapCb = (KwRguSapCb *) cb;

   if (rguSapCb->state == KW_SAP_BINDING)
   {
      if (rguSapCb->retryCnt < KW_MAX_SAP_BND_RETRY)
      {
         /* Send bind request */
         rguSapCb->retryCnt++;

         /* start timer to wait for bind confirm */
         kwStartTmr( (PTR)rguSapCb, KW_EVT_WAIT_BNDCFM);

         KwLiRguBndReq (&rguSapCb->pst, rguSapCb->suId, rguSapCb->spId);
      }
      else
      {
         rguSapCb->retryCnt = 0;
         rguSapCb->state = KW_SAP_CFG;

         /* Send alarm to the layer manager */
#ifdef LTE_L2_MEAS
         kwLmmSendAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                        LCM_CAUSE_TMR_EXPIRED, 0, 0, 0);
#else
         kwLmmSendAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,
                        LCM_CAUSE_TMR_EXPIRED, 0, 0);
#endif
      }
   }

   RETVOID;
}


  
/********************************************************************30**
  
         End of file:     gp_tmr.c@@/main/2 - Fri Nov 13 14:14:01 2009
  
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
/main/1      ---      vp   1. initial release.
/main/2      ---      nm   1. LTERLC Release 2.1
/main/3     kw002.201 vp   1. Patch for optimization,fixes for multi-region
                               and retransmission.
/main/4     kw005.201 ap   1. Updated for statistics.
                           2. Added L2 measurement support.
                           3. Changed wait caculation ccpu00117634
                      rd   4. If wait is coming to be 0 and the timer is
                              configured, then wait is set to 1. Change is
                              done is kwStartTmr. 
/main/5     kw006.201 ap   5. ccpu00120574, fixed the warning.
*********************************************************************91*/
