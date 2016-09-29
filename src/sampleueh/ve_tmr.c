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

     File:    ve_tmr.c

     Sid:      ve_tmr.c@@/main/2 - Wed Dec  1 08:44:41 2010

     Prg:     aj
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
#ifndef MSPD
#include"ve_dflt.h"
#else
#include "ve_dflt_ms.x"
#endif
/* RRM related includes */
#include "lve.x" 
#include "ve.x" 
#ifdef LTE_HO_SUPPORT
EXTERN VeS1SrcFSM veS1SrcFSM[VE_HO_MAX_STATES][VE_HO_SRC_MAX_EVENTS];
EXTERN VeX2TgtFSM veX2TgtFSM[VE_HO_MAX_STATES][VE_HO_TGT_MAX_EVENTS];
#endif

PRIVATE Void veProcTmr ARGS ((Ptr cb, S16 tmrEvnt));
/* ADD CRID:ccpu00118776 RRC CON timer */   
PRIVATE Void veProcRrcConTmr ARGS ((Ptr cb));

/*
 *
 *       Fun:   veRegInitTmr
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
PUBLIC S16 veRegInitTmr
(
Void
)
#else
PUBLIC S16 veRegInitTmr()
#endif
{
   U16 idx;
   S16 ret;
#ifdef VE_PERF_DL_DATA
   U16 cnt;
#endif 

   TRC2(veRegInitTmr);

   ret = SRegTmr((Ent)ENTVE, (Inst)0, VE_TMR_RES, veActvTmr);
   if (ret != ROK)
   {
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veRegInitTmr: SRegTmr Failed \n"));
#endif
      RETVALUE(RFAILED);
   }

   /* initialize timing queues */
   veCb.tqCp.tmrLen      = VE_APP_TQSIZE;

   for (idx = 0; idx < VE_APP_TQSIZE; idx++)
   {
      veCb.tq[idx].first    = NULLP;
      veCb.tq[idx].tail     = NULLP;
   }
#ifdef VE_PERF_DL_DATA
     veEuCb.tqCp.tmrLen = VE_NUM_TQENTRY;
     for(cnt = 0; cnt < VE_NUM_TQENTRY; cnt++)
     {
      veEuCb.tq[cnt].first = NULLP;
      veEuCb.tq[cnt].tail = NULLP;
     }
#endif

   RETVALUE(ROK);

} /* veRegInitTmr */

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
PUBLIC S16 veActvTmr
(
void
)
#else
PUBLIC S16 veActvTmr()
#endif
{
   TRC3(veActvTmr);

   cmPrcTmr(&veCb.tqCp, veCb.tq, (PFV) veProcTmr);

#ifdef VE_PERF_DL_DATA
   cmPrcTmr(&veEuCb.tqCp, veEuCb.tq, (PFV) vePerfTmrExp);
#endif

   RETVALUE(ROK);

} /* end of veActvTmr */


/*
*
*       Fun:    veStartTmr
*
*       Desc:   start a timer
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   ve_tmr.c
*
*/

#ifdef ANSI
PUBLIC Void veStartTmr
(
PTR                 cb,
S16                 tmrEvnt,
U32                 delay
)
#else
PUBLIC Void veStartTmr (cb, tmrEvnt, delay)
PTR                 cb;
S16                 tmrEvnt;
U32                 delay;
#endif /* ANSI */
{
   U16         wait;
   CmTmrArg    arg;
   CmTimer     *tmr;
   U8          maxTmrs;
   VeLiSapCb  *sapCb;
#ifdef LTE_HO_SUPPORT
   VeUeCb *ueCb;
   /* X2_CFG_UPD_CHANGES */
   VeNghCellCb *cellCb;
#endif
   VeMmeCb   *mmeCb;

   /* Added trace macro */
   TRC2(veStartTmr)

   wait = 0;
   switch (tmrEvnt)
   {
      /* For SZT Bind Timer Timer */
      case VE_TMR_NHU_SAP_BND:
      case VE_TMR_RGR_SAP_BND:
      case VE_TMR_CTF_SAP_BND:
      case VE_TMR_SZT_SAP_BND:
      case VE_TMR_EGT_SAP_BND:
      case VE_TMR_PJU_SAP_BND:
#ifdef LTE_HO_SUPPORT
      case VE_TMR_CZT_SAP_BND:
#endif
         sapCb = ( VeLiSapCb*)cb;
         tmr      = &sapCb->timer;
         maxTmrs  = 1;
         break;

#ifdef LTE_HO_SUPPORT
      case VE_TMR_S1_RELOC_TMR:
      {
         ueCb = (VeUeCb*)cb;
         tmr      = &ueCb->s1RelocTmr;
         maxTmrs  = 1;
         break;
      }
      case VE_TMR_S1_OVRL_TMR:
      {
         ueCb = (VeUeCb*)cb;
         tmr      = &ueCb->s1OvrallTmr;
         maxTmrs  = 1;
         break;
      }
      case VE_TMR_UE_HO_ATTACH_TMR:
      {
         ueCb = (VeUeCb *)cb;
         tmr = &ueCb->ueHoAttachTmr;
         maxTmrs = 1;
         break;
      }
      case VE_TMR_X2_SETUP_TMR:
      {
        cellCb = (VeNghCellCb *)cb;
        tmr = &cellCb->x2SetUpTmr;
        maxTmrs = 1;
        break;
      }
      case VE_TMR_X2_RESET_TMR:
      {
         cellCb = (VeNghCellCb *)cb;
         tmr = &cellCb->x2ResetTmr;
         maxTmrs = 1;
         break;
      }
     /* X2_CFG_UPD_CHANGE */
      case VE_TMR_X2_CFGUPD_TMR:
      {
        cellCb = (VeNghCellCb *)cb;
        tmr = &cellCb->cfgUpdTmr;
        maxTmrs = 1;
        break;
      }
    #endif /* LTE_HO_SUPPORTED */
      case VE_TMR_S1SETUP_TMR:
      {
         mmeCb = (VeMmeCb*)cb;
         tmr = &(mmeCb->s1SetupTmr);
         maxTmrs = 1;
         break;
      }
      case VE_ENB_CFG_UPD_TMR:
      {
         mmeCb = (VeMmeCb*)cb;
         tmr = &(mmeCb->enbUpdTmr);
         maxTmrs = 1;
         break;
      }

      case VE_S1_RESET_TMR:
      {
         mmeCb = (VeMmeCb*)cb;
         tmr = &(mmeCb->s1Reset);
         maxTmrs = 1;
         break;
      }
      case VE_TMR_ENB_OVERLOAD_TMR:
      {
         VeCb *veCb1 = (VeCb *)cb;
         tmr = &(veCb1->eNBOvldTmr);
         maxTmrs = 1;
         break;
      }
#ifdef VE_TEST_CODE
      case VE_TMR_TEST_ENB_UPD:
      {
         mmeCb = (VeMmeCb*)cb;
         tmr = &(mmeCb->tstenbUpd);
         maxTmrs = 1;
         break;
      }
      case VE_TMR_TEST_ENB_RST:
      {
         mmeCb = (VeMmeCb*)cb;
         tmr = &(mmeCb->tstS1Rest);
         maxTmrs = 1;
         break;
      }
#endif /* VE_TEST_CODE */
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
      arg.tqCp   = &(veCb.tqCp);
      arg.tq     = veCb.tq;
      arg.cb     = cb;
      arg.timers = tmr;
      arg.evnt   = tmrEvnt;
      arg.wait   = wait;
      arg.tNum   = 0;
      arg.max    = maxTmrs;

      cmPlcCbTq(&arg);
   }

   RETVOID;
} /* end of veStartTmr() */


/*
*
*       Fun:    veStopTmr
*
*       Desc:   stop a timer
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   ve_tmr.c
*
*/

#ifdef ANSI
PUBLIC Void veStopTmr
(
PTR     cb,
S16     event
)
#else
PUBLIC Void veStopTmr (cb, event)
PTR     cb;
S16     event;
#endif /* ANSI */
{

   CmTmrArg     arg;
   U8           idx;
   Bool         tmrRunning;
   CmTimer      *timers = NULLP;
   U8           max = 0;
   VeLiSapCb  *sapCb;
#ifdef LTE_HO_SUPPORT
   VeUeCb      *ueCb;
   /* X2_CFG_UPD_CHANGES */
   VeNghCellCb      *cellCb;
#endif
   VeMmeCb     *mmeCb;

   TRC3(veStopTmr)

   idx = 0;

   tmrRunning = FALSE;
   switch(event)
   {
      case VE_TMR_NHU_SAP_BND:
      case VE_TMR_RGR_SAP_BND:
      case VE_TMR_CTF_SAP_BND:
      case VE_TMR_SZT_SAP_BND:
      case VE_TMR_EGT_SAP_BND:
      case VE_TMR_PJU_SAP_BND:
#ifdef LTE_HO_SUPPORT
      case VE_TMR_CZT_SAP_BND:
#endif
      {
         sapCb = (VeLiSapCb*)cb;
         max     =  1;
         if(sapCb->timer.tmrEvnt == event)
         {
            tmrRunning = TRUE;
            sapCb->bndRetryCnt = 0;
         }
         timers = &sapCb->timer;
         break;
      }
#ifdef LTE_HO_SUPPORT
      case VE_TMR_S1_RELOC_TMR:
      {
         ueCb = (VeUeCb *)cb;
         max     =  1;
         if(ueCb->s1RelocTmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &ueCb->s1RelocTmr;
         break;
      }
      case VE_TMR_S1_OVRL_TMR:
      {
         ueCb = (VeUeCb *)cb;
         max     =  1;
         if(ueCb->s1OvrallTmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &ueCb->s1OvrallTmr;
         break;
      }
      case VE_TMR_UE_HO_ATTACH_TMR:
      {
         ueCb = (VeUeCb *)cb;
         max     =  1;
         if(ueCb->ueHoAttachTmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &ueCb->ueHoAttachTmr;
         break;
      }
      case VE_TMR_X2_SETUP_TMR:
      {
        cellCb = (VeNghCellCb *)cb;
        max = 1;
     
        if ( cellCb->x2SetUpTmr.tmrEvnt == event )
        {
            tmrRunning = TRUE;
        }

        timers = &cellCb->x2SetUpTmr;
        break;
      }
      case VE_TMR_X2_RESET_TMR:
      {
         cellCb = (VeNghCellCb *)cb;
         max = 1;

         if(cellCb->x2ResetTmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &cellCb->x2ResetTmr;
         break;
      }
     /* X2_CFG_UPD_CHANGES */
      case VE_TMR_X2_CFGUPD_TMR:
      {
        cellCb = (VeNghCellCb *)cb;
        max = 1;

        if ( cellCb->cfgUpdTmr.tmrEvnt == event )
        {
            tmrRunning = TRUE;
        }

        timers = &cellCb->cfgUpdTmr;
        break;
      }
#endif

      case VE_TMR_S1SETUP_TMR:
      {
         mmeCb = (VeMmeCb *) cb;
         max = 1;
         if(mmeCb->s1SetupTmr.tmrEvnt != TMR_NONE)
         {
            tmrRunning = TRUE;
         }
         timers = &(mmeCb->s1SetupTmr);
      }
      break;
      case VE_ENB_CFG_UPD_TMR:
      {
         mmeCb = (VeMmeCb*)cb;
         max = 1;
         if(mmeCb->enbUpdTmr.tmrEvnt != TMR_NONE)
         {
            tmrRunning = TRUE;
         }
         timers = &(mmeCb->enbUpdTmr);
      }
      break;
      case VE_S1_RESET_TMR:
      {
         mmeCb = (VeMmeCb*)cb;
         max = 1;
         if(mmeCb->s1Reset.tmrEvnt != TMR_NONE)
         {
            tmrRunning = TRUE;
         }
         timers = &(mmeCb->s1Reset);
      }
      break;
      case VE_TMR_ENB_OVERLOAD_TMR:
      {
         VeCb *veCb1 = (VeCb *)cb;
         max     =  1;
         if(veCb1->eNBOvldTmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &(veCb1->eNBOvldTmr);
      }
      break;

      default:
         break;
   }
   if(tmrRunning == FALSE)
   {
      RETVOID;
   }

   /* initialize argument for common timer function */
   arg.tqCp    = &veCb.tqCp;
   arg.tq      = veCb.tq; 
   arg.timers  = timers;
   arg.cb      = cb;
   arg.evnt    = event;
   arg.wait    = 0;
   arg.max     = max;
   arg.tNum  = idx;
   cmRmvCbTq(&arg);

   RETVOID;
} /* end of veStopTmr() */


/* ADD CRID:ccpu00118776 RRC CON timer */   
#ifdef ANSI
PUBLIC Void veUeConStopTmr
(
PTR     cb,
S16     event
)
#else
PUBLIC Void veUeConStopTmr (cb, event)
PTR     cb;
S16     event;
#endif /* ANSI */
{

   CmTmrArg     arg;
   U8           idx;
   Bool         tmrRunning;
   CmTimer      *timers = NULLP;
   U8           max = 0;
   VeUeCb *ueCb;


   TRC3(veUeConStopTmr)

   idx = 0;

   tmrRunning = FALSE;
   switch(event)
   {
      case VE_TMR_RRC_CON:
      {
         ueCb = (VeUeCb *)cb;
         max     =  1;
         if(ueCb->ueConTmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &ueCb->ueConTmr;
         break;
      }
   case VE_TMR_INITUE_CON:
      {
         ueCb = (VeUeCb *)cb;
         max     =  1;
         if(ueCb->ueConTmr.tmrEvnt == event)
         {
            tmrRunning = TRUE;
         }
         timers = &ueCb->ueConTmr;
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
   arg.tqCp    = &veCb.tqCp;
   arg.tq      = veCb.tq; 
   arg.timers  = timers;
   arg.cb      = cb;
   arg.evnt    = event;
   arg.wait    = 0;
   arg.max     = max;
   arg.tNum  = idx;
   cmRmvCbTq(&arg);

   RETVOID;
} /* end of veRrcConStopTmr() */


/*
*
*       Fun:    veProcTmr
*
*       Desc:   Handle the expiration of timers
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   ve_tmr.c
*
*/

#ifdef ANSI
PRIVATE Void veProcTmr
(
Ptr     cb,
S16     event
)
#else
PRIVATE Void veProcTmr (cb, event)
Ptr     cb;
S16     event;
#endif /* ANSI */
{
   VeLiSapCb  *sapCb;
   VeUeCb      *ueCb;
   VeMmeCb   *mmeCb;
#ifdef LTE_HO_SUPPORT
   VeNghCellCb *cellCb;
   CztReTxInfo *x2RtxInfo;
#endif

   TRC3(veProcTmr)

   sapCb = (VeLiSapCb *)cb;
   switch(event)
   {
      case VE_TMR_NHU_SAP_BND:
         {
            /* call timer expiry function for sap timer */
            /*Invoke the Bind function for the corresponding Layer*/
            if((++sapCb->bndRetryCnt < sapCb->maxBndRetry) &&
                  (sapCb->sapState != LVE_SAP_BOUND))
            {
               veStartTmr ((PTR)cb, event, sapCb->bndTmr.val);
                   /* issue bind request to the service provider */
              (Void)VeLiNhuBndReq(&sapCb->pst, sapCb->suId,
                                  sapCb->spId); 

            }
            else
            {
               sapCb->bndRetryCnt = 0;
               sapCb->sapState    = LVE_SAP_UNBOUND; 
               /*Send Alarm informing that Bnd was unsuccessful*/
               veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, 
                     LVE_CAUSE_NHU_SAP_BOUND);
            }
            break;
         }


      case VE_TMR_RGR_SAP_BND:
         {
            /* call timer expiry function for sap timer */
            /*Invoke the Bind function for the corresponding Layer*/
            if(++sapCb->bndRetryCnt < sapCb->maxBndRetry)
            {
               veStartTmr ((PTR)cb, event, sapCb->bndTmr.val);
                   /* issue bind request to the service provider */
              (Void)VeLiRgrBndReq(&sapCb->pst, sapCb->suId,
                                  sapCb->spId); 

            }
            else
            {
               /*Send Alarm informing that Bnd was unsuccessful*/
               veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, 
                     LVE_CAUSE_RGR_SAP_BOUND);
            }
            break;
         }

      case VE_TMR_CTF_SAP_BND:
         {
            /* call timer expiry function for sap timer */
            /*Invoke the Bind function for the corresponding Layer*/
            if(++sapCb->bndRetryCnt < sapCb->maxBndRetry)
            {
               veStartTmr ((PTR)cb, event, sapCb->bndTmr.val);
                   /* issue bind request to the service provider */
              (Void)VeLiCtfBndReq(&sapCb->pst, sapCb->suId,
                                  sapCb->spId); 

            }
            else
            {
               sapCb->bndRetryCnt = 0;
               sapCb->sapState    = LVE_SAP_UNBOUND; 
               /*Send Alarm informing that Bnd was unsuccessful*/
               veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL,LVE_CAUSE_CTF_SAP_BOUND);
            }
            break;
         }
#ifndef LTE_ENB_PERF
       case VE_TMR_SZT_SAP_BND:
         {
            /* call timer expiry function for sap timer */
            /*Invoke the Bind function for the corresponding Layer*/
            if(++sapCb->bndRetryCnt < sapCb->maxBndRetry)
            {
               veStartTmr ((PTR)cb, event, sapCb->bndTmr.val);
                   /* issue bind request to the service provider */
              (Void)VeLiSztBndReq(&sapCb->pst, sapCb->suId,
                                  sapCb->spId); 

            }
            else
            {
               sapCb->bndRetryCnt = 0;
               sapCb->sapState    = LVE_SAP_UNBOUND; 
               /*Send Alarm informing that Bnd was unsuccessful*/
               veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, 
                     LVE_CAUSE_SZT_SAP_BOUND);
            }
            break;
         }

          case VE_TMR_EGT_SAP_BND:
         {
            /* call timer expiry function for sap timer */
            /*Invoke the Bind function for the corresponding Layer*/
            if(++sapCb->bndRetryCnt < sapCb->maxBndRetry)
            {
               veStartTmr ((PTR)cb, event, sapCb->bndTmr.val);
                   /* issue bind request to the service provider */
              (Void)VeLiEgtBndReq(&sapCb->pst, sapCb->suId,
                                  sapCb->spId); 

            }
            else
            {
               /*Send Alarm informing that Bnd was unsuccessful*/
               veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, 
                     LVE_CAUSE_EGT_SAP_BOUND);
            }
            break;
         }
#endif
          case VE_TMR_PJU_SAP_BND:
         {
            /* call timer expiry function for sap timer */
            /*Invoke the Bind function for the corresponding Layer*/
            if(++sapCb->bndRetryCnt < sapCb->maxBndRetry)
            {
               veStartTmr ((PTR)cb, event, sapCb->bndTmr.val);
                   /* issue bind request to the service provider */
              (Void)VeLiPjuBndReq(&sapCb->pst, sapCb->suId,
                                  sapCb->spId); 

            }
            else
            {
               sapCb->bndRetryCnt = 0;
               sapCb->sapState    = LVE_SAP_UNBOUND; 
               /*Send Alarm informing that Bnd was unsuccessful*/
               veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, 
                     LVE_CAUSE_PJU_SAP_BOUND);
            }
            break;
         }
/* ADD CRID:ccpu00118776 RRC CON timer */   
          case VE_TMR_RRC_CON:
          {
             veProcRrcConTmr (cb);
          }  
          break;
          case VE_TMR_INITUE_CON:
          {
             /*VeUeCb *ueCb;*/
             ueCb = (VeUeCb *)cb; 
             /* Stopping of the  Con Procedure timer*/
             veUeConStopTmr((PTR)ueCb, VE_TMR_INITUE_CON);
             /*Send RRC Connection release to UE*/
             /* Trigger RRC Connection Release Signaling */
             veSndRrcConRel(ueCb);
          }  
          break;

#ifdef LTE_HO_SUPPORT
       case VE_TMR_CZT_SAP_BND:
         {
            /* call timer expiry function for sap timer */
            /*Invoke the Bind function for the corresponding Layer*/
            if(++sapCb->bndRetryCnt < sapCb->maxBndRetry)
            {
               veStartTmr ((PTR)cb, event, sapCb->bndTmr.val);
                   /* issue bind request to the service provider */
              (Void)VeLiCztBndReq(&sapCb->pst, sapCb->suId,
                                  sapCb->spId); 

            }
            else
            {
               sapCb->bndRetryCnt = 0;
               sapCb->sapState    = LVE_SAP_UNBOUND; 
               /*Send Alarm informing that Bnd was unsuccessful*/
               veSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, 
                     LVE_CAUSE_CZT_SAP_BOUND);
            }
            break;
         } /* end of case VE_TMR_CZT_SAP_BND */
        case VE_TMR_S1_RELOC_TMR:
        {
           /* S1AP relocation timer expired */            
            ueCb = (VeUeCb *)cb;
            if(ueCb != NULLP)
            {
               /* Invokes the function veHoPrcS1HOPrepTmrExpiry when the state
                * is HO_INITIATED */
               veS1SrcFSM[ueCb->mobCtrlState][VE_EVNT_HO_PREP_TMR_EXPIRY](ueCb->s1ConCb, NULLP);
            }
           break;
        }
        case VE_TMR_S1_OVRL_TMR:
        {
            /* S1AP Overall timer expired */
            ueCb = (VeUeCb *)cb;

            if(ueCb != NULLP)
            {
               /* Invokes the function veHoPrcS1OvrAllTmrExpiry when the state
                * is HO_EXECUTION 
               */
               veS1SrcFSM[ueCb->mobCtrlState][VE_EVNT_HO_OVERALL_TMR_EXPIRY](ueCb->s1ConCb, NULLP);
            } /* end of if statement */
           break;
        } /* end of case */
        case VE_TMR_UE_HO_ATTACH_TMR:
        {
            ueCb = (VeUeCb *)cb;
           /* UeHoAttach timer expired */

            if(ueCb != NULLP)
            {
               /* Invokes the function veHoPrcUeHoAttachTmrExpiry()  when the state
                * is HO_IN_PROGRESS
               */
               if(ueCb->hoType == VE_HO_TYPE_S1)
               {
                  veS1SrcFSM[ueCb->mobCtrlState][VE_EVNT_HO_UE_ATTACH_TMR_EXPIRY] (ueCb->s1ConCb, (void *)ueCb);
               }
               else
               {
                  veX2TgtFSM[ueCb->mobCtrlState][VE_EVNT_HO_UE_ATTACH_TMR_EXPIRY] (ueCb->x2ConCb, (void *)ueCb);
               }
            } /* end of if statement */
            break;
        } /* end of case */
        case VE_TMR_X2_SETUP_TMR:
        {
           /* X2-setup timer expired */
           cellCb = (VeNghCellCb *)cb;

           if(cellCb != NULLP)
           {
              if ( cellCb->maxSetupRetry == 0xFF )
              {
                 if( ROK == veCztX2SetupReq(cellCb->peerId))
                 {
                    cellCb->maxSetupRetry = 1;
                    veStartTmr ((PTR)cellCb, VE_TMR_X2_SETUP_TMR, VE_X2_SETUP_TMR_VAL);
                 }
                 else
                 {
                   cellCb->maxSetupRetry = 0;
                 }
              }
              else if ( ++cellCb->maxSetupRetry <= VE_MAX_X2_SETUP_RTX )
              { 

                 VE_ALLOC(&x2RtxInfo, sizeof(CztReTxInfo));
                 x2RtxInfo->peerId = cellCb->peerId; 
                 x2RtxInfo->msgType = CZT_MSG_X2_SETUP; 
                 if ( CzUiCztGpRetxReq(&veCb.cztSap[0]->pst, veCb.cztSap[0]->spId, x2RtxInfo) != ROK)
                 {
#ifdef DEBUGP
                    VE_DBG_ERROR((VE_PRNT_BUF,"veProcTmr: CzUiCztGpRetxReq failed \n"));
#endif
                 }
                 veStartTmr ((PTR)cellCb, VE_TMR_X2_SETUP_TMR, VE_X2_SETUP_TMR_VAL);
              }
              else 
              {
                 cellCb->maxSetupRetry = 0;
                 /* TODO: failure handling */
              }
           } /* end of if statement */
           break;
        } /* end of case */

        case VE_TMR_X2_RESET_TMR:
        {
           /* X2-reset timer expired */
           cellCb = (VeNghCellCb *)cb;

           if(cellCb != NULLP)
           {
              cellCb->x2ResetCb.maxResetRetry++;
              
              if ( cellCb->x2ResetCb.maxResetRetry <= VE_MAX_X2_RESET_RTX )
              { 
                 VE_ALLOC(&x2RtxInfo, sizeof(CztReTxInfo));
                 x2RtxInfo->peerId = cellCb->peerId; 
                 x2RtxInfo->msgType = CZT_MSG_RESET; 
                 if ( CzUiCztGpRetxReq(&veCb.cztSap[0]->pst, veCb.cztSap[0]->spId, x2RtxInfo) != ROK)
                 {
#ifdef DEBUGP
                    VE_DBG_ERROR((VE_PRNT_BUF,"veProcTmr: CzUiCztGpRetxReq failed \n"));
#endif
                 }
                 veStartTmr ((PTR)cellCb, VE_TMR_X2_RESET_TMR, VE_X2_RESET_TMR_VAL);
              }
              else 
              {
                cellCb->x2ResetCb.maxResetRetry = 0;
                /* TODO: failure handling */
              }
           } /* end of if statement */
           break;
        } /* end of case */
#endif
        /* This timer is started when the time to wait is sent by MME
         * for the S1-Setup Request sent */
        case VE_TMR_S1SETUP_TMR:
        {
           mmeCb = (VeMmeCb *) cb;
           /* Build and send the S1-Setup request when the timer expires */
           veSztS1SetReq(veCb.sztSap[0]->spId, mmeCb->mmeId);
        }
        break;
        /* X2_CFG_UPD_CHANGES */
#ifdef LTE_HO_SUPPORT
        case VE_TMR_X2_CFGUPD_TMR:
        {
           cellCb = (VeNghCellCb *)cb;
          
           if(cellCb != NULLP)
           {
              if ( cellCb->maxCfgupdRetry == 0xFF )
              {
                 if( ROK == veCztCfgUpdReq(cellCb->peerId, &(cellCb->cfgUpdInfo)))
                 {
                    cellCb->maxCfgupdRetry = 1;
                    veStartTmr ((PTR)cellCb, VE_TMR_X2_CFGUPD_TMR, VE_X2_CFGUPD_TMR_VAL);
                 }
                 else
                 {
               cellCb->maxCfgupdRetry = 0;
                 }
              }
              else if ( ++cellCb->maxCfgupdRetry < VE_MAX_X2_CFGUPD_RTX )
              {
                 VE_ALLOC(&x2RtxInfo, sizeof(CztReTxInfo));
                 x2RtxInfo->peerId = cellCb->peerId;
                 x2RtxInfo->msgType =  CZT_MSG_CFG_UPD_REQ; /* TODO : Modified for testing */
                 

                 /* Retransmit the request*/
                 if( CzUiCztGpRetxReq(&(veCb.cztSap[0]->pst), veCb.cztSap[0]->spId,x2RtxInfo) == ROK)
                 {
                    veStartTmr ((PTR)cellCb, VE_TMR_X2_CFGUPD_TMR, VE_X2_CFGUPD_TMR_VAL);
                 }
              }
              else
              {
                 cellCb->maxCfgupdRetry = 0;
                 /* TODO: failure handling */
              }
           } /* end of if statement */
           break;
      }/* end of case */
#endif /* LTE_HO_SUPPORT */
      /* X2_CFG_UPD_CHANGES END */ 
      case VE_ENB_CFG_UPD_TMR:
      {
         mmeCb = (VeMmeCb *) cb;
         /* Build and send the eNB configuration update message */
         veHdlEnbCfgUpdTimOut(mmeCb);
      }
      break;

      case VE_S1_RESET_TMR:
      {
         mmeCb = (VeMmeCb *) cb;
         /* Build and send the eNB configuration update message */
         veHdlEnbRsetTimOut(mmeCb);
      }
      break;
      case VE_TMR_ENB_OVERLOAD_TMR:
      {
        /* ENodeB Overload timer expired */
           /* VeCb *veCb = (VeCb *)cb;*/
            /* Invokes the function wrMonEnbOvldTmrExpiry */
            veMonEnbOvldTmrExpiry();
            veStartTmr ((PTR)cb, event, 1000);
      }
        break;
#ifdef VE_TEST_CODE
      case VE_TMR_TEST_ENB_UPD:
      {
         mmeCb = (VeMmeCb *) cb;
         veSndEnbCfgUpd();
      }
      break;
      case VE_TMR_TEST_ENB_RST:
      {
         mmeCb = (VeMmeCb *) cb;
         veTstEnbRst(mmeCb);
      }
      break;
#endif /* VE_TEST_CODE */

      default:
         /* Invalid Timer */
#ifdef DEBUGP
      VE_DBG_ERROR((VE_PRNT_BUF,"veProcTmr: Invalid timer \n"));
#endif
         break;
   }

   RETVOID;
} /* end of veProcTmr() */


/* ADD CRID:ccpu00118776 RRC CON timer */   
  
/*
*
*       Fun:    veProcRrcConTmr
*
*       Desc:   Handle the expiration of RRC Con timers
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   ve_tmr.c
*
*/

#ifdef ANSI
PRIVATE Void veProcRrcConTmr
(
Ptr     cb
)
#else
PRIVATE Void veProcRrcConTmr (cb)
Ptr     cb;
#endif /* ANSI */
{
   VeUeCb *ueCb;

   TRC3(veProcRrcConTmr)

   ueCb = (VeUeCb *)cb;
   if(ueCb)
   {
      /*Cleaning up of local resources through UE Cancel request*/
      veSendUeCnclReq(ueCb);            
   }   

   RETVOID;
} /* end of veProcRrcConTmr() */



/*
*
*       Fun:   veUeConStartTmr 
*
*       Desc:   start UE Con Procedure timer
*
*       Ret:    None
*
*       Notes:  None
*
*       File:   ve_tmr.c
*
*/

#ifdef ANSI
PUBLIC Void veUeConStartTmr 
(
PTR                 cb,
S16                 tmrEvnt,
U32                 delay
)
#else
PUBLIC Void veUeConStartTmr (cb, tmrEvnt, delay)
PTR                 cb;
S16                 tmrEvnt;
U32                 delay;
#endif /* ANSI */
{
   U16         wait;
   CmTmrArg    arg;
   CmTimer     *tmr;
   U8          maxTmrs;
   VeUeCb      *ueCb;
   U32          t300TmrVal = 0;

   TRC2(veUeConStartTmr)
      
   wait = 0;
   switch (tmrEvnt)
   {
      /* For  Rrc Connection Timer */
      case VE_TMR_RRC_CON:
         {
            /*Validation of T300 Value configured to UE and RRC Con Timer*/
            switch(veDfltTmrsAndConstsT300)
            {
               case 0:
                  t300TmrVal = 100;
                  break;
               case 1:
                  t300TmrVal = 200;
                  break;
               case 2:
                  t300TmrVal = 300;
                  break;
               case 3:
                  t300TmrVal = 400;
                  break;
               case 4:
                  t300TmrVal = 600;
                  break;
               case 5:
                  t300TmrVal = 1000;
                  break;
               case 6:
                  t300TmrVal = 1500;
               case 7:
                  t300TmrVal = 2000;
                  break;
               default:
                  {
#ifdef DEBUGP
                     VE_DBG_ERROR((VE_PRNT_BUF,"veUeConStartTmr: Invalid Timer value \n"));
#endif
                  }

            }
            /* Validation of delay */
            if(delay < (2 * t300TmrVal))      
            {

#ifdef DEBUGP
               VE_DBG_ERROR((VE_PRNT_BUF,"veUeConStartTmr: Delay is less than t300 value\n"));
#endif
               /*If user value is less than take it as default value*/
               delay = (U32)(1.5 * t300TmrVal);
            } 
            ueCb   = ( VeUeCb *)cb;
            tmr      = &ueCb->ueConTmr;
            maxTmrs  = 1;
            break;
         }
      case VE_TMR_INITUE_CON:
         {
            ueCb   = ( VeUeCb *)cb;
            tmr      = &ueCb->ueConTmr;
            maxTmrs  = 1;
            break;
         }
default:
#ifdef DEBUGP
         VE_DBG_ERROR((VE_PRNT_BUF,"veUeConStartTmr: Invalid tmrEvnt\n"));
#endif
         RETVOID;

   } /* end of switch */

   wait = (U16)((delay * SS_TICKS_SEC)/1000);

   /* initialize argument for common timer function */
   if(wait != 0)
   {
      arg.tqCp   = &(veCb.tqCp);
      arg.tq     = veCb.tq;
      arg.cb     = cb;
      arg.timers = tmr;
      arg.evnt   = tmrEvnt;
      arg.wait   = wait;
      arg.tNum   = 0;
      arg.max    = maxTmrs;

      cmPlcCbTq(&arg);
   }
   RETVOID;
} /* end of veUeConStartTmr() */




/********************************************************************30**

         End of file:     ve_tmr.c@@/main/2 - Wed Dec  1 08:44:41 2010

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
