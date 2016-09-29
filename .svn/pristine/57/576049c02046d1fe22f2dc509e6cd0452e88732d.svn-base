/**
 * @file    ueh_l2m.c
 * @brief This file contains L2 measurement handling functions
 * @details Copyright Powerwave Technologies, Inc., 2010
 *
 * #. Author, Date: Change
 * ------------------------
 * 1. Amt Gulia, 30Aug2012 :
 */

#include "ccpu_cmn.h"
#include "ueh_pmh_if.h"
#include "lueh.h"
#include "lueh.x" 
#include "ueh.h" 
#include "ueh_crm.h"
#include "log.h"
#include "rgr.x"
#include "lrg.x"
#include "lkw.h"
#include "ctf.x"
#include "ueh_crm_sm.h"
#include "pst_events.h"

#ifdef LTE_L2_MEAS

PUBLIC S16 uehMacL2MeasReq(UehL2MeasProfileCfg  *profileCfg, CounterLink  *counter)
{
    LrgSchMeasReqInfo  measCfgInfo;
    Pst                pst;
    Buffer            *mBuf = NULL;

    logInfo("l2m: uehMacL2MeasReq identity[%d] ", counter->identity);
   
    if (SGetMsg(pst.region, pst.pool, &mBuf) != ROK) 
    {
      RETVALUE(RFAILED);
    }       
    
    UEH_MEM_COPY(&pst,&uehCb.init.lmPst,sizeof(Pst));
    pst.event = EVTUEHMACL2MEASREQ;

    UehPrepLrgL2MeasReq(&measCfgInfo, profileCfg, counter);

    measCfgInfo.hdr.response.selector = pst.selector;
    measCfgInfo.hdr.response.prior = pst.prior;
    measCfgInfo.hdr.response.route = pst.route;
    measCfgInfo.hdr.response.mem.region = pst.region; 
    measCfgInfo.hdr.response.mem.pool = pst.pool;
    
    uehLrgSchL2MeasReq(&measCfgInfo,mBuf);
    
    SPstTsk(&pst, mBuf);
    
    RETVALUE(ROK);  
}

PUBLIC S16 uehRlcL2MeasReq(UehL2MeasProfileCfg  *profileCfg, CounterLink  *counter)
{
    KwL2MeasReqEvt    measReqEvt;
    Pst                pst;
    Buffer            *mBuf = NULL;
    
    logInfo("l2m: uehRlcL2MeasReq identity[%d] ", counter->identity);
   
    if (SGetMsg(pst.region, pst.pool, &mBuf) != ROK) 
    {
      RETVALUE(RFAILED);
    }       
    UEH_MEM_COPY(&pst,&uehCb.init.lmPst,sizeof(Pst));
    pst.event = EVTUEHRLCL2MEASREQ; 
    
    UehPrepKwL2MeasReqEvt(&measReqEvt, profileCfg, counter);
    uehLkwL2MeasReq(&measReqEvt,mBuf);
    
    SPstTsk(&pst, mBuf);
    
    RETVALUE(ROK);  
}

PUBLIC S16 uehPdcpL2MeasReq(UehL2MeasProfileCfg  *profileCfg, CounterLink  *counter)
{
    PjL2MeasReqEvt     measReqEvt;
    Pst                 pst;
    Buffer            *mBuf = NULL;
    
    logInfo("l2m: uehPdcpL2MeasReq identity[%d] ", counter->identity);
   
    if (SGetMsg(pst.region, pst.pool, &mBuf) != ROK) 
    {
      RETVALUE(RFAILED);
    }
    
    UEH_MEM_COPY(&pst,&uehCb.init.lmPst,sizeof(Pst));
    pst.event = EVTUEHPDCPL2MEASREQ; 
    
    UehPrepPjL2MeasReqEvt(&measReqEvt, profileCfg, counter);
    uehLpjL2MeasReq(&measReqEvt, mBuf);
    
    SPstTsk(&pst, mBuf);
    
    RETVALUE(ROK);  
}

void UehPrepLrgL2MeasReq( LrgSchMeasReqInfo   *measCfgInfo, 
                          UehL2MeasProfileCfg *profileCfg, 
                          CounterLink         *link)
{
    U8                       measType = 0;
    U32                      idx;

    logInfo("l2m: UehPrepLrgL2MeasReqEvt seqNo[%d]", profileCfg->sampleSeqNo);
    
    measCfgInfo->timePrd     = getMeasPeriod(profileCfg->samplingIntervalDur);
    measCfgInfo->cellId      = 1;
    measCfgInfo->hdr.transId = profileCfg->sampleSeqNo;
    
    switch (link->identity)
    {
      case UEH_L2M_MASK_TOTAL_PRB_USAGE_DL: 
        measType = LRG_L2MEAS_AVG_PRB_DL;
        break;

      case UEH_L2M_MASK_TOTAL_PRB_USAGE_UL:
        measType = LRG_L2MEAS_AVG_PRB_UL;
        break;

      case UEH_L2M_MASK_RECV_PREAMBLES:
        measType = LRG_L2MEAS_RA_PREAMBLE;
        break;

      case UEH_L2M_MASK_PRB_USAGE_DL_QCI:
        measType = LRG_L2MEAS_AVG_PRB_PER_QCI_DL;
        measCfgInfo->avgPrbQciDl.numQci = ((UehPrbUsageQci *) (link->addr))->numQci;
        for (idx = 0; idx <  measCfgInfo->avgPrbQciDl.numQci; idx++)
        {
          measCfgInfo->avgPrbQciDl.qci[idx] = ((UehPrbUsageQci *) (link->addr))->qciInfo[idx].qciId;
        }
        break;

      case UEH_L2M_MASK_PRB_USAGE_UL_QCI:
        measType = LRG_L2MEAS_AVG_PRB_PER_QCI_UL;
        measCfgInfo->avgPrbQciUl.numQci = ((UehPrbUsageQci *) (link->addr))->numQci;
        for (idx = 0; idx <  measCfgInfo->avgPrbQciUl.numQci; idx++)
        {
          measCfgInfo->avgPrbQciUl.qci[idx] = ((UehPrbUsageQci *) (link->addr))->qciInfo[idx].qciId;
        }
        break;

      case UEH_L2M_MASK_NUM_ACTV_UE_QCI_DL:
        measType = LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL;
        measCfgInfo->nmbActvUeQciDl.sampPrd = ((UehNumActvUesQci *) (link->addr))->l2SamplePeriod;
        measCfgInfo->nmbActvUeQciDl.numQci = ((UehNumActvUesQci *) (link->addr))->numQci;
        for (idx = 0; idx <  measCfgInfo->nmbActvUeQciDl.numQci; idx++)
        {
          measCfgInfo->nmbActvUeQciDl.qci[idx] = ((UehNumActvUesQci *) (link->addr))->qciInfo[idx].qciId;
        }
        break;

      case UEH_L2M_MASK_NUM_ACTV_UE_QCI_UL:
        measType = LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL;
        measCfgInfo->nmbActvUeQciUl.sampPrd = ((UehNumActvUesQci *) (link->addr))->l2SamplePeriod;
        measCfgInfo->nmbActvUeQciUl.numQci = ((UehNumActvUesQci *) (link->addr))->numQci;
        for (idx = 0; idx <  measCfgInfo->nmbActvUeQciUl.numQci; idx++)
        {
          measCfgInfo->nmbActvUeQciUl.qci[idx] = ((UehNumActvUesQci *) (link->addr))->qciInfo[idx].qciId;
        }
        break;

      default:
          logError("l2m: UehPrepLrgL2MeasReqEvt seqNo[%d], invalid identity", profileCfg->sampleSeqNo);
          break; 
    }  
    
    measCfgInfo->measType = measType;
}

void UehPrepKwL2MeasReqEvt( KwL2MeasReqEvt       *measReqEvt, 
                            UehL2MeasProfileCfg  *profileCfg, 
                            CounterLink          *link)
{
    U32                      idx;

    logInfo("l2m: UehPrepKwL2MeasReqEvt seqNo[%d]", profileCfg->sampleSeqNo);

    measReqEvt->transId    = profileCfg->sampleSeqNo;
    measReqEvt->measPeriod = getMeasPeriod(profileCfg->samplingIntervalDur);

    switch (link->identity)
    {
      case UEH_L2M_MASK_AVG_NUM_ACTV_UE_QCI:
        measReqEvt->measReq.measType                   = LKW_L2MEAS_ACT_UE;
        measReqEvt->measReq.val.nonIpThMeas.numSamples = ((UehAvgNumActvUeQci *)(link->addr))->numSamples;
        measReqEvt->measReq.val.nonIpThMeas.numQci     = ((UehAvgNumActvUeQci *)(link->addr))->numQci;

        for (idx = 0; idx < ((UehAvgNumActvUeQci *)(link->addr))->numQci; idx++)
        {
          measReqEvt->measReq.val.nonIpThMeas.qci[idx] = ((UehAvgNumActvUeQci *) (link->addr))->qciInfo[idx].qciId;
        }
        break;
      
      case UEH_L2M_MASK_PKT_LOSS_RATE_DL_QCI:
        measReqEvt->measReq.measType                   = LKW_L2MEAS_UU_LOSS;
        measReqEvt->measReq.val.nonIpThMeas.numSamples = ((UehPktUuLossRateDlQci *)(link->addr))->numSamples;
        measReqEvt->measReq.val.nonIpThMeas.numQci     = ((UehPktUuLossRateDlQci *)(link->addr))->numQci;

        for (idx = 0; idx < ((UehPktUuLossRateDlQci *)(link->addr))->numQci; idx++)
        {
          measReqEvt->measReq.val.nonIpThMeas.qci[idx] = ((UehPktUuLossRateDlQci *) (link->addr))->qciInfo[idx].qciId;
        }
        break;

      default:
        break; 
    }
}

PUBLIC U16 getMeasPeriod(SamplingInterval   interval)
{
  return interval *1000;
}

void UehPrepPjL2MeasReqEvt( PjL2MeasReqEvt       *measReqEvt, 
                            UehL2MeasProfileCfg  *profileCfg, 
                            CounterLink          *link)
{
    U32                      idx;
    
    logInfo("l2m: UehPrepPjL2MeasReqEvt seqNo[%d]", profileCfg->sampleSeqNo);
    
    measReqEvt->transId    = profileCfg->sampleSeqNo;
    measReqEvt->measPeriod = getMeasPeriod(profileCfg->samplingIntervalDur);

    switch (link->identity)
    {
      case UEH_L2M_MASK_PKT_DELAY_DL_QCI:
       
        measReqEvt->measReq.measType   = LPJ_L2MEAS_DL_DELAY;
        measReqEvt->measReq.numQci     = ((UehPktDelayDlQci *)(link->addr))->numQci;

        for (idx = 0; idx < measReqEvt->measReq.numQci; idx++)
        {
          measReqEvt->measReq.qci[idx] = ((UehPktDelayDlQci *)(link->addr))->qciInfo[idx].qciId;
        }
        break;

      case UEH_L2M_MASK_PKT_DISC_RATE_DL_QCI:
       
        measReqEvt->measReq.measType = LPJ_L2MEAS_DL_DISC;
        measReqEvt->measReq.numQci   = ((UehPktDiscRateDlQci *)(link->addr))->numQci;

        for (idx = 0; idx < measReqEvt->measReq.numQci; idx++)
        {
          measReqEvt->measReq.qci[idx] = ((UehPktDiscRateDlQci *) (link->addr))->qciInfo[idx].qciId;
        }
        break;

      case UEH_L2M_MASK_PKT_LOSS_UL_QCI:
       
        measReqEvt->measReq.measType = LPJ_L2MEAS_UL_LOSS;
        measReqEvt->measReq.numQci   = ((UehPktLossUlQci *)(link->addr))->numQci;

        for (idx = 0; idx < measReqEvt->measReq.numQci; idx++)
        {
          measReqEvt->measReq.qci[idx] = ((UehPktLossUlQci *) (link->addr))->qciInfo[idx].qciId;
        }
        break;

      default:
        break;
    }
}

void stopCounterCollection(void)
{
  UehCellCb                  *cellCb = uehGetCellCb();
  UehL2MeasCtx               *uehL2MeasCtx = &cellCb->uehL2MeasCtx;   
  UehL2MeasProfileCfg        *profile = NULL;
  U8                          idx = 0;
  S16                         profileTmrId;
   

  for (idx = 0; idx <MAX_NUM_PROFILES; idx++)
  {
    if (uehL2MeasCtx->profileCfg[idx].status == PROFILE_RUNNING)
    {
      profile = &(uehL2MeasCtx->profileCfg[idx]);

      logInfo("l2m: stopping profile[%d]", profile->profileId);
      if (profile->profileId == PROFILEID_PROFILE1)
      {
        profileTmrId = UEH_L2M_SAMPLE_PROFILE1;
      }
      else
      {
        profileTmrId = UEH_L2M_SAMPLE_PROFILE2;
      }
      uehStopTmr (profileTmrId,&(((UehL2MeasProfileCfg *)profile)->samplingIntervalTmr),(PTR) profile, 0);

      uehSendCounterStopInd(profile, ENODEB_LOCK);
      uehClearL2mProfile(profile);
    }
  }     
}

void uehProcCounterStartReq(
    Buffer *mBuf
  )
{
  MsgLen                      cCnt;
  MsgPmhUehCounterStartReq    counterStartReq;
  MsgUehPmhCounterStartSucc   counterStartSucc;
  UehCellCb                  *cellCb = uehGetCellCb();
  UehL2MeasCtx               *uehL2MeasCtx = &cellCb->uehL2MeasCtx;    
  UehL2MeasProfileCfg        *profileCfg = NULL;

 
  /*Configure the requested profile*/ 
  SCpyMsgFix( mBuf, 0, sizeof(MsgPmhUehCounterStartReq), (Data*)&counterStartReq , &cCnt );
  logInfo("l2m: uehProcCounterStartReq Profile[%d]", counterStartReq.profileId);
  
  profileCfg = uehGetEmptyL2mProfile(uehL2MeasCtx);
  if (profileCfg == NULL)
  {
    logInfo("l2m: Invalid Profile[%d]", counterStartReq.profileId);
    return;
  }    
  uehClearL2mProfile(profileCfg);
  uehConfigureProfile(uehL2MeasCtx, profileCfg, &counterStartReq);

  /*Send counterStartSucc to PMH*/
  clearBytes(&counterStartSucc, sizeof(MsgUehPmhCounterStartSucc));
  counterStartSucc.transId = counterStartReq.transId;
  counterStartSucc.profileId = counterStartReq.profileId;
  sendUehMsgToPmh(EVT_UEH_PMH_COUNTER_START_SUCC, sizeof(MsgUehPmhCounterStartSucc), (UINT8*)&counterStartSucc);

  /*Start the counter collection for the profile*/
  uehStartProfile(profileCfg);

}

UehL2MeasProfileCfg *uehGetEmptyL2mProfile(UehL2MeasCtx *uehL2MeasCtx)    
{
  U8                    profileIndex;  
  UehL2MeasProfileCfg  *profileCfg = NULL;
  
  for (profileIndex = 0; profileIndex < MAX_NUM_PROFILES; profileIndex++)
  {
    if (uehL2MeasCtx->profileCfg[profileIndex].profileId == PROFILEID_INVALID)
    {
      profileCfg = &(uehL2MeasCtx->profileCfg[profileIndex]);
    }
  }     
  return profileCfg;
}

UehL2MeasProfileCfg  *uehGetL2mProfile(ProfileId profileId, UehL2MeasCtx  *uehL2MeasCtx)    
{
  U8                    profileIndex;  
  UehL2MeasProfileCfg  *profileCfg = NULL;
  
  for (profileIndex = 0; profileIndex < MAX_NUM_PROFILES; profileIndex++)
  {
    if (uehL2MeasCtx->profileCfg[profileIndex].profileId == profileId)
    {
      profileCfg = &(uehL2MeasCtx->profileCfg[profileIndex]);
    }
  }     
  return profileCfg;
}

void uehClearL2MeasCtx(void)
{
  UehCellCb       *cellCb = uehGetCellCb();
  UehL2MeasCtx    *l2measCtx = &cellCb->uehL2MeasCtx;    
  
  logInfo("%s","l2m: uehClearL2MeasCtx: Clear All Profiles");

  clearBytes(l2measCtx, sizeof(UehL2MeasCtx));
}

void uehClearL2mProfile(UehL2MeasProfileCfg  *profileCfg)
{
  void     *ctxPtr = NULL;
  U32       size   = 0;
  U32       count; 

  logInfo("l2m: uehClearL2Profile, profileId[%d]", profileCfg->profileId);
  profileCfg->profileId           = PROFILEID_INVALID;
  profileCfg->status              = PROFILE_STOPPED;  
  profileCfg->numSamples          = 0;  
  profileCfg->samplingIntervalDur = 0;  
  profileCfg->sampleSeqNo         = 0;  
  profileCfg->mask                = UEH_L2M_MASK_CLEAR;  
  
  clearBytes((void *)&profileCfg->samplingIntervalTmr, sizeof(CmTimer));

  for (count = 0; count < profileCfg->numL2Counters; count++)
  {
    ctxPtr = profileCfg->counterLink[count].addr;
    size   = getCtxSize(profileCfg->counterLink[count].identity);
    clearBytes(ctxPtr, size);
  }
  profileCfg->numL2Counters = 0;  
}

U32 getCtxSize(U32 identity)
{
  U32 size = 0;
  
  switch (identity)
  {
    case UEH_L2M_MASK_AVG_NUM_ACTV_UE_QCI:
         size = sizeof(UehAvgNumActvUeQci);
         break;

    case UEH_L2M_MASK_PKT_LOSS_RATE_DL_QCI:
         size = sizeof(UehPktUuLossRateDlQci);
         break;

    case UEH_L2M_MASK_PKT_DELAY_DL_QCI:
         size = sizeof(UehPktDelayDlQci);
         break;

    case UEH_L2M_MASK_PKT_DISC_RATE_DL_QCI:
         size = sizeof(UehPktDiscRateDlQci);
         break;

    case UEH_L2M_MASK_PKT_LOSS_UL_QCI:
         size = sizeof(UehPktLossUlQci);
         break;

    case UEH_L2M_MASK_TOTAL_PRB_USAGE_DL: 
    case UEH_L2M_MASK_TOTAL_PRB_USAGE_UL:
         size = sizeof(UehTotalPrbUsage);
         break;

    case UEH_L2M_MASK_PRB_USAGE_DL_QCI:
    case UEH_L2M_MASK_PRB_USAGE_UL_QCI:
         size = sizeof(UehPrbUsageQci);
         break;

    case UEH_L2M_MASK_RECV_PREAMBLES:
         size = sizeof(UehRecvRaPreambles);
         break;

    case UEH_L2M_MASK_NUM_ACTV_UE_QCI_DL:
    case UEH_L2M_MASK_NUM_ACTV_UE_QCI_UL:
         size = sizeof(UehNumActvUesQci);
         break;

    default:
           logError("l2m: Invalid identity [%d]", identity);
  }
  return size;
}

void uehConfigureProfile(UehL2MeasCtx             *uehL2MeasCtx, 
                         UehL2MeasProfileCfg      *profileCfg, 
                         MsgPmhUehCounterStartReq *counterStartReq)
{
  UINT8   count = 0;
  UINT32  numOfCounters = counterStartReq->numOfCounters; 
  
  logInfo("l2m: uehConfigureProfile numOfCounters[%d]",numOfCounters);

  profileCfg->profileId           = counterStartReq->profileId;
  profileCfg->status              = PROFILE_RUNNING;  
  profileCfg->numSamples          = counterStartReq->numOfSamples;  
  profileCfg->samplingIntervalDur = counterStartReq->samplingInterval;  
  profileCfg->sampleSeqNo         = 1;  

  for (count = 0; count <numOfCounters; count++)
  {
    uehConfigureCounter(uehL2MeasCtx, profileCfg, counterStartReq->counterId[count]);
  }
  uehDumpProfile();    
} 

void uehDumpProfile(void)
{
  UehCellCb                *cellCb = uehGetCellCb();
  UehL2MeasCtx             *uehL2MeasCtx = &cellCb->uehL2MeasCtx;    
  UehL2MeasProfileCfg      *profile; 
  /* CounterLink              *link; */
  UehAvgNumActvUeQci       *pAvgNumActvUeQci = &uehL2MeasCtx->avgNumActvUeQci;
  /*UehPktUuLossRateDlQci    *pPktUuLossRateDlQci = &uehL2MeasCtx->pktUuLossRateDlQci;   
  UehPktDelayDlQci         *pPktDelayDlQci  = &uehL2MeasCtx->pktDelayDlQci;
  UehPktDiscRateDlQci      *pPktDiscRateDlQci  = &uehL2MeasCtx->pktDiscRateDlQci;
  UehPktLossUlQci          *pPktLossUlQci = &uehL2MeasCtx->pktLossUlQci;
  UehTotalPrbUsage         *pTotalPrbUsageDl = &uehL2MeasCtx->totalPrbUsageDl;  
  UehTotalPrbUsage         *pTotalPrbUsageUl = &uehL2MeasCtx->totalPrbUsageUl;
  UehPrbUsageQci           *pPrbUsageQciDl = &uehL2MeasCtx->prbUsageQciDl;
  UehPrbUsageQci           *pPrbUsageQciUl = &uehL2MeasCtx->prbUsageQciUl;
  UehRecvRaPreambles       *pRecvRaPreambles = &uehL2MeasCtx->recvRaPreambles;   
  UehNumActvUesQci         *pNumActvUesQciDl = &uehL2MeasCtx->numActvUesQciDl;
  UehNumActvUesQci         *pNumActvUesQciUl = &uehL2MeasCtx->numActvUesQciUl; */

  profile = &uehL2MeasCtx->profileCfg[0] ;
  logInfo("l2m: [Row1] prId[%d] Status[%d] numSamples[%d] sampDur[%d] seq[%d] mask[%d] numLink[%d]",
  profile->profileId, profile->status, profile->numSamples, profile->samplingIntervalDur,
  profile->sampleSeqNo, profile->mask, profile->numL2Counters );

  logInfo("l2m: link  0[%d][%d] 1[%d][%d] 2[%d][%d] 3[%d][%d] \n4[%d][%d] 5[%d][%d] 6[%d][%d] \n7[%d][%d] \
                         8[%d][%d] 9[%d][%d] \n10[%d][%d] 11[%d][%d]", \
                         profile->counterLink[0].identity, profile->counterLink[0].addr, 
                         profile->counterLink[1].identity, profile->counterLink[1].addr, 
                         profile->counterLink[2].identity, profile->counterLink[2].addr, 
                         profile->counterLink[3].identity, profile->counterLink[3].addr, 
                         profile->counterLink[4].identity, profile->counterLink[4].addr, 
                         profile->counterLink[5].identity, profile->counterLink[5].addr, 
                         profile->counterLink[6].identity, profile->counterLink[6].addr, 
                         profile->counterLink[7].identity, profile->counterLink[7].addr, 
                         profile->counterLink[8].identity, profile->counterLink[8].addr, 
                         profile->counterLink[9].identity, profile->counterLink[9].addr, 
                         profile->counterLink[10].identity, profile->counterLink[10].addr, 
                         profile->counterLink[11].identity, profile->counterLink[11].addr); 

  
  profile = &uehL2MeasCtx->profileCfg[1]; 
  logInfo("l2m: [Row2] prId[%d] Status[%d] numSamples[%d] sampDur[%d] seq[%d] mask[%d] numLink[%d]",
  profile->profileId, profile->status, profile->numSamples, profile->samplingIntervalDur,
  profile->sampleSeqNo, profile->mask, profile->numL2Counters );
  logInfo("l2m: link  0[%d][%d] 1[%d][%d] 2[%d][%d] 3[%d][%d] \n4[%d][%d] 5[%d][%d] 6[%d][%d] \n7[%d][%d] 8[%d][%d] 9[%d][%d] \n10[%d][%d] 11[%d][%d]", 
                         profile->counterLink[0].identity, profile->counterLink[0].addr, 
                         profile->counterLink[1].identity, profile->counterLink[1].addr, 
                         profile->counterLink[2].identity, profile->counterLink[2].addr, 
                         profile->counterLink[3].identity, profile->counterLink[3].addr, 
                         profile->counterLink[4].identity, profile->counterLink[4].addr, 
                         profile->counterLink[5].identity, profile->counterLink[5].addr, 
                         profile->counterLink[6].identity, profile->counterLink[6].addr, 
                         profile->counterLink[7].identity, profile->counterLink[7].addr, 
                         profile->counterLink[8].identity, profile->counterLink[8].addr, 
                         profile->counterLink[9].identity, profile->counterLink[9].addr, 
                         profile->counterLink[10].identity, profile->counterLink[10].addr, 
                         profile->counterLink[11].identity, profile->counterLink[11].addr); 

   logInfo("l2m: AvgNumActvUeQci numSamp[%d] nQci[%d] qciIdx0 [%d][%d][%d] \
             qciIdx1 [%d][%d][%d] qciIdx2 [%d][%d][%d]    "   ,
   pAvgNumActvUeQci->numSamples, pAvgNumActvUeQci->numQci,
   pAvgNumActvUeQci->qciInfo[0].qciId,pAvgNumActvUeQci->qciInfo[0].counterId,pAvgNumActvUeQci->qciInfo[0].numActUe,
   pAvgNumActvUeQci->qciInfo[1].qciId,pAvgNumActvUeQci->qciInfo[1].counterId,pAvgNumActvUeQci->qciInfo[1].numActUe,
   pAvgNumActvUeQci->qciInfo[2].qciId,pAvgNumActvUeQci->qciInfo[2].counterId,pAvgNumActvUeQci->qciInfo[2].numActUe);

}


void uehConfigureCounter(UehL2MeasCtx *uehL2MeasCtx, 
                         UehL2MeasProfileCfg  *profileCfg, 
                         CounterId counterId)
{
  void           *ctxPtr = NULL;
  CounterLink    *link;
  U8             *numQci = NULL;

  logInfo("l2m: uehConfigureCounter counterId[%d]",counterId);

  switch (counterId)
  {
    case COUNTERID_AVGNUMACTVUESQCI1: 
    case COUNTERID_AVGNUMACTVUESQCI2: 
    case COUNTERID_AVGNUMACTVUESQCI3:
    case COUNTERID_AVGNUMACTVUESQCI4: 
    case COUNTERID_AVGNUMACTVUESQCI5: 
    case COUNTERID_AVGNUMACTVUESQCI6: 
    case COUNTERID_AVGNUMACTVUESQCI7:
    case COUNTERID_AVGNUMACTVUESQCI8: 
    case COUNTERID_AVGNUMACTVUESQCI9: 
            
       {
         ctxPtr = &(uehL2MeasCtx->avgNumActvUeQci);
         numQci = &(((UehAvgNumActvUeQci *)ctxPtr)->numQci);
         if(*numQci == 0)         
         {
           ((UehAvgNumActvUeQci *)ctxPtr)->profile = profileCfg;
           ((UehAvgNumActvUeQci *)ctxPtr)->numSamples = L2M_NUM_SAMPLES;

           link =  &profileCfg->counterLink[profileCfg->numL2Counters];
           link->identity =  UEH_L2M_MASK_AVG_NUM_ACTV_UE_QCI;
           link->addr = ctxPtr;
           
           profileCfg->numL2Counters++;
           profileCfg->mask |= UEH_L2M_MASK_AVG_NUM_ACTV_UE_QCI;       
         }
         
         ((UehAvgNumActvUeQci *)ctxPtr)->qciInfo[*numQci].qciId = getQciId(counterId);
         ((UehAvgNumActvUeQci *)ctxPtr)->qciInfo[*numQci].counterId = counterId;
         ((UehAvgNumActvUeQci *)ctxPtr)->qciInfo[*numQci].numActUe = 0;
         (*numQci)++;
       }
       break;

    case COUNTERID_PKTUULOSSRATEDLQCI1: 
    case COUNTERID_PKTUULOSSRATEDLQCI2: 
    case COUNTERID_PKTUULOSSRATEDLQCI3: 
    case COUNTERID_PKTUULOSSRATEDLQCI4: 
    case COUNTERID_PKTUULOSSRATEDLQCI5: 
    case COUNTERID_PKTUULOSSRATEDLQCI6: 
    case COUNTERID_PKTUULOSSRATEDLQCI7: 
    case COUNTERID_PKTUULOSSRATEDLQCI8: 
    case COUNTERID_PKTUULOSSRATEDLQCI9: 
         
       {
         ctxPtr = &(uehL2MeasCtx->pktUuLossRateDlQci);  
         numQci = &(((UehPktUuLossRateDlQci *)ctxPtr)->numQci); 
         if(*numQci == 0)         
         {
           ((UehPktUuLossRateDlQci *)ctxPtr)->profile = profileCfg;
           ((UehPktUuLossRateDlQci *)ctxPtr)->numSamples = L2M_NUM_SAMPLES;

           link =  &profileCfg->counterLink[profileCfg->numL2Counters];
           link->identity =  UEH_L2M_MASK_PKT_LOSS_RATE_DL_QCI;
           link->addr = ctxPtr;
           
           profileCfg->numL2Counters++;
           profileCfg->mask |= UEH_L2M_MASK_PKT_LOSS_RATE_DL_QCI;
         }
         ((UehPktUuLossRateDlQci *)ctxPtr)->qciInfo[*numQci].qciId = getQciId(counterId);
         ((UehPktUuLossRateDlQci *)ctxPtr)->qciInfo[*numQci].counterId = counterId;
         ((UehPktUuLossRateDlQci *)ctxPtr)->qciInfo[*numQci].uuLossDl = 0;
         (*numQci)++;
       }
       break;


    case COUNTERID_PKTDELAYDLQCI1: 
    case COUNTERID_PKTDELAYDLQCI2: 
    case COUNTERID_PKTDELAYDLQCI3: 
    case COUNTERID_PKTDELAYDLQCI4: 
    case COUNTERID_PKTDELAYDLQCI5: 
    case COUNTERID_PKTDELAYDLQCI6: 
    case COUNTERID_PKTDELAYDLQCI7: 
    case COUNTERID_PKTDELAYDLQCI8: 
    case COUNTERID_PKTDELAYDLQCI9: 

      {
         ctxPtr = &(uehL2MeasCtx->pktDelayDlQci);
         numQci = &(((UehPktDelayDlQci *)ctxPtr)->numQci);
         
         if(*numQci == 0)         
         {
           ((UehPktUuLossRateDlQci *)ctxPtr)->profile = profileCfg;
           
           link =  &profileCfg->counterLink[profileCfg->numL2Counters];
           link->identity =  UEH_L2M_MASK_PKT_DELAY_DL_QCI;
           link->addr = ctxPtr;
           
           profileCfg->numL2Counters++;
           profileCfg->mask |= UEH_L2M_MASK_PKT_DELAY_DL_QCI;
         }
         ((UehPktDelayDlQci *)ctxPtr)->qciInfo[*numQci].qciId     = getQciId(counterId);
         ((UehPktDelayDlQci *)ctxPtr)->qciInfo[*numQci].counterId = counterId;
         ((UehPktDelayDlQci *)ctxPtr)->qciInfo[*numQci].dlDelay   = 0;
         (*numQci)++;
      }
      break;

    case COUNTERID_PKTDISCRATEDLQCI1: 
    case COUNTERID_PKTDISCRATEDLQCI2: 
    case COUNTERID_PKTDISCRATEDLQCI3:
    case COUNTERID_PKTDISCRATEDLQCI4: 
    case COUNTERID_PKTDISCRATEDLQCI5: 
    case COUNTERID_PKTDISCRATEDLQCI6:
    case COUNTERID_PKTDISCRATEDLQCI7: 
    case COUNTERID_PKTDISCRATEDLQCI8: 
    case COUNTERID_PKTDISCRATEDLQCI9: 
         
      {
         ctxPtr = &(uehL2MeasCtx->pktDiscRateDlQci);
         numQci = &(((UehPktDiscRateDlQci *)ctxPtr)->numQci);

         if(*numQci == 0)         
         {
           ((UehPktDiscRateDlQci *)ctxPtr)->profile = profileCfg;
           
           link           =  &profileCfg->counterLink[profileCfg->numL2Counters];
           link->identity =  UEH_L2M_MASK_PKT_DISC_RATE_DL_QCI;
           link->addr     = ctxPtr;
           
           profileCfg->numL2Counters++;
           profileCfg->mask |= UEH_L2M_MASK_PKT_DISC_RATE_DL_QCI;
         }
         
         ((UehPktDiscRateDlQci *)ctxPtr)->qciInfo[*numQci].qciId     = getQciId(counterId);
         ((UehPktDiscRateDlQci *)ctxPtr)->qciInfo[*numQci].counterId = counterId;
         ((UehPktDiscRateDlQci *)ctxPtr)->qciInfo[*numQci].dlDisc    = 0;
         (*numQci)++;
      }
      break;

    case COUNTERID_PKTLOSSULQCI1: 
    case COUNTERID_PKTLOSSULQCI2:
    case COUNTERID_PKTLOSSULQCI3:
    case COUNTERID_PKTLOSSULQCI4:
    case COUNTERID_PKTLOSSULQCI5:
    case COUNTERID_PKTLOSSULQCI6:
    case COUNTERID_PKTLOSSULQCI7:
    case COUNTERID_PKTLOSSULQCI8:
    case COUNTERID_PKTLOSSULQCI9:

       {
         ctxPtr = &(uehL2MeasCtx->pktLossUlQci);
         numQci = &(((UehPktLossUlQci *)ctxPtr)->numQci);
         if(*numQci == 0)         
         {
           ((UehPktLossUlQci *)ctxPtr)->profile = profileCfg;
           
           link           = &profileCfg->counterLink[profileCfg->numL2Counters];
           link->identity =  UEH_L2M_MASK_PKT_LOSS_UL_QCI;
           link->addr     =  ctxPtr;
           
           profileCfg->numL2Counters++;
           profileCfg->mask |= UEH_L2M_MASK_PKT_LOSS_UL_QCI;
         }

         ((UehPktLossUlQci *)ctxPtr)->qciInfo[*numQci].qciId     = getQciId(counterId);
         ((UehPktLossUlQci *)ctxPtr)->qciInfo[*numQci].counterId = counterId;
         ((UehPktLossUlQci *)ctxPtr)->qciInfo[*numQci].ulLoss    = 0;
         (*numQci)++;
       }
       break;

    case COUNTERID_PRBUSAGEDLQCI1:
    case COUNTERID_PRBUSAGEDLQCI2:
    case COUNTERID_PRBUSAGEDLQCI3:
    case COUNTERID_PRBUSAGEDLQCI4:
    case COUNTERID_PRBUSAGEDLQCI5:
    case COUNTERID_PRBUSAGEDLQCI6:
    case COUNTERID_PRBUSAGEDLQCI7:
    case COUNTERID_PRBUSAGEDLQCI8:
    case COUNTERID_PRBUSAGEDLQCI9:
      
       {   
         ctxPtr = &(uehL2MeasCtx->prbUsageQciDl);
         numQci = &(((UehPrbUsageQci *)ctxPtr)->numQci);
         if(*numQci == 0)         
         {
           ((UehPrbUsageQci *)ctxPtr)->profile = profileCfg;
           
           link =  &profileCfg->counterLink[profileCfg->numL2Counters];
           link->identity =  UEH_L2M_MASK_PRB_USAGE_DL_QCI;
           link->addr = ctxPtr;
           
           profileCfg->numL2Counters++;
           profileCfg->mask |= UEH_L2M_MASK_PRB_USAGE_DL_QCI;
         }
         ((UehPrbUsageQci *)ctxPtr)->qciInfo[*numQci].qciId = getQciId(counterId);
         ((UehPrbUsageQci *)ctxPtr)->qciInfo[*numQci].counterId = counterId;
         ((UehPrbUsageQci *)ctxPtr)->qciInfo[*numQci].prbPerc = 0;
         (*numQci)++;
       }
       break;

    case COUNTERID_PRBUSAGEULQCI1:
    case COUNTERID_PRBUSAGEULQCI2:
    case COUNTERID_PRBUSAGEULQCI3:
    case COUNTERID_PRBUSAGEULQCI4:
    case COUNTERID_PRBUSAGEULQCI5:
    case COUNTERID_PRBUSAGEULQCI6:
    case COUNTERID_PRBUSAGEULQCI7:
    case COUNTERID_PRBUSAGEULQCI8:
    case COUNTERID_PRBUSAGEULQCI9:
       {
         ctxPtr = &(uehL2MeasCtx->prbUsageQciUl);
         numQci = &(((UehPrbUsageQci *)ctxPtr)->numQci);
         if(*numQci == 0)         
         {
           ((UehPrbUsageQci *)ctxPtr)->profile = profileCfg;
           
           link =  &profileCfg->counterLink[profileCfg->numL2Counters];
           link->identity =  UEH_L2M_MASK_PRB_USAGE_UL_QCI;
           link->addr = ctxPtr;
           
           profileCfg->numL2Counters++;
           profileCfg->mask |= UEH_L2M_MASK_PRB_USAGE_UL_QCI;
         }
         ((UehPrbUsageQci *)ctxPtr)->qciInfo[*numQci].qciId     = getQciId(counterId);
         ((UehPrbUsageQci *)ctxPtr)->qciInfo[*numQci].counterId = counterId;
         ((UehPrbUsageQci *)ctxPtr)->qciInfo[*numQci].prbPerc   = 0;
         (*numQci)++;
       }
       break;

    case COUNTERID_NUMACTVUESDLQCI1:
    case COUNTERID_NUMACTVUESDLQCI2:
    case COUNTERID_NUMACTVUESDLQCI3:
    case COUNTERID_NUMACTVUESDLQCI4:
    case COUNTERID_NUMACTVUESDLQCI5:
    case COUNTERID_NUMACTVUESDLQCI6:
    case COUNTERID_NUMACTVUESDLQCI7:
    case COUNTERID_NUMACTVUESDLQCI8:
    case COUNTERID_NUMACTVUESDLQCI9:
       {   
         ctxPtr = &(uehL2MeasCtx->numActvUesQciDl);
         numQci = &(((UehNumActvUesQci *)ctxPtr)->numQci);
         if(*numQci == 0)         
         {
           ((UehNumActvUesQci *)ctxPtr)->profile        = profileCfg;
           ((UehNumActvUesQci *)ctxPtr)->l2SamplePeriod = L2M_SAMPLE_PERIOD;
           
           link           =  &profileCfg->counterLink[profileCfg->numL2Counters];
           link->identity =  UEH_L2M_MASK_NUM_ACTV_UE_QCI_DL;
           link->addr     =  ctxPtr;
           
           profileCfg->numL2Counters++;
           profileCfg->mask |= UEH_L2M_MASK_NUM_ACTV_UE_QCI_DL;
         }
         ((UehNumActvUesQci *)ctxPtr)->qciInfo[*numQci].qciId     = getQciId(counterId);
         ((UehNumActvUesQci *)ctxPtr)->qciInfo[*numQci].counterId = counterId;
         ((UehNumActvUesQci *)ctxPtr)->qciInfo[*numQci].numActvUe = 0;
         (*numQci)++;
       }
       break;

    case COUNTERID_NUMACTVUESULQCI1:
    case COUNTERID_NUMACTVUESULQCI2:
    case COUNTERID_NUMACTVUESULQCI3:
    case COUNTERID_NUMACTVUESULQCI4:
    case COUNTERID_NUMACTVUESULQCI5:
    case COUNTERID_NUMACTVUESULQCI6:
    case COUNTERID_NUMACTVUESULQCI7:
    case COUNTERID_NUMACTVUESULQCI8:
    case COUNTERID_NUMACTVUESULQCI9:
      {
         ctxPtr = &(uehL2MeasCtx->numActvUesQciUl);
         numQci = &(((UehNumActvUesQci *)ctxPtr)->numQci);

         if(*numQci == 0)         
         {
           ((UehNumActvUesQci *)ctxPtr)->profile        = profileCfg;
           ((UehNumActvUesQci *)ctxPtr)->l2SamplePeriod = L2M_SAMPLE_PERIOD;
           
           link           = &profileCfg->counterLink[profileCfg->numL2Counters];
           link->identity =  UEH_L2M_MASK_NUM_ACTV_UE_QCI_UL;
           link->addr     =  ctxPtr;
           
           profileCfg->numL2Counters++;
           profileCfg->mask |= UEH_L2M_MASK_NUM_ACTV_UE_QCI_UL;
         }
         ((UehNumActvUesQci *)ctxPtr)->qciInfo[*numQci].qciId     = getQciId(counterId);
         ((UehNumActvUesQci *)ctxPtr)->qciInfo[*numQci].counterId = counterId;
         ((UehNumActvUesQci *)ctxPtr)->qciInfo[*numQci].numActvUe = 0;
         (*numQci)++;
      }
      break;

    case COUNTERID_TOTALPRBUSAGEDL:
      {    
         ctxPtr = &(uehL2MeasCtx->totalPrbUsageDl);
         
         ((UehTotalPrbUsage *)ctxPtr)->profile   = profileCfg;
         ((UehTotalPrbUsage *)ctxPtr)->counterId = counterId;
         ((UehTotalPrbUsage *)ctxPtr)->prbPerc   = 0;
           
         link           =  &profileCfg->counterLink[profileCfg->numL2Counters];
         link->identity =  UEH_L2M_MASK_TOTAL_PRB_USAGE_DL;
         link->addr     = ctxPtr;
           
         profileCfg->numL2Counters++;
         profileCfg->mask |= UEH_L2M_MASK_TOTAL_PRB_USAGE_DL;
      }
      break;

    case COUNTERID_TOTALPRBUSAGEUL:
      {    
         ctxPtr = &(uehL2MeasCtx->totalPrbUsageUl);
         ((UehTotalPrbUsage *)ctxPtr)->profile   = profileCfg;
         ((UehTotalPrbUsage *)ctxPtr)->counterId = counterId;
         ((UehTotalPrbUsage *)ctxPtr)->prbPerc   = 0;
           
         link           = &profileCfg->counterLink[profileCfg->numL2Counters];
         link->identity =  UEH_L2M_MASK_TOTAL_PRB_USAGE_UL;
         link->addr     =  ctxPtr;
           
         profileCfg->numL2Counters++;
         profileCfg->mask |= UEH_L2M_MASK_TOTAL_PRB_USAGE_UL;
      }
      break;

    case COUNTERID_RECVRAPREAMBLESDEDICATED:
      {
         ctxPtr = &(uehL2MeasCtx->recvRaPreambles);   

         ((UehRecvRaPreambles *)ctxPtr)->profile         = profileCfg;
         ((UehRecvRaPreambles *)ctxPtr)->dedPreCounterId = counterId;
         
         if (((UehRecvRaPreambles *)ctxPtr)->flag == 0) 
         {
           link           = &profileCfg->counterLink[profileCfg->numL2Counters];
           link->identity =  UEH_L2M_MASK_RECV_PREAMBLES;
           link->addr     =  ctxPtr;
           
           profileCfg->numL2Counters++;
           profileCfg->mask |= UEH_L2M_MASK_RECV_PREAMBLES;
         
           ((UehRecvRaPreambles *)ctxPtr)->dedPreambles        = 0;
           ((UehRecvRaPreambles *)ctxPtr)->randSelPreLowRange  = 0;
           ((UehRecvRaPreambles *)ctxPtr)->randSelPreHighRange = 0;

           ((UehRecvRaPreambles *)ctxPtr)->flag = 1; 
         }
      }
      break;

    case COUNTERID_RECVRAPREAMBLESLOWRANGE:
      {
         ctxPtr = &(uehL2MeasCtx->recvRaPreambles);   
         ((UehRecvRaPreambles *)ctxPtr)->profile = profileCfg;
         ((UehRecvRaPreambles *)ctxPtr)->randLowCounterId = counterId;
         
         if (((UehRecvRaPreambles *)ctxPtr)->flag == 0) 
         {
           link           = &profileCfg->counterLink[profileCfg->numL2Counters];
           link->identity =  UEH_L2M_MASK_RECV_PREAMBLES;
           link->addr     =  ctxPtr;
           
           profileCfg->numL2Counters++;
           profileCfg->mask |= UEH_L2M_MASK_RECV_PREAMBLES;
         
           ((UehRecvRaPreambles *)ctxPtr)->dedPreambles        = 0;
           ((UehRecvRaPreambles *)ctxPtr)->randSelPreLowRange  = 0;
           ((UehRecvRaPreambles *)ctxPtr)->randSelPreHighRange = 0;

           ((UehRecvRaPreambles *)ctxPtr)->flag = 1; 
         }
      }
      break;

    case COUNTERID_RECVRAPREAMBLESHIGHRANGE:
      {
         ctxPtr = &(uehL2MeasCtx->recvRaPreambles);   
         ((UehRecvRaPreambles *)ctxPtr)->profile = profileCfg;
         ((UehRecvRaPreambles *)ctxPtr)->randHighCounterId = counterId;
         
         if (((UehRecvRaPreambles *)ctxPtr)->flag == 0) 
         {
           link           = &profileCfg->counterLink[profileCfg->numL2Counters];
           link->identity =  UEH_L2M_MASK_RECV_PREAMBLES;
           link->addr     =  ctxPtr;
           
           profileCfg->numL2Counters++;
           profileCfg->mask |= UEH_L2M_MASK_RECV_PREAMBLES;
         
           ((UehRecvRaPreambles *)ctxPtr)->dedPreambles        = 0;
           ((UehRecvRaPreambles *)ctxPtr)->randSelPreLowRange  = 0;
           ((UehRecvRaPreambles *)ctxPtr)->randSelPreHighRange = 0;

           ((UehRecvRaPreambles *)ctxPtr)->flag = 1; 
         }
      }
      break;

    default:
           logError("l2m: Invalid counterId [%d]", counterId);
  }
}

U8 getQciId(CounterId counterId)
{
  U8 qciId = 0;

  switch (counterId)
  {
    case COUNTERID_AVGNUMACTVUESQCI1: 
    case COUNTERID_PKTUULOSSRATEDLQCI1: 
    case COUNTERID_PKTDELAYDLQCI1: 
    case COUNTERID_PKTDISCRATEDLQCI1: 
    case COUNTERID_PKTLOSSULQCI1: 
    case COUNTERID_PRBUSAGEDLQCI1:
    case COUNTERID_PRBUSAGEULQCI1:
    case COUNTERID_NUMACTVUESDLQCI1:
    case COUNTERID_NUMACTVUESULQCI1:
           qciId = 1;
           break;


    case COUNTERID_AVGNUMACTVUESQCI2: 
    case COUNTERID_PKTUULOSSRATEDLQCI2: 
    case COUNTERID_PKTDELAYDLQCI2: 
    case COUNTERID_PKTDISCRATEDLQCI2: 
    case COUNTERID_PKTLOSSULQCI2:
    case COUNTERID_PRBUSAGEDLQCI2:
    case COUNTERID_PRBUSAGEULQCI2:
    case COUNTERID_NUMACTVUESDLQCI2:
    case COUNTERID_NUMACTVUESULQCI2:
           qciId = 2;
           break;
    
    case COUNTERID_AVGNUMACTVUESQCI3:
    case COUNTERID_PKTUULOSSRATEDLQCI3: 
    case COUNTERID_PKTDELAYDLQCI3: 
    case COUNTERID_PKTDISCRATEDLQCI3:
    case COUNTERID_PKTLOSSULQCI3:
    case COUNTERID_PRBUSAGEDLQCI3:
    case COUNTERID_PRBUSAGEULQCI3:
    case COUNTERID_NUMACTVUESDLQCI3:
    case COUNTERID_NUMACTVUESULQCI3:
           qciId = 3;
           break;
    
    case COUNTERID_AVGNUMACTVUESQCI4: 
    case COUNTERID_PKTUULOSSRATEDLQCI4: 
    case COUNTERID_PKTDELAYDLQCI4: 
    case COUNTERID_PKTDISCRATEDLQCI4: 
    case COUNTERID_PKTLOSSULQCI4:
    case COUNTERID_PRBUSAGEDLQCI4:
    case COUNTERID_PRBUSAGEULQCI4:
    case COUNTERID_NUMACTVUESDLQCI4:
    case COUNTERID_NUMACTVUESULQCI4:
           qciId = 4;
           break;
    
    case COUNTERID_AVGNUMACTVUESQCI5: 
    case COUNTERID_PKTUULOSSRATEDLQCI5: 
    case COUNTERID_PKTDELAYDLQCI5: 
    case COUNTERID_PKTDISCRATEDLQCI5: 
    case COUNTERID_PKTLOSSULQCI5:
    case COUNTERID_PRBUSAGEDLQCI5:
    case COUNTERID_PRBUSAGEULQCI5:
    case COUNTERID_NUMACTVUESDLQCI5:
    case COUNTERID_NUMACTVUESULQCI5:
           qciId = 5;
           break;
    
    case COUNTERID_AVGNUMACTVUESQCI6: 
    case COUNTERID_PKTUULOSSRATEDLQCI6: 
    case COUNTERID_PKTDELAYDLQCI6: 
    case COUNTERID_PKTDISCRATEDLQCI6:
    case COUNTERID_PKTLOSSULQCI6:
    case COUNTERID_PRBUSAGEDLQCI6:
    case COUNTERID_PRBUSAGEULQCI6:
    case COUNTERID_NUMACTVUESDLQCI6:
    case COUNTERID_NUMACTVUESULQCI6:
           qciId = 6;
           break;
    
    case COUNTERID_AVGNUMACTVUESQCI7:
    case COUNTERID_PKTUULOSSRATEDLQCI7: 
    case COUNTERID_PKTDELAYDLQCI7: 
    case COUNTERID_PKTDISCRATEDLQCI7: 
    case COUNTERID_PKTLOSSULQCI7:
    case COUNTERID_PRBUSAGEDLQCI7:
    case COUNTERID_PRBUSAGEULQCI7:
    case COUNTERID_NUMACTVUESDLQCI7:
    case COUNTERID_NUMACTVUESULQCI7:
           qciId = 7;
           break;
    
    case COUNTERID_AVGNUMACTVUESQCI8: 
    case COUNTERID_PKTUULOSSRATEDLQCI8: 
    case COUNTERID_PKTDELAYDLQCI8: 
    case COUNTERID_PKTDISCRATEDLQCI8: 
    case COUNTERID_PKTLOSSULQCI8:
    case COUNTERID_PRBUSAGEDLQCI8:
    case COUNTERID_PRBUSAGEULQCI8:
    case COUNTERID_NUMACTVUESDLQCI8:
    case COUNTERID_NUMACTVUESULQCI8:
           qciId = 8;
           break;
    
    case COUNTERID_AVGNUMACTVUESQCI9: 
    case COUNTERID_PKTUULOSSRATEDLQCI9: 
    case COUNTERID_PKTDELAYDLQCI9: 
    case COUNTERID_PKTDISCRATEDLQCI9: 
    case COUNTERID_PKTLOSSULQCI9:
    case COUNTERID_PRBUSAGEDLQCI9:
    case COUNTERID_PRBUSAGEULQCI9:
    case COUNTERID_NUMACTVUESDLQCI9:
    case COUNTERID_NUMACTVUESULQCI9:
           qciId = 9;
           break;


    default:
           logError("l2m: Invalid counterId[%d]", counterId);
  }
    
  logInfo("l2m: getQciId: counter[%d] qci[%d] %d", counterId, qciId);

  return qciId;
}


void uehStartProfile(UehL2MeasProfileCfg  *profileCfg)
{
  U32 count;
  U32 duration;
  U8 profileTmrId=0;

  logInfo("l2m: uehStartProfile profile[%d]",profileCfg->profileId);

  for (count = 0; count < profileCfg->numL2Counters; count++)
  {
    /*setting bitmask */
    profileCfg->mask |= profileCfg->counterLink[count].identity;

    /* build and send counter request */
    switch (profileCfg->counterLink[count].identity)
    {
      case UEH_L2M_MASK_AVG_NUM_ACTV_UE_QCI:
      case UEH_L2M_MASK_PKT_LOSS_RATE_DL_QCI:
        uehRlcL2MeasReq(profileCfg, &profileCfg->counterLink[count]);
        break;

      case UEH_L2M_MASK_PKT_DELAY_DL_QCI:
      case UEH_L2M_MASK_PKT_DISC_RATE_DL_QCI:
      case UEH_L2M_MASK_PKT_LOSS_UL_QCI:
        uehPdcpL2MeasReq(profileCfg, &profileCfg->counterLink[count]);
        break;

      case UEH_L2M_MASK_TOTAL_PRB_USAGE_DL: 
      case UEH_L2M_MASK_TOTAL_PRB_USAGE_UL:
      case UEH_L2M_MASK_PRB_USAGE_DL_QCI:
      case UEH_L2M_MASK_PRB_USAGE_UL_QCI:
      case UEH_L2M_MASK_RECV_PREAMBLES:
      case UEH_L2M_MASK_NUM_ACTV_UE_QCI_DL:
      case UEH_L2M_MASK_NUM_ACTV_UE_QCI_UL:
        uehMacL2MeasReq(profileCfg, &profileCfg->counterLink[count]);
        break;
    
      default:
           logError("l2m: Invalid identity [%d]", profileCfg->counterLink[count].identity);
    }
  }

  /*Start Sampling Timer*/
  if (profileCfg->profileId == PROFILEID_PROFILE1)
  {
    profileTmrId = UEH_L2M_SAMPLE_PROFILE1;
  }
  else
  {
    profileTmrId = UEH_L2M_SAMPLE_PROFILE2;
  }

  duration = getMeasPeriod(profileCfg->samplingIntervalDur);
#ifdef LTE_L2_MEAS_STUB
  duration = 2000;
#else
  duration = duration + 1000;
#endif
  uehStartTmr(profileTmrId,&((UehL2MeasProfileCfg *)profileCfg)->samplingIntervalTmr,(PTR)profileCfg,0);
}

PUBLIC S16 uehSndMsgToSm (IN EventType uniqueEventType, void *msg, Pst *pst)
{
    S16                 ret = 0;
    LrgSchMeasReqInfo  *macCfgInfo = (LrgSchMeasReqInfo *) msg;  

    logInfo ("l2m: uehSndMsgToSm eventType[%d]",uniqueEventType);

    switch (uniqueEventType)
    {  
        case EVENTTYPE_MACL2MEAS:
            pst->event = EVTUEHMACL2MEASREQ;
            macCfgInfo->hdr.response.selector = pst->selector;
            macCfgInfo->hdr.response.prior = pst->prior;
            macCfgInfo->hdr.response.route = pst->route;
            macCfgInfo->hdr.response.mem.region = pst->region; 
            macCfgInfo->hdr.response.mem.pool = pst->pool;
            /* ret = cmPkLrgSchL2MeasReq(&pst ,(LrgSchMeasReqInfo *)msg); */
            ret = SPstTsk(pst, msg);
            break;

        case EVENTTYPE_RLCL2MEAS:
            pst->event = EVTUEHRLCL2MEASREQ; 
            /* ret = cmPkLkwL2MeasReq(&pst ,(KwL2MeasReqEvt *)msg); */
            ret = SPstTsk(pst, msg);
            break;

        case EVENTTYPE_PDCPL2MEAS:
            pst->event = EVTUEHPDCPL2MEASREQ; 
            /* ret = cmPkLpjL2MeasReq(&pst ,(PjL2MeasReqEvt *)msg); */
            ret = SPstTsk(pst, msg);
            break;

        default:
            logInfo ("l2m: Invalid Event type[%d]",uniqueEventType);
            ret = RFAILED;
            break;
    }

    return ret;
}

PUBLIC S16 uehMacL2MeasCfm( Pst *pst, LrgSchMeasCfmInfo  *measInfo)
{
  S16                     retVal = FALSE;
  UehCellCb              *cellCb = uehGetCellCb();
  UehL2MeasCtx           *uehL2MeasCtx = &cellCb->uehL2MeasCtx;    
  UehTotalPrbUsage       *pTotalPrbUsageDl = NULL;  
  UehTotalPrbUsage       *pTotalPrbUsageUl = NULL;
  UehPrbUsageQci         *pPrbUsageQciDl = NULL;
  UehPrbUsageQci         *pPrbUsageQciUl = NULL;
  UehRecvRaPreambles     *pRecvRaPreambles = NULL;   
  UehNumActvUesQci       *pNumActvUesQciDl = NULL;
  UehNumActvUesQci       *pNumActvUesQciUl = NULL;
  UehL2MeasProfileCfg    *profile = NULL;
  U32                     identity;
  U32                     transId = measInfo->hdr.transId;
  U32                     idx = 0; 
  U32                     idy = 0; 
  TBOOL                   statusSucc = TRUE;
  S16                     profileTmrId;

    logInfo("uehMacL2MeasCfm received measType[%d]", measInfo->measType);

    switch( measInfo->measType)
    {
      case LRG_L2MEAS_AVG_PRB_DL:
        pTotalPrbUsageDl = &(uehL2MeasCtx->totalPrbUsageDl);
        identity = UEH_L2M_MASK_TOTAL_PRB_USAGE_DL;

        profile = pTotalPrbUsageDl->profile;

        if (TRUE == uehVerifyL2mCfm(profile, identity, transId))
        {
           if(measInfo->cfm.status == LCM_PRIM_NOK)
           {
             logError("l2m: LRG_L2MEAS_AVG_PRB_DL Fail, cause[%d]", measInfo->cfm.reason);
             statusSucc = FALSE;
             break;
           }
          pTotalPrbUsageDl->prbPerc = measInfo->avgPrbDl.prbPerc;
          profile->mask &= UEH_L2M_INV_TOTAL_PRB_USAGE_DL;
        }
        else
        {
          logError("%s","l2m: uehMacL2MeasCfm, spurious TOTAL_PRB_USAGE_DL");
        }
        break;

      case LRG_L2MEAS_AVG_PRB_UL:
        pTotalPrbUsageUl = &(uehL2MeasCtx->totalPrbUsageUl);
        identity = UEH_L2M_MASK_TOTAL_PRB_USAGE_UL;

        profile = pTotalPrbUsageUl->profile;

        if (TRUE == uehVerifyL2mCfm(profile, identity, transId))
        {
           if(measInfo->cfm.status == LCM_PRIM_NOK)
           {
             statusSucc = FALSE;
             logError("l2m: LRG_L2MEAS_AVG_PRB_UL Fail, cause[%d]", measInfo->cfm.reason);
             break;
           }
          pTotalPrbUsageUl->prbPerc = measInfo->avgPrbUl.prbPerc;
          profile->mask &= UEH_L2M_INV_TOTAL_PRB_USAGE_UL;
        }
        else
        { 
          logError("%s","l2m: uehMacL2MeasCfm, spurious TOTAL_PRB_USAGE_UL");
        }
        break;
      
      case LRG_L2MEAS_RA_PREAMBLE:
        pRecvRaPreambles = &(uehL2MeasCtx->recvRaPreambles);   
        identity = UEH_L2M_MASK_RECV_PREAMBLES;

        profile = pRecvRaPreambles->profile;

        if (TRUE == uehVerifyL2mCfm(profile, identity, transId))
        {
          if(measInfo->cfm.status == LCM_PRIM_NOK)
          {
            logError("l2m: LRG_L2MEAS_RA_PREAMBLE Fail, cause[%d]", measInfo->cfm.reason);
            statusSucc = FALSE;
            break;
          }
          pRecvRaPreambles->dedPreambles        = measInfo->raPrmbsCfm.dedPreambles;
          pRecvRaPreambles->randSelPreLowRange  = measInfo->raPrmbsCfm.randSelPreLowRange;
          pRecvRaPreambles->randSelPreHighRange = measInfo->raPrmbsCfm.randSelPreHighRange;
          
          profile->mask &= UEH_L2M_INV_RECV_PREAMBLES;
        }
        else
        {
          logError("%s","l2m: uehMacL2MeasCfm, spurious RECV_PREAMBLES");
        }
        break;
      
      case LRG_L2MEAS_AVG_PRB_PER_QCI_DL:
        pPrbUsageQciDl = &(uehL2MeasCtx->prbUsageQciDl);
        identity = UEH_L2M_MASK_PRB_USAGE_DL_QCI;

        profile = pPrbUsageQciDl->profile;

        if (TRUE == uehVerifyL2mCfm(profile, identity, transId))
        {
          if(measInfo->cfm.status == LCM_PRIM_NOK)
          {
            logError("l2m: LRG_L2MEAS_AVG_PRB_PER_QCI_DL Fail, cause[%d]", measInfo->cfm.reason);
            statusSucc = FALSE;
            break;
          }
          if(measInfo->avgPrbQciDlCfm.numQci != pPrbUsageQciDl->numQci)
           {
             logError("%s","l2m: numQci error, LRG_L2MEAS_AVG_PRB_PER_QCI_DL,numQci[%d]!=[%d] ",
               measInfo->avgPrbQciDlCfm.numQci, pPrbUsageQciDl->numQci);
             statusSucc = FALSE;
             break;
          }
          for (idx = 0; idx < measInfo->avgPrbQciDlCfm.numQci; idx++)
          {
             for (idy = 0; idy < pPrbUsageQciDl->numQci; idy++)
             {
                if (pPrbUsageQciDl->qciInfo[idx].qciId == measInfo->avgPrbQciDlCfm.prbPercQci[idy].qciValue)
                {
                   pPrbUsageQciDl->qciInfo[idx].prbPerc = 
                    measInfo->avgPrbQciDlCfm.prbPercQci[idx].prbPercQci;
                    break;
                }
             }
          }
          profile->mask &= UEH_L2M_INV_PRB_USAGE_DL_QCI;
        }
        else
        {
          logError("%s","l2m: uehMacL2MeasCfm, spurious PRB_USAGE_DL_QCI");
        }
        break;
      
      case LRG_L2MEAS_AVG_PRB_PER_QCI_UL:
        pPrbUsageQciUl = &(uehL2MeasCtx->prbUsageQciUl);
        identity = UEH_L2M_MASK_PRB_USAGE_UL_QCI;

        profile = pPrbUsageQciUl->profile;

        if (TRUE == uehVerifyL2mCfm(profile, identity, transId))
        {
          if(measInfo->cfm.status == LCM_PRIM_NOK)
          {
            logError("l2m: LRG_L2MEAS_AVG_PRB_PER_QCI_UL Fail, cause[%d]", measInfo->cfm.reason);
            statusSucc = FALSE;
            break;
          }
          if(measInfo->avgPrbQciUlCfm.numQci != pPrbUsageQciUl->numQci)
          {
             logError("%s","l2m: numQci Error, LRG_L2MEAS_AVG_PRB_PER_QCI_UL, numQci[%d]!=[%d] ",
                 measInfo->avgPrbQciUlCfm.numQci, pPrbUsageQciUl->numQci);
             statusSucc = FALSE;
             break;
          }
          for (idx = 0; idx < measInfo->avgPrbQciUlCfm.numQci; idx++)
          {
             for (idy = 0; idy < pPrbUsageQciUl->numQci; idy++)
             {
                if (pPrbUsageQciUl->qciInfo[idx].qciId == measInfo->avgPrbQciUlCfm.prbPercQci[idy].qciValue)
                {
                   pPrbUsageQciUl->qciInfo[idx].prbPerc = 
                    measInfo->avgPrbQciUlCfm.prbPercQci[idx].prbPercQci;
                    break;
                }
             }
          }
          profile->mask &= UEH_L2M_INV_PRB_USAGE_UL_QCI;
        }
        else
        {
          logError("%s","l2m: uehMacL2MeasCfm, spurious PRB_USAGE_UL_QCI");
        }
        break;
      
      case LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL:
        pNumActvUesQciDl = &(uehL2MeasCtx->numActvUesQciDl);
        identity = UEH_L2M_MASK_NUM_ACTV_UE_QCI_DL;

        profile = pNumActvUesQciDl->profile;

        if (TRUE == uehVerifyL2mCfm(profile, identity, transId))
        {
           if(measInfo->cfm.status == LCM_PRIM_NOK)
           {
             logError("l2m: LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL Fail, cause[%d]", measInfo->cfm.reason);
             statusSucc = FALSE;
             break;
           }
          if(measInfo->numUeQciDlCfm.numQci != pNumActvUesQciDl->numQci)
          {
             logError("l2m: LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL,numQci[%d]!=[%d] ",
                    measInfo->numUeQciDlCfm.numQci, pNumActvUesQciDl->numQci);
             statusSucc = FALSE;
             break;
          }
          for (idx = 0; idx < measInfo->numUeQciDlCfm.numQci; idx++)
          {
             for (idy = 0; idy < pNumActvUesQciDl->numQci; idy++)
             {
                if (pNumActvUesQciDl->qciInfo[idx].qciId == measInfo->numUeQciDlCfm.numActvUeQci[idy].qciValue)
                {
                   pNumActvUesQciDl->qciInfo[idx].numActvUe = measInfo->numUeQciDlCfm.numActvUeQci[idy].numActvUeQci;
                   break;
                }
             }
          }
          profile->mask &= UEH_L2M_INV_NUM_ACTV_UE_QCI_DL;
        }
        else
        {
          logError("%s","l2m: uehMacL2MeasCfm, spurious NUM_ACTV_UE_QCI_DL");
        }
        break;

      case LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL:
        pNumActvUesQciUl = &(uehL2MeasCtx->numActvUesQciUl);
        identity = UEH_L2M_MASK_NUM_ACTV_UE_QCI_UL;

        profile = pNumActvUesQciUl->profile;

        if (TRUE == uehVerifyL2mCfm(profile, identity, transId))
        {
           if(measInfo->cfm.status == LCM_PRIM_NOK)
           {
             logError("l2m: LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL Fail, cause[%d]", measInfo->cfm.reason);
             statusSucc = FALSE;
             break;
           }
           if(measInfo->numUeQciUlCfm.numQci != pNumActvUesQciUl->numQci)
          {
             logError("%s","l2m: LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL, msgNumQci[%d] ctxNumQci[%d]",
                 measInfo->numUeQciUlCfm.numQci, pNumActvUesQciUl->numQci);
             statusSucc = FALSE;
             break;
          }
          for (idx = 0; idx < measInfo->numUeQciUlCfm.numQci; idx++)
          {
             for (idy = 0; idy < pNumActvUesQciUl->numQci; idy++)
             {
                if (pNumActvUesQciUl->qciInfo[idx].qciId == measInfo->numUeQciUlCfm.numActvUeQci[idy].qciValue)
                {
                   pNumActvUesQciUl->qciInfo[idx].numActvUe = measInfo->numUeQciUlCfm.numActvUeQci[idy].numActvUeQci;
                   break;
                }
             }
          }
          profile->mask &= UEH_L2M_INV_NUM_ACTV_UE_QCI_UL;
        }
        else
        {
          logError("%s","l2m: uehMacL2MeasCfm, spurious NUM_ACTV_UE_QCI_UL");
        }
        break;

      default:
          break; 

  }
  if ((profile != NULL &&
      profile->mask == UEH_L2M_MASK_CLEAR) ||
      statusSucc == FALSE)
  {
     if (profile->profileId == PROFILEID_PROFILE1)
     {
       profileTmrId = UEH_L2M_SAMPLE_PROFILE1;
     }
     else
     {
       profileTmrId = UEH_L2M_SAMPLE_PROFILE2;
     }
    
    uehStopTmr (profileTmrId,&(((UehL2MeasProfileCfg *)profile)->samplingIntervalTmr),(PTR) profile, 0);
    uehL2mSamplingUpdate(profile, statusSucc);     
    
  }
  return retVal;
}  

PUBLIC S16 uehRlcL2MeasCfm( Pst *pst, KwL2MeasCfmEvt  *measCfmEvt)
{
  S16                     retVal = TRUE;
  UehCellCb              *cellCb = uehGetCellCb();
  UehL2MeasCtx           *uehL2MeasCtx = &cellCb->uehL2MeasCtx;    
  UehAvgNumActvUeQci     *pAvgNumActvUeQci = NULL;
  UehPktUuLossRateDlQci  *pPktUuLossRateDlQci = NULL;
  UehL2MeasProfileCfg    *profile = NULL;
  U32                     identity;
  U32                     transId = measCfmEvt->transId;
  U32                     idx = 0; 
  U32                     idy = 0; 
  TBOOL                   statusSucc = TRUE;
  S16                     profileTmrId;
     
    logInfo("uehRlcL2MeasCfm measType[%d]", measCfmEvt->measType);

    switch (measCfmEvt->measType)
    {
      case LKW_L2MEAS_ACT_UE:
           pAvgNumActvUeQci = &(uehL2MeasCtx->avgNumActvUeQci);
           identity = UEH_L2M_MASK_AVG_NUM_ACTV_UE_QCI;

           profile = pAvgNumActvUeQci->profile;

           if (TRUE == uehVerifyL2mCfm(profile, identity, transId))
           {
             if(measCfmEvt->status.status == LCM_PRIM_NOK)
             {
               logError("l2m: LKW_L2MEAS_ACT_UE Fail, cause[%d]", measCfmEvt->status.reason);
               statusSucc = FALSE;
               break;
             }
             if(measCfmEvt->numCfm != pAvgNumActvUeQci->numQci)
             {
               logError("l2m: LKW_L2MEAS_ACT_UE Fail, numCfm[%d] != numQci[%d]",
                                     measCfmEvt->numCfm, pAvgNumActvUeQci->numQci);
               statusSucc = FALSE;
               break;
             }
             for (idx = 0; idx < measCfmEvt->numCfm; idx++)
             {
               for (idy =0; idy < pAvgNumActvUeQci->numQci; idy++)
               {
                 if(measCfmEvt->measCfm[idx].qci == pAvgNumActvUeQci->qciInfo[idy].qciId)
                 {
                   pAvgNumActvUeQci->qciInfo[idy].numActUe = 
                                measCfmEvt->measCfm[idx].val.nonIpThrput.numActUe;
                   break;
                 }
               }
             }
             profile->mask &= UEH_L2M_INV_AVG_NUM_ACTV_UE_QCI;
           }
           else
           {
             logError("%s","l2m: uehRlcL2MeasCfm, spurious AVG_NUM_ACTV_UE_QCI");
           }
           break;

        
    case LKW_L2MEAS_UU_LOSS:
         pPktUuLossRateDlQci = &(uehL2MeasCtx->pktUuLossRateDlQci);
         identity            = UEH_L2M_MASK_PKT_LOSS_RATE_DL_QCI;
         profile             = pPktUuLossRateDlQci->profile;

         if (TRUE == uehVerifyL2mCfm(pPktUuLossRateDlQci->profile, identity, transId))
         {
           if(measCfmEvt->status.status == LCM_PRIM_NOK)
           {
             logError("l2m: LKW_L2MEAS_UU_LOSS Fail, cause[%d]", measCfmEvt->status.reason);
             statusSucc = FALSE;
             break;
           }
           if(measCfmEvt->numCfm != pPktUuLossRateDlQci->numQci)
           {
             logError("l2m: LKW_L2MEAS_UU_LOSS Fail, numCfm[%d] != numQci[%d]",
                                     measCfmEvt->numCfm, pPktUuLossRateDlQci->numQci);
             statusSucc = FALSE;
             break;
           }
           for (idx = 0; idx < measCfmEvt->numCfm; idx++)
           {
             for (idy =0; idy < pPktUuLossRateDlQci->numQci; idy++)
             {
               if(measCfmEvt->measCfm[idx].qci == pPktUuLossRateDlQci->qciInfo[idy].qciId)
               {
                 pPktUuLossRateDlQci->qciInfo[idy].uuLossDl = 
                               measCfmEvt->measCfm[idx].val.nonIpThrput.uuLoss;
                 break;
               }
             }
           }
           profile->mask &= UEH_L2M_INV_PKT_LOSS_RATE_DL_QCI;
         } 
         else
         {
           logError("%s","l2m: uehRlcL2MeasCfm, spurious PKT_LOSS_RATE_DL_QCI");
         }
         break;

    default:
       logError("l2m: uehRlcL2MeasCfm, spurious measType[%d]", measCfmEvt->measType);
    break;
  }
  if ((profile != NULL &&
      profile->mask == UEH_L2M_MASK_CLEAR) ||
      statusSucc == FALSE)
  {
     if (profile->profileId == PROFILEID_PROFILE1)
     {
       profileTmrId=UEH_L2M_SAMPLE_PROFILE1;
     }
     else
     {
       profileTmrId=UEH_L2M_SAMPLE_PROFILE2;
     }
    uehStopTmr (profileTmrId,&(((UehL2MeasProfileCfg *)profile)->samplingIntervalTmr),(PTR) profile, 0);
    uehL2mSamplingUpdate(profile, statusSucc);     
    
  }
  return retVal;
} 

PUBLIC S16 uehPdcpL2MeasCfm( Pst *pst, PjL2MeasCfmEvt  *measCfmEvt)
{
  S16                     retVal = TRUE;
  UehCellCb              *cellCb = uehGetCellCb();
  UehL2MeasCtx           *uehL2MeasCtx = &cellCb->uehL2MeasCtx;    
  UehL2MeasProfileCfg    *profile = NULL;
  UehPktDelayDlQci       *pPktDelayDlQci;
  UehPktDiscRateDlQci    *pPktDiscRateDlQci;
  UehPktLossUlQci        *pPktLossUlQci;
  U32                     identity;
  U32                     transId = measCfmEvt->transId;
  U32                     idx = 0; 
  U32                     idy = 0; 
  TBOOL                   statusSucc = TRUE;
  S16                     profileTmrId;

  logInfo("uehPdcpL2MeasCfm measType[%d]", measCfmEvt->measType);

  switch (measCfmEvt->measType)
  {
    case LPJ_L2MEAS_DL_DELAY:
      pPktDelayDlQci = &(uehL2MeasCtx->pktDelayDlQci);
      identity = UEH_L2M_MASK_PKT_DELAY_DL_QCI;

      profile = pPktDelayDlQci->profile;

      if (TRUE == uehVerifyL2mCfm(profile, identity, transId))
      {
         if(measCfmEvt->status.status == LCM_PRIM_NOK)
         {
            logError("l2m: LPJ_L2MEAS_DL_DELAY Fail, cause[%d]", measCfmEvt->status.reason);
            statusSucc = FALSE;
            break;
         }
         if(measCfmEvt->numCfm != pPktDelayDlQci->numQci)
         {
            logError("l2m: LPJ_L2MEAS_DL_DELAY Fail, numCfm[%d] != numQci[%d]",
                                     measCfmEvt->numCfm, pPktDelayDlQci->numQci);
            statusSucc = FALSE;
            break;
         }
        for (idx = 0; idx < measCfmEvt->numCfm; idx++)
        {
          for (idy =0; idy < pPktDelayDlQci->numQci; idy++)
          {
            if(measCfmEvt->measCfm[idx].qci == pPktDelayDlQci->qciInfo[idy].qciId)
            {
              pPktDelayDlQci->qciInfo[idy].dlDelay = measCfmEvt->measCfm[idx].dlDelay;
              break;
            }
          }
        }
        profile->mask &= UEH_L2M_INV_PKT_DELAY_DL_QCI;
      }
      else
      {
        logError("%s","l2m: uehPdcpL2MeasCfm, spurious PKT_DELAY_DL_QCI");
      }
      break;

      
    case LPJ_L2MEAS_DL_DISC:
      pPktDiscRateDlQci = &(uehL2MeasCtx->pktDiscRateDlQci);
      identity = UEH_L2M_MASK_PKT_DISC_RATE_DL_QCI;

      profile = pPktDiscRateDlQci->profile;

      if (TRUE == uehVerifyL2mCfm(profile, identity, transId))
      {
         if(measCfmEvt->status.status == LCM_PRIM_NOK)
         {
           logError("l2m: LPJ_L2MEAS_DL_DISC Fail, cause[%d]", measCfmEvt->status.reason);
           statusSucc = FALSE;
           break;
         }
         if(measCfmEvt->numCfm != pPktDiscRateDlQci->numQci)
         {
           logError("l2m: LPJ_L2MEAS_DL_DISC Fail, numCfm[%d] != numQci[%d]",
                                     measCfmEvt->numCfm, pPktDiscRateDlQci->numQci);
           statusSucc = FALSE;
           break;
         }
        for (idx = 0; idx < measCfmEvt->numCfm; idx++)
        {
          for (idy =0; idy < pPktDiscRateDlQci->numQci; idy++)
          {
            if(measCfmEvt->measCfm[idx].qci == pPktDiscRateDlQci->qciInfo[idy].qciId)
            {
              pPktDiscRateDlQci->qciInfo[idy].dlDisc = measCfmEvt->measCfm[idx].dlDisc;
              break;
            }
          }
        }
        profile->mask &= UEH_L2M_INV_PKT_DISC_RATE_DL_QCI;
      }
      else
      {
        logError("%s","l2m: uehPdcpL2MeasCfm, spurious PKT_DISC_RATE_DL_QCI");
      }
      break;


    case LPJ_L2MEAS_UL_LOSS:
      pPktLossUlQci = &(uehL2MeasCtx->pktLossUlQci);
      identity = UEH_L2M_MASK_PKT_LOSS_UL_QCI;

      profile = pPktLossUlQci->profile;

      if (TRUE == uehVerifyL2mCfm(profile, identity, transId))
      {
        if(measCfmEvt->status.status == LCM_PRIM_NOK)
        {
           logError("l2m: LPJ_L2MEAS_UL_LOSS Fail, cause[%d]", measCfmEvt->status.reason);
           statusSucc = FALSE;
           break;
        }
        if(measCfmEvt->numCfm != pPktLossUlQci->numQci)
        {
           logError("l2m: LPJ_L2MEAS_UL_LOSS Fail, numCfm[%d] != numQci[%d]",
                                     measCfmEvt->numCfm, pPktLossUlQci->numQci);
           statusSucc = FALSE;
           break;
        }
        for (idx = 0; idx < measCfmEvt->numCfm; idx++)
        {
          for (idy =0; idy < pPktLossUlQci->numQci; idy++)
          {
            if(measCfmEvt->measCfm[idx].qci == pPktLossUlQci->qciInfo[idy].qciId)
            {
              pPktLossUlQci->qciInfo[idy].ulLoss = measCfmEvt->measCfm[idx].ulLoss;
              break;
            }
          }
        }
        profile->mask &= UEH_L2M_INV_PKT_LOSS_UL_QCI;
      }
      else
      {
        logError("%s","l2m: uehPdcpL2MeasCfm, spurious PKT_LOSS_UL_QCI");
      }
      break;

    default:
       logError("l2m: uehPdcpL2MeasCfm, spurious measType[%d]", measCfmEvt->measType);
    break;
  }
  if ((profile != NULL &&
      profile->mask == UEH_L2M_MASK_CLEAR) ||
      statusSucc == FALSE)
  {
    if (profile->profileId == PROFILEID_PROFILE1)
    {
       profileTmrId = UEH_L2M_SAMPLE_PROFILE1;
    }
    else
    {
      profileTmrId = UEH_L2M_SAMPLE_PROFILE2;
    }
    uehStopTmr (profileTmrId,&(((UehL2MeasProfileCfg *)profile)->samplingIntervalTmr),(PTR) profile, 0);

    uehL2mSamplingUpdate(profile, statusSucc);     
  }
  return retVal;
}  
    
void uehL2mSamplingUpdate(UehL2MeasProfileCfg  *profile, TBOOL statusSucc)
{
  TBOOL    clearProfile = FALSE;
  uehBuildAndSendCounterReport(profile);
  if (statusSucc == TRUE)
  {
    if ((profile->numSamples == 0) ||
        (profile->sampleSeqNo < profile->numSamples ))
    {
      profile->sampleSeqNo++;
      uehStartProfile(profile);
    }
    else
    {
      profile->status = PROFILE_STOPPED;
      clearProfile = TRUE;
    }
  }
  else
  {
    uehSendCounterStopInd(profile, FAILURE_FROM_L2);
    clearProfile = TRUE;
  }
  if (clearProfile == TRUE)
  {
     uehClearL2mProfile(profile);
  }
}

void uehSendCounterStopInd(UehL2MeasProfileCfg *profile, MeasurementStopCause cause)
{
  MsgUehPmhCounterStopInd counterStopInd;

  clearBytes(&counterStopInd, sizeof(MsgUehPmhCounterStopInd));

  counterStopInd.profileId            = profile->profileId;
  counterStopInd.measurementStopCause = cause;
  
  sendUehMsgToPmh(EVT_UEH_PMH_COUNTER_STOP_IND, 
                  sizeof(MsgUehPmhCounterStopInd), 
                  (UINT8*)&counterStopInd);

}

void uehBuildAndSendCounterReport(UehL2MeasProfileCfg *profile)
{
  U32                       idx = 0;
  U32                       idy = 0;
  U32                       numCounters = 0;
  UehAvgNumActvUeQci       *pAvgNumActvUeQci = NULL;
  UehPktUuLossRateDlQci    *pPktUuLossRateDlQci = NULL;   
  UehPktDelayDlQci         *pPktDelayDlQci = NULL;
  UehPktDiscRateDlQci      *pPktDiscRateDlQci = NULL;
  UehPktLossUlQci          *pPktLossUlQci = NULL;
  UehTotalPrbUsage         *pTotalPrbUsageDl = NULL;  
  UehTotalPrbUsage         *pTotalPrbUsageUl = NULL;
  UehPrbUsageQci           *pPrbUsageQciDl = NULL;
  UehPrbUsageQci           *pPrbUsageQciUl = NULL;
  UehRecvRaPreambles       *pRecvRaPreambles = NULL;   
  UehNumActvUesQci         *pNumActvUesQciDl = NULL;
  UehNumActvUesQci         *pNumActvUesQciUl = NULL;

  MsgUehPmhCounterReportInd counterReportInd;    

  clearBytes(&counterReportInd, sizeof(MsgUehPmhCounterReportInd));
  counterReportInd.profileId        = profile->profileId;
  counterReportInd.sampleSeqNum     = profile->sampleSeqNo; 
  //counterReportInd.collectedat[0] = 0;
  
  getTimeStamp((char *)&counterReportInd.collectedAt[0]);
  
  for(idx = 0; idx < profile->numL2Counters; idx++)
  {
    switch (profile->counterLink[idx].identity)
    {
      case UEH_L2M_MASK_AVG_NUM_ACTV_UE_QCI:
       pAvgNumActvUeQci = (UehAvgNumActvUeQci *) profile->counterLink[idx].addr;
       for (idy = 0; idy < pAvgNumActvUeQci->numQci; idy++) 
       {
         counterReportInd.counterVal[numCounters].counterId = pAvgNumActvUeQci->qciInfo[idy].counterId; 
         counterReportInd.counterVal[numCounters].val = pAvgNumActvUeQci->qciInfo[idy].numActUe; 
         numCounters++;                                
       }
       break;

      case UEH_L2M_MASK_PKT_LOSS_RATE_DL_QCI:
       pPktUuLossRateDlQci = (UehPktUuLossRateDlQci *)profile->counterLink[idx].addr;
       for (idy = 0; idy < pPktUuLossRateDlQci->numQci; idy++) 
       {
         counterReportInd.counterVal[numCounters].counterId = pPktUuLossRateDlQci->qciInfo[idy].counterId; 
         counterReportInd.counterVal[numCounters].val = pPktUuLossRateDlQci->qciInfo[idy].uuLossDl; 
         numCounters++;                                
       }
       break;

      case UEH_L2M_MASK_PKT_DELAY_DL_QCI:
        pPktDelayDlQci = (UehPktDelayDlQci *)profile->counterLink[idx].addr;
        for (idy = 0; idy < pPktDelayDlQci->numQci; idy++) 
        {
          counterReportInd.counterVal[numCounters].counterId = pPktDelayDlQci->qciInfo[idy].counterId; 
          counterReportInd.counterVal[numCounters].val = pPktDelayDlQci->qciInfo[idy].dlDelay; 
          numCounters++;                                
        }
        break;

      case UEH_L2M_MASK_PKT_DISC_RATE_DL_QCI:
        pPktDiscRateDlQci = (UehPktDiscRateDlQci *) profile->counterLink[idx].addr;
        for (idy = 0; idy < pPktDiscRateDlQci->numQci; idy++) 
        {
          counterReportInd.counterVal[numCounters].counterId = pPktDiscRateDlQci->qciInfo[idy].counterId; 
          counterReportInd.counterVal[numCounters].val = pPktDiscRateDlQci->qciInfo[idy].dlDisc; 
          numCounters++;                                
        }
        break;

      case UEH_L2M_MASK_PKT_LOSS_UL_QCI:
        pPktLossUlQci = (UehPktLossUlQci *) profile->counterLink[idx].addr;
        for (idy = 0; idy < pPktLossUlQci->numQci; idy++) 
        {
          counterReportInd.counterVal[numCounters].counterId = pPktLossUlQci->qciInfo[idy].counterId; 
          counterReportInd.counterVal[numCounters].val = pPktLossUlQci->qciInfo[idy].ulLoss; 
          numCounters++;                                
        }
        break;

      case UEH_L2M_MASK_TOTAL_PRB_USAGE_DL: 
        pTotalPrbUsageDl = (UehTotalPrbUsage *) profile->counterLink[idx].addr;
        counterReportInd.counterVal[numCounters].counterId = pTotalPrbUsageDl->counterId; 
        counterReportInd.counterVal[numCounters].val = pTotalPrbUsageDl->prbPerc; 
        numCounters++;                                
        break;

      case UEH_L2M_MASK_TOTAL_PRB_USAGE_UL:
        pTotalPrbUsageUl = (UehTotalPrbUsage *) profile->counterLink[idx].addr;
        counterReportInd.counterVal[numCounters].counterId = pTotalPrbUsageUl->counterId; 
        counterReportInd.counterVal[numCounters].val = pTotalPrbUsageUl->prbPerc; 
        numCounters++;                                
        break;

      case UEH_L2M_MASK_PRB_USAGE_DL_QCI:
        pPrbUsageQciDl = (UehPrbUsageQci *) profile->counterLink[idx].addr;
        for (idy = 0; idy < pPrbUsageQciDl->numQci; idy++) 
        {
          counterReportInd.counterVal[numCounters].counterId = pPrbUsageQciDl->qciInfo[idy].counterId; 
          counterReportInd.counterVal[numCounters].val = pPrbUsageQciDl->qciInfo[idy].prbPerc; 
          numCounters++;                                
        }
        break;

      case UEH_L2M_MASK_PRB_USAGE_UL_QCI:
        pPrbUsageQciUl = (UehPrbUsageQci *) profile->counterLink[idx].addr;
        for (idy = 0; idy < pPrbUsageQciUl->numQci; idy++) 
        {
          counterReportInd.counterVal[numCounters].counterId = pPrbUsageQciUl->qciInfo[idy].counterId; 
          counterReportInd.counterVal[numCounters].val = pPrbUsageQciUl->qciInfo[idy].prbPerc; 
          numCounters++;                                
        }
        break;

      case UEH_L2M_MASK_RECV_PREAMBLES:
        pRecvRaPreambles = (UehRecvRaPreambles *) profile->counterLink[idx].addr;
        if(pRecvRaPreambles->dedPreCounterId != 0)
        {
          counterReportInd.counterVal[numCounters].counterId = pRecvRaPreambles->dedPreCounterId;
          counterReportInd.counterVal[numCounters].val = pRecvRaPreambles->dedPreambles; 
          numCounters++;
        }
        if(pRecvRaPreambles->randLowCounterId != 0)
        {
          counterReportInd.counterVal[numCounters].counterId = pRecvRaPreambles->randLowCounterId;
          counterReportInd.counterVal[numCounters].val = pRecvRaPreambles->randSelPreLowRange; 
          numCounters++;
        }
        if(pRecvRaPreambles->randHighCounterId != 0)
        {
          counterReportInd.counterVal[numCounters].counterId = pRecvRaPreambles->randHighCounterId;
          counterReportInd.counterVal[numCounters].val = pRecvRaPreambles->randSelPreHighRange; 
          numCounters++;
        }
        break;

      case UEH_L2M_MASK_NUM_ACTV_UE_QCI_DL:
        pNumActvUesQciDl = (UehNumActvUesQci *) profile->counterLink[idx].addr;
        for (idy = 0; idy < pNumActvUesQciDl->numQci; idy++) 
        {
          counterReportInd.counterVal[numCounters].counterId = pNumActvUesQciDl->qciInfo[idy].counterId; 
          counterReportInd.counterVal[numCounters].val = pNumActvUesQciDl->qciInfo[idy].numActvUe; 
          numCounters++;                                
        }
        break;

      case UEH_L2M_MASK_NUM_ACTV_UE_QCI_UL:
        pNumActvUesQciUl = (UehNumActvUesQci *) profile->counterLink[idx].addr;
        for (idy = 0; idy < pNumActvUesQciUl->numQci; idy++) 
        {
          counterReportInd.counterVal[numCounters].counterId = pNumActvUesQciUl->qciInfo[idy].counterId; 
          counterReportInd.counterVal[numCounters].val = pNumActvUesQciUl->qciInfo[idy].numActvUe; 
          numCounters++;                                
        }
        break;

      default:
      break;
  
    }
  }
  counterReportInd.numOfCounters = numCounters;

  int retVal;
  retVal = sendUehMsgToPmh(EVT_UEH_PMH_COUNTER_REPORT_IND,
                  sizeof(MsgUehPmhCounterReportInd), 
                  (UINT8*)&counterReportInd);
//  logError("EVT_UEH_PMH_COUNTER_REPORT_IND seqNum=%d, profileId=%d, retVal=%d", counterReportInd.sampleSeqNum, counterReportInd.profileId, retVal);
}

void uehProcCounterStopReq(
    Buffer *mBuf
  )
{
  MsgLen                     cCnt;
  MsgPmhUehCounterStopReq    counterStopReq;
  MsgUehPmhCounterStopSucc   counterStopSucc;
  UehCellCb                  *cellCb = uehGetCellCb();
  UehL2MeasCtx               *uehL2MeasCtx = &cellCb->uehL2MeasCtx;    
  UehL2MeasProfileCfg        *profileCfg = NULL;
  S16                         profileTmrId;

  
  SCpyMsgFix( mBuf, 0, sizeof(MsgPmhUehCounterStopReq), (Data*)&counterStopReq , &cCnt );
  logInfo("l2m: uehProcCounterStopReq profile[%d]", counterStopReq.profileId);
  uehDumpProfile();    
  
  clearBytes(&counterStopSucc, sizeof(MsgUehPmhCounterStopSucc));
  counterStopSucc.transId = counterStopReq.transId;
  counterStopSucc.profileId = counterStopReq.profileId;
  sendUehMsgToPmh(EVT_UEH_PMH_COUNTER_STOP_SUCC, sizeof(MsgUehPmhCounterStopSucc), (UINT8*)&counterStopSucc);
  
  profileCfg = uehGetL2mProfile(counterStopReq.profileId, uehL2MeasCtx);
  if (profileCfg == NULL)
  {
    logError("%s","l2m: Profile context Not Found");
  }
  else
  {
     if (profileCfg->profileId == PROFILEID_PROFILE1)
     {
       profileTmrId=UEH_L2M_SAMPLE_PROFILE1;
     }
     else
     {
       profileTmrId=UEH_L2M_SAMPLE_PROFILE2;
     }
     uehStopTmr (profileTmrId,&(((UehL2MeasProfileCfg *)profile)->samplingIntervalTmr),(PTR) profile, 0);

     uehClearL2mProfile(profileCfg);
     uehDumpProfile();    
  }  
}

TBOOL uehVerifyL2mCfm( UehL2MeasProfileCfg *profile, U32 identity, U32 transId)
{
  TBOOL    retVal = FALSE;
  U32      mask   = 0;

  if (profile != NULL)
  {
    logInfo("l2m: uehVerifyL2mCfm profileStatus[%d] sampleSeqNo[%d] transId[%d] mask[%d] identity[%d]",
      profile->status, profile->sampleSeqNo, transId, 
      profile->mask, identity);

    if (profile->status == PROFILE_RUNNING &&
        profile->sampleSeqNo == transId)
    {
      mask = profile->mask & identity;
      if (mask == identity)
      {
        retVal = TRUE;
      }
    }
  }
  else
  {
    logError("%s","l2m: uehVerifyL2mCfm profile is NULL");
  }
  return retVal;
}

void uehSamplingTimerExpiry(UehL2MeasProfileCfg *profile)
{
    logError("l2m: Sampling Timer Expiry profile[%d]", profile->profileId);
    uehBuildAndSendCounterReport(profile);
    uehSendCounterStopInd(profile, SAMPLING_TIMER_EXPIRED);
    uehClearL2mProfile(profile);
}

void getTimeStamp(char *dataAndTimeStampField
  )
{
  struct timeval  tv;
  struct timezone tz;
  struct tm* tm;
  UINT32   milliSeconds = 0;

  char month[3], date[3], hour[3], min[3], sec[3], msec[4];
  clearBytes( &month, 3 );
  clearBytes( &date, 3 );
  clearBytes( &hour, 3 );
  clearBytes( &min, 3 );
  clearBytes( &sec, 3 );
  clearBytes( &msec, 4 );

  gettimeofday(&tv, &tz);
  tm = localtime(&tv.tv_sec);

  if ( (tm->tm_mon+1) < 10 )  snprintf( month, 3, "0%d", (tm->tm_mon+1) );
  else                        snprintf( month, 3, "%d",  (tm->tm_mon+1) );

  if ( tm->tm_mday < 10 )     snprintf( date, 3, "0%d", tm->tm_mday );
  else                        snprintf( date, 3, "%d",  tm->tm_mday );

  if ( tm->tm_hour < 10 )     snprintf( hour, 3, "0%d", tm->tm_hour );
  else                        snprintf( hour, 3, "%d",  tm->tm_hour );

  if ( tm->tm_min < 10 )      snprintf( min, 3, "0%d", tm->tm_min );
  else                        snprintf( min, 3, "%d",  tm->tm_min );

  if ( tm->tm_sec < 10 )      snprintf( sec, 3, "0%d", tm->tm_sec );
  else                        snprintf( sec, 3, "%d",  tm->tm_sec );

  /* Compute milliseconds from microseconds. */
  milliSeconds = tv.tv_usec / 1000;
  if ( milliSeconds < 10 )         snprintf( msec, 4, "00%d", milliSeconds );
  else if ( milliSeconds < 100 )   snprintf( msec, 4, "0%d",  milliSeconds );
  else                             snprintf( msec, 4, "%d",   milliSeconds );

  snprintf( dataAndTimeStampField, MAX_SIZE_L2COUNTERS_LASTCOLLECTEDAT+1 , "%d%s%s%s%s%s%s",
          (tm->tm_year+1900), month, date, hour, min, sec, msec );
}


PRIVATE S16  uehPkNmbActvUeQciReq
(
LrgNmbActvUeQCI   *nmbActvUeQci, 
Buffer        *mBuf
)
{
   U8      idx;

   for(idx = 0; idx < nmbActvUeQci->numQci; idx++)
   {
       CMCHKPK(SPkU8, nmbActvUeQci->qci[idx], mBuf);
   }
   CMCHKPK(SPkU8, nmbActvUeQci->numQci, mBuf);
   CMCHKPK(SPkU8, nmbActvUeQci->sampPrd, mBuf);
   RETVALUE(ROK);
}
PRIVATE S16  uehPkAvgPrbQciReq
(
LrgAvgPrbQCI  *avgPrbQciReq,
Buffer        *mBuf
)
{
   U8      idx;

   TRC3(cmPkAvgPrbQciReq)
   
   for(idx = 0; idx < avgPrbQciReq->numQci; idx++)
   {
       CMCHKPK(SPkU8, avgPrbQciReq->qci[idx], mBuf);
   }
   CMCHKPK(SPkU8, avgPrbQciReq->numQci, mBuf);
   RETVALUE(ROK);

}

PUBLIC S16 uehLrgSchL2MeasReq
(
LrgSchMeasReqInfo  *measInfo,
Buffer            *mBuf
)
{
   if(measInfo->measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL)
   {
      CMCHKPK(uehPkNmbActvUeQciReq, &measInfo->nmbActvUeQciDl, mBuf);
   }
   if(measInfo->measType & LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL)
   {
      CMCHKPK(uehPkNmbActvUeQciReq, &measInfo->nmbActvUeQciUl, mBuf);
   }
   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_PER_QCI_DL)
   {
      CMCHKPK(uehPkAvgPrbQciReq, &measInfo->avgPrbQciDl, mBuf);
   }
   if(measInfo->measType & LRG_L2MEAS_AVG_PRB_PER_QCI_UL)
   {
      CMCHKPK(uehPkAvgPrbQciReq, &measInfo->avgPrbQciUl, mBuf);
   }

   CMCHKPK(cmPkLteCellId, measInfo->cellId, mBuf);
   CMCHKPK(SPkU16, measInfo->timePrd, mBuf);
   CMCHKPK(SPkU8, measInfo->measType, mBuf);
   CMCHKPK(cmPkHeader, &measInfo->hdr, mBuf);
   return ROK;
}

S16 uehKwL2MeasReqInfo
(
KwL2MeasReqInfo *param,
Buffer *mBuf
)
{

   U8 idx;
   for(idx = 0; idx < param->val.nonIpThMeas.numQci; idx++)
   {
       CMCHKPK(SPkU8, param->val.nonIpThMeas.qci[idx], mBuf);
   }
   CMCHKPK(SPkU8, param->val.nonIpThMeas.numQci, mBuf);
   CMCHKPK(SPkU16, param->val.nonIpThMeas.numSamples, mBuf);
   CMCHKPK(SPkU8, param->measType, mBuf);
   return ROK;
}

S16 uehLkwL2MeasReq( KwL2MeasReqEvt *measReqEvt,Buffer *mBuf)
{
   uehKwL2MeasReqInfo(&measReqEvt->measReq, mBuf);
   SPkU16(measReqEvt->measPeriod, mBuf);  
   SPkU32(measReqEvt->transId, mBuf);
   return ROK;
}


PUBLIC S16 uehPjL2MeasReqInfo
(
PjL2MeasReqInfo *param,
Buffer *mBuf
)
{

   U16 i;

   for(i = 0; i < param->numQci; i++)
      CMCHKPK(SPkU8, param->qci[i], mBuf);
   CMCHKPK(SPkU8, param->numQci, mBuf);
   CMCHKPK(SPkU8, param->measType, mBuf);
   RETVALUE(ROK);
}

PUBLIC S16 uehLpjL2MeasReq
(
PjL2MeasReqEvt *measReqEvt,
Buffer *mBuf
)
{
   if (uehPjL2MeasReqInfo(&measReqEvt->measReq, mBuf) != ROK) {
      RETVALUE(RFAILED);
   }
   if (SPkU16(measReqEvt->measPeriod, mBuf) != ROK) {
      RETVALUE(RFAILED);
   }
   if (SPkU32(measReqEvt->transId, mBuf) != ROK) {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}
#endif /*LTE_L2_MEAS */
