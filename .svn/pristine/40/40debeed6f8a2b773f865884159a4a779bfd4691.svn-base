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
  
     Name:     Stack Manager - portable LTE Convergence Layer
  
     Type:     C source file
  
     Desc:     Code for LTE Convergence Layer  layer management
               primitives. 
              
     File:     ve_sm_ve_ptmi.c
  
     Sid:      ve_sm_ve_ptmi.c@@/main/2 - Wed Dec  1 08:44:40 2010

     Prg:      an
  
*********************************************************************21*/

/*
  
Layer management provides the necessary functions to control and
monitor the condition of each protocol layer.

The following functions are provided in this file:

     SmMiLveCfgReq      Configure Request
     SmMiLveCntrlReq    Control Request
   
It is assumed that the following functions are provided in the
stack management body files:

     SmMiLveStaInd      Status Indication
     SmMiLveStaCfm      Status Confirm
     SmMiLveStsCfm      Statistics Confirm
     SmMiLveTrcInd      Trace Indication
   
*/   


/* header include files (.h) */
  
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "cm5.h"           /* common timer */
#include "ssi.h"           /* svetem services */
#include "cm_inet.h"       /* common sockets */
#include "cm_tpt.h"
#include "lve.h"           /* VE layer management */
#include "cm_err.h"        /* common error */
#include "ve_sm_init.h"
#include "ve.h"

/* header/extern include files (.x) */
  
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* common timer */
#include "cm_inet.x"       /* common sockets */
#include "cm_tpt.x"
#include "cm_lib.x"        /* common library */
#include "lve.x"           /* ve layer management */
#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif

PRIVATE U16 veVeCfg = 0;
PRIVATE U16 veCellCntr  = 0;


#define  MAXVEMI  2

#ifndef  LCSMVEMILVE
#define  PTSMVEMILVE
#else
#ifndef   VE
#define  PTSMVEMILVE
#endif
#endif

#ifdef PTSMVEMILVE
PRIVATE S16 PtMiLveCfgReq    ARGS((Pst *pst, VeMngmt *cfg));
PRIVATE S16 PtMiLveCntrlReq  ARGS((Pst *pst, VeMngmt *cntrl));
#endif


/*
the following matrices define the mapping between the primitives
called by the layer management interface of TCP UDP Convergence Layer
and the corresponding primitives in TUCL
 
The parameter MAXVEMI defines the maximum number of layer manager
entities on top of TUCL . There is an array of functions per primitive
invoked by TCP UDP Conbvergence Layer. Every array is MAXVEMI long
(i.e. there are as many functions as the number of service users).

The dispatcysng is performed by the configurable variable: selector.
The selector is configured during general configuration.

The selectors are:

   0 - loosely coupled (#define LCSMVEMILVE) 2 - Lve (#define VE)

*/


/* Configuration request primitive */
 
PRIVATE LveCfgReq SmMiLveCfgReqMt[MAXVEMI] =
{
#ifdef LCSMVEMILVE
   cmPkLveCfgReq,          /* 0 - loosely coupled  */
#else
   PtMiLveCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef VE
   VeMiLveCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLveCfgReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Control request primitive */
 
PRIVATE LveCntrlReq SmMiLveCntrlReqMt[MAXVEMI] =
{
#ifdef LCSMVEMILVE
   cmPkLveCntrlReq,          /* 0 - loosely coupled  */
#else
   PtMiLveCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef VE
   VeMiLveCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLveCntrlReq,          /* 1 - tightly coupled, portable */
#endif
};


/*
*     layer management interface functions 
*/
 
/*
*
*       Fun:   Configuration request
*
*       Desc:  Tves function is used to configure  TUCL
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_ve_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLveCfgReq
(
Pst *spst,                /* post structure */
VeMngmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLveCfgReq(spst, cfg)
Pst *spst;                /* post structure */   
VeMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLveCfgReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLveCfgReqMt[spst->selector])(spst, cfg); 
   RETVALUE(ROK);
} /* end of SmMiLveCfgReq */



/*
*
*       Fun:   Control request
*
*       Desc:  This function is used to send control request to 
*              TUCL
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_ve_ptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLveCntrlReq
(
Pst *spst,                 /* post structure */
VeMngmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLveCntrlReq(spst, cntrl)
Pst *spst;                 /* post structure */   
VeMngmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLveCntrlReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLveCntrlReqMt[spst->selector])(spst, cntrl); 
   RETVALUE(ROK);
} /* end of SmMiLveCntrlReq */

#ifdef PTSMVEMILVE

/*
 *             Portable Functions
 */

/*
*
*       Fun:   Portable configure Request- TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_ve_ptmi.c
*
*/
  
#ifdef ANSI
PRIVATE S16 PtMiLveCfgReq
(
Pst *spst,                  /* post structure */
VeMngmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLveCfgReq(spst, cfg)
Pst *spst;                  /* post structure */
VeMngmt *cfg;               /* configure */
#endif
{
  TRC3(PtMiLveCfgReq) 

  UNUSED(spst);
  UNUSED(cfg);

  RETVALUE(ROK);
} /* end of PtMiLveCfgReq */



/*
*
*       Fun:   Portable control Request TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ve_sm_ve_ptmi.c
*
*/
#ifdef ANSI
PRIVATE S16 PtMiLveCntrlReq
(
Pst *spst,                  /* post structure */
VeMngmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLveCntrlReq(spst, cntrl)
Pst *spst;                  /* post structure */
VeMngmt *cntrl;             /* control */
#endif
{
  TRC3(PtMiLveCntrlReq)

  UNUSED(spst);
  UNUSED(cntrl);

 RETVALUE(ROK);
} /* end of PtMiLveCntrlReq */

#endif /* PTSMVEMILHI */

/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_ve_ptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLveCfgCfm
(
Pst     *pst,          /* post structure */
VeMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLveCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
VeMngmt *cfm;          /* configuration */
#endif
{
   Pst      vePst;

   TRC2(SmMiLveCfgCfm);

   SM_SET_ZERO(&vePst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STVEGEN:
            veVeCfg |= VE_VE_GEN_CFG;
            break;      
#ifdef VE_RELAY
         case STVECTFSAP:
            veVeCfg |= VE_VE_CTF_SAP_CFG;
            break;
         case STVENHUSAP:
            veVeCfg |= VE_VE_NHU_SAP_CFG;
            break;
         case STVERGRSAP:
            veVeCfg |= VE_VE_RGR_SAP_CFG;
            break;
         case STVESZTSAP:
            veVeCfg |= VE_VE_SZT_SAP_CFG;
            break;
#ifdef LTE_HO_SUPPORT
         case STVECZTSAP:
            veVeCfg |= VE_VE_CZT_SAP_CFG;
            break;
         case STVENGH:
            veVeCfg |= VE_VE_NGH_CFG;
            break;
#endif            
#endif /* VE_RELAY */
#ifdef EU_DAT_APP
         case STVEPJUSAP:
            veVeCfg |= VE_VE_PJU_SAP_CFG;
            break;
         case STVEEGTSAP:
            veVeCfg |= VE_VE_EGT_SAP_CFG;
            break;
#endif /* EU_DAT_APP  */
         default:
            break;
      } /* end of switch statement */
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "APP Cfg Cfm received\
               for the element %d\n",cfm->hdr.elmId.elmnt));
#endif

      if (veVeCfg == VE_SM_VE_CONFIGURED)
      {
         smCb.smState = VE_SM_STATE_APP_CFG_DONE;
         VE_FILL_PST(vePst, SM_VE_PROC, ENTSM, ENTVE, EVTVERRCCFG, VE_POOL, SM_SELECTOR_LC);

         if(veSmSelfPstMsg(&vePst) != ROK)
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "APP CONFIGURATION \
                     SUCCESSFULLY PERFORMED. Failure at Self Pst.\n"));
#endif
         }
         else
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "APP CONFIGURATION \
                     SUCCESSFULLY PERFORMED. \n"));
#endif
         } /* end of else part */

      }  /* end of veVeCfg == VE_SM_VE_CONFIGURED */
   } /* end of if statement cfm->cfm.status == LCM_PRIM_OK */
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Configuration Confirm\
               received with Not OK.\n"));
#endif
   }

   RETVALUE(ROK);
} /* end of SmMiLveCfgCfm */

/*
 *
 *       Fun:    SmMiLveCntrlCfm - control confirm
 *
 *       Desc:   prints the control confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_ve_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLveCntrlCfm
(
Pst         *pst,
VeMngmt     *cntrl
)
#else
PUBLIC S16 SmMiLveCntrlCfm(pst, cntrl)
Pst         *pst;
VeMngmt     *cntrl;
#endif /* ANSI */
{
   Pst  vePst;

   /* Added trace macro */
   TRC2(SmMiLveCntrlCfm)

   SM_SET_ZERO(&vePst, sizeof(Pst));

#ifdef VE_RELAY
   if(cntrl->hdr.elmId.elmnt == STVECTFSAP)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF APP with CL(CTF)\
               IS IN PROGRESS.\n"));
#endif
   } /* end of if statement cntrl->hdr.elmId.elmnt == STVECTFSAP */
   else if(cntrl->hdr.elmId.elmnt == STVESZTSAP)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Bind of RRM Tsap with S1AP \
               is in progress.\n"));
#endif
   }
#ifdef LTE_HO_SUPPORT
   else if(cntrl->hdr.elmId.elmnt == STVECZTSAP)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Bind of RRM Tsap with X2AP \
               is in progress.\n"));
#endif
   }
#endif
   else if(cntrl->hdr.elmId.elmnt == STVENHUSAP)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Bind of RRM with RRC (NHU)\
               is in progress.\n"));
#endif
   } /* end of if statement cntrl->hdr.elmId.elmnt == STVENHUSAP */
   else if(cntrl->hdr.elmId.elmnt == STVERGRSAP)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Bind of RRM TSap with MAC\
               is in progress.\n"));
#endif
   } /* end of if statement cntrl->hdr.elmId.elmnt == STVERGRSAP */
   else if (cntrl->hdr.elmId.elmnt == STVEGEN)
   {

#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Enabling Alarms of App is \
               done.\n"));
#endif
      if(cntrl->cfm.status == LCM_PRIM_OK)
      {
         if((smCfgCb.veDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_VE_DBG_MASK))
         {
            /* Update DbgMsk */
            smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | VE_SM_VE_DBG_MASK);
            /* Trigger Debug Cntrl */
            veSmHdlEnbDebugs(EVTVEENBDEBUGS);
         }
      }
      else
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "\n SmMiLveCntrlCfm failed!\n"));
#endif

      }

   } /* end of else */
   else if( cntrl->hdr.elmId.elmnt == STVES1CONSETUP)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Received Control Confirm for S1 Setup.\n"));
#endif
         /* smBuildVeCellCntrl();  */

   } /* end of else part */
#ifdef LTE_HO_SUPPORT
   else if( cntrl->hdr.elmId.elmnt == STVEX2CONSETUP)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Received Control Confirm for X2 Setup.\n"));
#endif

   } /* end of else part */
#endif
   else if( cntrl->hdr.elmId.elmnt == STVECELLCFG)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Received Control Confirm for CELL CFG .\n"));
#endif

   } /* end of else part */
#endif  /* VE_RELAY */
#ifdef EU_DAT_APP
   else if(cntrl->hdr.elmId.elmnt == STVEPJUSAP)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Bind of RRM with RLC (PJU) \
               is in progress.\n"));
#endif
   } /* end of if statement cntrl->hdr.elmId.elmnt == STVEEGTSAP */
   else if(cntrl->hdr.elmId.elmnt == STVEEGTSAP)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Bind of RRM to EGTP \
               is in progress\n"));
#endif
   } /* end of if statement cntrl->hdr.elmId.elmnt == STVEPJUSAP */
#endif /* end of EU_DAT_APP */

   RETVALUE(ROK);
} /* end of SmMiLveCntrlCfm() */

/*
 *
 *       Fun:    SmMiLveStaInd - status indication
 *
 *       Desc:   Received the Status Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ve_sm_ve_ptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLveStaInd
(
Pst         *pst,
VeMngmt      *usta
)
#else
PUBLIC S16 SmMiLveStaInd(pst, usta)
Pst         *pst;
VeMngmt      *usta;
#endif /* ANSI */
{
   Pst  vePst;
   VeUstaDgn *dgn;

   /* Added trace macro */
   TRC2(SmMiLveStaInd)

   SM_SET_ZERO(&vePst, sizeof(Pst));

   dgn = &(usta->u.usta.dgn);
    if ( usta->u.usta.alarm.event == LVE_EVENT_S1CON_SET_SUCCESS)
   {
        if(veCellCntr == 0)
        {
           smBuildVeCellCntrl();
           veCellCntr++;
        }
   } 
   if ((usta->u.usta.alarm.event == LVE_EVENT_CELL_CFG_SUCCESS)
      && smCb.smState == VE_SM_STATE_AWAIT_CELL_UP)
   {
      smCb.smState = VE_SM_STATE_CELL_UP;
      printf("CELL CONFIGURATION successful \n");
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "CELL CONFIGURATION \
SUCCESSFULLY PERFORMED. \n"));
#endif
#ifdef CNE_UE_SIM_TEST
      /* eNodeB_Ue_Sim: Send the S1 Setup Now. */
      VE_FILL_PST(vePst, SM_VE_PROC, ENTSM, ENTVE, EVTVEINITS1SETUP, VE_POOL, SM_SELECTOR_LC);
      if(veSmSelfPstMsg(&vePst) != ROK)
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Failure at Self Pst. %d\n", smCb.smState));
#endif
      }
#endif
#ifdef LTE_HO_SUPPORT
      /* eNodeB_Ue_Sim: Send the x2 Setup Now. */
      VE_FILL_PST(vePst, SM_VE_PROC, ENTSM, ENTVE, EVTVEINITX2SETUP, VE_POOL, SM_SELECTOR_LC);
      if(veSmSelfPstMsg(&vePst) != ROK)
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Failure at Self Pst. %d\n", smCb.smState));
#endif
      }

#endif

   } /* end of if statement */
   else if (usta->u.usta.alarm.category == LCM_CATEGORY_INTERFACE)
   {
      if (usta->u.usta.alarm.event == LCM_EVENT_BND_OK)
      {
         switch (usta->u.usta.alarm.cause)
         {
            case LVE_CAUSE_SZT_SAP_BOUND:
            {
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRM S1AP IS \
SUCCESSFUL sapId = %d\n", dgn->u.sapId));
#endif
               if (smCb.smState == VE_SM_STATE_ENB_ALRMS_DONE)
               {
                  smCb.smState = VE_SM_STATE_S1AP_STK_BND_DONE;
#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF S1AP STACK SUCCESSFUL\n "));
#endif
                  /* Perform the bind of APP and SCTP */
                  VE_FILL_PST(vePst, SM_VE_PROC, ENTSM, ENTVE, EVTVEEGTPSTKBND, VE_POOL, SM_SELECTOR_LC);
                 if(veSmSelfPstMsg(&vePst) != ROK)
                 {
#ifdef DEBUGP
                    VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Failure at Self \
                         Pst. %d\n", smCb.smState));
#endif
                  }
               }
               break;
            }
            case LVE_CAUSE_EGT_SAP_BOUND:
            {
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRM TO EGTP IS \
SUCCESSFUL SAPID= %d\n", dgn->u.sapId));
#endif
               if (smCb.smState == VE_SM_STATE_S1AP_STK_BND_DONE)
               {
                  smCb.smState = VE_SM_STATE_EGTP_STK_BND_DONE;
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF EGTP STACK \
SUCCESSFUL \n"));
#endif
#ifdef LTE_HO_SUPPORT
                  /* Perform the bind of APP and SCTP */
                  VE_FILL_PST(vePst, SM_VE_PROC,ENTSM, ENTVE, EVTVEX2APSTKBND, VE_POOL, SM_SELECTOR_LC);
#else
                  /* Perform the bind of APP and SCTP */
                  VE_FILL_PST(vePst, SM_VE_PROC,ENTSM, ENTVE, EVTVERRCSTKBND, VE_POOL, SM_SELECTOR_LC);
#endif
                 if(veSmSelfPstMsg(&vePst) != ROK)
                 {
#ifdef DEBUGP
                    VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Failure at \
                          Self Pst. %d\n", smCb.smState));
#endif
                  }
               }
               break;
            }

#ifdef LTE_HO_SUPPORT
            case LVE_CAUSE_CZT_SAP_BOUND:
            {
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRM TO X2AP IS \
SUCCESSFUL SAPID= %d\n", dgn->u.sapId));
#endif
               if (smCb.smState == VE_SM_STATE_EGTP_STK_BND_DONE)
               {
                  smCb.smState = VE_SM_STATE_X2AP_STK_BND_DONE;
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF X2AP STACK \
SUCCESSFUL \n"));
#endif
                  /* Perform the bind of APP and SCTP */
                 VE_FILL_PST(vePst, SM_VE_PROC,ENTSM, ENTVE, EVTVERRCSTKBND, VE_POOL, SM_SELECTOR_LC);
                  
                 if(veSmSelfPstMsg(&vePst) != ROK)
                 {
#ifdef DEBUGP
                    VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Failure at \
                          Self Pst. %d\n", smCb.smState));
#endif
                 }
               }
               break;  
            }  
#endif
            case LVE_CAUSE_CTF_SAP_BOUND:
            {
#ifdef DEBUGP
                VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF APP CTF IS \
SUCCESSFUL sapId = %d\n", dgn->u.sapId));
#endif
                 /* Initiate Bind Request towards NHU Sap  */
                 smBindVeToNhuSap();
                 break;
            }
            case LVE_CAUSE_NHU_SAP_BOUND:
            {
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF APP NHU IS \
SUCCESSFUL sapId = %d\n", dgn->u.sapId));
#endif
               /* Initiate Bind Request towards PJU Sap  */
               smBindVeToPjuSap();
               break;
            }
            case LVE_CAUSE_PJU_SAP_BOUND: 
            {
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF APP PJU IS \
SUCCESSFUL sapId = %d\n", dgn->u.sapId));
#endif
               /* Initiate Bind Request towards PJU Sap  */
               smBindVeToRgrSap();
               break;
            }
            case LVE_CAUSE_RGR_SAP_BOUND:
            {
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF APP RGR IS \
SUCCESSFUL sapId = %d\n", dgn->u.sapId));
#endif
#ifdef LTE_ENB_PERF
               smCb.smState = VE_SM_STATE_EGTP_STK_BND_DONE;
#endif

#ifdef LTE_HO_SUPPORT
               if (smCb.smState == VE_SM_STATE_X2AP_STK_BND_DONE)
#else
               if (smCb.smState == VE_SM_STATE_EGTP_STK_BND_DONE)
#endif
               {
                  smCb.smState = VE_SM_STATE_RRC_STK_BND_DONE;
#ifdef DEBUGP
               VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF RRC STACK IS \
SUCCESSFUL \n"));
#endif
                  /* Perform the bind of APP and SCTP */
                 VE_FILL_PST(vePst, SM_VE_PROC, ENTSM, ENTVE, EVTVEINITS1SETUP, VE_POOL, SM_SELECTOR_LC);
                 if(veSmSelfPstMsg(&vePst) != ROK)
                 {
#ifdef DEBUGP
                    VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Failure at \
Self Pst. %d\n", smCb.smState));
#endif
                  }
               }
               break;
            }
            default:
            {
#ifdef DEBUGP
                    VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " invalid \
                         alarm. %d\n", usta->u.usta.alarm.cause));
#endif
               break;
            }
         }
      }
      else if (usta->u.usta.alarm.event == LCM_EVENT_BND_FAIL) 
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "BIND OF APP lsap id[%d] \
FAILED Cause %d \n", dgn->u.sapId,usta->u.usta.alarm.cause));
#endif
      }
   }

   RETVALUE(ROK);
} /* end of SmMiLveStaInd() */


/********************************************************************30**
  
         End of file:     ve_sm_ve_ptmi.c@@/main/2 - Wed Dec  1 08:44:40 2010
  
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
---------- --------- ---- -----------------------------------------------
$SID$     ----       mnawas     1. eNodeb 3.1 Release
*********************************************************************91*/
