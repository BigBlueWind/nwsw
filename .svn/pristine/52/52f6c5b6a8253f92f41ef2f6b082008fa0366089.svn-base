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

     Name:     LTE eNodeB Integrated Stack 
  
     Type:     C Source file
  
     Desc:     PDCP Layer Manager Interface

     File:     ve_sm_pj_ptmi.c

     Sid:      ve_sm_pj_ptmi.c@@/main/2 - Wed Dec  1 08:44:36 2010

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
#include "lkw.h"
#include "lpj.h"
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
#include "lkw.x"
#include "lpj.x"
#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif

/* g++ */
#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */

PRIVATE U8  vePjCfg = 0;
PRIVATE U8  vePjUsapCfgCnt = 0;


#if !(defined(LCPJMILPJ) && defined(PJ))
#define PTSMMILPJ
#endif

/* MAX Number of Service Providers of SM */
#define MAXPJMI 2


#ifdef PTSMMILPJ
EXTERN S16 PtMiLpjCfgReq   ARGS((Pst * pst, PjMngmt * cfg));
EXTERN S16 PtMiLpjCntrlReq ARGS((Pst * pst, PjMngmt * cfg));
#endif /*--#ifdef PTSMMILTF--*/


PRIVATE CONSTANT LpjCfgReq SmMiLpjCfgReqMt[MAXPJMI] =
{
#ifdef LCPJMILPJ
   cmPkLpjCfgReq,
#else
   PtMiLpjCfgReq,
#endif
#ifdef PJ
   PjMiLpjCfgReq,
#else
   PtMiLpjCfgReq,
#endif
};

PRIVATE CONSTANT LpjCntrlReq SmMiLpjCntrlReqMt[MAXPJMI] =
{
#ifdef LCPJMILPJ
   cmPkLpjCntrlReq,
#else
   PtMiLpjCntrlReq,
#endif
#ifdef PJ
   PjMiLpjCntrlReq,
#else
   PtMiLpjCntrlReq,
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
 *      File:  ve_sm_pj_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 SmMiLpjCfgReq
(
Pst * pst,
PjMngmt * cfg
)
#else
PUBLIC S16 SmMiLpjCfgReq(pst, cfg)
Pst * pst;
PjMngmt * cfg;
#endif
{

   TRC3(SmMiLpjCfgReq);

   (*SmMiLpjCfgReqMt[pst->selector])(pst, cfg);

   RETVALUE(ROK);

}

/**************************************************************************
 *
 *      Fun:   Control Request
 *
 *      Desc:  This function is used to configure RRC
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  smpjptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 SmMiLpjCntrlReq
(
Pst * pst,
PjMngmt * cfg
)
#else
PUBLIC S16 SmMiLpjCntrlReq(pst, cfg)
Pst * pst;
PjMngmt * cfg;
#endif
{

   TRC3(SmMiLpjCntrlReq);

   (*SmMiLpjCntrlReqMt[pst->selector])(pst, cfg);

   RETVALUE(ROK);

}


#ifdef PTSMMILPJ
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
 *      File:  ve_sm_pj_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtMiLpjCfgReq
(
Pst * pst,
PjMngmt * cfg
)
#else
PUBLIC S16 PtMiLpjCfgReq(pst, cfg)
Pst * pst;
PjMngmt * cfg;
#endif
{

   TRC3(PtMiLpjCfgReq);

   UNUSED(pst);
   UNUSED(cfg);

   RETVALUE(ROK);

}

/**************************************************************************
 *
 *      Fun:   Portable Control request
 *
 *      Desc:  This function is used to send the Control request RLC
 *             (Portable)
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  ve_sm_pj_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PUBLIC S16 PtMiLpjCntrlReq
(
Pst * pst,
PjMngmt * cfg
)
#else
PUBLIC S16 PtMiLpjCntrlReq(pst, cfg)
Pst * pst;
PjMngmt * cfg;
#endif
{

   TRC3(PtMiLpjCntrlReq);

   UNUSED(pst);
   UNUSED(cfg);

   RETVALUE(ROK);

}

#endif /* PTSMMILTF */

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
*       File:  ve_sm_pj_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLpjCfgCfm
(
Pst            *pst,
PjMngmt        *cfm
)
#else
PUBLIC S16 SmMiLpjCfgCfm(pst, cfm)
Pst            *pst;
PjMngmt        *cfm;
#endif
{
   Pst      pjPst;

   TRC2(SmMiLpjCfgCfm);

   SM_SET_ZERO(&pjPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            vePjCfg |= VE_PJ_GEN_CFG;
            break;      
         case STCPJSAP:
            vePjCfg |= VE_PJ_CPJ_SAP_CFG;
            break;
         case STPJUSAP:
            vePjCfg |= VE_PJ_PJU_SAP_CFG;
            vePjUsapCfgCnt++;
            break;
         default:
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf,"SmMiLpjCfgCfm PDCP Cfg \
                     Cfm received with invalid element \
                     %d\n",cfm->hdr.elmId.elmnt));
#endif
            break;
      } /* end of switch statement */

#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "PDCP Cfg Cfm received\
               for the element %d\n",cfm->hdr.elmId.elmnt));
#endif

      if (vePjCfg == VE_SM_PJ_CONFIGURED && vePjUsapCfgCnt == 2)
      {
         smCb.smState = VE_SM_STATE_PDCP_CFG_DONE;
         VE_FILL_PST(pjPst, SM_PJ_PROC, ENTSM, ENTVE, EVTVERLCCFG, VE_POOL, SM_SELECTOR_LC);

         if(veSmSelfPstMsg(&pjPst) != ROK)
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "PDCP CONFIGURATION \
                     SUCCESSFULLY PERFORMED. Failure at Self Pst.\n"));
#endif
         }
         else
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "PDCP CONFIGURATION \
                     SUCCESSFULLY PERFORMED.\n"));
#endif
         } /* end of else part */

      }  /* end of vePjCfg == VE_SM_PJ_CONFIGURED */
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SmMiLkwCfgCfm: Configuration Cfm\
               with NOK Received.\n"));
#endif
   }

   RETVALUE(ROK);
} /* end of SmMiLkwCfgCfm */


/*
*
*       Fun:   Control Confirm
*
*       Desc:  This function is used by to present Control confirm
*              information to Layer Managemer.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_pj_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLpjCntrlCfm
(
Pst            *pst,
PjMngmt        *cfm
)
#else
PUBLIC S16 SmMiLpjCntrlCfm(pst, cfm)
Pst            *pst;
PjMngmt        *cfm;
#endif
{
   TRC2(SmMiLpjCntrlCfm)

  /*-- Update Event --*/
   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
} /* end of SmMiLpjCntrlCfm */


#ifdef __cplusplus
}
#endif /* cplusplus */

/********************************************************************30**
  
         End of file:     ve_sm_pj_ptmi.c@@/main/2 - Wed Dec  1 08:44:36 2010
  
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
