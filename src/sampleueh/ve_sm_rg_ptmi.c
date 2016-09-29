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

/**********************************************************************

     Name:     LTE-Uu Integrated Stack 
  
     Type:     C Source file
  
     Desc:     MAC Layer Manager Interface

     File:     ve_sm_rg_ptmi.c

     Sid:      ve_sm_rg_ptmi.c@@/main/2 - Wed Dec  1 08:44:37 2010

     Prg:      an

**********************************************************************/

#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */
#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_lte.h"       /* Common UMTS library             */
#include "cm_inet.h"
#include "cm_tpt.h"
#include "ve_sm_init.h"
#include "ve.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_lte.x"       /* Common UMTS library             */
#include "cm_inet.x"
#include "cm_tpt.x"
#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif

PRIVATE U8 veRgCfg = 0;

#if !(defined(LCSMMILRG)  && defined(RG))
#define PTSMMILRG
#endif

/* MAX Number of Service Providers of SM */
#define SM_MAX_LRG_PROV   3


#ifdef PTSMMILRG
EXTERN S16 PtLiLrgCfgReq ARGS((Pst * pst, RgMngmt * cfg));
EXTERN S16 PtLiLrgSchCfgReq ARGS((Pst * pst, RgMngmt * cfg));
EXTERN S16 PtLiLrgCntrlReq ARGS((Pst * pst, RgMngmt * cntrl));
EXTERN S16 PtLiLrgSchCntrlReq ARGS((Pst * pst, RgMngmt * cntrl));
#endif /*--#ifdef PTSMMILRG--*/


PRIVATE CONSTANT LrgCfgReq SmLiLrgCfgReqMt[SM_MAX_LRG_PROV] =
{
#ifdef LCSMMILRG
   cmPkLrgCfgReq,
#else
   PtLiLrgCfgReq,
#endif
#ifdef RG
   RgMiLrgCfgReq,
#else
   PtLiLrgCfgReq,
#endif
};

PRIVATE CONSTANT LrgSchCfgReq SmLiLrgSchCfgReqMt[SM_MAX_LRG_PROV] =
{
#ifdef LCSMMILRG
   cmPkLrgSchCfgReq,
#else
   PtLiLrgSchCfgReq,
#endif
#ifdef RG
   RgMiLrgSchCfgReq,
#else
   PtLiLrgSchCfgReq,
#endif
};

PRIVATE CONSTANT LrgCntrlReq SmLiLrgCntrlReqMt[SM_MAX_LRG_PROV] =
{
#ifdef LCSMMILRG
   cmPkLrgCntrlReq,
#else
   PtLiLrgCntrlReq,
#endif
#ifdef RG
   RgMiLrgCntrlReq,
#else
   PtLiLrgCntrlReq,
#endif
};

PRIVATE CONSTANT LrgSchCntrlReq SmLiLrgSchCntrlReqMt[SM_MAX_LRG_PROV] =
{
#ifdef LCSMMILRG
   cmPkLrgSchCntrlReq,
#else
   PtLiLrgSchCntrlReq,
#endif
#ifdef RG
   RgMiLrgSchCntrlReq,
#else
   PtLiLrgSchCntrlReq,
#endif
};



/**************************************************************************
 *
 *      Fun:   Configuration request
 *
 *      Desc:  This function is used to configure RRC
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  ve_sm_rg_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 SmMiLrgCfgReq
(
Pst * pst,
RgMngmt * cfg
)
#else
PUBLIC S16 SmMiLrgCfgReq(pst, cfg)
Pst * pst;
RgMngmt * cfg;
#endif
{

   /* Corrected function name in trace macro */
   TRC3(SmMiLrgCfgReq);

   (*SmLiLrgCfgReqMt[pst->selector])(pst, cfg);

   RETVALUE(ROK);

}

/**************************************************************************
 *
 *      Fun:   Configuration request
 *
 *      Desc:  This function is used to configure RRC
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  ve_sm_rg_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 SmMiLrgSchCfgReq
(
Pst * pst,
RgMngmt * cfg
)
#else
PUBLIC S16 SmMiLrgSchCfgReq(pst, cfg)
Pst * pst;
RgMngmt * cfg;
#endif
{

   /* Corrected function name in trace macro */
   TRC3(SmMiLrgSchCfgReq);

   (*SmLiLrgSchCfgReqMt[pst->selector])(pst, cfg);

   RETVALUE(ROK);

}


/**************************************************************************
 *
 *      Fun:   Control request
 *
 *      Desc:  This function is used to send the control request RLC
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  ve_sm_rg_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 SmMiLrgCntrlReq
(
Pst * pst,
RgMngmt * cntrl
)
#else
PUBLIC S16 SmMiLrgCntrlReq(pst, cntrl)
Pst * pst;
RgMngmt * cntrl;
#endif
{

   TRC3(SmMiLrgCntrlReq);

   (*SmLiLrgCntrlReqMt[pst->selector])(pst, cntrl);

   RETVALUE(ROK);

}

/**************************************************************************
 *
 *      Fun:   SCH  Control request
 *
 *      Desc:  This function is used to send the control request MAC
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  ve_sm_rg_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 SmMiLrgSchCntrlReq
(
Pst * pst,
RgMngmt * cntrl
)
#else
PUBLIC S16 SmMiLrgSchCntrlReq(pst, cntrl)
Pst * pst;
RgMngmt * cntrl;
#endif
{

   TRC3(SmMiLrgSchCntrlReq);

   (*SmLiLrgSchCntrlReqMt[pst->selector])(pst, cntrl);

   RETVALUE(ROK);

}
#ifdef PTSMMILRG
/**************************************************************************
 *
 *      Fun:   Portable Configuration request
 *
 *      Desc:  This function is used to send the configuration request RLC
 *             (Portable)
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  ve_sm_rg_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtLiLrgCfgReq
(
Pst * pst,
RgMngmt * cfg
)
#else
PUBLIC S16 PtLiLrgCfgReq(pst, cfg)
Pst * pst;
RgMngmt * cfg;
#endif
{

   TRC3(PtLiLrgCfgReq);

   UNUSED(pst);
   UNUSED(cfg);

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PtLiLrgSchCfgReq
(
Pst * pst,
RgMngmt * cfg
)
#else
PUBLIC S16 PtLiLrgSchCfgReq(pst, cfg)
Pst * pst; 
RgMngmt * cfg;
#endif
{

   TRC3(PtLiLrgSchCfgReq);

   UNUSED(pst);
   UNUSED(cfg);

   RETVALUE(ROK);

}

/**************************************************************************
 *
 *      Fun:   Portable Control request
 *
 *      Desc:  This function is used to send the control request RLC
 *             (Portable)
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  ve_sm_rg_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtLiLrgCntrlReq
(
Pst * pst,
RgMngmt * cntrl
)
#else
PUBLIC S16 PtLiLrgCntrlReq(pst, cntrl)
Pst * pst;
RgMngmt * cntrl;
#endif
{

   TRC3(PtLiLrgCntrlReq);

   UNUSED(pst);
   UNUSED(cntrl);

   RETVALUE(ROK);

}

#ifdef ANSI
PUBLIC S16 PtLiLrgSchCntrlReq
(
Pst * pst,
RgMngmt * cntrl
)
#else
PUBLIC S16 PtLiLrgSchCntrlReq(pst, cntrl)
Pst * pst;
RgMngmt * cntrl;
#endif
{

   TRC3(PtLiLrgSchCntrlReq);

   UNUSED(pst);
   UNUSED(cntrl);

   RETVALUE(ROK);
}

#endif /* PTSMMILRG */


/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Managemer.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_rg_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLrgCfgCfm
(
Pst            *pst,
RgMngmt        *cfm
)
#else
PUBLIC S16 SmMiLrgCfgCfm(pst, cfm)
Pst            *pst;
RgMngmt        *cfm;
#endif
{
   Pst      rgPst;

   TRC2(SmMiLrgCfgCfm);

   SM_SET_ZERO(&rgPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            veRgCfg |= VE_RG_GEN_CFG;
            break;      
         case STRGUSAP:
            veRgCfg |= VE_RG_RGU_SAP_CFG;
            break;
         case STCRGSAP:
            veRgCfg |= VE_RG_CRG_SAP_CFG;
            break;
         case STSCHINST:
            veRgCfg |= VE_RG_SCH_INS_CFG;
            break;
         case STTFUSAP:
            veRgCfg |= VE_RG_TFU_SAP_CFG;
            break;
         default:
            break;
      } /* end of switch statement */

#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "MAC Cfg Cfm received\
               for the element %d\n",cfm->hdr.elmId.elmnt));
#endif

      if (veRgCfg == VE_SM_RG_CONFIGURED)
      {
         smCb.smState = VE_SM_STATE_MAC_CFG_DONE;
#ifdef LTE_ENB_PAL          
         VE_FILL_PST(rgPst, SM_RG_PROC, ENTSM, ENTVE, EVTVEENBDEBUGS, VE_POOL, SM_SELECTOR_LC);
#else
         VE_FILL_PST(rgPst, SM_RG_PROC, ENTSM, ENTVE, EVTVECLCFG, VE_POOL, SM_SELECTOR_LC);
#endif
         if(veSmSelfPstMsg(&rgPst) != ROK)
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "MAC CONFIGURATION \
                     SUCCESSFULLY PERFORMED. Failure at Self Pst.\n"));
#endif
         }
         else
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "MAC CONFIGURATION \
                     SUCCESSFULLY PERFORMED.\n"));
#endif
         } /* end of else part */

      }  /* end of veRgCfg == VE_SM_RG_CONFIGURED */
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SmMiLkwCfgCfm Configuration not OK.\n"));
#endif
   }

   RETVALUE(ROK);
} /* end of SmMiLkwCfgCfm */

/*
*
*       Fun:   Sch Config Confirm
*
*       Desc:  This function is used to handle config cfm from sch 
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_rg_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLrgSchCfgCfm
(
Pst            *pst,
RgMngmt        *cfm
)
#else
PUBLIC S16 SmMiLrgSchCfgCfm(pst, cfm)
Pst            *pst;
RgMngmt        *cfm;
#endif
{

   TRC2(SmMiLrgSchCfgCfm);

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STSCHINST:
            veRgCfg |= VE_RG_SCH_INS_CFG;
            break;
         default:
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Received Cfg Cfm\
                     with unknown element id %d\n", cfm->hdr.elmId.elmnt));
#endif
            break;
      } /* end of switch statement */
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SCH Cfg Cfm received\
               for the element %d\n",cfm->hdr.elmId.elmnt));
#endif

   }
   RETVALUE(ROK);
}
/*
*
*       Fun:   Control Confirm
*
*       Desc:  This function is used by to present control confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_rg_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLrgCntrlCfm
(
Pst            *pst,
RgMngmt        *cntrl
)
#else
PUBLIC S16 SmMiLrgCntrlCfm(pst, cntrl)
Pst            *pst;
RgMngmt        *cntrl;
#endif
{
   Pst  rgPst;

   TRC2(SmMiLrgCntrlCfm);

   SM_SET_ZERO(&rgPst, sizeof(Pst));

   if (cntrl->hdr.elmId.elmnt == STTFUSAP)
   {
      if (cntrl->cfm.status == LCM_PRIM_OK)
      {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF MAC WITH CL\
                     SUCCESSFUL \n"));
#endif
            /* Initiate control request for binding Sch with TFU sap*/
            smBindRgSchToTfuSap();
      } /* end of if statement */
      else if (cntrl->cfm.status == LCM_PRIM_OK_NDONE)
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF MAC WITH CL \
                  IS IN PROGRESS \n"));
#endif
      }
      else 
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF MAC WITH CL\
                  FAILED \n"));
#endif
      }
   } /* end of if statement */
   else
   {
      if (cntrl->cfm.status == LCM_PRIM_OK)
      {
         if((smCfgCb.rgDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_RG_DBG_MASK))
         {
            /* Update DbgMsk */
            smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | VE_SM_RG_DBG_MASK);
            /* Trigger Debug Cntrl */
            veSmHdlEnbDebugs(EVTVEENBDEBUGS);
         }
      }
   }

   RETVALUE(ROK);
} /* end of SmMiLrgCntrlCfm */

/*
*
*       Fun:   SCH Control Confirm
*
*       Desc:  This function is used by to present control confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_rg_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLrgSchCntrlCfm
(
Pst            *pst,
RgMngmt        *cntrl
)
#else
PUBLIC S16 SmMiLrgSchCntrlCfm(pst, cntrl)
Pst            *pst;
RgMngmt        *cntrl;
#endif
{
   Pst  rgPst;

   TRC2(SmMiLrgSchCntrlCfm);

   SM_SET_ZERO(&rgPst, sizeof(Pst));

   if (cntrl->hdr.elmId.elmnt == STTFUSAP)
   {
      if (cntrl->cfm.status == LCM_PRIM_OK)
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF SCH WITH CL \
                  SUCCESSFUL \n"));
#endif
         smBindKwToRguSap();
      } /* end of if statement */
      else if (cntrl->cfm.status == LCM_PRIM_OK_NDONE)
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF SCH WITH CL \
                  IS IN PROGRESS \n"));
#endif
      }
      else 
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF SCH WITH CL\
                  FAILED \n"));
#endif
      }
   } /* end of if statement */

   RETVALUE(ROK);
} /* end of SmMiLrgSchCntrlCfm */



/*
*
*       Fun:   Status Ind
*
*       Desc:  This function is used by to present Status Ind
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_rg_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLrgStaInd
(
Pst            *pst,
RgMngmt        *staInd
)
#else
PUBLIC S16 SmMiLrgStaInd(pst, staInd)
Pst            *pst;
RgMngmt        *staInd;
#endif
{
   TRC2(SmMiLrgStaInd)

   UNUSED(pst);
   UNUSED(staInd);

   RETVALUE(ROK);
} /* end of SmMiLrgStaInd */


/*
*
*       Fun:   Trc Ind
*
*       Desc:  This function is used by to present Trc Ind
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_rg_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLrgTrcInd 
(
Pst            *pst,
RgMngmt        *trcInd,
Buffer         *mBuf
)
#else
PUBLIC S16 SmMiLrgTrcInd(pst, trcInd, mBuf)
Pst            *pst;
RgMngmt        *trcInd;
Buffer         *mBuf;
#endif
{
   TRC2(SmMiLrgTrcInd)

   UNUSED(pst);
   UNUSED(trcInd);
   UNUSED(mBuf);

   RETVALUE(ROK);
} /* end of SmMiLrgTrcInd */


/*
*
*       Fun:   Sta Cfm
*
*       Desc:  This function is used by to present Sta Cfm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_rg_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLrgStaCfm
(
Pst            *pst,
RgMngmt        *cfm
)
#else
PUBLIC S16 SmMiLrgStaCfm(pst, cfm)
Pst            *pst;
RgMngmt        *cfm;
#endif
{
   TRC2(SmMiLrgStaCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
} /* end of SmMiLrgStaCfm */

/*
*
*       Fun:   Sta Cfm
*
*       Desc:  This function is used by to present Sta Cfm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_rg_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLrgStsCfm
(
Pst            *pst,
RgMngmt        *cfm
)
#else
PUBLIC S16 SmMiLrgStsCfm(pst, cfm)
Pst            *pst;
RgMngmt        *cfm;
#endif
{
   TRC2(SmMiLrgStsCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
} /* end of SmMiLrgStsCfm */

/********************************************************************30**
  
         End of file:     ve_sm_rg_ptmi.c@@/main/2 - Wed Dec  1 08:44:37 2010
  
*********************************************************************31*/

/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
