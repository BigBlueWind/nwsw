/**
 * @file    udh_main.c
 * @brief This file contains the entry point function of User Data Handler Module
 * @details Copyright Powerwave Technologies, Inc., 2011
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Bhogeswara, Madhu, 27Aprl2011 :
 *    a) Created initial version
 */
 
#include "udh_includes.h"

/** Global Context of UDH Module */
UdhCb       g_udhCb;

/** Functions **/

/** Entry point/Handling of incoming events from PRM  **/
void udhHandlePrmEvents(
  Pst      *pst,
  Buffer   *mBuf
  )
{
  logEventIn(pst, 0, "%s", __func__);
  switch (pst->event)
  {
    case EVT_HELLO:
      sendUdhHelloAckToPrm(mBuf);
      SPutMsg(mBuf);
      break;

    case EVT_EXIT:
      SPutMsg(mBuf);
      exit(0);
      break;

    case EVT_START_QUERY:
      sendUdhStartIndToPrm();
      SPutMsg(mBuf);
      break;

    default:
      BS_ERR_UNKNOWN_EVENT(pst);
      SPutMsg(mBuf);
      break;

  } /* switch (pst->event) */
}

/** Entry point/Handling of incoming events from BSM  **/
void udhHandleBsmEvents(
  Pst      *pst,
  Buffer   *mBuf
  )
{
  logEventIn(pst, 0, "%s", __func__);
  switch (pst->event)
  {
    case EVT_BSM_UDH_OPSTART:
    {
      udhProcOpStartReq();
      SPutMsg(mBuf);
      break;
    }
    case EVT_BSM_UDH_OPSTOP_CMD:
    {
      udhProcOpStopReq();
      SPutMsg(mBuf);
      break;
    }
    case EVT_BSM_UDH_CFG_REQ:
    {
      udhProcBsmUdhCfgReq(mBuf);
      SPutMsg(mBuf);
      break;
    }
    case EVT_BSM_UDH_ACTIVE_RAB_STATUS_REQ:
    {
      udhProcActiveRabStatusReq(mBuf);
      SPutMsg(mBuf);
      break;
    }
    case EVT_BSM_UDH_MOD_STATE_REQ:
    {
      udhProcModStateReq(mBuf);
      SPutMsg(mBuf);
      break;
    }
    case EVT_BSM_UDH_RAB_HISTORY_REQ:
    {
      udhProcRabHistoryReq(mBuf);
      SPutMsg(mBuf);
      break;
    }
    default:
    {
      BS_ERR_UNKNOWN_EVENT(pst);
      (Void)SPutMsg(mBuf);
      break;
    }
  } /* switch (pst->event) */
}

/** Entry point/Handling of incoming events from PMH  **/
void udhHandlePmhEvents(
  Pst      *pst,
  Buffer   *mBuf
  )
{
  switch (pst->event)
  {
    case EVT_PMH_UDH_DSTAT_REQ:
      logInfo("%s","udhHandlePmhEvents : EVT_PMH_UDH_DSTAT_REQ Received from PMH");
      udhProcDstatReq(mBuf);
      SPutMsg(mBuf);
      break;

    default:
      BS_ERR_UNKNOWN_EVENT(pst);
      SPutMsg(mBuf);
      break;

  } /* switch (pst->event) */
}

/** Entry point/Handling of incoming events from SM  **/
void udhHandleSmEvents(
  Pst      *pst,
  Buffer   *mBuf
  )
{
  S16 ret = ROK;

  logEventIn(pst, 0, "%s", __func__);
  switch (pst->event)
  {
    case EVT_SM_UDH_SAP_CFG_REQ:
    {
      CmSapCfg  sapCfg;
      cmUnpkSmUdhSapCfg(&sapCfg, mBuf);
      ret = udhProcSapCfgReq(pst, &sapCfg);
      logEventIn(pst, 0, "for SAP-ID %d return-value %d",sapCfg.sapId, ret);
      break;
    }
    case EVT_SM_UDH_SAP_BIND_REQ:
    {
      CmSapBindReq  cmSapBindReq;
      cmUnpkSmUdhBndReq(mBuf, &cmSapBindReq);
      ret = udhProcSmUdhBndReq(pst, &cmSapBindReq);
      logEventIn(pst, 0, "for SAP-ID %d return-value %d",cmSapBindReq.sapId,ret);
      break;
    }
    case EVT_SM_UDH_DBG_CNTRL_REQ:
    {
      SmUdhLogConfig  smUdhLogConfig;
      clearBytes(&smUdhLogConfig, sizeof(SmUdhLogConfig));
      cmUnPkAndSendSmUdhDbgCntrl(mBuf, &smUdhLogConfig);
      logInfo("EVT_SM_UDH_DBG_CNTRL_REQ: received from SM with logMask1=%d" , smUdhLogConfig.logMask1);
      ret = udhProcSmUdhDbgCntrl(&smUdhLogConfig);
      logInfo("Processing of event: EVT_SM_UDH_DBG_CNTRL_REQ is returned:%d",ret);
      break;
    }
    default:
    {
      BS_ERR_UNKNOWN_EVENT(pst);
      (Void)SPutMsg(mBuf);
      break;
    }
  } /* switch (pst->event) */
}

/**
 * Entry point/Handles incoming events from GTP
 */
void udhHandleGtpEvents(
    Pst     *pst,
    Buffer  *mBuf
    )
{
  Pst *udhUehPst = getUdhUehPst();

  logEventIn(pst, 0, "%s", __func__);
  switch (pst->event)
  {
    case EVTEGTUDATIND:
    {
      (Void)cmUnpkEgtEguDatInd(EuLiEgtEguDatInd, pst, mBuf);
      break;
    }
    
    case EVTEGTBNDCFM:
    {
      (Void)cmUnpkEgtBndCfm(EuLiEgtBndCfm, pst, mBuf);
      break;
    }
    case EVTEGTUERRIND:
    {
      /* TODO: why silent discard ?*/
      SPutMsg(mBuf);
      break;
    }
    case EVTEGTULCLTNLMGMTCFM:
    case EVTEGTUSTACFM:  /* TODO: when will it come, what to do */
    case EVTEGTUUSTAIND: /* TODO: when will it come, what to do */
    {
    /* UDH only relays this message to UEH */
      //logEventIn(pst, 0, "pool-%d, region-%d",pst->pool, pst->region);
      udhUehPst->pool   = pst->pool;
      udhUehPst->region = pst->region;
      udhUehPst->event  = pst->event;
      SPstTsk(udhUehPst, mBuf);
      break;
    }
    default:
    {
      //logError("udhHandleGtpEvents(): Unknown event=%d from EGT, dropped", pst->event);
      BS_ERR_UNKNOWN_EVENT(pst);
      (Void)SPutMsg(mBuf);
      break;
    }
  }
}
/**
 * Entry point/Handles incoming events from UEH
 */
void udhHandleUehEvents(
  Pst    *pst,
  Buffer *mBuf
  )
{
  Pst *udhGtpPst = getUdhGtpPst();

  logEventIn(pst, 0, "%s", __func__);
  switch (pst->event)
  {
    /* Handle events for which UDH acts as relay */
    case  EVTEGTULCLTNLMGMTREQ:
    {
      udhGtpPst->event = pst->event;
      SPstTsk(udhGtpPst, mBuf);
      break;
    }
    /* Handle non-relay events */
    case EVT_UEH_UDH_ADD_RAB_REQ:
    {
      (Void)cmUnpkUehUdhAddRabReq(udhProcUehUdhAddRabReq, pst , mBuf );
      break;
    }
    case EVT_UEH_UDH_DEL_RAB_REQ:
    {
      (Void)cmUnpkUehUdhDelRabReq(udhProcUehUdhDelRabReq, pst, mBuf);
      break;
    }
    case EVT_UEH_UDH_DEL_UE_REQ:
    {
      (Void)cmUnpkUehUdhDelUeReq(udhProcUehUdhDelUeReq, pst, mBuf);
      break;
    }
    case EVT_UEH_UDH_BND_REQ:
    {
      cmUnpkUehUdhBndReq(udhProcUehUdhBndReq, pst, mBuf );
      break;
    }
    default:
    {
      BS_ERR_UNKNOWN_EVENT(pst);
      (Void)SPutMsg(mBuf);
      break;
    }
  } /* switch (pst->event) */
} /* ENTUEH */

/**
 * Handles incoming events from PDCP
 */
void udhHandlePdcpEvents(
  Pst    *pst,
  Buffer *mBuf
  )
{
  logEventIn(pst, 0, "%s", __func__);
  switch (pst->event)
  {
    case  EVTPJUDATIND:
    {
      cmUnpkPjuDatInd(udhProcPjuDatInd, pst, mBuf); /* mBuf is not freed but passed to GTPU */
      break;
    }
    case EVTPJUBNDCFM:
    {
      cmUnpkPjuBndCfm(udhProcPjuBndCfm, pst, mBuf);
      break;
    }
    default:
    {
      BS_ERR_UNKNOWN_EVENT(pst);
      (Void)SPutMsg(mBuf);
      break;
    }
  }
}

/* Fun   :  udhActvTsk
 * Desc  :  Active Task Function of 'User Data Handler' TAPA Task
 * Ret   :  ROK     - succeeded
 *          RFAILED - failed
 */

PUBLIC S16 udhActvTsk (
  Pst      *pst,
  Buffer   *mBuf
)
{
  switch (pst->srcEnt)
  {
    case ENTEG:    udhHandleGtpEvents(pst, mBuf); break;   /* GTP */
    case ENTPJ:    udhHandlePdcpEvents(pst, mBuf); break;  /* PDCP */
    case ENTUEH:   udhHandleUehEvents(pst, mBuf); break;   /* UEH */
    case ENT_PRM:  udhHandlePrmEvents(pst, mBuf); break;   /* PRM */
    case ENT_BSM:  udhHandleBsmEvents(pst, mBuf); break;   /* BSM */
    case ENTSM:    udhHandleSmEvents(pst, mBuf); break;    /* Stack Manager */
    case ENT_PMH:  udhHandlePmhEvents(pst, mBuf); break;    /* PMH */
    default:
      BS_ERR_UNKNOWN_EVENT(pst);
      RETVALUE(RFAILED);
  }
  RETVALUE(ROK);
}
