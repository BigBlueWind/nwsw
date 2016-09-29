/**
 * @file  sm_pj_ptmi.c
 * @brief This file contains layer management functions for PDCP
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
#include "lth_sm_if.h"
#include "pst_events.h"

PRIVATE U8  uehPjCfg = 0;
PRIVATE U8  uehPjUsapCfgCnt = 0;
S16 smGetPdcpGenSts(
    PjMngmt         *sts,
    PdcpGenCounters *rsp
    );
S16 smGetPdcpUplaneSts(
    PjMngmt             *sts,
    PdcpUplaneCounters  *rsp
    );

#if !(defined(LCPJMILPJ) && defined(PJ))
#define PTSMMILPJ
#endif

/* MAX Number of Service Providers of SM */
#define MAXPJMI 2


#ifdef PTSMMILPJ
EXTERN S16 PtMiLpjCfgReq   ARGS((Pst * pst, PjMngmt * cfg));
EXTERN S16 PtMiLpjCntrlReq ARGS((Pst * pst, PjMngmt * cfg));
#endif /*--#ifdef PTSMMILTF--*/

#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */

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
 *      File:  ueh_sm_pj_ptmi.c
 *
 *************************************************************************/

PUBLIC S16 SmMiLpjCfgReq
(
Pst * pst,
PjMngmt * cfg
)
{

   logFunc("%s","SmMiLpjCfgReq");

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

PUBLIC S16 SmMiLpjCntrlReq
(
Pst * pst,
PjMngmt * cfg
)
{

   logFunc("%s","SmMiLpjCntrlReq");

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
 *      File:  ueh_sm_pj_ptmi.c
 *
 *************************************************************************/

PUBLIC S16 PtMiLpjCfgReq
(
Pst * pst,
PjMngmt * cfg
)
{

   logFunc("%s","PtMiLpjCfgReq");

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
 *      File:  ueh_sm_pj_ptmi.c
 *
 *************************************************************************/

PUBLIC S16 PtMiLpjCntrlReq
(
Pst * pst,
PjMngmt * cfg
)
{

   logFunc("%s","PtMiLpjCntrlReq");

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
*       File:  ueh_sm_pj_ptmi.c
*
*/
PUBLIC S16 SmMiLpjCfgCfm(
  Pst            *pst,
  PjMngmt        *cfm
  )
{
   Pst      pjPst;

   logFunc("%s","SmMiLpjCfgCfm");

   SM_SET_ZERO(&pjPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            uehPjCfg |= UEH_PJ_GEN_CFG;
            break;      
         case STCPJSAP:
            uehPjCfg |= UEH_PJ_CPJ_SAP_CFG;
            break;
         case STPJUSAP:
            uehPjCfg |= UEH_PJ_PJU_SAP_CFG;
            uehPjUsapCfgCnt++;
            break;
         default:
            logError("PDCP Config Confirm received with invalid element:%d", cfm->hdr.elmId.elmnt);
            break;
      } /* end of switch statement */
      logInfo("PDCP Config Confirm received from element:%d", cfm->hdr.elmId.elmnt);

      if (uehPjCfg == UEH_SM_PJ_CONFIGURED && uehPjUsapCfgCnt == 2)
      {
         logInfo("%s", "PDCP CONFIGURATION SUCCESSFULLY PERFORMED");

         smCb.smState = UEH_SM_STATE_PDCP_CFG_DONE;
         UEH_FILL_PST(pjPst, SM_PJ_PROC, ENTSM, ENTUEH, EVTUEHRLCCFG, UEH_POOL, SM_SELECTOR_LC);
         if(uehSmSelfPstMsg(&pjPst) != ROK)
         {
           logError("%s", "Error in posting the message to SM for Configuring RLC");
         }
         else
         {
           logInfo("%s", "Sent the message to SM for Configuring RLC");
         }
      }
   }
   else
   {
     logError("Configuration Failure received from PDCP with element:%d", cfm->hdr.elmId.elmnt);
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
*       File:  ueh_sm_pj_ptmi.c
*
*/
PUBLIC S16 SmMiLpjCntrlCfm(
  Pst            *pst,
  PjMngmt        *cntrl
  )
{
  MsgSmLthTraceCntrlCfm      trcCfm;
  U16                          sendMsgToLth = FALSE;
  U8                           trcMsgId;

  switch(cntrl->hdr.elmId.elmnt)
  {
    case STGEN:
    {
      switch(cntrl->hdr.transId)
      {
        case SM_PDCP_START_TRACE_TRANS_ID:
        {
        	sendMsgToLth = TRUE;
          if (cntrl->cfm.status == LCM_PRIM_OK)
          {
         trcMsgId = EVT_SM_LTH_TRC_START_SUCC;
         logError("%s","senging trace start succ from PDCP\n");
             logInfo("%s", "TRACE is ENABLED in PDCP");
             trcCfm.traceCtrlConfirm = TRUE;
           }
           else
           {
         trcMsgId = EVT_SM_LTH_TRC_START_FAIL;
             logInfo("%s", "TRACE is ENABLED in PDCP");
             trcCfm.traceCtrlConfirm = FALSE;
             logError("%s", "TRACE ENABLE in PDCP FAILED");
           }
          break;
        }
        case SM_PDCP_STOP_TRACE_TRANS_ID:
        {
        	sendMsgToLth = TRUE;
          if (cntrl->cfm.status == LCM_PRIM_OK)
          {
         trcMsgId = EVT_SM_LTH_TRC_STOP_SUCC;
            trcCfm.traceCtrlConfirm = TRUE;
            logInfo("%s", "TRACE is DISABLED in PDCP");
          }
          else
          {
         trcMsgId = EVT_SM_LTH_TRC_STOP_FAIL;
            trcCfm.traceCtrlConfirm = FALSE;
            logError("%s", "TRACE DISABLE in PDCP FAILED");
          }
          break;
        }
        default:
        {
          logError("Control Response received from PDCP for  Element:STGEN, Action:%d, Subaction:%d",
              cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
          break;
        }
      } /* End of Switch */

      trcCfm.protocolName = TRACECONFIGMOD_PDCP;
      if( TRUE == sendMsgToLth )
      {
        if( RFAILED == sendSmMsgToLth( trcMsgId, sizeof(MsgSmLthTraceCntrlCfm), (U8*)&trcCfm) )
        {
          logError("%s", "Error in sending PDCP TRACE CFM to BSM" );
        }
        else
        {
          logInfo("%s", "Sent PDCP TRACE CFM  to BSM" );
        }
      }
      break;
    } /* STGEN */
    default:
    {
      if(cntrl->cfm.status == LCM_PRIM_OK)
      {
        logInfo("Control confirm received from PDCP for Element:%d, Action:%d, Subaction:%d",
            cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
      }
      else
      {
        logError("Control fail received from PDCP for Element:%d, Action:%d, Subaction:%d",
            cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
      }
      break;
    }
  }

   RETVALUE(ROK);
} /* end of SmMiLpjCntrlCfm */

PUBLIC S16 SmMiLpjTrcInd(
    Pst         *pst,
    PjMngmt     *trcInd,
    Buffer      *mBuf
  )
{
  MsgLen               trcLen;
  MsgSmLthTrcInd     msgSmLthTrcInd;

  /* CCPU trace structure for PDCP is |RgMngmt HDR|PduLen|MacPdu
   * Copy the PDU to  msgPdcpLthTrcInd,
   * Add Time stamp*/
  logInfo("%s","Trace Indication received from PDCP");

  SM_SET_ZERO( &msgSmLthTrcInd, sizeof(MsgSmLthTrcInd));
  SFndLenMsg(mBuf, &trcLen );
  if( trcLen >= MAX_LEN_TRACE_MSG )
  {
    logError("Jumbo Trace packet of length:%d received from PDCP", trcLen);
    trcLen = MAX_LEN_TRACE_MSG;
  }
  msgSmLthTrcInd.traceLen = trcLen;
  msgSmLthTrcInd.protocolName = TRACECONFIGMOD_PDCP;

  if( trcLen > 0 )
  {
    SRemPreMsgMult((Data *)&(msgSmLthTrcInd.traceMsg), trcLen, mBuf);
    clock_gettime(CLOCK_REALTIME, &(msgSmLthTrcInd.timeVal));
    if( RFAILED == sendSmMsgToLth( EVT_SM_LTH_TRC_IND, sizeof(MsgSmLthTrcInd), (U8*)&msgSmLthTrcInd) )
    {
      logError("%s", "Error in sending PDCP TRACE IND to LTH" );
    }
    else
    {
      logInfo("%s", "Sent PDCP TRACE IND to LTH" );
    }
  }

  RETVALUE(ROK);
}


S16 smGetPdcpGenSts(
    PjMngmt         *sts,
    PdcpGenCounters *rsp
    )
{
  if(sts->hdr.elmId.elmnt != STGEN)
  {
    logError("Gen STATS are handled, Unknown element received:%d", sts->hdr.elmId.elmnt);
    return RFAILED;
  }
  
  /* OAM PDCP stats has the Index of size U32 as the first element, so we skip the first 4 bytes */
  cmMemcpy((U8 *) rsp+ sizeof(U32), (CONSTANT U8 *)&(sts->t.sts.s.gen), sizeof(PjGenSts) );
  return ROK;
}

S16 smGetPdcpUplaneSts(
    PjMngmt             *sts,
    PdcpUplaneCounters  *rsp
    )
{
  if(sts->hdr.elmId.elmnt != STPJUSAP)
  {
    logError("Pju STATS are handled, Unknown element received:%d", sts->hdr.elmId.elmnt);
    return RFAILED;
  }

  rsp->txSdus = sts->t.sts.s.pjuSap.txSdus;
  rsp->rxSdus = sts->t.sts.s.pjuSap.rxSdus;

  return ROK;
}

PUBLIC S16 SmMiLpjStsCfm(
    Pst       *pst,         /* post structure */
    Action     action,
    PjMngmt   *sts          /* confirmed statistics */
  )
{
  MsgSmPmhPdcpGenStatRsp       pdcpGenStatRsp;
  MsgSmPmhPdcpUplaneStatRsp    pdcpUplaneStatRsp;

  (void)cmMemset((U8 *) &(pdcpGenStatRsp), 0, sizeof(MsgSmPmhPdcpGenStatRsp));
  (void)cmMemset((U8 *) &(pdcpUplaneStatRsp), 0, sizeof(MsgSmPmhPdcpUplaneStatRsp));

  logFunc("%s","SmMiLpjStsCfm");

  logInfo("%s","STATS Confirm received from PDCP" );

  if( STGEN == sts->hdr.elmId.elmnt)
  {
    if(ROK != smGetPdcpGenSts(sts, &pdcpGenStatRsp.pdcpGenCounters) )
    {
      logError("%s", "Error in Extracting received PDCP General STATS Message");
      RETVALUE(RFAILED);
    }
    pdcpGenStatRsp.transId = sts->hdr.transId;

    if( RFAILED == sendSmMsgToPmh( EVT_SM_PMH_PDCP_GEN_STAT_RSP, sizeof(MsgSmPmhPdcpGenStatRsp), (U8*)&pdcpGenStatRsp) )
    {
        logError("%s", "Error in sending PDCP General STATS to PMH" );
    }
    else
    {
        logInfo("%s", "Sent PDCP General STATS to PMH" );
    }
  }
  else if(STPJUSAP == sts->hdr.elmId.elmnt )
  {
    if(ROK != smGetPdcpUplaneSts(sts,&pdcpUplaneStatRsp.pdcpUplaneCounters) )
    {
      logError("%s", "Error in Extracting received PDCP STPJUSAP STATS Message");
      RETVALUE(RFAILED);
    }
    pdcpUplaneStatRsp.transId = sts->hdr.transId;

    if( RFAILED == sendSmMsgToPmh( EVT_SM_PMH_PDCP_UPLANE_STAT_RSP, sizeof(MsgSmPmhPdcpUplaneStatRsp), (U8*)&pdcpUplaneStatRsp) )
    {
        logError("%s", "Error in sending PDCP STPJUSAP STATS to PMH" );
    }
    else
    {
        logInfo("%s", "Sent PDCP STPJUSAP STATS to PMH" );
    }
  }
  else
  {
    logError("Unknown Stats of type:%d received from PDCP", sts->hdr.elmId.elmnt );
  }


   RETVALUE(ROK);
} /* end of SmMiLnhStsCfm */


#ifdef __cplusplus
}
#endif /* cplusplus */
