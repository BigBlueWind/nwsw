/**
 * @file   sm_rg_ptmi.c
 * @brief This file contains management interface functions for MAC
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
#include "log.h"
#include "bsm_ueh_if.h"
#include "ueh_pmh_if.h"
#include "lth_sm_if.h"
#include "pst_events.h"

PRIVATE U8 uehRgCfg = 0;
S16 smGetMacGenSts(
    RgMngmt        *sts,
    MacGenCounters *rsp
    );
S16 smGetMacRlcSts(
    RgMngmt           *sts,
    MacRlcsapCounters *rsp
    );
S16 smGetMacPhySts(
    RgMngmt           *sts,
    MacPhysapCounters *rsp
    );

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
 *      File:  ueh_sm_rg_ptmi.c
 *
 *************************************************************************/

PUBLIC S16 SmMiLrgCfgReq
(
Pst * pst,
RgMngmt * cfg
)
{

   logFunc("%s","SmLiLrgCfgReq");

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
 *      File:  ueh_sm_rg_ptmi.c
 *
 *************************************************************************/

PUBLIC S16 SmMiLrgSchCfgReq
(
Pst * pst,
RgMngmt * cfg
)
{

   logFunc("%s","SmLiLrgSchCfgReq");

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
 *      File:  ueh_sm_rg_ptmi.c
 *
 *************************************************************************/

PUBLIC S16 SmMiLrgCntrlReq
(
Pst * pst,
RgMngmt * cntrl
)
{

   logFunc("%s","SmMiLrgCntrlReq");

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
 *      File:  ueh_sm_rg_ptmi.c
 *
 *************************************************************************/

PUBLIC S16 SmMiLrgSchCntrlReq
(
Pst * pst,
RgMngmt * cntrl
)
{

   logFunc("%s","SmMiLrgSchCntrlReq");

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
 *      File:  ueh_sm_rg_ptmi.c
 *
 *************************************************************************/

PUBLIC S16 PtLiLrgCfgReq
(
Pst * pst,
RgMngmt * cfg
)
{

   logFunc("%s","PtLiLrgCfgReq");

   UNUSED(pst);
   UNUSED(cfg);

   RETVALUE(ROK);

}


PUBLIC S16 PtLiLrgSchCfgReq
(
Pst * pst,
RgMngmt * cfg
)
{

   logFunc("%s","PtLiLrgSchCfgReq");

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
 *      File:  ueh_sm_rg_ptmi.c
 *
 *************************************************************************/

PUBLIC S16 PtLiLrgCntrlReq
(
Pst * pst,
RgMngmt * cntrl
)
{

   logFunc("%s","PtLiLrgCntrlReq");

   UNUSED(pst);
   UNUSED(cntrl);

   RETVALUE(ROK);

}

PUBLIC S16 PtLiLrgSchCntrlReq
(
Pst * pst,
RgMngmt * cntrl
)
{

   logFunc("%s","PtLiLrgSchCntrlReq");

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
*       File:  ueh_sm_rg_ptmi.c
*
*/


PUBLIC S16 SmMiLrgCfgCfm(
  Pst            *pst,
  RgMngmt        *cfm
  )
{
   Pst      rgPst;

   logFunc("%s","SmMiLrgCfgCfm");

   SM_SET_ZERO(&rgPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            uehRgCfg |= UEH_RG_GEN_CFG;
            break;      
         case STRGUSAP:
            uehRgCfg |= UEH_RG_RGU_SAP_CFG;
            break;
         case STCRGSAP:
            uehRgCfg |= UEH_RG_CRG_SAP_CFG;
            break;
         case STSCHINST:
            uehRgCfg |= UEH_RG_SCH_INS_CFG;
            break;
         case STTFUSAP:
            uehRgCfg |= UEH_RG_TFU_SAP_CFG;
            break;
         default:
            logError("MAC Config Confirm received with invalid element:%d", cfm->hdr.elmId.elmnt);
            break;
      } /* end of switch statement */
      logInfo("MAC Config Confirm received for element:%d", cfm->hdr.elmId.elmnt);
      if (uehRgCfg == UEH_SM_RG_CONFIGURED)
      {
         logInfo("%s", "MAC & SCH CONFIGURATION SUCCESSFULLY PERFORMED");

         smCb.smState = UEH_SM_STATE_MAC_CFG_DONE;

         UEH_FILL_PST(rgPst, SM_RG_PROC, ENTSM, ENTUEH, EVTUEHCLCFG, UEH_POOL, SM_SELECTOR_LC);

         if(uehSmSelfPstMsg(&rgPst) != ROK)
         {
           logError("%s", "Error in posting the message to SM for Configuring CL");
         }
         else
         {
           logInfo("%s", "Sent the message to SM for Configuring CL");
         }
      }
   }
   else
   {
     logError("Configuration Failure received from MAC with element:%d, reason:%d", cfm->hdr.elmId.elmnt, cfm->cfm.reason);
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
*       File:  ueh_sm_rg_ptmi.c
*
*/


PUBLIC S16 SmMiLrgSchCfgCfm(
  Pst            *pst,
  RgMngmt        *cfm
  )
{
   Pst      rgPst;

   logFunc("%s","SmMiLrgSchCfgCfm");
   SM_SET_ZERO(&rgPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STSCHINST:
            uehRgCfg |= UEH_RG_SCH_INS_CFG;
            break;
         default:
            logError("MAC-SCH Config Confirm received with invalid element:%d", cfm->hdr.elmId.elmnt);
            break;
      }
      logInfo("MAC-SCH Config Confirm received for element:%d", cfm->hdr.elmId.elmnt);
      if (uehRgCfg == UEH_SM_RG_CONFIGURED)
      {
         logInfo("%s", "MAC & SCH CONFIGURATION SUCCESSFULLY PERFORMED");

         smCb.smState = UEH_SM_STATE_MAC_CFG_DONE;

         UEH_FILL_PST(rgPst, SM_RG_PROC, ENTSM, ENTUEH, EVTUEHCLCFG, UEH_POOL, SM_SELECTOR_LC);

         if(uehSmSelfPstMsg(&rgPst) != ROK)
         {
           logError("%s", "Error in posting the message to SM for Configuring CL");
         }
         else
         {
           logInfo("%s", "Sent the message to SM for Configuring CL");
         }
      }
   }
   else
   {
     logError("Configuration Failure received from MAC-SCH with element:%d, reason:%d", cfm->hdr.elmId.elmnt, cfm->cfm.reason);
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
*       File:  ueh_sm_rg_ptmi.c
*
*/

PUBLIC S16 SmMiLrgCntrlCfm(
  Pst            *pst,
  RgMngmt        *cntrl
  )
{
   Pst                          rgPst;
   MsgSmLthTraceCntrlCfm      trcCfm;
   logFunc("%s","SmMiLrgCntrlCfm");

   SM_SET_ZERO(&rgPst, sizeof(Pst));

   switch(cntrl->hdr.elmId.elmnt)
   {
     case STTFUSAP: /* MAC binds only with TFU SAP, for STRGUSAP and STCRGSAP only delete is allowed */
     {
        if(AUBND == cntrl->t.cntrl.action)
        {
          if (cntrl->cfm.status == LCM_PRIM_OK)
          {
            logInfo("%s","UNBIND OF MAC WITH CL SUCCESSFUL");
          }
        }
        else
        {
          if (cntrl->cfm.status == LCM_PRIM_OK)
          {
            logInfo("%s","BIND OF MAC WITH CL SUCCESSFUL");
            logInfo("%s", "Sending control request for binding MAC-SCH with TFU");
            smBindRgSchToTfuSap();
          }
          else if (cntrl->cfm.status == LCM_PRIM_OK_NDONE)
          {
            logInfo("%s", "BIND OF MAC WITH CL IS IN PROGRESS");
          }
          else
          {
            logError("%s", "BIND OF MAC WITH CL FAILED");
          }
        }
        break;
     }
     case STGEN:
     {
       if( SATRC == cntrl->t.cntrl.subAction ) /* Trace Handling */
       {
          if( AENA == cntrl->t.cntrl.action )
          {
            if (cntrl->cfm.status == LCM_PRIM_OK)
            {
              trcCfm.traceCtrlConfirm = TRUE;
              logInfo("%s", "TRACE is ENABLED in MAC");
            }

            else
            {
              trcCfm.traceCtrlConfirm = FALSE;
              logInfo("TRACE ENABLE in MAC FAILED with reason = %d",cntrl->cfm.reason);
            }
          }
          else if (ADISIMM == cntrl->t.cntrl.action )
          {
            if (cntrl->cfm.status == LCM_PRIM_OK)
            {
              trcCfm.traceCtrlConfirm = TRUE;
              logError("%s", "TRACE is DISABLED in MAC");
            }
            else
            {
              trcCfm.traceCtrlConfirm = FALSE;
              logError("%s", "TRACE DISABLE in MAC FAILED");
            }
          }
          if( RFAILED == sendSmMsgToLth( EVT_SM_LTH_TRC_START_SUCC, sizeof(MsgSmLthTraceCntrlCfm), (U8*)&trcCfm) )
          {
            logError("%s", "Error in sending MAC TRACE CFM to BSM" );
          }
          else
          {
            logInfo("%s", "Sent MAC TRACE CFM  to BSM" );
          }

       }
       else if( SADBG == cntrl->t.cntrl.subAction ) /* Debug Handling */
       {
         if (cntrl->cfm.status == LCM_PRIM_OK)
         {
            logInfo("%s", "Debug is enabled for MAC");
            smCb.cfgCb.smDbgMsk = (smCb.cfgCb.smDbgMsk | UEH_SM_RG_DBG_MASK);
         }
         else
         {
           logError("General Control Fail received from MAC with, Action:%d, Subaction:%d",
                        cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
         }
       }
       break;
     }
     default:
     {
       if(cntrl->cfm.status == LCM_PRIM_OK)
       {
         logInfo("Control confirm received from MAC for Element:%d, Action:%d, Subaction:%d",
             cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
       }
       else
       {
         logError("Control fail received from MAC for Element:%d, Action:%d, Subaction:%d",
             cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
       }
       break;
     }
   }

   RETVALUE(ROK);
}

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
*       File:  ueh_sm_rg_ptmi.c
*
*/

PUBLIC S16 SmMiLrgSchCntrlCfm(
  Pst            *pst,
  RgMngmt        *cntrl
  )
{
   Pst  rgPst;

   logFunc("%s","SmMiLrgSchCntrlCfm");

   SM_SET_ZERO(&rgPst, sizeof(Pst));

   if (cntrl->hdr.elmId.elmnt == STTFUSAP)
   {
      if (cntrl->cfm.status == LCM_PRIM_OK)
      {
        logInfo("%s", "BIND OF SCH WITH CL IS SUCCESSFULL");
        logInfo("%s", "Sending control request for binding RLC with MAC");
        smBindKwToRguSap();
      }
      else if (cntrl->cfm.status == LCM_PRIM_OK_NDONE)
      {
        logInfo("%s", "BIND OF SCH WITH CL IS IN PROGRESS");
      }
      else 
      {
        logError("%s", "BIND OF SCH WITH CL FAILED");
      }
   }
   RETVALUE(ROK);
}



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
*       File:  ueh_sm_rg_ptmi.c
*
*/

PUBLIC S16 SmMiLrgStaInd
(
Pst            *pst,
RgMngmt        *staInd
)
{
   logFunc("%s","SmMiLrgStaInd");

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
*       File:  ueh_sm_rg_ptmi.c
*
*/

PUBLIC S16 SmMiLrgTrcInd (
  Pst            *pst,
  RgMngmt        *trcInd,
  Buffer         *mBuf
  )
{
  MsgLen                trcLen;
  MsgSmLthTrcInd     msgMacLthTrcInd;

  /* CCPU trace structure for MAC is |RgMngmt HDR|PduLen|MacPdu
   * Copy the PDU to  msgMacLthTrcInd,
   * Add Time stamp*/
  logInfo("%s","Trace Indication received from MAC");

  SM_SET_ZERO( &msgMacLthTrcInd, sizeof(MsgSmLthTrcInd));
  SFndLenMsg(mBuf, &trcLen );
  if( trcLen >= MAX_LEN_TRACE_MSG )
  {
    logError("Jumbo Trace packet of length:%d received from MAC", trcLen);
    trcLen = MAX_LEN_TRACE_MSG;
  }
  msgMacLthTrcInd.traceLen = trcLen;

  if( trcLen > 0 )
  {
    SRemPreMsgMult((Data *)&(msgMacLthTrcInd.traceMsg), trcLen, mBuf);
    clock_gettime(CLOCK_REALTIME, &(msgMacLthTrcInd.timeVal));
    if( RFAILED == sendSmMsgToLth( EVT_SM_LTH_TRC_IND, sizeof(MsgSmLthTrcInd), (U8*)&msgMacLthTrcInd) )
    {
      logError("%s", "Error in sending MAC TRACE IND to LTH" );
    }
    else
    {
      logInfo("%s", "Sent MAC TRACE IND to LTH" );
    }
  }

  RETVALUE(ROK);
}


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
*       File:  ueh_sm_rg_ptmi.c
*
*/

PUBLIC S16 SmMiLrgStaCfm
(
Pst            *pst,
RgMngmt        *cfm
)
{
   logFunc("%s","SmMiLrgStaCfm");

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
} /* end of SmMiLrgStaCfm */

S16 smGetMacGenSts(
    RgMngmt        *sts,
    MacGenCounters *rsp
    )
{
  if(sts->hdr.elmId.elmnt != STGEN)
  {
    logError("Gen STATS are handled, Unknown element received:%d", sts->hdr.elmId.elmnt);
    return RFAILED;
  }
  
   rsp->numHarqFail = sts->t.sts.s.genSts.numHarqFail;
   rsp->numUeCfg    = sts->t.sts.s.genSts.numUeCfg;
   rsp->numCellCfg  = sts->t.sts.s.genSts.numCellCfg;
   
  return ROK;
}

S16 smGetMacRlcSts(
    RgMngmt           *sts,
    MacRlcsapCounters *rsp
    )
{
  if(sts->hdr.elmId.elmnt != STRGUSAP)
  {
    logError("RLC STATS are handled, Unknown element received:%d", sts->hdr.elmId.elmnt);
    return RFAILED;
  }

  cmMemcpy((U8*)rsp, (CONSTANT U8 *)&(sts->t.sts.s.rguSts), sizeof(MacRlcsapCounters) );
  return ROK;
}

S16 smGetMacPhySts(
    RgMngmt           *sts,
    MacPhysapCounters *rsp
    )
{
  if(sts->hdr.elmId.elmnt != STTFUSAP)
  {
    logError("PHY STATS are handled, Unknown element received:%d", sts->hdr.elmId.elmnt);
    return RFAILED;
  }

  cmMemcpy( (U8*)rsp, (CONSTANT U8 *)&(sts->t.sts.s.tfuSts), sizeof(MacPhysapCounters) );
  return ROK;
}

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
*       File:  ueh_sm_rg_ptmi.c
*
*/

PUBLIC S16 SmMiLrgStsCfm(
    Pst            *pst,
    RgMngmt        *sts
  )
{
  MsgSmPmhMacGenStatRsp       macGenStatRsp;
  MsgSmPmhMacRlcsapStatRsp    macRlcsapStatRsp;
  MsgSmPmhMacPhysapStatRsp    macPhysapStatRsp;

  (void)cmMemset((U8 *) &(macGenStatRsp), 0, sizeof(MsgSmPmhMacGenStatRsp));
  (void)cmMemset((U8 *) &(macRlcsapStatRsp), 0, sizeof(MsgSmPmhMacRlcsapStatRsp));
  (void)cmMemset((U8 *) &(macPhysapStatRsp), 0, sizeof(MsgSmPmhMacPhysapStatRsp));

  logFunc("%s","SmMiLrgStsCfm");

  logInfo("%s","STATS Confirm received from MAC" );

  if( STGEN == sts->hdr.elmId.elmnt)
  {
    if(ROK != smGetMacGenSts(sts,&macGenStatRsp.macGenCounters) )
    {
      logError("%s", "Error in Extracting received MAC General STATS Message");
      RETVALUE(RFAILED);
    }
    macGenStatRsp.transId = sts->hdr.transId;

    if( RFAILED == sendSmMsgToPmh( EVT_SM_PMH_MAC_GEN_STAT_RSP, sizeof(MsgSmPmhMacGenStatRsp), (U8*)&macGenStatRsp) )
    {
        logError("%s", "Error in sending MAC General STATS to PMH" );
    }
    else
    {
        logInfo("%s", "Sent MAC General STATS to PMH" );
    }
  }
  else if(STRGUSAP == sts->hdr.elmId.elmnt )
  {
    if(ROK != smGetMacRlcSts(sts,&macRlcsapStatRsp.macRlcsapCounters) )
    {
      logError("%s", "Error in Extracting received MAC STRGUSAP STATS Message");
      RETVALUE(RFAILED);
    }
    macRlcsapStatRsp.transId = sts->hdr.transId;
    if( RFAILED == sendSmMsgToPmh( EVT_SM_PMH_MAC_RLCSAP_STAT_RSP, sizeof(MsgSmPmhMacRlcsapStatRsp), (U8*)&macRlcsapStatRsp) )
    {
        logError("%s", "Error in sending MAC STRGUSAP STATS to PMH" );
    }
    else
    {
        logInfo("%s", "Sent MAC STRGUSAP STATS to PMH" );
    }
  }
  else if(STTFUSAP == sts->hdr.elmId.elmnt )
  {
    if(ROK != smGetMacPhySts(sts,&macPhysapStatRsp.macPhysapCounters) )
    {
      logError("%s", "Error in Extracting received MAC STTFUSAP STATS Message");
      RETVALUE(RFAILED);
    }
    macPhysapStatRsp.transId = sts->hdr.transId;


    if( RFAILED == sendSmMsgToPmh( EVT_SM_PMH_MAC_PHYSAP_STAT_RSP, sizeof(MsgSmPmhMacPhysapStatRsp), (U8*)&macPhysapStatRsp) )
    {
        logError("%s", "Error in sending MAC STTFUSAP STATS to PMH" );
    }
    else
    {
        logInfo("%s", "Sent MAC STTFUSAP STATS to PMH" );
    }
  }
  else
  {
    logError("Unknown Stats of type:%d received from MAC", sts->hdr.elmId.elmnt );
  }

   RETVALUE(ROK);
} /* end of SmMiLrgStsCfm */
