
/**
 * @file   sm_ptmi.c
 * @brief This file contains UEH stack manager coupling functions
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */

/* header include files (.h) */
#include "ccpu_cmn.h"

/* RRM Related Files */
#include "lueh.h"           /* UEH layer management */
#include "ueh.h"
#include "lueh.x"           /* ueh layer management */
#include "bsm_ueh_if.h"
#include "log.h"
#include "pst_events.h"
#include "ts.h"
#include "sm.h"

PRIVATE U8 uehUehCfg = 0;

#define  MAXUEHMI  2

#ifndef  LCSMUEHMILUEH
#define  PTSMUEHMILUEH
#else
#ifndef   UEH
#define  PTSMUEHMILUEH
#endif
#endif

#ifdef PTSMUEHMILUEH
PRIVATE S16 PtMiLuehCfgReq    ARGS((Pst *pst, UehMngmt *cfg));
PRIVATE S16 PtMiLuehCntrlReq  ARGS((Pst *pst, UehMngmt *cntrl));
#endif


/*
the following matrices define the mapping between the primitives
called by the layer management interface of TCP UDP Convergence Layer
and the corresponding primitives in TUCL
 
The parameter MAXUEHMI defines the maximum number of layer manager
entities on top of TUCL . There is an array of functions per primitive
invoked by TCP UDP Conbvergence Layer. Every array is MAXUEHMI long
(i.e. there are as many functions as the number of service users).

The dispatcysng is performed by the configurable variable: selector.
The selector is configured during general configuration.

The selectors are:

   0 - loosely coupled (#define LCSMUEHMILUEH) 2 - Lve (#define UEH)

*/


/* Configuration request primitive */
 
PRIVATE LuehCfgReq SmMiLuehCfgReqMt[MAXUEHMI] =
{
#ifdef LCSMUEHMILUEH
   cmPkLuehCfgReq,          /* 0 - loosely coupled  */
#else
   PtMiLuehCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef UEH
   UehMiLuehCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLuehCfgReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Control request primitive */
 
PRIVATE LuehCntrlReq SmMiLuehCntrlReqMt[MAXUEHMI] =
{
#ifdef LCSMUEHMILUEH
   cmPkLuehCntrlReq,          /* 0 - loosely coupled  */
#else
   PtMiLuehCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef UEH
   UehMiLuehCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLuehCntrlReq,          /* 1 - tightly coupled, portable */
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
*       File:     ueh_sm_ueh_ptmi.c
*
*/
  
PUBLIC S16 SmMiLuehCfgReq
(
Pst *spst,                /* post structure */
UehMngmt *cfg              /* configure */
)
{
   logFunc("%s","SmMiLuehCfgReq");
   /* jump to specific primitive depending on configured selector */
   (*SmMiLuehCfgReqMt[spst->selector])(spst, cfg); 
   RETVALUE(ROK);
} /* end of SmMiLuehCfgReq */



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
*       File:     ueh_sm_ueh_ptmi.c
*
*/
  

PUBLIC S16 SmMiLuehCntrlReq
(
Pst *spst,                 /* post structure */
UehMngmt *cntrl            /* control */
)
{
   logFunc("%s","SmMiLuehCntrlReq");
   /* jump to specific primitiueh depending on configured selector */
   (*SmMiLuehCntrlReqMt[spst->selector])(spst, cntrl); 
   RETVALUE(ROK);
} /* end of SmMiLuehCntrlReq */

#ifdef PTSMUEHMILUEH

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
*       File:     ueh_sm_ueh_ptmi.c
*
*/
  
PRIVATE S16 PtMiLuehCfgReq
(
Pst *spst,                  /* post structure */
UehMngmt *cfg                /* configure */
)
{
  logFunc("%s","PtMiLuehCfgReq");

  UNUSED(spst);
  UNUSED(cfg);

  RETVALUE(ROK);
} /* end of PtMiLuehCfgReq */


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
*       File:     ueh_sm_ueh_ptmi.c
*
*/
PRIVATE S16 PtMiLuehCntrlReq
(
Pst *spst,                  /* post structure */
UehMngmt *cntrl              /* control */
)
{
  logFunc("%s","PtMiLuehCntrlReq");

  UNUSED(spst);
  UNUSED(cntrl);

 RETVALUE(ROK);
} /* end of PtMiLuehCntrlReq */

#endif /* PTSMUEHMILHI */

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
*       File:  ueh_sm_ueh_ptmi.c
*
*/

PUBLIC S16 SmMiLuehCfgCfm(
  Pst     *pst,          /* post structure */
  UehMngmt *cfm           /* configuration */
  )
{
   Pst      uehPst;
   
   logFunc("%s","SmMiLuehCfgCfm");

   SM_SET_ZERO(&uehPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STUEHGEN:
            uehUehCfg |= UEH_UEH_GEN_CFG;
            break;      
         case STUEHCTFSAP:
            uehUehCfg |= UEH_UEH_CTF_SAP_CFG;
            break;
         case STUEHNHUSAP:
            uehUehCfg |= UEH_UEH_NHU_SAP_CFG;
            break;
         case STUEHRGRSAP:
            uehUehCfg |= UEH_UEH_RGR_SAP_CFG;
            break;
         case STUEHSZTSAP:
            uehUehCfg |= UEH_UEH_SZT_SAP_CFG;
            break;
         case STUEHUDHSAP:
            uehUehCfg |= UEH_UEH_UDH_SAP_CFG;
            break;

         default:
            logError("UEH Config confirm received with invalid element:%d",cfm->hdr.elmId.elmnt);
            break;
      }

      logInfo("UEH Config confirm received for element:%d",cfm->hdr.elmId.elmnt);

      if (uehUehCfg == UEH_SM_UEH_CONFIGURED)
      {
         logInfo("%s", "UEH CONFIGURATION SUCCESSFULLY PERFORMED");

         smCb.smState = UEH_SM_STATE_APP_CFG_DONE;
         UEH_FILL_PST(uehPst, SM_UEH_PROC, ENTSM, ENTUEH, EVTUEHRRCCFG, UEH_POOL, SM_SELECTOR_LC);
         uehSmSelfPstMsg(&uehPst);
      }
   }
   else
   {
     logError("Configuration Failure received from UEH for element:%d",cfm->hdr.elmId.elmnt);
   }
   RETVALUE(ROK);
}

/*
 *
 *       Fun:    SmMiLuehCntrlCfm - control confirm
 *
 *       Desc:   prints the control confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_ueh_ptmi.c
 *
 */

PUBLIC S16 SmMiLuehCntrlCfm(
    Pst         *pst,
    UehMngmt     *cntrl
  )
{
   Pst  uehPst;

   SM_SET_ZERO(&uehPst, sizeof(Pst));

   switch(cntrl->hdr.elmId.elmnt)
   {
     case STUEHCTFSAP:
     {
       logInfo("%s","BIND OF APP with CL(CTF) IS IN PROGRESS");
       break;
     }
     case STUEHSZTSAP:
     {
       logInfo("%s","Bind of RRM TSAP with S1AP IS IN PROGRESS");
       break;
     }
     case STUEHNHUSAP:
     {
       logInfo("%s","Bind of RRM TSAP with RRC IS IN PROGRESS");
       break;
     }
     case STUEHRGRSAP:
     {
       logInfo("%s","Bind of RRM TSAP with MAC IS IN PROGRESS");
       break;
     }
     case STUEHUDHSAP:
     {
       logInfo("%s","Bind of RRM TSAP with UDH IS IN PROGRESS");
       break;
     }
     case STUEHGEN:
     {
       if (cntrl->cfm.status == LCM_PRIM_OK)
       {
         switch(cntrl->u.cntrl.subAction)
         {
           case SADBG:
           {
             logInfo("%s", "Debug is enabled for UEH");
             smCb.cfgCb.smDbgMsk = (smCb.cfgCb.smDbgMsk | UEH_SM_UEH_DBG_MASK);
             break;
           }
           case SAUSTA:
           {
             logInfo("%s", "Debug is enabled for UEH");
             break;
           }
           default:
           {
             logError("General Control confirm received from UEH for unknown Subaction:%d reason:%d", cntrl->u.cntrl.subAction , cntrl->cfm.reason);
             break;
           }
         }
       }
       else
       {
         logError("General Control Fail received from UEH with, Action:%d, Subaction:%d",
                      cntrl->u.cntrl.action, cntrl->u.cntrl.subAction );
       }
       break;
     }
     case STUEHCELLCFG:
     {
       logInfo("%s", "Received Control Confirm for CELL CFG");
       break;
     }
     default:
     {
       if(cntrl->cfm.status == LCM_PRIM_OK)
       {
         logInfo("Control confirm received from:%d for Element:%d, Action:%d, Subaction:%d", pst->srcEnt,
             cntrl->hdr.elmId.elmnt, cntrl->u.cntrl.action, cntrl->u.cntrl.subAction );
       }
       else
       {
         logError("Control fail received from:%d for Element:%d, Action:%d, Subaction:%d", pst->srcEnt,
             cntrl->hdr.elmId.elmnt, cntrl->u.cntrl.action, cntrl->u.cntrl.subAction );
       }
       break;
     }
  }

  RETVALUE(ROK);
}

/*
 *
 *       Fun:    SmMiLuehStaInd - status indication
 *
 *       Desc:   Received the Status Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_ueh_ptmi.c
 *
 */

PUBLIC S16 SmMiLuehStaInd(
    Pst         *pst,
    UehMngmt      *usta
  )
{
   Pst  uehPst;
   UehUstaDgn *dgn;

   MsgUehBsmEnbConfigSucc  enbCfgSucc;
   MsgUehBsmEnbConfigFail  enbCfgFail;

   SM_SET_ZERO(&uehPst, sizeof(Pst));

   dgn = &(usta->u.usta.dgn);

   if ((usta->u.usta.alarm.event == LUEH_EVENT_CELL_CFG_SUCCESS)
      && smCb.smState == UEH_SM_STATE_AWAIT_CELL_UP
      )
   {
      smCb.smState = UEH_SM_STATE_CELL_UP;
      logInfo("%s","CELL CONFIGURATION SUCCESSFULLY PERFORMED");
#ifdef PWAV_TS
      /* Set time stamp querry from LA */
      uehSndCtfSetTsReq();
#endif
      enbCfgSucc.transId = smCb.enbTransId;
      /*Send a confirmation to BSM*/
      sendS1mMsgToBsm(EVT_CEM_BSM_CFG_SUCC, sizeof(enbCfgSucc), (U8*)&enbCfgSucc);
   } /* end of if statement */
   else if (usta->u.usta.alarm.category == LCM_CATEGORY_INTERFACE)
   {
      if (usta->u.usta.alarm.event == LCM_EVENT_BND_OK)
      {
         switch (usta->u.usta.alarm.cause)
         {
            case LUEH_CAUSE_SZT_SAP_BOUND:
            {
               logInfo("BIND OF RRM S1AP IS SUCCESSFUL sapId:%d", dgn->u.sapId);

               if (smCb.smState == UEH_SM_STATE_ENB_ALRMS_DONE)
               {
                 logInfo("%s","BIND TO S1AP STACK SUCCESSFUL");

                 smCb.smState = UEH_SM_STATE_S1AP_STK_BND_DONE;
                 /* Perform the bind of APP and EGTP */
                 UEH_FILL_PST(uehPst, SM_UEH_PROC, ENTSM, ENTUEH, EVTUEHEGTPSTKBND, UEH_POOL, SM_SELECTOR_LC);
                 if(uehSmSelfPstMsg(&uehPst) != ROK)
                 {
                   logError("%s", "Error in posting the message to SM for Binding RRM with EGTP");
                 }
                 else
                 {
                   logInfo("%s", "Sent the message to SM for Binding RRM with EGTP");
                 }
               }
               break;
            }
            case LUEH_CAUSE_CTF_SAP_BOUND:
            {
                logInfo("BIND OF APP CTF IS SUCCESSFUL sapId: %d", dgn->u.sapId);
                /* Initiate Bind Request towards NHU Sap  */
                smBindUehToNhuSap();
                break;
            }
            case LUEH_CAUSE_NHU_SAP_BOUND:
            {
               logInfo("BIND OF APP NHU IS SUCCESSFUL sapId:%d", dgn->u.sapId);
               /* Initiate Bind Request towards UDH Sap  */
               smBindUehToUdhSap();
               break;
            }
            case LUEH_CAUSE_UDH_SAP_BOUND: 
            {
               logInfo("BIND OF APP UDH IS SUCCESSFUL sapId:%d", dgn->u.sapId);
               /* Initiate Bind Request towards Rgr Sap  */
               smBindUehToRgrSap();
               break;
            }
            case LUEH_CAUSE_RGR_SAP_BOUND:
            {
               logInfo("BIND OF APP RGR IS SUCCESSFUL sapId:%d", dgn->u.sapId );
               if (smCb.smState == UEH_SM_STATE_EGTP_STK_BND_DONE)
               {
                  smCb.smState = UEH_SM_STATE_RRC_STK_BND_DONE;
                  logInfo("%s", "BIND OF RRC STACK IS SUCCESSFUL");
                  UEH_FILL_PST(uehPst, SM_UEH_PROC, ENTSM, ENTUEH, EVTUEHCELLCFG, UEH_POOL, SM_SELECTOR_LC);
                  if(uehSmSelfPstMsg(&uehPst) != ROK)
                  {
                    logError("%s", "Error in posting the message to SM for Initiating CELL CONFIG");
                  }
                  else
                  {
                    logInfo("%s", "Sent the message to SM for Initiating CELL CONFIG");
                  }
               }
               break;
            }
            default:
            {
              logError("invalid Alarm:%d received", usta->u.usta.alarm.cause);
              break;
            }
         }
      }
      else if (usta->u.usta.alarm.event == LCM_EVENT_BND_FAIL) 
      {
         logError("BIND OF RRM LSAP id:%d FAILED with Cause:%d Src Ent:%d", dgn->u.sapId, usta->u.usta.alarm.cause ,pst->srcEnt );
         enbCfgFail.transId = smCb.enbTransId;
         sendCemMsgToBsm(EVT_CEM_BSM_CFG_FAIL, sizeof(enbCfgFail), (U8*)&enbCfgFail);
      }
   }

   RETVALUE(ROK);
} /* end of SmMiLuehStaInd() */
