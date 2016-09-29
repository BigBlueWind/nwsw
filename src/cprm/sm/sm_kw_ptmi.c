/**
 * @file  sm_kw_ptmi.c
 * @brief This file contains RLC coupling functions
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */

#include "ccpu_cmn.h"

/* RRM Related FIles */
#include "ueh.h"
#include "bsm_ueh_if.h"
#include "log.h"
#include "ueh_pmh_if.h"
#include "lth_sm_if.h"
#include "pst_events.h"

PRIVATE U8 uehKwCfg = 0;

#define MAXNHMI  2         /* The size of the LC/TC matrix */

/* local defines */
#ifndef LCKWMILKW
#define PTSMLKW
#endif

#ifndef KW
#define PTSMLKW
#endif

#ifndef SM 
#define PTSMLKW
#endif

/* local typedefs */

/* local externs */

#ifdef PTSMLKW
/* forward references */
PRIVATE S16 PtMiLkwCfgReq   ARGS((Pst *pst, KwMngmt *cfg ));
PRIVATE S16 PtMiLkwCntrlReq   ARGS((Pst *pst, KwMngmt *cfg ));
#endif


#ifdef __cplusplus
extern "C" {
#endif /* cplusplus */

PRIVATE LkwCfgReq smMiLkwCfgReqMt[] =
{
#ifdef LCKWMILKW
   cmPkLkwCfgReq,          /* 0 - loosely coupled - fc */
#else
   PtMiLkwCfgReq,          /* 0 - loosely coupled portable */
#endif /* LCKWMILKW */
#ifdef KW
   KwMiLkwCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLkwCfgReq,          /* 1 - tightly coupled portable */
#endif /* KW */
};


/* control request primitive */
PRIVATE LkwCntrlReq smMiLkwCntrlReqMt[] =
{
#ifdef LCKWMILKW 
   cmPkLkwCntrlReq,          /* 0 - loosely coupled - fc */
#else
   PtMiLkwCntrlReq,          /* 0 - tightly coupled, portable */
#endif /* LCKWMILKW */
#ifdef KW 
   KwMiLkwCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLkwCntrlReq,          /* 1 - tightly coupled, portable */
#endif /* RL */
};

S16 smGetRlcSts(
    KwMngmt        *sts,
    RlcGenCounters *rsp
    );

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
 *      File:  ueh_sm_kw_ptmi.c
 *
 *************************************************************************/

PUBLIC S16 SmMiLkwCfgReq
(
Pst *pst,                 /* post structure */
KwMngmt *cfg              /* configure */
)
{
   logFunc("%s","SmMiLkwCfgReq");
   /* jump to specific primitive depending on configured selector */
   (*smMiLkwCfgReqMt[pst->selector])(pst, cfg);
   RETVALUE(ROK);

} /* end SmMiLkwCfgReq */

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
 *      File:  ueh_sm_kw_ptmi.c
 *
 *************************************************************************/

PUBLIC S16 SmMiLkwCntrlReq
(
Pst *pst,                 /* post structure */
KwMngmt *cntrl            /* control */
)
{
   logFunc("%s","SmMiLkwCntrlReq");
   /* jump to specific primitive depending on configured selector */
   (*smMiLkwCntrlReqMt[pst->selector])(pst, cntrl);
   RETVALUE(ROK);
} /* end of SmMiLkwCntrlReq */


#ifdef PTSMLKW
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
 *      File:  ueh_sm_kw_ptmi.c
 *
 *************************************************************************/

PRIVATE S16 PtMiLkwCfgReq
(
Pst *pst,                   /* post structure */
KwMngmt *cfg                /* configure */
)
{
  logFunc("%s","PtMiLkwCfgReq");
  UNUSED(cfg);
  UNUSED(pst);
  logFunc("%s","This function is not implemented");
  RETVALUE(ROK);
} /* end of PtMiLkwCfgReq */

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
 *      File:  ve_sm_kw_ptmi.c
 *
 *************************************************************************/

PRIVATE S16 PtMiLkwCntrlReq
(
Pst *pst,                   /* post structure */
KwMngmt *cntrl              /* control */
)
{
  logFunc("%s","PtMiLkwCntrlReq");

  UNUSED(cntrl);
  UNUSED(pst);
  logFunc("%s","This function is not implemented");
  RETVALUE(ROK);
} /* end of PtMiLkwCntrlReq */


#endif /* PTSMLKW */


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
*       File:  ueh_sm_kw_ptmi.c
*
*/


PUBLIC S16 SmMiLkwCfgCfm(
  Pst     *pst,          /* post structure */
  KwMngmt *cfm           /* configuration */
  )
{
   Pst      kwPst;

   logFunc("%s","SmMiLkwCfgCfm");

   SM_SET_ZERO(&kwPst, sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STGEN:
            uehKwCfg |= UEH_KW_GEN_CFG;
            break;      
         case STCKWSAP:
            uehKwCfg |= UEH_KW_CKW_SAP_CFG;
            break;
         case STKWUSAP:
            uehKwCfg |= UEH_KW_KWU_SAP_CFG;
            break;
         case STRGUSAP: 
            uehKwCfg |= UEH_KW_RGU_SAP_CFG;
            break;
         default:
            logError("RLC Config Confirm received with invalid element:%d", cfm->hdr.elmId.elmnt);
            break;
      }
      logInfo("RLC Config Confirm received from element:%d", cfm->hdr.elmId.elmnt);

      if (uehKwCfg == UEH_SM_KW_CONFIGURED)
      {
         logInfo("%s", "RLC CONFIGURATION SUCCESSFULLY PERFORMED");

         smCb.smState = UEH_SM_STATE_RLC_CFG_DONE;
         UEH_FILL_PST(kwPst, SM_KW_PROC, ENTSM, ENTUEH, EVTUEHMACCFG, UEH_POOL, SM_SELECTOR_LC);
         uehSmSelfPstMsg(&kwPst);
      }
   }
   else
   {
     logError("Configuration Failure received from RLC with element:%d", cfm->hdr.elmId.elmnt);
   }

   RETVALUE(ROK);
} /* end of SmMiLkwCfgCfm */

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
*       File:  ueh_sm_kw_ptmi.c
*
*/

PUBLIC S16 SmMiLkwCntrlCfm(
  Pst     *pst,          /* post structure */
  KwMngmt *cntrl         /* control */
  )
{
   Pst                          kwPst;
   MsgSmLthTraceCntrlCfm      trcCfm;
   U16                          sndMsgToLth = FALSE;
   U8   trcMsgId;

   logFunc("%s","SmMiLkwCntrlCfm");

   SM_SET_ZERO(&kwPst, sizeof(Pst));

   switch(cntrl->hdr.elmId.elmnt)
   {
     case STRGUSAP:
     {
       if (cntrl->cfm.status == LCM_PRIM_OK )
       {
         logInfo("%s", "BIND OF RLC TO MAC (RGU) SAP SUCCESSFUL");
         logInfo("%s", "Initiate control request for binding UDH to PDCP");
         smUdhPdcpSapBindReq();
       }
       else
       {
         logError("%s", "BIND OF RLC WITH MAC FAILED");
       }
       break;
     }
     case STGEN:
     {
         switch(cntrl->hdr.transId)
         {
           case SM_RLC_DEBUG_TRANS_ID:
           {
             logInfo("%s", "Debug is enabled for RLC");
             smCb.cfgCb.smDbgMsk = (smCb.cfgCb.smDbgMsk | UEH_SM_KW_DBG_MASK);
             break;
           }
           case SM_RLC_START_TRACE_TRANS_ID:
           {
             sndMsgToLth = TRUE;
             if (cntrl->cfm.status == LCM_PRIM_OK)
             {
              trcMsgId = EVT_SM_LTH_TRC_START_SUCC;
         logError("%s","senging trace start succ from RLC\n");
               trcCfm.traceCtrlConfirm = TRUE;
               logInfo("%s", "TRACE is ENABLED in RLC");
             }
             else
             {
               trcMsgId = EVT_SM_LTH_TRC_START_FAIL;
               trcCfm.traceCtrlConfirm = FALSE;
               logError("%s", "TRACE ENABLE in RLC FAILED");
             }
             break;
           }
           case SM_RLC_STOP_TRACE_TRANS_ID:
           {
        	   sndMsgToLth = TRUE;
             if (cntrl->cfm.status == LCM_PRIM_OK)
             {
               trcMsgId = EVT_SM_LTH_TRC_STOP_SUCC;
               trcCfm.traceCtrlConfirm = TRUE;
               logInfo("%s", "TRACE is DISABLED in RLC");
             }
             else
             {
               trcMsgId = EVT_SM_LTH_TRC_STOP_FAIL;
               trcCfm.traceCtrlConfirm = FALSE;
               logError("%s", "TRACE DISABLE in RLC FAILED");
             }
             break;
           }
           default:
           {
             logError("General Control confirm received from RLC for unknown Subaction:%d", cntrl->t.cntrl.subAction );
             break;
           }
         } /* End of Switch */

         trcCfm.protocolName = TRACECONFIGMOD_RLC;

         if( TRUE EQ sndMsgToLth )
         {
           sendSmMsgToLth( trcMsgId, sizeof(MsgSmLthTraceCntrlCfm), (U8*)&trcCfm);
         }
         break;
     } /* STGEN */
     default:
     {
       if(cntrl->cfm.status == LCM_PRIM_OK)
       {
         logInfo("Control confirm received from RLC for Element:%d, Action:%d, Subaction:%d",
             cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
       }
       else
       {
         logError("Control fail received from RLC for Element:%d, Action:%d, Subaction:%d",
             cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
       }
       break;
     }

   }


   RETVALUE(ROK);
}

S16 smGetRlcSts(
    KwMngmt        *sts,
    RlcGenCounters *rsp
    )
{
  if(sts->hdr.elmId.elmnt != STGEN)
  {
    logError("Only General STATS are handled, Unknown element received:%d", sts->hdr.elmId.elmnt);
    return RFAILED;
  }
  
  rsp->numUeCfg      = sts->t.sts.s.gen.numUe;
 /*  g_rlcGenCounter.numLc         = sts->t.sts.s.gen.numLc; -- numLc support is not there in Sept30 release -- PWAV */
  rsp->pdusRecv      = sts->t.sts.s.gen.pdusRecv;
  rsp->pdusSent      = sts->t.sts.s.gen.pdusSent;
  rsp->pdusRetx      = sts->t.sts.s.gen.pdusRetx;
  rsp->bytesRecv     = sts->t.sts.s.gen.bytesRecv;
  rsp->bytesSent     = sts->t.sts.s.gen.bytesSent;
  rsp->unexpPdusRecv = sts->t.sts.s.gen.unexpPdusRecv;
  rsp->errorPdusRecv = sts->t.sts.s.gen.errorPdusRecv;
  rsp->protTimeOut   = sts->t.sts.s.gen.protTimeOut;
  rsp->numOfRb       = sts->t.sts.s.gen.numOfRb;
  rsp->numSduDisc    = sts->t.sts.s.gen.numSduDisc;
  /* TODO numCellCfg; is not defined in which category it should belong*/

  return ROK;
}

/*
*
*       Fun:   Sts Confirm
*
*       Desc:  This function is used by to present Sts confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_sm_kw_ptmi.c
*
*/

PUBLIC S16 SmMiLkwStsCfm (
    Pst     *pst,          /* post structure */
    Action  action,        /* Action */
    KwMngmt *sts           /* statistics */
  )
{
  MsgSmPmhRlcGenStatRsp    rsp;
  (void)cmMemset((U8 *) &(rsp), 0, sizeof(MsgSmPmhRlcGenStatRsp));

 logInfo("%s","STATS Confirm received from RLC" );

 if(ROK !=  smGetRlcSts(sts,&rsp.rlcGenCounters)) 
 {
   logError("%s", "Error in extracting received RLC STATS Message");
   RETVALUE(RFAILED);
 }
 rsp.transId = sts->hdr.transId;

 sendSmMsgToPmh( EVT_SM_PMH_RLC_GEN_STAT_RSP, sizeof(MsgSmPmhRlcGenStatRsp), (U8*)&rsp);

 RETVALUE(ROK);
} /* end of SmMiLkwStsCfm */


/*
*
*       Fun:   Status Confirm
*
*       Desc:  This function is used by to present Status confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_sm_kw_ptmi.c
*
*/

PUBLIC S16 SmMiLkwStaCfm
(
Pst     *pst,          /* post structure */
KwMngmt *sta           /* Status */
)
{
   logFunc("%s","SmMiLkwStaCfm");

   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* end of SmMiLkwStaCfm */


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
*       File:  ueh_sm_kw_ptmi.c
*
*/

PUBLIC S16 SmMiLkwStaInd
(
Pst     *pst,          /* post structure */
KwMngmt *usta           /* configuration */
)
{
   logFunc("%s","SmMiLkwStaInd");

   UNUSED(pst);
   UNUSED(usta);

   RETVALUE(ROK);
} /* end of SmMiLkwStaInd */


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
*       File:  ueh_sm_kw_ptmi.c
*
*/

PUBLIC S16 SmMiLkwTrcInd(
  Pst      *pst,          /* post structure */
  KwMngmt  *trc,          /* trace indication */
  Buffer   *mBuf          /* message buffer */
  )
{
  MsgLen               trcLen;
  MsgSmLthTrcInd     msgSmLthTrcInd;

  /* CCPU trace structure for RLC is |RgMngmt HDR|PduLen|MacPdu
   * Copy the PDU to  msgRlcLthTrcInd,
   * Add Time stamp*/
  logInfo("%s","Trace Indication received from RLC");

  SM_SET_ZERO( &msgSmLthTrcInd, sizeof(MsgSmLthTrcInd));
  SFndLenMsg(mBuf, &trcLen );
  if( trcLen >= MAX_LEN_TRACE_MSG )
  {
    logError("Jumbo Trace packet of length:%d received from RLC", trcLen);
    trcLen = MAX_LEN_TRACE_MSG;
  }
  msgSmLthTrcInd.traceLen = trcLen;
  msgSmLthTrcInd.protocolName = TRACECONFIGMOD_RLC;

  if( trcLen > 0 )
  {
    SRemPreMsgMult((Data *)&(msgSmLthTrcInd.traceMsg), trcLen, mBuf);
    clock_gettime(CLOCK_REALTIME, &(msgSmLthTrcInd.timeVal));
    logError("Trc Ind from RLC sent %d", 1);
    sendSmMsgToLth( EVT_SM_LTH_TRC_IND, sizeof(MsgSmLthTrcInd), (U8*)&msgSmLthTrcInd);
  }

  RETVALUE(ROK);
}


#ifdef __cplusplus
}
#endif /* cplusplus */
