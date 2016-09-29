/**
 * @file    ueh_ex_ms.c
 * @brief This file contains the UEH external interface functions
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amar Ramteke, 16Sep2010 :
*     a) Created initial version from sample application
 */
 
/* header include files (.h) */
#include "ccpu_cmn.h"

/* RRM related includes */
#include "ueh_eut.h"           /* MAC RRM control Interface */
#include "ueh_udh_if.h"
#include "lueh.h"
#include "ueh_eut_x.h"           /* MAC RRM control Interface */
#include "ueh.h"
#include "lueh.x" 
#include "s1ap_mgr.h"
#include "ueh_crm_sm.h"
#include "ueh_srs_res_mgmt.h"
#include "ueh_crm.h"
#include "pst_events.h"
#include "lrg.x"
#include "lpj.x"
#include "lkw.x"
#include "sm.h"

#ifdef LTE_L2_MEAS
EXTERN S16 cmUnpkLrgSchL2MeasCfm ( LrgSchL2MeasCfm func, Pst *pst,Buffer *mBuf);
EXTERN S16 cmUnpkLkwL2MeasCfm ( LkwL2MeasCfm func, Pst *pst, Buffer *mBuf);
EXTERN S16 cmUnpkLpjL2MeasCfm ( LpjL2MeasCfm func, Pst *pst, Buffer *mBuf);
#endif
extern void reportError( const char *format, ... );
#ifdef SS_HISTOGRAM_SUPPORT
    #include "pwav_memlkdtec.h"
#endif

#ifdef UEH_LNX_PCAP_PERF
#include "ueh_rout.x"
#endif
#ifdef PWAV_TS
#include "ts.h"
#endif


//U8 memInfoPrintFlag = 0;
extern unsigned g_allocReq, g_bytesAllocated, g_bytesFreed;
#ifdef PWAV_TS
extern S16 cmUnpkSetTsRsp (CtfSetTsRsp func, Pst *pst,  Buffer *mBuf);
#endif
/*
*
*       Fun:    uehActvTsk
*
*       Desc:   Active Task Function of 'UE Handler' TAPA Task
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
     File:   ueh_ex_ms.c
*
*/

PUBLIC S16 uehActvTsk
(
Pst      *pst,
Buffer   *mBuf
)
{
  Mem sMem = {.region = uehCb.init.region, .pool = UEH_MEM_POOL};

  switch (pst->srcEnt)
  {
      case ENT_PRM:
        switch (pst->event)
        {
          case EVT_HELLO:
            sendUehHelloAckToPrm(mBuf);

            SPutMsg(mBuf);
            break;
          case EVT_EXIT:
            procRcvdUehExitMsg();
            SPutMsg(mBuf);
            break;
          case EVT_START_QUERY:
            sendUehStartIndToPrm();
            /* Setting DBG-MASK to FFF.. at the start-up */
            uehCb.init.dbgMask = 0xffffffff;
            SPutMsg(mBuf);
            break;
        }
        break;

      case ENT_BSM:
        switch (pst->event)
        {
          case EVT_BSM_S1M_CFG_REQ:
            s1mProcS1mCfgReq(mBuf);
            SPutMsg(mBuf);
            break;

          case EVT_BSM_S1M_OPSTART:
            s1apProcOpStartReq(mBuf);
            SPutMsg(mBuf);
            break;

          case EVT_BSM_S1M_OPSTOP_CMD:
            s1apProcOpStopReq(mBuf);
            SPutMsg(mBuf);
            break;

          case EVT_BSM_CEM_CFG_REQ:
            cemProcEnbCfgReq(mBuf,pst);
            SPutMsg(mBuf);
            break;

          case EVT_BSM_CEM_OPSTART:
            crmInvokeCellSm(uehGetCellCb(), CELL_EVT_OP_START, NULL);
            SPutMsg(mBuf);
            break;

          case EVT_BSM_CEM_OPSTOP_CMD:
             cemProcOpstopCmd(mBuf);
             SPutMsg(mBuf);
          break;

          case EVT_BSM_UEH_ACTIVE_UE_STATUS_REQ:
             uehProcActiveUeStatusReq(mBuf);
             SPutMsg(mBuf);
             break;

          case EVT_BSM_UEH_UE_EVENT_HISTORY_REQ:
            uehProcUeEventHistoryStatusReq(mBuf);
            SPutMsg(mBuf);
            break;

          case EVT_BSM_UEH_MOD_STATE_REQ:
            uehProcModStateReq(mBuf);
            SPutMsg(mBuf);
            logCtr("PUCCHAudit=%d|UeStateAudit=%d|UeProcAudit=%d",\
               uehCb.uehCtxHangDetection.pucchAuditCounter,  
               uehCb.uehCtxHangDetection.ueStateAuditCounter,
               uehCb.uehCtxHangDetection.ueProcedureAuditCounter);

            logCtr("nRrcConnReq=%d|nRrcConnSetup=%d|nRrcConnSetupCmpl=%d|nRrcConnRej=%d|nRrcConnRel=%d|nRrcCtrChkReq=%d", \
               uehCb.uehCounters[EVENTTYPE_RRCCONNREQ],
               uehCb.uehCounters[EVENTTYPE_RRCCONNSETUP],
               uehCb.uehCounters[EVENTTYPE_RRCCONNSETUPCOMPL],
               uehCb.uehCounters[EVENTTYPE_RRCCONNREJ],
               uehCb.uehCounters[EVENTTYPE_RRCCONNRELEASE],
               uehCb.uehCounters[EVENTTYPE_RRCCOUNTERCHECKREQ]);
            logCtr("nRrcReconfReq=%d|nRrcReconfCmpl=%d|nRrcReEstabRej=%d|nSecModeCmd=%d|nSecModeCmpl=%d|nSecModeFail=%d", \
               uehCb.uehCounters[EVENTTYPE_RRCCONNRECONFIG],
               uehCb.uehCounters[EVENTTYPE_RRCCONNRECONFCOMPL],
               uehCb.uehCounters[EVENTTYPE_RRCREESTABREJ],
               uehCb.uehCounters[EVENTTYPE_SECMODECOMMAND],
               uehCb.uehCounters[EVENTTYPE_SECMODECOMPL],
               uehCb.uehCounters[EVENTTYPE_SECMODEFAIL]
            );
            logCtr("nInitCtxtSetReq=%d|nInitCtxtSetRsp=%d|nInitCtxtSetFail=%d|nInitUeMsg=%d", \
               uehCb.uehCounters[EVENTTYPE_INITCONTEXTSETUPREQ],
               uehCb.uehCounters[EVENTTYPE_INITCONTEXTSETUPRESP],
               uehCb.uehCounters[EVENTTYPE_INITCONTEXTSETUPFAIL],
               uehCb.uehCounters[EVENTTYPE_INITIALUEMSG]
               );
            logCtr("nUeCtxtRelCmd=%d|nUeCtxtRelCmpl=%d|nUeCapEnq=%d|nUeCapInfo=%d|nUeCapInfoInd=%d", \
               uehCb.uehCounters[EVENTTYPE_UECONTEXTRELCMD],
               uehCb.uehCounters[EVENTTYPE_UECONTEXTRELCOM],
               uehCb.uehCounters[EVENTTYPE_UECAPABILITYENQ],
               uehCb.uehCounters[EVENTTYPE_UECAPABILITYINFO],
               uehCb.uehCounters[EVENTTYPE_UECAPABILITYINFOIND]
               );
            logCtr("nDlNasTrfMsg=%d|nUlNasTrfMsg=%d|nUeCtxtRelReq=%d|nSrb1TmrExp=%d|nUeDel=%d",\
                    uehCb.uehCounters[EVENTTYPE_DLNASTRANSPORTMSG],
                    uehCb.uehCounters[EVENTTYPE_ULNASTRANSPORTMSG],
                    uehCb.uehCounters[EVENTTYPE_UECONTEXTRELREQ],
                    uehCb.uehCounters[EVENTTYPE_SRB1TMREXP],
                    uehCb.uehCounters[EVENTTYPE_UEDELETED]
                  );

          {
             UehCellCb *cellCb = uehGetCellCb();  
             if (cellCb) logCtr("RxTtiInd=%u|sfn=%u|mibsTx=%u", 
               cellCb->tickCount,
               cellCb->sfn,
               cellCb->numMibsTx);
          }
#ifdef PWAV_MEM2
            printPmTable(&g_blockBufTable, 0);
            printPmTable(&g_msgBufTable, 0);
            printPmTable(&g_staticBufTable, 0);
#else
 #warn "PWAV_MEM2 not defined"
#endif
            break;
          //BS-1799
          /*Cell barring procedure*/
          case EVT_BSM_CEM_CELL_BAR_REQ:
            cemProcCellBarReq(mBuf);
            SPutMsg(mBuf);
            break;         
          case EVT_BSM_CEM_CELL_UNBAR_REQ:
            cemProcCellUnbarReq(mBuf);
            SPutMsg(mBuf);
            break;         
          case EVT_BSM_UEH_UEEVENTHISTORYLOG_CONFIG_REQ:
            uehProcUeHistoryCfgReq(mBuf);
            SPutMsg(mBuf);
            break;
          default:
            BS_ERR_UNKNOWN_EVENT(pst);
            (Void)SPutMsg(mBuf);
            break;
        }
      break; /* ENT_BSM */

      case ENTSM:
        switch (pst->event)
        {
          case EVTLUEHCFGREQ:
            cmUnpkLuehCfgReq(UehMiLuehCfgReq, pst, mBuf);
            break;
          case EVTLUEHCNTRLREQ:
            cmUnpkLuehCntrlReq(UehMiLuehCntrlReq, pst, mBuf);
            break;

#ifdef LTE_L2_MEAS
          case EVTLUEHMACL2MEASCFM: 
            cmUnpkLrgSchL2MeasCfm (uehMacL2MeasCfm, pst, mBuf);
            break;
          case EVTLUEHRLCL2MEASCFM:
            cmUnpkLkwL2MeasCfm(uehRlcL2MeasCfm, pst, mBuf);
            break;
          case EVTLUEHPDCPL2MEASCFM:
            cmUnpkLpjL2MeasCfm (uehPdcpL2MeasCfm, pst, mBuf);
            break;
#endif
          default:
            BS_ERR_UNKNOWN_EVENT(pst);
            (Void)SPutMsg(mBuf);
            break;
          }
      break; /* ENTSM */

    case ENTNH: 
      switch(pst->event)
      {
        case EVTNHUBNDCFM:
          cmUnpkNhuBndCfm(NxLiNhuBndCfm, pst, mBuf);
          /* Memory is not allocated for EVTNHUBNDCFM, free is not required */
          break;

        case EVTNHUERRIND:
          cmUnpkNhuErrInd(NxLiNhuErrInd, pst, mBuf);
          /*  Memory is allocated in unpacking function and freed in NxLiNhuErrInd */
          break;

        case EVTNHUDATIND:
          cmUnpkNhuDatInd(NxLiNhuDatInd, pst, mBuf,&sMem, UEH_MEM_PDU_SIZE);
          /* Memory is allocated in unpacking function and freed in NxLiNhuDatInd */
          break;

        case EVTNHUCFGCFM:
          cmUnpkNhuCfgCfm(NxLiNhuCfgCfm, pst, mBuf);
          /* Memory is allocated in unpacking function and freed in NxLiNhuCfgCfm */
          break;

        case  EVTNHUCELLCFGCFM :
          cmUnpkNhuCellCfgCfm(NxLiNhuCellCfgCfm, pst, mBuf);
          /* Memory is allocated in unpacking function and freed in NxLiNhuCellCfgCfm */
          break;
        case EVTNHUSTAIND:
          cmUnpkNhuStaInd(NxLiNhuStaInd, pst, mBuf);
          break;

        default:
          BS_ERR_UNKNOWN_EVENT(pst);
          (Void)SPutMsg(mBuf);
          break;
      }
      break;

    case ENTRG:
      switch (pst->event)
      {
        case EVTRGRBNDCFM:
          cmUnpkRgrBndCfm(NxLiRgrBndCfm, pst, mBuf);
          /* Memory is not allocated for EVTRGRBNDCFM, free is not required */
          break;

#ifdef RGR_SI_SCH
        case EVTRGRSICFGCFM:
#endif
        case EVTRGRCFGCFM:
          cmUnpkRgrCfgCfm(NxLiRgrCfgCfm, pst, mBuf);
          /* Memory is not allocated for EVTRGRCFGCFM/EVTRGRSICFGCFM , free is not required */
          break;

#ifdef RGR_RRM_TICK
        case EVTRGRTTIIND: /* Indication every 10ms */
          cmUnpkRgrTtiInd(NxLiRgrTtiInd, pst, mBuf);
          break;
#endif
          case EVTRGRUESTAIND:
          cmUnpkRgrUeStaInd(NxLiRgrUeStaInd, pst, mBuf);
          break;

          case EVTRGRSTAIND:
          cmUnpkRgrStaInd(NxLiRgrStaInd, pst, mBuf);
          break;

        default:
          BS_ERR_UNKNOWN_EVENT(pst);
          (Void)SPutMsg(mBuf);
          break;
      } 
      break;

    case ENTTF:
      switch (pst->event)
      {
        case EVTCTFBNDCFM:
          cmUnpkCtfBndCfm (UehLiCtfBndCfm, pst, mBuf);
          /* Memory is not allocated for EVTCTFBNDCFM, free is not required */
          break;
#ifndef RRC_PHY_CFG_SUPP
        case EVTCTFCFGCFM:
          cmUnpkCtfCfgCfm (UehLiCtfCfgCfm, pst, mBuf);
          /* Memory is not allocated for EVTCTFCFGCFM, free is not required */
          break;
#endif
#ifdef PWAV_TS
        case EVTCTFUEHSETTSRSP:
          cmUnpkSetTsRsp (UehLiCtfSetTsRsp, pst, mBuf);
          break;
#endif
/***************SEC_CHANGE_START*****************/
        case EVTCTFKDFCFM:
          cmUnpkCtfKdfCfm(uehLiCtfDatRsp, pst, mBuf);
          break;			   
/***************SEC_CHANGE_END*****************/
        default:
          BS_ERR_UNKNOWN_EVENT(pst);
          (Void)SPutMsg(mBuf);
          break;
      } 
      break;

      case ENTSZ:
      switch (pst->event)
      {
        case EVTSZTBNDCFM:
          (Void)cmUnpkSztBndCfm(UzLiSztBndCfm, pst, mBuf);
          /* Memory is not allocated for EVTSZTBNDCFM, free is not required */
          break;

        case EVTSZTSTAIND:
          (Void)cmUnpkSztStaInd(UzLiSztStaInd, pst, mBuf);
          /* Memory is not allocated for EVTSZTSTAIND, free is not required */
          break;

        case EVTSZTERRIND:
          /* Memory is not allocated for EVTSZTSTAIND, free is not required */
          (Void)cmUnpkSztLclErrInd(UzLiSztErrInd, pst, mBuf);
          break;

        case EVTSZTUDATIND:
          (Void)cmUnpkSztUDatInd(UzLiSztUDatInd, pst, mBuf, &sMem);
          /* Memory is allocated in unpacking function and freed in UzLiSztUDatInd */
          break;

        case EVTSZTCONIND:
          /* TODO: Need to check when eNodeB is getting this message */
          (Void)cmUnpkSztConInd(UzLiSztConInd, pst, mBuf, &sMem);
          break;

        case EVTSZTCONCFM:
          (Void)cmUnpkSztConCfm(UzLiSztConCfm, pst, mBuf, &sMem);
          /* Memory is allocated in unpacking function and freed in UzLiSztConCfm */
          break;

        case EVTSZTRELIND:
          (Void)cmUnpkSztRelInd(UzLiSztRelInd, pst, mBuf, &sMem);
          /* Memory is allocated in unpacking function and freed in UzLiSztRelInd */
          break;

        case EVTSZTRELCFM:
          /* TODO: Need to check when eNodeB is getting this message */
          (Void)cmUnpkSztRelCfm(UzLiSztRelCfm, pst, mBuf, &sMem);
          /* Memory is allocated in unpacking function and freed in UzLiSztRelCfm */
          break;

        case EVTSZTDATIND:
          (Void)cmUnpkSztDatInd(UzLiSztDatInd, pst, mBuf, &sMem);
          /* Memory is allocated in unpacking function and freed in UzLiSztDatInd */
          break;

        case EVTSZTABORTS1CFM:
          (Void)cmUnpkSztAbortS1Cfm(UzLiSztAbortS1Cfm, pst, mBuf);
          break;

        default:
          BS_ERR_UNKNOWN_EVENT(pst);
          (Void)SPutMsg(mBuf);
          break;
      }
      break;

    case ENTUDH:
      switch (pst->event)
      {
        case EVTEGTULCLTNLMGMTCFM:
          (Void)cmUnpkEgtEguLclTnlMgmtCfm(EuLiEgtEguLclTnlMgmtCfm, pst, mBuf);
          /* Memory is allocated in unpacking function and freed in EuLiEgtEguLclTnlMgmtCfm */
          break;

        case EVTEGTUERRIND:
           (Void)cmUnpkEgtEguErrInd(EuLiEgtEguErrInd, pst, mBuf);
           /* Memory is allocated in unpacking function and freed in EuLiEgtEguErrInd */
           break;

        case EVTEGTUSTACFM:
           (Void)cmUnpkEgtEguStaCfm(EuLiEgtEguStaCfm, pst, mBuf);
           /* Memory is allocated in unpacking function and freed in EuLiEgtEguStaCfm */
           break;

        case EVTEGTUUSTAIND:
           (Void)cmUnpkEgtEguUStaInd(EuLiEgtEguUStaInd, pst, mBuf);
           /* Memory is allocated in unpacking function and freed in EuLiEgtEguUStaInd */
           break;

        case EVT_UDH_UEH_BND_CFM:
          cmUnpkUdhUehBndCfm(UehLiUdhBndCfm, pst, mBuf);
          /* Memory is not allocated for EVTSZTSTAIND, free is not required */
          break;

        case EVT_UDH_UEH_INACTIVE_UES_IND:
          cmUnpkUeInactiveInd(UehLiUdhUeInactvInd, pst, mBuf);
          /* Memory is not allocated for EVTSZTSTAIND, free is not required */
          break;

        case EVT_UDH_UEH_ADD_RAB_CFM:
          cmUnpkAddRabCfm(UehLiUdhAddRabCfm, pst, mBuf);
          /* Memory is not allocated for EVTSZTSTAIND, free is not required */
          break;

        case EVT_UDH_UEH_DEL_RAB_CFM:
          cmUnpkDelRabCfm(UehLiUdhDelRabCfm, pst, mBuf);
          /* Memory is not allocated for EVTSZTSTAIND, free is not required */
          break;

        case EVT_UDH_UEH_DEL_UE_CFM:
          cmUnpkDelUeCfm(UehLiUdhDelUeCfm, pst, mBuf);
          break;

        case EVT_UDH_UEH_UL_INACTIVE_IND:
          cmUnpkUlInactiveInd(UehLiUdhUlInactvInd, pst, mBuf);
          /* Memory is not allocated for EVTSZTSTAIND, free is not required */
          break;
        default:
          BS_ERR_UNKNOWN_EVENT(pst);
          (Void)SPutMsg(mBuf);
            break;
      }
      break;

      case ENTUEH:
        switch (pst->event)
        {
           case EVT_S1M_LINK_UP_IND:
            crmInvokeCellSm( uehGetCellCb(), CELL_EVT_S1_LINK_UP, NULL );
            SPutMsg(mBuf);
            break;
          case EVT_S1M_LINK_DOWN_IND:
            crmInvokeCellSm( uehGetCellCb(), CELL_EVT_S1_LINK_DOWN, NULL );
            SPutMsg(mBuf);
            break;
          default:
          BS_ERR_UNKNOWN_EVENT(pst);
          (Void)SPutMsg(mBuf);
              break;
        }
        break;
      case ENT_PMH:
        switch (pst->event)
        {
          case EVT_PMH_UEH_RRM_CELL_STAT_REQ:
             uehProcRrmCellCounter (mBuf);
             SPutMsg(mBuf);
             break;
#ifdef LTE_L2_MEAS

          case EVT_PMH_UEH_COUNTER_START_REQ:
             uehProcCounterStartReq (mBuf);
             SPutMsg(mBuf);
             break;
          case EVT_PMH_UEH_COUNTER_STOP_REQ:
             uehProcCounterStopReq (mBuf);
             SPutMsg(mBuf);
             break;
#endif /* LTE_L2_MEAS */
          default :
          BS_ERR_UNKNOWN_EVENT(pst);
          (Void)SPutMsg(mBuf);
            break;
        }

        break;

      default:
       BS_ERR_UNKNOWN_EVENT(pst);
       (Void)SPutMsg(mBuf);
      break;
  }

  RETVALUE(ROK);
} /* end of uehActvTsk() */

