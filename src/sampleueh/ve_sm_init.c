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

     Name:     LTE Uu Stack Manager 
  
     Type:     C Source file
  
     Desc:     Stack Manager Initialization Functions.

     File:     ve_sm_init.c

     Sid:      ve_sm_init.c@@/main/2 - Wed Dec  1 08:44:32 2010

     Prg:      an 

**********************************************************************/


#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include <stdlib.h>

#include "cm5.h"           /* common timers */
#include "cm_hash.h"       /* common hash */
#include "cm_inet.h"       /* common sockets */
#include "cm_tpt.h"        /* common transport addressing */
#include "cm_llist.h"
#include "ve_sm_init.h"
#include "ve.h"


/* header/extern include files (.x) */

#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash */
#include "cm_inet.x"       /* common sockets */
#include "cm_tpt.x"        /* common transport addressing */
#include "cm_llist.x"
#include "cm_lib.x"
#include "ve_sm_init.x"

#ifdef SS_SEUM_CAVIUM
#include "cvmx.h"
#endif

PUBLIC    SmCb smCb;
PUBLIC    SmCfgCb smCfgCb;   


PUBLIC const S8 *veCfgTagLst[MAX_CFG_PARAM] = {
 "VE_TAG_CELL_ID",
 "VE_TAG_MOD_TYPE",
 "VE_TAG_DUP_MODE",
 "VE_TAG_MAX_UE_SUPPORT",
 "VE_TAG_MCC_0",
 "VE_TAG_MCC_1",
 "VE_TAG_MCC_2",
 "VE_TAG_MNC_0",
 "VE_TAG_MNC_1",
 "VE_TAG_MNC_2",
 "VE_TAG_TA_CODE",
 "VE_TAG_FREQ_BAND_IND",
 "VE_TAG_ENB_IP_ADDR",
 "VE_TAG_MME_IP_ADDR",
 "VE_TAG_REJECT_WAIT_TIME",
 "VE_TAG_SCTP_IP_ADDR",
 "VE_TAG_HI_DBG",
 "VE_TAG_SB_DBG",
 "VE_TAG_SZ_DBG",
 "VE_TAG_EG_DBG",
 "VE_TAG_VE_DBG",
 "VE_TAG_NH_DBG",
 "VE_TAG_KW_DBG",
 "VE_TAG_RG_DBG",
 "VE_TAG_YS_DBG",
 "VE_TAG_SM_DBG",
 "VE_TAG_INACTIVITY_TIMER_VAL",
 "VE_TAG_MAX_EXPIRY",
 "VE_TAG_CZ_DBG",
 "VE_TAG_X2_PREP_TMR",
 "VE_TAG_X2_OVRALL_TMR",
 "VE_TAG_ENB_NAME",
 "VE_TAG_NO_OF_BRDCST_PLMN",
 "VE_TAG_PLMN_ID_LST",
 "VE_TAG_CELL_FDD_CFG",
 "VE_TAG_CELL_TDD_CFG",
 "VE_TAG_NO_OF_NGH_INFO",
 "VE_TAG_NGH_INFO_CFG",
 "VE_TAG_NO_OF_GU_GRP",
 "VE_TAG_GU_GRP_ID_CFG",
 "VE_TAG_S1_PREP_TMR",
 "VE_TAG_S1_OVRALL_TMR",
 "VE_TAG_NO_OF_NGH_CFG",
 "VE_TAG_NGH_CELL_CFG",
 "VE_TAG_MEAS_CFG_ENB",
 "VE_TAG_RSRP_THRSHLD_VAL",
 "VE_TAG_NO_OF_NGH_ENB_CFG",
 "VE_TAG_NGH_ENB_CFG",
 "VE_TAG_ENB_TYPE",
 "VE_TAG_PCI_VAL",
 "VE_TAG_MAX_X2_PEERS",
 "VE_TAG_X2_TIME_TO_WAIT",
 "VE_TAG_UE_HO_ATTACH_TMR",
 "VE_TAG_MAC_RNTI_STRT",
 "VE_TAG_MAX_MAC_RNTIS",
 "VE_TAG_MAC_NO_OF_PREMBL",
 "VE_TAG_MAX_RRM_RNTIS",
 "VE_TAG_RRM_NO_OF_DED_PREMBL",
 "VE_TAG_RRM_DED_PREMBL_STRT",
 "VE_TAG_SCTP_SRVC_TYPE",
 "VE_TAG_OP_MODE",
 "VE_TAG_COUNTER",
 "VE_TAG_PERIOD",
 "VE_RRC_CON_TIMER",
 "VE_TAG_ENB_SCTP_PORT",
 "VE_TAG_MME_SCTP_PORT"

};


/*
*
*       Fun:   tst
*
*       Desc:  tst entry point. First entry point.
*
*       Ret:   ROK on success, RFAILED on error
*
*       Notes: None
*
*       File:  ve_sm_init.c
*
*/
#ifdef ANSI
PUBLIC S16 tst
(
Void
)
#else
PUBLIC S16 tst(Void)
#endif
{
#ifdef VE_DONGWON_OAM_PLD
   U32   rc;
#endif /* VE_DONGWON_OAM_PLD */
   /* Added trace macro */

   /* System tasks that are needed as per the architecture */
   SSTskId egtp_dat_app_taskId;
   SSTskId rrc_s1ap_app_taskId;
   SSTskId sctp_taskId;
   SSTskId sm_taskId;
   SSTskId tucl_taskId;
   U16 smEvent;
   TRC2(tst)


   SSetProcId(SM_SM_PROC);

   /* All the System tasks(threads)  are created with same priority*/
   SCreateSTsk(PRIOR0, &egtp_dat_app_taskId); 
   SCreateSTsk(PRIOR0, &rrc_s1ap_app_taskId);
   SCreateSTsk(PRIOR0, &sctp_taskId);
   SCreateSTsk(PRIOR0, &sm_taskId);
   SCreateSTsk(PRIOR0, &tucl_taskId);
#ifdef DEBUGP
   /* Fix for CRId- 115717 */
   /* dbgMask initialized to 0 */
   smCb.init.dbgMask = 0;
/* MSPD changes */
#ifdef MSPD
   smCb.init.dbgMask = 0xffffffff;
#endif
#endif
   /* Create RRC, RRC App and S1AP TAPA tasks and attach them 
      to a single system thread */
   if(smNhInit(rrc_s1ap_app_taskId) != ROK)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC Tapa Task \
               initialization failed\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

#ifdef VE_RELAY
   if(smVeInit(rrc_s1ap_app_taskId) != ROK )
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "RRC App Tapa Task \
               initialization failed\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */
#endif /* end of VE_RELAY */
#ifndef LTE_ENB_PERF
   if(smSzInit(rrc_s1ap_app_taskId) != ROK )
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP Tapa Task \
               initialization failed\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

#ifdef LTE_HO_SUPPORT
   if(smCzInit(rrc_s1ap_app_taskId) != ROK )
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "S1AP Tapa Task \
               initialization failed\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */
#endif

   /* Create EGTP and EGTP APP TAPA tasks and attach them 
      to a single system thread */
   if(smEgInit(egtp_dat_app_taskId) != ROK)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "EGTP Tapa Task \
               initialization failed\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */
#endif
#ifdef EU_DAT_APP 
   if(smEuInit(egtp_dat_app_taskId) != ROK)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "EGTP Data APP Tapa Task \
               initialization failed\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */
#endif /* EU_DAT_APP */
#ifndef LTE_ENB_PERF
   /* Create SCTP TAPA tasks and attach them to system thread */
   if(smSbInit(sctp_taskId) != ROK)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SCTP Tapa Task \
               initialization failed\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Create TUCL TAPA task and attach them to system threads */
   if(smHiInit(tucl_taskId) != ROK)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "TUCL Tapa Task \
               initialization failed\n"));
#endif
      RETVALUE(RFAILED);
   } /* end of if statement */
#endif
   /* Create the SM TAPA task */
   SRegTTsk(ENTSM, (Inst) 0, TTNORM, PRIOR0, (PAIFS16)smActvInit, smActvTsk);
   SAttachTTsk(ENTSM, (Inst) 0, sm_taskId);

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "TAPA TASKS SUCCESSFULLY\
            REGISTERED\n"));
#endif

   /* Read the parameters from the file and copy into global control block */
   if(smVeReadConfigParams() != ROK)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "FAILED TO READ FROM FILE \
               ve_cfg.txt\n"));
#endif
   } /* end of if statement */

#ifdef DEBUGP
   /* Fix for CRId- 115717 */
   /* Enable sm debugs */
   if(smCfgCb.smDbg == 1)
   {
      /* Update DbgMsk */
      smCb.init.dbgMask = 0xffffffff;
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Enabled SM Debug Prints\n"));
   } /* end of if statement */
#endif

   smCb.smState = VE_SM_STATE_INIT;
#ifndef LTE_ENB_PERF
   smEvent = EVTVETUCLCFG;
#else
   smEvent = EVTVEAPPCFG;
#endif

#ifdef VE_DONGWON_OAM
   if(dbiSubscribeInit(4,0) != ROK)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "FAILED TO INIT PLD\n"));
#endif
   exit(0);
   }
   /*Read the parameters from the PLD*/
   if(vePopulateFromPld() != ROK)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "FAILED TO READ FROM FILE \
               PLD\n"));
#endif
   exit(0);
   }
   else
{
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SUCCESSFULLY READ FROM \
               PLD\n"));
#endif

}
#endif
   /* Invoke the state machine */
   smVeProcSm(smEvent);

   RETVALUE(ROK);

} /* end of tst function */

/*
*
*       Fun:  veSmSelfPstMsg 
*
*       Desc:  This function is used by  Layer to self post 
*              a message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ve_sm_init.c
*
*/
#ifdef ANSI
PUBLIC S16 veSmSelfPstMsg
(
Pst   *pst
)
#else
PUBLIC S16 veSmSelfPstMsg(pst)
Pst   *pst;
#endif
{
   Buffer *mBuf;          /* message buffer */

   TRC2(veSmSelfPstMsg);

   if(SGetMsg(DFLT_REGION, VE_POOL, &mBuf) != ROK)
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   RETVALUE(SPstTsk(pst, mBuf));

} /* end of veSmSelfPstMsg */


/*
*
*       Fun:   smVeProcSm
*
*       Desc:  This function is entry function to the  main State Machine
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/


#ifdef ANSI
PUBLIC Void smVeProcSm
(
U8 event
)
#else
PUBLIC Void smVeProcSm( event)
U8  event;
#endif /* ANSI */
{
   TRC2(smVeProcSm);

   switch (smCb.smState)
   {
      case VE_SM_STATE_INIT:
         {
#ifndef LTE_ENB_PERF
            veSmHdlTuclCfgEvent(event);
            break;
         }
      case VE_SM_STATE_TUCL_CFG_DONE:
         {
            veSmHdlSctpCfgEvent(event);
            break;
         }
      case VE_SM_STATE_SCTP_CFG_DONE:
         {
            veSmHdlS1apCfgEvent(event);
            break;
         }
      case VE_SM_STATE_S1AP_CFG_DONE:
         {
            veSmHdlEgtpCfgEvent(event);
            break;
         }
      case VE_SM_STATE_EGTP_CFG_DONE:
         {
#ifdef LTE_HO_SUPPORT
            veSmHdlX2apCfgEvent(event);       
            break;
         }
      case VE_SM_STATE_X2AP_CFG_DONE:
        {
#endif
#endif
            veSmHdlAppCfgEvent(event);
            /* SELVA MSPD */

            /*veSmHdlEnbDebugs(event);*/
            break;
         }
      case VE_SM_STATE_APP_CFG_DONE:
         {
            veSmHdlRrcCfgEvent(event);
            break;
         }
      case VE_SM_STATE_RRC_CFG_DONE:
         {
#ifdef CNE_UE_SIM_TEST
            /* For eNodeB Sim we donot required few steps,
             * so directly jumping to ENodeB Debug enabling*/
            smCb.smState = VE_SM_STATE_CL_CFG_DONE;
            veSkipSteps(SM_VE_PROC, EVTVEENBDEBUGS);
            break;
#else          
            veSmHdlPdcpCfgEvent(event);
            break;
#endif /* CNE_UE_SIM_TEST */
         }
      case VE_SM_STATE_PDCP_CFG_DONE:
         {
            veSmHdlRlcCfgEvent(event);
            break;
         }
      case VE_SM_STATE_RLC_CFG_DONE:
         {
            veSmHdlMacCfgEvent(event);
            break;
         }
      case VE_SM_STATE_MAC_CFG_DONE:
         {
#ifndef LTE_ENB_PAL
            veSmHdlCLCfgEvent(event);
            break;
         }
      case VE_SM_STATE_CL_CFG_DONE:
         {
#endif /* LTE_ENB_PAL */   
            /* Initializing Dbg Mask to Zero */
            smCfgCb.smDbgMsk = 0;
            veSmHdlEnbDebugs(event);
            break;
         }
      case VE_SM_STATE_ENB_DEBUGS_DONE:
         {
            printf("Sending alarms\n");
            veSmHdlEnbAlarms(event);
            break;
         }
      case VE_SM_STATE_ENB_ALRMS_DONE:
         {
#ifndef LTE_ENB_PERF
            veSmHdlBndS1apStack(event);
            break;
         }
      case VE_SM_STATE_S1AP_STK_BND_DONE:
         {
            veSmHdlBndEgtpStack(event);
            break;
         }
      case VE_SM_STATE_EGTP_STK_BND_DONE:
         {

#ifdef LTE_HO_SUPPORT
           veSmHdlBndX2apStack(event);
           break;
         }
      case VE_SM_STATE_X2AP_STK_BND_DONE:
         {
#endif
#endif    
#ifdef CNE_UE_SIM_TEST
            /* For eNodeB Sim we donot required few steps,
             * so directly jumping to next state*/
            smBindNhToCkwSap();
            break;
#else      
            veSmHdlBndRrcStack(event);
            break;
#endif /* CNE_UE_SIM_TEST*/
         }
      case VE_SM_STATE_RRC_STK_BND_DONE:
         {
#ifdef CNE_UE_SIM_TEST
            veSmHdlInitCellCfg(event);
            /* For eNodeB Sim we donot required few steps,
             * so directly jumping to next state*/
            smCb.smState = VE_SM_STATE_CELL_UP;
            veSkipSteps(SM_VE_PROC, EVTVEINITS1SETUP);
            break;
         }
      case VE_SM_STATE_CELL_UP:
         {
#endif
#ifndef LTE_ENB_PERF
            veSmHdlInitS1Setup(event);
#else
            smCb.smState = VE_SM_STATE_CELL_UP;
            veSmHdlInitCellCfg(EVTVECELLCFG);
#endif
            break;
         }
#ifdef LTE_HO_SUPPORT
      case VE_SM_STATE_CELL_UP:
      {
         veSmHdlInitX2Setup(event);
         break;
      }
#endif
      default:
         {
#ifdef DEBUGP
            VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "smVeProcSm() Invalid \
                     state received \n"));
#endif
            break;
         }
   }
   RETVOID;
} /* smVeProcSm */

#ifndef LTE_ENB_PERF
/*
*
*       Fun:   veSmHdlTuclCfgEvent
*
*       Desc:  This function is handles events in Init state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/


#ifdef ANSI
PUBLIC Void veSmHdlTuclCfgEvent
(
U8 event
)
#else
PUBLIC Void veSmHdlTuclCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2 (veSmHdlTuclCfgEvent);

   if(event == EVTVETUCLCFG)
   {
      /* Start TUCL Configuration */
      veSmHiCfg();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlTuclCfgEvent() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlTuclCfgEvent */
/*
*
*       Fun:   veSmHdlSctpCfgEvent
*
*       Desc:  This function is handles events in TUCL Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/


#ifdef ANSI
PUBLIC Void veSmHdlSctpCfgEvent
(
U8 event
)
#else
PUBLIC Void veSmHdlSctpCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlSctpCfgEvent);

   if(event == EVTVESCTPCFG )
   {
      /* Start SCTP Configuration */
      veSmSbCfg();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlSctpCfgEvent() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlSctpCfgEvent */

/*
*
*       Fun:   veSmHdlS1apCfgEvent
*
*       Desc:  This function is handles events in SCTP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/


#ifdef ANSI
PUBLIC Void veSmHdlS1apCfgEvent
(
U8 event
)
#else
PUBLIC Void veSmHdlS1apCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   /* Corrected function name in trace macro */
   TRC2(veSmHdlS1apCfgEvent);

   if(event == EVTVES1APCFG )
   {
      /* Start S1AP Configuration */
      veSmSzCfg();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlS1apCfgEvent() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlS1apCfgEvent */

#ifdef LTE_HO_SUPPORT
/*
*
*       Fun:   veSmHdlX2apCfgEvent
*
*       Desc:  This function is handles events in X2AP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/


#ifdef ANSI
PUBLIC Void veSmHdlX2apCfgEvent
(
U8 event
)
#else
PUBLIC Void veSmHdlX2apCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlX2apCfgEvent);

   if(event == EVTVEX2APCFG )
   {
      /* Start X2AP Configuration */
      veSmCzCfg();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlX2apCfgEvent() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlX2apCfgEvent */
#endif

/*
*
*       Fun:   veSmHdlEgtpCfgEvent
*
*       Desc:  This function is handles events in S1AP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/
#ifdef ANSI
PUBLIC Void veSmHdlEgtpCfgEvent
(
U8 event
)
#else
PUBLIC Void veSmHdlEgtpCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlEgtpCfgEvent);

   if(event == EVTVEEGTPCFG )
   {
      /* Start EGTP Configuration */
      veSmEgCfg();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlEgtpCfgEvent() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlEgtpCfgEvent */
#endif
/*
*
*       Fun:   veSmHdlAppCfgEvent
*
*       Desc:  This function is handles events in EGTP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/


#ifdef ANSI
PUBLIC Void veSmHdlAppCfgEvent
(
U8 event
)
#else
PUBLIC Void veSmHdlAppCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlAppCfgEvent);

   if(event == EVTVEAPPCFG )
   {
      /* Start Application Configuration */
      veSmVeCfg();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlAppCfgEvent() Invalid \
               event received\n "));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlAppCfgEvent */

/*
*
*       Fun:   veSmHdlRrcCfgEvent
*
*       Desc:  This function is handles events in EGTP App Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/


#ifdef ANSI
PUBLIC Void veSmHdlRrcCfgEvent
(
U8 event
)
#else
PUBLIC Void veSmHdlRrcCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlRrcCfgEvent);

   if(event == EVTVERRCCFG )
   {
      /* Start RRC Configuration */
      veSmNhCfg();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlRrcCfgEvent() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlRrcCfgEvent */

/*
*
*       Fun:   veSmHdlPjCfgEvent
*
*       Desc:  This function is handles events in RRC APP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/
#ifdef ANSI
PUBLIC Void veSmHdlPdcpCfgEvent
(
U8 event
)
#else
PUBLIC Void veSmHdlPdcpCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlPdcpCfgEvent);

   if(event == EVTVEPDCPCFG )
   {
      /* Start PDCP Configuration */
      veSmPjCfg();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlPdcpCfgEvent() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlPdcpCfgEvent */

/*
*
*       Fun:   veSmHdlRlcCfgEvent
*
*       Desc:  This function is handles events in PDCP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/


#ifdef ANSI
PUBLIC Void veSmHdlRlcCfgEvent
(
U8 event
)
#else
PUBLIC Void veSmHdlRlcCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlRlcCfgEvent);

   if(event == EVTVERLCCFG )
   {
      /* Start RLC Configuration */
      veSmKwCfg();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlRlcCfgEvent() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlRlcCfgEvent */

/*
*
*       Fun:   veSmHdlMacCfgEvent
*
*       Desc:  This function is handles events in RLC Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/


#ifdef ANSI
PUBLIC Void veSmHdlMacCfgEvent
(
U8 event
)
#else
PUBLIC Void veSmHdlMacCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlMacCfgEvent);

   if(event == EVTVEMACCFG )
   {
      /* Start LTE MAC Configuration */
      veSmRgCfg();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlMacCfgEvent() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlMacCfgEvent */


/*
*
*       Fun:   veSmHdlCLCfgEvent
*
*       Desc:  This function is handles events in MAC Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/


#ifdef ANSI
PUBLIC Void veSmHdlCLCfgEvent
(
U8 event
)
#else
PUBLIC Void veSmHdlCLCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlCLCfgEvent);

   if(event == EVTVECLCFG )
   {
      /* Start LTE Convergence Layer Configuration */
      veSmYsCfg();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlCLCfgEvent() Invalid \
               event received = %d \n", event));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlCLCfgEvent */
#ifndef LTE_ENB_PERF
/*
*
*       Fun:   veSmHdlBndS1apStack
*
*       Desc:  This function is handles events in CL Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/


#ifdef ANSI
PUBLIC Void veSmHdlBndS1apStack
(
U8 event
)
#else
PUBLIC Void veSmHdlBndS1apStack(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlBndS1apStack);

   if(event == EVTVES1APSTKBND)
   {
      /*Bind the SCTP lower SAP with TUCL */
      smBindSbToLSap(SB_HI_SPID);
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlCLCfgEvent() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlCLCfgEvent */

/*
*
*       Fun:   veSmHdlBndEgtpStack
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/

#ifdef ANSI
PUBLIC Void veSmHdlBndEgtpStack
(
U8 event
)
#else
PUBLIC Void veSmHdlBndEgtpStack(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlBndEgtpStack);

   if(event == EVTVEEGTPSTKBND)
   {
      /*Bind the EGTP lower SAP with */
      smBindEgToHitSap(EG_HI_SPID);
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlBndEgtpStack() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlBndEgtpStack */

#ifdef LTE_HO_SUPPORT
/*
*
*       Fun:   veSmHdlBndX2apStack
*
*       Desc:  This function is handles events in Egtp Stack Bind  done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/


#ifdef ANSI
PUBLIC Void veSmHdlBndX2apStack
(
U8 event
)
#else
PUBLIC Void veSmHdlBndX2apStack(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlBndX2apStack);

   if(event == EVTVEX2APSTKBND)
   {
      /*Bind the X2AP lower SAP with SCTP */
      smBindCzToLSap();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlBndX2apStack() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlCLCfgEvent */
#endif /* LTE_HO_SUPPORT */
#endif /* LTE_ENB_PERF */

/*
*
*       Fun:   veSmHdlBndRrcStack
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/

#ifdef ANSI
PUBLIC Void veSmHdlBndRrcStack
(
U8 event
)
#else
PUBLIC Void veSmHdlBndRrcStack(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlBndRrcStack);

   if(event == EVTVERRCSTKBND)
   {
      /*Bind the MAC lower SAP with CL */
      smBindRgToTfuSap();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlBndRrcStack() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlBndEgtpStack */

/*
*
*       Fun:   veSmHdlEnbAlarms
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/

#ifdef ANSI
PUBLIC Void  veSmHdlEnbAlarms
(
U8 event
)
#else
PUBLIC Void veSmHdlEnbAlarms(event)
U8  event;
#endif /* ANSI */
{
   Pst  vePst;

   TRC2(veSmHdlEnbAlarms);
   SM_SET_ZERO(&vePst, sizeof(Pst));

   if(event == EVTVEENBALRMS)
   {
      /* Send Control request to enable alarms in reqd. layers */
#ifndef LTE_ENB_PERF
      smBuildSzGenCntrl();

#ifdef LTE_HO_SUPPORT
      smBuildCzGenCntrl();
#endif
#endif
      smBuildNhGenCntrl();
      smBuildVeGenCntrl();

      smCb.smState = VE_SM_STATE_ENB_ALRMS_DONE;
      /* Trigger to next state */
#ifndef LTE_ENB_PERF
      VE_FILL_PST(vePst, SM_VE_PROC, ENTSM, ENTVE, EVTVES1APSTKBND, VE_POOL, SM_SELECTOR_LC);
#else
      VE_FILL_PST(vePst, SM_VE_PROC, ENTSM, ENTVE, EVTVERRCSTKBND, VE_POOL, SM_SELECTOR_LC);
#endif

      if(veSmSelfPstMsg(&vePst) != ROK)
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "ENABLING OF ALARMS IS \
                  SUCCESSFULLY PERFORMED. Failure at Self Pst.\n"));
#endif
      }
      else
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "ENABLING OF ALARMS IS \
                  SUCCESSFULLY PERFORMED. \n"));
#endif
      } /* end of else part */

   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlBndRrcStack() Invalid \
               event received \n"));
#endif
   } /* end of else part */
/* MSPD changes */
#ifdef MSPD
   smBuildNhGenCntrl();
#endif
RETVOID;
} /* end of veSmHdlBndEgtpStack */

/*
 *
 *       Fun:   veSmHdlEnbDebugs
 *
 *       Desc:  This function is handles events in done state
 *
 *       Ret:   None
 *
 *       Notes: <NONE>
 *
 *       File:  ve_sm_init.c
 *
 */

#ifdef ANSI
PUBLIC Void veSmHdlEnbDebugs
(
U8 event
)
#else
PUBLIC Void veSmHdlEnbDebugs(event)
U8  event;
#endif /* ANSI */
{
   Pst lclPst;
   U8  nextEvent = 0;

   TRC2(veSmHdlEnbDebugs);

   SM_SET_ZERO(&lclPst, sizeof(Pst));

#ifdef DEBUGP
   if(event == EVTVEENBDEBUGS)
   {
#ifndef LTE_ENB_PERF
      /* Send Control request to enable debugs */
      if((smCfgCb.hiDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_HI_DBG_MASK))
      {
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Enable HI Debug Prints\n"));
         smBuildHiDbgCntrl();
         RETVOID;
      } /* end of if statement */
      /* Send Control request to enable debugs */
      if((smCfgCb.sbDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_SB_DBG_MASK))
      {
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Enable SB Debug Prints\n"));
         smBuildSbDbgCntrl();
         RETVOID;
      } /* end of if statement */
      /* Send Control request to enable debugs */
      if((smCfgCb.szDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_SZ_DBG_MASK))
      {
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Enable SZ Debug Prints\n"));
         smBuildSzDbgCntrl();
         RETVOID;
      } /* end of if statement */
      /* Send Control request to enable debugs */
      if((smCfgCb.egDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_EG_DBG_MASK))
      {
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Enable EG Debug Prints\n"));
         smBuildEgDbgCntrl();
         RETVOID;
      } /* end of if statement */

#ifdef LTE_HO_SUPPORT
      /* Send Control request to enable debugs */
      if((smCfgCb.czDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_CZ_DBG_MASK))
      {
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Enable CZ Debug Prints\n"));
         smBuildCzDbgCntrl();
         RETVOID;
      } /* end of if statement */
#endif
#endif
      /* Send Control request to enable debugs */
      if((smCfgCb.veDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_VE_DBG_MASK))
      {
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Enable VE Debug Prints\n"));
         smBuildVeDbgCntrl();
         RETVOID;
      } /* end of if statement */
      /* Send Control request to enable debugs */
      if((smCfgCb.nhDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_NH_DBG_MASK))
      {
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Enable NH Debug Prints\n"));
         smBuildNhDbgCntrl();
         RETVOID;
      } /* end of if statement */
      /* Send Control request to enable debugs */
      if((smCfgCb.kwDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_KW_DBG_MASK))
      {
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Enable KW  Debug Prints\n"));
         smBuildKwDbgCntrl();
         RETVOID;
      } /* end of if statement */
      /* Send Control request to enable debugs */
      if((smCfgCb.rgDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_RG_DBG_MASK))
      {
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Enable RG  Debug Prints\n"));
         smBuildRgDbgCntrl();
/*CRID: ccpu00119280 Enabling Debug for MAC Scheduler*/
    /* Enabling the debug prints for MAC scheudler*/
         smBuildRgSchDbgCntrl();
                  RETVOID;
      } /* end of if statement */
#ifdef YS_PICO
      /* Send Control request to enable debugs */
      if((smCfgCb.ysDbg == 1) && !(smCfgCb.smDbgMsk & VE_SM_YS_DBG_MASK))
      {
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Enable YS  Debug Prints\n"));
         smBuildYsDbgCntrl();
         RETVOID;
      } /* end of if statement */
#endif /* YS_PICO */ 
   }
   else
   {
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlBndRrcStack() Invalid \
               event received \n"));
      RETVOID;
   } /* end of else part */

#endif /* DEBUGP */ 

   /* Assuming after all Debug Cntrl Reqs. */
   smCb.smState = VE_SM_STATE_ENB_DEBUGS_DONE;
   nextEvent = EVTVEENBALRMS;

   VE_FILL_PST(lclPst, SM_VE_PROC, ENTSM, ENTVE, nextEvent, VE_POOL, SM_SELECTOR_LC);

   if(veSmSelfPstMsg(&lclPst) != ROK)
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Enabling Debug Prints\
               SUCCESSFULLY PERFORMED. Failure at Self Pst.\n"));
#endif
   } /* end of if statement */

   RETVOID;
} /* end of veSmHdlEnbDebugs */

/*
*
*       Fun:   veSmHdlInitS1Setup
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/

#ifdef ANSI
PUBLIC Void  veSmHdlInitS1Setup
(
U8 event
)
#else
PUBLIC Void veSmHdlInitS1Setup(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlInitS1Setup);

   if(event == EVTVEINITS1SETUP)
   {
      /* Send Control request to APP to indicate binding is complete*/
      smBuildVeS1InitCntrl();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlInitS1Setup() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlInitS1Setup*/

/*
*
*       Fun:   veSmHdlInitCellCfg
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/

#ifdef ANSI
PUBLIC Void  veSmHdlInitCellCfg
(
U8 event
)
#else
PUBLIC Void veSmHdlInitCellCfg(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlInitCellCfg);

   if(event == EVTVECELLCFG)
   {
      /* Send Control request to APP to indicate binding is complete*/
      smBuildVeCellCntrl();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlInitCellCfg() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlInitCellCfg */

#ifdef LTE_HO_SUPPORT
/*
*
*       Fun:   veSmHdlInitX2Setup
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  ve_sm_init.c
*
*/

#ifdef ANSI
PUBLIC Void  veSmHdlInitX2Setup
(
U8 event
)
#else
PUBLIC Void veSmHdlInitX2Setup(event)
U8  event;
#endif /* ANSI */
{
   TRC2(veSmHdlInitX2Setup);

   if(event == EVTVEINITX2SETUP)
   {
      /* Send Control request */
      smBuildVeX2InitCntrl();
   }
   else
   {
#ifdef DEBUGP
      VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "veSmHdlInitX2Setup() Invalid \
               event received \n"));
#endif
   } /* end of else part */

   RETVOID;
} /* end of veSmHdlInitX2Setup*/
#endif

/*
*
*       Fun:   veCfgGetTagNum
*
*       Desc:  Retrive the Tag number corresponds to the tag string.
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  ve_sm_init.c
*
*/
#ifdef ANSI
PUBLIC Void smVeCfgGetTagNum
(
U8* tag,
U16* tagNum
)
#else
PUBLIC Void smVeCfgGetTagNum(tag, tagNum)
U8* tag;
U16* tagNum;
#endif
{
   S16 loopCnt = 0;

   TRC2(smVeCfgGetTagNum);

   for (loopCnt = 0; loopCnt < MAX_CFG_PARAM; loopCnt++)
   {
      if (0 == strcmp((const S8*)tag,veCfgTagLst[loopCnt]))
      {
         *tagNum = loopCnt;
         break;
      }
   }/* End of for */
   RETVOID;
}/* End of veCfgGetTagNum()*/

/*
*
*       Fun:   veCfgValdtInput
*
*       Desc:  This function validates the format of the input and 
*              throws an error in case it's wrong.
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  ve_sm_init.c
*
*/
#ifdef ANSI
PUBLIC Void veCfgValdtInput
(
U8* input,
Bool* status,
U8* noOfElem
)
#else
PUBLIC Void veCfgValdtInput(input, status, noOfElem)
U8* input;
Bool* status;
U8* noOfElem;
#endif
{
   U16 strLen = 0;
   S16 loopCnt = 0;
   U8 lCnt = 0;
   U8 rCnt = 0;
   U8 dupCnt = 0;   
   U8 numOfComa = 0;
   TRC2(veCfgValdtInput);
   strLen = strlen((const S8*)input);
   while (loopCnt < strLen)
   {
      if ('{' == input[loopCnt])
      {
        lCnt++;
      }
      else if ('}' == input[loopCnt])
      {
        rCnt++;
        /* Calculating the number of inner structures and arrays */
        if ((lCnt - rCnt) >= 2)
        {
           dupCnt++;
        }
      }
      else if (' ' == input[loopCnt])
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Please check the\
         Configuration format.There should not be any space in the\
         configuration values.\n"));
#endif
         *status = FALSE;
         break;
      }
      else if ((',' == input[loopCnt]) && (',' == input[loopCnt + 1]))      
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Please check the \
                  Configuration format.Parameter value is  missing between \
                  two commo's.\n"));
#endif
         *status = FALSE;
         break;
      }
      else if (('}' == input[loopCnt]) && ('{' == input[loopCnt + 1]))      
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Please check the \
                  Configuration format.commo is missing.\n"));
#endif
         *status = FALSE;
         break;
      }
      else if (',' == input[loopCnt])
      {
         numOfComa++;
      }
      loopCnt++;
   }/* End of while()*/
   if (FALSE != *status)
   {
      if (lCnt != rCnt)
      {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Please check the \
                  Configuration format. '{' or '}' missed out or remove the space \
                  if presents\n"));
#endif
         *status = FALSE;
      }
      else
      {
         /* Condition to check whether the input is a simple array of system
            defined data types or a single user defined structure.*/
         if (lCnt == 1)
         {
            /* Calculating the number of elements in the array or structure using the 
               number of commas */
            *noOfElem = numOfComa + 1;
            *status = TRUE;
         }
         else
         {
            /* Calculating the number of structures if the input is array 
             * of structures. deduction of 1 is for the outer most braces and 
             * deduction of dupCnt is for inner structures if any */
            *noOfElem = (lCnt - 1) - dupCnt;
            *status = TRUE;
         }
      }
   }/* End of if ()*/
   RETVOID;
}/* End of veCfgValdtInput() */

/*
*
*       Fun:   veCfgGetParamVal
*
*       Desc:  This function parses the validated input and makes the
*              the list of parameters and returns the list and the number of parameters.
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  ve_sm_init.c
*
*/
#ifdef ANSI
PUBLIC Void veCfgGetParamVal
(
U8* tagVal,
U8 paramLst[][50],
U16* noOfParams
)
#else
PUBLIC Void veCfgGetParamVal(tagVal, paramLst[][50], noOfParams)
U8* tagVal;
U8 paramLst[][50];
U16 *noOfParams;
#endif
{
   U16 strLen = 0;
   S16 loopCnt = 0;
   U16 loopCnt1 = 0;
   U8 param[50];
   TRC2(veCfgGetParamVal);
   strLen = strlen((const S8*)tagVal);
   while (loopCnt < strLen)
   {
      if (('{' != tagVal[loopCnt]) && ('}' != tagVal[loopCnt]) && 
         (',' != tagVal[loopCnt]))   
      {
         loopCnt1 = 0;
         while((',' != tagVal[loopCnt]) && ('}' != tagVal[loopCnt]))
         {
            param[loopCnt1++] = tagVal[loopCnt];
            loopCnt++;
         }
         param[loopCnt1] = '\0'; 
         /* Increment the param count for this parameter */
         strcpy((S8*)paramLst[*noOfParams], (const S8*)param);
         (*noOfParams)++;
      }
      else
      {
         /* skipping the charecter */
         loopCnt += 1;
      }
   }/* End of while()*/
   RETVOID;
}/* End of veCfgGetParamVal() */

/*
*
*       Fun:   smVeReadConfigParams
*
*       Desc:  Reads the values of the configurable parameters from 
*              the file ve_cfg.txt and stores them
*              in the smCfgCb
*
*       Ret:   ROK, RFAILED
*
*       File:  ve_sm_init.c
*
*/


#ifdef ANSI
PUBLIC S16 smVeReadConfigParams
(
)
#else
PUBLIC S16 smVeReadConfigParams (Void)
#endif
{
   FILE  *fp;
   U8 tag[50];
   U8 tagVal[50];
   U16 tagNum = 0;
#ifdef LTE_HO_SUPPORT
   Bool status = TRUE;
   U16 noOfParams = 0;
   U8  paramLst[50][50];
   U8 loopCnt = 0;
   U16 paramCnt = 0;
   U8 noOfElem = 0;
   VeGuGrpIdCfg *guGrpIdCfg = NULLP;
   VeNghInfoCfg *nghInfoCfg = NULLP;
   VeNghCellCfg *nghCellCfg = NULLP;
   VeNghENBCfg *nghENBCfg = NULLP;
#endif

   TRC2(smVeReadConfigParams);

   if((fp = fopen("ve_cfg.txt","r")) == NULL)
   {
      /* fix for Defect ccpu00117186 */
      /* configuraiton file checking */
      VE_LOGERROR(ERRCLS_DEBUG,0,ERRZERO,
            "FAILED to open the file ve_cfg.txt\n");
      exit(0);
   } /* end of if statement */
   /* Read the file line by line */
   while(feof(fp) == 0)
   {
      
      fscanf(fp,"%s %*s %s",tag,tagVal);
      if ((0 == strcmp("INVLD",(const S8*)tagVal)))
      continue;
      smVeCfgGetTagNum(tag,&tagNum);
      switch(tagNum)
      {
         case VE_TAG_CELL_ID:
            {
               smCfgCb.cellId = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_MOD_TYPE:
            {
               smCfgCb.modType = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_DUP_MODE:
            {
               smCfgCb.duplexMode = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_MAX_UE_SUPPORT:
            {
               smCfgCb.maxUeSupp = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_MCC_0:
            {
               smCfgCb.mcc[0].pres = TRUE;
               smCfgCb.mcc[0].val = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_MCC_1:
            {
               smCfgCb.mcc[1].pres = TRUE;
               smCfgCb.mcc[1].val = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_MCC_2:
            {
               smCfgCb.mcc[2].pres = TRUE;
               smCfgCb.mcc[2].val = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_MNC_0:
            {
               smCfgCb.mnc[0].pres = TRUE;
               smCfgCb.mnc[0].val = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_MNC_1:
            {
               smCfgCb.mnc[1].pres = TRUE;
               smCfgCb.mnc[1].val = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_MNC_2:
            {
               if (-1 == atoi((const S8*)tagVal))
               {
                  smCfgCb.mnc[2].pres= FALSE;
                  smCfgCb.mnc[2].val = atoi((const S8*)tagVal);
               }
               else
               {
                  smCfgCb.mnc[2].pres= TRUE;
                  smCfgCb.mnc[2].val = atoi((const S8*)tagVal);
               }
            }
            break;   
         case VE_TAG_TA_CODE:
            {
               smCfgCb.trackAreaCode = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_FREQ_BAND_IND:
            {
               smCfgCb.freqBandInd   = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_ENB_IP_ADDR:
            {
               cmInetAddr((S8*)tagVal,&(smCfgCb.enbIpAddr));
               smCfgCb.enbIpAddr = CM_INET_HTON_U32(smCfgCb.enbIpAddr);            
            }
            break;   
         case VE_TAG_MME_IP_ADDR:
            {
               if(smCfgCb.numMmes >= VE_MAX_MMES)
               {
                 RETVALUE(RFAILED);
               }
               cmInetAddr((S8*)tagVal,&(smCfgCb.mmeIpAddr[smCfgCb.numMmes]));
               smCfgCb.mmeIpAddr[smCfgCb.numMmes] = 
                     CM_INET_HTON_U32(smCfgCb.mmeIpAddr[smCfgCb.numMmes]);
               smCfgCb.mmeIdx[smCfgCb.numMmes] = smCfgCb.numMmes + 1;
               smCfgCb.numMmes++;
            }
            break;
         case VE_TAG_REJECT_WAIT_TIME:
            {
               smCfgCb.rejectWaitTime = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_SCTP_IP_ADDR:
            {
               cmInetAddr((S8*)tagVal,&(smCfgCb.sctpIpAddr));
#ifdef DEBUGP
VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SM IP %ld\n",smCfgCb.sctpIpAddr));
#endif
               smCfgCb.sctpIpAddr = CM_INET_HTON_U32(smCfgCb.sctpIpAddr);
#ifdef DEBUGP
VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "SM IP %ld\n",smCfgCb.sctpIpAddr));
#endif
            }
            break;   
         case VE_TAG_HI_DBG:
            {
               smCfgCb.hiDbg = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_SB_DBG:
            {
               smCfgCb.sbDbg = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_SZ_DBG:
            {
               smCfgCb.szDbg = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_EG_DBG:
            {
               smCfgCb.egDbg = atoi((const S8*)tagVal);               
            }
            break;   
         case VE_TAG_VE_DBG:
            {
               smCfgCb.veDbg  = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_NH_DBG:
            {
               smCfgCb.nhDbg = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_KW_DBG:
            {
               smCfgCb.kwDbg = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_RG_DBG:
            {
               smCfgCb.rgDbg = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_YS_DBG:
            {
               smCfgCb.ysDbg = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_SM_DBG:
            {
               smCfgCb.smDbg = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_INACTIVITY_TIMER_VAL:
            {
               smCfgCb.inactvTmrVal = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_MAX_EXPIRY:
            {
               smCfgCb.maxExpires = atoi((const S8*)tagVal);
            }
            break;   
         case VE_TAG_ENB_TYPE:
            {
               smCfgCb.eNBType = (VeeNBType) atoi((const S8*)tagVal);
            }
            break;
#ifdef LTE_HO_SUPPORT
         case VE_TAG_CZ_DBG:
            {
               smCfgCb.czDbg = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_X2_PREP_TMR:
            {
               smCfgCb.x2PrepTimerVal = atoi((const S8*)tagVal); 
            }
            break;
         case VE_TAG_X2_OVRALL_TMR:
            {
               smCfgCb.x2OvrAllTimerVal = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_ENB_NAME:
            {
               strcpy((S8*)smCfgCb.enbName, (const S8*)tagVal); 
            }
            break;
         case VE_TAG_NO_OF_BRDCST_PLMN:
            {
               smCfgCb.noOfBrdCstPlmn = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_PLMN_ID_LST:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               veCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  veCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  for (loopCnt = 0; loopCnt < noOfParams; loopCnt++)
                  {
                     strcpy((S8*)smCfgCb.plmnId[loopCnt], (const S8*)paramLst[loopCnt]);
                  }
               }
               else
               {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Error in VE_TAG_PLMN_ID_LST \
                  tag format \n"));
#endif
               }
            }
            break;
         case VE_TAG_CELL_FDD_CFG:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               veCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  veCfgGetParamVal(tagVal, paramLst, &noOfParams);
                 /* since fddCb ia single structure values are directly getting
                    mapped from the param list */
                  smCfgCb.fddCb.ulEarfcn = atoi((const S8*)paramLst[0]);
                  smCfgCb.fddCb.dlEarfcn = atoi((const S8*)paramLst[1]);
                  smCfgCb.fddCb.ulTxBndWdth = atoi((const S8*)paramLst[2]);
                  smCfgCb.fddCb.dlTxBndWdth = atoi((const S8*)paramLst[3]);
               }
               else
               {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Error in VE_TAG_CELL_FDD_CFG\
         tag format \n"));
#endif
               }
            }
            break;
         case VE_TAG_CELL_TDD_CFG:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               veCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  veCfgGetParamVal(tagVal, paramLst, &noOfParams);
                 /* since tddCb ia single structure values are directly getting
                    mapped from the array */
                  smCfgCb.tddCb.earfcn = atoi((const S8*)paramLst[0]);
                  smCfgCb.tddCb.txBndWdth = atoi((const S8*)paramLst[1]);
                  smCfgCb.tddCb.sfAssgnmnt = atoi((const S8*)paramLst[2]);
                  smCfgCb.tddCb.splSfPatrn = atoi((const S8*)paramLst[3]);
                  smCfgCb.tddCb.dlCycPrefix = atoi((const S8*)paramLst[4]);
                  smCfgCb.tddCb.ulCycPrefix = atoi((const S8*)paramLst[5]);
               }
               else
               {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Error in VE_TAG_CELL_TDD_CFG\
         tag format \n"));
#endif
               }
            }
            break;
         case VE_TAG_NO_OF_NGH_INFO:
            {
               smCfgCb.noOfNghInfo = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_NGH_INFO_CFG:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               veCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  veCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  VE_ALLOC(&smCfgCb.nghInfo,(noOfElem * sizeof(VeNghInfoCfg *)));
                  if(NULLP == smCfgCb.nghInfo)
                  {
#ifdef DEBUGP
                     VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Failed to allocate Memory.\n"));
#endif
                     RETVALUE(RFAILED);
                   }
                  paramCnt = 0;
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     VE_ALLOC(&nghInfoCfg, sizeof(VeNghInfoCfg));
                     if (NULLP != nghInfoCfg)
                     {
                        nghInfoCfg->ecgi.plmnId = atoi((const S8*)paramLst[paramCnt++]);
                        nghInfoCfg->ecgi.eutranCellId = atoi((const S8*)paramLst[paramCnt++]);
                        nghInfoCfg->physCellId = atoi((const S8*)paramLst[paramCnt++]);
                        nghInfoCfg->earfcn = atoi((const S8*)paramLst[paramCnt++]);
                     }
                     smCfgCb.nghInfo[loopCnt] = nghInfoCfg;
                  }/* End of for() */
               }/* End of if()*/
            }
            break;
         case VE_TAG_NO_OF_GU_GRP:
            {
               smCfgCb.guGrpId = (VeGuGrpIdCfg**) atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_GU_GRP_ID_CFG:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               veCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  veCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  VE_ALLOC(&smCfgCb.guGrpId,(noOfElem * sizeof(VeGuGrpIdCfg *)));
                  if(NULLP == smCfgCb.guGrpId)
                  {
#ifdef DEBUGP
                     VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Failed to allocate Memory.\n"));
#endif
                     RETVALUE(RFAILED);
                   } 
                  paramCnt = 0;
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     VE_ALLOC(&guGrpIdCfg, sizeof(VeGuGrpIdCfg));
                     if (NULLP != guGrpIdCfg)
                     {
                        guGrpIdCfg->plmnId = atoi((const S8*)paramLst[paramCnt++]);
                        guGrpIdCfg->mmeGrpId = atoi((const S8*)paramLst[paramCnt++]);
                     }
                     smCfgCb.guGrpId[loopCnt] = guGrpIdCfg;
                  }
               }
               else
               {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Error in VE_TAG_GU_GRP_ID_CFG\
         tag format \n"));
#endif
               }
            }
            break;
         case VE_TAG_S1_PREP_TMR:
            {
               smCfgCb.s1PrepTimerVal = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_S1_OVRALL_TMR:
            {
               smCfgCb.s1OvrAllTimerVal = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_NO_OF_NGH_CFG:
            {
               smCfgCb.noOfNghCfg = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_NGH_CELL_CFG:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               veCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  veCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  VE_ALLOC(&smCfgCb.veNghCellCfg,(noOfElem * sizeof(VeNghCellCfg *)));
                  if(NULLP == smCfgCb.veNghCellCfg)
                  {
#ifdef DEBUGP
                     VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Failed to allocate Memory.\n"));
#endif
                     RETVALUE(RFAILED);
                   }
                  
                  paramCnt = 0;
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     VE_ALLOC(&nghCellCfg, sizeof(VeNghCellCfg));
                     if (NULLP != nghCellCfg)
                     {
                        nghCellCfg->enbType = atoi((const S8*)paramLst[paramCnt++]);
                        cmInetAddr((S8*)paramLst[paramCnt++],&(nghCellCfg->enbAddr));
                        nghCellCfg->enbAddr = CM_INET_HTON_U32(nghCellCfg->enbAddr);
                        nghCellCfg->physCellId = atoi((const S8*)paramLst[paramCnt++]);
                        nghCellCfg->tac = atoi((const S8*)paramLst[paramCnt++]);
                        nghCellCfg->cellId = atoi((const S8*)paramLst[paramCnt++]);
                        nghCellCfg->plmnId = atoi((const S8*)paramLst[paramCnt++]);
                        nghCellCfg->peerId = atoi((const S8*)paramLst[paramCnt++]);
                        nghCellCfg->trgrX2 = atoi((const S8*)paramLst[paramCnt++]);
                     }
                     smCfgCb.veNghCellCfg[loopCnt] = nghCellCfg;
                  }
               }
               else
               {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Error in VE_TAG_NGH_CELL_CFG\
         tag format \n"));
#endif
               }
            }
            break;
         case VE_TAG_MEAS_CFG_ENB:
            {
               smCfgCb.suppMsrmntCfg = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_RSRP_THRSHLD_VAL:
            {
               smCfgCb.rsrpTrshldRange = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_NO_OF_NGH_ENB_CFG:
            {
               smCfgCb.noOfnghENBs = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_NGH_ENB_CFG:
            {
               status = TRUE;
               noOfElem = 0;
               noOfParams = 0;
               veCfgValdtInput(tagVal, &status, &noOfElem);
               if (TRUE == status)
               {
                  veCfgGetParamVal(tagVal, paramLst, &noOfParams);
                  VE_ALLOC(&smCfgCb.nghENBCfg, (noOfElem * sizeof(VeNghENBCfg *)));
                  if(NULLP == smCfgCb.nghENBCfg)
                  {
#ifdef DEBUGP
                     VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Failed to allocate Memory.\n"));
#endif
                     RETVALUE(RFAILED);
                  }
                  paramCnt = 0;
                  for (loopCnt = 0; loopCnt < noOfElem; loopCnt++)
                  {
                     VE_ALLOC(&nghENBCfg, sizeof(VeNghENBCfg));
                     if (NULLP != nghENBCfg)
                     {
                        nghENBCfg->peerId = atoi((const S8*)paramLst[paramCnt++]);
                        nghENBCfg->eAValue = atoi((const S8*)paramLst[paramCnt++]);
                        nghENBCfg->iPAValue = atoi((const S8*)paramLst[paramCnt++]);
                        nghENBCfg->priDstAddr = atoi((const S8*)paramLst[paramCnt++]);
                        nghENBCfg->dstPort = atoi((const S8*)paramLst[paramCnt++]);
                     }
                     smCfgCb.nghENBCfg[loopCnt] = nghENBCfg;
                  }
               }
               else
               {
#ifdef DEBUGP
         VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, " Error in VE_TAG_NGH_ENB_CFG\
         tag format \n"));
#endif
               }
            }
            break;
         case VE_TAG_PCI_VAL:
            {
               smCfgCb.pci = atoi((const S8*)tagVal); 
            }
            break;
         case VE_TAG_MAX_X2_PEERS:
            {
               smCfgCb.maxX2Peers = atoi((const S8*)tagVal);
            }
            break;

         case VE_TAG_X2_TIME_TO_WAIT:
            {
               smCfgCb.x2TimeToWaitTimerVal = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_UE_HO_ATTACH_TMR:
            {
               smCfgCb.ueHoAttachTimerVal = atoi((const S8*)tagVal);
            }
            break;
#endif
        case VE_TAG_MAX_RRM_RNTIS:
            { 
               smCfgCb.maxRrmRntis = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_MAC_RNTI_STRT:
            {
               smCfgCb.macRntiStart = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_MAX_MAC_RNTIS:
            {
               smCfgCb.maxMacRntis = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_RRM_NO_OF_DED_PREMBL:
            {
               smCfgCb.rrmNumDedPrm = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_RRM_DED_PREMBL_STRT:
            {
               smCfgCb.rrmDedPrmStart = atoi((const S8*)tagVal);
            }
            break;
         case VE_TAG_MAC_NO_OF_PREMBL:
            {
               smCfgCb.numOfMacPrm = atoi((const S8*)tagVal);
            }
            break;

         case VE_TAG_SCTP_SRVC_TYPE:
            {
               smCfgCb.sctpUdpServiceType = atoi((const S8*)tagVal);
            }
         break;   
       case VE_TAG_OP_MODE:
         {
               smCfgCb.opMode  = atoi((const S8*)tagVal);
         }
         break;
       case VE_TAG_COUNTER:
         {
               smCfgCb.counter = atoi((const S8*)tagVal);
         }
         break;
       case VE_TAG_PERIOD:
         {
               smCfgCb.period  = atoi((const S8*)tagVal);
         }
         break;
                 case VE_RRC_CON_TIMER:
                       {
                           smCfgCb.rrcConTmrVal = atoi((const S8*)tagVal);
                       }
                       break;

       case VE_TAG_ENB_SCTP_PORT:
         {
               smCfgCb.eNBsctpPortNum = atoi((const S8*)tagVal);
         }
         break;
       case VE_TAG_MME_SCTP_PORT:
         {
               smCfgCb.mmesctpPortNum = atoi((const S8*)tagVal);
         }
         break;

      }/* End of switch() */      
       tagNum = MAX_CFG_PARAM;
   }/* End of for() */

#ifdef DEBUGP
   VEDBGP(DBGMASK_SM, (smCb.init.prntBuf, "Read the ve_cfg.txt\
            file successfully\n"));
#endif
   fclose(fp);

   RETVALUE (ROK);
} /* end of smVeReadConfigParams */

/*
*
*       Fun:  rdConQ 
*
*       Desc:  
*
*       Ret:   ROK, RFAILED
*
*       File:  ve_sm_init.c
*
*/


#ifdef ANSI
PUBLIC S16 rdConQ
(
Data data
)
#else
PUBLIC S16 rdConQ(data)
Data data;
#endif
{
   U32  memAvail;

   /* ve002.102: Added trace macro */
   TRC2(rdConQ)

#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
   U32 memIndx;
#endif

   if ((data == 0x03) ||
       (data == 27))
   {
      printf("wrong key hit \n"); 
   }
   switch (data)
   {
      case 'm' :
      case 'M' :
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
         for(memIndx = 0; memIndx < 4/* (SS_MAX_REGS)*/; memIndx++)
         {
            SRegInfoShow(memIndx, &memAvail);
            memAvail = 0;
         }
#else
         SRegInfoShow(DFLT_REGION, &memAvail);
#endif
         break;
      case 'q' :
      case 'Q' :
         exit(0);
         break;

      default :
         break;
   }

   RETVALUE(ROK);
}

/********************************************************************30**
  
         End of file:     ve_sm_init.c@@/main/2 - Wed Dec  1 08:44:32 2010
  
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
