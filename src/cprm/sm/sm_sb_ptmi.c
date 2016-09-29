/**
 * @file  sm_sb_ptmi.c
 * @brief This file contains management interface functions for SCTP
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. @author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 14Sep2010 :
*     a) Created initial version from sample application
 */

/* header include files (.h) */
#include "ccpu_cmn.h"

/* RRM Related Files */
#include "ueh.h"
#include "log.h"
#include "bsm_sctp_if.h"
#include "bs_tables.h"
#include "ueh_pmh_if.h"
#include "pst_events.h"

/* local defines */

#define MAXSBMI                2

#ifndef LCSMSBMILSB
#define PTSMSBMILSB
#else
#ifndef SB
#define PTSMSBMILSB
#else
#endif
#endif

PRIVATE U8    uehSbCfg = 0;
PUBLIC S16 smGetSctpSts(
  SbMgmt          *sctpStsCfm,
  SctpGenCounters *rsp
  );
/* local externs */

/* forward references */
#ifdef PTSMSBMILSB
PRIVATE S16 PtMiLsbCfgReq   ARGS((Pst *pst, SbMgmt *cfg ));
PRIVATE S16 PtMiLsbStaReq   ARGS((Pst *pst, SbMgmt *sta ));
PRIVATE S16 PtMiLsbStsReq   ARGS((Pst *pst, Action action, SbMgmt *sts ));
PRIVATE S16 PtMiLsbCntrlReq ARGS((Pst *pst, SbMgmt *cntrl ));
#endif

/* functions in other modules */

/* public variable declarations */

/* private variable declarations */


/*
The following matrices define the mapping between the primitives
called by the layer management interface of SCTP layer and the corresponding
primitives in the driver.

The parameter MAXSBMI defines the maximum number of layer manager entities
on top of the driver. There is an array of functions per primitive insbked by
the driver. Every array is MAXSBMI long (i.e. there are as many functions as the
number of service users).

The dispatching is performed by the configurable variable: selector.
The selector is configured during general configuration.

The selectors are:

   0 - loosely coupled (#define LCSMSBMILSB)
   1 - Lsb (#define SB)

*/

/* Configuration request primitive */

PRIVATE LsbCfgReq SmMiLsbCfgReqMt[MAXSBMI] =
{
#ifdef LCSMSBMILSB
   cmPkLsbCfgReq,        /* 0 - loosely coupled - bc */
#else
   PtMiLsbCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbMiLsbCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLsbCfgReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Statistics request primitive */

PRIVATE LsbStsReq SmMiLsbStsReqMt[MAXSBMI] =
{
#ifdef LCSMSBMILSB
   cmPkLsbStsReq,        /* 0 - loosely coupled - bc */
#else
   PtMiLsbStsReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbMiLsbStsReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLsbStsReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Status request primitive */

PRIVATE LsbStaReq SmMiLsbStaReqMt[MAXSBMI] =
{
#ifdef LCSMSBMILSB
   cmPkLsbStaReq,        /* 0 - loosely coupled - bc */
#else
   PtMiLsbStaReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbMiLsbStaReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLsbStaReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Control request primitive */

PRIVATE LsbCntrlReq SmMiLsbCntrlReqMt[MAXSBMI] =
{
#ifdef LCSMSBMILSB
   cmPkLsbCntrlReq,        /* 0 - loosely coupled - bc */
#else
   PtMiLsbCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbMiLsbCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLsbCntrlReq,          /* 1 - tightly coupled, portable */
#endif
};

/*
*     layer management interface functions
*/

/*
*
*       Fun:   Configuration request
*
*       Desc:  This function is used to configure SCTP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_sb_ptmi.c
*
*/


PUBLIC S16 SmMiLsbCfgReq
(
Pst *pst,                 /* post structure */
SbMgmt *cfg              /* configure */
)
{
   logFunc("%s","SmMiLsbCfgReq");
   /* jump to specific primitive depending on configured selector */
   (*SmMiLsbCfgReqMt[pst->selector])(pst, cfg);
   RETVALUE(ROK);
} /* end of SmMiLsbCfgReq */

/*
*
*       Fun:   Status request
*
*       Desc:  This function is used to send a status request to SCTP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_sb_ptmi.c
*
*/


PUBLIC S16 SmMiLsbStaReq
(
Pst *pst,                 /* post structure */
SbMgmt *sta              /* status */
)
{
   logFunc("%s","SmMiLsbStaReq");
   /* jump to specific primitive depending on configured selector */
   (*SmMiLsbStaReqMt[pst->selector])(pst, sta);
   RETVALUE(ROK);
} /* end of SmMiLsbStaReq */

/*
*
*       Fun:   Statistics request
*
*       Desc:  This function is used to request statistics from SCTP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_sb_ptmi.c
*
*/


PUBLIC S16 SmMiLsbStsReq
(
Pst *pst,                 /* post structure */
Action action,
SbMgmt *sts              /* statistics */
)
{
   logFunc("%s","SmMiLsbStsReq");
   /* jump to specific primitive depending on configured selector */
   (*SmMiLsbStsReqMt[pst->selector])(pst, action, sts);
   RETVALUE(ROK);
} /* end of SmMiLsbStsReq */

/*
*
*       Fun:   Control request
*
*       Desc:  This function is used to send control request to SCTP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_sb_ptmi.c
*
*/


PUBLIC S16 SmMiLsbCntrlReq
(
Pst *pst,                 /* post structure */
SbMgmt *cntrl            /* control */
)
{
   logFunc("%s","SmMiLsbCntrlReq");
   /* jump to specific primitive depending on configured selector */
   (*SmMiLsbCntrlReqMt[pst->selector])(pst, cntrl);
   RETVALUE(ROK);
} /* end of SmMiLsbCntrlReq */


#ifdef PTSMSBMILSB
/*
*
*       Fun:   Portable configure Request SCTP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_sb_ptmi.c
*
*/


PRIVATE S16 PtMiLsbCfgReq
(
Pst *pst,                   /* post structure */
SbMgmt *cfg                /* configure */
)
{
   logFunc("%s","PtMiLsbCfgReq");
   UNUSED(pst);
   UNUSED(cfg);
   RETVALUE(ROK);
} /* end of PtMiLsbCfgReq */


/*
*
*       Fun:   Portable status Request SCTP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_sb_ptmi.c
*
*/

PRIVATE S16 PtMiLsbStaReq
(
Pst *pst,                   /* post structure */
SbMgmt *sta                /* status */
)
{
   logFunc("%s","PtMiLsbStaReq");
   UNUSED(pst);
   UNUSED(sta);
   RETVALUE(ROK);
} /* end of PtMiLsbStaReq */


/*
*
*       Fun:   Portable statistics Request SCTP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_sb_ptmi.c
*
*/

PRIVATE S16 PtMiLsbStsReq
(
Pst *pst,                   /* post structure */
Action action,
SbMgmt *sts                /* statistics */
)
{
   logFunc("%s","PtMiLsbStsReq");
   UNUSED(pst);
   UNUSED(action);
   UNUSED(sts);
   RETVALUE(ROK);
} /* end of PtMiLsbStsReq */


/*
*
*       Fun:   Portable control Request SCTP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:     ueh_sm_sb_ptmi.c
*
*/

PRIVATE S16 PtMiLsbCntrlReq
(
Pst *pst,                   /* post structure */
SbMgmt *cntrl              /* control */
)
{
   logFunc("%s","PtMiLsbCntrlReq");
   UNUSED(pst);
   UNUSED(cntrl);
   RETVALUE(ROK);
} /* end of PtMiLsbCntrlReq */

#endif

/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Manager.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  ueh_sm_sb_ptmi.c
*
*/

PUBLIC S16 SmMiLsbCfgCfm(
  Pst     *pst,          /* post structure */
  SbMgmt *cfm           /* configuration */
  )
{
   Pst  sbPst;

   logFunc("%s","SmMiLsbCfgCfm");

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
     switch(cfm->hdr.elmId.elmnt)
     {
        case STSBGEN:
           uehSbCfg |=  UEH_SB_GEN_CFG;
           break;
        case STSBTSAP:
           uehSbCfg |=  UEH_SB_TSAP_CFG;
           break;
        case STSBSCTSAP:
           uehSbCfg |=  UEH_SB_SCT_SAP_CFG;
           break;
        default:
           logError("SCTP Config confirm received with invalid element:%d",cfm->hdr.elmId.elmnt);
           break; 
     }

      logInfo("SCTP Config confirm received for element:%d",cfm->hdr.elmId.elmnt);

      if (uehSbCfg == UEH_SM_SB_CONFIGURED)
      {
         logInfo("%s", "SCTP CONFIGURATION SUCCESSFULLY PERFORMED");
         /* Initiate S1AP configuration after SCTP configuration. 
            So send EVTUEHS1APCFG */
         smCb.smState = UEH_SM_STATE_SCTP_CFG_DONE;
         UEH_FILL_PST(sbPst, SM_SB_PROC, ENTSM, ENTUEH, EVTUEHS1APCFG, UEH_POOL, SM_SELECTOR_LC);
         uehSmSelfPstMsg(&sbPst); 
      }
   }
   else
   {
     logError("Configuration Failure received from SCTP for element:%d",cfm->hdr.elmId.elmnt);
   }
   RETVALUE(ROK);
}

/*
 *
 *       Fun:    SmMiLsbCntrlCfm - control confirm
 *
 *       Desc:   prints the control confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_sb_ptmi.c
 *
 */

PUBLIC S16 SmMiLsbCntrlCfm(
    Pst         *pst,
    SbMgmt     *cntrl
  )
{
   Pst  sbPst;
   MsgSctpBsmInitConfigSucc sctpCfgSucc;
   MsgSctpBsmInitConfigFail sctpCfgFail;

   clearBytes( &sctpCfgSucc, sizeof(MsgSctpBsmInitConfigSucc));
   clearBytes( &sctpCfgFail, sizeof(MsgSctpBsmInitConfigFail));

   logFunc("%s","SmMiLsbCntrlCfm");

   SM_SET_ZERO(&sbPst, sizeof(Pst));

   switch( cntrl->hdr.elmId.elmnt )
   {
     case STSBTSAP:
     {
        if (cntrl->cfm.status == LCM_PRIM_OK)
        {
          logInfo("%s", "BIND OF SCTP TSAP WITH TUCL SUCCESSFUL");
          /* Send SCTP Config Success */
          sctpCfgSucc.transId = smCb.sctpTransId;
           /* Initiate the bind control request for S1AP and SCTP Saps */
           smBindSzToLSap();
        }
        else if (cntrl->cfm.status == LCM_PRIM_OK_NDONE)
        {
           logInfo("%s", "Bind of SCTP TSap with TUCL is in progress");
        }
        else
        {
          logError("%s","Bind of SCTP TSap with TUCL failed");
          sctpCfgFail.transId = smCb.sctpTransId;
        }
        break;
     }
     case STSZGEN:
     {
       if (cntrl->cfm.status == LCM_PRIM_OK)
       {
         switch(cntrl->hdr.transId)
         {
           case SM_SCTP_DEBUG_TRANS_ID:
           {
             logInfo("%s", "Debug is enabled for SCTP");
             smCb.cfgCb.smDbgMsk = (smCb.cfgCb.smDbgMsk | UEH_SM_SB_DBG_MASK);
             break;
           }
           default:
           {
             logError("General Control confirm received from SCTP for unknown Subaction:%d", cntrl->t.cntrl.subAction );
             break;
           }
         }
       }
       else
       {
         logError("General Control Fail received from SCTP with, Action:%d, Subaction:%d",
                      cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
       }
       break;
     }
     default:
     {
       if(cntrl->cfm.status == LCM_PRIM_OK)
       {
         logInfo("Control confirm received from SCTP for Element:%d, Action:%d, Subaction:%d",
             cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
       }
       else
       {
         logError("Control fail received from SCTP for Element:%d, Action:%d, Subaction:%d",
             cntrl->hdr.elmId.elmnt, cntrl->t.cntrl.action, cntrl->t.cntrl.subAction );
       }
       break;
     }
   }


   RETVALUE(ROK);
} /* end of SmMiLsbCntrlCfm() */


/**
 * This Function will get the counter statistics in second argument
 * and it is a callback function of sfiPrepSctpStsCfm.
 * In this function copy all the SCTP Counters to Local Structure, Further transmitted to PMH
 */
PUBLIC S16 smGetSctpSts(
  SbMgmt          *sctpStsCfm,
  SctpGenCounters *rsp
  )
{
  if(sctpStsCfm->hdr.elmId.elmnt != STSBGEN)
  {
    logError("Only General STATS are handled, Unknown element received:%d", sctpStsCfm->hdr.elmId.elmnt);
    return RFAILED;
  }
  rsp->noInitTx       = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noInitTx;
  rsp->noInitReTx     = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noInitReTx;
  rsp->noInitRx       = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noInitRx;
  rsp->noIAckTx       = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noIAckTx;
  rsp->noIAckRx       = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noIAckRx;

  rsp->noShDwnTx      = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noShDwnTx;
  rsp->noShDwnReTx    = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noShDwnReTx;
  rsp->noShDwnRx      = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noShDwnRx;
  rsp->noShDwnAckTx   = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noShDwnAckTx;
  rsp->noShDwnAckReTx = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noShDwnAckReTx;
  rsp->noShDwnAckRx   = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noShDwnAckRx;

  rsp->noCookieTx     = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noCookieTx;
  rsp->noCookieReTx   = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noCookieReTx;
  rsp->noCookieRx     = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noCookieRx;
  rsp->noCkAckTx      = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noCkAckTx;
  rsp->noCkAckRx      = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noCkAckRx;

  rsp->noDataTx       = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noDataTx;
  rsp->noDataReTx     = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noDataReTx;
  rsp->noDataRx       = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noDataRx;
  rsp->noDAckTx       = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noDAckTx;
  rsp->noDAckRx       = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noDAckRx;

  rsp->noShDwnCmpltTx = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noShDwnCmpltTx;
  rsp->noShDwnCmpltRx = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noShDwnCmpltRx;
  
  rsp->noDataReTx     = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noDataReTx;
  rsp->noDataRx       = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noDataRx;
  rsp->noDAckTx       = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noDAckTx;
  rsp->noDAckRx       = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noDAckRx;
#ifdef LSB2
  rsp->noHBeatTx      = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noHBeatTx;
  rsp->noHBeatRx      = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noHBeatRx;
  rsp->noHBAckTx      = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noHBAckTx;
  rsp->noHBAckRx      = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noHBAckRx;
#endif
#ifdef LSB3
  rsp->noAbortTx      = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noAbortTx;
  rsp->noAbortRx      = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noAbortRx;
#endif
#ifdef LSB6
  rsp->noErrorTx      = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noErrorTx;
  rsp->noErrorRx      = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noErrorRx;
#endif
  /* TODO: need to enable SB_ECN */
#ifdef SB_ECN
  rsp->noEcneTx       = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noEcneTx;
  rsp->noEcneReTx     = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noEcneReTx;
  rsp->noEcneRx       = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noEcneRx;
  rsp->noCwrTx        = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noCwrTx;
  rsp->noCwrRx        = sctpStsCfm->t.sts.u.genSts.sbChunkSts.noCwrRx;
#endif /* SB_ECN */

  rsp->noQueryTx      = sctpStsCfm->t.sts.u.genSts.sbDnsSts.noQueryTx;
  rsp->noQueryReTx    = sctpStsCfm->t.sts.u.genSts.sbDnsSts.noQueryReTx;
  rsp->noQueryRspRx   = sctpStsCfm->t.sts.u.genSts.sbDnsSts.noQueryRspRx;

  return (ROK);
}

/*
 *
 *       Fun:    SmMiLsbStsCfm - statistics confirm
 *
 *       Desc:   prints the statistics confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_sb_ptmi.c
 *
 */

PUBLIC S16 SmMiLsbStsCfm(
    Pst         *pst,
    SbMgmt     *sts
  )
{
  logFunc("%s","SmMiLsbStsCfm");
  logInfo("%s","STATS Confirm received from SCTP" );

  MsgSmPmhSctpGenStatRsp    rsp;
  (void)cmMemset((U8 *) &(rsp), 0, sizeof(MsgSmPmhSctpGenStatRsp));

  /* CCPU statistics structure for SCTP is sbMgmt|SctpPdu
   * First unpack SbMgmt, Copy the general,
   * Send the Message to PMH
   */
  if(ROK != smGetSctpSts(sts,&rsp.sctpGenCounters) )
  {
    logError("%s", "Error in Extracting received SCTP STATS Message");
    RETVALUE(RFAILED);
  }
  rsp.transId = sts->hdr.transId;

  if( RFAILED == sendSmMsgToPmh( EVT_SM_PMH_SCTP_GEN_STAT_RSP, sizeof(MsgSmPmhSctpGenStatRsp), (U8*)&rsp) )
  {
      logError("%s", "Error in sending SCTP STATS to PMH" );
  }
  else
  {
      logInfo("%s", "Sent SCTP STATS to PMH" );
  }

  RETVALUE(ROK);
}

/*
 *
 *       Fun:    SmMiLsbStaCfm - status confirm
 *
 *       Desc:   prints the status confirm 
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_sb_ptmi.c
 *
 */

PUBLIC S16 SmMiLsbStaCfm
(
Pst         *pst,
SbMgmt     *sta
)
{
   logFunc("%s","SmMiLsbStaCfm");

   RETVALUE(ROK);
} /* end of SmMiLsbStaCfm() */

/*
 *
 *       Fun:    SmMiLsbStaInd - status indication
 *
 *       Desc:   prints the status indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_sb_ptmi.c
 *
 */

PUBLIC S16 SmMiLsbStaInd
(
Pst         *pst,
SbMgmt     *ind
)
{
   logFunc("%s","SmMiLsbStaInd");

   RETVALUE(ROK);
} /* end of SmMiLsbStaInd() */

/*
 *
 *       Fun:    SmMiLsbTrcInd - trace indication
 *
 *       Desc:   prints the trace indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   ueh_sm_sb_ptmi.c
 *
 */

PUBLIC S16 SmMiLsbTrcInd
(
Pst         *pst,
SbMgmt     *trc
)
{
   logFunc("%s","SmMiLsbTrcInd");

   RETVALUE(ROK);
} /* end of SmMiLsbTrcInd() */
