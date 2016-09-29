/**
 * @file  sm_eg_ptmi.c
 * @brief This file contains layer management functions for PDCP
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
#include "ueh.h"
#include "bsm_egtp_if.h"
#include "log.h"
#include "bsm_ueh_if.h"
#include "ueh_pmh_if.h"
#include "lth_sm_if.h"
#include "pst_events.h"

/*local defines */
#define SM_MAX_EG_SEL           4
#define EGTP_MAX_TRACE_CFM      2

PRIVATE U8 uehEgCfg = 0;
PRIVATE U8 uehEgTrcRspCount = 0;
S16 smGetEgtpSts(
  EgMngmt        *gtpStsCfm,
  GtpGenCounters *rsp
  );

/*local typedefs */

/*forward references */

PUBLIC S16 PtMiLegCfgReq      ARGS((Pst *post, EgMngmt *cfg));
PUBLIC S16 PtMiLegStsReq      ARGS((Pst *post, Action action, EgMngmt *sts));
PUBLIC S16 PtMiLegStaReq      ARGS((Pst *post, EgMngmt *ssta));
PUBLIC S16 PtMiLegCntrlReq    ARGS((Pst *post, EgMngmt *cntrl));

/* public variable declarations */

/* public variable definitions */

/* private variable definitions */

/* public routines */

/* Following matrices define the mapping between the primitives called by the
 * layer manager (as SmMiLegXXXReq) and the corresponding primitives of the 
 * S1AP layer (layer management service provider) (as EgMiLegXXXReq).
 * Each primitive is mapped to one of SM_MAX_EG_SEL functions in the array.
 * The mapping is based on post->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCSMEGMILEG  loose         cmPkMiLegXXXReq
 * 1                 MG          tight         EgMiLegXXXReq
 * 2+                            tight         PtMiLegXXXReq
 */

PRIVATE LegCfgReq smMiLegCfgReqMt[] =
{
#ifdef LCSMEGMILEG
   cmPkLegCfgReq,
#else
   PtMiLegCfgReq,
#endif

#ifdef EG
   EgMiLegCfgReq,
#else
   PtMiLegCfgReq,
#endif

};


PRIVATE LegStsReq smMiLegStsReqMt[] =
{
#ifdef LCSMEGMILEG
   cmPkLegStsReq,
#else
   PtMiLegStsReq,
#endif

#ifdef EG
   EgMiLegStsReq,
#else
   PtMiLegStsReq,
#endif

};


PRIVATE LegStaReq smMiLegStaReqMt[] =
{
#ifdef LCSMEGMILEG
   cmPkLegStaReq,
#else
   PtMiLegStaReq,
#endif

#ifdef EG
   EgMiLegStaReq,
#else
   PtMiLegStaReq,
#endif

};


PRIVATE LegCntrlReq smMiLegCntrlReqMt[] =
{
#ifdef LCSMEGMILEG
   cmPkLegCntrlReq,
#else
   PtMiLegCntrlReq,
#endif

#ifdef EG
   EgMiLegCntrlReq,
#else
   PtMiLegCntrlReq,
#endif

};


/*
 *
 *       Fun:    SmMiLegCfgReq - Configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_sm_eg_ptmi.c
 *
 */

PUBLIC S16 SmMiLegCfgReq
(
Pst     *post,       /* post structure */
EgMngmt *cfg        /* management structure */
)
{

   logFunc("%s","SmMiLegCfgReq");

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_EG_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLegCfgReqMt[post->selector]))(post, cfg));
} /* end of SmMiLegCfgReq() */

/*
 *
 *       Fun:    SmMiLegStsReq - Statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_sm_eg_ptmi.c
 *
 */

PUBLIC S16 SmMiLegStsReq
(
Pst     *post,       /* post structure */
Action action,
EgMngmt *sts        /* management structure */
)
{
   logFunc("%s","SmMiLegStsReq");

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_EG_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLegStsReqMt[post->selector]))(post, action, sts));
} /* end of SmMiLegStsReq() */

/*
 *
 *       Fun:    SmMiLegStaReq - Status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_sm_eg_ptmi.c
 *
 */

PUBLIC S16 SmMiLegStaReq
(
Pst     *post,       /* post structure */
EgMngmt *ssta        /* management structure */
)
{
   logFunc("%s","SmMiLegStaReq");

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_EG_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLegStaReqMt[post->selector]))(post, ssta));
} /* end of SmMiLegStaReq() */

/*
 *
 *       Fun:    SmMiLegCntrlReq - Control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   eg_sm_eg_ptmi.c
 *
 */

PUBLIC S16 SmMiLegCntrlReq(
    Pst     *post,       /* post structure */
    EgMngmt *cntrl        /* management structure */
  )
{
   logFunc("%s","SmMiLegCntrlReq");

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_EG_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLegCntrlReqMt[post->selector]))(post, cntrl));
} /* end of SmMiLegCntrlReq() */

/*
 *
 *       Fun:    PtMiLegCfgReq - portable configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   eg_sm_eg_ptmi.c
 *
 */

PUBLIC S16 PtMiLegCfgReq
(
Pst     *post,       /* post structure */
EgMngmt *cfg        /* management structure */
)
{
   logFunc("%s","PtMiLegCfgReq");

   UNUSED(post);
   UNUSED(cfg);
   RETVALUE(RFAILED);
} /* end of PtMiLegCfgReq() */

/*
 *
 *       Fun:    PtMiLegStsReq - portable statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   eg_sm_eg_ptmi.c
 *
 */

PUBLIC S16 PtMiLegStsReq
(
Pst     *post,       /* post structure */
Action action,
EgMngmt *sts        /* management structure */
)
{
   logFunc("%s","PtMiLegStsReq");

   UNUSED(post);
   UNUSED(action);
   UNUSED(sts);
   RETVALUE(RFAILED);
} /* end of PtMiLegStsReq() */

/*
 *
 *       Fun:    PtMiLegStaReq - portable status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_sm_eg_ptmi.c
 *
 */

PUBLIC S16 PtMiLegStaReq
(
Pst     *post,       /* post structure */
EgMngmt *ssta        /* management structure */
)
{
   logFunc("%s","PtMiLegStaReq");

   UNUSED(post);
   UNUSED(ssta);
   RETVALUE(RFAILED);
} /* end of PtMiLegStaReq() */

/*
 *
 *       Fun:    PtMiLegCntrlReq - portable control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_sm_eg_ptmi.c
 *
 */

PUBLIC S16 PtMiLegCntrlReq
(
Pst     *post,       /* post structure */
EgMngmt *cntrl        /* management structure */
)
{
   logFunc("%s","PtMiLegCntrlReq");

   UNUSED(post);
   UNUSED(cntrl);
   RETVALUE(RFAILED);
} /* end of PtMiLegCntrlReq() */

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
*       File:  ueh_sm_eg_ptmi.c
*
*/

PUBLIC S16 SmMiLegCfgCfm(
  Pst     *pst,          /* post structure */
  EgMngmt *cfm           /* configuration */
  )
{
   Pst      egPst;

   logFunc("%s","SmMiLegCfgCfm");

   SM_SET_ZERO(&egPst,  sizeof(Pst));

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
       switch (cfm->hdr.elmId.elmnt)
       {
          case STEGGEN:
               uehEgCfg |= UEH_EG_GEN_CFG;
               break;      
          case STEGUSAP:
               uehEgCfg |= UEH_EG_EGT_SAP_CFG;
               break;
          case STEGTSAP:
               uehEgCfg |= UEH_EG_TSAP_CFG;
               break;
          case STEGSRVR:
               uehEgCfg |= UEH_EG_TSRV_CFG;
               break;
          case STEGIPADDR:
               uehEgCfg |= UEH_EG_IPADDR_CFG;
               break;
          default:
            logError("EGTP Config Confirm received for invalid element:%d",cfm->hdr.elmId.elmnt);
            break;
       } /* end of switch statement */

       logInfo("EGTP Config Confirm received for the element:%d",cfm->hdr.elmId.elmnt);

       if (uehEgCfg == UEH_SM_EG_CONFIGURED)
       {
          logInfo("%s", "EGTP CONFIGURATION SUCCESSFULLY PERFORMED");
          /* Configure the UEH APP */
          smCb.smState = UEH_SM_STATE_EGTP_CFG_DONE;
          UEH_FILL_PST(egPst, SM_EG_PROC, ENTSM, ENTUEH, EVTUEHUDHCFG, UDH_POOL, SM_SELECTOR_LC);
          uehSmSelfPstMsg(&egPst);
       }
   }
   else
   { /*reason defined in gen.h*/ 
     logError("Configuration Failure received from EGTP for element:%d reason:%d",cfm->hdr.elmId.elmnt,cfm->cfm.reason);
   }

   RETVALUE(ROK);
}

/*
 *
 *       Fun:    SmMiLegCntrlCfm - control confirm
 *
 *       Desc:   prints the control confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_eg_ptmi.c
 *
 */

PUBLIC S16 SmMiLegCntrlCfm(
    Pst         *pst,
    EgMngmt     *cntrl
  )
{
   Pst  egPst;
   MsgEgtpBsmInitConfigSucc egtpCfgSucc;
   MsgEgtpBsmInitConfigSucc egtpCfgFail;
   MsgSmLthTraceCntrlCfm      trcCfm;
   TBOOL                sndMsgToLth = FALSE;

   logFunc("%s","SmMiLegCntrlCfm");

   SM_SET_ZERO(&egPst, sizeof(Pst));

   switch(cntrl->hdr.elmId.elmnt)
   {
     case STEGTSAP:
     {
       if (cntrl->cfm.status == LCM_PRIM_OK  )
       {
         logInfo("%s", "BIND OF EGTP LSAP WITH TUCL SUCCESSFULLY PERFORMED");
         egtpCfgSucc.transId = smCb.egtpTransId;
         smUdhGtpSapBindReq();
       }
       else if (cntrl->cfm.status == LCM_PRIM_OK_NDONE  && cntrl->hdr.elmId.elmnt == STEGTSAP )
       {
         logInfo("%s", "Bind of EGTP LSAP with TUCL is in progress");
       }
       else
       {
          egtpCfgFail.transId = smCb.egtpTransId;
          logError("%s", "BIND OF EGTP LSAP WITH TUCL FAILED");
       }
       break;
     }
     case STSZGEN:
     {
         switch( cntrl->hdr.transId )
         {
           case SM_EGTP_DEBUG_TRANS_ID:
           {
             logInfo("%s", "Debug is enabled for EGTP");
             //smCfgCb.smDbgMsk = (smCfgCb.smDbgMsk | UEH_SM_EG_DBG_MASK);
             smCb.cfgCb.smDbgMsk = (smCb.cfgCb.smDbgMsk | UEH_SM_EG_DBG_MASK);
             break;
           }
           case SM_EGTP_START_TRACE_TRANS_ID:
           {
             uehEgTrcRspCount++;
             sndMsgToLth = TRUE;
             if (cntrl->cfm.status == LCM_PRIM_OK)
             {
               trcCfm.traceCtrlConfirm = TRUE;
               logInfo("%s", "TRACE is ENABLED in EGTP");
             }
             else
             {
               trcCfm.traceCtrlConfirm = FALSE;
               logError("%s", "TRACE ENABLE in EGTP FAILED");
             }
             break;
           }
           case SM_EGTP_STOP_TRACE_TRANS_ID:
           {
             uehEgTrcRspCount++;
             sndMsgToLth = TRUE;
             if (cntrl->cfm.status == LCM_PRIM_OK)
             {
               trcCfm.traceCtrlConfirm = TRUE;
               logInfo("%s", "TRACE is DISABLED in EGTP");
             }
             else
             {
               trcCfm.traceCtrlConfirm = FALSE;
               logError("%s", "TRACE DISABLE in EGTP FAILED");
             }
             break;
           }
           default:
           {
             if (cntrl->cfm.status == LCM_PRIM_OK)
             {
               logError("General Control Confirm received from EGTP with, Action:%d, Subaction:%d",
                                   cntrl->u.cntrl.action, cntrl->u.cntrl.subAction );
             }
             else
             {
               logError("General Control Fail received from EGTP with, Action:%d, Subaction:%d",
                                   cntrl->u.cntrl.action, cntrl->u.cntrl.subAction );
             }
             break;
           }
         } /* switch */

         if( (TRUE == sndMsgToLth) &&
             ( EGTP_MAX_TRACE_CFM == uehEgTrcRspCount ) )
         {
            if( RFAILED == sendSmMsgToLth( EVT_SM_LTH_TRC_START_SUCC, sizeof(MsgSmLthTraceCntrlCfm), (U8*)&trcCfm) )
            {
              logError("%s", "Error in sending EGTP TRACE CFM to BSM" );
            }
            else
            {
              logInfo("%s", "Sent EGTP TRACE CFM  to BSM" );
            }
            uehEgTrcRspCount = 0;
         }
       break;
     }
     default:
     {
       if(cntrl->cfm.status == LCM_PRIM_OK)
       {
         logInfo("Control confirm received from EGTP for Element:%d, Action:%d, Subaction:%d",
             cntrl->hdr.elmId.elmnt, cntrl->u.cntrl.action, cntrl->u.cntrl.subAction );
       }
       else
       {
         logError("Control fail received from EGTP for Element:%d, Action:%d, Subaction:%d",
             cntrl->hdr.elmId.elmnt, cntrl->u.cntrl.action, cntrl->u.cntrl.subAction );
       }
       break;
     }
   }

   RETVALUE(ROK);
}

S16 smGetEgtpSts(
  EgMngmt        *gtpStsCfm,
  GtpGenCounters *rsp
  )
{
  if(gtpStsCfm->hdr.elmId.elmnt != STEGGEN)
  {
    logError("Only General STATS are handled, Unknown element received:%d", gtpStsCfm->hdr.elmId.elmnt );
    return RFAILED;
  }

   rsp->numTxMsg       = gtpStsCfm->u.sts.s.gen.numTxMsg;
   rsp->numRxMsg       = gtpStsCfm->u.sts.s.gen.numRxMsg;
   rsp->numActvTnls    = gtpStsCfm->u.sts.s.gen.numActvTnls;
   rsp->numErrInd      = gtpStsCfm->u.sts.s.gen.numErrInd;
   rsp->rxDesgMsg      = gtpStsCfm->u.sts.s.gen.rxDesgMsg;

   rsp->invVerRx       = gtpStsCfm->u.sts.s.gen.errSts.invVerRx;
   rsp->invProtTypeRx  = gtpStsCfm->u.sts.s.gen.errSts.invProtTypeRx;
   rsp->invMsgRx       = gtpStsCfm->u.sts.s.gen.errSts.invMsgRx;
   rsp->invManIeLenRx  = gtpStsCfm->u.sts.s.gen.errSts.invMsgLenRx;
   rsp->invIeOrderRx   = gtpStsCfm->u.sts.s.gen.errSts.invIeOrderRx;
   rsp->invIeLenRx     = gtpStsCfm->u.sts.s.gen.errSts.invIeLenRx;
   rsp->invIeFormatRx  = gtpStsCfm->u.sts.s.gen.errSts.invIeFormatRx;
   rsp->invManIeLenRx  = gtpStsCfm->u.sts.s.gen.errSts.invManIeLenRx;
   rsp->missMandIeRx   = gtpStsCfm->u.sts.s.gen.errSts.missMandIeRx;
   rsp->unSuppExtHdrRx = gtpStsCfm->u.sts.s.gen.errSts.unSuppExtHdrRx;
   rsp->wrngExtHdrRx   = gtpStsCfm->u.sts.s.gen.errSts.wrngExtHdrRx;
   rsp->msgUnableCompRx= gtpStsCfm->u.sts.s.gen.errSts.msgUnableCompRx;
   rsp->msgUnableCompTx= gtpStsCfm->u.sts.s.gen.errSts.msgUnableCompTx;
   rsp->missMandIeTx   = gtpStsCfm->u.sts.s.gen.errSts.missMandIeTx;
   rsp->memAllocFldTx  = gtpStsCfm->u.sts.s.gen.errSts.memAllocFldTx;
   rsp->invMsgTx       = gtpStsCfm->u.sts.s.gen.errSts.invMsgTx;
   rsp->invTeidTx      = gtpStsCfm->u.sts.s.gen.errSts.invTeidTx;
   rsp->invTeidRx      = gtpStsCfm->u.sts.s.gen.errSts.invTeidRx;
   rsp->errSeqNoTx     = gtpStsCfm->u.sts.s.gen.errSts.errSeqNoTx;
   rsp->errSeqNoRx     = gtpStsCfm->u.sts.s.gen.errSts.errSeqNoRx;
   rsp->invDatTypeTx   = gtpStsCfm->u.sts.s.gen.errSts.invDatTypeTx;
  
  
  return (ROK);
}

/*
 *
 *       Fun:    SmMiLegStsCfm - Statistics confirm
 *
 *       Desc:   prints the statistics confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_eg_ptmi.c
 *
 */

PUBLIC S16 SmMiLegStsCfm(
    Pst         *pst,
    EgMngmt     *sts
  )
{
  MsgSmPmhGtpGenStatRsp    rsp;

  (void)cmMemset((U8 *) &(rsp), 0, sizeof(MsgSmPmhGtpGenStatRsp));

  logInfo("%s","STATS Confirm received from EGTP" );

  /* CCPU statistics structure for EGTP is egMgmt|EgPdu
   * First unpack egMgmt, Copy the general,
   * Send the Message to PMH
   */
  if(ROK != smGetEgtpSts(sts,&rsp.gtpGenCounters) )  {
    logError("%s", "Error in extracting received EGTP STATS Message");
    RETVALUE(RFAILED);
  }
  rsp.transId = sts->hdr.transId;

  if( RFAILED == sendSmMsgToPmh( EVT_SM_PMH_GTP_GEN_STAT_RSP, sizeof(MsgSmPmhGtpGenStatRsp), (U8*)&rsp) )
  {
      logError("%s", "Error in sending EGTP STATS to PMH" );
  }
  else
  {
      logInfo("%s", "Sent EGTP STATS to PMH" );
  }

  RETVALUE(ROK);
}

/*
 *
 *       Fun:    SmMiLegStaCfm - status confirm
 *
 *       Desc:   prints the status confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_eg_ptmi.c
 *
 */


PUBLIC S16 SmMiLegStaCfm
(
Pst         *pst,
EgMngmt     *sta
)
{
   logFunc("%s","SmMiLegStaCfm");

   RETVALUE(ROK);
} /* end of SmMiLegStaCfm() */

/*
 *
 *       Fun:    SmMiLegStaInd - status indication
 *
 *       Desc:   prints the status indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_eg_ptmi.c
 *
 */

PUBLIC S16 SmMiLegStaInd
(
Pst         *pst,
EgMngmt     *ind
)
{
   logFunc("%s","SmMiLegStaInd");

   RETVALUE(ROK);
} /* end of SmMiLegStaInd() */

/*
 *
 *       Fun:    SmMiLegTrcInd - Trace indication
 *
 *       Desc:   prints the trace indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_eg_ptmi.c
 *
 */


PUBLIC S16 SmMiLegTrcInd(
    Pst         *pst,
    EgMngmt     *ind,
    Buffer      *mBuf
  )
{
  MsgLen               trcLen;
  MsgSmLthTrcInd     msgEgtpLthTrcInd;

  /* CCPU trace structure for EGTP is |EgMngmt HDR|PduLen|MacPdu
   * Copy the PDU to  msgRlcLthTrcInd,
   * Add Time stamp*/
  logInfo("%s","Trace Indication received from EGTP");

  SM_SET_ZERO( &msgEgtpLthTrcInd, sizeof(MsgSmLthTrcInd));
  SFndLenMsg(mBuf, &trcLen );
  if( trcLen >= MAX_LEN_TRACE_MSG )
  {
    logError("Jumbo Trace packet of length:%d received from EGTP", trcLen);
    trcLen = MAX_LEN_TRACE_MSG;
  }
  msgEgtpLthTrcInd.traceLen = trcLen;

  if( trcLen > 0 )
  {
    SRemPreMsgMult((Data *)&(msgEgtpLthTrcInd.traceMsg), trcLen, mBuf);
    clock_gettime(CLOCK_REALTIME, &(msgEgtpLthTrcInd.timeVal));
    if( RFAILED == sendSmMsgToLth( EVT_SM_LTH_TRC_IND, sizeof(MsgSmLthTrcInd), (U8*)&msgEgtpLthTrcInd) )
    {
      logError("%s", "Error in sending EGTP TRACE IND to LTH" );
    }
    else
    {
      logInfo("%s", "Sent EGTP TRACE IND to LTH" );
    }
  }

  RETVALUE(ROK);
} /* end of SmMiLegTrcInd() */

