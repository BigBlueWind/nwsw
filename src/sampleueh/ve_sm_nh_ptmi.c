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
  
     Desc:     RRC Layer Manager Interface

     File:     ve_sm_nh_ptmi.c

     Sid:      ve_sm_nh_ptmi.c@@/main/2 - Wed Dec  1 08:44:35 2010

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
#include <unistd.h>
#include "lnh.h"

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
#include "lnh.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif

PRIVATE U16 veNhCfg = 0;
PRIVATE U16 numBinds = 0;
PRIVATE Bool bndInPrg = 1;


#define MAXNHMI  2         /* The size of the LC/TC matrix */

#ifndef LCSMNHMILNH
#define PTSMMILNH   1      /* Portable LNH Interface */
#else
#ifndef SM
#define PTSMMILNH   1      /* Portable LNH Interface */
#endif /* SM */
#endif /* LCSMNHMILNH */

#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */


#ifdef PTSMMILNH
/* forward references */
PRIVATE S16 PtMiLnhCfgReq   ARGS((Pst *pst, NhMngmt *cfg ));
PRIVATE S16 PtMiLnhCntrlReq ARGS((Pst *pst, NhMngmt *cntrl ));
#endif /* PTSMMILNH */


PRIVATE LnhCfgReq SmMiLnhCfgReqMt[MAXNHMI] =
{
#ifdef LCSMNHMILNH
   cmPkLnhCfgReq,          /* 0 - loosely coupled */
#else
   PtMiLnhCfgReq,          /* 0 - tightly coupled, portable */
#endif /* LCSMNHMILNH */
#ifdef NH
   NhMiLnhCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLnhCfgReq,          /* 1 - tightly coupled, portable */
#endif /* NH */
};

PRIVATE LnhCntrlReq SmMiLnhCntrlReqMt[MAXNHMI] =
{
#ifdef LCSMNHMILNH
   cmPkLnhCntrlReq,          /* 0 - loosely coupled */
#else
   PtMiLnhCntrlReq,          /* 0 - tightly coupled, portable */
#endif /* LCSMNHMILNH */
#ifdef NH
   NhMiLnhCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLnhCntrlReq,          /* 1 - tightly coupled, portable */
#endif /* NH */
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
 *      File:  ve_sm_nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PUBLIC S16 SmMiLnhCfgReq
(
Pst *pst,                 /* post structure */
NhMngmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLnhCfgReq(pst, cfg)
Pst *pst;                 /* post structure */   
NhMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLnhCfgReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLnhCfgReqMt[pst->selector])(pst, cfg); 
   RETVALUE(ROK);
} /* end of SmMiLnhCfgReq */


/**************************************************************************
 *
 *      Fun:   Control request
 *
 *      Desc:  This function is used to send the control request RRC
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  ve_sm_nh_ptmi.c
 *
 *************************************************************************/
 
 #ifdef ANSI
PUBLIC S16 SmMiLnhCntrlReq
(
Pst *pst,                 /* post structure */
NhMngmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLnhCntrlReq(pst, cntrl)
Pst *pst;                 /* post structure */   
NhMngmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLnhCntrlReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLnhCntrlReqMt[pst->selector])(pst, cntrl); 
   RETVALUE(ROK);
} /* end of SmMiLnhCntrlReq */

#ifdef PTSMMILNH
/**************************************************************************
 *
 *      Fun:   Portable Configuration request
 *
 *      Desc:  This function is used to send the configuration request RRC
 *             (Portable)
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  ve_sm_nh_ptmi.c
 *
 *************************************************************************/

#ifdef ANSI
PRIVATE S16 PtMiLnhCfgReq
(
Pst *pst,                   /* post structure */
NhMngmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLnhCfgReq(pst, cfg)
Pst *pst;                   /* post structure */
NhMngmt *cfg;               /* configure */
#endif
{
  TRC3(PtMiLnhCfgReq);

  UNUSED(cfg);
  UNUSED(pst);
  RETVALUE(ROK);
} /* end of PtMiLnhCfgReq */


/**************************************************************************
 *
 *      Fun:   Portable Control request
 *
 *      Desc:  This function is used to send the control request RRC
 *             (Portable)
 *
 *      Ret:   ROK      - ok
 *
 *      Notes: None
 *
 *      File:  ve_sm_nh_ptmi.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE S16 PtMiLnhCntrlReq
(
Pst *pst,                   /* post structure */
NhMngmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLnhCntrlReq(pst, cntrl)
Pst *pst;                   /* post structure */
NhMngmt *cntrl;             /* control */
#endif
{
  TRC3(PtMiLnhCntrlReq);

  UNUSED(cntrl);
  UNUSED(pst);

  RETVALUE(ROK);
} /* end of PtMiLnhCntrlReq */

#endif /* PTSMMILNH */


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
*       File:  ve_sm_nh_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLnhCfgCfm
(
Pst     *pst,          /* post structure */
NhMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLnhCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
NhMngmt *cfm;          /* configuration */
#endif
{
  Pst      nhPst;

   TRC2(SmMiLnhCfgCfm);

   SM_SET_ZERO(&nhPst,  sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
       switch (cfm->hdr.elmId.elmnt)
       {
          case STNHGEN:
               veNhCfg |= VE_NH_GEN_CFG;
               break;      
          case STNHPROT:
               veNhCfg |= VE_NH_PROT_CFG;
               break;
          case STNHUSAP:
               veNhCfg |= VE_NH_USAP_CFG;
               break;
          case STNHCKWSAP:
               veNhCfg |= VE_NH_CKW_SAP_CFG;
               break;
          case STNHKWUSAP:
               veNhCfg |= VE_NH_KWU_SAP_CFG;
               break;
          case STNHCRGSAP:
               veNhCfg |= VE_NH_CRG_SAP_CFG;
               break;
          case STNHCPJSAP:
               veNhCfg |= VE_NH_CPJ_SAP_CFG;
               break;
          case STNHPJUSAP:
               veNhCfg |= VE_NH_PJU_SAP_CFG;
               break;
          default:
#ifdef DEBUGP
       VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Cfg Cfm received \
with invalid element \n"));
#endif
               break;
       } /* end of switch statement */
#ifdef DEBUGP
       VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Cfg Cfm received \
for the element %d\n",cfm->hdr.elmId.elmnt));
#endif

       if (veNhCfg == VE_SM_NH_CONFIGURED)
       {
          smCb.smState = VE_SM_STATE_RRC_CFG_DONE;
          VE_FILL_PST(nhPst, SM_NH_PROC, ENTSM, ENTVE, EVTVEPDCPCFG, NH_POOL, SM_SELECTOR_LC);

          if(veSmSelfPstMsg(&nhPst) != ROK)
          {
#ifdef DEBUGP
             VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC CONFIGURATION \
SUCCESSFULLY PERFORMED. Failure at Self Pst.\n"));
#endif
          }
          else
          {
#ifdef DEBUGP
             VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC CONFIGURATION \
SUCCESSFULLY PERFORMED. \n"));
#endif
          } /* end of else part */

       }  /* end of veNhCfg == VE_SM_NH_CONFIGURED */
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Configuration Confirm \
not OK.\n"));
#endif
   } /* end of else part */

   RETVALUE(ROK);
} /* end of nhAcSmMiLnhCfgCfm */

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
*       File:  ve_sm_nh_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLnhCntrlCfm
(
Pst     *pst,          /* post structure */
NhMngmt *cntrl         /* control */
)
#else
PUBLIC S16 SmMiLnhCntrlCfm(pst, cntrl)
Pst     *pst;          /* post structure */
NhMngmt *cntrl;        /* control */
#endif
{
   Pst vePst;

   /* Added trace macro */
   TRC2(SmMiLnhCntrlCfm)

   SM_SET_ZERO(&vePst, sizeof(Pst));

   if(cntrl->cfm.status == LCM_PRIM_OK)
   {
      switch (cntrl->hdr.elmId.elmnt)
      {
         case STNHGEN:
            {
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Enabling RRC Alarms is \
                        successfully performed. \n"));
#endif
               if((smCfgCb.nhDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_NH_DBG_MASK))
               {
                  /* Update DbgMsk */
                  smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | VE_SM_NH_DBG_MASK);
                  /* Trigger Debug Cntrl */
                  veSmHdlEnbDebugs(EVTVEENBDEBUGS);
               }
            }
            break;
         case STNHCRGSAP:
            {
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRC CRG SAP \
                        IS IN PROGRESS \n"));
#endif
            }
            break;
         case STNHCPJSAP:
            {
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRC CPJ SAP \
                        IS IN PROGRESS \n"));
#endif

            }
            break;
         case STNHPJUSAP:
            {
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRC PJU SAP \
                        IS IN PROGRESS \n"));
#endif

            }
            break;
            /* Fix for CRId- 115717 */
            /* Handling cntrlCfm for RRC CKW SAP */
         case STNHCKWSAP:
            {
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRC CKW SAP \
                        IS IN PROGRESS \n"));
#endif

            }
            break;

         default:
            {
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Unknown CntrlCfm %d\n", cntrl->hdr.elmId.elmnt));
#endif
            }
            break;
      }
   } /* end of if cntrl->cfm.status == LCM_PRIM_OK */

   RETVALUE(ROK);
} /* end of nhAcSmMiLnhCntrlCfm */

/*
*
*       Fun:   Status Confirm
*
*       Desc:  This function is used by to present status confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_nh_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLnhStaCfm
(
Pst     *pst,          /* post structure */
NhMngmt *cfm           /* control */
)
#else
PUBLIC S16 SmMiLnhStaCfm(pst, cfm)
Pst     *pst;          /* post structure */
NhMngmt *cfm;          /* control */
#endif
{
   TRC2(SmMiLnhStaCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
} /* end of SmMiLnhStaCfm */

/*
*
*       Fun:   Sts Confirm
*
*       Desc:  This function is used by to present sts confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_nh_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLnhStsCfm
(
Pst       *pst,         /* post structure */
NhMngmt   *sts          /* confirmed statistics */
)
#else
PUBLIC S16 SmMiLnhStsCfm(pst, sts)
Pst       *pst;         /* post structure */
NhMngmt   *sts;         /* confirmed statistics */
#endif
{
   TRC2(SmMiLnhStsCfm)

   UNUSED(pst);
   UNUSED(sts);

   RETVALUE(ROK);
} /* end of SmMiLnhStsCfm */

/*
*
*       Fun:   Status Indication
*
*       Desc:  This function is used by to present status Indication
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_nh_ptmi.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLnhStaInd
(
Pst     *pst,           /* post structure */
NhMngmt *usta           /* unsolicited status */
)
#else
PUBLIC S16 SmMiLnhStaInd(pst, usta)
Pst     *pst;           /* post structure */
NhMngmt *usta;          /* unsolicited status */
#endif
{
   NhUstaDgn *dgn;

   TRC2(SmMiLnhStaInd)

   dgn = &usta->u.usta.dgn;

   switch(usta->u.usta.alarm.category)
   {
      case LCM_CATEGORY_INTERFACE:
         {
            switch(usta->u.usta.alarm.event)
            {
               case LCM_EVENT_BND_OK:
                  {
                     if (bndInPrg)
                     {
                        numBinds ++;
                     }
                     else
                     {
#ifdef DEBUGP
                           VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRC (NHU) \
Usap id[%d] IS SUCCESSFUL\n", dgn->u.sapInfo.sapId));
#endif
                           break;
                     }
                     switch (numBinds)
                     {
                        case 1:
                        {
#ifdef DEBUGP
                           VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRC CKW \
lsap id[%d] IS SUCCESSFUL\n", dgn->u.sapInfo.sapId));
#endif
                           
                           smBindNhToCpjSap(); 
                          break;
                        }
                        case 2:
                        {
#ifdef DEBUGP
                           VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRC CPJ \
lsap id[%d] IS SUCCESSFUL\n", dgn->u.sapInfo.sapId));
#endif
                           
                           smBindNhToPjuSap(); 
                          break;
                        }
                        case 3:
                        {
#ifdef DEBUGP
                           VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRC PJU \
lsap id[%d] IS SUCCESSFUL\n", dgn->u.sapInfo.sapId));
#endif
                           
                           smBindNhToCrgSap(); 
                          break;
                        }
                        case 4:
                        {
#ifdef DEBUGP
                           VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRC CRG \
lsap id[%d] IS SUCCESSFUL\n", dgn->u.sapInfo.sapId));
#endif
                           /* Initiate Bind request for KWU Sap btw NH and KW */
                           smBindNhToKwuSap(); 
                           
                          break;
                        }
                        case 5:
                        {
                           
#ifdef DEBUGP
                           VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRC KWU \
lsap id[%d] IS SUCCESSFUL\n", dgn->u.sapInfo.sapId));
#endif
                           bndInPrg = FALSE;
                          smBindVeToCtfSap();  
#ifdef CNE_UE_SIM_TEST
                          smCb.smState = VE_SM_STATE_RRC_STK_BND_DONE;
                          smVeProcSm(EVTVECELLCFG);


#endif
                          break;
                        }
                        default:
#ifdef DEBUGP
                           VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Multiple Alarms \
                            received %d\n", dgn->u.sapInfo.sapId));
#endif
                          break;
                     }
                  }
                  break;
               case LCM_EVENT_BND_FAIL:
                  {
#ifdef DEBUGP
                     VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRC \
                        lsap id[%d] FAILED\n", dgn->u.sapInfo.sapId));
#endif
                  }
                  break;
               default:
                  {
                  }
            }
         }
         break;
      default:
         {
         }
   }

   RETVALUE(ROK);
} /* end of SmMiLnhStaInd */


#ifdef __cplusplus
}
#endif /* cplusplus */

/********************************************************************30**
  
         End of file:     ve_sm_nh_ptmi.c@@/main/2 - Wed Dec  1 08:44:35 2010
  
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
