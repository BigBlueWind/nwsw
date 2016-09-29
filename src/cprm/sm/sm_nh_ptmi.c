/**
 * @file   sm_nh_ptmi.c
 * @brief This file contains Configuration functions for RRC
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */

#include "ccpu_cmn.h"

/* RRM Related Files */
#include "ueh.h"
#include "bsm_ueh_if.h"
#include "log.h"
#include "ueh_pmh_if.h"
#include "pst_events.h"
#include "sm.h"
PRIVATE U16 uehNhCfg = 0;
PRIVATE U16 numBinds = 0;
PRIVATE Bool bndInPrg = TRUE;

#define RRC_MAX_TRACE_CFM                    5 /* We have 5 SAP trace points */
#define RRC_TRC_RSPRCVD                      ( STNHKWUSAP + STNHCKWSAP + STNHCRGSAP + STNHCPJSAP + STNHPJUSAP )

PRIVATE U8 uehNhTrcCtrl = 0;
PRIVATE U8 uehNhTrcRspCount = 0;
S16 smGetRrcGenSts(
    NhMngmt         *sts,
    RrcGenCounters  *rsp
    );
S16 smGetRrcCellSts(
    NhMngmt         *sts,
    RrcCellCounters *rsp
    );


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
 *      File:  ueh_sm_nh_ptmi.c
 *
 *************************************************************************/


PUBLIC S16 SmMiLnhCfgReq
(
Pst *pst,                 /* post structure */
NhMngmt *cfg              /* configure */
)
{
   logFunc("%s","SmMiLnhCfgReq");
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
 *      File:  ueh_sm_nh_ptmi.c
 *
 *************************************************************************/
PUBLIC S16 SmMiLnhCntrlReq
(
Pst *pst,                 /* post structure */
NhMngmt *cntrl            /* control */
)
{
   logFunc("%s","SmMiLnhCntrlReq");
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
 *      File:  ueh_sm_nh_ptmi.c
 *
 *************************************************************************/

PRIVATE S16 PtMiLnhCfgReq
(
Pst *pst,                   /* post structure */
NhMngmt *cfg                /* configure */
)
{
  logFunc("%s","PtMiLnhCfgReq");

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
 *      File:  ueh_sm_nh_ptmi.c
 *
 *************************************************************************/

PRIVATE S16 PtMiLnhCntrlReq
(
Pst *pst,                   /* post structure */
NhMngmt *cntrl              /* control */
)
{
  logFunc("%s","PtMiLnhCntrlReq");

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
*       File:  ueh_sm_nh_ptmi.c
*
*/

PUBLIC S16 SmMiLnhCfgCfm(
  Pst     *pst,          /* post structure */
  NhMngmt *cfm           /* configuration */
  )
{
   Pst      nhPst;

   logFunc("%s","SmMiLnhCfgCfm");

   SM_SET_ZERO(&nhPst,  sizeof(Pst));
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
       switch (cfm->hdr.elmId.elmnt)
       {
          case STNHGEN:
               uehNhCfg |= UEH_NH_GEN_CFG;
               break;      
          case STNHPROT:
               uehNhCfg |= UEH_NH_PROT_CFG;
               break;
          case STNHUSAP:
               uehNhCfg |= UEH_NH_USAP_CFG;
               break;
          case STNHCKWSAP:
               uehNhCfg |= UEH_NH_CKW_SAP_CFG;
               break;
          case STNHKWUSAP:
               uehNhCfg |= UEH_NH_KWU_SAP_CFG;
               break;
          case STNHCRGSAP:
               uehNhCfg |= UEH_NH_CRG_SAP_CFG;
               break;
          case STNHCPJSAP:
               uehNhCfg |= UEH_NH_CPJ_SAP_CFG;
               break;
          case STNHPJUSAP:
               uehNhCfg |= UEH_NH_PJU_SAP_CFG;
               break;
          default:
               logError("RRC Cfg Cfm received with invalid element:%d", cfm->hdr.elmId.elmnt);
               break;
       }

       logInfo("RRC Config Confirm received with element:%d", cfm->hdr.elmId.elmnt);
       if (uehNhCfg == UEH_SM_NH_CONFIGURED)
       {
          logInfo("%s", "RRC CONFIGURATION SUCCESSFULLY PERFORMED");

          smCb.smState = UEH_SM_STATE_RRC_CFG_DONE;
          UEH_FILL_PST(nhPst, SM_NH_PROC, ENTSM, ENTUEH, EVTUEHPDCPCFG, UEH_NH_POOL, SM_SELECTOR_LC);
          if(uehSmSelfPstMsg(&nhPst) != ROK)
          {
            logError("%s", "Error in posting the message to SM for Configuring PDCP");
          }
          else
          {
            logInfo("%s", "Message to SM sent for Configuring PDCP");
          }
       }
   }
   else
   {
      logError("Configuration Failure received from RRC with element:%d", cfm->hdr.elmId.elmnt);
   }
   RETVALUE(ROK);
}


void uehNhProcTrcCtrlRsp(
    NhMngmt *cntrl,
    U8  *trcMsgId
    )
{
  uehNhTrcRspCount++;
  switch(cntrl->hdr.transId)
  {
    case SM_RRC_KWUSAP_START_TRACE_TRANS_ID:
    case SM_RRC_CKWSAP_START_TRACE_TRANS_ID:
    case SM_RRC_CRGSAP_START_TRACE_TRANS_ID:
    case SM_RRC_CPJSAP_START_TRACE_TRANS_ID:
    case SM_RRC_PJUSAP_START_TRACE_TRANS_ID:
    {
      if (cntrl->cfm.status == LCM_PRIM_OK)
      {
        *trcMsgId = EVT_SM_LTH_TRC_START_SUCC;
        uehNhTrcCtrl += cntrl->hdr.elmId.elmnt;
        logInfo("TRACE is ENABLED in RRC for Element:%d", cntrl->hdr.elmId.elmnt);
      }
      else
      {
        *trcMsgId = EVT_SM_LTH_TRC_START_FAIL;
        logError("TRACE ENABLE in RRC FAILED for Element:%d", cntrl->hdr.elmId.elmnt);
      }
     break;
    }
    case SM_RRC_KWUSAP_STOP_TRACE_TRANS_ID:
    case SM_RRC_CKWSAP_STOP_TRACE_TRANS_ID:
    case SM_RRC_CRGSAP_STOP_TRACE_TRANS_ID:
    case SM_RRC_CPJSAP_STOP_TRACE_TRANS_ID:
    case SM_RRC_PJUSAP_STOP_TRACE_TRANS_ID:
    {
      if (cntrl->cfm.status == LCM_PRIM_OK)
      {
        *trcMsgId = EVT_SM_LTH_TRC_STOP_SUCC;
        uehNhTrcCtrl += cntrl->hdr.elmId.elmnt;
        logInfo("TRACE is DISABLED in RRC for Element:%d", cntrl->hdr.elmId.elmnt);
      }
      else
      {
        *trcMsgId = EVT_SM_LTH_TRC_STOP_FAIL;
       logError("TRACE DISABLE in RRC FAILED for Element:%d", cntrl->hdr.elmId.elmnt);
      }
      break;
    }
  }
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
*       File:  ueh_sm_nh_ptmi.c
*
*/
PUBLIC S16 SmMiLnhCntrlCfm(
  Pst     *pst,          /* post structure */
  NhMngmt *cntrl         /* control */
  )
{
   Pst  uehPst;
   U8   trcMsgId;

   logFunc("%s","SmMiLnhCntrlCfm");

   SM_SET_ZERO(&uehPst, sizeof(Pst));

   if(cntrl->cfm.status == LCM_PRIM_OK)
   {
      switch (cntrl->hdr.elmId.elmnt)
      {
         case STNHGEN:
         {
           if (cntrl->cfm.status == LCM_PRIM_OK)
           {
             switch(cntrl->hdr.transId)
             {
               case SM_RRC_DEBUG_TRANS_ID:
               {
                 logInfo("%s", "Debug is enabled for RRC");
                 smCb.cfgCb.smDbgMsk = (smCb.cfgCb.smDbgMsk | UEH_SM_NH_DBG_MASK);
                 break;
               }
               case SM_RRC_ALARM_TRANS_ID:
               {
                 logInfo("%s", "ALARM is recieved from RRC");
                 break;
               }
               default:
               {
                 logError(" General Control confirm received from RRC for unknown Transaction Id :%d", cntrl->hdr.transId );
                 break;
               }
             }
           }
           else
           {
             logError("General Control Fail received from RRC with, Action:%d, Subaction:%d",
                          cntrl->u.cntrl.action, cntrl->u.cntrl.subAction );
           }
           break;
         }
         case STNHCRGSAP:
         {
           if( (SM_RRC_CRGSAP_START_TRACE_TRANS_ID == cntrl->hdr.transId ) ||
               (SM_RRC_CRGSAP_STOP_TRACE_TRANS_ID == cntrl->hdr.transId ) ) /* Trace Handling */
           {
             uehNhProcTrcCtrlRsp(cntrl, &trcMsgId);
           }
           else
           {
             logInfo("%s", "BIND OF RRC CRG SAP IS IN PROGRESS");
           }
           break;
         }
         case STNHCPJSAP:
         {
           if( (SM_RRC_CPJSAP_START_TRACE_TRANS_ID == cntrl->hdr.transId ) ||
               (SM_RRC_CPJSAP_STOP_TRACE_TRANS_ID == cntrl->hdr.transId )  ) /* Trace Handling */
           {
             uehNhProcTrcCtrlRsp(cntrl, &trcMsgId);
           }
           else
           {
             logInfo("%s", "BIND OF RRC CPJ SAP IS IN PROGRESS");
           }
           break;
         }
         case STNHPJUSAP:
         {
           if( (SM_RRC_PJUSAP_START_TRACE_TRANS_ID == cntrl->hdr.transId ) ||
               (SM_RRC_PJUSAP_STOP_TRACE_TRANS_ID == cntrl->hdr.transId ) ) /* Trace Handling */
           {
             uehNhProcTrcCtrlRsp(cntrl, &trcMsgId);
           }
           else
           {
             logInfo("%s", "BIND OF RRC PJU SAP IS IN PROGRESS");
           }
           break;
         }
         case  STNHKWUSAP:
         {
           if( (SM_RRC_KWUSAP_START_TRACE_TRANS_ID == cntrl->hdr.transId ) ||
               (SM_RRC_KWUSAP_STOP_TRACE_TRANS_ID == cntrl->hdr.transId ) ) /* Trace Handling */
           {
             uehNhProcTrcCtrlRsp(cntrl, &trcMsgId);
           }
           break;
         }
         case STNHCKWSAP:
         {
           if( (SM_RRC_CKWSAP_START_TRACE_TRANS_ID == cntrl->hdr.transId ) ||
               (SM_RRC_CKWSAP_STOP_TRACE_TRANS_ID == cntrl->hdr.transId ) ) /* Trace Handling */
           {
             uehNhProcTrcCtrlRsp(cntrl, &trcMsgId);
           }
           break;
         }
         default:
         {
           logError("Unknown control confirm received for element:%d",cntrl->hdr.elmId.elmnt);
           break;
         }

      }
   }


   /* We need Five replies from S1AP stack before sending response */
   if( uehNhTrcRspCount == RRC_MAX_TRACE_CFM )
   {
     MsgSmLthTraceCntrlCfm      trcCfm;
     if( RRC_TRC_RSPRCVD == uehNhTrcCtrl )
     {
       trcCfm.traceCtrlConfirm = TRUE;
     }
     else
     {
       trcCfm.traceCtrlConfirm = FALSE;
     }
     trcCfm.protocolName = TRACECONFIGMOD_RRC;
     if( RFAILED == sendSmMsgToLth( trcMsgId, sizeof(MsgSmLthTraceCntrlCfm), (U8*)&trcCfm) )
     {
       logError("%s", "Error in sending RRC TRACE CFM to BSM" );
     }
     else
     {
       logInfo("%s", "Sent RRC TRACE CFM to BSM" );
     }
     uehNhTrcRspCount = 0;
     uehNhTrcCtrl  = 0;
   }
   RETVALUE(ROK);
}

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
*       File:  ueh_sm_nh_ptmi.c
*
*/

PUBLIC S16 SmMiLnhStaCfm
(
Pst     *pst,          /* post structure */
NhMngmt *cfm           /* control */
)
{
   logFunc("%s","SmMiLnhStaCfm");

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
} /* end of SmMiLnhStaCfm */


S16 smGetRrcGenSts(
    NhMngmt        *sts,
    RrcGenCounters *rsp
    )
{
  if(sts->hdr.elmId.elmnt != STNHGEN)
  {
    logError("Gen STATS are handled, Unknown element received:%d", sts->hdr.elmId.elmnt);
    return RFAILED;
  }

  /* OAM RRC stats has the Index of size U32 as the first element, so we skip the first 4 bytes */
  cmMemcpy((U8 *)rsp + sizeof(U32), (CONSTANT U8 *)&(sts->u.sts.u.genSts), sizeof(NhGenSts) );

  return ROK;
}

S16 smGetRrcCellSts(
    NhMngmt         *sts,
    RrcCellCounters *rsp
    )
{
  if(sts->hdr.elmId.elmnt != STNHCELL)
  {
    logError("Cell STATS are handled, Unknown element received:%d", sts->hdr.elmId.elmnt);
    return RFAILED;
  }

  /* OAM RRC stats has the Index of size U32 as the first element, so we skip the first 4 bytes */
  cmMemcpy((U8 *)rsp + sizeof(U32), (CONSTANT U8 *)&(sts->u.sts.u.cellSts), sizeof(NhCellSts) );

  return ROK;
}

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
*       File:  ueh_sm_nh_ptmi.c
*
*/

PUBLIC S16 SmMiLnhStsCfm(
    Pst       *pst,         /* post structure */
    NhMngmt   *sts          /* confirmed statistics */
  )
{
  MsgSmPmhRrcGenStatRsp     rrcGenRsp;
  MsgSmPmhRrcCellStatRsp    rrcCellRsp;
  (void)cmMemset((U8 *) &(rrcGenRsp), 0, sizeof(MsgSmPmhRrcGenStatRsp));
  (void)cmMemset((U8 *) &(rrcCellRsp), 0, sizeof(MsgSmPmhRrcCellStatRsp));

  logInfo("%s","STATS Confirm received from RRC" );
  if( STNHGEN == sts->hdr.elmId.elmnt)
  {
    if(ROK != smGetRrcGenSts(sts,&rrcGenRsp.rrcGenCounters) )
    {
      logError("%s", "Error in Extracting received RRC General STATS Message");
      RETVALUE(RFAILED);
    }
    rrcGenRsp.transId = sts->hdr.transId;

    if( RFAILED == sendSmMsgToPmh( EVT_SM_PMH_RRC_GEN_STAT_RSP, sizeof(MsgSmPmhRrcGenStatRsp), (U8*)&rrcGenRsp) )
    {
        logError("%s", "Error in sending RRC General STATS to PMH" );
    }
    else
    {
        logInfo("%s", "Sent RRC General STATS to PMH" );
    }
  }
  else if(STNHCELL == sts->hdr.elmId.elmnt )
  {
    if(ROK != smGetRrcCellSts(sts,&rrcCellRsp.rrcCellCounters) )
    {
      logError("%s", "Error in Extracting received RRC Cell STATS Message");
      RETVALUE(RFAILED);
    }
    rrcCellRsp.transId = sts->hdr.transId;

    if( RFAILED == sendSmMsgToPmh( EVT_SM_PMH_RRC_CELL_STAT_RSP, sizeof(MsgSmPmhRrcCellStatRsp), (U8*)&rrcCellRsp) )
    {
        logError("%s", "Error in sending RRC Cell STATS to PMH" );
    }
    else
    {
        logInfo("%s", "Sent RRC Cell STATS to PMH" );
    }
  }
  else
  {
    logError("Unknown Stats of type:%d received from RRC", sts->hdr.elmId.elmnt );
  }


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
*       File:  ueh_sm_nh_ptmi.c
*
*/

PUBLIC S16 SmMiLnhStaInd(
  Pst     *pst,           /* post structure */
  NhMngmt *usta           /* unsolicited status */
  )
{
   NhUstaDgn *dgn;

   logFunc("%s","SmMiLnhStaInd");

   dgn = &usta->u.usta.dgn;

   switch(usta->u.usta.alarm.category)
   {
      case LCM_CATEGORY_INTERFACE:
       {
          switch(usta->u.usta.alarm.event)
          {
             case LCM_EVENT_BND_OK:
              {
                 if (FALSE == bndInPrg)
                 {
                   logInfo("BIND OF RRC (NHU) Usap id:%d IS SUCCESSFUL", dgn->u.sapInfo.sapId);
                   break;
                 }

                 numBinds ++;
                 switch (numBinds)
                 {
                    case 1:
                    {
                       logInfo("BIND OF RRC CKW lsap id:%d IS SUCCESSFUL", dgn->u.sapInfo.sapId);
                       smBindNhToCpjSap();
                      break;
                    }
                    case 2:
                    {
                      logInfo("BIND OF RRC CPJ lsap id:%d IS SUCCESSFUL", dgn->u.sapInfo.sapId);
                       smBindNhToPjuSap();
                      break;
                    }
                    case 3:
                    {
                      logInfo("BIND OF RRC PJU lsap id:%d IS SUCCESSFUL", dgn->u.sapInfo.sapId);
                      smBindNhToCrgSap();
                      break;
                    }
                    case 4:
                    {
                      logInfo("BIND OF RRC CRG lsap id:%d IS SUCCESSFUL", dgn->u.sapInfo.sapId);
                      smBindNhToKwuSap();
                      break;
                    }
                    case 5:
                    {
                      logInfo("BIND OF RRC KWU lsap id:%d IS SUCCESSFUL", dgn->u.sapInfo.sapId);
                      bndInPrg = FALSE;
                      smBindUehToCtfSap();
                      break;
                    }
                    default:
                    {
                      logError("Multiple Alarms received only five expected, received for SAP:%d\n", dgn->u.sapInfo.sapId);
                      break;
                    }
                 }
                 break;
              }
             case LCM_EVENT_BND_FAIL:
             {
               logError("BIND OF RRC lsap id:%d FAILED", dgn->u.sapInfo.sapId);
               break;
             }
             default:
             {
               logError("Unknown alarm event:%d received from RRC", usta->u.usta.alarm.event);
               break;
             }
          }
             break;
       }

      case LCM_CATEGORY_RESOURCE:
         logError("RESOURCE Alarm from RRC|event=%d|cause=%d", usta->u.usta.alarm.event, usta->u.usta.alarm.cause);
         break;
 
      default:
      {
        logError("Unknown alarm Category:%d received from RRC", usta->u.usta.alarm.category);
        break;
      }
   }

   RETVALUE(ROK);
} /* end of SmMiLnhStaInd */

/**************************************************************************
 *
 *      Fun:  SmMiLnhTrcInd (Portable)
 *
 *      Desc: Used to indicate the trace of the RRC layer to the layer manager.
 *
 *      Ret:  ROK - OK
 *
 *      Notes:None
 *
 *      File:  nh_ptmi.c
 *
 *************************************************************************/
PUBLIC S16 SmMiLnhTrcInd(
  Pst           *pst,                 /* Post structure */
  NhMngmt       *trc              /* Trace */
  )
{
  MsgSmLthTrcInd     msgSmLthTrcInd;

  /* CCPU trace structure for RRC Contains RRC PDU with in the message
   */
  logInfo("%s","Trace Indication received from RRC" );

  SM_SET_ZERO( &msgSmLthTrcInd, sizeof(MsgSmLthTrcInd));
  if( trc->u.trc.len >= MAX_LEN_TRACE_MSG )
  {
    logError("Jumbo Trace packet received from RRC of length:%d", trc->u.trc.len);
    msgSmLthTrcInd.traceLen = MAX_LEN_TRACE_MSG;
  }
  else
  {
    msgSmLthTrcInd.traceLen = trc->u.trc.len;
  }

  msgSmLthTrcInd.protocolName = TRACECONFIGMOD_RLC;
  if( msgSmLthTrcInd.traceLen > 0)
  {
    cmMemcpy((U8 *) &(msgSmLthTrcInd.traceMsg), (CONSTANT U8 *)&(trc->u.trc.evntParm), msgSmLthTrcInd.traceLen );
    clock_gettime(CLOCK_REALTIME, &(msgSmLthTrcInd.timeVal));
    if( RFAILED == sendSmMsgToLth( EVT_SM_LTH_TRC_IND, sizeof(MsgSmLthTrcInd), (U8*)&msgSmLthTrcInd) )
    {
      logError("%s", "Error in sending RRC TRACE IND to LTH" );
    }
    else
    {
      logInfo("%s", "Sent RRC TRACE IND to LTH" );
    }

  }

  RETVALUE(ROK);
}

#ifdef __cplusplus
}
#endif /* cplusplus */
