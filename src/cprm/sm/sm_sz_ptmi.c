/**
 * @file   sm_sz_ptmi.c
 * @brief This file contains management interface functions for S1AP
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
#include "bsm_s1ap_if.h"
#include "bsm_ueh_if.h"
#include "log.h"
#include "ueh_pmh_if.h"
#include "lth_sm_if.h"
#include "pst_events.h"


/*local defines */
#define SM_MAX_SZ_SEL 4

#define S1AP_MAX_TRACE_CFM              2 /* We have SCTSAP and Peer trace Points */
#define S1AP_PEER_TRC_ENABLED           0x01
#define S1AP_SCTSAP_TRC_ENABLED         0x02
#define S1AP_TRC_ENABLED                0x03 /*Trace Enable confirm for both SCTSAP and PEER */

#define S1AP_PEER_TRC_DISABLED           0xFE
#define S1AP_SCTSAP_TRC_DISABLED         0xFD
#define S1AP_TRC_DISABLED                0x00  /* Trace Disable confirm for both SCTSAP and PEER*/

PRIVATE U8 uehSzCfg = 0;
PRIVATE U8 uehSzTrcCtrl = 0;
PRIVATE U8 uehSzTrcRspCount = 0;
S16 smGetS1apPeerSts(
    SzMngmt              *s1apStsCfm,
    S1apPeerNodeCounters *rsp
    );
S16 smGetS1apGenSts(
    SzMngmt           *s1apStsCfm,
    S1apGenCounters   *rsp
    );

/*forward references */
PUBLIC S16 PtMiLszCfgReq      ARGS((Pst *post, SzMngmt *cfg));
PUBLIC S16 PtMiLszStsReq      ARGS((Pst *post, Action action, SzMngmt *sts));
PUBLIC S16 PtMiLszStaReq      ARGS((Pst *post, SzMngmt *ssta));
PUBLIC S16 PtMiLszCntrlReq    ARGS((Pst *post, SzMngmt *cntrl));

/* public variable declarations */

/* public variable definitions */

/* private variable definitions */

/* public routines */

/* Following matrices define the mapping between the primitives called by the
 * layer manager (as SmMiLszXXXReq) and the corresponding primitives of the 
 * S1AP layer (layer management service provider) (as SzMiLszXXXReq).
 * Each primitive is mapped to one of SM_MAX_SZ_SEL functions in the array.
 * The mapping is based on post->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCSMSZMILSZ  loose         cmPkMiLszXXXReq
 * 1                 MG          tight         SzMiLszXXXReq
 * 2+                            tight         PtMiLszXXXReq
 */

PRIVATE LszCfgReq smMiLszCfgReqMt[] =
{
#ifdef LCSMSZMILSZ
   cmPkLszCfgReq,
#else
   PtMiLszCfgReq,
#endif

#ifdef SZ
   SzMiLszCfgReq,
#else
   PtMiLszCfgReq,
#endif

};


PRIVATE LszStsReq smMiLszStsReqMt[] =
{
#ifdef LCSMSZMILSZ
   cmPkLszStsReq,
#else
   PtMiLszStsReq,
#endif

#ifdef SZ
   SzMiLszStsReq,
#else
   PtMiLszStsReq,
#endif

};


PRIVATE LszStaReq smMiLszStaReqMt[] =
{
#ifdef LCSMSZMILSZ
   cmPkLszStaReq,
#else
   PtMiLszStaReq,
#endif

#ifdef SZ
   SzMiLszStaReq,
#else
   PtMiLszStaReq,
#endif

};


PRIVATE LszCntrlReq smMiLszCntrlReqMt[] =
{
#ifdef LCSMSZMILSZ
   cmPkLszCntrlReq,
#else
   PtMiLszCntrlReq,
#endif

#ifdef SZ
   SzMiLszCntrlReq,
#else
   PtMiLszCntrlReq,
#endif

};


/*
 *
 *       Fun:    SmMiLszCfgReq - Configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_sm_sz_ptmi.c
 *
 */


PUBLIC S16 SmMiLszCfgReq
(
Pst     *post,       /* post structure */
SzMngmt *cfg        /* management structure */
)
{

   logFunc("%s","SmMiLszCfgReq");

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLszCfgReqMt[post->selector]))(post, cfg));
} /* end of SmMiLszCfgReq() */

/*
 *
 *       Fun:    SmMiLszStsReq - Statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_sm_sz_ptmi.c
 *
 */


PUBLIC S16 SmMiLszStsReq
(
Pst     *post,       /* post structure */
Action action,
SzMngmt *sts        /* management structure */
)
{
   logFunc("%s","SmMiLszStsReq");

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLszStsReqMt[post->selector]))(post, action, sts));
} /* end of SmMiLszStsReq() */

/*
 *
 *       Fun:    SmMiLszStaReq - Status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_sm_sz_ptmi.c
 *
 */


PUBLIC S16 SmMiLszStaReq
(
Pst     *post,       /* post structure */
SzMngmt *ssta        /* management structure */
)
{
   logFunc("%s","SmMiLszStaReq");

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE((*(smMiLszStaReqMt[post->selector]))(post, ssta));
} /* end of SmMiLszStaReq() */

/*
 *
 *       Fun:    SmMiLszCntrlReq - Control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_sm_sz_ptmi.c
 *
 */


PUBLIC S16 SmMiLszCntrlReq
(
Pst     *post,       /* post structure */
SzMngmt *cntrl        /* management structure */
)
{
   logFunc("%s","SmMiLszCntrlReq");

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif
   //logError("SzMngmt|action=%d|subaction=%d|status=%d",cntrl->u.cntrl.action,cntrl->u.cntrl.subAction,cntrl->cfm.status);
   RETVALUE((*(smMiLszCntrlReqMt[post->selector]))(post, cntrl));
} /* end of SmMiLszCntrlReq() */

/*
 *
 *       Fun:    PtMiLszCfgReq - portable configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_sm_sz_ptmi.c
 *
 */


PUBLIC S16 PtMiLszCfgReq
(
Pst     *post,       /* post structure */
SzMngmt *cfg        /* management structure */
)
{
   logFunc("%s","PtMiLszCfgReq");

   UNUSED(post);
   UNUSED(cfg);
   RETVALUE(RFAILED);
} /* end of PtMiLszCfgReq() */

/*
 *
 *       Fun:    PtMiLszStsReq - portable statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_sm_sz_ptmi.c
 *
 */

PUBLIC S16 PtMiLszStsReq
(
Pst     *post,       /* post structure */
Action action,
SzMngmt *sts        /* management structure */
)
{
   logFunc("%s","PtMiLszStsReq");

   UNUSED(post);
   UNUSED(action);
   UNUSED(sts);
   RETVALUE(RFAILED);
} /* end of PtMiLszStsReq() */

/*
 *
 *       Fun:    PtMiLszStaReq - portable status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_sm_sz_ptmi.c
 *
 */


PUBLIC S16 PtMiLszStaReq
(
Pst     *post,       /* post structure */
SzMngmt *ssta        /* management structure */
)
{
   logFunc("%s","PtMiLszStaReq");

   UNUSED(post);
   UNUSED(ssta);
   RETVALUE(RFAILED);
} /* end of PtMiLszStaReq() */

/*
 *
 *       Fun:    PtMiLszCntrlReq - portable control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   ueh_sm_sz_ptmi.c
 *
 */


PUBLIC S16 PtMiLszCntrlReq
(
Pst     *post,       /* post structure */
SzMngmt *cntrl        /* management structure */
)
{
   logFunc("%s","PtMiLszCntrlReq");

   UNUSED(post);
   UNUSED(cntrl);
   RETVALUE(RFAILED);
} /* end of PtMiLszCntrlReq() */

/******************** Responses from S1AP Terminate Here */
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
*       File:  ueh_sm_sz_ptmi.c
*
*/
PUBLIC S16 SmMiLszCfgCfm(
  Pst     *pst,          /* post structure */
  SzMngmt *cfm           /* configuration */
  )
{
   Pst      szPst;

   logFunc("%s","SmMiLszCfgCfm");

   //SM_SET_ZERO(&szPst, sizeof(Pst)); not required as UEH_FILL_PST is already doing it

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
       switch (cfm->hdr.elmId.elmnt)
       {
          case STSZGEN:
               uehSzCfg |= UEH_SZ_GEN_CFG;
               break;      
          case STSZPROT:
               uehSzCfg |= UEH_SZ_PROT_CFG;
               break;
          case STSZSZTSAP:
               uehSzCfg |= UEH_SZ_SZT_SAP_CFG;
               break;
          case STSZSCTSAP:
               uehSzCfg |= UEH_SZ_SCT_SAP_CFG;
               break;
          case STSZPEER:
               uehSzCfg |= UEH_SZ_PEER_CFG;
               break;
          default:
            logError("S1AP Config Confirm received for invalid element:%d",cfm->hdr.elmId.elmnt);
            break;
       } /* end of switch statement */

       logInfo("S1AP Config Confirm received for the element:%d",cfm->hdr.elmId.elmnt);
       if (uehSzCfg == UEH_SM_SZ_CONFIGURED)
       {
          logInfo("%s", "S1AP CONFIGURATION SUCCESSFULLY PERFORMED");

          smCb.smState = UEH_SM_STATE_S1AP_CFG_DONE;
          UEH_FILL_PST(szPst, SM_SZ_PROC, ENTSM, ENTUEH, EVTUEHEGTPCFG, UEH_POOL, SM_SELECTOR_LC);
          if(uehSmSelfPstMsg(&szPst) != ROK)
          {
             logError("%s", "Error in posting the message to SM for Configuring EGTP");
          }
          else
          {
            logInfo("%s", "Message to SM sent for Configuring EGTP");
          }
       }
   }
   else
   {
     logError("Configuration Failure received from S1AP for element:%d reason:%d", cfm->hdr.elmId.elmnt, cfm->cfm.reason);
   }

   RETVALUE(ROK);
} /* end of SmMiLszCfgCfm */

/*
 *
 *       Fun:    SmMiLszCntrlCfm - control confirm
 *
 *       Desc:   prints the control confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_sz_ptmi.c
 *
 */
PUBLIC S16 SmMiLszCntrlCfm(
  Pst         *pst,
  SzMngmt     *cntrl
  )
{
   Pst                          szPst;

   U8                           trcMsgId;

   SM_SET_ZERO(&szPst, sizeof(Pst));

   switch(cntrl->hdr.elmId.elmnt)
   {
     case STSZSCTSAP:
     {
        if( SM_S1AP_SCTSAP_START_TRACE_TRANS_ID == cntrl->hdr.transId )
        {
          uehSzTrcRspCount++;
          if (cntrl->cfm.status == LCM_PRIM_OK)
          {
             uehSzTrcCtrl |= S1AP_SCTSAP_TRC_ENABLED;
             logInfo("%s", "TRACE is ENABLED in S1AP for STSZSCTSAP");
          }
          else
          {
            logError("%s", "TRACE ENABLE in S1AP FAILED for STSZSCTSAP");
          }
        }
        else if (SM_S1AP_SCTSAP_STOP_TRACE_TRANS_ID == cntrl->hdr.transId )
        {
          uehSzTrcRspCount++;
          if (cntrl->cfm.status == LCM_PRIM_OK)
          {
            uehSzTrcCtrl &= S1AP_SCTSAP_TRC_DISABLED;
            logInfo("%s", "TRACE is DISABLED in S1AP for STSZSCTSAP" );
          }
          else
          {
           logError("%s", "TRACE DISABLE in S1AP FAILED for STSZSCTSAP" );
          }
        }
        else
        {
           logInfo("%s", "BIND OF S1AP WITH SCTP IS IN PROGRESS");
        }
        break;
     }
     case STSZGEN:
     {
       if (cntrl->cfm.status == LCM_PRIM_OK)
       {
         switch(cntrl->hdr.transId)
         {
           case SM_S1AP_DEBUG_TRANS_ID:
           {
             logInfo("%s", "Debug is enabled for S1AP");
             smCb.cfgCb.smDbgMsk = (smCb.cfgCb.smDbgMsk | UEH_SM_SZ_DBG_MASK);
             break;
           }
           case SM_S1AP_ALARM_TRANS_ID:
           {
             logInfo("%s", "Alarm is received from S1AP");
             break;
           }
           case SM_S1AP_PEERDEL_TRANS_ID:
           {
             logInfo("%s", "Peer Deletion transaction is received from S1AP");
             break;
           }
           case SM_S1AP_LSAPBND_TRANS_ID:
           {
             logInfo("%s", "Lower SAP bind is received from S1AP");
             break;
           }
           default:
           {
             logError("General Control confirm received from S1AP for unknown Trans Id:%d", cntrl->hdr.transId );
             break;
           }
         }
       }
       else
       {
         logError("General Control Fail received from S1AP with, Action:%d, Subaction:%d",
                      cntrl->u.cntrl.action, cntrl->u.cntrl.subAction );
       }
       break;
     }
     case STSZPEER:
     {
          if( SM_S1AP_PEER_START_TRACE_TRANS_ID == cntrl->hdr.transId )
          {
            uehSzTrcRspCount++;
            if (cntrl->cfm.status == LCM_PRIM_OK)
            {
               uehSzTrcCtrl |= S1AP_PEER_TRC_ENABLED;
               logInfo("%s", "TRACE is ENABLED in S1AP for STSZPEER");
            }
            else
            {
              logError("%s", "TRACE ENABLE in S1AP FAILED for STSZPEER");
            }
          }
          else if (SM_S1AP_PEER_STOP_TRACE_TRANS_ID == cntrl->hdr.transId )
          {
            uehSzTrcRspCount++;
            if (cntrl->cfm.status == LCM_PRIM_OK)
            {
              uehSzTrcCtrl &= S1AP_PEER_TRC_DISABLED;
              logInfo("%s", "TRACE is DISABLED in S1AP for STSZPEER" );
            }
            else
            {
             logError("%s", "TRACE DISABLE in S1AP FAILED for STSZPEER" );
            }
          }
          else if (SM_S1AP_PEERDEL_TRANS_ID == cntrl->hdr.transId )
          {
            if (cntrl->cfm.status == LCM_PRIM_OK)
            {
              logProc("SUCC|S1AP-PEER-DEL|transId=%d", cntrl->hdr.transId);
            }
          }
          else
          {
            logProc("FAIL|S1AP-PEER-DEL|transId=%d", cntrl->hdr.transId);
            logError("Rx|PEER Control Fail|transId=%d|status=%d|reason=%d", 
                     cntrl->hdr.transId, cntrl->cfm.status, cntrl->cfm.reason);
          }
          break;
     }
     default:
     {
       if(cntrl->cfm.status == LCM_PRIM_OK)
       {
         logInfo("Control confirm received from S1AP for Element:%d, Action:%d, Subaction:%d",
             cntrl->hdr.elmId.elmnt, cntrl->u.cntrl.action, cntrl->u.cntrl.subAction );
       }
       else
       {
         logError("Control fail received from S1AP for Element:%d, Action:%d, Subaction:%d",
             cntrl->hdr.elmId.elmnt, cntrl->u.cntrl.action, cntrl->u.cntrl.subAction );
       }
       break;
     }
   }

   /* We need two replies from S1AP stack before sending response */
   if( uehSzTrcRspCount == S1AP_MAX_TRACE_CFM )
   {
     MsgSmLthTraceCntrlCfm      trcCfm;
     if( (S1AP_TRC_ENABLED ==uehSzTrcCtrl ) ||
         (S1AP_TRC_DISABLED ==uehSzTrcCtrl) )
     {
       trcCfm.traceCtrlConfirm = TRUE;
     }
     else
     {
       trcCfm.traceCtrlConfirm = FALSE;
     }
     trcCfm.protocolName = TRACECONFIGMOD_S1AP;

     if( S1AP_TRC_ENABLED ==uehSzTrcCtrl ) {
         logError("%s","senging trace start succ from S1AP\n");
         trcMsgId = EVT_SM_LTH_TRC_START_SUCC;
     }
     else
         trcMsgId = EVT_SM_LTH_TRC_STOP_SUCC;

     if( RFAILED == sendSmMsgToLth( trcMsgId, sizeof(MsgSmLthTraceCntrlCfm), (U8*)&trcCfm) )
     {
       logError("%s", "Error in sending S1AP TRACE CFM to BSM" );
     }
     else
     {
       logInfo("%s", "Sent S1AP TRACE CFM to LTH" );
     }

     uehSzTrcRspCount = 0;
   }

   RETVALUE(ROK);
} /* end of SmMiLszCntrlCfm() */

/*
 *
 *       Fun:    SmMiLszStaInd - status indication
 *
 *       Desc:   Received the Status Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_sz_ptmi.c
 *
 */

PUBLIC S16 SmMiLszStaInd(
  Pst         *pst,
  SzMngmt      *staInd
  )
{
   Pst          szPst;
   SzUsta       *usta= NULLP;
   SzUstaDgn    *dgn;
   MsgS1apBsmConfigSucc s1apCfgSucc;
   MsgS1apBsmConfigSucc s1apCfgFail;

   logFunc("%s","SmMiLszStaInd");

   SM_SET_ZERO(&szPst, sizeof(Pst));
   usta = &(staInd->u.usta);
   dgn = &(usta->dgn);

   if (usta->alarm.event == LSZ_EVENT_LI_ASSOC_CFM  
      && usta->alarm.cause == LSZ_CAUSE_ASSOC_UP 
      )
   {
      logInfo("%s","S1AP ASSOCIATION SUCCESSFULLY ESTABLISHED");
   }
   else if (usta->alarm.category == LCM_CATEGORY_INTERFACE)
   {
      if (usta->alarm.event == LCM_EVENT_BND_OK)
      {
        logInfo("BIND OF S1AP lsap id:%d IS SUCCESSFUL", dgn->u.sapId);
        s1apCfgSucc.transId = smCb.s1apTransId;
        smBindUehToSztSap();
      }
      else if (usta->alarm.event == LCM_EVENT_BND_FAIL) 
      {
         logError("BIND OF S1AP lsap id:%d FAILED", dgn->u.sapId);
         s1apCfgFail.transId = smCb.s1apTransId;
      }
      else if (usta->alarm.event == LSZ_EVENT_LI_TERM_IND)
      {
         logInfo("Term Indication Recvd. lsap id:%d FAILED", dgn->u.sapId);
           /* Updating Stack Manager's state */
         smCb.smState = UEH_SM_STATE_AWAIT_S1_CON;
      }
   } /* end of LCM_CATEGORY_INTERFACE */

   RETVALUE(ROK);
} /* end of SmMiLszStaInd() */

/*
 *
 *       Fun:    SmMiLszTrcInd - trace indication
 *
 *       Desc:   Received the Trace Indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_sz_ptmi.c
 *
 */

PUBLIC S16 SmMiLszTrcInd(
  Pst          *pst,
  SzMngmt      *trcInd,
  Buffer       *mBuf
  )
{
   logFunc("%s","SmMiLszTrcInd");

   MsgLen               trcLen;
   MsgSmLthTrcInd       msgSmLthTrcInd;

   /* CCPU trace structure for S1AP is |szMgmt Header|S1apPdu|
    * find the length of remaining structure,
    * Copy the PDU to  msgS1apLthTrcInd,
    * Add Time stamp, and send to LTH
    */
   logInfo("%s","Trace Indication received from S1AP" );

   SM_SET_ZERO( &msgSmLthTrcInd, sizeof(MsgSmLthTrcInd));
   SFndLenMsg(mBuf, &trcLen );
   if( trcLen >= MAX_LEN_TRACE_MSG )
   {
     logError("Jumbo Trace packet received from S1 of Size:%d",trcLen );
     trcLen = MAX_LEN_TRACE_MSG;
   }
   msgSmLthTrcInd.traceLen = trcLen;
   msgSmLthTrcInd.protocolName = TRACECONFIGMOD_S1AP;
   if( trcLen > 0 )
   {
     SRemPreMsgMult((Data *)&(msgSmLthTrcInd.traceMsg), trcLen, mBuf);
     clock_gettime(CLOCK_REALTIME, &(msgSmLthTrcInd.timeVal) );
     if( RFAILED == sendSmMsgToLth( EVT_SM_LTH_TRC_IND, sizeof(MsgSmLthTrcInd), (U8*)&msgSmLthTrcInd) )
     {
       logError("%s", "Error in sending S1AP TRACE IND to LTH" );
     }
     else
     {
       logInfo("%s", "Sent S1AP TRACE IND to LTH" );
     }
   }
   RETVALUE(ROK);
} /* end of SmMiLszStaInd() */

/*
 *
 *       Fun:    SmMiLszStaCfm - status confirm
 *
 *       Desc:   Received the Status Confirm
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_sz_ptmi.c
 *
 */


PUBLIC S16 SmMiLszStaCfm
(
Pst         *pst,
SzMngmt      *staCfm
)
{
   logFunc("%s","SmMiLszStaCfm");

   logInfo ("%s", "Received S1AP StaCfm");

   RETVALUE(ROK);
} /* end of SmMiLszStaCfm() */


S16 smGetS1apPeerSts(
    SzMngmt              *s1apStsCfm,
    S1apPeerNodeCounters *rsp
    )
{
  SzPeerNodeSts         *nodeSts;

  if(s1apStsCfm->hdr.elmId.elmnt != STSZPEER)
  {
    logError("Peer STATS are handled, Unknown element received:%d", s1apStsCfm->hdr.elmId.elmnt);
    return RFAILED;
  }
  nodeSts = &(s1apStsCfm->u.sts.u.node);

  rsp->peerId = nodeSts->peerId.val;
  rsp->nmbUnexpEvt = nodeSts->cfm.nmbUnexpEvt;
  rsp->nmbTRafExp = nodeSts->cfm.nmbTRafExp;
  rsp->eRABSetupReq = nodeSts->cfm.pdu.eRABSetupReq.rx;
  rsp->eRABSetupResp = nodeSts->cfm.pdu.eRABSetupResp.tx;
  rsp->eRABModifyRequest = nodeSts->cfm.pdu.eRABModResp.rx;
  rsp->eRABModResp = nodeSts->cfm.pdu.eRABModResp.tx;
  rsp->eRABRelCmd = nodeSts->cfm.pdu.eRABRelCmd.rx;
  rsp->eRABRelComp = nodeSts->cfm.pdu.eRABRelCmd.tx;
  rsp->eRABRelReq = nodeSts->cfm.pdu.eRABRelReq.tx;
  rsp->initCxtSetup = nodeSts->cfm.pdu.initCxtSetup.rx;
  rsp->initCxtSetupResp = nodeSts->cfm.pdu.initCxtSetupResp.tx;
  rsp->initCxtSetupFail = nodeSts->cfm.pdu.initCxtSetupFail.tx;
  rsp->ueCxtRelReq = nodeSts->cfm.pdu.ueCxtRelReq.tx;
  rsp->ueCxtRelCmd = nodeSts->cfm.pdu.ueCxtRelCmd.rx;
  rsp->ueCxtRelComp = nodeSts->cfm.pdu.ueCxtRelComp.tx;
  rsp->ueCxtModReq = nodeSts->cfm.pdu.ueCxtModReq.rx;
  rsp->ueCxtModResp = nodeSts->cfm.pdu.ueCxtModResp.tx;
  rsp->ueCxtModFail = nodeSts->cfm.pdu.ueCxtModFail.tx;
  rsp->initUEMsg = nodeSts->cfm.pdu.initUEMsg.tx;
  rsp->dwnNASTpt = nodeSts->cfm.pdu.dwnNASTpt.rx;
  rsp->upNASTpt = nodeSts->cfm.pdu.upNASTpt.tx;
  rsp->nonDelIndNAS = nodeSts->cfm.pdu.nonDelIndNAS.tx;
  rsp->errIndtx = nodeSts->cfm.pdu.errInd.tx;
  rsp->errIndrx = nodeSts->cfm.pdu.errInd.rx;
  rsp->noDatatx = nodeSts->cfm.pdu.noData.tx;
  rsp->noDatarx = nodeSts->cfm.pdu.noData.rx;
  rsp->resettx = nodeSts->cfm.pdu.reset.tx;
  rsp->resetrx = nodeSts->cfm.pdu.reset.rx;
  rsp->resetAcktx = nodeSts->cfm.pdu.resetAck.tx;
  rsp->resetAckrx = nodeSts->cfm.pdu.resetAck.rx;
  rsp->setupReq = nodeSts->cfm.pdu.setupReq.tx;
  rsp->setupResp = nodeSts->cfm.pdu.setupResp.rx;
  rsp->setupFail = nodeSts->cfm.pdu.setupFail.rx;
  rsp->paging = nodeSts->cfm.pdu.paging.rx;
  rsp->handReqd = nodeSts->cfm.pdu.handReqd.tx;
  rsp->handCmd = nodeSts->cfm.pdu.handCmd.rx;
  rsp->handPrepFail = nodeSts->cfm.pdu.handPrepFail.rx;
  rsp->handReq = nodeSts->cfm.pdu.handReq.rx;
  rsp->handReqAck = nodeSts->cfm.pdu.handReqAck.tx;
  rsp->handFail = nodeSts->cfm.pdu.handFail.tx;
  rsp->handNtfy = nodeSts->cfm.pdu.handNtfy.tx;
  rsp->pathSwchReq = nodeSts->cfm.pdu.pathSwchReq.tx;
  rsp->pathSwchReqAck = nodeSts->cfm.pdu.pathSwchReqAck.rx;
  rsp->handCan = nodeSts->cfm.pdu.handCan.tx;
  rsp->handCanAck = nodeSts->cfm.pdu.handCanAck.rx;
  rsp->eNBStaTrans = nodeSts->cfm.pdu.eNBStaTrans.tx;
  rsp->mmeStaTrans = nodeSts->cfm.pdu.mmeStaTrans.rx;
  rsp->deactTrace = nodeSts->cfm.pdu.deactTrace.rx;
  rsp->traceStart = nodeSts->cfm.pdu.traceStart.rx;
  rsp->traceFailInd = nodeSts->cfm.pdu.traceFailInd.tx;
  rsp->locReportCntrl = nodeSts->cfm.pdu.locReportCntrl.rx;
  rsp->locReportFailInd = nodeSts->cfm.pdu.locReportFailInd.tx;
  rsp->locReport = nodeSts->cfm.pdu.locReport.tx;
  rsp->enbCfgUpd = nodeSts->cfm.pdu.enbCfgUpd.tx;
  rsp->enbCfgUpdAck = nodeSts->cfm.pdu.enbCfgUpdAck.rx;
  rsp->enbCfgUpdFail = nodeSts->cfm.pdu.enbCfgUpdFail.rx;
  rsp->mmeCfgUpd = nodeSts->cfm.pdu.mmeCfgUpd.rx;
  rsp->mmeCfgUpdAck = nodeSts->cfm.pdu.mmeCfgUpdAck.tx;
  rsp->mmeCfgUpdFail = nodeSts->cfm.pdu.mmeCfgUpdFail.tx;
  rsp->upS1CDMA2000Tunn = nodeSts->cfm.pdu.upS1CDMA2000Tunn.tx;
  rsp->dnS1CDMA2000Tunn = nodeSts->cfm.pdu.dnS1CDMA2000Tunn.rx;
  rsp->ueCapInfoInd = nodeSts->cfm.pdu.ueCapInfoInd.tx;
  rsp->overloadStart = nodeSts->cfm.pdu.overloadStart.rx;
  rsp->overloadStop = nodeSts->cfm.pdu.overloadStop.rx;
  rsp->writeRepWarnReq = nodeSts->cfm.pdu.writeRepWarnReq.rx;
  rsp->writeRepWarnRsp = nodeSts->cfm.pdu.writeRepWarnRsp.tx;
  rsp->enbDirInfoTrans = nodeSts->cfm.pdu.enbDirInfoTrans.tx;
  rsp->mmeDirInfoTrans = nodeSts->cfm.pdu.mmeDirInfoTrans.rx;
  rsp->privMsgtx = nodeSts->cfm.pdu.privMsg.tx;
  rsp->privMsgrx = nodeSts->cfm.pdu.privMsg.rx;
  rsp->eNBConfigTfr = nodeSts->cfm.pdu.eNBConfigTfr.tx;
  rsp->mmeConfigTfr = nodeSts->cfm.pdu.mmeConfigTfr.rx;
  rsp->cellTrafficTrace = nodeSts->cfm.pdu.cellTrafficTrace.tx;
  rsp->szDedProcStstx = nodeSts->cfm.szDedProcSts.tx;
  rsp->szDedProcStsrx = nodeSts->cfm.szDedProcSts.rx;
  rsp->szComProcStstx  = nodeSts->cfm.szComProcSts.tx;
  rsp->szComProcStsrx  = nodeSts->cfm.szComProcSts.rx;
 
  return ROK;
}

S16 smGetS1apGenSts(
    SzMngmt           *s1apStsCfm,
    S1apGenCounters   *rsp
    )
{
  SzGenSts      *gen;

  if(s1apStsCfm->hdr.elmId.elmnt != STSZGEN)
  {
    logError("Gen STATS are handled, Unknown element received:%d", s1apStsCfm->hdr.elmId.elmnt);
    return RFAILED;
  }
  gen = &(s1apStsCfm->u.sts.u.gen);

  rsp->nmbConn     = gen->nmbConn;
  rsp->nmbFailConn = gen->nmbFailConn;
  rsp->maxMemAlloc = gen->maxMemAlloc;
  rsp->nmbDecErr   = gen->nmbDecErr;
  rsp->nmbEncErr   = gen->nmbEncErr;
  rsp->nmbAudReq   = gen->audSts.nmbAudReq;
  rsp->nmbAudConn  = gen->audSts.nmbAudConn;
  rsp->nmbAudPeer  = gen->audSts.nmbAudPeer;
  rsp->nmbAudFlc   = gen->audSts.nmbAudFlc;
  
  return ROK;
}
/*
 *
 *       Fun:    SmMiLszStsCfm - statistics confirm
 *
 *       Desc:   Received the Statistics Confirm
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_sz_ptmi.c
 *
 */

PUBLIC S16 SmMiLszStsCfm(
    Pst         *pst,
    SzMngmt      *stsCfm
  )
{
   logFunc("%s","SmMiLszStsCfm");
   logInfo("%s","STATS Confirm received from S1AP" );

   MsgSmPmhS1apGenStatRsp       s1apGenNodeStatRsp ;
   MsgSmPmhS1apPeerNodeStatRsp  s1apPeerNodeStatRsp ;

   (void)cmMemset((U8 *) &(s1apGenNodeStatRsp), 0, sizeof(MsgSmPmhS1apGenStatRsp));
   (void)cmMemset((U8 *) &(s1apPeerNodeStatRsp), 0, sizeof(MsgSmPmhS1apPeerNodeStatRsp));

   if( STSZGEN == stsCfm->hdr.elmId.elmnt)
   {
     if(ROK != smGetS1apGenSts(stsCfm,&s1apGenNodeStatRsp.s1apGenCounters) )
     {
       logError("%s", "Error in Extracting received S1AP General STATS Message");
       RETVALUE(RFAILED);
     }
     s1apGenNodeStatRsp.transId = stsCfm->hdr.transId;

     if( RFAILED == sendSmMsgToPmh( EVT_SM_PMH_S1AP_GEN_STAT_RSP, sizeof(MsgSmPmhS1apGenStatRsp), (U8*)&s1apGenNodeStatRsp) )
     {
         logError("%s", "Error in sending S1AP General STATS to PMH" );
     }
     else
     {
         logInfo("%s", "Sent S1AP General STATS to PMH" );
     }
   }
   else if(STSZPEER == stsCfm->hdr.elmId.elmnt )
   {
     if(ROK != smGetS1apPeerSts(stsCfm,&s1apPeerNodeStatRsp.s1apPeerNodeCounters) )
     {
       logError("%s", "Error in Extracting received S1AP Peer STATS Message");
       RETVALUE(RFAILED);
     }
     s1apPeerNodeStatRsp.transId = stsCfm->hdr.transId;

     if( RFAILED == sendSmMsgToPmh( EVT_SM_PMH_S1AP_PEER_NODE_STAT_RSP, sizeof(MsgSmPmhS1apPeerNodeStatRsp), (U8*)&s1apPeerNodeStatRsp) )
     {
         logError("%s", "Error in sending S1AP Peer STATS to PMH" );
     }
     else
     {
         logInfo("%s", "Sent S1AP Peer STATS to PMH" );
     }
   }
   else
   {
     logError("Unknown Stats of type:%d received from S1AP", stsCfm->hdr.elmId.elmnt );
   }

   RETVALUE(ROK);
} /* end of SmMiLszStsCfm() */
